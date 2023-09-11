#pragma once

#include "IO.h"

#include "Def_CompileOption.h"
#include "Def_Sequence.h"
#include "Def_Structure.h"

class CMotion : public CIO
{
public:
	CMotion();
	~CMotion();

// 	enMCErrCode MO_IsConveyor1_Moving			();
// 	enMCErrCode MO_MoveConveyor1_Run			(__in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveConveyor1_Stop			();
// 	enMCErrCode MO_MoveConveyor1_Reset			();
// 
// 	enMCErrCode MO_IsConveyor2_Moving			();
// 	enMCErrCode MO_MoveConveyor2_Run			(__in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveConveyor2_Stop			();
// 	enMCErrCode MO_MoveConveyor2_Reset			();
// 
// 	enMCErrCode MO_IsConveyorRet_Moving			();
// 	enMCErrCode MO_MoveConveyorRet_Run			(__in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveConveyorRet_Stop			();
// 	enMCErrCode MO_MoveConveyorRet_Reset		();
// 
// 	enMCErrCode MO_Wait_To_Moving_End			(__in UINT nAxis, __in double dbPos);
// 
// 	enMCErrCode MO_CheckZ_To_SafetyPos			(__in InspectionPortType port);
// 	enMCErrCode MO_CheckZ_To_InspectionPos		(__in InspectionPortType port);
// 	enMCErrCode MO_CheckZ_To_ConveyorWaitPos	(__in InspectionPortType port);
// 	enMCErrCode MO_CheckX_To_LoadingPos			(__in InspectionPortType port);
// 	enMCErrCode MO_CheckX_To_InspectionPos		(__in InspectionPortType port);
// 
// 	enMCErrCode MO_MoveX_To_LoadPos				(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveX_To_InspectionPos		(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 
// 	enMCErrCode MO_MoveZ_To_GripUp				(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveZ_To_Inspection1			(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveZ_To_Inspection2			(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveZ_To_ConveyorWait		(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveZ_To_Safety				(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	enMCErrCode MO_MoveZ_To_180Inspection		(__in InspectionPortType port, __in enConveyorSpeed spd = High);	// distortion 설비만 해당
// 
// 	enMCErrCode MO_MoveZ_To_CL500A				(__in InspectionPortType port, __in enConveyorSpeed spd = High);
// 	double MO_MoveX_GetPosition					();
// 	double MO_MoveZ_GetPosition					(__in InspectionPortType port);
	// TLA 22.03.21
	enMCErrCode MO_MoveVignetting_X_Type(__in enVignetting_X Type);		//	Wait = 0, Test_01, Test_02
	enMCErrCode MO_MoveVignetting_R_Type(__in enVignetting_R Type);		//	Wait = 0, Test_01, Test_02
	enMCErrCode MO_MoveScrew_L_Type(__in enScrew Type);		//		Wait = 0, Commencement,	Work
	enMCErrCode MO_MoveScrew_R_Type(__in enScrew Type);		//		Wait = 0, Commencement,	Work
	enMCErrCode MO_MoveScrew_Type(__in enScrew Type);		//		Wait = 0, Commencement,	Work
	
	enMCErrCode MO_MoveWait_Type(__in enVignetting_X Type);

	enMCErrCode MO_MoveAdjust_Type(__in enAdjust Type);		//			Wait = 0, Work

	enMCErrCode MO_NonWait_MoveVignetting_X_Type(__in enVignetting_X Type);		//	Wait = 0, Test_01, Test_02
	enMCErrCode MO_NonWait_MoveVignetting_R_Type(__in enVignetting_R Type);		//	Wait = 0, Test_01, Test_02
	enMCErrCode MO_NonWait_MoveScrew_L_Type(__in enScrew Type);		//		Wait = 0, Commencement,	Work
	enMCErrCode MO_NonWait_MoveScrew_R_Type(__in enScrew Type);		//		Wait = 0, Commencement,	Work
	enMCErrCode MO_NonWait_MoveAdjust_Type(__in enAdjust Type);		//			Wait = 0, Work




	BOOL Check_Pos_Vignetting_X_Type(__in enVignetting_X Type);
	BOOL Check_Pos_Vignetting_R_Type(__in enVignetting_R Type);
	BOOL Check_Pos_Screw_L_Type(__in enScrew Type);
	BOOL Check_Pos_Screw_R_Type(__in enScrew Type);
	BOOL Check_Pos_Adjust_Type(__in enAdjust Type);

	


	// TLA 22.03.21
};

