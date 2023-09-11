#pragma once
#include "pch.h"
#include "User_Define.h"

#include "Fastech_PR_Include\\FAS_EziMOTIONPlusR.h"

//Timer
//#define TIMER_MOTION_STATUS_FASTECH				1
#define MOTOR_RESOLITION_FASTECH				10000

//Time Out
#define FASTECH_MOVING_TIME_OUT					10000

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))

class CFastech_Motion : public CWnd, public CSington<CFastech_Motion>
{
	DECLARE_DYNAMIC(CFastech_Motion)

public:
	CFastech_Motion();
	virtual ~CFastech_Motion();

public:
	BOOL Create(CWnd* pParentWnd = NULL);

	void InitVariables();
	void InitRobotVariables();

	bool m_bConnected;
	int m_bConnectedInitFlag;

	BYTE m_nPortNo;
	DWORD m_dwBaudrate;
	BYTE m_nSlaveNo;
	int m_nSlaveCount;
	int m_pSalveNumber[MAX_SLAVE_NUMS];
	bool m_bSalveEnable[MAX_SLAVE_NUMS];

	CString GetLastError()
	{
		return lastError;
	};
	  
	CString lastError;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL		m_bThreadRun;
	CWinThread *m_pEziServoThread;
	static UINT	Thread_EziServoRead(LPVOID lpVoid);


protected:
	DWORD *m_dwIOInputValue;
	DWORD *m_dwIOOutputValue;
	DWORD *m_dwAxisStatus;

	long *m_nCommandPos;
	long *m_nActualPos;
	long *m_nActualVel;
	long *m_nPosError;

	//Motion ���� ���� ����
	bool *m_bHoming;
	bool *m_bHomeOK;
	bool m_bJogging;
	bool m_bStopping;
	bool m_bRepeating;
	bool *m_bMotioning;
	bool *m_bServoStatus;
	bool *m_bInposStatus;
	bool *m_bHWPlusLimitStatus;
	bool *m_bHWMinusLimitStatus;
	bool *m_bHWHomeStatus;
	bool *m_bSWPlusLimitStatus;
	bool *m_bSWMinusLimitStatus;

	//Alarm ���� ����
	bool *m_bAlarmStatus;
	bool *m_bOverCurrentError;
	bool *m_bOverSpeedError;
	bool *m_bPostTrackingError;
	bool *m_bOverloadError;
	bool *m_bOverheatError;
	bool *m_bBackEMFError;
	bool *m_bMotorPowerError;
	bool *m_bInpositionError;
	bool *m_bPosOverflowError;
	bool *m_bSlowStopError;

	WORD *wPosItemNo;

	int m_nServoEnableFlag;											//Robot ���� �� �о� �ö� Servo ���� Flag ó��
	int m_nORGFlag;													//���� �̵� ��ɽ� Flag ó��

	CRITICAL_SECTION m_cs;
public:
	DWORD GetAxisStatus(int Index)			const { return m_dwAxisStatus[Index]; }

	long GetCommandPos(int Index)			const { return m_nCommandPos[Index]; }
	long GetActualPos(int Index)			const { return m_nActualPos[Index]; }
	long GetActualVel(int Index)			const { return m_nActualVel[Index]; }
	long GetPosError(int Index)				const { return m_nPosError[Index]; }

	//Motion ���� ���� ����
	bool IsHoming(int Index)				const { return m_bHoming[Index]; }				//Origin ���� ��
	bool IsHomeOK(int Index)				const { return m_bHomeOK[Index]; }				//Origin �Ϸ�
	bool IsMotioning(int Index)				const { return m_bMotioning[Index]; }				//Motion ���� ��
	bool IsServoStatus(int Index)			const { return m_bServoStatus[Index]; }			//Servo On/Off ����	
	bool IsInposStatus(int Index)			const { return m_bInposStatus[Index]; }			//Inposition ����
	bool IsHWPlusLimitStatus(int Index)		const { return m_bHWPlusLimitStatus[Index]; }		//H/W +Limit
	bool IsHWMinusLimitStatus(int Index)	const { return m_bHWMinusLimitStatus[Index]; }	//H/W -Limit
	bool IsHWHomeStatus(int Index)			const { return m_bHWHomeStatus[Index]; }		//H/W Home
	bool IsSWPlusLimitStatus(int Index)		const { return m_bSWPlusLimitStatus[Index]; }		//S/W +Limit
	bool IsSWMinusLimitStatus(int Index)	const { return m_bSWMinusLimitStatus[Index]; }	//S/W -Limit
	bool IsJogging()						const { return m_bJogging; }						//Jog ���� ��
	bool IsStopping()						const { return m_bStopping; }						//����
	bool IsRepeating()						const { return m_bRepeating; }					//�ݺ� ���� ��

	//Alarm ���� ����
	bool IsAlarmStatus(int Index)			const { return m_bAlarmStatus[Index]; }			//Alarm ����
	bool IsOverCurrentError(int Index)		const { return m_bOverCurrentError[Index]; }		//������ �̻�
	bool IsOverSpeedError(int Index)		const { return m_bOverSpeedError[Index]; }		//���ӵ� �̻�
	bool IsPostTrackingError(int Index)		const { return m_bPostTrackingError[Index]; }		//��ġ�����̻�
	bool IsOverloadError(int Index)			const { return m_bOverloadError[Index]; }			//������ �̻�	
	bool IsOverheatError(int Index)			const { return m_bOverheatError[Index]; }			//���� �̻�
	bool IsBackEMFError(int Index)			const { return m_bBackEMFError[Index]; }			//ȸ������ �̻�
	bool IsMotorPowerError(int Index)		const { return m_bMotorPowerError[Index]; }		//���� ���� �̻�
	bool IsInpositionError(int Index)		const { return m_bInpositionError[Index]; }		//�������� �̻�
	bool IsPosOverflowError(int Index)		const { return m_bPosOverflowError[Index]; }		//��ġ �ʰ����� �̻�
	bool IsSlowStopError(int Index)			const { return m_bSlowStopError[Index]; }			//Slow Stop �̻�


public:
	//��� ����
	bool Connect_EziServo(BYTE PortNo, DWORD Baudrate);						//��� ����
	void Disconnect_EziServo();												//��� ���� ����

	//Robot ���� ����
	void Error_Status_Display(int Return);									//Error Display
	bool Robot_Status();													//���� 1�� �κ� ����
	bool Robot_Status_All_Axis();											//���� ���� �κ� ����
	bool GetAllStatus();													//Robot�� ��� ���� ���� �о� �´�

	//Servo ���� ����
	bool Servo_Enable(BYTE SlaveNo);													//Servo On/Off //
	bool Servo_Enable_Init();												//Servo On/Off(Init�� ���)
	bool Servo_Off();														//Servo Off
	bool Servo_Off(BYTE SlaveNo);
	bool Servo_Alarm_Reset_All();											//Servo Alarm Reset ����
	bool Servo_Alarm_Reset_Single(int nSlave);								//Servo Alarm Reset ����

	double GetAxisCurrnetActualPos(int nIndex);


	//���� ����
	long GetPulse(double CommandPos, int nAxis);
	double GetActualmm(long ActualPos, int nAxis);
	double GetVelocity(int lVelocity, int nAxis);							//��� �̵� �� �ʿ��� �ӵ� ���
	double GetActVelocity_Disp(long lVelocity, int nAxis);					//GUI ǥ�ø� ���� ��� �̵� �ӵ� ���
	double GetActPosition_Disp(long lPosition, int nAxis);					//GUI ǥ�ø� ���� ��� �� ��ġ �� ���

	bool SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue);				//Parameter Setting
	bool SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel);						//SetIOAssignMAp Setting
	bool SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel);				//Slow Stop ��� Ȱ��ȭ/��Ȱ��ȭ

	void InitHomeOK(int Index);			//Origin �Ϸ�
	bool MoveHome(BYTE lAxis);													//�ش� �� ��ġ�� �̵�
	bool MoveHome_All(BYTE PortNo);										//�� �� �ʱ�ȭ
	int	 MoveLimit(BYTE PortNo, BYTE SlaveNo, DWORD lVelocity, int LimitPos);				//Limit��ġ�� �̵� -0 +1
	bool MoveStop();														//1�� ����
	bool MoveStop(BYTE SlaveNo);											//������ 1�� ����
	bool AllMoveStop();														//��� �� ����
	bool AllEmergencyStop();												//EMG Stop
	bool SingleEmergencyStop(BYTE SlaveNo);									//1�� Stop
	bool MoveVelocity(int nAxis, DWORD lVelocity, int nDir);				//Jog ����
	BOOL SingleAxisAbsMove(int nAxis, double dPos, DWORD lVelocity );										//abs Move	
	BOOL SingleAxisIncMove(int nAxis, double dPos, DWORD lVelocity);										//abs Move	
	bool SetResetPosition(BYTE SlaveNo);									//Position �� �ʱ�ȭ
};