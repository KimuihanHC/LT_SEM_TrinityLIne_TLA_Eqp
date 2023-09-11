// DlgBottom.cpp: 구현 파일
//

#include "pch.h"

#include "Def_CompileOption.h"

#include "DlgBottom.h"
#include "LuriMotion_BaseDlg.h"
#include "afxdialogex.h"

LPCTSTR ImageNameBottom[] = { L"설정화면",	L"장비설정",	L"io",	L"event",	L"장비이력",	L"history",	L"",	L"quit" };
//LPCTSTR ImageNameBottom[] = { L"설정화면",	L"장비설정",	L"io",	L"event",	L"alarm_red",	L"recipe",	L"history",	L"quit" };

// CDlgBottom 대화 상자

IMPLEMENT_DYNAMIC(CDlgBottom, CDialogEx)

CDlgBottom::CDlgBottom(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_BOTTOM, pParent)
{
	m_nCurrentSelectDlgNum = 1;
}

CDlgBottom::~CDlgBottom()
{
}

void CDlgBottom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TEST1, m_btnNavigationMenu[0]);
	DDX_Control(pDX, IDC_BTN_TEST2, m_btnNavigationMenu[1]);
	DDX_Control(pDX, IDC_BTN_TEST3, m_btnNavigationMenu[2]);
	DDX_Control(pDX, IDC_BTN_TEST4, m_btnNavigationMenu[3]);
	DDX_Control(pDX, IDC_BTN_TEST5, m_btnNavigationMenu[4]);
	DDX_Control(pDX, IDC_BTN_TEST6, m_btnNavigationMenu[5]);
	DDX_Control(pDX, IDC_BTN_TEST7, m_btnNavigationMenu[6]);
	DDX_Control(pDX, IDC_BTN_TEST8, m_btnNavigationMenu[7]);
	
}


BEGIN_MESSAGE_MAP(CDlgBottom, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TEST1, &CDlgBottom::OnBnClickedBtnTest1)
	ON_BN_CLICKED(IDC_BTN_TEST2, &CDlgBottom::OnBnClickedBtnTest2)
	ON_BN_CLICKED(IDC_BTN_TEST3, &CDlgBottom::OnBnClickedBtnTest3)
	ON_BN_CLICKED(IDC_BTN_TEST4, &CDlgBottom::OnBnClickedBtnTest4)
	ON_BN_CLICKED(IDC_BTN_TEST5, &CDlgBottom::OnBnClickedBtnTest5)
	ON_BN_CLICKED(IDC_BTN_TEST6, &CDlgBottom::OnBnClickedBtnTest6)
	ON_BN_CLICKED(IDC_BTN_TEST7, &CDlgBottom::OnBnClickedBtnTest7)
	ON_BN_CLICKED(IDC_BTN_TEST8, &CDlgBottom::OnBnClickedBtnTest8)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_INPUT_SOCKET, &CDlgBottom::OnBnClickedButtonInputSocket)
	ON_BN_CLICKED(IDC_BUTTON_DRYRUN, &CDlgBottom::OnBnClickedButtonDryrun)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_BYPAS_SOCKET, &CDlgBottom::OnBnClickedButtonInputBypasSocket)
END_MESSAGE_MAP()

BOOL CDlgBottom::PreTranslateMessage(MSG* pMsg)
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

void CDlgBottom::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.4]
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}
// CDlgBottom 메시지 처리기


BOOL CDlgBottom::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(64, 128, 128));		// SkyBlue
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

int CDlgBottom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	

	return 0;
}
void CDlgBottom::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{

	}
}
BOOL CDlgBottom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_Bottom();
	ChangeButtonState(1);
		
#ifdef DevMode_Simulator
	GetDlgItem(IDC_BUTTON_INPUT_SOCKET)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_BUTTON_INPUT_SOCKET)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgBottom::InitComponent_Bottom()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	int nGap = 15;
	CRect rt;
	CRect rt_Move;
	
	m_btnNavigationMenu[0].GetWindowRect(rt);

	int nStart_X = 0;
	int nStart_Y = 20;

	int nResult_X = 0;
	int nResult_Y = 0;

	int nWidth = rt.Width();
	for (int i = 0; i < 7; i++)
	{
		nResult_X = nStart_X + nGap + ( (nWidth *2) * i ) + (nGap*i);
		nResult_Y = nStart_Y;

		rt_Move.left = nResult_X + nGap;
		rt_Move.top = nResult_Y;
		rt_Move.right = nResult_X + (nWidth *2);
		rt_Move.bottom = nResult_Y + (rt.Height());
		
		m_btnNavigationMenu[i].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	}

	rt_Move.left = 1724;
	rt_Move.top = nResult_Y;
	rt_Move.right = 1724 + (rt.Width() * 2);
	rt_Move.bottom = nResult_Y + (rt.Height());

	m_btnNavigationMenu[7].MoveWindow(rt_Move, SWP_SHOWWINDOW);
	
	for (int i = 0; i < eMenu_Max; i++)
	{
		m_btnNavigationMenu[i].EnableWindowsTheming(FALSE);
		//m_btnNavigationMenu[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnNavigationMenu[i].m_bTransparent = false;
		m_btnNavigationMenu[i].SetTextColor(COLOR_BLACK);
		m_btnNavigationMenu[i].SetFaceColor(COLOR_SLATEGRAY1, true);

		if( stBasic.nLanguage == 0 )
			m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuKor[i]);
		else
			m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuEng[i]);
	}

	// Load image
	CString	strExePath = L"";
	CString strImgPath = L"";
	BOOL b;

	strExePath.Format(_T("%s"), COMMON->GetProgramPath());

	for (int i = 0; i < eMenu_Max; i++)
	{
		// Normal
		strImgPath.Format(_T("%s\\%s\\%s\\%s.bmp"), strExePath, _PROGRAM_IMAGE_FOLDER, _T("Normal"), ImageNameBottom[i]);
		if (b = (COMMON->IsFileExist(strImgPath)))
			m_cBitmap[i][0].m_hObject = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		else
			m_cBitmap[i][0].m_hObject = (HBITMAP)NULL;


		// Disable
		strImgPath.Format(_T("%s\\%s\\%s\\%s.bmp"), strExePath, _PROGRAM_IMAGE_FOLDER, _T("Disable"), ImageNameBottom[i]);
		if (b = (COMMON->IsFileExist(strImgPath)))
			m_cBitmap[i][2].m_hObject = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		else
			m_cBitmap[i][2].m_hObject = (HBITMAP)NULL;
	}

	// Set image
	for (int i = 0; i < eMenu_Max; i++)
		m_btnNavigationMenu[i].SetImage((HBITMAP)m_cBitmap[i][0].m_hObject, FALSE, NULL, 0, (HBITMAP)m_cBitmap[i][2].m_hObject);
}
 void CDlgBottom::ChangeButtonState(int nNumber)
{
	 if (nNumber < 1)
		 return;

	 EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	 for (int i = 0; i < eMenu_Max; i++)
	 {
		 if (i == nNumber - 1)
		 {
			 m_btnNavigationMenu[i].SetFaceColor(COLOR_GREEN1, true);
			 if (stBasic.nLanguage == 0)
				 m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuKor[i]);
			 else
				 m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuEng[i]);
		 }
		 else
		 {
			 m_btnNavigationMenu[i].SetFaceColor(COLOR_SLATEGRAY1, true);
			 if (stBasic.nLanguage == 0)
				 m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuKor[i]);
			 else
				 m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuEng[i]);
		 }
	 }
}
void CDlgBottom::OnBnClickedBtnTest1()
{
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;

	COMMON->m_nUICheck = 1;
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	m_nCurrentSelectDlgNum = 1;
	pParrentDlg->ChangeMainUi(m_nCurrentSelectDlgNum);
	ChangeButtonState(m_nCurrentSelectDlgNum);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Main_Button_Click,Start,"));
}
// [21.1017.13] Modified 
void CDlgBottom::OnBnClickedBtnTest2()
{
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;
	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;
	if (COMMON->GetOperationMode() == OperationMode::DryRun)
		return;
	COMMON->m_nUICheck = 2;
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	m_nCurrentSelectDlgNum = 2;
	pParrentDlg->ChangeMainUi(m_nCurrentSelectDlgNum);
	ChangeButtonState(m_nCurrentSelectDlgNum);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Maintenance_Button_Click,Start,"));
}
// [21.1017.14] Modified 
void CDlgBottom::OnBnClickedBtnTest3()
{
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;
	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;
	if (COMMON->GetOperationMode() == OperationMode::DryRun)
		return;

	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}
	COMMON->m_nUICheck = 3;
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	m_nCurrentSelectDlgNum = 3;
	pParrentDlg->ChangeMainUi(m_nCurrentSelectDlgNum);
	ChangeButtonState(m_nCurrentSelectDlgNum);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_DIO_Button_Click,Start,"));
}
// [21.1017.15] Modified 
void CDlgBottom::OnBnClickedBtnTest4()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;
	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;

	if (COMMON->GetPermissionGrade() < LVL_MAINT)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("You don't have permission."), MODE_INFORMATION);
		return;
	}
	COMMON->m_nUICheck = 4;
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	m_nCurrentSelectDlgNum = 4;
	pParrentDlg->ChangeMainUi(m_nCurrentSelectDlgNum);
	ChangeButtonState(m_nCurrentSelectDlgNum);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Equipment_Button_Click,Start,"));
}
void CDlgBottom::OnBnClickedBtnTest5()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;
	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;
	if (COMMON->GetOperationMode() == OperationMode::DryRun)
		return;

	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	m_nCurrentSelectDlgNum = 5;
	pParrentDlg->ChangeMainUi(m_nCurrentSelectDlgNum);
	ChangeButtonState(m_nCurrentSelectDlgNum);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Log_Button_Click,Start,"));

	//if (COMMON->GetPermissionGrade() < 2)
	//{
	//	COMMON->ShowMsg_Thread(_T("권한이 없습니다."), MODE_INFORMATION);
	//	return;
	//}

	//CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	//pParrentDlg->ChangeMainUi(5);

	//ChangeButtonState(5);

	//m_nCurrentSelectDlgNum = 5;
}
void CDlgBottom::OnBnClickedBtnTest6()
{
	// Hide
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	if (COMMON->GetBlockUIFlag() == TRUE)
		return;

	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;
	if (COMMON->GetOperationMode() == OperationMode::DryRun)
		return;


	pParrentDlg->Hide_Program();
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Hide_Button_Click,Start,"));

	//ChangeButtonState(6);

	//m_nCurrentSelectDlgNum = 6;

	//Recipe_Load();

	//// 각각의 다이얼로그에서 레시피를 변경하면 UI가 변경되지 않는다.
	//// 레시피 변경시 UI를 같이 변경시키기 위해 데이터를 다시 로드 하도록 하자
	//CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	//pParrentDlg->RecipeChange_Ui_Refresh();
}

void CDlgBottom::OnBnClickedBtnTest7()
{
	// Hide
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	pParrentDlg->Hide_Program();
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Hide_Button_Click,Start,"));
}

void CDlgBottom::OnBnClickedBtnTest8()
{
	// PROGRAM EXIT
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetOperationMode() == OperationMode::Auto)
		return;
	if (COMMON->GetOperationMode() == OperationMode::DryRun)
		return;

	int nState = -1;
	if(stBasic.nLanguage ==0)
		nState = AfxMessageBox(_T("프로그램을 종료하시겠습니까?"), MB_YESNO);
	else
		nState = AfxMessageBox(_T("Do you want to exit this program?"), MB_YESNO);
	WRITE_SEMCO_LOG(_T("M,-,-,Run,EVENT:Bottom_Exit_Button_Click,Start,"));
	if(nState == IDYES)pParrentDlg->OnClose();
}

void CDlgBottom::ChangeUi_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 7; i++)
	{
		m_btnNavigationMenu[i].SetTextColor(COLOR_BLACK);
		m_btnNavigationMenu[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0)
			m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuKor[i]);
		else
			m_btnNavigationMenu[i].SetWindowText(m_szBottomMenuEng[i]);
	}

	ChangeButtonState(m_nCurrentSelectDlgNum);
}


void CDlgBottom::Recipe_Load()
{
	EQ_BASIC_PARAM	stEq_Cws = *DATAMANAGER->GetCurrentEqBasicData();
	if (COMMON->GetPermissionGrade() == 0)
	{
		if(stEq_Cws.nLanguage ==0)
			COMMON->ShowMsgDlg_Domodal(_T("Admin 권한이 필요합니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("Admin rights are required.."), MODE_INFORMATION);
		return;
	}


	int nIndex = 0;
	int nLength = 0;
	CString strLog = _T("");
	CString strPath = _T("");
	CString strModelName = _T("");

	CFileDialog fileDlg(TRUE, L"*.Mod", L"*.Mod", OFN_READONLY,
		_T("Handler Model File (*.Mod)|*.Mod| All File (*.*)|*.*|"));

	strPath.Format(_T("%s"), _PROGRAM_RECIPE_DIR);
	fileDlg.m_ofn.lpstrInitialDir = strPath;

	if (fileDlg.DoModal() == IDOK)
	{
		//Model file 경로
		strPath = fileDlg.GetPathName();

		nLength = strPath.GetLength();
		strPath.Delete(nLength - 4, 4);

		//모델 명
		nLength = strPath.GetLength();
		nIndex = strPath.ReverseFind('\\');
		strModelName = strPath.Mid(nIndex + 1, nLength - nIndex - 1);
		strPath.Delete(nIndex, nLength - nIndex);

		// 변경하기전에 마지막으로 불러온 레시피를 저장하자.
		stEq_Cws.strLastRecipe = strModelName;
		DATAMANAGER->SetEqData(stEq_Cws);

		// UI 변경용 FLAG
		COMMON->m_bRecipeChange = TRUE;

		// 레시피 변경.
		DATAMANAGER->RecipeChange(strModelName);	
	}
}

void CDlgBottom::OnBnClickedButtonInputSocket()
{
// 	auto * p = SEQUENCE->GetPtrSeqConveyor1();
// 	p->InputSocket();

// 	auto * p = SEQUENCE->GetPtrSeqConveyor2();
// 
// 	while (1)
// 	{
// 		auto * p = SEQUENCE->GetPtrSeqConveyor1();
// 
// 		if(p->IsInputSocket() == false)
// 			p->InputSocket();		
// 		
// 		COMMON->DoEvent(500);
// 	}
}

void CDlgBottom::OnBnClickedButtonDryrun()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	CString szMsg;
	CString szLightOnOff;

	int nLightIdx = 0;
	
	szMsg = _T("$T_CONTROL[BLU=1, ON]#");
	m_UdpTester[0].ParsingData_HandlerToTester(szMsg);

//	auto * p = SEQUENCE->GetPtrSeqConveyor2();

// 	while (1)
// 	{
// 		auto * p = SEQUENCE->GetPtrSeqConveyor1();
// 				
// 		p->InputSocket();		
// 		
// 		COMMON->DoEvent(250);
// 	}
}

void CDlgBottom::OnBnClickedButtonInputBypasSocket()
{
// 	auto * p = SEQUENCE->GetPtrSeqConveyor1();
// 	ST_SocketInfo socket;
// 
// 	socket.szRFID = _T("BYPASS_RFID");
// 	socket.szBCR = _T("22222");
// 	socket.bBypass = true;
// 	p->InputSocket(socket);	
//	SEQUENCE->Reset_SeqFlags();

	auto * p = SEQUENCE->GetPtrSeqPickerMgr();
	p->SetWorkEnd();
}

INT32 CDlgBottom::GetSelectDlgNum()
{
	return(m_nCurrentSelectDlgNum);
}