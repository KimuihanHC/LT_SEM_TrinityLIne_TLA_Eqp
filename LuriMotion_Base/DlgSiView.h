#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgSiView 대화 상자

class CDlgSiView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSiView)

public:
	CDlgSiView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSiView();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_SI_VIEW };
	enum EStt { eSttName, eSttSerialNo, eSttRFID , eSttCodeID, eSttExist,eStt_Max};

	enum EStr { eSerialNo, eRFID, eCodeID ,eStr_Max};

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void InitComponent_SInfoView();

public :
	void PrepareForDestroy();

	void SetText(int offset, CString text);
	void SetData(SOCKET_DATA *pData);

	void UpdateExist();
	void UpdateSerialNo();
	void UpdateRFID();
	void UpdateCodeID();

private:
	CVGStatic	m_Stt[eStt_Max];

	CString m_Str[eStr_Max];
	CString m_StrBk[eStr_Max];

	BOOL  m_bExist;
	BOOL  m_bExistBk;

};
