#pragma once

typedef enum
{
	MCEC_OK = 0,
	MCEC_Unknown,
	MCEC_ForceStop,

	// 설비 유틸 io 와 관련된 에러코드
	MCEC_Door_Open,		// door
	MCEC_Door_OpenFront1,
	MCEC_Door_OpenFront2,
	MCEC_Door_OpenRear1,
	MCEC_Door_OpenRear2,

	//---------------------------- 에러 발생후 원점을 잡아야만 재시작 할수 있다 //-----------------
	MCEC_EMO,			// emo
	MCEC_AirPressure,	// air

	// 모션
	MCEC_Motion_NotInitialize,

	MCEC_Motion_Vignetting_X_NotOrigin,
	MCEC_Motion_Vignetting_X_MoveTimeOut,
	MCEC_Motion_Vignetting_X_NotSafetyPos,
	MCEC_Motion_Vignetting_X_NotInspectionPos,
	MCEC_Motion_Vignetting_X_Alarm,
	MCEC_Motion_Vignetting_X_PositionError,
	MCEC_Motion_Vignetting_X_IsNotMoving,
	MCEC_Motion_Vignetting_X_MoveError,

	MCEC_Motion_Vignetting_R_NotOrigin,
	MCEC_Motion_Vignetting_R_MoveTimeOut,
	MCEC_Motion_Vignetting_R_NotSafetyPos,
	MCEC_Motion_Vignetting_R_NotInspectionPos,
	MCEC_Motion_Vignetting_R_Alarm,
	MCEC_Motion_Vignetting_R_PositionError,
	MCEC_Motion_Vignetting_R_IsNotMoving,
	MCEC_Motion_Vignetting_R_MoveError,

	MCEC_Motion_Screw_L_NotOrigin,
	MCEC_Motion_Screw_L_MoveTimeOut,
	MCEC_Motion_Screw_L_NotSafetyPos,
	MCEC_Motion_Screw_L_NotInspectionPos,
	MCEC_Motion_Screw_L_Alarm,
	MCEC_Motion_Screw_L_PositionError,
	MCEC_Motion_Screw_L_IsNotMoving,
	MCEC_Motion_Screw_L_MoveError,

	MCEC_Motion_Screw_R_NotOrigin,
	MCEC_Motion_Screw_R_MoveTimeOut,
	MCEC_Motion_Screw_R_NotSafetyPos,
	MCEC_Motion_Screw_R_NotInspectionPos,
	MCEC_Motion_Screw_R_Alarm,
	MCEC_Motion_Screw_R_PositionError,
	MCEC_Motion_Screw_R_IsNotMoving,
	MCEC_Motion_Screw_R_MoveError,

	MCEC_Motion_Adjust_NotOrigin, 
	MCEC_Motion_Adjust_MoveTimeOut,
	MCEC_Motion_Adjust_NotSafetyPos,
	MCEC_Motion_Adjust_NotInspectionPos,
	MCEC_Motion_Adjust_Alarm,
	MCEC_Motion_Adjust_PositionError,
	MCEC_Motion_Adjust_IsNotMoving,
	MCEC_Motion_Adjust_MoveError,


// 	MCEC_AxisX_NotLoadPos,
// 	MCEC_AxisX_NotInspectionPos,
// 	MCEC_Motion_AxisZ1_NotSafetyPos,
// 	MCEC_Motion_AxisZ2_NotSafetyPos,
// 	MCEC_Motion_AxisZ1_NotInspectionPos,
// 	MCEC_Motion_AxisZ2_NotInspectionPos,
// 	MCEC_Motion_AxisZ1_NotConveyorWaitPos,
// 	MCEC_Motion_AxisZ2_NotConveyorWaitPos,
// 
// 	MCEC_Motion_AxisX_Command,
// 	MCEC_Motion_AxisZ_Command,
// 	MCEC_Motion_MoveTimeOut,
// 	
// 	MCEC_Motion_AxisX_Alarm,
// 	MCEC_Motion_AxisZ1_Alarm,
// 	MCEC_Motion_AxisZ2_Alarm,
// 
// 	MCEC_Motion_AxisX_PositionError,
// 	MCEC_Motion_AxisZ1_PositionError,
// 	MCEC_Motion_AxisZ2_PositionError,
// 
// 	MCEC_Motion_CV1_ConveyorResetCmd,
// 	MCEC_Motion_CV1_ConveyorStopCmd,
// 	MCEC_Motion_CV1_ConveyorRunCmd,
// 	MCEC_Motion_CV1_ConveyorIsNotMoving,
// 
// 	MCEC_Motion_CV2_ConveyorResetCmd,
// 	MCEC_Motion_CV2_ConveyorStopCmd,
// 	MCEC_Motion_CV2_ConveyorRunCmd,
// 	MCEC_Motion_CV2_ConveyorIsNotMoving,
// 
// 	MCEC_Motion_CVR_ConveyorResetCmd,
// 	MCEC_Motion_CVR_ConveyorStopCmd,
// 	MCEC_Motion_CVR_ConveyorRunCmd,
// 	MCEC_Motion_CVR_ConveyorIsNotMoving,
// 
// 	MCEC_Motion_CharZ_NotOrigin,
// 	MCEC_Motion_CharZ_NotGantryMode,
// 	MCEC_Motion_CharZ_MoveError,

	//-------------------------------------------------------------------------------------//

	// IO
	MCEC_Sensnor_Product_Socket_Lock_TimeOut,
	MCEC_Sensnor_Product_Socket_Unlock_TimeOut,

	MCEC_Sensnor_Chuck_Fwd_TimeOut,
	MCEC_Sensnor_Chuck_Bwd_TimeOut,


	MCEC_Screw_Complete_TimeOut,
	MCEC_Screw_Complete_L_TimeOut,
	MCEC_Screw_Complete_R_TimeOut,

	MCEC_ScrewCheckJig_Lock_L_TimeOut,
	MCEC_ScrewCheckJig_Lock_R_TimeOut,
	MCEC_ScrewCheckJig_Unlock_L_TimeOut,
	MCEC_ScrewCheckJig_Unlock_R_TimeOut,

	//Sensnor 
	MCEC_Sensnor_Product_L_NotChk,
	MCEC_Sensnor_Product_R_NotChk,

	MCEC_Sensnor_Jig_L_Chk,
	MCEC_Sensnor_Jig_R_Chk,
	MCEC_Sensnor_Jig_IN_Fwd_L_Chk,
	MCEC_Sensnor_Jig_IN_Fwd_R_Chk,
	MCEC_Sensnor_Jig_OUT_Bwd_L_Chk,
	MCEC_Sensnor_Jig_OUT_Bwd_R_Chk,

	MCEC_Sensnor_Out_Jig_Chk,// 토크체크 지그 감지 없어야합니다.
	MCEC_Sensnor_In_Jig_Chk,// 토크체크 지그 감지 있어야 합니다. 

	




	// RFID SCAN
//	MCEC_RFID_CV1_ScanFail,

	// 공정 SW
	MCEC_Tester_1_NotConnect,
	MCEC_Tester_1_SendCommand,
	MCEC_Tester_2_NotConnect,
	MCEC_Tester_2_SendCommand,
	MCEC_Tester_Ready,
	MCEC_Tester_Ack_Timeout,



	// 물류제어 서버
	MCEC_ICS_AckTimeout,
	MCEC_ICS_CV1_SendPortStatusAck_Timeout,
	MCEC_ICS_CV1_SendConveyorStatusAck_Timeout,
	MCEC_ICS_CV1_SendSocketRequestAck_Timeout,

	MCEC_ICS_CV2_SendPortStatusAck_Timeout,
	MCEC_ICS_CV2_SendConveyorStatusAck_Timeout,

	MCEC_ICS_CVR_SendPortStatusAck_Timeout,
	MCEC_ICS_CVR_SendConveyorStatusAck_Timeout,

	MCEC_ICS_PICKER_SendPortStatusAck_Timeout,
	MCEC_ICS_PICKER_SendConveyorStatusAck_Timeout,

	// CL500A
	MCEC_CL500A_NotConnected,

	MCEC_IO_AREA_Sensor,

	MCEC_MaxNum,

}enMCErrCode;

static LPCTSTR g_szMachineErrKor[] = 
{
	_T("성공"),															//MCEC_OK = 0,
	_T("이유를 알수없는 에러"),											//MCEC_Unknown,
	_T("정지"),															//MCEC_ForceStop

	//// 설비 유틸 io 와 관련된 에러코드
	_T("도어를 닫아주세요. Output Doorlock 신호를 켜주십시오"),			//MCEC_Door_Open,		// door
	_T("전면1 도어 가 잠기지 않았습니다. 전면1 도어를 확인해주세요."),	//MCEC_Door_OpenFront1,
	_T("전면2 도어 가 잠기지 않았습니다. 전면2 도어를 확인해주세요."),	//MCEC_Door_OpenFront2,
	_T("후면1 도어 가 잠기지 않았습니다. 후면1 도어를 확인해주세요."),	//MCEC_Door_OpenRear1,
	_T("후면2 도어 가 잠기지 않았습니다. 후면2 도어를 확인해주세요."),	//MCEC_Door_OpenRear2,
	//
	////---------------------------- 에러 발생후 원점을 잡아야만 재시작 할수 있다 //-----------------
	_T("EMO버튼이 눌렸습니다. EMO 해제후 Reset 버튼을 눌러주십시오."),	//MCEC_EMO,			// emo
	_T("AIR 공압이 빠졌습니다."),	//MCEC_AirPressure,	// air
	//
	//// 모션
	_T("모션 초기화를 하지 않았습니다."), //MCEC_Motion_NotInitialize,

	_T("비네팅 X축 모터 원점을 잡지 않았습니다."),		//MCEC_Motion_Vignetting_X_NotOrigin,
	_T("비네팅 X축 모션 움직임 타임아웃 에러 발생"),	//MCEC_Motion_Vignetting_X_MoveTimeOut,
	_T("비네팅 X축 모터 위치가 안전위치가 아닙니다."),	//MCEC_Motion_Vignetting_X_NotSafetyPos,
	_T("비네팅 X축 모터 위치가 검사위치가 아닙니다."),	//MCEC_Motion_Vignetting_X_NotInspectionPos,
	_T("비네팅 X축 모터 알람 발생"),					//MCEC_Motion_Vignetting_X_Alarm,
	_T("비네팅 X축 모터 이동위치가 맞지않습니다."),		//MCEC_Motion_Vignetting_X_PositionError,
	_T("비네팅 X축 현재 이동중이 아닙니다."),			//MCEC_Motion_Vignetting_X_IsNotMoving,
	_T("비네팅 X축 이동실패"),							//MCEC_Motion_Vignetting_X_MoveError,
	//
	_T("비네팅 R축 모터 원점을 잡지 않았습니다."),		//MCEC_Motion_Vignetting_R_NotOrigin,
	_T("비네팅 R축 모션 움직임 타임아웃 에러 발생"),	//MCEC_Motion_Vignetting_R_MoveTimeOut,
	_T("비네팅 R축 모터 위치가 안전위치가 아닙니다."),	//MCEC_Motion_Vignetting_R_NotSafetyPos,
	_T("비네팅 R축 모터 위치가 검사위치가 아닙니다."),	//MCEC_Motion_Vignetting_R_NotInspectionPos,
	_T("비네팅 R축 모터 알람 발생"),					//MCEC_Motion_Vignetting_R_Alarm,
	_T("비네팅 R축 모터 이동위치가 맞지않습니다."),		//MCEC_Motion_Vignetting_R_PositionError,
	_T("비네팅 R축 현재 이동중이 아닙니다."),			//MCEC_Motion_Vignetting_R_IsNotMoving,
	_T("비네팅 R축 이동실패"),							//MCEC_Motion_Vignetting_R_MoveError,
	//
	_T("스크류 L 모터 원점을 잡지 않았습니다."),		//MCEC_Motion_Screw_L_NotOrigin,
	_T("스크류 L 모션 움직임 타임아웃 에러 발생"),		//MCEC_Motion_Screw_L_MoveTimeOut,
	_T("스크류 L 모터 위치가 안전위치가 아닙니다."),	//MCEC_Motion_Screw_L_NotSafetyPos,
	_T("스크류 L 모터 위치가 검사위치가 아닙니다."),	//MCEC_Motion_Screw_L_NotInspectionPos,
	_T("스크류 L 모터 알람 발생"),						//MCEC_Motion_Screw_L_Alarm,
	_T("스크류 L 모터 이동위치가 맞지않습니다."),		//MCEC_Motion_Screw_L_PositionError,
	_T("스크류 L 현재 이동중이 아닙니다."),				//MCEC_Motion_Screw_L_IsNotMoving,
	_T("스크류 L 이동실패"),							//MCEC_Motion_Screw_L_MoveError,
	//
	_T("스크류 R 모터 원점을 잡지 않았습니다."),		//MCEC_Motion_Screw_R_NotOrigin,
	_T("스크류 R 모션 움직임 타임아웃 에러 발생"),		//MCEC_Motion_Screw_R_MoveTimeOut,
	_T("스크류 R 모터 위치가 안전위치가 아닙니다."),	//MCEC_Motion_Screw_R_NotSafetyPos,
	_T("스크류 R 모터 위치가 검사위치가 아닙니다."),	//MCEC_Motion_Screw_R_NotInspectionPos,
	_T("스크류 R 모터 알람 발생"),						//MCEC_Motion_Screw_R_Alarm,
	_T("스크류 R 모터 이동위치가 맞지않습니다."),		//MCEC_Motion_Screw_R_PositionError,
	_T("스크류 R 현재 이동중이 아닙니다."),				//MCEC_Motion_Screw_R_IsNotMoving,
	_T("스크류 R 이동실패"),							//MCEC_Motion_Screw_R_MoveError,
	//
	_T("검사 조정 모터 원점을 잡지 않았습니다."),		//MCEC_Motion_Adjust_NotOrigin,
	_T("검사 조정 모션 움직임 타임아웃 에러 발생"),		//MCEC_Motion_Adjust_MoveTimeOut,
	_T("검사 조정 모터 위치가 안전위치가 아닙니다."),	//MCEC_Motion_Adjust_NotSafetyPos,
	_T("검사 조정 모터 위치가 검사위치가 아닙니다."),	//MCEC_Motion_Adjust_NotInspectionPos,
	_T("검사 조정 모터 알람 발생"),						//MCEC_Motion_Adjust_Alarm,
	_T("검사 조정 모터 이동위치가 맞지않습니다."),		//MCEC_Motion_Adjust_PositionError,
	_T("검사 조정 현재 이동중이 아닙니다."),			//MCEC_Motion_Adjust_IsNotMoving,
	_T("검사 조정 이동실패"),							//MCEC_Motion_Adjust_MoveError,

	//
	////-------------------------------------------------------------------------------------//
	//
	//// IO
		// IO
	_T("제품 고정 실린더 타임아웃 발생 "), // MCEC_Sensnor_Product_Socket_Lock_TimeOut,
	_T("제품 풀림 실린더 타임아웃 발생 "), // MCEC_Sensnor_Product_Socket_Unlock_TimeOut,
	_T("제품 조정부 전후진 실린더 전진 타임아웃 발생 "),//MCEC_Sensnor_Chuck_Fwd_TimeOut,
	_T("제품 조정부 전후진 실린더 후진 타임아웃 발생 "),//MCEC_Sensnor_Chuck_Bwd_TimeOut,
	_T("스크류 완료 타임아웃 발생."),//MCEC_Screw_Complete_TimeOut,
	_T("스크류 완료 타임아웃 발생. Lift"),//MCEC_Screw_Complete_L_TimeOut,
	_T("스크류 완료 타임아웃 발생. Right"),//MCEC_Screw_Complete_R_TimeOut,
	_T("시업 점검용 왼쪽 지그안착부의 지그 고정용 실린더 타임아웃 발생. Lift"),//MCEC_ScrewCheckJig_Lock_L_TimeOut,
	_T("시업 점검용 왼쪽 지그안착부의 지그 고정용 실린더 타임아웃 발생. Right"),//MCEC_ScrewCheckJig_Lock_R_TimeOut,
	_T("시업 점검용 오른쪽  지그안착부의 지그 고정용 실린더 타임아웃 발생. Lift"),//MCEC_ScrewCheckJig_Unlock_L_TimeOut,
	_T("시업 점검용 오른쪽  지그안착부의 지그 고정용 실린더 타임아웃 발생. Right"),//MCEC_ScrewCheckJig_Unlock_R_TimeOut,

	//Sensnor 
	_T("제품이 감지 되지 안았습니다. Lift"),	//MCEC_Sensnor_Product_L_NotChk,
	_T("제품이 감지 되지 안았습니다. Right"),	//MCEC_Sensnor_Product_R_NotChk,
	_T("시업 점검용 지그가 정상적으로 제거 되지 않았습니다. Lift"),//MCEC_Sensnor_Jig_L_Chk,
	_T("시업 점검용 지그가 정상적으로 제거 되지 않았습니다. Right"),//MCEC_Sensnor_Jig_R_Chk,
	_T("시업 점검용 지그 레일이 전진 되어 있습니다. Lift"),//MCEC_Sensnor_Jig_IN_Fwd_L_Chk,
	_T("시업 점검용 지그 레일이 전진 되어 있습니다. Right"),//MCEC_Sensnor_Jig_IN_Fwd_R_Chk,

	_T("시업 점검용 지그 레일이 뒤로 빠져있지 않습니다. Lift"),	//MCEC_Sensnor_Jig_OUT_Bwd_L_Chk,
	_T("시업 점검용 지그 레일이 뒤로 빠져있지 않습니다. Right"),  //MCEC_Sensnor_Jig_OUT_Bwd_R_Chk,
	_T("시업 점검용 지그가 장비 안에 있으면 안됩니다."),	//MCEC_Sensnor_Out_Jig_Chk,// 토크체크 지그 감지 없어야합니다.
	_T("시업 점검용 지그가 장비 안에 있어야 합니다."),	//MCEC_Sensnor_In_Jig_Chk,// 토크체크 지그 감지 있어야 합니다. 


	//
	//// 공정 SW
	_T("테스터1 통신 연결안됨"),			//MCEC_Tester_1_NotConnect,
	_T("테스터1 명령어 전송 실패"),			//MCEC_Tester_1_SendCommand,
	_T("테스터2 통신 연결안됨"),			//MCEC_Tester_2_NotConnect,
	_T("테스터2 명령어 전송 실패"),			//MCEC_Tester_2_SendCommand,
	_T("테스터 Barcode Read가 안되었습니다. "),			//MCEC_Tester_2_SendCommand,
	_T("테스터 응답에 대한 타임아웃 발생 "), //MCEC_Tester_Ack_Timeout,

	//
	//// 물류제어 서버
	_T("ICS(물류서버) 타임아웃 발생"),	//MCEC_ICS_AckTimeout,
	_T("투입 컨베이어 ICS(물류서버) 포트상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CV1_SendPortStatusAck_Timeout,
	_T("투입 컨베이어 ICS(물류서버) 컨베이어상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CV1_SendConveyorStatusAck_Timeout,
	_T("투입 컨베이어 ICS(물류서버) 소켓 투입요청 명령 ACK 타임아웃 발생"),//MCEC_ICS_CV1_SendSocketRequestAck_Timeout,
	//
	_T("검사 컨베이어 ICS(물류서버) 포트상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CV2_SendPortStatusAck_Timeout,
	_T("검사 컨베이어 ICS(물류서버) 컨베이어상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CV2_SendConveyorStatusAck_Timeout,
	//
	_T("리턴 컨베이어 ICS(물류서버) 포트상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CVR_SendPortStatusAck_Timeout,
	_T("리턴 컨베이어 ICS(물류서버) 컨베이어상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_CVR_SendConveyorStatusAck_Timeout,
	//
	_T("검사부 ICS(물류서버) 포트 상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_PICKER_SendPortStatusAck_Timeout,
	_T("검사부 ICS(물류서버) 컨베이어상태 명령 ACK 타임아웃 발생"),//MCEC_ICS_PICKER_SendConveyorStatusAck_Timeout,

	// CLA500A
	_T("CL500A 통신이 연결 되지 않음"),

	_T("에어리어 센서 감지 됨"),
	
	NULL
};

static LPCTSTR g_szMachineErrEng[] =
{
	_T("success"),															//MCEC_OK = 0,
	_T("unknown error"),											//MCEC_Unknown,
	_T("stop"),															//MCEC_ForceStop

	//// 설비 유틸 io 와 관련된 에러코드
	_T("please close the door. turn on the Output Doorlock  signal"),			//MCEC_Door_Open,		// door
	_T("front 1 door isn't locked. please check the front 1 door."),	//MCEC_Door_OpenFront1,
	_T("front 2 door isn't locked. please check the front 2 door."),	//MCEC_Door_OpenFront2,
	_T("rear 1 door isn't locked. please check the rear 1 door."),	//MCEC_Door_OpenRear1,
	_T("rear 2 door isn't locked. please check the rear 2 door."),	//MCEC_Door_OpenRear2,
	//
	////---------------------------- 에러 발생후 원점을 잡아야만 재시작 할수 있다 //-----------------
	_T("EMO button is pushed. please release EMO button and push Reset button."),	//MCEC_EMO,			// emo
	_T("AIR pneumatic is out."),	//MCEC_AirPressure,	// air
	//
	//// 모션
	_T("Motion is not initialzed/"), //MCEC_Motion_NotInitialize,

	_T("Vignetting_X-axis Motor origin is not taken."),								//MCEC_Motion_Vignetting_X_NotOrigin,
	_T("Vignetting_X-axis Motion move timeout error"),								//MCEC_Motion_Vignetting_X_MoveTimeOut,
	_T("Vignetting_X-axis Motor position is not safe position."),					//MCEC_Motion_Vignetting_X_NotSafetyPos,
	_T("Vignetting_X-axis Motor position is not inspection position."),				//MCEC_Motion_Vignetting_X_NotInspectionPos,
	_T("Vignetting_X-axis Motor Alarm"),											//MCEC_Motion_Vignetting_X_Alarm,
	_T("The movement position of the Vignetting_X-axis motor is not correct."),		//MCEC_Motion_Vignetting_X_PositionError,
	_T("Inspection Vignetting_X-axis is not currently moving."),					//MCEC_Motion_Vignetting_X_IsNotMoving,
	_T("Vignetting_X-axis move Fail"),												//MCEC_Motion_Vignetting_X_MoveError,

	_T("Vignetting_R-axis Motor origin is not taken."),								//MCEC_Motion_Vignetting_R_NotOrigin,
	_T("Vignetting_R-axis Motion move timeout error"),								//MCEC_Motion_Vignetting_R_MoveTimeOut,
	_T("Vignetting_R-axis Motor position is not safe position."),					//MCEC_Motion_Vignetting_R_NotSafetyPos,
	_T("Vignetting_R-axis Motor position is not inspection position."),				//MCEC_Motion_Vignetting_R_NotInspectionPos,
	_T("Vignetting_R-axis Motor Alarm"),											//MCEC_Motion_Vignetting_R_Alarm,
	_T("The movement position of the Vignetting_R-axis motor is not correct."),		//MCEC_Motion_Vignetting_R_PositionError,
	_T("Inspection Vignetting_R-axis is not currently moving."),					//MCEC_Motion_Vignetting_R_IsNotMoving,
	_T("Vignetting_R-axis move Fail"),												//MCEC_Motion_Vignetting_R_MoveError,

	_T("Screw_L-axis Motor origin is not taken."),								//MCEC_Motion_Screw_L_NotOrigin,
	_T("Screw_L-axis Motion move timeout error"),								//MCEC_Motion_Screw_L_MoveTimeOut,
	_T("Screw_L-axis Motor position is not safe position."),					//MCEC_Motion_Screw_L_NotSafetyPos,
	_T("Screw_L-axis Motor position is not inspection position."),				//MCEC_Motion_Screw_L_NotInspectionPos,
	_T("Screw_L-axis Motor Alarm"),												//MCEC_Motion_Screw_L_Alarm,
	_T("The movement position of the Screw_L-axis motor is not correct."),		//MCEC_Motion_Screw_L_PositionError,
	_T("Inspection Screw_L-axis is not currently moving."),						//MCEC_Motion_Screw_L_IsNotMoving,
	_T("Screw_L-axis move Fail"),												//MCEC_Motion_Screw_L_MoveError,

	_T("Screw_R-axis Motor origin is not taken."),								//MCEC_Motion_Screw_R_NotOrigin,
	_T("Screw_R-axis Motion move timeout error"),								//MCEC_Motion_Screw_R_MoveTimeOut,
	_T("Screw_R-axis Motor position is not safe position."),					//MCEC_Motion_Screw_R_NotSafetyPos,
	_T("Screw_R-axis Motor position is not inspection position."),				//MCEC_Motion_Screw_R_NotInspectionPos,
	_T("Screw_R-axis Motor Alarm"),												//MCEC_Motion_Screw_R_Alarm,
	_T("The movement position of the Screw_R-axis motor is not correct."),		//MCEC_Motion_Screw_R_PositionError,
	_T("Inspection Screw_R-axis is not currently moving."),						//MCEC_Motion_Screw_R_IsNotMoving,
	_T("Screw_R-axis move Fail"),												//MCEC_Motion_Screw_R_MoveError,

	_T("Adjust-axis Motor origin is not taken."),								//MCEC_Motion_Adjust_NotOrigin,
	_T("Adjust-axis Motion move timeout error"),								//MCEC_Motion_Adjust_MoveTimeOut,
	_T("Adjust-axis Motor position is not safe position."),					//MCEC_Motion_Adjust_NotSafetyPos,
	_T("Adjust-axis Motor position is not inspection position."),				//MCEC_Motion_Adjust_NotInspectionPos,
	_T("Adjust-axis Motor Alarm"),												//MCEC_Motion_Adjust_Alarm,
	_T("The movement position of the Adjust-axis motor is not correct."),		//MCEC_Motion_Adjust_PositionError,
	_T("Inspection Adjust-axis is not currently moving."),						//MCEC_Motion_Adjust_IsNotMoving,
	_T("Adjust-axis move Fail"),												//MCEC_Motion_Adjust_MoveError,


	//
	////-------------------------------------------------------------------------------------//
	//
	//// IO
	_T("Product fixed cylinder timeout occurred "), // MCEC_Sensnor_Product_Socket_Lock_TimeOut,
	_T("Product loosening cylinder timeout occurred "), // MCEC_Sensnor_Product_Socket_Unlock_TimeOut,
	_T("Product adjustment part forward/backward cylinder reverse timeout "),//MCEC_Sensnor_Chuck_Fwd_TimeOut,
	_T("Product adjustment part forward/backward cylinder reverse timeout "),//MCEC_Sensnor_Chuck_Bwd_TimeOut,
	_T("Screw completion timeout occurred."),//MCEC_Screw_Complete_TimeOut,
	_T("Screw completion timeout occurred. Lift"),//MCEC_Screw_Complete_L_TimeOut,
	_T("Screw completion timeout occurred. Right"),//MCEC_Screw_Complete_R_TimeOut,
	_T("Cylinder timeout for jig fixing in the left jig seating part for start-up inspection. Lift"),//MCEC_ScrewCheckJig_Lock_L_TimeOut,
	_T("Cylinder timeout for jig fixing in the left jig seating part for start-up inspection. Right"),//MCEC_ScrewCheckJig_Lock_R_TimeOut,
	_T("Cylinder timeout for jig fixing in the right jig seating part for start-up inspection. Lift"),//MCEC_ScrewCheckJig_Unlock_L_TimeOut,
	_T("Cylinder timeout for jig fixing in the right jig seating part for start-up inspection. Right"),//MCEC_ScrewCheckJig_Unlock_R_TimeOut,

	//Sensnor 
	_T("No product detected. Lift"),	//MCEC_Sensnor_Product_L_NotChk,
	_T("No product detected. Right"),	//MCEC_Sensnor_Product_R_NotChk,
	_T("The start-up check jig was not removed normally. Lift"),//MCEC_Sensnor_Jig_L_Chk,
	_T("The start-up check jig was not removed normally. Right"),//MCEC_Sensnor_Jig_R_Chk,
	_T("The jig rail for startup inspection is advanced. Lift"),//MCEC_Sensnor_Jig_IN_Fwd_L_Chk,
	_T("The jig rail for startup inspection is advanced. Right"),//MCEC_Sensnor_Jig_IN_Fwd_R_Chk,

	_T("The jig rail for start-up inspection is not pulled back. Lift"),	//MCEC_Sensnor_Jig_OUT_Bwd_L_Chk,
	_T("The jig rail for start-up inspection is not pulled back. Right"),  //MCEC_Sensnor_Jig_OUT_Bwd_R_Chk,
	_T("The start-up inspection jig must not be inside the machine."),	//MCEC_Sensnor_Out_Jig_Chk,// 토크체크 지그 감지 없어야합니다.
	_T("The start-up inspection jig must be in the machine."),	//MCEC_Sensnor_In_Jig_Chk,// 토크체크 지그 감지 있어야 합니다. 

	//// 공정 SW
	_T("tester 1 communication is not connected"),					//MCEC_Tester_1_NotConnect,
	_T("tester 1 command transmission failure"),					//MCEC_Tester_1_SendCommand,
	_T("tester 2 communication is not connected"),					//MCEC_Tester_2_NotConnect,
	_T("tester 2 command transmission failure"),					//MCEC_Tester_2_SendCommand,
	_T("Tester Barcode Read failure. "),			//MCEC_Tester_2_SendCommand,
	_T("Timeout for tester response "),
		
	//
	//// 물류제어 서버
	_T("ICS(Logistics Server) timeout occurred"),	//MCEC_ICS_AckTimeout,
	_T("Feed conveyor ICS(Logistics Server) port status command ACK Timeout"),//MCEC_ICS_CV1_SendPortStatusAck_Timeout,
	_T("Feed conveyor ICS(Logistics Server) conveyor status command ACK Timeout"),//MCEC_ICS_CV1_SendConveyorStatusAck_Timeout,
	_T("Feed conceyor ICS(Logistics Server) socket insert request command ACK Timeout"),//MCEC_ICS_CV1_SendSocketRequestAck_Timeout,
	//
	_T("Inspection conveyor ICS(Logistics Server) port status command ACK Timeout"),//MCEC_ICS_CV2_SendPortStatusAck_Timeout,
	_T("Inspection conveyor ICS(Logistics Server) conveyor status command ACK Timeout"),//MCEC_ICS_CV2_SendConveyorStatusAck_Timeout,
	//
	_T("Return conveyor ICS(Logistics Server) port status ACK Timeout"),//MCEC_ICS_CVR_SendPortStatusAck_Timeout,
	_T("Return conveyor ICS(LOgistics Server) conveyor status command ACK Timeout"),//MCEC_ICS_CVR_SendConveyorStatusAck_Timeout,
	//
	_T("Inspection unit ICS(Logistics Server) port status command ACK Timeout"),//MCEC_ICS_PICKER_SendPortStatusAck_Timeout,
	_T("Inspection unit ICS(Logistics Server) conveyor status command ACK Timeout"),//MCEC_ICS_PICKER_SendConveyorStatusAck_Timeout,

	// CLA500A
	_T("CL500A communication not connected"),


	_T("AREA Sensor Check"),
		

	NULL
};