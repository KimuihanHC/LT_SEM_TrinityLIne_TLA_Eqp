// DlgMain.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgMain.h"
#include "DlgColorCalMain.h"
#include "DlgDistortionMain.h"
#include "DlgGhostFlareMain.h"
#include "DlgHotPixelMain.h"
#include "DlgSFRNarrowMain.h"
#include "DlgSFRWideHMain.h"
#include "DlgSFRWideVMain.h"
#include "afxdialogex.h"

#include "DlgSiMain.h"

static LPCTSTR g_szTitle[] =
{
	_T("Eqp Status"),
	_T("DOOR Status"),
	_T("Sensor Status"),
	_T("Motor Status"),
	_T("Work Status"),
};

static LPCTSTR g_szEqpDevice[] =
{
// 	_T("ICS"),
// 	_T("Motor"),
	_T("Tester1"),
/*	_T("Tester2"),*/
// 	_T("CL200A"),
// 	_T("RFID"),
	_T("PWR"),
	_T("Light1"),
	_T("Light2"),
// 	_T("Light3"),
// 	_T("CL500A"),
// 	_T("ICS"),
// 	_T("Motor"),
// 	_T("Tester1"),
// 	_T("Tester2"),
// 	_T("CL200A"),
// 	_T("RFID"),
// 	_T("PWR"),
// 	_T("Light1"),
// 	_T("Light2"),
// 	_T("Light3"),
// 	_T("CL500A"),
};

static LPCTSTR g_szSIF_Door[] =
{
	_T("(1) Door Front"),
	_T("(2) Side Left 1"),
	_T("(3) Side Left 2"),
	_T("(4) Side Left 3"),	
	_T("(5) Side Right 1"),
	_T("(6) Side Right 2"),
	_T("(7) Side Right 3"),
	_T("(8) Rear 1"),
	_T("(9) Rear 2"),
};

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)
CDlgMain *CDlgMain::m_pInstance = NULL;

CDlgMain *CDlgMain::Get_Instance(CWnd *pParent)
{
	if (!m_pInstance) {
		m_pInstance = new CDlgMain(pParent);
		if (!m_pInstance->m_hWnd) {
			m_pInstance->Create(IDD_DLG_MAIN, pParent);
		}
	}
	return m_pInstance;
}

void CDlgMain::Delete_Instance()
{
	if (m_pInstance->m_hWnd) m_pInstance->DestroyWindow();
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}

CDlgMain::CDlgMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAIN, pParent)
{
	m_nBtnUiMode = 1;	

	for (int i = 0; i < Door_Max; i++)
	{
		m_DoorStatus[i] = 100;
		m_DoorStatusBK[i] = 10000;
	}
	for (int i = 0; i < Sensor_Max; i++)
	{
		m_SensorStatus[i] = TRUE;
		m_SensorStatusBK[i] = 100000;
	}

	for (int i = 0; i < Motor_Max; i++)
	{
		m_MotorStatus[i] = TRUE;
		m_MotorStatusBK[i] = 100000;
	}
	for (int i = 0; i < Work_Max; i++)
	{
		m_WorkStatus[i] = TRUE;
		m_WorkStatusBK[i] = 100000;
	}
	m_PASS_NG_Status = TRUE;
	m_PASS_NG_StatusBK = 100000;

}

CDlgMain::~CDlgMain()
{
	KillTimer(MAIN_TIMER_MOTOR_STATUS);
	KillTimer(MAIN_TIMER_SENSOR_STATUS);
	KillTimer(MAIN_TIMER_MOTOR_POS_STATUS);

}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SEMIAUTO1, m_bnMainSemiAuto[0]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO2, m_bnMainSemiAuto[1]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO3, m_bnMainSemiAuto[2]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO4, m_bnMainSemiAuto[3]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO5, m_bnMainSemiAuto[4]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO6, m_bnMainSemiAuto[5]);

	DDX_Control(pDX, IDC_BTN_SEMIAUTO7, m_bnMainSemiAuto[6]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO8, m_bnMainSemiAuto[7]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO9, m_bnMainSemiAuto[8]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO10, m_bnMainSemiAuto[9]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO11, m_bnMainSemiAuto[10]);
	DDX_Control(pDX, IDC_BTN_SEMIAUTO12, m_bnMainSemiAuto[11]);

	// 	DDX_Control(pDX, IDC_STC_CV1_RFID,		m_stcSocket[0]);
	// 	DDX_Control(pDX, IDC_STC_CV1_BUFFER,	m_stcSocket[1]);
	// 	DDX_Control(pDX, IDC_STC_CV2_L_LOADING, m_stcSocket[2]);
	// 	DDX_Control(pDX, IDC_STC_CV2_R_LOADING, m_stcSocket[3]);
	// 	DDX_Control(pDX, IDC_STC_CV2_BYPASS,	m_stcSocket[4]);
	// 	DDX_Control(pDX, IDC_STC_PIC_L_WAIT,	m_stcSocket[5]);
	// 	DDX_Control(pDX, IDC_STC_PIC_L_INSPECTION,	m_stcSocket[6]);
	// 	DDX_Control(pDX, IDC_STC_PIC_R_WAIT,		m_stcSocket[7]);
	// 	DDX_Control(pDX, IDC_STC_PIC_R_INSPECTION,	m_stcSocket[8]);
	// 	DDX_Control(pDX, IDC_STC_CVR_BYPASS,		m_stcSocket[9]);

	DDX_Control(pDX, IDC_STC_EQP_STATUS_TITLE, m_stcTitle[STit_Eqp]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_STATUS, m_stcTitle[STit_Door]);
	DDX_Control(pDX, IDC_STC_IF_SENSOR_STATUS, m_stcTitle[STit_Sensnor]);
	DDX_Control(pDX, IDC_STC_IF_MOTOR_STATUS, m_stcTitle[STit_Motor]);
	DDX_Control(pDX, IDC_STC_IF_WORK_STATUS, m_stcTitle[STit_Work]);

	//	DDX_Control(pDX, IDC_STC_EQP_ICS_STATUS,	m_stcEqpStatus[SEqps_Ics]);
	//	DDX_Control(pDX, IDC_STC_EQP_MOTOR_STATUS,	m_stcEqpStatus[SEqps_Motor]);
	DDX_Control(pDX, IDC_STC_EQP_SEMCO1_STATUS, m_stcEqpStatus[SEqps_Semco1]);
	//	DDX_Control(pDX, IDC_STC_EQP_SEMCO2_STATUS, m_stcEqpStatus[SEqps_Semco2]);
	// 	DDX_Control(pDX, IDC_STC_EQP_CL200A_STATUS, m_stcEqpStatus[SEqps_CL200A]);
	// 	DDX_Control(pDX, IDC_STC_EQP_RFID_STATUS, m_stcEqpStatus[SEqps_RFID]);
	DDX_Control(pDX, IDC_STC_EQP_POWERSUPPLY_STATUS, m_stcEqpStatus[SEqps_PWR]);
	DDX_Control(pDX, IDC_STC_EQP_LIGHT1_STATUS, m_stcEqpStatus[SEqps_Light1]);
	DDX_Control(pDX, IDC_STC_EQP_LIGHT2_STATUS, m_stcEqpStatus[SEqps_Light2]);
	// 	DDX_Control(pDX, IDC_STC_EQP_LIGHT3_STATUS, m_stcEqpStatus[SEqps_Light3]);
	// 	DDX_Control(pDX, IDC_STC_EQP_CL500A_STATUS, m_stcEqpStatus[SEqps_CL500A]);

	DDX_Control(pDX, IDC_STC_DOOR_STATUS_01, m_stDoor[SIF_Door_Front]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_02, m_stDoor[SIF_Door_Side_Left1]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_03, m_stDoor[SIF_Door_Side_Left2]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_04, m_stDoor[SIF_Door_Side_Left3]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_05, m_stDoor[SIF_Door_Side_Right1]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_06, m_stDoor[SIF_Door_Side_Right2]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_07, m_stDoor[SIF_Door_Side_Right3]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_08, m_stDoor[SIF_Door_Rear1]);
	DDX_Control(pDX, IDC_STC_DOOR_STATUS_09, m_stDoor[SIF_Door_Rear2]);


	// 	DDX_Control(pDX, IDC_STC_IF_IN_FRONT_REQ,	m_stcIFStatus[SIFS_InFrontReq]);
	// 	DDX_Control(pDX, IDC_STC_IF_IN_FRONT_RES,	m_stcIFStatus[SIFS_InFrontRes]);
	// 	DDX_Control(pDX, IDC_STC_IF_IN_REAR_REQ,	m_stcIFStatus[SIFS_InRearReq]);
	// 	DDX_Control(pDX, IDC_STC_IF_IN_REAR_RES,	m_stcIFStatus[SIFS_InRearRes]);
	// 	DDX_Control(pDX, IDC_STC_IF_OUT_FRONT_REQ,	m_stcIFStatus[SIFS_OutFrontReq]);
	// 	DDX_Control(pDX, IDC_STC_IF_OUT_FRONT_RES,	m_stcIFStatus[SIFS_OutFrontRes]);
	// 	DDX_Control(pDX, IDC_STC_IF_OUT_REAR_REQ,	m_stcIFStatus[SIFS_OutRearReq]);
	// 	DDX_Control(pDX, IDC_STC_IF_OUT_REAR_RES,	m_stcIFStatus[SIFS_OutRearRes]);	


		// 		UINT nIDAxisName[] = {
	// 	IDC_STC_IF_DOOR_FRONT, IDC_STC_IF_DOOR_FRONT1, IDB_DISPEN_AXIS_NAME_2, IDB_DISPEN_AXIS_NAME_3, IDB_DISPEN_AXIS_NAME_4,
	// 	IDB_DISPEN_AXIS_NAME_5
	// 	};
	// 	for (i = 0; i < MAX_DISPEN_AXIS; i++) {
	// 		DDX_Control(pDX, nIDAxisName[i], m_ctrlAxisName[i]);
	// 	}


	DDX_Control(pDX, IDC_STC_IF_DOOR_FRONT_OUT, m_stcIDoor[Door_FrontOut]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_FRONT_IN, m_stcIDoor[Door_FrontIn]);

	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT1_OUT, m_stcIDoor[Door_Side_Left1_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT1_IN, m_stcIDoor[Door_Side_Left1_In]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT2_OUT, m_stcIDoor[Door_Side_Left2_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT2_IN, m_stcIDoor[Door_Side_Left2_In]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT3_OUT, m_stcIDoor[Door_Side_Left3_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_LEFT3_IN, m_stcIDoor[Door_Side_Left3_In]);

	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT1_OUT, m_stcIDoor[Door_Side_Right1_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT1_IN, m_stcIDoor[Door_Side_Right1_In]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT2_OUT, m_stcIDoor[Door_Side_Right2_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT2_IN, m_stcIDoor[Door_Side_Right2_In]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT3_OUT, m_stcIDoor[Door_Side_Right3_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_SIDE_RIGHT3_IN, m_stcIDoor[Door_Side_Right3_In]);

	DDX_Control(pDX, IDC_STC_IF_DOOR_REAR1_OUT, m_stcIDoor[Door_Rear1_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_REAR1_IN, m_stcIDoor[Door_Rear1_In]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_REAR2_OUT, m_stcIDoor[Door_Rear2_Out]);
	DDX_Control(pDX, IDC_STC_IF_DOOR_REAR2_IN, m_stcIDoor[Door_Rear2_In]);


	DDX_Control(pDX, IDC_STC_IF_FRONT_ON_OFF, m_st_Door[0]);
	DDX_Control(pDX, IDC_STC_IF_FRONT_IN_OUT, m_st_Door[1]);

	DDX_Control(pDX, IDC_STC_IF_LEFT_ON_OFF_1, m_st_Door[2]);
	DDX_Control(pDX, IDC_STC_IF_LEFT_IN_OUT_1, m_st_Door[3]);
	DDX_Control(pDX, IDC_STC_IF_LEFT_ON_OFF_2, m_st_Door[4]);
	DDX_Control(pDX, IDC_STC_IF_LEFT_IN_OUT_2, m_st_Door[5]);
	DDX_Control(pDX, IDC_STC_IF_LEFT_ON_OFF_3, m_st_Door[6]);
	DDX_Control(pDX, IDC_STC_IF_LEFT_IN_OUT_3, m_st_Door[7]);

	DDX_Control(pDX, IDC_STC_IF_RIGHT_ON_OFF_1, m_st_Door[8]);
	DDX_Control(pDX, IDC_STC_IF_RIGHT_IN_OUT_1, m_st_Door[9]);
	DDX_Control(pDX, IDC_STC_IF_RIGHT_ON_OFF_2, m_st_Door[10]);
	DDX_Control(pDX, IDC_STC_IF_RIGHT_IN_OUT_2, m_st_Door[11]);
	DDX_Control(pDX, IDC_STC_IF_RIGHT_ON_OFF_4, m_st_Door[12]);
	DDX_Control(pDX, IDC_STC_IF_RIGHT_IN_OUT_4, m_st_Door[13]);

	DDX_Control(pDX, IDC_STC_IF_REAR_ON_OFF_1, m_st_Door[14]);
	DDX_Control(pDX, IDC_STC_IF_REAR_ON_OFF_1, m_st_Door[14]);
	DDX_Control(pDX, IDC_STC_IF_REAR_IN_OUT_1, m_st_Door[15]);
	DDX_Control(pDX, IDC_STC_IF_REAR_ON_OFF_2, m_st_Door[16]);
	DDX_Control(pDX, IDC_STC_IF_REAR_IN_OUT_2, m_st_Door[17]);

	DDX_Control(pDX, IDC_STC_IF_PRODUCT_1, m_st_if_Sensnor[Sensor_Prodcut_Left]);
	DDX_Control(pDX, IDC_STC_IF_PRODUCT_2, m_st_if_Sensnor[Sensor_Prodcut_Right]);
	DDX_Control(pDX, IDC_STC_IF_JIG_LEFT_FWD, m_st_if_Sensnor[Sensor_Jig_Left_Fwd]);
	DDX_Control(pDX, IDC_STC_IF_JIG_LEFT_BWD, m_st_if_Sensnor[Sensor_Jig_Left_Bwd]);
	DDX_Control(pDX, IDC_STC_IF_JIG_LEFT_CHK, m_st_if_Sensnor[Sensor_Jig_Left_Chk]);
	DDX_Control(pDX, IDC_STC_IF_JIG_RIGHT_FWD, m_st_if_Sensnor[Sensor_Jig_Right_Fwd]);
	DDX_Control(pDX, IDC_STC_IF_JIG_RIGHT_BWD, m_st_if_Sensnor[Sensor_Jig_Right_Bwd]);
	DDX_Control(pDX, IDC_STC_IF_JIG_RIGHT_CHK, m_st_if_Sensnor[Sensor_Jig_Right_Chk]);
	DDX_Control(pDX, IDC_STC_IF_CHUCK_FWD, m_st_if_Sensnor[Sensor_Chuck_Fwd]);
	DDX_Control(pDX, IDC_STC_IF_CHUCK_BWD, m_st_if_Sensnor[Sensor_Chuck_Bwd]);

	DDX_Control(pDX, IDC_STC_PRODUCT_1, m_st_Sensnor[Sensor_Prodcut_Left]);
	DDX_Control(pDX, IDC_STC_PRODUCT_2, m_st_Sensnor[Sensor_Prodcut_Right]);
	DDX_Control(pDX, IDC_STC_JIG_LEFT_FWD, m_st_Sensnor[Sensor_Jig_Left_Fwd]);
	DDX_Control(pDX, IDC_STC_JIG_LEFT_BWD, m_st_Sensnor[Sensor_Jig_Left_Bwd]);
	DDX_Control(pDX, IDC_STC_JIG_LEFT_CHK, m_st_Sensnor[Sensor_Jig_Left_Chk]);
	DDX_Control(pDX, IDC_STC_JIG_RIGHT_FWD, m_st_Sensnor[Sensor_Jig_Right_Fwd]);
	DDX_Control(pDX, IDC_STC_JIG_RIGHT_BWD, m_st_Sensnor[Sensor_Jig_Right_Bwd]);
	DDX_Control(pDX, IDC_STC_JIG_RIGHT_CHK, m_st_Sensnor[Sensor_Jig_Right_Chk]);
	DDX_Control(pDX, IDC_STC_CHUCK_FWD, m_st_Sensnor[Sensor_Chuck_Fwd]);
	DDX_Control(pDX, IDC_STC_CHUCK_BWD, m_st_Sensnor[Sensor_Chuck_Bwd]);

	DDX_Control(pDX, IDC_STC_IF2_PRODUCT_1, m_st_if2_Sensnor[Sensor_Prodcut_Left]);
	DDX_Control(pDX, IDC_STC_IF2_PRODUCT_2, m_st_if2_Sensnor[Sensor_Prodcut_Right]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_LEFT_FWD, m_st_if2_Sensnor[Sensor_Jig_Left_Fwd]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_LEFT_BWD, m_st_if2_Sensnor[Sensor_Jig_Left_Bwd]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_LEFT_CHK, m_st_if2_Sensnor[Sensor_Jig_Left_Chk]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_RIGHT_FWD, m_st_if2_Sensnor[Sensor_Jig_Right_Fwd]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_RIGHT_BWD, m_st_if2_Sensnor[Sensor_Jig_Right_Bwd]);
	DDX_Control(pDX, IDC_STC_IF2_JIG_RIGHT_CHK, m_st_if2_Sensnor[Sensor_Jig_Right_Chk]);
	DDX_Control(pDX, IDC_STC_IF2_CHUCK_FWD, m_st_if2_Sensnor[Sensor_Chuck_Fwd]);
	DDX_Control(pDX, IDC_STC_IF2_CHUCK_BWD, m_st_if2_Sensnor[Sensor_Chuck_Bwd]);

	DDX_Control(pDX, IDC_STC_SCREW_L_WAIT, m_st_Motor[Motor_Screw_L_Wait]);
	DDX_Control(pDX, IDC_STC_SCREW_L_START_UP, m_st_Motor[Motor_Screw_L_Start_Up]);
	DDX_Control(pDX, IDC_STC_SCREW_L_WORK, m_st_Motor[Motor_Screw_L_Work]);
	DDX_Control(pDX, IDC_STC_SCREW_R_WAIT, m_st_Motor[Motor_Screw_R_Wait]);
	DDX_Control(pDX, IDC_STC_SCREW_R_START_UP, m_st_Motor[Motor_Screw_R_Start_Up]);
	DDX_Control(pDX, IDC_STC_SCREW_R_WORK, m_st_Motor[Motor_Screw_R_Work]);
	DDX_Control(pDX, IDC_STC_VIGNETTING_WAIT, m_st_Motor[Motor_Vignetting_Wait]);
	DDX_Control(pDX, IDC_STC_VIGNETTING_TEST1, m_st_Motor[Motor_Vignetting_Test1]);
	DDX_Control(pDX, IDC_STC_VIGNETTING_TEST2, m_st_Motor[Motor_Vignetting_Test2]);

	DDX_Control(pDX, IDC_STC_IF_SCREW_L_WAIT, m_st_If2_Motor[Motor_Screw_L_Wait]);
	DDX_Control(pDX, IDC_STC_IF_SCREW_L_START_UP, m_st_If2_Motor[Motor_Screw_L_Start_Up]);
	DDX_Control(pDX, IDC_STC_IF_SCREW_L_WORK, m_st_If2_Motor[Motor_Screw_L_Work]);
	DDX_Control(pDX, IDC_STC_IF_SCREW_R_WAIT, m_st_If2_Motor[Motor_Screw_R_Wait]);
	DDX_Control(pDX, IDC_STC_IF_SCREW_R_START_UP, m_st_If2_Motor[Motor_Screw_R_Start_Up]);
	DDX_Control(pDX, IDC_STC_IF_SCREW_R_WORK, m_st_If2_Motor[Motor_Screw_R_Work]);
	DDX_Control(pDX, IDC_STC_IF_VIGNETTING_WAIT, m_st_If2_Motor[Motor_Vignetting_Wait]);
	DDX_Control(pDX, IDC_STC_IF_VIGNETTING_TEST1, m_st_If2_Motor[Motor_Vignetting_Test1]);
	DDX_Control(pDX, IDC_STC_IF_VIGNETTING_TEST2, m_st_If2_Motor[Motor_Vignetting_Test2]);

	DDX_Control(pDX, IDC_STC_IF_L_WAIT, m_st_If_Motor[Motor_Screw_L_Wait]);
	DDX_Control(pDX, IDC_STC_IF_L_START_UP, m_st_If_Motor[Motor_Screw_L_Start_Up]);
	DDX_Control(pDX, IDC_STC_IF_L_WORK, m_st_If_Motor[Motor_Screw_L_Work]);
	DDX_Control(pDX, IDC_STC_IF_R_WAIT, m_st_If_Motor[Motor_Screw_R_Wait]);
	DDX_Control(pDX, IDC_STC_IF_R_START_UP, m_st_If_Motor[Motor_Screw_R_Start_Up]);
	DDX_Control(pDX, IDC_STC_IF_R_WORK, m_st_If_Motor[Motor_Screw_R_Work]);
	DDX_Control(pDX, IDC_STC_IF_V_WAIT, m_st_If_Motor[Motor_Vignetting_Wait]);
	DDX_Control(pDX, IDC_STC_IF_V_TEST1, m_st_If_Motor[Motor_Vignetting_Test1]);
	DDX_Control(pDX, IDC_STC_IF_V_TEST2, m_st_If_Motor[Motor_Vignetting_Test2]);


	DDX_Control(pDX, IDC_STC_LOCK, m_st_Work[Work_LOCK]);
	DDX_Control(pDX, IDC_STC_POWER, m_st_Work[Work_POWER]);
	DDX_Control(pDX, IDC_STC_PREVIEW, m_st_Work[Work_PREVIEW]);
	DDX_Control(pDX, IDC_STC_ADJUST, m_st_Work[Work_ADJUST]);
	DDX_Control(pDX, IDC_STC_VIGNETTING, m_st_Work[Work_VIGNETTING]);
	DDX_Control(pDX, IDC_STC_READY, m_st_Work[Work_READY]);

	DDX_Control(pDX, IDC_STC_IF_LOCK, m_st_if_Work[Work_LOCK]);
	DDX_Control(pDX, IDC_STC_IF_POWER, m_st_if_Work[Work_POWER]);
	DDX_Control(pDX, IDC_STC_IF_PREVIEW, m_st_if_Work[Work_PREVIEW]);
	DDX_Control(pDX, IDC_STC_IF_ADJUST, m_st_if_Work[Work_ADJUST]);
	DDX_Control(pDX, IDC_STC_IF_VIGNETTING, m_st_if_Work[Work_VIGNETTING]);
	DDX_Control(pDX, IDC_STC_IF_READY, m_st_if_Work[Work_READY]);

	DDX_Control(pDX, IDC_STC_IF_PASS, m_st_if_PASS);
	DDX_Control(pDX, IDC_STC_IF_NG, m_st_if_NG);
	DDX_Control(pDX, IDC_STC_IF_RE, m_st_if_Result);


	

}


BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMain::OnBnClickedButton2)	
	ON_BN_CLICKED(IDC_BTN_YIELD_RESET, &CDlgMain::OnBnClickedBtnYieldReset)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgMain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgMain::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgMain::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgMain::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDlgMain::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CDlgMain::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDlgMain::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CDlgMain::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CDlgMain::OnBnClickedButton14)
// 	ON_BN_CLICKED(IDC_PWSP_VOLT_SET1, &CDlgMain::OnBnClickedPwspVoltSet1)
// 	ON_BN_CLICKED(IDC_PWSP_VOLT_SET2, &CDlgMain::OnBnClickedPwspVoltSet2)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO1, &CDlgMain::OnBnClickedBtnSemiauto1)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO2, &CDlgMain::OnBnClickedBtnSemiauto2)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO3, &CDlgMain::OnBnClickedBtnSemiauto3)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO4, &CDlgMain::OnBnClickedBtnSemiauto4)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO5, &CDlgMain::OnBnClickedBtnSemiauto5)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO6, &CDlgMain::OnBnClickedBtnSemiauto6)

	ON_BN_CLICKED(IDC_BTN_SEMIAUTO7, &CDlgMain::OnBnClickedBtnSemiauto7)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO9, &CDlgMain::OnBnClickedBtnSemiauto9)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO8, &CDlgMain::OnBnClickedBtnSemiauto8)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO10, &CDlgMain::OnBnClickedBtnSemiauto10)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO11, &CDlgMain::OnBnClickedBtnSemiauto11)
	ON_BN_CLICKED(IDC_BTN_SEMIAUTO12, &CDlgMain::OnBnClickedBtnSemiauto12)


	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MV_BUTTON1, &CDlgMain::OnBnClickedMvButton1)
	ON_BN_CLICKED(IDC_MV_BUTTON2, &CDlgMain::OnBnClickedMvButton2)
	ON_BN_CLICKED(IDC_MV_BUTTON3, &CDlgMain::OnBnClickedMvButton3)
	ON_BN_CLICKED(IDC_BUTTON_SEMIAUTO_CANCEL, &CDlgMain::OnBnClickedButtonSemiautoCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEMIAUTO_CANCEL2, &CDlgMain::OnBnClickedButtonSemiautoCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEMIAUTO_CANCEL3, &CDlgMain::OnBnClickedButtonSemiautoCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEMIAUTO_CANCEL4, &CDlgMain::OnBnClickedButtonSemiautoCancel)

	

	ON_BN_CLICKED(IDC_BTN_POPUP_TEST, &CDlgMain::OnBnClickedBtnPopupTest)
	ON_STN_CLICKED(IDC_STATIC_MAINFLOW_BMP, &CDlgMain::OnStnClickedStaticMainflowBmp)
END_MESSAGE_MAP()

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.3]
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);
	CDialogEx::OnLButtonDown(nFlags, point);
}

int CDlgMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

BOOL CDlgMain::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	//pDC->FillSolidRect(rect, RGB(84, 169, 169));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

void CDlgMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		KillTimer(MAIN_TIMER_MOTOR_STATUS);
		KillTimer(MAIN_TIMER_SENSOR_STATUS);
		KillTimer(MAIN_TIMER_MOTOR_POS_STATUS);
		Sleep(10);
		SetTimer(MAIN_TIMER_MOTOR_STATUS, 250, NULL);
		SetTimer(MAIN_TIMER_1234, 250, NULL);
		SetTimer(MAIN_TIMER_SENSOR_STATUS, 250, NULL);
		SetTimer(MAIN_TIMER_MOTOR_POS_STATUS, 500, NULL);
		
		MainUi_Init();
	}
	else
	{


		
	}
}

void CDlgMain::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case MAIN_TIMER_MOTOR_STATUS:
	{
		UpdateSocketPos();

		UpdateEqpStatus();

	

		//Main Status
		if(COMMON->GetOperationMode() == OperationMode::SemiAuto ||
			COMMON->GetOperationMode() == OperationMode::StartUpCheck)
		{
	



			if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
			{
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
				OnBtnSemiAuto_Show(true);
				OnBtnSemiAuto_Enable(true);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
				OnBtnSemiAuto_Show(false);
				OnBtnSemiAuto_Enable(false);
#endif
				OnBtnSemiAuto_Show_1(true);
				OnBtnSemiAuto_Enable_1(true);
			

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL)->ShowWindow(SW_SHOW);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL2)->ShowWindow(SW_SHOW);
#endif
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL4)->ShowWindow(SW_HIDE);
			}
			else
			{
				OnBtnSemiAuto_Show(true);
				OnBtnSemiAuto_Enable(true);

				OnBtnSemiAuto_Show_1(false);
				OnBtnSemiAuto_Enable_1(false);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL4)->ShowWindow(SW_SHOW);
			}
		
			EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

			if (stBasic.nDeviceIoVersion == 0)
			{
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, FALSE);
				AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, FALSE);
			}
			
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
			for (int i = 0; i < 11; i++)
			{
				if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
				{
					if (stBasic.nLanguage == 0)
						m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Kor[i]);
					else
						m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Eng[i]);
				}
				else
				{
					if (i < 6)
					{
						if (stBasic.nLanguage == 0)
							m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[i]);
						else
							m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[i]);
					}
				}
			}
#elif(SET_INSPECTOR == SYS_TLA_OQC)
			for (int i = 0; i < 11; i++)
			{
				if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
				{
					if (i > 5)
					{
						if (stBasic.nLanguage == 0)
							m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Kor[i - 6]);
						else
							m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Eng[i - 6]);
					}
				}
				else
				{
					if (i < 6)
					{
						if (stBasic.nLanguage == 0)
							m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[i]);
						else
							m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[i]);
					}
				}
			}
#endif
	
		}
		else
		{
			OnBtnSemiAuto_Show_1(false);
			OnBtnSemiAuto_Show(false);
			GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SEMIAUTO_CANCEL4)->ShowWindow(SW_HIDE);

		}		
	}
	break;

	case MAIN_TIMER_1234:
	{
		CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

		if (pParrentDlg->m_bStartCompleate == TRUE)
		{
			//SetTowerLamp(1);
			KillTimer(MAIN_TIMER_1234);
		}
	}
	case MAIN_TIMER_SENSOR_STATUS:
	{
		UpdateIFStatus();
	}
	case MAIN_TIMER_MOTOR_POS_STATUS:
	{
		UpdateMotorStatus();
	}
	

	break;
	}
}
void CDlgMain::SetOperationMode(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Show_1(false);
		// semiauto, startup check. light measure, loto 버튼 누르면 안됨

		break;
	case OperationMode::Manual:
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Show_1(false);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, TRUE);
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, TRUE);


		// all enable

		break;
	case OperationMode::SemiAuto:
	case OperationMode::StartUpCheck:
	{


#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		OnBtnSemiAuto_Show(true);
		OnBtnSemiAuto_Enable(true);
		OnBtnSemiAuto_Show_1(true);
		OnBtnSemiAuto_Enable_1(true);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Enable(false);
		OnBtnSemiAuto_Show_1(true);
		OnBtnSemiAuto_Enable_1(true);
#else
		OnBtnSemiAuto_Show(true);
		OnBtnSemiAuto_Enable(true);
		OnBtnSemiAuto_Show_1(true);
		OnBtnSemiAuto_Enable_1(true);
#endif

		EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

		if (stBasic.nDeviceIoVersion == 0)
		{
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, FALSE);
			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, FALSE);
		}

		



#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		for (int i = 0; i < 6; i++)
		{
			if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
			{
				if (stBasic.nLanguage == 0)
					m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Kor[i]);
				else
					m_bnMainSemiAuto[i].SetText(m_szMainStartupCheck_Eng[i]);
			}
			else if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
			{
				if (stBasic.nLanguage == 0)
					m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[i]);
				else
					m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[i]);
			}
	}
#elif(SET_INSPECTOR == SYS_TLA_OQC)
		for (int i = 0; i < 6; i++)
		{
			if (COMMON->GetOperationMode() == OperationMode::StartUpCheck)
			{
				if (stBasic.nLanguage == 0)
					m_bnMainSemiAuto[i + 6].SetText(m_szMainStartupCheck_Kor[i]);
				else
					m_bnMainSemiAuto[i + 6].SetText(m_szMainStartupCheck_Eng[i]);
			}
			else if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
			{
				if (stBasic.nLanguage == 0)
					m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[i]);
				else
					m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[i]);
			}
		}
#else
#endif

	}
	break;

	case OperationMode::DryRun:
	{
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Show_1(false);
	}
	break;
	case OperationMode::Loto:
	{
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Show_1(false);
	}
	break;
	default:
		OnBtnSemiAuto_Show(false);
		OnBtnSemiAuto_Show_1(false);
		break;
	}
}

// CDlgMain 메시지 처리기
void CDlgMain::InitComponent_Main()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");

//	static LPCTSTR g_szTitle[] =


	m_st_if_PASS.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_st_if_PASS.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_if_PASS.SetFont_Gdip(L"Arial", 26.0F);
	m_st_if_PASS.SetText(_T("PASS"));

	m_st_if_NG.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_st_if_NG.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_if_NG.SetFont_Gdip(L"Arial", 26.0F);
	m_st_if_NG.SetText(_T("NG"));

	m_st_if_Result.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_st_if_Result.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_if_Result.SetFont_Gdip(L"Arial", 40.0F);
	m_st_if_Result.SetText(_T("ING...."));

	
	m_st_if_Result.ShowWindow(SW_HIDE);

	for (int i = 0; i < STit_Max; i++)
	{
		m_stcTitle[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
 		m_stcTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
 		m_stcTitle[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stcTitle[i].SetText(g_szTitle[i]);
	}

	for (int i = 0; i < SEqps_Max; i++)
	{
		m_stcEqpStatus[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_stcEqpStatus[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stcEqpStatus[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stcEqpStatus[i].SetText(g_szEqpDevice[i]);
	}

	for (int i = 0; i < SIF_Door_Max; i++)
	{
		m_stDoor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_stDoor[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stDoor[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stDoor[i].SetText(g_szSIF_Door[i]);
	}

// 	for (int i = 0; i < SIFS_Max; i++)
// 	{
// 		m_stcIFStatus[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
// 		m_stcIFStatus[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
// 		m_stcIFStatus[i].SetFont_Gdip(L"Arial", 9.0F);
// 	}
	
	for (int i = 0; i < Work_Max; i++)
	{
		m_st_Work[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Work[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Work[i].SetFont_Gdip(L"Arial", 8.0F);
		if (stBasic.nLanguage == 0)
			m_st_Work[i].SetText(m_szMainWork_Kor[i]);
		else
			m_st_Work[i].SetText(m_szMainWork_Eng[i]);


		m_st_if_Work[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if_Work[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if_Work[i].SetFont_Gdip(L"Arial", 7.0F);
	}


		for (int i = 0; i < Motor_Max; i++)
		{
			m_st_If2_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
			m_st_If2_Motor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_st_If2_Motor[i].SetFont_Gdip(L"Arial", 8.0F);

			m_st_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
			m_st_Motor[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_st_Motor[i].SetFont_Gdip(L"Arial", 6.0F);
			if (stBasic.nLanguage == 0)
				m_st_Motor[i].SetText(m_szMainMotor_Kor[i]);
			else
				m_st_Motor[i].SetText(m_szMainMotor_Eng[i]);

			m_st_If_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
			m_st_If_Motor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_st_If_Motor[i].SetFont_Gdip(L"Arial", 7.0F);
		}
		m_st_If_Motor[Motor_Screw_L_Wait].SetText(_T("Wait"));
		m_st_If_Motor[Motor_Screw_L_Start_Up].SetText(_T("Start Up"));
		m_st_If_Motor[Motor_Screw_L_Work].SetText(_T("Work"));

		m_st_If_Motor[Motor_Screw_R_Wait].SetText(_T("Wait"));
		m_st_If_Motor[Motor_Screw_R_Start_Up].SetText(_T("Start Up"));
		m_st_If_Motor[Motor_Screw_R_Work].SetText(_T("Work"));

		m_st_If_Motor[Motor_Vignetting_Wait].SetText(_T("Wait"));
		m_st_If_Motor[Motor_Vignetting_Test1].SetText(_T("Test 1"));
		m_st_If_Motor[Motor_Vignetting_Test2].SetText(_T("Test 2"));

	for (int i = 0; i < Sensor_Max; i++)
	{
		m_st_if_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if_Sensnor[i].SetFont_Gdip(L"Arial", 8.0F);

		m_st_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Sensnor[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stBasic.nLanguage == 0)
			m_st_Sensnor[i].SetText(m_szMainSensor_Kor[i]);
		else
			m_st_Sensnor[i].SetText(m_szMainSensor_Eng[i]);

		m_st_if2_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if2_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if2_Sensnor[i].SetFont_Gdip(L"Arial", 8.0F);
	}
	

	m_st_if_Sensnor[Sensor_Prodcut_Left].SetText(_T("CHK"));
	m_st_if_Sensnor[Sensor_Prodcut_Right].SetText(_T("CHK"));
	m_st_if_Sensnor[Sensor_Jig_Left_Fwd].SetText(_T("FWD"));
	m_st_if_Sensnor[Sensor_Jig_Left_Bwd].SetText(_T("BWD"));
	m_st_if_Sensnor[Sensor_Jig_Left_Chk].SetText(_T("CHK"));
	m_st_if_Sensnor[Sensor_Jig_Right_Fwd].SetText(_T("FWD"));
	m_st_if_Sensnor[Sensor_Jig_Right_Bwd].SetText(_T("BWD"));
	m_st_if_Sensnor[Sensor_Jig_Right_Chk].SetText(_T("CHK"));
	m_st_if_Sensnor[Sensor_Chuck_Fwd].SetText(_T("FWD"));
	m_st_if_Sensnor[Sensor_Chuck_Bwd].SetText(_T("BWD"));


	for (int i = 0; i < Door_Max; i++)
	{
		m_stcIDoor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_stcIDoor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stcIDoor[i].SetFont_Gdip(L"Arial", 8.0F);


		m_st_Door[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Door[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_Door[i].SetFont_Gdip(L"Arial", 8.0F);
	}
	
	m_stcIDoor[Door_FrontOut].SetText(_T("(1)"));
	m_stcIDoor[Door_FrontIn].SetText(_T("(1)"));
	m_stcIDoor[Door_Side_Left1_Out].SetText(_T("(2)"));
	m_stcIDoor[Door_Side_Left1_In].SetText(_T("(2)"));
	m_stcIDoor[Door_Side_Left2_Out].SetText(_T("(3)"));
	m_stcIDoor[Door_Side_Left2_In].SetText(_T("(3)"));
	m_stcIDoor[Door_Side_Left3_Out].SetText(_T("(4)"));
	m_stcIDoor[Door_Side_Left3_In].SetText(_T("(4)"));

	m_stcIDoor[Door_Side_Right1_Out].SetText(_T("(5)"));
	m_stcIDoor[Door_Side_Right1_In].SetText(_T("(5)"));
	m_stcIDoor[Door_Side_Right2_Out].SetText(_T("(6)"));
	m_stcIDoor[Door_Side_Right2_In].SetText(_T("(6)"));
	m_stcIDoor[Door_Side_Right3_Out].SetText(_T("(7)"));
	m_stcIDoor[Door_Side_Right3_In].SetText(_T("(7)"));

	m_stcIDoor[Door_Rear1_Out].SetText(_T("(8)"));
	m_stcIDoor[Door_Rear1_In].SetText(_T("(8)"));
	m_stcIDoor[Door_Rear2_Out].SetText(_T("(9)"));
	m_stcIDoor[Door_Rear2_In].SetText(_T("(9)"));

// 	m_stcEqpStatus[SEqps_CL500A].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_DeepDarkGray);
// 	m_stcEqpStatus[SEqps_CL500A].SetTextColor(Gdiplus::Color::Black);
// 
// 	m_stcEqpStatus[SEqps_CL200A].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_DeepDarkGray);
// 	m_stcEqpStatus[SEqps_CL200A].SetTextColor(Gdiplus::Color::Black);

	m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_DeepDarkGray);
	m_stcEqpStatus[SEqps_Light1].SetTextColor(Gdiplus::Color::Black);

	m_stcEqpStatus[SEqps_Light2].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_DeepDarkGray);
	m_stcEqpStatus[SEqps_Light2].SetTextColor(Gdiplus::Color::Black);

// 	m_stcEqpStatus[SEqps_Light3].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_DeepDarkGray);
// 	m_stcEqpStatus[SEqps_Light3].SetTextColor(Gdiplus::Color::Black);






// 	auto EqpType = SET_INSPECTOR;
// 	switch (EqpType)
// 	{
// 	case SYS_SFR_NARROW:
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
// 		m_stcEqpStatus[SEqps_Light1].SetTextColor(Gdiplus::Color::White);
// 		break;
// 	case SYS_HOTPIXEL:
// 		m_stcEqpStatus[SEqps_CL200A].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
// 		m_stcEqpStatus[SEqps_CL200A].SetTextColor(Gdiplus::Color::White);
// 		break;
// 	case SYS_DISTORTION:
		m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
		m_stcEqpStatus[SEqps_Light1].SetTextColor(Gdiplus::Color::White);

		m_stcEqpStatus[SEqps_Light2].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
		m_stcEqpStatus[SEqps_Light2].SetTextColor(Gdiplus::Color::White);

// 		m_stcEqpStatus[SEqps_Light3].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
// 		m_stcEqpStatus[SEqps_Light3].SetTextColor(Gdiplus::Color::White);
// 		break;
// 
// 	case SYS_COLOR_CAL:
// 		m_stcEqpStatus[SEqps_CL500A].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
// 		m_stcEqpStatus[SEqps_CL500A].SetTextColor(Gdiplus::Color::White);
// 
// 		m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle::ColorStyle_SlateGray);
// 		m_stcEqpStatus[SEqps_Light1].SetTextColor(Gdiplus::Color::White);
// 		break;
// 	}

	MainUi_Init();

// 	for (int i = 0; i < 7; i++)
// 	{
// 		m_stMainMotor_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stMainMotor_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
// 		m_stMainMotor_Title[i].SetFont_Gdip(L"Arial", 9.0F);
// 
// 		if (stBasic.nLanguage == 0)
// 			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Kor[i]);
// 		else
// 			m_stMainMotor_Title[i].SetText(m_szMainMotorSubTitle_Eng[i]);
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		strTemp.Format(_T("%d"), i);
// 		m_stMainMotor_Number[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stMainMotor_Number[i].SetColorStyle(CVGStatic::ColorStyle_Black);
// 		m_stMainMotor_Number[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Number[i].SetText(strTemp);
// 	}
	for (int i = 0; i < 6; i++)
	{
		int nCnt = i;
		if (i == 4 || i == 5) nCnt += 2;
		strTemp.Format(_T("%d"), i);
		m_bnMainSemiAuto[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_bnMainSemiAuto[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stBasic.nLanguage == 0)
			m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[nCnt]);
		else
			m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[nCnt]);
	}


	for (int i = 0; i < 5; i++)
	{
		int nCnt = i;
		m_bnMainSemiAuto[i+6].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[i + 6].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_bnMainSemiAuto[i + 6].SetFont_Gdip(L"Arial", 7.0F);
		if (stBasic.nLanguage == 0)
			m_bnMainSemiAuto[i + 6].SetText(m_szMainStartupCheck_Kor[nCnt + 6]);
		else
			m_bnMainSemiAuto[i + 6].SetText(m_szMainStartupCheck_Eng[nCnt + 6]);
	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stBasic.nLanguage == 0)
// 			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Kor[i]);
// 		else
// 			m_StMainMotor_Name[i].SetText(m_szMainMotorSubTitle_Eng[i]);
// 	}
// 
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_StMainMotor_Name[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_StMainMotor_Name[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_StMainMotor_Name[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stBasic.nLanguage == 0)
// 			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Kor[i]);
// 		else
// 			m_StMainMotor_Name[i].SetText(m_szMainMotorName_Eng[i]);
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_Pos[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_Pos[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_Pos[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Pos[i].SetText(_T("0.000"));
// 	}
// 
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_Limit_P[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_Limit_P[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_Limit_P[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Pos[i].SetText(_T(""));
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_Limit_M[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_Limit_M[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_Limit_M[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Limit_M[i].SetText(_T(""));
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_Error[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_Error[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_Error[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Error[i].SetText(_T(""));
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_InMotion[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_InMotion[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_InMotion[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_InMotion[i].SetText(_T(""));
// 	}
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stMainMotor_Home[i].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainMotor_Home[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 		m_stMainMotor_Home[i].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainMotor_Home[i].SetText(_T(""));
// 	}
// 
// 	m_stMainYield_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 	m_stMainYield_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
// 	m_stMainYield_Title.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stBasic.nLanguage == 0)
// 		m_stMainYield_Title.SetText(_T("일일생산량( Yield )"));
// 	else
// 		m_stMainYield_Title.SetText(_T("Yield"));
// 
// 	m_btnMainYield_Reset.EnableWindowsTheming(FALSE);
// 	m_btnMainYield_Reset.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnMainYield_Reset.m_bTransparent = false;
// 	m_btnMainYield_Reset.SetTextColor(COLOR_BLACK);
// 	m_btnMainYield_Reset.SetFaceColor(RGB(225, 225, 225), true);
// 	if (stBasic.nLanguage == 0)
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Kor[4]);
// 	else
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Eng[4]);
// 
// 	for (int i = 0; i < 4; i++)
// 	{
// 		m_stMainYield_Title_Sub[i][0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stMainYield_Title_Sub[i][0].SetColorStyle(CVGStatic::ColorStyle_Black);
// 		m_stMainYield_Title_Sub[i][0].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stBasic.nLanguage == 0)
// 			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Kor[i]);
// 		else
// 			m_stMainYield_Title_Sub[i][0].SetText(m_szMainYield_Eng[i]);
// 
// 		m_stMainYield_Title_Sub[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 		m_stMainYield_Title_Sub[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		m_stMainYield_Title_Sub[i][1].SetFont_Gdip(L"Arial", 9.0F);
// 		m_stMainYield_Title_Sub[i][1].SetText(_T(""));
// 	}
// 
// 	m_stMainBarcode_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 	m_stMainBarcode_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
// 	m_stMainBarcode_Title.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stBasic.nLanguage == 0)
// 		m_stMainBarcode_Title.SetText(_T("바코드(Barcode)"));
// 	else
// 		m_stMainBarcode_Title.SetText(_T("Barcode"));
}

void CDlgMain::EqpTypeShowDialog()
{
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		m_pInspectorMainDlg = new CDlgColorCalMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_COLOR_CAL_MAIN, this);
// 		break;
// 
// 	case SYS_DISTORTION:
		m_pInspectorMainDlg = new CDlgDistortionMain;
		m_pInspectorMainDlg->Create(IDD_DLG_DISTORTION_MAIN, this);
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 		m_pInspectorMainDlg = new CDlgGhostFlareMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_GHOSTFLARE_MAIN, this);
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 		m_pInspectorMainDlg = new CDlgHotPixelMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_HOTPIXEL_MAIN, this);
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 		m_pInspectorMainDlg = new CDlgSFRNarrowMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_SFR_NARROW_MAIN, this);
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 		m_pInspectorMainDlg = new CDlgSFRWideHMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_SFR_WIDE_H_MAIN, this);
// 		break;
// 
// 	case SYS_SFR_WIDE_V:
// 		m_pInspectorMainDlg = new CDlgSFRWideVMain;
// 		m_pInspectorMainDlg->Create(IDD_DLG_SFR_WIDE_V_MAIN, this);
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CDlgMain::EqpTypeShowOperation()
{
//	auto EqpType = SET_INSPECTOR;

	auto mode = COMMON->GetOperationMode();
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 	{
// 		auto * pDlg = static_cast<CDlgColorCalMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_DISTORTION:
// 	{
		auto * pDlg = static_cast<CDlgDistortionMain*>(m_pInspectorMainDlg);
		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 	{
// 		auto * pDlg = static_cast<CDlgGhostFlareMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 	{
// 		auto * pDlg = static_cast<CDlgHotPixelMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRNarrowMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRWideHMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	case SYS_SFR_WIDE_V:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRWideVMain*>(m_pInspectorMainDlg);
// 		pDlg->ShowOperation(mode);
// 	}		
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CDlgMain::InitInspectorMainDialog()
{
	EqpTypeShowDialog();
	EqpTypeShowOperation();
	
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_INSPECTOR_MAIN)->GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 		
// 	m_pInspectorMainDlg->MoveWindow(&rect);
// 	m_pInspectorMainDlg->ShowWindow(SW_SHOW);

// 	for (auto i = 0; i < (int)SocketPos::Max; i++)
// 	{
// 		m_pDlgSocket[i] = new CDlgSocket;
// 		m_pDlgSocket[i]->Create(IDD_DLG_SOCKET, this);
// 		
// 		m_stcSocket[i].GetWindowRect(&rect);
// 		m_stcSocket[i].ShowWindow(SW_HIDE);
// 
// 		ScreenToClient(&rect);
// 		m_pDlgSocket[i]->MoveWindow(&rect);
// 		m_pDlgSocket[i]->ShowWindow(SW_SHOW);
// 	}
}

void CDlgMain::ReleaseInspectorMainDialog()
{
	m_pInspectorMainDlg->DestroyWindow();
	delete m_pInspectorMainDlg;
	m_pInspectorMainDlg = nullptr;

// 	for (auto i = 0; i < (int)SocketPos::Max; i++)
// 	{
// 		m_pDlgSocket[i]->DestroyWindow();
// 		delete m_pDlgSocket[i];
// 		m_pDlgSocket[i] = nullptr;
// 	}
}

void CDlgMain::MainUi_Init()
{

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	m_st_if_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_SHOW);

	m_st_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_SHOW);

	m_st_if2_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_SHOW);

	m_st_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_L_Wait].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_L_Work].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_R_Wait].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_R_Work].ShowWindow(SW_SHOW);

	m_st_If_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_L_Wait].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_L_Work].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_R_Wait].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_R_Work].ShowWindow(SW_SHOW);

	m_st_If2_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_L_Wait].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_L_Work].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_R_Wait].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_R_Work].ShowWindow(SW_SHOW);

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	m_st_if_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_if_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_HIDE);
	m_st_if_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_HIDE);

	m_st_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_HIDE);
	m_st_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_HIDE);

	m_st_if2_Sensnor[Sensor_Prodcut_Left].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Prodcut_Right].ShowWindow(SW_SHOW);
	m_st_if2_Sensnor[Sensor_Jig_Left_Fwd].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Jig_Left_Bwd].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Jig_Left_Chk].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Jig_Right_Fwd].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Jig_Right_Bwd].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Jig_Right_Chk].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Chuck_Fwd].ShowWindow(SW_HIDE);
	m_st_if2_Sensnor[Sensor_Chuck_Bwd].ShowWindow(SW_HIDE);

	m_st_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_Motor[Motor_Screw_L_Wait].ShowWindow(SW_HIDE);
	m_st_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_HIDE);
	m_st_Motor[Motor_Screw_L_Work].ShowWindow(SW_HIDE);
	m_st_Motor[Motor_Screw_R_Wait].ShowWindow(SW_HIDE);
	m_st_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_HIDE);
	m_st_Motor[Motor_Screw_R_Work].ShowWindow(SW_HIDE);

	m_st_If_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_If_Motor[Motor_Screw_L_Wait].ShowWindow(SW_HIDE);
	m_st_If_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_HIDE);
	m_st_If_Motor[Motor_Screw_L_Work].ShowWindow(SW_HIDE);
	m_st_If_Motor[Motor_Screw_R_Wait].ShowWindow(SW_HIDE);
	m_st_If_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_HIDE);
	m_st_If_Motor[Motor_Screw_R_Work].ShowWindow(SW_HIDE);

	m_st_If2_Motor[Motor_Vignetting_Wait].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Vignetting_Test1].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Vignetting_Test2].ShowWindow(SW_SHOW);
	m_st_If2_Motor[Motor_Screw_L_Wait].ShowWindow(SW_HIDE);
	m_st_If2_Motor[Motor_Screw_L_Start_Up].ShowWindow(SW_HIDE);
	m_st_If2_Motor[Motor_Screw_L_Work].ShowWindow(SW_HIDE);
	m_st_If2_Motor[Motor_Screw_R_Wait].ShowWindow(SW_HIDE);
	m_st_If2_Motor[Motor_Screw_R_Start_Up].ShowWindow(SW_HIDE);
	m_st_If2_Motor[Motor_Screw_R_Work].ShowWindow(SW_HIDE);
#elif(SET_INSPECTOR == SYS_TLA_B)
#endif



}

void CDlgMain::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	for (int i = 0; i < 6; i++)
	{
		int nCnt = i;
		if (i == 4 || i == 5) nCnt += 2;
		strTemp.Format(_T("%d"), i);
		m_bnMainSemiAuto[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_bnMainSemiAuto[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stBasic.nLanguage == 0)
			m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Kor[nCnt]);
		else
			m_bnMainSemiAuto[i].SetText(m_szMainSemiAuto_Eng[nCnt]);
	}

	for (int i = 0; i < Work_Max; i++)
	{
		m_st_Work[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Work[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Work[i].SetFont_Gdip(L"Arial", 8.0F);
		if (stBasic.nLanguage == 0)
			m_st_Work[i].SetText(m_szMainWork_Kor[i]);
		else
			m_st_Work[i].SetText(m_szMainWork_Eng[i]);


		m_st_if_Work[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if_Work[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if_Work[i].SetFont_Gdip(L"Arial", 7.0F);
	}
	for (int i = 0; i < Motor_Max; i++)
	{
		m_st_If2_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_If2_Motor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_If2_Motor[i].SetFont_Gdip(L"Arial", 8.0F);

		m_st_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Motor[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Motor[i].SetFont_Gdip(L"Arial", 6.0F);
		if (stBasic.nLanguage == 0)
			m_st_Motor[i].SetText(m_szMainMotor_Kor[i]);
		else
			m_st_Motor[i].SetText(m_szMainMotor_Eng[i]);

		m_st_If_Motor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_If_Motor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_If_Motor[i].SetFont_Gdip(L"Arial", 7.0F);
	}
	for (int i = 0; i < Sensor_Max; i++)
	{
		m_st_if_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if_Sensnor[i].SetFont_Gdip(L"Arial", 8.0F);

		m_st_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Sensnor[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stBasic.nLanguage == 0)
			m_st_Sensnor[i].SetText(m_szMainSensor_Kor[i]);
		else
			m_st_Sensnor[i].SetText(m_szMainSensor_Eng[i]);

		m_st_if2_Sensnor[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_if2_Sensnor[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_st_if2_Sensnor[i].SetFont_Gdip(L"Arial", 8.0F);
	}

}

BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// [21.1029.6]
	InitComponent_Main();
	InitInspectorMainDialog();
	COMMON->SetMcStatus(eStop);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgMain::DestroyWindow()
{

	KillTimer(MAIN_TIMER_MOTOR_STATUS);
	KillTimer(MAIN_TIMER_SENSOR_STATUS);
	KillTimer(MAIN_TIMER_MOTOR_POS_STATUS);

	ReleaseInspectorMainDialog();
	return CDialogEx::DestroyWindow();
}

//---------------------------------------------------------
// tower lamp
// void CDlgMain::SetTowerLamp(int nMode)
// {
// 	EQ_TOWERLAMP stTower = *DATAMANAGER->GetCurrentTowerLampData();
// 
// 	BOOL bIsBlink = FALSE;
// 
// 	if (m_bIsTower_Blink == TRUE)
// 		KillTimer(ONTIMER_TOWERLAMP);
// 
// 	for (int i = 0; i < 3; i++)
// 	{
// 		
// 		if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_On)
// 			AJIN_IO->SendOutputBit((int)DIO_OUTPUT_Y0_09_TOWERLAMP_RED +i , TRUE);
// 		else if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_Off)
// 			AJIN_IO->SendOutputBit((int)DIO_OUTPUT_Y0_09_TOWERLAMP_RED + i,  FALSE);
// 		else
// 		{
// 			if (stTower.nTowerLampSetting[nMode][i] == eTowerLamp_Blink)
// 				bIsBlink = TRUE;
// 		}
// 		
// 	}
// 
// 	m_bIsTower_Blink = bIsBlink;
// 	if (m_bIsTower_Blink == TRUE)
// 		SetTimer(ONTIMER_TOWERLAMP, 800, NULL);
// 
// 	m_nCurrentMode = (int)nMode;
// 
// }

//=======================================================================================================
// Message
void CDlgMain::ReadMotorStatus_Fastech()
{
	// Home
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorHome_Done[i] != (BOOL)FAS_MO->IsHomeOK(i))
// 		{
// 			m_bMotorHome_Done[i] = (BOOL)FAS_MO->IsHomeOK(i);
// 			SendMessage(MESSAGE_UI_CHANGE_HOME, i, m_bMotorHome_Done[i]);
// 		}
// 	}

	//Limit
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorLimit_P[i] != (BOOL)FAS_MO->IsHWPlusLimitStatus(i))
// 		{
// 			m_bMotorLimit_P[i] = (BOOL)FAS_MO->IsHWPlusLimitStatus(i);
// 			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i, m_bMotorLimit_P[i]);
// 		}
// 
// 		if (m_bMotorLimit_M[i] != (BOOL)FAS_MO->IsHWMinusLimitStatus(i))
// 		{
// 			m_bMotorLimit_M[i] = (BOOL)FAS_MO->IsHWMinusLimitStatus(i);
// 			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i + 10, m_bMotorLimit_M[i]);
// 		}
// 	}
	// Motor Alarm
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorAlarm[i] != (BOOL)FAS_MO->IsAlarmStatus(i))
// 		{
// 			m_bMotorAlarm[i] = (BOOL)FAS_MO->IsAlarmStatus(i);
// 			SendMessage(MESSAGE_UI_CHANGE_ALARM, i, m_bMotorAlarm[i]);
// 		}
// 	}

	// Inmotion
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorInMotion[i] != (BOOL)FAS_MO->IsInposStatus(i))
// 		{
// 			m_bMotorInMotion[i] = (BOOL)FAS_MO->IsInposStatus(i);
// 			SendMessage(MESSAGE_UI_CHANGE_INMOTION, i, m_bMotorInMotion[i]);
// 		}
// 	}

}
void CDlgMain::ReadMotorStatus_Ajin()
{
	// Home
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorHome_Done[i] != (BOOL)AJIN_MO->m_bReadHome[i])
// 		{
// 			m_bMotorHome_Done[i] = (BOOL)AJIN_MO->m_bReadHome[i];
// 			SendMessage(MESSAGE_UI_CHANGE_HOME, i, m_bMotorHome_Done[i]);
// 		}
// 	}
	//Limit
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorLimit_P[i] != (BOOL)AJIN_MO->m_bReadLimitP[i])
// 		{
// 			m_bMotorLimit_P[i] = (BOOL)AJIN_MO->m_bReadLimitP[i];
// 			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i, m_bMotorLimit_P[i]);
// 		}
// 
// 		if (m_bMotorLimit_M[i] != (BOOL)AJIN_MO->m_bReadLimitM[i])
// 		{
// 			m_bMotorLimit_M[i] = (BOOL)AJIN_MO->m_bReadLimitM[i];
// 			SendMessage(MESSAGE_UI_CHANGE_LIMIT, i + 10, m_bMotorLimit_M[i]);
// 		}
// 	}
	// Motor Alarm
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorAlarm[i] != (BOOL)AJIN_MO->m_bReadError[i])
// 		{
// 			m_bMotorAlarm[i] = (BOOL)AJIN_MO->m_bReadError[i];
// 			SendMessage(MESSAGE_UI_CHANGE_ALARM, i, m_bMotorAlarm[i]);
// 		}
// 	}

	// Inmotion
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		if (m_bMotorInMotion[i] != (BOOL)AJIN_MO->m_bReadInPos[i])
// 		{
// 			m_bMotorInMotion[i] = (BOOL)AJIN_MO->m_bReadInPos[i];
// 			SendMessage(MESSAGE_UI_CHANGE_INMOTION, i, m_bMotorInMotion[i]);
// 		}
// 	}
}
void CDlgMain::GetCurrentPosition()
{
// 	CString strPos = _T("");
// 	double dPos = 0.0f;
// 
// 	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
// 	{
// 		dPos = FAS_MO->GetAxisCurrnetActualPos(i);
// 		strPos.Format(_T("%0.3f"), dPos);
// 
// 		m_stMainMotor_Pos[i].SetText(strPos);
// 	}
}

void CDlgMain::UpdateSocketPos()
{
	auto *pSeq = SEQUENCE;

// 	ST_SocketInfo socket;
// 		
// 	for (auto i = 0;i < (int)SocketPos::Max; i++)
// 	{
// 		SocketPos pos = (SocketPos)i;
// 		if (pSeq->GetSocketInfo(pos, socket))
// 		{
// 			m_pDlgSocket[(int)pos]->SetSocket(socket);
// 			m_pDlgSocket[(int)pos]->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			m_pDlgSocket[(int)pos]->ShowWindow(SW_HIDE);
// 		}
// 	}	
}

void CDlgMain::UpdateEqpStatus()
{
	// ics
// 	if (ICS_COMM->IsConnected())
// 		m_stcEqpStatus[SEqps_Ics].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_Ics].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);	
// 
// 	// motor
// 	if(AJIN_MO->IsOrigin())
// 		m_stcEqpStatus[SEqps_Motor].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_Motor].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// semco1
	if(m_UdpTester[0].m_bConnected)
		m_stcEqpStatus[SEqps_Semco1].SetColorStyle(CVGBase_Static::ColorStyle_Green);
	else
		m_stcEqpStatus[SEqps_Semco1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// semco2
// 	if (m_UdpTester[1].m_bConnected)
// 		m_stcEqpStatus[SEqps_Semco2].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_Semco2].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// cl200a
// #if(SET_INSPECTOR == SYS_HOTPIXEL)
// 	if(CL200A_COMM->IsPCConnected())
// 		m_stcEqpStatus[SEqps_CL200A].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_CL200A].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// #endif

	// rfid
// 	if(COMMON->m_bRFID_Connected)
// 		m_stcEqpStatus[SEqps_RFID].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_RFID].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// power supply
	if (m_ctrlLight_ODA_Q[0].m_bConnected)
		m_stcEqpStatus[SEqps_PWR].SetColorStyle(CVGBase_Static::ColorStyle_Green);
	else
		m_stcEqpStatus[SEqps_PWR].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);


	// light1
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_SFR_NARROW:
// 		if (m_ctrlLightController_Youth_DPS[0].Is_Connected())
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 		else
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		if (m_ctrlLightController_FN1500[0].Is_Connected())
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 		else
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// 		break;
// 
// 	case SYS_DISTORTION:
		if (m_ctrlLightController_Youth_DPS[0].Is_Connected())
			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_Green);
		else
			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

		if (m_ctrlLightController_Youth_DPS[1].Is_Connected())
			m_stcEqpStatus[SEqps_Light2].SetColorStyle(CVGBase_Static::ColorStyle_Green);
		else
			m_stcEqpStatus[SEqps_Light2].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

// 		if (m_ctrlLightController_Youth_DPS[2].Is_Connected())
// 			m_stcEqpStatus[SEqps_Light3].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 		else
// 			m_stcEqpStatus[SEqps_Light3].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// 		break;
// 	case SYS_COLOR_CAL:
// 		if (m_ctrlLightController_Youth[0].Is_Connected())
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 		else
// 			m_stcEqpStatus[SEqps_Light1].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// 		break;
// 	default:
// 		break;
// 	}

	// cl500a
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	if (CL500A_COMM->IsConnected())
// 		m_stcEqpStatus[SEqps_CL500A].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcEqpStatus[SEqps_CL500A].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
// #endif
}

void CDlgMain::UpdateMotorStatus()
{
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	m_MotorStatus[Motor_Screw_L_Wait] = AJIN_MO->Check_Move_Pos(SCREW_L_X, (double)stTeaching.nScrew_L_Wait / 1000);
	m_MotorStatus[Motor_Screw_L_Start_Up] = AJIN_MO->Check_Move_Pos(SCREW_L_X, (double)stTeaching.nScrew_L_Commencement / 1000);
	m_MotorStatus[Motor_Screw_L_Work] = AJIN_MO->Check_Move_Pos(SCREW_L_X, (double)stTeaching.nScrew_L_Work / 1000);
	m_MotorStatus[Motor_Screw_R_Wait] = AJIN_MO->Check_Move_Pos(SCREW_R_X, (double)stTeaching.nScrew_R_Wait / 1000);
	m_MotorStatus[Motor_Screw_R_Start_Up] = AJIN_MO->Check_Move_Pos(SCREW_R_X, (double)stTeaching.nScrew_R_Commencement / 1000);
	m_MotorStatus[Motor_Screw_R_Work] = AJIN_MO->Check_Move_Pos(SCREW_R_X, (double)stTeaching.nScrew_R_Work / 1000);
	m_MotorStatus[Motor_Vignetting_Wait] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Wait / 1000);
	m_MotorStatus[Motor_Vignetting_Test1] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Test_01 / 1000);
	m_MotorStatus[Motor_Vignetting_Test2] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Test_02 / 1000);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	m_MotorStatus[Motor_Vignetting_Wait] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Wait /1000);
	m_MotorStatus[Motor_Vignetting_Test1] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Test_01 / 1000);
	m_MotorStatus[Motor_Vignetting_Test2] = AJIN_MO->Check_Move_Pos(VIGNETTING_X, (double)stTeaching.nVignetting_X_Test_02 / 1000);
#elif(SET_INSPECTOR == SYS_TLA_B)
#endif







	for (int i = 0; i < Motor_Max; i++)
	{
		if (m_MotorStatus[i] != m_MotorStatusBK[i])
		{
			m_MotorStatusBK[i] = m_MotorStatus[i];
	
		if (m_MotorStatus[i] == TRUE)
		{
			m_st_If_Motor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
			m_st_If2_Motor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);

		}
		else
		{
			m_st_If_Motor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			m_st_If2_Motor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
		}
		}
	}

	m_PASS_NG_Status = SEQUENCE->nPASS_NG;
	if (m_PASS_NG_Status != m_PASS_NG_StatusBK)
	{
		m_PASS_NG_StatusBK = m_PASS_NG_Status;
		Update_PASSNG_Status(m_PASS_NG_Status);
	}


}
void CDlgMain::UpdateIFStatus()
{
	m_DoorStatus[0] = AJIN_IO->IsON(DIO_INPUT_X0_0E_FRONT_DOOR1_INTERLOCK_ON_OFF);
	m_DoorStatus[1] = AJIN_IO->IsON(DIO_INPUT_X0_0F_FRONT_DOOR1_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[2] = AJIN_IO->IsON(DIO_INPUT_X0_10_SIDE_LEFT_DOOR1_INTERLOCK_ON_OFF);
	m_DoorStatus[3] = AJIN_IO->IsON(DIO_INPUT_X0_11_SIDE_LEFT_DOOR1_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[4] = AJIN_IO->IsON(DIO_INPUT_X0_12_SIDE_LEFT_DOOR2_INTERLOCK_ON_OFF);
	m_DoorStatus[5] = AJIN_IO->IsON(DIO_INPUT_X0_13_SIDE_LEFT_DOOR2_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[6] = AJIN_IO->IsON(DIO_INPUT_X0_14_SIDE_LEFT_DOOR3_INTERLOCK_ON_OFF);
	m_DoorStatus[7] = AJIN_IO->IsON(DIO_INPUT_X0_15_SIDE_LEFT_DOOR3_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[8] = AJIN_IO->IsON(DIO_INPUT_X0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ON_OFF);
	m_DoorStatus[9] = AJIN_IO->IsON(DIO_INPUT_X0_17_SIDE_RIGHT_DOOR1_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[10] = AJIN_IO->IsON(DIO_INPUT_X0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ON_OFF);
	m_DoorStatus[11] = AJIN_IO->IsON(DIO_INPUT_X0_19_SIDE_RIGHT_DOOR2_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[12] = AJIN_IO->IsON(DIO_INPUT_X0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ON_OFF);
	m_DoorStatus[13] = AJIN_IO->IsON(DIO_INPUT_X0_1B_SIDE_RIGHT_DOOR3_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[14] = AJIN_IO->IsON(DIO_INPUT_X0_1C_REAR_DOOR1_INTERLOCK_ON_OFF);
	m_DoorStatus[15] = AJIN_IO->IsON(DIO_INPUT_X0_1D_REAR_DOOR1_INTERLOCK_OPEN_CLOSE);
	m_DoorStatus[16] = AJIN_IO->IsON(DIO_INPUT_X0_1E_REAR_DOOR2_INTERLOCK_ON_OFF);
	m_DoorStatus[17] = AJIN_IO->IsON(DIO_INPUT_X0_1F_REAR_DOOR2_INTERLOCK_OPEN_CLOSE);
	
	
	for (int i = 0; i < Door_Max; i++)
	{
		if (m_DoorStatus[i] != m_DoorStatusBK[i])
		{
			m_DoorStatusBK[i] = m_DoorStatus[i];

		if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16)
		{
			if (m_DoorStatus[i] == TRUE)
			{
				m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
				m_st_Door[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
				
			}
			else
			{
			//	m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

				m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
				m_st_Door[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			}
		}
		else
		{
			if (m_DoorStatus[i] == TRUE)
			{
				m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_Yellow);
				m_st_Door[i].SetColorStyle(CVGBase_Static::ColorStyle_Yellow);
			}
			else
			{
				//m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

				m_stcIDoor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
				m_st_Door[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			}
		}
		}

		

	}

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)			// SYS_TLA_ASSEMBLE
 	m_SensorStatus[Sensor_Prodcut_Left] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
 	m_SensorStatus[Sensor_Prodcut_Right] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
 	m_SensorStatus[Sensor_Jig_Left_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
 	m_SensorStatus[Sensor_Jig_Left_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);
 	m_SensorStatus[Sensor_Jig_Left_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
 	m_SensorStatus[Sensor_Jig_Right_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
 	m_SensorStatus[Sensor_Jig_Right_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
 	m_SensorStatus[Sensor_Jig_Right_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
 	m_SensorStatus[Sensor_Chuck_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR);
 	m_SensorStatus[Sensor_Chuck_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR);
	for (int i = 0; i < Sensor_Max; i++)
	{
		if (m_SensorStatus[i] != m_SensorStatusBK[i])
		{
			m_SensorStatusBK[i] = m_SensorStatus[i];

			if (m_SensorStatus[i] == TRUE)
			{
				m_st_if_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
				m_st_if2_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);

			}
			else
			{
				m_st_if_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
				m_st_if2_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			}
		}
	}
#elif(SET_INSPECTOR == SYS_TLA_OQC)		// SYS_TLA_OQC
	m_SensorStatus[Sensor_Prodcut_Left] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
	m_SensorStatus[Sensor_Prodcut_Right] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Chuck_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Chuck_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR);

	for (int i = 0; i < 2; i++)
	{
		if (m_SensorStatus[i] != m_SensorStatusBK[i])
		{
			m_SensorStatusBK[i] = m_SensorStatus[i];

			if (m_SensorStatus[i] == TRUE)
			{
				m_st_if_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
				m_st_if2_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);

}
			else
			{
				m_st_if_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
				m_st_if2_Sensnor[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			}
		}
	}

#elif(SET_INSPECTOR == SYS_TLA_B)		// SYS_TLA_OQC
// 	m_SensorStatus[Sensor_Prodcut_Left] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
// 	m_SensorStatus[Sensor_Prodcut_Right] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Left_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Jig_Right_Chk] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Chuck_Fwd] = AJIN_IO->IsON(DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR);
// 	m_SensorStatus[Sensor_Chuck_Bwd] = AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR);
#endif



		m_WorkStatus[Work_LOCK] = SEQUENCE->PickMgrJob0();
		m_WorkStatus[Work_POWER] = SEQUENCE->PickMgrJob1();
		m_WorkStatus[Work_PREVIEW] = SEQUENCE->PickMgrJob2();
		m_WorkStatus[Work_ADJUST] = SEQUENCE->PickMgrJob3();
		m_WorkStatus[Work_VIGNETTING] = SEQUENCE->PickMgrJob4();
		auto p = SEQUENCE->PickMgrStep();
		if (p == SEQ_PICKERMGR_STEP_READY_START ||
			p == SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF ||
			p == SEQ_PICKERMGR_STEP_READY_POWER_OFF ||
			p == SEQ_PICKERMGR_STEP_READY_UNLOCK1 ||
			p == SEQ_PICKERMGR_STEP_READY_UNLOCK2 ||
			p == SEQ_PICKERMGR_STEP_READY_END)
		{
			m_WorkStatus[Work_READY] = TRUE;
		}
		else
		{
			m_WorkStatus[Work_READY] = FALSE;
		}

		


	for (int i = 0; i < Work_Max; i++)
	{
		if (m_WorkStatus[i] != m_WorkStatusBK[i])
		{
			m_WorkStatusBK[i] = m_WorkStatus[i];
			if (m_WorkStatus[i] == TRUE)
			{
				m_st_if_Work[i].SetColorStyle(CVGBase_Static::ColorStyle_Green);
			}
			else
			{
				m_st_if_Work[i].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
			}
		}
	}





	
	// in front req
//	if(AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0C_FRONT_IN_STREAM))
//		m_stcIFStatus[SIFS_InFrontReq].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcIFStatus[SIFS_InFrontReq].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// in front res
//	if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_0C_FRONT_IN_STREAM))
//		m_stcIFStatus[SIFS_InFrontRes].SetColorStyle(CVGBase_Static::ColorStyle_Red);
// 	else
// 		m_stcIFStatus[SIFS_InFrontRes].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// in rear req
//	if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_0D_REAR_IN_STREAM))
//		m_stcIFStatus[SIFS_InRearReq].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcIFStatus[SIFS_InRearReq].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// in rear res
//	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0D_REAR_IN_STREAM))
//		m_stcIFStatus[SIFS_InRearRes].SetColorStyle(CVGBase_Static::ColorStyle_Red);
// 	else
// 		m_stcIFStatus[SIFS_InRearRes].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// out front req
//	if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_0E_FRONT_OUT_STREAM))
//		m_stcIFStatus[SIFS_OutFrontReq].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcIFStatus[SIFS_OutFrontReq].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// out front res
//	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0E_FRONT_OUT_STREAM))
//		m_stcIFStatus[SIFS_OutFrontRes].SetColorStyle(CVGBase_Static::ColorStyle_Red);
// 	else
// 		m_stcIFStatus[SIFS_OutFrontRes].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// out rear req
//	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0F_REAR_OUT_STREAM))
//		m_stcIFStatus[SIFS_OutRearReq].SetColorStyle(CVGBase_Static::ColorStyle_Green);
// 	else
// 		m_stcIFStatus[SIFS_OutRearReq].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);

	// out rear res
	//if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_0F_REAR_OUT_STREAM))
//		m_stcIFStatus[SIFS_OutRearRes].SetColorStyle(CVGBase_Static::ColorStyle_Red);
// 	else
// 		m_stcIFStatus[SIFS_OutRearRes].SetColorStyle(CVGBase_Static::ColorStyle_SlateGray);
}


void CDlgMain::Update_PASSNG_Status(INT On)
{

	if(On == 0)
	{
		//SEQUENCE->nPASS_NG = 2;


		m_st_if_PASS.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_if_NG.SetColorStyle(CVGStatic::ColorStyle_Red);

		if (SEQUENCE->StrNG == _T(""))
		{
			m_st_if_NG.SetText(_T("NG"));
		}
		else
		{
			m_st_if_NG.SetText(SEQUENCE->StrNG);
		}
	
		m_st_if_NG.ShowWindow(SW_SHOW);
		m_st_if_PASS.ShowWindow(SW_HIDE);

		//m_st_if_Result.ShowWindow(SW_SHOW);
		m_st_if_Result.ShowWindow(SW_HIDE);
// 		m_st_if_Result.SetText(_T("NG"));
// 		m_st_if_Result.SetColorStyle(CVGStatic::ColorStyle_Red);

		//	COMMON->DoEvent(500);
		
//		m_st_if_Result.ShowWindow(SW_HIDE);
	}
	else if (On == 1)
	{
		m_st_if_PASS.SetColorStyle(CVGStatic::ColorStyle_Green);
		m_st_if_NG.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_if_NG.SetText(_T("NG"));
		m_st_if_PASS.SetText(_T("PASS"));
		m_st_if_NG.ShowWindow(SW_HIDE);
		m_st_if_PASS.ShowWindow(SW_SHOW);

		//m_st_if_Result.ShowWindow(SW_SHOW);
		m_st_if_Result.ShowWindow(SW_HIDE);

		// 		m_st_if_Result.SetText(_T("PASS"));
		// 		m_st_if_Result.SetColorStyle(CVGStatic::ColorStyle_Green);

				//	COMMON->DoEvent(500);

				//COMMON->DoEvent(500);

		//		m_st_if_Result.ShowWindow(SW_HIDE);
	}
	else if(On == 2)
	{
		m_st_if_PASS.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_if_NG.SetColorStyle(CVGStatic::ColorStyle_DarkGray);

		m_st_if_NG.SetText(_T("READY"));
		m_st_if_NG.ShowWindow(SW_SHOW);
		m_st_if_PASS.ShowWindow(SW_HIDE);
		m_st_if_Result.ShowWindow(SW_HIDE);
	}
	else
	{
		m_st_if_PASS.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_if_NG.SetColorStyle(CVGStatic::ColorStyle_DarkGray);

		m_st_if_NG.SetText(_T("ING...."));
		m_st_if_NG.ShowWindow(SW_SHOW);
		m_st_if_PASS.ShowWindow(SW_HIDE);
		m_st_if_Result.ShowWindow(SW_HIDE);
	}



}
// Message End
//=======================================================================================================


void CDlgMain::OnBnClickedButton1()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
}


void CDlgMain::OnBnClickedButton2()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Ready);
}

void CDlgMain::OnBnClickedButton3()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStart);
}


void CDlgMain::OnBnClickedButton4()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_SFR);
}


void CDlgMain::OnBnClickedButton7()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_Insp_Blemisi);
}


void CDlgMain::OnBnClickedButton8()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStop);
}

void CDlgMain::OnBnClickedBtnYieldReset()
{
// 	CString strTemp;
// 	EQ_BASIC_PARAM	stEq_Cws = *DATAMANAGER->GetCurrentEqBasicData();
// 	if(stEq_Cws.nLanguage ==0)
// 		strTemp.Format(_T("일일생산량을 초기화 하시겠습니까?"));
// 	else
// 		strTemp.Format(_T("Do you want to reset the daily production?"));
// 
// 	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();
// 
// 	m_btnMainYield_Reset.SetFaceColor(COLOR_GREEN1, true);
// 	if (stBasic.nLanguage == 0)
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Kor[4]);
// 	else
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Eng[4]);
// 
// 	if (COMMON->ShowMsgDlg_Domodal(strTemp, MODE_INFORMATION) == TRUE)
// 	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			m_stMainYield_Title_Sub[i][1].SetStaticStyle(CVGStatic::StaticStyle_Data);
// 			m_stMainYield_Title_Sub[i][1].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			m_stMainYield_Title_Sub[i][1].SetFont_Gdip(L"Arial", 9.0F);
// 			m_stMainYield_Title_Sub[i][1].SetText(_T("0"));
// 		}
// 	}
// 
// 	if (stBasic.nLanguage == 0)
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Kor[4]);
// 	else
// 		m_btnMainYield_Reset.SetWindowText(m_szMainYield_Eng[4]);
}

void CDlgMain::OnBnClickedButton9()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[1].Connect_Tester(stTester.dwTesterIP[1], stTester.nTesterPort[1], stTester.strTesterConnect[1], 1);
}


void CDlgMain::OnBnClickedButton10()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[1].Send_Tester_Communication(eComMode_Ready);
}


void CDlgMain::OnBnClickedButton11()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[1].Send_Tester_Communication(eComMode_PreviewStart);
}


void CDlgMain::OnBnClickedButton12()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[1].Send_Tester_Communication(eComMode_Insp_SFR);
}


void CDlgMain::OnBnClickedButton13()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_UdpTester[1].Send_Tester_Communication(eComMode_Insp_Blemisi);
}


void CDlgMain::OnBnClickedButton14()
{
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
	m_UdpTester[1].Send_Tester_Communication(eComMode_PreviewStop);
}


// void CDlgMain::OnBnClickedPwspVoltSet1()
// {
// 	m_ctrlLight_ODA_Q[0].Write_Channel();
// 	if(m_bFlagPwsp[0] == FALSE)
// 	{
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Volt_12V();
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Current_1A();
// 		m_bFlagPwsp[0] = TRUE;
// 	}
// 	else
// 	{
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Volt_0V();
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Current_0A();
// 		m_bFlagPwsp[0] = FALSE;
// 	}
// }


// void CDlgMain::OnBnClickedPwspVoltSet2()
// {
// 	m_ctrlLight_ODA_Q[0].Write_Channel_2();
// 	if (m_bFlagPwsp[1] == FALSE)
// 	{
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Volt_12V();
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Current_1A();
// 		m_bFlagPwsp[1] = TRUE;
// 	}
// 	else
// 	{
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Volt_0V();
// 		COMMON->DoEvent(50);
// 		m_ctrlLight_ODA_Q[0].Write_Current_0A();
// 		m_bFlagPwsp[1] = FALSE;
// 	}
// }


//====================================================================================
// Method		: OnBnClickedBtnSemiauto1
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:39
// Desc.	   	: 세미오토 (left 대기위치 이동)
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto1()
{
	EqpTypeGetOption();

	if(COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 0);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 0);
}

//====================================================================================
// Method		: OnBnClickedBtnSemiauto2
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:42
// Desc.	   	: 세미 오토 대기위치 이동 (right)
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto2()
{
	EqpTypeGetOption();
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 3);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Right, 0);
}

//====================================================================================
// Method		: OnBnClickedBtnSemiauto3
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:41
// Desc.	   	: 소켓 그립 (left )
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto3()
{
	EqpTypeGetOption();
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 2);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 2);
}

//====================================================================================
// Method		: OnBnClickedBtnSemiauto4
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:43
// Desc.	   	: 세미 오토 시작 (right)
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto4()
{
	EqpTypeGetOption();
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 5);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Right, 2);
}


//====================================================================================
// Method		: OnBnClickedBtnSemiauto5
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:40
// Desc.	   	: 세미 오토 (left 그립 위치 이동)
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto5()
{
	EqpTypeGetOption();
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 1);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 1);
}


//====================================================================================
// Method		: OnBnClickedBtnSemiauto6
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:42
// Desc.	   	: 세미오토 그립 (right)
//=====================================================================================
void CDlgMain::OnBnClickedBtnSemiauto6()
{
	EqpTypeGetOption();
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
		SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 4);
	else
		SEQUENCE->Start_StartupCheck(InspectionPortType::Right, 1);
}

void CDlgMain::OnBnClickedBtnSemiauto7()
{
	EqpTypeGetOption();
// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 3);

}
void CDlgMain::OnBnClickedBtnSemiauto8()
{
	EqpTypeGetOption();
	// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
	// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 4);

}
void CDlgMain::OnBnClickedBtnSemiauto9()
{
	EqpTypeGetOption();
	// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
	// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 5);

}
void CDlgMain::OnBnClickedBtnSemiauto10()
{
	EqpTypeGetOption();
	// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
	// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 6);

}
void CDlgMain::OnBnClickedBtnSemiauto11()
{
	EqpTypeGetOption();
	// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
	// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 7);

}
void CDlgMain::OnBnClickedBtnSemiauto12()
{
	EqpTypeGetOption();
	// 	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	// 		SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 1);
	// 	else
	SEQUENCE->Start_StartupCheck(InspectionPortType::Left, 8);

}



void CDlgMain::OnBnClickedMvButton1()
{
	COMMON->SetMcStatus(eStop);
}

void CDlgMain::OnBnClickedMvButton2()
{
	COMMON->SetMcStatus(eRunning);
}


void CDlgMain::OnBnClickedMvButton3()
{
	COMMON->SetMcStatus(eError);
}


//====================================================================================
// Method		: OnBnClickedButtonSemiautoCancel
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/28 - 19:41
// Desc.	   	: 세미오토 Stop
//=====================================================================================
void CDlgMain::OnBnClickedButtonSemiautoCancel()
{
	if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
	{
		SEQUENCE->Stop_SemiAuto();
	}
	else
	{
		SEQUENCE->Stop_StartupCheck();
	}
}

BOOL CDlgMain::ShowError(__in enMCErrCode err)
{
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString sz;
	if (stEq.nLanguage == 0)
		sz = g_szMachineErrKor[err];
	else
		sz = g_szMachineErrEng[err];

	WRITE_ERR_LOG(sz);

	sz += _T("\r\n");
	if (pParrentDlg->RunMessageDlg_Domodal(sz, MODE_ERROR) == TRUE)
		return TRUE;
	
	return FALSE;
}

BOOL CDlgMain::ShowError(__in enMCErrCode err, __in CString szError)
{
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString sz;

	sz = szError;

	WRITE_ERR_LOG(sz);

	sz += _T("\r\n");
	if (pParrentDlg->RunMessageDlg_Domodal(sz, MODE_ERROR) == TRUE)
		return TRUE;

	return FALSE;
}

BOOL CDlgMain::ShowInformation(__in enMCErrCode err)
{
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString sz;
	if (stEq.nLanguage == 0)
		sz = g_szMachineErrKor[err];
	else
		sz = g_szMachineErrEng[err];

	sz += _T("\r\n");
	if (pParrentDlg->RunMessageDlg_Domodal(sz, MODE_INFORMATION) == TRUE)
		return TRUE;

	return FALSE;
}

//====================================================================================
// Method		: OnBnClickedBtnSemiauto7
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 14:09
// Desc.	   	: left 검사위치로 이동
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto7()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 3);
// }

//====================================================================================
// Method		: OnBnClickedBtnSemiauto9
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 14:09
// Desc.	   	: left 검사 시작
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto9()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 4);
// }

//====================================================================================
// Method		: OnBnClickedBtnSemiauto8
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 14:10
// Desc.	   	: left 소켓 배출
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto8()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Left, 5);
// }


//====================================================================================
// Method		: OnBnClickedBtnSemiauto10
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 15:09
// Desc.	   	: right 검사위치로 이동
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto10()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 3);
// }


//====================================================================================
// Method		: OnBnClickedBtnSemiauto12
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 15:09
// Desc.	   	: right 검사 시작
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto12()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 4);
// }


//====================================================================================
// Method		: OnBnClickedBtnSemiauto11
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2022/01/04 - 15:09
// Desc.	   	: right 소켓 배출 
//=====================================================================================
// void CDlgMain::OnBnClickedBtnSemiauto11()
// {
// 	SEQUENCE->Start_SemiAuto(InspectionPortType::Right, 5);
// }

void CDlgMain::OnBtnSemiAuto_Show(__in bool bShow)
{
	auto nShowCmd = (bShow) ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_BTN_SEMIAUTO1)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO2)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO3)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO4)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO5)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO6)->ShowWindow(nShowCmd);
	
}
void CDlgMain::OnBtnSemiAuto_Show_1(__in bool bShow)
{
	auto nShowCmd = (bShow) ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_BTN_SEMIAUTO7)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO8)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO9)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO10)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO11)->ShowWindow(nShowCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO12)->ShowWindow(SW_HIDE);
}

void CDlgMain::OnBtnSemiAuto_Enable(__in bool bEnable)
{
	auto nEnableCmd = (bEnable) ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_BTN_SEMIAUTO1)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO2)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO3)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO4)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO5)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO6)->EnableWindow(nEnableCmd);
}
void CDlgMain::OnBtnSemiAuto_Enable_1(__in bool bEnable)
{
	auto nEnableCmd = (bEnable) ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_BTN_SEMIAUTO7)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO8)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO9)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO10)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO11)->EnableWindow(nEnableCmd);
	GetDlgItem(IDC_BTN_SEMIAUTO12)->EnableWindow(SW_HIDE);
}
void CDlgMain::OnStatusSemiAuto_Reset()
{
	for (auto & ref : m_bnMainSemiAuto)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		ref.SetColorStyle(CVGStatic::ColorStyle_Black);
	}
}

void CDlgMain::OnStatusSemiAuto_Start(__in InspectionPortType port, __in int nIdx)
{
	switch (nIdx)
	{
	case 0:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[0].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[1].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;

	case 1:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[4].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[5].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[5].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;

	case 2:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[2].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[3].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;
	case 3:
		m_bnMainSemiAuto[6].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[6].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case 4:
		m_bnMainSemiAuto[7].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[7].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case 5:
		m_bnMainSemiAuto[8].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[8].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case 6:
		m_bnMainSemiAuto[9].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[9].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case 7:
		m_bnMainSemiAuto[10].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[10].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;
	case 8:
		m_bnMainSemiAuto[11].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_bnMainSemiAuto[11].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;

	default:
		break;
	}	
}

void CDlgMain::OnStatusSemiAuto_End(__in InspectionPortType port, __in int nIdx, __in bool bResultOk)
{
	for (auto & ref : m_bnMainSemiAuto)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		ref.SetColorStyle(CVGStatic::ColorStyle_Black);
	}

// 	switch (nIdx)
// 	{
// 	case 0:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[0].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[1].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	case 1:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[4].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[5].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[5].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	case 2:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[2].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[3].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CDlgMain::OnStatusStartupCheck_Reset()
{
	for (auto & ref : m_bnMainSemiAuto)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		ref.SetColorStyle(CVGStatic::ColorStyle_Black);
	}
}

void CDlgMain::OnStatusStartupCheck_Start(__in InspectionPortType port, __in int nIdx)
{
	switch (nIdx)
	{
	case 0:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[0].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[1].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;

	case 1:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[4].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[5].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[5].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;

	case 2:
		if (port == InspectionPortType::Left)
		{
			m_bnMainSemiAuto[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[2].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		else
		{
			m_bnMainSemiAuto[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_bnMainSemiAuto[3].SetColorStyle(CVGStatic::ColorStyle_Green);
		}
		break;

	default:
		break;
	}
}

void CDlgMain::OnStatusStartupCheck_End(__in InspectionPortType port, __in int nIdx, __in bool bResultOk)
{
	for (auto & ref : m_bnMainSemiAuto)
	{
		ref.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		ref.SetColorStyle(CVGStatic::ColorStyle_Black);
	}
	
// 	switch (nIdx)
// 	{
// 	case 0:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[0].SetColorStyle(CVGStatic::ColorStyle_Green);			
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[1].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[1].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	case 1:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[4].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[4].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[5].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[5].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	case 2:
// 		if (port == InspectionPortType::Left)
// 		{
// 			m_bnMainSemiAuto[2].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[2].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		else
// 		{
// 			m_bnMainSemiAuto[3].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 			m_bnMainSemiAuto[3].SetColorStyle(CVGStatic::ColorStyle_Green);
// 		}
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CDlgMain::OnUpdate_CL200A_Data(__in InspectionPortType port, __in float fData, __in bool bReadOk)
{
	auto * pDlg = static_cast<CDlgHotPixelMain*>(m_pInspectorMainDlg);
	pDlg->SetCL200A(port, fData, bReadOk);
}

void CDlgMain::OnUpdate_CL200A_Reset(__in InspectionPortType port)
{
	auto * pDlg = static_cast<CDlgHotPixelMain*>(m_pInspectorMainDlg);
	pDlg->ResetCL200A(port);
}

void CDlgMain::OnUpdate_CL500A_Data(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk)
{
	auto * pDlg = static_cast<CDlgColorCalMain*>(m_pInspectorMainDlg);
	pDlg->SetCL500A(port, nLightIdx, fColorTemper, fData, bReadOk);
}

void CDlgMain::OnUpdate_CL500A_Reset(__in InspectionPortType port)
{
	auto * pDlg = static_cast<CDlgColorCalMain*>(m_pInspectorMainDlg);
	pDlg->ResetCL500A(port);
}

void CDlgMain::OnBnClickedBtnPopupTest()
{
	CWnd *Wnd = GetDlgItem(IDC_STC_VM_SOCKET_INFO);

	WINDOWPLACEMENT wp;
	CRect rt;
	Wnd->GetWindowPlacement(&wp);
	Wnd->GetWindowRect(&rt);

	int w, h;
	w = 528;
	h = 543;
	rt.right = rt.left + w;
	rt.bottom = rt.top + h;

	CDlgSiMain dlg(this, rt);
	dlg.DoModal();
}

void CDlgMain::EqpTypeGetOption()
{
//	auto EqpType = SET_INSPECTOR;

	ST_OptSemiAuto opt;
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 	{
// 		auto * pDlg = static_cast<CDlgColorCalMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_DISTORTION:
// 	{
		auto * pDlg = static_cast<CDlgDistortionMain*>(m_pInspectorMainDlg);
		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_GHOSTFLARE:
// 	{
// 		auto * pDlg = static_cast<CDlgGhostFlareMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_HOTPIXEL:
// 	{
// 		auto * pDlg = static_cast<CDlgHotPixelMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_SFR_NARROW:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRNarrowMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_SFR_WIDE_H:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRWideHMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	case SYS_SFR_WIDE_V:
// 	{
// 		auto * pDlg = static_cast<CDlgSFRWideVMain*>(m_pInspectorMainDlg);
// 		opt = pDlg->GetOption();
// 	}
// 	break;
// 
// 	default:
// 		break;
// 	}

	DATAMANAGER->SetSemiAuto(opt);
}



void CDlgMain::OnStnClickedStaticMainflowBmp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
