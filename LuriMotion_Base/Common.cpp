#include <math.h>

#include "pch.h"
#include "Common.h"
#include "DlgMain.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCommon, CWnd)

CCommon::CCommon(void)
{
	m_bThreadShowMsg = FALSE;
	m_closeEvent = NULL;

	m_nMsg_NoCloseFlag = 0;

	m_nTestFlag = 0;

	m_strid = _T("OPER");
	m_nGrade = 0;

	m_bFlagSampleInput_Block_Foced = FALSE;
	m_bFlagSampleOutput_Block_Foced = FALSE;

	m_bRecipeChange = FALSE;
	for (int i = 0; i < 9; i++)
	{
		bConnected_LightController[i] = false;
	}
	m_bInterlockFlag_GrabBoardInit = FALSE;

	for( int i=0; i<5; i++)
	{
		m_bConnectTesterFlag[i] = FALSE;
	}
	// [21.1018.2]
	m_bLotoMode = FALSE;
	COMMON->m_nUICheck = 1;
}

CCommon::~CCommon(void)
{
}

BEGIN_MESSAGE_MAP(CCommon, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CCommon::ShowMsgDlg_Domodal(CString strInfo, int nMode)
{
 	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	strInfo += _T("\r\n");
	if (pParrentDlg->RunMessageDlg_Domodal(strInfo, nMode) == TRUE)
		return TRUE;

	return FALSE;
}

void CCommon::ShowMsg_Thread(CString strMsg, int nMode, bool bTesterAlarm)
{
	m_bThreadShowMsg = TRUE;
	m_ParamMsg.strMsg = strMsg;
	m_ParamMsg.nMode = nMode;
	m_ParamMsg.bTesterAlarm = bTesterAlarm;

	//m_ctrlOthers[0].Send_Stop(0);

	//if (AJIN_IO->GetOutputIoBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP) == FALSE)
	//		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP, TRUE);

	AfxBeginThread(Thread_ShowMsg, &m_ParamMsg, THREAD_PRIORITY_HIGHEST);
}
UINT CCommon::Thread_ShowMsg(LPVOID lpParam)
{
	THREAD_PARAM *pParam = (THREAD_PARAM *)lpParam;

	COMMON->Show_MsgBox(pParam->strMsg, pParam->nMode);
	COMMON->m_bThreadShowMsg = FALSE;

	return 0;
}

INT_PTR CCommon::Show_MsgBox(CString strMsg, int nMode)
{
	CDlgMessageBox_Instance *pUserMsgDlg = CDlgMessageBox_Instance::Get_Instance();
	
	int nSuppored = 0;
	LARGE_INTEGER llFreq;

	nSuppored = QueryPerformanceFrequency(&llFreq);
	QueryPerformanceCounter(&m_llErrorStartTime_DoModal);

	pUserMsgDlg->SetMsg(strMsg, nMode);
	
	switch (nMode)
	{
	case MODE_QUESTION:
		break;

	case MODE_ERROR:	//검사 시퀀스 중 Time Out Error 발생 시
		Close_MsgBox();

		m_nTestFlag = 1;
		break;

	case MODE_INFORMATION:	//Sequence 중 사용자에게 정보 표시를 위한 메세지일 경우에는  MSGDLG_CLOSE_TIME 만큰 표시 후 닫는다
		
		m_nTestFlag = 2;
		break;
	}

	
	m_closeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	pUserMsgDlg->ShowWindow(SW_SHOW);

	::WaitForSingleObject(m_closeEvent, INFINITE);
	CloseHandle(m_closeEvent);
	m_closeEvent = NULL;

	INT_PTR nReturn = 0;
	nReturn = pUserMsgDlg->m_nReturn;

	if (m_nMsgTimerOpenFlag == 1)
	{
		m_nMsgTimerOpenFlag = 0;
		KillTimer(100);
	}

	switch (nMode)
	{
	case MODE_QUESTION:
		break;

	case MODE_ERROR:				//검사 시퀀스 중 Time Out Error 발생 시
		break;

	case MODE_INFORMATION:
		break;

	}
	return TRUE;
}
void CCommon::Close_MsgBox(void)
{
	
	CDlgMessageBox_Instance *pUserMsgDlg = CDlgMessageBox_Instance::Get_Instance();
	
	//HWND hWnd = ::FindWindow(NULL, _T("User Message Box"));	
	//::SendMessage(hWnd, WM_CLOSE, NULL, NULL);

	pUserMsgDlg->ShowWindow(SW_HIDE);
	::SetEvent(m_closeEvent);

}

//====================================================================================
BOOL CCommon::Wait_Time(double msec, BOOL *pKill)
{
	LARGE_INTEGER start, end;
	double delay = 0;

	LARGE_INTEGER   freq;
	int bSuppored = QueryPerformanceFrequency(&freq);	//tick�� ����.

	QueryPerformanceCounter(&start);
	while (1)
	{
		QueryPerformanceCounter(&end);
		delay = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		if (delay > msec)
		{
			return  FALSE;
		}
		if (*pKill == TRUE)
		{
			return TRUE;
		}
		Sleep(1);
		DoEvents();
	}

	return FALSE;
}

void CCommon::DoEvent(int cnt)
{
	MSG msg;
	for (int i = 0; i < cnt; i++)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If it's a quit message, we're out of here.
		//	if (msg.message == WM_QUIT) 	return 1; 
			// Otherwise, dispatch the message.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} // End of PeekMessage while loop.
		Sleep(1);
	}
}

bool CCommon::DoEvents(void)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == 0)
		{
			return false;
		}
	}

	return true;
}

void CCommon::uSleep_NoReturn(double msec)
{
	LARGE_INTEGER start, end;
	double delay = 0;

	LARGE_INTEGER freq;
	int bSuppored = QueryPerformanceFrequency(&freq);	//tick�� ����.

	QueryPerformanceCounter(&start);

	while (1)
	{
		QueryPerformanceCounter(&end);
		delay = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		if (delay > msec)
		{
			return;
		}
		Sleep(1);
		DoEvents();
	}

	return;
}

CString CCommon::GetProgramPath()
{
	wchar_t szPath[4096];
	CString full_path;
	int full_path_len;
	int file_name_len;
	memset(szPath, 0x00, 4096);
	::GetModuleFileName(AfxGetInstanceHandle(), szPath, 4096);
	full_path.Format(_T("%s"), szPath);
	full_path_len = full_path.GetLength();
	file_name_len = full_path_len - full_path.ReverseFind(('\\'));
	full_path.Delete((full_path_len - file_name_len), file_name_len);
	//full_path += (_T('\\'));
	return full_path;
}

BOOL CCommon::IsFileExist(CString strFullPath)
{
	CFileStatus		fs;
	return CFile::GetStatus(strFullPath, fs);
}

//=====================================================================================================
// 
void CCommon::TESTRUN()
{
	
}

void CCommon::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case 1:
	{
	}
	break;

	case 100:
	{
		KillTimer(100);
		m_nMsgTimerOpenFlag = 0;
		Close_MsgBox();
	}
	break;

	}
}

void CCommon::Write_Main_Log(CString strLog)
{
	CDlgLog *pLog = CDlgLog::Get_Instance();
	pLog->Write_Main_Log(strLog);
}

void CCommon::Write_Err_Log(CString strLog)
{
	CDlgLog *pLog = CDlgLog::Get_Instance();
	pLog->Write_Err_Log(strLog);
}

void CCommon::Write_Seq_Log(CString strLog)
{
	CDlgLog *pLog = CDlgLog::Get_Instance();
	pLog->Write_Seq_Log(strLog);
}

void CCommon::Write_Comm_Log(CString strLog)
{
	CDlgLog *pLog = CDlgLog::Get_Instance();
	pLog->Write_Comm_Log(strLog);
}

void CCommon::Write_IcsComm_Log(CString strLog)
{
	CDlgLog *pLog = CDlgLog::Get_Instance();
	pLog->Write_IcsComm_Log(strLog);
}

//-------------------------------------------------
// Permission
void	CCommon::SetPermissionID(CString strID)
{
	m_strid = strID;
}
void CCommon::SetPermissionGrade(int nGrade)
{
	m_nGrade = nGrade;
}

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// bool CCommon::GetCtrlLightIsConnected(int nPos)
// {
// 	return bConnected_LightController[nPos];
// }
// 
// void CCommon::SetCtrlLightConnected(int nPos, bool nState)
// {
// 	bConnected_LightController[nPos] = nState;
// }
// 
// bool CCommon::SetCtrlLightConnectingAll()
// {	
// #ifdef	USE_LIGHT_TO_OUTPUT
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, TRUE);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, TRUE);
// #else
// 	m_ctrlLight_SOL_3_1.LIGHT_ON();
// 	m_ctrlLight_SOL_3_2.LIGHT_ON();
// 	m_ctrlLight_SOL_3_3.LIGHT_ON();
// 	m_ctrlLight_SOL_3_4.LIGHT_ON();
// 	m_ctrlLight_SOL_3_5.LIGHT_ON();
// 	m_ctrlLight_SOL_3_6.LIGHT_ON();
// 	m_ctrlLight_SOL_3_7.LIGHT_ON();
// 	m_ctrlLight_SOL_3_8.LIGHT_ON();
// 	m_ctrlLight_SOL_3_9.LIGHT_ON();
// #endif
// 	return true;
// }
//#endif

BOOL CCommon::GetEMOAllInitFlag()
{
	return m_bEMO_All_InitFlag;
}

void CCommon::SetEMOAllInitFlag(BOOL bState)
{
	m_bEMO_All_InitFlag = bState;
}

CString CCommon::GetPermissionId()
{
	return m_strid;
}
int	CCommon::GetPermissionGrade()
{
	return m_nGrade;
}

void	CCommon::SetSleepStarted(time_t time)
{
	m_tSleepStart = time;
}

time_t CCommon::GetSleepStarted()
{
	return m_tSleepStart;
}

void	CCommon::SetSleepElapsed(time_t time)
{
	m_tSleepElapsed = time;
}

time_t CCommon::GetSleepElapsed()
{
	return m_tSleepElapsed;
}

void	CCommon::SetSleepModeStep(int mode)
{
	m_SleepModeStep = mode;
}

int CCommon::GetSleepModeStep()
{
	return m_SleepModeStep;
}

//-------------------------------------------------
// Loto [21.1018.6] Add
void	CCommon::SetLotoMode(BOOL b)
{
	m_bLotoMode = b;
}

BOOL CCommon::GetLotoMode()
{
	return m_bLotoMode;
}

// #.#
BOOL CCommon::Check_EMO()
{
	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO))
	{	
		if (COMMON->GetSleepModeStep() == SLEEP_OFF)
		{
			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

// 			FAS_MO->Disconnect_EziServo();
// 			FAS_MO->m_bConnected = FALSE;
// 			FAS_MO->m_bConnectedInitFlag = 0;

			COMMON->SetMcStatus(eModeError);
			SEQUENCE->Stop_Run();
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, TRUE);

			if (stEq.nLanguage == 0) //211029  -cws26번-1
				COMMON->ShowMsgDlg_Domodal(_T("EMO버튼이 눌렸습니다. EMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("The EMO button has been pressed. Please press the Reset button after releasing the EMO."), MODE_ERROR);
			bEMOResetFlag = TRUE;
			COMMON->SetEMOAllInitFlag(TRUE);
			return FALSE;
		}
	}
	else if (TRUE == bEMOResetFlag)
	{
		SEQUENCE->Stop_Run();
		bEMOResetFlag = FALSE;
	}
	return TRUE;
}

BOOL CCommon::Check_AREA()
{
	if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_AREA))
	{
		if (COMMON->GetSleepModeStep() == SLEEP_OFF)
		{
			EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
			COMMON->SetMcStatus(eModeError);
			SEQUENCE->Stop_Run();
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, TRUE);

			if (stEq.nLanguage == 0) //211029  -cws26번-1
				COMMON->ShowMsgDlg_Domodal(_T("AREA 가 감지 되었습니다. AREA감지 제거 후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("AREA is detected. Please remove AREA and press the Reset button."), MODE_ERROR);
			COMMON->SetEMOAllInitFlag(TRUE);
			return FALSE;
		}
	}
	return TRUE;
}

//-------------------------------------------------
// Side Button Function
BOOL	CCommon::Check_Mch_Start()
{
	CString strMsg;
	// 1. 통신 연결이 되었는가?
//	if (COMMON->GetDryRunModeFlag() == FALSE)
	if(COMMON->GetOperationMode() != OperationMode::DryRun)
	{
		if (m_UdpTester[0].m_bConnected == FALSE)
		{
			strMsg.Format(_T("Tester 0 is Not Connected.\n"));
			WRITE_MAIN_LOG(strMsg);
			return FALSE;
		}
		if (m_UdpTester[1].m_bConnected == FALSE)
		{
			strMsg.Format(_T("Tester 1 is Not Connected.\n"));
			WRITE_MAIN_LOG(strMsg);
			return FALSE;
		}
	}

	// 2. 모터가 초기화가 되어있는가?
	BOOL bTemp = FALSE;
// 	if (FAS_MO->m_bConnected == TRUE)
// 	{
// 		for (int i = 3; i < (int)MOTOR_MAX_COUNT; i++)
// 		{
// 			bTemp = FAS_MO->IsHomeOK(i);
// 
// 			if (bTemp == FALSE)
// 			{
// 				strMsg.Format(_T("%d Motor is Not Initialize.\n"), i);
// 				WRITE_MAIN_LOG(strMsg);
// 				return FALSE;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		strMsg.Format(_T("Motor is Not Initialize.\n"));
// 		WRITE_MAIN_LOG(strMsg);
// 		return FALSE;
// 	}

	//3. 모터는 정상인가?
   // 4. Door Lock 은 닫혀있는가?
	if (TRUE != COMMON->GetInterlock_DoorCheck())
		return FALSE;

	return TRUE;
}
BOOL	CCommon::Mch_Pause()
{
	return TRUE;
}
BOOL	CCommon::Mch_Stop()
{
	return TRUE;
}

BOOL	CCommon::Input_Block()
{
	// 앞장비에서 자재 들어오는것 막기.
	// Output. Io.
	return TRUE;
}
BOOL	CCommon::Output_Block()
{
	// 자재가 뒷 장비로 넘어가는것 막기.
	return TRUE;
}
BOOL	CCommon::Check_Mch_DryRun()
{
	// 드라이런 모드 설정.
	return TRUE;
}

//-------------------------------------------------
// Conveyor ���� FUnction
// BOOL	CCommon::Conveyor1_Sensor_Select(int nPos)
// {
// 	if( nPos == 0)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 	}
// 	else if ( nPos == 1)
// 	{						  
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, TRUE);
// 	}
// 	return TRUE;
// }
// BOOL	CCommon::Conveyor2_Sensor_Select(int nPos)
// {
// 	if (nPos == _SLOW1_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 	}		
// 	else if (nPos == _STOP1_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 	}		
// 	else if (nPos == _SLOW2_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 	}		
// 	else if (nPos == _STOP2_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, TRUE);
// 	}
// 
// 	return TRUE;
// }
// BOOL	CCommon::Conveyor3_Sensor_Select(int nPos)
// {
// 	if (nPos == 0)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, FALSE);
// 	}
// 	else if (nPos == 1)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, TRUE);
// 	}
// 	return TRUE;
// }
// 
// BOOL	CCommon::Conveyor1_Sensor_Select_Clear()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 	
// 	return TRUE;
// }
// BOOL	CCommon::Conveyor2_Sensor_Select_Clear()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 	return TRUE;
// 
// }
// BOOL	CCommon::Conveyor3_Sensor_Select_Clear()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, FALSE);
// 
// 	return TRUE;
// }

void	CCommon::SetErrorLog_Write(CString strNumber)
{
	ERROR_LIST		stErr = *DATAMANAGER->GetCurrentErrorList();

	stErr.nErrorCount++;
	stErr.vtErrorNum.push_back(strNumber);

	DATAMANAGER->SetErrorListData(stErr);
}
// BOOL CCommon::GetDryRunModeFlag()
// {
// 	return m_bFlagDryRunMode;
// }
// void CCommon::SetDryRunModeFlag(BOOL bState)
// {
// 	m_bFlagDryRunMode = bState;
// }
// BOOL CCommon::GetSemiAutoModeFlag()
// {
// 	return m_bFlagSemiAutoMode;
// }
// void CCommon::SetSemiAutoModeFlag(BOOL bState)
// {
// 	m_bFlagSemiAutoMode = bState;
// }
// int CCommon::GetSemiAutoActionMode()
// {
// 	return m_nFlagSemiAutoActionMode;
// }
// void CCommon::SetSemiAutoActionMode(int nState)
// {
// 	m_nFlagSemiAutoActionMode = nState;
// }
// BOOL CCommon::GetStart_up_CheckMode()
// {
// 	return m_bFlagStart_up_CheckMode;
// }
// void CCommon::SetStart_up_CheckMode(BOOL bState)
// {
// 	m_bFlagStart_up_CheckMode = bState;
// }
// bool CCommon::GetOutPutModeFlag()
// {
// 	return m_bFlagOutputMode;
// }
// void CCommon::SetOutPutModeFlag(bool bState)
// {
// 	m_bFlagOutputMode = bState;
// }
CString CCommon::GetErrorName_EachLanguage(int nErrorNum)
{
	ERROR_LIST		stErr = *DATAMANAGER->GetCurrentErrorList();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strErrName = _T("");
	
	int nNumber = 0;
	int nPart = 0;
	int nSub = 0;

	nPart = nErrorNum / 1000;
	nSub = nErrorNum % 1000;

	if (nPart == 1)
		nNumber = nSub;
	else if (nPart == 2)
		nNumber = stErr.nErrorNameCount_1000 + nSub;
	else if (nPart == 3)
		nNumber = stErr.nErrorNameCount_1000 + stErr.nErrorNameCount_2000 + nSub;

	try
	{
		if (stEq.nLanguage == 0)
			strErrName = stErr.vtErr_Kor.at(nNumber);
		else if (stEq.nLanguage == 1)
			strErrName = stErr.vtErr_Eng.at(nNumber);
		else if (stEq.nLanguage == 2)
			strErrName = stErr.vtErr_Viet.at(nNumber);
	}
	catch (std::out_of_range &)
	{
		return CString();
	}

	return strErrName;
}


BOOL	CCommon::GetInterlock_DoorCheck()
{
	// 근접센서는 문이 닫혀있으면 FALSE
		// 도어락은 OUTPUT에서 On을 해야  TRUE가 뜬다.
	if (COMMON->GetPermissionGrade() != 0)
		return TRUE;

	EQ_BASIC_PARAM	stEq_Cws = *DATAMANAGER->GetCurrentEqBasicData();
	BOOL bRet_Front1_Lock = FALSE;
	BOOL bRet_Front1_Near = TRUE;

	BOOL bRet_Side_Left1_Lock = FALSE;
	BOOL bRet_Side_Left1_Near = TRUE;
	BOOL bRet_Side_Left2_Lock = FALSE;
	BOOL bRet_Side_Left2_Near = TRUE;
	BOOL bRet_Side_Left3_Lock = FALSE;
	BOOL bRet_Side_Left3_Near = TRUE;

	BOOL bRet_Side_Right1_Lock = FALSE;
	BOOL bRet_Side_Right1_Near = TRUE;
	BOOL bRet_Side_Right2_Lock = FALSE;
	BOOL bRet_Side_Right2_Near = TRUE;
	BOOL bRet_Side_Right3_Lock = FALSE;
	BOOL bRet_Side_Right3_Near = TRUE;

	BOOL bRet_Back1_Lock = FALSE;
	BOOL bRet_Back1_Near = TRUE;
	BOOL bRet_Back2_Lock = FALSE;
	BOOL bRet_Back2_Near = TRUE;
	
	
	bRet_Front1_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0E_FRONT_DOOR1_INTERLOCK_ON_OFF);
	bRet_Front1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0F_FRONT_DOOR1_INTERLOCK_OPEN_CLOSE);

	bRet_Side_Left1_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_SIDE_LEFT_DOOR1_INTERLOCK_ON_OFF);
	bRet_Side_Left1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_SIDE_LEFT_DOOR1_INTERLOCK_OPEN_CLOSE);
	bRet_Side_Left2_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_SIDE_LEFT_DOOR2_INTERLOCK_ON_OFF);
	bRet_Side_Left2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_SIDE_LEFT_DOOR2_INTERLOCK_OPEN_CLOSE);
	bRet_Side_Left3_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_SIDE_LEFT_DOOR3_INTERLOCK_ON_OFF);
	bRet_Side_Left3_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_SIDE_LEFT_DOOR3_INTERLOCK_OPEN_CLOSE);

	bRet_Side_Right1_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ON_OFF);
	bRet_Side_Right1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_SIDE_RIGHT_DOOR1_INTERLOCK_OPEN_CLOSE);
	bRet_Side_Right2_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ON_OFF);
	bRet_Side_Right2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_19_SIDE_RIGHT_DOOR2_INTERLOCK_OPEN_CLOSE);
	bRet_Side_Right3_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ON_OFF);
	bRet_Side_Right3_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1B_SIDE_RIGHT_DOOR3_INTERLOCK_OPEN_CLOSE);

	bRet_Back1_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1D_REAR_DOOR1_INTERLOCK_OPEN_CLOSE);
	bRet_Back1_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_REAR_DOOR1_INTERLOCK_ON_OFF);

	bRet_Back2_Near = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1F_REAR_DOOR2_INTERLOCK_OPEN_CLOSE);
	bRet_Back2_Lock = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1E_REAR_DOOR2_INTERLOCK_ON_OFF);

	if (stEq_Cws.nDeviceIoVersion == 1)
	{
		bRet_Front1_Lock = FALSE;
		bRet_Side_Left1_Lock = FALSE;
		bRet_Side_Left2_Lock = FALSE;
		bRet_Side_Right1_Lock = FALSE;
		bRet_Side_Right2_Lock = FALSE;
		bRet_Back1_Lock = FALSE;
		bRet_Back2_Lock = FALSE;
	}

	CString strErrName = _T("");

	if (bRet_Front1_Near == TRUE)
	{
		//도어(전면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1000);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(전면1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (front 1) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1000"));
	}
	if (bRet_Side_Left1_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1001);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽 1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 1) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1001"));
	}
	if (bRet_Side_Left2_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1002);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽 2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 2) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1002"));
	}
	if (bRet_Side_Left3_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1003);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽 3) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 3) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1003"));
	}
	if (bRet_Side_Right1_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1004);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right 1) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1004"));
	}
	if (bRet_Side_Right2_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1005);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right 2) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1005"));
	}
	if (bRet_Side_Right3_Near == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1006);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 3) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right 3) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1006"));
	}
	if (bRet_Back1_Near == TRUE)
	{
		//도어(후면1) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1007);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(후면1) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (back 1) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1007"));
	}
	if (bRet_Back2_Near == TRUE)
	{
		//도어(후면2) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1008);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(후면2) 센서가 감지되지 않습니다.문을 닫아주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (back 2) sensor is not detected.Please close the door.\n"));

		SetErrorLog_Write(_T("1008"));
	}

	// Lock
	if (bRet_Front1_Lock == FALSE)
	{
		//도어(전면) 잠금센서가 감지되지 않습니다. 문을 잠가주세요
		strErrName = GetErrorName_EachLanguage(1009);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(전면1) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (front 1) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1009"));
	}
	if (bRet_Side_Left1_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1010);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽1) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 1) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1010"));
	}
	if (bRet_Side_Left2_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1011);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽2) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 2) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1011"));
	}
	if (bRet_Side_Left3_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1012);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(왼쪽3) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Left 3) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1012"));
	}
	if (bRet_Side_Right1_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1013);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 1) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right 1) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1013"));
	}
	if (bRet_Side_Right2_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1014);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 2) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right2) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1014"));
	}
	if (bRet_Side_Right3_Lock == TRUE)
	{
		//도어(좌측면) 센서가 감지되지 않습니다. 문을 닫아주세요.
		strErrName = GetErrorName_EachLanguage(1015);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(오른쪽 3) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (Right3) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1015"));
	}
	if (bRet_Back1_Lock == FALSE)
	{
		//도어(후면1) 잠금센서가 감지되지 않습니다. 문을 잠가주세요
		strErrName = GetErrorName_EachLanguage(1016);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(후면1) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (back 1) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1016"));
	}
	if (bRet_Back2_Lock == FALSE)
	{
		//도어(후면2) 잠금센서가 감지되지 않습니다. 문을 잠가주세요
		strErrName = GetErrorName_EachLanguage(1017);
		if (stEq_Cws.nLanguage == 0)
			WRITE_MAIN_LOG(_T("도어(후면2) 잠금센서가 감지되지 않습니다. 문을 잠가주세요\n"));
		else
			WRITE_MAIN_LOG(_T("The door (back 2) lock sensor is not detected. Please lock the door.\n"));

		SetErrorLog_Write(_T("1017"));
	}
	if ( (bRet_Front1_Near == FALSE && bRet_Side_Left1_Near == FALSE && bRet_Side_Left2_Near == FALSE && bRet_Side_Left3_Near == FALSE && 
		bRet_Side_Right1_Near == FALSE && bRet_Side_Right2_Near == FALSE && bRet_Side_Right3_Near == FALSE && bRet_Back1_Near == FALSE && bRet_Back2_Near == FALSE)
	   && (bRet_Front1_Lock == TRUE && bRet_Side_Left1_Lock == TRUE && bRet_Side_Left2_Lock == TRUE && bRet_Side_Left3_Lock == TRUE && 
		   bRet_Side_Right1_Lock == TRUE && bRet_Side_Right2_Lock == TRUE && bRet_Side_Right3_Lock == TRUE && bRet_Back1_Lock == TRUE && bRet_Back2_Lock == TRUE))
		return TRUE;
	
	return FALSE;
}

BOOL	CCommon::GetInterlock_AreaSensor()
{
	return TRUE;
}
BOOL	CCommon::GetInterlock_SocketClose()
{
	return TRUE;
}

BOOL	CCommon::GetInterlock_GrabBoardInit()
{
	/*
	BOOL bRet = TRUE; 
	
	bRet = AJIN_IO->GetOutputIoBit(DIO_OUTPUT_Y0_05_CAM_BOARD_INIT_ONOFF);

	if (bRet == FALSE)
	{
		m_bInterlockFlag_GrabBoardInit = TRUE;
		return FALSE;
	}
	*/
	return TRUE;
}

BOOL CCommon::GetBlockUIFlag()
{
	return COMMON->m_bFlagInitingBlockUI;
}

void CCommon::SetBlockUIFlag(BOOL bState)
{
	COMMON->m_bFlagInitingBlockUI = bState;
}

void CCommon::SetOperationMode(__in OperationMode mode)
{
	m_OperationMode = mode;
}

OperationMode CCommon::GetOperationMode()
{
	return m_OperationMode;
}

// [21.1029.1] 명칭변경, 연결변경
void	CCommon::SetMcStatus(INT16 rm)
{
	SEQUENCE->SetRunMode(rm);
}

// #if(SET_INSPECTOR == SYS_DISTORTION) \
//  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
//  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// BOOL CCommon::SetLoading1VacuumOnOff(int nMode)
// {
// 	if (nMode == _VACUUM_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_LOADING1_VACCUM_ON, TRUE);
// 	}
// 	else
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_LOADING1_VACCUM_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, TRUE);
// 		Sleep(50);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, FALSE);
// 	}
// 	return TRUE;
// }
// 
// BOOL CCommon::SetLoading2VacuumOnOff(int nMode)
// {
// 	if (nMode == _VACUUM_ON)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_LOADING2_VACCUM_ON, TRUE);
// 	}
// 	else
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_LOADING2_VACCUM_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, TRUE);
// 		Sleep(50);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, FALSE);
// 	}
// 	return TRUE;
// }
// #endif
// void CCommon::Call_Main_Flow_UI(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->ChangeMainUIFlowStatus(nPart,nPos,bState);
// }

// void CCommon::UpdateMainUIConveyor1(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIConveyor1(nPart, nPos, bState);
// }
// 
// void CCommon::UpdateMainUIConveyor2(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIConveyor2(nPart, nPos, bState);
// }
// 
// void CCommon::UpdateMainUIConveyorReturn(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIConveyorReturn(nPart, nPos, bState);
// }
// 
// void CCommon::UpdateMainUIPicker(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIPicker(nPart, nPos, bState);
// }
// 
// void CCommon::UpdateMainUIInspection(int nPart, int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIInspection(nPart, nPos, bState);
// }
// 
// void CCommon::UpdateMainUIConv2Status(int nPos, BOOL bState)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// 	pMain->UpdateMainUIConveyor2Status(nPos, bState);
// }

CString CCommon::GetRfid_ReadData()
{
	m_strRFID_ReadData.Trim();
	return m_strRFID_ReadData;
}

BOOL CCommon::RFID_Connect(int nDevice, int ip0, int ip1, int ip2, int ip3, int port)
{
	short nRet;

	nRet = ::RFID_Connect(nDevice, ip0, ip1, ip2, ip3, port, 1000);
	if (nRet != RFID_OK)
		return FALSE;

	m_bRFID_Connected = TRUE;
	Write_Comm_Log(_T("[RFID] Coneect Success."));
	return TRUE;
}
BOOL CCommon::RFID_Disconnect(int nDevice)
{
	RFID_CloseDevice(nDevice);
	m_bRFID_Connected = FALSE;

	Write_Comm_Log(_T("[RFID] Disconnect Success."));
	return TRUE;
}
BOOL CCommon::RFID_GetRead(int nDevice)
{
	short nRet;
	char szTagID[20] = { 0, };

	nRet = RFID_EventRead(nDevice, 0, 1, szTagID, 20, 1000);

	if (nRet != RFID_OK)
		return FALSE;

	m_strRFID_ReadData = _T("");

	CString strTemp = _T("");

	for (int i = 0; i < 20; i++)
	{
		strTemp += szTagID[i];
	}

	m_strRFID_ReadData = strTemp;

//	Write_Comm_Log(strTemp);

	CDlgMain *pMain = CDlgMain::Get_Instance();
	//pMain->Change_MainUI_RFID();

	return TRUE;
}
BOOL CCommon::RFID_SetWrite(CString strData)
{
	short nRet;
	char szWrite[20] = { "", };

	// 초기화 한번해주고
	nRet = RFID_WriteData(0, 0, 1, szWrite, strlen(szWrite), 2000);
	if (nRet != RFID_OK)
		return FALSE;

	// 입력할 RFID 넣자.
	for (int i = 0; i < strData.GetLength(); i++)
	{
		szWrite[i] = strData.GetAt(i);
	}
	nRet = RFID_WriteData(0, 0, 1, szWrite, strlen(szWrite), 2000);
	if (nRet != RFID_OK)
		return FALSE;

	Write_Comm_Log(_T("[RFID] Write Success."));

	return TRUE;
}