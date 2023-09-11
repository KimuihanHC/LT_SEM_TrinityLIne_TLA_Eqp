#include "Socket_Udp.h"
#pragma comment (lib,"ws2_32.lib")

CUDPSocket m_UdpTester[LAN_COMMUNICATION_COUNT];

UINT CUDPSocket::Thread_UDPRead(LPVOID pVoid)
{
	DWORD dwWFSObj = 0;
	CUDPSocket *dlg = (CUDPSocket *)pVoid;

	int Recv_Size;

	BYTE Buffer[884] = { 0, };

	int nSize = sizeof(dlg->m_ClientAddr);
	int sendSize = 0;

	while (1)
	{
		Recv_Size = recvfrom(dlg->m_clientSock, reinterpret_cast<char*>(Buffer), 1024, 0, (struct sockaddr*) &dlg->m_ClientAddr, &nSize);

		// -1이면 에러 
		if (Recv_Size >= 0)
		{
			// RECV 성공 시 MSG 처리.. 
			CString strTemp, strMsg;
			for (int i = 0; i < Recv_Size; i++)
			{
				strTemp.Format(_T("%c"), Buffer[i]);
				strMsg += strTemp;
			}
		//	COMMON->Write_Comm_Log(strMsg);

			dlg->ParsingData_HandlerToTester(strMsg);

			strMsg = _T("");
			CString str;
			str.Format(_T("%s"), Buffer);	
		}

		if (dlg->m_bThreadRun == FALSE)
		{
			break;
		}
	}

	closesocket(dlg->m_clientSock); //소켓 닫기

	return 0;

}

CUDPSocket::CUDPSocket(void)
{
	for(int i=0; i<10; i++)
	{
		m_bAckFlag_Inspection_Flare[i] = FALSE;
		m_nTesterFlag_Inspection_FlareErrorCode[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		m_bAckFlag_Distortion[i] = FALSE;
		m_nTesterFlag_Inspection_DistortionErrorCode[i] = 0;
	}
	m_bThreadRun = TRUE;
	pUDP_RecvThread = AfxBeginThread(Thread_UDPRead, this);
}

CUDPSocket::~CUDPSocket(void)
{
	CloseSocket();
}

void CUDPSocket::InitVariable(int id)
{
	m_nId = id;
}

BEGIN_MESSAGE_MAP(CUDPSocket, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CUDPSocket::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, _T("SYSocket"), WS_CHILD, rect, GetDesktopWindow(), 1);
}

int CUDPSocket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

void CUDPSocket::CloseSocket()
{
		m_bConnected = FALSE;
}
BOOL CUDPSocket::Thread_Start()
{
	if (m_bThreadRun == FALSE)
	{
		m_bThreadRun = TRUE;
		pUDP_RecvThread = AfxBeginThread(Thread_UDPRead, this);
	}
	else
	{
		if (m_bThreadRun)
		{
			m_bThreadRun = FALSE;
			WaitForSingleObject(pUDP_RecvThread->m_hThread, INFINITE);
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CUDPSocket::Thread_Stop()
{
	m_bThreadRun = FALSE;
	return TRUE;
}



void CUDPSocket::Connect_Tester(DWORD server_ip, UINT server_port, CString strIP, int flag)
{
	CString strTemp = _T("$H0900000000000000#");

	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	WSADATA wsdata;
	WSAStartup(MAKEWORD(2, 2), &wsdata);

	m_clientSock = socket(AF_INET, SOCK_DGRAM, 0);
	ZeroMemory(&m_ClientAddr, sizeof(m_ClientAddr));
	ZeroMemory(&m_FromServer, sizeof(m_FromServer));
	

	m_ClientAddr.sin_family = AF_INET;
	USES_CONVERSION;
	m_ClientAddr.sin_addr.S_un.S_addr = inet_addr(T2A(strIP));
	m_ClientAddr.sin_port = htons(6565);

	//m_ClientAddr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	clock_t starttm = clock();

	do 
	{
		if (m_bAckFlag_Ready == TRUE)
		{
			m_bConnected = TRUE;
			break;
		}

		Sleep(10);
	} while ((clock() - starttm) < 2000);


	if (retval < 0 || m_bConnected == FALSE)
		COMMON->Write_Comm_Log(_T("SEND FAIL [READY]"));
	
	delete[] Buffer;	
}

bool CUDPSocket::Send_Flare_CLL_Communication(int nCondition, int nLL)
{
	CString strTemp = _T("");
	int	nLength = 0;

	strTemp.Format(_T("$H018000000%d%02d0000#"), nCondition, nLL);

	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	if (retval != 1)
	{
//		COMMON->Write_Comm_Log(_T("Msg Send Fail"));
		return FALSE;
	}

//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

bool CUDPSocket::Send_Barcode(__in CString szBCR)
{
	CString strTemp = _T("");
	int	nLength = 0;

	strTemp.Format(_T("$H_Query[BARCODE=%s]#"), szBCR);
	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

// 	if (retval != 1)
// 	{
// 		return FALSE;
// 	}

	//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

bool CUDPSocket::Send_Tester_Communication(int nMode, int nTestIdx)
{
	CString strTemp = _T("");
	int	nLength = 0;

	switch (nMode)
	{
	case eComMode_Ready:
		//strTemp = _T("$H0100000000000000#");
		strTemp = _T("$H0600000000000000#");
		break;
	case eComMode_PreviewStart:
		//strTemp = _T("$H0110000000000000#");
		strTemp = _T("$H0610000000000000#");
		break;
	case eComMode_Insp_TLA:
		//strTemp = _T("$H0120000000000000#");
		strTemp = _T("$H0620000000000000#");
		break;
	case eComMode_Insp_TLA_Test:
		//strTemp = _T("$H012?000000000000#");
		strTemp = _T("$H062?000000000000#");
		break;
	case eComMode_PreviewStop:
		//strTemp = _T("$H0190000000000000#");
		strTemp = _T("$H0690000000000000#");
		break;
	case eComMode_Insp_SFR:
		strTemp = _T("$H0130000000000000#");
		break;
	case eComMode_Insp_Blemisi:
		strTemp = _T("$H0150000000000000#");
		break;
	case eComMode_Insp_Vigneting46:
		//strTemp = _T("$H0160000000000000#");
		strTemp = _T("$H0660000000000000#");
		// 00 : poc port 셀렉트 되어있는 영상 검사
		// 01 : poc port 1
		// 02 : poc port 2 
		// 03 : poc port 3 
		// 04 : poc port 4 
		break;
	case eComMode_Insp_Vigneting82:
		//strTemp = _T("$H0160000000000000#");
		strTemp = _T("$H0660000000020000#");
		// 00 : poc port 셀렉트 되어있는 영상 검사
		// 01 : poc port 1
		// 02 : poc port 2 
		// 03 : poc port 3 
		// 04 : poc port 4 
		break;
	case eComMode_Insp_ColorCal:
		strTemp = _T("$H0280000000000000#");
		break;
	case eComMode_Ver2_Ready:
		strTemp = _T("$H0200000000000000#");
		break;
	case eComMode_Ver2_PreviewStart:
		strTemp = _T("$H0210000000000000#");
		break;
	case eComMode_Ver2_PreviewStop:
		strTemp = _T("$H0290000000000000#");
		break;
	case eComMode_Version:
		strTemp = _T("$H_Query[Version]#");
		break;
	case eComMode_SwInit:
		strTemp = _T("$H_Query[SWInitialized]#");
		break;
	case eComMode_FuseID:
		strTemp = _T("$H_Query[Fuse-ID]#");
		break;
	case eComMode_Status:
		strTemp = _T("$H_Query[Status]#");
		break;
	case eComMode_MES_CONNECT:
		strTemp = _T("$H_Query[MES_CONNECT]#");
		break;
	case eComMode_SendMES:
		strTemp = _T("$H_Query[SendMesDATA]#");
		break;
	}

	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));
	
// 	if (retval != 1)
// 	{
// 		return FALSE;
// 	}

//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

void CUDPSocket::ParsingData_HandlerToTester(CString strData)
{
	// Sample:
	// 1 $T01100000000000#
	// 2 $T_ANSWER[BARCODE=ssss]#

	// Color 광원 밝기 제어용
	// $T_CONTROL[BLU=1, ON]#
	CString strLogTemp = _T("");
	strLogTemp.Format(_T("[Recv][Tester%d] %s"), m_nId, strData);
//	COMMON->Write_Comm_Log(strLogTemp);

	CString strCommand = _T("");
	CString strRecData = _T("");
	CString strNgCode = _T("");
	CString strCLLNumber = _T("");
	CString strPixelY = _T("");
	CString strPixelX = _T("");
	CString str012cmp = _T("");


	if (strData.GetAt(1) != 'T')
	{
		strLogTemp.Format(_T("[Recv][Tester] %c"), strData.GetAt(1));
	//	COMMON->Write_Comm_Log(strLogTemp);
		return;
	}

	if (strData.GetAt(2) == '0')
	{
		// 숫자 Command 일경우.
		// 	$T0120YYYYXXXX0009#	 원형 
		strCommand = strData.Mid(2, 4); // 0120
		strRecData = strData.Mid(5, 8); // $T012 0YYYYXXX X0009#
		strCLLNumber = strData.Mid(12, 2); //	$T0120YYYYXX XX 0009#	
		strNgCode = strData.Mid(14, 4);//$T0120YYYYXXXX 0009 #
		strPixelY = strData.Mid(6, 4);//$T0120 YYYY XXXX0009#	
		strPixelX = strData.Mid(10, 4);//$T0120YYYY XXXX 0009#	
		str012cmp = strData.Mid(16, 2);//$T0120YYYYXXXX0009#	
		int nCll = _ttoi(strCLLNumber);

		if (strCommand == _T("0100") || strCommand == _T("0200")|| strCommand == _T("0600"))
		{
			m_bAckFlag_Ready = TRUE;
			m_nTesterFlag_ReadyErrorCode = _ttoi(strNgCode);
		}
		else if (strCommand == _T("0110") || strCommand == _T("0210") || strCommand == _T("0610"))
		{
// 			m_bAckFlag_Ready = TRUE;
// 			m_nTesterFlag_ReadyErrorCode = _ttoi(strNgCode);
			m_bAckFlag_PreviewStart = TRUE;
			m_nTesterFlag_PreviewStartErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0120")|| strCommand == _T("0620"))
		{
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
			m_bAckFlag_Inspection_TLA = TRUE;
			m_nTesterFlag_Inspection_TLAErrorCode = _ttoi(strNgCode);
			m_dTesterFlag_TLA_PixelY = _ttof(strPixelY) - ( 5000 + stTeach.dAdjust_Offset_Y);
			m_dTesterFlag_TLA_PixelX = _ttof(strPixelX) - 5000;
			
			double dY; dY = 0.0;
			CString strLogTemp; strLogTemp.Empty();

			dY = _ttof(strPixelY) - 5000;
			strLogTemp.Format(_T("to Test - not offset : %0.0f, Set Offset :  %0.0f"), dY, m_dTesterFlag_TLA_PixelY);
			COMMON->Write_Comm_Log(strLogTemp);



		}
		else if (strCommand == _T("012?")|| strCommand == _T("062?"))
		{
			TEACHING_PARAM	stTeach = *DATAMANAGER->GetCurrentTeachingData();
			m_bAckFlag_Inspection_TLA_Test = TRUE;
			m_nTesterFlag_Inspection_TLA_Test_ErrorCode = _ttoi(strNgCode);
			m_dTesterFlag_TLA_Test_PixelY = _ttof(strPixelY) - (5000 + stTeach.dAdjust_Offset_Y);
			m_dTesterFlag_TLA_Test_PixelX = _ttof(strPixelX) - 5000;

		}
		else if (strCommand == _T("0130"))
		{
			m_bAckFlag_Inspection_SFR = TRUE;
			m_nTesterFlag_Inspection_SFRErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0150"))
		{
			m_bAckFlag_Inspection_Blemisi = TRUE;
			m_nTesterFlag_Inspection_BlemisiErrorCode =_ttoi(strNgCode);
		}
		else if (strCommand == _T("0160") || strCommand == _T("0660"))
		{
// 			m_bAckFlag_Vigneting46 = TRUE;
// 			m_nTesterFlag_Inspection_Vigneting46ErrorCode = _ttoi(strNgCode);
			if (_ttof(strPixelX) == 1)
			{
				m_bAckFlag_Vigneting46 = TRUE;
				m_nTesterFlag_Inspection_Vigneting46ErrorCode = _ttoi(strNgCode);
			}
			else if (_ttof(strPixelX) == 0)
			{
				m_bAckFlag_Vigneting46 = TRUE;
				m_nTesterFlag_Inspection_Vigneting46ErrorCode = _ttoi(strNgCode);
// 				if (m_nTesterFlag_Inspection_Vigneting46ErrorCode == 61)
// 				{
// 					m_nTesterFlag_Inspection_Vigneting46ErrorCode = 0;
// 				}
		
			}
			else if (_ttof(strPixelX) == 2)
			{
				m_bAckFlag_Vigneting82 = TRUE;
				m_nTesterFlag_Inspection_Vigneting82ErrorCode = _ttoi(strNgCode);
			}
		}
// 		else if (strCommand == _T("0160")|| strCommand == _T("0660"))
// 		{
// 			m_bAckFlag_Vigneting82 = TRUE;
// 			m_nTesterFlag_Inspection_Vigneting82ErrorCode = _ttoi(strNgCode);
// 		}
		
		else if (strCommand == _T("0170"))
		{
			if (nCll == 10)
			{
				m_bAckFlag_Distortion[1] = TRUE;
				m_nTesterFlag_Inspection_DistortionErrorCode[1] = _ttoi(strNgCode);
			}			
			else
			{
				m_bAckFlag_Distortion[0] = TRUE;
				m_nTesterFlag_Inspection_DistortionErrorCode[0] = _ttoi(strNgCode);
			}			
		}
		else if (strCommand == _T("0180"))
		{
			m_bAckFlag_Inspection_Flare[nCll] = TRUE;
			m_nTesterFlag_Inspection_FlareErrorCode[nCll] = _ttoi(strNgCode);
		}
		else if (strCommand == _T("0280"))
		{
			m_bAckFlag_ColorCal = TRUE;
			m_nTesterFlag_Inspection_ColorCalErrorCode = _ttoi(strNgCode);
		}
		else if (strCommand == _T("0190") || strCommand == _T("0290")|| strCommand == _T("0690"))
		{
			m_bAckFlag_PreviewStop = TRUE;
			m_nTesterFlag_PreviewStopErrorCode = _ttoi(strNgCode);
		}
	}
	else if (strData.GetAt(2) == '_')
	{
		int nLength = strData.GetLength();
		int nParsingLenght = 0;
		int nStartPos = 0;
		CString strMsg = _T("");
		CString strTemp = _T("");
		CString strPart = _T("");
		CString strResult = _T("");

		for (int i = 0; i < nLength; i++)
		{
			if (strData.GetAt(i) == '[')
				nStartPos = i;
		}

		for (int i = nStartPos + 1; i < nLength - 2; i++)
		{
			strTemp.Format(_T("%c"), strData.GetAt(i));
			strMsg += strTemp;
		}

		strPart = strMsg.Left(3);

		if (strPart == _T("BAR"))
		{
			m_bAckFlag_Barcode = true;
			strMsg = strMsg + _T("=");
			AfxExtractSubString(m_strTesterQuery_Barcode, strMsg, 1, '=');
		}
		else if (strPart == _T("Ver"))
		{
			m_strTesterQuery_Version = strMsg;
		}
		else if (strPart == _T("SWI"))
		{
			strMsg = strMsg + _T("=");
			AfxExtractSubString(strResult, strMsg, 1, '=');

			if (strResult == _T("TRUE"))
				m_strTesterQuery_SWInital = TRUE;
			else
				m_strTesterQuery_SWInital = FALSE;
		}
		else if (strPart == _T("Fus"))
		{
			strMsg = strMsg + _T("=");
			AfxExtractSubString(m_strTesterQuery_FuseID, strMsg, 1, '=');
		}
		else if (strPart == _T("Wor"))
		{
			m_strTesterQuery_Version = strMsg;
		}
		else if (strPart == _T("MES"))
		{
			if (strMsg.GetAt(4) == '_')
			{
				strMsg = strMsg + _T("=");
				AfxExtractSubString(strResult, strMsg, 1, '=');

				if (strResult == _T("TRUE"))
					m_strTesterQuery_MES_Connect = TRUE;
				else
					m_strTesterQuery_MES_Connect = FALSE;
			}
			else if (strMsg.GetAt(4) == '=')
			{
				strMsg = strMsg + _T("=");
				AfxExtractSubString(m_strTesterQuery_MesData, strMsg, 1, '=');
			}
		}
		else if (strPart == _T("BLU"))
		{
			int nLightIdx = 0;
			bool bLightOn = false;

			CString sz;

			if (strMsg.GetAt(6) == '=')
			{
				strMsg = strMsg;
				AfxExtractSubString(sz, strMsg, 0, ',');
				
				auto iSepartor = sz.Find(_T("="), 0);
				CString szTemp = sz.Mid(iSepartor+1, sz.GetLength() - iSepartor);

				nLightIdx = _ttoi(szTemp);

				AfxExtractSubString(sz, strMsg, 1, ',');
				sz.Trim();
				bLightOn = (sz.CompareNoCase(_T("ON")) == 0) ? TRUE : FALSE;
			}

			ST_MsgLightArg arg;
			arg.nLightIdx = nLightIdx;
			arg.bLightOn = bLightOn;
			
			// PostMessage (광원 idx, onoff)
			::SendMessage(m_hOwnerWnd, m_nUMRecvLightControl, (WPARAM)m_nId, (LPARAM)&arg);
		}
	}
}

void CUDPSocket::Reset_TesterFlag()
{
	m_bAckFlag_Ready = FALSE;
	m_bAckFlag_PreviewStart = FALSE;
	m_bAckFlag_Inspection_TLA = FALSE;
	m_bAckFlag_Inspection_TLA_Test = FALSE;
	m_bAckFlag_Inspection_SFR = FALSE;
	m_bAckFlag_Inspection_Blemisi = FALSE;	


	m_bAckFlag_Vigneting46 = FALSE;
	m_bAckFlag_Vigneting82 = FALSE;
	m_bAckFlag_ColorCal = FALSE;	
	m_bAckFlag_PreviewStop = FALSE;
	m_bAckFlag_Barcode = FALSE;

	m_strTesterQuery_Barcode = _T("");
	m_strTesterQuery_Version = _T("");
	m_strTesterQuery_SWInital = FALSE;
	m_strTesterQuery_FuseID = _T("");
	m_strTesterQuery_Status = _T("");
	m_strTesterQuery_MES_Connect = FALSE;
	m_strTesterQuery_MesData = _T("");

	m_nTesterFlag_ReadyErrorCode = 0;
	m_nTesterFlag_PreviewStartErrorCode = 0;
	m_nTesterFlag_Inspection_SFRErrorCode = 0;
	m_nTesterFlag_Inspection_BlemisiErrorCode = 0;
	m_nTesterFlag_Inspection_Vigneting46ErrorCode = 0;
	m_nTesterFlag_Inspection_Vigneting82ErrorCode = 0;
	m_nTesterFlag_Inspection_ColorCalErrorCode = 0;
	m_nTesterFlag_Inspection_TLAErrorCode = 0;
	m_nTesterFlag_Inspection_TLA_Test_ErrorCode = 0;
	m_nTesterFlag_PreviewStopErrorCode = 0;

	m_dTesterFlag_TLA_PixelY = -5000;
	m_dTesterFlag_TLA_PixelX = -5000;
	m_dTesterFlag_TLA_Test_PixelY = -5000;
	m_dTesterFlag_TLA_Test_PixelX = -5000;
	
	// ghost & flare
	for (int i = 0; i < 10; i++)
	{
		m_bAckFlag_Inspection_Flare[i] = FALSE;
		m_nTesterFlag_Inspection_FlareErrorCode[i] = 0;
	}

	// distortion
	for (int i = 0; i < 2; i++)
	{
		m_bAckFlag_Distortion[i] = FALSE;
		m_nTesterFlag_Inspection_DistortionErrorCode[i] = 0;
	}
}

BOOL CUDPSocket::SetOwnerHWND(HWND hOwnerWnd)
{
	m_hOwnerWnd = hOwnerWnd;
	return TRUE;
}

void CUDPSocket::SetWmRecv(UINT nWindowMessage)
{
	m_nUMRecvLightControl = nWindowMessage;
}

BOOL CUDPSocket::SendAckBLU(__in UINT nLightIdx, __in BOOL bLightOn)
{
	CString strTemp = _T("");
	int	nLength = 0;

	strTemp.Format(_T("$H_CONTROL[BLU=%d, %s]#"), nLightIdx, (bLightOn)?_T("ON"):_T("OFF"));
	int nLen = 0;

	for (int i = 0; i < strTemp.GetLength(); i++)
	{
		if ((strTemp.GetAt(i) <= 0) || (strTemp.GetAt(i) > 127))
			nLen += 2;
		else
			nLen += 1;
	}

	char* Buffer = new char[nLen + 1];

	size_t CharactersConverted = 0;

	// 유니코드 -> 아스키코드 (멀티바이트 -> 싱글바이트)
	wcstombs_s(&CharactersConverted, Buffer, nLen + 1, strTemp, _TRUNCATE);

	int retval = sendto(m_clientSock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));

	if (retval != 1)
	{
		return FALSE;
	}

	return TRUE;
}
