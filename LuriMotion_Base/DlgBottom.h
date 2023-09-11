#pragma once

#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"

// CDlgBottom 대화 상자

class CDlgBottom : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBottom)

public:
	CDlgBottom(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgBottom();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BOTTOM };

	enum enMenu
	{
		eMenu_Main = 0,
		eMenu_Teaching,
		eMenu_Dio,
		eMenu_Commset,
		eMenu_Log,
		eMenu_Hide,
		eMenu_None,
		eMenu_Exit,
		eMenu_Max,
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void InitComponent_Bottom();

	CMFCButton	m_btnNavigationMenu[eMenu_Max];
	CBitmap		m_cBitmap[eMenu_Max][3];

	afx_msg void OnBnClickedBtnTest1();
	afx_msg void OnBnClickedBtnTest2();
	afx_msg void OnBnClickedBtnTest3();
	afx_msg void OnBnClickedBtnTest4();
	afx_msg void OnBnClickedBtnTest5();
	afx_msg void OnBnClickedBtnTest6();
	afx_msg void OnBnClickedBtnTest7();
	afx_msg void OnBnClickedBtnTest8();
	afx_msg void ChangeButtonState(int nNumber);
	int		m_nCurrentSelectDlgNum;

public:
	afx_msg void ChangeUi_Language();

public:
	afx_msg void Recipe_Load();
	afx_msg void OnBnClickedButtonInputSocket();
	afx_msg void OnBnClickedButtonDryrun();
	afx_msg void OnBnClickedButtonInputBypasSocket();
	INT32 GetSelectDlgNum();
};
