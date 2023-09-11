#pragma once

#include <afxwin.h>
#include "IcsDef.h"

// [21.1110.1]
#define	SYS_TLA_ASSEMBLE		0
#define	SYS_TLA_OQC				1
#define	SYS_TLA_BACKUP			2
#define	SYS_TLA_B		3

typedef enum enInsptrSysType
{
	Sys_TLA_Assemble	= SYS_TLA_ASSEMBLE,		
	Sys_TLA_Oqc			= SYS_TLA_OQC,			
	Sys_TLA_Backup		= SYS_TLA_BACKUP,

}enInsptrSysType;

static LPCTSTR g_szInsptrSysType[] =
{
	_T("TLA Adjust"),
	_T("TLA OQC"),
	_T("TLA Backup"),
	NULL
};
// TLA 22.03.21
typedef enum
{
	X_Wait = 0,
	X_Test_01,
	X_Test_02
}enVignetting_X;
typedef enum
{
	R_Wait = 0,
	R_Test_01,
	R_Test_02
}enVignetting_R;

typedef enum
{
	Screw_Wait = 0,
	Screw_Commencement,
	Screw_Work
}enScrew;
typedef enum
{
	Adjust_Wait = 0,
	Adjust_Work
}enAdjust;

// TLA 22.03.21
typedef enum 
{
	Angle_46 = 0,
	Angle_82_5,
	Angle_180
}enAngleType;

typedef enum
{
	Socket_H = 0,
	Socket_V,
}enSocketType;

typedef enum
{
	Status_Ready = 0,
	Status_Testing,
	Status_Complete,
	Status_Error,
}enSeqStatus;

typedef enum
{
	Low = 0,
	Middle,
	High
}enConveyorSpeed;

typedef enum
{
	Cv1_Input = 0,
	Cv1_Stop_RFID,
	Cv1_Slow,
	Cv1_Stop,
	Cv1_Out,
	Cv1_clear,
}enConv1SensorSelect;

typedef enum
{
	Cv2_Input = 0,
	Cv2_Slow1,
	Cv2_Stop1,
	Cv2_Slow2,
	Cv2_Stop2,
	Cv2_Out,
	Cv2_clear,
}enConv2SensorSelect;

typedef enum
{
	Cvr_Input = 0,
	Cvr_Slow,
	Cvr_Stop,
	Cvr_clear,
}enConvRetSensorSelect;

// enum class SocketPos
// {
// 	Cv1_Rfid ,
// 	Cv1_Buffer,
// 	Cv2_L_loading,
// 	Cv2_R_loading,
// 	Cv2_Bypass,
// 	Pic_L_Wait,
// 	Pic_L_Insp,
// 	Pic_R_Wait,
// 	Pic_R_Insp,
// 	Cvr_Bypass,
// 	Max,
// };


