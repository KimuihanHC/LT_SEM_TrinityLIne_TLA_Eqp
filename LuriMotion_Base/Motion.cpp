#include "pch.h"
#include "Motion.h"
#include "LuriMotion_BaseDlg.h"

#define WaitForEndTimeout	10000

CMotion::CMotion()
{
}

CMotion::~CMotion()
{
}

// enMCErrCode CMotion::MO_IsConveyor1_Moving()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->IsMotioning(CONVEYOR_1) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV1_ConveyorIsNotMoving;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor1_Run(__in enConveyorSpeed spd)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// // 	double dVelocity = stMotion.nMotorSpeed[CONVEYOR_1][spd];
// // 
// // 	if (FAS_MO->MoveVelocity(CONVEYOR_1, (DWORD)dVelocity, 1) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV1_ConveyorRunCmd;
// // 	}
// 
// 	Sleep(50);
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor1_Stop()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->MoveStop(CONVEYOR_1) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV1_ConveyorStopCmd;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor1_Reset()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->SetResetPosition(CONVEYOR_1) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV1_ConveyorResetCmd;
// // 	}		
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_IsConveyor2_Moving()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->IsMotioning(CONVEYOR_2) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV2_ConveyorIsNotMoving;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor2_Run(__in enConveyorSpeed spd)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// // 	double dVelocity = stMotion.nMotorSpeed[CONVEYOR_2][spd];
// // 
// // 	if (FAS_MO->MoveVelocity(CONVEYOR_2, (DWORD)dVelocity, 1) == FALSE)
// // 	{
// // //		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		WRITE_MAIN_LOG(_T("CONVEYOR2 RUN"));
// // 		err = MCEC_Motion_CV2_ConveyorRunCmd;
// // 	}
// 	//Sleep(50);
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor2_Stop()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->MoveStop(CONVEYOR_2) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV2_ConveyorStopCmd;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyor2_Reset()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->SetResetPosition(CONVEYOR_2) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CV2_ConveyorResetCmd;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_IsConveyorRet_Moving()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->IsMotioning(CONVEYOR_3) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CVR_ConveyorIsNotMoving;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyorRet_Run(__in enConveyorSpeed spd)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	double dVelocity = stMotion.nMotorSpeed[CONVEYOR_3][spd];
// 
// // 	if (FAS_MO->MoveVelocity(CONVEYOR_3, (DWORD)dVelocity, 1) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CVR_ConveyorRunCmd;
// // 	}
// 	//Sleep(50);
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyorRet_Stop()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->MoveStop(CONVEYOR_3) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CVR_ConveyorStopCmd;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveConveyorRet_Reset()
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->SetResetPosition(CONVEYOR_3) == false)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_CVR_ConveyorResetCmd;
// // 	}
// 
// 	return err;
// }
// 
// 
// enMCErrCode CMotion::MO_Wait_To_Moving_End(__in UINT nAxis, __in double dbPos)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	Sleep(100);
// 
// 	// 모터 원점 확인
// // 	if (FAS_MO->IsHomeOK(nAxis) == false)
// // 	{
// // 		if (nAxis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (nAxis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (nAxis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 	
// 	// 모터 이동완료 확인
// 	bool bComplete = false;
// 	clock_t starttm = clock();
// 	do 
// 	{
// // 		if (FAS_MO->IsMotioning(nAxis) == false)
// // 		{
// // 			bComplete = true;
// // 			break;
// // 		}
// 
// 		Sleep(10);
// 	} while ((clock() - starttm) < WaitForEndTimeout);
// 	
// 
// 	if (!bComplete)
// 	{
// 		err = MCEC_Motion_MoveTimeOut;
// 		return err;
// 	}		
// 		
// 	// 모터 상태 확인
// // 	if (FAS_MO->IsAlarmStatus(nAxis) == true)
// // 	{
// // 		// 알람 발생
// // 		if (nAxis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_Alarm;
// // 
// // 		if (nAxis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_Alarm;
// // 
// // 		if (nAxis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_Alarm;
// // 
// // 		return err;
// // 	}
// 
// 	Sleep(50);
// 	
// 	// 모터 위치값이 같은지 확인
// //	auto CurPos = FAS_MO->FAS_MO->GetAxisCurrnetActualPos(nAxis);
// // 	if (abs(CurPos - dbPos) <= 3)
// // 	{
// // 
// // 	}
// // 	else
// // 	{
// // 		if (nAxis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_PositionError;
// // 
// // 		if (nAxis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_PositionError;
// // 
// // 		if (nAxis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_PositionError;
// //	}
// 	
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_CheckZ_To_SafetyPos(__in InspectionPortType port)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (port == InspectionPortType::Left)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z1) == FALSE)
// // 			return MCEC_Motion_AxisZ1_NotOrigin;
// // 	}
// // 	else if (port == InspectionPortType::Right)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z2) == FALSE)
// // 			return MCEC_Motion_AxisZ2_NotOrigin;
// // 	}
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 	
// 	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// // 	auto Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 	auto Target = 0.0;
// // 	switch (port)
// // 	{
// // 	case InspectionPortType::Left:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 		Target = stTeach.nSocketZ_Wait_Pos / 1000;
// // 	}		
// // 		break;
// // 
// // 	case InspectionPortType::Right:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z2);
// // 		Target = stTeach.nSocketZ2_Wait_Pos / 1000;
// // 	}		
// // 		break;
// // 	default:
// // 		break;
// // 	}
// 
// // 	if (((Cur > Target - 1) &&
// // 		(Cur < Target + 1)) == false)
// // 	{
// // 		if (port == InspectionPortType::Left)
// // 			return MCEC_Motion_AxisZ1_NotSafetyPos;
// // 
// // 		if (port == InspectionPortType::Right)
// // 			return MCEC_Motion_AxisZ2_NotSafetyPos;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_CheckZ_To_InspectionPos(__in InspectionPortType port)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (port == InspectionPortType::Left)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z1) == FALSE)
// // 			return MCEC_Motion_AxisZ1_NotOrigin;
// // 	}
// // 	else if (port == InspectionPortType::Right)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z2) == FALSE)
// // 			return MCEC_Motion_AxisZ2_NotOrigin;
// // 	}
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// // 	auto Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 	auto Target = 0.0;
// // 	switch (port)
// // 	{
// // 	case InspectionPortType::Left:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 		Target = stTeach.nSocketZ_FlareInspection1_Pos / 1000;
// // 	}
// // 	break;
// // 
// // 	case InspectionPortType::Right:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z2);
// // 		Target = stTeach.nSocketZ2_FlareInspection1_Pos / 1000;
// // 	}
// // 	break;
// // 	default:
// // 		break;
// // 	}
// 
// // 	if (((Cur > Target - 1) &&
// // 		(Cur < Target + 1)) == false)
// // 	{
// // 		if (port == InspectionPortType::Left)
// // 			return MCEC_Motion_AxisZ1_NotInspectionPos;
// // 
// // 		if (port == InspectionPortType::Right)
// // 			return MCEC_Motion_AxisZ2_NotInspectionPos;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_CheckZ_To_ConveyorWaitPos(__in InspectionPortType port)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (port == InspectionPortType::Left)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z1) == FALSE)
// // 			return MCEC_Motion_AxisZ1_NotOrigin;
// // 	}
// // 	else if (port == InspectionPortType::Right)
// // 	{
// // 		if (FAS_MO->IsHomeOK(PICKER_Z2) == FALSE)
// // 			return MCEC_Motion_AxisZ2_NotOrigin;
// // 	}
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// // 	auto Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 	auto Target = 0.0;
// // 	switch (port)
// // 	{
// // 	case InspectionPortType::Left:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 		Target = stTeach.nSocketZ_Conveyor_Wait_Pos / 1000;
// // 	}
// // 	break;
// // 
// // 	case InspectionPortType::Right:
// // 	{
// // 		Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z2);
// // 		Target = stTeach.nSocketZ2_Conveyor_Wait_Pos / 1000;
// // 	}
// // 	break;
// // 	default:
// // 		break;
// // 	}
// 
// // 	if (((Cur > Target - 1) &&
// // 		(Cur < Target + 1)) == false)
// // 	{
// // 		if (port == InspectionPortType::Left)
// // 			return MCEC_Motion_AxisZ1_NotConveyorWaitPos;
// // 
// // 		if (port == InspectionPortType::Right)
// // 			return MCEC_Motion_AxisZ2_NotConveyorWaitPos;
// // 	}
// 
// 	return err;
// }
// 
// 
// enMCErrCode CMotion::MO_CheckX_To_LoadingPos(__in InspectionPortType port)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->IsHomeOK(PICKER_X) == FALSE)
// // 		return MCEC_Motion_AxisX_NotOrigin;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// 	//auto Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_X);
// 	auto Target = 0.0;
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Target = stTeach.nSocketX_Loading2_Inpsection_Pos / 1000;
// 		break;
// 
// 	case InspectionPortType::Right:
// //		Target = stTeach.nSocketX_Loading1_Inpsection_Pos / 1000;
// 		break;
// 	default:
// 		break;
// 	}
// 
// // 	if (((Cur > Target - 1) &&
// // 		(Cur < Target + 1)) == false)
// // 	{
// // 		err = MCEC_AxisX_NotLoadPos;
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_CheckX_To_InspectionPos(__in InspectionPortType port)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// // 	if (FAS_MO->IsHomeOK(PICKER_X) == FALSE)
// // 		return MCEC_Motion_AxisX_NotOrigin;
// 
// 	TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 
// // 	auto Cur = FAS_MO->GetAxisCurrnetActualPos(PICKER_X);
// // 	auto Target = 0.0;
// // 	switch (port)
// // 	{
// // 	case InspectionPortType::Left:
// // 		Target = stTeach.nSocketX_Loading1_Inpsection_Pos / 1000;
// // 		break;
// // 
// // 	case InspectionPortType::Right:
// // 		Target = stTeach.nSocketX_Loading2_Inpsection_Pos / 1000;
// // 		break;
// // 	default:
// // 		break;
// // 	}
// 
// // 	if (((Cur > Target - 1) &&
// // 		(Cur < Target + 1)) == false)
// // 	{
// // 		err = MCEC_AxisX_NotInspectionPos;
// // 	}
// 	
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveX_To_LoadPos(__in InspectionPortType port, enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 	
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	err = MO_CheckZ_To_SafetyPos(InspectionPortType::Left);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	err = MO_CheckZ_To_SafetyPos(InspectionPortType::Right);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// Z1, Z2 축이 모두 올라와 있을 경우 움직이자(대기위치)
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketX_Loading2_Inpsection_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_X;
// 		break;
// 
// 	case InspectionPortType::Right:
// 	//	Pos = (double)stTeaching.nSocketX_Loading1_Inpsection_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_X;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisX_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveX_To_InspectionPos(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// Z1, Z2 축이 모두 올라와 있을 경우 움직이자(대기위치)
// 	err = MO_CheckZ_To_SafetyPos(InspectionPortType::Left);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	err = MO_CheckZ_To_SafetyPos(InspectionPortType::Right);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketX_Loading1_Inpsection_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_X;
// 		break;
// 
// 	case InspectionPortType::Right:
// 	//	Pos = (double)stTeaching.nSocketX_Loading2_Inpsection_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_X;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisX_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_GripUp(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	/*err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;*/
// 
// 	// X 축이 로딩 위치에 있을 경우 
// 	/*err = MO_CheckX_To_LoadingPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;*/
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_Conveyor_Grap_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// 	//	Pos = (double)stTeaching.nSocketZ2_Conveyor_Grap_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisX_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_Inspection1(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// Shutter Close 일 경우 움직이면 안된다.
// 	if (!IO_Light_ShutterOpen())
// 	{
// 		err = enMCErrCode::MCEC_IO_LIGHT_Shutter_Open;
// 		return err;
// 	}
// 
// 	// X 축이 검사 위치에 있을 움직이자
// 	err = MO_CheckX_To_InspectionPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_FlareInspection1_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// 	//	Pos = (double)stTeaching.nSocketZ2_FlareInspection1_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_Inspection2(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// Shutter Close 일 경우 움직이면 안된다.
// 	if (!IO_Light_ShutterOpen())
// 	{
// 		err = enMCErrCode::MCEC_IO_LIGHT_Shutter_Open;
// 		return err;
// 	}
// 
// 	// X 축이 검사 위치에 있을 움직이자
// 	err = MO_CheckX_To_InspectionPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_FlareInspection2_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// 	//	Pos = (double)stTeaching.nSocketZ2_FlareInspection2_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_ConveyorWait(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 		
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	err = MO_CheckX_To_LoadingPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_Conveyor_Wait_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// //		Pos = (double)stTeaching.nSocketZ2_Conveyor_Wait_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// 	//	Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_Safety(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_Wait_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// //		Pos = (double)stTeaching.nSocketZ2_Wait_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_180Inspection(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// CL500A 지그 장착된 상태에서는 움직이면 안된다.
// 	err = IO_Picker_CL500A_Empty_Check();
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	// Shutter Close 일 경우 움직이면 안된다.
// 	if (!IO_Light_ShutterOpen())
// 	{
// 		err = enMCErrCode::MCEC_IO_LIGHT_Shutter_Open;
// 		return err;
// 	}
// 
// 	// X 축이 검사 위치에 있을 움직이자
// 	err = MO_CheckX_To_InspectionPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_Reserved_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// //		Pos = (double)stTeaching.nSocketZ2_Reserved_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }
// 
// enMCErrCode CMotion::MO_MoveZ_To_CL500A(__in InspectionPortType port, __in enConveyorSpeed spd /*= High*/)
// {
// 	enMCErrCode err = MCEC_OK;
// 
// 	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
// 	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
// 
// 	auto Pos = 0.0;
// 	auto Axis = 0;
// 	auto Speed = 0.0;
// 
// 	auto TargetPos = 0.0;
// 
// 	// Shutter Close 일 경우 움직이면 안된다.
// 	if (!IO_Light_ShutterOpen())
// 	{
// 		err = enMCErrCode::MCEC_IO_LIGHT_Shutter_Open;
// 		return err;
// 	}
// 	
// 	// X 축이 검사 위치에 있을 움직이자
// 	err = MO_CheckX_To_InspectionPos(port);
// 	if (err != enMCErrCode::MCEC_OK)
// 		return err;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// //		Pos = (double)stTeaching.nSocketZ_Reserved_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z1;
// 		break;
// 
// 	case InspectionPortType::Right:
// //		Pos = (double)stTeaching.nSocketZ2_Reserved_Pos / 1000;
// 		TargetPos = Pos;
// 		Pos = Pos * 500;
// //		Axis = PICKER_Z2;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	double dSpeed = stMotion.nMotorSpeed[Axis][spd];
// 
// // 	if (FAS_MO->IsHomeOK(Axis) == false)
// // 	{
// // 		if (Axis == PICKER_X)
// // 			err = MCEC_Motion_AxisX_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z1)
// // 			err = MCEC_Motion_AxisZ1_NotOrigin;
// // 
// // 		if (Axis == PICKER_Z2)
// // 			err = MCEC_Motion_AxisZ2_NotOrigin;
// // 
// // 		return err;
// // 	}
// 
// // 	if (FAS_MO->SingleAxisAbsMove(Axis, Pos, dSpeed) == FALSE)
// // 	{
// // 		WRITE_ERR_LOG(FAS_MO->GetLastError());
// // 		err = MCEC_Motion_AxisZ_Command;
// // 	}
// // 	else
// // 	{
// // 		// 모션이동이 정상적으로 끝났는지 확인하자
// // 		err = MO_Wait_To_Moving_End(Axis, TargetPos);
// // 	}
// 
// 	return err;
// }

// double CMotion::MO_MoveX_GetPosition()
// {
// 	double dbPos = 0.0;
// //	dbPos = FAS_MO->GetAxisCurrnetActualPos(PICKER_X);
// 	return dbPos;
// }
// 
// double CMotion::MO_MoveZ_GetPosition(__in InspectionPortType port)
// {
// 	double dbPos = 0.0;
// 
// // 	switch (port)
// // 	{
// // 	case InspectionPortType::Left:
// // 		dbPos = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z1);
// // 		break;
// // 	case InspectionPortType::Right:
// // 		dbPos = FAS_MO->GetAxisCurrnetActualPos(PICKER_Z2);
// // 		break;
// // 	default:
// // 		break;
// // 	}
// 	return dbPos;
// }
// TLA 22.03.21
enMCErrCode CMotion::MO_MoveVignetting_X_Type(__in enVignetting_X Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(VIGNETTING_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Vignetting_X_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[0][3];
	double pos = (double)stTeaching.nVignetting_X_Wait / 1000;
	switch (Type)
	{
	case enVignetting_X::X_Wait:		pos = (double)stTeaching.nVignetting_X_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		pos = (double)stTeaching.nVignetting_X_Test_01 / 1000;	break;
	case enVignetting_X::X_Test_02:		pos = (double)stTeaching.nVignetting_X_Test_02 / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_Wait(VIGNETTING_X, pos, dSpeed))
	{
		err = MCEC_Motion_Vignetting_X_MoveError;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_X);

	if (AJIN_MO->Check_Move_Pos(VIGNETTING_X, pos) == FALSE)
	{
		return MCEC_Motion_Vignetting_X_PositionError;
	}
	return err;
}


enMCErrCode CMotion::MO_MoveVignetting_R_Type(__in enVignetting_R Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(VIGNETTING_R) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Vignetting_R_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_R);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[1][3];
	double pos = (double)stTeaching.nVignetting_R_Wait / 1000;
	switch (Type)
	{
	case enVignetting_R::R_Wait:		pos = (double)stTeaching.nVignetting_R_Wait / 1000;	break;
	case enVignetting_R::R_Test_01:		pos = (double)stTeaching.nVignetting_R_Test_01 / 1000;	break;
	case enVignetting_R::R_Test_02:		pos = (double)stTeaching.nVignetting_R_Test_02 / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_Wait(VIGNETTING_R, pos, dSpeed))
	{
		err = MCEC_Motion_Vignetting_R_MoveError;
		return err;
	}



	AJIN_MO->Check_Move_End(VIGNETTING_R);

	if (AJIN_MO->Check_Move_Pos(VIGNETTING_R, pos) == FALSE)
	{
		return MCEC_Motion_Vignetting_R_PositionError;
	}
	return err;
}


enMCErrCode CMotion::MO_MoveScrew_L_Type(__in enScrew Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_L_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(SCREW_L_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[2][3];
	double pos = (double)stTeaching.nScrew_L_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_L_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_L_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_L_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_Wait(SCREW_L_X, pos, dSpeed))
	{
		err = MCEC_Motion_Screw_L_MoveError;
		return err;
	}



	AJIN_MO->Check_Move_End(SCREW_L_X);

	if (AJIN_MO->Check_Move_Pos(SCREW_L_X, pos) == FALSE)
	{
		return MCEC_Motion_Screw_L_PositionError;
	}
	return err;
}

enMCErrCode CMotion::MO_MoveScrew_R_Type(__in enScrew Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_R_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(SCREW_R_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[3][3];
	double pos = (double)stTeaching.nScrew_R_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_R_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_R_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_R_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_Wait(SCREW_R_X, pos, dSpeed))
	{
		err = MCEC_Motion_Screw_R_MoveError;
		return err;
	}



	AJIN_MO->Check_Move_End(SCREW_R_X);

	if (AJIN_MO->Check_Move_Pos(SCREW_R_X, pos) == FALSE)
	{
		return MCEC_Motion_Screw_R_PositionError;
	}
	return err;
}
enMCErrCode CMotion::MO_MoveWait_Type(__in enVignetting_X Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_L_NotOrigin;
		return err;
	}
	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_R_NotOrigin;
		return err;
	}	
	if (AJIN_MO->IsOrigin(VIGNETTING_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Vignetting_X_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_X);
	AJIN_MO->Check_Move_End(SCREW_L_X);
	AJIN_MO->Check_Move_End(SCREW_R_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeedVignetting = (double)stMotion.nMotorSpeed[0][3];
	double posVignetting = (double)stTeaching.nVignetting_X_Wait / 1000;
	switch (Type)
	{
	case enVignetting_X::X_Wait:		posVignetting = (double)stTeaching.nVignetting_X_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		posVignetting = (double)stTeaching.nVignetting_X_Test_01 / 1000;	break;
	case enVignetting_X::X_Test_02:		posVignetting = (double)stTeaching.nVignetting_X_Test_02 / 1000;	break;
	}


	double dSpeed = (double)stMotion.nMotorSpeed[3][3];
	double posL = (double)stTeaching.nScrew_L_Wait / 1000;
	double posR = (double)stTeaching.nScrew_R_Wait / 1000;
	switch (Type)
	{
	case enVignetting_X::X_Wait:		posL = (double)stTeaching.nScrew_L_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		posL = (double)stTeaching.nScrew_L_Commencement / 1000;	break;
	case enVignetting_X::X_Test_02:		posL = (double)stTeaching.nScrew_L_Work / 1000;	break;
	}
	switch (Type)
	{
	case enVignetting_X::X_Wait:		posR = (double)stTeaching.nScrew_R_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		posR = (double)stTeaching.nScrew_R_Commencement / 1000;	break;
	case enVignetting_X::X_Test_02:		posR = (double)stTeaching.nScrew_R_Work / 1000;	break;
	}
	if (!AJIN_MO->MoveAxis_NonWait(VIGNETTING_X, posVignetting, dSpeedVignetting))
	{
		return err = MCEC_Motion_Vignetting_X_MoveError;
	}
	if (!AJIN_MO->MoveAxis_NonWait(SCREW_L_X, posL, dSpeed))
	{
		return err = MCEC_Motion_Screw_R_MoveError;
	}
	if (!AJIN_MO->MoveAxis_NonWait(SCREW_R_X, posR, dSpeed))
	{
		return err = MCEC_Motion_Screw_R_MoveError;
	}
	
	Sleep(100);
	AJIN_MO->Check_Move_End(SCREW_L_X);
	AJIN_MO->Check_Move_End(SCREW_R_X);
	AJIN_MO->Check_Move_End(VIGNETTING_X);

	if (AJIN_MO->Check_Move_Pos(SCREW_L_X, posL) == FALSE)
	{
		return MCEC_Motion_Screw_L_PositionError;
	}
	if (AJIN_MO->Check_Move_Pos(SCREW_R_X, posR) == FALSE)
	{
		return MCEC_Motion_Screw_R_PositionError;
	}
	if (AJIN_MO->Check_Move_Pos(VIGNETTING_X, posVignetting) == FALSE)
	{
		return MCEC_Motion_Vignetting_X_PositionError;
	}
	return err;
}

enMCErrCode CMotion::MO_MoveScrew_Type(__in enScrew Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_L_NotOrigin;
		return err;
	}
	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_R_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(SCREW_L_X);
	AJIN_MO->Check_Move_End(SCREW_R_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[3][3];
	double posL = (double)stTeaching.nScrew_L_Wait / 1000;
	double posR = (double)stTeaching.nScrew_R_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				posL = (double)stTeaching.nScrew_L_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		posL = (double)stTeaching.nScrew_L_Commencement / 1000;	break;
	case enScrew::Screw_Work:				posL = (double)stTeaching.nScrew_L_Work / 1000;	break;
	}
	switch (Type)
	{
	case enScrew::Screw_Wait:				posR = (double)stTeaching.nScrew_R_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		posR = (double)stTeaching.nScrew_R_Commencement / 1000;	break;
	case enScrew::Screw_Work:				posR = (double)stTeaching.nScrew_R_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(SCREW_L_X, posL, dSpeed))
		err = MCEC_Motion_Screw_R_MoveError;
	if (!AJIN_MO->MoveAxis_NonWait(SCREW_R_X, posR, dSpeed))
		err = MCEC_Motion_Screw_R_MoveError;
	Sleep(100);
	AJIN_MO->Check_Move_End(SCREW_L_X);
	AJIN_MO->Check_Move_End(SCREW_R_X);

	if (AJIN_MO->Check_Move_Pos(SCREW_L_X, posL) == FALSE)
	{
		return MCEC_Motion_Screw_L_PositionError;
	}
	if (AJIN_MO->Check_Move_Pos(SCREW_R_X, posR) == FALSE)
	{
		return MCEC_Motion_Screw_R_PositionError;
	}
	return err;
}


enMCErrCode CMotion::MO_MoveAdjust_Type(__in enAdjust Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(TILT_ADJUST_R) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Adjust_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(TILT_ADJUST_R);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[4][3];
	double pos = (double)stTeaching.nAdjust_Wait / 1000;
	switch (Type)
	{
	case enAdjust::Adjust_Wait:				pos = (double)stTeaching.nAdjust_Wait / 1000;	break;
	case enAdjust::Adjust_Work:				pos = (double)stTeaching.nAdjust_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, pos, dSpeed))
	{
		err = MCEC_Motion_Adjust_MoveError;
		return err;
	}
	


	AJIN_MO->Check_Move_End(TILT_ADJUST_R);

	if (AJIN_MO->Check_Move_Pos(TILT_ADJUST_R, pos) == FALSE)
	{
		return MCEC_Motion_Adjust_PositionError;
	}
	return err;
}



enMCErrCode CMotion::MO_NonWait_MoveVignetting_X_Type(__in enVignetting_X Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(VIGNETTING_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Vignetting_X_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_X);

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[0][3];
	double pos = (double)stTeaching.nVignetting_X_Wait / 1000;
	switch (Type)
	{
	case enVignetting_X::X_Wait:		pos = (double)stTeaching.nVignetting_X_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		pos = (double)stTeaching.nVignetting_X_Test_01 / 1000;	break;
	case enVignetting_X::X_Test_02:		pos = (double)stTeaching.nVignetting_X_Test_02 / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(VIGNETTING_X, pos, dSpeed))
		err = MCEC_Motion_Vignetting_X_MoveError;

	return err;
}

enMCErrCode CMotion::MO_NonWait_MoveVignetting_R_Type(__in enVignetting_R Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(VIGNETTING_R) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Vignetting_R_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(VIGNETTING_R);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[1][3];
	double pos = (double)stTeaching.nVignetting_R_Wait / 1000;
	switch (Type)
	{
	case enVignetting_R::R_Wait:		pos = (double)stTeaching.nVignetting_R_Wait / 1000;	break;
	case enVignetting_R::R_Test_01:		pos = (double)stTeaching.nVignetting_R_Test_01 / 1000;	break;
	case enVignetting_R::R_Test_02:		pos = (double)stTeaching.nVignetting_R_Test_02 / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(VIGNETTING_R, pos, dSpeed))
		err = MCEC_Motion_Vignetting_R_MoveError;

	return err;
}

enMCErrCode CMotion::MO_NonWait_MoveScrew_L_Type(__in enScrew Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_L_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_L_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(SCREW_L_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[2][3];
	double pos = (double)stTeaching.nScrew_L_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_L_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_L_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_L_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(SCREW_L_X, pos, dSpeed))
		err = MCEC_Motion_Screw_L_MoveError;

	return err;
}

enMCErrCode CMotion::MO_NonWait_MoveScrew_R_Type(__in enScrew Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(SCREW_R_X) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Screw_R_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(SCREW_R_X);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[3][3];
	double pos = stTeaching.nScrew_R_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_R_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_R_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_R_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(SCREW_R_X, pos, dSpeed))
		err = MCEC_Motion_Screw_R_MoveError;

	return err;
}

enMCErrCode CMotion::MO_NonWait_MoveAdjust_Type(__in enAdjust Type)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (AJIN_MO->IsOrigin(TILT_ADJUST_R) == FALSE) // 오리진 플레그 
	{
		err = MCEC_Motion_Adjust_NotOrigin;
		return err;
	}
	AJIN_MO->Check_Move_End(TILT_ADJUST_R);
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[4][3];
	double pos = (double)stTeaching.nAdjust_Wait / 1000;
	switch (Type)
	{
	case enAdjust::Adjust_Wait:				pos = (double)stTeaching.nAdjust_Wait / 1000;	break;
	case enAdjust::Adjust_Work:				pos = (double)stTeaching.nAdjust_Work / 1000;	break;
	}

	if (!AJIN_MO->MoveAxis_NonWait(TILT_ADJUST_R, pos, dSpeed))
		err = MCEC_Motion_Adjust_MoveError;

	return err;
}


BOOL CMotion::Check_Pos_Vignetting_X_Type(__in enVignetting_X Type)
{
	int nRet = FALSE;

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[0][3];
	double pos = stTeaching.nVignetting_X_Wait / 1000;
	switch (Type)
	{
	case enVignetting_X::X_Wait:		pos = (double)stTeaching.nVignetting_X_Wait / 1000;	break;
	case enVignetting_X::X_Test_01:		pos = (double)stTeaching.nVignetting_X_Test_01 / 1000;	break;
	case enVignetting_X::X_Test_02:		pos = (double)stTeaching.nVignetting_X_Test_02 / 1000;	break;
	}
	return AJIN_MO->Check_Move_Pos(VIGNETTING_X, pos);
}

BOOL CMotion::Check_Pos_Vignetting_R_Type(__in enVignetting_R Type)
{
	int nRet = FALSE;

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[1][3];
	double pos = (double)stTeaching.nVignetting_R_Wait / 1000;
	switch (Type)
	{
	case enVignetting_R::R_Wait:		pos = (double)stTeaching.nVignetting_R_Wait / 1000;	break;
	case enVignetting_R::R_Test_01:		pos = (double)stTeaching.nVignetting_R_Test_01 / 1000;	break;
	case enVignetting_R::R_Test_02:		pos = (double)stTeaching.nVignetting_R_Test_02 / 1000;	break;
	}
	return AJIN_MO->Check_Move_Pos(VIGNETTING_R, pos);
}

BOOL CMotion::Check_Pos_Screw_L_Type(__in enScrew Type)
{
	int nRet = FALSE;
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[2][3];
	double pos = (double)stTeaching.nScrew_L_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_L_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_L_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_L_Work / 1000;	break;
	}
	return AJIN_MO->Check_Move_Pos(SCREW_L_X, pos);
}

BOOL CMotion::Check_Pos_Screw_R_Type(__in enScrew Type)
{
	int nRet = FALSE;
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[3][3];
	double pos = (double)stTeaching.nScrew_R_Wait / 1000;
	switch (Type)
	{
	case enScrew::Screw_Wait:				pos = (double)stTeaching.nScrew_R_Wait / 1000;	break;
	case enScrew::Screw_Commencement:		pos = (double)stTeaching.nScrew_R_Commencement / 1000;	break;
	case enScrew::Screw_Work:				pos = (double)stTeaching.nScrew_R_Work / 1000;	break;
	}
	return AJIN_MO->Check_Move_Pos(SCREW_R_X, pos);
}

BOOL CMotion::Check_Pos_Adjust_Type(__in enAdjust Type)
{
	int nRet = FALSE;

	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	double dSpeed = (double)stMotion.nMotorSpeed[4][3];
	double pos = (double)stTeaching.nAdjust_Wait / 1000;
	switch (Type)
	{
	case enAdjust::Adjust_Wait:				pos = (double)stTeaching.nAdjust_Wait / 1000;	break;
	case enAdjust::Adjust_Work:				pos = (double)stTeaching.nAdjust_Work / 1000;	break;
	}
	return AJIN_MO->Check_Move_Pos(TILT_ADJUST_R, pos);
}




// TLA 22.03.21