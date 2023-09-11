#pragma once
#include "pch.h"
#include "resource.h"	

#include "Def_Enum.h"

#include "VGBase/VGStatic.h"
#include "DlgLog.h"
#include "DlgSocket.h"

#include "Device.h"

// CDlgMain 대화 상자
#define MAIN_TIMER_MOTOR_STATUS					200
#define MAIN_TIMER_1234							1234

#define MAIN_TIMER_SENSOR_STATUS			    201

#define MAIN_TIMER_MOTOR_POS_STATUS			    202

class CDlgMain : public CDialogEx, public CDevice
{
	DECLARE_DYNAMIC(CDlgMain)

private:
	static CDlgMain *m_pInstance;
	CCriticalSection m_CritSec;

	HANDLE m_hOpenTester;

	typedef enum 
	{
		STit_Eqp = 0,
		STit_Door,
		STit_Sensnor,
		STit_Motor,
		STit_Work,
		STit_Max
	}SI_Title;

	typedef enum 
	{
// 		SEqps_Ics = 0,
// 		SEqps_Motor,
// 		SEqps_Semco1,
// 		SEqps_Semco2,
// 		SEqps_CL200A,
// 		SEqps_RFID,
// 		SEqps_PWR,
// 		SEqps_Light1,
// 		SEqps_Light2,
// 		SEqps_Light3,
// 		SEqps_CL500A,
		SEqps_Semco1,
		SEqps_PWR,
		SEqps_Light1,
		SEqps_Light2,
		SEqps_Max,
	}SI_EqpStatus;

	typedef enum 
	{
		SIF_Door_Front = 0,
		SIF_Door_Side_Left1,
		SIF_Door_Side_Left2,
		SIF_Door_Side_Left3,
		SIF_Door_Side_Right1,
		SIF_Door_Side_Right2,
		SIF_Door_Side_Right3,
		SIF_Door_Rear1,
		SIF_Door_Rear2,
		SIF_Door_Max,
	}SI_Interface;

	typedef enum 
	{
		SIFS_InFrontReq = 0,
		SIFS_InFrontRes,
		SIFS_InRearReq,
		SIFS_InRearRes,
		SIFS_OutFrontReq,
		SIFS_OutFrontRes,
		SIFS_OutRearReq,
		SIFS_OutRearRes,
		SIFS_Max,
	}SI_IF_Status;


	typedef enum
	{
		Door_FrontOut = 0,
		Door_FrontIn,
		Door_Side_Left1_Out,
		Door_Side_Left1_In,
		Door_Side_Left2_Out,
		Door_Side_Left2_In,
		Door_Side_Left3_Out,
		Door_Side_Left3_In,
		Door_Side_Right1_Out,
		Door_Side_Right1_In,
		Door_Side_Right2_Out,
		Door_Side_Right2_In,
		Door_Side_Right3_Out,
		Door_Side_Right3_In,
		Door_Rear1_Out,
		Door_Rear1_In,
		Door_Rear2_Out,
		Door_Rear2_In,
		Door_Max,
	}SI_Door_Status;


	typedef enum
	{
		Sensor_Prodcut_Left = 0,
		Sensor_Prodcut_Right,
		Sensor_Jig_Left_Fwd,
		Sensor_Jig_Left_Bwd,
		Sensor_Jig_Left_Chk,
		Sensor_Jig_Right_Fwd,
		Sensor_Jig_Right_Bwd,
		Sensor_Jig_Right_Chk,
		Sensor_Chuck_Fwd,
		Sensor_Chuck_Bwd,
		Sensor_Max,
	}SI_Sensor_Status;

	typedef enum
	{
		Motor_Screw_L_Wait = 0,
		Motor_Screw_L_Start_Up,
		Motor_Screw_L_Work,
		Motor_Screw_R_Wait,
		Motor_Screw_R_Start_Up,
		Motor_Screw_R_Work,
		Motor_Vignetting_Wait,
		Motor_Vignetting_Test1,
		Motor_Vignetting_Test2,
		Motor_Max,
	}SI_Motor_Status;

	typedef enum
	{
		Work_LOCK = 0,
		Work_POWER,
		Work_PREVIEW,
		Work_ADJUST,
		Work_VIGNETTING,
		Work_READY,
		Work_Max,
	}SI_Work_Status;

public:
	static CDlgMain *Get_Instance(CWnd *pParent = NULL);
	void Delete_Instance();

public:
	CDlgMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange			(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage	(MSG* pMsg);
	virtual BOOL OnInitDialog			();
	virtual BOOL DestroyWindow			();
	afx_msg int OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd			(CDC* pDC);
	afx_msg void OnShowWindow			(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void OnTimer				(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	void SetOperationMode(__in OperationMode mode);

	afx_msg void InitComponent_Main();
	afx_msg void ChangeUI_Language();

	afx_msg void MainUi_Init();

	void EqpTypeShowDialog();
	void EqpTypeShowOperation();

public:
	// Timer
	BOOL	m_bMainStatus[MOTOR_MAX_COUNT];
	void	GetCurrentPosition();
	void	UpdateSocketPos();
	void	UpdateEqpStatus();
	void	UpdateIFStatus();
	void	Update_PASSNG_Status(INT On);
	

	void	UpdateMotorStatus();

	void	ReadMotorStatus_Ajin();
	void	ReadMotorStatus_Fastech();

public:
	CVGStatic			m_bnMainSemiAuto[12];					// 메인화면 SemiAuto UI의 동작중 확인 상태

public:
	//--------------------------------------------------------------------------------------
	// Tower Lamp
	afx_msg void SetTowerLamp(int nMode);
	int		m_nCurrentMode;
	int		m_nLastMode;
	BOOL	m_bIsTower_Blink;
	int		m_nBtnUiMode;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnYieldReset();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

public:
// 	void MchState_View();
// 	BOOL m_bMchState_View_1;
// 	BOOL m_bMchState_View_2;
// 	BOOL m_bMchState_View_3;

	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();

	afx_msg void OnBnClickedBtnSemiauto1();
	afx_msg void OnBnClickedBtnSemiauto2();
	afx_msg void OnBnClickedBtnSemiauto3();
	afx_msg void OnBnClickedBtnSemiauto4();

	afx_msg void OnBnClickedBtnSemiauto5();
	afx_msg void OnBnClickedBtnSemiauto6();
	afx_msg void OnBnClickedMvButton1();
	afx_msg void OnBnClickedMvButton2();
	afx_msg void OnBnClickedMvButton3();
	afx_msg void OnBnClickedButtonSemiautoCancel();

	BOOL ShowError(__in enMCErrCode err);
	BOOL ShowError(__in enMCErrCode err, __in CString szError);
	BOOL ShowInformation(__in enMCErrCode err);	
	afx_msg void OnBnClickedBtnSemiauto7();
	afx_msg void OnBnClickedBtnSemiauto9();
	afx_msg void OnBnClickedBtnSemiauto8();
	afx_msg void OnBnClickedBtnSemiauto10();
	afx_msg void OnBnClickedBtnSemiauto12();
	afx_msg void OnBnClickedBtnSemiauto11();

	void OnBtnSemiAuto_Show			(__in bool bShow);
	void OnBtnSemiAuto_Show_1	(__in bool bShow);

	
	void OnBtnSemiAuto_Enable		(__in bool bEnable);
	void OnBtnSemiAuto_Enable_1(__in bool bEnable);

	void OnStatusSemiAuto_Reset		();
	void OnStatusSemiAuto_Start		(__in InspectionPortType port, __in int nIdx);
	void OnStatusSemiAuto_End		(__in InspectionPortType port, __in int nIdx, __in bool bResultOk);

	void OnStatusStartupCheck_Reset	();
	void OnStatusStartupCheck_Start	(__in InspectionPortType port, __in int nIdx);
	void OnStatusStartupCheck_End	(__in InspectionPortType port, __in int nIdx, __in bool bResultOk);

	void OnUpdate_CL200A_Data		(__in InspectionPortType port, __in float fData, __in bool bReadOk);
	void OnUpdate_CL200A_Reset		(__in InspectionPortType port);
	void OnUpdate_CL500A_Data		(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk);
	void OnUpdate_CL500A_Reset		(__in InspectionPortType port);

	void EqpTypeGetOption			();
private:
	void InitInspectorMainDialog();
	void ReleaseInspectorMainDialog();

	CDialogEx * m_pInspectorMainDlg = nullptr;

//	CDlgSocket*	m_pDlgSocket[(int)SocketPos::Max];
//	CVGStatic		m_stcSocket[(int)SocketPos::Max];

	CVGStatic	m_stcTitle[STit_Max];
	CVGStatic	m_stcEqpStatus[SEqps_Max];
	CVGStatic	m_stDoor[SIF_Door_Max];

//	CVGStatic	m_stcIFStatus[SIFS_Max]; 

	CVGStatic	m_stcIDoor[Door_Max];
	CVGStatic	m_st_Door[Door_Max];
	INT 	m_DoorStatus[Door_Max];
	INT 	m_DoorStatusBK[Door_Max];


	CVGStatic	m_st_Sensnor[Sensor_Max];
	CVGStatic	m_st_if_Sensnor[Sensor_Max];
	CVGStatic	m_st_if2_Sensnor[Sensor_Max];
	INT 	m_SensorStatus[Sensor_Max];
	INT 	m_SensorStatusBK[Sensor_Max];


	CVGStatic	m_st_Motor[Motor_Max];
	CVGStatic	m_st_If_Motor[Motor_Max];
	CVGStatic	m_st_If2_Motor[Motor_Max];
	INT 	m_MotorStatus[Motor_Max];
	INT 	m_MotorStatusBK[Motor_Max];


	CVGStatic	m_st_Work[Work_Max];
	CVGStatic	m_st_if_Work[Work_Max];
	INT 	m_WorkStatus[Work_Max];
	INT 	m_WorkStatusBK[Work_Max];

	CVGStatic	m_st_if_NG;
	CVGStatic	m_st_if_PASS;

	CVGStatic	m_st_if_Result;
	

	INT 	m_PASS_NG_Status;
	INT 	m_PASS_NG_StatusBK;
public:
	afx_msg void OnBnClickedBtnPopupTest();	
	afx_msg void OnStnClickedStaticMainflowBmp();
};
