           // HeaderBoard.cpp : ���� �����Դϴ�.
//

//#include "stdafx.h"
#include "LightController_ODA_Q.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#include "Common.h"
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLightController_ODA_PT
CLightController_ODA_Q m_ctrlLight_ODA_Q[LIGHT_CONTROLLER_PT_BOARD_COUNT];

IMPLEMENT_DYNAMIC(CLightController_ODA_Q, CWnd)

CLightController_ODA_Q::CLightController_ODA_Q()
{	
	m_nRecvType = -1;
}

CLightController_ODA_Q::~CLightController_ODA_Q()
{
	ClosePort();
}


BEGIN_MESSAGE_MAP(CLightController_ODA_Q, CWnd)
	ON_MESSAGE(WM_LIGHT_CONTROL_MSG, OnLightControlReceive)
END_MESSAGE_MAP()



// CLightController_ODA_PT �޽��� ó�����Դϴ�.

BOOL CLightController_ODA_Q::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // ȭ�� ���� ����
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, GetDesktopWindow(), 0);
}

void CLightController_ODA_Q::InitVariables(int nID)
{
	m_nId = nID;

	m_bConnected = FALSE;
	m_bHW = FALSE;
	m_bSW = FALSE;

	m_npos = 0;

	m_strRevData = _T("");

	memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	m_hEvent_HeaderBoard = NULL;
	m_hEvent_HeaderBoard = ::CreateEvent(NULL, TRUE,FALSE, NULL);
	
}

void CALLBACK CLightController_ODA_Q::Control_LIGHT_CONTROL_Q_IRQ0(int port)
{	
	if(m_ctrlLight_ODA_Q[0].m_bConnected == TRUE)
	{
		if(m_ctrlLight_ODA_Q[0].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_ODA_Q[0].m_hWnd, m_ctrlLight_ODA_Q[0].m_MSG, m_ctrlLight_ODA_Q[0].m_nPort, 0);
		}
	}	
}

void CALLBACK CLightController_ODA_Q::Control_LIGHT_CONTROL_Q_IRQ1(int port)
{	
	if(m_ctrlLight_ODA_Q[1].m_bConnected == TRUE)
	{
		if(m_ctrlLight_ODA_Q[1].m_hWnd != NULL)
		{
			::SendMessage(m_ctrlLight_ODA_Q[1].m_hWnd, m_ctrlLight_ODA_Q[1].m_MSG, m_ctrlLight_ODA_Q[1].m_nPort, 0);
		}
	}	
}

BOOL CLightController_ODA_Q::OpenPort(int Port,int BaudRate, UINT Msg, int Parity, int DataBit, int StopBit, int nIdx)
{
	if(m_bConnected == FALSE)
	{


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
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_Q_IRQ0, 1);
			}
			else if(nIdx == 1)
			{
				ret = sio_cnt_irq(Port, Control_LIGHT_CONTROL_Q_IRQ1, 1);
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
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CLightController_ODA_Q::ClosePort()
{
	if(m_bConnected == TRUE)
	{
#ifdef SCARA_CONTROL_USE
		sio_close(m_nPort);
#endif
		m_bConnected = FALSE;
	}	
}

BOOL CLightController_ODA_Q::WritePort(int Port, char *szBuffer)
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

BOOL CLightController_ODA_Q::ReadPort(int Port, char *szBuffer)
{
	char buf[LIGHT_CONTROL_Q_BUFFER_MAX_RCV];
		
	if(m_bConnected == TRUE)
	{
		int ret;
		ret = sio_read(Port, buf, LIGHT_CONTROL_Q_BUFFER_MAX_RCV);
		
		if(ret > 0)
		{
			if(ret > LIGHT_CONTROL_Q_BUFFER_MAX_RCV)
			{
				ret = LIGHT_CONTROL_Q_BUFFER_MAX_RCV;
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

LRESULT CLightController_ODA_Q::OnLightControlReceive(WPARAM nPort, LPARAM nLen)
{
	int len = 0;
	int nPos = 0;
	int bufferlen = 0;
	
	bool re = true;

	char buffer[LIGHT_CONTROL_Q_BUFFER_MAX_RCV];
	CString str = _T("");
	CString strLog = _T("");
	CString strTemp = _T("");
	CString strTemp_Command = _T("");

	////////////////////////////////////////////////////////////
	
	memset(buffer, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX_RCV);

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

BOOL CLightController_ODA_Q::WaitOK(int timeout, BOOL& kill)
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

BOOL CLightController_ODA_Q::Write_OFF()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'O';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 't';
		strValue[3] = 'p';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'F';
		strValue[7] = 'F';
		strValue[8] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Write_Channel()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'I';		// stx
		strValue[1] = 'n';		// Command
		strValue[2] = 's';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'U';
		strValue[7] = 'T';
		strValue[8] = 'P';
		strValue[9] = '1';
		strValue[10] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Write_Channel_2()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'I';		// stx
		strValue[1] = 'n';		// Command
		strValue[2] = 's';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'U';
		strValue[7] = 'T';
		strValue[8] = 'P';
		strValue[9] = '2';
		strValue[10] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Wrtie_ON()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		strValue[0] = 'O';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 't';
		strValue[3] = 'p';
		strValue[4] = 0x20;
		strValue[5] = 'O';
		strValue[6] = 'N';
		strValue[7] = LF;
		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Write_Volt_12V()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		int nTemp = 0;
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'V';		// stx
		strValue[1] = 'o';		// Command
		strValue[2] = 'l';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = '1';
		strValue[6] = '2';
		strValue[7] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		
		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;
	}
	else
	{
	}
	return FALSE;
}
BOOL CLightController_ODA_Q::Write_Current_1A()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'C';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 'r';
		strValue[3] = 'r';
		strValue[4] = 0x20;
		strValue[5] = '1';
		strValue[6] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Write_Volt_0V()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		int nTemp = 0;
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'V';		// stx
		strValue[1] = 'o';		// Command
		strValue[2] = 'l';
		strValue[3] = 't';
		strValue[4] = 0x20;
		strValue[5] = '0';
		strValue[6] = '0';
		strValue[7] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Write_Current_0A()
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strInputVolt[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strInputVolt, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'C';		// stx
		strValue[1] = 'u';		// Command
		strValue[2] = 'r';
		strValue[3] = 'r';
		strValue[4] = 0x20;
		strValue[5] = '0';
		strValue[6] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;

	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Measure_Volt(float & value)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strResult[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	value = -1;
	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strResult, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'M';		// stx
		strValue[1] = 'e';		// Command
		strValue[2] = 'a';
		strValue[3] = 's';
		strValue[4] = ':';
		strValue[5] = 'V';
		strValue[6] = 'o';
		strValue[7] = 'l';
		strValue[8] = 't';
		strValue[9] = '?';
		strValue[10] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		Sleep(50);

		if (sio_read(m_nPort, strResult, LIGHT_CONTROL_Q_BUFFER_MAX) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		value = stof(strResult);

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;
	}
	else
	{
	}
	return FALSE;
}

BOOL CLightController_ODA_Q::Measure_Current(float & value)
{
	ResetEvent(m_hEvent_HeaderBoard);

	char strValue[LIGHT_CONTROL_Q_BUFFER_MAX];
	char strResult[LIGHT_CONTROL_Q_BUFFER_MAX];
	CString strSendMsg = _T("");

	BOOL bKill = TRUE;

	value = -1;
	/////////////////////////////////////////////////////////////////////////////////////

	memset(strValue, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
	memset(strResult, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

	if (m_bConnected == TRUE)
	{
		CString strTemp = _T("");
		m_strRecvData = _T("");
		m_npos = 0;
		memset(recv, 0x00, sizeof(char)  * LIGHT_CONTROL_BUFFER_MAX);
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_BUFFER_MAX);

		//Command		
		strValue[0] = 'M';		// stx
		strValue[1] = 'e';		// Command
		strValue[2] = 'a';
		strValue[3] = 's';
		strValue[4] = ':';
		strValue[5] = 'C';
		strValue[6] = 'u';
		strValue[7] = 'r';
		strValue[8] = 'r';
		strValue[9] = '?';
		strValue[10] = LF;

		for (int i = 0; i < (int)LIGHT_CONTROL_Q_BUFFER_MAX; i++)
		{
			strSendMsg += strValue[i];
		}

		if (WritePort(m_nPort, strValue) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		Sleep(50);

		if(sio_read(m_nPort, strResult, LIGHT_CONTROL_Q_BUFFER_MAX) == FALSE)
		{
			goto ABNORMAL_EXIT;
		}

		value = stof(strResult);

		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);

		return TRUE;

	ABNORMAL_EXIT:
		memset(packet, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		memset(recv, 0x00, sizeof(char) * LIGHT_CONTROL_Q_BUFFER_MAX);
		return FALSE;
	}
	else
	{
	}
	return FALSE;
}
