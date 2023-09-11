#pragma once
#include "Singleton.h"
#include "Def_CompileOption.h"

#include "IcsCommunicator.h"

#define _SIMUL_MODE_				1
#define _DRY_RUN_SOCKET_NONE		1
#define _DRY_RUN_MODULE_NONE		1
#define _DRY_RUN_INTERFACE_NONE		1

#define _MOTION_CW					1
#define _MOTION_CCW					0

#define _SENSOR_ON					1
#define _SENSOR_OFF					0

#define _CYLINDER_UP				1
#define _CYLINDER_DOWN				0
#define _CYLINDER_ON				1
#define _CYLINDER_OFF				0
#define _VACUUM_ON					1
#define _VACUUM_OFF					0

#define _SLOW1_ON					0
#define _STOP1_ON					1
#define _SLOW2_ON					2
#define _STOP2_ON					3

#define _PROGRAM_ROOT_DIR			_T("C:\\Exec_Trinity\\Luritech_Motion")
#define _PROGRAM_ERRORLIST_DIR		_T("C:\\Exec_Trinity\\Luritech_Motion\\ErrorList")
#define _MCH_LOG_SYSTEM_FOLDER		_T("C:\\Exec_Trinity\\Luritech_Motion\\Log")
#define _MCH_LOG_COMM_FOLDER		_T("C:\\Exec_Trinity\\Luritech_Motion\\Comm_Log")
#define _MCH_LOG_SEQ_FOLDER			_T("C:\\Exec_Trinity\\Luritech_Motion\\Seq_Log")
#define _MCH_LOG_BARCODE_FOLDER		_T("C:\\Exec_Trinity\\Luritech_Motion\\Barcode_Log")
#define _MCH_LOG_SEMCO_FOLDER		_T("D:\\Log")
#define _PROGRAM_RECIPE_DIR			_T("C:\\Exec_Trinity\\Recipe")
#define _PROGRAM_IMAGE_FOLDER		_T("IMAGE")

//====================================================================
#define _SYSTEM_LOG		_T("[SYSTEM]")
#define _ERROR_LOG		_T("[ERROR]")
#define _MOTION_LOG		_T("[MOTION]")
#define _THREAD_LOG		_T("[THREAD]")
#define _COMM_LOG		_T("[COMM]")
#define _SEQ_LOG		_T("[SEQ]")
#define _BARCODE_LOG	_T("[BARCODE]")
//====================================================================

#define LANGAUGE_MAX_COUNT				2		// 영어, 한국어

// 장비별 Setting 필요부분 JYS.
//====================================================================
// MOTOR DEFINE
#define VIGNETTING_X					0
#define VIGNETTING_R					1
#define SCREW_L_X						2
#define SCREW_R_X						3
#define TILT_ADJUST_R						4


// #define CONVEYOR_1						0
// #define CONVEYOR_2						1
// #define CONVEYOR_3						2
// #define PICKER_X						3
// #define PICKER_Z1						4
// #define PICKER_Z2						5

// JYS 211230. TEST CODE
// #define AJIN_GANTRY_MASTER_Z			0
// #define AJIN_GANTRY_SLAVE_Z				1

//#define AJIN_MOTOR_MAX_COUNT			5

#define MAIN_FLOW_TRAY_LEFT				0
#define MAIN_FLOW_TRAY_RIGHT			1
#define MAIN_FLOW_CV_SENSOR				2
#define MAIN_FLOW_SOCKET_POS			3
#define MAIN_FLOW_DETECT_POS			4
#define MAIN_FLOW_CYLINDER_STATE		5
#define MAIN_FLOW_MATERIAL_POS			6

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	#define MOTOR_MAX_COUNT				5
	#define AJIN_MOTOR_MAX_COUNT			5

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	#define MOTOR_MAX_COUNT				2
	#define AJIN_MOTOR_MAX_COUNT			2
#else
	#define MOTOR_MAX_COUNT				1
	#define AJIN_MOTOR_MAX_COUNT			1
#endif

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)			// COLOR CAL 
#define INPUT_IO_MAX_COUNT				96
#define OUTPUT_IO_MAX_COUNT				96
#elif(SET_INSPECTOR == SYS_TLA_OQC)	// COLOR CAL 
#define INPUT_IO_MAX_COUNT				64
#define OUTPUT_IO_MAX_COUNT				64
#else
#define INPUT_IO_MAX_COUNT				32
#define OUTPUT_IO_MAX_COUNT				32
#endif

// #define OUTPUT_IO_MAX_COUNT				64

#define INPUT_IO_READ_COUNT				32		// 한번에 읽어올 i/O 수
#define OUTPUT_IO_READ_COUNT			32		// 한번에 읽어올 i/O 수


// PC에 아진보드 연결시 INPUT IO 먼저 인식이 되게 진행하면 EZSOFT 에서 OUTPUT 보드가 인식되는 번호가 몇번쨰 인지 확인후 작성
// 아웃풋을 먼저 설치하게 디되면 INPUT I/O번호가 밀리게 된다 따라서 한가지 방법으로 PC에 연결하는걸 추천
// 무조건 I/O 보드 연결은 INPUT 모두 연결후에 OUTPUT 연결하도록 하자.
#define AJIN_OUTPUT_IO_START_NUMBER		3		


#define SOCKET_USE
#define LAN_COMMUNICATION_COUNT			5


//====================================================================
// 조명 컨트롤러 연결 UI MAX 2개 ( DlgSystem.h )
// 위의 두개를 동시에 사용은 안된다.
//====================================================================
//#define LIGHT_CONTROLLER_LURITECH_BOARD_USE	
#define LIGHT_CONTROLLER_FN1500_BOARD_USE
//#define LIGHT_CONTROLLER_PT_BOARD_USE
//#define LIGHT_CONTROLLER_L100_USE
#define LIGHT_CONTROLLER_TYPE_COUNT		5

//====================================================================
// 조명컨트롤러 고정 사항.
// 조명 컨트롤러 MAX Voltage, MIN Voltage
#define LIGHT_CONTROLLER_MAX_VOLTAGE	15
#define LIGHT_CONTROLLER_MIN_VOLTAGE	0

// 조명 컨트롤러 MAX Current, MIN Current
#define LIGHT_CONTROLLER_MAX_CURRENT	3000
#define LIGHT_CONTROLLER_MIN_CURRENT	0

// Msgbox 사용 Type
#define MODE_QUESTION					0
#define MODE_ERROR						1
#define MODE_INFORMATION				2
#define MODE_NO_CLOSE					3
#define MODE_PASS						4
#define MODE_FAIL					5

// 나중에 지우자
#define LIGHT_CONTROLLER_LURITECH_BOARD_COUNT		4
#define LIGHT_CONTROLLER_PT_BOARD_COUNT				4
#define LIGHT_CONTROLLER_IPL_L100_COUNT				4
#define LIGHT_CONTROLLER_FN1000W_13CH_COUNT			13
#define LIGHT_CONTROLLER_SOL3_BOARD_COUNT			9

#pragma comment(lib, "PCOMM.lib")

// Serial
#include "LightController_Luri.h"
#include "LightController_ODA_PT.h"
#include "LightController_IPL_L100.h"
#include "LightController_PD_FN1500W_13CH.h"
#include "LightController_YouthTech.h"
#include "LightController_YouthTech_DPS.h"
#include "LightController_SOL_3.h"
#include "LightController_ODA_Q.h"

// Ajinboard 
#pragma comment(lib, "AXL.lib")
#include "AXA.h"
#include "AXC.h"
#include "AXD.h"
#include "AXDev.h"
#include "AXHD.h"
#include "AXHS.h"
#include "AXL.h"
#include "AXM.h"
#define AXL_DEFAULT_IRQNO				7

#pragma comment(lib, "Lib/SYSTEMK.lib")
#include "Lib/SYSTEMK.h"

#include "DataManager.h"
#define DATAMANAGER		CDataManager::Instance()

#include "Common.h"
#define COMMON			CCommon::Instance()

#include "Sequence.h"
#define SEQUENCE		CSequence::Instance()

#include "AjinBoard.h"
#define AJIN_MO			CAjinMotion::Instance()

#include "Ajin_IO.h"
#define AJIN_IO			CAjinDio::Instance()

// #include "Fastech_PR_Motion.h"
// #define FAS_MO			CFastech_Motion::Instance()

#define ICS_COMM		CIcsCommunicator::Instance()

// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// #include "CL500ACommunicator.h"
// #define CL500A_COMM		CCL500ACommunicator::Instance()
// #endif

// #if(SET_INSPECTOR == SYS_HOTPIXEL)
// #include "CL200A.h"
// #define CL200A_COMM		CCL200A::Instance()
// #endif

#define WRITE_SYSTEM_FILE		GetLogSystem().LogOut
#define WRITE_COM_FILE			GetLogComm().LogOut
#define WRITE_BARCODE_FILE		GetLogBarcode().LogOut
#define WRITE_SEQ_FILE			GetLogSeq().LogOut
#define WRITE_SEMCO_FILE		GetLogSemco().CsvOut

#define WRITE_MAIN_LOG			CDlgLog::Get_Instance()->Write_Main_Log		// UI Log 사용하려는 파일에 DlgLog.h 를 include  해주어야 한다.
#define WRITE_ERR_LOG			CDlgLog::Get_Instance()->Write_Err_Log		// UI Log 사용하려는 파일에 DlgLog.h 를 include  해주어야 한다.
#define WRITE_SEQ_LOG			CDlgLog::Get_Instance()->Write_Seq_Log		// UI Log 사용하려는 파일에 DlgLog.h 를 include  해주어야 한다.
#define WRITE_COMM_LOG			CDlgLog::Get_Instance()->Write_Comm_Log		// UI Log 사용하려는 파일에 DlgLog.h 를 include  해주어야 한다.
#define WRITE_ICSCOMM_LOG		CDlgLog::Get_Instance()->Write_IcsComm_Log	// UI Log 사용하려는 파일에 DlgLog.h 를 include  해주어야 한다.
#define WRITE_SEMCO_LOG			CDlgLog::Get_Instance()->Write_Semco_Log
#define SHOW_ERROR				CDlgMain::Get_Instance()->ShowError
#define SHOW_INFORMATION		CDlgMain::Get_Instance()->ShowInformation

#define TIMEOUT_DELAY_100		(int)100
#define TIMEOUT_DELAY_200		(int)200
#define TIMEOUT_DELAY_500		(int)500
#define TIMEOUT_DELAY_1000		(int)1000
#define TIMEOUT_DELAY_2000		(int)2000
#define TIMEOUT_DELAY_3000		(int)3000
#define TIMEOUT_DELAY_5000		(int)5000
#define TIMEOUT_DELAY_10000		(int)10000
#define TIMEOUT_DELAY_30000		(int)30000
#define TIMEOUT_DELAY_45000		(int)45000

#define USE_LIGHT_TO_OUTPUT

#pragma warning(disable : 4995)
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
#pragma warning(disable : 4005)
