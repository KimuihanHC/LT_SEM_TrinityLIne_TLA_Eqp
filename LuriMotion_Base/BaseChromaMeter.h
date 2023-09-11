
#ifndef BaseChromaMeter_h__
#define BaseChromaMeter_h__

#pragma once

#include "Buffer.h"
#include "SerialPort.h"
#include "Def_Chromameter.h"

#include <string>

using namespace std;
class CBaseChromaMeter : public CSerialPort
{
public:
	CBaseChromaMeter();
	~CBaseChromaMeter();

protected:
	virtual		void OnRecvData(__in LPBYTE rxBuf, __in UINT nPort);

	BOOL	Measure(CHROMA_PACKET & Packet, eHeadNumber num, eCommandType cmd, eCalibrationMode mode, DWORD dwTimeout);

	BOOL	SendAndWaitForPacket(basic_string <char> & strPacket, CHROMA_PACKET & Packet, DWORD dwTimeOut);

	BOOL	WaitForPacket(DWORD dwTimeOut);

	void	MakePacket(basic_string <char> & strPacket, eHeadNumber num, eCommandType cmd, char cData4[]);
	void	ParsePacket(basic_string <char> & strPacket, CHROMA_PACKET & Packet);
	void	ParseData(basic_string <char> & strData, char cData[]);

	short	CalcBCC(basic_string <char> & strCmd);

	//	void	AddMessage(basic_string <char> & strMessage, BOOL bTransmit = TRUE);

public:
	BOOL	MeasureXYZ(CHROMA_PACKET & Packet, eCalibrationMode mode = CM_NORMAL, eHeadNumber num = NUMBER_0, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	MeasureEvxy(CHROMA_PACKET & Packet, eCalibrationMode mode = CM_NORMAL, eHeadNumber num = NUMBER_0, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	MeasureEvuv(CHROMA_PACKET & Packet, eCalibrationMode mode = CM_NORMAL, eHeadNumber num = NUMBER_0, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	MeasureTemperature(CHROMA_PACKET & Packet, eCalibrationMode mode = CM_NORMAL, eHeadNumber num = NUMBER_0, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	SetEXTMode(eEXTMode mode, BOOL bResponse = TRUE, CHROMA_PACKET * pPacket = NULL, eHeadNumber num = NUMBER_0, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	SetHeadTerminalNumber(eHeadNumber num, eTerminalNumber tnum, CHROMA_PACKET * pPacket = NULL, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	SetPCConnectMode(BOOL bOn = TRUE, CHROMA_PACKET * pPacket = NULL, DWORD dwTimeout = COMM_TIME_OUT);
	BOOL	SetHold(BOOL bOn = TRUE, eHeadNumber num = NUMBER_0);

	LPCTSTR	ErrorReport(eCL200AError eErr);

	BOOL	IsPCConnected();

	COM_ERR	Connect(int nPort, int nBaud, char parity = 'N', UINT databits = 8, UINT stopbits = 1);
	COM_ERR	Disconnect(void);
	COM_ERR	SendCommand(basic_string <char> & strCmd);

	BOOL	WaitForAck(DWORD dwTimeOut = COMM_TIME_OUT);

	void	SetReceivedData(basic_string <char> & strReceived);
	void	SetReceivedData(char cReceived[]);
	basic_string <char> &	GetReceivedData();
	void	ClearReceivedData();

private:

	BOOL	m_bPCConnected;

	CQueue <char>		m_ReceivedBuffer;
	basic_string <char>	m_strReceived;
};

#endif // BaseChromaMeter_h__
