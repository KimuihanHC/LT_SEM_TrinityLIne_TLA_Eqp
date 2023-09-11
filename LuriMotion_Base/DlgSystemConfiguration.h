#pragma once

#include "VGbase/VGStatic.h"

// CDlgSystemConfiguration 대화 상자

class CDlgSystemConfiguration : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSystemConfiguration)

public:
	CDlgSystemConfiguration(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSystemConfiguration();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SYSTEM_CONFIGURATION };
#endif

public:
	void SaveConfiguration();
	void ChangeUI_Language();
	void Load_ComponentDlg();

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnNMDblclkListCommDelaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnChangeWaitTime();

	void InitComponent_Component();
	void ChangeUI_System();
	void ChangeUI_WaitList();

private:
	CVGStatic			m_stComponent_Title[2];					// Title
	CVGStatic			m_stComponent_SubTitle[4];				// Sub Title

	CMFCEditBrowseCtrl	m_MFCEditBrowse_SystemFolderPath;
	CEdit				m_edComponent_EQPName;
	CEdit				m_edComponent_SleepModeTime;

	CListCtrl			m_ListCommDelay;
	int					m_nSelectColumn = -1;

	DECLARE_MESSAGE_MAP()
};
