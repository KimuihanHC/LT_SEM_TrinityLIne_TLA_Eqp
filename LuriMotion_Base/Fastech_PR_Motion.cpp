// NI6509.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "Fastech_PR_Motion.h"
#include <mutex>


// CEziServo_PR
IMPLEMENT_DYNAMIC(CFastech_Motion, CWnd)
CFastech_Motion::CFastech_Motion()
{
	InitVariables();

	InitializeCriticalSection(&m_cs);
}

CFastech_Motion::~CFastech_Motion()
{
	Disconnect_EziServo();

	DeleteCriticalSection(&m_cs);
}

BEGIN_MESSAGE_MAP(CFastech_Motion, CWnd)
	//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CEziServo_PR �޽��� ó�����Դϴ�.

BOOL CFastech_Motion::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // ȭ�� ���� ����
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

//������ �Ÿ���ŭ�� Pulse �� ���
long CFastech_Motion::GetPulse(double CommandPos, int nAxis)
{
	long lCalPulse = 0;

	lCalPulse = (long)((10000 / 20) * CommandPos);

	return lCalPulse;
}

//���� ������ �� mm�� ȯ��
double CFastech_Motion::GetActualmm(long ActualPos, int nAxis)
{
	double dValue = 0;

	dValue = ActualPos / (double)10000 * 20;
	return dValue;
}

//�̵� �ӵ� ���
double CFastech_Motion::GetVelocity(int Velocity, int nAxis)
{
	long lValue = 0;

	lValue = (long)((10000 / 20) * Velocity);
	return lValue;
}

double CFastech_Motion::GetActVelocity_Disp(long lVelocity, int nAxis)
{
	double dValue = 0;
	dValue = (double)lVelocity / (double)10000 * (double)88;
	return dValue;
}

double CFastech_Motion::GetActPosition_Disp(long lPosition, int nAxis)
{
	double dValue = 0;
	dValue = (double)lPosition / (double)10000 * (double)20;
	return dValue;
}

void CFastech_Motion::InitVariables()
{
	m_bThreadRun = FALSE;
	m_pEziServoThread = NULL;

	m_bConnected = false;
	m_nPortNo = 0;
	m_dwBaudrate = 0;
	m_nSlaveNo = 0;
	m_nSlaveCount = 0;
	m_bConnectedInitFlag = 0;
	m_nServoEnableFlag = 0;
	m_nORGFlag = 0;

	//Robot Status	
	m_bJogging = false;
	m_bStopping = false;
	m_bRepeating = false;


	for (int i = 0; i < MAX_SLAVE_NUMS; i++)
	{
		m_pSalveNumber[i] = 0;
		m_bSalveEnable[i] = false;
	}
}

void CFastech_Motion::InitRobotVariables()
{
	//Robot ���� �� ����
	m_dwIOInputValue = new DWORD[MOTOR_MAX_COUNT];
	m_dwIOOutputValue = new DWORD[MOTOR_MAX_COUNT];
	m_dwAxisStatus = new DWORD[MOTOR_MAX_COUNT];
	wPosItemNo = new WORD[MOTOR_MAX_COUNT];
	m_nCommandPos = new long[MOTOR_MAX_COUNT];
	m_nActualPos = new long[MOTOR_MAX_COUNT];
	m_nActualVel = new long[MOTOR_MAX_COUNT];
	m_nPosError = new long[MOTOR_MAX_COUNT];

	//Motion ���� ���� ����
	m_bHoming = new bool[MOTOR_MAX_COUNT];
	m_bHomeOK = new bool[MOTOR_MAX_COUNT];
	m_bServoStatus = new bool[MOTOR_MAX_COUNT];
	m_bInposStatus = new bool[MOTOR_MAX_COUNT];
	m_bMotioning = new bool[MOTOR_MAX_COUNT];
	m_bHWPlusLimitStatus = new bool[MOTOR_MAX_COUNT];
	m_bHWMinusLimitStatus = new bool[MOTOR_MAX_COUNT];
	m_bHWHomeStatus = new bool[MOTOR_MAX_COUNT];
	m_bSWPlusLimitStatus = new bool[MOTOR_MAX_COUNT];
	m_bSWMinusLimitStatus = new bool[MOTOR_MAX_COUNT];

	//Alarm ���� ����
	m_bAlarmStatus = new bool[MOTOR_MAX_COUNT];
	m_bOverCurrentError = new bool[MOTOR_MAX_COUNT];
	m_bOverSpeedError = new bool[MOTOR_MAX_COUNT];
	m_bPostTrackingError = new bool[MOTOR_MAX_COUNT];
	m_bOverheatError = new bool[MOTOR_MAX_COUNT];
	m_bOverloadError = new bool[MOTOR_MAX_COUNT];
	m_bBackEMFError = new bool[MOTOR_MAX_COUNT];
	m_bMotorPowerError = new bool[MOTOR_MAX_COUNT];
	m_bInpositionError = new bool[MOTOR_MAX_COUNT];
	m_bPosOverflowError = new bool[MOTOR_MAX_COUNT];
	m_bSlowStopError = new bool[MOTOR_MAX_COUNT];



	//�ʱ�ȭ
	for (int i = 0; i < int(MOTOR_MAX_COUNT); i++)
	{
		//Robot ���� �� ����
		m_dwIOInputValue[i] = 0x00;
		m_dwIOOutputValue[i] = 0x00;
		m_dwAxisStatus[i] = 0x00;
		wPosItemNo[i] = 0x00;
		m_nCommandPos[i] = 0;
		m_nActualPos[i] = 0;
		m_nActualVel[i] = 0;
		m_nPosError[i] = 0;

		//Motion ���� ���� ����
		m_bHoming[i] = false;
		m_bHomeOK[i] = false;
		m_bServoStatus[i] = false;
		m_bInposStatus[i] = false;
		m_bMotioning[i] = false;
		m_bHWPlusLimitStatus[i] = false;
		m_bHWMinusLimitStatus[i] = false;
		m_bHWHomeStatus[i] = false;
		m_bSWPlusLimitStatus[i] = false;
		m_bSWMinusLimitStatus[i] = false;

		//Alarm ���� ����
		m_bAlarmStatus[i] = false;
		m_bOverCurrentError[i] = false;
		m_bOverSpeedError[i] = false;
		m_bPostTrackingError[i] = false;
		m_bOverheatError[i] = false;
		m_bOverloadError[i] = false;
		m_bBackEMFError[i] = false;
		m_bMotorPowerError[i] = false;
		m_bInpositionError[i] = false;
		m_bPosOverflowError[i] = false;
		m_bSlowStopError[i] = false;
	}
}


bool CFastech_Motion::Connect_EziServo(BYTE PortNo, DWORD Baudrate)
{
	if (m_bConnected) return true;

	m_nSlaveCount = 0;


	if (!FAS_Connect(PortNo, Baudrate))
	{
		InitRobotVariables();
		lastError.Format(L"Connection failed.");
		return false;
	}
	else
	{
		for (int i = 0; i < MAX_SLAVE_NUMS; i++)
		{
			//���� �Ǿ� �ִ� Slave Board ���� Ȯ��
			if (FAS_IsSlaveExist(PortNo, i))
			{
				m_pSalveNumber[i] = i;
				m_bSalveEnable[i] = true;
				m_nSlaveCount++;
			}
		}

		m_bConnected = true;

		m_nPortNo = PortNo;
		m_dwBaudrate = Baudrate;

	}

	m_nSlaveCount = (int)MOTOR_MAX_COUNT;
	m_bConnected = true;


	InitRobotVariables();

	m_bThreadRun = TRUE;
	m_pEziServoThread = AfxBeginThread(Thread_EziServoRead, this);

	return true;
}

UINT CFastech_Motion::Thread_EziServoRead(LPVOID lpVoid)
{
	int nErrorCount = 0;

// 	while (FAS_MO->m_bThreadRun)
// 	{
// 		if (FAS_MO->GetAllStatus() == false)
// 		{
// 			nErrorCount++;
// 
// 			if (nErrorCount > 3)
// 			{
// 				FAS_MO->Disconnect_EziServo();
// 			}
// 		}
// 		Sleep(10);
// 
// 		nErrorCount = 0;
// 	}
// 	FAS_MO->m_bThreadRun = FALSE;
// 	FAS_MO->m_pEziServoThread = NULL;
	return 0;
}

void CFastech_Motion::Disconnect_EziServo()
{
	if (m_bConnected)
	{
		if (m_pEziServoThread)
		{
			m_bThreadRun = FALSE;
			WaitForSingleObject(m_pEziServoThread->m_hThread, INFINITE);
		}

		FAS_Close(m_nPortNo);

		lastError.Format(L"Disconnection successed.");

		//Robot ���� �� ����
		if (m_dwIOInputValue)	delete[]m_dwIOInputValue;
		if (m_dwIOOutputValue)	delete[]m_dwIOOutputValue;
		if (m_dwAxisStatus)		delete[]m_dwAxisStatus;
		if (wPosItemNo)			delete[]wPosItemNo;
		if (m_nCommandPos)		delete[]m_nCommandPos;
		if (m_nActualPos)		delete[]m_nActualPos;
		if (m_nActualVel)		delete[]m_nActualVel;
		if (m_nPosError)			delete[]m_nPosError;


		//Motion ���� ���� ����
		if (m_bHoming)				delete[]m_bHoming;
		if (m_bHomeOK)				delete[]m_bHomeOK;
		if (m_bServoStatus)			delete[]m_bServoStatus;
		if (m_bInposStatus)			delete[]m_bInposStatus;
		if (m_bMotioning)			delete[]m_bMotioning;
		if (m_bHWPlusLimitStatus)	delete[]m_bHWPlusLimitStatus;
		if (m_bHWMinusLimitStatus)	delete[]m_bHWMinusLimitStatus;
		if (m_bHWHomeStatus)		delete[]m_bHWHomeStatus;

		if (m_bSWPlusLimitStatus)	delete[]m_bSWPlusLimitStatus;
		if (m_bSWMinusLimitStatus)	delete[]m_bSWMinusLimitStatus;

		if (m_bAlarmStatus)			delete[]m_bAlarmStatus;
		if (m_bOverCurrentError)		delete[]m_bOverCurrentError;
		if (m_bOverSpeedError)		delete[]m_bOverSpeedError;
		if (m_bPostTrackingError)	delete[]m_bPostTrackingError;
		if (m_bOverloadError)		delete[]m_bOverloadError;
		if (m_bOverheatError)		delete[]m_bOverheatError;
		if (m_bBackEMFError)			delete[]m_bBackEMFError;
		if (m_bMotorPowerError)		delete[]m_bMotorPowerError;
		if (m_bInpositionError)		delete[]m_bInpositionError;
		if (m_bPosOverflowError)		delete[]m_bPosOverflowError;
		if (m_bSlowStopError)		delete[]m_bSlowStopError;

		m_bConnected = false;
	}
}


void CFastech_Motion::Error_Status_Display(int Return)
{
	switch (Return)
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
double CFastech_Motion::GetAxisCurrnetActualPos(int nIndex)
{
	long lPos = 0;

	FAS_GetActualPos(m_nPortNo, nIndex, &lPos);

	double dPos = (double)lPos / (double)500;

	return dPos;
}
bool CFastech_Motion::Servo_Enable_Init()
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}


		//Servo On ���°� �ƴϸ�
		if (!m_bServoStatus[m_nSlaveNo])
		{
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, TRUE);
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

bool CFastech_Motion::Servo_Off()
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}


		//Servo On ���� �� ��쿡��
		if (m_bServoStatus[m_nSlaveNo])
		{
			FAS_ServoEnable(m_nPortNo, m_nSlaveNo, FALSE);
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

bool CFastech_Motion::Servo_Off(BYTE SlaveNo)
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_nServoEnableFlag = 1;
		nReturn = FAS_ServoEnable(m_nPortNo, SlaveNo, FALSE);
		m_bHomeOK[SlaveNo] = false;							//Servo ���� �� ���� OK Flag �ʱ�ȭ(��Ȯ�� ��ġ ������)
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}
	m_nServoEnableFlag = 0;

	return true;
}

bool CFastech_Motion::Servo_Enable(BYTE SlaveNo)
{
	int nReturn = 0;


	if (m_bConnected)
	{
		m_nServoEnableFlag = 1;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}


		//Servo On ���°� �ƴϸ�
		if (!m_bServoStatus[SlaveNo])
		{
			FAS_ServoEnable(m_nPortNo, SlaveNo, TRUE);
		}
		else
		{
			FAS_ServoEnable(m_nPortNo, SlaveNo, FALSE);
			m_bHomeOK[SlaveNo] = false;							//Servo ���� �� ���� OK Flag �ʱ�ȭ(��Ȯ�� ��ġ ������)
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

bool CFastech_Motion::GetAllStatus()
{
	int nReturn = 0;
	BYTE nAlarmType = 0;
	EZISERVO_AXISSTATUS AxisStatus;
	int nIndex = 0, nSlaveNo = 0;


	/////////////////////////////////////////////////////////////////////////////////////////


	//*************************************************************************************
	//���� Robot ��� ���� �� �о� ����
	for (int nIndex = 0; nIndex < (int)MOTOR_MAX_COUNT; nIndex++)
	{
		if (m_bSalveEnable[nIndex] == false)
		{
			continue;
		}

		nSlaveNo = m_pSalveNumber[nIndex];

		nReturn = FAS_GetAllStatus(m_nPortNo, nSlaveNo, &m_dwIOInputValue[nIndex], &m_dwIOOutputValue[nIndex], &m_dwAxisStatus[nIndex], &m_nCommandPos[nIndex], &m_nActualPos[nIndex], &m_nPosError[nIndex], &m_nActualVel[nIndex], &wPosItemNo[nIndex]);

		if (nReturn == FMM_OK)
		{
			AxisStatus.dwValue = GetAxisStatus(nIndex);

			//*************************************************************************************
			//Motion
			if (AxisStatus.FFLAG_MOTIONING || AxisStatus.FFLAG_MOTIONACCEL || AxisStatus.FFLAG_MOTIONDECEL || AxisStatus.FFLAG_MOTIONCONST)
			{
//				if (IsMotioning(nIndex) == false)
//				{
					m_bMotioning[nIndex] = true;
//				}
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
			if (AxisStatus.FFLAG_ORIGINRETURNING)
			{
				if (IsHoming(nIndex) == false)
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
			if (AxisStatus.FFLAG_SERVOON)
			{
				if (IsServoStatus(nIndex) == false)
				{
					m_bServoStatus[nIndex] = true;
				}
			}
			else
			{
				if(IsServoStatus(nIndex))
				{
				m_bServoStatus[nIndex] = false;
				}
			}


			//*************************************************************************************
			//Inposition
			if(AxisStatus.FFLAG_INPOSITION)
			{
				if(IsInposStatus(nIndex) == false)
				{
					m_bInposStatus[nIndex] = true;
				}
			}
			else
			{
				if(IsInposStatus(nIndex))
				{
					m_bInposStatus[nIndex] = false;
				}
			}


			//*************************************************************************************
			//H//W + Limit
			if(AxisStatus.FFLAG_HWPOSILMT)
			{
				if(IsHWPlusLimitStatus(nIndex) == false)
				{
					m_bHWPlusLimitStatus[nIndex] = true;
				}
			}
			else
			{
				if(IsHWPlusLimitStatus(nIndex))
				{
					m_bHWPlusLimitStatus[nIndex] = false;
				}
			}


			//*************************************************************************************
			//H//W - Limit
			if(AxisStatus.FFLAG_HWNEGALMT)
			{
				if(IsHWMinusLimitStatus(nIndex) == false)
				{
					m_bHWMinusLimitStatus[nIndex] = true;
				}
			}
			else
			{
				if(IsHWMinusLimitStatus(nIndex))
				{
					m_bHWMinusLimitStatus[nIndex] = false;
				}
			}

			//*************************************************************************************
			//H//W Origin Sensor
			if (AxisStatus.FFLAG_ORIGINSENSOR)
			{
				if (IsHWHomeStatus(nIndex) == false)
				{
					m_bHWHomeStatus[nIndex] = true;
				}
			}
			else
			{
				if (IsHWHomeStatus(nIndex))
				{
					m_bHWHomeStatus[nIndex] = false;
				}
			}

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
			if (AxisStatus.FFLAG_ERROVERCURRENT)
			{
				if (IsOverCurrentError(nIndex) == false)
				{
					m_bOverCurrentError[nIndex] = true;
				}
			}
			else
			{
				if (IsOverCurrentError(nIndex))
				{
					m_bOverCurrentError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���ӵ� �̻�
			if (AxisStatus.FFLAG_ERROVERSPEED)
			{
				if (IsOverSpeedError(nIndex) == false)
				{
					m_bOverSpeedError[nIndex] = true;
				}
			}
			else
			{
				if (IsOverSpeedError(nIndex))
				{
					m_bOverSpeedError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//��ġ�����̻�
			if (AxisStatus.FFLAG_ERRPOSTRACKING)
			{
				if (IsPostTrackingError(nIndex) == false)
				{
					m_bPostTrackingError[nIndex] = true;
				}
			}
			else
			{
				if (IsPostTrackingError(nIndex))
				{
					m_bPostTrackingError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//������ �̻�
			if (AxisStatus.FFLAG_ERROVERLOAD)
			{
				if (IsOverloadError(nIndex) == false)
				{
					m_bOverloadError[nIndex] = true;
				}
			}
			else
			{
				if (IsOverloadError(nIndex))
				{
					m_bOverloadError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���� �̻�
			if (AxisStatus.FFLAG_ERROVERHEAT)
			{
				if (IsOverheatError(nIndex) == false)
				{
					m_bOverheatError[nIndex] = true;
				}
			}
			else
			{
				if (IsOverheatError(nIndex))
				{
					m_bOverheatError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//ȸ������ �̻�
			if (AxisStatus.FFLAG_ERRBACKEMF)
			{
				if (IsBackEMFError(nIndex) == false)
				{
					m_bBackEMFError[nIndex] = true;
				}
			}
			else
			{
				if (IsBackEMFError(nIndex))
				{
					m_bBackEMFError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//���� ���� �̻�
			if (AxisStatus.FFLAG_ERRMOTORPOWER)
			{
				if (IsMotorPowerError(nIndex) == false)
				{
					m_bMotorPowerError[nIndex] = true;
				}
			}
			else
			{
				if (IsMotorPowerError(nIndex))
				{
					m_bMotorPowerError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//�������� �̻�
			if (AxisStatus.FFLAG_ERRINPOSITION)
			{
				if (IsInpositionError(nIndex) == false)
				{
					m_bInpositionError[nIndex] = true;
				}
			}
			else
			{
				if (IsInpositionError(nIndex))
				{
					m_bInpositionError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//��ġ �ʰ����� �̻�
			if (AxisStatus.FFLAG_ERRPOSOVERFLOW)
			{
				if (IsPosOverflowError(nIndex) == false)
				{
					m_bPosOverflowError[nIndex] = true;
				}
			}
			else
			{
				if (IsPosOverflowError(nIndex))
				{
					m_bPosOverflowError[nIndex] = false;
				}
			}


			//*************************************************************************************
			//Error ����
			if (AxisStatus.FFLAG_ERRORALL)
			{
				if (IsAlarmStatus(nIndex) == false)
				{
					m_bAlarmStatus[nIndex] = true;
				}
			}
			else
			{
				if (IsAlarmStatus(nIndex))
				{
					m_bAlarmStatus[nIndex] = false;
				}
			}

			//*************************************************************************************
			if (AxisStatus.FFLAG_SLOWSTOP)
			{
				if (IsSlowStopError(nIndex) == false)
				{
					m_bSlowStopError[nIndex] = true;
				}
			}
			else
			{
				if (IsSlowStopError(nIndex))
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

	return true;
}


bool CFastech_Motion::Robot_Status()
{
	int nReturn = 0;
	BYTE nAlarmType = 0;

	/////////////////////////////////////////////////////////////////////////////////////////

	//Error ����
	if (IsAlarmStatus(m_nSlaveNo))
	{

		//���� �˶��� ���� Ȯ��
		if (FAS_GetAlarmType(m_nPortNo, m_nSlaveNo, &nAlarmType) == FMM_OK)
		{
			//TRACE("Robot Status Error!    \n");
			Error_Status_Display(nAlarmType);
		}
		else
		{
			lastError.Format(L"Problem has been encountered (Alarm)");
		}
		return false;
	}


	//*************************************************************************************
	//Servo On ���°� �ƴѰ�� && Servo On/Off ������ �ƴ� ���
	if (m_nServoEnableFlag != 1)
	{
		if (!IsServoStatus(m_nSlaveNo))
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


bool CFastech_Motion::Robot_Status_All_Axis()
{
	int nReturn = 0;
	BYTE nAlarmType = 0;

	/////////////////////////////////////////////////////////////////////////////////////////

	//Error ����
	for (int i = 0; i < int(MOTOR_MAX_COUNT); i++)
	{
		if (m_bSalveEnable[i] == false)
		{
			continue;
		}

		if (IsAlarmStatus(i))
		{
			//���� �˶��� ���� Ȯ��
			if (FAS_GetAlarmType(m_nPortNo, m_nSlaveNo, &nAlarmType) == FMM_OK)
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
	for (int i = 0; i < int(MOTOR_MAX_COUNT); i++)
	{
		if (m_bSalveEnable[i] == false)
		{
			continue;
		}

		if (m_nServoEnableFlag != 1)
		{
			if (!IsServoStatus(i))
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

	return true;
}

bool CFastech_Motion::Servo_Alarm_Reset_All()
{
	int nReturn = 0;
	int nSlaveNo = 0;


	for (int i = 0; i < int(MOTOR_MAX_COUNT); i++)
	{
		if (m_bSalveEnable[i] == false)
		{
			continue;
		}

		nSlaveNo = m_pSalveNumber[i];

		if (IsAlarmStatus(i))
		{
			nReturn = FAS_ServoAlarmReset(m_nPortNo, nSlaveNo);

			//���� �˶��� ���� Ȯ��
			if (nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
		}
	}


	return true;
}

bool CFastech_Motion::Servo_Alarm_Reset_Single(int nslave)
{
	int nReturn = 0;
	int nSlaveNo = 0;

	nSlaveNo = nslave;

	if (IsAlarmStatus(nSlaveNo))
	{

		nReturn = FAS_ServoAlarmReset(m_nPortNo, nSlaveNo);

		//���� �˶��� ���� Ȯ��
		if (nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}

	}

	return true;
}

bool CFastech_Motion::SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue)
{
	int nReturn = 0;

	if (m_bConnected)
	{

		nReturn = FAS_SetParameter(m_nPortNo, nSlaveNo, nParaNo, lValue);

		//������ Motor ���� ����
		if (nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}

	}
	else
	{
		lastError.Format(L"[SET_PARAMETER] Communication is not connected.");
		return false;
	}

	return true;
}

bool CFastech_Motion::SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel)
{
	int nReturn = 0;

	if (m_bConnected)
	{

		nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x04000000, bLevel);

		//������ Motor ���� ����
		if (nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}
	}
	else
	{
		lastError.Format(L"[SET_IOASSIGNMAP] Communication is not connected.");
		return false;
	}

	return true;
}

bool CFastech_Motion::SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel)
{
	int nReturn = 0;

	if (m_bConnected)
	{
		if (bLevel == 1)		//Enable
		{
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x04000000, 0);
			if (nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 4, 0x08000000, 0);
			if (nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
		}
		else    //Disable
		{
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 3, 0x00000000, 0);
			if (nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
			nReturn = FAS_SetIOAssignMap(m_nPortNo, nSlaveNo, 4, 0x00000000, 0);
			if (nReturn != FMM_OK)
			{
				Error_Status_Display(nReturn);
				return false;
			}
		}
	}
	else
	{
		lastError.Format(L"[SET_IOASSIGNMAP_SLOW_STOP] Communication is not connected.");
		return false;
	}

	return true;
}

void CFastech_Motion::InitHomeOK(int Index)
{
	m_bHomeOK[Index] = false;
}

bool CFastech_Motion::MoveHome(BYTE lAxis)
{
	DWORD	dwRet;
	CString strResult = _T("");

	if (m_bConnected)
	{
		dwRet = FAS_MoveOriginSingleAxis(m_nPortNo, lAxis);
		if (dwRet != FMM_OK)
		{
			CString strData;
			strResult.Format(_T("FAS_MoveOriginSingleAxis return error[Code:%04d]"), dwRet);
			return false;
		}
		else
		{
			m_bHomeOK[lAxis] = true;
			return true;
		}		
	}
	return false;
}

bool CFastech_Motion::MoveHome_All(BYTE PortNo)
{
	DWORD	dwRet;
	CString strResult = _T("");

	if (m_bConnected)
	{
		dwRet = FAS_AllMoveOriginSingleAxis(PortNo);
		if (dwRet != FMM_OK)
		{
			CString strData;
			strResult.Format(_T("FAS_AllMoveOriginSingleAxis return error[Code:%04d]"), dwRet);
			return false;
		}
	}

	
	return false;
}

int CFastech_Motion::MoveLimit(BYTE PortNo, BYTE SlaveNo, DWORD lVelocity, int LimitPos)
{
	DWORD	dwRet;
	CString strResult = _T("");

	if (m_bConnected)
	{
		dwRet = FAS_MoveToLimit(PortNo, SlaveNo, lVelocity, LimitPos);
		if (dwRet != FMM_OK)
		{
			CString strData;
			strResult.Format(_T("FAS_MoveToLimit return error[Code:%04d]"), dwRet);
			return false;
		}
		else if (dwRet == FMM_OK)
			return true;
	}


	return false;
}

//1�� ���� ����
bool CFastech_Motion::MoveStop()
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

		nReturn = FAS_MoveStop(m_nPortNo, m_nSlaveNo);

		//������ Motor ���� ����
		if (nReturn != FMM_OK)
		{
			//TRACE("MoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//1�� ���� ����
bool CFastech_Motion::MoveStop(BYTE SlaveNo)
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

		nReturn = FAS_MoveStop(m_nPortNo, SlaveNo);

		//������ Motor ���� ����
		if (nReturn != FMM_OK)
		{
			//TRACE("MoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(50);
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//���� ���� ����
bool CFastech_Motion::AllMoveStop()
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

		nReturn = FAS_AllMoveStop(m_nPortNo);

		//���� ���� ����
		if (nReturn != FMM_OK)
		{
			//TRACE("AllMoveStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//EMG Stop
bool CFastech_Motion::AllEmergencyStop()
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_bStopping = true;
		m_bJogging = false;

		nReturn = FAS_AllEmergencyStop(m_nPortNo);

		//���� ���� ��� ����
		if (nReturn != FMM_OK)
		{
			//TRACE("AllEmergencyStop Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//Single EmergencyStop
bool CFastech_Motion::SingleEmergencyStop(BYTE SlaveNo)
{
	int nReturn = 0;

	if (m_bConnected)
	{
		nReturn = FAS_EmergencyStop(m_nPortNo, SlaveNo);

		//���� ���� ��� ����
		if (nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}

		m_bJogging = false;
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

bool CFastech_Motion::SetResetPosition(BYTE SlaveNo)
{
	int nReturn = 0;

	if (m_bConnected)
	{	
		nReturn = FAS_ClearPosition(m_nPortNo, SlaveNo);
		if (nReturn != FMM_OK)
		{
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(20);
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}

	return true;
}

//Jog ����
bool CFastech_Motion::MoveVelocity(int nAxis, DWORD lVelocity, int nDir )
{
	int nReturn = 0;

	if (m_bConnected)
	{
		m_nSlaveNo = nAxis;

		if (IsMotioning(nAxis) == true)
		{
			return true;
		}

		m_bStopping = false;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}

		EnterCriticalSection(&m_cs);
		
		nReturn = FAS_MoveVelocity(m_nPortNo, m_nSlaveNo, (DWORD)GetVelocity(lVelocity, m_nSlaveNo), nDir);

		LeaveCriticalSection(&m_cs);

		if (nReturn != FMM_OK)
		{
			//TRACE("MoveVelocity Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(30);


		m_bJogging = true;
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}
	return true;
}

BOOL CFastech_Motion::SingleAxisAbsMove(int nAxis, double dPos, DWORD lVelocity)
{
	int nReturn = 0;
	long lActPos = 0;

	if (m_bConnected)
	{
		m_nSlaveNo = nAxis;

		if (IsMotioning(nAxis) == true)
		{
			return true;
		}

		m_bStopping = false;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}
		
		// abs �̵�
		nReturn = FAS_MoveSingleAxisAbsPos(m_nPortNo, m_nSlaveNo, dPos, (DWORD)GetVelocity(lVelocity, m_nSlaveNo));

		if (nReturn != FMM_OK)
		{
			//TRACE("FAS_MoveSingleAxisIncPos Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(20);
	
	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}


	return TRUE;
}

BOOL CFastech_Motion::SingleAxisIncMove(int nAxis, double dPos, DWORD lVelocity)
{
	int nReturn = 0;
	long lActPos = 0;

	if (m_bConnected)
	{
		m_nSlaveNo = nAxis;

		if (IsMotioning(nAxis) == true)
		{
			return true;
		}

		m_bStopping = false;

		//���� Robot ���� Ȯ��
		if (!Robot_Status())
		{
			//���� Robot ���� ���� �о� ���� ������ ���
			return false;
		}

		// abs �̵�
		nReturn = FAS_MoveSingleAxisIncPos(m_nPortNo, m_nSlaveNo, dPos, (DWORD)GetVelocity(lVelocity, m_nSlaveNo));

		if (nReturn != FMM_OK)
		{
			//TRACE("FAS_MoveSingleAxisIncPos Error!    \n");
			Error_Status_Display(nReturn);
			return false;
		}
		Sleep(20);

	}
	else
	{
		lastError.Format(L"[MOTOR] Communication is not connected.");
		return false;
	}


	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
