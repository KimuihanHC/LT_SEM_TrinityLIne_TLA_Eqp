#include "pch.h"
#include "Device.h"
#include "DlgLog.h"

#include "Util/BasicTimer.h"
#include "PCOMM/PCOMM.H"
#include "SerialParam.h"

#define ICS_Use

CDevice::CDevice()
{

}

CDevice::~CDevice()
{
}

void CDevice::InitDevice()
{
	Start_ICS_AutoConnect();
}

void CDevice::CloseDevice()
{
	m_bThreadAutoConnect = false;

	WaitForSingleObject(m_hThreadAutoConnect, 1000);

	CloseHandle(m_hThreadAutoConnect);
	m_hThreadAutoConnect = NULL;
}

void CDevice::Start_ICS_AutoConnect()
{
	m_bThreadAutoConnect = true;

	stThreadParam* pParam = new stThreadParam;
	pParam->pOwner = this;
	pParam->nPort = 0;
	pParam->nArg = 0;

	m_hThreadAutoConnect = HANDLE(_beginthreadex(NULL, 0, Thread_ICS_AutoConnect, pParam, 0, NULL));
}

UINT CDevice::Thread_ICS_AutoConnect(LPVOID lpParam)
{
	CDevice*	pThis = (CDevice*)((stThreadParam*)lpParam)->pOwner;
	UINT		nIdx = ((stThreadParam*)lpParam)->nArg;
	UINT		Port = ((stThreadParam*)lpParam)->nPort;

	if (NULL != lpParam)
		delete lpParam;

	while (1)
	{
		pThis->ICS_Connect();
		Sleep(100);
	}
	return 0;
}

void CDevice::Connectz()
{
// 	InitDevice();
// 
// 	RFID_Connect();
	if (LIGHT_Distortion_Connect())
		LIGHT_Distortion_On();


// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		CL500A_COLORCAL_Connect();
// 		LIGHT_Color_Cal_Connect();
// 		break;
// 	case SYS_DISTORTION:
// 		if (LIGHT_Distortion_Connect())
// 			LIGHT_Distortion_On();
// 		break;
// 	case SYS_GHOSTFLARE:
// 		break;
// 	case SYS_HOTPIXEL:
// 		CL200A_HOTPIXEL_Connect();
// 		break;
// 	case SYS_SFR_NARROW:
// 		if (LIGHT_SFR_Narrow_Connect())
// 			LIGHT_SFR_Narrow_On();
// 		break;
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		if(LIGHT_SFR_Wide_Connect())
// 			LIGHT_SFR_Wide_On();
// 		break;
// 	}
	
//	ICS_Connect();

	Sleep(500);
	POWERSUPPLY_VoltOn(InspectionPortType::Left);
	Sleep(500);
	POWERSUPPLY_VoltOn(InspectionPortType::Right);
}

void CDevice::Disconnectz()
{
	RFID_Disconnect();
	LIGHT_Distortion_Off();
	LIGHT_Distortion_Disconnect();
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		LIGHT_Color_Cal_Off();
// 		CL500A_COLORCAL_Disconnect();
// 		LIGHT_Color_Cal_Disconnect();
// 		break;
// 	case SYS_DISTORTION:
// 		LIGHT_Distortion_Off();
// 		LIGHT_Distortion_Disconnect();
// 		break;
// 	case SYS_GHOSTFLARE:
// 		break;
// 	case SYS_HOTPIXEL:
// 		CL200A_HOTPIXEL_Disconnect();
// 		break;
// 	case SYS_SFR_NARROW:
// 		LIGHT_SFR_Narrow_Off();
// 		LIGHT_SFR_Narrow_Disconnect();
// 		break;
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		LIGHT_SFR_Wide_Off();
// 		LIGHT_SFR_Wide_Disconnect();
// 		break;
// 	}

	ICS_Disconnect();

	POWERSUPPLY_VoltOff(InspectionPortType::Left);
	Sleep(300);
	POWERSUPPLY_VoltOff(InspectionPortType::Right);
}

bool CDevice::IsConnected_SEMCO(__in InspectionPortType port)
{
	return m_UdpTester[(int)port].m_bConnected;
}

bool CDevice::IsConnected_PowerSupply()
{
	return m_ctrlLight_ODA_Q[0].Is_Connected();
}

bool CDevice::ICS_Connect()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (ICS_COMM->IsConnected())
		return true;

	// ICS 연결
	ICS_COMM->Connect(stTester.strTesterConnect[3], stTester.nTesterPort[3]);

	if (ICS_COMM->IsConnected())
	{
		Sleep(200);
		ICS_COMM->SendEquipmentId(g_szInsptrSysType[SET_INSPECTOR]);
		ICS_SendEquipmentStatus(EquipmentStatus::None, 0, _T(""));
	}

	return ICS_COMM->IsConnected();
}

void CDevice::ICS_Disconnect()
{
	ICS_COMM->Disconnect();
}

bool CDevice::ICS_IsUse()
{
#ifdef ICS_Use
	return true;
#else
	return false;
#endif
}

bool CDevice::ICS_SendOperationMode(__in OperationMode mode)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendOperationMode(mode))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendSocketRequest(__in LPCTSTR rfid)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendSocketRequest(rfid))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendTestResult(__in LPCTSTR rfid, __in INT32 resultCode, __in InspectionPortType port)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendTestResult(rfid, resultCode, port))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendPortClear(__in PortType type)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendPortClear(type))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendEquipmentStatus(__in EquipmentStatus status, __in INT32 alarmCode, __in LPCTSTR alarmReason)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendEquipmentStatus(status, alarmCode, alarmReason))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendPortStatus(__in PortType type, __in PortStatus status, __in LPCTSTR rfid, __in LPCTSTR productSn)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendPortStatus(type, status, rfid, productSn))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendPortStatusRequest(__in PortType type, __in PortStatus & status, __in CString & rfid, __in CString & productSn)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendPortStatusRequest(type, status, rfid, productSn))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::ICS_SendConveyorStatus(__in ConveyorType type, __in ConveyorStatus1 status1, __in ConveyorStatus2 status2, __in LPCTSTR rfid, __in LPCTSTR productSn)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendConveyorStatus(type, status1, status2, rfid, productSn))
		{
			bRet = true;
			break;
		}
	}
	return true;
#else
	return true;
#endif
}

bool CDevice::ICS_SendConveyorStatusRequest(__in ConveyorType type, __in ConveyorStatus1 & status1, __in ConveyorStatus2 & status2, __in CString & rfid, __in CString & productSn)
{
#ifdef ICS_Use
	bool bRet = false;

	for (int i = 0; i < 3; i++)
	{
		if (ICS_COMM->SendConveyorStatusRequest(type, status1, status2, rfid, productSn))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
#else
	return true;
#endif
}

bool CDevice::SEMCO_SendBarcode(__in InspectionPortType port, __in CString szBCR)
{
	bool bRet = true;

	if (IsConnected_SEMCO(port) == false)
		return false;

	m_UdpTester[(int)port].Send_Barcode(szBCR);
		
	return bRet;
}

bool CDevice::SEMCO_WaitBarcodeAck(__in InspectionPortType port, __in UINT nWaitTime, __out CString& szBCR)
{
	bool bRet = false;

	if (IsConnected_SEMCO(port) == false)
		return false;

	lt::CBasicTimer timer;
	bool bAck = false;
	int nAckErrorCode = 0;

	do
	{
		bool bRet = false;
		bAck = m_UdpTester[(int)port].m_bAckFlag_Barcode;

		if (bAck)
		{
			szBCR = m_UdpTester[(int)port].m_strTesterQuery_Barcode;
			bRet = true;
			break;
		}

		Sleep(10);
	} while (timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) < nWaitTime);
	return true;
}

bool CDevice::SEMCO_SendStartTest(__in InspectionPortType port, __in int nTestIdex)
{
	bool bRet = false;

// 	if (IsConnected_SEMCO(port) == false)
// 		return false;
	if (nTestIdex == eComMode_Insp_TLA)
	{
		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_TLA, nTestIdex);
	}
	else if (nTestIdex == eComMode_Insp_Vigneting46)
	{
		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_Vigneting46, nTestIdex);
	}
	else if (nTestIdex == eComMode_Insp_Vigneting82)
	{
		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_Vigneting82, nTestIdex);
	}
	if (bRet == true)
	{
		m_UdpTester[(int)port].m_bConnected = TRUE;
	}
	
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_ColorCal);
// 		break;
// 
// 	case SYS_DISTORTION:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_Distortion, nTestIdex);
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 		bRet = m_UdpTester[(int)port].Send_Flare_CLL_Communication(0, nTestIdex);
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 		m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_Vigneting);
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Insp_SFR);
// 		break;
// 	}

	return bRet;
}

bool CDevice::SEMCO_WaitStartTestAck(__in InspectionPortType port, __in int nTestIdx, __in UINT nWaitTime, __out UINT& nRetCode)
{
	bool bRet = false;

	if (IsConnected_SEMCO(port) == false)
		return false;

	lt::CBasicTimer timer;

	bool bAck = false;
	int nAckErrorCode = 0;
	UINT nWaitTimelong = 0;

	do
	{
	
		if (nTestIdx == eComMode_Insp_TLA)
		{
			bAck = m_UdpTester[(int)port].m_bAckFlag_Inspection_TLA;
			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_TLAErrorCode;
		}
		else if (nTestIdx == eComMode_Insp_TLA_Test)
		{
			bAck = m_UdpTester[(int)port].m_bAckFlag_Inspection_TLA;
			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_TLAErrorCode;

			
		}
		else if (nTestIdx == eComMode_Insp_Vigneting46)
		{
			bAck = m_UdpTester[(int)port].m_bAckFlag_Vigneting46;
			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_Vigneting46ErrorCode;
		}
		else if (nTestIdx == eComMode_Insp_Vigneting82)
		{
			bAck = m_UdpTester[(int)port].m_bAckFlag_Vigneting82;
			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_Vigneting82ErrorCode;
		}

// 		auto EqpType = SET_INSPECTOR;
// 
// 		switch (EqpType)
// 		{
// 		case SYS_COLOR_CAL:
// 			bAck = m_UdpTester[(int)port].m_bAckFlag_ColorCal;
// 			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_ColorCalErrorCode;
// 			break;
// 
// 		case SYS_DISTORTION:
// 			bAck = m_UdpTester[(int)port].m_bAckFlag_Distortion[nTestIdx];
// 			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_DistortionErrorCode[nTestIdx];
// 			break;
// 
// 		case SYS_GHOSTFLARE:
// 			bAck = m_UdpTester[(int)port].m_bAckFlag_Inspection_Flare[nTestIdx];
// 			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_FlareErrorCode[nTestIdx];
// 			break;
// 
// 		case SYS_HOTPIXEL:
// 			bAck = m_UdpTester[(int)port].m_bAckFlag_HotPixel;
// 			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_HotPixelErrorCode;
// 			break;
// 
// 		case SYS_SFR_NARROW:
// 		case SYS_SFR_WIDE_H:
// 		case SYS_SFR_WIDE_V:
// 			bAck = m_UdpTester[(int)port].m_bAckFlag_Inspection_SFR;
// 			nAckErrorCode = m_UdpTester[(int)port].m_nTesterFlag_Inspection_SFRErrorCode;
// 			break;
//		}

		if (bAck)
		{
			nRetCode = nAckErrorCode;
			bRet = true;
			break;
		}
// 		if (SEQUENCE->GetRunMode() == (RUN_MODE)eStop)
// 		{
// 			break;
// 		}
		Sleep(10);
		nWaitTimelong = timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds);
	} while (nWaitTimelong < nWaitTime);

	return bRet;
}

bool CDevice::SEMCO_SendReadyStart(__in InspectionPortType port)
{
	bool bRet = true;

// 	if (IsConnected_SEMCO(port) == false)
// 		return false;
	bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Ready);
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Ver2_Ready);
// 		break;
// 
// 	default:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Ready);
// 		break;
// 	}
	if (bRet == true)
	{
		m_UdpTester[(int)port].m_bConnected = TRUE;
	}
	return bRet;
}

bool CDevice::SEMCO_WaitReadyStartAck(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode)
{
	bool bRet = false;

	if (IsConnected_SEMCO(port) == false)
		return false;

	lt::CBasicTimer timer;

	do
	{
		if (m_UdpTester[(int)port].m_bAckFlag_Ready)
		{
			nRetCode = m_UdpTester[(int)port].m_nTesterFlag_ReadyErrorCode;
			bRet = true;
			break;
		}

		Sleep(10);
	} while (timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) < nWaitTime);

	return bRet;
}

bool CDevice::SEMCO_SendPreviewStart(__in InspectionPortType port)
{
// 	if (IsConnected_SEMCO(port) == false)
// 		return false;

	bool bRet = true;
	bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_PreviewStart);
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Ver2_PreviewStart);
// 		break;
// 
// 	default:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_PreviewStart);
// 		break;
// 	}
	if (bRet == true)
	{
		m_UdpTester[(int)port].m_bConnected = TRUE;
	}
	return bRet;
}

bool CDevice::SEMCO_WaitPreviewStartAck(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode)
{
	if (IsConnected_SEMCO(port) == false)
		return false;

	bool bRet = false;

	lt::CBasicTimer timer;

	do
	{
		if (m_UdpTester[(int)port].m_bAckFlag_PreviewStart)
		{
			nRetCode = m_UdpTester[(int)port].m_nTesterFlag_PreviewStartErrorCode;
			bRet = true;
			break;
		}
		
		Sleep(10);
	} while (timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) < nWaitTime);

	return bRet;
}

bool CDevice::SEMCO_SendPreviewStop(__in InspectionPortType port)
{
	if (IsConnected_SEMCO(port) == false)
		return false;

	bool bRet = true;
	bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_PreviewStop);
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_Ver2_PreviewStop);
// 		break;
// 
// 	default:
// 		bRet = m_UdpTester[(int)port].Send_Tester_Communication(eComMode_PreviewStop);
// 		break;
// 	}
	return bRet;
}

bool CDevice::SEMCO_WaitPreviewStopAck(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode)
{
	if (IsConnected_SEMCO(port) == false)
		return false;

	bool bRet = false;

	lt::CBasicTimer timer;

	do
	{
		if (m_UdpTester[(int)port].m_bAckFlag_PreviewStop)
		{
			nRetCode = m_UdpTester[(int)port].m_nTesterFlag_PreviewStopErrorCode;
			bRet = true;
			break;
		}

		Sleep(10);
	} while (timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) < nWaitTime);

	return bRet;
}

bool CDevice::SEMCO_SendPowerOff(__in InspectionPortType port)
{
	BOOL bRet = false;
	return bRet;
}

bool CDevice::SEMCO_Reset(__in InspectionPortType port)
{
	BOOL bRet = true;
	m_UdpTester[(int)port].Reset_TesterFlag();
	return bRet;
}

bool CDevice::PowerSupply_Connect()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_ctrlLight_ODA_Q[0].m_hWnd == NULL)
	{
		m_ctrlLight_ODA_Q[0].Create();
		m_ctrlLight_ODA_Q[0].Invalidate(0);
	}

	int nPort = stTester.nPort[0];
	int nBaudRate = stTester.nBaudRate[0];
	int nDataBit = stTester.nDataBit[0];
	int nStopBit = stTester.nStopBit[0];
	int nParityBit = stTester.nParityBit[0];

	CString strLog = _T("");
	if (m_ctrlLight_ODA_Q[0].OpenPort(nPort + 1, nBaudRate, WM_LIGHT_CONTROL_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] Power Supply Connect Success."));
		WRITE_MAIN_LOG(strLog);
		return true;
	}
	else
	{
		strLog.Format(_T("[Progress] Power Supply Connect Fail."));
		WRITE_MAIN_LOG(strLog);
		return false;
	}
}

bool CDevice::PowerSupply_Disconnect()
{
	m_ctrlLight_ODA_Q[0].ClosePort();
	return true;
}

BOOL CDevice::POWERSUPPLY_VoltOn(__in InspectionPortType port)
{
	BOOL bRet = TRUE;

	CString strLog;
	strLog.Format(_T("Power Supply On Channel: %d"), static_cast<int>(port));
	COMMON->Write_Main_Log(strLog);

	switch (port)
	{
	case InspectionPortType::Left:
		m_ctrlLight_ODA_Q[0].Write_Channel();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Volt_12V();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Current_1A();
		break;

	case InspectionPortType::Right:
		m_ctrlLight_ODA_Q[0].Write_Channel_2();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Volt_12V();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Current_1A();
		break;

	default:
		break;
	}	

	float volt, current;
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Volt(volt);
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Current(current);

	strLog.Format(_T("Power Supply On Measure Volt: %f, Current: %f"), volt, current);
	COMMON->Write_Main_Log(strLog);

	return bRet;
}

BOOL CDevice::POWERSUPPLY_VoltOff(__in InspectionPortType port)
{
	BOOL bRet = TRUE;

	CString strLog;
	strLog.Format(_T("Power Supply Off Channel: %d"), static_cast<int>(port));
	COMMON->Write_Main_Log(strLog);

	switch (port)
	{
	case InspectionPortType::Left:
		//통신신호보냄
		m_ctrlLight_ODA_Q[0].Write_Channel();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Volt_0V();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Current_0A();
		break;

	case InspectionPortType::Right:
		//통신신호보냄
		m_ctrlLight_ODA_Q[0].Write_Channel_2();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Volt_0V();
		COMMON->DoEvent(50);
		m_ctrlLight_ODA_Q[0].Write_Current_0A();
		break;

	default:
		break;
	}

	float volt, current;
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Volt(volt);
	COMMON->DoEvent(50);
	m_ctrlLight_ODA_Q[0].Measure_Current(current);

	strLog.Format(_T("Power Supply Off Measure Volt: %f, Current: %f"), volt, current);
	COMMON->Write_Main_Log(strLog);

	return bRet;
}

BOOL CDevice::POWERSUPPLY_MeasureCurrent(__in InspectionPortType port, __out float & value)
{
	switch (port)
	{
	case InspectionPortType::Left:
		//?듭??몃낫?
		m_ctrlLight_ODA_Q[0].Write_Channel();
		COMMON->DoEvent(50);
		return m_ctrlLight_ODA_Q[0].Measure_Current(value);

	case InspectionPortType::Right:
		//?듭??몃낫?
		m_ctrlLight_ODA_Q[0].Write_Channel_2();
		COMMON->DoEvent(50);
		return m_ctrlLight_ODA_Q[0].Measure_Current(value);

	default:
		break;
	}
	return FALSE;
}

bool CDevice::LIGHT_On()
{
	bool bRet = false;
	bRet = LIGHT_Distortion_On();
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = LIGHT_Color_Cal_On();
// 		break;
// 
// 	case SYS_DISTORTION:
// 		bRet = LIGHT_Distortion_On();
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 		
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 		bRet = LIGHT_SFR_Narrow_On();
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		bRet = LIGHT_SFR_Wide_On();
// 		break;
// 
// 	default:
// 		break;
// 	}

	return bRet;
}

bool CDevice::LIGHT_Off()
{
	bool bRet = false;
	bRet = LIGHT_Distortion_Off();
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		bRet = LIGHT_Color_Cal_Off();
// 		break;
// 
// 	case SYS_DISTORTION:
// 		bRet = LIGHT_Distortion_Off();
// 		break;
// 
// 	case SYS_GHOSTFLARE:
// 
// 		break;
// 
// 	case SYS_HOTPIXEL:
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 		bRet = LIGHT_SFR_Narrow_Off();
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 		bRet = LIGHT_SFR_Wide_Off();
// 		break;
// 
// 	default:
// 		break;
// 	}

	return bRet;
}

bool CDevice::LIGHT_Color_Cal_Connect()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_ctrlLightController_Youth[0].m_hWnd == NULL)
	{
		m_ctrlLightController_Youth[0].Create();
		m_ctrlLightController_Youth[0].InitVariables(0);
	}

	int nPort = stTester.nPort[1];
	int nBaudRate = stTester.nBaudRate[1];
	int nDataBit = stTester.nDataBit[1];
	int nStopBit = stTester.nStopBit[1];
	int nParityBit = stTester.nParityBit[1];

	CString strLog = _T("");
	if (m_ctrlLightController_Youth[0].OpenPort(nPort + 1, nBaudRate, WM_LIGHT_CONTROL_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] Color Cal Light 0 Connect Success."));
		WRITE_MAIN_LOG(strLog);
		return true;
	}
	else
	{
		strLog.Format(_T("[Progress] Color Cal Light 0 Connect Fail."));
		WRITE_MAIN_LOG(strLog);
		return false;
	}
}

bool CDevice::LIGHT_Color_Cal_Disconnect()
{
	m_ctrlLightController_Youth[0].ClosePort();
	return true;
}

bool CDevice::LIGHT_Color_Cal_On()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	BOOL bResult = TRUE;

	bResult &= m_ctrlLightController_Youth[0].Send_Light_Command(0, YOUTHTECH_CMD_ONOFF_SELECT, 1); // Channel 1 ON
	bResult &= m_ctrlLightController_Youth[0].Send_Light_Command(1, YOUTHTECH_CMD_ONOFF_SELECT, 1); // Channel 2 ON
	
	return bResult == TRUE;
}

bool CDevice::LIGHT_Color_Cal_On(int channel)
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	BOOL bResult = m_ctrlLightController_Youth[0].Send_Light_Command(channel, YOUTHTECH_CMD_ONOFF_SELECT, 1); // ON

	return bResult == TRUE;
}

bool CDevice::LIGHT_Color_Cal_Off()
{
	BOOL bResult = TRUE;

	bResult = m_ctrlLightController_Youth[0].Send_Light_Command(0, YOUTHTECH_CMD_ONOFF_SELECT, 0); // Channel 1 OFF
	bResult &= m_ctrlLightController_Youth[0].Send_Light_Command(1, YOUTHTECH_CMD_ONOFF_SELECT, 0); // Channel 2 OFF
	
	return bResult == TRUE;
}

bool CDevice::LIGHT_Color_Cal_Off(int channel)
{
	return m_ctrlLightController_Youth[0].Send_Light_Command(channel, YOUTHTECH_CMD_ONOFF_SELECT, 0) == TRUE; // Channel 1 OFF
}

bool CDevice::LIGHT_Distortion_Connect()
{
	LIGHT_Distortion_Connect(0);
	LIGHT_Distortion_Connect(1);
	//LIGHT_Distortion_Connect(2);

	return true;
}

bool CDevice::LIGHT_Distortion_Connect(int index)
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_ctrlLightController_Youth_DPS[index].m_hWnd == NULL)
	{
		m_ctrlLightController_Youth_DPS[index].Create(index);
		m_ctrlLightController_Youth_DPS[index].InitVariables(index);
	}

	int nPort = stTester.nPort[index + 1];
	int nBaudRate = stTester.nBaudRate[index + 1];
	int nDataBit = stTester.nDataBit[index + 1];
	int nStopBit = stTester.nStopBit[index + 1];
	int nParityBit = stTester.nParityBit[index + 1];

	CString strLog = _T("");
	if (m_ctrlLightController_Youth_DPS[index].OpenPort(nPort + 1, nBaudRate, WM_LIGHT_CONTROL_MSG + index, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] Distortion Light %d Connect Success."), index);
		WRITE_MAIN_LOG(strLog);
	}
	else
	{
		strLog.Format(_T("[Progress] Distortion Light %d Connect Fail."), index);
		WRITE_MAIN_LOG(strLog);
		return false;
	}
	return true;
}

bool CDevice::LIGHT_Distortion_Disconnect()
{
	LIGHT_Distortion_Disconnect(0);
	LIGHT_Distortion_Disconnect(1);
	//LIGHT_Distortion_Disconnect(2);

	return true;
}

bool CDevice::LIGHT_Distortion_Disconnect(int index)
{
	m_ctrlLightController_Youth_DPS[index].ClosePort();

	return true;
}

bool CDevice::LIGHT_Distortion_On()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	bool bResult = true;

	bResult = m_ctrlLightController_Youth_DPS[0].LightOn(stTester.nLightValue[0]) == TRUE;
	bResult &= m_ctrlLightController_Youth_DPS[1].LightOn(stTester.nLightValue[1]) == TRUE;
//	bResult &= m_ctrlLightController_Youth_DPS[2].LightOn(stTester.nLightValue[2]) == TRUE;

	return bResult;
}

bool CDevice::LIGHT_Distortion_On(int index)
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	return m_ctrlLightController_Youth_DPS[index].LightOn(stTester.nLightValue[index]) == TRUE;
}

bool CDevice::LIGHT_Distortion_Off()
{
	bool bResult;

	bResult = m_ctrlLightController_Youth_DPS[0].LightOff() == TRUE;
	bResult &= m_ctrlLightController_Youth_DPS[1].LightOff() == TRUE;
	bResult &= m_ctrlLightController_Youth_DPS[2].LightOff() == TRUE;
	
	return bResult;
}

bool CDevice::LIGHT_Distortion_Off(int index)
{
	return m_ctrlLightController_Youth_DPS[index].LightOff() == TRUE;
}

bool CDevice::LIGHT_SFR_Narrow_Connect()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_ctrlLightController_Youth_DPS[0].m_hWnd == NULL)
	{
		m_ctrlLightController_Youth_DPS[0].Create(0);
		m_ctrlLightController_Youth_DPS[0].InitVariables(0);
	}

	int nPort = stTester.nPort[1];
	int nBaudRate = stTester.nBaudRate[1];
	int nDataBit = stTester.nDataBit[1];
	int nStopBit = stTester.nStopBit[1];
	int nParityBit = stTester.nParityBit[1];

	CString strLog = _T("");
	if (m_ctrlLightController_Youth_DPS[0].OpenPort(nPort + 1, nBaudRate, WM_LIGHT_CONTROL_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] SFR Narrow Light 0 Connect Success."));
		WRITE_MAIN_LOG(strLog);
		return true;
	}
	else
	{
		strLog.Format(_T("[Progress] SFR Narrow Light 0 Connect Fail."));
		WRITE_MAIN_LOG(strLog);
		return false;
	}
}

bool CDevice::LIGHT_SFR_Narrow_Disconnect()
{
	m_ctrlLightController_Youth_DPS[0].ClosePort();
	return true;
}

bool CDevice::LIGHT_SFR_Narrow_On()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	return m_ctrlLightController_Youth_DPS[0].LightOn(stTester.nLightValue[0]) == TRUE;
}

bool CDevice::LIGHT_SFR_Narrow_Off()
{
	return m_ctrlLightController_Youth_DPS[0].LightOff() == TRUE;
}

bool CDevice::LIGHT_SFR_Wide_Connect()
{
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_ctrlLightController_FN1500[0].m_hWnd == NULL)
	{
		m_ctrlLightController_FN1500[0].Create();
		m_ctrlLightController_FN1500[0].InitVariables(0);
	}

	int nPort = stTester.nPort[1];
	int nBaudRate = stTester.nBaudRate[1];
	int nDataBit = stTester.nDataBit[1];
	int nStopBit = stTester.nStopBit[1];
	int nParityBit = stTester.nParityBit[1];

	CString strLog = _T("");
	if (m_ctrlLightController_FN1500[0].OpenPort(nPort + 1, nBaudRate, WM_LIGHT_CONTROL_MSG, nParityBit, nDataBit, nStopBit, 0))
	{
		strLog.Format(_T("[Progress] SFR Wide Light 0 Connect Success."));
		WRITE_MAIN_LOG(strLog);
		return true;
	}
	else
	{
		strLog.Format(_T("[Progress] SFR Wide Light 0 Connect Fail."));
		WRITE_MAIN_LOG(strLog);
		return false;
	}
}

bool CDevice::LIGHT_SFR_Wide_Disconnect()
{
	m_ctrlLightController_FN1500[0].ClosePort();
	return true;
}

bool CDevice::LIGHT_SFR_Wide_On()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int channel = 0; channel < 9; channel++)
	{
		m_ctrlLightController_FN1500[0].Set_FN1500_C(channel + 1, stTester.nLightValue[channel]);
		Sleep(100);
	}

	return m_ctrlLightController_FN1500[0].Set_FN1500_All_On() == TRUE;
}

bool CDevice::LIGHT_SFR_Wide_On(int channel)
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	m_ctrlLightController_FN1500[0].Set_FN1500_C(channel, stTester.nLightValue[channel - 1]);

	Sleep(100);

	return m_ctrlLightController_FN1500[0].Set_FN1500_Channel_On(channel) == TRUE;
}

bool CDevice::LIGHT_SFR_Wide_Off()
{
	return m_ctrlLightController_FN1500[0].Set_FN1500_All_Off() == TRUE;
}

bool CDevice::LIGHT_SFR_Wide_Off(int channel)
{
	return m_ctrlLightController_FN1500[0].Set_FN1500_Channel_Off(channel) == TRUE;
}

bool CDevice::CL200A_HOTPIXEL_Connect()
{
#if(SET_INSPECTOR == SYS_HOTPIXEL)
	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

// 	if (CL200A_COMM->IsOwnerWndNull())
// 	{
// 		auto mainWndHwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
// 		CL200A_COMM->SetOwnerWnd(mainWndHwnd);
// 	}

	static constexpr auto CL200A_INDEX = 6;
	int nPort = stTester.nPort[CL200A_INDEX];
	int nBaudRate = stTester.nBaudRate[CL200A_INDEX];
	int nDataBit = stTester.nDataBit[CL200A_INDEX];
	int nStopBit = stTester.nStopBit[CL200A_INDEX];
	int nParityBit = stTester.nParityBit[CL200A_INDEX];

	nBaudRate = _ttoi(BaudList[nBaudRate]);
	nDataBit = _ttoi(DataBitList[nDataBit]);
	nStopBit = _ttoi(StopBitList[nStopBit]);

	WRITE_COMM_LOG(_T("CL-200A try connect"));

// 	if (!CL200A_COMM->Connect(nPort + 1, nBaudRate, nParityBit, nStopBit, nDataBit))
// 	{
// 		WRITE_ERR_LOG(_T("CL-200A connection failed"));
// 		return false;
// 	}

	WRITE_COMM_LOG(_T("CL-200A try PC connect mode on"));

// 	if (!CL200A_COMM->SetPCConnectMode())
// 	{
// 		WRITE_ERR_LOG(_T("CL-200A PC connect mode failed"));
// 		return false;
// 	}

	WRITE_COMM_LOG(_T("CL-200A try to set measure mode"));

// 	CHROMA_PACKET pk;
// 	if (!CL200A_COMM->SetMeasureMode(NUMBER_0, pk))
// 	{
// 		WRITE_ERR_LOG(_T("CL-200A set measure mode failed"));
// 		return false;
// 	}
#endif
	return true;
}

bool CDevice::CL200A_HOTPIXEL_Disconnect()
{
// #if(SET_INSPECTOR == SYS_HOTPIXEL)
// 	WRITE_COMM_LOG(_T("CL-200A try PC connect mode off"));
// 
// 	if (!CL200A_COMM->SetPCConnectMode(FALSE))
// 		WRITE_ERR_LOG(_T("CL-200A PC connect mode failed"));
// 
// 	WRITE_COMM_LOG(_T("CL-200A try disconnect"));
// 
// 	CL200A_COMM->Disconnect();
// #endif
	return true;
}

bool CDevice::CL200A_HOTPIXEL_ReadData(float & data)
{
// #if(SET_INSPECTOR == SYS_HOTPIXEL)
// 	CHROMA_PACKET pk = {};
// 	if (!CL200A_COMM->GetMeasureData(NUMBER_0, CT_Evxy, pk))
// 	{
// 		WRITE_ERR_LOG(_T("CL-200A measurement failed"));
// 		return false;
// 	}
// 
// 	data = atof(pk.cData3);
// 	CString strLog;
// 	strLog.Format(_T("CL-200A measurement succeeded. Lux Data: %.4f"), data);
// 	WRITE_COMM_LOG(strLog);
// #endif
	return true;
}

bool CDevice::CL500A_COLORCAL_Connect()
{
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	return CL500A_COMM->Connect();
// 
// 	//if (!CL500A_COMM->ZeroCalibration())
// 	//	return;
// #endif
	return true;
}

bool CDevice::CL500A_COLORCAL_Disconnect()
{
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	CL500A_COMM->Disconnect();
// #endif
	return true;
}

bool CDevice::CL500A_COLORCAL_ReadData(float & Ev, float & Tcp)
{
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	if (!CL500A_COMM->Measurement(Ev, Tcp))
// 	{
// 		WRITE_ERR_LOG(_T("CL-500A measurement failed"));
// 		return false;
// 	}
// #endif
	return true;
}

bool CDevice::RFID_Connect()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	auto & ipAddress = stTester.dwTesterIP[2];
	auto & port = stTester.nTesterPort[2];

	auto ip0 = FIRST_IPADDRESS(ipAddress);
	auto ip1 = SECOND_IPADDRESS(ipAddress);
	auto ip2 = THIRD_IPADDRESS(ipAddress);
	auto ip3 = FOURTH_IPADDRESS(ipAddress);

	return COMMON->RFID_Connect(0, ip0, ip1, ip2, ip3, port) == TRUE;
}

bool CDevice::RFID_Disconnect()
{
	return COMMON->RFID_Disconnect(0) == TRUE;
}

bool CDevice::RFID_Read(CString & strData)
{
	if (!COMMON->RFID_GetRead(0))
		return false;

	strData = COMMON->GetRfid_ReadData();

	return true;
}
