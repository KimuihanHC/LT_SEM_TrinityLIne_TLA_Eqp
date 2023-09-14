
// LuriMotionUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "afxdialogex.h"

#if (20230905)
#include "Common_TCP.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_BASE_STARTUP					100

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLuriMotionUIDlg 대화 상자
CLuriMotion_BaseDlg::CLuriMotion_BaseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LT_BASE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDlgMain				= NULL;
	m_pDlgSide				= NULL;
	m_pDlgTitle				= NULL;
	m_pDlgBtm				= NULL;
	
	m_pDlgTeach				= NULL;
	m_pDlgIO				= NULL;
	m_pDlgConfiguration		= NULL;
	m_pDlgLog				= NULL;

	m_bStartCompleate		= FALSE;
	m_bMessageBlock = false;

	VERIFY(m_font_List.CreateFont(
		18,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Cambria")));			// lpszFacename

	VERIFY(m_font_Large.CreateFont(
		16,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Default.CreateFont(
		24,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Normal.CreateFont(
		18,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Gothic")));			// lpszFacename
}	
void CLuriMotion_BaseDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BASE_STATIC, m_ctrlStatic);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLuriMotion_BaseDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(MESSAGE_UI_WINDOW_TOP,		OnWindow2Top)
	ON_MESSAGE(MESSAGE_UI_STARTUP,			OnStartup2ui)
	ON_MESSAGE(UM_RECV_SEMCO_LIGHT_CONTROL, OnRecvLightArgFromSEMCO)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

LRESULT CLuriMotion_BaseDlg::OnRecvLightArgFromSEMCO(WPARAM wParam, LPARAM lParam)
{
	LRESULT lReturn = 0;

	int nIdx = (int)wParam;
	ST_MsgLightArg* arg = reinterpret_cast<ST_MsgLightArg*>(lParam);

	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	
	// 광원 변경 
	auto semcoIdx = arg->nLightIdx;
	auto bOn = arg->bLightOn;
	auto nUseLightCh = 0;

	if (semcoIdx == 0 && bOn == false)
	{
		LIGHT_Color_Cal_Off();
	}
	else
	{
		nUseLightCh = stTester.nColorCal_SemcoLight[semcoIdx - 1];

		if (bOn)
			LIGHT_Color_Cal_On(nUseLightCh);
		else
			LIGHT_Color_Cal_Off(nUseLightCh);
	}

	Sleep(500);
	
	// 로그 출력 
	CString sz;
	sz.Format(_T("Recv LightControl To Semco [BLU Idx = %d, SetLightCh_%d, OnOff = %s]"), semcoIdx, nUseLightCh + 1, (arg->bLightOn) ? _T("ON") : _T("OFF"));
	WRITE_MAIN_LOG(sz);

	// ack 전송
	m_UdpTester[nIdx].SendAckBLU(arg->nLightIdx, arg->bLightOn);
	return lReturn;
}

BOOL CLuriMotion_BaseDlg::PreTranslateMessage(MSG* pMsg)
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

// CLuriMotionUIDlg 메시지 처리기
BOOL CLuriMotion_BaseDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	srand(time(NULL));
	int  r, g, b;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;

	pDC->FillSolidRect(rect, RGB(r, g, b));	// rand()
	return TRUE;
}

BOOL CLuriMotion_BaseDlg::EraseBkGnd_Image(CWnd * pWnd, CDC * pDC, /*CBitmap * pBmp*/EBITMAP_LIST nImageId, int nBit)
{
	CRect		rect;
	pWnd->GetClientRect(rect);

	CGdiObject *m_old;
	CDC			dc;
	CBitmap * pBmp = &m_cbitBack[nImageId];
	dc.CreateCompatibleDC(pDC);
	m_old = dc.SelectObject(pBmp);

	BITMAP		bmp;
	pBmp->GetBitmap(&bmp);
	if (nBit == 1)
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	else
		pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	dc.SelectObject(m_old);
	return TRUE;
}

BOOL CLuriMotion_BaseDlg::IsImageHandleValid(EBITMAP_LIST nImageId)
{
	BOOL b;
	return (b = (m_cbitBack[nImageId].m_hObject != NULL ? TRUE : FALSE));
}

BOOL CLuriMotion_BaseDlg::IsIconHandleValid(EICON_LIST nIconId)
{
	BOOL b;
	return (b = (m_IconBack[nIconId] != NULL ? TRUE : FALSE));
}

HICON CLuriMotion_BaseDlg::GetIconHandle(EICON_LIST nIconId)
{
	return (m_IconBack[nIconId]);
}

BOOL CLuriMotion_BaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_ctrlStatic.SetFont(&m_font_List);

	InitBitmap();
	InitProgram();
	
	ShowWindow(SW_NORMAL);

	SetTimer(TIMER_BASE_STARTUP, 100, NULL);	

	m_bStartCompleate = TRUE;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLuriMotion_BaseDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent)
	{
	case TIMER_BASE_STARTUP:
	{
		CreateDlg();

		SendMessage(MESSAGE_UI_STARTUP, 2, 1);

		Dlg_MovePosition();

		SendMessage(MESSAGE_UI_STARTUP, 3, 1);

		InitDevice();

		m_pDlgMain->ShowWindow(SW_SHOW);
		m_pDlgSide->ShowWindow(SW_SHOW);
		m_pDlgBtm->ShowWindow(SW_SHOW);
		m_pDlgTitle->ShowWindow(SW_SHOW);
	}
	break;

	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CLuriMotion_BaseDlg::Connect_Tester()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Thread_Start();
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	
	return TRUE;
}

void CLuriMotion_BaseDlg::CreateDlg()
{
	m_pDlgMain = CDlgMain::Get_Instance(this);
	
	//m_pDlgMain->ShowWindow(SW_SHOW);
	SendMessage(MESSAGE_UI_STARTUP, 1, 1);
	m_pDlgSide = new CDlgSide;
	m_pDlgSide->Create(CDlgSide::IDD, 0);
	//m_pDlgSide->ShowWindow(SW_SHOW);

	SendMessage(MESSAGE_UI_STARTUP, 1, 2);
	m_pDlgBtm = new CDlgBottom;
	m_pDlgBtm->Create(CDlgBottom::IDD, 0);
	//m_pDlgBtm->ShowWindow(SW_SHOW);

	SendMessage(MESSAGE_UI_STARTUP, 1, 3);
	m_pDlgTitle = new CDlgTitle;
	m_pDlgTitle->Create(CDlgTitle::IDD, 0);
	//m_pDlgTitle->ShowWindow(SW_SHOW);

	SendMessage(MESSAGE_UI_STARTUP, 1, 4);
	m_pDlgTeach = new CDlgMotorTeach;
	m_pDlgTeach->Create(CDlgMotorTeach::IDD, 0);
	m_pDlgTeach->ShowWindow(SW_HIDE);

	SendMessage(MESSAGE_UI_STARTUP, 1, 5);
	m_pDlgIO = new CDlgIo;
	m_pDlgIO->Create(CDlgIo::IDD, 0);
	m_pDlgIO->ShowWindow(SW_HIDE);

	SendMessage(MESSAGE_UI_STARTUP, 1, 6);
	m_pDlgErrSolution = new CDlgErrorSolution;
	m_pDlgErrSolution->Create(CDlgErrorSolution::IDD, 0);
	m_pDlgErrSolution->ShowWindow(SW_HIDE);	

	SendMessage(MESSAGE_UI_STARTUP, 1, 7);
	m_pDlgLog = CDlgLog::Get_Instance(this);
	m_pDlgLog->Create(CDlgLog::IDD, 0);
	m_pDlgLog->ShowWindow(SW_HIDE);

	SendMessage(MESSAGE_UI_STARTUP, 1, 8);
	m_pDlgConfiguration = new CDlgConfiguration;
	m_pDlgConfiguration->Create(CDlgConfiguration::IDD, 0);
	m_pDlgConfiguration->ShowWindow(SW_HIDE);

	m_pMsgBox_Instance = CDlgMessageBox_Instance::Get_Instance(this);
}

void CLuriMotion_BaseDlg::Dlg_MovePosition()
{
	CRect rt;

	if (m_pDlgTitle == nullptr ||
		m_pDlgMain == nullptr ||
		m_pDlgTeach == nullptr ||
		m_pDlgIO == nullptr ||
		m_pDlgConfiguration == nullptr ||
		m_pDlgErrSolution == nullptr ||
		m_pDlgLog == nullptr ||
		m_pDlgSide == nullptr ||
		m_pDlgBtm == nullptr)
		return;

	// Title.
	rt.left = 0;
	rt.top = 0;
	rt.right = 1920;
	rt.bottom = 150;
	m_pDlgTitle->MoveWindow(rt, TRUE);

	// Main 먼저.
	rt.left = 0;
	rt.top = 150;
	rt.right = 1700;
	rt.bottom = 940;
	m_pDlgMain->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgTeach->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgIO->MoveWindow(rt, TRUE);

	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgConfiguration->MoveWindow(rt, TRUE);
	
	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgErrSolution->MoveWindow(rt, TRUE);
	
	rt.left = 0;
	rt.top = 150;
	rt.right = 1701;
	rt.bottom = 940;
	m_pDlgLog->MoveWindow(rt, TRUE);

	// Side 
	rt.left = 1700;
	rt.top = 150;
	rt.right = 1920;
	rt.bottom = 940;
	m_pDlgSide->MoveWindow(rt, TRUE);

	//Btm
	rt.left = 0;
	rt.top = 940;
	rt.right = 1920;
	rt.bottom = 1080;
	m_pDlgBtm->MoveWindow(rt, TRUE);
	
	Invalidate();
}
void CLuriMotion_BaseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLuriMotion_BaseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLuriMotion_BaseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLuriMotion_BaseDlg::OnClose()
{
	SEQUENCE->End_RunThread();
	auto pSide = CDlgSide::Get_Instance();
	pSide->End_RunThread();


	Disconnectz();

	m_ctrlLight_SOL_3_1.ClosePort();
	m_ctrlLight_SOL_3_2.ClosePort();
	m_ctrlLight_SOL_3_3.ClosePort();
	m_ctrlLight_SOL_3_4.ClosePort();
	m_ctrlLight_SOL_3_5.ClosePort();
	m_ctrlLight_SOL_3_6.ClosePort();
	m_ctrlLight_SOL_3_7.ClosePort();
	m_ctrlLight_SOL_3_8.ClosePort();
	m_ctrlLight_SOL_3_9.ClosePort();

	PostQuitMessage(WM_QUIT);

	HANDLE hHandle = AfxGetMainWnd()->m_hWnd;
	DWORD dwID;
	GetWindowThreadProcessId((HWND)hHandle, &dwID);

	hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
	TerminateProcess(hHandle, 0);
	CloseHandle(hHandle);
}

void CLuriMotion_BaseDlg::ChangeMainUi(int nPart)
{
	m_pMsgBox_Instance->ShowWindow(SW_HIDE);

	switch ( nPart )
	{
	case 1:
		COMMON->m_nUICheck = 1;
		m_pDlgMain->ShowWindow(SW_SHOW);
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgConfiguration->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgLog->ShowWindow(SW_HIDE);

		m_pDlgMain->SetOperationMode(COMMON->GetOperationMode());
		break;
	case 2:
		COMMON->m_nUICheck = 2;
		m_pDlgTeach->ShowWindow(SW_SHOW);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgIO->ShowWindow(SW_HIDE);	
		m_pDlgConfiguration->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgLog->ShowWindow(SW_HIDE);
		break;
	case 3:
		COMMON->m_nUICheck = 3;
		m_pDlgIO->ShowWindow(SW_SHOW);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgConfiguration->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgLog->ShowWindow(SW_HIDE);
		break;
	case 4:
		COMMON->m_nUICheck = 4;
		m_pDlgConfiguration->ShowWindow(SW_SHOW);
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		m_pDlgLog->ShowWindow(SW_HIDE);
		break;
	case 5:
		COMMON->m_nUICheck = 5;
		m_pDlgLog->ShowWindow(SW_SHOW);
		m_pDlgIO->ShowWindow(SW_HIDE);
		m_pDlgTeach->ShowWindow(SW_HIDE);
		m_pDlgMain->ShowWindow(SW_HIDE);
		m_pDlgConfiguration->ShowWindow(SW_HIDE);
		m_pDlgErrSolution->ShowWindow(SW_HIDE);
		break;
	case 6:
		break;
	case 7:
		break;
	}
}
void CLuriMotion_BaseDlg::RecipeChange_Ui_Refresh()
{
	m_pDlgTeach->Load_TeachingData();

	m_pDlgConfiguration->Load_ComponentDlg();
}

// [21.1017.10] Modified , 이름변경
void CLuriMotion_BaseDlg::ChangeUi_ByLanguage()
{
	m_pDlgBtm->ChangeUi_Language();
	m_pDlgMain->ChangeUI_Language();
	m_pDlgIO->ChangeUI_Language();
	m_pDlgTeach->ChangeUI_Language();
	m_pDlgConfiguration->ChangeUI_Language();
	m_pDlgSide->ChangeUI_Language();
	m_pDlgErrSolution->ChangeUI_Language();
	m_pDlgLog->ChangeUI_Language();
}
// [21.1017.11] Added
void CLuriMotion_BaseDlg::ChangeUi_ByLevel()
{
	m_pDlgTeach->ChangeUI_Level();
}

BOOL CLuriMotion_BaseDlg::RunMessageDlg_Domodal(CString strInfo, int nMode)
{
	if (nMode == (int)MODE_PASS|| nMode == (int)MODE_FAIL)
	{
// 		if (m_bMessageBlock == true)
// 			return true;
		m_DlgMessage.SetMsg(strInfo, nMode);
		m_bMessageBlock = false;
		if (m_DlgMessage.DoModal() == IDOK)
		{
			m_bMessageBlock = false;
			return TRUE;
		}
		m_bMessageBlock = false;
	}
	else
	{
		if (m_bMessageBlock == true)
			return true;
		m_DlgMessage.SetMsg(strInfo, nMode);
		m_bMessageBlock = true;
		if (m_DlgMessage.DoModal() == IDOK)
		{
			m_bMessageBlock = false;
			return TRUE;
		}
		m_bMessageBlock = false;
	}

	return FALSE;
}

void CLuriMotion_BaseDlg::RunMessageDlg_Instance(CString strInfo, int nMode)
{
	m_pMsgBox_Instance->SetMsg(strInfo, nMode);
}
void CLuriMotion_BaseDlg::RunMessageDlg_Instance_Show()
{
	m_pMsgBox_Instance->ShowWindow(SW_SHOW);
}

void CLuriMotion_BaseDlg::Hide_Program()
{
	ShowWindow(SW_SHOWMINIMIZED);
//	ShowWindow(SW_HIDE);
	
}	

// [21.1021.7]
LRESULT CLuriMotion_BaseDlg::OnWindow2Top(WPARAM wParam, LPARAM lParam)
{
	BringWindowToTop();
	m_pDlgSide->BringWindowToTop();
	return TRUE;
}

LRESULT CLuriMotion_BaseDlg::OnStartup2ui(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str = _T("");
	switch (wParam)
	{
	case 1:
	{
		if (lParam == 1)
			str = _T("Creating Side View");
		else if (lParam == 2)
			str = _T("Creating Bottom View");
		else if (lParam == 3)
			str = _T("Creating Title View");
		else if (lParam == 4)
			str = _T("Creating Motor Teach View");
		else if (lParam == 5)
			str = _T("Creating I/O View");
		else if (lParam == 6)
			str = _T("Creating Error View");
		else if (lParam == 7)
			str = _T("Creating Log View");
		else if (lParam == 8)
			str = _T("Creating Configuration View");
		else
			str = _T("Creating UI");
	}
	break;
	case 2:
	{
		str = _T("Moving UI");
	}
	break;
	case 3:
	{
		if (lParam == 1)
			str = _T("Connecting Motion");
		else
			str = _T("Connecting Tester");
	}
	break;
	}
	m_ctrlStatic.SetWindowText(str);
	return TRUE;
}

void CLuriMotion_BaseDlg::InitBitmap()
{
	// Create Bitmap Handle .
	int		nBitmap[eBITMAP_COUNT] = { /*IDB_BITMAP_VIEWMAIN, */IDB_BITMAP_TEACHING , /*IDB_BITMAP_SETTING ,*/  IDB_BITMAP_INITIAL ,/*IDB_BITMAP_MESSAGE,*/ IDB_BITMAP_SOCKET_INFO, /*IDB_BITMAP_TR_CONV,*/IDB_BITMAP_SOCKET };

	HBITMAP		hBitmap;
	HINSTANCE	hInstResource;
	int			nBitmapX;

	for (int i = 0; i < eBITMAP_COUNT; i++)
	{
		nBitmapX = nBitmap[i];
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapX), RT_BITMAP);
		hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapX), IMAGE_BITMAP, 0, 0, 0);
		m_cbitBack[i].Attach(hBitmap);
	}

	// Create Icon Handle .
	int		nIcon[eICON_COUNT] = { IDI_ICON_OK, IDI_ICON_CANCEL , IDI_ICON_DELETE ,  IDI_ICON_SAVE ,IDI_ICON_LED_ON, IDI_ICON_LED_OFF, IDI_ICON_RLED_GRN,IDI_ICON_RLED_RED, IDI_ICON_RLED_OFF,IDI_ICON_MORE,IDI_ICON_FEWER };

	HICON		hIcon;
	int			nIconX;

	for (int i = 0; i < eICON_COUNT; i++)
	{
		nIconX = nIcon[i];
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIconX), RT_GROUP_ICON);
		hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconX), IMAGE_ICON, 0, 0, 0);
		m_IconBack[i] = hIcon;
	}
}

void CLuriMotion_BaseDlg::InitProgram()
{
	COMMON->SetOperationMode(OperationMode::Manual);
}

void CLuriMotion_BaseDlg::InitDevice()
{
	// Ajin board init
	AJIN_IO->Start_AjinIO();

	// Sleep on 을 꺼주자..
	IO_O_SleepMode(false);
// 	IO_O_CV1_IF_FrontIn(false);
// 	IO_O_CVR_IF_FrontOut(false);
// 	IO_O_CV2_IF_RearIn(false);
// 	IO_O_CVR_IF_RearOut(false);
	
// 	if (SET_INSPECTOR == SYS_DISTORTION)
// 	{
		AJIN_MO->Init_Ajin();
// 		if (AJIN_MO->GetGantryModeEnable() == FALSE)
// 		{
// 			if (AJIN_MO->GantryEnable((long)0, (long)1) == TRUE)
// 			{
// 				WRITE_MAIN_LOG(_T("Ajin Motion Gantry Setting Success"));
// 			}
// 			else
// 			{
// 				WRITE_MAIN_LOG(_T("Ajin Motion Gantry Setting Fail"));
// 				COMMON->ShowMsgDlg_Domodal(_T("Ajin Motion Gantry Setting Fail"), MODE_INFORMATION);
// 			}
// 		}
//	}
	
//	DWORD BaudInitList_Motor[] = { 9600, 19200, 38400, 57600, 115200 };

	// Fastech EziMotion.
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	BOOL bRet = FALSE;

// 	bRet = FAS_MO->Connect_EziServo(stSpeed.nPort[0], BaudInitList_Motor[stSpeed.nBaudRate[0]]);
// 	if (bRet == FALSE)
// 		WRITE_MAIN_LOG(_T("EziServo_Connect Fail"));
// 	else
// 	{
// 		FAS_MO->Servo_Enable_Init();
// 		for (int i = 0; i < MOTOR_MAX_COUNT; i++)
// 			FAS_MO->Servo_Enable(i);
// 	}

	SendMessage(MESSAGE_UI_STARTUP, 3, 2);

	//테스터 연결
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[0].InitVariable(0);
	m_UdpTester[1].InitVariable(1);

	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	m_UdpTester[1].Connect_Tester(stTester.dwTesterIP[1], stTester.nTesterPort[1], stTester.strTesterConnect[1], 1);
	COMMON->DoEvent(100);

	//bool bRet = false;
	bRet = m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStop);
	if (bRet == true)
	{
		m_UdpTester[0].m_bConnected = TRUE;
	}
	m_UdpTester[1].Send_Tester_Communication(eComMode_PreviewStop);

	m_UdpTester[0].SetOwnerHWND(this->m_hWnd);
	m_UdpTester[1].SetOwnerHWND(this->m_hWnd);

	m_UdpTester[0].SetWmRecv(UM_RECV_SEMCO_LIGHT_CONTROL);
	m_UdpTester[1].SetWmRecv(UM_RECV_SEMCO_LIGHT_CONTROL);

	if (m_ctrlLight_ODA_Q[0].Is_Connected() == FALSE)
	{
		m_ctrlLight_ODA_Q[0].Create();
		m_ctrlLight_ODA_Q[0].Invalidate(0);
	}

	if (m_ctrlLight_ODA_Q[0].OpenPort(stTester.nPort[0] + 1, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0) == FALSE)
	{
		WRITE_MAIN_LOG(_T("FAIL CONNECT"));
	}
	else
	{
		WRITE_MAIN_LOG(_T("CONNECT SUCCESS"));

		if (m_ctrlLight_ODA_Q[0].Wrtie_ON())
		{
			WRITE_MAIN_LOG(_T("POWER ON SUCCESS\n"));
		}
		else
			WRITE_MAIN_LOG(_T("POWER ON FAILED\n"));
	}
		
#if (20230905)
	CString strTemp = _T("192.168.0.2");
	DWORD dwTesterIP;
	int nTesterPort = 4545;
	dwTesterIP = (DWORD)_ttof(strTemp);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].Connect_Common(strTemp, nTesterPort);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].SetOwnerHWND(this->m_hWnd);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].InitVariable(IP_COMMON_SCREW_LEFT);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_Communication_Start);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_DataMessageSubscribe, 15);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_DataMessageSubscribe, 1201);


	strTemp = _T("192.168.1.2");
	//DWORD dwTesterIP;
	//nTesterPort = 4545;
	dwTesterIP = (DWORD)_ttof(strTemp);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].Connect_Common(strTemp, nTesterPort);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].SetOwnerHWND(this->m_hWnd);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].InitVariable(IP_COMMON_SCREW_RIGHT);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_Communication_Start);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_DataMessageSubscribe, 15);
	COMMON->DoEvent(100);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_DataMessageSubscribe, 1201);
#endif
	Connectz();
}

void CLuriMotion_BaseDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	__super::OnWindowPosChanging(lpwndpos);

	Dlg_MovePosition();
}
