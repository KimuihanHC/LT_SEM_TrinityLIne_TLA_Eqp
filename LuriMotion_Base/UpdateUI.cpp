#include "pch.h"
#include "UpdateUI.h"

#include "DlgMain.h"

CUpdateUI::CUpdateUI()
{
}

CUpdateUI::~CUpdateUI()
{
}

void CUpdateUI::EqpTypeGetOption()
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->EqpTypeGetOption();
}

//====================================================================================
// Method		: UpdateUI_Conveyor1
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/08 - 20:39
// Desc.	   	: 컨베이어 상태 업데이트 UI
//=====================================================================================
// void CUpdateUI::UpdateUI_Conveyor1()
// {
// 	// input 센서 감지 상태
// 
// 	// slow1 센서 감지 상태
// 
// 	// stop1 센서 감지 상태
// 
// 	// slow2 센서 감지 상태
// 
// 	// stop2 센서 감지 상태
// 
// 	// RFID 정보 표시
// 
// 	// 버퍼에 제품 loading 상태 표시
// 
// 	// 버퍼있는 
// 
// 	// 상태에 따라 UI led lamp 를 변경해주자
// 
// 	// case 1
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 1, 1);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 2, 1);
// 
// 	// case 2
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 1, 0);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 2, 0);
// }

// void CUpdateUI::UpdateUI_Conveyor1(__in UI_Pos_Conv1 pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIConveyor1(0, pos, OnOff);
// }
// 
// void CUpdateUI::UpdateUI_Conveyor1_RFID(__in CString szRFID)
// {
// }
// 
// void CUpdateUI::UpdateUI_Conveyor1_BCR(__in CString szBCR)
// {
// }

//====================================================================================
// Method		: UpdateUI_Conveyor2
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/09 - 15:20
// Desc.	   	:
//=====================================================================================
// void CUpdateUI::UpdateUI_Conveyor2()
// {
// 	// 상태에 따라 UI led lamp 를 변경해주자
// 
// 	// case 1
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 3, 1);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 4, 1);
// 
// 	// case 2
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 3, 0);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 4, 0);
// }

// void CUpdateUI::UpdateUI_Conveyor2(__in UI_Pos_Conv2 pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIConveyor2(0, pos, OnOff);
// }

//====================================================================================
// Method		: UpdateUI_ConveyorReturn
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update 	: 2021/12/09 - 15:20
// Desc.	   	:
//=====================================================================================
// void CUpdateUI::UpdateUI_ConveyorReturn()
// {
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 7, 1);
// 	
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 7, 0);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 8, 1);
// 
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 9, 1);
// 
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 8, 0);
// 	COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 9, 0);
// }

// void CUpdateUI::UpdateUI_ConveyorReturn(__in UI_Pos_ConvReturn pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIConveyorReturn(0, pos, OnOff);
// }

// void CUpdateUI::UpdateUI_Picker(__in InspectionPortType port)
// {
// 	switch (type)
// 	{
// 	case InspectionPortType::Left:
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_MATERIAL_POS, 1, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 0, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 0, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_DETECT_POS, 0, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_MATERIAL_POS, 1, 0);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 3, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 4, 0);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 4, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 5, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 4, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 5, 0);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 0, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 1, 0);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 1, 1);
// 
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 2, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_SOCKET_POS, 2, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_SOCKET_POS, 2, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 3, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 0, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 1, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 4, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_DETECT_POS, 0, 0);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 0, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 1, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 2, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 3, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_LEFT, 4, 0);
// 
// 		break;
// 
// 	case InspectionPortType::Right:
// 		
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_SOCKET_POS, 3, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 3, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_DETECT_POS, 3, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 4, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 2, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 3, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 0, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 1, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 2, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 3, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 4, 0);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_MATERIAL_POS, 2, 1);
// 
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 0, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 5, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CV_SENSOR, 6, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_MATERIAL_POS, 2, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_DETECT_POS, 1, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 6, 0);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 7, 1);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 6, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 7, 0);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 2, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_CYLINDER_STATE, 3, 0);
// 
// 		//
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_SOCKET_POS, 3, 1);
// 		COMMON->Call_Main_Flow_UI(MAIN_FLOW_TRAY_RIGHT, 2, 1);
// 		break;
// 
// 	default:
//		break;
// 	}	
// }

// void CUpdateUI::UpdateUI_Picker(__in InspectionPortType port, __in UI_Pos_Picker pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIPicker((int)port, pos, OnOff);
// }
// 
// void CUpdateUI::UpdateUI_Inspection(__in InspectionPortType port, __in UI_Pos_Inspection pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIInspection((int)port, pos, OnOff);
// }
// 
// void CUpdateUI::UpdateUI_Conv2Status(__in UI_Pos_Conv2Status pos, __in UI_Lamp OnOff)
// {
// 	COMMON->UpdateMainUIConv2Status(pos, OnOff);
// }
// 
// void CUpdateUI::UpdateUI_Inspection_RFID(__in CString szRFID)
// {
// 	CDlgMain *pMain = CDlgMain::Get_Instance();
// }
// 
// void CUpdateUI::UpdateUI_Inspection_BCR(__in CString szBCR)
// {
// 
// }

void CUpdateUI::UpdateUI_SemiAuto_Start(__in InspectionPortType port, __in int nIdx)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnStatusSemiAuto_Start(port, nIdx);
}

void CUpdateUI::UpdateUI_SemiAuto_End(__in InspectionPortType port, __in int nIdx, __in bool bResultOk)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnStatusSemiAuto_End(port, nIdx, bResultOk);
}

void CUpdateUI::UpdateUI_StartUpCheck_Start(__in InspectionPortType port, __in int nIdx)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnStatusStartupCheck_Start(port, nIdx);
}

void CUpdateUI::UpdateUI_StartUpCheck_End(__in InspectionPortType port,__in int nIdx, __in bool bResultOk)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnStatusStartupCheck_End(port,nIdx, bResultOk);
}

void CUpdateUI::UpdateUI_CL200A_Data(__in InspectionPortType port, __in float fData, __in bool bReadOk)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnUpdate_CL200A_Data(port, fData, bReadOk);
}

void CUpdateUI::UpdateUI_CL200A_Reset(__in InspectionPortType port)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnUpdate_CL200A_Reset(port);
}

void CUpdateUI::UpdateUI_CL500A_Data(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnUpdate_CL500A_Data(port, nLightIdx, fColorTemper, fData, bReadOk);
}

void CUpdateUI::UpdateUI_CL500A_Reset(__in InspectionPortType port)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->OnUpdate_CL500A_Reset(port);
}
