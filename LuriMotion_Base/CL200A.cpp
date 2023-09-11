#include "pch.h"

#if(SET_INSPECTOR == SYS_TLA_B)
#include "CL200A.h"

CCL200A::CCL200A()
{
}

CCL200A::~CCL200A()
{
	Disconnect();
}

enCL200AErr CCL200A::GetLastError(__out LPCTSTR& sz)
{
	sz = g_szCl200A[m_Error];
	return m_Error;
}

BOOL CCL200A::Connect(UINT InPort, DWORD inBaudRate, BYTE inParity, BYTE inStopBits, BYTE inByteSize)
{
	BOOL bRet(FALSE);

	char szParity = 0x00;

	switch (inParity)
	{
	case 0:		szParity = 'N';		break;
	case 1:		szParity = 'E';		break;
	case 2:		szParity = 'O';		break;
	case 3:		szParity = 'M';		break;
	case 4:		szParity = 'S';		break;
	}

	if (__super::Connect(InPort, inBaudRate, szParity, inByteSize) == M_OK)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CCL200A::Disconnect()
{
	__super::Disconnect();

	m_bPCConnectOn = FALSE;
	m_bPCMeasureOn = FALSE;

	return TRUE;
}

BOOL CCL200A::SetPCConnectMode(__in BOOL bConnect)
{
	CHROMA_PACKET pk;
	BOOL bRet = __super::SetPCConnectMode(bConnect, &pk);
	
	Sleep(500);

	m_bPCConnectOn = bRet;
	return bRet;
}

BOOL CCL200A::SetMeasureMode(__in eHeadNumber enHeader, __out CHROMA_PACKET& pk)
{
	BOOL bRet(FALSE);

	m_Error = Cl200A_Err_OK;

	if (FALSE == SetHold(TRUE, enHeader))
	{
		m_Error = Cl200A_Err_FuncHold;
		goto TEST_END;
	}

	Sleep(500);

	if (FALSE == SetEXTMode(EXT_TURN_ON, TRUE, &pk, enHeader))
	{
		m_Error = Cl200A_Err_FuncExt;
		goto TEST_END;
	}

	Sleep(175);

	if (FALSE == SetEXTMode(EXT_START_MEASUREMENT, FALSE, &pk, enHeader))
	{
		m_Error = Cl200A_Err_FuncMeasure;
		goto TEST_END;
	}

	Sleep(500);

	bRet = TRUE;
	m_bPCMeasureOn = bRet;

TEST_END:

	return bRet;
}

BOOL CCL200A::GetMeasureData(__in eHeadNumber enHeader, __in eCommandType enCmd, __out CHROMA_PACKET& pk)
{
	BOOL bRet(FALSE);

	SYSTEMTIME tm;

	GetLocalTime(&tm);

	//TRACE(_T("Start =========================== %02d:%02d:%3d \r\n"), tm.wMinute, tm.wSecond, tm.wMilliseconds);
	m_Error = Cl200A_Err_OK;

	if (FALSE == SetEXTMode(EXT_START_MEASUREMENT, FALSE, &pk, enHeader))
	{
		m_Error = Cl200A_Err_FuncExt;
		goto TEST_END;
	}

	Sleep(500);

	GetLocalTime(&tm);
	//TRACE(_T("EXT Start =========================== %02d:%02d:%3d \r\n"), tm.wMinute, tm.wSecond, tm.wMilliseconds);

	switch (enCmd)
	{
	case CT_XYZ:
		bRet = MeasureXYZ(pk, CM_NORMAL, enHeader, COMM_TIME_OUT);
		break;

	case CT_Evxy:
		bRet = MeasureEvxy(pk, CM_NORMAL, enHeader, COMM_TIME_OUT);
		break;

	case CT_Evuv:
		bRet = MeasureEvuv(pk, CM_NORMAL, enHeader, COMM_TIME_OUT);
		break;
			
	default:
		break;
	}

	if (FALSE == bRet)
		m_Error = Cl200A_Err_FuncMeasure;
		
TEST_END:
	GetLocalTime(&tm);
	//TRACE(_T("END =========================== %02d:%02d:%3d \r\n"), tm.wMinute, tm.wSecond, tm.wMilliseconds);
	return bRet;
}
#endif
