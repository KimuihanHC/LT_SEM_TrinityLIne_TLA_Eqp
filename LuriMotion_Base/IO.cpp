#include "pch.h"
#include "IO.h"


#define WaitForDetectTimeout 3000

CIO::CIO()
{
}

CIO::~CIO()
{
}

void CIO::DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CIO::DoEvents(DWORD dwMiliSeconds)
{
	clock_t start_tm = clock();
	do
	{
		DoEvents();
		::Sleep(1);
	} while ((DWORD)(clock() - start_tm) < dwMiliSeconds);
}

bool CIO::WaitStatus(__in int OnPort, __in int OffPort)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(OnPort) == true 
			&& AJIN_IO->GetInputIoBit(OffPort) == false)
		{
			bRet = true;
			break;
		}

		DoEvents(1);
	} while ((clock() - startm) < WaitForDetectTimeout);


	return bRet;
}

bool CIO::WaitStatus(__in int Port, __in bool Status)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(Port) == Status)
		{
			bRet = true;
			break;
		}

		DoEvents(1);
	} while ((clock() - startm) < WaitForDetectTimeout);


	return bRet;
}

bool CIO::IO_I_WaitStatus(__in int idx, __in bool bStatus, __in DWORD dwMilisecond)
{
	bool bRet = false;

	clock_t startm = clock();

	do
	{
		if (AJIN_IO->GetInputIoBit(idx) == bStatus)
		{
			bRet = true;
			break;
		}

		DoEvents(10);
	} while ((clock() - startm) < dwMilisecond);

	return bRet;
}

bool CIO::IO_I_EMO()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_00_EMO);
}

bool CIO::IO_I_PowerOff()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_01_POWER_OFF);
}

bool CIO::IO_I_INIT()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_INIT);
}

bool CIO::IO_I_MainAirPress()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_MAIN_PRESSURE);
}
bool CIO::IO_I_AREA()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_AREA);
}
bool CIO::IO_I_SleepMode()
{
	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_03_SLEEP_MODE);
}

void CIO::IO_O_SleepMode(__in bool bOn)
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_03_SLEEP_MODE_ON, bOn);
}






void CIO::IO_O_FlareLightOn(__in int idx)
{
// #if SET_INSPECTOR == SYS_GHOSTFLARE
// 	if (idx == 0)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// 		return;
// 	}
// 
// 	if (idx == 1) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, TRUE);
// 	else if (idx == 2) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, TRUE);
// 	else if (idx == 3) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, TRUE);
// 	else if (idx == 4) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, TRUE);
// 	else if (idx == 5) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, TRUE);
// 	else if (idx == 6) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, TRUE);
// 	else if (idx == 7) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, TRUE);
// 	else if (idx == 8) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, TRUE);
// 	else if (idx == 9) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, TRUE);
// #endif
}

void CIO::IO_O_FlareLightOff(__in int idx)
{
// #if SET_INSPECTOR == SYS_GHOSTFLARE
// 	if (idx == 0)
// 		return;
// 
// 	if (idx == 1) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 	else if (idx == 2) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 	else if (idx == 3) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 	else if (idx == 4) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 	else if (idx == 5) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 	else if (idx == 6) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 	else if (idx == 7) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 	else if (idx == 8) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 	else if (idx == 9) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// #endif
}

void CIO::IO_O_FlareLightAllOff()
{
// #if SET_INSPECTOR == SYS_GHOSTFLARE
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// #endif
}

// bool CIO::IO_I_CV1_IF_FrontIn()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0C_FRONT_IN_STREAM);
// }
// 
// bool CIO::IO_I_CVR_IF_FrontOut()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0E_FRONT_OUT_STREAM);
// }
// 
// bool CIO::IO_I_CV2_IF_RearIn()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0D_REAR_IN_STREAM);
// }
// 
// bool CIO::IO_I_CVR_IF_RearOut()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0F_REAR_OUT_STREAM);
// }
// 
// void CIO::IO_O_CV1_IF_FrontIn(__in bool bOn)
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_FRONT_IN_STREAM, bOn);
// }
// 
// void CIO::IO_O_CVR_IF_FrontOut(__in bool bOn)
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_OUT_STREAM, bOn);
// }
// 
// void CIO::IO_O_CV2_IF_RearIn(__in bool bOn)
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0D_REAR_IN_STREAM, bOn);
// }
// 
// void CIO::IO_O_CVR_IF_RearOut(__in bool bOn)
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0F_REAR_OUT_STREAM, bOn);
// }
// 
// // CV1 버퍼 LIFT CHECK
// bool CIO::IO_I_CV1_BufLiftCheck()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1B_BUFFER_LIFT_CHECK_SENSOR);
// }
// 
// bool CIO::IO_I_CV1_Sensor(__in enConv1SensorSelect sensor)
// {
// 	bool bRet = false;
// 	switch (sensor)
// 	{
// 	case enConv1SensorSelect::Cv1_Input:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_10_CONVEYOR1_INPUT_SENSOR);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Stop_RFID:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_02_CONVEYOR1_RFID_STOP_SENSOR);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Slow:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_11_CONVEYOR1_SLOW_SENSOR);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Stop:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_12_CONVEYOR1_STOP_SENSOR);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Out:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_09_CONVEYOR1_OUPUT_SENSOR);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_clear:
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	return bRet;
// }
// 
// //====================================================================================
// // Method		: IO_O_CV1_MoveMode
// // Access		: public 
// // Returns		: void
// // Parameter	: __in enConv1SensorSelect sensor
// // Qualifier	:
// // Last Update 	: 2021/12/21 - 17:28
// // Desc.	   	: 선택된 센서가 감지되었을때 컨베이어 동작 속도 모드가 다르게 동작한다.
// //				  드라이버 내부에 셋팅된 파라미터 값으로 동작함
// //=====================================================================================
// void CIO::IO_O_CV1_MoveMode(__in enConv1SensorSelect sensor)
// {
// 	switch (sensor)
// 	{
// 	case enConv1SensorSelect::Cv1_Input:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_CONVEYOR1_RFID_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_CONVEYOR1_INPUT_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Stop_RFID:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_CONVEYOR1_INPUT_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_CONVEYOR1_RFID_STOP_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Slow:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_CONVEYOR1_INPUT_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_CONVEYOR1_RFID_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_Stop:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_CONVEYOR1_INPUT_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_CONVEYOR1_RFID_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConv1SensorSelect::Cv1_clear:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_CONVEYOR1_INPUT_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_13_CONVEYOR1_RFID_STOP_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_11_CONVEYOR1_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_CONVEYOR1_STOP_SENSOR_ON, FALSE);
// 		break;
// 	default:
// 		break;
// 	}
// }
// 
// bool CIO::IO_I_CV2_Loading1_Check1()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_04_SOCKET_LOADING1_CHECK1);
// }
// 
// bool CIO::IO_I_CV2_Loading1_Check2()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_05_SOCKET_LOADING1_CHECK2);
// }
// 
// bool CIO::IO_I_CV2_Loading2_Check3()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_07_SOCKET_LOADING2_CHECK3);
// }
// 
// bool CIO::IO_I_CV2_Loading2_Check4()
// {
// 	return AJIN_IO->GetInputIoBit(DIO_INPUT_X0_08_SOCKET_LOADING2_CHECK4);
// }
// 
// bool CIO::IO_I_CV2_LoadingCheck(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	Sleep(500);
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		if (IO_I_CV2_Loading1_Check1() == false && IO_I_CV2_Loading1_Check2() == false)
// 			bRet = true;
// 		break;
// 	case InspectionPortType::Right:
// 		if (IO_I_CV2_Loading2_Check3() == false && IO_I_CV2_Loading2_Check3() == false)
// 			bRet = true;
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	return bRet;
// }
// 
// bool CIO::IO_I_CV2_LoadingReverseCheck(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:		
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X1_16_LOADING1_MATERIAL_REVERSE_CHECK_SENSOR);
// 		break;
// 	case InspectionPortType::Right:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X1_17_LOADING2_MATERIAL_REVERSE_CHECK_SENSOR);
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	return bRet;
// }
// 
// bool CIO::IO_I_CV2_Sensor(__in enConv2SensorSelect sensor)
// {
// 	bool bRet = false;
// 	switch (sensor)
// 	{
// 	case enConv2SensorSelect::Cv2_Input:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_13_CONVEYOR2_INPUT_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Slow1:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_14_CONVEYOR2_SLOW1_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Stop1:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_15_CONVEYOR2_STOP1_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Slow2:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_16_CONVEYOR2_SLOW2_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Stop2:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_17_CONVEYOR2_STOP2_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Out:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_0A_CONVEYOR2_OUPUT_SENSOR);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_clear:
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	return bRet;
// }
// 
// //====================================================================================
// // Method		: IO_O_CV2_MoveMode
// // Access		: public 
// // Returns		: void
// // Parameter	: __in enConv2SensorSelect sensor
// // Qualifier	:
// // Last Update 	: 2021/12/21 - 17:39
// // Desc.	   	: 선택된 센서가 감지되었을때 컨베이어 동작 속도 모드가 다르게 동작한다.
// //				  드라이버 내부에 셋팅된 파라미터 값으로 동작함
// //=====================================================================================
// void CIO::IO_O_CV2_MoveMode(__in enConv2SensorSelect sensor)
// {
// 	switch (sensor)
// 	{
// 	case enConv2SensorSelect::Cv2_Input:
// //		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// //		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// //		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// //		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Slow1:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Stop1:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Slow2:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_Stop2:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConv2SensorSelect::Cv2_clear:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_CONVEYOR2_SLOW1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_15_CONVEYOR2_STOP1_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_CONVEYOR2_SLOW2_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_17_CONVEYOR2_STOP2_SENSOR_ON, FALSE);
// 		break;
// 
// 	default:
// 		break;
// 	}
// }

// bool CIO::IO_I_Picker_SocketCheck(__in InspectionPortType port)
// {
// 	auto In = 0;
// 
// 	In = (port == InspectionPortType::Left) ? DIO_INPUT_X1_09_LOADING1_SOCKET_CHECK_SENSOR : DIO_INPUT_X1_13_LOADING2_SOCKET_CHECK_SENSOR;
// 	return AJIN_IO->GetInputIoBit(In);
// }
// 
// bool CIO::IO_I_Picker_LoadReverse(__in InspectionPortType port)
// {
// 	auto In = 0;
// 
// 	In = (port == InspectionPortType::Left) ? DIO_INPUT_X1_16_LOADING1_MATERIAL_REVERSE_CHECK_SENSOR : DIO_INPUT_X1_17_LOADING2_MATERIAL_REVERSE_CHECK_SENSOR;
// 	return AJIN_IO->GetInputIoBit(In);
// }
// 
// bool CIO::IO_I_Picker_VaccumOn(__in InspectionPortType port)
// {
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	auto In = 0;
// // 
// // 	In = (port == InspectionPortType::Left) ? DIO_INPUT_X1_08_LOADING1_VACCUM_SENSOR : DIO_INPUT_X1_12_LOADING2_VACCUM_SENSOR;
// // 	return AJIN_IO->GetInputIoBit(In);
// // #else
//  	return true;
// // #endif
// }

// bool CIO::IO_I_Picker_CL500A_JigCheck(__in InspectionPortType port)
// {
// 	bool bRet = false;
// // #if(SET_INSPECTOR == SYS_COLOR_CAL)
// // 		bRet = (port == InspectionPortType::Left) ? AJIN_IO->GetInputIoBit(DIO_INPUT_X2_00_LOADING1_JIG_CHECK) : AJIN_IO->GetInputIoBit(DIO_INPUT_X2_01_LOADING2_JIG_CHECK);
// // #else
//  		bRet = false;
// // #endif
// 
// 	return bRet;
// }

// bool CIO::IO_I_Picker_CL500A_JigErrorCheck(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// // #if(SET_INSPECTOR == SYS_COLOR_CAL)
// // 	bRet = (port == InspectionPortType::Left) ? AJIN_IO->GetInputIoBit(DIO_INPUT_X2_02_LOADING1_JIG_ERROR_CHECK) : AJIN_IO->GetInputIoBit(DIO_INPUT_X2_03_LOADING2_JIG_ERROR_CHECK);
// // #else
//  	bRet = false;
// // #endif
// 	return bRet;
// }
// 
// enMCErrCode CIO::IO_Picker_CL500A_Empty_Check()
// {
// 	enMCErrCode err = enMCErrCode::MCEC_OK;
// 
// 	if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Left))
// 	{
// 		return MCEC_IO_PICKER_CL500A_JigCheck1_Detect;
// 	}
// 
// // 	if (IO_I_Picker_CL500A_JigErrorCheck(InspectionPortType::Left))
// // 	{
// // 		return MCEC_IO_PICKER_CL500A_JigErrorCheck1_Detect;
// // 	}
// 
// 	if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Right))
// 	{
// 		return MCEC_IO_PICKER_CL500A_JigCheck2_Detect;
// 	}
// 
// // 	if (IO_I_Picker_CL500A_JigErrorCheck(InspectionPortType::Right))
// // 	{
// // 		return MCEC_IO_PICKER_CL500A_JigErrorCheck2_Detect;
// // 	}
// 
// 	return err;
// }
// 
// // PORT LOADING1/2 VACCUM 
// void CIO::IO_O_Picker_LoadVaccum(__in InspectionPortType port, __in BOOL bOn)
// {
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	auto Out = 0;
// // 	Out = (port == InspectionPortType::Left) ? DIO_OUTPUT_Y1_08_LOADING1_VACCUM_ON : DIO_OUTPUT_Y1_12_LOADING2_VACCUM_ON;
// // 	AJIN_IO->SendOutputBit(Out, bOn);
// // #endif
// }
// 
// // PORT LOADING1/2 BLOW
// void CIO::IO_O_Picker_LoadBlow(__in InspectionPortType port, __in BOOL bOn)
// {
// // #if(SET_INSPECTOR == SYS_DISTORTION) \
// //  ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
// //  ||(SET_INSPECTOR == SYS_SFR_NARROW) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
// //  ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
// // 	auto Out = 0;
// // 	Out = (port == InspectionPortType::Left) ? DIO_OUTPUT_Y1_09_LOADING1_BLOW_DOWN : DIO_OUTPUT_Y1_13_LOADING2_BLOW_DOWN;
// // 	AJIN_IO->SendOutputBit(Out, bOn);
// // #endif
// }



// bool CIO::IO_I_CVR_Sensor(__in enConvRetSensorSelect sensor)
// {
// 	bool bRet = false;
// 	switch (sensor)
// 	{
// 	case enConvRetSensorSelect::Cvr_Input:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_18_CONVEYOR3_INPUT_SENSOR);
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_Slow:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_19_CONVEYOR3_SLOW_SENSOR);
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_Stop:
// 		bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X0_1A_CONVEYOR3_STOP_SENSOR);
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_clear:
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	return bRet;
// }
// 
// void CIO::IO_O_CVR_MoveMode(__in enConvRetSensorSelect sensor)
// {
// 	switch (sensor)
// 	{
// 	case enConvRetSensorSelect::Cvr_Input:
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_Slow:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, TRUE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, FALSE);
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_Stop:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, TRUE);
// 		break;
// 
// 	case enConvRetSensorSelect::Cvr_clear:
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_19_CONVEYOR3_SLOW_SENSOR_ON, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_CONVEYOR3_STOP_SENSOR_ON, FALSE);
// 		break;
// 
// 	default:
// 		break;
// 	}
// }
// 
// // CV1 버퍼 실린더 업
// bool CIO::IO_CV1_BufCylUp()
// {
// 	auto bRet = false;
// 
// 	// hot pixel, color cal, Ghost&Flare 설비는 cv1 rfid stop 센서 감지되었을때 움직이면 안됨
// 	
// 
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_BUFFER_LIFT_CYLINDER_UP,   TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_BUFFER_LIFT_CYLINDER_DOWN, FALSE);
// 
// 	auto bOn  = DIO_INPUT_X0_1C_BUFFER_LIFT_CYLINDER_UP_SENSOR;
// 	auto bOff = DIO_INPUT_X0_1D_BUFFER_LIFT_CYLINDER_DOWN_SENSOR;
// 
// 	bRet = WaitStatus(bOn, bOff);
// 
// 	return bRet;
// }
// 
// // CV1 버퍼 실린더 다운
// bool CIO::IO_CV1_BufCylDn()
// {
// 	auto bRet = false;
// 
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1D_BUFFER_LIFT_CYLINDER_DOWN, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_BUFFER_LIFT_CYLINDER_UP, FALSE);
// 
// 	auto bOn	= DIO_INPUT_X0_1D_BUFFER_LIFT_CYLINDER_DOWN_SENSOR;
// 	auto bOff	= DIO_INPUT_X0_1C_BUFFER_LIFT_CYLINDER_UP_SENSOR;
// 	
// 	bRet = WaitStatus(bOn, bOff);
// 
// 	return bRet;
// }
// 
// // CV1 클램프 ON
// bool CIO::IO_CV1_BufClampCylOn()
// {
// 	auto bRet = false;
// 
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_BUFFER_SOCKET_CLAMP_SOL_ON, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_BUFFER_SOCKET_CLAMP_SOL_OFF, FALSE);
// 
// 	auto bOn  = DIO_INPUT_X0_1E_BUFFER_SOCKET_CLAMP_ON_SENSOR;
// 	auto bOff = DIO_INPUT_X0_1F_BUFFER_SOCKET_CLAMP_OFF_SENSOR;
// 
// 	bRet = WaitStatus(bOn, bOff);
// 	Sleep(500);
// 	return bRet;
// }
// 
// // CV1 클램프 OFF
// bool CIO::IO_CV1_BufClampCylOff()
// {
// 	auto bRet = false;
// 
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1F_BUFFER_SOCKET_CLAMP_SOL_OFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_BUFFER_SOCKET_CLAMP_SOL_ON, FALSE);
// 	
// 	auto bOn  = DIO_INPUT_X0_1F_BUFFER_SOCKET_CLAMP_OFF_SENSOR;
// 	auto bOff = DIO_INPUT_X0_1E_BUFFER_SOCKET_CLAMP_ON_SENSOR;
// 	
// 	bRet = WaitStatus(bOn, bOff);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 SOCKET 클램프 ON
// bool CIO::IO_Picker_LoadClampCylOn(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn  = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn  = DIO_OUTPUT_Y1_00_LOADING1_SOCKET_CLAMP_ON;
// 		OutOff = DIO_OUTPUT_Y1_01_LOADING1_SOCKET_CLAMP_OFF;
// 
// 		InOn  = DIO_INPUT_X1_00_LOADING1_SOCKET_CLAMP_ON_SENSOR;
// 		InOff = DIO_INPUT_X1_01_LOADING1_SOCKET_CLAMP_OFF_SENSOR;
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn  = DIO_OUTPUT_Y1_0A_LOADING2_SOCKET_CLAMP_ON;
// 		OutOff = DIO_OUTPUT_Y1_0B_LOADING2_SOCKET_CLAMP_OFF;
// 
// 		InOn  = DIO_INPUT_X1_0A_LOADING2_SOCKET_CLAMP_ON_SENSOR;
// 		InOff = DIO_INPUT_X1_0B_LOADING2_SOCKET_CLAMP_OFF_SENSOR;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	Sleep(500);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 SOCKET 클램프 OFF
// bool CIO::IO_PIcker_LoadClampCylOff(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn = DIO_OUTPUT_Y1_01_LOADING1_SOCKET_CLAMP_OFF;
// 		OutOff = DIO_OUTPUT_Y1_00_LOADING1_SOCKET_CLAMP_ON;
// 
// 		InOn = DIO_INPUT_X1_01_LOADING1_SOCKET_CLAMP_OFF_SENSOR;
// 		InOff = DIO_INPUT_X1_00_LOADING1_SOCKET_CLAMP_ON_SENSOR;
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn = DIO_OUTPUT_Y1_0B_LOADING2_SOCKET_CLAMP_OFF;
// 		OutOff = DIO_OUTPUT_Y1_0A_LOADING2_SOCKET_CLAMP_ON;
// 
// 		InOn = DIO_INPUT_X1_0B_LOADING2_SOCKET_CLAMP_OFF_SENSOR;
// 		InOff = DIO_INPUT_X1_0A_LOADING2_SOCKET_CLAMP_ON_SENSOR;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 PROBEPIN MOVE ON (H TYPE)
// // PORT LOADING1/2 PROBEPIN MOVE OFF (V TYPE)
// bool CIO::IO_Picker_LoadProbeChange(__in InspectionPortType port, __in enSocketType socket)
// {
// 	auto bRet = false;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 	{
// 		switch (socket)
// 		{
// 		case enSocketType::Socket_H:
// 		{
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_LOADING1_PROBEPIN_MOVE_ON, FALSE);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_LOADING1_PROBEPIN_MOVE_OFF, TRUE);
// 
// 			auto bOn = DIO_INPUT_X1_03_LOADING1_PROBEPIN_MOVE_OFF_SENSOR;
// 			auto bOff = DIO_INPUT_X1_02_LOADING1_PROBEPIN_MOVE_ON_SENSOR;
// 
// 			bRet = WaitStatus(bOn, bOff);
// 		}
// 		break;
// 
// 		case enSocketType::Socket_V:
// 		{
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_02_LOADING1_PROBEPIN_MOVE_ON, TRUE);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_03_LOADING1_PROBEPIN_MOVE_OFF, FALSE);
// 
// 			auto bOn = DIO_INPUT_X1_02_LOADING1_PROBEPIN_MOVE_ON_SENSOR;
// 			auto bOff = DIO_INPUT_X1_03_LOADING1_PROBEPIN_MOVE_OFF_SENSOR;
// 
// 			bRet = WaitStatus(bOn, bOff);
// 		}
// 		break;
// 
// 		default:
// 			break;
// 		}
// 	}
// 		break;
// 
// 	case InspectionPortType::Right:
// 	{
// 		switch (socket)
// 		{
// 		case enSocketType::Socket_H:
// 		{
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0C_LOADING2_PROBEPIN_MOVE_ON, FALSE);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0D_LOADING2_PROBEPIN_MOVE_OFF, TRUE);
// 
// 			auto bOn = DIO_INPUT_X1_0D_LOADING2_PROBEPIN_MOVE_OFF_SENSOR;
// 			auto bOff = DIO_INPUT_X1_0C_LOADING2_PROBEPIN_MOVE_ON_SENSOR;
// 
// 			bRet = WaitStatus(bOn, bOff);
// 		}
// 		break;
// 
// 		case enSocketType::Socket_V:
// 		{
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0C_LOADING2_PROBEPIN_MOVE_ON, TRUE);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_0D_LOADING2_PROBEPIN_MOVE_OFF, FALSE);
// 
// 			auto bOn = DIO_INPUT_X1_0C_LOADING2_PROBEPIN_MOVE_ON_SENSOR;
// 			auto bOff = DIO_INPUT_X1_0D_LOADING2_PROBEPIN_MOVE_OFF_SENSOR;
// 
// 			bRet = WaitStatus(bOn, bOff);
// 		}
// 		break;
// 
// 		default:
// 			break;
// 		}
// 	}
// 		break;
// 
// 	default:
// 		break;
// 	}	
// 	
// 	return bRet;
// }
// 
// // PORT LOADING1/2 PROBEPIN CONNECT UP
// bool CIO::IO_Picker_LoadProbeUp(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn = DIO_OUTPUT_Y1_04_LOADING1_PROBEPIN_CONNECT_UP;
// 		OutOff = DIO_OUTPUT_Y1_05_LOADING1_PROBEPIN_CONNECT_DOWN;
// 
// 		InOn = DIO_INPUT_X1_04_LOADING1_PROBEPIN_CONNECT_UP_SENSOR;
// 		InOff = DIO_INPUT_X1_05_LOADING1_PROBEPIN_CONNECT_DOWN_SENSOR;
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn = DIO_OUTPUT_Y1_0E_LOADING2_PROBEPIN_CONNECT_UP;
// 		OutOff = DIO_OUTPUT_Y1_0F_LOADING2_PROBEPIN_CONNECT_DOWN;
// 
// 		InOn = DIO_INPUT_X1_0E_LOADING2_PROBEPIN_CONNECT_UP_SENSOR;
// 		InOff = DIO_INPUT_X1_0F_LOADING2_PROBEPIN_CONNECT_DOWN_SENSOR;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 PROBE PIN CONNECT DOWN
// bool CIO::IO_Picker_LoadProbeDn(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn = DIO_OUTPUT_Y1_05_LOADING1_PROBEPIN_CONNECT_DOWN;
// 		OutOff = DIO_OUTPUT_Y1_04_LOADING1_PROBEPIN_CONNECT_UP;		
// 
// 		InOn = DIO_INPUT_X1_05_LOADING1_PROBEPIN_CONNECT_DOWN_SENSOR;
// 		InOff = DIO_INPUT_X1_04_LOADING1_PROBEPIN_CONNECT_UP_SENSOR;
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn = DIO_OUTPUT_Y1_0F_LOADING2_PROBEPIN_CONNECT_DOWN;
// 		OutOff = DIO_OUTPUT_Y1_0E_LOADING2_PROBEPIN_CONNECT_UP;
// 		
// 		InOn = DIO_INPUT_X1_0F_LOADING2_PROBEPIN_CONNECT_DOWN_SENSOR;
// 		InOff = DIO_INPUT_X1_0E_LOADING2_PROBEPIN_CONNECT_UP_SENSOR;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 SOCKET UP
// bool CIO::IO_Picker_LoadSocketUp(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn	= DIO_OUTPUT_Y1_06_LOADING1_SOCKET_UP;
// 		OutOff	= DIO_OUTPUT_Y1_07_LOADING1_SOCKET_DOWN;
// 
// 		InOn	= DIO_INPUT_X1_06_LOADING1_SOCKET_UP_SENSOR;
// 		InOff	= DIO_INPUT_X1_07_LOADING1_SOCKET_DOWN_SENSOR;
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn = DIO_OUTPUT_Y1_10_LOADING2_SOCKET_UP;
// 		OutOff = DIO_OUTPUT_Y1_11_LOADING2_SOCKET_DOWN;
// 
// 		InOn = DIO_INPUT_X1_10_LOADING2_SOCKET_UP_SENSOR;
// 		InOff = DIO_INPUT_X1_11_LOADING2_SOCKET_DOWN_SENSOR;
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	return bRet;
// }
// 
// // PORT LOADING1/2 SOCKET DOWN
// bool CIO::IO_Picker_LoadSocketDn(__in InspectionPortType port)
// {
// 	bool bRet = false;
// 
// 	auto OutOn = 0;
// 	auto OutOff = 0;
// 
// 	auto InOn = 0;
// 	auto InOff = 0;
// 
// 	switch (port)
// 	{
// 	case InspectionPortType::Left:
// 		OutOn  = DIO_OUTPUT_Y1_07_LOADING1_SOCKET_DOWN;
// 		OutOff = DIO_OUTPUT_Y1_06_LOADING1_SOCKET_UP;
// 		
// 		InOn  = DIO_INPUT_X1_07_LOADING1_SOCKET_DOWN_SENSOR;
// 		InOff = DIO_INPUT_X1_06_LOADING1_SOCKET_UP_SENSOR;		
// 		break;
// 
// 	case InspectionPortType::Right:
// 		OutOn  = DIO_OUTPUT_Y1_11_LOADING2_SOCKET_DOWN;
// 		OutOff = DIO_OUTPUT_Y1_10_LOADING2_SOCKET_UP;		
// 
// 		InOn  = DIO_INPUT_X1_11_LOADING2_SOCKET_DOWN_SENSOR;
// 		InOff = DIO_INPUT_X1_10_LOADING2_SOCKET_UP_SENSOR;		
// 		break;
// 
// 	default:
// 		break;
// 	}
// 
// 	AJIN_IO->SendOutputBit(OutOn, TRUE);
// 	AJIN_IO->SendOutputBit(OutOff, FALSE);
// 
// 	bRet = WaitStatus(InOn, InOff);
// 
// 	return bRet;
// }

// DOOR ALL LOCK
enMCErrCode CIO::IO_Door_All_Lock(__in bool bLock)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	if (IO_Door_Front1_Lock(bLock))
	{
		err = MCEC_Door_OpenFront1;
		goto TEST_END;
	}		

	if (IO_Door_Front2_Lock(bLock))
	{
		err = MCEC_Door_OpenFront2;
		goto TEST_END;
	}	
	
	
	if (IO_Door_Rear1_Lock(bLock))
	{
		err = MCEC_Door_OpenRear1;
		goto TEST_END;
	}
	
	if (IO_Door_Rear2_Lock(bLock))
	{
		err = MCEC_Door_OpenRear2;
		goto TEST_END;
	}	

TEST_END:	
	return err;
}

// DOOR FRONT1 LOCK
bool CIO::IO_Door_Front1_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_10_SIDE_LEFT_DOOR1_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_12_SIDE_LEFT_DOOR2_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_14_SIDE_LEFT_DOOR3_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_16_SIDE_RIGHT_DOOR1_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_18_SIDE_RIGHT_DOOR2_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1A_SIDE_RIGHT_DOOR3_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1C_REAR_DOOR1_INTERLOCK_ONOFF, TRUE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1E_REAR_DOOR2_INTERLOCK_ONOFF, TRUE);


	Out = DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF;
	AJIN_IO->SendOutputBit(Out, bLock);

	// door 상태 확인
// 	inLock		= DIO_INPUT_X1_18_FRONT_DOOR1_INTERLOCK_ON_OFF;
// 	InOpenClose = DIO_INPUT_X1_19_FRONT_DOOR1_INTERLOCK_OPEN_CLOSE;
	
	bRet = WaitStatus(InOpenClose, bLock);
	bRet = (bRet) ? WaitStatus(inLock, bLock) : bRet;
	return bRet;
}

// DOOR FRONT2 LOCK
bool CIO::IO_Door_Front2_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

// 	Out = DIO_OUTPUT_Y1_1D_FRONT_DOOR2_INTERLOCK_ONOFF;
// 	AJIN_IO->SendOutputBit(Out, bLock);
// 
// 	// door 상태 확인 
// 	inLock = DIO_INPUT_X1_1A_FRONT_DOOR2_INTERLOCK_ON_OFF;
// 	InOpenClose = DIO_INPUT_X1_1B_FRONT_DOOR2_INTERLOCK_OPEN_CLOSE;

	bRet = WaitStatus(InOpenClose, bLock);
	bRet = (bRet) ? WaitStatus(inLock, bLock) : bRet;
	return bRet;
}

// DOOR REAR1 LOCK
bool CIO::IO_Door_Rear1_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

// 	Out = DIO_OUTPUT_Y1_1E_REAR_DOOR1_INTERLOCK_ONOFF;
// 	AJIN_IO->SendOutputBit(Out, bLock);
// 
// 	// door 상태 확인 
// 	inLock = DIO_INPUT_X1_1C_REAR_DOOR1_INTERLOCK_ON_OFF;
// 	InOpenClose = DIO_INPUT_X1_1D_REAR_DOOR1_INTERLOCK_OPEN_CLOSE;

	bRet = WaitStatus(InOpenClose, bLock);
	bRet = (bRet) ? WaitStatus(inLock, bLock) : bRet;
	return bRet;
}

// DOOR REAR2 LOCK
bool CIO::IO_Door_Rear2_Lock(__in bool bLock)
{
	auto bRet = false;

	auto Out = 0;
	auto InOpenClose = 0;
	auto inLock = 0;

// 	Out = DIO_OUTPUT_Y1_1F_REAR_DOOR2_INTERLOCK_ONOFF;
// 	AJIN_IO->SendOutputBit(Out, bLock);
// 
// 	// door 상태 확인 
// 	inLock = DIO_INPUT_X1_1E_REAR_DOOR2_INTERLOCK_ON_OFF;
// 	InOpenClose = DIO_INPUT_X1_1F_REAR_DOOR2_INTERLOCK_OPEN_CLOSE;

	bRet = WaitStatus(InOpenClose, bLock);
	bRet = (bRet) ? WaitStatus(inLock, bLock) : bRet;
	return bRet;
}

bool CIO::IO_Light_ShutterOpen()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	
// 	IO_O_Light_ShutterOpen();
// 	bRet = WaitStatus(DIO_INPUT_X1_15_LIGHT_SHUTTER_CYLINDER_BWD, true);	
// #else
// 	bRet = true;
// #endif
	return bRet;
}

bool CIO::IO_Light_ShutterClose()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 
// 	IO_O_Light_ShutterClose();
// 	bRet = WaitStatus(DIO_OUTPUT_Y1_14_LIGHT_SHUTTER_CYLINDER_FWD_SOL, true);
// #else
// 	bRet = true;
// #endif
	return bRet;
}

bool CIO::IO_O_Light_ShutterOpen()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	auto on = 0;
// 	auto off = 0;
// 	
// 	on = DIO_OUTPUT_Y1_15_LIGHT_SHUTTER_CYLINDER_BWD_SOL;
// 	off = DIO_OUTPUT_Y1_14_LIGHT_SHUTTER_CYLINDER_FWD_SOL;
// 	AJIN_IO->SendOutputBit(on, true);
// 	AJIN_IO->SendOutputBit(off, false);
// 	bRet = true;
// #else
// 	bRet = true;
// #endif

	return bRet;
}

bool CIO::IO_O_Light_ShutterClose()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	auto on = 0;
// 	auto off = 0;
// 
// 	off = DIO_OUTPUT_Y1_15_LIGHT_SHUTTER_CYLINDER_BWD_SOL;
// 	on = DIO_OUTPUT_Y1_14_LIGHT_SHUTTER_CYLINDER_FWD_SOL;
// 	AJIN_IO->SendOutputBit(on, true);
// 	AJIN_IO->SendOutputBit(off, false);
// 	bRet = true;
// #else
// 	bRet = true;
// #endif

	return bRet;
}

bool CIO::IO_I_Light_ShutterOpen()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X1_15_LIGHT_SHUTTER_CYLINDER_BWD);
// #else
// 	bRet = true;
// #endif

	return bRet;
}

bool CIO::IO_I_Light_ShutterClose()
{
	bool bRet = false;
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	bRet = AJIN_IO->GetInputIoBit(DIO_INPUT_X1_14_LIGHT_SHUTTER_CYLINDER_FWD);
// #else
// 	bRet = false;
// #endif

	return bRet;
}

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// bool CIO::IO_LightTurnOn(__in int nIdx)
// {
// 	if (nIdx == 0)
// 	{
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// 		return true;
// 	}
// 	if (nIdx == 1) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, TRUE);
// 	else if (nIdx == 2) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, TRUE);
// 	else if (nIdx == 3) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, TRUE);
// 	else if (nIdx == 4) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, TRUE);
// 	else if (nIdx == 5) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, TRUE);
// 	else if (nIdx == 6) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, TRUE);
// 	else if (nIdx == 7) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, TRUE);
// 	else if (nIdx == 8) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, TRUE);
// 	else if (nIdx == 9) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, TRUE);
// 	return true;
// }
// #endif

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// bool CIO::IO_LightTurnOff(__in int nIdx)
// {
// 
// 	if (nIdx == 0)
// 		return true;
// 
// 	if (nIdx == 1) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 	else if (nIdx == 2) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 	else if (nIdx == 3) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 	else if (nIdx == 4) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 	else if (nIdx == 5) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 	else if (nIdx == 6) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 	else if (nIdx == 7) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 	else if (nIdx == 8) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 	else if (nIdx == 9) AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// 
// 	return true;
// }
// #endif

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// bool CIO::IO_LightTurnAllOff()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// 	return true;
// }
// #endif
bool CIO::IO_O_JIG_L_Lock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Set_Cyl_IO_Lock(DIO_OUTPUT_Y1_11_DBUL_JIG_L_CYL_DOWN, DIO_OUTPUT_Y1_10_DBUL_JIG_L_CYL_UP,
		DIO_INPUT_X1_10_DBUL_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_11_DBUL_JIG_R_CYL_DOWN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif
	return bRet;
}
bool CIO::IO_O_JIG_L_Unlock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	 	bRet = Set_Cyl_IO_Unock(DIO_OUTPUT_Y1_10_DBUL_JIG_L_CYL_UP, DIO_OUTPUT_Y1_11_DBUL_JIG_L_CYL_DOWN,
 		DIO_INPUT_X1_10_DBUL_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_11_DBUL_JIG_R_CYL_DOWN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_JIG_L_Lock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO_Lock(DIO_INPUT_X1_10_DBUL_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_11_DBUL_JIG_R_CYL_DOWN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_JIG_L_Unlock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO_Unlock(DIO_INPUT_X1_10_DBUL_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_11_DBUL_JIG_R_CYL_DOWN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_O_JIG_R_Lock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		bRet = Set_Cyl_IO_Lock(DIO_OUTPUT_Y1_17_DBUR_JIG_R_CYL_DOWN, DIO_OUTPUT_Y1_16_DBUR_JIG_R_CYL_UP,
 		DIO_INPUT_X1_16_DBUR_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_17_DBUR_JIG_R_CYL_DOWN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif
	return bRet;
}
bool CIO::IO_O_JIG_R_Unlock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	 	bRet = Set_Cyl_IO_Unock(DIO_OUTPUT_Y1_16_DBUR_JIG_R_CYL_UP, DIO_OUTPUT_Y1_17_DBUR_JIG_R_CYL_DOWN,
 		DIO_INPUT_X1_16_DBUR_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_17_DBUR_JIG_R_CYL_DOWN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_JIG_R_Lock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO_Lock(DIO_INPUT_X1_16_DBUR_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_17_DBUR_JIG_R_CYL_DOWN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_JIG_R_Unlock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO_Unlock(DIO_INPUT_X1_16_DBUR_JIG_L_CYL_DOWN_CHK_SENSOR, DIO_INPUT_X1_17_DBUR_JIG_R_CYL_DOWN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}



bool CIO::IO_O_Socket_Lock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = Set_Cyl_IO_Lock(DIO_OUTPUT_Y1_03_STAU_CYL_UP, DIO_OUTPUT_Y1_04_STAU_CYL_DOWN, 
		DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR, DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR, 5000, bChk);
#else
	bRet = true;
#endif
	return bRet;
}
bool CIO::IO_O_Socket_Unlock(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = Set_Cyl_IO_Unock(DIO_OUTPUT_Y1_04_STAU_CYL_DOWN , DIO_OUTPUT_Y1_03_STAU_CYL_UP,
		DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR, DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR, 5000 , bChk);
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_Socket_Lock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = Check_Cyl_IO_Lock(DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR, DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR);
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_Socket_Unlock()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = Check_Cyl_IO_Unlock(DIO_INPUT_X1_03_STAU_L_CYL_IN_CHK_SENSOR, DIO_INPUT_X1_06_STAU_R_CYL_IN_CHK_SENSOR);
#else
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_O_Chuck_Fwd(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	 	bRet = Set_Cyl_IO(DIO_OUTPUT_Y1_09_STAU_CHUCK_FWD,
 		DIO_OUTPUT_Y1_0A_STAU_CHUCK_BWD,
 		DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR,
 		DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Chuck_Bwd(BOOL bChk)
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	 	bRet = Set_Cyl_IO(DIO_OUTPUT_Y1_0A_STAU_CHUCK_BWD,
 		DIO_OUTPUT_Y1_09_STAU_CHUCK_FWD,
 		DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR,
 		DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR, 5000, bChk);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_Chuck_Fwd()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO(DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR, DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_I_Chuck_Bwd()
{
	bool bRet = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	bRet = Check_Cyl_IO(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR, DIO_INPUT_X1_09_STAU_CHUCK_FWD_IN_CHK_SENSOR);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_O_Screw_Bolt_Unlock(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	 startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);

			if (bDbul == TRUE &&
				bDbur == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	DoEvents(200);
	//	ACT
	 AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	 AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	 DoEvents(100);
	 startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbul == FALSE &&
				bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE &&
		AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	if (bRet == FALSE)
	{
		return bRet;
	}
//살짝 잠궈라 
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);

	bDbul = FALSE;
	bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbul == TRUE &&
				bDbur == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(100);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbul == FALSE &&
				bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE &&
		AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	//살짝 잠궈라 

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Screw_Bolt_Lock_1(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbul == TRUE &&
				bDbur == TRUE)
			{
				bRet = TRUE;
		
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(1000);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{

			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbul == FALSE &&
				bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(500);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE &&
		AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Screw_Bolt_Lock_2(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbul == TRUE &&
				bDbur == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(1000);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbul == FALSE &&
				bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(500);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE &&
		AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}


bool CIO::IO_O_Screw_Bolt_L_Unlock(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);


	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);


			if (bDbul == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
			
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);

		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);

	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

	DoEvents(100);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);

			if (bDbul == FALSE )
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);

				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE )
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

	if (bRet == FALSE)
	{
		return bRet;
	}
	//살짝 잠궈라 
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);


	bDbul = FALSE;
	bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);

			if (bDbul == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);

		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);

	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

	DoEvents(100);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);

			if (bDbul == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);

				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

	//살짝 잠궈라 

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Screw_Bolt_L_Lock_1(BOOL bChk)
{
bool bRet = false;
clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 

AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
DoEvents(100);
AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
DoEvents(200);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);

AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);

BOOL bDbul = FALSE;
BOOL bDbur = FALSE;
startm = clock();
if (TRUE)
{
	do
	{
		bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);

		if (bDbul == TRUE)
		{
			bRet = TRUE;

			AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
			break;
		}

		DoEvents(10);
	} while ((clock() - startm) < 10000);
}
if (bRet == FALSE)
{
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);


	return bRet;
}
bRet = FALSE;
AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);

DoEvents(200);
//	ACT
AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);

DoEvents(1000);
startm = clock();
//	CHK
if (TRUE)
{
	do
	{

		bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);

		if (bDbul == FALSE)
		{
			bRet = TRUE;
			AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
			AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
			break;
		}

		DoEvents(10);
	} while ((clock() - startm) < 10000);
}
if (bRet == FALSE)
{

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	return bRet;
}
bRet = FALSE;
DoEvents(500);
if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE)
{
	bRet = TRUE;
}
else
{
	bRet = FALSE;
}
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
bRet = true;
#else
bRet = true;
#endif

return bRet;

}
bool CIO::IO_O_Screw_Bolt_L_Lock_2(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_03_DBUL_DRIVER_L_3);
			if (bDbul == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
				break;
			}

			DoEvents(10);
} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(1000);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbul = AJIN_IO->IsON(DIO_INPUT_X2_00_DBUL_DRIVER_L_0);
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbul == FALSE &&
				bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(500);
	if (AJIN_IO->IsON(DIO_INPUT_X2_01_DBUL_DRIVER_L_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_00_DBUL_DRIVER_L_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}

bool CIO::IO_O_Screw_Bolt_R_Unlock(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
		
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);

			if (bDbur == TRUE)
			{
				bRet = TRUE;
			
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		return bRet;
	}
	bRet = FALSE;

	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	DoEvents(200);
	//	ACT

	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(100);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbur == FALSE)
			{
				bRet = TRUE;
		
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
	
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	if (bRet == FALSE)
	{
		return bRet;
	}
	//살짝 잠궈라 

	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);

	bDbul = FALSE;
	bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbur == TRUE)
			{
				bRet = TRUE;
			
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(100);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(100);
	if (AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	//살짝 잠궈라 

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Screw_Bolt_R_Lock_1(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	DoEvents(200);
	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbur == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
			} while ((clock() - startm) < 10000);
		}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

		return bRet;
	}
	bRet = FALSE;
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	DoEvents(200);
	//	ACT
	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(1000);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{

			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbur == FALSE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(500);
	if (AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif

	return bRet;
}
bool CIO::IO_O_Screw_Bolt_R_Lock_2(BOOL bChk)
{
	bool bRet = false;
	clock_t startm = clock();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_01_DBUL_DRIVER_L_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_02_DBUL_DRIVER_L_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_03_DBUL_DRIVER_L_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_04_DBUL_DRIVER_L_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_05_DBUL_DRIVER_L_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_06_DBUL_DRIVER_L_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_07_DBUL_DRIVER_L_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_08_DBUL_DRIVER_L_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_09_DBUL_DRIVER_L_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0A_DBUL_DRIVER_L_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0B_DBUL_DRIVER_L_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_0C_DBUL_DRIVER_L_12);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_12_DBUR_DRIVER_R_2);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_15_DBUR_DRIVER_R_5);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_16_DBUR_DRIVER_R_6);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_17_DBUR_DRIVER_R_7);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_18_DBUR_DRIVER_R_8);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_19_DBUR_DRIVER_R_9);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1A_DBUR_DRIVER_R_10);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1B_DBUR_DRIVER_R_11);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_1C_DBUR_DRIVER_R_12);
	DoEvents(100);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	DoEvents(200);
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);

	BOOL bDbul = FALSE;
	BOOL bDbur = FALSE;
	startm = clock();
	if (TRUE)
	{
		do
		{
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_13_DBUR_DRIVER_R_3);
			if (bDbur == TRUE)
			{
				bRet = TRUE;
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_11_DBUR_DRIVER_R_1);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		return bRet;
	}
	bRet = FALSE;

	AJIN_IO->SetON(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

	AJIN_IO->SetON(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
	DoEvents(200);
	//	ACT

	AJIN_IO->SetON(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
	DoEvents(1000);
	startm = clock();
	//	CHK
	if (TRUE)
	{
		do
		{
		
			bDbur = AJIN_IO->IsON(DIO_INPUT_X2_10_DBUR_DRIVER_R_0);
			if (bDbur == FALSE)
			{
				bRet = TRUE;
				
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
			
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
				
				AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < 10000);
	}
	if (bRet == FALSE)
	{
		
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);
		
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);
		
		AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
		return bRet;
	}
	bRet = FALSE;
	DoEvents(500);
	if (AJIN_IO->IsON(DIO_INPUT_X2_11_DBUR_DRIVER_R_1) == TRUE)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_13_DBUR_DRIVER_R_3);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_14_DBUR_DRIVER_R_4);

	AJIN_IO->SetOFF(DIO_OUTPUT_Y2_10_DBUR_DRIVER_R_0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	bRet = true;
#else
	bRet = true;
#endif
	

	return bRet;
}





bool CIO::Check_Cyl_IO(int I_IO1, int I_IO2)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsON(I_IO1);
	if (I_IO2 != -1) bRet2 = AJIN_IO->IsOFF(I_IO2);

	if (bRet1  &&
		bRet2)
		return TRUE;

	return FALSE;
}
bool CIO::Check_Cyl_IO_Lock(int I_IO1, int I_IO2)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsON(I_IO1);
	if (I_IO2 != -1) bRet2 = AJIN_IO->IsON(I_IO2);

	if (bRet1  &&
		bRet2)
		return TRUE;

	return FALSE;
}
bool CIO::Check_Cyl_IO_Unlock(int I_IO1, int I_IO2)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsOFF(I_IO1);
	if (I_IO2 != -1) bRet2 = AJIN_IO->IsOFF(I_IO2);

	if (bRet1  &&
		bRet2)
		return TRUE;

	return FALSE;
}

bool CIO::Check_Cyl_Screw_Bolt_IO(int I_IO1, int I_IO2)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsON(I_IO1);
	if (I_IO2 != -1) bRet2 = AJIN_IO->IsON(I_IO2);

	if (bRet1  &&
		bRet2)
		return TRUE;

	return FALSE;
}

bool CIO::Check_Cyl_Screw_L_Bolt_IO(int I_IO1)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsON(I_IO1);
	if (bRet1)
		return TRUE;

	return FALSE;
}
bool CIO::Check_Cyl_Screw_R_Bolt_IO(int I_IO1)
{
	BOOL bRet1 = TRUE, bRet2 = TRUE;

	if (I_IO1 != -1) bRet1 = AJIN_IO->IsON(I_IO1);
	if (bRet1)
		return TRUE;

	return FALSE;
}







bool CIO::Set_Cyl_IO(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1) AJIN_IO->SetON(O_IO1);
	if (O_IO2 != -1) AJIN_IO->SetOFF(O_IO2);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_IO(I_IO1, I_IO2) == TRUE)
			{
				bRet = true;
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	return bRet;
}
bool CIO::Set_Cyl_Screw_Bolt_IO(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1)  AJIN_IO->SetON(O_IO1);
	if (O_IO2 != -1)  AJIN_IO->SetON(O_IO2);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_Screw_Bolt_IO(I_IO1, I_IO2) == TRUE)
			{
				bRet = true;
				AJIN_IO->SetOFF(O_IO1);
				AJIN_IO->SetOFF(O_IO2);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	AJIN_IO->SetOFF(O_IO1);
	AJIN_IO->SetOFF(O_IO2);
	return bRet;
}

bool CIO::Set_Cyl_Screw_L_Bolt_IO(int O_IO1, int I_IO1, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1)  AJIN_IO->SetON(O_IO1);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_Screw_L_Bolt_IO(I_IO1) == TRUE)
			{
				bRet = true;
				AJIN_IO->SetOFF(O_IO1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	AJIN_IO->SetOFF(O_IO1);
	return bRet;
}

bool CIO::Set_Cyl_Screw_R_Bolt_IO(int O_IO1, int I_IO1, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1)  AJIN_IO->SetON(O_IO1);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_Screw_R_Bolt_IO(I_IO1) == TRUE)
			{
				bRet = true;
				AJIN_IO->SetOFF(O_IO1);
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	AJIN_IO->SetOFF(O_IO1);
	return bRet;
}



bool CIO::Set_Cyl_IO_Lock(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1) AJIN_IO->SetON(O_IO1);
	if (O_IO2 != -1) AJIN_IO->SetOFF(O_IO2);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_IO_Lock(I_IO1, I_IO2) == TRUE)
			{
				bRet = true;
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	return bRet;
}
bool CIO::Set_Cyl_IO_Unock(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk)
{
	//  TIME
	bool bRet = false;
	clock_t startm = clock();

	//	ACT
	if (O_IO1 != -1) AJIN_IO->SetON(O_IO1);
	if (O_IO2 != -1) AJIN_IO->SetOFF(O_IO2);

	//	CHK
	if (bChk)
	{
		do
		{
			if (Check_Cyl_IO_Unlock(I_IO1, I_IO2) == TRUE)
			{
				bRet = true;
				break;
			}

			DoEvents(10);
		} while ((clock() - startm) < dwMilisecond);
	}
	else
	{
		bRet = true;
	}
	return bRet;
}

// int CIO::Set_Cyl_IO_SHUTTER(int O_IO1, int On, int I_IO1, int I_IO2, int I_IO3, int I_IO4, int nWaitTime, BOOL bChk)
// {
// 	//  TIME
// 	CUCheckTime CheckTime;
// 	CheckTime.SetCheckTime(nWaitTime);
// 	CheckTime.StartTime();
// 
// 	//	ACT
// 	if (On == TRUE)
// 	{
// 		if (O_IO1 != O_VIRTUAL) g_HWMan.SetON(O_IO1);
// 	}
// 	else
// 	{
// 		if (O_IO1 != O_VIRTUAL) g_HWMan.SetOFF(O_IO1);
// 	}
// 
// 	//	CHK
// 	while (bChk) {
// 		Sleep(10);
// 
// 		if (Check_Cyl_IO(I_IO1, I_IO2) == SEQ_OK && Check_Cyl_IO(I_IO3, I_IO4) == SEQ_OK) {
// 			break;
// 		}
// 
// 		if (CheckTime.IsTimeOver()) {
// 			//	ALL OFF
// 			g_HWMan.SetOFF(O_IO1);
// 			return 1;
// 		}
// 	}
// 
// 	// DELAY
// 	//	int nDelay =0;
// 	//	if(I_IO1 == IN_STA1_PICK_ALIGN_FWD)
// 	//		nDelay = g_Data.m_Profile.paramSta1.paPick.nProbeCloseDelay;
// 	//	else if(I_IO1 == IN_STA1_PICK_ALIGN_BWD)
// 	//		nDelay = g_Data.m_Profile.paramSta1.paPick.nProbeOpenDelay;
// 
// 	//	if(nDelayTime >=0) 
// 	//		Sleep(nDelayTime) ;
// 	return 0;
// }


