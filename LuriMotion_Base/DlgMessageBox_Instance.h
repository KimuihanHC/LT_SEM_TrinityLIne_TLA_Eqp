#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

#define BUTTON_OK						0x1
#define BUTTON_CANCEL					0x2
#define BUTTON_BUZZER_OFF				0x4
#define BUTTON_DOOR_LOCK				0x8
#define BUTTON_ALL						0x7
// CDlgMessageBox 대화 상자

class CDlgMessageBox_Instance : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessageBox_Instance)
private:
	static CDlgMessageBox_Instance *m_pInstance;
	CDialogEx* m_pParentWnd;

public:
	static CDlgMessageBox_Instance *Get_Instance(CWnd *pParent = NULL);
	void Delete_Instance();

public:
	CDlgMessageBox_Instance( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMessageBox_Instance();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MESSAGEBOX_INSTANCE };

	enum EBtn { eBtnOk, eBtnCancel, eBtnBuzOff, eBtnDoorLock, eBtn_Max };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
	afx_msg void OnBnClickedBtn4();

public:
	void InitComponent_MessageBox();
	void ChangeComponent_MessageBox();

public:
	CString			m_strError;
	int				m_nMode;
	INT				m_nButton;
	CButton			m_ctrlButton[eBtn_Max];
	CVGStatic		m_stTitle;

	int				m_nReturn;
	void			SetMsg(CString str, int nMode);
};
