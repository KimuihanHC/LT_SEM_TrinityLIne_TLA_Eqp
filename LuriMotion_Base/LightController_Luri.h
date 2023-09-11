#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_MSG					WM_USER + 600 
#define LIGHT_CONTROL_BUFFER_MAX				23
#define LIGHT_CONTROL_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_READY						0
#define LIGHT_CONTROL_DEFAULT_MODE				1


class CLightController_Luri : public CWnd
{
	DECLARE_DYNAMIC(CLightController_Luri)

public:
	CLightController_Luri();
	virtual ~CLightController_Luri();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_IRQ0(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_IRQ1(int port);	


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLightControlReceive(WPARAM nPort, LPARAM nLen);

private:
	BOOL m_bHW;
	BOOL m_bSW;
	BOOL m_bConnected;
	
	int m_nPort;
	int	m_npos;
	int m_nMode;

	char packet[LIGHT_CONTROL_BUFFER_MAX];
	char recv[LIGHT_CONTROL_BUFFER_MAX_RCV];
	

public:
	int m_nId;

	CString m_strRecvData_LightController;
	CString m_strLastError_LightController;
	
	UINT m_MSG;

	BOOL Is_Connected() { return m_bConnected; }

	BOOL Create(CWnd* pParentWnd=NULL);
	void InitVariables(int nID);

	BOOL OpenPort(int Port, int BaudRate, UINT Msg,int Parity=0, int DataBit=1, int StopBit=0, int nIdx = 0);
	BOOL ReadPort(int Port, char *szBuffer);	
	
	void ClosePort();

	CString m_strRevData;
	CString m_strRecvData;

	LURI_LIGHT_RECV		m_stRecv;
	// LURITECHT BOARD
public:
	
	float m_fMinCurrent;
	float m_fMaxCurrent;
	float m_fUnitCurrent;

	void SetMinMaxVoltage(float fMin, float fMax);
	void SetMinMaxCurrent(float fMin, float fMax);

	// ��Ʈ üũ
	BOOL		Send_PortCheck( BYTE byPortNo);
	BOOL		m_bRecv_PortCheck;
	// ���� ���� ����
	BOOL		Send_EtcSetVolt( UINT nSlotNo,  float fVolt);
	BOOL		m_bRecv_EtcSetVolt;
	// ���� ����
	BOOL		Send_DetectVolt(LURI_LIGHT_RECV& stRecv);
	BOOL		m_bRecv_DetectVolt;
	// ���� oN
	BOOL		SetVoltage_OnOff(bool bState);									//������ ���� On/ Off
	// ���� ���� ����
	BOOL		Send_EtcSetCurrent( UINT nSlotNo,  float fCurrent);
	BOOL		m_bRecv_EtcSetCurrent;
	// ���� ����
	BOOL		Send_DetectCurrent(LURI_LIGHT_RECV& stRecv);
	BOOL		m_bRecv_DetectCurrent;
	// �ڵ�/���� ��� üũ
	BOOL		Send_ControlCheck( BOOL& bAuto,  BOOL& bManual);
	BOOL		m_bRecv_ControlCheck_Auto;
	BOOL		m_bRecv_ControlCheck_Dial;

	// ��� �ܰ踦 �̿��� ���� ����, ����
	BOOL		Send_AmbientVoltOn_All(__in float fVolt);
	// ��ü ���� ����, ����
	BOOL		Send_AmbientVoltOff_All();

	// ��� �ܰ踦 �̿��� ���� ����, ����
	BOOL		Send_AmbientCurrentOn_All(__in WORD wValue);
	// ��ü ���� ����, ����
	BOOL		Send_AmbientCurrentOff_All();


};
extern CLightController_Luri m_ctrlLightController[LIGHT_CONTROLLER_LURITECH_BOARD_COUNT];


