// CDlgSystemConfiguration.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSystemConfiguration.h"
#include "afxdialogex.h"


// CDlgSystemConfiguration 대화 상자

IMPLEMENT_DYNAMIC(CDlgSystemConfiguration, CDialogEx)

CDlgSystemConfiguration::CDlgSystemConfiguration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SYSTEM_CONFIGURATION, pParent)
{

}

CDlgSystemConfiguration::~CDlgSystemConfiguration()
{
}

void CDlgSystemConfiguration::SaveConfiguration()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	CString strTemp;

	m_MFCEditBrowse_SystemFolderPath.GetWindowText(strTemp);
	stTester.strSystemFolderPath = strTemp;

	m_edComponent_EQPName.GetWindowText(strTemp);
	stTester.strEQPName = strTemp;

	m_edComponent_SleepModeTime.GetWindowText(strTemp);
	stTester.nSleepModeTimer = _ttoi(strTemp);

	for (int i = 0; i < WAITLIST_COMM_MAX; i++)
	{
		strTemp = m_ListCommDelay.GetItemText(i, 1);
		stTester.m_strWaitTimeLIst[i] = strTemp;
	}

	DATAMANAGER->SetTesterCommData(stTester);
}

void CDlgSystemConfiguration::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		if (stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szSystemConfigurationTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szSystemConfigurationTitle_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		if (stBasic.nLanguage == 0)
			m_stComponent_SubTitle[i].SetText(m_szSystemConfigurationSubTitle_Kor[i]);
		else
			m_stComponent_SubTitle[i].SetText(m_szSystemConfigurationSubTitle_Eng[i]);
	}

	for (int i = 0; i < WAIT_COMM_MAX; i++)
	{
		LVCOLUMN lvCol;
		CString strName;

		lvCol.mask = LVCF_TEXT;

		if (stBasic.nLanguage == 0)
			strName = m_szComponentWaitList_Kor[i];
		else
			strName = m_szComponentWaitList_Eng[i];

		lvCol.pszText = strName.GetBuffer();
		lvCol.cchTextMax = strName.GetLength();

		m_ListCommDelay.SetColumn(i, &lvCol);
	}

	if (stBasic.nLanguage == 0)
		GetDlgItem(IDC_BTN_CHANGE_WAITTIME)->SetWindowText(_T("변경"));
	else
		GetDlgItem(IDC_BTN_CHANGE_WAITTIME)->SetWindowText(_T("Change"));
}

void CDlgSystemConfiguration::Load_ComponentDlg()
{
	ChangeUI_System();
	ChangeUI_WaitList();
}

BOOL CDlgSystemConfiguration::OnInitDialog()
{
	__super::OnInitDialog();

	InitComponent_Component();

	return TRUE;
}

void CDlgSystemConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_SYSTEM, m_stComponent_Title[0]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_COMM_DELAY, m_stComponent_Title[1]);

	DDX_Control(pDX, IDC_STC_SYSTEM_FOLDER_PATH, m_stComponent_SubTitle[0]);
	DDX_Control(pDX, IDC_STC_EQP_NAME, m_stComponent_SubTitle[1]);
	DDX_Control(pDX, IDC_STC_SLEEP_MODE_TIME, m_stComponent_SubTitle[2]);
	DDX_Control(pDX, IDC_STC_COMPONENT_SUBTITLE_COMM_DELAY, m_stComponent_SubTitle[3]);

	DDX_Control(pDX, IDC_MFCEDITBROWSE_SYSTEM_FOLDER_PATH, m_MFCEditBrowse_SystemFolderPath);
	DDX_Control(pDX, IDC_ED_EQP_NAME, m_edComponent_EQPName);
	DDX_Control(pDX, IDC_ED_SLEEP_MODE_TIME, m_edComponent_SleepModeTime);
	DDX_Control(pDX, IDC_LIST_COMM_DELAYLIST, m_ListCommDelay);
}

BOOL CDlgSystemConfiguration::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgSystemConfiguration::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

void CDlgSystemConfiguration::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		Load_ComponentDlg();
}

BEGIN_MESSAGE_MAP(CDlgSystemConfiguration, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMM_DELAYLIST, &CDlgSystemConfiguration::OnNMDblclkListCommDelaylist)
	ON_BN_CLICKED(IDC_BTN_CHANGE_WAITTIME, &CDlgSystemConfiguration::OnBnClickedBtnChangeWaitTime)
END_MESSAGE_MAP()

void CDlgSystemConfiguration::OnNMDblclkListCommDelaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelectColumn = pNMListView->iItem;

	CString sIndexValue = m_ListCommDelay.GetItemText(m_nSelectColumn, 1);
	GetDlgItem(IDC_EDIT_WAITTIME)->SetWindowText(sIndexValue);
	*pResult = 0;

	UpdateData(FALSE);
}

void CDlgSystemConfiguration::OnBnClickedBtnChangeWaitTime()
{
	if (m_nSelectColumn == -1)
		return;

	CString strTemp;
	GetDlgItemText(IDC_EDIT_WAITTIME, strTemp);
	m_ListCommDelay.SetItem(m_nSelectColumn, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	m_nSelectColumn = -1;
	UpdateData(FALSE);
}

void CDlgSystemConfiguration::InitComponent_Component()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	CRect rt;
	m_ListCommDelay.GetWindowRect(&rt);
	m_ListCommDelay.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCommDelay.DeleteAllItems();
	while (m_ListCommDelay.GetHeaderCtrl()->GetItemCount() > 0)
		m_ListCommDelay.DeleteColumn(0);

	for (int i = 0; i < 2; i++)
	{
		if (stBasic.nLanguage == 0)
			m_ListCommDelay.InsertColumn(i, m_szComponentWaitList_Kor[i], LVCFMT_CENTER, (int)rt.Width()*0.495);
		else
			m_ListCommDelay.InsertColumn(i, m_szComponentWaitList_Eng[i], LVCFMT_CENTER, (int)rt.Width()*0.495);
	}

	ChangeUI_WaitList();

	for (int i = 0; i < WAIT_COMM_MAX; i++)
	{
		if (stBasic.nLanguage == 0)
			m_ListCommDelay.InsertItem(i, m_szWaitTimeList_Kor[i]);
		else
			m_ListCommDelay.InsertItem(i, m_szWaitTimeList_Eng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_SubTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_SubTitle[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_SubTitle[i].SetFont_Gdip(L"Arial", 9.0F);
	}

	ChangeUI_Language();
}

void CDlgSystemConfiguration::ChangeUI_System()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	CString strTemp;

	m_MFCEditBrowse_SystemFolderPath.SetWindowText(stTester.strSystemFolderPath);
	m_edComponent_EQPName.SetWindowText(stTester.strEQPName);

	strTemp.Format(_T("%d"), stTester.nSleepModeTimer);
	m_edComponent_SleepModeTime.SetWindowText(strTemp);

	UpdateData(FALSE);
}

void CDlgSystemConfiguration::ChangeUI_WaitList()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int i = 0; i < WAIT_COMM_MAX; i++)
		m_ListCommDelay.SetItem(i, 1, LVIF_TEXT, stTester.m_strWaitTimeLIst[i], NULL, NULL, NULL, NULL);

	UpdateData(FALSE);
}
