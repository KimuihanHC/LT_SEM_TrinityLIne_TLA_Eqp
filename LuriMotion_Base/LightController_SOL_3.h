#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_MSG					WM_USER + 600 
#define LIGHT_CONTROL_SOL_BUFFER_MAX				23
#define LIGHT_CONTROL_SOL_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_SOL_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_SOL_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_SOL_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_SOL_READY						0
#define LIGHT_CONTROL_SOL_DEFAULT_MODE				1


class CLightController_SOL_3 : public CWnd
{
	DECLARE_DYNAMIC(CLightController_SOL_3)

public:
	CLightController_SOL_3();
	virtual ~CLightController_SOL_3();
	

private:
	

	BOOL WritePort(int Port, char *szBuffer,int nSize);
	BOOL WaitOK(int timeout, BOOL &kill);
		
	HANDLE m_hEvent_HeaderBoard;

	static void	CALLBACK Control_LIGHT_CONTROL_SOL_IRQ0(int port);
	static void	CALLBACK Control_LIGHT_CONTROL_SOL_IRQ1(int port);


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLightControlReceive(WPARAM nPort, LPARAM nLen);

private:
	BOOL m_bHW;
	BOOL m_bSW;
	
	
	int m_nPort;
	int	m_npos;
	int m_nMode;

	char packet[LIGHT_CONTROL_SOL_BUFFER_MAX];
	char recv[LIGHT_CONTROL_SOL_BUFFER_MAX_RCV];
	

public:
	int m_nId;
	BOOL m_bConnected;
	CString m_strRecvData_LightController;
	CString m_strLastError_LightController;
	
	UINT m_MSG;

	BOOL Is_Connected() { return m_bConnected; }

	BOOL Create(CWnd* pParentWnd=NULL);
	void InitVariables(int nID);

	BOOL OpenPort(int Port, int BaudRate, UINT Msg,int Parity=0, int DataBit=1, int StopBit = 1, int nIdx = 0);
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


	// 포트 체크	
	int			m_nRecvType;

	BOOL		LIGHT_OFF();
	BOOL		LIGHT_ON();
};
extern CLightController_SOL_3 m_ctrlLight_SOL_3_1; //0F
extern CLightController_SOL_3 m_ctrlLight_SOL_3_2; //0.5B
extern CLightController_SOL_3 m_ctrlLight_SOL_3_3; //1FLR
extern CLightController_SOL_3 m_ctrlLight_SOL_3_4; //0.5R
extern CLightController_SOL_3 m_ctrlLight_SOL_3_5; //1FUR
extern CLightController_SOL_3 m_ctrlLight_SOL_3_6; //0.5T
extern CLightController_SOL_3 m_ctrlLight_SOL_3_7; //1FUL
extern CLightController_SOL_3 m_ctrlLight_SOL_3_8; //0.5L
extern CLightController_SOL_3 m_ctrlLight_SOL_3_9; //1FLL


