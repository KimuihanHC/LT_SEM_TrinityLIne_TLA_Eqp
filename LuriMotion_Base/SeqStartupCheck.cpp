#include "pch.h"
#include "SeqStartupCheck.h"
#include "LuriMotion_BaseDlg.h"
#include "Util/BasicTimer.h"

CSeqStartupCheck::CSeqStartupCheck()
{
	m_Portid = InspectionPortType::Left;
	SetNextStep(SEQ_STARTUPCHECK_STEP_IDEL);

	m_pThread = NULL;
	m_bThread = FALSE;
}

CSeqStartupCheck::~CSeqStartupCheck()
{

}

void CSeqStartupCheck::SetPortId(__in InspectionPortType port)
{
	m_Portid = port;
}

InspectionPortType CSeqStartupCheck::GetPortid()
{
	return m_Portid;
}

bool CSeqStartupCheck::Start()
{
	DeleteThread();
	return __super::Start();
}

UINT CSeqStartupCheck::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	//while (IsThreadStart())
	while (SEQUENCE->IsThread_StartupCheck())
	{
		Sleep(10);

		if (IsThreadStop())
			continue;

		if (IsError())
			continue;

		if (IsChangeStep())
		{
			CString szLog;

			szLog.Format(_T("StartupCheck =========> %s(start)"), g_szSeqName_StartupCheck[GetStep()]);
			WRITE_MAIN_LOG(szLog);

			StartTimer();
			SetStatus(enSeqStatus::Status_Testing);
		}

		if (GetStatus() != enSeqStatus::Status_Complete)
		{
			lt::CBasicTimer timer;

			err = StartOperation_Step();

			CString szElaped;
			szElaped.Format(_T("StartupCheck =========> %s(end),ResultCode:%d,Elaped : %d ms"), g_szSeqName_StartupCheck[GetOldStep()], err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
			WRITE_MAIN_LOG(szElaped);
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
			if (GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_READY
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END
				|| GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_END
				)
			{
				ForceStop();
				break;
			}
		}
	}
	return 0;
}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
enMCErrCode CSeqStartupCheck::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	bool Sensnor[10] = { 0,0, };
	auto nStep = GetStep();

	switch ((enSeqStartupCheckStep)GetStep())
	{
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_READY:
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
			if (m_Portid == InspectionPortType::Left) {
				if (IO_I_JIG_L_Unlock() == TRUE)
				{
					if (IO_O_JIG_L_Unlock() == FALSE)
					{
						err = MCEC_ScrewCheckJig_Unlock_L_TimeOut;
					}
				}
			}
			else
			{
				if (IO_I_JIG_R_Unlock() == TRUE)
				{
					if (IO_O_JIG_R_Unlock() == FALSE)
					{
						err = MCEC_ScrewCheckJig_Unlock_R_TimeOut;
					}
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_START:
	{

		if (m_Portid == InspectionPortType::Left) {
			Sensnor[0] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
			Sensnor[1] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
			if (!AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				!AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_L_Type(Screw_Commencement);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
				else
				{
					Sleep(500);
				}
			}
	
		}
		else if (m_Portid == InspectionPortType::Right)
		{
			Sensnor[0] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
			Sensnor[1] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
			if (!AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				!AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_R_Type(Screw_Commencement);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
				else
				{
					Sleep(500);
				}
			}
		}
		if (m_Portid == InspectionPortType::Left) {
			if (IO_O_Screw_Bolt_L_Lock_1() == FALSE)
			{
				err = MCEC_Screw_Complete_L_TimeOut;
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{
			if (IO_O_Screw_Bolt_R_Lock_1() == FALSE)
			{
				err = MCEC_Screw_Complete_R_TimeOut;
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_END:
	{

	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_START:
	{

		if (m_Portid == InspectionPortType::Left) {

			if (!AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) ||
				!AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_L_Type(Screw_Commencement);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{

			if (!AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) ||
				!AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}			
			if (err == enMCErrCode::MCEC_OK)
			{
				err = MO_MoveScrew_R_Type(Screw_Commencement);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
				else
				{
					Sleep(500);
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (m_Portid == InspectionPortType::Left) {
				if (IO_O_Screw_Bolt_L_Lock_2() == FALSE)
				{
					err = MCEC_Screw_Complete_L_TimeOut;
				}

				if (err == enMCErrCode::MCEC_OK)
				{
					err = MO_MoveScrew_L_Type(Screw_Wait);
					if (err != enMCErrCode::MCEC_OK)
					{
						break;
					}
					else
					{
						Sleep(500);
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if (IO_I_JIG_L_Unlock() == FALSE)
					{
						if (IO_O_JIG_L_Unlock() == FALSE)
						{
							err = MCEC_ScrewCheckJig_Unlock_L_TimeOut;
						}
					}
				}
			}
			else if (m_Portid == InspectionPortType::Right)
			{
				if (IO_O_Screw_Bolt_R_Lock_2() == FALSE)
				{
					err = MCEC_Screw_Complete_R_TimeOut;
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					err = MO_MoveScrew_R_Type(Screw_Wait);
					if (err != enMCErrCode::MCEC_OK)
					{
						break;
					}
					else
					{
						Sleep(500);
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if (IO_I_JIG_R_Unlock() == FALSE)
					{
						if (IO_O_JIG_R_Unlock() == FALSE)
						{
							err = MCEC_ScrewCheckJig_Unlock_R_TimeOut;
						}
					}
				}
				
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_END:
	{

	}
	break;

	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START:
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
			// 			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(300);
			// 			if (IO_O_Chuck_Fwd() == false)
			// 			{
			// 				err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
			// 				break;
			// 			}
			// 			Sleep(300);
			// 			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(300);
			// 			err = MO_MoveScrew_Type(Screw_Work);
			// 			if (err != enMCErrCode::MCEC_OK)
			// 			{
			// 				break;
			// 			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END:
		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
		// 		if (IO_O_Screw_Bolt_Unlock() == FALSE)
		// 		{
		// 			err = MCEC_Screw_Complete_TimeOut;
		// 		}
		break;


	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_START:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}
		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_END:
		break;



	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_START:
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
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_TLA))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START:
	{
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
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST:
	{
		UINT nRetCode = 0;
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_Vigneting46))
		{
			/*bRet = false;
			break;*/
		}

		 nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}


		if (err == enMCErrCode::MCEC_OK)
		{
			if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_Vigneting82))
			{
				/*bRet = false;
				break;*/
			}

			nRetCode = 0;
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
			}

		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START:
	{
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
				err = MO_MoveVignetting_X_Type(X_Test_02);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_02);
				if (err != enMCErrCode::MCEC_OK)
				{
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
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, eComMode_Insp_Vigneting82))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_START:
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

			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
			{
			}


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
					if (IO_O_JIG_L_Lock(FALSE) == FALSE)
					{
						err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
					}
				
			}
			if (err == enMCErrCode::MCEC_OK)
			{
					if (IO_O_JIG_R_Lock(FALSE) == FALSE)
					{
						err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
					}
			}
			UINT nRetCode = 0;
			if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_END:
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	return err;
}

#elif(SET_INSPECTOR == SYS_TLA_OQC)
enMCErrCode CSeqStartupCheck::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	auto nStep = GetStep();

	switch ((enSeqStartupCheckStep)GetStep())
	{
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_READY:
	{
		err = MO_MoveVignetting_R_Type(R_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		err = MO_MoveVignetting_X_Type(X_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		if (IO_O_Socket_Unlock() == false)
		{
			err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_READY)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_01_START);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_START:
	{

		if (m_Portid == InspectionPortType::Left) {

			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{

			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}

		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_SCREW_01_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_01_END);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_END:
	{
// 		if (m_Portid == InspectionPortType::Left) {
// 			if (IO_O_Screw_Bolt_L_Lock_1() == FALSE)
// 			{
// 				err = MCEC_Screw_Complete_L_TimeOut;
// 			}
// 		}
// 		else if (m_Portid == InspectionPortType::Right)
// 		{
// 			if (IO_O_Screw_Bolt_R_Lock_1() == FALSE)
// 			{
// 				err = MCEC_Screw_Complete_R_TimeOut;
// 			}
// 		}
// 		if (err == enMCErrCode::MCEC_OK)
// 		{
// 			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_SCREW_01_END)
// 			{
// 				SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_02_START);
// 			}
// 		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_START:
	{

		if (m_Portid == InspectionPortType::Left) {

			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_SCREW_02_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_02_END);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_END:
	{
// 		if (m_Portid == InspectionPortType::Left) {
// 			if (IO_O_Screw_Bolt_L_Lock_2() == FALSE)
// 			{
// 				err = MCEC_Screw_Complete_L_TimeOut;
// 			}
// 		}
// 		else if (m_Portid == InspectionPortType::Right)
// 		{
// 			if (IO_O_Screw_Bolt_R_Lock_2() == FALSE)
// 			{
// 				err = MCEC_Screw_Complete_R_TimeOut;
// 			}
// 		}
// 		if (err == enMCErrCode::MCEC_OK)
// 		{
// 			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_SCREW_02_END)
// 			{
// 				SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START);
// 			}
// 		}
	}
	break;

	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START:
	{

		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);

		err = MO_MoveVignetting_R_Type(R_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		err = MO_MoveVignetting_X_Type(X_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		if (IO_O_Socket_Unlock() == false)
		{
			err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START)
				{
					SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT);
				}
 		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END:

		break;


	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_START:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}
		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);


		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_PREVIEW_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_PREVIEW_END);
			}
		}
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_END:
		break;



	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_START:
	{

		err = MO_MoveVignetting_R_Type(R_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		err = MO_MoveVignetting_X_Type(X_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_TLA_TEST_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_TLA_TEST);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_TLA))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_TLA_TEST)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_TLA_TEST_END);
			}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START:
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
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		UINT nRetCode = 0;

		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code 46 : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		Sleep(50);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82))
		{
		}
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}

		sz.Format(_T("SEMCO Send Test TLA Start Ack code 82: %d"), nRetCode);
		WRITE_MAIN_LOG(sz);

		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END);
			}
		}
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START:
	{

		err = MO_MoveVignetting_R_Type(R_Wait);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		err = MO_MoveVignetting_X_Type(X_Test_02);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
		err = MO_MoveVignetting_R_Type(R_Test_02);
		if (err != enMCErrCode::MCEC_OK)
		{
			break;
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_START:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
		{
		}
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
// 			err = MO_MoveScrew_Type(Screw_Wait);
// 			if (err != enMCErrCode::MCEC_OK)
// 			{
// 				break;
// 			}
			if (err == enMCErrCode::MCEC_OK)
			{
				UINT nRetCode = 0;
				if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
				}
				POWERSUPPLY_VoltOff(InspectionPortType::Left);
				POWERSUPPLY_VoltOff(InspectionPortType::Right);
				if (IO_O_Socket_Unlock() == false)
				{
					err = MCEC_Sensnor_Product_Socket_Unlock_TimeOut;
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqStartupCheckStep)GetStep() == SEQ_STARTUPCHECK_STEP_UNLOCK_START)
			{
				SetNextStep(SEQ_STARTUPCHECK_STEP_UNLOCK_END);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_END:
	{
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	return err;
	}
}

#elif(SET_INSPECTOR == SYS_TLA_B)
enMCErrCode CSeqStartupCheck::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	auto nStep = GetStep();

	switch ((enSeqStartupCheckStep)GetStep())
	{
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_READY:
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_START:
	{

		if (m_Portid == InspectionPortType::Left) {

			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) &&
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			err = MO_MoveScrew_L_Type(Screw_Commencement);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{

			if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) &&
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			err = MO_MoveScrew_R_Type(Screw_Commencement);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_END:
	{
		if (m_Portid == InspectionPortType::Left) {
			if (IO_O_Screw_Bolt_L_Lock_1() == FALSE)
			{
				err = MCEC_Screw_Complete_L_TimeOut;
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{
			if (IO_O_Screw_Bolt_R_Lock_1() == FALSE)
			{
				err = MCEC_Screw_Complete_R_TimeOut;
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_START:
	{

		if (m_Portid == InspectionPortType::Left) {

			if (AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR) &&
				AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_L_Lock() == FALSE)
			{
				if (IO_O_JIG_L_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_L_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			err = MO_MoveScrew_L_Type(Screw_Commencement);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{

			if (AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR) &&
				AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR))
			{
				err = MCEC_Sensnor_In_Jig_Chk;
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			if (IO_I_JIG_R_Lock() == FALSE)
			{
				if (IO_O_JIG_R_Lock() == FALSE)
				{
					err = MCEC_ScrewCheckJig_Lock_R_TimeOut;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
			err = MO_MoveScrew_R_Type(Screw_Commencement);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_END:
	{
		if (m_Portid == InspectionPortType::Left) {
			if (IO_O_Screw_Bolt_L_Lock_2() == FALSE)
			{
				err = MCEC_Screw_Complete_L_TimeOut;
			}
		}
		else if (m_Portid == InspectionPortType::Right)
		{
			if (IO_O_Screw_Bolt_R_Lock_2() == FALSE)
			{
				err = MCEC_Screw_Complete_R_TimeOut;
			}
		}
	}
	break;

	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START:
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
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT:
	{
		if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR) && AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
		//if (TRUE)
		{
			if (IO_O_Socket_Lock() == false)
			{
				err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
				break;
			}
			// 			AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(300);
			// 			if (IO_O_Chuck_Fwd() == false)
			// 			{
			// 				err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
			// 				break;
			// 			}
			// 			Sleep(300);
			// 			AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
			// 			Sleep(300);
			// 			err = MO_MoveScrew_Type(Screw_Work);
			// 			if (err != enMCErrCode::MCEC_OK)
			// 			{
			// 				break;
			// 			}
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR))
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
		}
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END:
		POWERSUPPLY_VoltOn(InspectionPortType::Left);
		POWERSUPPLY_VoltOn(InspectionPortType::Right);
		// 		if (IO_O_Screw_Bolt_Unlock() == FALSE)
		// 		{
		// 			err = MCEC_Screw_Complete_TimeOut;
		// 		}
		break;


	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_START:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}
		CString sz;
		sz.Format(_T("SEMCO Send Preview Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_END:
		break;



	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_START:
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
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_TLA)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START:
	{
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
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START:
	{
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
				err = MO_MoveVignetting_X_Type(X_Test_02);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
				err = MO_MoveVignetting_R_Type(R_Test_02);
				if (err != enMCErrCode::MCEC_OK)
				{
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
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST:
	{
		SEMCO_Reset(InspectionPortType::Left);
		if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting82)
		{
			/*bRet = false;
			break;*/
		}

		UINT nRetCode = 0;
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 30000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
		}

		CString sz;
		sz.Format(_T("SEMCO Send Test TLA Start Ack code : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);
		break;
	}
	break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END:
		break;
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_START:
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

			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendPreviewStop(InspectionPortType::Left))
			{
			}


			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}

			UINT nRetCode = 0;
			if (!SEMCO_WaitPreviewStopAck(InspectionPortType::Left, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
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
	case enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_END:
		POWERSUPPLY_VoltOff(InspectionPortType::Left);
		POWERSUPPLY_VoltOff(InspectionPortType::Right);
		break;
	}
	return err;
}

#endif

void CSeqStartupCheck::StartThread(__in int nIdx)
{
	m_bThread = TRUE;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner = this;
	pParam->nArg = nIdx;
	m_pThread = AfxBeginThread(Thread_Func, pParam, THREAD_PRIORITY_NORMAL);
}

UINT CSeqStartupCheck::Thread_Func(__in LPVOID lpVoid)
{
	CSeqStartupCheck* pThis = (CSeqStartupCheck*)((stThreadParam*)lpVoid)->pOwner;
	UINT	nArg = ((stThreadParam*)lpVoid)->nArg;

	if (NULL != lpVoid)
		delete lpVoid;

	pThis->m_ThdErrorCode = enMCErrCode::MCEC_OK;

	switch (nArg)
	{
	case 0:
	{
		auto st = DATAMANAGER->GetSemiAuto();
	//	pThis->m_ThdErrorCode = pThis->MO_MoveChart_To_AngleType(st->Angle);
	}
	break;

	default:
		break;
	}

	pThis->m_bThread = FALSE;
	return 0;
}

enMCErrCode CSeqStartupCheck::EqpType_StartStandBy()
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
// 	break;
// 
// 	case SYS_GHOSTFLARE:
// 	{
// 		IO_O_FlareLightAllOff();
// 	}
// 	break;
// 
// 	default:
// 		break;
// 	}

	return err;
}


enMCErrCode CSeqStartupCheck::EqpType_Init_Finalize()
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
// 	break;
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

enMCErrCode CSeqStartupCheck::EqpTypeStartupCheck()
{
	auto errcode = enMCErrCode::MCEC_OK;

//	auto EqpType = SET_INSPECTOR;

	float Ev = 0.0f;
	float Tcp = 0.0f;

	bool bReadOk = true;

	CString sz;
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 	{
		UpdateUI_CL500A_Reset(GetPortid());

		for (int idx = 0; idx < 20; idx++)
		{
			LIGHT_Color_Cal_On(idx);

			Sleep(500);

			if (!CL500A_COLORCAL_ReadData(Ev, Tcp))
			{
				Log(_T("Error CL500A MEASURE [CH%d]"), idx);	
				bReadOk = false;
			}
			else
			{
				if (GetPortid() == InspectionPortType::Left)
					Log(_T("(L) CL500A Measure[CH%d] ColorTemper :%d, Lux : %f"), idx, Tcp, Ev);
				else
					Log(_T("(R) CL500A Measure[CH%d] ColorTemper :%d, Lux : %f"), idx, Tcp, Ev);

				bReadOk = true;
			}

			// Ui Update
			UpdateUI_CL500A_Data(GetPortid(), idx, Tcp, Ev, bReadOk);
		}

		// 파일 저장
		SaveReportFile();
// 	}		
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 	{
// 		float fData = 0.0f;
// 
// 		UpdateUI_CL200A_Reset(GetPortid());
// 
// 		if (!CL200A_HOTPIXEL_ReadData(fData))
// 		{
// 			bReadOk = false;
// 			Log(_T("Error CL200A MEASURE"));
// 		}			
// 		else
// 		{
// 			CString sz;
// 			if (GetPortid() == InspectionPortType::Left)
// 				Log(_T("(L) CL200A Measure : %f"), fData);
// 			else
// 				Log(_T("(R) CL200A Measure : %f"), fData);
// 
// 			bReadOk = true;
// 		}
// 
// 		// Ui Update
// 		UpdateUI_CL200A_Data(GetPortid(), fData, bReadOk);
// 	}		
// 		break;
// 
// 	default:
// 		break;
// 	}

	return errcode;
}

enMCErrCode CSeqStartupCheck::EqpType_MoveZInspectionPos(__in InspectionPortType port)
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
// 	break;
// 
// 	case SYS_COLOR_CAL:
// 	{
// 		// CL500A 측정위치로 이동 
// 		err = MO_MoveZ_To_CL500A(port);
// 	}
// 		break;
// 
// 	default:
// 		err = MO_MoveZ_To_Inspection1(port);
// 		break;
// 	}

	return err;
}


enMCErrCode CSeqStartupCheck::Conveyor1Run(__in enConv1SensorSelect mode)
{
	auto errcode = enMCErrCode::MCEC_OK;

//	IO_O_CV1_MoveMode(mode);
// 	errcode = MO_IsConveyor1_Moving();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? errcode : MO_MoveConveyor1_Run();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor1_Moving() : errcode;
	return errcode;
}

enMCErrCode CSeqStartupCheck::Conveyor2Run(__in enConv2SensorSelect mode)
{
	auto errcode = enMCErrCode::MCEC_OK;

//	IO_O_CV2_MoveMode(mode);
// 	errcode = MO_IsConveyor2_Moving();
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? errcode : MO_MoveConveyor2_Run();
// 	Sleep(200);
// 	errcode = (errcode == enMCErrCode::MCEC_OK) ? MO_IsConveyor2_Moving() : errcode;
	return errcode;
}

void CSeqStartupCheck::SaveReportFile()
{
	// 일자, 시간, 작업자 레벨, 작업자, 측정 채널값 색온도, Lux
	DATAMANAGER->GetSemiAuto();
}
