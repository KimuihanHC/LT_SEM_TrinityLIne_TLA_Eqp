// DlgManualMove.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgManualMove.h"
#include "afxdialogex.h"
#include "DlgMotorTeach.h"

#include "Util/BasicTimer.h"

// CDlgManualMove 대화 상자

IMPLEMENT_DYNAMIC(CDlgManualMove, CDialogEx)

CDlgManualMove::CDlgManualMove(CWnd * pParent, CDlgMotorTeach * pMotorTeachDlg)
	: CDialogEx(IDD_DLG_MANUAL_MOVE, pParent)
{
	m_pMotorTeachDlg = pMotorTeachDlg;
}

CDlgManualMove::~CDlgManualMove()
{
	m_pMotorTeachDlg = nullptr;
}

BOOL CDlgManualMove::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_ManualMove();
	SelectSocket(InspectionPortType::Left);

	return TRUE;
}

void CDlgManualMove::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE_MANUAL_MOVE, m_stTitle[0]);
	DDX_Control(pDX, IDC_STC_TITLE_SELECT_SOCKET, m_stTitle[1]);

	DDX_Control(pDX, IDC_BTN_MANUAL_MOVE_BACK, m_btnPageMoveBack);
	DDX_Control(pDX, IDC_BTN_MANUAL_MOVE_FRONT, m_btnPageMoveFront);

	for (auto i = 0; i < PAGE_LIST_MAX; i++)
	{
		DDX_Control(pDX, IDC_STC_INDEX_1 + i * 4, m_stIndex[i]);
		DDX_Control(pDX, IDC_STC_MANUAL_MOVE_1 + i * 4, m_stName[i]);
		DDX_Control(pDX, IDC_BTN_MANUAL_MOVE_1 + i * 4, m_btnMove[i]);
		DDX_Control(pDX, IDC_STC_MOVE_RESULT_1 + i * 4, m_stResult[i]);
	}

	DDX_Control(pDX, IDC_BTN_SOCKET_LEFT, m_btnSocketLeft);
	DDX_Control(pDX, IDC_BTN_SOCKET_RIGHT, m_btnSocketRight);
}

BEGIN_MESSAGE_MAP(CDlgManualMove, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_SOCKET_LEFT, &CDlgManualMove::OnBnClickedBtnSocketLeft)
	ON_BN_CLICKED(IDC_BTN_SOCKET_RIGHT, &CDlgManualMove::OnBnClickedBtnSocketRight)
	ON_STN_CLICKED(IDC_BTN_MANUAL_MOVE_BACK, &CDlgManualMove::OnStnClickedStcManualMoveBack)
	ON_STN_CLICKED(IDC_BTN_MANUAL_MOVE_FRONT, &CDlgManualMove::OnStnClickedStcManualMoveFront)
	ON_BN_CLICKED(IDC_BTN_VIEW_MOTOR_TEACHING, &CDlgManualMove::OnBnClickedBtnViewMotorTeaching)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MANUAL_MOVE_1, IDC_BTN_MANUAL_MOVE_13, &CDlgManualMove::OnBnClickedBtnManualMove)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CDlgManualMove::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgManualMove::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));

	return TRUE;
}

void CDlgManualMove::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		CRect rect;
		m_pMotorTeachDlg->GetWindowRect(rect);
		MoveWindow(rect, FALSE);
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}

void CDlgManualMove::ChangeUI_Language()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	CString strName;

	if (stEq.nLanguage == 0)
	{
		strName.Format(_T("수동 이동 - %d"), m_nPage + 1);

		m_stTitle[0].SetText(strName);
		m_stTitle[1].SetText(_T("소켓 선택"));
	}
	else
	{
		strName.Format(_T("Manual Move - %d"), m_nPage + 1);

		m_stTitle[0].SetText(strName);
		m_stTitle[1].SetText(_T("Select Socket"));
	}

	m_btnPageMoveBack.SetWindowText(_T("BACK"));
	m_btnPageMoveFront.SetWindowText(_T("FRONT"));

	for (auto i = 0; i < PAGE_LIST_MAX; i++)
	{
		CString strTemp;
		auto currentIndex = m_nPage * PAGE_LIST_MAX + i;

		strTemp.Format(_T("%d"), currentIndex + 1);

		m_stIndex[i].SetText(strTemp);

		if (stEq.nLanguage == 0)
		{
			m_stName[i].SetText(m_szManualMoveList_Kor[currentIndex]);
			m_btnMove[i].SetWindowText(_T("이동"));
		}
		else
		{
			m_stName[i].SetText(m_szManualMoveList_Eng[currentIndex]);
			m_btnMove[i].SetWindowText(_T("MOVE"));
		}
	}

	m_btnSocketLeft.SetWindowText(_T("Left"));
	m_btnSocketRight.SetWindowText(_T("Right"));

	auto * pViewMotorTeachButton = GetDlgItem(IDC_BTN_VIEW_MOTOR_TEACHING);
	
	if (stEq.nLanguage == 0)
		pViewMotorTeachButton->SetWindowText(_T("모터 티칭 화면 보기"));
	else
		pViewMotorTeachButton->SetWindowText(_T("View Motor Teaching Menu"));
}

void CDlgManualMove::OnStnClickedStcManualMoveBack()
{
	ChangePage(m_nPage - 1);
}

void CDlgManualMove::OnStnClickedStcManualMoveFront()
{
	ChangePage(m_nPage + 1);
}

void CDlgManualMove::OnBnClickedBtnSocketLeft()
{
	SelectSocket(InspectionPortType::Left);
}

void CDlgManualMove::OnBnClickedBtnSocketRight()
{
	SelectSocket(InspectionPortType::Right);
}

void CDlgManualMove::OnBnClickedBtnViewMotorTeaching()
{
	m_pMotorTeachDlg->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);
}

void CDlgManualMove::OnBnClickedBtnManualMove(UINT nID)
{
	if ((nID - IDC_BTN_MANUAL_MOVE_1) % 4 != 0)
		return;

	auto index = (nID - IDC_BTN_MANUAL_MOVE_1) / 4 + m_nPage * PAGE_LIST_MAX + 1;
	auto idx = (index - 1) % PAGE_LIST_MAX;

	m_stResult[idx].SetText(_T(""));
	m_stResult[idx].SetColorStyle(CVGBase_Static::ColorStyle_Orange);

	lt::CBasicTimer timer;

	auto result = ManualMove(index);
	
	auto ElapTime = timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds);

	CString sz;
	sz.Format(_T("%.3f msec"), (double)(ElapTime / (double)1000));
	m_stResult[idx].SetText(sz);
	m_stResult[idx].SetColorStyle(
		result ? CVGBase_Static::ColorStyle_Green : CVGBase_Static::ColorStyle_Red);
}

void CDlgManualMove::InitComponent_ManualMove()
{
	CString strTemp;
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		m_stTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTitle[i].SetFont_Gdip(L"Arial", 9.0F);
	}

	m_btnPageMoveBack.EnableWindowsTheming(FALSE);
	m_btnPageMoveBack.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnPageMoveBack.m_bTransparent = false;
	m_btnPageMoveBack.SetTextColor(RGB(255, 237, 200));
	m_btnPageMoveBack.SetFaceColor(RGB(201, 92, 25), true);

	m_btnPageMoveFront.EnableWindowsTheming(FALSE);
	m_btnPageMoveFront.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnPageMoveFront.m_bTransparent = false;
	m_btnPageMoveFront.SetTextColor(RGB(255, 237, 200));
	m_btnPageMoveFront.SetFaceColor(RGB(201, 92, 25), true);

	for (int i = 0; i < PAGE_LIST_MAX; i++)
	{
		m_stIndex[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stIndex[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stIndex[i].SetFont_Gdip(L"Arial", 9.0F);

		m_stName[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stName[i].SetColorStyle(CVGStatic::ColorStyle_White);
		m_stName[i].SetFont_Gdip(L"Arial", 12.0F);

		m_btnMove[i].EnableWindowsTheming(FALSE);
		m_btnMove[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMove[i].m_bTransparent = false;
		m_btnMove[i].SetTextColor(COLOR_BLACK);
		m_btnMove[i].SetFaceColor(RGB(225, 225, 225), true);

		m_stResult[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stResult[i].SetColorStyle(CVGStatic::ColorStyle_White);
		m_stResult[i].SetFont_Gdip(L"Arial", 9.0F);
	}

	m_btnSocketLeft.EnableWindowsTheming(FALSE);
	m_btnSocketLeft.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnSocketLeft.m_bTransparent = false;

	m_btnSocketRight.EnableWindowsTheming(FALSE);
	m_btnSocketRight.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnSocketRight.m_bTransparent = false;

	ChangeUI_Language();
}

void CDlgManualMove::ChangePage(int nPage)
{
	static constexpr auto PAGE_COUNT = 2;
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	CString strTemp;

	if (nPage < 0 || nPage >= PAGE_COUNT)
		return;

	m_nPage = nPage;

	ChangeUI_Language();

	for (auto i = 0; i < PAGE_LIST_MAX; i++)
		m_stResult[i].SetColorStyle(CVGStatic::ColorStyle_White);
}

void CDlgManualMove::SelectSocket(InspectionPortType type)
{
	m_portType = type;

	auto * selectedSocket = type == InspectionPortType::Left ? &m_btnSocketLeft : &m_btnSocketRight;
	auto * otherSocket = selectedSocket == &m_btnSocketRight ? &m_btnSocketLeft : &m_btnSocketRight;

	otherSocket->SetTextColor(COLOR_BLACK);
	selectedSocket->SetFaceColor(COLOR_GREEN1, true);
	otherSocket->SetFaceColor(RGB(225, 225, 225), true);
}


#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
bool CDlgManualMove::ManualMove(int nIndex)
{
	bool bRet = true;

	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch (nIndex)
	{
	case 1:		// 초기화 (Z축을 모두 안전 위치로 올린다)
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			bRet = false;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{

			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Bwd() == false)
			{
				bRet = false;
				break;
			}
			if (IO_O_Socket_Unlock() == false)
			{
				bRet = false;
				break;
			}
		}
	}
	break;
	case 2:		// 광원 OFF
	{
		bRet = LIGHT_Off();
	}
	break;
	case 3:		// 광원 ON
	{
		bRet = LIGHT_On();
	}
	break;
	case 4:		//제품 고정
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveScrew_Type(Screw_Work);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Fwd() == false)
			{
				bRet = false;
				break;
			}
			Sleep(500);
			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);

		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 5:		//파워 서플라이 켜기
	{
		//bRet = POWERSUPPLY_VoltOn(InspectionPortType::Left);

		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
	}
	break;
	case 6:		//	SEMCO 프로그램 Ready
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Ready Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
	}
	break;
	case 7:	// SEMCO 프로그램 Preview Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 8:		// Screw 풀기
	{
		bRet = IO_O_Screw_Bolt_Unlock();
	}
	break;
	case 9:	// SEMCO 프로그램 Test TLA Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_TLA)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_TLA, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 10:	// SEMCO 프로그램 Test TLA 조정
	{
		TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
		MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
		double dSpeed = (double)stMotion.nMotorSpeed[0][2];
		double dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * stTeaching.nAdjust_Wait;
		double dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
		dTargetPos = dCurPos + dTargetPos;
		if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
		{
			err = MCEC_Motion_Adjust_MoveError;
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
		}
		break;
	}
	break;
	case 11:		// Screw 잠그기 1
	{
		bRet = IO_O_Screw_Bolt_Lock_1();
	}
	break;
	case 12:		// Screw 잠그기 2
	{
		bRet = IO_O_Screw_Bolt_Lock_2();
	}
	break;
	case 13:		// 조정 풀기
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		Sleep(500);
		if (IO_O_Chuck_Bwd() == false)
		{
			bRet = false;
			break;
		}
	}
	break;
	case 14:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveVignetting_X_Type(X_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 15:	// SEMCO 프로그램 Test Vigneting 46 Start
	{


		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting46)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting46, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting46 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 16:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveVignetting_X_Type(X_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 17:	// SEMCO 프로그램 Test Vigneting 82 Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting82)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting82 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 18:	// SEMCO 프로그램 Preview Stop
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStop(m_portType))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStopAck(m_portType, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Stop Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 19:	// 파워 서플라이 끄기
	{
		//bRet = POWERSUPPLY_VoltOff(InspectionPortType::Left);

		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	break;
	default:
		return false;
	}

	return bRet;
}
#elif(SET_INSPECTOR == SYS_TLA_OQC)
bool CDlgManualMove::ManualMove(int nIndex)
{
	bool bRet = true;

	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch (nIndex)
	{
	case 1:		// 초기화 (Z축을 모두 안전 위치로 올린다)
	{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}

// 			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
// 			Sleep(500);
// 			if (IO_O_Chuck_Bwd() == false)
// 			{
// 				bRet = false;
// 				break;
// 			}
			if (IO_O_Socket_Unlock() == false)
			{
				bRet = false;
				break;
			}
		
	}
	break;
	case 2:		// 광원 OFF
	{
		bRet = LIGHT_Off();
	}
	break;
	case 3:		// 광원 ON
	{
		bRet = LIGHT_On();
	}
	break;
	case 4:		//제품 고정
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 5:		//파워 서플라이 켜기
	{
		//bRet = POWERSUPPLY_VoltOn(InspectionPortType::Left);

		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
	}
	break;
	case 6:		//	SEMCO 프로그램 Ready
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Ready Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
	}
	break;
	case 7:	// SEMCO 프로그램 Preview Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 8:		// Screw 풀기
	{
		bRet = IO_O_Screw_Bolt_Unlock();
	}
	break;
	case 9:	// SEMCO 프로그램 Test TLA Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_TLA)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_TLA, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 10:	// SEMCO 프로그램 Test TLA 조정
	{
		TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
		MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
		double dSpeed = (double)stMotion.nMotorSpeed[0][2];
		double dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * stTeaching.nAdjust_Wait;
		double dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
		dTargetPos = dCurPos + dTargetPos;
		if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
		{
			err = MCEC_Motion_Adjust_MoveError;
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
		}
		break;
	}
	break;
	case 11:		// Screw 잠그기 1
	{
		bRet = IO_O_Screw_Bolt_Lock_1();
	}
	break;
	case 12:		// Screw 잠그기 2
	{
		bRet = IO_O_Screw_Bolt_Lock_2();
	}
	break;
	case 13:		// 조정 풀기
	{
		bRet = false;
		break;
	}
	break;
	case 14:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 15:	// SEMCO 프로그램 Test Vigneting 46 Start
	{


		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting46)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting46, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting46 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 16:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 17:	// SEMCO 프로그램 Test Vigneting 82 Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting82)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting82 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 18:	// SEMCO 프로그램 Preview Stop
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStop(m_portType))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStopAck(m_portType, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Stop Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 19:	// 파워 서플라이 끄기
	{
		//bRet = POWERSUPPLY_VoltOff(InspectionPortType::Left);

		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	break;
	default:
		return false;
	}

	return bRet;
}
#elif(SET_INSPECTOR == SYS_TLA_B)
bool CDlgManualMove::ManualMove(int nIndex)
{
	bool bRet = true;

	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch (nIndex)
	{
	case 1:		// 초기화 (Z축을 모두 안전 위치로 올린다)
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			bRet = false;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{

			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Bwd() == false)
			{
				bRet = false;
				break;
			}
			if (IO_O_Socket_Unlock() == false)
			{
				bRet = false;
				break;
			}
		}
	}
	break;
	case 2:		// 광원 OFF
	{
		bRet = LIGHT_Off();
	}
	break;
	case 3:		// 광원 ON
	{
		bRet = LIGHT_On();
	}
	break;
	case 4:		//제품 고정
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveScrew_Type(Screw_Work);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Fwd() == false)
			{
				bRet = false;
				break;
			}
			Sleep(500);
			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);

		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 5:		//파워 서플라이 켜기
	{
		//bRet = POWERSUPPLY_VoltOn(InspectionPortType::Left);

		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
	}
	break;
	case 6:		//	SEMCO 프로그램 Ready
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Ready Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
	}
	break;
	case 7:	// SEMCO 프로그램 Preview Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 8:		// Screw 풀기
	{
		bRet = IO_O_Screw_Bolt_Unlock();
	}
	break;
	case 9:	// SEMCO 프로그램 Test TLA Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_TLA)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_TLA, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 10:	// SEMCO 프로그램 Test TLA 조정
	{
		TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
		MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
		double dSpeed = (double)stMotion.nMotorSpeed[0][2];
		double dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * stTeaching.nAdjust_Wait;
		double dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
		dTargetPos = dCurPos + dTargetPos;
		if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
		{
			err = MCEC_Motion_Adjust_MoveError;
			if (err != enMCErrCode::MCEC_OK)
			{
				bRet = false;
				break;
			}
		}
		break;
	}
	break;
	case 11:		// Screw 잠그기 1
	{
		bRet = IO_O_Screw_Bolt_Lock_1();
	}
	break;
	case 12:		// Screw 잠그기 2
	{
		bRet = IO_O_Screw_Bolt_Lock_2();
	}
	break;
	case 13:		// 조정 풀기
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		Sleep(500);
		if (IO_O_Chuck_Bwd() == false)
		{
			bRet = false;
			break;
		}
	}
	break;
	case 14:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveVignetting_X_Type(X_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 15:	// SEMCO 프로그램 Test Vigneting 46 Start
	{


		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting46)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting46, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting46 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 16:	//	_T("Vigneting 46 광원 이동 "),
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				bRet = false;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				bRet = false;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveVignetting_X_Type(X_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_01);
				if (err != enMCErrCode::MCEC_OK)
				{
					bRet = false;
					break;
				}
			}
			else
			{
				bRet = false;
				break;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	break;
	case 17:	// SEMCO 프로그램 Test Vigneting 82 Start
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendStartTest(m_portType), eComMode_Insp_Vigneting82)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(m_portType, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test Vigneting82 Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 18:	// SEMCO 프로그램 Preview Stop
	{
		SEMCO_Reset(m_portType);
		if (!SEMCO_SendPreviewStop(m_portType))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStopAck(m_portType, 10000, nRetCode))
		{
			bRet = false;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Preview Stop Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case 19:	// 파워 서플라이 끄기
	{
		//bRet = POWERSUPPLY_VoltOff(InspectionPortType::Left);

		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	break;
	default:
		return false;
	}

	return bRet;
}
#endif