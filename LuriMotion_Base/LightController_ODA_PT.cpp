           // HeaderBoard.cpp : ���� �����Դϴ�.
//

//#include "stdafx.h"
#include "LightController_ODA_PT.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_ODA_PT
CLightController_ODA_PT m_ctrlLight_PT[LIGHT_CONTROLLER_PT_BOARD_COUNT];

IMPLEMENT_DYNAMIC(CLightController_ODA_PT, CWnd)

CLightController_ODA_PT::CLightController_ODA_PT()
{	
	m_nRecvType = -1;
}

CLightController_ODA_PT::~CLightController_ODA_PT()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_ODA_PT, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_ODA_PT �޽��� ó�����Դϴ�.

BOOL CLightController_ODA_PT::Create(CWnd* pParentWnd) 
{
	CRect rect = CRect(0, 0, 0, 0); // ȭ�� ���� ����
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_ODA_PT::InitVariables(int nID)
{
	m_nId = nID;

	m_bConnected = FALSE;
	m_bHW = FALSE;
	m_bSW = FALSE;

	m_npos = 0;
	m_nMode = LIGHT_CONTROL_PT_READY;

	m_strRevData = _T("");

	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
	memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	m_hEvent_HeaderBoard = NULL;
	m_hEvent_HeaderBoard = ::CreateEvent(NULL, TRUE,FALSE, NULL);
	
}

void CALLBACK CLightController_ODA_PT::Control_LIGHT_CONTROL_PT_IRQ0(int port)
{	
	if(m_ctrlLight_PT[0].m_bConnected == TRUE)
	{
		if(m_ctrlLight_PT[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_PT[0].m_hWnd, m_ctrlLight_PT[0].m_MSG, m_ctrlLight_PT[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_ODA_PT::Control_LIGHT_CONTROL_PT_IRQ1(int port)
{	
	if(m_ctrlLight_PT[1].m_bConnected == TRUE)
	{
		if(m_ctrlLight_PT[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_PT[1].m_hWnd, m_ctrlLight_PT[1].m_MSG, m_ctrlLight_PT[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_ODA_PT::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
{
	if(m_bConnected == FALSE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE

		if(IsWindow(this->m_hWnd) == TRUE)
		{
			int ret = 0, mode = 0;		

			ret = sio_open(Port);
			if(ret != SIO_OK)
			{
				return FALSE;
			}


			mode = ParityTable[Parity] | DataBitTable[DataBit] | StopBitTable[StopBit];
			ret = sio_ioctl(Port, BaudTable[BaudRate], mode);
			if(ret != SIO_OK)
			{
				return FALSE;
			}


			int	hw = m_bHW ? 3 : 0;	
			int	sw = m_bSW ? 12 : 0;   
			//if((ret = sio_flowctrl(Port, hw | sw)) != SIO_OK)
			if(ret = sio_flowctrl(Port, 0) != SIO_OK)				//H/W ��翡 �°� Flow Control ���� ��� ���� �ʿ�
			{
				return FALSE;
			}


			BOOL Dtr = TRUE , RTS = TRUE;						 
			if((ret = sio_DTR(Port, (Dtr ? 1 : 0))) != SIO_OK)
			{
				return FALSE;
			}


			if(!m_bHW)
			{
				if((ret = sio_RTS(Port, (RTS ? 1 : 0))) != SIO_OK)
				{
					return FALSE;
				}
			}
		

			ret = sio_flush(Port, 2);
			if(ret != SIO_OK)
			{
				return FALSE;
			}
		
			if(nIdx == 0)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_PT_IRQ0, 1);
			}
			else if(nIdx == 1)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_PT_IRQ1, 1);
			}
			if(ret != SIO_OK)
			{
				return false;
			}
			
			m_nPort = Port;
			m_MSG = Msg;
			m_bConnected = TRUE;
			m_nRecvType = 0;
		}
#else
		m_nPort = Port;
		m_bConnected = TRUE;
		m_nRecvType = 0;
#endif
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CLightController_ODA_PT::ClosePort()
{
	if(m_bConnected == TRUE)
	{
#ifdef SCARA_CONTROL_USE
		sio_close(m_nPort);
#endif
		m_bConnected = FALSE;
		m_nMode = LIGHT_CONTROL_PT_READY;
	}	
}

BOOL CLightController_ODA_PT::WritePort(int Port, char *szBuffer)
{	
	CString strLog = _T("");
	CString strTemp = _T("");

	if(m_bConnected == TRUE)
	{
		int ret = 0, len = 0;
		CString str = _T("");		
		
		len = (int)strlen(szBuffer) ;

		// Flush any data within input and/or output buffer
		// 0 : Input Buffer , 1: Output Buffer , 2 : Input / Output Buffer;

		ret = sio_flush(Port, 2);
		if(ret != SIO_OK)
		{
			return FALSE;
		}

		ret = sio_write(Port, szBuffer, len);
		if(ret < 0)
		{
			return FALSE;
		}

		
		//Log
		for(int j = 0; j < len - 1; j++)		//CR ����
		{
			strTemp += szBuffer[j];
		}
		strLog.Format(_T("[LIGHT_CONTROL] TX : %s [ Scara : %d ]\n"), strTemp, m_nId + 1);
	}
	else
	{
		if(m_bConnected == FALSE) 
		{
		}	
		return FALSE;
	}

	return TRUE;
}

BOOL CLightController_ODA_PT::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_PT_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_PT_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_PT_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_PT_BUFFER_MAX_RCV;
			}

			memcpy(szBuffer, buf, sizeof(char) * ret);
		}
		else
		{
			return FALSE;
		}		
	}
	else
	{
		if(m_bConnected == FALSE) 
		{
		}
		return FALSE;
	}
    return TRUE;
}

LRESULT CLightController_ODA_PT::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_PT_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX_RCV);

	COMMON->uSleep_NoReturn(20);  

	if(ReadPort((int)nPort, buffer) == TRUE)
	{
		len = (int)strlen(buffer);
		
		if(len > 0)
		{
			for(int i = 0; i < len; i++)
			{
				recv[m_npos + i] = buffer[i];
			}
			m_npos += len;

			for(int j = 0; j < m_npos; j++)
			{
				str += recv[j];
			}
			
		}
		//if(len > 0)

	}
	//if(ReadPort((int)wParam, buffer) == TRUE)

	return 0;
}

BOOL CLightController_ODA_PT::WaitOK(int timeout, BOOL& kill)
{
	DWORD time = GetTickCount();

	while(1)
	{
		if(kill == FALSE)
		{
			return FALSE;
		}
		if(WaitForSingleObject(m_hEvent_HeaderBoard, 5) == WAIT_OBJECT_0)
		{
			return TRUE;
		}
		if(GetTickCount() - time > (DWORD)timeout)
		{
			return FALSE;
		}
		Sleep(1);
		COMMON->DoEvents();
	}
	return FALSE;
}


void CLightController_ODA_PT::SetMinMaxVoltage(float fMin, float fMax)
{
	m_fMinCurrent = fMin;
	m_fMaxCurrent = fMax;

	m_fUnitCurrent = (m_fMaxCurrent - m_fMinCurrent) / 1024.0f;
}
void CLightController_ODA_PT::SetMinMaxCurrent(float fMin, float fMax)
{

}

BOOL CLightController_ODA_PT::Send_PortCheck()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_VERS_Req;

		strCommand = g_szCommand[CMD_SYST_VERS_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength ; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength ] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:
		
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}


BOOL	CLightController_ODA_PT::Send_Apply(__in float fVoltage, __in float fCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_APPLy;

		strCommand = g_szCommand[CMD_APPLy];
		
		strPara.Format(_T("%.4f,%.4f"), fVoltage, fCurrent);
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Apply(__in float fVoltage)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_APPLy;

		strCommand = g_szCommand[CMD_APPLy];

		strPara.Format(_T("%.4f"), fVoltage);
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;

}
BOOL	CLightController_ODA_PT::Send_Apply_Status(__out float& fOutVoltage, __out float& fOutCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_APPLy;
		strCommand = g_szCommand[CMD_APPLy_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;

}
BOOL	CLightController_ODA_PT::Send_Voltage(__in float fVoltage)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_VOLT;
		strCommand = g_szCommand[CMD_VOLT];

		strPara.Format(_T("%.4f"), fVoltage);
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Voltage_Req(__out float& fOutVoltage)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_VOLT_Req;
		strCommand = g_szCommand[CMD_VOLT_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Current(__in float fCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_CURR;
		strCommand = g_szCommand[CMD_CURR];

		strPara.Format(_T("%.4f"), fCurrent);
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Current_Req(__out float& fOutCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_CURR_Req;
		strCommand = g_szCommand[CMD_CURR_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}

BOOL	CLightController_ODA_PT::Send_Flow_Status(__in enCV_CC nOutCVCC)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_FLOW_Req;

		strCommand = g_szCommand[CMD_FLOW_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Meas_ALL(__out float& fOutVoltage, __out float& fOutCurrent)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_MEAS_ALL_Req;
		strCommand = g_szCommand[CMD_MEAS_ALL_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Error_Status(__out int& iOutErrCode, __out CString& szOutErrorDesc)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_ERR_Req;
		strCommand = g_szCommand[CMD_SYST_ERR_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Version_Status(__out CString& szOutVersion)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_VERS_Req;
		strCommand = g_szCommand[CMD_SYST_VERS_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Output(__in enSwitchOnOff nOutputOnOff)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_OUTP;
		strCommand = g_szCommand[CMD_SYST_OUTP];

		strPara = g_szOnOff[nOutputOnOff];
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_Output_Status(__out enSwitchOnOff& nOutputOnOff)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_OUTP_Req;
		strCommand = g_szCommand[CMD_SYST_OUTP_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_KeyLock(__in enSwitchOnOff nKeyLockOnOff)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	CString strPara = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol

		m_nRecvType = CMD_SYST_KEYL;
		
		strCommand = g_szCommand[CMD_SYST_KEYL];

		strPara = g_szOnOff[nKeyLockOnOff];
		strCommand += strPara;

		nstrLength = strCommand.GetLength();

		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
BOOL	CLightController_ODA_PT::Send_KeyLock_Status(__out enSwitchOnOff& nKeyLockOnOff)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_PT_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;
	CString strCommand = _T("");
	int nstrLength = 0;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
#ifdef LIGHT_CONTROLLER_PT_BOARD_USE
		if (m_nMode != LIGHT_CONTROL_PT_READY)
		{
			goto ABNORMAL_EXIT;
		}

		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		m_nMode = LIGHT_CONTROL_PT_DEFAULT_MODE;

		////////////////////////////////////////////////////////////////
		//make Protocol
		m_nRecvType = CMD_SYST_KEYL_Req;
		strCommand = g_szCommand[CMD_SYST_KEYL_Req];
		nstrLength = strCommand.GetLength();
		for (int i = 0; i < nstrLength; i++)
		{
			strValue[i] = strCommand.GetAt(i);
		}
		strValue[nstrLength] = 0x20;		// ��ĭ
		strValue[nstrLength + 1] = 0x0A;		// LF

		// End Protocol
		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nstrLength + 2; i++)
		{
			//Result
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}


		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_PT_BUFFER_MAX);
		return FALSE;

#else	
		return TRUE;
#endif
	}
	else
	{
	}
	return FALSE;
}
