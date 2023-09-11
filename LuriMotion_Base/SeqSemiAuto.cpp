#include "pch.h"
#include "SeqSemiAuto.h"
#include "LuriMotion_BaseDlg.h"
#include "Util/BasicTimer.h"

CSeqSemiAuto::CSeqSemiAuto()
{
	m_Portid = InspectionPortType::Left;
	SetNextStep(SEQ_SEMIAUTO_STEP_IDEL);

	m_pThread = NULL;
	m_bThread = FALSE;
}

CSeqSemiAuto::~CSeqSemiAuto()
{
}

void CSeqSemiAuto::SetPortId(__in InspectionPortType port)
{
	m_Portid = port;
}

InspectionPortType CSeqSemiAuto::GetPortid()
{
	return m_Portid;
}

bool CSeqSemiAuto::Start()
{
	DeleteThread();
	return __super::Start();
}

UINT CSeqSemiAuto::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

//	while (IsThreadStart())
	while (SEQUENCE->IsThread_SemiAuto())
	{
		Sleep(10);

		if (IsThreadStop())
			continue;

		if (IsError())
			continue;

		if (IsChangeStep())
		{
			Log(_T("SemiAuto =========> %s(start)"), g_szSeqName_SemiAuto[GetStep()]);
			
			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();

			Log(_T("SemiAuto =========> %s(end),ResultCode:%d,Elaped : %d ms"), g_szSeqName_SemiAuto[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));			
		}

		if (err != enMCErrCode::MCEC_OK)
		{
			Error(err);
			SetStatus(enSeqStatus::Status_Error);

			// 에러 reason 포함 
		}
		else
		{
			SetStatus(enSeqStatus::Status_Complete);

			// 지정된 동작이 끝나면 스레드를 종료하자...
			if (GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_LOADING_END
				|| GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_LOCK_END
				|| GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_PREVIEW_END
				|| GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_TLA_END
				|| GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_VIGNETING_END
				|| GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_UNLOADING_END)
			{
				ForceStop();
				break;
			}
		}
	}
	return 0;
}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
enMCErrCode CSeqSemiAuto::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dCurPos = 0.0f;
	double dSettingPos = 0.0f;
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	auto nStep = GetStep();

	switch ((enSeqSemiAutoStep)GetStep())
	{
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_IDEL:
		break;
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_READY:
		break;

		// (LOADING)
	case SEQ_SEMIAUTO_STEP_LOADING_START:

		break;
	case SEQ_SEMIAUTO_STEP_LOADING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_Type(Screw_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Socket_Unlock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
				break;
			}
			if (IO_O_Chuck_Bwd() == false)
			{
				err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
				break;
			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOADING_END:
		break;


	case SEQ_SEMIAUTO_STEP_LOCK_START:
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Out_Jig_Chk;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveScrew_Type(Screw_Work);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			else
			{
				err = MCEC_Sensnor_Out_Jig_Chk;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Fwd() == false)
			{
				err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
				break;
			}
			Sleep(500);
			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);

		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) == FALSE)
			{
				err = MCEC_Sensnor_Product_L_NotChk;
			}
			else
			{
				err = MCEC_Sensnor_Product_R_NotChk;
			}
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOCK_MOVE:
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_END:
		break;



	case SEQ_SEMIAUTO_STEP_PREVIEW_START:
		break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON:
	{
		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_END:
		break;

		//TLA 조정 관련 
	case SEQ_SEMIAUTO_STEP_TLA_START:
		nRetryChk = 0;
		break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK:
	{
		if (nRetryChk > stTeaching.nAdjust_RetryCnt)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
			}
		}
		else
		{
			if (IO_O_Screw_Bolt_Unlock() == false)
			{
				err = MCEC_Screw_Complete_TimeOut;
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_TLA))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}

		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_ADJUST);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_ADJUST:
	{
		if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Spec) == FALSE) {
			AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
			dSpeed = (double)stMotion.nMotorSpeed[0][2];
			dSettingPos = stTeaching.nAdjust_Wait;
			dSettingPos = dSettingPos / 1000;
			dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * dSettingPos;
			dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
			dTargetPos = dCurPos + dTargetPos;


			if (dTargetPos <= 0)
			{
				SEQUENCE->nADJUST_Reverse = 0;
			}
			else
			{
				SEQUENCE->nADJUST_Reverse = 1;
			}
			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
			{
				AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
				err = MCEC_Motion_Adjust_MoveError;
				break;
			}
			else if (err == enMCErrCode::MCEC_OK)
			{
				AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
				}
			}
		}
		else
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
			{

				AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK:
	{
		if (IO_O_Screw_Bolt_Lock_1() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
		else
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_TLA))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw1_Spec) == FALSE) {
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
				{
					nRetryChk++;
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
				}
			}
			else
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK:
	{
		if (IO_O_Screw_Bolt_Lock_2() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
		else
		{

			if (SEQUENCE->nADJUST_Reverse == 0)
			{
				dSettingPos = stTeaching.nAdjust_Reverse;
				dSettingPos = dSettingPos / 1000;
				dTargetPos =  dSettingPos;
				dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
				dTargetPos = dCurPos + dTargetPos;
				dTargetPos = dTargetPos;
			}
			else
			{
				dSettingPos = stTeaching.nAdjust_Reverse;
				dSettingPos = dSettingPos / 1000;
				dTargetPos = dSettingPos;
				dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
				dTargetPos = dCurPos + dTargetPos;
				dTargetPos = dTargetPos * (-1);
			}
			dSpeed = (double)stMotion.nMotorSpeed[0][2];

			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
			{
				err = MCEC_Motion_Adjust_MoveError;
				break;
			}
			else
			{
				AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
			}

			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_TLA))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
				{
					nRetryChk++;
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
				}
			}
			else
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_END:
		break;
		//(Vigneting)
	case SEQ_SEMIAUTO_STEP_VIGNETING_START:
		break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			// 			err = MO_MoveScrew_Type(Screw_Wait);
			// 			if (err != enMCErrCode::MCEC_OK)
			// 			{
			// 				break;
			// 			}
			// 			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(500);
			// 			if (IO_O_Socket_Unlock() == false)
			// 			{
			// 				err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
			// 				break;
			// 			}
			// 			if (IO_O_Chuck_Bwd() == false)
			// 			{
			// 				err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
			// 				break;
			// 			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_Vigneting46))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}

		if (err == enMCErrCode::MCEC_OK)
		{
			Sleep(50);
			if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_Vigneting82))
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
		}
	
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_END:
		break;
		// (UNLOADING)
	case SEQ_SEMIAUTO_STEP_UNLOADING_START:
	{
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_Type(Screw_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
				Sleep(500);
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Chuck_Bwd() == false)
				{
					err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
			}

		}
		else
		{
			if (err == enMCErrCode::MCEC_OK)
			{
				if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
				{
					err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
				}
				else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
				{
					err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF:
	{
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_END:
		break;

	default:
		break;
	}

	return err;
}

#elif(SET_INSPECTOR == SYS_TLA_OQC)
enMCErrCode CSeqSemiAuto::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dCurPos = 0.0f;
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	auto nStep = GetStep();

	switch ((enSeqSemiAutoStep)GetStep())
	{
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_IDEL:
		break;
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_READY:
		break;

		// (LOADING)
	case SEQ_SEMIAUTO_STEP_LOADING_START:
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_LOADING_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_LOADING_CHK);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_LOADING_CHK:
	{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
// 			if (err == enMCErrCode::MCEC_OK)
// 			{
// 				err = MO_MoveScrew_Type(Screw_Wait);
// 				if (err != enMCErrCode::MCEC_OK)
// 				{
// 					break;
// 				}
// 			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
			}

			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_LOADING_CHK)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_LOADING_END);
				}
			}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOADING_END:
		break;


	case SEQ_SEMIAUTO_STEP_LOCK_START:
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_LOCK_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_CHK);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
// 			err = MO_MoveScrew_Type(Screw_Work);
// 			if (err != enMCErrCode::MCEC_OK)
// 			{
// 				break;
// 			}

		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) == FALSE)
			{
				err = MCEC_Sensnor_Product_L_NotChk;
			}
			else
			{
				err = MCEC_Sensnor_Product_R_NotChk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_LOCK_CHK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_MOVE);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOCK_MOVE:
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_LOCK_MOVE)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_END);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_END:
		break;



	case SEQ_SEMIAUTO_STEP_PREVIEW_START:
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_PREVIEW_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON:
	{
		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_PREVIEW)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW_END);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_END:
		break;

		//TLA 조정 관련 
	case SEQ_SEMIAUTO_STEP_TLA_START:
		nRetryChk = 0;
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK:
	{
// 		if (nRetryChk > stTeaching.nAdjust_RetryCnt)
// 		{
// 			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
// 			{
// 				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
// 			}
// 		}
// 		else
// 		{
// 			if (IO_O_Screw_Bolt_Unlock() == false)
// 			{
// 				err = MCEC_Screw_Complete_TimeOut;
// 				break;
// 			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
				}
			}
	//	}

	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_TLA))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}

		if (err != enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
			}
		}
	}
	break;
// 	case SEQ_SEMIAUTO_STEP_TLA_ADJUST:
// 	{
// 		if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Spec) == FALSE) {
// 			dSpeed = (double)stMotion.nMotorSpeed[0][2];
// 			dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * stTeaching.nAdjust_Wait;
// 			dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
// 			dTargetPos = dCurPos + dTargetPos;
// 			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
// 			{
// 				err = MCEC_Motion_Adjust_MoveError;
// 				break;
// 			}
// 			else if (err != enMCErrCode::MCEC_OK)
// 			{
// 				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
// 				{
// 					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
// 			{
// 				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK);
// 			}
// 		}
// 	}
// 	break;
// 	case SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK:
// 	{
// // 		if (IO_O_Screw_Bolt_L_Lock_1() == false)
// // 		{
// // 			err = MCEC_Screw_Complete_TimeOut;
// // 			break;
// // 		}
// 	}
// 	break;
// 	case SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST:
// 	{
// 		UINT nRetCode = 0;
// 		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
// 		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
// 		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = -5000;
// 		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
// 		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
// 		{
// 		}
// 		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
// 		{
// 			err = MCEC_Tester_Ack_Timeout;
// 			break;
// 		}
// 		if (nRetCode != 0)
// 		{
// 			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
// 			{
// 				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
// 
// 				break;
// 			}
// 		}
// 		if (err != enMCErrCode::MCEC_OK)
// 		{
// 			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw1_Spec) == FALSE) {
// 				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
// 				{
// 					nRetryChk++;
// 					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
// 				}
// 			}
// 			else
// 			{
// 				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
// 				{
// 					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK);
// 				}
// 			}
// 		}
// 	}
// 	break;
// 	case SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK:
// 	{
// 		if (IO_O_Screw_Bolt_L_Lock_2() == false)
// 		{
// 			err = MCEC_Screw_Complete_TimeOut;
// 			break;
// 		}
// 	}
// 	break;
// 	case SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST:
// 	{
// 		UINT nRetCode = 0;
// 		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
// 		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
// 		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = -5000;
// 		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
// 		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
// 		{
// 		}
// 		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
// 		{
// 			err = MCEC_Tester_Ack_Timeout;
// 			break;
// 		}
// 		if (nRetCode != 0)
// 		{
// 			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
// 			{
// 				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
// 
// 				break;
// 			}
// 		}
// 		if (err != enMCErrCode::MCEC_OK)
// 		{
// 			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
// 				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
// 				{
// 					nRetryChk++;
// 					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
// 				}
// 			}
// 			else
// 			{
// 				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
// 				{
// 					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
// 				}
// 			}
// 		}
// 	}
// 	break;
	case SEQ_SEMIAUTO_STEP_TLA_END:
		break;
		//(Vigneting)
	case SEQ_SEMIAUTO_STEP_VIGNETING_START:
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_VIGNETING_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_CHK);
			}
		}
		break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_CHK:
	{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}

			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_VIGNETING_CHK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST);
			}
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);


		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		Sleep(50);
		if (err == enMCErrCode::MCEC_OK)
		{
			if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82))
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_END);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_END:
		break;
		// (UNLOADING)
	case SEQ_SEMIAUTO_STEP_UNLOADING_START:
	{
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_UNLOADING_START)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_CHK);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_CHK:
	{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
// 			if (err == enMCErrCode::MCEC_OK)
// 			{
// 				err = MO_MoveScrew_Type(Screw_Wait);
// 				if (err != enMCErrCode::MCEC_OK)
// 				{
// 					break;
// 				}
// 			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
			}

			if (err == enMCErrCode::MCEC_OK)
			{

				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_UNLOADING_CHK)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP);
				}
			}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}

		if (err == enMCErrCode::MCEC_OK)
		{

			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF:
	{
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_END);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_END:
		break;

	default:
		break;
	}

	return err;
}

#elif(SET_INSPECTOR == SYS_TLA_B)
enMCErrCode CSeqSemiAuto::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dCurPos = 0.0f;
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	auto nStep = GetStep();

	switch ((enSeqSemiAutoStep)GetStep())
	{
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_IDEL:
		break;
	case enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_READY:
		break;

		// (LOADING)
	case SEQ_SEMIAUTO_STEP_LOADING_START:

		break;
	case SEQ_SEMIAUTO_STEP_LOADING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_Type(Screw_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
	
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
				if (IO_O_Chuck_Bwd() == false)
				{
					err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
					break;
				}
			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOADING_END:
		break;


	case SEQ_SEMIAUTO_STEP_LOCK_START:
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Out_Jig_Chk;
				break;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MO_MoveScrew_Type(Screw_Work);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			else
			{
				err = MCEC_Sensnor_Out_Jig_Chk;
				break;
			}
			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (IO_O_Chuck_Fwd() == false)
			{
				err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
				break;
			}
			Sleep(500);
			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);

		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) == FALSE)
			{
				err = MCEC_Sensnor_Product_L_NotChk;
			}
			else
			{
				err = MCEC_Sensnor_Product_R_NotChk;
			}
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_LOCK_MOVE:
		break;
	case SEQ_SEMIAUTO_STEP_LOCK_END:
		break;



	case SEQ_SEMIAUTO_STEP_PREVIEW_START:
		break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON:
	{
		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendReadyStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitReadyStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_END:
		break;

		//TLA 조정 관련 
	case SEQ_SEMIAUTO_STEP_TLA_START:
		nRetryChk = 0;
		break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK:
	{
		if (nRetryChk > stTeaching.nAdjust_RetryCnt)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
			}
		}
		else
		{
			if (IO_O_Screw_Bolt_Unlock() == false)
			{
				err = MCEC_Screw_Complete_TimeOut;
				break;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}

		if (err != enMCErrCode::MCEC_OK)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_ADJUST);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_ADJUST:
	{
		if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Spec) == FALSE) {
			dSpeed = (double)stMotion.nMotorSpeed[0][2];
			dTargetPos = m_UdpTester[0].m_dTesterFlag_TLA_PixelY * stTeaching.nAdjust_Wait;
			dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
			dTargetPos = dCurPos + dTargetPos;
			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
			{
				err = MCEC_Motion_Adjust_MoveError;
				break;
			}
			else if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
				}
			}
		}
		else
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_ADJUST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK);
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK:
	{
		if (IO_O_Screw_Bolt_L_Lock_1() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw1_Spec) == FALSE) {
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
				{
					nRetryChk++;
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
				}
			}
			else
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK:
	{
		if (IO_O_Screw_Bolt_L_Lock_2() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST:
	{
		TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
		UINT nRetCode = 0;
		m_UdpTester[0].m_bAckFlag_Inspection_TLA = FALSE;
		m_UdpTester[0].m_nTesterFlag_Inspection_TLAErrorCode = 100;
		m_UdpTester[0].m_dTesterFlag_TLA_PixelY = (5000 + stTeach.dAdjust_Offset_Y);
		m_UdpTester[0].m_dTesterFlag_TLA_PixelX = -5000;
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
			{
				SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);

				break;
			}
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
				{
					nRetryChk++;
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
				}
			}
			else
			{
				if ((enSeqSemiAutoStep)GetStep() == SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST)
				{
					SetNextStep(SEQ_SEMIAUTO_STEP_TLA_END);
				}
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_TLA_END:
		break;
		//(Vigneting)
	case SEQ_SEMIAUTO_STEP_VIGNETING_START:
		break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			// 			err = MO_MoveScrew_Type(Screw_Wait);
			// 			if (err != enMCErrCode::MCEC_OK)
			// 			{
			// 				break;
			// 			}
			// 			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(500);
			// 			if (IO_O_Socket_Unlock() == false)
			// 			{
			// 				err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
			// 				break;
			// 			}
			// 			if (IO_O_Chuck_Bwd() == false)
			// 			{
			// 				err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
			// 				break;
			// 			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_END:
		break;
		// (UNLOADING)
	case SEQ_SEMIAUTO_STEP_UNLOADING_START:
	{
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_CHK:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
			AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
		{
			err = MCEC_Sensnor_Out_Jig_Chk;
			break;
		}
		else if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_Type(Screw_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}

			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			Sleep(500);
			if (err == enMCErrCode::MCEC_OK)
			{
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
				if (IO_O_Chuck_Bwd() == false)
				{
					err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
					break;
				}
			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
		{
		}
		if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF:
	{
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
	}
	break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_END:
		break;

	default:
		break;
	}

	return err;
}

#endif

enMCErrCode CSeqSemiAuto::Conveyor1Run(__in enConv1SensorSelect mode)
{
	auto errcode = enMCErrCode::MCEC_OK;

	//IO_O_CV1_MoveMode(mode);
// 	errcode = MO_IsConveyor1_Moving();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? errcode : MO_MoveConveyor1_Run();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor1_Moving() : errcode;
	return errcode;
}

enMCErrCode CSeqSemiAuto::Conveyor2Run(__in enConv2SensorSelect mode)
{
	auto errcode = enMCErrCode::MCEC_OK;

	//IO_O_CV2_MoveMode(mode);
// 	errcode = MO_IsConveyor2_Moving();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? errcode : MO_MoveConveyor2_Run();
// 	Sleep(200);
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor2_Moving() : errcode;
	return errcode;
}

void CSeqSemiAuto::StartThread(__in int nIdx)
{
	m_bThread = TRUE;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner = this;
	pParam->nArg = nIdx;
	m_pThread = AfxBeginThread(Thread_Func, pParam, THREAD_PRIORITY_NORMAL);
}

UINT CSeqSemiAuto::Thread_Func(__in LPVOID lpVoid)
{
	CSeqSemiAuto* pThis = (CSeqSemiAuto*)((stThreadParam*)lpVoid)->pOwner;
	UINT	nArg = ((stThreadParam*)lpVoid)->nArg;
	
	if (NULL != lpVoid)
		delete lpVoid;

	pThis->m_ThdErrorCode = enMCErrCode::MCEC_OK;

	switch (nArg)
	{
	case 0:
	{
		auto st = DATAMANAGER->GetSemiAuto();
//		pThis->m_ThdErrorCode = pThis->MO_MoveChart_To_AngleType(st->Angle);
	}
		break;
		
	default:
		break;
	}

	pThis->m_bThread = FALSE;
	return 0;
}

enMCErrCode CSeqSemiAuto::EqpType_StartStandBy()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_DISTORTION:
// 	{
		// 차트를 이동하자		
		StartThread(0);
// 	}
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 	{
// 		IO_O_FlareLightAllOff();
// 	}
// 		break;
// 
// 	default:
// 		break;
// 	}

	return err;
}

enMCErrCode CSeqSemiAuto::EqpType_Init_Finalize()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_DISTORTION:
// 	{
		// 정해진 위치에 차트가 이동했는지확인하자..
		while (1)
		{
			if (m_bThread == FALSE)
				break;

			Sleep(10);
		}

		err = m_ThdErrorCode;
// 	}
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 		IO_O_FlareLightAllOff();
// 		break;
// 
// 	case SYS_COLOR_CAL:
// 		if (!IO_O_Light_ShutterClose())
// 			err = MCEC_IO_LIGHT_Shutter_Close;
// 		break;
// 
// 	default:
// 		break;
// 	}

	return err;
}

enMCErrCode CSeqSemiAuto::EqpType_Inspection()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	int nRetCode = -1;
	CString sz;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_DISTORTION:
// 	{
		SEMCO_SendStartTest(m_Portid, eComMode_Insp_TLA);
		SEMCO_WaitStartTestAck(m_Portid, eComMode_Insp_TLA, 40000, (UINT&)nRetCode);

		Log(_T("Recv Image Capture Code : %d"), nRetCode);
		
// 		if (nRetCode == 0)
// 		{
// 			SEMCO_SendStartTest(m_Portid);
// 			SEMCO_WaitStartTestAck(m_Portid, 0, 45000, (UINT&)nRetCode);
// 
// 			Log(_T("Recv Calibration Code : %d"), nRetCode);
// 		}		
// 	}
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 	{
// 		IO_O_FlareLightAllOff();
// 		for (auto i = 0; i < 10; i++)
// 		{
// 			nRetCode = -1;
// 
// 			// io 변경
// 			IO_O_FlareLightOn(i);			
// 
// 			SEMCO_SendStartTest(m_Portid,i);
// 			SEMCO_WaitStartTestAck(m_Portid, i, 30000, (UINT&)nRetCode);
// 
// 			IO_O_FlareLightOff(i);
// 			Log(_T("Recv GhostFlare Ack Code: %d"), nRetCode);			
// 		}
// 	}
// 	break;
// 
// 	default:
// 		SEMCO_SendStartTest(m_Portid);
// 		SEMCO_WaitStartTestAck(m_Portid, 0, 30000, (UINT&)nRetCode);
// 
// 		Log(_T("Recv Inpsection Ack Code: %d"), nRetCode);
// 		WRITE_MAIN_LOG(sz);
// 		break;
// 	}

	return err;
}

enMCErrCode CSeqSemiAuto::EqpType_MoveZInspectionPos(__in InspectionPortType port)
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_DISTORTION:
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 	{
		auto st = DATAMANAGER->GetSemiAuto();
		
		// 180 모델일 경우
// 		if (st->Angle == enAngleType::Angle_180)
// 			err = MO_MoveZ_To_180Inspection(port);
// 		else
// 			err = MO_MoveZ_To_Inspection1(port);
// 	}
// 		break;
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
