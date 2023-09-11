
#include "pch.h"
#include "SeqPickerMgr.h"
#include "LuriMotion_BaseDlg.h"

#include "Util/BasicTimer.h"

CSeqPickerMgr::CSeqPickerMgr()
{
	ResetWorkEnd();

	SetNextStep(SEQ_PICKERMGR_STEP_IDEL);
	SetMode();
	startmMotionChk = clock();
}

CSeqPickerMgr::~CSeqPickerMgr()
{

}

void CSeqPickerMgr::SetMode()
{
	SetMode(enPickUseType::Use_LR);
	SetAutoChange(false);
}

bool CSeqPickerMgr::Start()
{
// 	for (auto idx = 0; idx < 2; idx++)
// 	{
// 		m_SeqPicker[idx].SetPortId((InspectionPortType)idx);
// 		m_SeqPicker[idx].SetPortSkip(m_bPortSkip[(int)idx]);
// 		m_SeqPicker[idx].Start();
// 	}	

	__super::Start();
	return true;
}

bool CSeqPickerMgr::Stop()
{
// 	for (auto idx = 0; idx < 2; idx++)
// 		m_SeqPicker[idx].Stop();
	
	__super::Stop();
	return true;
}

void CSeqPickerMgr::ErrorClear()
{
// 	for (auto idx = 0; idx < 2; idx++)
// 		m_SeqPicker[idx].ErrorClear();
	
	m_PickErrorCode = enMCErrCode::MCEC_OK;
	m_bPickErrorCode = false;
	__super::ErrorClear();
}

void CSeqPickerMgr::Reset()
{
	SetMode();
// 	for (auto idx = 0; idx < 2; idx++)
// 		m_SeqPicker[idx].Reset();
	
	m_PickErrorCode = enMCErrCode::MCEC_OK;
	m_bPickErrorCode = false;
	__super::Reset();
}

bool CSeqPickerMgr::ForceStop()
{
// 	for (auto idx = 0; idx < 2; idx++)
// 		m_SeqPicker[idx].ForceStop();

	__super::ForceStop();
	return true;
}

UINT CSeqPickerMgr::RunProcess()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	startmMotionChk = clock();

	while (IsThreadStart())
	{
		Sleep(50);
		
		if (IsThreadStop())
			continue;

		if(IsError())
			continue;

		if (m_bPickErrorCode)
			continue;
		if (IO_I_AREA() == FALSE)
		{
			err = enMCErrCode::MCEC_IO_AREA_Sensor;
		}
			//continue;
		
//		Wait();
		if (err == enMCErrCode::MCEC_OK)
		{
			if (IsChangeStep() )
			{
	//			CString szLog;

				Log(_T("Picker Mgr =========> %s(start)"), g_szSeqName_PickerMgr[GetStep()]);
				SemcoLog(_T("O,-,-,Run,TLA:%s,Start,"), g_szSeqName_PickerMgr[GetStep()]);
	//			WRITE_MAIN_LOG(szLog);
			
				StartTimer();
				SetStatus(enSeqStatus::Status_Testing);
			}

			if (GetStatus() != enSeqStatus::Status_Complete)
			{
				lt::CBasicTimer timer;

				err = StartOperation_Step();

	//			CString szElaped;
				Log(_T("Picker Mgr =========> %s(end), ResultCode:%d, Elaped:%d ms"), 
					g_szSeqName_PickerMgr[GetOldStep()],err, timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
				SemcoLog(_T("O,-,-,Run,TLA:%s,End,Elaped=%dmsec"), g_szSeqName_PickerMgr[GetStep()], timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds));
	//			WRITE_MAIN_LOG(szElaped);
			}
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			SEMCO_Reset(InspectionPortType::Left);
			SEMCO_SendPreviewStop(InspectionPortType::Left);

			Error(err);

			m_PickErrorCode = err;
			m_bPickErrorCode = true;
			SetStatus(enSeqStatus::Status_Error);
		}
		else
		{

			SetStatus(enSeqStatus::Status_Complete);
		
		}	
		
//		Lock();
	}
	return 0;
}

enMCErrCode CSeqPickerMgr::WaitCompletePickerStep(__in InspectionPortType port, __in enSeqPickerStep step)
{
	int CheckPort = (int)port;
	enMCErrCode err = enMCErrCode::MCEC_OK;
	bool bComplete = false;

	do
	{
		if (m_SeqPicker[CheckPort].IsStatusComplete(step))
		{
			bComplete = true;
			break;
		}

		// 에러가 발생했을 경우
		if (m_SeqPicker[CheckPort].IsError())
		{
			break;
		}

		Sleep(10);
	} while (GetElapTime() < 45000);

	if (!bComplete)
	{
		if (m_SeqPicker[CheckPort].IsError())
			err = m_SeqPicker[CheckPort].GetErrorCode();
		else
			err = MCEC_Unknown;
	}		

	return err;
}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
enMCErrCode CSeqPickerMgr::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_IDEL:	// 준비
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STOP:	// 준비
		SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_MAIN:	// 시작
		if (nJobCheck[0] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_START); break;
		}
		else if (nJobCheck[1] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_START); break;
		}
		else if (nJobCheck[2] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START); break;
		}
		else if (nJobCheck[3] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START); break;
		}
		else if (nJobCheck[4] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START); break;
		}
		// 		else if (nJobCheck[5] == FALSE)
		// 		{
		// 			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START);
		// 		}
		else if (nJobCheck[0] == TRUE && nJobCheck[1] == TRUE && nJobCheck[2] == TRUE && nJobCheck[3] == TRUE && nJobCheck[4] == TRUE)
		{
			SEQUENCE->nPASS_NG = 1;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
		}

		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:
		err = StartReady();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		err = StartLock();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		err = StartInsp_Power();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
		err = StartInsp_Prewiew();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		err = StartInsp_OC();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
		err = StartInsp_Vignetting_46();
		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
// 		err = StartInsp_Vignetting_82();
// 		break;
	default:
		break;
	}
	//SetStatus(enSeqStatus::Status_Testing);
	return err;
}
enMCErrCode CSeqPickerMgr::StartReady()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	auto pMain = CDlgMain::Get_Instance();
	auto pSide = CDlgSide::Get_Instance();
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_START)
		{
			reTry = 0;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			UINT nRetCode = 0;
			if (m_UdpTester[0].m_bConnected)
			{
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
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_POWER_OFF);
					break;
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_POWER_OFF);
				break;
			}
		}

	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			device.POWERSUPPLY_VoltOff(InspectionPortType::Left);
			device.POWERSUPPLY_VoltOff(InspectionPortType::Right);
		}
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_POWER_OFF)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK1); //
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
			{
				err = MO_MoveVignetting_R_Type(R_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Screw_L_Type(Screw_Wait) == FALSE && Check_Pos_Screw_R_Type(Screw_Wait) == FALSE
				&& Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				err = MO_MoveWait_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Screw_L_Type(Screw_Wait) == FALSE || Check_Pos_Screw_R_Type(Screw_Wait) == FALSE)
			{
				err = MO_MoveScrew_Type(Screw_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK2);
				Sleep(500);
				break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
		IO_O_Chuck_Bwd(FALSE);
		Sleep(500);
		IO_O_Socket_Unlock(FALSE);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK2)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_END);
			//Sleep(300);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:

// 		if (COMMON->GetOperationMode() == OperationMode::SemiAuto)
// 		{
// 
// 		}
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			if (SEQUENCE->m_nbuttonFlagIs == TRUE)
			{
				nJobCheck[0] = FALSE;
				nJobCheck[1] = FALSE;
				nJobCheck[2] = FALSE;
				nJobCheck[3] = FALSE;
				nJobCheck[4] = FALSE;
				nJobCheck[5] = FALSE;
				SetNextStep(SEQ_PICKERMGR_STEP_IDEL);
				SEQUENCE->SetRunMode(eStop);
				COMMON->SetMcStatus(eStop);
			}
			else
			{
				nJobCheck[0] = FALSE;
				nJobCheck[1] = FALSE;
				nJobCheck[2] = FALSE;
				nJobCheck[3] = FALSE;
				nJobCheck[4] = FALSE;
				nJobCheck[5] = FALSE;
				SetNextStep(SEQ_PICKERMGR_STEP_MAIN);
			}
	
			SEQUENCE->nPASS_NG = 100;
			if (stEq.nLanguage == 0)
				strTemp.Format(_T("DRY RUN 버튼[UI] 누름."));
			else
				strTemp.Format(_T("DRY Run Restart."));
			COMMON->Write_Main_Log(strTemp);
		}
		else
		{
			SEQUENCE->SetRunMode(eStop);
// 			while (1)
// 			{
// 				if (!SEQUENCE->IsThreadRun())
// 					break;
// 
// 				COMMON->DoEvent(10);
// 			}

// 			COMMON->SetOperationMode(OperationMode::Manual);
// 			pSide->SetMenuClickColor(pSide->enMenu::Menu_Manual);
// 			pMain->EqpTypeShowOperation();

			//SEQUENCE->Stop_Run();
			COMMON->SetMcStatus(eStop);


			if (stEq.nLanguage == 0)
				strTemp.Format(_T("STOP 버튼[UI] 누름."));
			else
				strTemp.Format(_T("STOP BUTTON[UI] PRESSED."));
			COMMON->Write_Main_Log(strTemp);
			m_UdpTester[0].Reset_TesterFlag();
			SetNextStep(SEQ_PICKERMGR_STEP_IDEL);
		}



		//쓰래드 종료
		break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartLock()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			nRet[0] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
			nRet[1] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
// 			nRet[0] = TRUE;
// 			nRet[1] = TRUE;
		}
		else
		{
			nRet[0] = TRUE;
			nRet[1] = TRUE;
		}
		nRet[2] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
		nRet[3] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
		nRet[4] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
		nRet[5] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
		nRet[6] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);
		nRet[7] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
		if (nRet[0] == FALSE || nRet[1] == FALSE)
		{//제품이 정상적으로 감지 안었습니다.
			if (nRet[0] == FALSE)
			{
				err = MCEC_Sensnor_Product_L_NotChk;
			}
			else
			{
				err = MCEC_Sensnor_Product_R_NotChk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[2] == TRUE || nRet[3] == TRUE))
		{//Jig 가 정상적으로 제거 되지 않았습니다. 
			if (nRet[2] == FALSE)
			{
				err = MCEC_Sensnor_Jig_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[4] == TRUE || nRet[5] == TRUE))
		{//Jig 가 전진 되었습니다. 
			if (nRet[4] == FALSE)
			{
				err = MCEC_Sensnor_Jig_IN_Fwd_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_IN_Fwd_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[6] == FALSE || nRet[7] == FALSE))
		{//Jig 가 정상적으로 후진 되어있지 않습니다.  
			if (nRet[6] == FALSE)
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	{
		if (IO_O_Socket_Lock(FALSE) == false)
		{
			err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
			break;
		}
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		if (Check_Pos_Screw_L_Type(Screw_Work) == FALSE || Check_Pos_Screw_R_Type(Screw_Work) == FALSE)
		{
			err = MO_MoveScrew_Type(Screw_Work);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
			{
				err = MO_MoveVignetting_R_Type(R_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2);
				Sleep(500);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:
		nRet[0] = IO_O_Chuck_Fwd();
		if (nRet[0] == TRUE)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP);
				Sleep(300); break;
			}
		}
		else
		{
			err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
		AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		Sleep(500);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP)
		{
			nJobCheck[0] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK)
		{

			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_END); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;

		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Power()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			device.POWERSUPPLY_VoltOn(InspectionPortType::Left);
			device.POWERSUPPLY_VoltOn(InspectionPortType::Right);
		}
		nJobCheck[1] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);

			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Prewiew()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	{	
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			UINT nRetCode = 0;
			if (m_UdpTester[0].m_bConnected)
			{
				if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
				{
				}
				if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END);
			break;
		}

	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
	{
		nJobCheck[2] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_OC()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dSettingPos = 0.0f;
	double dCurPos = 0.0f;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
		reTry = 0;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (reTry > stTeaching.nAdjust_RetryCnt)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
				{
					SEQUENCE->nPASS_NG = 0;
					SEQUENCE->StrNG.Format(_T("NG : Adjst Retry Over"));
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
				}
			}
			else
			{
				if (IO_O_Screw_Bolt_Unlock() == false)
				{
					//err = MCEC_Screw_Complete_TimeOut;
					SEQUENCE->StrNG.Format(_T("NG : Screw timeout."));
					SEQUENCE->nPASS_NG = 0;
// 					_T("Screw completion timeout occurred."),//MCEC_Screw_Complete_TimeOut,
// 						_T("Screw completion timeout occurred. Lift"),//MCEC_Screw_Complete_L_TimeOut,
// 						_T("Screw completion timeout occurred. Right"),//MCEC_Screw_Complete_R_TimeOut,
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
					}

					break;
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END); break;
					}
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END); break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
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
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}

				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
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
					err = MCEC_Motion_Adjust_MoveError;
					break;
				}
				else
				{
					AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
						break;
					}
				}
			}
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
				{					
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_END); break;
				}
			}
		}
		else
		{
			dSpeed = (double)stMotion.nMotorSpeed[0][2];
			dTargetPos = -36.0;
			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
			{
				err = MCEC_Motion_Adjust_MoveError;
				break;
			}
			AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
			COMMON->DoEvent(1000);
			dTargetPos = 36.0;
			if (AJIN_MO->MoveAxis_Wait(TILT_ADJUST_R, dTargetPos, dSpeed) == FALSE)
			{
				err = MCEC_Motion_Adjust_MoveError;
				break;
			}
			AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_END); break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (IO_O_Screw_Bolt_Lock_1(TRUE) == false)
			{
				//err = MCEC_Screw_Complete_TimeOut;

				SEQUENCE->StrNG.Format(_T("NG : Screw timeout."));
				SEQUENCE->nPASS_NG = 0;
				// 					_T("Screw completion timeout occurred."),//MCEC_Screw_Complete_TimeOut,
				// 						_T("Screw completion timeout occurred. Lift"),//MCEC_Screw_Complete_L_TimeOut,
				// 						_T("Screw completion timeout occurred. Right"),//MCEC_Screw_Complete_R_TimeOut,
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
				}
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END);
					break;
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END);
				break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
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
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw1_Spec) == FALSE) {
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
					{
						reTry++;
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START);
						break;
					}
				}
				else
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START);
						break;
					}
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (IO_O_Screw_Bolt_Lock_2(TRUE) == false)
			{
				//err = MCEC_Screw_Complete_TimeOut;

				SEQUENCE->StrNG.Format(_T("NG : Screw timeout."));
				SEQUENCE->nPASS_NG = 0;
				// 					_T("Screw completion timeout occurred."),//MCEC_Screw_Complete_TimeOut,
				// 						_T("Screw completion timeout occurred. Lift"),//MCEC_Screw_Complete_L_TimeOut,
				// 						_T("Screw completion timeout occurred. Right"),//MCEC_Screw_Complete_R_TimeOut,
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
				}
				break;
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END);
					break;
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END:
		if (SEQUENCE->nADJUST_Reverse == 0)
		{
			dSettingPos = stTeaching.nAdjust_Reverse;
			dSettingPos = dSettingPos / 1000;
			dTargetPos = dSettingPos;
			dCurPos = AJIN_MO->GetActPos(TILT_ADJUST_R);
			dTargetPos = dCurPos + dTargetPos;
			dTargetPos = dTargetPos ;
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
			// kks 2023.04.24 OC 검사 후에 Chuck bwd 이후 체크 
			if (IO_I_Chuck_Bwd() == FALSE)
			{
				nRet[0] = IO_O_Chuck_Bwd();
				if (nRet[0] == FALSE)
				{
					err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
					break;
				}
			}
			// kks 2023.04.24 OC 검사 후에 Chuck bwd 이후 체크 
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END)
				{
					AxmStatusSetPosMatch((long)TILT_ADJUST_R, 0.0);
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK);
					break;
				}
			}
		break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
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
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err == enMCErrCode::MCEC_OK)
			{
				if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
					{
						// kks 2023.04.24 OC 검사 후에 Chuck bwd 이후 체크 
						if (IO_I_Chuck_Fwd() == FALSE)
						{
							nRet[0] = IO_O_Chuck_Fwd();
							if (nRet[0] == FALSE)
							{
								err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
								break;
							}
						}
						// kks 2023.04.24 OC 검사 후에 Chuck bwd 이후 체크 
						reTry++;
						if (err == enMCErrCode::MCEC_OK)
						{
							SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START);
						}
						break;
					}
				}
				else
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END);
						break;
					}
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END)
		{
			nJobCheck[3] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);
			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_46()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
			{
				UINT nRetCode = 0;
				SEMCO_Reset(InspectionPortType::Left);
		
				if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46))
				{
				}
				if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
				break;
			}
		}
	}
	break;
	case SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
			{
				UINT nRetCode = 0;
				SEMCO_Reset(InspectionPortType::Left);

				if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82))
				{
				}
				if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 15000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END);
				break;
			}
		}
	}
	break;
	
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (IO_I_Chuck_Bwd() == FALSE)
		{
			nRet[0] = IO_O_Chuck_Bwd();
			if (nRet[0] == FALSE)
			{
				err = MCEC_Sensnor_Chuck_Bwd_TimeOut;
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START); break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START:
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
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
				SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
				SEQUENCE->nPASS_NG = 0;
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : Adjust Spec Out."));
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END); break;
				}
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_MOVE:
	{
		
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_MOVE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END); break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END:
	{
		nJobCheck[4] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_LAST_OC_CHECK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_82()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	{
		if (m_UdpTester[0].m_bConnected)
		{
			UINT nRetCode = 0;
			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
	{
		nJobCheck[5] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
#elif(SET_INSPECTOR == SYS_TLA_OQC)
enMCErrCode CSeqPickerMgr::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	


	//Log(_T("Picker Mgr =========> %d"), (enSeqPickerMgrStep)GetStep());
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_IDEL:	// 준비
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STOP:	// 준비
		SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_MAIN:	// 시작
		if (nJobCheck[0] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_START); break;
		}
		else if (nJobCheck[1] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_START); break;
		}
		else if (nJobCheck[2] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START); break;
		}
		else if (nJobCheck[3] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START); break;
		}
		else if (nJobCheck[4] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START); break;
		}
		// 		else if (nJobCheck[5] == FALSE)
		// 		{
		// 			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START);
		// 		}
		else if (nJobCheck[0] == TRUE && nJobCheck[1] == TRUE && nJobCheck[2] == TRUE && nJobCheck[3] == TRUE && nJobCheck[4] == TRUE)
		{
			SEQUENCE->nPASS_NG = 1;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
		}

		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:
		err = StartReady();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		err = StartLock();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		err = StartInsp_Power();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
		err = StartInsp_Prewiew();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		err = StartInsp_OC();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
		err = StartInsp_Vignetting_46();
		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
// 		err = StartInsp_Vignetting_82();
// 		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartReady()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	auto pMain = CDlgMain::Get_Instance();
	auto pSide = CDlgSide::Get_Instance();
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_START)
		{
			reTry = 0;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	{
		UINT nRetCode = 0;
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
			{
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
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
	

		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_POWER_OFF);
				break;
			}
		}

	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			device.POWERSUPPLY_VoltOff(InspectionPortType::Left);
			device.POWERSUPPLY_VoltOff(InspectionPortType::Right);
		}
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_POWER_OFF)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK1); //
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
	
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK2);
				Sleep(500);
				break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
		IO_O_Socket_Unlock(FALSE);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK2)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_END);
			Sleep(300);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:
		if (COMMON->GetOperationMode() == OperationMode::DryRun)
		{
			if (SEQUENCE->m_nbuttonFlagIs == TRUE)
			{
				nJobCheck[0] = FALSE;
				nJobCheck[1] = FALSE;
				nJobCheck[2] = FALSE;
				nJobCheck[3] = FALSE;
				nJobCheck[4] = FALSE;
				nJobCheck[5] = FALSE;
				SetNextStep(SEQ_PICKERMGR_STEP_IDEL);
				SEQUENCE->SetRunMode(eStop);
				COMMON->SetMcStatus(eStop);
			}
			else
			{
				nJobCheck[0] = FALSE;
				nJobCheck[1] = FALSE;
				nJobCheck[2] = FALSE;
				nJobCheck[3] = FALSE;
				nJobCheck[4] = FALSE;
				nJobCheck[5] = FALSE;
				SetNextStep(SEQ_PICKERMGR_STEP_MAIN);
			}
			COMMON->DoEvent(2000);
			SEQUENCE->nPASS_NG = 100;
			if (stEq.nLanguage == 0)
				strTemp.Format(_T("DRY RUN 버튼[UI] 누름."));
			else
				strTemp.Format(_T("DRY Run Restart."));
			COMMON->Write_Main_Log(strTemp);
		}
		else
		{
			SEQUENCE->SetRunMode(eStop);


			COMMON->SetMcStatus(eStop);


			if (stEq.nLanguage == 0)
				strTemp.Format(_T("STOP 버튼[UI] 누름."));
			else
				strTemp.Format(_T("STOP BUTTON[UI] PRESSED."));
			COMMON->Write_Main_Log(strTemp);
			m_UdpTester[0].Reset_TesterFlag();
			SetNextStep(SEQ_PICKERMGR_STEP_IDEL);
			COMMON->DoEvent(500);
		}
		//쓰레드 종료
		break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartLock()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	{

		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			nRet[0] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
			nRet[1] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
// 			nRet[0] = TRUE;
// 			nRet[1] = TRUE;
		}
		else
		{
			nRet[0] = TRUE;
			nRet[1] = TRUE;
		}
		if (nRet[0] == FALSE || nRet[1] == FALSE)
		{//제품이 정상적으로 감지 안었습니다. 
			if (nRet[0] == FALSE)
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
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	{
		if (IO_O_Socket_Lock(FALSE) == false)
		{
			err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
			break;
		}
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2);
				Sleep(500);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:

			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP);
				Sleep(300); break;
			}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
		Sleep(500);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP)
		{
			nJobCheck[0] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK)
		{

			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_END); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;

		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Power()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			device.POWERSUPPLY_VoltOn(InspectionPortType::Left);
			device.POWERSUPPLY_VoltOn(InspectionPortType::Right);
		}
		nJobCheck[1] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);

			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Prewiew()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			UINT nRetCode = 0;
			if (m_UdpTester[0].m_bConnected)
			{
				if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
				{
				}
				if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END);
				break;
			}
		}
	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
	{
		nJobCheck[2] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_OC()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dCurPos = 0.0f;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
		reTry = 0;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:
	{

			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END); break;
			}
		break;
	}
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
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
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}

				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Spec) == FALSE) {
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : Adjust Spec Out"));
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END); break;
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END); break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END)
		{
			nJobCheck[3] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);
			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_46()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
			{
				UINT nRetCode = 0;
				SEMCO_Reset(InspectionPortType::Left);

				if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting46))
				{
				}
				if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
				break;
			}
		}
	}
	break;
	case SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	{
		if (COMMON->GetOperationMode() != OperationMode::DryRun)
		{
			if (m_UdpTester[0].m_bConnected)
			{
				UINT nRetCode = 0;
				SEMCO_Reset(InspectionPortType::Left);

				if (!SEMCO_SendStartTest(InspectionPortType::Left, (int)eComMode_Insp_Vigneting82))
				{
				}
				if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting82, 10000, nRetCode))
				{
					err = MCEC_Tester_Ack_Timeout;
					break;
				}
				if (nRetCode != 0)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
						SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
						SEQUENCE->nPASS_NG = 0;
						break;
					}
				}
				if (err == enMCErrCode::MCEC_OK)
				{
					if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
					{
						SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END);
						break;
					}
				}
			}
			else
			{
				err = MCEC_Tester_1_NotConnect;
				break;
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
	{
		nJobCheck[4] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_82()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	{
		if (m_UdpTester[0].m_bConnected)
		{
			UINT nRetCode = 0;
			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
	{
		nJobCheck[5] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
#elif(SET_INSPECTOR == SYS_TLA_B)
enMCErrCode CSeqPickerMgr::StartOperation_Step()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_IDEL:	// 준비
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STOP:	// 준비
		SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_MAIN:	// 시작
		if (nJobCheck[0] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_START); break;
		}
		else if (nJobCheck[1] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_START); break;
		}
		else if (nJobCheck[2] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START); break;
		}
		else if (nJobCheck[3] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START); break;
		}
		else if (nJobCheck[4] == FALSE)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START); break;
		}
		// 		else if (nJobCheck[5] == FALSE)
		// 		{
		// 			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START);
		// 		}
		else if (nJobCheck[0] == TRUE && nJobCheck[1] == TRUE && nJobCheck[2] == TRUE && nJobCheck[3] == TRUE && nJobCheck[4] == TRUE)
		{
			SEQUENCE->nPASS_NG = 1;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
		}

		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:
		err = StartReady();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		err = StartLock();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		err = StartInsp_Power();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
		err = StartInsp_Prewiew();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		err = StartInsp_OC();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
		err = StartInsp_Vignetting_46();
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
		err = StartInsp_Vignetting_82();
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartReady()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	auto pMain = CDlgMain::Get_Instance();
	auto pSide = CDlgSide::Get_Instance();
	CString strTemp = _T("");
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_START)
		{
			reTry = 0;
			SetNextStep(SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF:
	{
		UINT nRetCode = 0;
		if (m_UdpTester[0].m_bConnected)
		{
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
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_PREVIEW_OFF)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_POWER_OFF);
				break;
			}
		}

	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_POWER_OFF:
		device.POWERSUPPLY_VoltOff(InspectionPortType::Left);
		device.POWERSUPPLY_VoltOff(InspectionPortType::Right);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_POWER_OFF)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK1); //
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK1:
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		if (Check_Pos_Screw_L_Type(Screw_Wait) == FALSE || Check_Pos_Screw_R_Type(Screw_Wait) == FALSE)
		{
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
			{
				err = MO_MoveVignetting_R_Type(R_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_UNLOCK2);
				Sleep(500);
				break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_UNLOCK2:
		IO_O_Socket_Unlock(FALSE);
		IO_O_Chuck_Bwd(FALSE);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_READY_UNLOCK2)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_READY_END);
			Sleep(300);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY_END:

		SEQUENCE->SetRunMode(eStop);

		//	WaitForSingleObject(SEQUENCE->GetWinThreadRun()->m_hThread, 20000);

		while (1)
		{
			if (!SEQUENCE->IsThreadRun())
				break;

			COMMON->DoEvent(10);
		}

		COMMON->SetOperationMode(OperationMode::Manual);
		pSide->SetMenuClickColor(pSide->enMenu::Menu_Manual);
		pMain->EqpTypeShowOperation();

		SEQUENCE->Stop_Run();

		COMMON->SetMcStatus(eStop);


		if (stEq.nLanguage == 0)
			strTemp.Format(_T("STOP 버튼[UI] 누름."));
		else
			strTemp.Format(_T("STOP BUTTON[UI] PRESSED."));
		COMMON->Write_Main_Log(strTemp);

		SetNextStep(SEQ_PICKERMGR_STEP_IDEL);

		//쓰레드 종료
		break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartLock()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_START:
	{
		nRet[0] = AJIN_IO->IsON(DIO_INPUT_X1_00_STAU_PRODUCT_L_SENSOR);
		//nRet[0] = TRUE;
		nRet[1] = AJIN_IO->IsON(DIO_INPUT_X1_01_STAU_PRODUCT_R_SENSOR);
		nRet[2] = AJIN_IO->IsON(DIO_INPUT_X1_12_DBUL_JIG_LOAD_CHK_SENSOR);
		nRet[3] = AJIN_IO->IsON(DIO_INPUT_X1_18_DBUR_JIG_LOAD_CHK_SENSOR);
		nRet[4] = AJIN_IO->IsON(DIO_INPUT_X1_19_DBUR_JIG_IO_IN_CHK_SENSOR);
		nRet[5] = AJIN_IO->IsON(DIO_INPUT_X1_13_DBUL_JIG_IO_IN_CHK_SENSOR);
		nRet[6] = AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR);
		nRet[7] = AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR);
		if (nRet[0] == FALSE || nRet[1] == FALSE)
		{//제품이 정상적으로 감지 안었습니다. 
			if (nRet[0] == FALSE)
			{
				err = MCEC_Sensnor_Product_L_NotChk;
			}
			else
			{
				err = MCEC_Sensnor_Product_R_NotChk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[2] == TRUE || nRet[3] == TRUE))
		{//Jig 가 정상적으로 제거 되지 않았습니다. 
			if (nRet[2] == FALSE)
			{
				err = MCEC_Sensnor_Jig_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[4] == TRUE || nRet[5] == TRUE))
		{//Jig 가 전진 되었습니다. 
			if (nRet[4] == FALSE)
			{
				err = MCEC_Sensnor_Jig_IN_Fwd_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_IN_Fwd_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK && (nRet[6] == FALSE || nRet[7] == FALSE))
		{//Jig 가 정상적으로 후진 되어있지 않습니다.  
			if (nRet[6] == FALSE)
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_L_Chk;
			}
			else
			{
				err = MCEC_Sensnor_Jig_OUT_Bwd_R_Chk;
			}
			break;
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1:
	{
		if (IO_O_Socket_Lock(FALSE) == false)
		{
			err = MCEC_Sensnor_Product_Socket_Lock_TimeOut;
			break;
		}
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		if (Check_Pos_Screw_L_Type(Screw_Wait) == FALSE || Check_Pos_Screw_R_Type(Screw_Wait) == FALSE)
		{
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
			{
				err = MO_MoveVignetting_R_Type(R_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if (Check_Pos_Vignetting_X_Type(X_Wait) == FALSE)
			{
				err = MO_MoveVignetting_X_Type(X_Wait);
				if (err != enMCErrCode::MCEC_OK)
				{
					break;
				}
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK1)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2);
				Sleep(500);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2:
		nRet[0] = IO_O_Chuck_Fwd();
		if (nRet[0] == TRUE)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP);
				Sleep(300); break;
			}
		}
		else
		{
			err = MCEC_Sensnor_Chuck_Fwd_TimeOut;
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP:
		AJIN_IO->SetON(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
		Sleep(500);
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_INTERLOCK2_GRIP)
		{
			nJobCheck[0] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_PRODUCT_CHK)
		{

			SetNextStep(SEQ_PICKERMGR_STEP_LOCK_END); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_LOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;

		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Power()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CDevice device;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_START:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START:
		device.POWERSUPPLY_VoltOn(InspectionPortType::Left);
		device.POWERSUPPLY_VoltOn(InspectionPortType::Right);
		nJobCheck[1] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_WAiT)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_POWER_ON_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);

			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Prewiew()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START:
	{
		UINT nRetCode = 0;
		if (m_UdpTester[0].m_bConnected)
		{
			if (!SEMCO_SendPreviewStart(InspectionPortType::Left))
			{
			}
			if (!SEMCO_WaitPreviewStartAck(InspectionPortType::Left, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END:
	{
		nJobCheck[2] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_PREVIEW_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}
	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_OC()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();
	double dPos = 0.0;
	double dSpeed = 0.0;
	double dTargetPos = 0.0f;
	double dCurPos = 0.0f;
	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START:
		reTry = 0;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_START)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START); break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START:

		if (reTry > stTeaching.nAdjust_RetryCnt)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_START); break;
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
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END); break;
				}
			}
		}
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_START:
	{
		if (m_UdpTester[0].m_bConnected)
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
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}

			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST:
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
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
					break;
				}
			}
		}
		else
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_END); break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START:
		if (IO_O_Screw_Bolt_L_Lock_1() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END);
				break;
			}
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_1_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK:
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
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
				SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
				SEQUENCE->nPASS_NG = 0;
				break;
			}
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw1_Spec) == FALSE) {
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
				{
					reTry++;
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START);
					break;
				}
			}
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_1_CHK)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START);
					break;
				}
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START:
	{
		if (IO_O_Screw_Bolt_L_Lock_2() == false)
		{
			err = MCEC_Screw_Complete_TimeOut;
			break;
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END);
				break;
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_LOCK_2_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK);
			break;
		}
		break;

	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK:
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
		if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_TLA, 10000, nRetCode))
		{
			err = MCEC_Tester_Ack_Timeout;
			break;
		}
		if (nRetCode != 0)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
				SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
				SEQUENCE->nPASS_NG = 0;
				break;
			}
		}
		if (err != enMCErrCode::MCEC_OK)
		{
			if ((fabs(m_UdpTester[0].m_dTesterFlag_TLA_PixelY) <= stTeaching.nAdjust_Screw2_Spec) == FALSE) {
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
				{
					reTry++;
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START);
					break;
				}
			}
			else
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_RESULT_2_CHK)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END);
					break;
				}
			}
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_OC_ADJUST_END)
		{
			nJobCheck[3] = TRUE;
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN);
			break;
		}
		break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_46()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();


	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START:
	{
		if (m_UdpTester[0].m_bConnected)
		{
			UINT nRetCode = 0;
			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END:
	{
		nJobCheck[4] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_46_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
enMCErrCode CSeqPickerMgr::StartInsp_Vignetting_82()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	enMCErrCode errRet[10] = { enMCErrCode::MCEC_OK , };
	int nRet[10] = { 0, };
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();

	switch ((enSeqPickerMgrStep)GetStep())
	{
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START:
	{
		if (Check_Pos_Vignetting_R_Type(R_Wait) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_X_Type(X_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_X_Type(X_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (Check_Pos_Vignetting_R_Type(R_Test_01) == FALSE)
		{
			err = MO_MoveVignetting_R_Type(R_Test_01);
			if (err != enMCErrCode::MCEC_OK)
			{
				break;
			}
		}
		if (err == enMCErrCode::MCEC_OK)
		{
			if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_START)
			{
				SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END);
				Sleep(500);
				break;
			}
		}

	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END:
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_MOVE_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START);
			break;
		}
		break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START:
	{
		if (m_UdpTester[0].m_bConnected)
		{
			UINT nRetCode = 0;
			SEMCO_Reset(InspectionPortType::Left);
			if (!SEMCO_SendStartTest(InspectionPortType::Left), eComMode_Insp_Vigneting46)
			{
			}
			if (!SEMCO_WaitStartTestAck(InspectionPortType::Left, eComMode_Insp_Vigneting46, 10000, nRetCode))
			{
				err = MCEC_Tester_Ack_Timeout;
				break;
			}
			if (nRetCode != 0)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_READY_START);
					SEQUENCE->StrNG.Format(_T("NG : %d."), nRetCode);
					SEQUENCE->nPASS_NG = 0;
					break;
				}
			}
			if (err != enMCErrCode::MCEC_OK)
			{
				if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_TEST_START)
				{
					SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END);
					break;
				}
			}
		}
		else
		{
			err = MCEC_Tester_1_NotConnect;
			break;
		}
	}
	break;
	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END:
	{
		nJobCheck[5] = TRUE;
		if ((enSeqPickerMgrStep)GetStep() == SEQ_PICKERMGR_STEP_INSPECTION_VIGNETTING_82_END)
		{
			SetNextStep(SEQ_PICKERMGR_STEP_MAIN); break;
		}
	}
	break;
	default:
		break;
	}

	return err;
}
#endif


bool CSeqPickerMgr::IsPickerUnloadReqeust()
{
	bool bRet = false;

	if (m_SeqPicker[0].IsStatusComplete(enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_REQEUST))
		bRet = true;

	if (m_SeqPicker[1].IsStatusComplete(enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_REQEUST))
		bRet = true;

	return bRet;
}

bool CSeqPickerMgr::IsPickerWorkEndRFID()
{
	bool bRet = false;
	bRet = GetWorkEnd();
	return bRet;
}

bool CSeqPickerMgr::IsReadyWorkEnd()
{
	bool bLoadOk = m_SeqPicker[(int)m_LoadingPort].IsStatusComplete(enSeqPickerStep::SEQ_PICKER_STEP_INSPECTION_END);
	bool bUnloadOk = m_SeqPicker[(int)m_UnloadingPort].IsStatusComplete(enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET);
	if (bLoadOk && bUnloadOk)
	{
		return true;
	}
	
	return false;
}