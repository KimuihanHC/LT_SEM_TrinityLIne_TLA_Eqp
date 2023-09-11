// DlgMotorTeach.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgMotorTeach.h"
#include "afxdialogex.h"
#include "DlgSaveAsModel.h"
#include "DlgManualMove.h"
#include "Resource.h"

// CDlgMotorTeach 대화 상자
#define ON 1
#define OFF 0
IMPLEMENT_DYNAMIC(CDlgMotorTeach, CDialogEx)

CDlgMotorTeach::CDlgMotorTeach(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MOTOR_TEACHING, pParent)
{
	m_nJogMode				= 0;
	m_nSelectAxisNum		= 0;
	m_nSelectDistance		= 0;
	m_nSelectSpeed			= 0;
	m_dwOldAxisStatus		= 0;
	m_dMoveDistance			= 0.0f;
	m_nSetSpeed_AxisNum		= 0;
	m_nSampleModelType		= 0;
	m_bJogBtnBlockFlag = FALSE;
	m_nSelectAxisNumBK = -1;
	//m_pDlgManualMove = new CDlgManualMove(AfxGetApp()->GetMainWnd(), this);
	//m_pDlgManualMove->Create(IDD_DLG_MANUAL_MOVE);
	for (int i = 0; i < 16; i++)
	{
		m_nTimeSensor[i] = 100;				// 센서 상태
		m_nTimeSensorBK[i] = 10;				// 센서 상태
	}
	m_BarcodeSt = 100;
	m_BarcodeStBK = 10;
	m_Barcode = 1;
}

CDlgMotorTeach::~CDlgMotorTeach()
{
	//m_pDlgManualMove->DestroyWindow();
	//delete m_pDlgManualMove;
}

void CDlgMotorTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_1, m_stMotorTitle[0]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_2, m_stMotorTitle[1]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_3, m_stMotorTitle[2]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_4, m_stMotorTitle[3]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_5, m_stMotorTitle[4]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_6, m_stMotorTitle[5]);
	DDX_Control(pDX, IDC_STC_MOTOR_TITLE_7, m_stMotorTitle[6]);

	DDX_Control(pDX, IDC_STC_AXIS_1,		m_stMotorCurPos[0][0]);
	DDX_Control(pDX, IDC_STC_AXIS_2,		m_stMotorCurPos[0][1]);
	DDX_Control(pDX, IDC_STC_AXIS_3,		m_stMotorCurPos[0][2]);
	DDX_Control(pDX, IDC_STC_AXIS_4,		m_stMotorCurPos[0][3]);
	DDX_Control(pDX, IDC_STC_AXIS_5,		m_stMotorCurPos[0][4]);
// 	DDX_Control(pDX, IDC_STC_AXIS_6,		m_stMotorCurPos[0][5]);
// 	DDX_Control(pDX, IDC_STC_AXIS_7,		m_stMotorCurPos[0][6]);
// 	DDX_Control(pDX, IDC_STC_AXIS_8,		m_stMotorCurPos[0][7]);
// 	DDX_Control(pDX, IDC_STC_AXIS_9,		m_stMotorCurPos[0][8]);
// 	DDX_Control(pDX, IDC_STC_AXIS_10,		m_stMotorCurPos[0][9]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_1, m_stMotorCurPos[1][0]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_2, m_stMotorCurPos[1][1]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_3, m_stMotorCurPos[1][2]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_4, m_stMotorCurPos[1][3]);
	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_5, m_stMotorCurPos[1][4]);
// 	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_6, m_stMotorCurPos[1][5]);
// 	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_7, m_stMotorCurPos[1][6]);
// 	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_8, m_stMotorCurPos[1][7]);
// 	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_9, m_stMotorCurPos[1][8]);
// 	DDX_Control(pDX, IDC_STC_AXIS_CURPOS_10, m_stMotorCurPos[1][9]);
	
	DDX_Control(pDX, IDC_STC_JOG_SELECT_1, m_stMotorJogTitleSub[0]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_2, m_stMotorJogTitleSub[1]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_3, m_stMotorJogTitleSub[2]);
	DDX_Control(pDX, IDC_STC_JOG_SELECT_4, m_stMotorJogTitleSub[3]);

	DDX_Control(pDX, IDC_BTN_JOG_SELECT_MODE_1, m_btnMotorJogSelect_Mode[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_MODE_2, m_btnMotorJogSelect_Mode[1]);

	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_1, m_btnMotorJogSelect_Axis[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_2, m_btnMotorJogSelect_Axis[1]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_3, m_btnMotorJogSelect_Axis[2]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_4, m_btnMotorJogSelect_Axis[3]);
	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_5, m_btnMotorJogSelect_Axis[4]);
//	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_6, m_btnMotorJogSelect_Axis[5]);
// 	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_7, m_btnMotorJogSelect_Axis[6]);
// 	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_8, m_btnMotorJogSelect_Axis[7]);
// 	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_9, m_btnMotorJogSelect_Axis[8]);
// 	DDX_Control(pDX, IDC_BTN_JOG_SELECT_AXIS_10, m_btnMotorJogSelect_Axis[9]);

	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_1, m_btnMotorJogSelect_Distance[0]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_2, m_btnMotorJogSelect_Distance[1]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_3, m_btnMotorJogSelect_Distance[2]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_4, m_btnMotorJogSelect_Distance[3]);
	DDX_Control(pDX, IDC_BTN_JOG_DISTANCE_5, m_btnMotorJogSelect_Distance[4]);

	DDX_Control(pDX, IDC_BTN_JOG_SPEED_1, m_btnMotorJogSelect_Speed[0]);
	DDX_Control(pDX, IDC_BTN_JOG_SPEED_2, m_btnMotorJogSelect_Speed[1]);
	DDX_Control(pDX, IDC_BTN_JOG_SPEED_3, m_btnMotorJogSelect_Speed[2]);

	DDX_Control(pDX, IDC_BTN_JOG_MOVE_1, m_btnMotorJogSelect_Move[0]);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_2, m_btnMotorJogSelect_Move[1]);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_3, m_btnMotorJogSelect_Home);

	DDX_Text(pDX, IDC_EDIT1, m_strInput_JogDistance);
	DDX_Control(pDX, IDC_EDIT1, m_edInput_JogDistance);


	DDX_Control(pDX, IDC_BUTTON8, m_Nobarcode);
	DDX_Control(pDX, IDC_BUTTON9, m_Setbarcode);

	
	//door
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK1, m_stDoorLockManual[0]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK2, m_stDoorLockManual[1]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK3, m_stDoorLockManual[2]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK4, m_stDoorLockManual[3]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK5, m_stDoorLockManual[4]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK6, m_stDoorLockManual[5]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK7, m_stDoorLockManual[6]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK8, m_stDoorLockManual[7]);
	DDX_Control(pDX, IDC_STC_TEACH_DOORLOCK9, m_stDoorLockManual[8]);

	DDX_Control(pDX, IDC_BTN_DOLOCK1, m_btnDoorLockManual[0][0]);
	DDX_Control(pDX, IDC_BTN_DOLOCK2, m_btnDoorLockManual[0][1]);
	DDX_Control(pDX, IDC_BTN_DOLOCK3, m_btnDoorLockManual[0][2]);
	DDX_Control(pDX, IDC_BTN_DOLOCK4, m_btnDoorLockManual[0][3]);
	DDX_Control(pDX, IDC_BTN_DOLOCK5, m_btnDoorLockManual[0][4]);
	DDX_Control(pDX, IDC_BTN_DOLOCK6, m_btnDoorLockManual[0][5]);
	DDX_Control(pDX, IDC_BTN_DOLOCK7, m_btnDoorLockManual[0][6]);
	DDX_Control(pDX, IDC_BTN_DOLOCK8, m_btnDoorLockManual[0][7]);
	DDX_Control(pDX, IDC_BTN_DOLOCK9, m_btnDoorLockManual[0][8]);

	DDX_Control(pDX, IDC_BTN_DOUNLOCK1, m_btnDoorLockManual[1][0]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK2, m_btnDoorLockManual[1][1]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK3, m_btnDoorLockManual[1][2]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK4, m_btnDoorLockManual[1][3]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK5, m_btnDoorLockManual[1][4]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK6, m_btnDoorLockManual[1][5]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK7, m_btnDoorLockManual[1][6]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK8, m_btnDoorLockManual[1][7]);
	DDX_Control(pDX, IDC_BTN_DOUNLOCK9, m_btnDoorLockManual[1][8]);
	//door

	DDX_Control(pDX, IDC_STC_SENSOR_001, m_stSensorManual[0]);
	DDX_Control(pDX, IDC_STC_SENSOR_002, m_stSensorManual[1]);
	DDX_Control(pDX, IDC_STC_SENSOR_003, m_stSensorManual[2]);
	DDX_Control(pDX, IDC_STC_SENSOR_004, m_stSensorManual[3]);
	DDX_Control(pDX, IDC_STC_SENSOR_005, m_stSensorManual[4]);
	DDX_Control(pDX, IDC_STC_SENSOR_006, m_stSensorManual[5]);
	DDX_Control(pDX, IDC_STC_SENSOR_007, m_stSensorManual[6]);
	DDX_Control(pDX, IDC_STC_SENSOR_008, m_stSensorManual[7]);
	DDX_Control(pDX, IDC_STC_SENSOR_009, m_stSensorManual[8]);
	DDX_Control(pDX, IDC_STC_SENSOR_010, m_stSensorManual[9]);
	DDX_Control(pDX, IDC_STC_SENSOR_011, m_stSensorManual[10]);
	DDX_Control(pDX, IDC_STC_SENSOR_012, m_stSensorManual[11]);
	DDX_Control(pDX, IDC_STC_SENSOR_013, m_stSensorManual[12]);
	DDX_Control(pDX, IDC_STC_SENSOR_014, m_stSensorManual[13]);
	DDX_Control(pDX, IDC_STC_SENSOR_015, m_stSensorManual[14]);
	DDX_Control(pDX, IDC_STC_SENSOR_016, m_stSensorManual[15]);

	DDX_Control(pDX, IDC_BTN_SENSOR_001, m_stSensorChk[0]);
	DDX_Control(pDX, IDC_BTN_SENSOR_002, m_stSensorChk[1]);
	DDX_Control(pDX, IDC_BTN_SENSOR_003, m_stSensorChk[2]);
	DDX_Control(pDX, IDC_BTN_SENSOR_004, m_stSensorChk[3]);
	DDX_Control(pDX, IDC_BTN_SENSOR_005, m_stSensorChk[4]);
	DDX_Control(pDX, IDC_BTN_SENSOR_006, m_stSensorChk[5]);
	DDX_Control(pDX, IDC_BTN_SENSOR_007, m_stSensorChk[6]);
	DDX_Control(pDX, IDC_BTN_SENSOR_008, m_stSensorChk[7]);
	DDX_Control(pDX, IDC_BTN_SENSOR_009, m_stSensorChk[8]);
	DDX_Control(pDX, IDC_BTN_SENSOR_010, m_stSensorChk[9]);
	DDX_Control(pDX, IDC_BTN_SENSOR_011, m_stSensorChk[10]);
	DDX_Control(pDX, IDC_BTN_SENSOR_012, m_stSensorChk[11]);
	DDX_Control(pDX, IDC_BTN_SENSOR_013, m_stSensorChk[12]);
	DDX_Control(pDX, IDC_BTN_SENSOR_014, m_stSensorChk[13]);
	DDX_Control(pDX, IDC_BTN_SENSOR_015, m_stSensorChk[14]);
	DDX_Control(pDX, IDC_BTN_SENSOR_016, m_stSensorChk[15]);




	DDX_Control(pDX, IDC_STC_CYLINDER_1, m_stMotorManual[0]);
	DDX_Control(pDX, IDC_STC_CYLINDER_2, m_stMotorManual[1]);
	DDX_Control(pDX, IDC_STC_CYLINDER_3, m_stMotorManual[2]);
	DDX_Control(pDX, IDC_STC_CYLINDER_4, m_stMotorManual[3]);
	DDX_Control(pDX, IDC_STC_CYLINDER_5, m_stMotorManual[4]);
//	DDX_Control(pDX, IDC_STC_CYLINDER_6, m_stMotorManual[5]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_7, m_stMotorManual[6]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_8, m_stMotorManual[7]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_9, m_stMotorManual[8]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_10, m_stMotorManual[9]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_11, m_stMotorManual[10]);
// 	DDX_Control(pDX, IDC_STC_CYLINDER_12, m_stMotorManual[11]);

	DDX_Control(pDX, IDC_BTN_CYLINDER_1, m_btnMotorManual[0][0]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_2, m_btnMotorManual[0][1]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_3, m_btnMotorManual[0][2]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_4, m_btnMotorManual[0][3]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_5, m_btnMotorManual[0][4]);
//	DDX_Control(pDX, IDC_BTN_CYLINDER_6, m_btnMotorManual[0][5]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_7, m_btnMotorManual[0][6]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_8, m_btnMotorManual[0][7]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_9, m_btnMotorManual[0][8]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_10, m_btnMotorManual[0][9]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_11, m_btnMotorManual[0][10]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_12, m_btnMotorManual[0][11]);

	DDX_Control(pDX, IDC_BTN_CYLINDER_13, m_btnMotorManual[1][0]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_14, m_btnMotorManual[1][1]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_15, m_btnMotorManual[1][2]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_16, m_btnMotorManual[1][3]);
	DDX_Control(pDX, IDC_BTN_CYLINDER_17, m_btnMotorManual[1][4]);
//	DDX_Control(pDX, IDC_BTN_CYLINDER_18, m_btnMotorManual[1][5]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_19, m_btnMotorManual[1][6]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_20, m_btnMotorManual[1][7]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_21, m_btnMotorManual[1][8]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_22, m_btnMotorManual[1][9]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_23, m_btnMotorManual[1][10]);
// 	DDX_Control(pDX, IDC_BTN_CYLINDER_24, m_btnMotorManual[1][11]);

// 	DDX_Control(pDX, IDC_COMBO_PORTNO1, m_cbFASBoard_Port);
// 	DDX_Control(pDX, IDC_COMBO_BAUDRATE1, m_cbFASBoard_BaudRate);


// 	DDX_Control(pDX, IDC_BTN_MOTOR_CONNECT1,	m_btnMotorConnect[0]);
// 	DDX_Control(pDX, IDC_BTN_MOTOR_DISCONNECT1, m_btnMotorConnect[1]);

	DDX_Control(pDX, IDC_BTN_MOTOR_SERVO_ON,		m_btnMotorState[0]);
	DDX_Control(pDX, IDC_BTN_MOTOR_SERVO_OFF,	m_btnMotorState[1]);
	DDX_Control(pDX, IDC_BTN_MOTOR_STOP,				m_btnMotorState[2]);
	DDX_Control(pDX, IDC_BTN_MOTOR_ALARMRESET,	m_btnMotorState[3]);
	
	
	DDX_Control(pDX, IDC_STC_SPEED_AXIS_1, m_stMotorSpeedSet_Sub[0]);
	DDX_Control(pDX, IDC_STC_SPEED_AXIS_2, m_stMotorSpeedSet_Sub[1]);

	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_1, m_btnMotorSpeedSet_AxisNum[0]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_2, m_btnMotorSpeedSet_AxisNum[1]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_3, m_btnMotorSpeedSet_AxisNum[2]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_4, m_btnMotorSpeedSet_AxisNum[3]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_5, m_btnMotorSpeedSet_AxisNum[4]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_6, m_btnMotorSpeedSet_AxisNum[5]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_7, m_btnMotorSpeedSet_AxisNum[6]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_8, m_btnMotorSpeedSet_AxisNum[7]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_9, m_btnMotorSpeedSet_AxisNum[8]);
	DDX_Control(pDX, IDC_BTN_SPEED_AXIS_SELECT_10, m_btnMotorSpeedSet_AxisNum[9]);
	
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_1, m_stMotorSpeedSet_Thirdsub[0][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_2, m_stMotorSpeedSet_Thirdsub[1][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_3, m_stMotorSpeedSet_Thirdsub[2][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_4, m_stMotorSpeedSet_Thirdsub[3][0]);
	DDX_Control(pDX, IDC_STC_SPEED_TITLE_5, m_stMotorSpeedSet_Thirdsub[4][0]);

	DDX_Control(pDX, IDC_STC_SPEED_SETTING_1, m_stMotorSpeedSet_Thirdsub[0][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_2, m_stMotorSpeedSet_Thirdsub[1][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_3, m_stMotorSpeedSet_Thirdsub[2][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_4, m_stMotorSpeedSet_Thirdsub[3][1]);
	DDX_Control(pDX, IDC_STC_SPEED_SETTING_5, m_stMotorSpeedSet_Thirdsub[4][1]);


	DDX_Control(pDX, IDC_STATIC_SHOW_MOTORNUM2, m_stStatic_Text[0]);

	for (int nState = 0; nState < AXIS_MOTOR_STATUES; nState++)
	{
		DDX_Control(pDX, IDC_STATIC_MOTOR_STATUS1 + nState, m_stStatic_Motor_Statuses[nState]);
	}
	//teaching part
		//TITLE
	DDX_Control(pDX, IDC_STC_VIGNETING_X, m_stTeach_Vigneting_X);
	DDX_Control(pDX, IDC_STC_VIGNETING_R, m_stTeach_Vigneting_R);
	DDX_Control(pDX, IDC_STC_ADJUST, m_stTeach_Adjust);
	DDX_Control(pDX, IDC_STC_SCREW_L, m_stTeach_Screw_L);
	DDX_Control(pDX, IDC_STC_SCREW_R, m_stTeach_Screw_R);
	DDX_Control(pDX, IDC_STC_ADJUST2, m_stTeach_AdjustSpec);

	DDX_Control(pDX, IDC_STC_SCREW, m_stTeach_Screw);
	

		//sub
	DDX_Control(pDX, IDC_STC_A_S_POS_01, m_stTeach_Adjust_Spec_Sub[0]);
	DDX_Control(pDX, IDC_STC_A_S_POS_02, m_stTeach_Adjust_Spec_Sub[1]);
	DDX_Control(pDX, IDC_STC_A_S_POS_03, m_stTeach_Adjust_Spec_Sub[2]);
	DDX_Control(pDX, IDC_STC_A_S_POS_04, m_stTeach_Adjust_Spec_Sub[3]);
	DDX_Control(pDX, IDC_STC_A_S_POS_05, m_stTeach_Adjust_Spec_Sub[4]);
	DDX_Control(pDX, IDC_STC_A_S_POS_06, m_stTeach_Adjust_Spec_Sub[5]);
	
	DDX_Control(pDX, IDC_STC_X_POS_1, m_stTeach_Vigneting_X_Sub[0]);
	DDX_Control(pDX, IDC_STC_X_POS_2, m_stTeach_Vigneting_X_Sub[1]);
	DDX_Control(pDX, IDC_STC_X_POS_3, m_stTeach_Vigneting_X_Sub[2]);

	DDX_Control(pDX, IDC_STC_R_POS_1, m_stTeach_Vigneting_R_Sub[0]);
	DDX_Control(pDX, IDC_STC_R_POS_2, m_stTeach_Vigneting_R_Sub[1]);
	DDX_Control(pDX, IDC_STC_R_POS_3, m_stTeach_Vigneting_R_Sub[2]);

	DDX_Control(pDX, IDC_STC_A_POS_1, m_stTeach_Adjust_Sub[0]);


	DDX_Control(pDX, IDC_STC_SCREW_L_POS_1, m_stTeach_Screw_L_Sub[0]);
	DDX_Control(pDX, IDC_STC_SCREW_L_POS_2, m_stTeach_Screw_L_Sub[1]);
	DDX_Control(pDX, IDC_STC_SCREW_L_POS_3, m_stTeach_Screw_L_Sub[2]);

	DDX_Control(pDX, IDC_STC_SCREW_R_POS_1, m_stTeach_Screw_R_Sub[0]);
	DDX_Control(pDX, IDC_STC_SCREW_R_POS_2, m_stTeach_Screw_R_Sub[1]);
	DDX_Control(pDX, IDC_STC_SCREW_R_POS_3, m_stTeach_Screw_R_Sub[2]);
		//EDIT
	DDX_Text(pDX, IDC_EDIT_X_POS_1, m_strTeach_Vigneting_X_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_X_POS_1, m_edTeach_Vigneting_X_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_X_POS_2, m_strTeach_Vigneting_X_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_X_POS_2, m_edTeach_Vigneting_X_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_X_POS_3, m_strTeach_Vigneting_X_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_X_POS_3, m_edTeach_Vigneting_X_Pos[2]);

	DDX_Text(pDX, IDC_EDIT_R_POS_1, m_strTeach_Vigneting_R_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_R_POS_1, m_edTeach_Vigneting_R_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_R_POS_2, m_strTeach_Vigneting_R_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_R_POS_2, m_edTeach_Vigneting_R_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_R_POS_3, m_strTeach_Vigneting_R_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_R_POS_3, m_edTeach_Vigneting_R_Pos[2]);

	DDX_Text(pDX, IDC_EDIT_A_POS_1, m_strTeach_Adjust_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_A_POS_1, m_edTeach_Adjust_Pos[0]);

	DDX_Text(pDX, IDC_EDIT_A_S_POS_01, m_strTeach_Adjust_Spec_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_01, m_edTeach_Adjust_Spec_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_A_S_POS_02, m_strTeach_Adjust_Spec_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_02, m_edTeach_Adjust_Spec_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_A_S_POS_03, m_strTeach_Adjust_Spec_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_03, m_edTeach_Adjust_Spec_Pos[2]);

	DDX_Text(pDX, IDC_EDIT_A_S_POS_04, m_strTeach_Adjust_Spec_Pos[3]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_04, m_edTeach_Adjust_Spec_Pos[3]);

	DDX_Text(pDX, IDC_EDIT_A_S_POS_05, m_strTeach_Adjust_Spec_Pos[4]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_05, m_edTeach_Adjust_Spec_Pos[4]);

	DDX_Text(pDX, IDC_EDIT_A_S_POS_06, m_strTeach_Adjust_Spec_Pos[5]);
	DDX_Control(pDX, IDC_EDIT_A_S_POS_06, m_edTeach_Adjust_Spec_Pos[5]);
	
		

	DDX_Text(pDX, IDC_EDIT_SCREW_L_POS_1, m_strTeach_Screw_L_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_SCREW_L_POS_1, m_edTeach_Screw_L_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_SCREW_L_POS_2, m_strTeach_Screw_L_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_SCREW_L_POS_2, m_edTeach_Screw_L_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_SCREW_L_POS_3, m_strTeach_Screw_L_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_SCREW_L_POS_3, m_edTeach_Screw_L_Pos[2]);

	DDX_Text(pDX, IDC_EDIT_SCREW_R_POS_1, m_strTeach_Screw_R_Pos[0]);
	DDX_Control(pDX, IDC_EDIT_SCREW_R_POS_1, m_edTeach_Screw_R_Pos[0]);
	DDX_Text(pDX, IDC_EDIT_SCREW_R_POS_2, m_strTeach_Screw_R_Pos[1]);
	DDX_Control(pDX, IDC_EDIT_SCREW_R_POS_2, m_edTeach_Screw_R_Pos[1]);
	DDX_Text(pDX, IDC_EDIT_SCREW_R_POS_3, m_strTeach_Screw_R_Pos[2]);
	DDX_Control(pDX, IDC_EDIT_SCREW_R_POS_3, m_edTeach_Screw_R_Pos[2]);
}

BEGIN_MESSAGE_MAP(CDlgMotorTeach, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_MODE_1, &CDlgMotorTeach::OnBnClickedBtnJogSelectMode1)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_MODE_2, &CDlgMotorTeach::OnBnClickedBtnJogSelectMode2)

	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_1, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis1)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_2, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis2)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_3, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis3)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_4, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis4)
	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_5, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis5)
// 	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_6, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis6)
// 	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_7, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis7)
// 	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_8, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis8)
// 	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_9, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis9)
// 	ON_BN_CLICKED(IDC_BTN_JOG_SELECT_AXIS_10, &CDlgMotorTeach::OnBnClickedBtnJogSelectAxis10)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_1, &CDlgMotorTeach::OnBnClickedBtnJogDistance1)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_2, &CDlgMotorTeach::OnBnClickedBtnJogDistance2)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_3, &CDlgMotorTeach::OnBnClickedBtnJogDistance3)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_4, &CDlgMotorTeach::OnBnClickedBtnJogDistance4)
	ON_BN_CLICKED(IDC_BTN_JOG_DISTANCE_5, &CDlgMotorTeach::OnBnClickedBtnJogDistance5)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_1, &CDlgMotorTeach::OnBnClickedBtnJogSpeed1)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_2, &CDlgMotorTeach::OnBnClickedBtnJogSpeed2)
	ON_BN_CLICKED(IDC_BTN_JOG_SPEED_3, &CDlgMotorTeach::OnBnClickedBtnJogSpeed3)

	ON_BN_CLICKED(IDC_BTN_JOG_MOVE_3, &CDlgMotorTeach::OnBnClickedBtnJogMove3)			// 원점 복귀



	ON_BN_CLICKED(IDC_BTN_DOLOCK1, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK2, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK3, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK4, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK5, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK6, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK7, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK8, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOLOCK9, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK1, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK2, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK3, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK4, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK5, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK6, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK7, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK8, &CDlgMotorTeach::CylinderManual_Acitve)
	ON_BN_CLICKED(IDC_BTN_DOUNLOCK9, &CDlgMotorTeach::CylinderManual_Acitve)
	

	
	
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_1, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect1)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_2, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect2)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_3, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect3)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_4, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect4)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_5, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect5)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_6, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect6)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_7, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect7)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_8, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect8)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_9, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect9)
	ON_BN_CLICKED(IDC_BTN_SPEED_AXIS_SELECT_10, &CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect10)

	//ON_CONTROL_RANGE(BN_CLICKED, IDC_STC_SPEED_SETTING_1, IDC_STC_SPEED_SETTING_5, &CDlgMotorTeach::MotorSpeedChange )
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_1, &CDlgMotorTeach::OnStnClickedStcSpeedSetting1)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_2, &CDlgMotorTeach::OnStnClickedStcSpeedSetting2)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_3, &CDlgMotorTeach::OnStnClickedStcSpeedSetting3)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_4, &CDlgMotorTeach::OnStnClickedStcSpeedSetting4)
	ON_STN_CLICKED(IDC_STC_SPEED_SETTING_5, &CDlgMotorTeach::OnStnClickedStcSpeedSetting5)

	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_1, &CDlgMotorTeach::OnBnClickedBtnTeachSave1)
	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_2, &CDlgMotorTeach::OnBnClickedBtnTeachSave2)
	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE_3, &CDlgMotorTeach::OnBnClickedBtnTeachSave3)
// 	ON_BN_CLICKED(IDC_BTN_MOTOR_CONNECT1, &CDlgMotorTeach::OnBnClickedBtnMotorConnect1)
// 	ON_BN_CLICKED(IDC_BTN_MOTOR_DISCONNECT1, &CDlgMotorTeach::OnBnClickedBtnMotorDisconnect1)
	ON_BN_CLICKED(IDC_BTN_MOTOR_SERVO_ON, &CDlgMotorTeach::OnBnClickedBtnMotorServoOn)
	ON_BN_CLICKED(IDC_BTN_MOTOR_SERVO_OFF, &CDlgMotorTeach::OnBnClickedBtnMotorServoOff)
	ON_BN_CLICKED(IDC_BTN_MOTOR_STOP, &CDlgMotorTeach::OnBnClickedBtnMotorStop)
	ON_BN_CLICKED(IDC_BTN_MOTOR_ALARMRESET, &CDlgMotorTeach::OnBnClickedBtnMotorAlarmreset)
	ON_WM_TIMER()
	
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_CHART_01, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Chart_Z1)
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_CHART_02, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Chart_Z2)
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_CHART_03, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Chart_Z3)
// 	
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_CHART_01, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveZChart01)
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_CHART_02, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveZChart02)
// 	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_CHART_03, &CDlgMotorTeach::OnBnClickedBtnTeachingMoveZChart03)
	
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_X_1, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_X)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_X_2, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_X)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_X_3, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_X)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_X_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_X)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_X_2, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_X)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_X_3, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_X)

	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_R_1, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_R_2, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_R_3, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_R_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_R_2, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_R_3, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_R)

	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_L_1, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_L)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_L_2, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_L)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_L_3, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_L)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_L_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_L)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_L_2, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_L)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_L_3, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_L)

	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_R_1, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_R_2, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_POS_SCREW_R_3, &CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_R_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_R_2, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_SCREW_R_3, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_R)
	ON_BN_CLICKED(IDC_BTN_TEACHING_MOVE_A_1, &CDlgMotorTeach::OnBnClickedBtnTeachingMove_Adjust)
	

	ON_BN_CLICKED(IDC_BTN_CYLINDER_1, &CDlgMotorTeach::OnBnClickedBtnCylinder1)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_13, &CDlgMotorTeach::OnBnClickedBtnCylinder13)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_2, &CDlgMotorTeach::OnBnClickedBtnCylinder2)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_14, &CDlgMotorTeach::OnBnClickedBtnCylinder14)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_3, &CDlgMotorTeach::OnBnClickedBtnCylinder3)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_15, &CDlgMotorTeach::OnBnClickedBtnCylinder15)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_4, &CDlgMotorTeach::OnBnClickedBtnCylinder4)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_16, &CDlgMotorTeach::OnBnClickedBtnCylinder16)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_5, &CDlgMotorTeach::OnBnClickedBtnCylinder5)
	ON_BN_CLICKED(IDC_BTN_CYLINDER_17, &CDlgMotorTeach::OnBnClickedBtnCylinder17)



	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_UNLOCK, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK1, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK2, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2)

	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_UNLOCK2, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock_L)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK3, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1_L)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK5, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2_L)

	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_UNLOCK3, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock_R)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK6, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1_R)
	ON_BN_CLICKED(IDC_BTN_SCREW_CONTROL_LOCK7, &CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2_R)



	ON_BN_CLICKED(IDC_BTN_SCREW_MOVE_01, &CDlgMotorTeach::OnStnClickedStcScrew_MoveControl)
	ON_BN_CLICKED(IDC_BTN_SCREW_MOVE_02, &CDlgMotorTeach::OnStnClickedStcScrew_MoveControl)
	ON_BN_CLICKED(IDC_BTN_SCREW_MOVE_03, &CDlgMotorTeach::OnStnClickedStcScrew_MoveControl)


		


	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMotorTeach::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMotorTeach::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgMotorTeach::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgMotorTeach::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgMotorTeach::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgMotorTeach::OnBnClickedButton7)



	ON_BN_CLICKED(IDC_BTN_TEACH_VIEW_MANUAL, &CDlgMotorTeach::OnBnClickedBtnTeachViewManual)
		ON_BN_CLICKED(IDC_BUTTON8, &CDlgMotorTeach::OnBnClickedButton8)
		ON_BN_CLICKED(IDC_BUTTON9, &CDlgMotorTeach::OnBnClickedButton9)
		END_MESSAGE_MAP()

BOOL CDlgMotorTeach::PreTranslateMessage(MSG* pMsg)
{
	CString strLog = _T("");
	double dCurPos = 0.0f;
	double dTargetPos = 0.0f;
	long   lAxisNum = 0;

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_1)->GetSafeHwnd())
		{
			UpdateData(TRUE);

			if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
			{
				if (AJIN_MO->IsMotion(m_nSelectAxisNum))
				{
					return FALSE;
				}
				int nVel = 0;

				MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();

// 				if (m_nSelectSpeed == 0)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedSlow];
// 				}
// 				else if (m_nSelectSpeed == 1)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedMiddle];
// 				}
// 				else if (m_nSelectSpeed == 2)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedHigh];
// 				}
// 				else
// 				{
					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedHigh];
	//			}
				if (m_nJogMode == 1)
				{
					dCurPos = AJIN_MO->GetActPos(m_nSelectAxisNum);
					if (m_nSelectAxisNum == 4)
					{
						dCurPos = dCurPos / 1000;
						dTargetPos = dCurPos - m_dMoveDistance;
					}
					else
					{
						dCurPos = dCurPos / 1000;
						dTargetPos = dCurPos - m_dMoveDistance;
					}
// 					if (m_nSelectAxisNum == 4)
// 					{

						if (!AJIN_MO->MoveAxis_NonWait(m_nSelectAxisNum, dTargetPos, nVel))
						{
							return FALSE;
						}
// 					}
// 					else
// 					{
// 						if (!AJIN_MO->MoveAxis_Wait(m_nSelectAxisNum, dTargetPos, nVel))
// 						{
// 							return FALSE;
// 						}
// 					}

				}
				else
				{
					// Velocity Move
					if (!AJIN_MO->MoveAxis_Velocity(m_nSelectAxisNum, (double)nVel* -1))
					{
						return FALSE;
					}
				}
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_2)->GetSafeHwnd())
		{
			UpdateData(TRUE);

			if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
			{
				if (AJIN_MO->IsMotion(m_nSelectAxisNum))
				{
					return FALSE;
				}
				int nVel = 0;

				MOTION_PARAM   stSpeed = *DATAMANAGER->GetCurrentMotionParam();

// 				if (m_nSelectSpeed == 0)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedSlow];
// 				}
// 				else if (m_nSelectSpeed == 1)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedMiddle];
// 				}
// 				else if (m_nSelectSpeed == 2)
// 				{
// 					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedHigh];
// 				}
// 				else
// 				{
					nVel = stSpeed.nMotorSpeed[(int)m_nSelectAxisNum][eSpeedHigh];
//				}

				if (m_nJogMode == 1)
				{
					dCurPos = AJIN_MO->GetActPos(m_nSelectAxisNum);
					if (m_nSelectAxisNum == 4)
					{
						dCurPos = dCurPos / 1000;
						dTargetPos = dCurPos + m_dMoveDistance;
					}
					else
					{
						dCurPos = dCurPos / 1000;
						dTargetPos = dCurPos + m_dMoveDistance;
					}
			
// 					if (m_nSelectAxisNum == 4)
// 					{

						if (!AJIN_MO->MoveAxis_NonWait(m_nSelectAxisNum, dTargetPos, nVel))
						{
							return FALSE;
						}
// 					}
// 					else
// 					{
// 						if (!AJIN_MO->MoveAxis_Wait(m_nSelectAxisNum, dTargetPos, nVel))
// 						{
// 							return FALSE;
// 						}
// 					}

					// Distance
	
				}
				else
				{
					// Velocity Move
					if (!AJIN_MO->MoveAxis_Velocity(m_nSelectAxisNum, nVel))
					{
						return FALSE;
					}
				}
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->GetSafeHwnd())
		{
			m_btnMotorState[3].SetFaceColor(COLOR_GREEN1, true);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_STOP)->GetSafeHwnd())
		{
			m_btnMotorState[2].SetFaceColor(COLOR_GREEN1, true);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		if (m_nJogMode != 1)
		{
			if ((pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_1)->GetSafeHwnd()))
			{
				if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
				{
					AJIN_MO->MoveEStop(m_nSelectAxisNum);
				}
			}

			if ((pMsg->hwnd == GetDlgItem(IDC_BTN_JOG_MOVE_2)->GetSafeHwnd()))
			{
				if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
				{
					AJIN_MO->MoveEStop(m_nSelectAxisNum);
				}
			}
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->GetSafeHwnd())
		{
			m_btnMotorState[3].SetTextColor(COLOR_BLACK);
			m_btnMotorState[3].SetFaceColor(RGB(225, 225, 225), true);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_MOTOR_STOP)->GetSafeHwnd())
		{
			m_btnMotorState[2].SetTextColor(COLOR_BLACK);
			m_btnMotorState[2].SetFaceColor(RGB(225, 225, 225), true);
		}
	}
	break;
	}

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

void CDlgMotorTeach::OnLButtonDown(UINT nFlags, CPoint point)
{
	// [21.1021.6]
	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();
	pParrentDlg->SendMessage(MESSAGE_UI_WINDOW_TOP, NULL, NULL);

	CDialogEx::OnLButtonDown(nFlags, point);
}

// CDlgTitle 메시지 처리기
void CDlgMotorTeach::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TEACH_TIMER_MOTOR_STATUS:
	{
		double dPos = 0.0f;
		CString strTemp = _T("");
// 		if (FAS_MO->m_bConnected == TRUE )
// 		{
			for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
			{
				dPos = AJIN_MO->GetActPos((long)i);
				strTemp.Format(_T("%0.3f"), dPos / 1000);

				m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
				m_stMotorCurPos[1][i].SetText(strTemp);
			}

// 			if (m_nSelectAxisNum >= 0 && m_nSelectAxisNum < (int)MOTOR_MAX_COUNT)
// 			{
// 				if (FAS_MO->IsServoStatus(m_nSelectAxisNum) == TRUE)
// 					SetAxisStatus(FAS_MO->GetAxisStatus(m_nSelectAxisNum));
// 			}
//		}
// 		if (SET_INSPECTOR == SYS_DISTORTION)
// 		{
// 			if (AJIN_MO->IsInvaildAxis() != FALSE)
// 			{
	//			if (m_nSelectAxisNum == 6)
	//			{
// 					dPos = AJIN_MO->GetActPos((long)AJIN_GANTRY_MASTER_Z);
// 					strTemp.Format(_T("%0.3f"), dPos/1000);
// 
// 					m_stMotorCurPos[1][6].SetFont_Gdip(L"Arial", 9.0F);
// 					m_stMotorCurPos[1][6].SetText(strTemp);
	//			}
//			}

//		}
		if (m_nSelectAxisNum != m_nSelectAxisNumBK)
		{
			m_nSelectAxisNumBK = m_nSelectAxisNum;
			ChangeUI_Select(m_nSelectAxisNum);
		}


	}
	break;
	case TEACH_TIMER_SENSOR_STATUS:
	{

		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();

		m_BarcodeSt = stTeach.nBarcode;
		if (m_BarcodeSt != m_BarcodeStBK)
		{
			m_BarcodeStBK = m_BarcodeSt;
			
			//stTeach.nBarcode = m_Barcode;
			if (m_BarcodeSt == 0)
			{
				m_Nobarcode.SetFaceColor(COLOR_GREEN1, true);

				m_Setbarcode.SetTextColor(COLOR_BLACK);
				m_Setbarcode.SetFaceColor(RGB(225, 225, 225), true);
			}
			else
			{
				m_Setbarcode.SetFaceColor(COLOR_GREEN1, true);

				m_Nobarcode.SetTextColor(COLOR_BLACK);
				m_Nobarcode.SetFaceColor(RGB(225, 225, 225), true);
			}
		}




		#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		m_nTimeSensor[0] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
		m_nTimeSensor[1] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
		m_nTimeSensor[2] = AJIN_IO->IsON(DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR);
		m_nTimeSensor[3] = AJIN_IO->IsON(DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR);
		m_nTimeSensor[4] = AJIN_IO->IsON(DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR);
		m_nTimeSensor[5] = AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR);

		m_nTimeSensor[6] = AJIN_IO->IsON(DIO_INPUT_X1_10_DBUL_JIG_L_CYL_DOWN_CHK_SENSOR);
		m_nTimeSensor[7] = AJIN_IO->IsON(DIO_INPUT_X1_11_DBUL_JIG_R_CYL_DOWN_CHK_SENSOR);
		m_nTimeSensor[8] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
		m_nTimeSensor[9] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
		m_nTimeSensor[10] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);

		m_nTimeSensor[11] = AJIN_IO->IsON(DIO_INPUT_X1_16_DBUR_JIG_L_CYL_DOWN_CHK_SENSOR);
		m_nTimeSensor[12] = AJIN_IO->IsON(DIO_INPUT_X1_17_DBUR_JIG_R_CYL_DOWN_CHK_SENSOR);
		m_nTimeSensor[13] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
		m_nTimeSensor[14] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
		m_nTimeSensor[15] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
		for (int i = 0; i < 16; i++)
		{
			if (m_nTimeSensor[i] != m_nTimeSensorBK[i])
			{
				m_nTimeSensorBK[i] = m_nTimeSensor[i];
				if (m_nTimeSensor[i] == TRUE)
				{
					m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_Green);
				}
				else
				{
					m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
				}
			}
		}
		#elif(SET_INSPECTOR == SYS_TLA_OQC)		// GHOST FLARE
			m_nTimeSensor[0] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
			m_nTimeSensor[1] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
			m_nTimeSensor[2] = AJIN_IO->IsON(DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR);
			m_nTimeSensor[3] = AJIN_IO->IsON(DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR);
			for (int i = 0; i < 4; i++)
			{
				if (m_nTimeSensor[i] != m_nTimeSensorBK[i])
				{
					m_nTimeSensorBK[i] = m_nTimeSensor[i];
					if (m_nTimeSensor[i] == TRUE)
					{
						m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_Green);
					}
					else
					{
						m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
					}
				}
			}
		#endif
	}
	break;
	}
}

BOOL CDlgMotorTeach::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	//pDC->FillSolidRect(rect, RGB(227, 255, 0));		// Lime
	return TRUE;
}

int CDlgMotorTeach::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	

	return 0;
}
BOOL CDlgMotorTeach::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_MotorTeach();
	InitComponent_MotorTeach_EditBox();

	InitComponent_Vigneting_X_Teach_EditBox();
	InitComponent_Vigneting_R_Teach_EditBox();
	InitComponent_Adjust_Teach_EditBox();
	InitComponent_Screw_L_Teach_EditBox();
	InitComponent_Screw_R_Teach_EditBox();
// 	InitComponent_AxisY_Teach_EditBox();
// 	InitComponent_AxisZ_Teach_EditBox();
// 	InitComponent_Axis_Chart_Teach_EditBox();
	// [21.1017.16]
	ChangeUI_Level();
	// [21.1027.3]
	ChangeUI_Jog(m_nJogMode);
	// [21.1104.3]
	m_dMoveSpeed = 2.0f;
	m_dMoveDistance = 1.0f;
	m_bFlagJogError = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgMotorTeach::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		for (int i = 0; i < 16; i++)
		{
			m_nTimeSensor[i] = 100;				// 센서 상태
			m_nTimeSensorBK[i] = 10;				// 센서 상태
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			SetTimer(TEACH_TIMER_MOTOR_STATUS, 250, NULL);
			SetTimer(TEACH_TIMER_SENSOR_STATUS, 100, NULL);
		}
		Load_TeachingData();
		OnChangeBtnUI();
// 		if (FAS_MO->m_bConnected == TRUE)
// 		{
// 			//ChangeConnectUI(TRUE);
// 			if (m_nSelectAxisNum < 0) m_nSelectAxisNum = 0;
// 				
// 				ChangeServoUI(TRUE);
// 				m_btnMotorJogSelect_Axis[m_nSelectAxisNum].SetFaceColor(COLOR_GREEN1, true);
// 		}
	}
	else
	{
		KillTimer(TEACH_TIMER_MOTOR_STATUS);
		KillTimer(TEACH_TIMER_SENSOR_STATUS);
		
	}
}
// void CDlgMotorTeach::InitComponent_Axis_Chart_Teach_EditBox()
// {
// 	CClientDC dc1(GetDlgItem(IDC_EDIT_CHART_POS_01));
// 	CClientDC dc2(GetDlgItem(IDC_EDIT_CHART_POS_02));
// 	CClientDC dc3(GetDlgItem(IDC_EDIT_CHART_POS_03));
// 	CRect rt1;
// 	CRect rt2;
// 	CRect rt3;
// 	CRect rt4;
// 
// 	GetDlgItem(IDC_EDIT_CHART_POS_01)->GetClientRect(&rt1);
// 
// 	rt1.left += 30;
// 	rt1.right -= 30;
// 
// 	rt1.top += 7;
// 	rt1.bottom -= 7;
// 
// 	((CEdit*)GetDlgItem(IDC_EDIT_CHART_POS_01))->SetRect(&rt1);
// 
// 	GetDlgItem(IDC_EDIT_CHART_POS_02)->GetClientRect(&rt2);
// 
// 	rt2.left += 30;
// 	rt2.right -= 30;
// 
// 	rt2.top += 7;
// 	rt2.bottom -= 7;
// 
// 	((CEdit*)GetDlgItem(IDC_EDIT_CHART_POS_02))->SetRect(&rt2);
// 
// 	GetDlgItem(IDC_EDIT_CHART_POS_03)->GetClientRect(&rt3);
// 
// 	rt3.left += 30;
// 	rt3.right -= 30;
// 
// 	rt3.top += 7;
// 	rt3.bottom -= 7;
// 
// 	((CEdit*)GetDlgItem(IDC_EDIT_CHART_POS_03))->SetRect(&rt3);
// 	UpdateData(FALSE);
// }


void CDlgMotorTeach::InitComponent_MotorTeach_EditBox()
{
	CClientDC dc(GetDlgItem(IDC_EDIT1));
	CRect rt;

	GetDlgItem(IDC_EDIT1)->GetClientRect(&rt);

	rt.left += 5;
	rt.right -= 5;

	rt.top += 7;
	rt.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT1))->SetRect(&rt);

	UpdateData(FALSE);
}
void CDlgMotorTeach::InitComponent_Vigneting_X_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_X_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_X_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_X_POS_3));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_X_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_X_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_X_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_X_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_X_POS_3)->GetClientRect(&rt3);

	rt3.left += 30;
	rt3.right -= 30;

	rt3.top += 7;
	rt3.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_X_POS_3))->SetRect(&rt3);
	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_Vigneting_R_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_R_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_R_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_R_POS_3));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_R_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_R_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_R_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_R_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_R_POS_3)->GetClientRect(&rt3);

	rt3.left += 30;
	rt3.right -= 30;

	rt3.top += 7;
	rt3.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_R_POS_3))->SetRect(&rt3);
	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_Adjust_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_A_POS_1));
	//CClientDC dc2(GetDlgItem(IDC_EDIT_A_POS_2));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_A_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_A_POS_1))->SetRect(&rt1);

// 	GetDlgItem(IDC_EDIT_A_POS_2)->GetClientRect(&rt2);
// 
// 	rt2.left += 30;
// 	rt2.right -= 30;
// 
// 	rt2.top += 7;
// 	rt2.bottom -= 7;
// 
// 	((CEdit*)GetDlgItem(IDC_EDIT_A_POS_2))->SetRect(&rt2);
	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_Screw_L_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_SCREW_L_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_SCREW_L_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_SCREW_L_POS_3));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_L_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_L_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->GetClientRect(&rt3);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_L_POS_3))->SetRect(&rt3);
	UpdateData(FALSE);
}
void CDlgMotorTeach::InitComponent_Screw_R_Teach_EditBox()
{
	CClientDC dc1(GetDlgItem(IDC_EDIT_SCREW_R_POS_1));
	CClientDC dc2(GetDlgItem(IDC_EDIT_SCREW_R_POS_2));
	CClientDC dc3(GetDlgItem(IDC_EDIT_SCREW_R_POS_3));
	CRect rt1;
	CRect rt2;
	CRect rt3;
	CRect rt4;

	GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->GetClientRect(&rt1);

	rt1.left += 30;
	rt1.right -= 30;

	rt1.top += 7;
	rt1.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_R_POS_1))->SetRect(&rt1);

	GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->GetClientRect(&rt2);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_R_POS_2))->SetRect(&rt2);

	GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->GetClientRect(&rt3);

	rt2.left += 30;
	rt2.right -= 30;

	rt2.top += 7;
	rt2.bottom -= 7;

	((CEdit*)GetDlgItem(IDC_EDIT_SCREW_R_POS_3))->SetRect(&rt3);
	UpdateData(FALSE);
}

void CDlgMotorTeach::InitComponent_MotorTeach()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	for (int i = 0; i < 7; i++)
	{
		m_stMotorTitle[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMotorTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMotorTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorTitle[i].SetText(m_szTeachTitleKor[i]);
		else
			m_stMotorTitle[i].SetText(m_szTeachTitleEng[i]);
	}

	//--------------------------------------------------------------------------
	// 현재위치

	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{
			if (i < (int)MOTOR_MAX_COUNT)
			{
				m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			}
			else
			{
				m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			}

			m_stMotorCurPos[0][i].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
				m_stMotorCurPos[0][i].SetText(m_szMainMotorName_Kor[i]);
			else
				m_stMotorCurPos[0][i].SetText(m_szMainMotorName_Eng[i]);
		}

		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{
			strTemp = _T("0.0");

			if (i < (int)MOTOR_MAX_COUNT)
			{
				m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			}
			else
			{
				m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
				m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			}

			m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
			m_stMotorCurPos[1][i].SetText(strTemp);
		}
	}
	else
	{

	}

	//--------------------------------------------------------------------------
	// 조그이동
	for (int i = 0; i < 4; i++)
	{
		m_stMotorJogTitleSub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorJogTitleSub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorJogTitleSub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogKor[i]);
		else
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogEng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Mode[i].EnableWindowsTheming(FALSE);
		m_btnMotorJogSelect_Mode[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorJogSelect_Mode[i].m_bTransparent = false;
		m_btnMotorJogSelect_Mode[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Mode[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnKor[i]);
		else
			m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnEng[i]);
	}

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_btnMotorJogSelect_Axis[i].EnableWindowsTheming(FALSE);
		m_btnMotorJogSelect_Axis[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorJogSelect_Axis[i].m_bTransparent = false;
		m_btnMotorJogSelect_Axis[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Axis[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);

	}

	for (int i = 0; i < 5; i++)
	{
		m_btnMotorJogSelect_Distance[i].EnableWindowsTheming(FALSE);
		m_btnMotorJogSelect_Distance[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorJogSelect_Distance[i].m_bTransparent = false;
		m_btnMotorJogSelect_Distance[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Distance[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnKor[12 + i]);
		else
			m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnEng[12 + i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnMotorJogSelect_Speed[i].EnableWindowsTheming(FALSE);
		m_btnMotorJogSelect_Speed[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorJogSelect_Speed[i].m_bTransparent = false;
		m_btnMotorJogSelect_Speed[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Speed[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnKor[17 + i]);
		else
			m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnEng[17 + i]);
	}



	m_Nobarcode.EnableWindowsTheming(FALSE);
	m_Nobarcode.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_Nobarcode.m_bTransparent = false;
	m_Nobarcode.SetTextColor(COLOR_BLACK);
	m_Nobarcode.SetFaceColor(RGB(225, 225, 225), true);

	if (stEq.nLanguage == 0)
		m_Nobarcode.SetWindowText(_T("바코드사용 X"));
	else
		m_Nobarcode.SetWindowText(_T("No Barcode"));

	m_Setbarcode.EnableWindowsTheming(FALSE);
	m_Setbarcode.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_Setbarcode.m_bTransparent = false;
	m_Setbarcode.SetTextColor(COLOR_BLACK);
	m_Setbarcode.SetFaceColor(RGB(225, 225, 225), true);

	if (stEq.nLanguage == 0)
		m_Setbarcode.SetWindowText(_T("바코드사용 O"));
	else
		m_Setbarcode.SetWindowText(_T("Set Barcode"));




	// [21.1017.18] Modified
	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Move[i].SetTooltip(_T("JOGGING"));
		m_btnMotorJogSelect_Move[i].SetTextColor(CVGStatic::ColorStyle_Red);
		//m_btnMotorJogSelect_Move[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnKor[20 + i]);
		else
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnEng[20 + i]);
	}

	m_btnMotorJogSelect_Home.SetTooltip(_T("HOMMING"));
	m_btnMotorJogSelect_Home.SetTextColor(CVGStatic::ColorStyle_Red);
	//m_btnMotorJogSelect_Home.SetFont_Gdip(L"Arial", 9.0F);


	if (stEq.nLanguage == 0)
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnKor[22]);
	else
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnEng[22]);

	// Load image
	CString	strExePath = L"";
	CString strImgPath = L"";
	BOOL b;

	strExePath.Format(_T("%s"), COMMON->GetProgramPath());

	auto * pParrentDlg = static_cast<CLuriMotion_BaseDlg*>(AfxGetApp()->GetMainWnd());
	HICON hIcon[2];

	auto hInstResource1 = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_FEWER), RT_GROUP_ICON);
	hIcon[0] = (HICON)::LoadImage(hInstResource1, MAKEINTRESOURCE(IDI_ICON_FEWER), IMAGE_ICON, 0, 0, 0);

	auto hInstResource2 = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_MORE), RT_GROUP_ICON);
	hIcon[1] = (HICON)::LoadImage(hInstResource2, MAKEINTRESOURCE(IDI_ICON_MORE), IMAGE_ICON, 0, 0, 0);

	m_btnMotorJogSelect_Move[0].SetIcon(hIcon[0]);
	m_btnMotorJogSelect_Move[1].SetIcon(hIcon[1]);

	//--------------------------------------------------------------------------
	for (int i = 0; i < 9; i++)
	{
		m_stDoorLockManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stDoorLockManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stDoorLockManual[i].SetFont_Gdip(L"Arial", 7.0F);

		if (stEq.nLanguage == 0)
			m_stDoorLockManual[i].SetText(m_szTeachConvKor[i]);
		else
			m_stDoorLockManual[i].SetText(m_szTeachConvEng[i]);
	}
	// 메뉴얼 UI
	for (int i = 0; i < 5; i++)
	{
		m_stMotorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorManual[i].SetFont_Gdip(L"Arial", 7.0F);

		if (stEq.nLanguage == 0)
			m_stMotorManual[i].SetText(m_szTeachCylinderKor[i]);
		else
			m_stMotorManual[i].SetText(m_szTeachCylinderEng[i]);
	}
	for (int i = 0; i < 16; i++)
	{

		m_stSensorChk[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stSensorChk[i].SetFont_Gdip(L"Arial", 9.0F);

		m_stSensorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stSensorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stSensorManual[i].SetFont_Gdip(L"Arial", 7.0F);

		if (stEq.nLanguage == 0)
			m_stSensorManual[i].SetText(m_szTeachSensnorKor[i]);
		else
			m_stSensorManual[i].SetText(m_szTeachSensorEng[i]);
	}







	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			m_btnDoorLockManual[i][j].EnableWindowsTheming(FALSE);
			m_btnDoorLockManual[i][j].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
			m_btnDoorLockManual[i][j].m_bTransparent = false;
			m_btnDoorLockManual[i][j].SetTextColor(COLOR_BLACK);
			m_btnDoorLockManual[i][j].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
			{
				if (i == 0)
					m_btnDoorLockManual[i][j].SetWindowTextW(_T("잠금"));
				else
					m_btnDoorLockManual[i][j].SetWindowTextW(_T("열림"));
			}
			else
			{
				if (i == 0)
					m_btnDoorLockManual[i][j].SetWindowTextW(_T("Lock"));
				else
					m_btnDoorLockManual[i][j].SetWindowTextW(_T("Unlock"));
			}
		}

		for (int j = 0; j < 5; j++)
		{
			m_btnMotorManual[i][j].EnableWindowsTheming(FALSE);
			m_btnMotorManual[i][j].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
			m_btnMotorManual[i][j].m_bTransparent = false;
			m_btnMotorManual[i][j].SetTextColor(COLOR_BLACK);
			m_btnMotorManual[i][j].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnKor[j]);
				else
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnKor[5+ j]);
			}
			else
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnEng[j]);
				else
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnEng[5 + j]);
			}

		}
	}
	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		m_btnMotorState[nIndex].EnableWindowsTheming(FALSE);
		m_btnMotorState[nIndex].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorState[nIndex].m_bTransparent = false;
		m_btnMotorState[nIndex].SetTextColor(COLOR_BLACK);
		m_btnMotorState[nIndex].SetFaceColor(RGB(225, 225, 225), true);
	}

	if (stEq.nLanguage == 0)
	{
		m_btnMotorState[0].SetWindowText(_T("Servo On"));
		m_btnMotorState[1].SetWindowText(_T("Servo Off"));
		m_btnMotorState[2].SetWindowText(_T("정지"));
		m_btnMotorState[3].SetWindowText(_T("알람 리셋"));
	}
	else
	{
		m_btnMotorState[0].SetWindowText(_T("Servo On"));
		m_btnMotorState[1].SetWindowText(_T("Servo Off"));
		m_btnMotorState[2].SetWindowText(_T("Stop"));
		m_btnMotorState[3].SetWindowText(_T("Alarm Reset"));
	}

// 	m_btnMotorConnect[0].EnableWindowsTheming(FALSE);
// 	m_btnMotorConnect[0].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnMotorConnect[0].m_bTransparent = false;
// 	m_btnMotorConnect[0].SetTextColor(COLOR_BLACK);
// 	m_btnMotorConnect[0].SetFaceColor(RGB(225, 225, 225), true);
// 
// 	m_btnMotorConnect[1].EnableWindowsTheming(FALSE);
// 	m_btnMotorConnect[1].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnMotorConnect[1].m_bTransparent = false;
// 	m_btnMotorConnect[1].SetTextColor(COLOR_BLACK);
// 	m_btnMotorConnect[1].SetFaceColor(RGB(225, 225, 225), true);

// 	if (stEq.nLanguage == 0)
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("연결"));
// 		m_btnMotorConnect[1].SetWindowText(_T("해제"));
// 	}
// 	else
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("Connect"));
// 		m_btnMotorConnect[1].SetWindowText(_T("Disconnect"));
// 	}

// 	m_stModelSelect.SetStaticStyle(CVGStatic::StaticStyle_Data);
// 	m_stModelSelect.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	m_stModelSelect.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stEq.nLanguage == 0)
// 		m_stModelSelect.SetText(_T("연결"));
// 	else
// 		m_stModelSelect.SetText(_T("Connect"));

	TEACHING_PARAM stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// 	m_btnModelSelect[0].EnableWindowsTheming(FALSE);
// 	m_btnModelSelect[0].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnModelSelect[0].m_bTransparent = false;
// 		
// 	if (stTeach.nSampleModelType == 1)
// 	{
// 		m_btnModelSelect[0].SetFaceColor(COLOR_GREEN1, true);
// 	}
// 	else
// 	{
// 		m_btnModelSelect[0].SetTextColor(COLOR_BLACK);
// 		m_btnModelSelect[0].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 
// 	if (stEq.nLanguage == 0)
// 		m_btnModelSelect[0].SetWindowText(_T("H 모델"));
// 	else
// 		m_btnModelSelect[0].SetWindowText(_T("H Model"));
// 
// 	m_btnModelSelect[1].EnableWindowsTheming(FALSE);
// 	m_btnModelSelect[1].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnModelSelect[1].m_bTransparent = false;
// 
// 	if (stTeach.nSampleModelType == 2)
// 	{
// 		m_btnModelSelect[1].SetFaceColor(COLOR_GREEN1, true);
// 	}
// 	else
// 	{
// 		m_btnModelSelect[1].SetTextColor(COLOR_BLACK);
// 		m_btnModelSelect[1].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 
// 	if (stEq.nLanguage == 0)
// 		m_btnModelSelect[1].SetWindowText(_T("V 모델"));
// 	else
// 		m_btnModelSelect[1].SetWindowText(_T("V Model"));
	// Port
// 	m_cbFASBoard_Port.ResetContent();
// 	m_cbFASBoard_BaudRate.ResetContent();

// 	COMMCONFIG lpCC;
// 	CString str, str1;
// 	unsigned long ccsize = sizeof(COMMCONFIG);
// 	int index = 0;
// 	BOOL bExistFlag = FALSE;

// 
// 	for (int i = 1; i < 15; i++)
// 	{
// 		str = _T("COM");
// 		str1.Format(_T("%d"), i);
// 		str += str1;
// 		bExistFlag = FALSE;
// 
// 		// 중요 포인트 return값이 TRUE이면 COMPORT존재
// 		bExistFlag = GetDefaultCommConfig(str, &lpCC, &ccsize);
// 
// // 		//if (bExistFlag)
// // 		{
// // 			index = m_cbFASBoard_Port.AddString(str);
// // 			m_cbFASBoard_Port.SetItemData(index, i);
// // 
// // 		}
// 	}
//	TCHAR *BaudList_Motor[] = { _T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200") };

// 	for (int i = 0; i < 5; i++)
// 	{
// 		str.Format(_T("%s"), BaudList_Motor[i]);
// 		index = m_cbFASBoard_BaudRate.AddString(str);
// 		m_cbFASBoard_BaudRate.SetItemData(index, i);
// 
// 	}



	//--------------------------------------------------------------------------
	// Speed Setting

	for (int i = 0; i < 2; i++)
	{
		m_stMotorSpeedSet_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorSpeedSet_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorSpeedSet_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedKor[i]);
		else
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedEng[i]);
	}

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_btnMotorSpeedSet_AxisNum[i].EnableWindowsTheming(FALSE);
		m_btnMotorSpeedSet_AxisNum[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnMotorSpeedSet_AxisNum[i].m_bTransparent = false;
		m_btnMotorSpeedSet_AxisNum[i].SetTextColor(COLOR_BLACK);
		m_btnMotorSpeedSet_AxisNum[i].SetFaceColor(RGB(225, 225, 225), true);
		if (stEq.nLanguage == 0)
			m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
			}
			else
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_White);
			}

			m_stMotorSpeedSet_Thirdsub[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedKor[i + 2]);
			}
			else
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedEng[i + 2]);
			}
		}
	}
	if (stEq.nLanguage == 0)
	{
		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->SetWindowTextW(_T("수동 이동 화면 보기"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("저장"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("다른이름으로 저장"));
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->SetWindowTextW(_T("View Manual Move Menu"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("Save"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("Save as"));
	}
	//--------------------------------------------------------------------------
	// Teaching  
	m_stTeach_Vigneting_X.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Vigneting_X.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Vigneting_X.SetFont_Gdip(L"Arial", 9.0F);
	if(stEq.nLanguage == 0)
		m_stTeach_Vigneting_X.SetText(_T("비네팅 X 축"));
	else
		m_stTeach_Vigneting_X.SetText(_T("Vigneting X AXIS"));

	m_stTeach_Vigneting_R.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Vigneting_R.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Vigneting_R.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Vigneting_R.SetText(_T("비네팅 R 축"));
	else
		m_stTeach_Vigneting_R.SetText(_T("Vigneting R AXIS"));

	m_stTeach_Adjust.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Adjust.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Adjust.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Adjust.SetText(_T("조정 축"));
	else
		m_stTeach_Adjust.SetText(_T("Adjust AXIS"));

	m_stTeach_Screw_L.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw_L.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw_L.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw_L.SetText(_T("스크류 왼쪽"));
	else
		m_stTeach_Screw_L.SetText(_T("Screw Left AXIS"));

	m_stTeach_Screw_R.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw_R.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw_R.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw_R.SetText(_T("스크류 오른쪽"));
	else
		m_stTeach_Screw_R.SetText(_T("Screw Right AXIS"));



	m_stTeach_AdjustSpec.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_AdjustSpec.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_AdjustSpec.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_AdjustSpec.SetText(_T("조정 통과 스펙(Y Pixel,±)"));
	else
		m_stTeach_AdjustSpec.SetText(_T("Adjust Pass Spec(Y Pixel,±)"));
	
	m_stTeach_Screw.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw.SetText(_T("스크류 제어"));
	else
		m_stTeach_Screw.SetText(_T("Screw Control"));

	

	for (int i = 0; i < 3; i++)
	{
		m_stTeach_Vigneting_X_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Vigneting_X_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Vigneting_X_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Vigneting_X_Sub[i].SetText(m_szTeachTrinity_Vigneting_X_List_Kor[i]);
		else
			m_stTeach_Vigneting_X_Sub[i].SetText(m_szTeachTrinity_Vigneting_X_List_Eng[i]);

		m_stTeach_Vigneting_R_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Vigneting_R_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Vigneting_R_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Vigneting_R_Sub[i].SetText(m_szTeachTrinity_Vigneting_R_List_Kor[i]);
		else
			m_stTeach_Vigneting_R_Sub[i].SetText(m_szTeachTrinity_Vigneting_R_List_Eng[i]);


		m_stTeach_Screw_L_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Screw_L_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Screw_L_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Screw_L_Sub[i].SetText(m_szTeachTrinity_Screw_List_Kor[i]);
		else
			m_stTeach_Screw_L_Sub[i].SetText(m_szTeachTrinity_Screw_List_Eng[i]);

		m_stTeach_Screw_R_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Screw_R_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Screw_R_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Screw_R_Sub[i].SetText(m_szTeachTrinity_Screw_List_Kor[i]);
		else
			m_stTeach_Screw_R_Sub[i].SetText(m_szTeachTrinity_Screw_List_Eng[i]);




	}
	for (int i = 0; i < 6; i++)
	{
		m_stTeach_Adjust_Spec_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Adjust_Spec_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Adjust_Spec_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Adjust_Spec_Sub[i].SetText(m_szTeachTrinity_SPEC_List_Kor[i]);
		else
			m_stTeach_Adjust_Spec_Sub[i].SetText(m_szTeachTrinity_SPEC_List_Eng[i]);
	}


	for (int i = 0; i < 1; i++)
	{
		m_stTeach_Adjust_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Adjust_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Adjust_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Adjust_Sub[i].SetText(m_szTeachTrinity_Adjust_List_Kor[i]);
		else
			m_stTeach_Adjust_Sub[i].SetText(m_szTeachTrinity_Adjust_List_Eng[i]);
	}



// 	for (int i = 0; i < 4; i++)
// 	{
		m_stStatic_Text[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stStatic_Text[0].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stStatic_Text[0].SetFont_Gdip(L"Arial", 9.0F);
// 		if (i == 0) m_stStatic_Text[i].SetText(_T("ComPort"));
// 		if (i == 1) m_stStatic_Text[i].SetText(_T("BaudRate"));
// 		if (i == 2)
// 		{
// 			if (stEq.nLanguage == 0)
// 				m_stStatic_Text[i].SetText(_T("모터 컨트롤러"));
// 			else
// 				m_stStatic_Text[i].SetText(_T("Motor Controller"));
// 		}
// 		if (i == 3)
// 		{
			if (stEq.nLanguage == 0)
				m_stStatic_Text[0].SetText(_T("모터 컨트롤러"));
			else
				m_stStatic_Text[0].SetText(_T("Motor Controller"));
//		}
//	}

	for (int nState = 0; nState < AXIS_MOTOR_STATUES; nState++)
	{
		m_stStatic_Motor_Statuses[nState].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stStatic_Motor_Statuses[nState].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stStatic_Motor_Statuses[nState].SetFont_Gdip(L"Arial", 7.0F, FontStyleBoldItalic);
		m_stStatic_Motor_Statuses[nState].SetText(m_szTeachTrinity_AxisStatus_List[nState ]);
	}
}

void CDlgMotorTeach::ChangeUI_Language()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM stTeach = *DATAMANAGER->GetCurrentTeachingData();

	//m_pDlgManualMove->ChangeUI_Language();

	CString strTemp = _T("");
	for (int i = 0; i <7; i++)
	{
		m_stMotorTitle[i].SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
		m_stMotorTitle[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stMotorTitle[i].SetFont_Gdip(L"Arial", 9.0F);

		if( stEq.nLanguage == 0)
			m_stMotorTitle[i].SetText(m_szTeachTitleKor[i]);
		else
			m_stMotorTitle[i].SetText(m_szTeachTitleEng[i]);
	}

	//--------------------------------------------------------------------------
	// 현재위치
	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
	{
		m_stMotorCurPos[0][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorCurPos[0][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorCurPos[0][i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorCurPos[0][i].SetText(m_szMainMotorName_Kor[i]);
		else
			m_stMotorCurPos[0][i].SetText(m_szMainMotorName_Eng[i]);
	}

	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
	{
		strTemp = _T("0.0");

		m_stMotorCurPos[1][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorCurPos[1][i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorCurPos[1][i].SetFont_Gdip(L"Arial", 9.0F);
		m_stMotorCurPos[1][i].SetText(strTemp);
	}

	if (stEq.nLanguage == 0)
	{
		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->SetWindowTextW(_T("수동 이동 화면 보기"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("저장"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("다른이름으로 저장"));
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->SetWindowTextW(_T("View Manual Move Menu"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->SetWindowTextW(_T("Save"));
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->SetWindowTextW(_T("Save as"));
	}
	//--------------------------------------------------------------------------
	// 조그이동
	for (int i = 0; i < 4; i++)
	{
		m_stMotorJogTitleSub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorJogTitleSub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorJogTitleSub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogKor[i]);
		else
			m_stMotorJogTitleSub[i].SetText(m_szTeachJogEng[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Mode[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Mode[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnKor[i]);
		else
			m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnEng[i]);
	}

	for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
	{
		m_btnMotorJogSelect_Axis[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Axis[i].SetFaceColor(RGB(225, 225, 225), true);
		
		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);

	}

	for (int i = 0; i < 5; i++)
	{
		m_btnMotorJogSelect_Distance[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Distance[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnKor[12 + i]);
		else
			m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnEng[12 + i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_btnMotorJogSelect_Speed[i].SetTextColor(COLOR_BLACK);
		m_btnMotorJogSelect_Speed[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnKor[17 + i]);
		else
			m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnEng[17 + i]);
	}


	m_Nobarcode.SetTextColor(COLOR_BLACK);
	m_Nobarcode.SetFaceColor(RGB(225, 225, 225), true);

	if (stEq.nLanguage == 0)
		m_Nobarcode.SetWindowText(_T("바코드사용 X"));
	else
		m_Nobarcode.SetWindowText(_T("No Barcode"));

	m_Setbarcode.SetTextColor(COLOR_BLACK);
	m_Setbarcode.SetFaceColor(RGB(225, 225, 225), true);

	if (stEq.nLanguage == 0)
		m_Setbarcode.SetWindowText(_T("바코드사용 O"));
	else
		m_Setbarcode.SetWindowText(_T("Set Barcode"));

	// [21.1017.19] Modified
	for (int i = 0; i < 2; i++)
	{
		m_btnMotorJogSelect_Move[i].SetTooltip(_T("JOGGING"));;
		m_btnMotorJogSelect_Move[i].SetTextColor(CVGStatic::ColorStyle_Red);
		//m_btnMotorJogSelect_Move[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnKor[20 + i]);
		else
			m_btnMotorJogSelect_Move[i].SetWindowText(m_szTeachJogBtnEng[20 + i]);
	}

	m_btnMotorJogSelect_Home.SetTooltip(_T("HOMMING"));
	m_btnMotorJogSelect_Home.SetTextColor(CVGStatic::ColorStyle_Red);
	//m_btnMotorJogSelect_Home.SetFont_Gdip(L"Arial", 9.0F);

	if (stEq.nLanguage == 0)
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnKor[22]);
	else
		m_btnMotorJogSelect_Home.SetWindowText(m_szTeachJogBtnEng[22]);

	//--------------------------------------------------------------------------
	for (int i = 0; i < 9; i++)
	{
		m_stDoorLockManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stDoorLockManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stDoorLockManual[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stEq.nLanguage == 0)
			m_stDoorLockManual[i].SetText(m_szTeachConvKor[i]);
		else
			m_stDoorLockManual[i].SetText(m_szTeachConvEng[i]);
	}
	//--------------------------------------------------------------------------
// Teaching  
	m_stTeach_Vigneting_X.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Vigneting_X.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Vigneting_X.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Vigneting_X.SetText(_T("비네팅 X 축"));
	else
		m_stTeach_Vigneting_X.SetText(_T("Vigneting X AXIS"));

	m_stTeach_Vigneting_R.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Vigneting_R.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Vigneting_R.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Vigneting_R.SetText(_T("비네팅 R 축"));
	else
		m_stTeach_Vigneting_R.SetText(_T("Vigneting R AXIS"));

	m_stTeach_Adjust.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Adjust.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Adjust.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Adjust.SetText(_T("조정 축"));
	else
		m_stTeach_Adjust.SetText(_T("Adjust AXIS"));

	m_stTeach_Screw_L.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw_L.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw_L.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw_L.SetText(_T("스크류 왼쪽"));
	else
		m_stTeach_Screw_L.SetText(_T("Screw Left AXIS"));

	m_stTeach_Screw_R.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw_R.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw_R.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw_R.SetText(_T("스크류 오른쪽"));
	else
		m_stTeach_Screw_R.SetText(_T("Screw Right AXIS"));


	m_stTeach_AdjustSpec.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_AdjustSpec.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_AdjustSpec.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_AdjustSpec.SetText(_T("조정 통과 스펙(Y Pixel,±)"));
	else
		m_stTeach_AdjustSpec.SetText(_T("Adjust Pass Spec(Y Pixel,±)"));

	m_stTeach_Screw.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
	m_stTeach_Screw.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stTeach_Screw.SetFont_Gdip(L"Arial", 9.0F);
	if (stEq.nLanguage == 0)
		m_stTeach_Screw.SetText(_T("스크류 제어"));
	else
		m_stTeach_Screw.SetText(_T("Screw Control"));



	for (int i = 0; i < 3; i++)
	{
		m_stTeach_Vigneting_X_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Vigneting_X_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Vigneting_X_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Vigneting_X_Sub[i].SetText(m_szTeachTrinity_Vigneting_X_List_Kor[i]);
		else
			m_stTeach_Vigneting_X_Sub[i].SetText(m_szTeachTrinity_Vigneting_X_List_Eng[i]);

		m_stTeach_Vigneting_R_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Vigneting_R_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Vigneting_R_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Vigneting_R_Sub[i].SetText(m_szTeachTrinity_Vigneting_R_List_Kor[i]);
		else
			m_stTeach_Vigneting_R_Sub[i].SetText(m_szTeachTrinity_Vigneting_R_List_Eng[i]);


		m_stTeach_Screw_L_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Screw_L_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Screw_L_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Screw_L_Sub[i].SetText(m_szTeachTrinity_Screw_List_Kor[i]);
		else
			m_stTeach_Screw_L_Sub[i].SetText(m_szTeachTrinity_Screw_List_Eng[i]);

		m_stTeach_Screw_R_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Screw_R_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Screw_R_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Screw_R_Sub[i].SetText(m_szTeachTrinity_Screw_List_Kor[i]);
		else
			m_stTeach_Screw_R_Sub[i].SetText(m_szTeachTrinity_Screw_List_Eng[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		m_stTeach_Adjust_Spec_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Adjust_Spec_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Adjust_Spec_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Adjust_Spec_Sub[i].SetText(m_szTeachTrinity_SPEC_List_Kor[i]);
		else
			m_stTeach_Adjust_Spec_Sub[i].SetText(m_szTeachTrinity_SPEC_List_Eng[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		m_stTeach_Adjust_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTeach_Adjust_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stTeach_Adjust_Sub[i].SetFont_Gdip(L"Arial", 7.0F);
		if (stEq.nLanguage == 0)
			m_stTeach_Adjust_Sub[i].SetText(m_szTeachTrinity_Adjust_List_Kor[i]);
		else
			m_stTeach_Adjust_Sub[i].SetText(m_szTeachTrinity_Adjust_List_Eng[i]);
	}


// 	m_stTeach_Y.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
// 	m_stTeach_Y.SetColorStyle(CVGStatic::ColorStyle_Black);
// 	m_stTeach_Y.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stEq.nLanguage == 0)
// 		m_stTeach_Y.SetText(_T("소켓 X 축"));
// 	else
// 		m_stTeach_Y.SetText(_T("SOCKET X AXIS"));
// 
// 	m_stTeach_Z2.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
// 	m_stTeach_Z2.SetColorStyle(CVGStatic::ColorStyle_Black);
// 	m_stTeach_Z2.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stEq.nLanguage == 0)
// 		m_stTeach_Z2.SetText(_T("소켓 Z 축"));
// 	else
// 		m_stTeach_Z2.SetText(_T("SOCKET Z AXIS"));
// 
// // 	m_stTeach_CV.SetStaticStyle(CVGStatic::StaticStyle_GroupHeader);
// // 	m_stTeach_CV.SetColorStyle(CVGStatic::ColorStyle_Black);
// // 	m_stTeach_CV.SetFont_Gdip(L"Arial", 9.0F);
// // 	if (stEq.nLanguage == 0)
// // 		m_stTeach_CV.SetText(_T("컨베이어"));
// // 	else
// // 		m_stTeach_CV.SetText(_T("Conveyor"));
// 
// 	for (int i = 0; i < 4; i++)
// 	{
// 		m_stTeach_Y_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stTeach_Y_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
// 		m_stTeach_Y_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stEq.nLanguage == 0)
// 			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Kor[i + 1]);
// 		else
// 			m_stTeach_Y_Sub[i].SetText(m_szTeachTrinity_AxisList_Eng[i + 1]);
// 	}
// 
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stTeach_Z_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stTeach_Z_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
// 		m_stTeach_Z_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stEq.nLanguage == 0)
// 			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisZList_Kor[i + 1]);
// 		else
// 			m_stTeach_Z_Sub[i].SetText(m_szTeachTrinity_AxisZList_Eng[i + 1]);
// 	}
// 
// 	for (int i = 0; i < 6; i++)
// 	{
// 		m_stTeach_Z2_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stTeach_Z2_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
// 		m_stTeach_Z2_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stEq.nLanguage == 0)
// 			m_stTeach_Z2_Sub[i].SetText(m_szTeachTrinity_AxisZList_Kor[i + 7]);
// 		else
// 			m_stTeach_Z2_Sub[i].SetText(m_szTeachTrinity_AxisZList_Eng[i + 7]);
// 	}
	
// 	for (int i = 0; i < 3; i++)
// 	{
// 		m_stTeach_CV_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stTeach_CV_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
// 		m_stTeach_CV_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stEq.nLanguage == 0)
// 			m_stTeach_CV_Sub[i].SetText(m_szTeachTrinity_CvList_Kor[i]);
// 		else
// 			m_stTeach_CV_Sub[i].SetText(m_szTeachTrinity_CvList_Eng[i]);
// 	}

// 
// 	for (int i = 0; i < 3; i++)
// 	{
// 		m_stTeach_Z_Chart[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
// 		m_stTeach_Z_Chart[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
// 		m_stTeach_Z_Chart[i].SetFont_Gdip(L"Arial", 9.0F);
// 		if (stEq.nLanguage == 0)
// 			m_stTeach_Z_Chart[i].SetText(m_szTeachTrinity_Chart_List_Kor[i]);
// 		else
// 			m_stTeach_Z_Chart[i].SetText(m_szTeachTrinity_Chart_List_Eng[i]);
// 	}
	// 메뉴얼 UI
	for (int i = 0; i < 5; i++)
	{
		m_stMotorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorManual[i].SetFont_Gdip(L"Arial", 7.0F);
		
		if (stEq.nLanguage == 0)
			m_stMotorManual[i].SetText(m_szTeachCylinderKor[i]);
		else
			m_stMotorManual[i].SetText(m_szTeachCylinderEng[i]);
	}
	for (int i = 0; i < 16; i++)
	{

		m_stSensorChk[i].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_stSensorChk[i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stSensorChk[i].SetFont_Gdip(L"Arial", 9.0F);

		m_stSensorManual[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stSensorManual[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stSensorManual[i].SetFont_Gdip(L"Arial", 7.0F);

		if (stEq.nLanguage == 0)
			m_stSensorManual[i].SetText(m_szTeachSensnorKor[i]);
		else
			m_stSensorManual[i].SetText(m_szTeachSensorEng[i]);
	}
// 	if (stTeach.nSampleModelType == 1)
// 	{
// 		m_btnModelSelect[0].SetFaceColor(COLOR_GREEN1, true);
// 	}
// 	else
// 	{
// 		m_btnModelSelect[0].SetTextColor(COLOR_BLACK);
// 		m_btnModelSelect[0].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 
// 	if (stEq.nLanguage == 0)
// 		m_btnModelSelect[0].SetWindowText(_T("H 모델"));
// 	else
// 		m_btnModelSelect[0].SetWindowText(_T("H Model"));
// 
// 	if (stTeach.nSampleModelType == 2)
// 	{
// 		m_btnModelSelect[1].SetFaceColor(COLOR_GREEN1, true);
// 	}
// 	else
// 	{
// 		m_btnModelSelect[1].SetTextColor(COLOR_BLACK);
// 		m_btnModelSelect[1].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 
// 	if (stEq.nLanguage == 0)
// 		m_btnModelSelect[1].SetWindowText(_T("V 모델"));
// 	else
// 		m_btnModelSelect[1].SetWindowText(_T("V Model"));

	for (int i = 0; i < 2; i++)
	{
 		for (int j = 0; j < 9; j++)
 		{
			m_btnDoorLockManual[i][j].SetTextColor(COLOR_BLACK);
			m_btnDoorLockManual[i][j].SetFaceColor(RGB(225, 225, 225), true);
 
 			if (stEq.nLanguage == 0)
 			{
 				if (i == 0)
 					m_btnDoorLockManual[i][j].SetWindowText(_T("잠금"));
 				else
 					m_btnDoorLockManual[i][j].SetWindowText(_T("열림"));
 			}
 			else
 			{
 				if (i == 0)
 					m_btnDoorLockManual[i][j].SetWindowText(_T("Lock"));
 				else
 					m_btnDoorLockManual[i][j].SetWindowText(_T("Unlock"));
 			}
 		}

		for (int j = 0; j < 5; j++)
		{
			m_btnMotorManual[i][j].SetTextColor(COLOR_BLACK);
			m_btnMotorManual[i][j].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnKor[j]);
				else
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnKor[5 + j]);
			}
			else
			{
				if (i == 0)
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnEng[j]);
				else
					m_btnMotorManual[i][j].SetWindowText(m_szTeachCylindeBtnEng[5 + j]);
			}

		}
	}

// 	for (int i = 0; i < 3; i++)
// 	{
		m_stStatic_Text[0].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stStatic_Text[0].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stStatic_Text[0].SetFont_Gdip(L"Arial", 9.0F);
// 		if (i == 0) m_stStatic_Text[i].SetText(_T("ComPort"));
// 		if (i == 1) m_stStatic_Text[i].SetText(_T("BaudRate"));
// 		if (i == 2)
// 		{
// 			if (stEq.nLanguage == 0)
// 				m_stStatic_Text[i].SetText(_T("모터 컨트롤러"));
// 			else
// 				m_stStatic_Text[i].SetText(_T("Motor Controller"));
// 		}
// 		if (i == 3)
// 		{
		if (stEq.nLanguage == 0)
			m_stStatic_Text[0].SetText(_T("모터 컨트롤러"));
		else
			m_stStatic_Text[0].SetText(_T("Motor Controller"));
//		}
//	}

	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		m_btnMotorState[nIndex].SetTextColor(COLOR_BLACK);
		m_btnMotorState[nIndex].SetFaceColor(RGB(225, 225, 225), true);
	}

	if (stEq.nLanguage == 0)
	{
		m_btnMotorState[0].SetWindowText(_T("Servo On"));
		m_btnMotorState[1].SetWindowText(_T("Servo Off"));
		m_btnMotorState[2].SetWindowText(_T("정지"));
		m_btnMotorState[3].SetWindowText(_T("알람 리셋"));
	}
	else
	{
		m_btnMotorState[0].SetWindowText(_T("Servo On"));
		m_btnMotorState[1].SetWindowText(_T("Servo Off"));
		m_btnMotorState[2].SetWindowText(_T("Stop"));
		m_btnMotorState[3].SetWindowText(_T("Alarm Reset"));
	}

// 	m_btnMotorConnect[0].EnableWindowsTheming(FALSE);
// 	m_btnMotorConnect[0].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnMotorConnect[0].m_bTransparent = false;
// 	m_btnMotorConnect[0].SetTextColor(COLOR_BLACK);
// 	m_btnMotorConnect[0].SetFaceColor(RGB(225, 225, 225), true);
// 
// 	m_btnMotorConnect[1].EnableWindowsTheming(FALSE);
// 	m_btnMotorConnect[1].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
// 	m_btnMotorConnect[1].m_bTransparent = false;
// 	m_btnMotorConnect[1].SetTextColor(COLOR_BLACK);
// 	m_btnMotorConnect[1].SetFaceColor(RGB(225, 225, 225), true);
// 
// 	if (stEq.nLanguage == 0)
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("연결"));
// 		m_btnMotorConnect[1].SetWindowText(_T("해제"));
// 	}
// 	else
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("Connect"));
// 		m_btnMotorConnect[1].SetWindowText(_T("Disconnect"));
// 	}

// 	m_stModelSelect.SetStaticStyle(CVGStatic::StaticStyle_Data);
// 	m_stModelSelect.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	m_stModelSelect.SetFont_Gdip(L"Arial", 9.0F);
// 	if (stEq.nLanguage == 0)
// 		m_stModelSelect.SetText(_T("연결"));
// 	else
// 		m_stModelSelect.SetText(_T("Connect"));

	//--------------------------------------------------------------------------
	// Speed Setting

	for (int i = 0; i < 2; i++)
	{
		m_stMotorSpeedSet_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stMotorSpeedSet_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stMotorSpeedSet_Sub[i].SetFont_Gdip(L"Arial", 9.0F);
		
		if (stEq.nLanguage == 0)
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedKor[i]);
		else
			m_stMotorSpeedSet_Sub[i].SetText(m_szTeachMotorSpeedEng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		m_btnMotorSpeedSet_AxisNum[i].SetTextColor(COLOR_BLACK);
		m_btnMotorSpeedSet_AxisNum[i].SetFaceColor(RGB(225, 225, 225), true);
		if (stEq.nLanguage == 0)
			m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
		else
			m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_SkyBlue);
			}
			else
			{
				m_stMotorSpeedSet_Thirdsub[i][j].SetStaticStyle(CVGStatic::StaticStyle_Data);
				m_stMotorSpeedSet_Thirdsub[i][j].SetColorStyle(CVGStatic::ColorStyle_White);
			}

			m_stMotorSpeedSet_Thirdsub[i][j].SetFont_Gdip(L"Arial", 9.0F);

			if (stEq.nLanguage == 0)
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedKor[i + 2]);
			}
			else
			{
				if (j == 0)
					m_stMotorSpeedSet_Thirdsub[i][j].SetText(m_szTeachMotorSpeedEng[i + 2]);
			}
		}
	}
	OnShowWindow(1,1);
}

// [21.1017.20] Button Enable / Disable
void CDlgMotorTeach::ChangeUI_Level()
{
	int nLevel = COMMON->GetPermissionGrade();
	if (nLevel > LVL_OPER)
	{
		if (nLevel == LVL_ADMIN)
		{
			m_btnMotorJogSelect_Move[0].EnableWindow(TRUE);
			m_btnMotorJogSelect_Move[1].EnableWindow(TRUE);
			m_btnMotorJogSelect_Home.EnableWindow(TRUE);
		}
		else
		{
			m_btnMotorJogSelect_Move[0].EnableWindow(FALSE);
			m_btnMotorJogSelect_Move[1].EnableWindow(FALSE);
			m_btnMotorJogSelect_Home.EnableWindow(FALSE);
		}

		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->EnableWindow(TRUE);

		//TEACHING
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_SHOW);


			GetDlgItem(IDC_STC_SCREW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK7)->ShowWindow(SW_SHOW);

// 			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_HIDE);
			


		

			GetDlgItem(IDC_BTN_SCREW_MOVE_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_MOVE_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_MOVE_03)->ShowWindow(SW_SHOW);
			
				
				
			GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_SHOW);
			



			GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(TRUE);

			GetDlgItem(IDC_STC_ADJUST2)->EnableWindow(TRUE);
			GetDlgItem(IDC_STC_A_S_POS_01)->EnableWindow(TRUE);
			GetDlgItem(IDC_STC_A_S_POS_06)->EnableWindow(TRUE);
			

			GetDlgItem(IDC_STC_A_S_POS_02)->EnableWindow(TRUE);
			GetDlgItem(IDC_STC_A_S_POS_03)->EnableWindow(TRUE);
			GetDlgItem(IDC_STC_A_S_POS_04)->EnableWindow(TRUE);
			GetDlgItem(IDC_STC_A_S_POS_05)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->EnableWindow(TRUE);

		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_03)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK7)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(FALSE);

		}
		else
		{
			GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(FALSE);
		}

		//CYL
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_SHOW);
			
			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_SHOW);
		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_SHOW);

		}
		else
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_HIDE);
		}
	
//
		//Door
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOUNLOCK9)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_MOTOR_TITLE_6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK9)->ShowWindow(SW_HIDE);
		}


 		//auto EqpType = SET_INSPECTOR;
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_SHOW);



			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_SHOW);
		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_HIDE);

		}
	}
	else
	{
		m_btnMotorJogSelect_Move[0].EnableWindow(FALSE);
		m_btnMotorJogSelect_Move[1].EnableWindow(FALSE);
		m_btnMotorJogSelect_Home.EnableWindow(FALSE);
		//TEACHING
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_SHOW);
	
			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_SHOW);
			
			GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_SHOW);
	
			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->EnableWindow(FALSE);
			
		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SCREW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_UNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_CONTROL_LOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SCREW_MOVE_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(FALSE);

		}
		else
		{
		GetDlgItem(IDC_STC_VIGNETING_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_VIGNETING_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_L)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_ADJUST)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_X_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_X_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_X_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_R_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_R_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_R_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_A_POS_1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_X_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_X_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_X_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_R_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_R_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_R_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_A_POS_1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STC_ADJUST2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_01)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_06)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_A_S_POS_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_01)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_02)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_03)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_04)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_05)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_A_S_POS_06)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_X_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_X_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_X_POS_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_R_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_R_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_R_POS_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_A_POS_1)->EnableWindow(FALSE);
		}


		

		//CYL
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_SHOW);
		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_13)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_14)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_15)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_16)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CYLINDER_17)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_CYLINDER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_CYLINDER_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SENSOR_001)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_002)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_003)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_004)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SENSOR_016)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SENSOR_001)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_002)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_003)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_004)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_005)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_006)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_007)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_008)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_009)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_010)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_011)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_012)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_013)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_014)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_015)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SENSOR_016)->ShowWindow(SW_HIDE);
		}


		GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_HIDE);




		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_DOLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK9)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_STC_MOTOR_TITLE_6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_TEACH_DOORLOCK9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOLOCK9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_DOUNLOCK9)->ShowWindow(SW_HIDE);
		}
		
		/////////
		GetDlgItem(IDC_BTN_TEACH_VIEW_MANUAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACH_SAVE_2)->EnableWindow(TRUE);
		//AXIS
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_SHOW);
		
			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_SHOW);



			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_SHOW);
 		}
		else if (SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_STC_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_AXIS_CURPOS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_AXIS_CURPOS_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_MOTOR_SERVO_ON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_SERVO_OFF)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_ALARMRESET)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_MOTOR_STOP)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_MODE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_DISTANCE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SPEED_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_MOVE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_JOG_SELECT_AXIS_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SPEED_AXIS_SELECT_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_TITLE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_SETTING_5)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_MOTOR_TITLE_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_MOTOR_TITLE_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_MOTOR_TITLE_5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_JOG_SELECT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SHOW_MOTORNUM2)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_SPEED_AXIS_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_SPEED_AXIS_2)->ShowWindow(SW_HIDE);

		}
	}
}


void CDlgMotorTeach::ChangeUI_Select(int nSelect)
{
	if (nSelect == 0)
	{
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_X_1) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_1) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3) ->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
		}
	}
	else if (nSelect == 1)
	{
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(FALSE);

			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
		}
	}
	else if (nSelect == 2)
	{
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
	}
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
	}
	}
	else if (nSelect == 3)
	{
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
	}
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
	}
	}
	else if (nSelect == 4)
	{
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
	}
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
	}
	}
	else
	{
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_X_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_R_3)->ShowWindow(SW_HIDE);
	}
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_L_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_POS_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_SCREW_R_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TEACHING_MOVE_A_1)->ShowWindow(SW_HIDE);
	}
	}
}



// [21.1027.2] Button Enable / Disable
void CDlgMotorTeach::ChangeUI_Jog(int mode)
{
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		BOOL bVisible1, bVisible2;
		if (mode == MOVE_DISTANCE)
		{
			bVisible1 = TRUE;
			bVisible2 = FALSE;
		}
		else
		{
			bVisible1 = FALSE;
			bVisible2 = TRUE;
		}

		for (int i = 0; i < 5; i++)
			m_btnMotorJogSelect_Distance[i].ShowWindow(bVisible1);
		m_edInput_JogDistance.ShowWindow(bVisible1);
		m_stMotorJogTitleSub[2].ShowWindow(bVisible1);

		for (int i = 0; i < 3; i++)
			m_btnMotorJogSelect_Speed[i].ShowWindow(bVisible2);
		m_stMotorJogTitleSub[3].ShowWindow(bVisible2);
	}
}

//---------------------------------------------------------------------------------------------------
// Teaching Data Load/Save
void CDlgMotorTeach::Save_TeachingData()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
	int index = 0;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			stSpeed.nMotorSpeed[i][j] = m_nMotorSpeed[i][j];
		}
	}

	double dTemp = 0;
	
	UpdateData(TRUE);

// 	index = m_cbFASBoard_Port.GetCurSel();
// 	stSpeed.nPort[0] = (int)m_cbFASBoard_Port.GetItemData(index);
// 	index = m_cbFASBoard_BaudRate.GetCurSel();
// 	stSpeed.nBaudRate[0] = (int)m_cbFASBoard_BaudRate.GetItemData(index);

	// Vigneting_X
	dTemp = _ttof(m_strTeach_Vigneting_X_Pos[0]) * 1000;
	stTeach.nVignetting_X_Wait = (int)dTemp;
	dTemp = _ttof(m_strTeach_Vigneting_X_Pos[1]) * 1000;
	stTeach.nVignetting_X_Test_01 = (int)dTemp;
	dTemp = _ttof(m_strTeach_Vigneting_X_Pos[2]) * 1000;
	stTeach.nVignetting_X_Test_02 = (int)dTemp;

	// Vigneting_R
	dTemp = _ttof(m_strTeach_Vigneting_R_Pos[0]) * 1000;
	stTeach.nVignetting_R_Wait = (int)dTemp;
	dTemp = _ttof(m_strTeach_Vigneting_R_Pos[1]) * 1000;
	stTeach.nVignetting_R_Test_01 = (int)dTemp;
	dTemp = _ttof(m_strTeach_Vigneting_R_Pos[2]) * 1000;
	stTeach.nVignetting_R_Test_02 = (int)dTemp;

	// Screw_L
	dTemp = _ttof(m_strTeach_Screw_L_Pos[0]) * 1000;
	stTeach.nScrew_L_Wait = (int)dTemp;
	dTemp = _ttof(m_strTeach_Screw_L_Pos[1]) * 1000;
	stTeach.nScrew_L_Commencement = (int)dTemp;
	dTemp = _ttof(m_strTeach_Screw_L_Pos[2]) * 1000;
	stTeach.nScrew_L_Work = (int)dTemp;

	// Screw_R
	dTemp = _ttof(m_strTeach_Screw_R_Pos[0]) * 1000;
	stTeach.nScrew_R_Wait = (int)dTemp;
	dTemp = _ttof(m_strTeach_Screw_R_Pos[1]) * 1000;
	stTeach.nScrew_R_Commencement = (int)dTemp;
	dTemp = _ttof(m_strTeach_Screw_R_Pos[2]) * 1000;
	stTeach.nScrew_R_Work = (int)dTemp;

	// Screw_R
	dTemp = _ttof(m_strTeach_Adjust_Pos[0]) * 1000;
	stTeach.nAdjust_Wait = (int)dTemp;
	dTemp = _ttof(m_strTeach_Adjust_Pos[1]) * 1000;
	stTeach.nAdjust_Work = (int)dTemp;


	

	dTemp = _tstoi(m_strTeach_Adjust_Spec_Pos[0]);
	stTeach.nAdjust_Spec = (int)dTemp;
	dTemp = _tstoi(m_strTeach_Adjust_Spec_Pos[1]);
	stTeach.nAdjust_Screw1_Spec = (int)dTemp;
	dTemp = _tstoi(m_strTeach_Adjust_Spec_Pos[2]);
	stTeach.nAdjust_Screw2_Spec = (int)dTemp;
	dTemp = _tstoi(m_strTeach_Adjust_Spec_Pos[3]);
	stTeach.nAdjust_RetryCnt = (int)dTemp;
	dTemp = _tstof(m_strTeach_Adjust_Spec_Pos[4]) * 1000;
	stTeach.nAdjust_Reverse = (int)dTemp;


	dTemp = _tstof(m_strTeach_Adjust_Spec_Pos[5]);
	stTeach.dAdjust_Offset_Y = dTemp;
	
	


	stTeach.nBarcode = m_Barcode;
// 	//모터 Z
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[0]) * 1000;
// //	stTeach.nSocketZ_Wait_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[1]) * 1000;
// //	stTeach.nSocketZ_FlareInspection1_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[2]) * 1000;
// //	stTeach.nSocketZ_Conveyor_Grap_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[3]) * 1000;
// //	stTeach.nSocketZ_Conveyor_Wait_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[4]) * 1000;
// //	stTeach.nSocketZ_FlareInspection2_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ_Pos[5]) * 1000;
// //	stTeach.nSocketZ_Reserved_Pos = dTemp;
// 
// 	//모터 Z2
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[0]) * 1000;
// //	stTeach.nSocketZ2_Wait_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[1]) * 1000;
// //	stTeach.nSocketZ2_FlareInspection1_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[2]) * 1000;
// // 	stTeach.nSocketZ2_Conveyor_Grap_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[3]) * 1000;
// //	stTeach.nSocketZ2_Conveyor_Wait_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[4]) * 1000;
// //	stTeach.nSocketZ2_FlareInspection2_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_AxisZ2_Pos[5]) * 1000;
// //	stTeach.nSocketZ2_Reserved_Pos = dTemp;
// 
// 	dTemp = _ttof(m_strTeach_ChartAxisZ_Pos[0]) * 1000;
// //	stTeach.nChartZ_01_Pos = dTemp;
// 	dTemp = _ttof(m_strTeach_ChartAxisZ_Pos[1]) * 1000;
// //	stTeach.nChartZ_02_Pos = dTemp;
// 	dTemp = _ttof(m_strTeach_ChartAxisZ_Pos[2]) * 1000;
//	stTeach.nChartZ_03_Pos = dTemp;	

//	stTeach.nSampleModelType = m_nSampleModelType;
	
	DATAMANAGER->SetMotionParam(stSpeed);
	DATAMANAGER->SetTeachingData(stTeach);
}

void CDlgMotorTeach::Load_TeachingData()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_nMotorSpeed[i][j] = stSpeed.nMotorSpeed[i][j];
		}
	}

	CString strTemp = _T("");
	//Vigneting_X
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_X_Wait / 1000));
	m_strTeach_Vigneting_X_Pos[0] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_X_Test_01 / 1000));
	m_strTeach_Vigneting_X_Pos[1] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_X_Test_02 / 1000));
	m_strTeach_Vigneting_X_Pos[2] = strTemp;

	//Vigneting_R
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_R_Wait / 1000));
	m_strTeach_Vigneting_R_Pos[0] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_R_Test_01 / 1000));
	m_strTeach_Vigneting_R_Pos[1] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nVignetting_R_Test_02 / 1000));
	m_strTeach_Vigneting_R_Pos[2] = strTemp;

	//Screw_L
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_L_Wait / 1000));
	m_strTeach_Screw_L_Pos[0] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_L_Commencement / 1000));
	m_strTeach_Screw_L_Pos[1] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_L_Work / 1000));
	m_strTeach_Screw_L_Pos[2] = strTemp;
	//Screw_R
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_R_Wait / 1000));
	m_strTeach_Screw_R_Pos[0] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_R_Commencement / 1000));
	m_strTeach_Screw_R_Pos[1] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nScrew_R_Work / 1000));
	m_strTeach_Screw_R_Pos[2] = strTemp;

	//Screw_R
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nAdjust_Wait / 1000));
	m_strTeach_Adjust_Pos[0] = strTemp;
	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nAdjust_Work / 1000));
	m_strTeach_Adjust_Pos[1] = strTemp;


	strTemp.Format(_T("%d"), (int)((int)stTeach.nAdjust_Spec));
	m_strTeach_Adjust_Spec_Pos[0] = strTemp;

	strTemp.Format(_T("%d"), (int)((int)stTeach.nAdjust_Screw1_Spec));
	m_strTeach_Adjust_Spec_Pos[1] = strTemp;

	strTemp.Format(_T("%d"), (int)((int)stTeach.nAdjust_Screw2_Spec));
	m_strTeach_Adjust_Spec_Pos[2] = strTemp;

	strTemp.Format(_T("%d"), (int)((int)stTeach.nAdjust_RetryCnt));
	m_strTeach_Adjust_Spec_Pos[3] = strTemp;

	strTemp.Format(_T("%0.3f"), (double)((double)stTeach.nAdjust_Reverse / 1000));
	m_strTeach_Adjust_Spec_Pos[4] = strTemp;

	strTemp.Format(_T("%0.1f"), (double)((double)stTeach.dAdjust_Offset_Y));
	m_strTeach_Adjust_Spec_Pos[5] = strTemp;
	
	m_Barcode = stTeach.nBarcode;

	UpdateData(FALSE);
}

void CDlgMotorTeach::OnBnClickedBtnJogSelectMode1()
{
	m_nJogMode = 0;		// 속도모드
	CheckSelectJogMode();
	ChangeUI_Jog(m_nJogMode);
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectMode2()
{
	m_nJogMode = 1;		// 상대이동 모드
	CheckSelectJogMode();
	ChangeUI_Jog(m_nJogMode);
}
void CDlgMotorTeach::CheckSelectJogMode()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 2; i++)
	{
		if (i == m_nJogMode)
		{
			m_btnMotorJogSelect_Mode[i].SetFaceColor(COLOR_GREEN1, true);

			if( stEq.nLanguage == 0)
				m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnKor[i]);
			else
				m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnEng[i]);
		}
		else
		{
			m_btnMotorJogSelect_Mode[i].SetTextColor(COLOR_BLACK);
			m_btnMotorJogSelect_Mode[i].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnKor[i]);
			else
				m_btnMotorJogSelect_Mode[i].SetWindowText(m_szTeachJogBtnEng[i]);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis1()
{
	m_nSelectAxisNum = 0;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis2()
{
	m_nSelectAxisNum = 1;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis3()
{
	m_nSelectAxisNum = 2;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis4()
{
	m_nSelectAxisNum = 3;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis5()
{
	m_nSelectAxisNum = 4;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis6()
{
	m_nSelectAxisNum = 5;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis7()
{
	m_nSelectAxisNum = 6;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis8()
{
	m_nSelectAxisNum = 7;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis9()
{
	m_nSelectAxisNum = 8;	
	CheckSelectAxis();
}
void CDlgMotorTeach::OnBnClickedBtnJogSelectAxis10()
{
	m_nSelectAxisNum = 9;	
	CheckSelectAxis();
}

void CDlgMotorTeach::CheckSelectAxis()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	CString strText = _T("");

	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		if (m_nSelectAxisNum >= (int)MOTOR_MAX_COUNT)
		{
			m_nSelectAxisNum = -1;
		}
		else
		{
			if (stEq.nLanguage == 0)
			{
				strText.Format(_T("축 %d"), m_nSelectAxisNum + 1);
				m_stStatic_Text[3].SetText(m_szTeachJogBtnKor[2 + m_nSelectAxisNum]);
			}
			else
			{
				strText.Format(_T("Axis %d"), m_nSelectAxisNum + 1);
				m_stStatic_Text[3].SetText(m_szTeachJogBtnEng[2 + m_nSelectAxisNum]);
			}
		}
 	}
// 	else
// 	{
// 		if (m_nSelectAxisNum >= (int)MOTOR_MAX_COUNT)
// 		{
// 			m_nSelectAxisNum = -1;
// 		}
// 		else
// 		{
// 			if (stEq.nLanguage == 0)
// 			{
// 				strText.Format(_T("축 %d"), m_nSelectAxisNum + 1);
// 				m_stStatic_Text[3].SetText(m_szTeachJogBtnKor[2 + m_nSelectAxisNum]);
// 			}
// 			else
// 			{
// 				strText.Format(_T("Axis %d"), m_nSelectAxisNum + 1);
// 				m_stStatic_Text[3].SetText(m_szTeachJogBtnEng[2 + m_nSelectAxisNum]);
// 			}
// 		}
// 	}
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{
			if (i == m_nSelectAxisNum)
			{
				m_btnMotorJogSelect_Axis[i].SetFaceColor(COLOR_GREEN1, true);
			}
			else
			{
				m_btnMotorJogSelect_Axis[i].SetTextColor(COLOR_BLACK);
				m_btnMotorJogSelect_Axis[i].SetFaceColor(RGB(225, 225, 225), true);
			}

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
			else
				m_btnMotorJogSelect_Axis[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);
		}

		if (m_nSelectAxisNum == -1)
		{
			//COMMON->ShowMsgDlg_Domodal(_T("조그이동에 사용할 모터 번호를 선택하세요."), MODE_INFORMATION);
			if (stEq.nLanguage == 0)
				COMMON->ShowMsg_Thread(_T("조그이동에 사용할 모터 번호를 선택하세요."), MODE_INFORMATION);
			else
				COMMON->ShowMsg_Thread(_T("Select the motor number to use for JOG movement.."), MODE_INFORMATION);

		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogDistance1()
{
	m_nSelectDistance = 0;
	m_dMoveDistance = 1.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance2()
{
	m_nSelectDistance = 1;
	m_dMoveDistance = 3.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance3()
{
	m_nSelectDistance = 2;
	m_dMoveDistance = 5.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance4()
{
	m_nSelectDistance = 3;
	m_dMoveDistance = 10.0f;

	CheckSelectDistance();
}
void CDlgMotorTeach::OnBnClickedBtnJogDistance5()
{
	m_nSelectDistance = 4;

	CString strTemp = _T("");
	double dDistance = 0.0f;

	GetDlgItemText(IDC_EDIT1, strTemp);
	dDistance = _ttof(strTemp);

	m_dMoveDistance = dDistance;

	CheckSelectDistance();
}
void CDlgMotorTeach::CheckSelectDistance()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 5; i++)
	{
		if (m_nSelectDistance == i)
		{
			m_btnMotorJogSelect_Distance[i].SetFaceColor(COLOR_GREEN1, true);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnKor[12 + i]);
			else
				m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnEng[12 + i]);
		}
		else
		{
			m_btnMotorJogSelect_Distance[i].SetTextColor(COLOR_BLACK);
			m_btnMotorJogSelect_Distance[i].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnKor[12 + i]);
			else
				m_btnMotorJogSelect_Distance[i].SetWindowText(m_szTeachJogBtnEng[12 + i]);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogSpeed1()
{
	m_nSelectSpeed = 0;
	m_dMoveSpeed = 2.0f;

	CheckSelectSpeed();
}
void CDlgMotorTeach::OnBnClickedBtnJogSpeed2()
{
	m_nSelectSpeed = 1;
	m_dMoveSpeed = 10.0f;

	CheckSelectSpeed();
}
void CDlgMotorTeach::OnBnClickedBtnJogSpeed3()
{
	m_nSelectSpeed = 2;
	m_dMoveSpeed = 50.0f;

	CheckSelectSpeed();
}

void CDlgMotorTeach::CheckSelectSpeed()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	for (int i = 0; i < 3; i++)
	{
		if (m_nSelectSpeed == i)
		{
			m_btnMotorJogSelect_Speed[i].SetFaceColor(COLOR_GREEN1, true);

			if(stEq.nLanguage == 0 )
				m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnKor[17 + i]);
			else
				m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnEng[17 + i]);
		}
		else
		{
			m_btnMotorJogSelect_Speed[i].SetTextColor(COLOR_BLACK);
			m_btnMotorJogSelect_Speed[i].SetFaceColor(RGB(225, 225, 225), true);

			if (stEq.nLanguage == 0)
				m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnKor[17 + i]);
			else
				m_btnMotorJogSelect_Speed[i].SetWindowText(m_szTeachJogBtnEng[17 + i]);
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnJogMove3()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	if (m_nSelectAxisNum < 0)
	{
		if (m_nSelectAxisNum == -1)
		{
			if(stEq.nLanguage == 0)
				COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
			else
				COMMON->ShowMsgDlg_Domodal(_T("Motor shaft is not selected. Please choose the motor shaft."), MODE_INFORMATION);
		}

		return;
	}

	CDlgAxisHomeProgress dlg(0);
	dlg.DoModal();
}

void CDlgMotorTeach::CylinderManual_Acitve()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

// 	int ID = nID - IDC_BTN_DOLOCK1;
// 
// 	int nFront = 0;
// 	int nBack = 0;
 	BOOL nReverse = 0;
 	int nOutSignal = 0;
// 	if (ID > 3)
// 		nReverse = 1;

	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_DOLOCK1:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 0;
	}
	break;
	case IDC_BTN_DOLOCK2:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 1;
	}
	break;
	case IDC_BTN_DOLOCK3:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 2;
	}
	break;
	case IDC_BTN_DOLOCK4:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 3;
	}
	break;
	case IDC_BTN_DOLOCK5:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 4;
	}
	break;
	case IDC_BTN_DOLOCK6:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 5;
	}
	break;
	case IDC_BTN_DOLOCK7:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 6;
	}
	break;
	case IDC_BTN_DOLOCK8:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 7;
	}
	break;
	case IDC_BTN_DOLOCK9:
	{
		AJIN_IO->SetON(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF);
		nReverse = 0;
		nOutSignal = 8;
	}
	break;

	case IDC_BTN_DOUNLOCK1:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 0;
	}
	break;
	case IDC_BTN_DOUNLOCK2:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 1;
	}
	break;
	case IDC_BTN_DOUNLOCK3:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 2;
	}
	break;
	case IDC_BTN_DOUNLOCK4:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 3;
	}
	break;
	case IDC_BTN_DOUNLOCK5:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 4;
	}
	break;
	case IDC_BTN_DOUNLOCK6:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 5;
	}
	break;
	case IDC_BTN_DOUNLOCK7:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 6;
	}
	break;
	case IDC_BTN_DOUNLOCK8:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 7;
	}
	break;
	case IDC_BTN_DOUNLOCK9:
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF);
		nReverse = 1;
		nOutSignal = 8;
	}
	break;
	}

	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF + nOutSignal, !nReverse);

	m_btnDoorLockManual[nReverse][nOutSignal].SetFaceColor(COLOR_GREEN1, true);
	nReverse = nReverse == 1 ? 0 : 1;
	m_btnDoorLockManual[nReverse][nOutSignal].SetTextColor(COLOR_BLACK);
	m_btnDoorLockManual[nReverse][nOutSignal].SetFaceColor(RGB(225, 225, 225), true);
	
// 	if (stEq.nLanguage == 0)
// 	{
// 		if (nBack == 0)
// 			m_btnDoorLockManual[nBack][nFront].SetWindowText(_T("잠금"));
// 		else
// 			m_btnDoorLockManual[nBack][nFront].SetWindowText(_T("열림"));
// 	}
// 	else
// 	{
// 		if (nBack == 0)
// 			m_btnDoorLockManual[nBack][nFront].SetWindowText(_T("Lock"));
// 		else
// 			m_btnDoorLockManual[nBack][nFront].SetWindowText(_T("Unlock"));
// 	}	
}

void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect1()
{
	m_nSetSpeed_AxisNum = 0;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect2()
{
	m_nSetSpeed_AxisNum = 1;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect3()
{
	m_nSetSpeed_AxisNum = 2;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect4()
{
	m_nSetSpeed_AxisNum = 3;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect5()
{
	m_nSetSpeed_AxisNum = 4;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect6()
{
	m_nSetSpeed_AxisNum = 5;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect7()
{
	m_nSetSpeed_AxisNum = 6;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect8()
{
	m_nSetSpeed_AxisNum = 7;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect9()
{
	m_nSetSpeed_AxisNum = 8;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::OnBnClickedBtnSpeedAxisSelect10()
{
	m_nSetSpeed_AxisNum = 9;
	CheckSelectMotorSpeed_AxisNum();
}
void CDlgMotorTeach::CheckSelectMotorSpeed_AxisNum()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
	{
		for (int i = 0; i < (int)MOTOR_MAX_COUNT; i++)
		{

			if (m_nSetSpeed_AxisNum == i)
			{
				m_btnMotorSpeedSet_AxisNum[i].SetFaceColor(COLOR_GREEN1, true);
				if (stEq.nLanguage == 0)
					m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
				else
					m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);
			}
			else
			{
				m_btnMotorSpeedSet_AxisNum[i].SetTextColor(COLOR_BLACK);
				m_btnMotorSpeedSet_AxisNum[i].SetFaceColor(RGB(225, 225, 225), true);
				if (stEq.nLanguage == 0)
					m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnKor[2 + i]);
				else
					m_btnMotorSpeedSet_AxisNum[i].SetWindowText(m_szTeachJogBtnEng[2 + i]);
			}
		}

		for (int i = 0; i < 5; i++)
		{
			strTemp.Format(_T("%d"), m_nMotorSpeed[m_nSetSpeed_AxisNum][i]);
			m_stMotorSpeedSet_Thirdsub[i][1].SetText(strTemp);
		}
	}
}

void CDlgMotorTeach::MotorSpeedChange(UINT nID)
{
	int ID = nID - IDC_STC_SPEED_SETTING_1;

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][ID]);

	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][ID] = nNum;
		m_stMotorSpeedSet_Thirdsub[ID][1].SetText(strTemp);
	}
}

void CDlgMotorTeach::OnStnClickedStcSpeedSetting1()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	
		if(stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The motor axis is not selected. Please select a motor axis."), MODE_INFORMATION);

		return;
	}
		
	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][0]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());


			if (FALSE == SetInsertValue(0, nNum))
				return;


		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][0] = nNum;
		m_stMotorSpeedSet_Thirdsub[0][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting2()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The motor axis is not selected. Please select a motor axis."), MODE_INFORMATION);

		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][1]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());

			if (FALSE == SetInsertValue(0, nNum))
				return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][1] = nNum;
		m_stMotorSpeedSet_Thirdsub[1][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting3()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The motor axis is not selected. Please select a motor axis."), MODE_INFORMATION);

		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][2]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());

			if (FALSE == SetInsertValue(0, nNum))
				return;
		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][2] = nNum;
		m_stMotorSpeedSet_Thirdsub[2][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting4()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The motor axis is not selected. Please select a motor axis."), MODE_INFORMATION);

		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][3]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInsertValue(1, nNum))
				return;

		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][3] = nNum;
		m_stMotorSpeedSet_Thirdsub[3][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}
void CDlgMotorTeach::OnStnClickedStcSpeedSetting5()
{
	if (m_nSetSpeed_AxisNum < 0)
	{
		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

		if (stEq.nLanguage == 0)
			COMMON->ShowMsgDlg_Domodal(_T("모터축이 선택되지 않았습니다. 모터축을 선택해주세요."), MODE_INFORMATION);
		else
			COMMON->ShowMsgDlg_Domodal(_T("The motor axis is not selected. Please select a motor axis."), MODE_INFORMATION);

		return;
	}

	int nNum = 0;
	CString strTemp = _T("");

	CDlgNumPad Dlg;
	Dlg.SetValue(nNum = m_nMotorSpeed[m_nSelectAxisNum][4]);
	if (Dlg.DoModal() == IDOK)
	{
		nNum = _ttoi(Dlg.GetCurrentNum());
		if (FALSE == SetInsertValue(1, nNum))
		return;

		strTemp.Format(_T("%d"), nNum);

		m_nMotorSpeed[m_nSetSpeed_AxisNum][4] = nNum;
		m_stMotorSpeedSet_Thirdsub[4][1].SetText(strTemp);

		UpdateData(FALSE);
	}
}


void CDlgMotorTeach::OnBnClickedBtnTeachSave1()
{
	Save_TeachingData();
}


void CDlgMotorTeach::OnBnClickedBtnTeachSave2()
{
	CDlgSaveAsModel Dlg;
	Dlg.DoModal();

}


void CDlgMotorTeach::OnBnClickedBtnTeachSave3()
{
	// Load
}


// void CDlgMotorTeach::OnBnClickedBtnMotorConnect1()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 	MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
// 	
// 	if (FAS_MO->m_bConnected == TRUE)
// 	{
// 		WRITE_MAIN_LOG(_T("Connected Already"));
// 		return;
// 	}
// 	
// 	int nPort = stSpeed.nPort[0];
// 	DWORD dwBaudRate = 0;
// 
// 	CString strTemp = _T("");
// 	TCHAR *BaudList_Motor[] = { _T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200") };
// 	strTemp.Format(_T("%s"), BaudList_Motor[ stSpeed.nBaudRate[0] ]);
// 
// 	dwBaudRate = _ttoi(strTemp);
// 
// 	BOOL bRet = FALSE;
// 	bRet = FAS_MO->Connect_EziServo(nPort, dwBaudRate);
// 
// 	if (bRet == TRUE)
// 	{
// 		ChangeConnectUI(TRUE);
// 		if(stEq.nLanguage ==0)
// 			WRITE_MAIN_LOG(_T("[CONNCECT] Connect에 성공하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[CONNCECT] Succeeded in connecting \n"));
// 		m_dwOldAxisStatus = 0x00;
// 
// 		OnServoInitMotor();
// 
// 		FAS_MO->Servo_Alarm_Reset_All();
// 	}
// 	else
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[CONNCECT] Connect에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[CONNCECT] Failed in connecting \n"));
// 
// 	}
// 
// 
// 	if (stEq.nLanguage == 0)
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("연결"));
// 		m_btnMotorConnect[1].SetWindowText(_T("해제"));
// 	}
// 	else
// 	{
// 		m_btnMotorConnect[0].SetWindowText(_T("Connect"));
// 		m_btnMotorConnect[1].SetWindowText(_T("Disconnect"));
// 	}
// }
// void CDlgMotorTeach::OnBnClickedBtnMotorDisconnect1()
// {
// 	FAS_MO->Disconnect_EziServo();
// 	ChangeConnectUI(FALSE);
// }

void CDlgMotorTeach::SetComboBox_Serial(CComboBox *pBox, int Data)
{
	int count;
	count = pBox->GetCount();
	for (int i = 0; i < count; i++)
	{
		if (Data == pBox->GetItemData(i))
		{
			pBox->SetCurSel(i);
			break;
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnMotorServoOn()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
//	FAS_MO->m_nSlaveNo = m_nSelectAxisNum;
	//Servo On 실패 시
	AJIN_MO->ServoOn((long)m_nSelectAxisNum);
	if (!AJIN_MO->IsServoOn((long)m_nSelectAxisNum))
	{
		if(stEq.nLanguage ==0)
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo On에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo On failed. \n"));
		return;
	}
	ChangeServoUI(TRUE);
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo On에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo On succeeded.\n"));
}

void CDlgMotorTeach::OnBnClickedBtnMotorServoOff()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	AJIN_MO->ServoOff((long)m_nSelectAxisNum);
	if (AJIN_MO->IsServoOn((long)m_nSelectAxisNum))
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo On\ff 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[SERVO ON] Servo Off failed. \n"));
		return;
	}
	ChangeServoUI(FALSE);
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo Off에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[SERVO ON] Servo Off succeeded.\n"));
}

void CDlgMotorTeach::OnBnClickedBtnMotorStop()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	//선택한 축 Stop
	if (!AJIN_MO->MoveEStop((long)m_nSelectAxisNum))
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[MOTOR STOP] Stop에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[MOTOR STOP] Motor Stop failed \n"));
		return;
	}
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[MOTOR STOP] Stop에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[MOTOR STOP] Motor Stop succeeded \n"));

}


void CDlgMotorTeach::OnBnClickedBtnMotorAlarmreset()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	//Alarm reset 실패 시
	if (!AJIN_MO->SetErrorReset((long)m_nSelectAxisNum))
	{
		if(stEq.nLanguage ==0)
			WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset Failed\n"));
		return;
	}
	if (stEq.nLanguage == 0)
		WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset에 성공하였습니다 \n"));
	else
		WRITE_MAIN_LOG(_T("[ALARM] Alarm Reset succeeded \n"));
}

void CDlgMotorTeach::OnServoInitMotor()
{
// 	for (int i = 0; i < MOTOR_MAX_COUNT; i++)
// 		FAS_MO->Servo_Enable(i);
}

// void CDlgMotorTeach::ChangeConnectUI(BOOL bState)
// {
// 		MOTION_PARAM	stSpeed = *DATAMANAGER->GetCurrentMotionParam();
// 
// // 		m_cbFASBoard_Port.SetCurSel(stSpeed.nPort[0]-1);
// // 		m_cbFASBoard_BaudRate.SetCurSel(stSpeed.nBaudRate[0]);
// 
// // 		m_btnMotorConnect[!bState].SetFaceColor(COLOR_GREEN1, true);
// // 		m_btnMotorConnect[bState].SetTextColor(COLOR_BLACK);
// // 		m_btnMotorConnect[bState].SetFaceColor(RGB(225, 225, 225), true);
// }

void CDlgMotorTeach::ChangeServoUI(BOOL bState)
{
		m_btnMotorState[!bState].SetFaceColor(COLOR_GREEN1, true);
		m_btnMotorState[bState].SetTextColor(COLOR_BLACK);
		m_btnMotorState[bState].SetFaceColor(RGB(225, 225, 225), true);

	//	m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Red) : m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Black)


	//	SetAxisStatus(FAS_MO->GetAxisStatus(m_nSelectAxisNum));
}

void CDlgMotorTeach::SetAxisStatus(DWORD dwValue)
{
	CStatic* pStatic;
	char	chValue[12];
	DWORD	dwXOR;
// 	EZISERVO_AXISSTATUS AxisStatus;
// 	CString strValue = _T("");
// 
// 	dwXOR = dwValue ^ m_dwOldAxisStatus;
// 
// 	if (dwXOR == 0)
// 	{
// 		return;
// 	}

// 	m_dwOldAxisStatus = dwValue;
// 	AxisStatus.dwValue = dwValue;
//	sprintf_s(chValue, "0x%08X", dwValue);

//	strValue = (CString)chValue;

//	SetDlgItemText(IDC_STATIC_AXISSTATUS, strValue);

// 	for (int i = 0; i < AXIS_MOTOR_STATUES; i++)
// 	{
// 		if (IS_BIT_SET(dwXOR, i))
// 		{
// 			IS_BIT_SET(dwValue, i) ? m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Red) : m_stStatic_Motor_Statuses[i].SetColorStyle(CVGStatic::ColorStyle_Black);
// 		}
// 	}
}

void CDlgMotorTeach::OnChangeBtnUI()
{
	ChangeManualBtnUI();
	CheckSelectJogMode();
	CheckSelectAxis();
	CheckSelectDistance();
	CheckSelectSpeed();
	CheckSelectMotorSpeed_AxisNum();
/*
	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X1_18_FRONT_DOOR1_INTERLOCK_ON_OFF))
	{
		m_btnDoorLockManual[0][0].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[1][0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}
	else
	{
		m_btnDoorLockManual[1][0].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[0][0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}

	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X1_1A_FRONT_DOOR2_INTERLOCK_ON_OFF))
	{
		m_btnDoorLockManual[0][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[1][1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}
	else
	{
		m_btnDoorLockManual[1][1].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[0][1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}

	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X1_1C_REAR_DOOR1_INTERLOCK_ON_OFF))
	{
		m_btnDoorLockManual[0][2].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[1][2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}
	else
	{
		m_btnDoorLockManual[1][2].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[0][2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}

	if (TRUE == AJIN_IO->GetInputIoBit(DIO_INPUT_X1_1E_REAR_DOOR2_INTERLOCK_ON_OFF))
	{
		m_btnDoorLockManual[0][3].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[1][3].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}
	else
	{
		m_btnDoorLockManual[1][3].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_btnDoorLockManual[0][3].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	}
	*/
}
void CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_X()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	int nPos = 0;
	CString strPos;

	if (AfxMessageBox(_T("Do you want change Teaching Data?"), MB_OKCANCEL) == IDCANCEL)
		return;
	if (AJIN_MO->IsOrigin(VIGNETTING_X) == FALSE) {
			AfxMessageBox(_T("Origin Check please!"), MB_OK);
			return;
	}
	nPos = AJIN_MO->GetActPos(VIGNETTING_X);
	strPos.Format(_T("%0.3f"), (double)nPos / 1000);
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_POS_X_1:
	{
		GetDlgItem(IDC_EDIT_X_POS_1)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_X_2:
	{
		GetDlgItem(IDC_EDIT_X_POS_2)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_X_3:
	{
		GetDlgItem(IDC_EDIT_X_POS_3)->SetWindowTextW(strPos);
		break;
	}
	break;
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_X()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();


	if (AJIN_MO->IsOrigin(VIGNETTING_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("L Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("R Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("L Check Jig seating Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("R Check Jig seating Sensnor"), MB_OK);
		return;
	}
#endif
	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	BOOL bChk = FALSE;
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_MOVE_X_1:
	{
		GetDlgItemText(IDC_EDIT_X_POS_1, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_X_2:
	{
		GetDlgItemText(IDC_EDIT_X_POS_2, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_X_3:
	{
		GetDlgItemText(IDC_EDIT_X_POS_3, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	break;
	}	
	if (bChk == TRUE)
	{
		if (AJIN_MO->MoveAxis_Wait(VIGNETTING_X, dPos, dSpeed) == FALSE)
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting X 이동에 실패하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting X failed to move. \n"));
		}
		else
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting X 이동에 성공하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting X successfully to move. \n"));
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPos_Vigneting_R()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	int nPos = 0;
	CString strPos;

	if (AfxMessageBox(_T("Do you want change Teaching Data?"), MB_OKCANCEL) == IDCANCEL)
		return;
	if (AJIN_MO->IsOrigin(VIGNETTING_R) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
	nPos = AJIN_MO->GetActPos(VIGNETTING_R);
	strPos.Format(_T("%0.3f"), (double)nPos / 1000);
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_POS_R_1:
	{
		GetDlgItem(IDC_EDIT_R_POS_1)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_R_2:
	{
		GetDlgItem(IDC_EDIT_R_POS_2)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_R_3:
	{
		GetDlgItem(IDC_EDIT_R_POS_3)->SetWindowTextW(strPos);
		break;
	}
	break;
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMove_Vigneting_R()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	if (AJIN_MO->IsOrigin(VIGNETTING_R) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("L Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("R Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("L Check Jig seating Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("R Check Jig seating Sensnor"), MB_OK);
		return;
	}
#endif
	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	BOOL bChk = FALSE;
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_MOVE_R_1:
	{
		GetDlgItemText(IDC_EDIT_R_POS_1, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_R_2:
	{
		GetDlgItemText(IDC_EDIT_R_POS_2, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_R_3:
	{
		GetDlgItemText(IDC_EDIT_R_POS_3, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	break;
	}
	if (bChk == TRUE)
	{
		if (AJIN_MO->MoveAxis_Wait(VIGNETTING_R, dPos, dSpeed) == FALSE)
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting R 이동에 실패하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting R failed to move. \n"));
		}
		else
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting R 이동에 성공하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] Vignetting R successfully to move. \n"));
		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_L()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	int nPos = 0;
	CString strPos;

	if (AfxMessageBox(_T("Do you want change Teaching Data?"), MB_OKCANCEL) == IDCANCEL)
		return;
	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
	nPos = AJIN_MO->GetActPos(SCREW_L_X);
	strPos.Format(_T("%0.3f"), (double)nPos / 1000);
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_POS_SCREW_L_1:
	{
		GetDlgItem(IDC_EDIT_SCREW_L_POS_1)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_SCREW_L_2:
	{
		GetDlgItem(IDC_EDIT_SCREW_L_POS_2)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_SCREW_L_3:
	{
		GetDlgItem(IDC_EDIT_SCREW_L_POS_3)->SetWindowTextW(strPos);
		break;
	}
	break;
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_L()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
// 		AfxMessageBox(_T("L Check Jig Out Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
// 		AfxMessageBox(_T("R Check Jig Out Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) == TRUE) {
// 		AfxMessageBox(_T("L Check Jig seating Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) == TRUE) {
// 		AfxMessageBox(_T("R Check Jig seating Sensnor"), MB_OK);
// 		return;
// 	}
#endif
	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	BOOL bChk = FALSE;
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_MOVE_SCREW_L_1:
	{
		GetDlgItemText(IDC_EDIT_SCREW_L_POS_1, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_SCREW_L_2:
	{
		GetDlgItemText(IDC_EDIT_SCREW_L_POS_2, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_SCREW_L_3:
	{
		GetDlgItemText(IDC_EDIT_SCREW_L_POS_3, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	break;
	}
	if (bChk == TRUE)
	{
// 		if (nID == IDC_BTN_TEACHING_MOVE_SCREW_L_1)
// 		{
// 			if (MO_MoveScrew_Type(Screw_Wait))
// 			{
// 				if (stEq.nLanguage == 0)
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 실패하였습니다 \n"));
// 				else
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X failed to move. \n"));
// 			}
// 			else
// 			{
// 				if (stEq.nLanguage == 0)
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 성공하였습니다 \n"));
// 				else
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X successfully to move. \n"));
// 			}
// 		}
// 		else if (nID ==  IDC_BTN_TEACHING_MOVE_SCREW_L_3)
// 		{
// 
// 			if (MO_MoveScrew_Type(Screw_Work))
// 			{
// 				if (stEq.nLanguage == 0)
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 실패하였습니다 \n"));
// 				else
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X failed to move. \n"));
// 			}
// 			else
// 			{
// 				if (stEq.nLanguage == 0)
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 성공하였습니다 \n"));
// 				else
// 					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X successfully to move. \n"));
// 			}
// 	
// 		}
// 		else
// 		{
			if (AJIN_MO->MoveAxis_Wait(SCREW_L_X, dPos, dSpeed) == FALSE)
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 실패하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X failed to move. \n"));
			}
			else
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 성공하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X successfully to move. \n"));
			}
	//	}

// 		if (AJIN_MO->MoveAxis_Wait(SCREW_L_X, dPos, dSpeed) == FALSE)
// 		{
// 			if (stEq.nLanguage == 0)
// 				WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 실패하였습니다 \n"));
// 			else
// 				WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X failed to move. \n"));
// 		}
// 		else
// 		{
// 			if (stEq.nLanguage == 0)
// 				WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X 이동에 성공하였습니다 \n"));
// 			else
// 				WRITE_MAIN_LOG(_T("[TEACHING] SCREW L X successfully to move. \n"));
// 		}
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingPos_Screw_R()
{
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	int nPos = 0;
	CString strPos;

	if (AfxMessageBox(_T("Do you want change Teaching Data?"), MB_OKCANCEL) == IDCANCEL)
		return;
	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
	nPos = AJIN_MO->GetActPos(SCREW_R_X);
	strPos.Format(_T("%0.3f"), (double)nPos / 1000);
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_POS_SCREW_R_1:
	{
		GetDlgItem(IDC_EDIT_SCREW_R_POS_1)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_SCREW_R_2:
	{
		GetDlgItem(IDC_EDIT_SCREW_R_POS_2)->SetWindowTextW(strPos);
		break;
	}
	case IDC_BTN_TEACHING_POS_SCREW_R_3:
	{
		GetDlgItem(IDC_EDIT_SCREW_R_POS_3)->SetWindowTextW(strPos);
		break;
	}
	break;
	}
}

void CDlgMotorTeach::OnBnClickedBtnTeachingMove_Screw_R()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
// 		AfxMessageBox(_T("L Check Jig Out Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
// 		AfxMessageBox(_T("R Check Jig Out Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) == TRUE) {
// 		AfxMessageBox(_T("L Check Jig seating Sensnor"), MB_OK);
// 		return;
// 	}
// 	if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) == TRUE) {
// 		AfxMessageBox(_T("R Check Jig seating Sensnor"), MB_OK);
// 		return;
// 	}
#endif
	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	BOOL bChk = FALSE;
	UINT nID = GetFocus()->GetDlgCtrlID();
	switch (nID) {
	case IDC_BTN_TEACHING_MOVE_SCREW_R_1:
	{
		GetDlgItemText(IDC_EDIT_SCREW_R_POS_1, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_SCREW_R_2:
	{
		GetDlgItemText(IDC_EDIT_SCREW_R_POS_2, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	case IDC_BTN_TEACHING_MOVE_SCREW_R_3:
	{
		GetDlgItemText(IDC_EDIT_SCREW_R_POS_3, strPos);
		dPos = _ttof(strPos);
		dPos = dPos;
		dSpeed = (double)stMotion.nMotorSpeed[0][2];
		bChk = TRUE;
		break;
	}
	break;
	}
	if (bChk == TRUE)
	{
		if (AJIN_MO->MoveAxis_Wait(SCREW_R_X, dPos, dSpeed) == FALSE)
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] SCREW R X 이동에 실패하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] SCREW R X failed to move. \n"));
		}
		else
		{
			if (stEq.nLanguage == 0)
				WRITE_MAIN_LOG(_T("[TEACHING] SCREW R X 이동에 성공하였습니다 \n"));
			else
				WRITE_MAIN_LOG(_T("[TEACHING] SCREW R X successfully to move. \n"));
		}
	}
}



void CDlgMotorTeach::OnBnClickedBtnTeachingMove_Adjust()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	if (AJIN_MO->IsOrigin(TILT_ADJUST_R) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}

	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	
	GetDlgItemText(IDC_EDIT_A_POS_1, strPos);
	dPos = _ttof(strPos);
	dPos = dPos;
	dSpeed = (double)stMotion.nMotorSpeed[0][2];

	double dTargetPos = 0.0f;
	double dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
	dTargetPos = dCurPos + dPos;

	if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] ADJUST R 이동에 실패하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] ADJUST R failed to move. \n"));
	}
	else
	{
		if (stEq.nLanguage == 0)
			WRITE_MAIN_LOG(_T("[TEACHING] ADJUST R 이동에 성공하였습니다 \n"));
		else
			WRITE_MAIN_LOG(_T("[TEACHING] ADJUST R successfully to move. \n"));
	}
}




void CDlgMotorTeach::OnBnClickedBtnCylinder1()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif
	//AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_BUFFER_LIFT_CYLINDER_DOWN,  OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_BUFFER_LIFT_CYLINDER_UP,ON);

	m_btnMotorManual[0][0].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[1][0].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[1][0].SetFaceColor(RGB(225, 225, 225), true);
}

void CDlgMotorTeach::OnBnClickedBtnCylinder13()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif
	
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_BUFFER_LIFT_CYLINDER_UP, OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_BUFFER_LIFT_CYLINDER_DOWN, ON);

	m_btnMotorManual[1][0].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[0][0].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[0][0].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder2()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();


#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetON(DIO_OUTPUT_Y1_03_STAU_CYL_UP);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_04_STAU_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
	AJIN_IO->SetON(DIO_OUTPUT_Y1_03_STAU_CYL_UP);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_04_STAU_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_BUFFER_SOCKET_CLAMP_SOL_OFF,  OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_BUFFER_SOCKET_CLAMP_SOL_ON,  ON);

	m_btnMotorManual[0][1].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[1][1].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[1][1].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder14()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_03_STAU_CYL_UP);
	AJIN_IO->SetON(DIO_OUTPUT_Y1_04_STAU_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_03_STAU_CYL_UP);
	AJIN_IO->SetON(DIO_OUTPUT_Y1_04_STAU_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_BUFFER_SOCKET_CLAMP_SOL_ON, OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_BUFFER_SOCKET_CLAMP_SOL_OFF, ON);

	m_btnMotorManual[1][1].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[0][1].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[0][1].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder3()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetON(DIO_OUTPUT_Y1_09_STAU_CHUCK_FWD);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_0A_STAU_CHUCK_BWD);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_01_LOADING1_SOCKET_CLAMP_OFF,  OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_00_LOADING1_SOCKET_CLAMP_ON,  ON);

	m_btnMotorManual[0][2].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[1][2].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[1][2].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder15()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_09_STAU_CHUCK_FWD);
	AJIN_IO->SetON(DIO_OUTPUT_Y1_0A_STAU_CHUCK_BWD);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif


	m_btnMotorManual[1][2].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[0][2].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[0][2].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder4()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_10_DBUL_JIG_L_CYL_UP);
	AJIN_IO->SetON(DIO_OUTPUT_Y1_11_DBUL_JIG_L_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_LOADING1_PROBEPIN_MOVE_OFF,OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_LOADING1_PROBEPIN_MOVE_ON, ON);

	m_btnMotorManual[0][3].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[1][3].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[1][3].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder16()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetON(DIO_OUTPUT_Y1_10_DBUL_JIG_L_CYL_UP);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_11_DBUL_JIG_L_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_LOADING1_PROBEPIN_MOVE_ON, OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_LOADING1_PROBEPIN_MOVE_OFF, ON);

	m_btnMotorManual[1][3].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[0][3].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[0][3].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder5()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_16_DBUR_JIG_R_CYL_UP);
	AJIN_IO->SetON(DIO_OUTPUT_Y1_17_DBUR_JIG_R_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_05_LOADING1_PROBEPIN_CONNECT_DOWN, OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_04_LOADING1_PROBEPIN_CONNECT_UP, ON);

	m_btnMotorManual[0][4].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[1][4].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[1][4].SetFaceColor(RGB(225, 225, 225), true);
}


void CDlgMotorTeach::OnBnClickedBtnCylinder17()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AJIN_IO->SetON(DIO_OUTPUT_Y1_16_DBUR_JIG_R_CYL_UP);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y1_17_DBUR_JIG_R_CYL_DOWN);
#elif(SET_INSPECTOR == SYS_TLA_OQC)	
#elif(SET_INSPECTOR == SYS_TLA_B)	
#endif

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_04_LOADING1_PROBEPIN_CONNECT_UP, OFF);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_05_LOADING1_PROBEPIN_CONNECT_DOWN, ON);

	m_btnMotorManual[1][4].SetFaceColor(COLOR_GREEN1, true);
	m_btnMotorManual[0][4].SetTextColor(COLOR_BLACK);
	m_btnMotorManual[0][4].SetFaceColor(RGB(225, 225, 225), true);
}


// void CDlgMotorTeach::OnBnClickedBtnCylinder6()
// {
// //	double dPos = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 	if ((AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_CONVEYOR2_SLOW1_SENSOR) == TRUE &&
// // 		AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_CONVEYOR2_STOP1_SENSOR) == TRUE)
// // 		|| dPos>200
// // 		)
// // 	{
// 		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_07_LOADING1_SOCKET_DOWN, OFF);
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_06_LOADING1_SOCKET_UP, ON);
// 
// 		m_btnMotorManual[0][5].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][5].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][5].SetFaceColor(RGB(225, 225, 225), true);
// 
// //	}
// }


// void CDlgMotorTeach::OnBnClickedBtnCylinder18()
// {
// // 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_CONVEYOR2_SLOW1_SENSOR) == FALSE &&
// // 		AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_CONVEYOR2_STOP1_SENSOR) == FALSE
// // 		)
// // 	{
// // 		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// // 
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_06_LOADING1_SOCKET_UP, OFF);
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_07_LOADING1_SOCKET_DOWN, ON);
// // 
// // 		m_btnMotorManual[1][5].SetFaceColor(COLOR_GREEN1, true);
// // 		m_btnMotorManual[0][5].SetTextColor(COLOR_BLACK);
// // 		m_btnMotorManual[0][5].SetFaceColor(RGB(225, 225, 225), true);
// // 	}
// }

// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder7()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_LOADING1_VACCUM_ON, ON);
// // #endif
// 
// 	m_btnMotorManual[0][6].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[1][6].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[1][6].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder19()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_08_LOADING1_VACCUM_ON, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, ON);
// // 	Sleep(100);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN, OFF);
// // #endif
// 
// 	m_btnMotorManual[1][6].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[0][6].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[0][6].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder8()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0B_LOADING2_SOCKET_CLAMP_OFF, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0A_LOADING2_SOCKET_CLAMP_ON, ON);
// 
// 	m_btnMotorManual[0][7].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[1][7].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[1][7].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder20()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0A_LOADING2_SOCKET_CLAMP_ON, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0B_LOADING2_SOCKET_CLAMP_OFF, ON);
// 
// 	m_btnMotorManual[1][7].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[0][7].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[0][7].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder9()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0D_LOADING2_PROBEPIN_MOVE_OFF, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0C_LOADING2_PROBEPIN_MOVE_ON, ON);
// 
// 	m_btnMotorManual[0][8].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[1][8].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[1][8].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder21()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0C_LOADING2_PROBEPIN_MOVE_ON, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0D_LOADING2_PROBEPIN_MOVE_OFF, ON);
// 
// 	m_btnMotorManual[1][8].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[0][8].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[0][8].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder10()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0F_LOADING2_PROBEPIN_CONNECT_DOWN, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0E_LOADING2_PROBEPIN_CONNECT_UP, ON);
// 
// 	m_btnMotorManual[0][9].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[1][9].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[1][9].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder22()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0E_LOADING2_PROBEPIN_CONNECT_UP, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0F_LOADING2_PROBEPIN_CONNECT_DOWN, ON);
// 
// 	m_btnMotorManual[1][9].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[0][9].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[0][9].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder11()
// {
// //	double dPos = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z2);
// // 	if ((AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_CONVEYOR2_SLOW2_SENSOR) == TRUE &&
// // 		AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_CONVEYOR2_STOP2_SENSOR) == TRUE) ||
// // 		dPos>200
// // 		)
// // 	{
// 		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// 		BOOL nState;
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_11_LOADING2_SOCKET_DOWN, OFF);
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_10_LOADING2_SOCKET_UP, ON);
// 
// 		m_btnMotorManual[0][10].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][10].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][10].SetFaceColor(RGB(225, 225, 225), true);
// //	}
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder23()
// {
// // 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_CONVEYOR2_SLOW2_SENSOR) == FALSE &&
// // 		AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_CONVEYOR2_STOP2_SENSOR) == FALSE
// // 		)
// // 	{
// // 		EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// // 
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_10_LOADING2_SOCKET_UP, OFF);
// // 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_11_LOADING2_SOCKET_DOWN, ON);
// // 
// // 		m_btnMotorManual[1][10].SetFaceColor(COLOR_GREEN1, true);
// // 		m_btnMotorManual[0][10].SetTextColor(COLOR_BLACK);
// // 		m_btnMotorManual[0][10].SetFaceColor(RGB(225, 225, 225), true);
// // 	}
// }
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder12()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_LOADING2_VACCUM_ON, ON);
// // #endif
// 
// 	m_btnMotorManual[0][11].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[1][11].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[1][11].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnCylinder24()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_12_LOADING2_VACCUM_ON, OFF);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, ON);
// // 	Sleep(100);
// // 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN, OFF);
// // #endif
// 	m_btnMotorManual[1][11].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnMotorManual[0][11].SetTextColor(COLOR_BLACK);
// 	m_btnMotorManual[0][11].SetFaceColor(RGB(225, 225, 225), true);
// }

// void CDlgMotorTeach::OnBnClickedBtnTeachingForCv1() //211029 cws 13번 하단으로 쭉 Stop시그널
// {
// //	FAS_MO->MoveStop(CONVEYOR_1);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[0][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP1)->EnableWindow(FALSE);
// 	Sleep(500);
// //	FAS_MO->MoveVelocity((long)0, dSpeed, 1);
// 
// //	GetDlgItem(IDC_BTN_TEACHING_FOR_CV1)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV1)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP1)->EnableWindow(TRUE);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingForCv2()
// {
// //	FAS_MO->MoveStop(CONVEYOR_2);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[1][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP2)->EnableWindow(FALSE);
// 	Sleep(500);
// //	FAS_MO->MoveVelocity((long)1, dSpeed, 1);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV2)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV2)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP2)->EnableWindow(TRUE);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingForCv3()
// {
// //	FAS_MO->MoveStop(CONVEYOR_3);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[2][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP3)->EnableWindow(FALSE);
// 	Sleep(500);
// //	FAS_MO->MoveVelocity((long)2, dSpeed, 1);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV3)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV3)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP3)->EnableWindow(TRUE);
// }


// void CDlgMotorTeach::OnBnClickedBtnTeachingBackCv1()
// {
// //	FAS_MO->MoveStop(CONVEYOR_1);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[0][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP1)->EnableWindow(FALSE);
// 	Sleep(500);
// //	FAS_MO->MoveVelocity((long)0, dSpeed, 0);
// //	GetDlgItem(IDC_BTN_TEACHING_FOR_CV1)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV1)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP1)->EnableWindow(TRUE);
// 
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingBackCv2()
// {
// //	FAS_MO->MoveStop(CONVEYOR_2);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[1][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP2)->EnableWindow(FALSE);
// 	Sleep(500);
// //	FAS_MO->MoveVelocity((long)1, dSpeed, 0);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV2)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV2)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP2)->EnableWindow(TRUE);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingBackCv3()
// {
// //	FAS_MO->MoveStop(CONVEYOR_3);
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 	double dSpeed = (double)stMotion.nMotorSpeed[2][2];
// 	GetDlgItem(IDC_BTN_TEACHING_STOP3)->EnableWindow(FALSE);
// 	Sleep(500);
// 
// //	FAS_MO->MoveVelocity((long)2, dSpeed, 0);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV3)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV3)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP3)->EnableWindow(TRUE);
// }
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingStop1()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// 
// 	//if (!FAS_MO->MoveStop(CONVEYOR_1))
// 	if(TRUE)
// 	{
// 		if(stEq.nLanguage ==0)
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop failed \n"));
// 
// 		return;
// 	}
// 	if (stEq.nLanguage == 0)
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 성공하였습니다 \n"));
// 	else
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop success \n"));
// 	//FAS_MO->SetResetPosition(CONVEYOR_1);
// //	GetDlgItem(IDC_BTN_TEACHING_FOR_CV1)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV1)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP1)->EnableWindow(FALSE);
// 
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingStop2()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// //	if (!FAS_MO->MoveStop(CONVEYOR_2))
// 	{
// 		if(stEq.nLanguage ==0)
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop failed \n"));
// 		return;
// 	}
// 	if(stEq.nLanguage ==0)
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 성공하였습니다 \n"));
// 	else
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop success \n"));
// 
// //	FAS_MO->SetResetPosition(CONVEYOR_2);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV2)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV2)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP2)->EnableWindow(FALSE);
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnTeachingStop3()
// {
// 	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
// //	if (!FAS_MO->MoveStop(CONVEYOR_3))
// 	{
// 		if (stEq.nLanguage == 0)
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 실패하였습니다 \n"));
// 		else
// 			WRITE_MAIN_LOG(_T("[STOP] Servo Stop failed \n"));
// 		return;
// 	}
// 	if (stEq.nLanguage == 0)
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop에 성공하였습니다 \n"));
// 	else
// 		WRITE_MAIN_LOG(_T("[STOP] Servo Stop success \n"));
// //	FAS_MO->SetResetPosition(CONVEYOR_3);
// 	GetDlgItem(IDC_BTN_TEACHING_FOR_CV3)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_BACK_CV3)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BTN_TEACHING_STOP3)->EnableWindow(FALSE);
// }


// void CDlgMotorTeach::OnBnClickedBtnSampleModelSelect1()
// {
// 	m_btnModelSelect[0].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnModelSelect[1].SetTextColor(COLOR_BLACK);
// 	m_btnModelSelect[1].SetFaceColor(RGB(225, 225, 225), true);
// 	m_nSampleModelType = 1;
// }
// 
// 
// void CDlgMotorTeach::OnBnClickedBtnSampleModelSelect2()
// {
// 	m_btnModelSelect[1].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnModelSelect[0].SetTextColor(COLOR_BLACK);
// 	m_btnModelSelect[0].SetFaceColor(RGB(225, 225, 225), true);
// 	m_nSampleModelType = 2;
// }

void CDlgMotorTeach::ChangeManualBtnUI()
{
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_00_LOADING1_SOCKET_CLAMP_ON_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][2].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][2].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][2].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][2].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][2].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][2].SetFaceColor(RGB(225, 225, 225), true);
// 	}

// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_02_LOADING1_PROBEPIN_MOVE_ON_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][3].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][3].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][3].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][3].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][3].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][3].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_04_LOADING1_PROBEPIN_CONNECT_UP_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][4].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][4].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][4].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][4].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][4].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][4].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_06_LOADING1_SOCKET_UP_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][5].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][5].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][5].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][5].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][5].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][5].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// #if(SET_INSPECTOR == SYS_DISTORTION) \
//  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
//  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_08_LOADING1_VACCUM_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][6].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][6].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][6].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][6].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][6].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][6].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// #endif

	////////////////////////Right
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_0A_LOADING2_SOCKET_CLAMP_ON_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][7].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][7].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][7].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][7].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][7].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][7].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_0C_LOADING2_PROBEPIN_MOVE_ON_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][8].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][8].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][8].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][8].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][8].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][8].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_0E_LOADING2_PROBEPIN_CONNECT_UP_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][9].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][9].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][9].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][9].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][9].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][9].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_10_LOADING2_SOCKET_UP_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][10].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][10].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][10].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][10].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][10].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][10].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// #if(SET_INSPECTOR == SYS_DISTORTION) \
//  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
//  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
//  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X1_12_LOADING2_VACCUM_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][11].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][11].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][11].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][11].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][11].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][11].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// #endif
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1C_BUFFER_LIFT_CYLINDER_UP_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][0].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][0].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][0].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][0].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][0].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][0].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	if (AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1E_BUFFER_SOCKET_CLAMP_ON_SENSOR) == TRUE)
// 	{
// 		m_btnMotorManual[0][1].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[1][1].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[1][1].SetFaceColor(RGB(225, 225, 225), true);
// 	}
// 	else
// 	{
// 		m_btnMotorManual[1][1].SetFaceColor(COLOR_GREEN1, true);
// 		m_btnMotorManual[0][1].SetTextColor(COLOR_BLACK);
// 		m_btnMotorManual[0][1].SetFaceColor(RGB(225, 225, 225), true);
// 	}
}

BOOL CDlgMotorTeach::SetInsertValue(int nMode, int nAxisSpeed)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

// 	if (m_nSetSpeed_AxisNum == 6)
// 	{
		if (nMode == 0)
		{
			if (nAxisSpeed > 100000 || nAxisSpeed < 1)
			{
				if (stEq.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("저속 중속 고속 속도 입력 범위는 1~100000입니다. 다시입력해주세요"), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("The low-speed medium-speed high-speed input range is 1 to 400. Please input it again."), MODE_INFORMATION);

			}
		}
		if (nMode == 1)
		{
			if (nAxisSpeed > 200000 || nAxisSpeed < 1)
			{
				if (stEq.nLanguage == 0)
					COMMON->ShowMsgDlg_Domodal(_T("가속 감속 속도 입력 범위는 1~200000입니다. 다시입력해주세요"), MODE_INFORMATION);
				else
					COMMON->ShowMsgDlg_Domodal(_T("The acceleration deceleration speed input range is 1 to 1000. Please input it again."), MODE_INFORMATION);

			}
		}

// 	}
// 	else
// 	{
// 		if (nMode == 0)
// 		{
// 			if (nAxisSpeed > 400 || nAxisSpeed < 1)
// 			{
// 				if (stEq.nLanguage == 0)
// 					COMMON->ShowMsgDlg_Domodal(_T("저속 중속 고속 속도 입력 범위는 1~400입니다. 다시입력해주세요"), MODE_INFORMATION);
// 				else
// 					COMMON->ShowMsgDlg_Domodal(_T("The low-speed medium-speed high-speed input range is 1 to 400. Please input it again."), MODE_INFORMATION);
// 
// 			}
// 		}
// 		if (nMode == 1)
// 		{
// 			if (nAxisSpeed > 1000 || nAxisSpeed < 1)
// 			{
// 				if (stEq.nLanguage == 0)
// 					COMMON->ShowMsgDlg_Domodal(_T("가속 감속 속도 입력 범위는 1~1000입니다. 다시입력해주세요"), MODE_INFORMATION);
// 				else
// 					COMMON->ShowMsgDlg_Domodal(_T("The acceleration deceleration speed input range is 1 to 1000. Please input it again."), MODE_INFORMATION);
// 
// 			}
// 		}
// 	}

	return TRUE;
}


void CDlgMotorTeach::OnBnClickedButton2()
{
	CDevice device;
	
	device.POWERSUPPLY_VoltOn(InspectionPortType::Left);
	device.POWERSUPPLY_VoltOn(InspectionPortType::Right);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
}

void CDlgMotorTeach::OnBnClickedButton3()
{
	CDevice device;
	//device.POWERSUPPLY_VoltOff(InspectionPortType::Left);

	device.POWERSUPPLY_VoltOff(InspectionPortType::Left);
	device.POWERSUPPLY_VoltOff(InspectionPortType::Right);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

void CDlgMotorTeach::OnBnClickedButton4()
{
	CDevice device;
	device.POWERSUPPLY_VoltOn(InspectionPortType::Right);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
}

void CDlgMotorTeach::OnBnClickedButton5()
{
	CDevice device;
	device.POWERSUPPLY_VoltOff(InspectionPortType::Right);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}

void CDlgMotorTeach::OnBnClickedButton6()
{
	float volt, current;
	m_ctrlLight_ODA_Q[0].Write_Channel();
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Volt(volt);
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Current(current);

	CString strMsg;
	strMsg.Format(_T("Power Supply Left Measure Volt: %f, Current: %f"), volt, current);
	COMMON->Write_Main_Log(strMsg);
	COMMON->ShowMsgDlg_Domodal(strMsg, MODE_INFORMATION);
}

void CDlgMotorTeach::OnBnClickedButton7()
{
	float volt, current;
	m_ctrlLight_ODA_Q[0].Write_Channel_2();
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Volt(volt);
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Current(current);

	CString strMsg;
	strMsg.Format(_T("Power Supply Right Measure Volt: %f, Current: %f"), volt, current);
	COMMON->Write_Main_Log(strMsg);
	COMMON->ShowMsgDlg_Domodal(strMsg, MODE_INFORMATION);
}

void CDlgMotorTeach::OnBnClickedBtnTeachViewManual()
{
	//m_pDlgManualMove->ShowWindow(SW_SHOW);
	//ShowWindow(SW_HIDE);
}

void CDlgMotorTeach::OnStnClickedStcScrew_MoveControl()
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) {
		AfxMessageBox(_T("Origin Check please!"), MB_OK);
		return;
	}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("L Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) == FALSE) {
		AfxMessageBox(_T("R Check Jig Out Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("L Check Jig seating Sensnor"), MB_OK);
		return;
	}
	if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) == TRUE) {
		AfxMessageBox(_T("R Check Jig seating Sensnor"), MB_OK);
		return;
	}
#endif

	double dPos = 0.0;
	double dSpeed = 0.0;
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	CString strPos = 0;
	auto bok = 0;
	BOOL bChk = FALSE;
	UINT nID = GetFocus()->GetDlgCtrlID();
		if (nID == IDC_BTN_SCREW_MOVE_01)
		{
			if (MO_MoveScrew_Type(Screw_Wait))
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 실패하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X failed to move. \n"));
			}
			else
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 성공하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X successfully to move. \n"));
			}
		}
		else if (nID == IDC_BTN_SCREW_MOVE_02)
		{

			if (MO_MoveScrew_Type(Screw_Commencement))
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 실패하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X failed to move. \n"));
			}
			else
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 성공하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X successfully to move. \n"));
			}

		}
		else
		{
			if (MO_MoveScrew_Type(Screw_Work))
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 실패하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X failed to move. \n"));
			}
			else
			{
				if (stEq.nLanguage == 0)
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X 이동에 성공하였습니다 \n"));
				else
					WRITE_MAIN_LOG(_T("[TEACHING] SCREW X successfully to move. \n"));
			}
		}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_Unlock() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 풀림!."));
		else
			strTemp.Format(_T("Screw Unlock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안풀림 에러 발생!."));
		else
			strTemp.Format(_T("Screw Unlock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_Lock_1() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_Lock_2() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}

void CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock_L()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_L_Unlock() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 풀림!."));
		else
			strTemp.Format(_T("Screw Unlock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안풀림 에러 발생!."));
		else
			strTemp.Format(_T("Screw Unlock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1_L()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_L_Lock_1() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2_L()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_L_Lock_2() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}

void CDlgMotorTeach::OnStnClickedStcScrew_Control_Unlock_R()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_R_Unlock() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 풀림!."));
		else
			strTemp.Format(_T("Screw Unlock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안풀림 에러 발생!."));
		else
			strTemp.Format(_T("Screw Unlock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock1_R()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_R_Lock_1() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}
void CDlgMotorTeach::OnStnClickedStcScrew_Control_Lock2_R()
{
	CString strTemp; strTemp.Empty();
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();
	if (IO_O_Screw_Bolt_R_Lock_2() == TRUE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 정상적으로 잠김!."));
		else
			strTemp.Format(_T("Screw lock Success!."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("스크류 안잠김 에러 발생!."));
		else
			strTemp.Format(_T("Screw lock Failed!."));
		COMMON->Write_Main_Log(strTemp);
	}
}



void CDlgMotorTeach::OnBnClickedButton8() // No barcode 
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//stTeach.nBarcode = m_Barcode;
	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	m_Barcode = 0;
	stTeach.nBarcode = m_Barcode;
	DATAMANAGER->SetTeachingData(stTeach);
}


void CDlgMotorTeach::OnBnClickedButton9() // Set Barcode
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
	m_Barcode = 1;
	stTeach.nBarcode = m_Barcode;
	DATAMANAGER->SetTeachingData(stTeach);
}
