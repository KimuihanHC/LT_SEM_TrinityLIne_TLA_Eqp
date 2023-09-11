// DlgLog.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgLog.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "RichEditCtrlEx.h"

// CDlgLog 대화 상자

IMPLEMENT_DYNAMIC(CDlgLog, CDialogEx)

CDlgLog::CDlgLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOG, pParent)
{
	m_pTabEtc = new CTabCtrl;

	m_ppLogs = new CRichEditCtrlEx*[MAX_LOG_COUNT];

	for (auto count = 0; count < MAX_LOG_COUNT; count++)
		m_ppLogs[count] = new CRichEditCtrlEx;
}

CDlgLog::~CDlgLog()
{
	delete m_pTabEtc;

	for (auto count = 0; count < MAX_LOG_COUNT; count++)
		delete m_ppLogs[count];

	delete[] m_ppLogs;
}

CDlgLog* CDlgLog::Get_Instance(CWnd *pParent /*= NULL*/)
{
	static CDlgLog instance;
	return &instance;
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_ETC_LOG, *m_pTabEtc);

	DDX_Control(pDX, IDC_RICHEDIT_MAIN_LOG, GetLogCtrl(Main));
	DDX_Control(pDX, IDC_RICHEDIT_ERR_LOG, GetLogCtrl(Err));
	DDX_Control(pDX, IDC_RICHEDIT_SEQ_LOG, GetLogCtrl(Seq));
	DDX_Control(pDX, IDC_RICHEDIT_COMM_LOG, GetLogCtrl(Comm));
	DDX_Control(pDX, IDC_RICHEDIT_ICSCOMM_LOG, GetLogCtrl(IcsComm));

	DDX_Control(pDX, IDC_STC_MAIN_LOG, m_stLogTitle[0]);
	DDX_Control(pDX, IDC_STC_ERR_LOG, m_stLogTitle[1]);
	DDX_Control(pDX, IDC_STC_ETC_LOG, m_stLogTitle[2]);
}

BEGIN_MESSAGE_MAP(CDlgLog, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_MAIN_LOG, &CDlgLog::OnBnClickedButtonClearMainlog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ERR_LOG, &CDlgLog::OnBnClickedButtonClearErrlog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ETC_LOG, &CDlgLog::OnBnClickedButtonClearEtclog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ETC_LOG, &CDlgLog::OnTcnSelchangeTabEtcLog)
END_MESSAGE_MAP()

BOOL CDlgLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (auto index = 0; index < MAX_LOG_COUNT; index++)
	{
		auto logType = static_cast<LogType>(index);
		auto & logCtrl = GetLogCtrl(logType);

		Write_Log(logType, _T("Program Start..."));
	}

	static constexpr LPCTSTR LOG_NAME[] =
	{
		_T("Main Log"),
		_T("Err Log"),
		_T("Sequence Log")
	};

	for (int i = 0; i < 3; i++)
	{
		m_stLogTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stLogTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stLogTitle[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stLogTitle[i].SetText(LOG_NAME[i]);
	}

	m_pTabEtc->InsertItem(0, _T("Sequence Log"));
	m_pTabEtc->InsertItem(1, _T("Comm Log"));
	m_pTabEtc->InsertItem(2, _T("Ics Comm Log"));

	m_pTabEtc->SetCurSel(0);

	return TRUE;
}

BOOL CDlgLog::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgLog::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));

	return TRUE;
}

void CDlgLog::Write_Log(LogType type, CString strLog)
{
	auto & logCtrl = GetLogCtrl(type);

	CString strTime;

	SYSTEMTIME tm;
	GetLocalTime(&tm);
	strTime.Format(_T("[%02d:%02d:%02d.%03d]"), tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	CString strTimeLog;
	strTimeLog.Format(_T("%s %s\r\n"), strTime, strLog);

	if (logCtrl.GetLineCount() > 5000)
	{
		logCtrl.SetSel(0, logCtrl.GetLineCount());
		logCtrl.ReplaceSel(_T(""));
	}
		
	logCtrl.SetSel(-1, -1);
	logCtrl.ReplaceSel(strTimeLog);

	WriteLogToFile(type, strTime, strLog);
}

void CDlgLog::Write_Main_Log(CString strLog)
{
	Write_Log(Main, strLog);
}

void CDlgLog::Write_Err_Log(CString strLog)
{
	Write_Log(Err, strLog);
}

void CDlgLog::Write_Seq_Log(CString strLog)
{
	Write_Log(Seq, strLog);
}

void CDlgLog::Write_Comm_Log(CString strLog)
{
	Write_Log(Comm, strLog);
}

void CDlgLog::Write_IcsComm_Log(CString strLog)
{
	Write_Log(IcsComm, strLog);
}

void CDlgLog::Write_Semco_Log(CString strLog)
{
	CString str;
	CString strTime;
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	WRITE_SEMCO_FILE(strTime, strLog);
}

void CDlgLog::ChangeUI_Language()
{
}

void CDlgLog::OnBnClickedButtonClearMainlog()
{
	ClearLog(Main);
}

void CDlgLog::OnBnClickedButtonClearErrlog()
{
	ClearLog(Err);
}

void CDlgLog::OnBnClickedButtonClearEtclog()
{
	UpdateData(TRUE);

	auto selectTab = m_pTabEtc->GetCurSel();

	switch (selectTab)
	{
	case 0:
		ClearLog(Seq);
		break;
	case 1:
		ClearLog(Comm);
		break;
	case 2:
		ClearLog(IcsComm);
		break;
	}
}

void CDlgLog::OnTcnSelchangeTabEtcLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	*pResult = 0;

	if (pNMHDR->idFrom != IDC_TAB_ETC_LOG)
		return;

	auto selectTab = m_pTabEtc->GetCurSel();

	bool showSeqLog = false, showCommLog = false, showIcsCommLog = false;

	switch (selectTab)
	{
	case 0:
		showSeqLog = true;
		m_stLogTitle[2].SetText(_T("Sequence Log"));
		break;
	case 1:
		showCommLog = true;
		m_stLogTitle[2].SetText(_T("Comm Log"));
		break;
	case 2:
		showIcsCommLog = true;
		m_stLogTitle[2].SetText(_T("Ics Comm Log"));
		break;
	}

	GetLogCtrl(Seq).ShowWindow(showSeqLog ? SW_SHOW : SW_HIDE);
	GetLogCtrl(Comm).ShowWindow(showCommLog ? SW_SHOW : SW_HIDE);
	GetLogCtrl(IcsComm).ShowWindow(showIcsCommLog ? SW_SHOW : SW_HIDE);
}

CRichEditCtrlEx & CDlgLog::GetLogCtrl(LogType type)
{
	return *m_ppLogs[type];
}

void CDlgLog::ClearLog(LogType type)
{
// 	if(type == Comm)
// 		CDlgMain::Get_Instance()->ChangeMainUIFlowStatus(2, 0, 1);

	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	if (COMMON->GetPermissionGrade() < 1)
	{
		if (stBasic.nLanguage == 0)
			COMMON->ShowMsg_Thread(_T("권한이 없습니다."), MODE_INFORMATION);
		else
			COMMON->ShowMsg_Thread(_T("You don't have permission."), MODE_INFORMATION);

		return;
	}

	CString strMsg;

	if (stBasic.nLanguage == 0)
		strMsg.Format(_T("로그를 지우시겠습니까?"));
	else
		strMsg.Format(_T("Do you want to clear the log?"));

	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES)
		return;

	auto & logCtrl = GetLogCtrl(type);

	logCtrl.EraseAll();
}

void CDlgLog::WriteLogToFile(LogType type, CString strTime, CString strLog) const
{
	CString sz;
	switch (type)
	{
	case CDlgLog::Main:
	{
		sz += _SYSTEM_LOG;
		sz += strLog;
		WRITE_SYSTEM_FILE(strTime, sz);
	}
		break;
	case CDlgLog::Err:
		sz += _ERROR_LOG;
		sz += strLog;
		WRITE_SYSTEM_FILE(strTime, sz);
		break;
	case CDlgLog::Seq:
	{
		sz += _SEQ_LOG;
		sz += strLog;
		WRITE_SEQ_FILE(strTime, sz);
	}		
		break;
	case CDlgLog::Comm:
	{
		sz += _COMM_LOG;
		sz += strLog;
		WRITE_COM_FILE(strTime, sz);
	}		
		break;
	case CDlgLog::IcsComm:
		//WRITE_COM_FILE(strTime, _COMM_LOG, strLog);
		break;
	}
}
