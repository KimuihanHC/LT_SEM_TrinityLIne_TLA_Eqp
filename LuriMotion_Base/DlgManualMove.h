#pragma once
#include "VGbase/VGStatic.h"

#include "IO.h"
#include "Motion.h"
#include "Device.h"

// CDlgManualMove 대화 상자

class CDlgMotorTeach;
class CDlgManualMove : public CDialogEx, public CMotion, public CDevice
{
	DECLARE_DYNAMIC(CDlgManualMove)

public:
	CDlgManualMove(CWnd * pParent, CDlgMotorTeach * pMotorTeachDlg);   // 표준 생성자입니다.
	virtual ~CDlgManualMove();

	void ChangeUI_Language();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MANUAL_MOVE };
#endif

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnSocketLeft();
	afx_msg void OnBnClickedBtnSocketRight();
	afx_msg void OnStnClickedStcManualMoveBack();
	afx_msg void OnStnClickedStcManualMoveFront();
	afx_msg void OnBnClickedBtnViewMotorTeaching();
	afx_msg void OnBnClickedBtnManualMove(UINT nID);

	void InitComponent_ManualMove();
	void ChangePage(int nPage);
	void SelectSocket(InspectionPortType type);
	bool ManualMove(int nIndex);

private:
	static constexpr auto PAGE_LIST_MAX = 13;

	CVGStatic			m_stTitle[2];
	CMFCButton			m_btnPageMoveBack;
	CMFCButton			m_btnPageMoveFront;

	CVGStatic			m_stIndex[PAGE_LIST_MAX];
	CVGStatic			m_stName[PAGE_LIST_MAX];
	CMFCButton			m_btnMove[PAGE_LIST_MAX];
	CVGStatic			m_stResult[PAGE_LIST_MAX];

	CMFCButton			m_btnSocketLeft;
	CMFCButton			m_btnSocketRight;

	int					m_nPage = 0;
	InspectionPortType	m_portType = InspectionPortType::Left;

	CDlgMotorTeach *	m_pMotorTeachDlg = nullptr;

	DECLARE_MESSAGE_MAP()
};
