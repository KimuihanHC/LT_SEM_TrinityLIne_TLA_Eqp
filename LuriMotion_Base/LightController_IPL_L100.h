#pragma once

#include "math.h"
#include "pch.h"
#include "DataManager.h"
#include "User_Define.h"

#define	WM_LIGHT_CONTROL_L100_MSG					WM_USER + 600 
#define LIGHT_CONTROL_L100_BUFFER_MAX				23
#define LIGHT_CONTROL_L100_BUFFER_MAX_RCV			100
#define LIGHT_CONTROL_L100_COMMAND_TIMEOUT			4000
#define LIGHT_CONTROL_L100_COMMAND_ALLDATA_TIMEOUT	4000
#define LIGHT_CONTROL_L100_ETX_MSG					0x40				//'@'

#define LIGHT_CONTROL_L100_READY						0
#define LIGHT_CONTROL_L100_DEFAULT_MODE				1


class CLightController_L100 : public CWnd
{
	DECLARE_DYNAMIC(CLightController_L100)

public:
	CLightController_L100();
	virtual ~CLightController_L100();
	

private:

																	// �ϳ��� ���ļ� ������ �����Ƽ�...
	BOOL WritePort(int Port, char *szBuffer);						// Buffer Size Ȯ���ϴ� �κп��� strlen ���� üũ�ϱ� ������ �����߰��� 0x00�� ���� ���̰� ª�� �νĵȴ�.
	BOOL WritePort_Length(int Port, int nLength, char *szBuffer);	// ���� Buffer ���̸� �Լ����ڷ� �ϴ� ���� �ڵ带 �߰� �Ͽ���. 200908. Jys.

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

	char packet[LIGHT_CONTROL_L100_BUFFER_MAX];
	char recv[LIGHT_CONTROL_L100_BUFFER_MAX_RCV];
	

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
	

public:
	BOOL SendLampOn();
	BOOL SendLampOff();
	BOOL SendLampValueChange(int nValue);


};
extern CLightController_L100 m_ctrlLight_L100[LIGHT_CONTROLLER_IPL_L100_COUNT];


