// NI6509.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EziServo_PR.h"

#include "../Common.h"
#include "../DataManager.h"


// CEziServo_PR

CEziServo_PR *CEziServo_PR::m_pInstance = NULL;

CEziServo_PR* CEziServo_PR::Get_Instance()
{
	if (!m_pInstance) 
	{
		m_pInstance = new CEziServo_PR();
		if (!m_pInstance->m_hWnd) {
			m_pInstance->CreateEx(0, AfxRegisterWndClass(0), _T("CEziServo_PR"), 0, CRect(0, 0, 0, 0), NULL, 0, NULL);
		}
	}

	if (!m_pInstance) m_pInstance = new CEziServo_PR();
	return m_pInstance;
}

void CEziServo_PR::Delete_Instance()
{
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}

IMPLEMENT_DYNAMIC(CEziServo_PR, CWnd)
CEziServo_PR::CEziServo_PR()
{
	InitVariables();
}

CEziServo_PR::~CEziServo_PR()
{
	Disconnect_EziServo();
}


BEGIN_MESSAGE_MAP(CEziServo_PR, CWnd)
//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CEziServo_PR �޽��� ó�����Դϴ�.

BOOL CEziServo_PR::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // ȭ�� ���� ����
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);	
}

//������ �Ÿ���ŭ�� Pulse �� ���
long CEziServo_PR::GetPulse(double CommandPos, int nAxis)
{
	long lCalPulse = 0;

	switch(nAxis)
	{
	case AXIS_LOADER1:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_LOADER1_PITCH) * CommandPos);
		break;
	case AXIS_LOADER2:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_LOADER2_PITCH) * CommandPos);
		break;
	case AXIS_LOADER3:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_LOADER3_PITCH) * CommandPos);
		break;
	case AXIS_UNLOADER1:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_UNLOADER1_PITCH) * CommandPos);
		break;
	case AXIS_UNLOADER2:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_UNLOADER2_PITCH) * CommandPos);
		break;
	case AXIS_UNLOADER3:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_UNLOADER3_PITCH) * CommandPos);
		break;
	case AXIS_NGBUFFER1:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_NGBUFFER1_PITCH) * CommandPos);
		break;
	case AXIS_NGBUFFER2:
		lCalPulse = (long)((MOTOR_RESOLITION / AXIS_NGBUFFER2_PITCH) * CommandPos);
		break;
	}

	return lCalPulse;
}

//���� ������ �� mm�� ȯ��
double CEziServo_PR::GetActualmm(long ActualPos, int nAxis)
{
	double dValue = 0;

	switch(nAxis)
	{
	case AXIS_LOADER1:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_LOADER1_PITCH;
		break;
	case AXIS_LOADER2:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_LOADER2_PITCH;
		break;
	case AXIS_LOADER3:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_LOADER3_PITCH;
		break;
	case AXIS_UNLOADER1:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_UNLOADER1_PITCH;
		break;
	case AXIS_UNLOADER2:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_UNLOADER2_PITCH;
		break;
	case AXIS_UNLOADER3:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_UNLOADER3_PITCH;
		break;
	case AXIS_NGBUFFER1:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_NGBUFFER1_PITCH;
		break;
	case AXIS_NGBUFFER2:
		dValue = ActualPos / (double)MOTOR_RESOLITION * AXIS_NGBUFFER2_PITCH;
		break;
	}
	return dValue;
}

//�̵� �ӵ� ���
double CEziServo_PR::GetVelocity(int Velocity, int nAxis)
{
	long lValue = 0;

	switch(nAxis)
	{
	case AXIS_LOADER1:
		lValue = (long)(MOTOR_RESOLITION / AXIS_LOADER1_PITCH * Velocity);
		break;
	case AXIS_LOADER2:
		lValue = (long)(MOTOR_RESOLITION / AXIS_LOADER2_PITCH * Velocity);
		break;
	case AXIS_LOADER3:
		lValue = (long)(MOTOR_RESOLITION / AXIS_LOADER3_PITCH * Velocity);
		break;
	case AXIS_UNLOADER1:
		lValue = (long)(MOTOR_RESOLITION / AXIS_UNLOADER1_PITCH * Velocity);
		break;
	case AXIS_UNLOADER2:
		lValue = (long)(MOTOR_RESOLITION / AXIS_UNLOADER2_PITCH * Velocity);
		break;
	case AXIS_UNLOADER3:
		lValue = (long)(MOTOR_RESOLITION / AXIS_UNLOADER3_PITCH * Velocity);
		break;
	case AXIS_NGBUFFER1:
		lValue = (long)(MOTOR_RESOLITION / AXIS_NGBUFFER1_PITCH * Velocity);
		break;
	case AXIS_NGBUFFER2:
		lValue = (long)(MOTOR_RESOLITION / AXIS_NGBUFFER2_PITCH * Velocity);
		break;
	}
	return lValue;
}

double CEziServo_PR::GetActVelocity_Disp(long lVelocity, int nAxis)
{
	double dValue = 0;
	switch(nAxis)
	{
	case AXIS_LOADER1:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_LOADER1_PITCH;
		break;
	case AXIS_LOADER2:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_LOADER2_PITCH;
		break;
	case AXIS_LOADER3:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_LOADER3_PITCH;
		break;
	case AXIS_UNLOADER1:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER1_PITCH;
		break;
	case AXIS_UNLOADER2:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER2_PITCH;
		break;
	case AXIS_UNLOADER3:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER3_PITCH;
		break;
	case AXIS_NGBUFFER1:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_NGBUFFER1_PITCH;
		break;
	case AXIS_NGBUFFER2:
		dValue = (double)lVelocity / (double)MOTOR_RESOLITION * (double)AXIS_NGBUFFER2_PITCH;
		break;
	}	
	return dValue;
}

double CEziServo_PR::GetActPosition_Disp(long lPosition, int nAxis)
{
	double dValue = 0;
	switch(nAxis)
	{
	case AXIS_LOADER1:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_LOADER1_PITCH;
		break;
	case AXIS_LOADER2:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_LOADER2_PITCH;
		break;
	case AXIS_LOADER3:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_LOADER3_PITCH;
		break;
	case AXIS_UNLOADER1:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER1_PITCH;
		break;
	case AXIS_UNLOADER2:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER2_PITCH;
		break;
	case AXIS_UNLOADER3:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_UNLOADER3_PITCH;
		break;
	case AXIS_NGBUFFER1:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_NGBUFFER1_PITCH;
		break;
	case AXIS_NGBUFFER2:
		dValue = (double)lPosition / (double)MOTOR_RESOLITION * (double)AXIS_NGBUFFER2_PITCH;
		break;
	}	
	return dValue;
}

void CEziServo_PR::InitVariables()
{	
	m_bThreadRun = FALSE;
	m_pEziServoThread = NULL;

	m_bConnected = false;
	m_nPortNo = 0;
	m_dwBaudrate = 0;
	m_nSlaveNo = 0;
	m_nSlaveCount = 0;
	
	m_nServoEnableFlag = 0;
	m_nORGFlag = 0;

	//Robot Status	
	m_bJogging = false;
	m_bStopping = false;
	m_bRepeating = false;
		

	for(int i = 0; i < MAX_SLAVE_NUMS; i++)
	{
		m_pSalveNumber[i] = 0;
		m_bSalveEnable[i] = false;
	}
}

void CEziServo_PR::InitRobotVariables()
{	
	//Robot ���� �� ����
	m_dwIOInputValue	= new DWORD[MOTION_AXIS];
	m_dwIOOutputValue	= new DWORD[MOTION_AXIS];
	m_dwAxisStatus		= new DWORD[MOTION_AXIS];
	wPosItemNo			= new WORD[MOTION_AXIS];
	m_nCommandPos		= new long[MOTION_AXIS];
	m_nActualPos		= new long[MOTION_AXIS];
	m_nActualVel		= new long[MOTION_AXIS];
	m_nPosError			= new long[MOTION_AXIS];

	//Motion ���� ���� ����
	m_bHoming				= new bool[MOTION_AXIS];
	m_bHomeOK				= new bool[MOTION_AXIS];
	m_bServoStatus			= new bool[MOTION_AXIS];	
	m_bInposStatus			= new bool[MOTION_AXIS];
	m_bMotioning			= new bool[MOTION_AXIS];
	m_bHWPlusLimitStatus	= new bool[MOTION_AXIS];
	m_bHWMinusLimitStatus	= new bool[MOTION_AXIS];
	m_bSWPlusLimitStatus	= new bool[MOTION_AXIS];
	m_bSWMinusLimitStatus	= new bool[MOTION_AXIS];

	//Alarm ���� ����
	m_bAlarmStatus			= new bool[MOTION_AXIS];
	m_bOverCurrentError		= new bool[MOTION_AXIS];
	m_bOverSpeedError		= new bool[MOTION_AXIS];
	m_bPostTrackingError	= new bool[MOTION_AXIS];
	m_bOverheatError		= new bool[MOTION_AXIS];
	m_bOverloadError		= new bool[MOTION_AXIS];
	m_bBackEMFError			= new bool[MOTION_AXIS];
	m_bMotorPowerError		= new bool[MOTION_AXIS];
	m_bInpositionError		= new bool[MOTION_AXIS];
	m_bPosOverflowError		= new bool[MOTION_AXIS];
	m_bSlowStopError		= new bool[MOTION_AXIS];
	


	//�ʱ�ȭ
	for(int i = 0; i < int(MOTION_AXIS); i++)
	{
		//Robot ���� �� ����
		m_dwIOInputValue[i]		= 0x00;
		m_dwIOOutputValue[i]	= 0x00;
		m_dwAxisStatus[i]		= 0x00;
		wPosItemNo[i]			= 0x00;
		m_nCommandPos[i]		= 0;
		m_nActualPos[i]			= 0;
		m_nActualVel[i]			= 0;
		m_nPosError[i]			= 0;

		//Motion ���� ���� ����
		m_bHoming[i]				= false;
		m_bHomeOK[i]				= false;
		m_bServoStatus[i]			= false;		
		m_bInposStatus[i]			= false;
		m_bMotioning[i]				= false;
		m_bHWPlusLimitStatus[i]		= false;
		m_bHWMinusLimitStatus[i]	= false;
		m_bSWPlusLimitStatus[i]		= false;
		m_bSWMinusLimitStatus[i]	= false;

		//Alarm ���� ����
		m_bAlarmStatus[i]			= false;
		m_bOverCurrentError[i]		= false;
		m_bOverSpeedError[i]		= false;
		m_bPostTrackingError[i]		= false;
		m_bOverheatError[i]			= false;
		m_bOverloadError[i]			= false;
		m_bBackEMFError[i]			= false;
		m_bMotorPowerError[i]		= false;
		m_bInpositionError[i]		= false;
		m_bPosOverflowError[i]		= false;
		m_bSlowStopError[i]			= false;
	}
}


bool CEziServo_PR::Connect_EziServo(BYTE PortNo, DWORD Baudrate)
{
	if(m_bConnected) return true;
		
	m_nSlaveCount = 0;

#ifdef EZI_SERVO_USE
	if(!FAS_Connect(PortNo, Baudrate))
	{
		InitRobotVariables();
		lastError.Format(L"Connection failed.");
		return false;
	}
	else
	{
		for(int i = 0; i < MAX_SLAVE_NUMS; i++)
		{
			//���� �Ǿ� �ִ� Slave Board ���� Ȯ��
			if(FAS_IsSlaveExist(PortNo, i))
			{
				m_pSalveNumber[i] = i;
				m_bSalveEnable[i] = true;
				m_nSlaveCount++;
			}
		}

#ifdef EZI_SERVO_ALL_AXIS
		if(m_nSlaveCount == MOTION_AXIS)
		{			
			m_bConnected = true;
		}
		//����Ǿ� �ִ� Slave Board �� ���� ���
		else
		{
			lastError.Format(_T("The slave board is not connected. (Total Axis : %d )"), MOTION_AXIS);				
			m_bConnected = false;				
			return false;
		}
#endif

		m_bConnected = true;

		m_nPortNo = PortNo;
		m_dwBaudrate = Baudrate;

	}
#else
	m_nSlaveCount = (int)MOTION_AXIS;
	m_bConnected = true;
#endif
	
	InitRobotVariables();

	m_bThreadRun = TRUE;
	m_pEziServoThread = AfxBeginThread(Thread_EziServoRead, this);

    return true;
}

UINT CEziServo_PR::Thread_EziServoRead(LPVOID lpVoid)
{
	CEziServo_PR *pEziServo = (CEziServo_PR*)lpVoid;
	CCommon *pCommon = (CCommon*)lpVoid;

	int nErrorCount = 0;
	
	while(pEziServo->m_bThreadRun) 
	{
		if(pEziServo->GetAllStatus() == false)
		{
			pCommon->SetLogFile(pEziServo->lastError + _T("\n"));

			nErrorCount++;

			if(nErrorCount > 3)
			{
				pEziServo->Disconnect_EziServo();
			}
		}
		Sleep(60);

		nErrorCount = 0;
	}
	pEziServo->m_bThreadRun = FALSE;
	pEziServo->m_pEziServoThread = NULL;
	return 0;
}

void CEziServo_PR::Disconnect_EziServo()
{
	if(m_bConnected)
	{
		if(m_pEziServoThread) 
		{
			m_bThreadRun = FALSE;
			WaitForSingleObject(m_pEziServoThread->m_hThread, INFINITE);
		}

#ifdef EZI_SERVO_USE
		FAS_Close(m_nPortNo);
#endif

		lastError.Format(L"Disconnection successed.");
		
		//Robot ���� �� ����
		if(m_dwIOInputValue)	delete []m_dwIOInputValue;
		if(m_dwIOOutputValue)	delete []m_dwIOOutputValue;
		if(m_dwAxisStatus)		delete []m_dwAxisStatus;
		if(wPosItemNo)			delete []wPosItemNo;
		if(m_nCommandPos)		delete []m_nCommandPos;
		if(m_nActualPos)		delete []m_nActualPos;
		if(m_nActualVel)		delete []m_nActualVel;
		if(m_nPosError)			delete []m_nPosError;


		//Motion ���� ���� ����
		if(m_bHoming)				delete []m_bHoming;
		if(m_bHomeOK)				delete []m_bHomeOK;
		if(m_bServoStatus)			delete []m_bServoStatus;
		if(m_bInposStatus)			delete []m_bInposStatus;
		if(m_bMotioning)			delete []m_bMotioning;
		if(m_bHWPlusLimitStatus)	delete []m_bHWPlusLimitStatus;
		if(m_bHWMinusLimitStatus)	delete []m_bHWMinusLimitStatus;
		if(m_bSWPlusLimitStatus)	delete []m_bSWPlusLimitStatus;
		if(m_bSWMinusLimitStatus)	delete []m_bSWMinusLimitStatus;

		if(m_bAlarmStatus)			delete []m_bAlarmStatus;
		if(m_bOverCurrentError)		delete []m_bOverCurrentError;
		if(m_bOverSpeedError)		delete []m_bOverSpeedError;
		if(m_bPostTrackingError)	delete []m_bPostTrackingError;
		if(m_bOverloadError)		delete []m_bOverloadError;
		if(m_bOverheatError)		delete []m_bOverheatError;
		if(m_bBackEMFError)			delete []m_bBackEMFError;
		if(m_bMotorPowerError)		delete []m_bMotorPowerError;
		if(m_bInpositionError)		delete []m_bInpositionError;
		if(m_bPosOverflowError)		delete []m_bPosOverflowError;
		if(m_bSlowStopError)		delete []m_bSlowStopError;

		m_bConnected = false;
	}
}


void CEziServo_PR::Error_Status_Display(int Return)
{
	switch(Return)
	{
		//�߸��� Port ��ȣ�� �Է� �Ͽ����ϴ�.
		case FMM_NOT_OPEN:
			lastError.Format(L"[FMM_NOT_OPEN] Port number was entered wrong.");
			break;

		//������� ���� Port ��ȣ �Դϴ�.
		case FMM_INVALID_PORT_NUM:
			lastError.Format(L"[FMM_INVALID_PORT_NUM] Port number is not connected.");
			break;

		//�߸��� Slave ��ȣ�� �Է��Ͽ����ϴ�.
		case FMM_INVALID_SLAVE_NUM:
			lastError.Format(L"[FMM_INVALID_SLAVE_NUM] Slave number was entered wrong.");
			break;

        //�ش� Board �� ���� ���� �Ǿ����ϴ�.
		case FMC_DISCONNECTED:
			lastError.Format(L"[FMC_DISCONNECTED] The board was disconnected.");
			break;

		//������ �ð�(100msec) ���� ������ �����ϴ�.
		case FMC_TIMEOUT_ERROR:
			lastError.Format(L"[FMC_TIMEOUT_ERROR] Communication error [TIME OUT].");
			break;

		//��� �� Checksum ������ �߻��Ͽ����ϴ�.
		case FMC_CRCFAILED_ERROR:
			lastError.Format(L"[FMC_CRCFAILED_ERROR] Communication error [Check SUM]");
			break;

		//����̺�κ��� ���� ��Ŷ���� �������� ������ ������ �߻��Ͽ����ϴ�.
		case FMC_RECVPACKET_ERROR:
			lastError.Format(L"[FMC_RECVPACKET_ERROR] Communication error.");
			break;

		//���������̺� �б�/������ ������ �߻� �Ͽ����ϴ�.
		case FMM_POSTABLE_ERROR:
			lastError.Format(L"[FMM_POSTABLE_ERROR] Command execution failure.");
			break;

		//Board�� �ν����� ���ϴ� ��ɾ� �Դϴ�.
		case FMP_FRAMETYPEERROR:
			lastError.Format(L"[FMP_FRAMETYPEERROR] Wrong command.");
			break;

		//�Է��� �����Ͱ� ������ ������ϴ�.
		case FMP_PACKETERROR:
			lastError.Format(L"[FMP_PACKETERROR] Outside the range of the data entered has occurred.");
			break;

		//���Ͱ� �̹� �������̰ų� ������ �� �ִ� �غ� �Ǿ� ���� �ʴµ� �߸��� ����Դϴ�.
		case FMP_RUNFAIL:
			lastError.Format(L"[FMP_RUNFAIL] Wrong command.");
			break;

		//Servo On ���¿��� Alarm Reset ����� ���� �� �� �����ϴ�.
		case FMP_RESETFAIL:
			lastError.Format(L"[FMP_RESETFAIL] Alarm reset command is not executed that servo on status.");
			break;

		//Alarm�� �߻��� ���� �Դϴ�.
		case FMP_SERVOONFAIL1:
			lastError.Format(L"[FMP_SERVOONFAIL1] Error status.");
			break;

		//Emergency Stop ���Դϴ�.
		case FMP_SERVOONFAIL2:
			lastError.Format(L"[FMP_SERVOONFAIL2] Emergency stopping.");
			break;

		//�Է� ��ȣ�� Servo On�� �̹� ���� �Ǿ����ϴ�.
		case FMP_SERVOONFAIL3:
			lastError.Format(L"[FMP_SERVOONFAIL3] Servo On the input signal has already been set.");
			break;

		//����̺갡 ���� ��Ŷ���� �������� ������ ������ �߻��Ͽ����ϴ�.
		case FMP_PACKETCRCERROR:
			lastError.Format(L"[FMP_PACKETCRCERROR] Communication error.");
			break;

		//����̺갡 ���� ��Ŷ�� CRC ��갪�� ��ġ���� �ʽ��ϴ�.
		case FMM_UNKNOWN_ERROR:
			lastError.Format(L"[FMM_UNKNOWN_ERROR] Unknown error.");
			break;

		default:
			break;

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////
//Servo ����

bool CEziServo_PR::Servo_Enable_Init()
{
	int nReturn = 0;	

	if(m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if(!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		

		//Servo On ���°� �ƴϸ�
		if(!m_bServoStatus[m_nSlaveNo])
		{
#ifdef EZI_SERVO_USE
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, TRUE);
#endif
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	m_nServoEnableFlag = 0;

	return true;
}

bool CEziServo_PR::Servo_Off()
{
	int nReturn = 0;	

	if(m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if(!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		

		//Servo On ���� �� ��쿡��
		if(m_bServoStatus[m_nSlaveNo])
		{
#ifdef EZI_SERVO_USE
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, FALSE);
#endif
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}
	m_nServoEnableFlag = 0;

	return true;
}

bool CEziServo_PR::Servo_Enable()
{
	int nReturn = 0;
	

	if(m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if(!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		

		//Servo On ���°� �ƴϸ�
		if(!m_bServoStatus[m_nSlaveNo])
		{
#ifdef EZI_SERVO_USE
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, TRUE);
#endif
		}
		else
		{
#ifdef EZI_SERVO_USE
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, FALSE);
#endif
			m_bHomeOK[m_nSlaveNo] = false;							//Servo ���� �� ���� OK Flag �ʱ�ȭ(��Ȯ�� ��ġ ������)
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	m_nServoEnableFlag = 0;

	return true;
}

bool CEziServo_PR::GetAllStatus()
{
	int nReturn = 0;	
	BYTE nAlarmType = 0;
	EZISERVO_AXISSTATUS AxisStatus;
	int nIndex = 0, nSlaveNo = 0;
	

	/////////////////////////////////////////////////////////////////////////////////////////

#ifdef EZI_SERVO_USE
	//*************************************************************************************
	//���� Robot ��� ���� �� �о� ����
	for(int nIndex = 0; nIndex < (int)MOTION_AXIS; nIndex++)
	{
		if(m_bSalveEnable[nIndex] == false)
		{
			continue;
		}

		nSlaveNo = m_pSalveNumber[nIndex];

		nReturn = FAS_GetAllStatus(m_nPortNo, nSlaveNo, &m_dwIOInputValue[nIndex], &m_dwIOOutputValue[nIndex], &m_dwAxisStatus[nIndex], &m_nCommandPos[nIndex], &m_nActualPos[nIndex], &m_nPosError[nIndex], &m_nActualVel[nIndex], &wPosItemNo[nIndex]);

		if(nReturn == FMM_OK)
		{
			AxisStatus.dwValue = GetAxisStatus(nIndex);			

			//*************************************************************************************
			//Motion
			if(AxisStatus.FFLAG_MOTIONING || AxisStatus.FFLAG_MOTIONACCEL || AxisStatus.FFLAG_MOTIONDECEL || AxisStatus.FFLAG_MOTIONCONST)
			{
				if(IsMotioning(nIndex) == false)
				{
					m_bMotioning[nIndex] = true;
				}
			}
			else
			{
				//if(IsMotioning(nIndex))
				//{
					m_bMotioning[nIndex] = false;
				//}
			}
			

			//*************************************************************************************
			//Origin
			if(AxisStatus.FFLAG_ORIGINRETURNING)
			{
				if(IsHoming(nIndex) == false)
				{
					m_bHoming[nIndex] = true;
				}
			}
			else
			{
				//if(IsHoming(nIndex))
				//{
					m_bHoming[nIndex] = false;
				//}
			}



			//*************************************************************************************
			//Servo On/Off ����
			if(AxisStatus.FFLAG_SERVOON)
			{
				if(IsServoStatus(nIndex) == false)
				{
					m_bServoStatus[nIndex] = true;
				}
			}
			else
			{
				//if(IsServoStatus(nIndex))
				//{
					m_bServoStatus[nIndex] = false;
				//}
			}


			////*************************************************************************************
			////Inposition
			//if(AxisStatus.FFLAG_INPOSITION)
			//{
			//	if(IsInposStatus(nIndex) == false)
			//	{
			//		m_bInposStatus[nIndex] = true;
			//	}
			//}
			//else
			//{
			//	if(IsInposStatus(nIndex))
			//	{
			//		m_bInposStatus[nIndex] = false;
			//	}
			//}


			////*************************************************************************************
			////H//W + Limit
			//if(AxisStatus.FFLAG_HWPOSILMT)
			//{
			//	if(IsHWPlusLimitStatus(nIndex) == false)
			//	{
			//		m_bHWPlusLimitStatus[nIndex] = true;
			//	}
			//}
			//else
			//{
			//	if(IsHWPlusLimitStatus(nIndex))
			//	{
			//		m_bHWPlusLimitStatus[nIndex] = false;
			//	}
			//}


			////*************************************************************************************
			////H//W - Limit
			//if(AxisStatus.FFLAG_HWNEGALMT)
			//{
			//	if(IsHWMinusLimitStatus(nIndex) == false)
			//	{
			//		m_bHWMinusLimitStatus[nIndex] = true;
			//	}
			//}
			//else
			//{
			//	if(IsHWMinusLimitStatus(nIndex))
			//	{
			//		m_bHWMinusLimitStatus[nIndex] = false;
			//	}
			//}


			////*************************************************************************************
			////S//W + Limit
			//if(AxisStatus.FFLAG_SWPOGILMT)
			//{
			//	if(IsSWPlusLimitStatus(nIndex) == false)
			//	{
			//		m_bSWPlusLimitStatus[nIndex] = true;
			//	}
			//}
			//else
			//{
			//	if(IsSWPlusLimitStatus(nIndex))
			//	{
			//		m_bSWPlusLimitStatus[nIndex] = false;
			//	}
			//}


			////*************************************************************************************
			////S//W - Limit
			//if(AxisStatus.FFLAG_SWNEGALMT)
			//{
			//	if(IsSWMinusLimitStatus(nIndex) == false)
			//	{
			//		m_bSWMinusLimitStatus[nIndex] = true;
			//	}
			//}
			//else
			//{
			//	if(IsSWMinusLimitStatus(nIndex))
			//	{
			//		m_bSWMinusLimitStatus[nIndex] = false;
			//	}
			//}


			//*************************************************************************************
			//������ �̻�
			if(AxisStatus.FFLAG_ERROVERCURRENT)
			{
				if(IsOverCurrentError(nIndex) == false)
				{
					m_bOverCurrentError[nIndex] = true;
				}
			}
			else
			{
				if(IsOverCurrentError(nIndex))
				{
					m_bOverCurrentError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���ӵ� �̻�
			if(AxisStatus.FFLAG_ERROVERSPEED)
			{
				if(IsOverSpeedError(nIndex) == false)
				{
					m_bOverSpeedError[nIndex] = true;
				}
			}
			else
			{
				if(IsOverSpeedError(nIndex))
				{
					m_bOverSpeedError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//��ġ�����̻�
			if(AxisStatus.FFLAG_ERRPOSTRACKING)
			{
				if(IsPostTrackingError(nIndex) == false)
				{
					m_bPostTrackingError[nIndex] = true;
				}
			}
			else
			{
				if(IsPostTrackingError(nIndex))
				{
					m_bPostTrackingError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//������ �̻�
			if(AxisStatus.FFLAG_ERROVERLOAD)
			{
				if(IsOverloadError(nIndex) == false)
				{
					m_bOverloadError[nIndex] = true;
				}
			}
			else
			{
				if(IsOverloadError(nIndex))
				{
					m_bOverloadError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���� �̻�
			if(AxisStatus.FFLAG_ERROVERHEAT)
			{
				if(IsOverheatError(nIndex) == false)
				{
					m_bOverheatError[nIndex] = true;
				}
			}
			else
			{
				if(IsOverheatError(nIndex))
				{
					m_bOverheatError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//ȸ������ �̻�
			if(AxisStatus.FFLAG_ERRBACKEMF)
			{
				if(IsBackEMFError(nIndex) == false)
				{
					m_bBackEMFError[nIndex] = true;
				}
			}
			else
			{
				if(IsBackEMFError(nIndex))
				{
					m_bBackEMFError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���� ���� �̻�
			if(AxisStatus.FFLAG_ERRMOTORPOWER)
			{
				if(IsMotorPowerError(nIndex) == false)
				{
					m_bMotorPowerError[nIndex] = true;
				}
			}
			else
			{
				if(IsMotorPowerError(nIndex))
				{
					m_bMotorPowerError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//�������� �̻�
			if(AxisStatus.FFLAG_ERRINPOSITION)
			{
				if(IsInpositionError(nIndex) == false)
				{
					m_bInpositionError[nIndex] = true;
				}
			}
			else
			{
				if(IsInpositionError(nIndex))
				{
					m_bInpositionError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//��ġ �ʰ����� �̻�
			if(AxisStatus.FFLAG_ERRPOSOVERFLOW)
			{
				if(IsPosOverflowError(nIndex) == false)
				{
					m_bPosOverflowError[nIndex] = true;
				}
			}
			else
			{
				if(IsPosOverflowError(nIndex))
				{
					m_bPosOverflowError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//Error ����
			if(AxisStatus.FFLAG_ERRORALL)
			{
				if(IsAlarmStatus(nIndex) == false)
				{
					m_bAlarmStatus[nIndex] = true;
				}
			}
			else
			{
				if(IsAlarmStatus(nIndex))
				{
					m_bAlarmStatus[nIndex] = false;
				}
			}

			//*************************************************************************************
			if(AxisStatus.FFLAG_SLOWSTOP)
			{
				if(IsSlowStopError(nIndex) == false)
				{
					m_bSlowStopError[nIndex] = true;
				}
			}
			else
			{
				if(IsSlowStopError(nIndex))
				{
					m_bSlowStopError[nIndex] = false;
				}
			}
		}
		//Error ���� ���
		else
		{
			//TRACE("GetAllStatus Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}

	}
#endif

	return true;
}


bool CEziServo_PR::Robot_Status()
{
	int nReturn = 0;	
	BYTE nAlarmType = 0;
	
	/////////////////////////////////////////////////////////////////////////////////////////
		
	//Error ����
	if(IsAlarmStatus(m_nSlaveNo))
	{
#ifdef EZI_SERVO_USE
		//���� �˶��� ���� Ȯ��
		if(FAS_GetAlarmType(m_nPortNo, m_nSlaveNo, &nAlarmType) == FMM_OK)
		{
			//TRACE("Robot Status Error!    \n");
			Error_Status_Display(nAlarmType);
		}
		else
		{
			lastError.Format(L"Problem has been encountered (Alarm)");
		}
#endif
		return false;
	}


	//*************************************************************************************
	//Servo On ���°� �ƴѰ�� && Servo On/Off ������ �ƴ� ���
	if(m_nServoEnableFlag != 1)
	{
		if(!IsServoStatus(m_nSlaveNo))
		{
			lastError.Format(L"Use after the servo on.");
			return false;
		}
	}



	//*************************************************************************************
	//���� �Ϸ� ���°� �ƴ� ���
	//if(!m_nORGFlag && m_nServoEnableFlag == 0)
	//{
	//	if(!IsHomeOK(m_nSlaveNo))
	//	{
	//		lastError.Format(L"Use after origin operation on.");
	//		return false;
	//	}
	//}


	return true;
}


bool CEziServo_PR::Robot_Status_All_Axis()
{
	int nReturn = 0;	
	BYTE nAlarmType = 0;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	
#ifdef EZI_SERVO_USE
	//Error ����
	for(int i = 0; i < int(MOTION_AXIS); i++)
	{
		if(m_bSalveEnable[i] == false)
		{
			continue;
		}

		if(IsAlarmStatus(i))
		{
			//���� �˶��� ���� Ȯ��
			if(FAS_GetAlarmType(m_nPortNo, m_nSlaveNo, &nAlarmType) == FMM_OK)
			{
				//TRACE("Robot_Status_All_Axis Error!    \n");
				Error_Status_Display(nAlarmType);
			}
			else
			{
				lastError.Format(L"Problem has been encountered (Alarm_%d)", i);
			}

			return false;
		}
	}


	//*************************************************************************************
	//Servo On ���°� �ƴѰ�� && Servo On/Off ������ �ƴ� ���
	for(int i = 0; i < int(MOTION_AXIS); i++)
	{
		if(m_bSalveEnable[i] == false)
		{
			continue;
		}

		if(m_nServoEnableFlag != 1)
		{
			if(!IsServoStatus(i))
			{
				lastError.Format(L"Use after the servo on. (%d)", i);
				return false;
			}
		}
	}



	//*************************************************************************************
	//���� �Ϸ� ���°� �ƴ� ���
	//if(!m_nORGFlag && m_nServoEnableFlag != 1)
	//{
	//	for(int i = 0; i < m_nSlaveCount; i++)
	//	{
	//		if(!IsHomeOK(i))
	//		{
	//			lastError.Format(L"Use after origin operation on. (%d)  \n", i);
	//			return false;
	//		}
	//	}
	//}
#endif

	return true;
}

bool CEziServo_PR::Servo_Alarm_Reset_All()
{
	int nReturn = 0;
	int nSlaveNo = 0;

#ifdef EZI_SERVO_USE
	for(int i = 0; i < int(MOTION_AXIS); i++)
	{
		if(m_bSalveEnable[i] == false)
		{
			continue;
		}

		nSlaveNo = m_pSalveNumber[i];

		if(IsAlarmStatus(i))
		{
			nReturn = FAS_ServoAlarmReset(m_nPortNo, nSlaveNo);

			//���� �˶��� ���� Ȯ��
			if(nReturn != FMM_OK)
			{				
				Error_Status_Display(nReturn);
				return false;
			}
		}
	}
#endif

	return true;
}

bool CEziServo_PR::Servo_Alarm_Reset_Single(int nslave)
{
	int nReturn = 0;
	int nSlaveNo = 0;
	
	nSlaveNo = nslave;

	if(IsAlarmStatus(nSlaveNo))
	{
#ifdef EZI_SERVO_USE
		nReturn = FAS_ServoAlarmReset(m_nPortNo, nSlaveNo);

		//���� �˶��� ���� Ȯ��
		if(nReturn != FMM_OK)
		{				
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}

	return true;
}

bool CEziServo_PR::SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue)
{
	int nReturn = 0;

	if(m_bConnected)
	{
#ifdef EZI_SERVO_USE
		nReturn = FAS_SetParameter(m_nPortNo, nSlaveNo, nParaNo, lValue);

		//������ Motor ���� ����
		if(nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}
	else
	{
		lastError.Format(L"[SET_PARAMETER] Communication is not connected.");
		return false;
	}

	return true;
}

bool CEziServo_PR::SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel)
{
	int nReturn = 0;

	if(m_bConnected)
	{
#ifdef EZI_SERVO_USE
		nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x04000000, bLevel);

		//������ Motor ���� ����
		if(nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}
	else
	{
		lastError.Format(L"[SET_IOASSIGNMAP] Communication is not connected.");
		return false;
	}

	return true;
}

bool CEziServo_PR::SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel)
{
	int nReturn = 0;

	if(m_bConnected)
	{
#ifdef EZI_SERVO_USE
		if(bLevel == 1)		//Enable
		{
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x04000000, 0);
			if(nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}			
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 4, 0x08000000, 0);
			if(nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
		}
		else    //Disable
		{
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x00000000, 0);
			if(nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 4, 0x00000000, 0);
			if(nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
		}
#endif
	}
	else
	{
		lastError.Format(L"[SET_IOASSIGNMAP_SLOW_STOP] Communication is not connected.");
		return false;
	}

	return true;
}

//1�� ���� ����
bool CEziServo_PR::MoveStop()
{
	int nReturn = 0;

	if(m_bConnected)
	{
		m_bStopping = true;	
		m_bJogging = false;

#ifdef EZI_SERVO_USE
		nReturn = FAS_MoveStop(m_nPortNo, m_nSlaveNo);

		//������ Motor ���� ����
		if(nReturn != FMM_OK)
		{
			//TRACE("MoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//1�� ���� ����
bool CEziServo_PR::MoveStop(BYTE SlaveNo)
{
	int nReturn = 0;

	if(m_bConnected)
	{
		m_bStopping = true;	
		m_bJogging = false;

#ifdef EZI_SERVO_USE
		nReturn = FAS_MoveStop(m_nPortNo, SlaveNo);

		//������ Motor ���� ����
		if(nReturn != FMM_OK)
		{
			//TRACE("MoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(50);
#endif
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//���� ���� ����
bool CEziServo_PR::AllMoveStop()
{
	int nReturn = 0;

	if(m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

#ifdef EZI_SERVO_USE
		nReturn = FAS_AllMoveStop(m_nPortNo);

		//���� ���� ����
		if(nReturn != FMM_OK)
		{
			//TRACE("AllMoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//EMG Stop
bool CEziServo_PR::AllEmergencyStop()
{
	int nReturn = 0;

	if(m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

#ifdef EZI_SERVO_USE
		nReturn = FAS_AllEmergencyStop(m_nPortNo);

		//���� ���� ��� ����
		if(nReturn != FMM_OK)
		{
			//TRACE("AllEmergencyStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
#endif
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//Single EmergencyStop
bool CEziServo_PR::SingleEmergencyStop(BYTE SlaveNo)
{
	int nReturn = 0;

	if(m_bConnected)
	{	
		//TRACE("SingleEmergencyStop!    \n");

#ifdef EZI_SERVO_USE
		nReturn = FAS_EmergencyStop(m_nPortNo, SlaveNo);

		//���� ���� ��� ����
		if(nReturn != FMM_OK)
		{
			//TRACE("SingleEmergencyStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
#endif

		m_bJogging = false;
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

bool CEziServo_PR::SetResetPosition(BYTE SlaveNo)
{
	int nReturn = 0;

	if(m_bConnected)
	{
#ifdef EZI_SERVO_USE		
		nReturn = FAS_ClearPosition(m_nPortNo, SlaveNo);
		if(nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(20);
#endif
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//Jog ����
bool CEziServo_PR::MoveVelocity(DWORD lVelocity, int nDir, int nAxis)
{	
	CCommon *pCommon = CCommon::Get_Instance();
	int nReturn = 0;

	if(m_bConnected)
	{
		m_nSlaveNo = nAxis;

		if(IsMotioning(nAxis) == true)
		{
			return true;
		}

		m_bStopping = false;

		//���� Robot ���� Ȯ��
		if(!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		
#ifdef EZI_SERVO_USE		
		//Jog ����
		nReturn = FAS_MoveVelocity(m_nPortNo, m_nSlaveNo, (DWORD)GetVelocity(lVelocity, m_nSlaveNo), nDir);

		if(nReturn != FMM_OK)
		{
			//TRACE("MoveVelocity Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(30);

#endif
		
		m_bJogging = true;
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}
	return true;
}

bool CEziServo_PR::SingleAxisAbsMove(DWORD lVelocity, int nAxis)
{
	CCommon *pCommon = CCommon::Get_Instance();
	int nReturn = 0;
	long lActPos = 0;

	if(m_bConnected)
	{
		m_nSlaveNo = nAxis;

		if(IsMotioning(nAxis) == true)
		{
			return true;
		}

		m_bStopping = false;

		//���� Robot ���� Ȯ��
		if(!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		
#ifdef EZI_SERVO_USE
		nReturn = FAS_GetActualPos(m_nPortNo, m_nSlaveNo, &lActPos);
		if(nReturn != FMM_OK)
		{
			//TRACE("FAS_GetActualPos Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}

		//Jog ����
		if(lActPos < 0)
		{
			nReturn = FAS_MoveSingleAxisAbsPos(m_nPortNo, m_nSlaveNo, lActPos - 150, (DWORD)GetVelocity(lVelocity, m_nSlaveNo));
		}
		else
		{
			nReturn = FAS_MoveSingleAxisAbsPos(m_nPortNo, m_nSlaveNo, lActPos + 150, (DWORD)GetVelocity(lVelocity, m_nSlaveNo));
		}
		if(nReturn != FMM_OK)
		{
			//TRACE("FAS_MoveSingleAxisIncPos Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(20);

#endif		
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}


	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
