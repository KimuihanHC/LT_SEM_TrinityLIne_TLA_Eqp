
#include "AjinBoard.h"
#include <stdio.h>

// CAjinMotion

IMPLEMENT_DYNAMIC(CAjinMotion, CWnd)
CAjinMotion::CAjinMotion()
{	
	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_bReadError[i]		= FALSE;
		m_bReadHome[i]		= FALSE;
		m_bReadLimitP[i]	= FALSE;
		m_bReadLimitM[i]	= FALSE;
		m_bReadInPos[i]		= FALSE;
	}

	m_strLastError			= _T("");
	m_dwLastError			= 0;

	m_bConnected			= FALSE;
}

CAjinMotion::~CAjinMotion()
{

}

BEGIN_MESSAGE_MAP(CAjinMotion, CWnd)
//	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAjinMotion ¸Þ½ÃÁö Ã³¸®±âÀÔ´Ï´Ù.

BOOL CAjinMotion::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // È­¸é ¿µ¿ª ¼³Á¤
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);	
}

void CAjinMotion::InitVariables()
{	

}

void CAjinMotion::InitRobotVariables()
{	

}

UINT CAjinMotion::Thread_MotionRead(LPVOID lpVoid)
{	
	while (AJIN_MO->m_bThreadRun)
	{
		if (AJIN_MO->m_dwLastError != AXT_RT_SUCCESS)
			continue;

		//if (AJIN_MO->IsInvaildAxis() == TRUE)
		{
			AJIN_MO->IsServoAlarmOn();
			AJIN_MO->IsServoCurPos();
			AJIN_MO->IsMotionInpos();
			AJIN_MO->IsLimitChk();
			AJIN_MO->isServoHome();
		}
		//else
		//{
		//	AJIN_MO->m_strLastError.Format(_T("%u"), AJIN_MO->m_dwLastError);
		//	WRITE_MAIN_LOG(AJIN_MO->m_strLastError);
		//}

		Sleep(200);
	}

	AJIN_MO->m_bThreadRun = FALSE;
	AJIN_MO->m_pMotionThread = NULL;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAjinMotion::Init_Ajin()
{
	DWORD dwResult;

	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	char szFilePath[] = "C:\\Exec_Trinity\\Mot\\1.mot";

	if(AxmMotLoadParaAll(szFilePath) != AXT_RT_SUCCESS)
	{
		return false;
	}
	
	m_bConnected = true;

	m_bThreadRun = TRUE;
	m_pMotionThread = AfxBeginThread(Thread_MotionRead, this);

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		AJIN_MO->ServoOn((long)i);
	}

	return true;
}

bool CAjinMotion::IsInit_Ajin()
{
	if( m_bConnected != true ) 
		return false;

	DWORD dwResult;

	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	if ( dwResult == AXT_RT_OPEN_ALREADY )
		return TRUE;

	return FALSE;
}

//========================================================================================
// Monitoring Func

bool CAjinMotion::IsInvaildAxis()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)2; i++)
	{
		dwResult = AxmInfoIsInvalidAxisNo((long)i);

		if (AXT_RT_SUCCESS != dwResult)
		{
			m_dwLastError = dwResult;
			return FALSE;
		}
			

		dwResult = 0;
	}

	return TRUE;
}
bool CAjinMotion::IsServoAlarmOn()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadServoAlarm( (long)i, &m_bReadError[i]);
	}

	return TRUE;
}


bool CAjinMotion::IsServoCurPos()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmStatusGetActPos((long)i, &m_dbPos[i]);
	}

	return TRUE;
}
bool CAjinMotion::IsMotionInpos()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadInpos((long)i, &m_bReadInPos[i]);
	}

	return TRUE;
}
bool CAjinMotion::IsLimitChk()
{
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmSignalReadLimit((long)i, &m_bReadLimitP[i], &m_bReadLimitM[i]);
	}

	return TRUE;
}
bool CAjinMotion::isServoHome()
{
	
	DWORD dwResult = 0;

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		dwResult = AxmHomeReadSignal((long)i, &m_bReadHome[i]);
	}
	

	return TRUE;
}

// End
//========================================================================================
bool CAjinMotion::LoadFile_Mot()
{
	CString	temp;
	CFileDialog dlg(TRUE, _T("Mot"), NULL, OFN_HIDEREADONLY, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{		
		temp				= dlg.GetPathName();
		char *strFilename	= (LPSTR)(LPCTSTR)temp.GetBuffer(0);

		//++ 함수 실행 성공시 지정한 Mot파일의 설정값으로 모션축 설정이 일괄 변경됩니다.
		if (AxmMotLoadParaAll(strFilename) != AXT_RT_SUCCESS)
			return false;
		else
			return true;
	}
	
	return false;
}
bool CAjinMotion::SaveFile_Mot()
{
	CString temp;
	CFileDialog dlg(FALSE, _T("mot"), NULL, OFN_HIDEREADONLY, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		temp = dlg.GetPathName();
		char *strFilename = (LPSTR)(LPCTSTR)temp.GetBuffer(0);
		//++ 함수 실행 성공시 각각의 모션축 설정값으로 지정한 Mot파일을 생성 및 저장 됩니다.
		if (AXT_RT_SUCCESS != AxmMotSaveParaAll(strFilename))
			return false;
		temp.ReleaseBuffer();	

		return true;
	}

	return false;
}

bool CAjinMotion::Move_Home(long lAxis)
{
	DWORD	dwRet;
	CString strResult = _T("");

// 	if (false == GetGantryModeEnable())
// 		return false;
	if (lAxis == (long)TILT_ADJUST_R) //현위치를 원점으로 셋팅한다 
	{
		AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
		SetOrigin((long)TILT_ADJUST_R);
		SetOriginFlag((long)TILT_ADJUST_R , FALSE);
	}
	else // 정상적으로 원점을 시작한다.
	{
		dwRet = AxmHomeSetStart(lAxis);
		if (dwRet != AXT_RT_SUCCESS)
		{
			CString strData;
			strResult.Format(_T("AxmHomeSetStart return error[Code:%04d]"), dwRet);
			return false;
		}
	}
	return true;
}
bool CAjinMotion::HomeResult(long lAxis)
{
	CString strResult;	
	DWORD	dwRet;

	AxmHomeGetResult(lAxis, &dwRet);

	if (dwRet == HOME_SUCCESS)
	{
		SetOrigin(lAxis);
		return TRUE;
	}

	return FALSE;
}

BOOL CAjinMotion::GetHomeRate(long lAxis)
{
	DWORD dwRet;
	m_dwMain = 0;
	m_dwSub = 0;

	dwRet = AxmHomeGetRate(lAxis, &m_dwMain, &m_dwSub);
	if(dwRet == 100)
	{
		return TRUE;
	}

	return FALSE;
}

int  CAjinMotion::GetHomeRate_Gage(long lAxis)
{
	DWORD dwRet;
	m_dwMain = 0;
	m_dwSub = 0;

	dwRet = AxmHomeGetRate(lAxis, &m_dwMain, &m_dwSub);

	return (int)dwRet;
}
DWORD CAjinMotion::Get_HomeRate_Sub()
{
	return m_dwSub;
}
void CAjinMotion::Set_HomeRate_Sub_Zero()
{
	m_dwSub = 0;
}
BOOL CAjinMotion::IsOrigin(int Motor)
{
	return m_Origin[Motor].bOrigin;
}

void CAjinMotion::SetOrigin(int Motor)
{
	m_Origin[Motor].bOrigin = MOTION_ORG;
}


BOOL CAjinMotion::GetOriginFlag(int Motor)
{
	return m_Origin[Motor].bOrgActFlag;
}

void CAjinMotion::SetOriginFlag(int Motor,BOOL bTrue)
{
	m_Origin[Motor].bOrgActFlag = bTrue;
}

void CAjinMotion::ResetOrigin(int Motor)
{
	m_Origin[Motor].bOrigin = MOTION_NOTORG;
}
bool CAjinMotion::MoveEStop(long lAxis)
{
	DWORD dwRet;

	dwRet = AxmMoveEStop(lAxis);

	if( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}

	return true;
}

bool CAjinMotion::MoveSStop(long lAxis)
{
	DWORD dwRet;

	dwRet = AxmMoveSStop(lAxis);

	if( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}

	return true;
}


void CAjinMotion::ServoOn(long lAxis)
{
	AxmSignalServoOn(lAxis, TRUE);
}
void CAjinMotion::ServoOff(long lAxis)
{
	AxmSignalServoOn(lAxis, FALSE);
}
bool CAjinMotion::IsServoOn(long lAxis)
{
	DWORD dwOnOff = 0;

	AxmSignalIsServoOn(lAxis, &dwOnOff);

	if( dwOnOff == 0 )
		return FALSE;
	
	return TRUE;
}

bool CAjinMotion::SetErrorReset(long lAxis)
{
	DWORD dwStatus = 0;

	// 서보 알람 상태를 읽고
	AxmSignalReadServoAlarm(lAxis, &dwStatus);
	if (FALSE == dwStatus)
		return TRUE;

	// 서보가 꺼져있으면 서보를 키자.
	AxmSignalIsServoOn(lAxis, &dwStatus);
	if( dwStatus == 0 )
		AxmSignalServoOn(lAxis, TRUE);

	//그후 에러처리
	dwStatus = AxmSignalServoAlarmReset(lAxis, ENABLE);

	Sleep(500);
	dwStatus = AxmSignalServoAlarmReset(lAxis, DISABLE);

	if (dwStatus != AXT_RT_SUCCESS)
		return FALSE;

	return TRUE;

}

void CAjinMotion::GetPulseAndUnit(long lAxis, double *dUnit, long *dPulse)
{
	AxmMotGetMoveUnitPerPulse(lAxis, dUnit, dPulse);
}

double CAjinMotion::GetActPos(long lAxis)
{
	double dPos = 0.0f;

	if (lAxis == 4)
	{
		AxmStatusGetCmdPos(lAxis, &dPos);
	}
	else
	{
		AxmStatusGetActPos(lAxis, &dPos);
	}
	

	return (dPos);
}
double CAjinMotion::GetCmdPos(long lAxis)
{
	double dPos = 0.0;
	AxmStatusGetCmdPos(lAxis, &dPos);

	return (dPos);
}


//////////////////////////////////////////////////////////////////////////
// Else
void CAjinMotion::DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void CAjinMotion::DoEvents(DWORD dwMiliSeconds)
{
	clock_t start_tm = clock();
	do
	{
		DoEvents();
		::Sleep(1);
	} while ((DWORD)(clock() - start_tm) < dwMiliSeconds);
}
bool CAjinMotion::IsMotion(long lAxis)
{
	DWORD	dwRet;

	AxmStatusReadInMotion(lAxis, &dwRet);

	return dwRet;
}

void CAjinMotion::PositionClear(long lAxis)
{
	AxmStatusSetPosMatch(lAxis, 0.0);
}

void CAjinMotion::SetMotionMode( long lAxis, int nMode )
{
	AxmMotSetAbsRelMode( lAxis, nMode );
}
bool CAjinMotion::MoveAxis_NonWait(long lAxis, double dPos, double dVel)
{
	DWORD	dwRetCode;
	CString strData;
	MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	double dPosn = 0.0;
	double dAcc = 0.0;
	double dDec = 0.0;
	dAcc = stSpeed.nMotorSpeed[(int)lAxis][eSpeedAcc];
	dDec = stSpeed.nMotorSpeed[(int)lAxis][eSpeedDec];
	dPosn = dPos * 1000;
	dwRetCode = AxmMoveStartPos(lAxis, dPosn, dVel, dVel , dVel );

	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);

		return FALSE;
	}
	return TRUE;
}

bool CAjinMotion::MoveAxis_Wait(long lAxis, double dPos, double dVel)
{
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	if ((long)TILT_ADJUST_R == lAxis)
	{
		AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
	}
#endif

	DWORD	dwRetCode;
	CString strData;
	MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	double dPosn = 0.0;
	double dAcc = 0.0;
	double dDec = 0.0;
	dAcc = stSpeed.nMotorSpeed[(int)lAxis][eSpeedAcc];
	dDec = stSpeed.nMotorSpeed[(int)lAxis][eSpeedDec];
	dPosn = dPos * 1000;
	dwRetCode = AxmMovePos(lAxis, dPosn, dVel, dAcc, dDec);

	if(dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);

		return FALSE;
	}
	else
	{
		Check_Move_End(lAxis);
		if (Check_Move_Pos(lAxis, dPos) == FALSE)
		{
			return FALSE;
		}
		else
		{

		}
	}
	return TRUE;
}
int CAjinMotion::MotionAxisDone(long lAxis)
{
	bool bRet = false;

	// 펄스 출력 중인지 확인하는 함수
	DWORD dwStatus = FALSE;

	AxmSignalReadInpos(lAxis, &dwStatus);


	//AxmStatusReadInMotion(lAxis, &dwStatus);
	if (dwStatus == TRUE) {
		bRet =  FALSE;
	}
	else {
		bRet = TRUE;
	}

	return bRet;
}
int CAjinMotion::Check_Move_Pos(long lAxis, double dPos) {
	m_csMotor[0].Lock();
	CString str;
	str.Empty();

	int iErrCode = 0;
	LONG nAxisID = 0;

// 	if (lAxis == AJIN_GANTRY_MASTER_Z)
// 	{
// 
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
	// 위치확인
	double dCurPos = 0;
	dCurPos = GetActPos(lAxis);
	dCurPos = dCurPos / 1000;
	if ((fabs(dCurPos - dPos) <= 0.1) == FALSE){
		m_csMotor[0].Unlock();
		return FALSE;
	}
	else
	{
		m_csMotor[0].Unlock();
		return TRUE;
	}
}
int CAjinMotion::Check_Move_End(long lAxis)
{
	clock_t startm = clock();
	bool bRet = false;
	clock_t startmbb = clock();
	do {
		DoEvents(10);

		if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO) == false) {
			MoveSStop(lAxis);
			break;
		}
		startmbb = (clock() - startm);
		if (startmbb > 100000) {
			MoveSStop(lAxis);
			break;
		}
		if (AJIN_MO->m_bReadError[lAxis])
		{
			MoveSStop(lAxis);
			break;
		}
	} while (!MotionAxisDone(lAxis));
	bRet = MotionAxisDone(lAxis);
	if (!bRet)
	{
		bRet = true;
	}
	Sleep(100);
	//	Delay(g_Data.m_Profile.paLens.nMoveStableTime) ;
	return bRet;
}

bool CAjinMotion::MoveAxis_Velocity(long lAxis, double dVel)
{
	CString strData;

	double dAcc = dVel;
	if (dVel < 0) dAcc = dVel * -1;


	double dDec = 0.0f;

	DWORD dwRetCode;

// 	if (lAxis == 0)
// 	{
// 		if (AJIN_MO->GetGantryModeEnable() == FALSE)
// 		{
// 			strData.Format(_T("Gantry Mode False"));
// 
// 			return FALSE;
// 		}
// 	}

	

	MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	dAcc = stSpeed.nMotorSpeed[(int)lAxis][eSpeedAcc];
	dDec = stSpeed.nMotorSpeed[(int)lAxis][eSpeedDec];




	dwRetCode = AxmMoveVel(lAxis, dVel, dAcc, dDec);

	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartPos Return Error[Code:%04d]"), dwRetCode);

		return FALSE;
	}

	return TRUE;
}
bool CAjinMotion::MotorMultiAxis_NonWait(long lAxisCnt, long *lpAxisNo, double *dpPos, double *dpVel)
{
	CString strData;

	/*
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double* dpAcc = new double[lAxisCnt];
	double* dpDec = new double[lAxisCnt];

	for( int i=0; i<(int)lAxisCnt; i++)
	{
		dpAcc[i] = (double)stMotion.nMotorSpeed[lpAxisNo[i]][eSpeedAcc];
		dpDec[i] = (double)stMotion.nMotorSpeed[lpAxisNo[i]][eSpeedDec];
	}

	dwRetCode = AxmMoveStartMultiPos(lAxisCnt, lpAxisNo, dpPos, dpVel, dpAcc, dpDec);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStartMultiPos Return Error[Code:%04d]"), dwRetCode);
		return FALSE;
	}
	*/
	return TRUE;
}
bool CAjinMotion::GantryEnable(long lAxisMaster, long lAxisSlave)
{
	DWORD dwRet;

	dwRet = AxmGantrySetEnable(lAxisMaster, lAxisSlave, 0, 0, 0);
	if(dwRet != AXT_RT_SUCCESS)
	{
		return false;
	}

	return TRUE;
}

bool CAjinMotion::IsGantryGet(long lAxisMaster)
{
	//dwRet = AxmGantryGetEnable(lAxisMaster, lAxisSlave, 0, 0, 0);
	Sleep(200);
	DWORD upSlHomeUse;
	double dpSlOffset;
	double dpSlORange;
	DWORD upGatryOn;

	AxmGantryGetEnable(lAxisMaster, &upSlHomeUse, &dpSlOffset, &dpSlORange, &upGatryOn); // °ÕÆ®¸® ¼ÂÆÃ »óÅÂ¸¦ È®ÀÎÇÑ´Ù. 
	if (upGatryOn != TRUE) // Á¤»óÀûÀÎ °ªÀÌ ¹ÝÈ¯ µÇ´Â °ÍÀ» È®ÀÎÈÄ Á¶°ÇÀ» º¯°æ ÇØ¾ßÇÔ 
	{
		return FALSE;
	}
	return TRUE;
}
bool CAjinMotion::GantryDisEnable(long lAxisMaster, long lAxisSlave)
{
	DWORD dwRet;

	dwRet = AxmGantrySetDisable(lAxisMaster, lAxisSlave);
	if(dwRet != AXT_RT_SUCCESS)
	{
		return false;
	}

	return TRUE;
}
bool CAjinMotion::GetGantryModeEnable()
{
	DWORD dwRet;

	AxmGantryGetEnable((long)0, NULL, NULL, NULL, &dwRet);
	if(dwRet != ENABLE)
	{
		return false;
	}

	return TRUE;
}
