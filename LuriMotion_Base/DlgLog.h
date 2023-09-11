#pragma once
#include "pch.h"
#include "VGbase/VGStatic.h"

class CRichEditCtrlEx;

class CDlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	static CDlgLog* Get_Instance(CWnd *pParent = NULL);

	enum { IDD = IDD_DLG_LOG };

	enum LogType
	{
		Main,
		Err,
		Seq,
		Comm,
		IcsComm
	};

	static constexpr auto MAX_LOG_COUNT = 5;

	void Write_Log(LogType type, CString strLog);
	void Write_Main_Log(CString strLog);
	void Write_Err_Log(CString strLog);
	void Write_Seq_Log(CString strLog);
	void Write_Comm_Log(CString strLog);
	void Write_IcsComm_Log(CString strLog);
	void Write_Semco_Log(CString strLog);

	void ChangeUI_Language();

private:
	CDlgLog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgLog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonClearMainlog();
	afx_msg void OnBnClickedButtonClearErrlog();
	afx_msg void OnBnClickedButtonClearEtclog();
	afx_msg void OnTcnSelchangeTabEtcLog(NMHDR *pNMHDR, LRESULT *pResult);

	CRichEditCtrlEx & GetLogCtrl(LogType type);
	void ClearLog(LogType type);
	void WriteLogToFile(LogType type, CString strTime, CString strLog) const;

private:
	CVGStatic m_stLogTitle[3];
	CTabCtrl * m_pTabEtc = nullptr;

	CRichEditCtrlEx ** m_ppLogs = nullptr;

	DECLARE_MESSAGE_MAP()
};
