#include "Common_TCP.h"

#pragma comment (lib,"ws2_32.lib")

CTCPSocket m_TCPCommon[IP_COMMON_MAX];

UINT CTCPSocket::Thread_TCPRead(LPVOID pVoid)
{
	DWORD dwWFSObj = 0;
	CTCPSocket *dlg = (CTCPSocket *)pVoid;

	int Recv_Size;

	BYTE Buffer[884] = { 0, };

	int nSize = sizeof(dlg->m_Sock);
	int sendSize = 0;

	while (1)
	{
		Recv_Size = recv(dlg->m_Sock, reinterpret_cast<char*>(Buffer), 1024, 0);

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

			dlg->ParsingData_HandlerToScrew(strMsg);

			strMsg = _T("");
			CString str;
			str.Format(_T("%s"), Buffer);	
		}

		if (dlg->m_bThreadRun == FALSE)
		{
			break;
		}
	}

	closesocket(dlg->m_Sock); //소켓 닫기

	return 0;

}

CTCPSocket::CTCPSocket(void)
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
	pTCP_RecvThread = AfxBeginThread(Thread_TCPRead, this);
}

CTCPSocket::~CTCPSocket(void)
{
	CloseSocket();
}

void CTCPSocket::InitVariable(int id)
{
	m_nId = id;
}

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CTCPSocket::Create(CWnd* pParentWnd)
{
	CRect rect = CRect(0, 0, 0, 0); // 화면 영역 설정
	return CWnd::Create(NULL, _T("CommonSocket"), WS_CHILD, rect, GetDesktopWindow(), 1);
}

int CTCPSocket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

void CTCPSocket::CloseSocket()
{
	auto error = closesocket(m_Sock);
	
	if (error == SOCKET_ERROR)
	{

	}
	WSACleanup();
	m_bConnected = FALSE;
}
BOOL CTCPSocket::Thread_Start()
{
	if (m_bThreadRun == FALSE)
	{
		m_bThreadRun = TRUE;
		pTCP_RecvThread = AfxBeginThread(Thread_TCPRead, this);
	}
	else
	{
		if (m_bThreadRun)
		{
			m_bThreadRun = FALSE;
			WaitForSingleObject(pTCP_RecvThread->m_hThread, INFINITE);
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CTCPSocket::Thread_Stop()
{
	m_bThreadRun = FALSE;
	return TRUE;
}



void CTCPSocket::Connect_Common(CString strIP, USHORT nPort)
{
	WSADATA wsdata;
	WSAStartup(MAKEWORD(2, 2), &wsdata);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&m_Addr, sizeof(m_Addr));
	ZeroMemory(&m_FromServer, sizeof(m_FromServer));
	

	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(nPort);
	if ((strIP.Compare(_T("")) == 0) || _tcslen(strIP) == 0)
		m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		InetPton(AF_INET, strIP, &m_Addr.sin_addr);

	auto error = connect(m_Sock, reinterpret_cast<const sockaddr *>(&m_Addr), sizeof(m_Addr));

	if (error == SOCKET_ERROR)
	{
		return;
	}
	m_bConnected = true;
}

bool CTCPSocket::Send_Flare_CLL_Communication(int nCondition, int nLL)
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

	int retval = sendto(m_Sock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_Addr, sizeof(m_Addr));

	if (retval != 1)
	{
//		COMMON->Write_Comm_Log(_T("Msg Send Fail"));
		return FALSE;
	}

//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

bool CTCPSocket::Send_Barcode(__in CString szBCR)
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

	int retval = sendto(m_Sock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_Addr, sizeof(m_Addr));

// 	if (retval != 1)
// 	{
// 		return FALSE;
// 	}

	//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

bool CTCPSocket::Send_Common_Communication(int nHeader, int nMessageData)
{
	CString strLogTemp = _T("");
	CString strTemp = _T(""), strHeader = _T(""), strMessageData = _T("");
	int	nLength = 0;
	int nSize = 0;
	int nHeaderLen = 20;
	int nMessageDataLen = 0;
	int nEndLen = 1;

	CString strLength = _T("0000");
	CString strMID = _T("0000");
	CString strRevision = _T("000");
	CString strNoAckFlag = _T("0");
	CString strStationId = _T("  ");
	CString strSpindleId = _T("  ");
	CString strSequenceNo = _T("  ");
	CString strMessageParts = _T(" ");
	CString strMessagePartNo = _T(" ");



	CString strSubscriptionMID = _T("");
	CString strWartedRevision = _T("");
	CString strExtraDataLength = _T("");
	CString strExtraData = _T("");
#if (0)
	int n = 0;
	char* Buffer = new char[21];
	switch (nHeader)
	{
	case eScrew_Communication_Start:
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x32;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x31;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x36;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x00;
		break;
	case eScrew_Communication_Stop:
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x32;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x33;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x31;
		Buffer[n++] = 0x30;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x20;
		Buffer[n++] = 0x00;
		break;
	}

	int retval = send(m_Sock, reinterpret_cast<char*>(Buffer), n, 0);
#else
	
	switch (nHeader)
	{
	case eScrew_Communication_Start:
		 strMID.Format(_T("%04d"), nHeader);
		 strRevision.Format(_T("%03d"), 6);
		break;
	case eScrew_Communication_Stop:
		strMID.Format(_T("%04d"), nHeader);
		strRevision.Format(_T("%03d"), 1);
		break;
	case eScrew_DataMessageSubscribe:
		strMID.Format(_T("%04d"), nHeader);
		strRevision.Format(_T("%03d"), 1);
		switch(nMessageData) {
		case 15:
			strSubscriptionMID.Format(_T("%04d"), nMessageData);
			strWartedRevision.Format(_T("%03d"), 1);
			strExtraData = _T("");
			strExtraDataLength.Format(_T("%02d"), strExtraData.GetLength());
			break;
		case 1201:
			strSubscriptionMID.Format(_T("%04d"), nMessageData);
			strWartedRevision.Format(_T("%03d"), 1);
			strExtraData.Format(_T("%031s"),"");
			strExtraDataLength.Format(_T("%02d"), strExtraData.GetLength());
			break;
		}
		break;
	}
	strMessageData = strSubscriptionMID + strWartedRevision + strExtraDataLength + strExtraData;
	nMessageDataLen = strSubscriptionMID.GetLength() + strWartedRevision.GetLength() + strExtraDataLength.GetLength() + strExtraData.GetLength();

	nLength = nHeaderLen + nMessageDataLen;
	nSize = nLength + nEndLen;
	strLength.Format(_T("%04d"), nLength);
	strHeader.Format(_T("%03s%04s%03s%s% 2s% 2s% 2s%s%s"),
		strLength,
		strMID,
		strRevision,
		strNoAckFlag,
		strStationId,
		strSpindleId,
		strSequenceNo,
		strMessageParts,
		strMessagePartNo);
	strTemp = strHeader + strMessageData;
	size_t CharactersConverted = 0;
	char* Buffer = new char[nSize];
	wcstombs_s(&CharactersConverted, Buffer, nSize, strTemp, _TRUNCATE);
	int retval = send(m_Sock, reinterpret_cast<char*>(Buffer), nSize, 0);
#endif

	//strLogTemp.Format(_T("[Send][%s] %s"), m_szIP_Common_UI_Text[m_nId], strTemp);
	//COMMON->Write_Comm_Log(strLogTemp);

 	if (retval != 1)
 	{
 		return FALSE;
 	}

//	COMMON->Write_Comm_Log(strTemp);

	return TRUE;
}

void CTCPSocket::ParsingData_HandlerToScrew(CString strData)
{
	CString strLogTemp = _T(""), strLogHeader = _T(""), strLogMessagedata = _T("");
	//strLogTemp.Format(_T("[Recv][%s] %s"), m_szIP_Common_UI_Text[m_nId], strData);
	//COMMON->Write_Comm_Log(strLogTemp);
	unsigned int nParameterSet;
	char value[12];
	CString strLength = _T("");
	unsigned long	ulLength = 0;
	CString strMID = _T("");
	unsigned long	ulMID = 0;
	CString strRevision = _T("");
	unsigned long	ulRevision = 0;
	CString strNoAckFlag = _T("");
	CString strStationId = _T("");
	CString strSpindleId = _T("");
	CString strSequenceNo = _T("");
	CString strMessageParts = _T("");
	CString strMessagePartNo = _T("");

	int     nCutValueLen = 17;
	CString temp = _T("");
	CString strPeaktorque = _T("");
	CString strTotalangle = _T("");
	auto nDataSize = sizeof(strData);

	if (nDataSize > 0) {
		strLength = strData.Mid(0, 4); // 0120
		ulLength = _ttoi(strLength);
		strMID = strData.Mid(4, 4); // $T012 0YYYYXXX X0009#
		ulMID = _ttoi(strMID);
		strRevision = strData.Mid(8, 3); //	$T0120YYYYXX XX 0009#	
		ulRevision = _ttoi(strRevision);
		strNoAckFlag = strData.Mid(11, 1);//$T0120YYYYXXXX 0009 #
		strStationId = strData.Mid(12, 2);//$T0120 YYYY XXXX0009#	
		strSpindleId = strData.Mid(14, 2);
		strSequenceNo = strData.Mid(16, 2);//$T0120YYYY XXXX 0009#	
		strMessageParts = strData.Mid(18, 1);//$T0120YYYYXXXX0009#	
		strMessagePartNo = strData.Mid(19, 1);//$T0120YYYYXXXX0009#	

		strLogHeader.Format(_T("[Recv][%s] Hearder Length = %d, MID = %d, Revision = %d, NoAckFlag = %s, StationID = %s, SpindleId = %s, SequenceNo = %s, MessageParts = %s, MessagePartNo = %s"),
			m_szIP_Common_UI_Text[m_nId], 
			ulLength, 
			ulMID,
			ulRevision,
			strNoAckFlag,
			strStationId,
			strSpindleId,
			strSequenceNo,
			strMessageParts,
			strMessagePartNo);
		if (ulMID == 1202) {
			CString PeakTorque = _T("30230");
			CString Totalangle = _T("30231");
			CString TotalDuration = _T("30232");
			int nPeakTorque = strData.Find(PeakTorque);
			int nTotalangle = strData.Find(Totalangle);
			int nTotalDuration = strData.Find(TotalDuration);
			if ((nPeakTorque > 0)) {
				int length = 29;
				temp = strData.Mid((nPeakTorque + nCutValueLen), (length - nCutValueLen - 4));
				m_ScrewLog.strPeakTorque.Format(_T("%s "), temp);
			}
			if ((nTotalangle > 0)) {
				int length = 29;
				temp = strData.Mid((nTotalangle + nCutValueLen), (length - nCutValueLen - 4));
				m_ScrewLog.strTotalAngle.Format(_T("%s "), temp);
			}
			if ((nTotalDuration > 0)) {
				int length = 29;
				temp = strData.Mid((nTotalDuration + nCutValueLen), (length - nCutValueLen - 4));
				m_ScrewLog.strDuration.Format(_T("%s "), temp);
			}
		}
		else if (ulMID == 69) {

		}		
		else if (ulMID == 15) {
			m_ScrewLog.strParameterSet = strData.Mid(20, 3);
			nParameterSet = _ttoi(m_ScrewLog.strParameterSet);
			if (nParameterSet > 0) {
				m_ScrewLog.nParameterSet = nParameterSet;
				strLogTemp.Format(_T("0,,,Run,%s %s,Start,"),m_szIP_Common_UI_Text[m_nId] , m_szScrew_Parameter_UI_Text[m_ScrewLog.nParameterSet]);
				COMMON->Write_Comm_Log(strLogTemp);
				//SemcoLog(strLogTemp);
			}
			else {
				//End
				strLogTemp.Format(_T("0,,,Run,%s %s,End, PeakTorque(Kgf.cm)=%s totalangle()=%s Duration=%s"), m_szIP_Common_UI_Text[m_nId], m_szScrew_Parameter_UI_Text[m_ScrewLog.nParameterSet], m_ScrewLog.strPeakTorque, m_ScrewLog.strDuration);
				COMMON->Write_Comm_Log(strLogTemp);
				//SemcoLog(strLogTemp);
				//clear
				m_ScrewLog.Reset();
			}
		}
		else {
			//strLogMessagedata = strData.Mid(20, (ulLength - 20));
			//strLogTemp = _T("[") + strLogHeader + _T(" ][") + strLogMessagedata + _T("]");
			//COMMON->Write_Comm_Log(strLogTemp);
		}
		
	}
		//SemcoLog(_T("0,,,Run,%s,Start,"), g_szSeqName_PickerMgr[GetStep()]);
		
		/*
		switch (ulMID) {
		case	eScrew_Communication_Start_ACK:
			strLogMessagedata = strData.Mid(20, (ulLength - 20)); //	$T0120YYYYXX XX 0009#	
			strLogTemp = strLogHeader + strLogMessagedata;
			COMMON->Write_Comm_Log(strLogTemp);
			break;
		case	eScrew_Communication_Stop_Nak:
		case	eScrew_Communication_Stop_ACK:
			COMMON->Write_Comm_Log(strLogHeader);
			break;
		}*/

	/*
	if (strData.GetAt(1) != 'T')
	{
		strLogTemp.Format(_T("[Recv][Tester] %c"), strData.GetAt(1));
		COMMON->Write_Comm_Log(strLogTemp);
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
	}*/
}

void CTCPSocket::Reset_TesterFlag()
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

BOOL CTCPSocket::SetOwnerHWND(HWND hOwnerWnd)
{
	m_hOwnerWnd = hOwnerWnd;
	return TRUE;
}

void CTCPSocket::SetWmRecv(UINT nWindowMessage)
{
	m_nUMRecvLightControl = nWindowMessage;
}

BOOL CTCPSocket::SendAckBLU(__in UINT nLightIdx, __in BOOL bLightOn)
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

	int retval = sendto(m_Sock, reinterpret_cast<char*>(Buffer), nLen, 0, (SOCKADDR*)&m_Addr, sizeof(m_Addr));

	if (retval != 1)
	{
		return FALSE;
	}

	return TRUE;
}


void CTCPSocket::SemcoLog(__in LPCTSTR szCsv, ...)
{
	TCHAR *lpszBuffer = new TCHAR[4096];

	size_t cb = 0;
	va_list args;
	va_start(args, szCsv);
	::StringCchVPrintfEx(lpszBuffer, 4096, NULL, &cb, 0, szCsv, args);
	va_end(args);
	
	WRITE_SEMCO_LOG(lpszBuffer);

	delete[] lpszBuffer;
}