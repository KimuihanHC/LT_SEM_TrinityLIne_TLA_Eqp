#pragma once

#include "IcsDef.h"

typedef enum
{
	Ui_Conv1_Input = 0,
	Ui_Conv1_Slow,
	Ui_Conv1_Stop,
	Ui_Conv1_Buffer,
}UI_Pos_Conv1;

typedef enum
{
	Ui_Conv2_Slow1 = 0,
	Ui_Conv2_Stop1,
	Ui_Conv2_Slow2,
	Ui_Conv2_Stop2,
}UI_Pos_Conv2;

typedef enum
{
	Ui_ConvReturn_Input = 0,
	Ui_ConvReturn_Slow,
	Ui_ConvReturn_Stop	
}UI_Pos_ConvReturn;

typedef enum 
{
	Ui_Picker_Up = 0,
	Ui_Picker_Down,
	Ui_Picker_For,
	Ui_Picker_Back,
}UI_Pos_Picker;

typedef enum 
{
	Ui_Insp_Clamping = 0,
	Ui_Insp_MoveWait,
	Ui_Insp_Testing,
	Ui_Insp_UnloadWait,
	Ui_Insp_UnloadReq,
	Ui_Insp_SocketOut,
}UI_Pos_Inspection;

typedef enum
{
	Ui_Conv2_Status_ReservCon1 = 0,
	Ui_Conv2_Status_ReservPick,
	Ui_Conv2_Status_WaitWork,
}UI_Pos_Conv2Status;

typedef enum
{
	Lamp_Off = 0,
	Lamp_On,
}UI_Lamp;

class CUpdateUI
{
public:
	CUpdateUI();
	~CUpdateUI();

// 	void UpdateUI_Conveyor1			(__in UI_Pos_Conv1 pos, __in UI_Lamp OnOff);
// 	void UpdateUI_Conveyor1_RFID	(__in CString szRFID);
// 	void UpdateUI_Conveyor1_BCR		(__in CString szBCR);
// 
// 	void UpdateUI_Conveyor2			(__in UI_Pos_Conv2 pos, __in UI_Lamp OnOff);
// 	void UpdateUI_ConveyorReturn	(__in UI_Pos_ConvReturn pos, __in UI_Lamp OnOff);
// 	void UpdateUI_Picker			(__in InspectionPortType port, __in UI_Pos_Picker pos, __in UI_Lamp OnOff);
// 	
// 	void UpdateUI_Inspection		(__in InspectionPortType port, __in UI_Pos_Inspection pos, __in UI_Lamp OnOff);
// 	void UpdateUI_Conv2Status		(__in UI_Pos_Conv2Status pos, __in UI_Lamp OnOff);
// 	void UpdateUI_Inspection_RFID	(__in CString szRFID);
// 	void UpdateUI_Inspection_BCR	(__in CString szBCR);

	void EqpTypeGetOption				();

	void UpdateUI_SemiAuto_Start		(__in InspectionPortType port, __in int nIdx);
	void UpdateUI_SemiAuto_End			(__in InspectionPortType port, __in int nIdx, __in bool bResultOk);

	void UpdateUI_StartUpCheck_Start	(__in InspectionPortType port, __in int nIdx);
	void UpdateUI_StartUpCheck_End		(__in InspectionPortType port, __in int nIdx, __in bool bResultOk);

	// hot pixel
	void UpdateUI_CL200A_Data			(__in InspectionPortType port, __in float fData, __in bool bReadOk);
	void UpdateUI_CL200A_Reset			(__in InspectionPortType port);


	// color cal
	void UpdateUI_CL500A_Data			(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk);
	void UpdateUI_CL500A_Reset			(__in InspectionPortType port);
};

