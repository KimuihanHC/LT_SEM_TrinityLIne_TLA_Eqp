// DlgConfiguration.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgConfiguration.h"
#include "afxdialogex.h"
#include "DlgSystemConfiguration.h"
#include "DlgCommConfiguration.h"
#include "DlgDeviceConfiguration.h"
#include "DlgSemcoComTest.h"


// CDlgConfiguration 대화 상자

IMPLEMENT_DYNAMIC(CDlgConfiguration, CDialogEx)

CDlgConfiguration::CDlgConfiguration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CONFIGURATION, pParent)
{
	m_pSystemConfig = new CDlgSystemConfiguration;
	m_pCommConfig = new CDlgCommConfiguration;
	m_pDeviceConfig = new CDlgDeviceConfiguration;
	m_pSemcoComTest = new CDlgSemcoComTest;
}

CDlgConfiguration::~CDlgConfiguration()
{
	delete m_pSystemConfig;
	delete m_pCommConfig;
	delete m_pDeviceConfig;
	delete m_pSemcoComTest;
}

void CDlgConfiguration::Load_ComponentDlg()
{
	m_pSystemConfig->Load_ComponentDlg();
	m_pCommConfig->Load_ComponentDlg();
	m_pDeviceConfig->Load_ComponentDlg();
}

void CDlgConfiguration::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (stBasic.nLanguage == 0)
		m_btnSaveConfiguration.SetWindowText(_T("저장"));
	else
		m_btnSaveConfiguration.SetWindowText(_T("Save"));

	for (int i = 0; i < 4; i++)
	{
		if (stBasic.nLanguage == 0)
			m_btnConfiguration[i].SetWindowText(m_szConfigurationButton_Kor[i]);
		else
			m_btnConfiguration[i].SetWindowText(m_szConfigurationButton_Eng[i]);
	}

	m_pSystemConfig->ChangeUI_Language();
	m_pCommConfig->ChangeUI_Language();
	m_pDeviceConfig->ChangeUI_Language();
}

BOOL CDlgConfiguration::OnInitDialog()
{
	__super::OnInitDialog();

	InitButton();
	InitConfigDlg();
	ChangeUI_Language();

	EqpTypeShowControl();

	Load_ComponentDlg();
	return TRUE;
}

BOOL CDlgConfiguration::DestroyWindow()
{
	m_pSystemConfig->DestroyWindow();
	m_pCommConfig->DestroyWindow();
	m_pDeviceConfig->DestroyWindow();
	m_pSemcoComTest->DestroyWindow();

	return __super::DestroyWindow();
}

void CDlgConfiguration::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SAVE_CONFIGURATION, m_btnSaveConfiguration);
	DDX_Control(pDX, IDC_BTN_SYSTEM_CONFIGURATION, m_btnConfiguration[0]);
	DDX_Control(pDX, IDC_BTN_COMMUNICATION_CONFIGURATION, m_btnConfiguration[1]);
	DDX_Control(pDX, IDC_BTN_DEVICE_CONFIGURATION, m_btnConfiguration[2]);
	DDX_Control(pDX, IDC_BTN_SEMCO_TEST, m_btnConfiguration[3]);
}

BOOL CDlgConfiguration::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgConfiguration, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_SAVE_CONFIGURATION, &CDlgConfiguration::OnBnClickedBtnSaveConfiguration)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_CONFIGURATION, &CDlgConfiguration::OnBnClickedBtnSystemConfiguration)
	ON_BN_CLICKED(IDC_BTN_COMMUNICATION_CONFIGURATION, &CDlgConfiguration::OnBnClickedBtnCommunicationConfiguration)
	ON_BN_CLICKED(IDC_BTN_DEVICE_CONFIGURATION, &CDlgConfiguration::OnBnClickedBtnDeviceConfiguration)
	ON_BN_CLICKED(IDC_BTN_SEMCO_TEST, &CDlgConfiguration::OnBnClickedBtnSemcoTest)
END_MESSAGE_MAP()

void CDlgConfiguration::EqpTypeShowControl()
{
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 
// 		break;
// 	default:
// 		break;
// 	}
}

void CDlgConfiguration::OnBnClickedBtnSaveConfiguration()
{
	m_pSystemConfig->SaveConfiguration();
	m_pCommConfig->SaveConfiguration();
	m_pDeviceConfig->SaveConfiguration();
}

void CDlgConfiguration::OnBnClickedBtnSystemConfiguration()
{
	ChangeConfigView(enConfiguration::System);
}

void CDlgConfiguration::OnBnClickedBtnCommunicationConfiguration()
{
	ChangeConfigView(enConfiguration::Communication);
}

void CDlgConfiguration::OnBnClickedBtnDeviceConfiguration()
{
	ChangeConfigView(enConfiguration::Device);
}

void CDlgConfiguration::OnBnClickedBtnSemcoTest()
{
	ChangeConfigView(enConfiguration::SEMCOTest);
}

void CDlgConfiguration::InitButton()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_btnSaveConfiguration.EnableWindowsTheming(FALSE);
	m_btnSaveConfiguration.m_bTransparent = false;
	m_btnSaveConfiguration.SetTextColor(COLOR_BLACK);
	m_btnSaveConfiguration.SetFaceColor(COLOR_SLATEGRAY1, true);

	if (stBasic.nLanguage == 0)
		m_btnSaveConfiguration.SetWindowText(_T("저장"));
	else
		m_btnSaveConfiguration.SetWindowText(_T("Save"));

	for (int i = 0; i < 4; i++)
	{
		m_btnConfiguration[i].EnableWindowsTheming(FALSE);
		m_btnConfiguration[i].m_bTransparent = false;
		m_btnConfiguration[i].SetTextColor(COLOR_BLACK);
		m_btnConfiguration[i].SetFaceColor(COLOR_SLATEGRAY1, true);

		if (stBasic.nLanguage == 0)
			m_btnConfiguration[i].SetWindowText(m_szConfigurationButton_Kor[i]);
		else
			m_btnConfiguration[i].SetWindowText(m_szConfigurationButton_Eng[i]);
	}

	// Load image
	CString	strExePath = L"";
	CString strImgPath = L"";

	strExePath.Format(_T("%s"), COMMON->GetProgramPath());
	strImgPath.Format(_T("%s\\%s\\%s\\%s.bmp"), strExePath, _PROGRAM_IMAGE_FOLDER, _T("Normal"), _T("Save"));

	if (COMMON->IsFileExist(strImgPath))
	{
		m_bitmapSave.m_hObject = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		m_btnSaveConfiguration.SetImage(m_bitmapSave, FALSE, NULL, 0);
	}
	else
		m_bitmapSave.m_hObject = (HBITMAP)NULL;
}

void CDlgConfiguration::InitConfigDlg()
{
	m_pSystemConfig->Create(IDD_DLG_SYSTEM_CONFIGURATION, this);
	m_pCommConfig->Create(IDD_DLG_COMMUNICATION_CONFIGURATION, this);
	m_pDeviceConfig->Create(IDD_DLG_DEVICE_CONFIGURATION, this);
	m_pSemcoComTest->Create(IDD_DLG_SEMCO_TEST, this);

	CRect rect;
	GetDlgItem(IDC_STATIC_INSPECTOR_CONFIGURATION)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pSystemConfig->MoveWindow(&rect);
	m_pCommConfig->MoveWindow(&rect);
	m_pDeviceConfig->MoveWindow(&rect);
	m_pSemcoComTest->MoveWindow(&rect);

	ChangeConfigView(enConfiguration::System);
}

void CDlgConfiguration::ChangeConfigView(enConfiguration config)
{
	bool viewConfig[4];

	for (auto i = 0; i < 4; i++)
	{
		viewConfig[i] = (config == static_cast<enConfiguration>(i) ? true : false);

		if (viewConfig[i])
			m_btnConfiguration[i].SetFaceColor(COLOR_GREEN1, true);
		else
			m_btnConfiguration[i].SetFaceColor(RGB(225, 225, 225), true);
	}

	m_pSystemConfig->ShowWindow(viewConfig[static_cast<int>(enConfiguration::System)] ? SW_SHOW : SW_HIDE);
	m_pCommConfig->ShowWindow(viewConfig[static_cast<int>(enConfiguration::Communication)] ? SW_SHOW : SW_HIDE);
	m_pDeviceConfig->ShowWindow(viewConfig[static_cast<int>(enConfiguration::Device)] ? SW_SHOW : SW_HIDE);
	m_pSemcoComTest->ShowWindow(viewConfig[static_cast<int>(enConfiguration::SEMCOTest)] ? SW_SHOW : SW_HIDE);
}
