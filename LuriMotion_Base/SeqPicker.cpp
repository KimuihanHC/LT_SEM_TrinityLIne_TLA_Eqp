#include "pch.h"
#include "SeqPicker.h"
#include "LuriMotion_BaseDlg.h"

#include "Util/BasicTimer.h"

CSeqPicker::CSeqPicker()
{
	m_bRecvEject = false;
	m_nPortId = InspectionPortType::Left;
//	SetNextStep(SEQ_PICKER_STEP_READY);	
	Reset();
}

CSeqPicker::~CSeqPicker()
{
}

void CSeqPicker::Reset()
{
// 	std::list<ST_SocketInfo>::iterator it = m_list.begin();
// 	while (m_list.size() > 0)
// 	{
// 		it = m_list.begin();
// 		m_list.erase(it);
// 	}

	m_bSocketEmptyInput = true;
	m_SocketInput.Reset();

	__super::Reset();
}

UINT CSeqPicker::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	while (IsThreadStart())
	{
		Sleep(10);
		
		if (IsThreadStop())
		{
	//		Log(_T("Picker [%d] =========> Thread Stop"), GetPortId());
			continue;
		}

		if (IsError())
		{
	//		Log(_T("Picker [%d] =========> Error : %d"), GetPortId(), GetErrorCode());
			continue;
		}

	//	Wait();

		if (IsChangeStep())
		{
			Log(_T("Picker [%d] =========> %s(start)"), GetPortId(), g_szSeqName_Picker[GetStep()]);
			
			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();

			Log(_T("Picker [%d] =========> %s(end),ResultCode:%d,Elaped : %d ms"), GetPortId(), g_szSeqName_Picker[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
		}
		
		if (err != enMCErrCode::MCEC_OK)
		{
			Error(err);
			SetStatus(enSeqStatus::Status_Error);
		}
		else
		{
			SetStatus(enSeqStatus::Status_Complete);
		}
		
	//	Lock();
	}

	Log(_T("Picker [%d] =========> Thread Exit"), GetPortId());
	return 0;
}

enMCErrCode CSeqPicker::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	enSocketType socket = enSocketType::Socket_V;

	switch ((enSeqPickerStep)GetStep())
	{
	case enSeqPickerStep::SEQ_PICKER_STEP_READY:	// 준비
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_START:
		// Z 축 안전 위치로 이동한다.
		// 실린더 모두 UP 등 시퀀스 수행
	//	Sleep(1000);
//		SetNextStep(SEQ_PICKER_STEP_INIT_CHECK_SOCKET);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_CHECK_SOCKET:
	{
#ifdef DevMode_Simulator

#else
	//	err = IO_I_Picker_SocketCheck(m_nPortId) ? MCEC_IO_PICKER_LoadSocket_Loading_Detect : err;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_TRAY_UP);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_TRAY_UP:
	{
#ifdef DevMode_Simulator
		
#else
		//err = IO_Picker_LoadSocketUp(m_nPortId) ? err : MCEC_IO_PICKER_LoadSocketTrayUp_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_PROBE_DN);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_PROBE_DN:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_Picker_LoadProbeDn(m_nPortId) ? err : MCEC_IO_PICKER_LoadProbeDn_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_MOVE_Z_TO_SAFETY_POS);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_MOVE_Z_TO_SAFETY_POS:
	{
#ifdef DevMode_Simulator

#else
//		err = MO_MoveZ_To_Safety(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_END);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_END:
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_START:
//		SetNextStep(SEQ_PICKER_STEP_STANDBY_CHECK_SOCKET_ON_CONVEYOR);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_CHECK_SOCKET_ON_CONVEYOR:
	{
#ifdef DevMode_Simulator

#else
		switch (GetPortId())
		{
		case InspectionPortType::Left:
		{
// 			err = (IO_I_CV2_Loading1_Check1() == false) ? err : MCEC_IO_PICKER_LoadSocket_TiltCheck1_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Loading1_Check2() == false) ? err : MCEC_IO_PICKER_LoadSocket_TiltCheck2_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow1) == false) ? err : MCEC_IO_CV2_Slow1_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop1) == false) ? err : MCEC_IO_CV2_Stop1_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
		}
		break;
		case InspectionPortType::Right:
		{
// 			err = (IO_I_CV2_Loading2_Check3() == false) ? err : MCEC_IO_PICKER_LoadSocket_TiltCheck1_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Loading2_Check4() == false) ? err : MCEC_IO_PICKER_LoadSocket_TiltCheck2_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow2) == false) ? err : MCEC_IO_CV2_Slow2_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop2) == false) ? err : MCEC_IO_CV2_Stop2_Detect;
// 			if (err != enMCErrCode::MCEC_OK) break;
		}
		break;
		default:
			break;
	}
#endif
//		SetNextStep(SEQ_PICKER_STEP_STANDBY_MOVE_Z_TO_WAIT_SOCKET);
	}
		break;

	// Z 컨베이어 대기 위치로 이동	
	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_MOVE_Z_TO_WAIT_SOCKET:
	{
		
#ifdef DevMode_Simulator

#else
//		err = MO_MoveZ_To_ConveyorWait(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_STANDBY_TRAY_CYLINDER_DN);
	}		
		break;
		
	// TRAY CYLINDER DOWN
	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_TRAY_CYLINDER_DN:
	{
#ifdef DevMode_Simulator

#else
	//	err = IO_Picker_LoadSocketDn(m_nPortId) ? err : MCEC_IO_PICKER_LoadSocketTrayDn_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_STANDBY_PROBE_DN);
	}
		
		break;

	// PROBE DOWN
	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_PROBE_DN:
	{
#ifdef DevMode_Simulator

#else
	//	err = IO_Picker_LoadProbeDn(m_nPortId) ? err : MCEC_IO_PICKER_LoadProbeDn_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_STANDBY_END);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_STANDBY_END:
	{
#ifdef DevMode_Simulator

#else
		
#endif
//		SetNextStep(SEQ_PICKER_STEP_COMPLETE);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_CHECK_STATUS:
		break;
	
	case enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET:
	{
		auto port = PortType::Left;

		if (m_nPortId == InspectionPortType::Left)
			port = PortType::Left;
		else
			port = PortType::Right;

		if (ICS_SendPortStatus(port, PortStatus::Empty, _T(""), _T("")) == false)
			err = MCEC_ICS_PICKER_SendPortStatusAck_Timeout;		
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_START:

#ifdef DevMode_Simulator

#else
		SEMCO_SendReadyStart(m_nPortId);
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_MOVE_Z_GRIP_UP);
		break;
	
	// Z 트레이받는 위치로 UP
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_MOVE_Z_GRIP_UP:
	{
		ST_SocketInfo socket;
		if (m_bSocketEmptyInput)
		{
			if (ICS_SendPortStatus(m_PortType, PortStatus::Loaded, _T(""), _T("")) == false)
			{
				err = MCEC_ICS_PICKER_SendPortStatusAck_Timeout;
				break;
			}
		}
		else
		{
		//	socket = m_list.front();
			socket = m_SocketInput;

			if (ICS_SendPortStatus(m_PortType, PortStatus::Loaded, socket.szRFID, socket.szBCR) == false)
			{
				err = MCEC_ICS_PICKER_SendPortStatusAck_Timeout;
				break;
			}
		}
		
#ifdef DevMode_Simulator

#else
//		err = MO_MoveZ_To_GripUp(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_CHECK_SOCKET);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_CHECK_SOCKET:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else	
		{
// 			if (IO_I_Picker_SocketCheck(m_nPortId) == false)
// 				err = MCEC_IO_PICKER_LoadSocket_Check_Socket_Empty;
// 			if (err != enMCErrCode::MCEC_OK) break;
		}		
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_CHECK_TILT);
	}
	break;

	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_CHECK_TILT:
	{		
#ifdef DevMode_Simulator

#else
// 		if (IO_I_CV2_LoadingCheck(m_nPortId) == false)
// 		{
// 			if(m_nPortId == InspectionPortType::Left)
// 				err = MCEC_IO_PICKER_LoadSocket_TiltCheck1_Detect;
// 			else
// 				err = MCEC_IO_PICKER_LoadSocket_TiltCheck2_Detect;
// 		}

		if (err != enMCErrCode::MCEC_OK) break;
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_GRIP_OFF);
	}
	break;

	// GRIP OFF
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_GRIP_OFF:
	{
#ifdef DevMode_Simulator

#else
// 		err = IO_PIcker_LoadClampCylOff(m_nPortId) ? err : MCEC_IO_PICKER_LoadClampGripOff_Timeout;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_GRIP_ON);
	}		
		break;

	// GRIP ON
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_GRIP_ON:
	{
#ifdef DevMode_Simulator

#else
// 		err = IO_Picker_LoadClampCylOn(m_nPortId) ? err : MCEC_IO_PICKER_LoadClampGripOn_Timeout;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_REVERSE_CHECK);
	}		
		break;

	// REVERSE CHECK (빠렛트 역삽체크)
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_REVERSE_CHECK:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				//err = (IO_I_Picker_LoadReverse(m_nPortId) == false) ? MCEC_OK : MCEC_IO_PICKER_LoadSocket_Reverse_Detect;

				if (err == enMCErrCode::MCEC_OK)
					break;

				Sleep(100);
			}
			if (err != enMCErrCode::MCEC_OK) break;
		}		
#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_GRIP_OFF_RE);
	}		
		break;

	// GRIP OFF
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_GRIP_OFF_RE:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_PIcker_LoadClampCylOff(m_nPortId) ? err : MCEC_IO_PICKER_LoadClampGripOff_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_TRAY_UP);
	}		
		break;

	// TRAY UP
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_TRAY_UP:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_Picker_LoadSocketUp(m_nPortId) ? err : MCEC_IO_PICKER_LoadSocketTrayUp_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_CHANGE_PROBE_H_V_TYPE);
	}		
		break;

	// H / V 타입 설정
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_CHANGE_PROBE_H_V_TYPE:
	{
#ifdef DevMode_Simulator

#else
//	#if SET_INSPECTOR == SYS_DISTORTION
//		EqpTypeGetOption();

		auto * st = DATAMANAGER->GetSemiAuto();
		enSocketType type = st->Socket;
	//	err = IO_Picker_LoadProbeChange(GetPortId(), type) ? err : MCEC_IO_PICKER_LoadSocketTypeV_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
// 	#else 
// 		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
// 		enSocketType type = (stTeach.nSampleModelType == 1) ? Socket_H : Socket_V;
// 		err = IO_Picker_LoadProbeChange(m_nPortId, type) ? err : MCEC_IO_PICKER_LoadSocketTypeV_Timeout;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 	#endif
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_PROBE_UP);
	}
		break;

	// PROBE UP
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_PROBE_UP:
	{
#ifdef DevMode_Simulator

#else
// 		for (int i = 0; i < 3; i++)
// 		{
// 			err = IO_Picker_LoadProbeUp(m_nPortId) ? err : MCEC_IO_PICKER_LoadProbeUp_Timeout;
// 
// 			if (err == enMCErrCode::MCEC_OK)
// 				break;
// 				
// 			IO_Picker_LoadProbeDn(m_nPortId); 
// 		}
		
		if (err != enMCErrCode::MCEC_OK) break;
#endif

//		SetNextStep(SEQ_PICKER_STEP_LOAD_VACCUMON);
	}
		break;

	// VACCUM ON
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_VACCUMON:
	{
#ifdef DevMode_Simulator

#else
//		IO_O_Picker_LoadVaccum(m_nPortId, true);

#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_CHECK_VACCUM_ON);
	}
	
		break;

	// VACCUM CHECK
	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_CHECK_VACCUM_ON:
	{
#ifdef DevMode_Simulator

#else
// 		Sleep(300);
// 		err = IO_I_Picker_VaccumOn(GetPortId()) ? err : MCEC_IO_PICKER_LoadSocket_Vaccum_Detect;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_LOAD_MOVE_TO_SAFETY_POS);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_MOVE_TO_SAFETY_POS:
	{
#ifdef DevMode_Simulator

#else
//		err = MO_MoveZ_To_Safety(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;
#endif
	}
//		SetNextStep(SEQ_PICKER_STEP_LOAD_END);
//		WRITE_MAIN_LOG(_T("Z end"));
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_LOAD_END:
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_START:
		EqpType_Initialize();

//		SetNextStep(SEQ_PICKER_STEP_INIT_INSPECTION_VOLT_ON);
		break;

	// POWER SUPPLY ON
	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_VOLT_ON:
	{
#ifdef DevMode_Simulator

#else
		POWERSUPPLY_VoltOn(GetPortId());

		Sleep(500);
#endif		
//		SetNextStep(SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_INITIALIZE);
	}
		break;

	// SEND INIT TO SEMCO
	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_INITIALIZE:
	{
#ifdef DevMode_Simulator

#else
		SEMCO_Reset(GetPortId());
		SEMCO_SendReadyStart(GetPortId());
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_BARCODE);
	}		
		break;

		// SEND INIT TO SEMCO
	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_BARCODE:
	{
#ifdef DevMode_Simulator

#else
		
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			ST_SocketInfo socket;
			GetSocketInfo(socket);
			SEMCO_SendBarcode(GetPortId(), socket.szBCR);

			// 받은 바코드와 동일한지 확인 
			CString szRecvBCR;
			SEMCO_WaitBarcodeAck(m_nPortId, 2000, szRecvBCR);

			if (socket.szBCR != szRecvBCR)
			{
				CString sz;
				sz.Format(_T("Send BCR : %s, Recv BCR : %s"), socket.szBCR, szRecvBCR);
			}
		}
#endif
//		SetNextStep(SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_PREVIEWSTART);
	}
	break;

	// SEND PREVIEW START TO SEMCO
	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_SEMCO_PREVIEWSTART:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			Sleep(500);
			if (SEMCO_SendPreviewStart(GetPortId()) == false)
			{
				break;
			}
		}
#endif

//		SetNextStep(SEQ_PICKER_STEP_INIT_INSPECTION_END);
	}		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INIT_INSPECTION_END:
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_START:
//		SetNextStep(SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS:
	{
#ifdef DevMode_Simulator

#else

#endif
//		SetNextStep(SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_END);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_SAFETY_POS_END:
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_START:
	{		
#ifdef DevMode_Simulator

#else
	
#endif

//		SetNextStep(SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_INSPECTION_POS);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_INSPECTION_POS:
	{
#ifdef DevMode_Simulator

#else
		err = EqpType_MoveZInspectionPos(m_nPortId);
		if (err != enMCErrCode::MCEC_OK)break;
#endif
		
// #if SET_INSPECTOR == SYS_HOTPIXEL
// //		SetNextStep(SEQ_PICKER_STEP_INSPECTION_CL200A_MEASURE);
// #else
// 		SetNextStep(SEQ_PICKER_STEP_INSPECTION_TEST);
// #endif
	}
	break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_CL200A_MEASURE:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			bool bReadOk = false;

			float fData = 0.0f;
			if (!CL200A_HOTPIXEL_ReadData(fData))
			{
				Log(_T("Error CL200A MEASURE"));
				bReadOk = false;
			}
			else
			{
				CString sz;
				if (m_nPortId == InspectionPortType::Left)
					Log(_T("(L) CL200A Measure : %f"), fData);
				else
					Log(_T("(R) CL200A Measure : %f"), fData);

				bReadOk = true;
			}

			// Ui Update
			UpdateUI_CL200A_Data(m_nPortId, fData, bReadOk);
		}
#endif
	}
//		SetNextStep(SEQ_PICKER_STEP_INSPECTION_TEST);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_TEST:
	{
		SetSocketStatus(Testing);

#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			SetTestResult(0);
		}
		else
		{
			err = StartOperation_Inspection(m_nPortId);
		}
#endif
		// SFR 설비(협각, 광각 H,V) 는 PREWVIEW STOP, POWER OFF 해주자..
// 		auto EqpType = SET_INSPECTOR;
// 
// 		switch (EqpType)
// 		{
// 		case SYS_SFR_NARROW:
// 		case SYS_SFR_WIDE_H:
// 		case SYS_SFR_WIDE_V:
// 		case SYS_HOTPIXEL:
// 		case SYS_GHOSTFLARE:
// 			SetNextStep(SEQ_PICKER_STEP_INSPECTION_SEMCO_PREVIEW_STOP);
// 			break;
// 
// 		default:
// 			// 설비별 달라지는 검사 커맨드를 날려주자...
//			SetNextStep(SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_SAFETY_POS);
			break;
//		}
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_SEMCO_PREVIEW_STOP:
	{
#ifdef DevMode_Simulator

#else
		SEMCO_SendPreviewStop(m_nPortId);
#endif
//		SetNextStep(SEQ_PICKER_STEP_INSPECTION_POWER_OFF);
	}
	break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_POWER_OFF:
	{
#ifdef DevMode_Simulator

#else
		POWERSUPPLY_VoltOff(m_nPortId);
#endif

//		SetNextStep(SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_SAFETY_POS);
	}
	break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_MOVE_Z_TO_SAFETY_POS:
	{
#ifdef DevMode_Simulator

#else
//		err = MO_MoveZ_To_Safety(m_nPortId);
		if (err != enMCErrCode::MCEC_OK)break;
#endif
	}
//		SetNextStep(SEQ_PICKER_STEP_INSPECTION_END);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_END:		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_START:
	{
#ifdef DevMode_Simulator

#else
		
#endif		
//		SetNextStep(SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS);
	}
		
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS:
	{
#ifdef DevMode_Simulator

#else
		// 검사 위치로 움직이자
//		err = MO_MoveZ_To_Inspection1(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;

#endif	
//		SetNextStep(SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_END);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_MOVE_TO_INSPECTION_POS_END:
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_START:
		SetSocketStatus(Done);
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_VACCUM_OFF);
		break;

	// VACCUM OFF
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_VACCUM_OFF:
	{
#ifdef DevMode_Simulator

#else
		//IO_O_Picker_LoadVaccum(m_nPortId, false);

#endif	
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_GRIP_OFF);
	}
		break;

	// GRIP OFF
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_GRIP_OFF:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_PIcker_LoadClampCylOff(m_nPortId) ? err : MCEC_IO_PICKER_LoadClampGripOff_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif	
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_MOVE_Z_TO_WAIT_SOCKET);
	}		
		break;

	// Z 컨베이어 대기 위치로 이동
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_MOVE_Z_TO_WAIT_SOCKET:
	{
#ifdef DevMode_Simulator

#else
	//	err = MO_MoveZ_To_ConveyorWait(m_nPortId);
		if (err != enMCErrCode::MCEC_OK) break;

#endif	
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_TRAY_CYLINDER_DN);
	}		
		break;

	// TRAY CYLINDER DOWN
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_TRAY_CYLINDER_DN:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_Picker_LoadSocketDn(m_nPortId) ? err : MCEC_IO_PICKER_LoadSocketTrayDn_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif	
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_PROBE_DN);
	}
		break;

	// PROBE DOWN
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_PROBE_DN:
	{
#ifdef DevMode_Simulator

#else
		//err = IO_Picker_LoadProbeDn(m_nPortId) ? err : MCEC_IO_PICKER_LoadProbeDn_Timeout;
		if (err != enMCErrCode::MCEC_OK) break;
#endif
//		SetNextStep(SEQ_PICKER_STEP_UNLOAD_END);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_END:
//		SetNextStep(SEQ_PICKER_STEP_COMPLETE);
		break;
		
	case enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_REQEUST:
	{
		//		Sleep(3000);
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(1000);
		}
		else
		{
			Sleep(50);

			if (IsInputSocket())
 			{
 				ST_SocketInfo socket = m_SocketInput;
 
 				m_bRecvEject = false;

				if (ICS_SendTestResult(socket.szRFID, socket.nSEMCO_Code, GetPortId()))
				{

				}

				CString sz;
				sz.Format(_T("Send TestResult of SEMCO To ICS [%d] "), socket.nSEMCO_Code);
 
 				// 배출 허락이 올때까지 대기....
 				if (ICS_IsUse())
 				{
 					if (ICS_SendPortStatus(m_PortType, PortStatus::Unloading, socket.szRFID, socket.szBCR) == false)
 					{
 						err = MCEC_ICS_PICKER_SendPortStatusAck_Timeout;
 						break;
 					}
 				
 					lt::CBasicTimer timer;

					while (1)
					{
						if (GetRecvEject())
						{
							ST_SocketInfo* socket = &m_SocketInput;
							Log(_T("Eject Success [RFID :%s, Barcode : %s]"), socket->szRFID, socket->szBCR);
							break;
						}

						if (IsForceStop())
						{
							err = MCEC_ForceStop;
							break;
						}

						Sleep(100);
					} 					
 				}
 			}
		}		
	}	
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_START:
	{
		Sleep(100);

// 		auto EqpType = SET_INSPECTOR;
// 
// 		switch (EqpType)
// 		{
// 		case SYS_SFR_NARROW:
// 		case SYS_SFR_WIDE_H:
// 		case SYS_SFR_WIDE_V:
// 		case SYS_HOTPIXEL:
// 		case SYS_GHOSTFLARE:
// 			SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEND_TESTRESULT_TO_ICS);
// 			break;
// 
// 		case SYS_DISTORTION:
			//SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEMCO_TEST);
// 			break;
// 
// 		default:
// 			// 설비별 달라지는 검사 커맨드를 날려주자...
// 			SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEMCO_EEPROM_WRITE);
// 			break;
// 		}
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_SEMCO_TEST:		// distortion cal
	{
#ifdef DevMode_Simulator

#else
		UINT nRetCode = -1;

		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			if (IsConnected_SEMCO(m_nPortId))
			{
				ST_SocketInfo* socket = &m_SocketInput;

				if (socket->nSEMCO_Code == 0)
				{
					SEMCO_WaitStartTestAck(m_nPortId, 0, 45000, nRetCode);

					socket->nSEMCO_Code = nRetCode;
				}

				nRetCode = socket->nSEMCO_Code;

				SetTestResult(nRetCode);
				Log(_T("Recv Distortion Calibration ack Code From Tester : %d"), nRetCode);
			}		
		}		
#endif
		//SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEMCO_EEPROM_WRITE);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_SEMCO_EEPROM_WRITE:
#ifdef DevMode_Simulator

#else
		// EEPROM WRITE 작업 필요 

#endif
		//SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEMCO_PREVIEW_STOP);
		break;	

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_SEMCO_PREVIEW_STOP:
#ifdef DevMode_Simulator

#else
		SEMCO_SendPreviewStop(m_nPortId);
#endif
		//SetNextStep(SEQ_PICKER_STEP_FINALIZE_POWER_OFF);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_POWER_OFF:
#ifdef DevMode_Simulator

#else
		POWERSUPPLY_VoltOff(m_nPortId);
#endif
		//SetNextStep(SEQ_PICKER_STEP_FINALIZE_SEND_TESTRESULT_TO_ICS);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_SEND_TESTRESULT_TO_ICS:
	{
#ifdef DevMode_Simulator

#else
		// 테스트 결과 전송
		ST_SocketInfo socket;
		if (!GetSocketInfo(socket))
		{
			// error socket 정보가 없다

		}

		if (ICS_SendTestResult(socket.szRFID, socket.nSEMCO_Code, GetPortId()))
		{

		}

		CString sz;
		sz.Format(_T("Send TestResult of SEMCO To ICS [%d] "),socket.nSEMCO_Code);
#endif
	//	SetNextStep(SEQ_PICKER_STEP_FINALIZE_END);
	}
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_FINALIZE_END:
		//SetNextStep(SEQ_PICKER_STEP_UNLOAD_REQEUST);
		break;

	case enSeqPickerStep::SEQ_PICKER_STEP_COMPLETE: // 완료
		//SetNextStep(SEQ_PICKER_STEP_WAIT_SOCKET);
		break;

	default:
		break;
	}

	return err;
}

enMCErrCode CSeqPicker::StartOperation_Inspection(__in InspectionPortType port)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	UINT nRetCode = -1;

	if (false == IsConnected_SEMCO(port))
	{
		// 에러 
		return err;
	}

	// preview start 성공했는지 확인
	auto bSuccess = false;

	for (int i = 0; i < 5; i++)
	{
		nRetCode = -1;
		bSuccess = SEMCO_WaitPreviewStartAck(GetPortId(), 10000, nRetCode);
		Log(_T("Recv PreviewStart ack Code From Tester : %d"), nRetCode);

		if (nRetCode == 0)
			break;

		POWERSUPPLY_VoltOff(port);
		Sleep(200);
		POWERSUPPLY_VoltOn(port);

		Sleep(500);
		SEMCO_SendPreviewStart(GetPortId());
		SEMCO_Reset(GetPortId());		
	}
		
	if (bSuccess && nRetCode == 0)
	{
		// 검사를 시작한다.
// 		auto EqpType = SET_INSPECTOR;
// 		switch (EqpType)
// 		{
// 		case SYS_GHOSTFLARE:
// 			for (auto TestIdx = 0; TestIdx < 10; TestIdx++)
// 			{
// 				// io 변경
// 				IO_O_FlareLightOn(TestIdx);
// 
// 				SEMCO_SendStartTest(port, TestIdx);
// 				SEMCO_WaitStartTestAck(port, TestIdx, 3000, nRetCode);	// ack 체크 
// 
// 				Log(_T("Recv GhostFlare Ack Code: %d"), nRetCode);		
// 
// 				IO_O_FlareLightOff(TestIdx);
// 			}
// 
// 			// 에러코드 업데이트
// 			SetTestResult(nRetCode);
// 			break;
// 
// 		case SYS_DISTORTION:
// 		{
			SEMCO_SendStartTest(port, eComMode_Insp_TLA);
			SEMCO_WaitStartTestAck(port, 1, 15000, nRetCode);

			// 에러코드 업데이트 
			SetTestResult(nRetCode);

			Log(_T("Recv Distortion Capture Image ack Code From Tester : %d"), nRetCode);

			if (nRetCode == 0)
			{
				SEMCO_SendStartTest(m_nPortId, eComMode_Insp_TLA);
			}
// 		}
// 			break;
// 		
// 		default:
// 			SEMCO_SendStartTest(port);
// 			SEMCO_WaitStartTestAck(port, 0, 20000, nRetCode);
// 
// 			SetTestResult(nRetCode);
// 
// 			Log(_T("Recv Code Test ack Result: %d"), nRetCode);			
// 			break;
// 		}	
	}
	else
	{
		// 실패시 SEMCO 에러코드 표시
		SetTestResult(nRetCode);
	}

	ST_SocketInfo socket;
	GetSocketInfo(socket);
	socket.nSEMCO_Code = nRetCode;

	return err;
}

void CSeqPicker::EqpType_Initialize()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_GHOSTFLARE:
// 		IO_O_FlareLightAllOff();
// 	break;
// 
// 	default:
// 		break;
// 	}
}

enMCErrCode CSeqPicker::EqpType_MoveZInspectionPos(__in InspectionPortType port)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

//	auto EqpType = SET_INSPECTOR;

//	EqpTypeGetOption();

// 	switch (EqpType)
// 	{
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 	case SYS_DISTORTION:
// 	{
		auto st = DATAMANAGER->GetSemiAuto();

		// 180 모델일 경우
// 		if (st->Angle == enAngleType::Angle_180)
// 			err = MO_MoveZ_To_180Inspection(port);
// 		else
// 			err = MO_MoveZ_To_Inspection1(port);
// 	}
// 	break;
// 
// 	case SYS_COLOR_CAL:
// 	{
// 		auto st = DATAMANAGER->GetSemiAuto();
// 
// 		// 180 모델일 경우
// 		if (st->Angle == enAngleType::Angle_82_5)
// 			err = MO_MoveZ_To_Inspection2(port);
// 		else
// 			err = MO_MoveZ_To_Inspection1(port);
// 	}
// 		break;
// 
// 	default:
// 		err = MO_MoveZ_To_Inspection1(port);
// 		break;
// 	}

	return err;
}

void CSeqPicker::EqpType_Finalize()
{

}

