#pragma once

#include "pch.h"
#include "afxwin.h"
#include "DataManager.h"


class CTCPSocket: public CWnd
{
public:
	CTCPSocket(void);
	virtual ~CTCPSocket(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static UINT	Thread_TCPRead(LPVOID lpVoid);

	DECLARE_MESSAGE_MAP()
	BOOL m_bThreadRun;
	CWinThread* pTCP_RecvThread;
	
public:

	void CloseSocket();
	BOOL Create(CWnd* pParentWnd = NULL);


	afx_msg void Connect_Common			(CString strIP, USHORT nPort);
	afx_msg BOOL Thread_Start			();
	afx_msg BOOL Thread_Stop();

	SOCKET		m_Sock;
	SOCKADDR_IN	m_Addr;
	SOCKADDR_IN	m_FromServer;

public:
	int m_nId;
	void InitVariable(int id);
	bool m_bConnected;

	// 여기는 삼성전기 TESTER PROGRAM 통신
	void ParsingData_HandlerToScrew(CString strData);

	bool Send_Common_Communication(int nHeader,int nMessageData = 0);
	bool Send_Flare_CLL_Communication	(int nCondition, int nLL);
	bool Send_Barcode					(__in CString szBCR);
	void Reset_TesterFlag				();

	BOOL			SetOwnerHWND(HWND hOwnerWnd);
	void			SetWmRecv(UINT nWindowMessage);

	BOOL SendAckBLU	(__in UINT nLightIdx, __in BOOL bLightOn);
	// 통신을 내가 보낸후에 검사기 프로그램에서 리턴이 오면 TRUE
	BOOL			m_bAckFlag_Ready;
	BOOL			m_bAckFlag_PreviewStart;
	BOOL			m_bAckFlag_Inspection_Blemisi;			// 이물검사
	BOOL			m_bAckFlag_Inspection_SFR;				// SFR 검사
	BOOL			m_bAckFlag_Inspection_TLA;				// TAL 조정
	BOOL			m_bAckFlag_Inspection_TLA_Test;				// TAL 조정
	BOOL			m_bAckFlag_Inspection_Flare[10];		// Ghost&Flare 검사
	BOOL			m_bAckFlag_Vigneting46;					// HotPixel 검사
	BOOL			m_bAckFlag_Vigneting82;					// HotPixel 검사
	BOOL			m_bAckFlag_ColorCal;					// ColorCal 검사
	BOOL			m_bAckFlag_Distortion[2];				// Distortion 검사
	BOOL			m_bAckFlag_PreviewStop;

	BOOL			m_bAckFlag_Barcode;						// 바코드



	// 받은 리턴중에 검사 결과값을 저장하는 변수들.
	int				m_nTesterFlag_ReadyErrorCode;
	int				m_nTesterFlag_PreviewStartErrorCode;
	int				m_nTesterFlag_Inspection_SFRErrorCode;					// SFR 검사
	int				m_nTesterFlag_Inspection_TLAErrorCode;					// SFR 검사
	int				m_nTesterFlag_Inspection_TLA_Test_ErrorCode;					// SFR 검사
	int				m_nTesterFlag_Inspection_BlemisiErrorCode;				// 이물검사
	int				m_nTesterFlag_Inspection_Vigneting46ErrorCode;				// HotPixel 검사
	int				m_nTesterFlag_Inspection_Vigneting82ErrorCode;				// HotPixel 검사
	int				m_nTesterFlag_Inspection_ColorCalErrorCode;				// ColorCal 검사
	int				m_nTesterFlag_Inspection_DistortionErrorCode[2];		// Distortion 검사
	int				m_nTesterFlag_Inspection_FlareErrorCode[10];			// Ghost&Flare 검사
	int				m_nTesterFlag_PreviewStopErrorCode;
	double				m_dTesterFlag_TLA_PixelY;
	double				m_dTesterFlag_TLA_PixelX;
	double				m_dTesterFlag_TLA_Test_PixelY;
	double				m_dTesterFlag_TLA_Test_PixelX;
	


	// 숫자로 리턴되는 결과값이 아닌 데이터들 저장하는 변수.
	CString			m_strTesterQuery_Barcode;
	CString			m_strTesterQuery_Version;
	BOOL			m_strTesterQuery_SWInital;
	CString			m_strTesterQuery_FuseID;
	CString			m_strTesterQuery_Status;
	BOOL			m_strTesterQuery_MES_Connect;
	CString			m_strTesterQuery_MesData;

private:
	HWND m_hOwnerWnd;
	UINT m_nUMRecvLightControl;
};

extern CTCPSocket m_TCPCommon[IP_COMMON_MAX];