﻿#pragma once

#include "Def_Structure.h"
#include "Def_Enum.h"
#include "Def_ErrorCode.h"

// conveyor1 동작 시퀀스
typedef enum
{
	SEQ_CV1_STEP_READY = 0,
	SEQ_CV1_STEP_CHECK_STATUS,
	SEQ_CV1_STEP_WAIT_SOCKET,								// 작업대기
	SEQ_CV1_STEP_MOVE_TO_BUFFER_START,						// 소켓 버퍼안착부로 이동 시작
	SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_SLOW_SENSOR,			// 소켓 버퍼 slow 센서 위치로 이동중
	SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_STOP_SENSOR,			// 소켓 버퍼 stop 센서 위치로 이동중 
	SEQ_CV1_STEP_MOVE_TO_BUFFER_CYLINDER_UP,				// cylinder up
	SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_ON,					// grip on
	SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_OFF,					// grip off
	SEQ_CV1_STEP_MOVE_TO_BUFFER_END,
	SEQ_CV1_STEP_MOVE_TO_RFID_START,
	SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_INPUT_SENSOR,			// 소켓 투입 이동중
	SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR,		// 소켓 rfid stop 센서까지 이동중
	SEQ_CV1_STEP_MOVE_TO_RFID_SCAN_SOCKET,					// 소켓 rfid stop 위치에 멈춤 & rfid 스캔 
	SEQ_CV1_STEP_MOVE_TO_RFID_END,							// 소켓 rfid 스캔 완료
	SEQ_CV1_STEP_MOVE_TO_CONV2_START,						// 소켓 cv1 -> cv2 전달 시작
	SEQ_CV1_STEP_MOVE_TO_CONV2_RUN,							// 소켓 cv1 -> cv2 전달중 & 전달 확인
	SEQ_CV1_STEP_MOVE_TO_CONV2_END,							// 소켓 cv1 -> cv2 전달 완료
	SEQ_CV1_STEP_BUFFER_DOWN_START,
	SEQ_CV1_STEP_BUFFER_DOWN_CYLINDER_DN,
	SEQ_CV1_STEP_BUFFER_DOWN_GRIP_OFF,
	SEQ_CV1_STEP_BUFFER_DOWN_CHECK_STOP_SENSOR,
	SEQ_CV1_STEP_BUFFER_DOWN_END,
	SEQ_CV1_STEP_COMPLETE
}enSeqCv1Step;

static LPCTSTR g_szSeqName_CV1[] = 
{
	_T("SEQ_CV1_STEP_READY"), 
	_T("SEQ_CV1_STEP_CHECK_STATUS"),
	_T("SEQ_CV1_STEP_WAIT_SOCKET"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_START"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_SLOW_SENSOR"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_STOP_SENSOR"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_CYLINDER_UP"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_ON"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_OFF"),
	_T("SEQ_CV1_STEP_MOVE_TO_BUFFER_END"),
	_T("SEQ_CV1_STEP_MOVE_TO_RFID_START"),
	_T("SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_INPUT_SENSOR"),
	_T("SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR"),
	_T("SEQ_CV1_STEP_MOVE_TO_RFID_SCAN_SOCKET"),
	_T("SEQ_CV1_STEP_MOVE_TO_RFID_END"),
	_T("SEQ_CV1_STEP_MOVE_TO_CONV2_START"),
	_T("SEQ_CV1_STEP_MOVE_TO_CONV2_RUN"),
	_T("SEQ_CV1_STEP_MOVE_TO_CONV2_END"),
	_T("SEQ_CV1_STEP_BUFFER_DOWN_START"),
	_T("SEQ_CV1_STEP_BUFFER_DOWN_CYLINDER_DN"),
	_T("SEQ_CV1_STEP_BUFFER_DOWN_GRIP_OFF"),
	_T("SEQ_CV1_STEP_BUFFER_DOWN_CHECK_STOP_SENSOR"),
	_T("SEQ_CV1_STEP_BUFFER_DOWN_END"),
	_T("SEQ_CV1_STEP_COMPLETE"),
};

// conveyor2 동작 시퀀스
typedef enum
{
	SEQ_CV2_STEP_READY = 0,
	SEQ_CV2_STEP_CHECK_STATUS,
	SEQ_CV2_STEP_WAIT_SOCKET,										// 작업 대기
	SEQ_CV2_STEP_RESERVED_FROM_PICKER,								// picker 작업 예약
	SEQ_CV2_STEP_RESERVED_FROM_CONVEYOR1,							// conveyor1 작업 예약
	SEQ_CV2_STEP_MOVE_TO_LOADING_START,								// 소켓 로딩(cv1 에서 소켓 투입 시작)
	SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_FROM_IN,				// 소켓 로딩(input 센서 감지 확인)
	SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_SLOW_SENSOR,		// 소켓 로딩(slow 센서 감지 확인)
	SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_STOP_SENSOR,		// 소켓 로딩(stop 센서 감지 확인)
	SEQ_CV2_STEP_MOVE_TO_LOADING_CHECK_SOCKET,						// 소켓 로딩(소켓 정지 확인)
	SEQ_CV2_STEP_MOVE_TO_LOADING_END,								// 소켓 로딩완료
	SEQ_CV2_STEP_MOVE_TO_UNLOADING_START,							// 소켓 배출시작
	SEQ_CV2_STEP_MOVE_TO_UNLOADING_CHECK_SOCKET,					// 소켓 배출
	SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN,								// 소켓 배출(cv2 -> 뒷설비)
	SEQ_CV2_STEP_MOVE_TO_UNLOADING_END,
	SEQ_CV2_STEP_MOVE_BYPASS_START,
	SEQ_CV2_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_OUT_SENSOR,				// 소켓 BYPASS out 센서 감지 확인
	SEQ_CV2_STEP_MOVE_BYPASS_RUN,									// 소켓 BYPASS(cv2 -> 뒷설비)
	SEQ_CV2_STEP_MOVE_BYPASS_END,
	SEQ_CV2_STEP_COMPLETE
}enSeqCv2Step;

static LPCTSTR g_szSeqName_CV2[] =
{
	_T("SEQ_CV2_STEP_READY"),
	_T("SEQ_CV2_STEP_CHECK_STATUS"),
	_T("SEQ_CV2_STEP_WAIT_SOCKET"),
	_T("SEQ_CV2_STEP_RESERVED_FROM_PICKER"),
	_T("SEQ_CV2_STEP_RESERVED_FROM_CONVEYOR1"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_START"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_FROM_IN"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_SLOW_SENSOR"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_STOP_SENSOR"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_CHECK_SOCKET"),
	_T("SEQ_CV2_STEP_MOVE_TO_LOADING_END"),
	_T("SEQ_CV2_STEP_MOVE_TO_UNLOADING_START"),
	_T("SEQ_CV2_STEP_MOVE_TO_UNLOADING_CHECK_SOCKET"),
	_T("SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN"),
	_T("SEQ_CV2_STEP_MOVE_TO_UNLOADING_END"),
	_T("SEQ_CV2_STEP_MOVE_BYPASS_START"),
	_T("SEQ_CV2_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_OUT_SENSOR"),
	_T("SEQ_CV2_STEP_MOVE_BYPASS_RUN"),
	_T("SEQ_CV2_STEP_MOVE_BYPASS_END"),
	_T("SEQ_CV2_STEP_COMPLETE"),
};

// conveyor 리턴 동작 시퀀스
typedef enum
{
	SEQ_CVR_STEP_READY = 0,
	SEQ_CVR_STEP_CHECK_STATUS,
	SEQ_CVR_STEP_WAIT_SOCKET,
	SEQ_CVR_STEP_MOVE_BYPASS_START,
	SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN,
	SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR,
	SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR,
	SEQ_CVR_STEP_MOVE_BYPASS_RUN,
	SEQ_CVR_STEP_MOVE_BYPASS_END,
	SEQ_CVR_STEP_COMPLETE
}enSeqCvrStep;

static LPCTSTR g_szSeqName_CVR[] =
{
	_T("SEQ_CVR_STEP_READY"),
	_T("SEQ_CVR_STEP_CHECK_STATUS"),
	_T("SEQ_CVR_STEP_WAIT_SOCKET"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_START"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_RUN"),
	_T("SEQ_CVR_STEP_MOVE_BYPASS_END"),
	_T("SEQ_CVR_STEP_COMPLETE"),
};

// picker 1,2, 동작 시퀀스
typedef enum
{
	SEQ_PICKER_STEP_READY = 0,
	SEQ_PICKER_STEP_INIT_START,
	SEQ_PICKER_STEP_INIT_CHECK_SOCKET,
	SEQ_PICKER_STEP_INIT_TRAY_UP,				// TRAY UP
	SEQ_PICKER_STEP_INIT_PROBE_DN,				// PROBE DN
	SEQ_PICKER_STEP_INIT_MOVE_Z_TO_SAFETY_POS,	// MOVE Z TO SAFETY POS
	SEQ_PICKER_STEP_INIT_END,
	SEQ_PICKER_STEP_STANDBY_START,			// 비어 있는 상태에서제품 받기 위해 이동 	
	SEQ_PICKER_STEP_STANDBY_CHECK_SOCKET_ON_CONVEYOR,
	SEQ_PICKER_STEP_STANDBY_MOVE_Z_TO_WAIT_SOCKET,
	SEQ_PICKER_STEP_STANDBY_TRAY_CYLINDER_DN,
	SEQ_PICKER_STEP_STANDBY_PROBE_DN,
	SEQ_PICKER_STEP_STANDBY_END,
	SEQ_PICKER_STEP_CHECK_STATUS,
	SEQ_PICKER_STEP_WAIT_SOCKET,
	SEQ_PICKER_STEP_LOAD_START,				//소켓을 잡고 올린다.
	SEQ_PICKER_STEP_LOAD_MOVE_Z_GRIP_UP,
	SEQ_PICKER_STEP_LOAD_CHECK_SOCKET,
	SEQ_PICKER_STEP_LOAD_CHECK_TILT,
	SEQ_PICKER_STEP_LOAD_REVERSE_CHECK,
	SEQ_PICKER_STEP_LOAD_GRIP_OFF,
	SEQ_PICKER_STEP_LOAD_GRIP_ON,
	SEQ_PICKER_STEP_LOAD_GRIP_OFF_RE,
	SEQ_PICKER_STEP_LOAD_TRAY_UP,
	SEQ_PICKER_STEP_LOAD_CHANGE_PROBE_H_V_TYPE,
	SEQ_PICKER_STEP_LOAD_PROBE_UP,
	SEQ_PICKER_STEP_LOAD_VACCUMON,
	SEQ_PICKER_STEP_LOAD_CHECK_VACCUM_ON,
	SEQ_PICKER_STEP_LOAD_MOVE_TO_SAFETY_POS,
	SEQ_PICKER_STEP_LOAD_END,
	SEQ_PICKER_STEP_INIT_INSPECTION_START,
	SEQ_PICKER_STEP_INIT_INSPECTION_VOLT_ON,
	SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_INITIALIZE,
	SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_BARCODE,
	SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_PREVIEWSTART,
	SEQ_PICKER_STEP_INIT_INSPECTION_END,
	SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_START,
	SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS,
	SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_END,
	SEQ_PICKER_STEP_INSPECTION_START,		// 검사를 시작한다.
	SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_INSPECTION_POS,
	SEQ_PICKER_STEP_INSPECTION_CL200A_MEASURE,
	SEQ_PICKER_STEP_INSPECTION_TEST,
	SEQ_PICKER_STEP_INSPECTION_SEMCO_PREVIEW_STOP,
	SEQ_PICKER_STEP_INSPECTION_POWER_OFF,
	SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_SAFETY_POS,
	SEQ_PICKER_STEP_INSPECTION_END,
	SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_START,
	SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS,
	SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_END,
	SEQ_PICKER_STEP_UNLOAD_START,			// 소켓을 내려 놓는다.
	SEQ_PICKER_STEP_UNLOAD_VACCUM_OFF,
	SEQ_PICKER_STEP_UNLOAD_GRIP_OFF,
	SEQ_PICKER_STEP_UNLOAD_MOVE_Z_TO_WAIT_SOCKET,
	SEQ_PICKER_STEP_UNLOAD_TRAY_CYLINDER_DN,
	SEQ_PICKER_STEP_UNLOAD_PROBE_DN,
	SEQ_PICKER_STEP_UNLOAD_END,
	SEQ_PICKER_STEP_UNLOAD_REQEUST,
	SEQ_PICKER_STEP_FINALIZE_START,
	SEQ_PICKER_STEP_FINALIZE_SEMCO_TEST,
	SEQ_PICKER_STEP_FINALIZE_SEMCO_EEPROM_WRITE,
	SEQ_PICKER_STEP_FINALIZE_SEMCO_PREVIEW_STOP,
	SEQ_PICKER_STEP_FINALIZE_POWER_OFF,
	SEQ_PICKER_STEP_FINALIZE_SEND_TESTRESULT_TO_ICS,
	SEQ_PICKER_STEP_FINALIZE_END,
	SEQ_PICKER_STEP_COMPLETE
}enSeqPickerStep;

static LPCTSTR g_szSeqName_Picker[] =
{
	_T("SEQ_PICKER_STEP_READY"),	
	_T("SEQ_PICKER_STEP_INIT_START"),
	_T("SEQ_PICKER_STEP_INIT_CHECK_SOCKET"),
	_T("SEQ_PICKER_STEP_INIT_TRAY_UP"),					// TRAY UP
	_T("SEQ_PICKER_STEP_INIT_PROBE_DN"),				// PROBE DN
	_T("SEQ_PICKER_STEP_INIT_MOVE_Z_TO_SAFETY_POS"),	// MOVE Z TO SAFETY POS
	_T("SEQ_PICKER_STEP_INIT_END"),
	_T("SEQ_PICKER_STEP_STANDBY_START"),
	_T("SEQ_PICKER_STEP_STANDBY_CHECK_SOCKET_ON_CONVEYOR"),
	_T("SEQ_PICKER_STEP_STANDBY_MOVE_Z_TO_WAIT_SOCKET"),
	_T("SEQ_PICKER_STEP_STANDBY_TRAY_CYLINDER_DN"),
	_T("SEQ_PICKER_STEP_STANDBY_PROBE_DN"),
	_T("SEQ_PICKER_STEP_STANDBY_END"),
	_T("SEQ_PICKER_STEP_CHECK_STATUS"),
	_T("SEQ_PICKER_STEP_WAIT_SOCKET"),
	_T("SEQ_PICKER_STEP_LOAD_START"),
	_T("SEQ_PICKER_STEP_LOAD_MOVE_Z_GRIP_UP"),
	_T("SEQ_PICKER_STEP_LOAD_CHECK_SOCKET"),
	_T("SEQ_PICKER_STEP_LOAD_CHECK_TILT"),
	_T("SEQ_PICKER_STEP_LOAD_REVERSE_CHECK"),
	_T("SEQ_PICKER_STEP_LOAD_GRIP_OFF"),
	_T("SEQ_PICKER_STEP_LOAD_GRIP_ON"),
	_T("SEQ_PICKER_STEP_LOAD_GRIP_OFF_RE"),
	_T("SEQ_PICKER_STEP_LOAD_TRAY_UP"),
	_T("SEQ_PICKER_STEP_LOAD_CHANGE_PROBE_H_V_TYPE"),
	_T("SEQ_PICKER_STEP_LOAD_PROBE_UP"),
	_T("SEQ_PICKER_STEP_LOAD_VACCUMON"),
	_T("SEQ_PICKER_STEP_LOAD_CHECK_VACCUM_ON"),
	_T("SEQ_PICKER_STEP_LOAD_MOVE_TO_SAFETY_POS"),
	_T("SEQ_PICKER_STEP_LOAD_END"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_START"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_VOLT_ON"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_INITIALIZE"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_BARCODE"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_PREVIEWSTART"),
	_T("SEQ_PICKER_STEP_INIT_INSPECTION_END"),	
	_T("SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_START"),
	_T("SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS"),
	_T("SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_END"),
	_T("SEQ_PICKER_STEP_INSPECTION_START"),
	_T("SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_INSPECTION_POS"),
	_T("SEQ_PICKER_STEP_INSPECTION_CL200A_MEASURE"),
	_T("SEQ_PICKER_STEP_INSPECTION_TEST"),
	_T("SEQ_PICKER_STEP_INSPECTION_SEMCO_PREVIEW_STOP"),
	_T("SEQ_PICKER_STEP_INSPECTION_POWER_OFF"),
	_T("SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_SAFETY_POS"),
	_T("SEQ_PICKER_STEP_INSPECTION_END"),
	_T("SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_START"),
	_T("SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS"),
	_T("SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_END"),
	_T("SEQ_PICKER_STEP_UNLOAD_START"),
	_T("SEQ_PICKER_STEP_UNLOAD_VACCUM_OFF"),
	_T("SEQ_PICKER_STEP_UNLOAD_GRIP_OFF"),
	_T("SEQ_PICKER_STEP_UNLOAD_MOVE_Z_TO_WAIT_SOCKET"),
	_T("SEQ_PICKER_STEP_UNLOAD_TRAY_CYLINDER_DN"),
	_T("SEQ_PICKER_STEP_UNLOAD_PROBE_DN"),
	_T("SEQ_PICKER_STEP_UNLOAD_END"),
	_T("SEQ_PICKER_UNLOAD_REQUEST"),
	_T("SEQ_PICKER_STEP_FINALIZE_START"),
	_T("SEQ_PICKER_STEP_FINALIZE_SEMCO_TEST"),
	_T("SEQ_PICKER_STEP_FINALIZE_SEMCO_EEPROM_WRITE"),
	_T("SEQ_PICKER_STEP_FINALIZE_PREVIEW_STOP"),
	_T("SEQ_PICKER_STEP_FINALIZE_POWER_OFF"),
	_T("SEQ_PICKER_STEP_FINALIZE_SEND_TESTRESULT_TO_ICS"),
	_T("SEQ_PICKER_STEP_FINALIZE_END"),
	_T("SEQ_PICKER_STEP_COMPLETE"),
};

typedef enum
{
	SEQ_PICKERMGR_STEP_IDEL = 0,
	SEQ_PICKERMGR_STEP_STOP,
	SEQ_PICKERMGR_STEP_MAIN,
	SEQ_PICKERMGR_STEP_READY_START,
	SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF,
	SEQ_PICKERMGR_STEP_READY_POWER_OFF, 
	SEQ_PICKERMGR_STEP_READY_UNLOCK1,
	SEQ_PICKERMGR_STEP_READY_UNLOCK2,
	SEQ_PICKERMGR_STEP_READY_END,


	SEQ_PICKERMGR_STEP_LOCK_START,
	SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1,
	SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2,
	SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP,
	SEQ_PICKERMGR_STEP_LOCK_PRODUCT, // Screw
	SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK, // Screw
	SEQ_PICKERMGR_STEP_LOCK_END, //
	SEQ_PICKERMGR_STEP_INSPECTION_POWER_START,
	SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START,
	SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT,
	SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END,
	SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START,
	SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_START,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST, // OC START 로 이동 반복 하여 조정이 완료 된후에 
	SEQ_PICKERMGR_STEP_INSPECTION_OC_END,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK,// 결과에 따라 SEQ_PICKERMGR_STEP_INSPECTION_SCREW_UNLOCK_START 로 이동 
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END,
	SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK,// 결과에 따라 SEQ_PICKERMGR_STEP_INSPECTION_SCREW_UNLOCK_START 로 이동 
	SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START,
	SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END,
	SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START,
	SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_MOVE,
	SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END,
	

}enSeqPickerMgrStep;

static LPCTSTR g_szSeqName_PickerMgr[] =
{
_T("SEQ_PICKERMGR_STEP_IDEL"),
	_T("SEQ_PICKERMGR_STEP_STOP"),
	_T("SEQ_PICKERMGR_STEP_MAIN"),
	_T("SEQ_PICKERMGR_STEP_READY_START"),
	_T("SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF"),
	_T("SEQ_PICKERMGR_STEP_READY_POWER_OFF"),
	_T("SEQ_PICKERMGR_STEP_READY_UNLOCK1"),
	_T("SEQ_PICKERMGR_STEP_READY_UNLOCK2"),
	_T("SEQ_PICKERMGR_STEP_READY_END"),
	_T("SEQ_PICKERMGR_STEP_LOCK_START"),
	_T("SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1"),
	_T("SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2"),
	_T("SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP"),
	_T("SEQ_PICKERMGR_STEP_LOCK_PRODUCT"),
	_T("SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK"),
	_T("SEQ_PICKERMGR_STEP_LOCK_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_POWER_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_MOVE"),
	_T("SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END"),
};

typedef enum
{
	SEQ_SEMIAUTO_STEP_IDEL = 0,
	SEQ_SEMIAUTO_STEP_READY,
	// (LOADING)
	SEQ_SEMIAUTO_STEP_LOADING_START,
	SEQ_SEMIAUTO_STEP_LOADING_CHK,
	SEQ_SEMIAUTO_STEP_LOADING_END,
	//(Lock)
	SEQ_SEMIAUTO_STEP_LOCK_START,
	SEQ_SEMIAUTO_STEP_LOCK_CHK,
	SEQ_SEMIAUTO_STEP_LOCK_MOVE,
	SEQ_SEMIAUTO_STEP_LOCK_END,
	//(Preview)
	SEQ_SEMIAUTO_STEP_PREVIEW_START,
	SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON,
	SEQ_SEMIAUTO_STEP_PREVIEW,
	SEQ_SEMIAUTO_STEP_PREVIEW_END,
	//(TLA)
	SEQ_SEMIAUTO_STEP_TLA_START,
	SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK,
	SEQ_SEMIAUTO_STEP_TLA_TEST,
	SEQ_SEMIAUTO_STEP_TLA_ADJUST,
	SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK,
	SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST,
	SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK,
	SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST,
	SEQ_SEMIAUTO_STEP_TLA_END,
	//(Vigneting)
	SEQ_SEMIAUTO_STEP_VIGNETING_START,
	SEQ_SEMIAUTO_STEP_VIGNETING_CHK,
//	SEQ_SEMIAUTO_STEP_VIGNETING_46_MOVE,
	SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST,
//	SEQ_SEMIAUTO_STEP_VIGNETING_82_MOVE,
//	SEQ_SEMIAUTO_STEP_VIGNETING_82_TEST,
	SEQ_SEMIAUTO_STEP_VIGNETING_END,
	// (UNLOADING)
	SEQ_SEMIAUTO_STEP_UNLOADING_START,
	SEQ_SEMIAUTO_STEP_UNLOADING_CHK,
	SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP,
	SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF,
	SEQ_SEMIAUTO_STEP_UNLOADING_END,
}enSeqSemiAutoStep;

static LPCTSTR g_szSeqName_SemiAuto[] =
{
	_T("SEQ_SEMIAUTO_STEP_IDEL"),
	_T("SEQ_SEMIAUTO_STEP_READY"),

	_T("SEQ_SEMIAUTO_STEP_LOADING_START"),
	_T("SEQ_SEMIAUTO_STEP_LOADING_CHK"),
	_T("SEQ_SEMIAUTO_STEP_LOADING_END"),

	_T("SEQ_SEMIAUTO_STEP_LOCK_START"),
	_T("SEQ_SEMIAUTO_STEP_LOCK_CHK"),
	_T("SEQ_SEMIAUTO_STEP_LOCK_MOVE"),
	_T("SEQ_SEMIAUTO_STEP_LOCK_END"),

	_T("SEQ_SEMIAUTO_STEP_PREVIEW_START"),
	_T("SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON"),
	_T("SEQ_SEMIAUTO_STEP_PREVIEW"),
	_T("SEQ_SEMIAUTO_STEP_PREVIEW_END"),


	_T("SEQ_SEMIAUTO_STEP_TLA_START"),
	_T("SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK"),
	_T("SEQ_SEMIAUTO_STEP_TLA_TEST"),
	_T("SEQ_SEMIAUTO_STEP_TLA_ADJUST"),
	_T("SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK"),
	_T("SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST"),
	_T("SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK"),
	_T("SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST"),
	_T("SEQ_SEMIAUTO_STEP_TLA_END"),




	_T("SEQ_SEMIAUTO_STEP_VIGNETING_START"),
	_T("SEQ_SEMIAUTO_STEP_VIGNETING_CHK"),
	_T("SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST"),
	_T("SEQ_SEMIAUTO_STEP_VIGNETING_END"),



	_T("SEQ_SEMIAUTO_STEP_UNLOADING_START"),
	_T("SEQ_SEMIAUTO_STEP_UNLOADING_CHK"),
	_T("SEQ_SEMIAUTO_STEP_UNLOADING_MOVE"),
	_T("SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF"),
	_T("SEQ_SEMIAUTO_STEP_UNLOADING_END"),
};

typedef enum
{
	SEQ_STARTUPCHECK_STEP_IDEL = 0,

	SEQ_STARTUPCHECK_STEP_READY,

	SEQ_STARTUPCHECK_STEP_SCREW_01_START,//
	SEQ_STARTUPCHECK_STEP_SCREW_01_END,

	SEQ_STARTUPCHECK_STEP_SCREW_02_START,
	SEQ_STARTUPCHECK_STEP_SCREW_02_END,

	SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START,
	SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT,
	SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END,

	SEQ_STARTUPCHECK_STEP_PREVIEW_START,
	SEQ_STARTUPCHECK_STEP_PREVIEW_END,

	SEQ_STARTUPCHECK_STEP_TLA_TEST_START,
	SEQ_STARTUPCHECK_STEP_TLA_TEST,
	SEQ_STARTUPCHECK_STEP_TLA_TEST_END,

	SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START,
	SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST,
	SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END,

	SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START,
	SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST,
	SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END,

	SEQ_STARTUPCHECK_STEP_UNLOCK_START,
	SEQ_STARTUPCHECK_STEP_UNLOCK_END,


// 	SEQ_STARTUPCHECK_STEP_WAIT_SOCKET,
// 
// 	// (INIT)
// 	SEQ_STARTUPCHECK_STEP_INIT_START,
// 	SEQ_STARTUPCHECK_STEP_INIT_PREVIEW_STOP,						// SEMCO PREVIEW STOP
// 	SEQ_STARTUPCHECK_STEP_INIT_POWER_OFF,							// POWER SUPPLY OFF
// //	SEQ_STARTUPCHECK_STEP_INIT_SOCKET_UP,							// LEFT, RIGHT SOCKET UP
// 	SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_SAFETY_POS,					// LEFT, RIGHT 안전위치로 UP
// 	SEQ_STARTUPCHECK_STEP_INIT_MOVE_X_LOAD_POS,						// X 로딩 위치로 이동(Z축이 안전위치인지 확인하자)
// 	SEQ_STARTUPCHECK_STEP_INIT_INSPECTION_CONVEYOR_CHECK,			// 내려놓을곳에 자재 있는지 확인(센서 감지 상태 먼저 확인하자)
// 	SEQ_STARTUPCHECK_STEP_INIT_LOAD_PROBE_DN,						// PROBE DN
// 	SEQ_STARTUPCHECK_STEP_INIT_LOAD_GRIPOFF,						// GRIP OFF
// 	SEQ_STARTUPCHECK_STEP_INIT_LOAD_SOCKET_DN,						// 선택 포트 SOCKET DN
// 	SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_CONVEYOR_WAIT_POS,			// Z축 컨베이어 대기 위치로 이동 
// 	SEQ_STARTUPCHECK_STEP_INIT_FINALIZE,
// 	SEQ_STARTUPCHECK_STEP_INIT_END,
// 
// 	// (GRIP)
// 	SEQ_STARTUPCHECK_STEP_GRIP_START,
// 	SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET_ON_CONVEYOR,			// 컨베이어위 소켓 확인
// 	SEQ_STARTUPCHECK_STEP_GRIP_GRIP_OFF,							// GRIP OFF
// 	SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_GRIP_POS,						// Z축 GRIP 위치로 올리기																
// 	SEQ_STARTUPCHECK_STEP_GRIP_CHECK_LOADING,						// CHECK LOADING
// 	SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET,						// CHECK SOCKET	
// 	SEQ_STARTUPCHECK_STEP_GRIP_CHECK_CAMERA_REVERSE,				// CHECK CAMERA REVERSE
// 	SEQ_STARTUPCHECK_STEP_GRIP_GRIP_ON,								// GRIP ON
// 	SEQ_STARTUPCHECK_STEP_GRIP_GRIP_RE_OFF,							// GRIP OFF
// 	SEQ_STARTUPCHECK_STEP_GRIP_PROBE_CHANGE_MODEL,					// CHANGE  MODEL 선택
// 	SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON,							// VACCUM
// 	SEQ_STARTUPCHECK_STEP_GRIP_SOCKET_UP,							// SOCKET UP
// 	SEQ_STARTUPCHECK_STEP_GRIP_PROBE_UP,							// PROBE UP
// 	SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON_CHECK,						// VACCUM CHECK
// 	SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_SAFETY_POS,					// Z축 안전위치로 이동
// 	SEQ_STARTUPCHECK_STEP_GRIP_END,
// 
// 	// (INSPECTION POS)
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_START,
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_POWER_ON,					// POWER SUPPLY ON
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_READY,				// SEMCO READY
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_INITIALIZE,			// SEMCO INITIALIZE
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_Z_SAFETY_CHECK,			// Z축 안전위치 확인
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_X_INSPECTION_POS,		// X축 검사 위치로 이동(Z축 안전위치 확인)
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_Z_INSPECTION_POS,		// Z축 검사 위치로 이동
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_TEST,						// 시업점검 측정
// 	SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_END,
// 
// 	// COLOR CAL INIT
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_START,
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_SAFETY_POS,			// MOVE Z 안전위치로 이동(UP)
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_X_CL500A_POS,			// X축 측정위치로 이동
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_CL500A_WAIT_POS,		// Z축 대기 위치로 이동(DN)
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_FINALIZE,
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_END,
// 
// 	// COLOR CAL CL500A 안착
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_START,
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_JIGCHECK,			// 안착 안내 및 지그 센서 확인 
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_CONNECT,				// CL500A 연결
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_MOVE_Z_TEST_POS,		// Z축 측정 위치로 이동(UP)
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_END,
// 
// 	// COLOR CAL CL500A 측정 
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_START,
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST,						// CL500A 측정 
// 	SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_END,
// 

//	SEQ_STARTUPCHECK_STEP_COMPLETE,	
}enSeqStartupCheckStep;

static LPCTSTR g_szSeqName_StartupCheck[] =
{
	_T("SEQ_STARTUPCHECK_STEP_READY"),
	_T("SEQ_STARTUPCHECK_STEP_IDEL"),
	_T("SEQ_STARTUPCHECK_STEP_READY"),
	_T("SEQ_STARTUPCHECK_STEP_SCREW_01_START"),
	_T("SEQ_STARTUPCHECK_STEP_SCREW_01_END"),
	_T("SEQ_STARTUPCHECK_STEP_SCREW_02_START"),
	_T("SEQ_STARTUPCHECK_STEP_SCREW_02_END"),
	_T("SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START"),
	_T("SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT"),
	_T("SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END"),
	_T("SEQ_STARTUPCHECK_STEP_PREVIEW_START"),
	_T("SEQ_STARTUPCHECK_STEP_PREVIEW_END"),
	_T("SEQ_STARTUPCHECK_STEP_TLA_TEST_START"),
	_T("SEQ_STARTUPCHECK_STEP_TLA_TEST"),
	_T("SEQ_STARTUPCHECK_STEP_TLA_TEST_END"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST"),
	_T("SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END"),
	_T("SEQ_STARTUPCHECK_STEP_UNLOCK_START"),
	_T("SEQ_STARTUPCHECK_STEP_UNLOCK_END"),
};