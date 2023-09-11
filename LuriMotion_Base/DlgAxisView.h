#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGBase/VGStatic.h"

// CDlgAxisView 대화 상자

class CDlgAxisView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAxisView)

public:
	CDlgAxisView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgAxisView();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_AXIS_VIEW };
	enum EStt { eSttName, eSttOrigin ,eSttPos ,eSttServo ,eSttAlarm ,eStt_Max};
	enum EAX { eOrigin, eServo, eAlarm, eAX_Max };

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

	afx_msg void InitComponent_AxView();

public :
	//void Init();
	void PrepareForDestroy();

	void SetText(int offset, CString text);
	void SetData(AXIS_DATA *pAX);

	void UpdateOrigin();
	void UpdateCurPos();
	void UpdateServo();
	void UpdateAlarm();
	void UpdateSensor();

private:
	CVGStatic	m_Stt[eStt_Max];

	CString m_strPos;
	CString m_strPosBk;

	BOOL m_bAxis[eAX_Max];
	BOOL m_bAxisBk[eAX_Max];
};
