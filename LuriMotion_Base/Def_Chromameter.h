#ifndef Def_Chromameter_h__
#define Def_Chromameter_h__

#include <afxwin.h>

#define COMM_CR		0x0D	//End of charactor [CR]
#define COMM_LF		0x0A	//End of charactor [LF]
#define COMM_SOH	0x01
#define COMM_STX	0x02
#define COMM_ETX	0x03
#define COMM_EOT	0x04

// Command codes
#define COMMAND10_0			0x30
#define COMMAND10_4			0x34
#define COMMAND10_5			0x35
#define COMMAND10_XYZ		COMMAND10_0
#define COMMAND1_XYZ		0x31
#define COMMAND10_EVXY		COMMAND10_0
#define COMMAND1_EVXY		0x32
#define COMMAND10_EVUV		COMMAND10_0
#define COMMAND1_EVUV		0x33
#define COMMAND10_TEMP		COMMAND10_0
#define COMMAND1_TEMP		0x38
#define COMMAND10_EXT		COMMAND10_4
#define COMMAND1_EXT		0x30
#define COMMAND10_HEAD_TERM	COMMAND10_5
#define COMMAND1_HEAD_TERM	0x33
#define COMMAND10_PC_CONN	COMMAND10_5
#define COMMAND1_PC_CONN	0x34
#define COMMAND10_HOLD		COMMAND10_5
#define COMMAND1_HOLD		0x35

// EXT Mode response settings
#define RESPONSE_ON			0x30
#define RESPONSE_OFF		0x31

// Set head terminal number CF settings
#define CF_ON				0x31
#define CF_OFF				0x30

// PC Connect mode settings
#define PC_CONNECT_ON		0x31
#define PC_CONNECT_OFF		0x30

// Hold settings
#define HOLD_ON				0x31
#define HOLD_OFF			0x30

// Indexes
#define HEAD10_INDEX		1
#define HEAD1_INDEX			2

#define COMMAND10_INDEX		3
#define COMMAND1_INDEX		4

#define ERROR_CODE_INDEX			6

#define MEASUREMENT_HOLD_INDEX		5
#define MEASUREMENT_RNG_INDEX		7
#define MEASUREMENT_BATT_INDEX		8
#define MEASUREMENT_DATA1_INDEX		21
#define MEASUREMENT_DATA2_INDEX		15
#define MEASUREMENT_DATA3_INDEX		9
#define MEASUREMENT_BCC10_INDEX		28
#define MEASUREMENT_BCC1_INDEX		29
#define MEASUREMENT_DATA_LENGTH		6

#define HEAD_TERM_CF_INDEX			7
#define OTHER_BCC10_INDEX			10
#define OTHER_BCC1_INDEX			11

#define MINIMUM_DATA_LENGTH			12	// 12 charactors and over
#define MAXIMUM_DATA_LENGTH			32

typedef enum
{
	NUMBER_0 = 0x3030,
	NUMBER_1 = 0x3031,
	NUMBER_2 = 0x3032,
	NUMBER_3 = 0x3033,
	NUMBER_4 = 0x3034,
	NUMBER_5 = 0x3035,
	NUMBER_6 = 0x3036,
	NUMBER_7 = 0x3037,
	NUMBER_8 = 0x3038,
	NUMBER_9 = 0x3039,
	NUMBER_10 = 0x3130,
	NUMBER_11 = 0x3131,
	NUMBER_12 = 0x3132,
	NUMBER_13 = 0x3133,
	NUMBER_14 = 0x3134,
	NUMBER_15 = 0x3135,
	NUMBER_16 = 0x3136,
	NUMBER_17 = 0x3137,
	NUMBER_18 = 0x3138,
	NUMBER_19 = 0x3139,
	NUMBER_20 = 0x3230,
	NUMBER_21 = 0x3231,
	NUMBER_22 = 0x3232,
	NUMBER_23 = 0x3233,
	NUMBER_24 = 0x3234,
	NUMBER_25 = 0x3235,
	NUMBER_26 = 0x3236,
	NUMBER_27 = 0x3237,
	NUMBER_28 = 0x3238,
	NUMBER_29 = 0x3239,
	NUMBER_99 = 0x3939,
} eHeadNumber, eTerminalNumber;

typedef enum
{
	CT_XYZ			= 0x3031,
	CT_Evxy			= 0x3032,
	CT_Evuv			= 0x3033,
	CT_TEMP			= 0x3038,
	CT_EXT			= 0x3430,
	CT_HEAD_TERM	= 0x3533,
	CT_PC_CONN		= 0x3534,
	CT_HOLD			= 0x3535
} eCommandType;

typedef enum
{
	ERR_OK			= 0x20,	// No error
	ERR_POWERON		= 0x31,	// Indicates that power to the head has been cut off
	ERR_EEPROM		= 0x32,	// Indicates that an improper value has been written to the head's EE-PROM
	ERR_VALUE		= 0x33,	// Error in CCF value or ¥Ätarget value
	ERR_OUTRANGE	= 0x34,	// Setting out of range
	ERR_OVERRANGE	= 0x35,	// Indicates that the measurement results exceeded the measurement range
	ERR_UNDERRANGE	= 0x36,	// Brightness too low to ensure reliable result
	ERR_VALUERANGE	= 0x37	// The Tcp, ¥Äuv measurement is out of range
} eCL200AError;

typedef enum
{
	EXT_TURN_OFF			= 0x30,
	EXT_TURN_ON				= 0x31,
	EXT_START_MEASUREMENT	= 0x32
} eEXTMode;

typedef enum
{
	CM_NORMAL	= 0x30,
	CM_MULTI	= 0x31
} eCalibrationMode;

typedef struct
{
	eHeadNumber		eHead;
	eCommandType	eCommand;
	BOOL			bHoldOn;
	eCL200AError	eErr;
	BOOL			bBateriesLow;
	char			cData1[10];
	char			cData2[10];
	char			cData3[10];
	short			sBCC;

	union
	{
		BOOL	bRngChanging;
		BOOL	bCFOn;
	};
} CHROMA_PACKET;

#define COMM_TIME_OUT	3000

typedef enum
{
	M_ERR_COM_OWNER = -5,
	M_ERR_COM_TIMEOUT = -4,
	M_ERR_COM_NOTFOUND = -3,
	M_ERR_COM_NOTCONNECTED = -2,
	M_ERR_COM = -1,
	M_OK = 0
} COM_ERR;

#endif // Def_Chromameter_h__
