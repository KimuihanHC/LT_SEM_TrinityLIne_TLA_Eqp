// DlgTitle.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgTitle.h"
#include "afxdialogex.h"
#include "LuriMotion_BaseDlg.h"

#define TIMER_TITLEVIEW_TIME		500
#define TIMER_TITLEVIEW_DATE		501
#define TIMER_TITLEVIEW_TWR_LAMP	502
#define TIMER_TITLEVIEW_TWR_FLICK	503
#define TIMER_TITLEVIEW_RM_STATUS	504

// CDlgTitle 대화 상자

IMPLEMENT_DYNAMIC(CDlgTitle, CDialogEx)

CDlgTitle::CDlgTitle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TITLE, pParent)
{
	m_strTimer = _T("");
	m_strDate = _T("");

	for (int i = 0; i < 3; i++)
	{
		m_bBlinkBk[i] = 3;
		m_bBlinkMode[i] = FALSE;
		m_bBlinkState[i] = FALSE;
	}
	// [21.1029.5] 초기상태변경
	m_Mode_Old = (RUN_MODE)eNone;

	m_bProgramStartFlag = TRUE;
}

CDlgTitle::~CDlgTitle()
{
	KillTimer(TIMER_TITLEVIEW_TIME);
	KillTimer(TIMER_TITLEVIEW_DATE);
	KillTimer(TIMER_TITLEVIEW_TWR_LAMP);
	KillTimer(TIMER_TITLEVIEW_TWR_FLICK);
	KillTimer(TIMER_TITLEVIEW_RM_STATUS);
}

void CDlgTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_BTN_TITLE_1, m_btnTitleToolbar[0]);
	DDX_Control(pDX, IDC_BTN_TITLE_2, m_btnTitleToolbar[1]);
	DDX_Control(pDX, IDC_BTN_TITLE_3, m_btnTitleToolbar[2]);
	DDX_Control(pDX, IDC_BTN_TITLE_4, m_btnTitleToolbar[3]);

	DDX_Control(pDX, IDC_STC_TITLE_MCH_NAME, m_stTitleMchName);
	DDX_Control(pDX, IDC_STC_TITLE_MCH_NAME2, m_stTitleMchName_Recipe);
	

	DDX_Control(pDX, IDC_STC_TITLE_SUB_1, m_stTitleView[0]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_2, m_stTitleView[1]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_3, m_stTitleView[2]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_4, m_stTitleView[3]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_5, m_stTitleView[4]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_6, m_stTitleView[5]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_7, m_stTitleView[6]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_8, m_stTitleView[7]);
	DDX_Control(pDX, IDC_STC_TITLE_SUB_9, m_stTitleView[8]);		// run mode
	DDX_Control(pDX, IDC_STC_TITLE_SUB_10, m_stTitleView[9]);		// dry run
	DDX_Control(pDX, IDC_STC_TITLE_SUB_11, m_stTitleView[10]);		// simul mode
}


BEGIN_MESSAGE_MAP(CDlgTitle, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TITLE_1, &CDlgTitle::OnBnClickedBtnTitle1)
	ON_BN_CLICKED(IDC_BTN_TITLE_2, &CDlgTitle::OnBnClickedBtnTitle2)
	ON_BN_CLICKED(IDC_BTN_TITLE_3, &CDlgTitle::OnBnClickedBtnTitle3)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_TITLE_4, &CDlgTitle::OnBnClickedBtnTitle4)
END_MESSAGE_MAP()

BOOL CDlgTitle::PreTranslateMessage(MSG* pMsg)
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

void CDlgTitle::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.2]
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}
// CDlgTitle 메시지 처리기

BOOL CDlgTitle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}



int CDlgTitle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	SetTimer(TIMER_TITLEVIEW_TIME, 1000, NULL);			// 시간 TImer
	SetTimer(TIMER_TITLEVIEW_DATE, 5000, NULL);			// 날짜 Timer

	SetTimer(TIMER_TITLEVIEW_TWR_LAMP, 250, NULL);		// 타워램프 Timer
	SetTimer(TIMER_TITLEVIEW_TWR_FLICK, 500, NULL);		// 타워램프 Timer Blink
	SetTimer(TIMER_TITLEVIEW_RM_STATUS, 500, NULL);		// 런스테이터스 Blink

	return 0;
}
void CDlgTitle::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_t = CTime::GetCurrentTime();
		m_strTimer.Format(_T("%02dH %02dM %02dS"), m_t.GetHour(), m_t.GetMinute(), m_t.GetSecond());
		m_stTitleView[0].SetText(m_strTimer);
		
		m_strDate.Format(_T("%04dY %02dM %02dD"), m_t.GetYear(), m_t.GetMonth(), m_t.GetDay());
		m_stTitleView[1].SetText(m_strDate);

		// [21.1017.8] Modified
		m_stTitleView[3].SetText(COMMON->GetPermissionId());
		m_stTitleView[4].SetText(_T("-----"));
	}
	
}
BOOL CDlgTitle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ui 사이즈 및 폰트설정
	InitComponent_Title();
		   
	// 설비 명칭
	SetSystemTitle();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTitle::MoveWindow_Status(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	CRect	rct[3];

	UINT	iX = 0;
	UINT	iY = 0;
	UINT	iW = 0;
	UINT	iH = 0;
	UINT	iXSpace = (UINT)(nWidth * 0.005);
	UINT	iYSpace = (UINT)(nHeight * 0.1);

	// Simul mode status
	rct[0].left = x;
	rct[0].top = y;
	rct[0].right = rct[0].left + (LONG)(nWidth * 1.0);
	rct[0].bottom = rct[0].top + (LONG)(nHeight* 0.25);

	// Dry run status
	rct[1].left = x;
	rct[1].top = rct[0].bottom + iYSpace;
	rct[1].right = rct[1].left + (LONG)(nWidth * 1.0);
	rct[1].bottom = rct[1].top + (LONG)(nHeight * 0.25);

	// Eq status
	rct[2].left = x;
	rct[2].top = rct[1].bottom + iYSpace;
	rct[2].right = rct[2].left + (LONG)(nWidth * 1.0);
	rct[2].bottom = rct[2].top + (LONG)(nHeight * 0.25);


	// 실행
	for (int n = 0; n < 3; n++) {
		iX = rct[n].left;
		iY = rct[n].top;
		iW = rct[n].Width();
		iH = rct[n].Height();

		if (n == 0)
			m_stTitleView[10].SetWindowPos(nullptr, iX, iY, iW, iH, SWP_NOZORDER);
		else if (n == 1)
			m_stTitleView[9].SetWindowPos(nullptr, iX, iY, iW, iH, SWP_NOZORDER);
		else
			m_stTitleView[8].SetWindowPos(nullptr, iX, iY, iW, iH, SWP_NOZORDER);
	}
}

void CDlgTitle::InitComponent_Title()
{	
	CRect rt_Window;
	CRect rt;
	CRect rt_Move;

	GetClientRect(rt_Window);

	// 장비 이름을 기준으로 좌우로 나누자 
	// 우선은 장비 이름부터.
	// TITLE DLG의 x,y 길이는 1920,150 이다.이유는 전체화면으로 할떄..
	m_stTitleMchName.GetWindowRect(rt);
	int nCenterX = 1920 / 2;	
	int nCenterY = 150 / 2;

	int nTitle_Width = 400;
	int nTitle_Height = rt_Window.Height() / 2;

	rt_Move.left = nCenterX - nTitle_Width / 2;
	rt_Move.top = nCenterY - (nTitle_Height / 2) - 15;
	rt_Move.right = nCenterX + nTitle_Width / 2;
	rt_Move.bottom = nCenterY + (nTitle_Height / 2) - 15;

	m_stTitleMchName.MoveWindow(rt_Move, SWP_SHOWWINDOW);

	rt_Move.left = rt_Move.left;
	rt_Move.top = rt_Move.bottom + 5;
	rt_Move.right = rt_Move.right;
	rt_Move.bottom = rt_Move.top + 30 + 5;

	m_stTitleMchName_Recipe.MoveWindow(rt_Move, SWP_SHOWWINDOW);

	m_stTitleMchName_Recipe.MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 왼쪽 버튼 4개
	int nStart_X = 50;
	int nStart_Y = 25;

	int nTitleBtn_Width = 150;
	int nTitleBtn_Height = 100;
	int nGap = 15;

	for (int i = 0; i < 4; i++)
	{
		rt_Move.left = nStart_X + (nTitleBtn_Width* i) + (nGap * i);;
		rt_Move.top = nStart_Y;
		rt_Move.right = rt_Move.left + (nTitleBtn_Width);
		rt_Move.bottom = nStart_Y + nTitleBtn_Height;

		m_btnTitleToolbar[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	// 우측 Static 2개 시간/날짜

	int nStart_Time_X = 1225;
	int nStart_Time_Y = 25;

	int nWidth_Time = 150;
	int nHeith_Time = 50;

	int nPermissionStatic = 100;

	// 시간
	rt_Move.left = nStart_Time_X;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = nStart_Time_X+ nWidth_Time;
	rt_Move.bottom = rt_Move.top+ nHeith_Time;
	m_stTitleView[0].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 날짜
	rt_Move.left = nStart_Time_X;
	rt_Move.top = nStart_Time_Y + nHeith_Time + 1;
	rt_Move.right = nStart_Time_X + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[1].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 Static
	rt_Move.left = nStart_Time_X + nWidth_Time + 90;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = nStart_Time_X + nWidth_Time + nPermissionStatic + 90;
	rt_Move.bottom = rt_Move.top + (nHeith_Time*2) + 2;
	m_stTitleView[2].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 이름
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3;
	rt_Move.top = nStart_Time_Y - 1;
	rt_Move.right = rt_Move.left + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[3].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// 권한 level
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3;
	rt_Move.top = nStart_Time_Y + nHeith_Time + 1;
	rt_Move.right = rt_Move.left + nWidth_Time;
	rt_Move.bottom = rt_Move.top + nHeith_Time;
	m_stTitleView[4].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// TowerLamp 색상
	rt_Move.left = nStart_Time_X + nWidth_Time + nPermissionStatic + 90 + 3 + nWidth_Time + 70;
	rt_Move.top = nStart_Time_Y -1 ;
	rt_Move.right = rt_Move.left + 90;
	rt_Move.bottom = rt_Move.top + ((nHeith_Time*2) + 2) / 3;
	m_stTitleView[5].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	rt_Move.top += ((nHeith_Time * 2) + 2) / 3;
	rt_Move.bottom += ((nHeith_Time * 2) + 2) / 3;
	m_stTitleView[6].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	rt_Move.top += ((nHeith_Time * 2) + 2) / 3;
	rt_Move.bottom += ((nHeith_Time * 2) + 2) / 3;
	m_stTitleView[7].MoveWindow(rt_Move, SWP_SHOWWINDOW);

	// Move status
	MoveWindow_Status((int)(1920 / 5), 10, (int)(1920 / 8), 130);

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_stTitleMchName.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTitleMchName.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTitleMchName.SetFont_Gdip(L"Arial", 18.0F);
	m_stTitleMchName.SetText(_T("Loader Machine"));

	m_stTitleMchName_Recipe.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_stTitleMchName_Recipe.SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
	m_stTitleMchName_Recipe.SetFont_Gdip(L"Arial", 12.0F);
	m_stTitleMchName_Recipe.SetText(_T("Recipe Name"));	

	// UI 입히기
	for (int i = 0; i < 4; i++)
	{
		m_btnTitleToolbar[i].EnableWindowsTheming(FALSE);
		m_btnTitleToolbar[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnTitleToolbar[i].m_bTransparent = false;
		m_btnTitleToolbar[i].SetTextColor(COLOR_BLACK);
		m_btnTitleToolbar[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0 )
			m_btnTitleToolbar[i].SetWindowText(m_szTitleMenuKor[i]);
		else
			m_btnTitleToolbar[i].SetWindowText(m_szTitleMenuEng[i]);
	}

	for (int i = 0; i < 11; i++)
	{
		if (i < 8)
		{
			m_stTitleView[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_stTitleView[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_stTitleView[i].SetFont_Gdip(L"Arial", 11.0F);
		}
		else
		{
			m_stTitleView[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_stTitleView[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			m_stTitleView[i].SetFont_Gdip(L"Arial", 11.0F);
		}

		if (i == 2) 
		{
			if (stBasic.nLanguage == 0)
				m_stTitleView[2].SetText(_T("계정\n\n권한"));
			else
				m_stTitleView[2].SetText(_T("Account\n\nPermission"));
		}
			
		else
			m_stTitleView[i].SetText(_T(""));
	}

	m_stTitleView[8].SetText(_T("---"));
	m_stTitleView[9].SetText(_T("Dry run"));
	m_stTitleView[10].SetText(_T("Simul mode"));

	m_stTitleView[8].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_stTitleView[9].SetColorStyle(CVGStatic::ColorStyle_LightGreen);
	m_stTitleView[10].SetColorStyle(CVGStatic::ColorStyle_LightGreen);

	m_stTitleView[9].ShowWindow(FALSE);	
}

void CDlgTitle::SetSystemTitle()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_stTitleMchName.SetText(stTester.strEQPName);

	CString szTitleName;
	
	m_stTitleMchName.GetText(szTitleName);
	
#ifdef DevMode_Simulator
	szTitleName += _T(" [Debugging]");
	m_stTitleMchName.SetText(szTitleName);
#endif // DevMode_Simulator

}

void CDlgTitle::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_TITLEVIEW_TIME:
	{
		m_t = CTime::GetCurrentTime();
		m_strTimer.Format(_T("%02d : %02d : %02d"), m_t.GetHour(), m_t.GetMinute(), m_t.GetSecond());

		m_stTitleView[0].SetText(m_strTimer);

		if (m_bProgramStartFlag == TRUE)
		{
			EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

			// [21.1021.9] S/W VER
			m_stTitleMchName_Recipe.SetText(SW_VERSION);
			m_bProgramStartFlag = FALSE;
		}

		if (COMMON->m_bRecipeChange == TRUE)
		{
//			m_stTitleMchName_Recipe.SetText(DATAMANAGER->m_strCurrenctRecipeName);
//			COMMON->m_bRecipeChange = FALSE;
		}

		CString sz;

		auto * p = SEQUENCE->GetPtrSeqPickerMgr();

		switch (p->GetMode())
		{
		case enPickUseType::Use_LR:
			sz = _T("L:Use, R:Use");
			break;
		case enPickUseType::Use_RL:
			sz = _T("R:Use, L:Use");
			break;
		case enPickUseType::Use_L_Skip_R:
			sz = _T("L:Use, R:Skip");
			sz += (p->GetAutoChange()) ? _T(",Change") : _T("");
			break;
		case enPickUseType::Use_R_Skip_L:
			sz = _T("Right : Use, Left : Skip");
			sz += (p->GetAutoChange()) ? _T(",Change") : _T("");
			break;
		}

		m_stTitleView[10].SetText(sz);
	}
	break;

	case TIMER_TITLEVIEW_DATE:
	{
		m_t = CTime::GetCurrentTime();
		m_strDate.Format(_T("%04d / %02d / %02d"), m_t.GetYear(), m_t.GetMonth(), m_t.GetDay());

		m_stTitleView[1].SetText(m_strDate);
	}
	break;

	// [21.1029.2]
	case TIMER_TITLEVIEW_TWR_LAMP:
	{
		EQ_TOWERLAMP	stTower = *DATAMANAGER->GetCurrentTowerLampData();
		RUN_MODE rm = SEQUENCE->GetRunMode();

		if (SEQUENCE->GetRunMode() == (RUN_MODE)eStandby && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();

			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeReady][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eRunning && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();

//			if (COMMON->GetSemiAutoModeFlag() == TRUE)
			if(COMMON->GetOperationMode() == OperationMode::SemiAuto)
			{
				for (int i = 0; i < 3; i++)
				{
					m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeStop][i];
				}
			}
			else{
				for (int i = 0; i < 3; i++)
				{
					m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeRun][i];
				}
			}

		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeStop][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)eError && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModeError][i];
			}
		}
		else if (SEQUENCE->GetRunMode() == (RUN_MODE)ePause && m_Mode_Old != SEQUENCE->GetRunMode())
		{
			m_Mode_Old = SEQUENCE->GetRunMode();


			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = stTower.nTowerLampSetting[ETwrMode::eModePause][i];
			}
		}
	}
	break;

	// [21.1029.3]
	case TIMER_TITLEVIEW_TWR_FLICK:
	{
		// Twr Red
		if (m_bBlinkMode[eTowerRed] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerRed] != m_bBlinkMode[eTowerRed])
			{
				m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];
				//m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Red);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_RED, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerRed] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerRed] != m_bBlinkMode[eTowerRed])
			{
				m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];
				SetTowerLampUI(eTowerRed);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_RED, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerRed] == eTowerLamp_Blink)
		{
			m_bBlinkBk[eTowerRed] = m_bBlinkMode[eTowerRed];

			if (m_bBlinkState[eTowerRed] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_RED, TRUE);
				SetTowerLampUI(eTowerRed);
				m_bBlinkState[eTowerRed] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_09_TOWERLAMP_RED, FALSE);
				SetTowerLampUI(eTowerMax);
				m_bBlinkState[eTowerRed] = FALSE;
			}
		}
		// Twr Yellow
		if (m_bBlinkMode[eTowerYellow] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerYellow] != m_bBlinkMode[eTowerYellow])
			{
				m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];
				//m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Yellow);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0A_TOWERLAMP_YELLOW, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerYellow] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerYellow] != m_bBlinkMode[eTowerYellow])
			{
				m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];
				SetTowerLampUI(eTowerYellow);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0A_TOWERLAMP_YELLOW, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerYellow] == eTowerLamp_Blink)
		{
			m_bBlinkBk[eTowerYellow] = m_bBlinkMode[eTowerYellow];

			if (m_bBlinkState[eTowerYellow] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0A_TOWERLAMP_YELLOW, TRUE);
				SetTowerLampUI(eTowerYellow);
				m_bBlinkState[eTowerYellow] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0A_TOWERLAMP_YELLOW, FALSE);
				SetTowerLampUI(eTowerMax);
				m_bBlinkState[eTowerYellow] = FALSE;
			}
		}
		// Twr Green
		if (m_bBlinkMode[eTowerGreen] == eTowerLamp_Off)
		{
			if (m_bBlinkBk[eTowerGreen] != m_bBlinkMode[eTowerGreen])
			{
				m_bBlinkBk[eTowerGreen] = m_bBlinkMode[eTowerGreen];
				//m_stTitleView[5].SetColorStyle(CVGStatic::ColorStyle_Green);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_TOWERLAMP_GREEN, FALSE);
			}
		}
		else if (m_bBlinkMode[eTowerGreen] == eTowerLamp_On)
		{
			if (m_bBlinkBk[eTowerGreen] != m_bBlinkMode[eTowerGreen])
			{
				m_bBlinkBk[eTowerGreen] = m_bBlinkMode[eTowerGreen];
				SetTowerLampUI(eTowerGreen);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_TOWERLAMP_GREEN, TRUE);
			}
		}
		else if (m_bBlinkMode[eTowerGreen] == eTowerLamp_Blink)
		{
			if (m_bBlinkState[eTowerGreen] == FALSE)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_TOWERLAMP_GREEN, TRUE);
				SetTowerLampUI(eTowerGreen);
				m_bBlinkState[eTowerGreen] = TRUE;
			}
			else
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0B_TOWERLAMP_GREEN, FALSE);
				SetTowerLampUI(eTowerMax);
				m_bBlinkState[eTowerGreen] = FALSE;
			}
		}
	}
	break;
	case TIMER_TITLEVIEW_RM_STATUS:
	{
		CString str;
		RUN_MODE rm;
		rm = SEQUENCE->GetRunMode();
		if (rm == eNone) str.Format(_T("None"));
		else if (rm == eStandby) str.Format(_T("Stop idle"));
		else if (rm == eStop) str.Format(_T("Stop idle"));
		else if (rm == eEStop) str.Format(_T("EStop idle"));
		else if (rm == ePause) str.Format(_T("Pause idle"));
		else if (rm == eError) str.Format(_T("Stop Error"));
		else if (rm == eRunning) str.Format(_T("Running"));
		else str.Format(_T("---"));

		m_stTitleView[8].SetText(str);

		if (rm == eRunning)
		{
			if (m_nFlickCount++ < 2)
			{
				m_stTitleView[8].SetColorStyle(CVGStatic::ColorStyle_Yellow);
			}
			else if (m_nFlickCount < 4)
			{
				m_stTitleView[8].SetColorStyle(CVGStatic::ColorStyle_Green);
			}
			else
			{
				m_nFlickCount = 0;
			}
		}
	}
	break;
	}
}

void CDlgTitle::SetTowerLampUI(ETwrLamp color)
{
	auto & redCtrl = m_stTitleView[5];
	auto & yellowCtrl = m_stTitleView[6];
	auto & greenCtrl = m_stTitleView[7];

	redCtrl.SetColorStyle(CVGStatic::ColorStyle_White);
	yellowCtrl.SetColorStyle(CVGStatic::ColorStyle_White);
	greenCtrl.SetColorStyle(CVGStatic::ColorStyle_White);

	switch (color)
	{
	case eTowerRed:
		redCtrl.SetColorStyle(CVGStatic::ColorStyle_Red);
		break;
	case eTowerYellow:
		yellowCtrl.SetColorStyle(CVGStatic::ColorStyle_Yellow);
		break;
	case eTowerGreen:
		greenCtrl.SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case eTowerBuzzer:
	case eTowerMax:
		break;
	}
}

void CDlgTitle::OnBnClickedBtnTitle1()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnTitleToolbar[0].SetFaceColor(COLOR_GREEN1, true);

	if( stBasic.nLanguage == 0 )
		m_btnTitleToolbar[0].SetWindowText(m_szTitleMenuKor[0]);
	else
		m_btnTitleToolbar[0].SetWindowText(m_szTitleMenuEng[0]);

	CDlgLanguage Dlg;
	
	if (Dlg.DoModal() == IDOK)
	{
		EQ_BASIC_PARAM	stBasic_Change = *DATAMANAGER->GetCurrentEqBasicData();

		m_btnTitleToolbar[0].SetTextColor(COLOR_BLACK);
		m_btnTitleToolbar[0].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic_Change.nLanguage == 0)
			m_btnTitleToolbar[0].SetWindowText(m_szTitleMenuKor[0]);
		else
			m_btnTitleToolbar[0].SetWindowText(m_szTitleMenuEng[0]);

		if (Dlg.GetChangeLanguage() != -1)
		{
			for (int i = 0; i < 4; i++)
			{
				m_btnTitleToolbar[i].SetTextColor(COLOR_BLACK);
				m_btnTitleToolbar[i].SetFaceColor(RGB(225, 225, 225), true);

				if (stBasic_Change.nLanguage == 0)
					m_btnTitleToolbar[i].SetWindowText(m_szTitleMenuKor[i]);
				else
					m_btnTitleToolbar[i].SetWindowText(m_szTitleMenuEng[i]);
			}

			m_stTitleView[2].SetStaticStyle(CVGStatic::StaticStyle_Data);
			m_stTitleView[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_stTitleView[2].SetFont_Gdip(L"Arial", 11.0F);

			if (stBasic_Change.nLanguage == 0)
				m_stTitleView[2].SetText(_T("계정\n\n권한"));
			else
				m_stTitleView[2].SetText(_T("Account\n\nPermission"));

			// 타이머는 돌고 있는 상태라서 UI 상으로는 바로 변경이 안되니 종료후 다시 시작하자.
			KillTimer(100);
			KillTimer(101);
			KillTimer(200);
			KillTimer(201);

			for (int i = 0; i < 3; i++)
			{
				m_bBlinkMode[i] = FALSE;
				m_bBlinkState[i] = FALSE;
			}

			SetTimer(100, 1000, NULL);			// 시간 TImer
			SetTimer(101, 5000, NULL);			// 날짜 Timer
			SetTimer(200, 250, NULL);			// 타워램프 Timer
			SetTimer(201, 500, NULL);			// 타워램프 Timer Blink

			// 2. 전체 DLG의 UI를 변경해야 한다.
			CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
			pParrentDlg->ChangeUi_ByLanguage();
		}
	}
}
// [21.1017.9] Modified
void CDlgTitle::OnBnClickedBtnTitle2()
{
	int nMode = COMMON->GetPermissionGrade();
	CDlgAdmin Dlg(nMode);

	if (Dlg.DoModal() == IDOK)
	{
		if (Dlg.GetChangFlag() == TRUE)
		{
			CString strTemp = _T("");
			if (COMMON->GetPermissionGrade() == LVL_ADMIN)
				strTemp = _T("ADMINISTRATOR");
			else if (COMMON->GetPermissionGrade() == LVL_MAINT)
				strTemp = _T("MAINTENANCE");
			else if (COMMON->GetPermissionGrade() == LVL_OPER)
				strTemp = _T("OPERATOR");

			m_stTitleView[3].SetText(COMMON->GetPermissionId());
			m_stTitleView[4].SetText(strTemp);

			CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
			pParrentDlg->ChangeUi_ByLevel();
		}
	}
	else
	{

	}
}


void CDlgTitle::OnBnClickedBtnTitle3()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnTitleToolbar[2].SetFaceColor(COLOR_GREEN1, true);
	if (stBasic.nLanguage == 0)
		m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuKor[2]);
	else
		m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuEng[2]);


	CDlgTowerLamp Dlg(0);

	if (Dlg.DoModal() == IDOK)
	{
		m_btnTitleToolbar[2].SetTextColor(COLOR_BLACK);
		m_btnTitleToolbar[2].SetFaceColor(RGB(225, 225, 225), true);
		if (stBasic.nLanguage == 0)
			m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuKor[2]);
		else
			m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuEng[2]);
	}
	else
	{
		m_btnTitleToolbar[2].SetTextColor(COLOR_BLACK);
		m_btnTitleToolbar[2].SetFaceColor(RGB(225, 225, 225), true);
		if (stBasic.nLanguage == 0)
			m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuKor[2]);
		else
			m_btnTitleToolbar[2].SetWindowText(m_szTitleMenuEng[2]);
	}
}

void CDlgTitle::OnBnClickedBtnTitle4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
