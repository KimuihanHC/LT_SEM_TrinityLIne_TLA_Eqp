#pragma once
#include "pch.h"
#include "resource.h"	
#include "VGbase/VGStatic.h"
#include "DlgAxisHomeProgress.h"

#include "Device.h"

// CDlgSide 대화 상자
#define TIMER_SWITCH_LAMPCHECK 100
#define TIMER_SWITCH_START 101
#define TIMER_STABLE 102

class CDlgSide : public CDialogEx, public CDevice
{
	DECLARE_DYNAMIC(CDlgSide)
private:
	static CDlgSide *m_pInstance;
public:
	CDlgSide(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSide();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SIDE };

	// [21.1018.9] Added
	enum enMenu
	{
		Menu_Auto = 0,
		Menu_Manual,
		Menu_SemiAuto,
		Menu_StartupCheck,
		Menu_Loto,
		Menu_DryRun,
		Menu_Null,
		Menu_Init,
		Menu_Max,
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);



	DECLARE_MESSAGE_MAP()


public:
	bool Begin_RunThread();
	void End_RunThread();
	BOOL		m_bThreadRun;
	CWinThread*	m_pThreadRun;
	static UINT	Thread_Run(LPVOID lpParam);
	BOOL		IsThreadRunSide() const;
	void		SetThreadRun(BOOL val);
	CWinThread* GetWinThreadRun() const;
	void		SetWinThreadRun(CWinThread * val);


public:
	static CDlgSide *Get_Instance(CWnd *pParent = NULL);
	void Delete_Instance();
public:
	afx_msg void InitComponent_Side();

	CMFCButton	m_btnSideMenu[Menu_Max];
	CBitmap		m_cBitmap[Menu_Max][3];
	afx_msg void OnBnClickedBtnSideCommand1();
	afx_msg void OnBnClickedBtnSideCommand2();
	afx_msg void OnBnClickedBtnSideCommand3();
	afx_msg void OnBnClickedBtnSideCommand4();
	afx_msg void OnBnClickedBtnSideCommand5();
	afx_msg void OnBnClickedBtnSideCommand6();
	afx_msg void OnBnClickedBtnSideCommand7();
	afx_msg void OnBnClickedBtnSideCommand8();

	afx_msg void SetSIdeBarStartButton_LampOn();			// START 와 STOP은 서로 반대쪽은 꺼지게 만들자.
	afx_msg void SetSIdeBarStopButton_LampOn();				// START 와 STOP은 서로 반대쪽은 꺼지게 만들자.
//	afx_msg void SetSideBarOutputBtn_LampOn(bool bState);

	afx_msg void SetSIdeBarIntialButton_LampOn(int nOn);
	afx_msg void SetSIdeBarSemiAutoButton_LampOn(int nOn);

	afx_msg void SetSIdeBarDryRunButton_LampOn();

	BOOL m_bFlagSampleInput_Block_Foced;
	BOOL m_bFlagSampleOutput_Block_Foced;
public:
	afx_msg void ChangeUI_Language();

	afx_msg enMCErrCode Check_Start();

	BOOL	Check_Switch_Start();
	BOOL	Check_Switch_Stop();
	BOOL	Check_Switch_Init();

	void	SetOperationMode		(__in OperationMode mode);
	void	SetMenuClickColor		(__in enMenu menu);

	void	EqpTypeShowOperation();

	__int64 m_tStartBtnPress;
	__int64 m_tStopBtnPress;
	__int64 m_tInitBtnPress;
	BOOL m_bPowerStatus;
	int m_bStartStatus;
	int m_bStartStatusBK;
	int m_bStopStatus;
	int m_bStopStatusBK;
	BOOL	m_SideStopFlag;
	BOOL	m_bStartFlag;
	BOOL	m_bStopFlag;
	BOOL	m_bInitFlag;
	BOOL	m_bStartFlagIsActive;
	BOOL	m_bStopFlagIsActive;
	BOOL	m_bInitFlagIsActive;

	int	m_nbuttonFlagIs;
	int	m_nbuttonFlagIsStop;

	int m_nStopstatus;
	int m_nStopstatusBK;
};


