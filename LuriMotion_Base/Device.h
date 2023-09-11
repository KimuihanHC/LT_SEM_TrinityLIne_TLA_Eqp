#pragma once

//#include "User_Define.h"
#include "Def_Structure.h"
#include "Def_Sequence.h"
#include "IcsDef.h"

class CDevice
{
public:
	CDevice();
	~CDevice();

	void InitDevice();
	void CloseDevice();
	void Start_ICS_AutoConnect();
	static UINT	Thread_ICS_AutoConnect(LPVOID lpParam);

	void Connectz();
	void Disconnectz();

	bool IsConnected_SEMCO				(__in InspectionPortType port);
	bool IsConnected_PowerSupply		();

	// ICS 통신
	bool ICS_Connect					();
	void ICS_Disconnect					();
	bool ICS_IsUse						();
	bool ICS_SendOperationMode			(__in OperationMode mode);
	bool ICS_SendSocketRequest			(__in LPCTSTR rfid);
	bool ICS_SendTestResult				(__in LPCTSTR rfid, __in INT32 resultCode, __in InspectionPortType port);
	bool ICS_SendPortClear				(__in PortType type);
	bool ICS_SendEquipmentStatus		(__in EquipmentStatus status, __in INT32 alarmCode , __in LPCTSTR alarmReason);
	bool ICS_SendPortStatus				(__in PortType type, __in PortStatus status, __in LPCTSTR rfid, __in LPCTSTR productSn);
	bool ICS_SendPortStatusRequest		(__in PortType type, __in PortStatus & status, __in CString & rfid, __in CString & productSn);
	bool ICS_SendConveyorStatus			(__in ConveyorType type, __in ConveyorStatus1 status1, __in ConveyorStatus2 status2, __in LPCTSTR rfid, __in LPCTSTR productSn);
	bool ICS_SendConveyorStatusRequest	(__in ConveyorType type, __in ConveyorStatus1 & status1, __in ConveyorStatus2 & status2, __in CString & rfid, __in CString & productSn);

	// 공정 SW 와 통신 관련 구현부
	bool SEMCO_SendBarcode					(__in InspectionPortType port, __in CString szBCR);
	bool SEMCO_WaitBarcodeAck				(__in InspectionPortType port, __in UINT nWaitTime, __out CString& szBCR);

	bool SEMCO_SendStartTest				(__in InspectionPortType port, __in int nTestIdx = 0);
	bool SEMCO_WaitStartTestAck				(__in InspectionPortType port, __in int nTestIdx, __in UINT nWaitTime, __out UINT& nRetCode);

	bool SEMCO_SendReadyStart				(__in InspectionPortType port);
	bool SEMCO_WaitReadyStartAck			(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode);

	bool SEMCO_SendPreviewStart				(__in InspectionPortType port);
	bool SEMCO_WaitPreviewStartAck			(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode);

	bool SEMCO_SendPreviewStop				(__in InspectionPortType port);
	bool SEMCO_WaitPreviewStopAck			(__in InspectionPortType port, __in UINT nWaitTime, __out UINT& nRetCode);

	bool SEMCO_SendPowerOff					(__in InspectionPortType port);
	bool SEMCO_Reset						(__in InspectionPortType port);
	
	// 파워서플라이
	bool PowerSupply_Connect				();
	bool PowerSupply_Disconnect				();
	BOOL POWERSUPPLY_VoltOn					(__in InspectionPortType port);
	BOOL POWERSUPPLY_VoltOff				(__in InspectionPortType port);
	BOOL POWERSUPPLY_MeasureCurrent			(__in InspectionPortType port, __out float & value);

	bool LIGHT_On();
	bool LIGHT_Off();

	// Color Cal 광원
	bool LIGHT_Color_Cal_Connect			();
	bool LIGHT_Color_Cal_Disconnect			();
	bool LIGHT_Color_Cal_On					();
	bool LIGHT_Color_Cal_On					(int channel);
	bool LIGHT_Color_Cal_Off				();
	bool LIGHT_Color_Cal_Off				(int channel);

	// Distortion 광원
	bool LIGHT_Distortion_Connect			();
	bool LIGHT_Distortion_Connect			(int index);
	bool LIGHT_Distortion_Disconnect		();
	bool LIGHT_Distortion_Disconnect		(int index);
	bool LIGHT_Distortion_On				();
	bool LIGHT_Distortion_On				(int index);
	bool LIGHT_Distortion_Off				();
	bool LIGHT_Distortion_Off				(int index);

	// SFR 협각 광원 
	bool LIGHT_SFR_Narrow_Connect			();
	bool LIGHT_SFR_Narrow_Disconnect		();
	bool LIGHT_SFR_Narrow_On				();
	bool LIGHT_SFR_Narrow_Off				();

	// SFR 광각 H, V 광원 
	bool LIGHT_SFR_Wide_Connect				();
	bool LIGHT_SFR_Wide_Disconnect			();
	bool LIGHT_SFR_Wide_On					();
	bool LIGHT_SFR_Wide_On					(int channel);
	bool LIGHT_SFR_Wide_Off					();
	bool LIGHT_SFR_Wide_Off					(int channel);

	// HOTPIXEL 조도계
	bool CL200A_HOTPIXEL_Connect			();
	bool CL200A_HOTPIXEL_Disconnect			();
	bool CL200A_HOTPIXEL_ReadData			(float & data);

	// COLOR CAL 조도계
	bool CL500A_COLORCAL_Connect			();
	bool CL500A_COLORCAL_Disconnect			();
	bool CL500A_COLORCAL_ReadData			(float & Ev, float & Tcp);

	// RFID
	bool RFID_Connect						();
	bool RFID_Disconnect					();
	bool RFID_Read							(CString & strData);

private:
	bool m_bThreadAutoConnect;
	HANDLE m_hThreadAutoConnect;
};

