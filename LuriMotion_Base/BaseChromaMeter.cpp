#include "pch.h"
#include "BaseChromaMeter.h"

CBaseChromaMeter::CBaseChromaMeter()
{
	m_bPCConnected = FALSE;
	m_ReceivedBuffer.AllocQueue(MAXIMUM_DATA_LENGTH);
}

CBaseChromaMeter::~CBaseChromaMeter()
{

}

void CBaseChromaMeter::OnRecvData(__in LPBYTE rxBuf, __in UINT nPort)
{
	char	cRxChar = (char)rxBuf;

	if (cRxChar == COMM_LF)
	{
		int		i;
		int		nCount = m_ReceivedBuffer.GetCount();
		char	cRxData;

		ClearReceivedData();

		for (i = 0; i < nCount; i++)
		{
			m_ReceivedBuffer.Pop(cRxData);

			m_strReceived.append(1, cRxData);
		}

//		AddMessage(m_strReceived, FALSE);
	}
	else
	{
		m_ReceivedBuffer.Push(cRxChar);
	}
}

BOOL CBaseChromaMeter::Measure(CHROMA_PACKET & Packet, eHeadNumber num, eCommandType cmd, eCalibrationMode mode, DWORD dwTimeout)
{
	char	cData4[5];

	sprintf(cData4, "120%c", mode);

	basic_string <char>	strPacket;

	MakePacket(strPacket, num, cmd, cData4);

	BOOL	bResult;

	// Compare command
	if ((bResult = SendAndWaitForPacket(strPacket, Packet, dwTimeout)))
		bResult = (Packet.eCommand == cmd) ? TRUE : FALSE;

	return bResult;
}

BOOL CBaseChromaMeter::SendAndWaitForPacket(basic_string <char> & strPacket, CHROMA_PACKET & Packet, DWORD dwTimeout)
{
	BOOL	bResult;

	if ((bResult = (SendCommand(strPacket) == M_OK) ? TRUE : FALSE))
	{
		if ((bResult = WaitForPacket(dwTimeout)))
		{
			basic_string <char> & strReceived = GetReceivedData();

			ParsePacket(strReceived, Packet);

			// Compare BCC
			switch (Packet.eCommand)
			{
			case CT_XYZ:
			case CT_Evxy:
			case CT_Evuv:
			case CT_TEMP:
				bResult = (CalcBCC(strReceived.substr(1, MEASUREMENT_DATA1_INDEX + MEASUREMENT_DATA_LENGTH - 1)) == Packet.sBCC) ? TRUE : FALSE;
				break;
			default:
				bResult = (CalcBCC(strReceived.substr(1, 8)) == Packet.sBCC) ? TRUE : FALSE;
				break;
			}
		}
	}

	return bResult;
}

BOOL CBaseChromaMeter::MeasureXYZ(CHROMA_PACKET & Packet, eCalibrationMode mode /* = CM_NORMAL */, eHeadNumber num /* = NUMBER_0 */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	return Measure(Packet, num, CT_XYZ, mode, dwTimeout);
}

BOOL CBaseChromaMeter::MeasureEvxy(CHROMA_PACKET & Packet, eCalibrationMode mode /* = CM_NORMAL */, eHeadNumber num /* = NUMBER_0 */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	return Measure(Packet, num, CT_Evxy, mode, dwTimeout);
}

BOOL CBaseChromaMeter::MeasureEvuv(CHROMA_PACKET & Packet, eCalibrationMode mode /* = CM_NORMAL */, eHeadNumber num /* = NUMBER_0 */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	return Measure(Packet, num, CT_Evuv, mode, dwTimeout);
}

BOOL CBaseChromaMeter::MeasureTemperature(CHROMA_PACKET & Packet, eCalibrationMode mode /* = CM_NORMAL */, eHeadNumber num /* = NUMBER_0 */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	return Measure(Packet, num, CT_TEMP, mode, dwTimeout);
}

BOOL CBaseChromaMeter::SetEXTMode(eEXTMode mode, BOOL bResponse /* = TRUE */, CHROMA_PACKET * pPacket /* = NULL */, eHeadNumber num /* = NUMBER_0 */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	char	cData4[5];

	sprintf(cData4, "%c%c  ", mode, bResponse ? RESPONSE_ON : RESPONSE_OFF);

	basic_string <char> strPacket;

	MakePacket(strPacket, num, CT_EXT, cData4);

	BOOL	bResult;

	if (bResponse)
	{
		CHROMA_PACKET	* ppk;
		CHROMA_PACKET	pk;

		if (pPacket)
			ppk = pPacket;
		else
			ppk = &pk;

		// Compare command
		if ((bResult = SendAndWaitForPacket(strPacket, *ppk, dwTimeout)))
			bResult = (ppk->eCommand == CT_EXT) ? TRUE : FALSE;
	}
	else
	{
		bResult = (SendCommand(strPacket) == M_OK) ? TRUE : FALSE;
	}

	return bResult;
}

BOOL CBaseChromaMeter::SetHeadTerminalNumber(eHeadNumber num, eTerminalNumber tnum, CHROMA_PACKET * pPacket /* = NULL */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	char	cData4[5];

	sprintf(cData4, " 0%c%c", HIBYTE(tnum), LOBYTE(tnum));

	basic_string <char> strPacket;

	MakePacket(strPacket, num, CT_HEAD_TERM, cData4);

	BOOL	bResult;
	CHROMA_PACKET	* ppk;
	CHROMA_PACKET	pk;

	if (pPacket)
		ppk = pPacket;
	else
		ppk = &pk;

	// Compare command
	if ((bResult = SendAndWaitForPacket(strPacket, *ppk, dwTimeout)))
		bResult = (ppk->eCommand == CT_HEAD_TERM) ? TRUE : FALSE;

	return bResult;
}

BOOL CBaseChromaMeter::SetPCConnectMode(BOOL bOn /* = TRUE */, CHROMA_PACKET * pPacket /* = NULL */, DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	char	cData4[5];

	sprintf(cData4, "%c   ", bOn ? PC_CONNECT_ON : PC_CONNECT_OFF);

	basic_string <char> strPacket;

	MakePacket(strPacket, NUMBER_99, CT_PC_CONN, cData4);

	BOOL	bResult;
	CHROMA_PACKET	* ppk;
	CHROMA_PACKET	pk;

	if (pPacket)
		ppk = pPacket;
	else
		ppk = &pk;

	if ((bResult = SendAndWaitForPacket(strPacket, *ppk, dwTimeout)))
	{
		// Compare command
		if ((bResult = (ppk->eCommand == CT_PC_CONN) ? TRUE : FALSE))
			m_bPCConnected = bOn;
	}

	return bResult;
}

BOOL CBaseChromaMeter::SetHold(BOOL bOn /* = TRUE */, eHeadNumber num /* = NUMBER_0 */)
{
	char	cData4[5];

	sprintf(cData4, "1  %c", bOn ? HOLD_ON : HOLD_OFF);

	basic_string <char> strPacket;

	MakePacket(strPacket, num, CT_HOLD, cData4);

	// No response
	return (SendCommand(strPacket) == M_OK) ? TRUE : FALSE;
}

void CBaseChromaMeter::MakePacket(basic_string <char> & strPacket, eHeadNumber num, eCommandType cmd, char cData4[])
{
	basic_string <char> strCmd;

	strCmd.append(1, HIBYTE(num));
	strCmd.append(1, LOBYTE(num));
	strCmd.append(1, HIBYTE(cmd));
	strCmd.append(1, LOBYTE(cmd));
	strCmd.append(cData4);

	char	cBCC[3];

	sprintf(cBCC, "%02x", CalcBCC(strCmd));

	strPacket.append(1, COMM_STX);
	strPacket.append(strCmd);
	strPacket.append(1, COMM_ETX);
	strPacket.append(cBCC);
	strPacket.append(1, COMM_CR);
	strPacket.append(1, COMM_LF);
}

short CBaseChromaMeter::CalcBCC(basic_string <char> & strCmd)
{
	short	sBCC = 0;
	basic_string <char>::iterator	iter;

	for (iter = strCmd.begin(); iter != strCmd.end(); iter++)
		sBCC ^= *iter;

	sBCC ^= COMM_ETX;

	return sBCC;
}

void CBaseChromaMeter::ParsePacket(basic_string <char> & strPacket, CHROMA_PACKET & Packet)
{
	// Head
	Packet.eHead = (eHeadNumber)((strPacket[HEAD10_INDEX] << 8) | strPacket[HEAD1_INDEX]);

	// Error
	Packet.eErr = (eCL200AError)strPacket[ERROR_CODE_INDEX];

	// Command
	Packet.eCommand = (eCommandType)((strPacket[COMMAND10_INDEX] << 8) | strPacket[COMMAND1_INDEX]);

	if (((Packet.eCommand >> 8) & 0x00FF) == COMMAND10_0)	// Read color measurement commands
	{
		// Hold state
		char	cHoldState = strPacket[MEASUREMENT_HOLD_INDEX];

		if (cHoldState == 0x30 || cHoldState == 0x31)
			Packet.bHoldOn = FALSE;
		else if (cHoldState == 0x34 || cHoldState == 0x35)
			Packet.bHoldOn = TRUE;

		// Range changing
		Packet.bRngChanging = (strPacket[MEASUREMENT_RNG_INDEX] == 0x36) ? TRUE : FALSE;

		// Batteries state
		Packet.bBateriesLow = (strPacket[MEASUREMENT_BATT_INDEX] == 0x30) ? FALSE : TRUE;

		// Data
		ParseData(strPacket.substr(MEASUREMENT_DATA1_INDEX, MEASUREMENT_DATA_LENGTH), Packet.cData1);
		ParseData(strPacket.substr(MEASUREMENT_DATA2_INDEX, MEASUREMENT_DATA_LENGTH), Packet.cData2);
		ParseData(strPacket.substr(MEASUREMENT_DATA3_INDEX, MEASUREMENT_DATA_LENGTH), Packet.cData3);

		// BCC
		Packet.sBCC = (short)strtol(strPacket.substr(MEASUREMENT_BCC10_INDEX, 2).data(), NULL, 16);
	}
	else	// Other commands
	{
		// BCC
		Packet.sBCC = (short)strtol(strPacket.substr(OTHER_BCC10_INDEX, 2).data(), NULL, 16);

		if (((Packet.eCommand >> 8) & 0x00FF) == COMMAND10_5)
		{
			if (Packet.eCommand == CT_HEAD_TERM)
				Packet.bCFOn = (strPacket[HEAD_TERM_CF_INDEX] == CF_ON) ? TRUE : FALSE;	// Indicates the unit's CF setting
		}
	}
}

void CBaseChromaMeter::ParseData(basic_string <char> & strData, char cData[])
{
	char	cSign = strData[0];
	char	cExp = strData[5];
	double	dData = strtod(strData.substr(1, 4).data(), NULL);

	switch (cExp)
	{
	case 0x30:	dData *= 0.0001;	break;
	case 0x31:	dData *= 0.001;		break;
	case 0x32:	dData *= 0.01;		break;
	case 0x33:	dData *= 0.1;		break;
	case 0x34:	dData *= 1;			break;
	case 0x35:	dData *= 10;		break;
	case 0x36:	dData *= 100;		break;
	case 0x37:	dData *= 1000;		break;
	case 0x38:	dData *= 10000;		break;
	case 0x39:	dData *= 100000;	break;
	}

	switch (cSign)
	{
	case 0x2B:	sprintf(cData, "%0.4f", dData);		break;
	case 0x2D:	sprintf(cData, "%0.4f", -dData);	break;
	case 0x3D:	sprintf(cData, "±%0.4f", dData);	break;
	}
}

BOOL CBaseChromaMeter::WaitForPacket(DWORD dwTimeout /* = COMM_TIME_OUT */)
{
	DWORD	dwStart = ::GetTickCount();
	basic_string <char> & strReceived = GetReceivedData();

	do
	{
		if (strReceived.empty() == false)
		{
			if (strReceived.length() >= MINIMUM_DATA_LENGTH)
			{
				if (*strReceived.begin() == COMM_STX &&
					*(strReceived.end() - 4) == COMM_ETX)
					return TRUE;
			}
		}

		Sleep(1);
	} while (::GetTickCount() - dwStart < dwTimeout);

	return FALSE;
}

LPCTSTR CBaseChromaMeter::ErrorReport(eCL200AError eErr)
{
	LPCTSTR	lpszErr;

	switch (eErr)
	{
	case ERR_OK:
		lpszErr = _T("No error.");
		break;
	case ERR_POWERON:
		lpszErr = _T("Indicates that power to the head has been cut off.\r\nTurn the CL-200 off and then back on.");
		break;
	case ERR_EEPROM:
		lpszErr = _T("Indicates that an improper value has been written to the head's EE-PROM.\r\nTry turning the CL-200 off and then back on.\r\nIf the problem recurs, the unit requires service.");
		break;
	case ERR_VALUE:
		lpszErr = _T("Error in CCF value or Δtarget value.\r\nTurn the CL-200 off and then back on.");
		break;
	case ERR_OUTRANGE:
		lpszErr = _T("Setting out of range.\r\nIf returned in response to command 40 (EXT mode command), this code means that you issued the command without first switching the head's HOLD state to ON. To correct the problem, set the HOLD to ON and then reissue command 40.\r\nIf returned in response to any other command, this code should be considered equivalent to 'normal'.\r\nNote that code does not imply that there is any problem with the unit's measurement data.");
		break;
	case ERR_OVERRANGE:
		lpszErr = _T("Indicates that the measurement results exceeded the measurement range.\r\nThe response carrying this error code also includes the measurement results obtained in the measurement cycle just before this error occurred.");
		break;
	case ERR_UNDERRANGE:
		lpszErr = _T("Brightness too low to ensure reliable result.");
		break;
	case ERR_VALUERANGE:
		lpszErr = _T("The Tcp, Δuv measurement is out of range.");
		break;
	default:
		lpszErr = _T("Unknown error.");
		break;
	}

	return lpszErr;
}

BOOL CBaseChromaMeter::IsPCConnected()
{
	return m_bPCConnected;
}

COM_ERR CBaseChromaMeter::Connect(int nPort, int nBaud, char parity /* = 'N' */, UINT databits /* = 8 */, UINT stopbits /* = 1 */)
{
	COM_ERR	err;

	if (m_pOwner == NULL)
	{
		err = M_ERR_COM_OWNER;
		return err;
	}

	if (InitPort(m_pOwner, nPort, nBaud, parity, databits, stopbits, EV_RXCHAR))
	{
		StartMonitoring();

		err = M_OK;
	}
	else
	{
		err = M_ERR_COM_NOTFOUND;
	}

	return err;
}

COM_ERR CBaseChromaMeter::Disconnect()
{
	if (IsPCConnected())
	{
		SetPCConnectMode(FALSE);
	}

	BOOL	bResult = ClosePort();

	m_ReceivedBuffer.Clear();
	ClearReceivedData();

	return bResult ? M_OK : M_ERR_COM;
}

COM_ERR CBaseChromaMeter::SendCommand(basic_string <char> & strCmd)
{
	if (IsConnected())
	{
		ClearReceivedData();
		WriteToPort(strCmd.data());
		return M_OK;
	}

	return M_ERR_COM;
}

BOOL CBaseChromaMeter::WaitForAck(DWORD dwTimeout)
{
	return WaitForPacket(dwTimeout);
}

void CBaseChromaMeter::SetReceivedData(basic_string <char> & strReceived)
{
	m_strReceived = strReceived;
}

void CBaseChromaMeter::SetReceivedData(char cReceived[])
{
	m_strReceived = cReceived;
}

basic_string <char> & CBaseChromaMeter::GetReceivedData()
{
	return m_strReceived;
}

void CBaseChromaMeter::ClearReceivedData()
{
	m_strReceived.erase();
}
