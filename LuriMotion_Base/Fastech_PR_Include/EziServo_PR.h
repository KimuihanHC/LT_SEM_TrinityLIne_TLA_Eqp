#pragma once

#include "FAS_EziMOTIONPlusR.h"

//Timer
#define TIMER_MOTION_STATUS				1


//Pitch ���
//���� 1ȸ�� �� ���� �̵� �Ÿ� (���� : mm)						������ Ǯ�� ���� �� ��쿡�� ���� 1���� ���� ȸ���� ���� �����̴� �Ÿ��� �ⱸ�� Ȯ�� �Ͽ� �Է��Ѵ�.
//���� 1ȸ�� �� ���� �̵� ����	(50:1 ���ӱ�)  7.2��			1 : 1/50 = 360' : X
//���� 1ȸ�� �� ���� �̵� �Ÿ�  (20���� ) (���� : mm)			�ϵ� ��ġ�� �׳� ����
//���� 1ȸ�� �� ���� �̵� ����	(1:4.5 Ǯ�� ��)					1 : 4.5 = X : 360`

#define MOTOR_RESOLITION				10000

#define AXIS_LOADER1_PITCH				88						//���� 1ȸ�� �� ���� �̵� �Ÿ� (���� : mm)
#define AXIS_LOADER2_PITCH				88						//87.575
#define AXIS_LOADER3_PITCH				88
#define AXIS_UNLOADER1_PITCH			88
#define AXIS_UNLOADER2_PITCH			88
#define AXIS_UNLOADER3_PITCH			62
#define AXIS_NGBUFFER1_PITCH			62						//62.360
#define AXIS_NGBUFFER2_PITCH			62



//Axis
#define AXIS_LOADER1					0
#define AXIS_LOADER2					1
#define AXIS_LOADER3					2
#define AXIS_UNLOADER1					3
#define AXIS_UNLOADER2					4
#define AXIS_UNLOADER3					5
#define AXIS_NGBUFFER1					6
#define AXIS_NGBUFFER2					7

#define LOADER2_POS1					1			//Rotator ��
#define LOADER2_POS2					2			//Buffer ��
#define LOADER2_POS3					3			//Loading ��

#define NG_BUFFER1_POS1					1
#define NG_BUFFER1_POS2					2

#define NG_BUFFER2_POS1					1
#define NG_BUFFER2_POS2					2

//Parameter
#define PARAMETER_SLOWDOWN_SENSOR		35
#define PARAMETER_STOP_SENSOR			36
#define PARAMETER_SLOWDOWN_SPEED		37


//Time Out
#define MOVING_TIME_OUT					10000

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))


class CEziServo_PR : public CWnd
{
private:
	static CEziServo_PR *m_pInstance;

public:
	static CEziServo_PR* Get_Instance();
	void Delete_Instance();

	DECLARE_DYNAMIC(CEziServo_PR)

public:
	CEziServo_PR();
	virtual ~CEziServo_PR();

	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariables();
	void InitRobotVariables();
	
	bool m_bConnected;	

	BYTE m_nPortNo;
	DWORD m_dwBaudrate;
	BYTE m_nSlaveNo;
	int m_nSlaveCount;
	int m_pSalveNumber[MAX_SLAVE_NUMS];
	bool m_bSalveEnable[MAX_SLAVE_NUMS];

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

public:
	DWORD GetAxisStatus(int Index)			const {return m_dwAxisStatus[Index];}

	long GetCommandPos(int Index)			const {return m_nCommandPos[Index];}		
	long GetActualPos(int Index)			const {return m_nActualPos[Index];}
	long GetActualVel(int Index)			const {return m_nActualVel[Index];}
	long GetPosError(int Index)				const {return m_nPosError[Index];}

	//Motion ���� ���� ����
	bool IsHoming(int Index)				const {return m_bHoming[Index];}				//Origin ���� ��
	bool IsHomeOK(int Index)				const {return m_bHomeOK[Index];}				//Origin �Ϸ�
	bool IsMotioning(int Index)				const {return m_bMotioning[Index];}				//Motion ���� ��
	bool IsServoStatus(int Index)			const {return m_bServoStatus[Index];}			//Servo On/Off ����	
	bool IsInposStatus(int Index)			const {return m_bInposStatus[Index];}			//Inposition ����
	bool IsHWPlusLimitStatus(int Index)		const {return m_bHWPlusLimitStatus[Index];}		//H/W +Limit
	bool IsHWMinusLimitStatus(int Index)	const {return m_bHWMinusLimitStatus[Index];}	//H/W -Limit
	bool IsSWPlusLimitStatus(int Index)		const {return m_bSWPlusLimitStatus[Index];}		//S/W +Limit
	bool IsSWMinusLimitStatus(int Index)	const {return m_bSWMinusLimitStatus[Index];}	//S/W -Limit
	bool IsJogging()						const {return m_bJogging;}						//Jog ���� ��
	bool IsStopping()						const {return m_bStopping;}						//����
	bool IsRepeating()						const {return m_bRepeating;}					//�ݺ� ���� ��

	//Alarm ���� ����
	bool IsAlarmStatus(int Index)			const {return m_bAlarmStatus[Index];}			//Alarm ����
	bool IsOverCurrentError(int Index)		const {return m_bOverCurrentError[Index];}		//������ �̻�
	bool IsOverSpeedError(int Index)		const {return m_bOverSpeedError[Index];}		//���ӵ� �̻�
	bool IsPostTrackingError(int Index)		const {return m_bPostTrackingError[Index];}		//��ġ�����̻�
	bool IsOverloadError(int Index)			const {return m_bOverloadError[Index];}			//������ �̻�	
	bool IsOverheatError(int Index)			const {return m_bOverheatError[Index];}			//���� �̻�
	bool IsBackEMFError(int Index)			const {return m_bBackEMFError[Index];}			//ȸ������ �̻�
	bool IsMotorPowerError(int Index)		const {return m_bMotorPowerError[Index];}		//���� ���� �̻�
	bool IsInpositionError(int Index)		const {return m_bInpositionError[Index];}		//�������� �̻�
	bool IsPosOverflowError(int Index)		const {return m_bPosOverflowError[Index];}		//��ġ �ʰ����� �̻�
	bool IsSlowStopError(int Index)			const {return m_bSlowStopError[Index];}			//Slow Stop �̻�
	

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
	bool Servo_Enable();													//Servo On/Off
	bool Servo_Enable_Init();												//Servo On/Off(Init�� ���)
	bool Servo_Off();														//Servo Off
	bool Servo_Alarm_Reset_All();											//Servo Alarm Reset ����
	bool Servo_Alarm_Reset_Single(int nSlave);								//Servo Alarm Reset ����



	//���� ����
	long GetPulse(double CommandPos, int nAxis);
	double GetActualmm(long ActualPos, int nAxis);
	double GetVelocity(int lVelocity, int nAxis);							//��� �̵� �� �ʿ��� �ӵ� ���
	double GetActVelocity_Disp(long lVelocity, int nAxis);					//GUI ǥ�ø� ���� ��� �̵� �ӵ� ���
	double GetActPosition_Disp(long lPosition, int nAxis);					//GUI ǥ�ø� ���� ��� �� ��ġ �� ���

	bool SetParamer(BYTE nSlaveNo, BYTE nParaNo, long lValue);				//Parameter Setting
	bool SetIOAssignMap(BYTE nSlaveNo, BYTE bLevel);						//SetIOAssignMAp Setting
	bool SetIOAssignMap_SlowStop(BYTE nSlaveNo, BYTE bLevel);				//Slow Stop ��� Ȱ��ȭ/��Ȱ��ȭ

	bool MoveStop();														//1�� ����
	bool MoveStop(BYTE SlaveNo);											//������ 1�� ����
	bool AllMoveStop();														//��� �� ����
	bool AllEmergencyStop();												//EMG Stop
	bool SingleEmergencyStop(BYTE SlaveNo);									//1�� Stop
	bool MoveVelocity(DWORD lVelocity, int nDir, int nAxis);				//Jog ����
	bool SingleAxisAbsMove(DWORD lVelocity, int nAxis);						//Ins Move	

	bool SetResetPosition(BYTE SlaveNo);									//Position �� �ʱ�ȭ
};

//extern CEziServo_PR m_ctrlEziServo;