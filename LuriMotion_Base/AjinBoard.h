#pragma once

#include "pch.h"
#include "User_Define.h"



//Timer
//#define TIMER_MOTION_STATUS				1


//Pitch ���
//���� 1ȸ�� �� ���� �̵� �Ÿ� (���� : mm)						������ Ǯ�� ���� �� ��쿡�� ���� 1���� ���� ȸ���� ���� �����̴� �Ÿ��� �ⱸ�� Ȯ�� �Ͽ� �Է��Ѵ�.
//���� 1ȸ�� �� ���� �̵� ����	(50:1 ���ӱ�)  7.2��			1 : 1/50 = 360' : X
//���� 1ȸ�� �� ���� �̵� �Ÿ�  (20���� ) (���� : mm)			�ϵ� ��ġ�� �׳� ����
//���� 1ȸ�� �� ���� �̵� ����	(1:4.5 Ǯ�� ��)					1 : 4.5 = X : 360`


//Time Out
#define NGTRAY_WAITING_TIME_OUT			30000		// NG TRAY ��ü ��� �ð�.
#define MOTION_MOVING_TIME_OUT			20000		// ���� ���� �� �Ϸ� ��� �ð�.
#define CYLINDER_MOVING_TIME_OUT		5000		// �Ǹ��� ���� �� �Ϸ� ��� �ð�.

#define	IS_BIT_SET(value, index)		(value & ((DWORD)0x01 << index))


//���� ���� ���� �Ͽ������� üũ�ϴ� �÷���
#define MOTION_NOTORG 	0
#define MOTION_ORG 		1
typedef struct __tagMOTION_ORIGIN
{
	BOOL bOrigin;
	BOOL bOrgActFlag;
	__tagMOTION_ORIGIN()
	{
		bOrigin = FALSE;
		bOrgActFlag = FALSE;
	}
}MOTION_ORIGIN;



class CAjinMotion : public CWnd, public CSington<CAjinMotion>
{
	DECLARE_DYNAMIC(CAjinMotion)

public:
	CAjinMotion();
	virtual ~CAjinMotion();

	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariables();
	void InitRobotVariables();
	
	bool m_bConnected;	

	int m_nBdID;
	
	CString lastError;	
	BOOL IsOrigin(int Motor);
	void SetOrigin(int Motor);
	void ResetOrigin(int Motor);
	BOOL GetOriginFlag(int Motor);
	void SetOriginFlag(int Motor,BOOL bTrue);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL		m_bThreadRun;
	CWinThread *m_pMotionThread;
	static UINT	Thread_MotionRead(LPVOID lpVoid);
	

protected:	

	int m_nServoEnableFlag;											//Robot ���� �� �о� �ö� Servo ���� Flag ó��
	int m_nORGFlag;		
	MOTION_ORIGIN		m_Origin[MOTOR_MAX_COUNT];//���� �̵� ��ɽ� Flag ó��
private:
	void DoEvents();
	void DoEvents(DWORD dwMiliSeconds);

public:		
	//��� ����

	afx_msg bool Init_Ajin();
	afx_msg bool IsInit_Ajin();

	afx_msg bool LoadFile_Mot();
	afx_msg bool SaveFile_Mot();

	afx_msg void ServoOn(long lAxis);
	afx_msg void ServoOff(long lAxis);
	afx_msg bool IsServoOn(long lAxis);

	afx_msg bool SetErrorReset(long lAxis);

	afx_msg double GetActPos(long lAxis);
	afx_msg double GetCmdPos(long lAxis);
	afx_msg void GetPulseAndUnit(long lAxis, double *dUnit, long *dPulse);

	afx_msg bool Move_Home(long lAxis);
	afx_msg bool HomeResult(long lAxis);
	afx_msg BOOL GetHomeRate(long lAxis );
	afx_msg int  GetHomeRate_Gage(long lAxis);
	afx_msg DWORD Get_HomeRate_Sub();
	afx_msg void  Set_HomeRate_Sub_Zero();
	DWORD	m_dwSub;
	DWORD	m_dwMain;

	afx_msg bool MoveEStop(long lAxis);
	afx_msg bool MoveSStop(long lAxis);

	afx_msg void PositionClear(long lAxis);
	afx_msg bool MoveAxis_NonWait(long lAxis, double dPos, double dVel = 0.0f);
	afx_msg bool MoveAxis_Wait(long lAxis, double dPos, double dVel);

	afx_msg int Check_Move_Pos(long lAxis, double dPos);
	afx_msg int MotionAxisDone(long lAxis);
	afx_msg int Check_Move_End(long lAxis);
	
	
	afx_msg bool MoveAxis_Velocity(long lAxis, double dvel);
	afx_msg bool MotorMultiAxis_NonWait(long lAxisCnt, long *lpAxisNo, double *dpPos, double *dpVel);
	afx_msg void SetMotionMode( long lAxis, int nMode );

	afx_msg bool GantryEnable(long lAxisMaster, long lAxisSlave);
	afx_msg bool IsGantryGet(long lAxisMaster);

	
	afx_msg bool GantryDisEnable(long lAxisMaster, long lAxisSlave);
	afx_msg bool GetGantryModeEnable();

	afx_msg bool IsMotion(long lAxis);	


	// invaild Chk �� ������ ���� ��쿡�� �Ʒ� 5�� ������ �Ѵ�.
	afx_msg bool IsInvaildAxis();

	afx_msg bool IsServoAlarmOn();
	afx_msg bool IsServoCurPos();
	afx_msg bool IsMotionInpos();
	afx_msg bool IsLimitChk();
	afx_msg bool isServoHome();
	

public:

	// ���⼭ Ȯ���� Data�� �� Class���� �����Ѵ�.
	DWORD		m_dwLastError;
	CString		m_strLastError;

	double		m_dbPos[MOTOR_MAX_COUNT];
	DWORD		m_bReadError[MOTOR_MAX_COUNT];
	DWORD		m_bReadHome[MOTOR_MAX_COUNT];
	DWORD		m_bReadLimitP[MOTOR_MAX_COUNT];
	DWORD		m_bReadLimitM[MOTOR_MAX_COUNT];
	DWORD		m_bReadInPos[MOTOR_MAX_COUNT];
	CCriticalSection	m_csMotor[MOTOR_MAX_COUNT];
};

