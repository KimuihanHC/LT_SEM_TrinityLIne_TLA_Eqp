#pragma once
#include "pch.h"
#include "VGbase/VGStatic.h"

#include "IO.h"

class CDlgAxisView;
// CDlgAxisHomeProgress 대화 상자
class CDlgAxisHomeProgress : public CDialogEx, public CIO
{
	DECLARE_DYNAMIC(CDlgAxisHomeProgress)

public:
	CDlgAxisHomeProgress( CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgAxisHomeProgress();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MOTOR_HOME_RATE };
	enum EStt { eSttInitial, eStt_Max };
	enum EBtn { eBtnInitial, eBtnStop, eBtnExit, eBtn_Max };


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
	afx_msg void InitComponent_AxisMoveHome(BYTE nAxis); //총 모터수, Z축 번호1, Z축 번호2, Z축 번호3
	afx_msg void OnBnClickedButtonMotorOriginall();
	afx_msg void OnBnClickedButtonMotorStop();
	afx_msg void OnBnClickedButtonMotorExit();

private:
	CVGStatic		m_stTitle;
	CMFCButton		m_Btn[eBtn_Max];
	CDlgAxisView	*m_pAxisView[MOTOR_MAX_COUNT];

	AXIS_DATA		m_AxisData;
	UINT			m_nFlickCount;
	int				m_nAxisZPos[3];

public:
	void SetPos_AxisView(UINT nID, int x, int y, BOOL bShow = TRUE);
	void SetName_AxisView(UINT nID, CString str);
	void SetData_AxisView(UINT nID, AXIS_DATA *pAX);

	void UpdateStatus();
};
