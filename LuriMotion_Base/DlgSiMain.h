#pragma once
#include "pch.h"
#include "VGbase/VGStatic.h"

class CDlgSiView;
// CDlgSiMain 대화 상자

class CDlgSiMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSiMain)

public:
	CDlgSiMain( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDlgSiMain(CWnd* pParent , CRect rt);   // 표준 생성자입니다.
	virtual ~CDlgSiMain();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_SI_MAIN };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void InitComponent_SiMain(); 

private:
	CRect			m_rt;
	CVGStatic		m_stTitle;
	CDlgSiView		*m_pSiView[eSPL_MAX];

	SOCKET_DATA		m_SiData;


public:
	void SetPos_SiView	(UINT nID, int x, int y, BOOL bShow = TRUE);
	void SetName_SiView	(UINT nID, CString str);
	void SetData_SiView	(UINT nID, SOCKET_DATA *pAX);

	void UpdateStatus();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
