#include "pch.h"
#include "SeqConveyor1.h"
#include "LuriMotion_BaseDlg.h"
#include "Util/BasicTimer.h"

#define Timeout_WaitForDetectSignal  10000

CSeqConveyor1::CSeqConveyor1()
{
	SetNextStep(SEQ_CV1_STEP_READY);
	m_Accept = false;

	Reset();
}

CSeqConveyor1::~CSeqConveyor1()
{

}

void CSeqConveyor1::Reset()
{
	m_bSocketEmptyBuffer = true;
	m_SocketBuffer.Reset();

	m_bSocketEmptyInput = true;
	m_SocketInput.Reset();

	__super::Reset();
}

bool CSeqConveyor1::WaitForDetectSignal(__in enConv1SensorSelect sensor, __in bool bStatus)
{
	bool bRet = false;
	clock_t starttm = clock();
	do
	{
// 		if (IO_I_CV1_Sensor(sensor) == bStatus)
// 		{
// 			bRet = true;
// 			break;
// 		}

		// 강제정지시 멈춤 
		if (IsForceStop())
		{
			break;
		}

		Sleep(10);
	} while ((clock() - starttm) < Timeout_WaitForDetectSignal);

	return bRet;
}

bool CSeqConveyor1::WaitForSocketToOut()
{
	auto bRet = false;

	// 감지 될때까지 대기
	bRet = WaitForDetectSignal(enConv1SensorSelect::Cv1_Out);

	if (IsForceStop())
		return false;

	// 벗어날때까지 대기
	bRet = (bRet) ? WaitForDetectSignal(enConv1SensorSelect::Cv1_Out, false) : bRet;

	if (IsForceStop())
		return false;

	return bRet;
}

UINT CSeqConveyor1::RunProcess()
{
	enMCErrCode err =enMCErrCode::MCEC_OK;

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
			Log(_T("Conveyor1 =========> %s(start)"), g_szSeqName_CV1[GetStep()]);
			
			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();

			Log(_T("Conveyor1 =========> %s(end),ResultCode:%d, Elaped : %d ms"), g_szSeqName_CV1[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
		}			

		if (err != enMCErrCode::MCEC_OK)
		{
			Error(err);
			SetStatus(enSeqStatus::Status_Error);
// 			IO_O_CV1_IF_FrontIn(false);
// 			MO_MoveConveyor1_Stop();

			// 에러 reason 포함 
		}
		else
		{
			SetStatus(enSeqStatus::Status_Complete);
		}

//		Lock();
	}
	return 0;
}

enMCErrCode CSeqConveyor1::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqCv1Step)GetStep())
	{	
	case enSeqCv1Step::SEQ_CV1_STEP_READY:	// 준비
	{
		// 버퍼 초기화
		// 버퍼 DOWN
		// 버퍼 GRIP OFF	
#ifdef DevMode_Simulator
		
#else
// 		err = (IO_CV1_BufClampCylOff() == false) ? MCEC_IO_CV1_BufferClampOff_Timeout : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_CV1_BufCylDn() == false) ? MCEC_IO_CV1_BufferLiftCylDn_Timeout : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		IO_O_CV1_MoveMode(enConv1SensorSelect::Cv1_Input);
#endif 
	}
		break;	

	case enSeqCv1Step::SEQ_CV1_STEP_CHECK_STATUS:
	{

#ifdef DevMode_Simulator


#else
// 		err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Input)) ? MCEC_IO_CV1_Input_Detect : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Stop_RFID))	? MCEC_IO_CV1_StopRFID_Detect : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Slow)) ? MCEC_IO_CV1_Slow_Detect : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 
// 		err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Stop)) ? MCEC_IO_CV1_Stop_Detect: err;
// 		if (err != enMCErrCode::MCEC_OK) break;
// 		
// 		err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Out)) ? MCEC_IO_CV1_Output_Detect : err;
// 		if (err != enMCErrCode::MCEC_OK) break;
#endif 
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_WAIT_SOCKET:
	{
		if (ICS_SendPortStatus(PortType::RFID, PortStatus::Empty, _T(""), _T("")) == false)
		{
			// 통신 에러 
			err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
			break;
		}

		if (ICS_SendConveyorStatus(ConveyorType::RFID, ConveyorStatus1::Idle, 
									ConveyorStatus2::Empty,_T(""), _T("")) == false)
		{
			// 통신 에러 
			err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
			break;
		}

#ifdef DevMode_Simulator


#else
		//MO_MoveConveyor1_Reset();
#endif		
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_START:
	{
		
	}
		break;

		// 버퍼 위치 센서까지 도달할때까지 대기
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_SLOW_SENSOR:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			//MO_MoveConveyor1_Run();
			Sleep(2000);
		}
		else
		{
			err = ConveyorRun(enConv1SensorSelect::Cv1_Slow);
			if (err != enMCErrCode::MCEC_OK) break;

			auto bDetect = WaitForDetectSignal(enConv1SensorSelect::Cv1_Stop_RFID, false);
			if (!bDetect)
			{
				//err = MCEC_IO_CV1_StopRFID_Timeout;
				break;
			}			

			// 앞설비에 신호를 보내주자
			//IO_O_CV1_IF_FrontIn(false);

			WaitForDetectSignal(enConv1SensorSelect::Cv1_Slow, true);

			if (!bDetect)
			{
				//err = MCEC_IO_CV1_Slow_Timeout;
				break;
			}
		}
#endif 
	}
	break;

	// 버퍼 위치 센서까지 도달할때까지 대기
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_STOP_SENSOR:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			//MO_MoveConveyor1_Stop();
			Sleep(1000);
		}
		else
		{
			err = ConveyorRun(enConv1SensorSelect::Cv1_Stop);
			if (err != enMCErrCode::MCEC_OK) break;
		
			auto bDetect = WaitForDetectSignal(enConv1SensorSelect::Cv1_Stop);

			if (!bDetect)
			{
//				err = MCEC_IO_CV1_Stop_Timeout;
				break;
			}

			//MO_MoveConveyor1_Stop();
		}		
#endif 
	}
		break;

	// CYLINDER UP
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CYLINDER_UP:
	{
#ifdef DevMode_Simulator

#else
	//	err = (IO_CV1_BufCylUp() == false) ? MCEC_IO_CV1_BufferLiftCylUp_Timeout : err;
#endif 
	}
		break;

		// GRIP ON
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_ON:
	{
#ifdef DevMode_Simulator

#else
		//err = (IO_CV1_BufClampCylOn() == false) ? MCEC_IO_CV1_BufferClampOn_Timeout : err;
#endif 
	}
	break;

	// GRIP OFF
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_OFF:
	{
#ifdef DevMode_Simulator

#else
		//err = (IO_CV1_BufClampCylOff() == false) ? MCEC_IO_CV1_BufferClampOff_Timeout : err;
#endif 
	}
	break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_END:
	{
		ST_SocketInfo socket = m_SocketBuffer;
		if (ICS_SendPortStatus(PortType::Buffer, PortStatus::Loaded, socket.szRFID, socket.szBCR) == false)
		{
			// 통신 에러 
			err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
			break;
		}

#ifdef DevMode_Simulator

#else

#endif	
	}
	break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_START:
	{		
		
	}
		break;

	// Input Sensor 감지 확인
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_INPUT_SENSOR:
	{
#ifdef DevMode_Simulator
		if (ICS_SendConveyorStatus(ConveyorType::RFID, ConveyorStatus1::Moving, ConveyorStatus2::Loaded, _T(""), _T("")) == false)
		{
			// 통신 에러 
			err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
			break;
		}
#else			
		// 컨베이어 위치 reset
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			err = ConveyorRun(enConv1SensorSelect::Cv1_clear);
			Sleep(1000);
			if (err != enMCErrCode::MCEC_OK) break;
		}
		else
		{
 			if (ICS_SendConveyorStatus(ConveyorType::RFID, ConveyorStatus1::Moving, ConveyorStatus2::Loaded, _T(""), _T("")) == false)
 			{
 				// 통신 에러 
 				err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
 				break;
 			}

			if (GetUseRFIDStop())
				err = ConveyorRun(enConv1SensorSelect::Cv1_Input);
			else
				err = ConveyorRun(enConv1SensorSelect::Cv1_clear);

			if (err != enMCErrCode::MCEC_OK) break;

			// 앞설비에 신호를 보내주자
			//IO_O_CV1_IF_FrontIn(true);
			
			// 소켓 감지 될때까지 기달리자..
			auto bDetect = WaitForDetectSignal(enConv1SensorSelect::Cv1_Input);

			if (!bDetect) {}
//				err = MCEC_IO_CV1_Input_Timeout;
		}	
#endif
	}
		break;

		// rfid stop 센서 감지 확인
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR:
	{
		ST_SocketInfo socket = m_SocketInput;
	
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(1000);
			//MO_MoveConveyor1_Stop();
			Sleep(1000);
		}
		else
		{
			if (GetUseRFIDStop())
				err = ConveyorRun(enConv1SensorSelect::Cv1_Stop_RFID);
			else
				err = ConveyorRun(enConv1SensorSelect::Cv1_clear);

			if (err != enMCErrCode::MCEC_OK) break;

			auto bDetect = WaitForDetectSignal(enConv1SensorSelect::Cv1_Stop_RFID);

			if (!bDetect)
//				err = MCEC_IO_CV1_StopRFID_Timeout;


			// 앞설비에 신호를 보내주자
			//IO_O_CV1_IF_FrontIn(false);

			Sleep(400);

			// 소켓 데이터생성 
			InputSocket();
		}		
 		if (ICS_SendPortStatus(PortType::RFID, PortStatus::Loaded, socket.szRFID, socket.szBCR) == false)
 		{
			err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
 			break;
 		}
#endif
	}
		break;

		// RFID 스캔
	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_SCAN_SOCKET:
	{
		CString szRFID;
		ST_SocketInfo* socket = &m_SocketInput;
			
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(1000);
		}
		else
		{
			// 여기서 RFID를 찍자...
			// RFID 스캔 
			Log(_T("RFID READ START"));

			auto nDelayTime = 100;

//			err = (RFID_Read(szRFID)) ? err : MCEC_RFID_CV1_ScanFail;

	//		RFID_Read(szRFID);

// 			for (int retry = 0; retry < 5; retry++)
// 			{
// 				err = (RFID_Read(szRFID)) ? MCEC_OK : MCEC_RFID_CV1_ScanFail;
// 
// 				if (err == enMCErrCode::MCEC_OK)
// 					break;
// 
// 				Sleep(nDelayTime * retry);
// 
// 				RFID_Disconnect();
// 				Sleep(nDelayTime * retry);
// 
// 				RFID_Connect();
// 
// 				Sleep(nDelayTime * retry);
// 				Log(_T("RFID SCAN Retry[%d]"), retry);
// 			}

			if (err != enMCErrCode::MCEC_OK) break;

			Log(_T("RFID READ END : %s"), szRFID);
			socket->szRFID = szRFID;

			Sleep(100);

			if (ICS_SendPortStatus(PortType::RFID, PortStatus::Loaded, socket->szRFID, _T("")) == false)
			{
				err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
				break;
			}

			if (ICS_SendConveyorStatus(ConveyorType::RFID, ConveyorStatus1::Idle, ConveyorStatus2::Loaded,
				socket->szRFID, socket->szBCR) == false)
			{
				err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
				break;
			}

			if (ICS_SendSocketRequest(socket->szRFID) == false)
			{
				err = MCEC_ICS_CV1_SendSocketRequestAck_Timeout;
				break;
			}

			if (ICS_IsUse())
			{
				lt::CBasicTimer timer;

				ResetAccept();

				do
				{
					if (GetAccept() == 0)
					{
						// 에러

					}
					else
					{
						ST_SocketInfo* socket = &m_SocketInput;
						Log(_T("Accept Success [RFID :%s, Barcode : %s]"), socket->szRFID, socket->szBCR);
						break;
					}

					if (IsForceStop())
					{
						err = MCEC_ForceStop;
						break;
					}

					Sleep(10);
				} while (1);
			}
		}		
#endif			
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_END:
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_START:
	{
	
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_RUN:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			//MO_MoveConveyor1_Run();
			Sleep(3000);
			//MO_MoveConveyor1_Stop();
			Sleep(2000);
		}
		else
		{
			err = ConveyorRun(enConv1SensorSelect::Cv1_clear);
			if (err != enMCErrCode::MCEC_OK) break;
			
			if (WaitForDetectSignal(enConv1SensorSelect::Cv1_Out, true))
			{
				if (WaitForSocketToOut() == false)
				{
//					err = MCEC_IO_CV1_Output_Timeout;
					break;
				}
				else
				{
				//	MO_MoveConveyor1_Stop();
				}
			}
			else
			{
//				err = MCEC_IO_CV1_Output_Timeout;
			}
		}		
#endif
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_END:
	{
		if (ICS_SendPortStatus(PortType::RFID, PortStatus::Empty, _T(""), _T("")) == false)
		{
			err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
			break;
		}

		if (ICS_SendConveyorStatus(ConveyorType::RFID, ConveyorStatus1::Idle, ConveyorStatus2::Empty,
			_T(""), _T("")) == false)
		{
			err = MCEC_ICS_CV1_SendConveyorStatusAck_Timeout;
			break;
		}


#ifdef DevMode_Simulator

#else
		
#endif
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_START:
		break;

		// GRIP OFF
	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_GRIP_OFF:
	{
#ifdef DevMode_Simulator

#else
		//err = (IO_CV1_BufClampCylOff() == false) ? MCEC_IO_CV1_BufferClampOff_Timeout : err;		
#endif
	}
	break;

	// CYLINDER DOWN
	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_CYLINDER_DN:
	{
#ifdef DevMode_Simulator

#else
		//err = (IO_CV1_BufCylDn() == false) ? MCEC_IO_CV1_BufferLiftCylDn_Timeout : err;		
#endif
	}
		break;

		// 센서 확인
	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_CHECK_STOP_SENSOR:
	{
#ifdef DevMode_Simulator

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(1000);
		}
		else
		{
			//err = (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Stop) == false) ? MCEC_IO_CV1_Stop_Detect : err;
		}
#endif
	}
		break;

	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_END:
	{
		//ST_SocketInfo socket = m_list.front();
		if (ICS_SendPortStatus(PortType::Buffer, PortStatus::Empty, _T(""), _T("")) == false)
		{
			// 통신 에러 
			err = MCEC_ICS_CV1_SendPortStatusAck_Timeout;
			break;
		}
#ifdef DevMode_Simulator

#else

#endif
	}
		
		break;
	
	case enSeqCv1Step::SEQ_CV1_STEP_COMPLETE:  // 완료
		break;

	default:
		break;
	}

	return err;
}

enMCErrCode CSeqConveyor1::ConveyorRun(__in enConv1SensorSelect mode)
{
	auto errcode = enMCErrCode::MCEC_OK;

	//IO_O_CV1_MoveMode(mode);
	Log(_T("Conveyor1 Run mode : %d"), mode);
	
	if (mode == enConv1SensorSelect::Cv1_Stop || mode == enConv1SensorSelect::Cv1_Stop_RFID)
	{
	//	errcode = MO_MoveConveyor1_Run();
	}
	else
	{
	//	errcode = MO_IsConveyor1_Moving();
	//	errcode = MO_MoveConveyor1_Run();
		Sleep(200);
	//	errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor1_Moving() : errcode;
	}

	return errcode;
}



