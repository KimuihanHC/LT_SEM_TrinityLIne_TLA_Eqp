#pragma once

#include "pch.h"
#include "afxwin.h"
#include "resource.h"	

#include "User_Define.h"
#include "System_Define.h"
#include "Singleton.h"
#include "color_rgb.h"

class CCommon : public CWnd, public CSington<CCommon>
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon(void);
	virtual ~CCommon(void);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:

	HANDLE				m_closeEvent;

	THREAD_PARAM		m_ParamMsg;
	BOOL				m_bThreadShowMsg;
	int					m_nMsg_NoCloseFlag;

	int					m_nMsgTimerOpenFlag;

	static	UINT	Thread_ShowMsg(LPVOID lpParam);
	afx_msg void	ShowMsg_Thread(CString strMsg, int nMode, bool bTesterAlarm = false);

	afx_msg INT_PTR	Show_MsgBox(CString strMsg, int nMode);			//User Message Box Open	
	afx_msg void	Close_MsgBox(void);


	// dialog Msgbox
	LARGE_INTEGER m_llErrorStartTime_DoModal;
	LARGE_INTEGER m_llErrorEndTime_DoModal;
	LARGE_INTEGER m_llErrorStartTime_Thread;
	LARGE_INTEGER m_llErrorEndTime_Thread;

public:
	//=================================================
	// dlg
	bool bConnected_LightController[9];
	BOOL ShowMsgDlg_Domodal(CString strInfo, int nMode);
	//=================================================
	// Wait
	BOOL Wait_Time(double msec, BOOL *pKill);
	void DoEvent(int cnt);

	bool DoEvents(void);

	void uSleep_NoReturn(double msec);

	CString GetProgramPath();
	BOOL IsFileExist(CString strFullPath);

	int	m_nTestFlag;
	void TESTRUN();

public:
	//=================================================
	// COMMON ���� ���°͵� ������
	void Write_Main_Log(CString strLog);
	void Write_Err_Log(CString strLog);
	void Write_Seq_Log(CString strLog);
	void Write_Comm_Log(CString strLog);
	void Write_IcsComm_Log(CString strLog);
//	void Write_Barcode_Log(CString strLog);
	 
	BOOL	m_bTesterConnectFlag[5];

public:
	//=================================================
	// ������ ���� ���� ���α׷� ����� ����.
	CString m_strid;
	int		m_nGrade;

	CString GetPermissionId();
	int		GetPermissionGrade();
	
	void	SetPermissionID( CString strID );
	void	SetPermissionGrade( int nGrade);

#if(SET_INSPECTOR == SYS_GHOSTFLARE)
	bool GetCtrlLightIsConnected(int nPos);
	void SetCtrlLightConnected(int nPos, bool nState);
	bool SetCtrlLightConnectingAll();
#endif

	BOOL m_bEMO_All_InitFlag;
	BOOL GetEMOAllInitFlag();
	void SetEMOAllInitFlag(BOOL bState);

public:
	//=================================================
	// ��� START / STOP / PAUSE ��� SIDE PANNEL ���
	BOOL	Check_EMO();

	BOOL Check_AREA();
	BOOL	Check_Mch_Start();
	BOOL	Mch_Pause();
	BOOL	Mch_Stop();

	BOOL	bEMOResetFlag;

	bool	bInitMotorFlag[10];					// �̻��
	BOOL	m_bMotorOrginFlag[10];				// �̻��
//	BOOL	GetMotorOriginFlag(int nIdx);
//	void	SetMotorOriginFlag(int nIdx, BOOL bFlag);

	BOOL	Input_Block();
	BOOL	Output_Block();
	BOOL	Check_Mch_DryRun();

public:
	//=================================================
	// Conveyor Sensor Ȱ��ȭ ����.  ( OQA���� ������ ) Inline��
// 	BOOL	Conveyor1_Sensor_Select(int nPos);
// 	BOOL	Conveyor2_Sensor_Select(int nPos);
// 	BOOL	Conveyor3_Sensor_Select(int nPos);
// 
// 	BOOL	Conveyor1_Sensor_Select_Clear();
// 	BOOL	Conveyor2_Sensor_Select_Clear();
// 	BOOL	Conveyor3_Sensor_Select_Clear();

	//=================================================
	// Conveyor Sensor Ȱ��ȭ ����.
	BOOL	m_bFlagSampleInput_Block_Foced;
	BOOL	m_bFlagSampleOutput_Block_Foced;

	//=================================================
	// Dryrun Mode Ȱ��ȭ ����
// 	BOOL	m_bFlagDryRunMode;					// �̻��
 //	BOOL GetDryRunModeFlag();
// 	void SetDryRunModeFlag(BOOL bState);

	// Semi Auto Mode Ȱ��ȭ ����
// 	BOOL	m_bFlagSemiAutoMode;					// �̻��
// 	BOOL GetSemiAutoModeFlag();
// 	void SetSemiAutoModeFlag(BOOL bState);
// 	int	m_nFlagSemiAutoActionMode;					// �̻��
// 	int GetSemiAutoActionMode();
// 	void SetSemiAutoActionMode(int nState);

// 	BOOL m_bFlagStart_up_CheckMode;
// 	BOOL GetStart_up_CheckMode();
// 	void SetStart_up_CheckMode(BOOL bState);

	// ���� ����Mode
// 	bool m_bFlagOutputMode;
// 	bool GetOutPutModeFlag();
// 	void SetOutPutModeFlag(bool bState);

	BOOL	m_bRecipeChange;

	//================================================
	// ������ ���� �������� �Լ�.
	CString GetErrorName_EachLanguage(int nErrorNum);

	//================================================
	// �����߻��� �����α� �ۼ��ϴ� �Լ�.
	void	SetErrorLog_Write(CString strNumber);

	//================================================
	// Interlock
	BOOL	GetInterlock_DoorCheck();
	BOOL	GetInterlock_AreaSensor();
	BOOL	GetInterlock_SocketClose();

	BOOL	GetInterlock_GrabBoardInit();		// �̰� Main���� ���鼭 Ȯ���ؾ��ϴ� Interlock
	BOOL	m_bInterlockFlag_GrabBoardInit;

	BOOL	m_bFlagInitingBlockUI;
	BOOL	GetBlockUIFlag();
	void	SetBlockUIFlag(BOOL bState);
	int m_nUICheck;
	//================================================
	// Tester Connect Flag.
	BOOL	m_bConnectTesterFlag[LAN_COMMUNICATION_COUNT];

	// operation mode
	void SetOperationMode(__in OperationMode mode);
	OperationMode GetOperationMode();
	
private:
	//Loto Mode // [21.1018.1]
	BOOL m_bLotoMode;

	// operation mode
	OperationMode	m_OperationMode;

public:
	BOOL bUseSleepMode;
	int m_SleepModeStep;
	//Sleep Mode
	time_t m_tSleepStart;
	// [21.1018.8] Added
	time_t m_tSleepElapsed;

	//================================================
	// TOWERLAMp COMMAND
	void	SetMcStatus(INT16 rm);

	void	SetSleepModeStep(int nMode);
	int		GetSleepModeStep();
	void	SetSleepStarted(time_t time);
	time_t  GetSleepStarted();
	void	SetSleepElapsed(time_t time);
	time_t  GetSleepElapsed();

	//Loto Mode
	void SetLotoMode(BOOL b);
	BOOL GetLotoMode();

public:

#if(SET_INSPECTOR == SYS_DISTORTION) \
 ||(SET_INSPECTOR == SYS_GHOSTFLARE) \
 ||(SET_INSPECTOR == SYS_SFR_NARROW) \
 ||(SET_INSPECTOR == SYS_SFR_WIDE_H) \
 ||(SET_INSPECTOR == SYS_SFR_WIDE_V)
	BOOL SetLoading1VacuumOnOff(int nMode);
	BOOL SetLoading2VacuumOnOff(int nMode);
#endif

// 	void UpdateMainUIConveyor1			(int nPart, int nPos, BOOL bState);
// 	void UpdateMainUIConveyor2			(int nPart, int nPos, BOOL bState);
// 	void UpdateMainUIConveyorReturn		(int nPart, int nPos, BOOL bState);
// 	void UpdateMainUIPicker				(int nPart, int nPos, BOOL bState);
// 	void UpdateMainUIInspection			(int nPart, int nPos, BOOL bState);
// 	void UpdateMainUIConv2Status		(int nPos, BOOL bState);

	// RFID ���� ����
	BOOL m_bRFID_Connected;
	CString m_strRFID_ReadData;
	CString GetRfid_ReadData();

	BOOL RFID_Connect(int nDevice, int ip0, int ip1, int ip2, int ip3, int port);
	BOOL RFID_Disconnect(int nDevice);
	BOOL RFID_GetRead(int nDevice);
	BOOL RFID_SetWrite(CString strData);
};