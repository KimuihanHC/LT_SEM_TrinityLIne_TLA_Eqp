#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

#define BUTTON_OK						0x1
#define BUTTON_CANCEL					0x2
#define BUTTON_BUZZER_OFF				0x4
#define BUTTON_DOOR_LOCK				0x8
#define BUTTON_ALL						0x15
#define TIMER_END_DIALOG 100
// CDlgMessageBox 대화 상자

class CDlgMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox)

public:
	CDlgMessageBox( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMessageBox();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MESSAGEBOX };

	enum EBtn { eBtnOk, eBtnCancel, eBtnBuzOff, eBtnDoorLock, eBtn_Max };
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void InitComponent_MessageBox();

	afx_msg void ChangeComponent_MessageBox();
	virtual BOOL OnInitDialog();

public:
	CString			m_strError;
	int				m_nMode;

	CVGStatic		m_stTitle;
	INT				m_nButton;
	CButton			m_ctrlButton[eBtn_Max];
	void SetMsg(CString str, int nMode);

	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
	afx_msg void OnBnClickedBtn4();
};
