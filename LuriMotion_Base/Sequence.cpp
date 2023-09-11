#include "pch.h"
#include "Sequence.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CSequence::CSequence()
{
	SetWinThreadRun(NULL);
	SetRunMode(eStop);

	m_hThreadSemiAuto = NULL;
	m_hThreadStartupCheck = NULL;

	nPASS_NG = 2;
	m_SeqPickerMgr.DeleteThread();
	StrNG.Empty();
}

CSequence::~CSequence()
{
	if (GetWinThreadRun())
	{
		SetThreadRun(FALSE);
		WaitForSingleObject(GetWinThreadRun()->m_hThread, 500);
	}

	if (m_hThreadSemiAuto)
	{
		CloseHandle(m_hThreadSemiAuto);
		m_hThreadSemiAuto = NULL;
	}

	if (m_hThreadStartupCheck)
	{
		CloseHandle(m_hThreadStartupCheck);
		m_hThreadStartupCheck = NULL;
	}
}

// int PickMgrJob1();
// int PickMgrJob2();
// int PickMgrJob3();
// int PickMgrJob4();
void CSequence::DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void CSequence::DoEvents(DWORD dwMiliSeconds)
{
	clock_t start_tm = clock();
	do
	{
		DoEvents();
		::Sleep(1);
	} while ((DWORD)(clock() - start_tm) < dwMiliSeconds);
}
int CSequence::PickMgrJob0()
{
	return m_SeqPickerMgr.nJobCheck[0];
}
int CSequence::PickMgrJob1()
{
	return m_SeqPickerMgr.nJobCheck[1];
}
int CSequence::PickMgrJob2()
{
	return m_SeqPickerMgr.nJobCheck[2];
}
int CSequence::PickMgrJob3()
{
	return m_SeqPickerMgr.nJobCheck[3];
}
int CSequence::PickMgrJob4()
{
	return m_SeqPickerMgr.nJobCheck[4];
}


int CSequence::PickMgrStep()
{
	return m_SeqPickerMgr.GetStep();
}
void CSequence::PickMgrSetNextStep(__in enSeqPickerMgrStep step)
{
	m_SeqPickerMgr.SetNextStep(step);
}

void CSequence::LogSeq(__in CString sz)
{
	WRITE_MAIN_LOG(sz);
//	Log(sz);
}

void CSequence::LogError(__in CString sz)
{
	WRITE_MAIN_LOG(sz);
}

bool CSequence::Begin_RunThread()
{
	if (GetWinThreadRun())
	{
		End_RunThread();
	}
	SetThreadRun(TRUE);
	SetWinThreadRun(AfxBeginThread(Thread_Run, this, THREAD_PRIORITY_NORMAL));

	return true;
}

void CSequence::End_RunThread()
{
	if (GetWinThreadRun())
	{
		SetThreadRun(FALSE);
		WaitForSingleObject(GetWinThreadRun()->m_hThread, 500);				//Log Message 때문에 무한대기에 빠진다
	}
}

bool CSequence::Begin_RunThreadStop()
{
	if (IsThreadRunStop() == TRUE)
	{

	}
	else
	{

	}
	return true;
}

void CSequence::End_RunThreadStop()
{
	if (IsThreadRunStop() == TRUE)
	{
		SetThreadRunStop(FALSE);
		WaitForSingleObject(GetWinThreadRunStop()->m_hThread, 500);				//Log Message 때문에 무한대기에 빠진다
	}
}

void CSequence::Start_Run()
{
	if (GetRunMode() == eRunning)
		return;
	SetRunMode(eRunning);
	m_SeqPickerMgr.ErrorClear();
	End_RunThread();
	Sleep(100);
 	if (IsThreadRun() == FALSE)
 		Begin_RunThread();
	
	
	m_SeqPickerMgr.nJobCheck[0] = FALSE;
	m_SeqPickerMgr.nJobCheck[1] = FALSE;
	m_SeqPickerMgr.nJobCheck[2] = FALSE;
	m_SeqPickerMgr.nJobCheck[3] = FALSE;
	m_SeqPickerMgr.nJobCheck[4] = FALSE;
	m_SeqPickerMgr.nJobCheck[5] = FALSE; // 워킹 초기화 
	m_SeqPickerMgr.ForceStop();

	m_SeqPickerMgr.SetStatus(enSeqStatus::Status_Testing);

	m_SeqPickerMgr.DeleteThread();
	DoEvents(500);
	SEQUENCE->nPASS_NG = 3;
// 	m_SeqConveyor1.Start();
// 	m_SeqConveyor2.Start();
// 	m_SeqConveyorReturn.Start();
	m_SeqPickerMgr.Start();

// 	m_SeqConveyor1.ErrorClear();
// 	m_SeqConveyor2.ErrorClear();
// 	m_SeqConveyorReturn.ErrorClear();
	

	
	LogSeq(_T("SEQUENCE Stsart."));
	WRITE_SEMCO_LOG(_T("O,-,-,Run,TLA:Main,Start,"));
}

void CSequence::Stop_Run()
{
// 	if (GetRunMode() == ePause)
// 	{
		SetRunMode(eStop);

// 		m_SeqConveyor1.ForceStop();
// 		m_SeqConveyor2.ForceStop();
// 		m_SeqConveyorReturn.ForceStop();
		m_SeqPickerMgr.Stop();

		LogSeq(_T("SEQUENCE Normal Stop."));
		WRITE_SEMCO_LOG(_T("O,-,-,Stop,TLA:Main,Start,"));
// 	}
// 	else
// 	{
// 		SetRunMode(ePause);
// 
// // 		m_SeqConveyor1.ForceStop();
// // 		m_SeqConveyor2.ForceStop();
// // 		m_SeqConveyorReturn.ForceStop();
// 		m_SeqPickerMgr.Stop();
// 
// 		LogSeq(_T("SEQUENCE Normal Stop."));
// 
// 	}
	return;
}

void CSequence::Stop_Run(RUN_MODE rm)
{
//	m_nRunMode = rm;
	SetRunMode(rm);

// 	m_SeqConveyor1.ForceStop();
// 	m_SeqConveyor2.ForceStop();
// 	m_SeqConveyorReturn.ForceStop();
	m_SeqPickerMgr.ForceStop();
	//m_SeqPicker.ForceStop();
	if (rm == eStop)
		WRITE_SEQ_LOG(_T("SEQUENCE Normal Stop."));
	else if (rm == eError)
		WRITE_SEQ_LOG(_T("SEQUENCE Normal Error."));
}

void CSequence::Pause_Run()
{
	SetRunMode(ePause);
	LogSeq(_T("SEQUENCE Normal Pause."));
}
void CSequence::Emergency_Run()
{
	SetRunMode(eEStop);
	LogSeq(_T("SEQUENCE Emergency Stop."));
	WRITE_SEMCO_LOG(_T("O,-,-,Stop,TLA:Main,Start,"));
}
void CSequence::Init_Compleate()
{
	SetRunMode(eStandby);
	LogSeq(_T("SEQUENCE Init Compleate."));
}
RUN_MODE CSequence::GetRunMode()
{
	return (RUN_MODE)m_bRunMode;
}
void CSequence::SetRunMode(INT16 rm)
{
	m_bRunMode = rm;
}
BOOL CSequence::IsThreadRun() const
{
	return m_bThreadRun;
}
void CSequence::SetThreadRun(BOOL val)
{
	m_bThreadRun = val;
}
CWinThread* CSequence::GetWinThreadRun() const
{
	return m_pThreadRun;
}
void CSequence::SetWinThreadRun(CWinThread * val)
{
	m_pThreadRun = val;
}
BOOL CSequence::IsThreadRunStop() const
{
	return m_bThreadRunStop;
}
void CSequence::SetThreadRunStop(BOOL val)
{
	m_bThreadRunStop = val;
}
CWinThread* CSequence::GetWinThreadRunStop() const
{
	return m_pThreadRunStop;
}
void CSequence::SetWinThreadRunStop(CWinThread * val)
{
	m_pThreadRunStop = val;
}

int CSequence::GetFlareShotCnt() const
{
	return m_nFlareShotCnt;
}
void CSequence::SetFlareShotCnt(int val)
{
	m_nFlareShotCnt = val;
}

void CSequence::Start_SemiAuto(__in InspectionPortType port, __in int nIdx)
{
	// 동작중일경우
	if (IsTesting_SemiAuto())
		return;

	if (m_hThreadSemiAuto)
	{
		CloseHandle(m_hThreadSemiAuto);
		m_hThreadSemiAuto = NULL;
	}

	m_bThreadSemiAuto = true;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner	= this;
	pParam->nPort   = (UINT)port;
	pParam->nArg	= nIdx;

	m_hThreadSemiAuto = HANDLE(_beginthreadex(NULL, 0, Thread_SemiAuto, pParam, 0, NULL));
}

void CSequence::Stop_SemiAuto()
{
	if (m_hThreadSemiAuto)
	{
		CloseHandle(m_hThreadSemiAuto);
		m_hThreadSemiAuto = NULL;
	}
	m_bThreadSemiAuto = false;
	m_SeqSemiAuto.Stop();
}

void CSequence::Start_StartupCheck(__in InspectionPortType port, __in int nIdx)
{
	// 동작중일경우
	if (IsTesting_StartupCheck())
		return;

	if (m_hThreadStartupCheck)
	{
		CloseHandle(m_hThreadStartupCheck);
		m_hThreadStartupCheck = NULL;
	}

	m_bThreadStartupCheck = true;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner = this;
	pParam->nPort = (UINT)port;
	pParam->nArg = nIdx;

	m_hThreadStartupCheck = HANDLE(_beginthreadex(NULL, 0, Thread_StartupCheck, pParam, 0, NULL));
}

void CSequence::Stop_StartupCheck()
{
	m_bThreadStartupCheck = false;
	m_SeqStartupCheck.Stop();
}

void CSequence::Reset_SeqFlags()
{
// 	m_SeqConveyor1.Reset();
// 	m_SeqConveyor2.Reset();
// 	m_SeqConveyorReturn.Reset();
	m_SeqPickerMgr.Reset();

	SetRunMode(eStop);
}

UINT CSequence::Thread_Run(LPVOID lpVoid)
{
	/////////////////////////////////////////////	

	// 초기
	SEQUENCE->Initialize_Run();
	auto err = SEQUENCE->StartOperation_Run();
	SEQUENCE->Finalize_Run(err);	
	return 0;
}

UINT CSequence::Thread_RunStop(LPVOID lpVoid)
{
	/////////////////////////////////////////////
	while (SEQUENCE->IsThreadRunStop() == TRUE)
	{		
		Sleep(30);
		
		if (SEQUENCE->GetRunMode() == eRunning)
			continue;
	}

	SEQUENCE->SetWinThreadRunStop(NULL);
	SEQUENCE->SetThreadRunStop(FALSE);
	return 0;
}

UINT CSequence::Thread_SemiAuto(__in LPVOID lParam)
{
	CSequence*	pThis = (CSequence*)((stThreadParam*)lParam)->pOwner;
	UINT		nIdx = ((stThreadParam*)lParam)->nArg;
	UINT		Port = ((stThreadParam*)lParam)->nPort;

	if (NULL != lParam)
		delete lParam;

	pThis->Initialize_SemiAuto((InspectionPortType)Port, nIdx);
	auto err = pThis->StartOperation_SemiAuto();
	pThis->Finalize_SemiAuto(err,nIdx);

	return 0;
}

UINT CSequence::Thread_StartupCheck(__in LPVOID lParam)
{
	CSequence*	pThis = (CSequence*)((stThreadParam*)lParam)->pOwner;
	UINT		nIdx = ((stThreadParam*)lParam)->nArg;
	UINT		Port = ((stThreadParam*)lParam)->nPort;

	if (NULL != lParam)
		delete lParam;

	pThis->Initialize_StartupCheck((InspectionPortType)Port, nIdx);
	auto err = pThis->StartOpertation_StartupCheck();
	pThis->Finalize_StartupCheck(err,nIdx);

	return 0;
}

void CSequence::Run_Conveyor1()
{
// 	auto * p			= &m_SeqConveyor1;
// 	auto * pConveyor2	= &m_SeqConveyor2;
	auto * pPickermgr	= &m_SeqPickerMgr;

//	auto nStep = p->GetStep();

// 	switch (nStep)
// 	{
// 	case enSeqCv1Step::SEQ_CV1_STEP_READY:	// 준비
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_CHECK_STATUS);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_CHECK_STATUS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_WAIT_SOCKET);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_WAIT_SOCKET:
// 	{
// 		// 앞설비에서 넘어오는 IO 를 여기서 확인하자...
// #ifdef DevMode_Simulator
// 		if (p->IsInputSocket())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_START);
// 		else if (p->IsInputBufferSocket())
// 		{
// 			if (pConveyor2->IsStatusComplete(enSeqCv2Step::SEQ_CV2_STEP_WAIT_SOCKET)
// 				&& pPickermgr->GetEmptyPortId() != -1)
// 			{
// 				p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_START);
// 			}
// 		}
// #else
// 		if (IsDryRun())
// 		{
// 			ST_SocketInfo socket;
// 			socket.szRFID = _T("DryRun Sample");
// 			socket.szBCR = _T("BCR");
// 
// 			if (p->IsInputBufferSocket())
// 			{
// 				if (pConveyor2->IsStatusComplete(enSeqCv2Step::SEQ_CV2_STEP_WAIT_SOCKET)
// 					&& pPickermgr->GetEmptyPortId() != -1)
// 				{
// 					p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_START);
// 				}
// 			}else if (p->InputSocket(socket))
// 			{
// 				p->SetUseRFIDStop(false);
// 				p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_START);
// 			}				
// 		}
// 		else
// 		{
// // 			if (IO_I_CV1_IF_FrontIn())   // 앞설비에서 신호 받았을 경우 & 설비내부에 알람이 없을 경우에만 받자 
// // 			{
// // //  			ST_SocketInfo socket;
// // //  			socket.szRFID = _T("test");
// // //  			socket.szBCR = _T("bcr1");
// //  				
// // // 				p->InputSocket(socket);
// // 
// // 				p->SetUseRFIDStop(true);
// // 				p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_START);
// // 			}
// // 			else if (IO_I_CV1_Sensor(enConv1SensorSelect::Cv1_Input))
// // 			{
// // //  			ST_SocketInfo socket;
// // //  			socket.szRFID = _T("test");
// // //  			socket.szBCR = _T("bcr1"); 			
// // //  			p->InputSocket(socket);
// // 
// // 				p->SetUseRFIDStop(true);
// // 				p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_START);
// // 			}
// // 			else if (IO_I_CV1_BufLiftCheck())
// // 			{
// // 				if (pConveyor2->IsStatusComplete(enSeqCv2Step::SEQ_CV2_STEP_WAIT_SOCKET)
// // 					&& pPickermgr->GetEmptyPortId() != -1)
// // 				{
// // 					p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_START);
// // 				}
// // 			}
// 		}		
// #endif		
// 	}
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_SLOW_SENSOR);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_SLOW_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_STOP_SENSOR);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CHECK_STOP_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_CYLINDER_UP);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_CYLINDER_UP:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_ON);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_ON:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_OFF);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_GRIP_OFF:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_END);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_BUFFER_END:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// 			ST_SocketInfo socket;
// 			p->OutputSocketToConv2(socket);
// 			p->InputBufferSocket(socket);
// 			p->SetNextStep(SEQ_CV1_STEP_COMPLETE);
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_INPUT_SENSOR);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_INPUT_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_SCAN_SOCKET);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_SCAN_SOCKET:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_RFID_END);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_RFID_END:
// 	{
// 		// 컨베이어1 RFID 에 소켓이 도착했을 경우
// 		int nPortId = pPickermgr->GetEmptyPortId();
// 
// 		// 1. picker 를 모두 사용 중일 경우 버퍼로 이동하자.. (만약 버퍼에 자재가 있을 경우 알람)
// 		// 양산용 버전
// 		if (nPortId == -1 && p->IsInputBufferSocket() == false)
// 		{
// 			if(!p->IsBypassSocket())
// 				p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_START);
// 		}				
// 
// 		// 설비 확인용 버전
// // 		if(p->IsInputBufferSocket() == false)
// // 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_BUFFER_START);
// 	}		
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_CONV2_RUN);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_RUN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_MOVE_TO_CONV2_END);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_MOVE_TO_CONV2_END:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// // 			ST_SocketInfo socket;
// // 			p->OutputSocketToConv2(socket);
// // 			pConveyor2->InputSocket(socket);
// 
// 			p->SetNextStep(enSeqCv1Step::SEQ_CV1_STEP_WAIT_SOCKET);
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_GRIP_OFF);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_GRIP_OFF:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_CYLINDER_DN);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_CYLINDER_DN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_CHECK_STOP_SENSOR);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_CHECK_STOP_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_BUFFER_DOWN_END);
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_BUFFER_DOWN_END:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// 			ST_SocketInfo socket;
// 			p->OutputBufferSocketToConv1(socket);
// 
// 			if (!p->IsInputSocket())
// 				p->InputSocket(socket);
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv1Step::SEQ_CV1_STEP_COMPLETE: // 완료
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV1_STEP_CHECK_STATUS);
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CSequence::Run_Conveyor2()
{
// 	auto * p = &m_SeqConveyor2;
// 
// 	auto * pConveyor1	= &m_SeqConveyor1;
// 	auto * pPickermgr	= &m_SeqPickerMgr;
// 
// 	auto * pConveyorRet = &m_SeqConveyorReturn;
// 	
// 	auto nStep = p->GetStep();
// 	switch (nStep)
// 	{
// 	case enSeqCv2Step::SEQ_CV2_STEP_READY:	// 준비
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_CHECK_STATUS);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_CHECK_STATUS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_WAIT_SOCKET);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_WAIT_SOCKET:
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_RESERVED_FROM_PICKER:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// 			if (pPickermgr->IsStatusComplete(enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WAIT_SOCKET))
// 			{
// 				InspectionPortType Port;
// 				if (pPickermgr->IsSocketAtUnloadPort(Port))
// 				{
// 					ST_SocketInfo socket;
// 					pPickermgr->OutputSocketFromUnload(socket);
// 					p->InputUnloadSocket(socket);
// 
// 					p->SetWorkPort((int)Port);
// 					p->SetNextStep(enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_START);
// 				}
// 				else
// 				{
// 					p->SetNextStep(enSeqCv2Step::SEQ_CV2_STEP_COMPLETE);
// 				}
// 			}
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_RESERVED_FROM_CONVEYOR1:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// 			// 비어있는 Port 는 어디인가?
// 			int PortId = pPickermgr->GetEmptyPortId();
// 
// 			p->SetWorkPort(PortId);
// 			pConveyor1->SetNextStep(SEQ_CV1_STEP_MOVE_TO_CONV2_START);
// 
// 			// Bypass 일경우 
// 			if (pConveyor1->IsBypassSocket())
// 			{
// 				ST_SocketInfo socket;
// 				pConveyor1->OutputSocketToConv2(socket);
// 				p->InputUnloadSocket(socket);
// 
// 				p->SetNextStep(SEQ_CV2_STEP_MOVE_BYPASS_START);
// 			}
// 			else
// 			{
// 				ST_SocketInfo socket;
// 				pConveyor1->OutputSocketToConv2(socket);
// 				p->InputSocket(socket);
// 
// 				p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_START);
// 			}
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_FROM_IN);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_FROM_IN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_SLOW_SENSOR);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_SLOW_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_STOP_SENSOR);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_WAIT_SOCKET_TO_STOP_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_CHECK_SOCKET);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_CHECK_SOCKET:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_LOADING_END);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_LOADING_END:
// 	{
// 		if (pPickermgr->IsStatusComplete(enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WAIT_SOCKET))
// 		{
// 			// PICKER 에서 로딩을 완료 했을 경우 소켓 정보를 PICKER 에 전달하자 
// 			ST_SocketInfo socket;
// 			p->OutputSocket(socket);
// 			pPickermgr->InputSocketToLoad(socket);
// 
// 			pPickermgr->SetNextStep(SEQ_PICKERMGR_STEP_ARRIVED_SOCKET);
// 		}
// 
// 		if (pPickermgr->IsStatusComplete(enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOAD_END))
// 		{
// 			p->SetNextStep(enSeqCv2Step::SEQ_CV2_STEP_COMPLETE);
// 			pPickermgr->SetNextStep(enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_START);
// 		}
// 	}
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_UNLOADING_CHECK_SOCKET);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_CHECK_SOCKET:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_TO_UNLOADING_END);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_TO_UNLOADING_END:
// 		if (p->IsStatusComplete())
// 		{
// 			// 임시로 conveyor return 에 제품을 넣어주자.
// #ifdef DevMode_Simulator
// 			if (pConveyorRet->IsStatusComplete(enSeqCvrStep::SEQ_CVR_STEP_WAIT_SOCKET))
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);
// 				pConveyorRet->InputSocket(socket);
// 
// 				p->SetNextStep(SEQ_CV2_STEP_COMPLETE);
// 
// 				if (pPickermgr->IsUsePortSkip() && pPickermgr->GetAutoChange())
// 					pPickermgr->SetNextStep(SEQ_PICKERMGR_STEP_AUTOCHANGE_START);
// 				
// 				break;
// 			}
// #else
// 			if (IsDryRun())
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);
// 				pConveyorRet->InputSocket(socket);
// 			}
// 			else
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);		
// 
// 				// auto change 
// 				if (pPickermgr->IsUsePortSkip() && pPickermgr->GetAutoChange())
// 					pPickermgr->SetNextStep(SEQ_PICKERMGR_STEP_AUTOCHANGE_START);
// 			}
// #endif
// 			p->SetNextStep(SEQ_CV2_STEP_COMPLETE);
// 		}			
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_OUT_SENSOR);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_OUT_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_BYPASS_RUN);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_RUN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_MOVE_BYPASS_END);
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_MOVE_BYPASS_END:
// 		if (p->IsStatusComplete())
// 		{
// 			// 임시로 conveyor return 에 제품을 넣어주자.
// #ifdef DevMode_Simulator
// 			if (pConveyorRet->IsStatusComplete(enSeqCvrStep::SEQ_CVR_STEP_WAIT_SOCKET))
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);
// 				pConveyorRet->InputSocket(socket);
// 			}			
// #else
// 			if (IsDryRun())
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);
// 				pConveyorRet->InputSocket(socket);
// 			}
// 			else
// 			{
// 				ST_SocketInfo socket;
// 				p->OutputUnloadSocket(socket);
// 			}
// #endif
// 			p->SetNextStep(SEQ_CV2_STEP_COMPLETE);
// 		}			
// 		break;
// 
// 	case enSeqCv2Step::SEQ_CV2_STEP_COMPLETE: // 완료
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CV2_STEP_CHECK_STATUS);
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CSequence::Run_ConveyorReturn()
{
// 	auto * p = &m_SeqConveyorReturn;
// 	auto nStep = p->GetStep();
// 
// 	switch (nStep)
// 	{
// 	case enSeqCvrStep::SEQ_CVR_STEP_READY:			// 준비
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_CHECK_STATUS);
// 		
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_CHECK_STATUS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_WAIT_SOCKET);
// 
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_WAIT_SOCKET:
// 	{
// 		if (p->IsStatusComplete())
// 		{
// #ifdef DevMode_Simulator
// 			if (p->IsInputSocket())
// 			{
// 				p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_START);
// 			}
// #else
// 			if (IsDryRun())
// 			{
// 				if (p->IsInputSocket())
// 					p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_START);
// 			}
// 			else
// 			{
// // 				if (IO_I_CVR_IF_RearOut() 
// // 					|| IO_I_CVR_Sensor(enConvRetSensorSelect::Cvr_Input)) //  & 설비내부에 알람이 없을 경우에만 받자 
// // 				{
// // 			//		ST_SocketInfo socket;
// // 			//		p->InputSocket(socket);
// // 					p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_START);
// // 				}
// 			}			
// #endif			
// 		}
// 	}
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN);
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN:
// 		if (p->IsStatusComplete())
// 		{
// #ifndef DevMode_Simulator
// 			ST_SocketInfo socket;
// 			p->InputSocket(socket);
// #endif	
// 
// 			p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR);
// 		}
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR);
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_RUN);
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_RUN:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_MOVE_BYPASS_END);
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_MOVE_BYPASS_END:
// 		if (p->IsStatusComplete())
// 		{
// 			ST_SocketInfo socket;
// 			p->OutputSocket(socket);
// 			p->SetNextStep(SEQ_CVR_STEP_COMPLETE);
// 		}
// 		break;
// 
// 	case enSeqCvrStep::SEQ_CVR_STEP_COMPLETE:		// 완료
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_CVR_STEP_WAIT_SOCKET);
// 		break;
// 
// 	default:
// 		break;
// 	}
}

void CSequence::Run_Picker()
{
	auto * p			= &m_SeqPickerMgr;
//	auto * pConveyor2	= &m_SeqConveyor2;

	auto nStep = p->GetStep();

// 	switch (nStep)
// 	{
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_READY:			// 준비
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INIT_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_INIT_MOVE_Z);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INIT_MOVE_Z:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_INIT_MOVE_X);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INIT_MOVE_X:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_INIT_END);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_INIT_END:
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STANDBY_UNLOAD_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_STANDBY_UNLOAD_END);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_STANDBY_UNLOAD_END:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_COMPLETE);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHECK_STATUS:	// 초기 준비 상황
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WAIT_SOCKET:
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_ARRIVED_SOCKET:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_LOAD_START);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOAD_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_LOAD_UP_CHECK);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOAD_UP_CHECK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_LOAD_END);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_LOAD_END:
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_CHANGE_PORT_CHECK_UNLOADING_PORT);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_CHECK_UNLOADING_PORT:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_CHANGE_PORT_MOVE_Z_SAFETY_POS);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_MOVE_Z_SAFETY_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_CHANGE_PORT_MOVE_X);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_MOVE_X:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_CHANGE_PORT_SEND_FINALIZE_TO_UNLOAD_PORT);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_SEND_FINALIZE_TO_UNLOAD_PORT:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_CHANGE_PORT_END);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_CHANGE_PORT_END:
// 		if(p->IsUsePortSkip())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_START);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_START: 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_WAIT_INSPECTION);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_WAIT_INSPECTION: 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_MOVE_SAFETY_POS);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_MOVE_SAFETY_POS: 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_MOVE_X);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_MOVE_X: 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_SEND_FINALIZE);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_SEND_FINALIZE: 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_PORT_USE_SKIP_END);
// 		break;		
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_PORT_USE_SKIP_END: 
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_UNLOAD_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_UNLOAD_PICKER_CHECK);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_UNLOAD_PICKER_CHECK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_UNLOAD_END);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_UNLOAD_END:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_COMPLETE);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WORKEND_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_WORKEND_MOVE_Z_SAFETY_POS);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WORKEND_MOVE_Z_SAFETY_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_WORKEND_MOVE_X);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WORKEND_MOVE_X:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_WORKEND_SEND_FINALIZE);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WORKEND_SEND_FINALIZE:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_WORKEND_END);
// 		break;
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_WORKEND_END:
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_AUTOCHANGE_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_AUTOCHANGE_SETTING);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_AUTOCHANGE_SETTING:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_AUTOCHANGE_END);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_AUTOCHANGE_END:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_INIT_START);
// 		break;
// 
// 	case enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_COMPLETE: // 완료
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_PICKERMGR_STEP_WAIT_SOCKET);
// 		break;
// 	default:
// 		break;
// 	}
}

void CSequence::Run_SemiAuto()
{
	auto * p	= &m_SeqSemiAuto;
	auto nStep	= p->GetStep();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	switch (nStep)
	{
	case SEQ_SEMIAUTO_STEP_IDEL:
		break;

	case SEQ_SEMIAUTO_STEP_READY:
		break;

	// (LOADING)
	case SEQ_SEMIAUTO_STEP_LOADING_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_LOADING_CHK);
		break;
	case SEQ_SEMIAUTO_STEP_LOADING_CHK:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_LOADING_END);
		break;
	case SEQ_SEMIAUTO_STEP_LOADING_END:		
		break;					


	case SEQ_SEMIAUTO_STEP_LOCK_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_CHK);
		break;						
	case SEQ_SEMIAUTO_STEP_LOCK_CHK: 
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_MOVE);
		break;			
	case SEQ_SEMIAUTO_STEP_LOCK_MOVE:				
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_END);
		break;						
	case SEQ_SEMIAUTO_STEP_LOCK_END:	
		break;		

	case SEQ_SEMIAUTO_STEP_PREVIEW_START:			
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON);
		break;						
	case SEQ_SEMIAUTO_STEP_PREVIEW_POW_ON:	
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW);
		break;	
	case SEQ_SEMIAUTO_STEP_PREVIEW:			
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW_END);
		break;
	case SEQ_SEMIAUTO_STEP_PREVIEW_END:
		break;

	case SEQ_SEMIAUTO_STEP_TLA_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK);
		break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT_UNLOCK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_SEMIAUTO_STEP_TLA_TEST);
		break;
	case SEQ_SEMIAUTO_STEP_TLA_TEST:
		break;
	case SEQ_SEMIAUTO_STEP_TLA_ADJUST:	
		break;	
	case SEQ_SEMIAUTO_STEP_TLA_BOLT1_LOCK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST);
		break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK1_TEST:
		break;
	case SEQ_SEMIAUTO_STEP_TLA_BOLT2_LOCK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST);
		break;
	case SEQ_SEMIAUTO_STEP_TLA_LOCK2_TEST:
		break;
	case SEQ_SEMIAUTO_STEP_TLA_END:
		break;


	case SEQ_SEMIAUTO_STEP_VIGNETING_START:				
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_CHK);
		break;
	case SEQ_SEMIAUTO_STEP_VIGNETING_CHK:				
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST);
		break;						
	case SEQ_SEMIAUTO_STEP_VIGNETING_46_TEST:					
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_END);
		break;							
// 	case SEQ_SEMIAUTO_STEP_VIGNETING_82_MOVE:			
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_82_TEST);
// 		break;					
// 	case SEQ_SEMIAUTO_STEP_VIGNETING_82_TEST:						
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_END);
// 		break;							
	case SEQ_SEMIAUTO_STEP_VIGNETING_END:						
		break;	



	case SEQ_SEMIAUTO_STEP_UNLOADING_START:						// PROBE UP
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_CHK);
		break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_CHK:				// VACCUM CHECK
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP);
		break;		
	case SEQ_SEMIAUTO_STEP_UNLOADING_PREVIEWSTOP:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF);
		break;
	case SEQ_SEMIAUTO_STEP_UNLOADING_POW_OFF:				// Z축 안전위치로 이동
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_END);
		break;					

	case SEQ_SEMIAUTO_STEP_UNLOADING_END:
		break;
	default:
		break;
	}

#endif
}

void CSequence::Run_StartupCheck()
{
	auto * p = &m_SeqStartupCheck;
	auto nStep = p->GetStep();
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	switch (nStep)
	{
	case SEQ_STARTUPCHECK_STEP_IDEL:
		break;

	case SEQ_STARTUPCHECK_STEP_READY:
		break;
	case SEQ_STARTUPCHECK_STEP_SCREW_01_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_01_END);
		break;
	case SEQ_STARTUPCHECK_STEP_SCREW_01_END:
		break;

	case SEQ_STARTUPCHECK_STEP_SCREW_02_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_02_END);
		break;
	case SEQ_STARTUPCHECK_STEP_SCREW_02_END:
		break;

	case SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT);
		break;
	case SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_WAIT:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END);
		break;
	case SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END:
		break;

	case SEQ_STARTUPCHECK_STEP_PREVIEW_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_PREVIEW_END);
		break;
	case SEQ_STARTUPCHECK_STEP_PREVIEW_END:
		break;

	case SEQ_STARTUPCHECK_STEP_TLA_TEST_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_TLA_TEST);
		break;
	case SEQ_STARTUPCHECK_STEP_TLA_TEST:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_TLA_TEST_END);
		break;
	case SEQ_STARTUPCHECK_STEP_TLA_TEST_END:
		break;

	case SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST);
		break;
	case SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END);
		break;
	case SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END:
		break;

	case SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST);
		break;
	case SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END);
		break;
	case SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END:
		break;

	case SEQ_STARTUPCHECK_STEP_UNLOCK_START:
		if (p->IsStatusComplete())
			p->SetNextStep(SEQ_STARTUPCHECK_STEP_UNLOCK_END);
		break;
	case SEQ_STARTUPCHECK_STEP_UNLOCK_END:
		break;





		// (INIT)
// 	case SEQ_STARTUPCHECK_STEP_INIT_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_PREVIEW_STOP);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_PREVIEW_STOP:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_POWER_OFF);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_POWER_OFF:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_SAFETY_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_SOCKET_UP:					// SOCKET UP
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_SAFETY_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_SAFETY_POS:			// 안전위치로 UP
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_MOVE_X_LOAD_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_MOVE_X_LOAD_POS:			// X 로딩 위치로 이동(Z축이 안전위치인지 확인하자)
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_INSPECTION_CONVEYOR_CHECK);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_INSPECTION_CONVEYOR_CHECK: // 내려놓을곳에 자재 있는지 확인(센서 감지 상태 먼저 확인하자)
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_LOAD_PROBE_DN);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_LOAD_PROBE_DN:				// PROBE DN
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_LOAD_GRIPOFF);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_LOAD_GRIPOFF:				// GRIP OFF
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_LOAD_SOCKET_DN);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_LOAD_SOCKET_DN:				// 선택 포트 SOCKET DN
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_CONVEYOR_WAIT_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_MOVE_Z_CONVEYOR_WAIT_POS:	// Z축 컨베이어 대기 위치로 이동 
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_FINALIZE);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_FINALIZE:	
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_INIT_END);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_INIT_END:
// 		break;

	// (GRIP)
// 	case SEQ_STARTUPCHECK_STEP_GRIP_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET_ON_CONVEYOR);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET_ON_CONVEYOR:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_GRIP_OFF);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_GRIP_OFF:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_GRIP_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_GRIP_POS:				// Z축 GRIP 위치로 올리기
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_CHECK_LOADING);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_GRIP_CHECK_LOADING:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_GRIP_CHECK_SOCKET:				// Z축 GRIP 위치로 올리기
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_CHECK_CAMERA_REVERSE);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_GRIP_CHECK_CAMERA_REVERSE:				// Z축 GRIP 위치로 올리기
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_GRIP_ON);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_GRIP_GRIP_ON:						// GRIP ON
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_GRIP_RE_OFF);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_GRIP_RE_OFF:						// GRIP OFF
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_PROBE_CHANGE_MODEL);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_PROBE_CHANGE_MODEL:				// CHANGE  MODEL 선택
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON:						// VACCUM
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_SOCKET_UP);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_SOCKET_UP:						// SOCKET UP
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_PROBE_UP);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_PROBE_UP:						// PROBE UP
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON_CHECK);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_VACCUM_ON_CHECK:				// VACCUM CHECK
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_SAFETY_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_MOVE_Z_SAFETY_POS:				// Z축 안전위치로 이동
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_GRIP_END);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_GRIP_END:
// 		break;

		// (INSPECTION POS)
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_POWER_ON);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_POWER_ON:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_READY);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_READY:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_INITIALIZE);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_SEMCO_INITIALIZE:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_Z_SAFETY_CHECK);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_Z_SAFETY_CHECK:			// Z축 안전위치 확인
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_X_INSPECTION_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_X_INSPECTION_POS:	// X 검사 위치로 이동(Z축 안전위치 확인)
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_Z_INSPECTION_POS);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_MOVE_Z_INSPECTION_POS:	// Z 축 검사 위치로 이동
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_TEST);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_TEST:	// Z 축 검사 위치로 이동
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_END);
// 		break;
// 	case SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_END:
// 		break;
			
	// COLOR CAL INIT
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_SAFETY_POS);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_SAFETY_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_X_CL500A_POS);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_X_CL500A_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_CL500A_WAIT_POS);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_MOVE_Z_CL500A_WAIT_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_FINALIZE);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_FINALIZE:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_END);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_END:
// 		break;

	// COLOR CAL CL500A 안착
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_JIGCHECK);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_JIGCHECK:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_CONNECT);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_CONNECT:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_MOVE_Z_TEST_POS);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_MOVE_Z_TEST_POS:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_END);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_END:
// 		break;

	// COLOR CAL CL500A
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_START:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST:
// 		if (p->IsStatusComplete())
// 			p->SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_END);
// 		break;
// 
// 	case SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_END:
// 		break;
// 
// 	case SEQ_STARTUPCHECK_STEP_COMPLETE:
// 		break;
		
	default:
		break;
	}
#endif
}

bool CSequence::IsTesting_AutoRun()
{
	DWORD dwStatus = NULL;

	if (GetExitCodeThread(m_pThreadRun, &dwStatus))
	{
		if (STILL_ACTIVE == dwStatus)
			return true;
		else
			return false;
	}
	return false;
}

bool CSequence::IsTesting_SemiAuto()
{
	DWORD dwStatus = NULL;

	if (GetExitCodeThread(m_hThreadSemiAuto, &dwStatus))
	{
		if (STILL_ACTIVE == dwStatus)
			return true;
		else
			return false;
	}
	return false;
}

bool CSequence::IsTesting_StartupCheck()
{
	DWORD dwStatus = NULL;

	if (GetExitCodeThread(m_hThreadStartupCheck, &dwStatus))
	{
		if (STILL_ACTIVE == dwStatus)
			return true;
		else
			return false;
	}
	return false;
}

void CSequence::Initialize_Run()
{
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_DISTORTION:
// 	{
		auto st = DATAMANAGER->GetSemiAuto();
//		MO_MoveChart_To_AngleType(st->Angle);
// 	}
// 		break;
// 	default:
// 		break;
// 	}

// 	if (m_SeqPickerMgr.GetStep() == enSeqPickerMgrStep::SEQ_PICKERMGR_STEP_IDEL)
 		m_SeqPickerMgr.SetNextStep(SEQ_PICKERMGR_STEP_MAIN);

//	m_SeqPickerMgr.SetNextStep(SEQ_PICKERMGR_STEP_INSPECTION_OC_SCREW_UNLOCK_START);
	
}

enMCErrCode CSequence::StartOperation_Run()
{
	CString strMsg; strMsg.Empty();

	enMCErrCode err = MCEC_OK;

// 	auto * pConveyor1 = &m_SeqConveyor1;
// 	auto * pConveyor2 = &m_SeqConveyor2;
// 	auto * pConveyorReturn = &m_SeqConveyorReturn;
	auto * pPickermgr = &m_SeqPickerMgr;

	ICS_SendEquipmentStatus(EquipmentStatus::Run, 0, _T(""));

	while (IsThreadRun() == TRUE)
	{
		Sleep(50);

#ifdef DevMode_Simulator

#else		
		if (COMMON->Check_EMO() == FALSE)
			break;

// 		if (COMMON->Check_EMO() == FALSE)
// 			break;

		// 설비별로 감지시 에러 항목 구현 
// 		err = CheckStart_Auto();
// 		if (err != enMCErrCode::MCEC_OK) break;

#endif	
		if (GetRunMode() == ePause)
			continue;

// 		if (GetRunMode() == eStop)
// 			continue;



		if (err != MCEC_OK)
		{
			// 만약에 conveyor1, return 컨베이어위에 소켓이 이동중일 경우 회전이 완료 된후 종료하자..
			if (IsAutoRunStop())
				break;
		}

		if (GetRunMode() == eStop)
		{
			// 만약에 conveyor1, return 컨베이어위에 소켓이 이동중일 경우 회전이 완료 된후 종료하자..
			if (IsAutoRunStop())
				break;
		}
		//Run_Picker();				
		// 알람 발생시
		if(err == MCEC_OK)
			err = CheckMCError();
	}

	return err;
}

void CSequence::Finalize_Run(__in enMCErrCode err)
{
	CString sz;

// 	MO_MoveConveyor1_Stop();
// 	MO_MoveConveyor2_Stop();
// 	MO_MoveConveyorRet_Stop();

	if (err != MCEC_OK)
	{
		EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();
		
// 		if (m_SeqConveyor1.GetErrorCode() != MCEC_OK)
// 		{
// 			CString szPort;
// 			if (stEq.nLanguage == 0)
// 				sz = g_szMachineErrKor[err];
// 			else
// 				sz = g_szMachineErrEng[err];
// 
// 			szPort = _T("Conveyor1 ");
// 			sz = szPort + sz;
// 			goto TEST_END;
// 		}
// 
// 		if (m_SeqConveyor2.GetErrorCode() != MCEC_OK)
// 		{
// 			CString szPort;
// 			if (stEq.nLanguage == 0)
// 				sz = g_szMachineErrKor[err];
// 			else
// 				sz = g_szMachineErrEng[err];
// 
// 			szPort = _T("Conveyor2 ");
// 			sz = szPort + sz;
// 			goto TEST_END;
// 		}
// 
// 		if (m_SeqConveyorReturn.GetErrorCode() != MCEC_OK)
// 		{
// 			CString szPort;
// 			if (stEq.nLanguage == 0)
// 				sz = g_szMachineErrKor[err];
// 			else
// 				sz = g_szMachineErrEng[err];
// 
// 			szPort = _T("Return Coneveyor ");
// 			sz = szPort + sz;
// 			goto TEST_END;
// 		}

// 		if (m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Left) != MCEC_OK)
// 		{
// 			CString szPort;
// 
// 			if (stEq.nLanguage == 0)
// 				sz = g_szMachineErrKor[err];
// 			else
// 				sz = g_szMachineErrEng[err];
// 
// 			szPort = _T("LEFT ");
// 			sz = szPort + sz;
// 			goto TEST_END;
// 		}

// 		if (m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Right) != MCEC_OK)
// 		{
// 			CString szPort;
// 
// 			if (stEq.nLanguage == 0)
// 				sz = g_szMachineErrKor[err];
// 			else
// 				sz = g_szMachineErrEng[err];
// 
// 			szPort = _T("RIGHT ");
// 			sz = szPort + sz;
// 			goto TEST_END;
// 		}

		if (m_SeqPickerMgr.GetPickerErrorCode() != MCEC_OK)
		{
			CString szPort;
			if (stEq.nLanguage == 0)
				sz = g_szMachineErrKor[err];
			else
				sz = g_szMachineErrEng[err];

			szPort = _T("Handler");
			sz = szPort + sz;
			goto TEST_END;
		}		
	}

TEST_END:
	if (err != enMCErrCode::MCEC_OK)
	{
		Stop_Run(eError);
		ICS_SendEquipmentStatus(EquipmentStatus::Alarm, (int)err, sz);

		CString seqStr;
		seqStr.Format(_T("O,-,-,Error,TLA:%s,%s,Error_Code=%d"), "", sz, (int)err);
		WRITE_SEMCO_LOG(seqStr);

		SHOW_ERROR(err, sz);

	}		
	else
		ICS_SendEquipmentStatus(EquipmentStatus::Idle, 0, _T(""));
	
	Stop_Run();
	
	SetWinThreadRun(NULL);
	SetThreadRun(FALSE);
}

// 만약에 conveyor1, return 컨베이어위에 소켓이 이동중일 경우 회전이 완료 된후 종료하자..
bool CSequence::IsAutoRunStop()
{
	bool bRet = true;

// 	auto step_cv1 = m_SeqConveyor1.GetStep();
// 	auto step_cv2 = m_SeqConveyor2.GetStep();
// 	auto step_cvr = m_SeqConveyorReturn.GetStep();

	// 소켓 투입중일때..
// 	if (step_cv1 == SEQ_CV1_STEP_MOVE_TO_RFID_CHECK_RFID_STOP_SENSOR)
// 	{
// 		if (m_SeqConveyor1.GetStatus() != enSeqStatus::Status_Error)
// 		{
// // 			if (IO_I_CV1_IF_FrontIn())
// // 				bRet = false;
// 		}
// 	}
// 
// 	// 소켓 배출중일때..
// 	if (step_cv2 == SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN
// 		|| step_cv2 == SEQ_CV2_STEP_MOVE_BYPASS_RUN)
// 	{
// 		if (m_SeqConveyor2.GetStatus() != enSeqStatus::Status_Error)
// 		{
// // 			if (IO_I_CV2_IF_RearIn())
// // 				bRet = false;
// 		}
// 	}
// 
// 	if (step_cvr == SEQ_CVR_STEP_MOVE_BYPASS_RUN)
// 	{
// 		if (m_SeqConveyorReturn.GetStatus() != enSeqStatus::Status_Error)
// 		{
// // 			if (IO_I_CVR_IF_FrontOut())
// // 				bRet = false;
// 		}
// 	}
// 
// 	if (step_cvr == SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_FROM_IN
// 		||step_cvr == SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_SLOW_SENSOR)
// 	{
// 		bRet = false;
// 	}
// 
// 	if (step_cvr == SEQ_CVR_STEP_MOVE_BYPASS_WAIT_SOCKET_TO_STOP_SENSOR)
// 	{
// 		if (m_SeqConveyorReturn.GetStatus() == enSeqStatus::Status_Complete)
// 		{
// 
// 		}else if (m_SeqConveyorReturn.GetStatus() != enSeqStatus::Status_Error)
// 			bRet = false;
// 		
// 	}

	//auto err_cv1 = m_SeqConveyor1.IsError();
	//auto err_cv2 = m_SeqConveyor2.IsError();
	//auto err_cvr = m_SeqConveyorReturn.IsError();

	//auto err_pickmgr = m_SeqPickerMgr.IsError();

	//auto err_left = m_SeqPickerMgr.IsErrorPicker(InspectionPortType::Left);
	//auto err_right = m_SeqPickerMgr.IsErrorPicker(InspectionPortType::Right);

	//if (err_pickmgr || err_left || err_right)
	//{
	//	// cv2 
	//	// SEQ_CV2_STEP_MOVE_TO_UNLOADING_RUN
	//	// SEQ_CV2_STEP_MOVE_BYPASS_RUN

	//	// cvr
	//	//
	//	// SEQ_CVR_STEP_MOVE_BYPASS_RUN
	//}
	
	
	// cv1 상황
	/*
	1. 앞설비에서 소켓이 전달되고 있을 경우
	   --> 소켓은 다 받고 정지하자


	// cv3
	1. 투입설비에서 제품 투입 할경우
	   --> 소켓 다 받고 배출 대기 위치로 가서 정지하자(배출 쪽 설비에는 신호를 보내주면 안됨)

	2. 배출설비방향으로 제품 배출하고 있을 경우
	   --> 주고 있던 신호는 그대로 유지하고, 소켓을 다 배출한후 정지하자..

	//
	cv1 투입 , cv2 배출

	cv3 투입, 배출
	*/

	// left, right, pick mgr 에서 에러 발생시
	/*if (IsErrorAtPickerz())
	{

	}

	if (IsErrorAtConveyor1())
	{

	}

	if (IsErrorAtConveyor2())
	{

	}

	if (IsErrorAtConveyorRet())
	{

	}*/

	return bRet;
}

void CSequence::Initialize_SemiAuto(__in InspectionPortType port,  __in UINT nIdx)
{
	// UI 업데이트
	UpdateUI_SemiAuto_Start(port, nIdx);
	
	if (nIdx == 0)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_LOADING_START);
	if (nIdx == 1)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_LOCK_START);
	if (nIdx == 2)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_PREVIEW_START);
	if (nIdx == 3)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_TLA_START);
	if (nIdx == 4)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_VIGNETING_START);
	if (nIdx == 5)
		m_SeqSemiAuto.SetNextStep(SEQ_SEMIAUTO_STEP_UNLOADING_START);

	m_SeqSemiAuto.ErrorClear();
	m_SeqSemiAuto.SetPortId(port);
	m_SeqSemiAuto.Start();
}

enMCErrCode CSequence::StartOperation_SemiAuto()
{
	enMCErrCode err = MCEC_OK;

	auto * p = &m_SeqSemiAuto;
	
	while (IsThread_SemiAuto())
	{
		Sleep(10);

#ifdef DevMode_Simulator

#else
		if (COMMON->Check_EMO() == FALSE)
			break;

		// 설비별로 감지시 에러 항목 구현 
		err = CheckStart_SemiAuto();
		if (err != enMCErrCode::MCEC_OK) break;
#endif	
		if (GetRunMode() == eRunning)
			continue;
// 
// 		if (GetRunMode() == ePause)
// 			continue;

		Run_SemiAuto();

		if (p->IsError())
		{
			err = p->GetErrorCode();
			break;
		}		
		
		if (p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_LOADING_END
			|| p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_LOCK_END
			|| p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_PREVIEW_END
			|| p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_TLA_END
			|| p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_VIGNETING_END
			|| p->GetStep() == enSeqSemiAutoStep::SEQ_SEMIAUTO_STEP_UNLOADING_END)
		{
			break;
		}
	}
	return err;
}

void CSequence::Finalize_SemiAuto(__in enMCErrCode err, __in int nIdx)
{
	if (err != MCEC_OK)
		SHOW_ERROR(err);		

	bool bResultOk;

	m_SeqSemiAuto.ForceStop();
	//m_SeqSemiAuto.Stop();
	bResultOk = (err == enMCErrCode::MCEC_OK) ? true : false;

	// UI 업데이트
	UpdateUI_SemiAuto_End(m_SeqSemiAuto.GetPortid(),nIdx, bResultOk);
}

void CSequence::Initialize_StartupCheck(__in InspectionPortType port, __in UINT nIdx)
{
	// UI 업데이트
	UpdateUI_StartUpCheck_Start(port, nIdx);

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		if (nIdx == 0)
// 			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_START);
// 
// 		if (nIdx == 1)
// 			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_START);
// 
// 		if (nIdx == 2)
// 			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_START);
// 		break;
// 
// 	default:
		if (nIdx == 0)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_READY);
		if (nIdx == 1)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_01_START);
		if (nIdx == 2)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_SCREW_02_START);
		if (nIdx == 3)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_START);
		if (nIdx == 4)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_PREVIEW_START);
		if (nIdx == 5)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_TLA_TEST_START);
		if (nIdx == 6)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_START);
		if (nIdx == 7)
			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_UNLOCK_START);
// 		if (nIdx == 8)
// 			m_SeqStartupCheck.SetNextStep(SEQ_STARTUPCHECK_STEP_UNLOCK_START);
//		break;
//	}

	m_SeqStartupCheck.ErrorClear();
	m_SeqStartupCheck.SetPortId(port);
	m_SeqStartupCheck.Start();
}

enMCErrCode CSequence::StartOpertation_StartupCheck()
{
	enMCErrCode err = MCEC_OK;

	auto * p = &m_SeqStartupCheck;

	while (IsThread_StartupCheck())
	{
		Sleep(10);

#ifdef DevMode_Simulator

#else
		if (COMMON->Check_EMO() == FALSE)
			break;
		


		// 설비별로 감지시 에러 항목 구현 
		err = CheckStart_StartupCheck();
		if (err != enMCErrCode::MCEC_OK) break;
#endif	
		if (GetRunMode() == eRunning)
			continue;

// 		if (GetRunMode() == ePause)
// 			continue;

		Run_StartupCheck();

		if (p->IsError())
		{
			err = p->GetErrorCode();
			break;
		}

		if (p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_READY
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_01_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_SCREW_02_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_LOCK_STARTINGPOS_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_PREVIEW_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_TLA_TEST_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_46_TEST_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_VIGNETING_82_TEST_END
			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_UNLOCK_END
			
			
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_INIT_END
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_GRIP_END
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_STEP_MOVEINSPECTION_END
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_COLORCAL_STEP_INIT_END
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_LOAD_END
// 			|| p->GetStep() == enSeqStartupCheckStep::SEQ_STARTUPCHECK_COLORCAL_STEP_CL500A_TEST_END
		)
		{
			break;
		}
	}
	return err;
}

void CSequence::Finalize_StartupCheck(__in enMCErrCode err, __in int nIdx)
{
	if (err != MCEC_OK)
		SHOW_ERROR(err);

	bool bResultOk = false;

	m_SeqStartupCheck.ForceStop();

	bResultOk = (err == enMCErrCode::MCEC_OK) ? true : false;

	// UI 업데이트
	UpdateUI_StartUpCheck_End(m_SeqStartupCheck.GetPortid(), nIdx, bResultOk);
}

enMCErrCode CSequence::CheckStart_Auto()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

//	auto EqpType = SET_INSPECTOR;

	// 공통

	// air 공압 알람 발생시

// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Left) == true)
// 			err = MCEC_IO_PICKER_CL500A_JigCheck1_Detect;

		//if (err != enMCErrCode::MCEC_OK) break;
		if (err != enMCErrCode::MCEC_OK) return err;

// 		if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Right) == true)
// 			err = MCEC_IO_PICKER_CL500A_JigCheck2_Detect;
// 		break;
// 
// 	default:
// 		break;
// 	}

	return err;
}

enMCErrCode CSequence::CheckStart_SemiAuto()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Left) == true)
// 			err = MCEC_IO_PICKER_CL500A_JigCheck1_Detect;
// 
// 		//if (err != enMCErrCode::MCEC_OK) break;
// 		if (err != enMCErrCode::MCEC_OK) return err;
// 
// 		if (IO_I_Picker_CL500A_JigCheck(InspectionPortType::Right) == true)
// 			err = MCEC_IO_PICKER_CL500A_JigCheck2_Detect;
// 		break;
// 
// 	default:
// 		break;
// 	}

	return err;
}

enMCErrCode CSequence::CheckStart_StartupCheck()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;

	return err;
}

enMCErrCode CSequence::CheckMCError()
{
	enMCErrCode err = enMCErrCode::MCEC_OK;
	CString sz;

// 	if (m_SeqConveyor1.GetErrorCode() != MCEC_OK)
// 	{
// 		err = m_SeqConveyor1.GetErrorCode();
// 
// 		if(err < MCEC_MaxNum)
// 			sz.Format(_T("Conveyor1 Error [Step : %s], [Code : %s]"), g_szSeqName_CV1[m_SeqConveyor1.GetStep()], g_szMachineErrEng[err]);
// 		else 
// 			sz.Format(_T("Conveyor1 Error [Step : %s], [Code : Unknown]"), g_szSeqName_CV1[m_SeqConveyor1.GetStep()]);
// 		LogError(sz);		
// 	}
// 
// 	if (m_SeqConveyor2.GetErrorCode() != MCEC_OK)
// 	{
// 		err = m_SeqConveyor2.GetErrorCode();
// 
// 		if(err < MCEC_MaxNum)
// 			sz.Format(_T("Conveyor2 Error [Step : %s], [Code : %s]"), g_szSeqName_CV2[m_SeqConveyor2.GetStep()], g_szMachineErrEng[err]);
// 		else 
// 			sz.Format(_T("Conveyor2 Error [Step : %s], [Code : Unknown]"), g_szSeqName_CV2[m_SeqConveyor2.GetStep()]);
// 		LogError(sz);
// 	}
// 
// 	if (m_SeqConveyorReturn.GetErrorCode() != MCEC_OK)
// 	{
// 		err = m_SeqConveyorReturn.GetErrorCode();
// 
// 		if(err < MCEC_MaxNum)
// 			sz.Format(_T("Conveyor Return Error [Step : %s], [Code : %s]"), g_szSeqName_CVR[m_SeqConveyorReturn.GetStep()], g_szMachineErrEng[err]);
// 		else 
// 			sz.Format(_T("Conveyor Return Error [Step : %s], [Code : Unknown]"), g_szSeqName_CVR[m_SeqConveyorReturn.GetStep()]);
// 		LogError(sz);
// 	}

	if (m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Left) != MCEC_OK)
	{
		err = m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Left);

		if(err < MCEC_MaxNum)
			sz.Format(_T("Picker Left Error [Step : %s], [Code : %s]"), g_szSeqName_Picker[m_SeqPickerMgr.GetPickerStep(InspectionPortType::Left)], g_szMachineErrEng[err]);
		else
			sz.Format(_T("Picker Left Error [Step : %s], [Code : Unknown]"), g_szSeqName_Picker[m_SeqPickerMgr.GetPickerStep(InspectionPortType::Left)]);

		LogError(sz);
	}

	if (m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Right) != MCEC_OK)
	{
		err = m_SeqPickerMgr.GetPickerErrorCode(InspectionPortType::Right);

		if (err < MCEC_MaxNum)
			sz.Format(_T("Picker Right Error [Step : %s], [Code : %s]"), g_szSeqName_Picker[m_SeqPickerMgr.GetPickerStep(InspectionPortType::Right)], g_szMachineErrEng[err]);
		else
			sz.Format(_T("Picker Right Error [Step : %s], [Code : Unknown]"), g_szSeqName_Picker[m_SeqPickerMgr.GetPickerStep(InspectionPortType::Right)]);

		LogError(sz);
	}

	if (m_SeqPickerMgr.GetPickerErrorCode() != MCEC_OK)
	{
		err = m_SeqPickerMgr.GetPickerErrorCode();

		if (err < MCEC_MaxNum)
			sz.Format(_T("Picker mgr Error [Step : %s], [Code : %s]"), g_szSeqName_PickerMgr[m_SeqPickerMgr.GetStep()], g_szMachineErrEng[err]);
		else
			sz.Format(_T("Picker mgr Error [Step : %s], [Code : Unknown]"), g_szSeqName_PickerMgr[m_SeqPickerMgr.GetStep()]);

		LogError(sz);
	}

	return err;
}

bool CSequence::GetSocketInfo(__in ESOCKET_POS_LIST spl, __out SOCKET_INFO & socket)
{
	bool bRet = false;
	switch (spl)
	{
// 	case eSPL_CV_INPUT:	
// 	{
// 		ST_SocketInfo info;
// 		//bRet = m_SeqConveyor1.GetInputSocketInfo(info);
// 
// 		socket.strRFID		= info.szRFID;
// 		socket.strSerialNo	= info.szBCR;
// 	}
// 		break;
// 
// 	case eSPL_CV_BUFFER:
// 	{
// 		ST_SocketInfo info;
// 		//bRet = m_SeqConveyor1.GetBufferSocketInfo(info);
// 
// 		socket.strRFID		= info.szRFID;
// 		socket.strSerialNo	= info.szBCR;
// 	}
// 		break;
// 
// 	case eSPL_CV_INSPECTION:
// 	{
// 		ST_SocketInfo info;
// 		bRet = m_SeqConveyor2.GetSocketInfo(info);
// 
// 		socket.strRFID		= info.szRFID;
// 		socket.strSerialNo	= info.szBCR;
// 	}
// 		break;
// 
// 	case eSPL_CV_RETURN:
// 		break;

	case eSPL_PICK_LEFT:
	{
		ST_SocketInfo info;
		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Left, info);

		socket.strRFID		= info.szRFID;
		socket.strSerialNo	= info.szBCR;
	}
		break;
	case eSPL_PICK_RIGHT:
	{
		ST_SocketInfo info;
		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Right, info);

		socket.strRFID		= info.szRFID;
		socket.strSerialNo	= info.szBCR;
	}
		break;

	case eSPL_MAX:
		break;

	default:
		break;
	}
	return bRet;
}

// bool CSequence::GetSocketInfo(__in SocketPos pos, __out ST_SocketInfo & socket)
// {
// 	bool bRet = false;
// 
// 	switch (pos)
// 	{
// // 	case SocketPos::Cv1_Rfid:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyor1.GetInputSocketInfo(info);
// // 		socket = info;
// // 	}
// // 		break;
// // 	case SocketPos::Cv1_Buffer:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyor1.GetBufferSocketInfo(info);
// // 		socket = info;
// // 	}
// // 		break;
// // 	case SocketPos::Cv2_L_loading:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyor2.GetSocketInfo(info);
// // 		socket = info;
// // 
// // 		if (m_SeqConveyor2.GetWorkPort() != InspectionPortType::Left)
// // 			bRet = false;
// // 
// // 		if (info.bBypass)
// // 			bRet = false;
// // 	}
// // 		break;
// // 	case SocketPos::Cv2_R_loading:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyor2.GetSocketInfo(info);
// // 		socket = info;
// // 
// // 		if (m_SeqConveyor2.GetWorkPort() != InspectionPortType::Right)
// // 			bRet = false;
// // 
// // 		if (info.bBypass)
// // 			bRet = false;
// // 	}
// // 		break;
// // 	case SocketPos::Cv2_Bypass:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyor2.GetSocketInfo(info);
// // 		socket = info;
// // 
// // 		if (!info.bBypass)
// // 			bRet = false;
// // 	}
// // 		break;
// 	case SocketPos::Pic_L_Wait:
// 	{
// 		ST_SocketInfo info;
// 		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Left, info);
// 		socket = info;
// 
// 		if (info.status == Testing)	bRet = false;
// 	}
// 		break;
// 	case SocketPos::Pic_L_Insp:
// 	{
// 		ST_SocketInfo info;
// 		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Left, info);
// 		socket = info;
// 
// 		if (info.status != Testing)	bRet = false;
// 	}
// 		break;
// 	case SocketPos::Pic_R_Wait:
// 	{
// 		ST_SocketInfo info;
// 		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Right, info);
// 		socket = info;
// 
// 		if (info.status == Testing)	bRet = false;
// 	}
// 		break;
// 	case SocketPos::Pic_R_Insp:
// 	{
// 		ST_SocketInfo info;
// 		bRet = m_SeqPickerMgr.GetSocketInfo(InspectionPortType::Right, info);
// 		socket = info;
// 
// 		if (info.status != Testing)	bRet = false;
// 	}
// 		break;
// // 	case SocketPos::Cvr_Bypass:
// // 	{
// // 		ST_SocketInfo info;
// // 		bRet = m_SeqConveyorReturn.GetSocketInfo(info);
// // 		socket = info;
// // 	}
// // 		break;
// 	case SocketPos::Max:
// 		break;
// 	default:
// 		break;
// 	}
// 	return bRet;
// }

//----------------------------------------------------------------------------------
// Thread.
bool CSequence::Begin_Thread(ETHREAD_ID tid)
{
	if (tid == THREAD_MANUAL)
	{
// 		if (m_pThread[tid])
// 		{
// 			End_Thread(tid);
// 		}
// 		m_bThread[tid] = TRUE;
// 		m_pThread[tid] = AfxBeginThread(Thread_Manual, this, THREAD_PRIORITY_NORMAL);
	}
	else if (tid == THREAD_INITIAL)
	{
		if (m_pThread[tid])
		{
			End_Thread(tid);
		}
		m_bThread[tid] = TRUE;
		m_pThread[tid] = AfxBeginThread(Thread_Initial, this, THREAD_PRIORITY_NORMAL);
	}
	else if (tid == THREAD_INTLK)
	{
// 		if (m_pThread[tid])
// 		{
// 			End_Thread(tid);
// 		}
// 		m_bThread[tid] = TRUE;
// 		m_pThread[tid] = AfxBeginThread(Thread_Intlk, this, THREAD_PRIORITY_NORMAL);
	}
	return true;
}

void CSequence::End_Thread(ETHREAD_ID tid)
{
	if (m_bThread[tid] == TRUE)
	{
		m_bThread[tid] = FALSE;
		WaitForSingleObject(m_pThread[tid]->m_hThread, 500);
	}
}

INIT_STATUS CSequence::GetInitStatus()
{
	return (INIT_STATUS)m_nInitStatus;
}

void CSequence::SetInitStatus(INT16 is)
{
	m_nInitStatus = is;
}

int CSequence::GetStpInitial()
{
	UINT step;
	return (step = SEQUENCE->m_RunInfo[THREAD_INITIAL].nStep);
}

void CSequence::SetStpInitial(int step)
{
	SEQUENCE->m_RunInfo[THREAD_INITIAL].nStep = step;
}

void CSequence::Start_Initial()
{
	if (m_bThread[THREAD_INITIAL] == FALSE)
	{
		Begin_Thread(THREAD_INITIAL);
	}
	if (GetRunMode() == eRunning)
		return;

//	m_nRunMode = eInitializing;
	SetRunMode(eInitializing);

	WRITE_SEQ_LOG(_T("INITIAL Start."));
}

UINT CSequence::Thread_Initial(LPVOID lpVoid)
{
	RUN_MODE rm;
//	SEQUENCE->ResetError();

	while (SEQUENCE->m_bThread[THREAD_INITIAL] == TRUE)
	{
		Sleep(20);

		if ((rm = SEQUENCE->GetRunMode()) != eInitializing)
			break;

		SEQUENCE->Run_Total_Origin();
	}


	if (SEQUENCE->m_RunInfo[THREAD_INITIAL].bError == 0)
	{
		SEQUENCE->nhomeprogress = 1;
	}
	
	SEQUENCE->m_pThread[THREAD_INITIAL] = NULL;
	SEQUENCE->m_bThread[THREAD_INITIAL] = FALSE;

	return 0;
}

BOOL	CSequence::Run_Total_Origin()
{
	TESTER_COMM		stTester = *DATAMANAGER->GetCurrentTesterCommData();
	TEACHING_PARAM	stTeaching = *DATAMANAGER->GetCurrentTeachingData();
	MOTION_PARAM	stMotion = *DATAMANAGER->GetCurrentMotionParam();

	ETHREAD_ID eThrID = THREAD_INITIAL;

	BOOL b1, b2, b3, b4, b5, b6;
	BOOL bError = FALSE;
	time_t time = m_RunInfo[eThrID].nTimer;
	int step = m_RunInfo[eThrID].nStep;
	time_t time_now;
	b1 = FALSE; 
	b2 = FALSE; 
	b3 = FALSE; 
	b4 = FALSE; 
	b5 = FALSE; 
	b6 = FALSE;
	CString szMsg;
	enMCErrCode err = enMCErrCode::MCEC_OK;
	switch (step)
	{
	case 0:
		SetInitStatus(eInit_Working);
		ICS_SendEquipmentStatus(EquipmentStatus::Init, 0, _T(""));

		IO_O_SleepMode(false);
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		AJIN_IO->SetOFF(DIO_OUTPUT_Y1_00_STAU_CHUCK_GRIP);
#endif

		if (AJIN_MO->m_bReadError[0])
		{
			if (!AJIN_MO->SetErrorReset((long)VIGNETTING_X))
			{
				WRITE_MAIN_LOG(_T("[ALARM] VIGNETTING_X Alarm Reset Failed\n"));
				bError = TRUE;
			}
		}
		if (AJIN_MO->m_bReadError[1])
		{
			if (!AJIN_MO->SetErrorReset((long)VIGNETTING_R))
			{
				WRITE_MAIN_LOG(_T("[ALARM] VIGNETTING_R Alarm Reset Failed\n"));
				bError = TRUE;
			}
		}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		if (AJIN_MO->m_bReadError[2])
		{
			if (!AJIN_MO->SetErrorReset((long)SCREW_L_X))
			{
				WRITE_MAIN_LOG(_T("[ALARM] SCREW_L_X Alarm Reset Failed\n"));
				bError = TRUE;
			}
		}
		if (AJIN_MO->m_bReadError[3])
		{
			if (!AJIN_MO->SetErrorReset((long)SCREW_R_X))
			{
				WRITE_MAIN_LOG(_T("[ALARM] SCREW_R_X Alarm Reset Failed\n"));
				bError = TRUE;
			}
		}
		if (AJIN_MO->m_bReadError[4])
		{
			if (!AJIN_MO->SetErrorReset((long)TILT_ADJUST_R))
			{
				WRITE_MAIN_LOG(_T("[ALARM] TILT_ADJUST_R Alarm Reset Failed\n"));
				bError = TRUE;
			}
		}
#endif

		Sleep(500);
		IO_O_Socket_Unlock(FALSE);
		IO_O_Chuck_Bwd(FALSE);
		IO_O_JIG_L_Lock(FALSE);
		IO_O_JIG_R_Lock(FALSE);


		AJIN_MO->ServoOn((long)VIGNETTING_X);
		AJIN_MO->ServoOn((long)VIGNETTING_R);
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		AJIN_MO->ServoOn((long)SCREW_L_X);
		AJIN_MO->ServoOn((long)SCREW_R_X);
		AJIN_MO->ServoOn((long)TILT_ADJUST_R);
#endif
		Sleep(2000);
		if (!AJIN_MO->IsServoOn((long)VIGNETTING_X))
		{
			szMsg.Format(_T("[Initial] [SERVO ON] VIGNETTING X Servo On failed."));
			bError = TRUE;
		}
		if (!AJIN_MO->IsServoOn((long)VIGNETTING_R))
		{
			szMsg.Format(_T("[Initial] [SERVO ON] VIGNETTING R Servo On failed."));
			bError = TRUE;
		}
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		if (!AJIN_MO->IsServoOn((long)SCREW_L_X))
		{
			szMsg.Format(_T("[Initial] [SERVO ON] SCREW_L X Servo On failed."));
			bError = TRUE;
		}
		if (!AJIN_MO->IsServoOn((long)SCREW_R_X))
		{
			szMsg.Format(_T("[Initial] [SERVO ON] SCREW_R X Servo On failed."));
			bError = TRUE;
		}
		if (!AJIN_MO->IsServoOn((long)TILT_ADJUST_R))
		{
			szMsg.Format(_T("[Initial] [SERVO ON] TILT_ADJUST_R Servo On failed."));
			bError = TRUE;
		}
#endif
		Sleep(1000);
	
// 		IO_O_CV1_IF_FrontIn(false);
// 		IO_O_CVR_IF_FrontOut(false);
// 		IO_O_CV2_IF_RearIn(false);
// 		IO_O_CVR_IF_RearOut(false);

		step++;
		break;

	case 1:		// 컨베이어에 소켓 감지
	{
		#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR) &&
			AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR) &&
			AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR))
		{
			step = 20;
		}
		else
		{
			if (AJIN_IO->IsON(DIO_INPUT_X1_14_DBUL_JIG_IO_OUT_CHK_SENSOR))
			{
				szMsg.Format(_T("[Initial] Set DBUL Jig Out Sensnor Check Error"));
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_1A_DBUR_JIG_IO_OUT_CHK_SENSOR))
			{
				szMsg.Format(_T("[Initial] Set DBUR Jig Out Sensnor Check Error"));
			}
			else if (AJIN_IO->IsON(DIO_INPUT_X1_0A_STAU_CHUCK_BWD_IN_CHK_SENSOR))
			{
				szMsg.Format(_T("[Initial] Set STAU Jig Bwd Sensnor Check Error"));
			}
			bError = TRUE;
			break;
		}
		#elif(SET_INSPECTOR == SYS_TLA_OQC)
		step = 20;
		#endif
// 		enMCErrCode err = enMCErrCode::MCEC_OK;
// 		err = IO_Picker_CL500A_Empty_Check();
// 		if (enMCErrCode::MCEC_OK != err)
// 		{
// 			szMsg = g_szMachineErrEng[err];
// 			bError = true;
// 			break;
//		}
	
	}
		break;
	
	case 20:		// 디스토션 차트 원점 이동 
	{

		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			AJIN_MO->ResetOrigin(VIGNETTING_X);
			AJIN_MO->ResetOrigin(VIGNETTING_R);
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			AJIN_MO->ResetOrigin(SCREW_L_X);
			AJIN_MO->ResetOrigin(SCREW_R_X);
			AJIN_MO->ResetOrigin(TILT_ADJUST_R);
		}

		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			if (!AJIN_MO->Move_Home((long)VIGNETTING_X))
			{
				szMsg.Format(_T("[Initial] Set VIGNETTING X Move Home Error"));
				bError = TRUE;
				break;
			}
			if (!AJIN_MO->Move_Home((long)VIGNETTING_R))
			{
				szMsg.Format(_T("[Initial] Set VIGNETTING R Move Home Error"));
				bError = TRUE;
				break;
			}
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			if (!AJIN_MO->Move_Home((long)SCREW_L_X))
			{
				szMsg.Format(_T("[Initial] Set SCREW L X Move Home Error"));
				bError = TRUE;
				break;
			}
			if (!AJIN_MO->Move_Home((long)SCREW_R_X))
			{
				szMsg.Format(_T("[Initial] Set SCREW R X Move Home Error"));
				bError = TRUE;
				break;
			}
			if (!AJIN_MO->Move_Home((long)TILT_ADJUST_R))
			{
				szMsg.Format(_T("[Initial] Set TILT ADJUST R Move Home Error"));
				bError = TRUE;
				break;
			}
		}
		time = GetTickCount64();
		step++;
	}
		break;

	case 21:		// 디스토션 차트 원점 이동/완료 확인
	{
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE || SET_INSPECTOR == SYS_TLA_OQC)
		{
			b1 = AJIN_MO->HomeResult((long)VIGNETTING_X);
			b2 = AJIN_MO->HomeResult((long)VIGNETTING_R);
			b3 = TRUE;
			b4 = TRUE;
		}
		if (SET_INSPECTOR == SYS_TLA_ASSEMBLE)
		{
			b3 = AJIN_MO->HomeResult((long)SCREW_L_X);
			b4 = AJIN_MO->HomeResult((long)SCREW_R_X);
			//b5 = AJIN_MO->HomeResult(TILT_ADJUST_R);
		}


		//
		if (GetTickCount64() - time < TIMEOUT_DELAY_1000)
		{

		}
		else if (b1 == TRUE && b2 == TRUE && b3 == TRUE && b4 == TRUE/*&& b5 == TRUE*/)
		{
			time = GetTickCount64();
			step = 30;
		}
		else if (GetTickCount64() - time > TIMEOUT_DELAY_45000)
		{
			if (b1 == FALSE)
				szMsg.Format(_T("[Initial] VIGNETTING X Homing End TimeOut"));
			else if (b2 == FALSE)
				szMsg.Format(_T("[Initial] VIGNETTING R Homing End TimeOut"));
			else if (b3 == FALSE)
				szMsg.Format(_T("[Initial] SCREW L X Homing End TimeOut"));
			else if (b4 == FALSE)
				szMsg.Format(_T("[Initial] SCREW R X Homing End TimeOut"));
// 			else if (b5 == FALSE)
// 				szMsg.Format(_T("[Initial] TILT ADJUST R Homing End TimeOut"));
			bError = TRUE;
		}
	}
	break;

	case 30:
		if (Check_Pos_Screw_L_Type(Screw_Wait) == FALSE || Check_Pos_Screw_R_Type(Screw_Wait) == FALSE)
		{
			err = MO_MoveScrew_Type(Screw_Wait);
			if (err != enMCErrCode::MCEC_OK)
			{
				bError = TRUE;
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
					bError = TRUE;
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
					bError = TRUE;
					break;
				}
			}
		}
// 		if (!IO_Light_ShutterClose())
// 		{
// 			szMsg = _T("[Initial] Light Shutter could not close");
// 			bError = TRUE;
// 		}
// 		else
// 		{
			step = 99;
//		}
		break;

	case 99:		// Z1, Z2 안전 위치로 이동 (원점 수행완료)
	{
// 		if (MO_MoveZ_To_Safety(InspectionPortType::Left) != enMCErrCode::MCEC_OK)
// 		{
// 			bError = TRUE;
// 			szMsg.Format(_T("[Initial] Z1 Axis can not move to safety Position"));
// 			break;
// 		}
// 
// 		if (MO_MoveZ_To_Safety(InspectionPortType::Right) != enMCErrCode::MCEC_OK)
// 		{
// 			bError = TRUE;
// 			szMsg.Format(_T("[Initial] Z2 Axis can not move to safety Position"));
// 			break;
// 		}

		// 제품이 있을 경우 작업자가 뺄수있게 TRAY 를 DN 해야 한다.
// 		if (IO_I_Picker_SocketCheck(InspectionPortType::Left))
// 		{
// 			if (!IO_Picker_LoadSocketDn(InspectionPortType::Left))
// 			{
// 				bError = TRUE;
// 				szMsg.Format(_T("[Initial] Left Socket Tray Down Sensor Detect Timeout"));
// 				break;
// 			}
// 		}
// 
// 		if (IO_I_Picker_SocketCheck(InspectionPortType::Right))
// 		{
// 			if (!IO_Picker_LoadSocketDn(InspectionPortType::Right))
// 			{
// 				bError = TRUE;
// 				szMsg.Format(_T("[Initial] Right Socket Tray Down Sensor Detect Timeout"));
// 				break;
// 			}
// 		}

		// 시퀀스 스텝 & 데이타 초기화
		SEQUENCE->Reset_SeqFlags();

		szMsg.Format(_T("[Intial] Home completed."));
		COMMON->Write_Main_Log(szMsg);
		SetInitStatus(eInit_Done);
		Stop_Run(eStop);

		ICS_SendPortClear(PortType::All);
		ICS_SendEquipmentStatus(EquipmentStatus::Idle, 0, _T(""));
		AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_04_INIT_SWITCH_LAMP, FALSE);

		
	}
		break;
		
	default:
		break;
	}

	m_RunInfo[eThrID].nStep = step;
	m_RunInfo[eThrID].nTimer = time;
	m_RunInfo[eThrID].bError = bError;

	// Error 발생
	if (bError)
	{
		COMMON->ShowMsg_Thread(szMsg, MODE_ERROR);
		Stop_Run(eError);
	}

	return TRUE;
}
