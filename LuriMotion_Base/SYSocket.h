#if !defined(AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_)
#define AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_

#include "pch.h"
#include "EzSock.h"	// Added by ClassView

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000
// SYsocket.h : header file
//
#include "afxwin.h"

#include "DataManager.h"



//#define WM_MODBUS_MSG				(WM_USER+9000)

#define FLAG_SOCKET_RECEIVE			1
#define FLAG_SOCKET_CONNECT			2
#define FLAG_SOCKET_CLOSE			3
#define FLAG_SOCKET_ERROR			4

#define MODE_SOCKET_READ			1
#define MODE_SOCKET_STOP			2

#define SOCKET_READ_FC				0x03
#define SOCKET_READ_ERROR_FC		0x83
#define SOCKET_WRITE_FC				0x10
#define SOCKET_WRITE_ERROR_FC		0x90

#define SOCKET_DATA_MAX_SIZE		200

#define TIMER_RECIEVE				1
#define RECIEVE_WAIT_TIME			(int)1000
#define RECIEVE_DELAY_TIME			10

enum 
{
	eComMode_Ready, 
	eComMode_PreviewStart, 
	eComMode_Insp_TLA,
	eComMode_Insp_TLA_Test,
	eComMode_Insp_SFR, 
	eComMode_Insp_Vigneting46, 
	eComMode_Insp_Vigneting82,
	eComMode_Insp_Flare, 
	eComMode_Insp_ColorCal, 
	eComMode_Insp_Blemisi, 
	eComMode_PreviewStop, 
	eComMode_Barcode, 
	eComMode_Version, 
	eComMode_SwInit, 
	eComMode_FuseID, 
	eComMode_Status, 
	eComMode_MES_CONNECT, 
	eComMode_Ver2_Ready,
	eComMode_Ver2_PreviewStart,
	eComMode_Ver2_PreviewStop,
	eComMode_SendMES, 
	eComMode_MAX
};

#if (20230905)
enum
{
	eScrew_NULL						= 0,
	eScrew_Communication_Start		= 1,
	eScrew_Communication_Start_ACK	= 2,
	eScrew_Communication_Stop		= 3,
	eScrew_Communication_Stop_Nak	= 4,
	eScrew_Communication_Stop_ACK	= 5,
	eScrew_DataMessageRequest		= 6,
	eScrew_Share_7 = 7,
	eScrew_DataMessageSubscribe		= 8,
	eScrew_MAX
};
#endif
class CSYSocket : public CWnd
{
public:
	CSYSocket(void);
	virtual ~CSYSocket(void);

// Generated message map functions
protected:
	//{{AFX_MSG(SYSocket)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnSockConnectMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockCloseMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockAcceptMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSockRecvMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	_u8 send_buf[SOCKET_DATA_MAX_SIZE];
	_u8 recv_buf[SOCKET_DATA_MAX_SIZE];

	int m_nRecvPos;
	int m_nRecieveSetTimerFlag;
	BOOL m_bRecieveCR_OK;
	BOOL m_server_accept;
	CEzSock m_socket;
	CEzSock m_socket_server;

	int m_nInitFlag;
	
	void send_msg_target(int flag, int code);


public:
	BOOL Create(CWnd* pParentWnd=NULL);

	void InitVariable(int id);
	void CloseSocket();
	void CreateSocket(DWORD server_ip, UINT server_port, int flag);
	void CreateSocket(DWORD server_ip, UINT server_port, CString strIP, int flag);

	BOOL		m_bRetrun_Recv;				// ������ Recv �ÿ� ��ȯ�Ǵ� Flag
	CString		m_strReturn_Log;			// Log�� ����� ���� ���ڿ�
		
public:	
	int m_nId;
	int m_nMode;
	int m_nLogMode_Socket;

	CString m_strLastError;
	CString m_strRecvData;			//���� �� Data
		
	BOOL m_bConnected;	
	BOOL m_bReturn;


	bool Send_Command(CString strMsg);

	void NoatepadDebug(__in LPCTSTR szDebugString);


public:
	// ������ ����
	bool Request_Recv(BYTE *byExt, BYTE *byData, int nLength);
	// 'S' �ϰ�� Return;

	// Request_Recv ���� ������.
	BOOL m_bFlag_Status;
	BOOL m_bFlag_Tester_Start;
	CString  m_strTester_ReturnFail;

	BOOL m_bFlag_Tester_Stop;

	BOOL m_bFlag_Tester_Result;
	CString m_strTester_Result;

	BOOL m_bFlag_Emg;
	BOOL m_bFlag_Origin;

	BOOL m_bFlag_Lot_Start;
	BOOL m_bFlag_Lot_Stop;
	BOOL m_bFlag_Lot_Info;

	BOOL m_bFlag_Move_Stage;				// ������ġ�� �̵��϶�� Flag
	BOOL m_bFlag_Move_Motor;				// ���͸� ��ġ����ŭ �̵��϶�� Flag Single Mode���� ���
	BOOL m_bFlag_Move_Motor_Abs;			// ���͸� ��ġ����ŭ �̵��϶�� Flag Dual Mode���� ���

	BOOL m_bFlag_ChartLight;
	BOOL m_bFlag_BlemishLight;


	int  m_nData_MoveStage_TeachingPosition;
	int  m_nData_MoveStage_TeachingPositionBK;

	int  m_nData_Motor[MOTOR_MAX_COUNT];
	int  m_nData_MoveDistance[MOTOR_MAX_COUNT];

	BOOL m_bLightChangeCommandRecv;
	int  m_nData_LightNum[LIGHT_CONTROLLER_TYPE_COUNT];
	int  m_nData_LightValue[LIGHT_CONTROLLER_TYPE_COUNT];

	BOOL	m_bVaibration_Run;
	int		m_nVaibration_Angle;
	int		m_nVaibration_Hz;
	BOOL	m_bVaibration_Stop;


	 BYTE szBuffer_Save[50];
	 int m_nLastEtxCnt;


public:
	// �̰��� �ڵ鷯�� �˻���� ��ſ� ���Ǵ� �Լ���		
	// ������ ȸ��
	bool Send_Return_s(BYTE *byExt, BYTE byTesterNum);
	bool Send_Return(BYTE *byExt, BYTE byTesterNum);
	bool Send_Return_LongData(BYTE *byExt, BYTE *byData, int nDataLength, BYTE byTesterNum);


	// Reset
	void Reset_FlagData();
	void Reset_MotorMove_FlagData();
	void Reset_LightController_FlagData();
	

	// Response
	// ���� ����ϴ� Light Contorller
	bool Response_LightController(int nTester);
	// �̵� �Ϸ��� Response.
	bool Response_StageMove(int nPos);
	bool Response_MotorMove_Rel(int nPos);
	bool Response_MotorMove_Abs(int nPos);
	bool Response_Vaibration(int nPos);
	bool Response_Vaibration_Stop(int nPos);

	bool m_bTest;


	// �۽� Request
	bool Send_Status(int nTesterNum);
	bool Send_Start(int nTesterNum, CString strBarcode);
	bool Send_Start_NoBarcode(int nTesterNum);
	bool Send_Stop(int nTesterNum);
	bool Send_Result(int nTesterNum);
	bool Send_EmgStop(int nTesterNum);
	bool Send_Origin();

	bool Send_LotStart(int nTesterNum);
	bool Send_LotStop(int nTesterNum);
	bool Send_LotInfo(CString strInfo);

	bool Send_StageMove(int nPos);
	bool Send_MotorMove_Rel(CString strMove);
	bool Send_MotorMove_Abs(CString strMove);

	// �̰� ���Ƿ� ����� ������..?
	bool Send_ChartLight(CString strLight);
	bool Send_BlemishLight(CString strLight);
		
	

	// �Ʒ��� �ڵ鷯 to �ڵ鷯 ���.
public:
	
	void ParsingData_HandlerToHandler(CString strData);

	bool Send_Handler_SampleInfo(int nHandlerNum);
	bool Send_Handler_MchStart(int nHandlerNum, CString strBarcode);
	bool Send_Handler_LivePing(int nHandlerNum);

	bool Return_Handler_SampleInfo(int nHandlerNum, int nState);
	bool Return_Handler_MchStart(int nHandlerNum, int nState);
	bool Return_Handler_LivePing(int nHandlerNum);

	// Sample:
	// @001,1,			// LIVE PING  ������ 2��° �Ķ� 1�� �;ߵȴ�.
	// @002,Barcode,    // Barcode
	// @003,1,			// 1 : START, 0 : STOP
	// @004,1,			// 1 : SHUTTLE
	CString m_strReciveBarcode;
	BOOL	m_bConnection_Live;
	BOOL	m_bRecive_StartSignal;
	BOOL    m_bRecive_ShuttleMove_OK;



};
extern CSYSocket m_ctrlOthers[LAN_COMMUNICATION_COUNT];

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSOCKET_H__0ECF3FE6_C149_45A1_827A_010F81F73C0C__INCLUDED_)
