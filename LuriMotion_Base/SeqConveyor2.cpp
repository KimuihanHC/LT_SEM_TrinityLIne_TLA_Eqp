#include "pch.h"
#include "SeqConveyor2.h"
#include "LuriMotion_BaseDlg.h"

#include "Util/BasicTimer.h"

#define Timeout_WaitForDetectSignal  10000

CSeqConveyor2::CSeqConveyor2()
{
	m_WorkPort = InspectionPortType::Left;
	Reset();
}

CSeqConveyor2::~CSeqConveyor2()
{
}

void CSeqConveyor2::Reset()
{
// 	std::list<ST_SocketInfo>::iterator	it = m_list.begin();
// 	while (m_list.size() > 0)
// 	{
// 		it = m_list.begin();
// 		m_list.erase(it);
// 	}
// 
// 	while (m_listUnload.size() > 0)
// 	{
// 		it = m_listUnload.begin();
// 		m_listUnload.erase(it);
// 	}

	m_bSocketEmptyInput = true;
	m_SocketInput.Reset();

	m_bSocketEmptyUnload = true;
	m_SocketUnload.Reset();

	__super::Reset();
}

bool CSeqConveyor2::WaitForDetectSignal(enConv2SensorSelect sensor, __in bool bStatus)
{
	bool bRet = false;
	clock_t starttm = clock();

	do
	{
// 		if (IO_I_CV2_Sensor(sensor) == bStatus)
// 		{
// 			bRet = true;
// 			break;
// 		}

		/*if (MO_IsConveyor2_Moving() != MCEC_OK)
		{
			break;
		}*/

		Sleep(10);
	} while ((clock() - starttm) < Timeout_WaitForDetectSignal);

	return bRet;
}

bool CSeqConveyor2::WaitForDetectSlowSignal(__in InspectionPortType port, __in bool bStatus)
{
	bool bRet = false;
	clock_t starttm = clock();

	auto sensor = enConv2SensorSelect::Cv2_Slow1;

	switch (port)
	{
	case InspectionPortType::Left:
		sensor = enConv2SensorSelect::Cv2_Slow1;
		break;

	case InspectionPortType::Right:
		sensor = enConv2SensorSelect::Cv2_Slow2;
		break;
	}

	do
	{
// 		if (IO_I_CV2_Sensor(sensor) == bStatus)
// 		{
// 			bRet = true;
// 			break;
// 		}

		/*if (MO_IsConveyor2_Moving() != MCEC_OK)
		{
			break;
		}*/

		Sleep(10);
	} while ((clock() - starttm) < Timeout_WaitForDetectSignal);

	return bRet;
}

bool CSeqConveyor2::WaitForDetectStopSignal(__in InspectionPortType port,  __in bool bStatus)
{
	bool bRet = false;
	clock_t starttm = clock();

	auto sensor = enConv2SensorSelect::Cv2_Stop1;

	switch (port)
	{
	case InspectionPortType::Left:
		sensor = enConv2SensorSelect::Cv2_Stop1;
		break;

	case InspectionPortType::Right:
		sensor = enConv2SensorSelect::Cv2_Stop2;
		break;
	}

	do
	{
// 		if (IO_I_CV2_Sensor(sensor) == bStatus)
// 		{
// 			bRet = true;
// 			break;
// 		}

		Sleep(10);
	} while ((clock() - starttm) < Timeout_WaitForDetectSignal);

	return bRet;
}

bool CSeqConveyor2::WaitForSocketToOut()
{
	auto bRet = false;

	// 감지 될때까지 대기
	bRet = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out);

	// 벗어날때까지 대기
	bRet = (bRet) ? WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, false) : bRet;

	return bRet;
}

bool CSeqConveyor2::WaitForSocketFromIn()
{
	auto bRet = false;

	// 감지 될때까지 대기
	bRet = WaitForDetectSignal(enConv2SensorSelect::Cv2_Input);

	// 벗어날때까지 대기
	bRet = (bRet) ? WaitForDetectSignal(enConv2SensorSelect::Cv2_Input, false) : bRet;

	return bRet;
}

UINT CSeqConveyor2::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	while (IsThreadStart())
	{
		Sleep(10);

		if (IsThreadStop())
			continue;

		if (IsError())
			continue;
		
//		Wait();

		if (IsChangeStep())
		{
			Log(_T("Conveyor2 =========> %s(start)"), g_szSeqName_CV2[GetStep()]);
			
			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();
			Log(_T("Conveyor2 =========> %s(end),ResultCode:%d,Elaped:%d ms"), g_szSeqName_CV2[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
		}
		
		if (err != enMCErrCode::MCEC_OK)
		{
			Error(err);
			SetStatus(enSeqStatus::Status_Error);
// 			IO_O_CV2_IF_RearIn(false);
// 			MO_MoveConveyor2_Stop();
		}
		else
		{
			SetStatus(enSeqStatus::Status_Complete);
		}

//		Lock();
	}
	return 0;
}

enMCErrCode CSeqConveyor2::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqCv2Step)GetStep())
	{
	case enSeqCv2Step::SEQ_CV2_STEP_READY:	// 준비
		break;
		
	case enSeqCv2Step::SEQ_CV2_STEP_CHECK_STATUS:
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_WAIT_SOCKET:
	{
		if (ICS_SendConveyorStatus(ConveyorType::Inspection, ConveyorStatus1::Idle,
			ConveyorStatus2::Empty, _T(""), _T("")) == false)
		{
			err = MCEC_ICS_CV2_SendConveyorStatusAck_Timeout;
			break;
		}

		// 제품있을 경우 알람

#ifdef DevMode_Simulator

#else

		//MO_MoveConveyor2_Reset();
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_RESERVED_FROM_PICKER:
	{
		// 컨베이어 위에 제품이 있을 경우 알람 발생

#ifdef DevMode_Simulator

#else

#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_RESERVED_FROM_CONVEYOR1:
	{
		// 제품이 들어오는지 확인하고 있자... 제품 들어오는것이 감지 되면 loading 을 시작하자..
		
#ifdef DevMode_Simulator
		Sleep(500);
#else
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow1)) ? MCEC_IO_CV2_Slow1_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop1)) ? MCEC_IO_CV2_Stop1_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow2)) ? MCEC_IO_CV2_Slow2_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop2)) ? MCEC_IO_CV2_Stop2_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Out)) ? MCEC_IO_CV2_Output_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif

	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_START:
	{
		// 어디로 가야할지 이 이전에 정해져 있어야 한다.
		// 설정 시간동안 오지 않을 경우 timeout 에러 (제품이 넘어오지 않음)

#ifdef DevMode_Simulator

		Sleep(500);

		do
		{
			if (IsInputSocket())
				break;

			Sleep(1);
		} while (GetElapTime() < 5000);

		if (IsInputSocket())
		{
			
		}
		else
		{
			err = enMCErrCode::MCEC_Unknown;
		}
#else
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow1)) ? MCEC_IO_CV2_Slow1_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop1)) ? MCEC_IO_CV2_Stop1_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Slow2)) ? MCEC_IO_CV2_Slow2_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Stop2)) ? MCEC_IO_CV2_Stop2_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Out)) ? MCEC_IO_CV2_Output_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif		
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_FROM_IN:
	{
		// 소켓 투입이 감지 될때 까지 기달린다..
		ST_SocketInfo socket = m_SocketInput;

		if (ICS_SendConveyorStatus(ConveyorType::Inspection, ConveyorStatus1::Moving,
			ConveyorStatus2::Loaded, socket.szRFID, socket.szBCR) == false)
		{
			err = MCEC_ICS_CV2_SendConveyorStatusAck_Timeout;
			break;
		}

#ifdef DevMode_Simulator

#else
		auto bDetect = false;

		err = ConveyorRun(enConv2SensorSelect::Cv2_clear);
		if (err != enMCErrCode::MCEC_OK) break;

		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(1000);
		}
		else
		{
			bDetect = WaitForDetectSignal(enConv2SensorSelect::Cv2_Input);

			if (!bDetect) {}
//				err = MCEC_IO_CV2_Input_Timeout;
		}		
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_SLOW_SENSOR:
	{
		// 원하는 목적지의 컨베이어에 멈추게 하기		
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{

		}
		else
		{
			auto bDetect = false;

			auto sensor = enConv2SensorSelect::Cv2_Slow1;
			switch (m_WorkPort)
			{
			case InspectionPortType::Left:
				sensor = enConv2SensorSelect::Cv2_Slow1;
				break;

			case InspectionPortType::Right:
				sensor = enConv2SensorSelect::Cv2_Slow2;
				break;
			}

			err = ConveyorRun(sensor);
			if (err != enMCErrCode::MCEC_OK) break;	

			if (WaitForDetectSlowSignal(m_WorkPort, true) == false)
			{
				switch (m_WorkPort)
				{
				case InspectionPortType::Left:
//					err = MCEC_IO_CV2_Slow1_Timeout;
					break;

				case InspectionPortType::Right:
//					err = MCEC_IO_CV2_Slow2_Timeout;
					break;
				}
			}			
		}		
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_STOP_SENSOR:
	{
		// 원하는 목적지의 컨베이어에 멈추게 하기
		ST_SocketInfo socket = m_SocketInput;

		if (ICS_SendConveyorStatus(ConveyorType::Inspection, ConveyorStatus1::Idle,
			ConveyorStatus2::Loaded, socket.szRFID, socket.szBCR) == false)
		{
			err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
			break;
		}

#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(3000);
			//MO_MoveConveyor2_Stop();
		}
		else
		{
			auto sensor = enConv2SensorSelect::Cv2_Stop1;

			switch (m_WorkPort)
			{
			case InspectionPortType::Left:
				sensor = enConv2SensorSelect::Cv2_Stop1;
				break;

			case InspectionPortType::Right:
				sensor = enConv2SensorSelect::Cv2_Stop2;
				break;
			}

			err = ConveyorRun(sensor);
			if (err != enMCErrCode::MCEC_OK) break;

			if (WaitForDetectStopSignal(m_WorkPort, sensor) == false)
			{
				switch (m_WorkPort)
				{
				case InspectionPortType::Left:
//					err = MCEC_IO_CV2_Stop1_Timeout;
					break;

				case InspectionPortType::Right:
//					err = MCEC_IO_CV2_Stop2_Timeout;
					break;
				}
			}
		}		
#endif
	}
	break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_CHECK_SOCKET:
	{
		// 안착 틀어짐 있는지 확인하기 
#ifdef DevMode_Simulator

#else
		switch (m_WorkPort)
		{
		case InspectionPortType::Left:
		{
// 			err = (IO_I_CV2_Loading1_Check1()) ? MCEC_IO_CV2_LeftLoadingCheck1_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
// 
// 			err = (IO_I_CV2_Loading1_Check2()) ? MCEC_IO_CV2_LeftLoadingCheck2_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
				break;
		}
		break;

		case InspectionPortType::Right:
		{
// 			err = (IO_I_CV2_Loading2_Check3()) ? MCEC_IO_CV2_RightLoadingCheck1_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
// 
// 			err = (IO_I_CV2_Loading2_Check4()) ? MCEC_IO_CV2_RightLoadingCheck2_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
				break;
		}
		break;

		default:
			break;
		}
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_END:
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_START:
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_CHECK_SOCKET:
	{
		// 소켓 안착 상태 확인

#ifdef DevMode_Simulator
		Log(_T("Unloading Port : %d"), m_WorkPort);		
#else
		switch (m_WorkPort)
		{
		case InspectionPortType::Left:
		{
// 			err = (IO_I_CV2_Loading1_Check1()) ? MCEC_IO_CV2_LeftLoadingCheck1_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
// 
// 			err = (IO_I_CV2_Loading1_Check2()) ? MCEC_IO_CV2_LeftLoadingCheck2_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
		}
		break;

		case InspectionPortType::Right:
		{
// 			err = (IO_I_CV2_Loading2_Check3()) ? MCEC_IO_CV2_RightLoadingCheck1_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
// 
// 			err = (IO_I_CV2_Loading2_Check4()) ? MCEC_IO_CV2_RightLoadingCheck2_Detect : err;
// 			if (err != enMCErrCode::MCEC_OK)
// 				break;
		}
		break;

		default:
			break;
		}
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN:
	{
		ST_SocketInfo socket = m_SocketUnload;

		if (ICS_SendConveyorStatus(ConveyorType::Inspection, ConveyorStatus1::Moving,
			ConveyorStatus2::Loaded, socket.szRFID, socket.szBCR) == false)
		{
			err = MCEC_ICS_CV2_SendConveyorStatusAck_Timeout;
			break;
		}
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			err = ConveyorRun(Cv2_clear);
			Sleep(3000);
			//MO_MoveConveyor2_Stop();
		}
		else
		{
			// 뒷설비에 OUT 신호 보내기
			//IO_O_CV2_IF_RearIn(true);

			lt::CBasicTimer timer;
			auto bMoveContinue = false;
			
			do
			{
// 				if (IO_I_CV2_IF_RearIn())
// 				{
// 					bMoveContinue = true;
// 					break;
// 				}

				// stop 할때까지 대기하자..
				if (IsForceStop())
				{
					err = MCEC_ForceStop;
					break;
				}					

				Sleep(10);
			} while (1);

			if (bMoveContinue)
			{
				err = ConveyorRun(enConv2SensorSelect::Cv2_clear);
				if (err != enMCErrCode::MCEC_OK) break;

//				err = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, true) ? err : MCEC_IO_CV2_Output_Timeout;
				if (err != enMCErrCode::MCEC_OK) break;

			//	err = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, false) ? err : MCEC_IO_CV2_Output_Timeout;

// 				IO_O_CV2_IF_RearIn(false);
// 				MO_MoveConveyor2_Stop();
			}
			else
			{
				// 에러 
//				err = MCEC_IO_CV2_IF_InRear_Timeout;
				Log(_T("CV2 IF Timeout (Input RearIn)"));
				//IO_O_CV2_IF_RearIn(false);
			}
		}
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_END:
	{
#ifdef DevMode_Simulator
		
#else
		ST_SocketInfo* socket = &m_SocketUnload;
		Log(_T("OutSocket RFID : %s, BCR : %s, Result:%d"), socket->szRFID, socket->szBCR, socket->bResultOk);
#endif
	}
		break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_START:
	{

	}
	break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_OUT_SENSOR:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			err = ConveyorRun(Cv2_clear);
			Sleep(3000);
			//MO_MoveConveyor2_Stop();
		}
		else
		{
			// 컨베이어 회전
			err = ConveyorRun(enConv2SensorSelect::Cv2_clear, enConveyorSpeed::Middle);
			if (err != enMCErrCode::MCEC_OK) break;

			lt::CBasicTimer timer;
			auto bMoveContinue = false;

			//IO_O_CV2_IF_RearIn(true);

			do
			{
// 				if (IO_I_CV2_Sensor(enConv2SensorSelect::Cv2_Out))
// 				{
// 					if (!IO_I_CV2_IF_RearIn())
// 						MO_MoveConveyor2_Stop();
// 
// 					bMoveContinue = true;
// 					break;
// 				}

// 				if (MO_IsConveyor2_Moving() != MCEC_OK)
// 				{
// 					err = MCEC_Motion_CV2_ConveyorIsNotMoving;
// 					break;
// 				}

				if (IsForceStop())
				{
					err = MCEC_ForceStop;
					break;
				}

				Sleep(10);
			} while (timer.GetElapsedTime(lt::CBasicTimer::Unit::Seconds) < 10);
		}
#endif		
	}
	break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_RUN:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			err = ConveyorRun(Cv2_clear);
			Sleep(3000);
			//MO_MoveConveyor2_Stop();
		}
		else
		{
			// 뒷설비에 OUT 신호 보내기
// 			IO_O_CV2_IF_RearIn(true);			
// 
// 			if (IO_I_CV2_IF_RearIn())
// 			{
// 				// 멈춰있을경우 컨베이어를 회전 시켜줘야 한다.
// 				err = ConveyorRun(enConv2SensorSelect::Cv2_clear, enConveyorSpeed::Middle);
// 				if (err != enMCErrCode::MCEC_OK) break;
// 
// 				// out 센서 벗어날때까지 기달리자..
// 				err = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, false) ? err : MCEC_IO_CV2_Output_Timeout;
// 				if (err != enMCErrCode::MCEC_OK) break;
// 
// // 				MO_MoveConveyor2_Stop();
// // 				IO_O_CV2_IF_RearIn(false);
// 			}
// 			else
// 			{
// 				err = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, true) ? err : MCEC_IO_CV2_Output_Timeout;
// 				if (err != enMCErrCode::MCEC_OK) break;
// 
// 			//	MO_MoveConveyor2_Stop();
// 
// 				// 뒷설비에서 받을 준비 될때까지 기달리자..
// 				do
// 				{
// // 					if (IO_I_CV2_IF_RearIn())	
// // 					{
// // 						break;
// // 					}
// 
// 					if (IsForceStop())
// 					{
// 						err = MCEC_ForceStop;
// 						break;
// 					}
// 
// 					Sleep(10);
// 				} while (1);
// 
// 				if (err != MCEC_OK) break;
// 
// 				// 컨베이어 run
// 				// out 센서 벗어날때까지 기달리자.
// 
// 				err = ConveyorRun(enConv2SensorSelect::Cv2_clear, enConveyorSpeed::Middle);
// 				if (err != enMCErrCode::MCEC_OK) break;
// 
// 				err = WaitForDetectSignal(enConv2SensorSelect::Cv2_Out, false) ? err : MCEC_IO_CV2_Output_Timeout;
// 				if (err != enMCErrCode::MCEC_OK) break;
// 
// // 				MO_MoveConveyor2_Stop();
// // 				IO_O_CV2_IF_RearIn(false);
			//}
 
		}
#endif
		
	}
	break;

	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_END:
	{
		ST_SocketInfo* socket = &m_SocketUnload;
		Log(_T("OutSocket RFID : %s, BCR : %s, Result:%d"), socket->szRFID, socket->szBCR, socket->bResultOk);
		Sleep(500);
	}
		break;
		
	case enSeqCv2Step::SEQ_CV2_STEP_COMPLETE: // 완료
		break;

	default:
		break;
	}

	return err;
}

enMCErrCode CSeqConveyor2::ConveyorRun(__in enConv2SensorSelect mode, __in enConveyorSpeed spd)
{
	auto errcode = enMCErrCode::MCEC_OK;

//	IO_O_CV2_MoveMode(mode);
	Log(_T("Conveyor2 Run mode : %d"), mode);

	if (mode == enConv2SensorSelect::Cv2_Stop1 || mode == enConv2SensorSelect::Cv2_Stop2)
	{
		//errcode = MO_MoveConveyor2_Run(spd);
	}
	else
	{
		Sleep(50);

// 		errcode = MO_IsConveyor2_Moving();
// 
// 		Sleep(100);

		for (int i = 0; i < 5; i++)
		{
// 			errcode = (errcode == enMCErrCode::MCEC_OK) ? errcode : MO_MoveConveyor2_Run(spd);
// 			Sleep(100);
// 			errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor2_Moving() : errcode;

			if (errcode == enMCErrCode::MCEC_OK)
				break;

//			Log(_T("Conveyor2 Run Retry(%d)"), i);
			Sleep(100);
		}
	}

//	return MCEC_OK;
	return errcode;
}

