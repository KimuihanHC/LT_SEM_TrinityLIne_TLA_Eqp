#pragma once

#include "Def_CompileOption.h"
#include "Def_Sequence.h"
#include "Def_Structure.h"

class CIO
{
public:
	CIO();
	~CIO();

private:
	void DoEvents					();
	void DoEvents					(DWORD dwMiliSeconds);

	bool WaitStatus					(__in int OnPort, __in int OffPort);
	bool WaitStatus					(__in int Port, __in bool Status);

public:
	bool IO_I_WaitStatus			(__in int idx, __in bool bStatus, __in DWORD dwMilisecond);
	bool IO_I_EMO					();
	bool IO_I_PowerOff				();
	bool IO_I_INIT					();
	bool IO_I_MainAirPress			();
	bool IO_I_AREA();
	
	bool IO_I_SleepMode				();
	void IO_O_SleepMode				(__in bool bOn);

	void IO_O_FlareLightOn			(__in int idx);
	void IO_O_FlareLightOff			(__in int idx);
	void IO_O_FlareLightAllOff		();

// 	bool IO_I_CV1_IF_FrontIn		();
// 	bool IO_I_CVR_IF_FrontOut		();
// 	bool IO_I_CV2_IF_RearIn			();
// 	bool IO_I_CVR_IF_RearOut		();
// 
// 	void IO_O_CV1_IF_FrontIn		(__in bool bOn);
// 	void IO_O_CVR_IF_FrontOut		(__in bool bOn);
// 	void IO_O_CV2_IF_RearIn			(__in bool bOn);
// 	void IO_O_CVR_IF_RearOut		(__in bool bOn);
// 
// 	bool IO_I_CV1_BufLiftCheck();
// 
// 	bool IO_I_CV1_Sensor			(__in enConv1SensorSelect sensor);
// 	void IO_O_CV1_MoveMode			(__in enConv1SensorSelect sensor);
// 
// 	bool IO_I_CV2_Loading1_Check1	();
// 	bool IO_I_CV2_Loading1_Check2	();
// 	bool IO_I_CV2_Loading2_Check3	();
// 	bool IO_I_CV2_Loading2_Check4	();
// 	bool IO_I_CV2_LoadingCheck		(__in InspectionPortType port);
// 
// 	bool IO_I_CV2_LoadingReverseCheck(__in InspectionPortType port);
// 	bool IO_I_CV2_Sensor			(__in enConv2SensorSelect sensor);
// 	void IO_O_CV2_MoveMode			(__in enConv2SensorSelect sensor);

// 	bool IO_I_Picker_SocketCheck	(__in InspectionPortType port);
// 	bool IO_I_Picker_LoadReverse	(__in InspectionPortType port);
// 	bool IO_I_Picker_VaccumOn		(__in InspectionPortType port);

// 	bool IO_I_Picker_CL500A_JigCheck		(__in InspectionPortType port);
// 	bool IO_I_Picker_CL500A_JigErrorCheck	(__in InspectionPortType port);
// 
// 	enMCErrCode IO_Picker_CL500A_Empty_Check		();
// 
// 	void IO_O_Picker_LoadVaccum		(__in InspectionPortType port, __in BOOL bOn);
// 	void IO_O_Picker_LoadBlow		(__in InspectionPortType port, __in BOOL bOn);
// 
// 	//bool IO_I_CVR_Sensor			(__in enConvRetSensorSelect sensor);
// 	void IO_O_CVR_MoveMode			(__in enConvRetSensorSelect sensor);
// 
// 	bool IO_CV1_BufCylUp			();
// 	bool IO_CV1_BufCylDn			();
// 	bool IO_CV1_BufClampCylOn			();
// 	bool IO_CV1_BufClampCylOff			();
// 
// 	bool IO_Picker_LoadClampCylOn	(__in InspectionPortType port);
// 	bool IO_PIcker_LoadClampCylOff	(__in InspectionPortType port);
// 	bool IO_Picker_LoadProbeChange	(__in InspectionPortType port, __in enSocketType socket);
// 	bool IO_Picker_LoadProbeUp		(__in InspectionPortType port);
// 	bool IO_Picker_LoadProbeDn		(__in InspectionPortType port);
// 	bool IO_Picker_LoadSocketUp		(__in InspectionPortType port);
// 	bool IO_Picker_LoadSocketDn		(__in InspectionPortType port);
	
	enMCErrCode IO_Door_All_Lock	(__in bool bLock);
	bool IO_Door_Front1_Lock		(__in bool bLock);
	bool IO_Door_Front2_Lock		(__in bool bLock);
	bool IO_Door_Rear1_Lock			(__in bool bLock);
	bool IO_Door_Rear2_Lock			(__in bool bLock);

	bool IO_Light_ShutterOpen		();
	bool IO_Light_ShutterClose		();
	bool IO_O_Light_ShutterOpen		();
	bool IO_O_Light_ShutterClose	();
	bool IO_I_Light_ShutterOpen		();
	bool IO_I_Light_ShutterClose	();

	bool IO_O_JIG_L_Lock(BOOL bChk = TRUE);
	bool IO_O_JIG_L_Unlock(BOOL bChk = TRUE);
	bool IO_I_JIG_L_Lock();
	bool IO_I_JIG_L_Unlock();

	bool IO_O_JIG_R_Lock(BOOL bChk = TRUE);
	bool IO_O_JIG_R_Unlock(BOOL bChk = TRUE);
	bool IO_I_JIG_R_Lock();
	bool IO_I_JIG_R_Unlock();


	bool IO_O_Socket_Lock(BOOL bChk = TRUE);
	bool IO_O_Socket_Unlock(BOOL bChk = TRUE);
	bool IO_I_Socket_Lock();
	bool IO_I_Socket_Unlock();

	bool IO_O_Chuck_Fwd(BOOL bChk = TRUE);
	bool IO_O_Chuck_Bwd(BOOL bChk = TRUE);
	bool IO_I_Chuck_Fwd();
	bool IO_I_Chuck_Bwd();


	bool IO_O_Screw_Bolt_Lock_1(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_Lock_2(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_Unlock(BOOL bChk = TRUE);


	bool IO_O_Screw_Bolt_L_Unlock(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_L_Lock_1(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_L_Lock_2(BOOL bChk = TRUE);
		
	bool IO_O_Screw_Bolt_R_Unlock(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_R_Lock_1(BOOL bChk = TRUE);
	bool IO_O_Screw_Bolt_R_Lock_2(BOOL bChk = TRUE);
		


	bool Set_Cyl_IO_Lock(int O_IO1, int O_IO2, int I_IO1, int I_IO2,  DWORD dwMilisecond, BOOL bChk = TRUE);
	bool Set_Cyl_IO_Unock(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk = TRUE);
	bool Set_Cyl_IO(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk = TRUE);
	bool Set_Cyl_Screw_Bolt_IO(int O_IO1, int O_IO2, int I_IO1, int I_IO2, DWORD dwMilisecond, BOOL bChk = TRUE);

	bool Set_Cyl_Screw_L_Bolt_IO(int O_IO1, int I_IO1, DWORD dwMilisecond, BOOL bChk = TRUE);
	bool Set_Cyl_Screw_R_Bolt_IO(int O_IO1, int I_IO1, DWORD dwMilisecond, BOOL bChk = TRUE);
	
		


	bool Check_Cyl_IO(int I_IO1, int I_IO2);
	bool Check_Cyl_Screw_Bolt_IO(int I_IO1, int I_IO2);
	
	bool Check_Cyl_Screw_L_Bolt_IO(int I_IO1);
	bool Check_Cyl_Screw_R_Bolt_IO(int I_IO1);

	

	bool Check_Cyl_IO_Lock(int I_IO1, int I_IO2);
	bool Check_Cyl_IO_Unlock(int I_IO1, int I_IO2);
	

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	bool IO_LightTurnOn				(__in int nIdx);
// 	bool IO_LightTurnOff			(__in int nIdx);
// 	bool IO_LightTurnAllOff			();
// #endif
};

