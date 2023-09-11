#include "pch.h"
#include "SeqConveyorReturn.h"
#include "LuriMotion_BaseDlg.h"

#include "Util/BasicTimer.h"

#define Timeout_WaitForDetectSignal  10000

CSeqConveyorReturn::CSeqConveyorReturn()
{
	SetNextStep(SEQ_CVR_STEP_READY);
	Reset();
}

CSeqConveyorReturn::~CSeqConveyorReturn()
{
}

void CSeqConveyorReturn::Reset()
{
// 	std::list<ST_SocketInfo>::iterator it = m_list.begin();
// 
// 	while (m_list.size() > 0)
// 	{
// 		it = m_list.begin();
// 		m_list.erase(it);
// 	}

	m_bSocketEmptyInput = true;
	m_SocketInput.Reset();
	__super::Reset();
}

bool CSeqConveyorReturn::WaitForDetectSignal(__in enConvRetSensorSelect sensor, __in bool bStatus)
{
	bool bRet = false;
// 	clock_t starttm = clock();
// 	do
// 	{
// 		if (IO_I_CVR_Sensor(sensor) == bStatus)
// 		{
// 			bRet = true;
// 			break;
// 		}
// 
// 		//if (MO_IsConveyorRet_Moving() != MCEC_OK)
// 		//{
// 		//	break;
// 		//}
// 
// 		Sleep(10);
// 	} while ((clock() - starttm) < Timeout_WaitForDetectSignal);

	return bRet;
}

bool CSeqConveyorReturn::WaitForSocketToOut()
{
	auto bRet = false;

	// 감지 될때까지 대기
	bRet = WaitForDetectSignal(enConvRetSensorSelect::Cvr_Stop);

	// 벗어날때까지 대기
	bRet = (bRet) ? WaitForDetectSignal(enConvRetSensorSelect::Cvr_Stop, false) : bRet;

	return bRet;
}

bool CSeqConveyorReturn::WaitForSocketFromIn()
{
	auto bRet = false;

	// 감지 될때까지 대기
	bRet = WaitForDetectSignal(enConvRetSensorSelect::Cvr_Input);

	// 벗어날때까지 대기
	bRet = (bRet) ? WaitForDetectSignal(enConvRetSensorSelect::Cvr_Input, false) : bRet;

	return bRet;
}

UINT CSeqConveyorReturn::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	while (IsThreadStart())
	{
		Sleep(10);
	
		if (IsThreadStop())
			continue;

		if (IsChangeStep())
		{
			Log(_T("Conveyor Return =========> %s(start)"), g_szSeqName_CVR[GetStep()]);
			
			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();

			Log(_T("Conveyor Return =========> %s(end),ResultCode:%d,Elaped : %d ms"), g_szSeqName_CVR[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));			
		}

		if (err != enMCErrCode::MCEC_OK)
		{
// 			IO_O_CVR_IF_RearOut(false);
// 			IO_O_CVR_IF_FrontOut(false);
// 
// 			Error(err);
// 			SetStatus(enSeqStatus::Status_Error);
// 			MO_MoveConveyorRet_Stop();
		}
		else
		{
			SetStatus(enSeqStatus::Status_Complete);
		}	
		
	}
	return 0;
}

enMCErrCode CSeqConveyorReturn::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqCvrStep)GetStep())
	{
	case enSeqCvrStep::SEQ_CVR_STEP_READY:			// 준비
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_CHECK_STATUS:	
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_WAIT_SOCKET:	
	{
		if (ICS_SendConveyorStatus(ConveyorType::Returning, ConveyorStatus1::Idle,
			ConveyorStatus2::Empty, _T(""), _T("")) == false)
		{
			err = MCEC_ICS_CVR_SendConveyorStatusAck_Timeout;
			break;
		}

		//MO_MoveConveyorRet_Reset();
	}
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_START:	
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN:
	{
#ifdef DevMode_Simulator
		ST_SocketInfo socket = m_SocketInput;

		if (ICS_SendConveyorStatus(ConveyorType::Returning, ConveyorStatus1::Moving,
			ConveyorStatus2::Loaded, socket.szRFID, socket.szBCR) == false)
		{
			err = MCEC_ICS_CVR_SendConveyorStatusAck_Timeout;
			break;
		}

		Sleep(1000);
#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(3000);
			ConveyorRun(enConvRetSensorSelect::Cvr_clear);			
		}
		else
		{	
			err = ConveyorRun(enConvRetSensorSelect::Cvr_clear);
			if (err != enMCErrCode::MCEC_OK) break;

			// 뒷설비에 OUT 신호 보내기
// 			IO_O_CVR_IF_RearOut(true);
// 
// 				// 자재가 들어올때까지 기달린다.
// 			err = WaitForSocketFromIn() ? err : MCEC_IO_CVR_Input_Timeout;
// 			if (err != enMCErrCode::MCEC_OK) break;
// 
// 			// 자재를 받았으면 신호를 꺼주자..
// 			IO_O_CVR_IF_RearOut(false);
		}		
#endif
	}
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR:
	{
#ifdef DevMode_Simulator
		ST_SocketInfo socket = m_SocketInput;

		if (socket.bBypass)
		{
			Sleep(1000);
		}
		else
		{
			Sleep(1000);
		}
		
#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(3000);
		}
		else
		{
			ST_SocketInfo socket = m_SocketInput;

			if (ICS_SendConveyorStatus(ConveyorType::Returning, ConveyorStatus1::Moving,
				ConveyorStatus2::Loaded, socket.szRFID, socket.szBCR) == false)
			{
				err = MCEC_ICS_CVR_SendConveyorStatusAck_Timeout;
				break;
			}

			err = ConveyorRun(Cvr_Slow);
			if (err != enMCErrCode::MCEC_OK) break;

			// Slow 센서 감지될때까지 기달린다.
//			err = WaitForDetectSignal(Cvr_Slow, true) ? err : MCEC_IO_CVR_Slow_Timeout;
			if (err != enMCErrCode::MCEC_OK) break;
		}		
#endif
	}
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR:
	{
#ifdef DevMode_Simulator
		ST_SocketInfo socket = m_SocketInput;

		if (socket.bBypass)
		{
			Sleep(1000);
		}
		else
		{
			Sleep(1000);
		}

#else
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			Sleep(3000);

			// 배출시 컨베이어를 멈춘다.
			//MO_MoveConveyorRet_Stop();
		}
		else
		{
			err = ConveyorRun(Cvr_Stop);
			if (err != enMCErrCode::MCEC_OK) break;

			// Stop 센서 감지될때까지 기달린다.
//			err = WaitForDetectSignal(Cvr_Stop, true) ? err : MCEC_IO_CVR_Stop_Timeout;
			if (err != enMCErrCode::MCEC_OK) break;			
		}
#endif
	}
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_RUN:
	{
#ifdef DevMode_Simulator

#else
		// 뒷설비에 신호를 살린다.
	//	IO_O_CVR_IF_FrontOut(true);

		//	if (!IO_I_CVR_IF_FrontOut())
		//		MO_MoveConveyorRet_Stop();

		Sleep(100);

		lt::CBasicTimer timer;
		auto bMoveContinue = false;

		do
		{
// 			if (IO_I_CVR_IF_FrontOut())
// 			{
// 				bMoveContinue = true;
// 				break;
// 			}

			if (IsForceStop())
				break;

			Sleep(10);
		} while (1);

		if (bMoveContinue)
		{
			err = ConveyorRun(Cvr_clear);

//			err = WaitForDetectSignal(enConvRetSensorSelect::Cvr_Stop, false) ? err : MCEC_IO_CVR_Stop_Timeout;

// 			IO_O_CVR_IF_FrontOut(false);
// 			MO_MoveConveyorRet_Stop();

			if (ICS_SendConveyorStatus(ConveyorType::Returning, ConveyorStatus1::Idle,
				ConveyorStatus2::Loaded, _T(""), _T("")) == false)
			{
				err = MCEC_ICS_CVR_SendConveyorStatusAck_Timeout;
				break;
			}
		}
		else
		{
//			err = MCEC_IO_CVR_IF_OutFront_Timeout;
			Log(_T("CV2 IF Timeout (Input FrontOut)"));
	//		IO_O_CVR_IF_FrontOut(false);
			WaitForDetectSignal(enConvRetSensorSelect::Cvr_Stop, false);
		}
#endif
	}
		break;

	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_END:	
	{
#ifdef DevMode_Simulator
		ST_SocketInfo socket;
		OutputSocket(socket);
		Sleep(100);
#else
		
#endif		
	}
		break;
		
	case enSeqCvrStep::SEQ_CVR_STEP_COMPLETE:		// 완료
		break;

	default:
		break;
	}

	return err;
}

enMCErrCode CSeqConveyorReturn::ConveyorRun(__in enConvRetSensorSelect mode)
{
 	auto errcode = enMCErrCode::MCEC_OK;
// 	Log(_T("Conveyor Return Run mode : %d"), mode);
// 	
// 	IO_O_CVR_MoveMode(mode);
// 
// 	if (mode == enConvRetSensorSelect::Cvr_Stop)
// 	{
// 		errcode = MO_MoveConveyorRet_Run();
// 	}
// 	else
// 	{
// //		errcode = MO_IsConveyorRet_Moving();
// 		errcode = MO_MoveConveyorRet_Run();
// 		Sleep(200);
// 		errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyorRet_Moving() : errcode;
// 	}

	return errcode;
}

