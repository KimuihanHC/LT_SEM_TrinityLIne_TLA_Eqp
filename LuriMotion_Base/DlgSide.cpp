// DlgSide.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgSide.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "DlgSleepMode.h"
#include "DlgLotoMode.h"

LPCTSTR ImageNameSide[] = { L"실행",		L"stop",		L"자동모드",	L"수동모드",		L"작업",	L"---",		L"---",	L"로딩데이터" };

// CDlgSide 대화 상자

IMPLEMENT_DYNAMIC(CDlgSide, CDialogEx)
CDlgSide *CDlgSide::m_pInstance = NULL;
CDlgSide *CDlgSide::Get_Instance(CWnd *pParent)
{
	if (!m_pInstance) {
		m_pInstance = new CDlgSide(pParent);
		if (!m_pInstance->m_hWnd) {
			m_pInstance->Create(IDD_DLG_SIDE, pParent);
		}
	}
	return m_pInstance;
}

void CDlgSide::Delete_Instance()
{
	if (m_pInstance->m_hWnd) m_pInstance->DestroyWindow();
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}

CDlgSide::CDlgSide(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SIDE, pParent)
{
	m_bFlagSampleInput_Block_Foced = FALSE;
	m_bFlagSampleOutput_Block_Foced = FALSE;
	m_bStartFlag = FALSE;
	m_bStopFlag = FALSE;
	m_bInitFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;
	m_bStartFlagIsActive = FALSE;

	COMMON->SetSleepModeStep(SLEEP_OFF);
	COMMON->bUseSleepMode = TRUE;
	m_bPowerStatus = FALSE;
	m_nbuttonFlagIs = 0;
	SEQUENCE->m_nbuttonFlagIs = 0;


	m_bStartStatus = 100;
	m_bStartStatusBK = 2;
	m_bStopStatus= 100;
	m_bStopStatusBK = 2;
	m_nbuttonFlagIsStop = FALSE;
	m_nStopstatus = 0;
	m_nStopstatusBK = 100;
	SetWinThreadRun(NULL);
	SetThreadRun(FALSE);
	if (GetWinThreadRun())
	{
		SetThreadRun(FALSE);
		WaitForSingleObject(GetWinThreadRun()->m_hThread, 500);
	}

// 	if (IsThreadRunSide() == FALSE) // 바코드 할때 써야함 
// 		Begin_RunThread();
}

CDlgSide::~CDlgSide()
{

	KillTimer(TIMER_SWITCH_LAMPCHECK);
	KillTimer(TIMER_SWITCH_START);
	KillTimer(TIMER_STABLE);
	End_RunThread();

}

void CDlgSide::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_1, m_btnSideMenu[enMenu::Menu_Auto]);			// 시작버튼
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_2, m_btnSideMenu[enMenu::Menu_Manual]);		// 정지버튼
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_3, m_btnSideMenu[enMenu::Menu_SemiAuto]);		// 세미오토
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_4, m_btnSideMenu[enMenu::Menu_StartupCheck]);	// 시업점검
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_5, m_btnSideMenu[enMenu::Menu_Loto]);			// 로토모드
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_6, m_btnSideMenu[enMenu::Menu_DryRun]);		// 드라이런
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_7, m_btnSideMenu[enMenu::Menu_Null]);			// 임시
	DDX_Control(pDX, IDC_BTN_SIDE_COMMAND_8, m_btnSideMenu[enMenu::Menu_Init]);			// 장비초기화	
}

BEGIN_MESSAGE_MAP(CDlgSide, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_1, &CDlgSide::OnBnClickedBtnSideCommand1)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_2, &CDlgSide::OnBnClickedBtnSideCommand2)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_3, &CDlgSide::OnBnClickedBtnSideCommand3)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_4, &CDlgSide::OnBnClickedBtnSideCommand4)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_5, &CDlgSide::OnBnClickedBtnSideCommand5)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_6, &CDlgSide::OnBnClickedBtnSideCommand6)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_7, &CDlgSide::OnBnClickedBtnSideCommand7)
	ON_BN_CLICKED(IDC_BTN_SIDE_COMMAND_8, &CDlgSide::OnBnClickedBtnSideCommand8)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CDlgSide::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgSide::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.5]
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

// CDlgSide 메시지 처리기
int CDlgSide::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}
void CDlgSide::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		KillTimer(TIMER_SWITCH_LAMPCHECK);
		KillTimer(TIMER_SWITCH_START);
		KillTimer(TIMER_STABLE);
		Sleep(10);


		SetTimer(TIMER_SWITCH_LAMPCHECK, 200, NULL);
		SetTimer(TIMER_SWITCH_START, 300, NULL);
		SetTimer(TIMER_STABLE, 100, NULL);
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_BTN_SIDE_COMMAND_8)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_SIDE_COMMAND_8)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
// 		KillTimer(TIMER_SWITCH_LAMPCHECK);
// 		KillTimer(TIMER_SWITCH_START);
// 		KillTimer(TIMER_STABLE);
	}

}
BOOL CDlgSide::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_Side();
	SetSIdeBarStopButton_LampOn();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSide::InitComponent_Side()
{
	int nGap = 10;
	CRect rt;
	CRect rt_Move;

	m_btnSideMenu[0].GetWindowRect(rt);

	int nStart_X = 20;
	int nStart_Y = 10;

	int nHeight = rt.Height();

	for (int i = 0; i < Menu_Max; i++)
	{
		rt_Move.left = nStart_X;
		rt_Move.top = nStart_Y + ((rt.Height() * 2) * i) + (nGap * i);
		rt_Move.right = nStart_X + rt.Width()+27;
		rt_Move.bottom = rt_Move.top + (rt.Height() * 2);

		m_btnSideMenu[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	// [21.1018.10] Modified
	for (int i = 0; i < Menu_Max; i++)
	{
		m_btnSideMenu[i].EnableWindowsTheming(FALSE);
		m_btnSideMenu[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnSideMenu[i].m_bTransparent = false;
		m_btnSideMenu[i].SetTextColor(COLOR_BLACK);
		m_btnSideMenu[i].SetFaceColor(COLOR_SLATEGRAY1, true);

		if (stBasic.nLanguage == 0)
			m_btnSideMenu[i].SetWindowText(m_szSideMenuKor[i]);
		else
			m_btnSideMenu[i].SetWindowText(m_szSideMenuEng[i]);
	}

	// Load image
	CString	strExePath = L"";
	CString strImgPath = L"";
	BOOL b;

	strExePath.Format(_T("%s"), COMMON->GetProgramPath());

	for (int i = 0; i < Menu_Max; i++)
	{
		// Normal
		strImgPath.Format(_T("%s\\%s\\%s\\%s.bmp"), strExePath, _PROGRAM_IMAGE_FOLDER, _T("Normal"), ImageNameSide[i]);
		if (b = (COMMON->IsFileExist(strImgPath)))
			m_cBitmap[i][0].m_hObject = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		else
			m_cBitmap[i][0].m_hObject = (HBITMAP)NULL;

		// Disable
		strImgPath.Format(_T("%s\\%s\\%s\\%s.bmp"), strExePath, _PROGRAM_IMAGE_FOLDER, _T("Disable"), ImageNameSide[i]);
		if (b = (COMMON->IsFileExist(strImgPath)))
			m_cBitmap[i][2].m_hObject = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		else
			m_cBitmap[i][2].m_hObject = (HBITMAP)NULL;
	}

	// Set image
	for (int i = 0; i < Menu_Max; i++)
		m_btnSideMenu[i].SetImage((HBITMAP)m_cBitmap[i][0].m_hObject, FALSE, NULL, 0, (HBITMAP)m_cBitmap[i][2].m_hObject);
}
void CDlgSide::ChangeUI_Language()
{
	if ((SEQUENCE->GetRunMode() == (RUN_MODE)eModeRun || SEQUENCE->GetRunMode() == eRunning))
		SetMenuClickColor(Menu_Auto);
	else
		SetMenuClickColor(Menu_Manual);
	
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SetMenuClickColor(Menu_SemiAuto);

	if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
		SetMenuClickColor(Menu_StartupCheck);
}

BOOL CDlgSide::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(64, 128, 128));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

void CDlgSide::SetSIdeBarStartButton_LampOn()
{
	SetMenuClickColor(Menu_Auto);	
}

void CDlgSide::SetSIdeBarStopButton_LampOn()
{
	SetMenuClickColor(Menu_Manual);
}

void CDlgSide::SetSIdeBarSemiAutoButton_LampOn(int nOn)
{
	SetMenuClickColor(Menu_SemiAuto);
}

void CDlgSide::SetSIdeBarDryRunButton_LampOn()
{
	SetMenuClickColor(Menu_DryRun);
}

void CDlgSide::SetSIdeBarIntialButton_LampOn(int nOn)
{
	SetMenuClickColor(Menu_Init);
}

enMCErrCode CDlgSide::Check_Start()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	CString strMsg;
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	// 1. 통신 연결이 되었는가?
//	if (COMMON->GetDryRunModeFlag() == FALSE)
	if(COMMON->GetOperationMode() != OperationMode::DryRun)
	{
		if (m_UdpTester[0].m_bConnected == FALSE)
		{
			strMsg.Format(_T("Tester 0 is Not Connected.\n"));
			WRITE_MAIN_LOG(strMsg);
// 			if (stBasic.nLanguage == 0)
// 				COMMON->ShowMsgDlg_Domodal(_T("검사기 0번(Left)을 연결해주십시오.\n"), MODE_INFORMATION);
// 			else
// 				COMMON->ShowMsgDlg_Domodal(_T("Please connect Tester 0(Left)\n"), MODE_INFORMATION);
			return MCEC_Tester_1_NotConnect;
		}
// 		if (m_UdpTester[1].m_bConnected == FALSE)
// 		{
// 			strMsg.Format(_T("Tester 1 is Not Connected.\n"));
// 			WRITE_MAIN_LOG(strMsg);
// 			if (stBasic.nLanguage == 0)
// 				COMMON->ShowMsgDlg_Domodal(_T("검사기 1번(right)을 연결해주십시오.\n"), MODE_INFORMATION);
// 			else
// 				COMMON->ShowMsgDlg_Domodal(_T("Please connect Tester 1(right)\n"), MODE_INFORMATION);
// 			return MCEC_Tester_2_NotConnect;
// 		}
	}

	// 2. 모터가 초기화가 되어있는가?
	BOOL bTemp = FALSE;
		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{
			bTemp = AJIN_MO->IsOrigin(i);
			if (bTemp == FALSE)
			{
				strMsg.Format(_T("%d Motor is Not Initialize.\n"), i);
				WRITE_MAIN_LOG(strMsg);
// 				if (stBasic.nLanguage == 0)
// 					COMMON->ShowMsgDlg_Domodal(_T("모터를 초기화해주십시오.\n"), MODE_INFORMATION);
// 				else
// 					COMMON->ShowMsgDlg_Domodal(_T("Motor is not initialize\n"), MODE_INFORMATION);
				return MCEC_Motion_NotInitialize;
			}

		}

	int nLevel = COMMON->GetPermissionGrade();
	if (nLevel == LVL_ADMIN)
	{
	}
	else
	{
		//3. 모터는 정상인가?
		// 4. Door Lock 은 닫혀있는가?
		if (TRUE != COMMON->GetInterlock_DoorCheck())
		{
// 			if (stBasic.nLanguage == 0)
// 				COMMON->ShowMsgDlg_Domodal(_T("도어를 닫아주세요. Output Doorlock 신호를 켜주십시오\n"), MODE_INFORMATION);
// 			else
// 				COMMON->ShowMsgDlg_Domodal(_T("Please close the door and then turn off the output signal\n"), MODE_INFORMATION);
			return MCEC_Door_Open;
		}
	}
		


	return err;
}

//====================================================================================
// Method		: OnBnClickedBtnSideCommand1
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/20 - 20:11
// Desc.	   	: 오토 시작
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand1()
{

	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->m_nUICheck != 1)
	{

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("메인 화면에서 시작을 눌러 주세요.\n"), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Please Main View Check .\n"), MODE_INFORMATION);

		return;
	}

	// START BUTTON
	CString strTemp = _T("");

	auto err = MCEC_OK;
	CString strMsg;
	m_nbuttonFlagIs = 0;
	SEQUENCE->m_nbuttonFlagIs = 0;
#ifdef DevMode_Simulator
	err = enMCErrCode::MCEC_OK;
	if (err != MCEC_OK)
	{
		SHOW_INFORMATION(err);
	}
	else
	{
		SEQUENCE->Start_Run();

		SetOperationMode(OperationMode::Auto);
		EqpTypeShowOperation();

		m_SideStopFlag = FALSE;
		SetSIdeBarStartButton_LampOn();

		COMMON->SetMcStatus(eRunning);
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("START 버튼[UI] 누름."));
		else
			strTemp.Format(_T("START BUTTON[UI] PRESSED."));

		WRITE_MAIN_LOG(strTemp);
	}
#else
	//if (m_UdpTester[0].m_bAckFlag_Ready != TRUE || m_UdpTester[0].m_nTesterFlag_ReadyErrorCode != 0)

	stTeach.nBarcode = TRUE;
	if (stTeach.nBarcode == 0)
	{
// 		if (m_UdpTester[0].m_bAckFlag_Ready != TRUE)
// 		{
// 			strMsg.Format(_T("Tester Barcode Read failure. \n"));
// 			WRITE_MAIN_LOG(strMsg);
// 			// 		if (stEq.nLanguage == 0)
// 			// 			COMMON->ShowMsgDlg_Domodal(_T("바코드를 다시 읽어 주세요 , 바코드가 정상 시작을 못했습니다.\n"), MODE_INFORMATION);
// 			// 		else
// 			// 			COMMON->ShowMsgDlg_Domodal(_T("Please Barcode ReTray Check, Tester Barcode Read failure.\n"), MODE_INFORMATION);
// 
// 			err = MCEC_Tester_Ready;
// 			m_UdpTester[0].Reset_TesterFlag();
// 			SHOW_ERROR(err);
// 		}
// 		else
// 		{
			err = Check_Start();
			if (err != MCEC_OK)
			{
				SHOW_ERROR(err);
			}
			else
			{

				m_nbuttonFlagIsStop = FALSE;
				SEQUENCE->Start_Run();
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, TRUE);
				SetOperationMode(OperationMode::Auto);
				EqpTypeShowOperation();

				// 		COMMON->SetSemiAutoModeFlag(FALSE);
				// 		COMMON->SetStart_up_CheckMode(FALSE);

				m_SideStopFlag = FALSE;
				SetSIdeBarStartButton_LampOn();

				COMMON->SetMcStatus(eRunning);
				if (stEq.nLanguage == 0)
					strTemp.Format(_T("START 버튼[UI] 누름"));
				else
					strTemp.Format(_T("START BUTTON[UI] PRESSED"));

				WRITE_MAIN_LOG(strTemp);
				WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Main_Button_Click,Start,"));

				SEQUENCE->StrNG.Empty();
				SEQUENCE->nPASS_NG = 3;
			}
	//	}
	}
	else
	{
		if (m_UdpTester[0].m_bAckFlag_Ready != TRUE)
		{
			strMsg.Format(_T("Tester Barcode Read failure. \n"));
			WRITE_MAIN_LOG(strMsg);
			// 		if (stEq.nLanguage == 0)
			// 			COMMON->ShowMsgDlg_Domodal(_T("바코드를 다시 읽어 주세요 , 바코드가 정상 시작을 못했습니다.\n"), MODE_INFORMATION);
			// 		else
			// 			COMMON->ShowMsgDlg_Domodal(_T("Please Barcode ReTray Check, Tester Barcode Read failure.\n"), MODE_INFORMATION);

			err = MCEC_Tester_Ready;
			m_UdpTester[0].Reset_TesterFlag();
			SHOW_ERROR(err);
		}
		else
		{
			err = Check_Start();
			if (err != MCEC_OK)
			{
				SHOW_ERROR(err);
			}
			else
			{

				m_nbuttonFlagIsStop = FALSE;
				SEQUENCE->Start_Run();
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, TRUE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, TRUE);
				SetOperationMode(OperationMode::Auto);
				EqpTypeShowOperation();

				// 		COMMON->SetSemiAutoModeFlag(FALSE);
				// 		COMMON->SetStart_up_CheckMode(FALSE);

				m_SideStopFlag = FALSE;
				SetSIdeBarStartButton_LampOn();

				COMMON->SetMcStatus(eRunning);
				if (stEq.nLanguage == 0)
					strTemp.Format(_T("START 버튼[UI] 누름"));
				else
					strTemp.Format(_T("START BUTTON[UI] PRESSED"));

				WRITE_MAIN_LOG(strTemp);
				WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Main_Button_Click,Start,"));

				SEQUENCE->StrNG.Empty();
				SEQUENCE->nPASS_NG = 3;
			}
		}
	}

#endif
	
}
void CDlgSide::OnBnClickedBtnSideCommand2()
{


// 	UINT nRetCode = 0;
// 	if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
// 	{
// 	}
// 	if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
// 	{
// 	}
// 	SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
// 	SEQUENCE->nPASS_NG = 0;


	if (m_nbuttonFlagIs == 0)
	{
		m_nbuttonFlagIs = 1;
		SEQUENCE->m_nbuttonFlagIs = 1;
		if (SEQUENCE->PickMgrStep() != enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_IDEL &&
			SEQUENCE->PickMgrStep() != enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STOP)
		{
			//SEQUENCE->SetRunMode(eStop);
			SEQUENCE->PickMgrSetNextStep(SEQ_PICKERMGR_STEP_READY_START);
		}
		else
		{
			SEQUENCE->SetRunMode(eStop);
			while (1)
			{
				if (!SEQUENCE->IsThreadRun())
					break;

				COMMON->DoEvent(10);
			}

			SetOperationMode(OperationMode::Manual);
			SetMenuClickColor(Menu_Manual);
			EqpTypeShowOperation();

			SEQUENCE->Stop_Run();

			COMMON->SetMcStatus(eStop);

			CString strTemp = _T("");
			EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
			if (stEq.nLanguage == 0)
				strTemp.Format(_T("STOP 버튼[UI] 누름."));
			else
				strTemp.Format(_T("STOP BUTTON[UI] PRESSED."));
			COMMON->Write_Main_Log(strTemp);
			WRITE_SEMCO_LOG(_T("M,-,-,Stop,EVENT:Manual_Mode,Start,"));
		}
	}
	else
	{
		SEQUENCE->SetRunMode(eStop);
		while (1)
		{
			if (!SEQUENCE->IsThreadRun())
				break;

			COMMON->DoEvent(10);
		}

		SetOperationMode(OperationMode::Manual);
		SetMenuClickColor(Menu_Manual);
		EqpTypeShowOperation();

		SEQUENCE->Stop_Run();

		COMMON->SetMcStatus(eStop);

		CString strTemp = _T("");
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("STOP 버튼[UI] 누름."));
		else
			strTemp.Format(_T("STOP BUTTON[UI] PRESSED."));
		COMMON->Write_Main_Log(strTemp);
		WRITE_SEMCO_LOG(_T("M,-,-,Stop,EVENT:Manual_Mode,Start,"));

		UINT nRetCode = 0;
		if (m_UdpTester[0].m_bConnected)
		{
			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
			{
			}
// 			if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
// 			{
// 			}
		}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		if (AJIN_MO->m_bReadError[0] == FALSE &&
			AJIN_MO->m_bReadError[1] == FALSE &&
			AJIN_MO->m_bReadError[2] == FALSE &&
			AJIN_MO->m_bReadError[3] == FALSE &&
			AJIN_MO->m_bReadError[4] == FALSE)
#elif(SET_INSPECTOR == SYS_TLA_OQC)
		if (AJIN_MO->m_bReadError[0] == FALSE &&
			AJIN_MO->m_bReadError[1] == FALSE)
#endif
		{
			if (SEQUENCE->Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
			{
				SEQUENCE->MO_MoveVignetting_R_Type(R_Wait);
			}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
			if (SEQUENCE->Check_Pos_Screw_L_Type(Screw_Wait) == FALSE && SEQUENCE->Check_Pos_Screw_R_Type(Screw_Wait) == FALSE
				&& SEQUENCE->Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				SEQUENCE->MO_MoveWait_Type(X_Wait);
			}
			if (SEQUENCE->Check_Pos_Screw_L_Type(Screw_Wait) == FALSE || SEQUENCE->Check_Pos_Screw_R_Type(Screw_Wait) == FALSE)
			{
				SEQUENCE->MO_MoveScrew_Type(Screw_Wait);
			}
#endif
			if (SEQUENCE->Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				SEQUENCE->MO_MoveVignetting_X_Type(X_Wait);
			}
		}
		SEQUENCE->IO_O_Chuck_Bwd(FALSE);
		Sleep(500);
		SEQUENCE->IO_O_Socket_Unlock(FALSE);
		SEQUENCE->StrNG.Empty();
		SEQUENCE->nPASS_NG = 2;
	}
	SEQUENCE->StrNG.Empty();
	SEQUENCE->nPASS_NG = 2;


	m_nStopstatus = 0;
	m_nStopstatusBK = 100;
}

//====================================================================================
// Method		: OnBnClickedBtnSideCommand3
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/20 - 20:06
// Desc.	   	: 세미오토 모드 화면 전환
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand3()
{
	SetOperationMode(OperationMode::SemiAuto);
	SetMenuClickColor(enMenu::Menu_SemiAuto);
	EqpTypeShowOperation();
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Semi_Auto_Mode,Start,"));
}

//====================================================================================
// Method		: OnBnClickedBtnSideCommand4
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/20 - 20:06
// Desc.	   	: 시업점검 화면 전환
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand4()
{
	SetMenuClickColor(Menu_StartupCheck);
	SetOperationMode(OperationMode::StartUpCheck);
	EqpTypeShowOperation();
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Startup_Check_Mode,Start,"));
}

//====================================================================================
// Method		: OnBnClickedBtnSideCommand5
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/08 - 12:12
// Desc.	   	: loto mode
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand5()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsg_Thread(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsg_Thread(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}

	CString strTemp = _T("");
	// [21.1018.4] 인터락
	if (SEQUENCE->GetRunMode() != (RUN_MODE)eStop)
	{
		if (stBasic.nLanguage == 0)
			strTemp.Format(_T("Stop 상태가 아님. Loto Mode 사용할 수 없음."));
		else
			strTemp.Format(_T("Not in Stop state. Loto mode is not available.."));
		COMMON->Write_Main_Log(strTemp);

		CDlgMessageBox dlg;
		dlg.SetMsg(strTemp, MODE_INFORMATION);
		dlg.DoModal();
		return;
	}
	if (stBasic.nLanguage == 0)
		strTemp.Format(_T("LOTO MODE 버튼[UI] 누름."));
	else
		strTemp.Format(_T("LOTO MODE BUTTON[UI] PRESSED."));
	COMMON->Write_Main_Log(strTemp);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Loto_Mode_Mode,Start,"));

	SetOperationMode(OperationMode::Loto);
	
	CDlgLotoMode Dlg;
	if (IDOK == Dlg.DoModal())
	{

	}
}
//====================================================================================
// Method		: OnBnClickedBtnSideCommand6
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/08 - 12:13
// Desc.	   	: dry run mode
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand6()
{	
//	SEQUENCE->nPASS_NG =0;
//	COMMON->ShowMsgDlg_Domodal(_T("PASS."), MODE_FAIL);
// 	Sleep(500);
// 	COMMON->Close_MsgBox();
//	return;

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, TRUE);

	// START BUTTON
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	m_nbuttonFlagIs = 0;
	SEQUENCE->m_nbuttonFlagIs = 0;
#ifdef DevMode_Simulator
	auto err = enMCErrCode::MCEC_OK;
	if (err != MCEC_OK)
	{
		SHOW_INFORMATION(err);
	}
	else
	{
		SetOperationMode(OperationMode::DryRun);

		SEQUENCE->Start_Run();

		m_SideStopFlag = FALSE;
		SetSIdeBarDryRunButton_LampOn();

		COMMON->SetMcStatus(eRunning);
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("DRY RUN 버튼[UI] 누름."));
		else
			strTemp.Format(_T("DRY RUN BUTTON[UI] PRESSED."));

		WRITE_MAIN_LOG(strTemp);
	}
#else
	auto err = Check_Start();
	if (err != MCEC_OK)
	{
		SHOW_ERROR(err);
	}
	else
	{
		SetOperationMode(OperationMode::DryRun);
		m_nbuttonFlagIsStop = FALSE;
		SEQUENCE->Start_Run();

		m_SideStopFlag = FALSE;
		SetSIdeBarDryRunButton_LampOn();

		COMMON->SetMcStatus(eRunning);
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("DRY RUN 버튼[UI] 누름."));
		else
			strTemp.Format(_T("DRY RUN BUTTON[UI] PRESSED."));

		WRITE_MAIN_LOG(strTemp);
		WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Dry_Run_Mode,Start,"));
	}

	
#endif
}
void CDlgSide::OnBnClickedBtnSideCommand7()
{	
// 	bool bOutputState = COMMON->GetOutPutModeFlag();
// 	COMMON->SetOutPutModeFlag(!bOutputState);
// 	SetSideBarOutputBtn_LampOn(!bOutputState);
// 
// 	COMMON->bUseSleepMode = COMMON->bUseSleepMode == TRUE ? FALSE : TRUE;
}
//====================================================================================
// Method		: OnBnClickedBtnSideCommand8
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/08 - 12:13
// Desc.	   	: initialize
//=====================================================================================
void CDlgSide::OnBnClickedBtnSideCommand8()
{
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	SEQUENCE->nhomeprogress = 0;
	pParrentDlg->m_pDlgMain->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTitle->EnableWindow(FALSE);
	pParrentDlg->m_pDlgTeach->EnableWindow(FALSE);
	pParrentDlg->m_pDlgIO->EnableWindow(FALSE);
	SetMenuClickColor(Menu_Init);

	// 초기화
	CDlgAxisHomeProgress Dlg;
	Dlg.DoModal();
	m_UdpTester[0].Reset_TesterFlag();
	//m_UdpTester[1].Reset_TesterFlag();

	SEQUENCE->Reset_SeqFlags();
	
	SetOperationMode(OperationMode::Manual);
	SetMenuClickColor(Menu_Manual);
	pParrentDlg->m_pDlgMain->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTitle->EnableWindow(TRUE);
	pParrentDlg->m_pDlgTeach->EnableWindow(TRUE);
	pParrentDlg->m_pDlgIO->EnableWindow(TRUE);
}

void CDlgSide::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_SWITCH_LAMPCHECK:
	{
		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
		
#ifndef DevMode_Simulator
		if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO) && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF))
		{
			m_bPowerStatus = 2;
			if (stBasic.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("EMO버튼이 눌렸습니다. \nEMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
			else
				COMMON->ShowMsgDlg_Domodal(_T("The EMO button has been pressed. \nPlease press the Reset button after releasing the EMO."), MODE_ERROR);
		}

		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO) && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF) && 
			COMMON->GetSleepModeStep() != SLEEP_ENABLE)
		{
			m_bPowerStatus = 1;
			if (stBasic.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("메인파워가 공급되지 않습니다. Reset 버튼을 눌러주십시오"), MODE_ERROR);
				else
					COMMON->ShowMsgDlg_Domodal(_T("Main Power is not supplied. Please press the Reset button."), MODE_ERROR);
		}
#endif


		// [21.1018.5] Loto Mode 시 Start, Init 금지
		if (1)
		{
			if (m_bPowerStatus == 0) // 분기확인
			{
				if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO) && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF))
					m_bPowerStatus = 1; // 온
				else
					m_bPowerStatus = 2; // 오프
			}
			else if (m_bPowerStatus == 1 && TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO) && TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF))
			{
				//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, FALSE);
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_05_RESET_SWITCH_LAMP, FALSE);
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, FALSE);
				if (COMMON->GetEMOAllInitFlag() == TRUE)
				{
				//	AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_05_RESET_SWITCH_LAMP, TRUE, 500, 500);
					AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, TRUE, 500, 500);
// 					FAS_MO->m_bConnected = FALSE;
// 					FAS_MO->m_bConnectedInitFlag = 0;
				}
				m_bPowerStatus = 2;
			}
			else if (m_bPowerStatus == 2 && FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF))
			{
				//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_04_RESET_SWITCH_LAMP, TRUE);
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, FALSE);
				AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_05_RESET_SWITCH_LAMP, TRUE, 500, 500);

				m_bPowerStatus = 3;
			}
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF) &&
				(AJIN_MO->IsOrigin(VIGNETTING_X) == TRUE ||
					AJIN_MO->IsOrigin(VIGNETTING_R) == TRUE ||
					AJIN_MO->IsOrigin(SCREW_L_X) == TRUE ||
					AJIN_MO->IsOrigin(SCREW_R_X) == TRUE ||
					AJIN_MO->IsOrigin(TILT_ADJUST_R) == TRUE))
			{
				AJIN_MO->ResetOrigin(VIGNETTING_X);
				AJIN_MO->ResetOrigin(VIGNETTING_R);
				AJIN_MO->ResetOrigin(SCREW_L_X);
				AJIN_MO->ResetOrigin(SCREW_R_X);
				AJIN_MO->ResetOrigin(TILT_ADJUST_R);
			}
		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF) &&
				(AJIN_MO->IsOrigin(VIGNETTING_X) == TRUE ||
					AJIN_MO->IsOrigin(VIGNETTING_R) == TRUE))
			{
				AJIN_MO->ResetOrigin(VIGNETTING_X);
				AJIN_MO->ResetOrigin(VIGNETTING_R);
			}
		}

		if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF) == TRUE)
		{
			AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_05_RESET_SWITCH_LAMP, FALSE);
		}


		m_bStartStatus = m_UdpTester[0].m_bAckFlag_Ready;
		if (m_bStartStatusBK != m_bStartStatus)
		{
			if (m_bStartStatus == TRUE)
			{
				AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_06_START_SWITCH_LAMP, TRUE, 500, 500);
			}
			else
			{
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_06_START_SWITCH_LAMP, FALSE);
			}
		}
		if (SEQUENCE->GetRunMode() == (RUN_MODE)eRunning)
		{
			 m_bStopStatus = TRUE;
		}
		else
		{
			 m_bStopStatus = FALSE;
		}
		if (m_bStopStatusBK != m_bStopStatus)
		{
			if (m_bStartStatus == TRUE)
			{
				//AJIN_IO->BlinkStart(DIO_OUTPUT_Y0_07_STOP_SWITCH_LAMP, TRUE, 500, 500);

				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_07_STOP_SWITCH_LAMP, FALSE);
			}
			else
			{
				AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_07_STOP_SWITCH_LAMP, FALSE);
			}
		}
		//if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop)
		//{
		//	if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1) == FALSE)
		//	{
		//		if (FALSE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO))
		//		{
		//			FAS_MO->Disconnect_EziServo();
		//			FAS_MO->m_bConnected = FALSE;
		//			FAS_MO->m_bConnectedInitFlag = 0;

		//			EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
		//			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_BEEP1,TRUE);
		//			if (stBasic.nLanguage == 0) //211029 -cws26번-1
		//				COMMON->ShowMsgDlg_Domodal(_T("EMO버튼이 눌렸습니다. EMO 해제후 Reset 버튼을 눌러주십시오."), MODE_ERROR);
		//			else
		//				COMMON->ShowMsgDlg_Domodal(_T("The EMO button has been pressed. Please press the Reset button after releasing the EMO."), MODE_ERROR);
		//		}
		//	}
		//}

		if (COMMON->GetLotoMode() == TRUE)
			break;

		if (Check_Switch_Init() == FALSE)
			break;

		if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop && m_SideStopFlag == FALSE)
		{
			SetSIdeBarStopButton_LampOn();
			m_SideStopFlag = TRUE;
		}
		//if (COMMON->bUseSleepMode == TRUE)
		{
			if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop)
			{
				int mode = COMMON->GetSleepModeStep();
				if (COMMON->GetSleepModeStep() == SLEEP_FINISH)
				{
					EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
					COMMON->SetSleepModeStep(SLEEP_SUCCESS);
					if(stBasic.nLanguage ==0)
						COMMON->ShowMsgDlg_Domodal(_T("Sleep모드가 해제되었습니다. Reset 버튼을 눌러주십시오."), MODE_INFORMATION);
					else
						COMMON->ShowMsgDlg_Domodal(_T("Sleep mode has been released. Please press the Reset button."), MODE_INFORMATION);

					COMMON->SetSleepModeStep(SLEEP_OFF);
				}

				if (COMMON->GetSleepModeStep() == SLEEP_OFF)
				{
					//COMMON->m_tSleepStart = GetTickCount64();
					COMMON->SetSleepStarted(GetTickCount64());
					COMMON->SetSleepModeStep(SLEEP_ON);
				}
				else
				{
					TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
					int nTime = stTester.nSleepModeTimer < 10 ? 10 : stTester.nSleepModeTimer;
					//time_t time = GetTickCount64() - COMMON->m_tSleepStart;
					time_t time = GetTickCount64() - COMMON->GetSleepStarted();
					COMMON->SetSleepElapsed(time);

					if (COMMON->GetSleepElapsed() > nTime * 1000 && COMMON->GetSleepModeStep() == SLEEP_ON)
					{
						COMMON->SetSleepModeStep(SLEEP_ENABLE);
						AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_SLEEP_MODE_ON, TRUE);
						CDlgSleepMode Dlg;
						if (IDOK == Dlg.DoModal())
						{
							AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_SLEEP_MODE_ON, FALSE);
							Sleep(300);
// 							FAS_MO->Disconnect_EziServo();
// 							FAS_MO->m_bConnectedInitFlag = 0;
							COMMON->SetSleepModeStep(SLEEP_FINISH);
						}
						m_bPowerStatus = 0;
						COMMON->SetEMOAllInitFlag(TRUE);
					}
				}
			}
			else if (SEQUENCE->GetRunMode() == (RUN_MODE)eRunning)
			{
				COMMON->SetSleepModeStep(SLEEP_OFF);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_SLEEP_MODE_ON, FALSE);
			}
		}
	}
	break;
	case TIMER_SWITCH_START:
	{

		if (COMMON->GetLotoMode() == TRUE)
			break;
		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_06_START)) // START 
		{
			if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop)
			{
				OnBnClickedBtnSideCommand1();
			}	
		}
		if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_07_STOP)) // START 
		{
			if (m_nStopstatus != m_nStopstatusBK) // STOP
			{
				m_nStopstatusBK = m_nStopstatus;
// 				if (SEQUENCE->GetRunMode() == (RUN_MODE)eRunning)
// 				{
					OnBnClickedBtnSideCommand2();
	//			}

			}
		}
		break;
	}
	break;
	case TIMER_STABLE:
	{
		if (SEQUENCE->GetRunMode() == (RUN_MODE)ePause)
		{
			SetOperationMode(OperationMode::Manual);
			SetMenuClickColor(Menu_Manual);
			EqpTypeShowOperation();
			SEQUENCE->Stop_Run();
			COMMON->SetMcStatus(eStop);
		}

		if (!SEQUENCE->IsThreadRun() && SEQUENCE->GetRunMode() == eStop && m_nbuttonFlagIsStop == FALSE)
		{
			m_nbuttonFlagIsStop = TRUE;
			m_nbuttonFlagIs = 1;
			SEQUENCE->SetRunMode(eStop);
			SetOperationMode(OperationMode::Manual);
			SetMenuClickColor(Menu_Manual);
			EqpTypeShowOperation();
			SEQUENCE->Stop_Run();
			COMMON->SetMcStatus(eStop);
		}
		break;
	}
	break;
	};
}
BOOL CDlgSide::Check_Switch_Start()
{
	//if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_START))
	//{
	//	if (m_bStartFlag == FALSE) m_tStartBtnPress = GetTickCount64();
	//	m_bStartFlag = TRUE;
	//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);

	//	if (GetTickCount64() - m_tStartBtnPress > 200 && m_bStartFlagIsActive == FALSE)
	//	{
	//		m_bStartFlagIsActive = TRUE;

	//		if (Check_Start() == FALSE)
	//		{
	//			if (COMMON->ShowMsgDlg_Domodal(_T("START에 실패하였습니다. 로그를 확인하세요."), MODE_INFORMATION))
	//			{
	//				// 시작만 안하면 되기 때문에 딱히 아무것도 넣지 않아도 될듯..?
	//			}
	//			m_bStartFlagIsActive = FALSE;
	//		}
	//		else
	//		{
	//			m_SideStopFlag = FALSE;
	//			SetSIdeBarStartButton_LampOn();
	//			SEQUENCE->Start_Run();
	//		}
	//	}
	//}
	//else if (m_bStartFlag == TRUE)
	//{
	//	m_bStartFlagIsActive = FALSE;
	//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
	//	m_bStartFlag = FALSE;
	//}

	return TRUE;
}

BOOL CDlgSide::Check_Switch_Stop()
{
	//if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_STOP))
	//{
	//	if (m_bStopFlag == FALSE) m_tStopBtnPress = GetTickCount64();
	//	m_bStopFlag = TRUE;
	//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);

	//	if (GetTickCount64() - m_tStopBtnPress > 200 && m_bStopFlagIsActive == FALSE)
	//	{
	//		m_bStopFlagIsActive = TRUE;
	//		SetSIdeBarStopButton_LampOn();
	//		SEQUENCE->Stop_Run();
	//	}
	//}
	//else if (m_bStopFlag == TRUE)
	//{
	//	m_bStopFlagIsActive = FALSE;
	//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
	//	m_bStopFlag = FALSE;
	//}

	return TRUE;
}

BOOL CDlgSide::Check_Switch_Init()
{
	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_INIT))
	{
		if (m_bInitFlag == FALSE) m_tInitBtnPress = GetTickCount64();

		m_bInitFlag = TRUE;
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, TRUE);

		if (GetTickCount64() - m_tInitBtnPress > 200 && m_bInitFlagIsActive == FALSE)
		{
			SetSIdeBarIntialButton_LampOn(TRUE);
			m_bInitFlagIsActive = TRUE;
			SEQUENCE->nhomeprogress = 0;
			CDlgAxisHomeProgress Dlg;
			if (IDCANCEL == Dlg.DoModal())
			{
				m_UdpTester[0].Reset_TesterFlag();
				//m_UdpTester[1].Reset_TesterFlag();
				SEQUENCE->Reset_SeqFlags();
			}
			SetSIdeBarIntialButton_LampOn(FALSE);
		}
	}
	else if (m_bInitFlag == TRUE)
	{
		m_bInitFlagIsActive = FALSE;
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, FALSE);
		m_bInitFlag = FALSE;
	}

	return TRUE;
}

void CDlgSide::SetOperationMode(__in OperationMode mode)
{
	COMMON->SetOperationMode(mode);

	auto pMain = CDlgMain::Get_Instance();

	switch (mode)
	{
	case OperationMode::Auto:
		m_btnSideMenu[Menu_SemiAuto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_StartupCheck].EnableWindow(FALSE);
		//m_btnSideMenu[Menu_LightMeasure].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Loto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Init].EnableWindow(FALSE);
		m_btnSideMenu[Menu_DryRun].EnableWindow(FALSE);
		WRITE_MAIN_LOG(_T("Operation : Auto Mode"));
		break;
	case OperationMode::Manual:
		m_btnSideMenu[Menu_Auto].EnableWindow(TRUE);
		m_btnSideMenu[Menu_Manual].EnableWindow(TRUE);
		m_btnSideMenu[Menu_SemiAuto].EnableWindow(TRUE);
		m_btnSideMenu[Menu_StartupCheck].EnableWindow(TRUE);
		//m_btnSideMenu[Menu_LightMeasure].EnableWindow(TRUE);
		m_btnSideMenu[Menu_Loto].EnableWindow(TRUE);
		m_btnSideMenu[Menu_Init].EnableWindow(TRUE);
		m_btnSideMenu[Menu_DryRun].EnableWindow(TRUE);
		WRITE_MAIN_LOG(_T("Operation : Manual Mode"));
		break;
	case OperationMode::SemiAuto:
		m_btnSideMenu[Menu_Auto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_SemiAuto].EnableWindow(TRUE);
		m_btnSideMenu[Menu_StartupCheck].EnableWindow(TRUE);
		//m_btnSideMenu[Menu_LightMeasure].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Loto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Init].EnableWindow(FALSE);
		m_btnSideMenu[Menu_DryRun].EnableWindow(FALSE);
		WRITE_MAIN_LOG(_T("Operation : Semi Auto Mode"));
		break;
	case OperationMode::StartUpCheck:
		m_btnSideMenu[Menu_Auto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_SemiAuto].EnableWindow(TRUE);
		m_btnSideMenu[Menu_StartupCheck].EnableWindow(TRUE);
		//m_btnSideMenu[Menu_LightMeasure].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Loto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Init].EnableWindow(FALSE);
		m_btnSideMenu[Menu_DryRun].EnableWindow(FALSE);
		WRITE_MAIN_LOG(_T("Operation : Startup Check Mode"));
		break;
	case OperationMode::DryRun:
		m_btnSideMenu[Menu_Auto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_SemiAuto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_StartupCheck].EnableWindow(FALSE);
		//m_btnSideMenu[Menu_LightMeasure].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Loto].EnableWindow(FALSE);
		m_btnSideMenu[Menu_Init].EnableWindow(FALSE);
		m_btnSideMenu[Menu_DryRun].EnableWindow(FALSE);
		WRITE_MAIN_LOG(_T("Operation : DryRun Mode"));
		break;
// 	case OperationMode::LightMeasMode:
// 		m_btnSideMenu[Menu_Auto].EnableWindow(FALSE);
// 		m_btnSideMenu[Menu_SemiAuto].EnableWindow(FALSE);
// 		m_btnSideMenu[Menu_StartupCheck].EnableWindow(FALSE);
// 		m_btnSideMenu[Menu_Loto].EnableWindow(FALSE);
// 		m_btnSideMenu[Menu_Init].EnableWindow(FALSE);
// 		WRITE_MAIN_LOG(_T("Operation : Light Measure Mode"));
// 		break;

	case OperationMode::Loto:
		WRITE_MAIN_LOG(_T("Operation : Loto Mode"));
		break;
	default:
		break;
	}

	// main dialog
	pMain->SetOperationMode(mode);

	// ICS 에 변경 상태 전송
	ICS_SendOperationMode(mode);
}


void CDlgSide::SetMenuClickColor(__in enMenu menu)
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (auto idx = 0; idx < Menu_Max; idx++)
	{
		m_btnSideMenu[idx].SetTextColor(COLOR_BLACK);
		m_btnSideMenu[idx].SetFaceColor(COLOR_SLATEGRAY1, true);

		if (stBasic.nLanguage == 0)
			m_btnSideMenu[idx].SetWindowText(m_szSideMenuKor[idx]);
		else
			m_btnSideMenu[idx].SetWindowText(m_szSideMenuEng[idx]);
	}

	switch (menu)
	{
	case CDlgSide::Menu_Auto:
		m_btnSideMenu[menu].SetFaceColor(COLOR_GREEN1, true);
		break;
	case CDlgSide::Menu_Manual:
		m_btnSideMenu[menu].SetTextColor(COLOR_BLACK);
		m_btnSideMenu[menu].SetFaceColor(RGB(255, 102, 0), true);
		break;
	case CDlgSide::Menu_SemiAuto:
		m_btnSideMenu[menu].SetFaceColor(COLOR_GREEN1, true);
		break;
	case CDlgSide::Menu_StartupCheck:
		m_btnSideMenu[menu].SetFaceColor(COLOR_GREEN1, true);
		break;
	case CDlgSide::Menu_Loto:
		break;
	case CDlgSide::Menu_DryRun:
		m_btnSideMenu[menu].SetFaceColor(COLOR_GREEN1, true);
		break;
	case CDlgSide::Menu_Null:
		break;
	case CDlgSide::Menu_Init:
		m_btnSideMenu[menu].SetTextColor(RGB(255, 237, 200));
		m_btnSideMenu[menu].SetFaceColor(RGB(201, 92, 25), true);
		break;
	case CDlgSide::Menu_Max:
		break;
	default:
		break;
	}
}

void CDlgSide::EqpTypeShowOperation()
{
	auto pMain = CDlgMain::Get_Instance();
	pMain->EqpTypeShowOperation();
}



bool CDlgSide::Begin_RunThread()// 양쪽 혼입방지 바코드 스켄용 쓰레드 
{
	if (GetWinThreadRun())
	{
		End_RunThread();
	}
	SetThreadRun(TRUE);
	SetWinThreadRun(AfxBeginThread(Thread_Run, this, THREAD_PRIORITY_NORMAL));

	return true;
}

void CDlgSide::End_RunThread()
{
	if (GetWinThreadRun())
	{
		SetThreadRun(FALSE);
		WaitForSingleObject(GetWinThreadRun()->m_hThread, 500);				//Log Message 때문에 무한대기에 빠진다
	}
}
UINT CDlgSide::Thread_Run(LPVOID lpVoid) // 양쪽 혼입방지 바코드 스켄용 쓰레드  // 스위치 무한대기 및 처리 예정 
{
//	((CDlgSide*)lpVoid)->IsThreadRunSide();
	while (((CDlgSide*)lpVoid)->IsThreadRunSide() == TRUE)
	{
		Sleep(50);
		//((CDlgSide*)lpVoid)->SetThreadRun(FALSE);
	}
	return 0;
}
void CDlgSide::SetThreadRun(BOOL val)
{
	m_bThreadRun = val;
}
BOOL CDlgSide::IsThreadRunSide() const
{
	return m_bThreadRun;
}
void CDlgSide::SetWinThreadRun(CWinThread * val)
{
	m_pThreadRun = val;
}
CWinThread* CDlgSide::GetWinThreadRun() const
{
	return m_pThreadRun;
}