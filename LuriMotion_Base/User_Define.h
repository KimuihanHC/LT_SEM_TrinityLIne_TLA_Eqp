#pragma once
#include <vector>
#include "Def_Enum.h"

//==========================================================================
// Socket 통신용
typedef BYTE			_u8;
typedef WORD			_u16;
typedef DWORD			_u32;

struct tcp_keepalive {
	u_long  onoff;
	u_long  keepalivetime;
	u_long  keepaliveinterval;
};

// New WSAIoctl Options
#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11)
//=================================================================================

// Values for use with SIO_RCVALL* options
#define RCVALL_OFF				0
#define RCVALL_ON				1
#define RCVALL_SOCKETLEVELONLY	2
#define WAITLIST_COMM_MAX		6
// [21.1017.2]
#define MAX_USER_NUM			20
// [21.1021.1]
#define	MESSAGE_UI_WINDOW_TOP	(WM_USER + 100)
#define	MESSAGE_UI_STARTUP		(WM_USER + 101)

#define MN_NONE		0x0
#define MN_MAIN		0x1
#define MN_TEACHING	0x2
#define MN_DIO		0x4
#define MN_COMMSET	0x8
#define MN_LOG		0x10	
#define MN_HIDE		0x20
#define MN_EXIT		0x80
#define MN_ALL		0xFF

// user message
#define UM_RECV_SEMCO_LIGHT_CONTROL	(WM_USER + 200)

// MsgBox
#pragma pack(push, 1)
typedef struct _THREAD_PARAM
{
	CString strMsg;
	int nMode;
	bool bTesterAlarm;

} THREAD_PARAM;
#pragma pack(pop, 1)

enum SleepModeStatus
{
	SLEEP_OFF,
	SLEEP_ON,
	SLEEP_ENABLE,
	SLEEP_FINISH,
	SLEEP_SUCCESS,
};
enum enCV_CC
{
	Const_CV = 0,
	Const_CC,
};
enum enSwitchOnOff
{
	Switch_ON = 0,
	Switch_OFF,
};

enum enSlot
{
	Slot_All = 0,
	Slot_A,
	Slot_B,
	Slot_C,
	Slot_D,
	Slot_E,
	Slot_Max = Slot_E,

};
// [21.1017.1]
enum enPermisionLevel
{
	LVL_OPER = 0,
	LVL_MAINT,
	LVL_ADMIN
};
// [21.1027.1]
enum enMoveMode
{
	MOVE_VELOCITY = 0,
	MOVE_DISTANCE
};

enum EBITMAP_LIST
{
//	eBITMAP_MAIN = 0,
	eBITMAP_TEACH = 0,
//	eBITMAP_SETTING,
	eBITMAP_INIT,
//	eBITMAP_MESSAGE,
	eBITMAP_SOCKET_INFO,
//	eBITMAP_TR_CONV,
	eBITMAP_SOCKET,
	eBITMAP_COUNT
};

enum EICON_LIST
{
	eICON_OK = 0,
	eICON_CANCEL,
	eICON_CLEAR,
	eICON_SAVE,
	eICON_LED_ON,
	eICON_LED_OFF,
	eICON_RLED_GRN,
	eICON_RLED_RED,
	eICON_RLED_OFF,
	eICON_MORE,
	eICON_FEWER,
	eICON_COUNT
};

enum ESOCKET_POS_LIST
{
	eSPL_CV_INPUT = 0,
	eSPL_CV_BUFFER,
	eSPL_CV_INSPECTION,
	eSPL_CV_RETURN,
	eSPL_PICK_LEFT,
	eSPL_PICK_RIGHT,
	eSPL_MAX
};

#pragma pack(push, 1)
typedef struct __tagAxisData 
{
	double	dPos;
	int		nData[8];
}AXIS_DATA, *pAXIS_DATA;

typedef struct _MODULE_INFO
{
	UINT nResult;
	CString strModuleID;
	BOOL bExist;
	BOOL bPseudoExist;
	BOOL bSupplied;
	BOOL bEjected;

	_MODULE_INFO& operator= (_MODULE_INFO& ref)
	{
		nResult = ref.nResult;
		strModuleID = ref.strModuleID;

		bExist = ref.bExist;
		bPseudoExist = ref.bPseudoExist;
		bSupplied = ref.bSupplied;
		bEjected = ref.bEjected;
		return *this;
	};

	_MODULE_INFO()
	{
		Reset();
	}

	void Reset()
	{
		strModuleID = _T("");
		nResult = 0;
		bExist = FALSE;
		bPseudoExist = FALSE;
		bSupplied = FALSE;
		bEjected = FALSE;
	};

}MODULE_INFO;

typedef struct _SOCKET_INFO
{
	BOOL bExist;
	CString strSerialNo;
	CString strRFID;
	MODULE_INFO stModule;

	_SOCKET_INFO()
	{
		Reset();
	}
	_SOCKET_INFO& operator= (_SOCKET_INFO& ref)
	{
		bExist = ref.bExist;
		strSerialNo = ref.strSerialNo;
		strRFID = ref.strRFID;
		stModule = ref.stModule;
		return *this;
	};

	void Reset()
	{
		bExist = FALSE;
		strRFID = _T("");
		strSerialNo = _T("");
		stModule.Reset();
	};

}SOCKET_INFO;

typedef struct tagRunInfo
{
	BOOL	bError;
	time_t	nTimer;
	int		nAction;
	int		nParam;
	double	dParam;
	int		nStep;

	tagRunInfo()
	{
		bError = FALSE;
		nTimer = GetTickCount();
		nAction = 0;
		nParam = 0;
		dParam = 0;
		nStep = 0;
	};

}stRunInfo, *pstRunInfo;

typedef struct _LURI_LIGHT_RECV
{
	float fVolt[Slot_Max];
	float fCurrent[Slot_Max];

	void Reset()
	{
		for (UINT nIdx = 0; nIdx < Slot_Max; nIdx++)
		{
			fVolt[nIdx] = 0.0f;
			fCurrent[nIdx] = 0.0f;
		}
	};

}LURI_LIGHT_RECV;

typedef struct __tagSocketData 
{
	BOOL 		bExist;
	CString		str[3];
}SOCKET_DATA, *pSOCKET_DATA;

// [21.1017.3] Added
typedef struct _EQ_USER_DATA
{

	CString strUserID;
	CString strPassword;
	int		nPermission;

	_EQ_USER_DATA()
	{
		strUserID = _T("");
		strPassword = _T("");
		nPermission = 0;
	}

}EQ_USER_DATA;

// [21.1017.4] Added
typedef struct _EQ_LEVEL_DATA
{
	CString strNo;
	CString strLevel;
	CString strUserID;

	_EQ_LEVEL_DATA()
	{
		strNo = _T("");
		strLevel = _T("");
		strUserID = _T("");
	}

}EQ_LEVEL_DATA;

// [21.1017.5] Modified
typedef struct _EQ_BASIC_PARAM
{
	CString strLastRecipe;
	int nLanguage;
	int nDeviceIoVersion;

	EQ_USER_DATA stUserData[MAX_USER_NUM];

	_EQ_BASIC_PARAM()
	{
		strLastRecipe = _T("");
		nLanguage = 0;
		nDeviceIoVersion = 0;

		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			stUserData[i].strUserID = _T("");
			stUserData[i].strPassword = _T("");
			stUserData[i].nPermission = 0;
		}
	}

}EQ_BASIC_PARAM;

// TOWERLAMP // [21.1026.1]
enum ETwrMode { eModeRun = 0, eModeStop, eModeError, eModeReady, eModeCall, eModePause, eModeMax };
enum ETwrLamp { eTowerRed = 0, eTowerYellow, eTowerGreen, eTowerBuzzer, eTowerMax };
enum ETwrType { eTowerLamp_Off = 0, eTowerLamp_On, eTowerLamp_Blink, eTowerLamp_Max };

typedef struct _EQ_TOWERLAMP
{
	int nTowerLampSetting[eModeMax][eTowerMax];

	_EQ_TOWERLAMP()
	{
		for (int i = 0; i < (int)eModeMax; i++)
		{
			for (int j = 0; j < (int)eTowerMax; j++)
			{
				nTowerLampSetting[i][j] = (int)eTowerLamp_Off;
			}
		}
	}
}EQ_TOWERLAMP;

// Motor Speed
enum { eSpeedSlow, eSpeedMiddle, eSpeedHigh, eSpeedAcc, eSpeedDec, eSpeedMax };

typedef struct _MOTION_PARAM
{
	int nPort[2];								// Serial 2개
	int nBaudRate[2];							// Serial 2개

	int nMotorSpeed[10][eSpeedMax];			// 속도 저장용.

	CString	strChangeTime_New;				// 혹시 사용자가 저장버튼을 눌렀는지 확인하기 위해 생성 ( 저장 버튼을 누른 가장 최근 시간 )
	CString strChangeTime_Last;				// 혹시 사용자가 저장버튼을 눌렀을 경우 가장 마지막 저장된 시간
	CString strDiff[10][eSpeedMax];			// 저장 버튼을 눌렀을 경우 변경된 값이 있다면 일단 남겨두자. (가장 최근의 것만 가지고 있는다. )

	_MOTION_PARAM()
	{
		for (int i = 0; i < 2; i++)
		{
			nPort[i] = i + 1;
			nBaudRate[i] = 9600;
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < (int)eSpeedMax; j++)
			{
				strDiff[i][j] = _T("");

				if (i == eSpeedAcc || i == eSpeedDec)
					nMotorSpeed[i][j] = (int)100;
				else
					nMotorSpeed[i][j] = (int)1;
			}
		}

		strChangeTime_New = _T("");
		strChangeTime_Last = _T("");
	}
}MOTION_PARAM;

typedef struct _TEACHING_PARAM
{

	int nVignetting_X_Wait;
	int nVignetting_X_Test_01;
	int nVignetting_X_Test_02;

	int nVignetting_R_Wait;
	int nVignetting_R_Test_01;
	int nVignetting_R_Test_02;

	int nScrew_L_Wait;
	int nScrew_L_Commencement;
	int nScrew_L_Work;

	int nScrew_R_Wait;
	int nScrew_R_Commencement;
	int nScrew_R_Work;

	int nAdjust_Wait;
	int nAdjust_Work;
	

	int nAdjust_Spec;
	int nAdjust_Screw1_Spec;
	int nAdjust_Screw2_Spec;
	int nAdjust_RetryCnt;
	int nAdjust_Reverse;
	double dAdjust_Offset_Y;
	int nBarcode;


// 	int		nSocketX_Loading1_Inpsection_Pos;
// 	int		nSocketX_Loading2_Inpsection_Pos;
// 
// 	int		nSocketZ_Wait_Pos;
// 	int		nSocketZ_FlareInspection1_Pos;
// 	int		nSocketZ_Conveyor_Grap_Pos;
// 	int		nSocketZ_Conveyor_Wait_Pos;
// 	int		nSocketZ_FlareInspection2_Pos;
// 	int		nSocketZ_Reserved_Pos;		// Distortion 설비 : 180도 검사위치, ColorCal 설비 : 시업측정모드위치
// 
// 	int		nSocketZ2_Wait_Pos;
// 	int		nSocketZ2_FlareInspection1_Pos;
// 	int		nSocketZ2_Conveyor_Grap_Pos;
// 	int		nSocketZ2_Conveyor_Wait_Pos;
// 	int		nSocketZ2_FlareInspection2_Pos;
// 	int		nSocketZ2_Reserved_Pos;
// 		
// 	int		nChartZ_01_Pos;		// 46도 차트 위치
// 	int		nChartZ_02_Pos;		// 82.5도 차트 위치
// 	int		nChartZ_03_Pos;		// 180도 차트위치
// 
// 
// 	int		nSampleModelType;				// 1 : H Type , 2 : V : Type
// 
// 	CString strChangeTime_New_Pos;
// 	CString strChangeTime_Last_Pos;
	
	_TEACHING_PARAM()
	{
		nVignetting_X_Wait = 0;
		nVignetting_X_Test_01 = 0;
		nVignetting_X_Test_02 = 0;

		nVignetting_R_Wait = 0;
		nVignetting_R_Test_01 = 0;
		nVignetting_R_Test_02 = 0;

		nScrew_L_Wait = 0;
		nScrew_L_Commencement = 0;
		nScrew_L_Work = 0;

		nScrew_R_Wait = 0;
		nScrew_R_Commencement = 0;
		nScrew_R_Work = 0;

		nAdjust_Wait = 0;
		nAdjust_Work = 0;
		nAdjust_Spec = 0;
		nAdjust_Screw1_Spec = 0;
		nAdjust_Screw2_Spec = 0;
		nAdjust_RetryCnt = 0;
		nBarcode = 1;
		dAdjust_Offset_Y = 0.0;
// 		strChangeTime_New_Pos = _T("");
// 		strChangeTime_Last_Pos = _T("");
// 
// 		nSocketX_Loading1_Inpsection_Pos = 0;
// 		nSocketX_Loading2_Inpsection_Pos = 0;
// 
// 		nSocketZ_Wait_Pos = 0;
// 		nSocketZ_FlareInspection1_Pos = 0;
// 		nSocketZ_Conveyor_Grap_Pos = 0;
// 		nSocketZ_Conveyor_Wait_Pos = 0;
// 		nSocketZ_FlareInspection2_Pos = 0;
// 		nSocketZ_Reserved_Pos = 0;
// 
// 		nSocketZ2_Wait_Pos = 0;
// 		nSocketZ2_FlareInspection1_Pos = 0;
// 		nSocketZ2_Conveyor_Grap_Pos = 0;
// 		nSocketZ2_Conveyor_Wait_Pos = 0;
// 		nSocketZ2_FlareInspection2_Pos = 0;
// 		nSocketZ2_Reserved_Pos = 0;
// 
// 		nChartZ_01_Pos = 0;
// 		nChartZ_02_Pos = 0;
// 		nChartZ_03_Pos = 0;
// 		nSampleModelType = 0;
 	}

}TEACHING_PARAM;

typedef struct _tagSemiAuto
{
	enAngleType		Angle;
	enSocketType	Socket;

	_tagSemiAuto()
	{
		Angle = Angle_46;
		Socket = Socket_H;
	};

}ST_OptSemiAuto;

typedef struct _TESTER_COMM
{
	CString strSystemFolderPath;			// 시스템 폴더 경로
	CString strEQPName;						// 설비 이름

	CString strTesterConnect[4];			// Single 5개 연결
	int nTesterPort[4];						// Single 5개 연결
	DWORD dwTesterIP[4];				
	CString	 m_strWaitTimeLIst[WAITLIST_COMM_MAX];
	int nSleepModeTimer;

	int nUseMode[4];							// 사용 유무 저장용
	int nServerMode[4];							// 서버 모드 사용 유무

	int nPort[7];								// Serial
	int nBaudRate[7];							// Serial
	int nDataBit[7];							// Serial
	int nStopBit[7];							// Serial
	int nParityBit[7];							// Serial

	// 설비별 광원 Value 개수
	// COLOR CAL		20
	// DISTORTION		3
	// GHOSTFLARE		0
	// HOTPIXEL			0
	// SFR_NARROW		1
	// SFR_WIDE_H		9
	// SFR_WIDE_V		9
	int nLightValue[20];

	int nColorCal_SemcoLight[4];

	_TESTER_COMM()
	{
		strSystemFolderPath = _T("C:\\Exec_Trinity");
		strEQPName = g_szInsptrSysType[SET_INSPECTOR];

		for (int i = 0; i < WAITLIST_COMM_MAX; i++)
			m_strWaitTimeLIst[i] = _T("");
		nSleepModeTimer = 0;
		for (int i = 0; i < 4; i++)
		{
			strTesterConnect[i] = _T("");
			nTesterPort[i] = 5000 + i;
			dwTesterIP[i] = 0;

			nUseMode[i] = 0;
			nServerMode[i] = 0;
		}

		for (int i = 0; i < 7; i++)
		{
			nPort[i] = i + 1;
			nBaudRate[i] = 9600;
			nDataBit[i] = 8;
			nStopBit[i] = 0;
			nParityBit[i] = 1;
		}

		for (int i = 0; i < 20; i++)
		{
			nLightValue[i] = 0;
		}

		for (int i = 0; i < 4; i++)
		{
			nColorCal_SemcoLight[i] = 0;
		}
	}
}TESTER_COMM;

typedef struct _ERROR_LIST
{
	int nErrorCount;
	std::vector<CString> vtErrorNum;

	int nErrorNameCount_1000;
	int nErrorNameCount_2000;
	int nErrorNameCount_3000;
	std::vector<CString> vtErr_Kor;
	std::vector<CString> vtErr_Eng;
	std::vector<CString> vtErr_Viet;

	int nSolutionNameCount_1000;
	int nSolutionNameCount_2000;
	int nSolutionNameCount_3000;
	std::vector<CString> vtSol_Kor;
	std::vector<CString> vtSol_Eng;
	std::vector<CString> vtSol_Viet;

	_ERROR_LIST()
	{
		nErrorCount = 0;
		vtErrorNum.clear();

		nErrorNameCount_1000 = 0;
		nErrorNameCount_2000 = 0;
		nErrorNameCount_3000 = 0;

		vtErr_Kor.clear();
		vtErr_Eng.clear();
		vtErr_Viet.clear();

		nSolutionNameCount_1000 = 0;
		nSolutionNameCount_2000 = 0;
		nSolutionNameCount_3000 = 0;

		vtSol_Kor.clear();
		vtSol_Eng.clear();
		vtSol_Viet.clear();
	}
}ERROR_LIST;
#pragma pack(pop, 1)


//-------------------------------------------------------------------------
// LuriMotion Dlg 

static LPCTSTR m_szBottomMenuKor[] =
{
	_T("메인"),
	_T("메인터런스"),
	_T("IO"),
	_T("설비셋팅"),
	_T("로그"),
	_T("숨기기"),
	_T("숨기기"),
	_T("종료"),
};

static LPCTSTR m_szBottomMenuEng[] =
{
	_T("Main"),
	_T("Maintenance"),
	_T("DIO"),
	_T("EQUIPMENT"),
	_T("Log"),
	_T("HIDE"),
	_T("HIDE"),
	_T("EXIT"),
};

// [21.1018.7] Modyfied
static LPCTSTR m_szSideMenuKor[] =
{
	_T("오토모드"),
	_T("\n\r \n\r 매뉴얼모드\n\r 정지 모드"),
	_T("세미오토모드"),
	_T("시업점검"), 
	_T("로토모드"),
	_T("드라이런"),
	_T("강제 배출 모드"),
	_T("장비 초기화"),
};

static LPCTSTR m_szSideMenuEng[] =
{
	_T("AUTO MODE"),
	_T("\n\r \n\r MANUAL MODE \n\r STOP MODE"),
	_T("SEMI AUTO MODE"),
	_T("STARTUP CHECK"),
	_T("LOTO MODE"),
	_T("DRYRYUN MODE"),
	_T("Output Mode"),
	_T("INITIAL"),
};

static LPCTSTR m_szTitleMenuKor[] =
{
	_T("언어 설정"),
	_T("권한 설정"),
	_T("타워 램프"),	
	_T("Tool 4"),
};

static LPCTSTR m_szTitleMenuEng[] =
{
	_T("Language"),
	_T("Permission"),
	_T("TowerLamp"),	
	_T("Tool 4"),
};

// LuriMotion Dlg End
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Main Dlg 
static LPCTSTR m_szMainWork_Kor[] =
{
	_T("고정"),
	_T("파워"),
	_T("프리뷰"),
	_T("조정"),
	_T("비네팅"),
	_T("레디/언로딩"),
};
static LPCTSTR m_szMainWork_Eng[] =
{
	_T("LOCK/LOADING"),
	_T("POWER"),
	_T("PREVIEW"),
	_T("ADJUST"),
	_T("VIGNETTING"),
	_T("READY/UNLOADING"),

};


static LPCTSTR m_szMainMotor_Kor[] =
{
	_T("왼쪽 스크류 대기 위치"),
	_T("왼쪽 스크류 시업점검 위치"),
	_T("왼쪽 스크류 작업 위치"),
	_T("오른쪽 스크류 대기 위치"),
	_T("오른쪽 스크류 시업점검 위치"),
	_T("오른쪽 스크류 작업 위치"),

	_T("비네팅 대기 위치"),
	_T("비네팅 테스트 위치1"),
	_T("비네팅 테스트 위치2"),
};
static LPCTSTR m_szMainMotor_Eng[] =
{
	_T("Screw Left Wait"),
	_T("Screw Left Start Up"),
	_T("Screw Left Work"),
	_T("Screw Right Wait"),
	_T("Screw Right Start Up"),
	_T("Screw Right Work"),
	_T("Vignetting Wait"),
	_T("Vignetting Test1"),
	_T("Vignetting Test2"),

};


static LPCTSTR m_szMainSensor_Kor[] =
{
	_T("제품 왼쪽"),
	_T("제품 오른쪽"),

	_T("왼쪽 지그 안착부 전진"),
	_T("왼쪽 지그 안착부 후진"),
	_T("왼쪽 지그 감지"),
	_T("오른쪽 지그 안착부 전진"),
	_T("오른쪽 지그 안착부 후진"),
	_T("오른쪽 지그 감지"),
	_T("조정부 전진"),
	_T("조정부 후진"),
};
static LPCTSTR m_szMainSensor_Eng[] =
{
	_T("PRODUCT L"),
	_T("PRODUCT R"),
	_T("JIG LEFT FWD"),
	_T("JIG LEFT BWD"),
	_T("JIG LEFT CHK"),
	_T("JIG RIGHT FWD"),
	_T("JIG RIGHT BWD"),
	_T("JIG RIGHT CHK"),

	_T("CHUCK FWD"),
	_T("CHUCK BWD"),



};





static LPCTSTR m_szMainSemiAuto_Kor[] =
{
	_T("(1) 대기위치 이동"),
	_T("(4) TLA 조정 Start"),
	
	_T("(3) Preview Start"),
	_T("(6) 완료 이동"),
	_T("(2) 제품 고정"),
	_T("(5) Vigneting Start"),

// 	_T("(1) 대기위치 이동"),
// 	_T("(2) 제품 고정"),
// 	_T("(5) Vigneting Start"),
// 	_T("(6) 완료 이동"),
// 	_T("(3) Preview Start"),
// 	_T("(4) TLA 조정 Start"),

};
static LPCTSTR m_szMainSemiAuto_Eng[] =
{
	_T("(1) Loading"),
	_T("(4) TLA Adjust Start"),
	_T("(3) Preview Start"),
	_T("(6) Complet Unloading"),
	_T("(2) Product lock"),
	_T("(5) Vigneting Start"),
};




static LPCTSTR m_szMainStartupCheck_Kor[] =
{ 
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) 
	_T("(1) 대기위치 이동"),
	_T("(1) 대기위치 이동"),
	_T("(3) Left Screw 02 시업점검 이동"),
	_T("(3) Right Screw 02 시업점검 이동"),
	_T("(2) Left Screw 01 그립위치 이동"),
	_T("(2) Right Screw 01 그립위치 이동"),

	_T("(4) 제품 고정, 시작위치"),
	_T("(5) Preview Start"),
	_T("(6) TLA 측정 Start"),
	_T("(7)Vigneting"),
	_T("(8) 대기 위치"),
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	_T("(1) 제품 고정, 시작위치"),
	_T("(2) Preview Start"),
	_T("(3) TLA 측정 Start"),
	_T("(4)Vigneting"),
	_T("(5) 대기 위치"),
#else
		_T("(1) 대기위치 이동"),
	_T("(1) 대기위치 이동"),
	_T("(3) Left Screw 02 시업점검 이동"),
	_T("(3) Right Screw 02 시업점검 이동"),
	_T("(2) Left Screw 01 그립위치 이동"),
	_T("(2) Right Screw 01 그립위치 이동"),

	_T("(4) 제품 고정, 시작위치"),
	_T("(5) Preview Start"),
	_T("(6) TLA 측정 Start"),
	_T("(7)Vigneting"),
	_T("(8) 대기 위치"),
#endif


// 	_T("(7)Vigneting 46도"),
// 	_T("(8)Vigneting 82.5도"),
//	_T("(9) 대기 위치"),
	
};
static LPCTSTR m_szMainStartupCheck_Eng[] =
{ 


#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	_T("(1) Ready WaitPos Move"),
	_T("(1) Ready WaitPos Move"),
	_T("(3) Left Start Screw 02 Check"),
	_T("(3) Right Start Screw 02 Check"),
	_T("(2) Left Start Screw 01 Check"),
	_T("(2) Right Start Screw 01 Check"),

	_T("(4) Product lock, starting pos"),
	_T("(5) Preview Start"),
	_T("(6) TLA Measurement Start"),
	_T("(7) Vigneting"),
	_T("(8) Ready WaitPos Move"),

#elif(SET_INSPECTOR == SYS_TLA_OQC)
	_T("(1) Product lock, starting pos"),
	_T("(2) Preview Start"),
	_T("(3) TLA Measurement Start"),
	_T("(4) Vigneting"),
	_T("(5) Ready WaitPos Move"),
#else
	_T("(1) Ready WaitPos Move"),
	_T("(1) Ready WaitPos Move"),
	_T("(3) Left Start Screw 02 Check"),
	_T("(3) Right Start Screw 02 Check"),
	_T("(2) Left Start Screw 01 Check"),
	_T("(2) Right Start Screw 01 Check"),

	_T("(4) Product lock, starting pos"),
	_T("(5) Preview Start"),
	_T("(6) TLA Measurement Start"),
	_T("(7) Vigneting"),
	_T("(8) Ready WaitPos Move"),

	#endif

	//_T("(7) Vigneting 46 degrees"),
	//_T("(8) Vigneting 82.5 degrees"),
	//_T("(9) Ready WaitPos Move"),
};

static LPCTSTR m_szMainMotorSubTitle_Kor[] =
{
	_T("모터이름"),
	_T("현재위치"),
	_T("+ 리밋"),
	_T("- 리밋"),
	_T("에러"),
	_T("정지"),
	_T("홈"),
};
static LPCTSTR m_szMainMotorSubTitle_Eng[] =
{
	_T("Motor Name"),
	_T("Current Pos"),
	_T("+ Limit"),
	_T("- Limit"),
	_T("Error"),
	_T("InPosition"),
	_T("Home"),
};

static LPCTSTR m_szMainMotorName_Kor[] =
{
	_T("비네팅 X"),
	_T("비네팅 R"),
	_T("스크류 L"),
	_T("스크류 R"),
	_T("조정 R"),
};
static LPCTSTR m_szMainMotorName_Eng[] =
{
	_T("Vignetting X"),
	_T("Vignetting R"),
	_T("Screw L"),
	_T("Screw R"),
	_T("Adjust R"),
};

static LPCTSTR m_szAxisNameKor[] =
{
	_T("투입 컨베이어"),
	_T("검사 컨베이어"),
	_T("리턴 컨베이어"),
	_T("소켓 X축"),
	_T("소켓 LEFT Z축"),
	_T("소켓 RIGHT Z축"),
	_T("차트 Z축"),
};
//-------------------------------------------------------------------------
// Socket Info
static LPCTSTR m_szSocketPosNameEng[] =
{
	_T("CV INPUT"),
	_T("CV INSPECTION"),
	_T("CV OUTPUT"),
	_T("Socket X"),
	_T("Socket Left Z"),
	_T("Socket Right Z"),
	_T("Char Z"),
};

static LPCTSTR m_szMainCommName_Kor[] =
{
	_T("검사기"),
	_T("모션 컨트롤러"),
	_T("IO 컨트롤러"),
};
static LPCTSTR m_szMainCommName_Eng[] =
{
	_T("Tester"),
	_T("Motion Controller"),
	_T("IO Controller"),
};

static LPCTSTR m_szMainCommIO_Kor[] =
{
	_T("언로더 이동완료"),
	_T("샘플 이동완료"),
	_T(" "),

	_T("전장비응답완료"),
	_T("전장비해제"),
	_T(" "),
};
static LPCTSTR m_szMainCommIO_Eng[] =
{
	_T("Unlaoding Move Ok"),
	_T("Sample Move OK"),
	_T("-"),

	_T("Sample RCV OK"),
	_T("Move Disable"),
	_T("-"),
};
static LPCTSTR m_szMainLightCtrlBtn_Total[] =
{
	_T("0F"),
	_T("0.5F B"),
	_T("0.5F L"),
	_T("0.5F T"),
	_T("0.5F R"),
	_T("1F LL "),
	_T("1F UL"),
	_T("1F LR"),
	_T("1F UR"),
	_T("All On"),
	_T("All Off"),
};

static LPCTSTR m_szMainYield_Kor[] =
{
	_T("투입 수량"),
	_T("양품 수량"),
	_T("NG 수량"),
	_T("수율"),

	_T("리셋"),
};
static LPCTSTR m_szMainYield_Eng[] =
{
	_T("Input Count"),
	_T("OK Count"),
	_T("NG Count"),
	_T("Yield"),

	_T("Reset"),
};

static LPCTSTR m_szMainUI_LeftRightStep_Kor[] = 
{
	_T("소켓잡기"),
	_T("검사위치"),
	_T("검사중"),
	_T("배출위치"),
	_T("배출요청"),
	_T("소켓배출"),
};

static LPCTSTR m_szMainUI_LeftRightStep_Eng[] =
{
	_T("Clamping"),
	_T("Move Wait"),
	_T("Inspection"),
	_T("Unload Wait"),
	_T("Unload Request"),
	_T("Socket out"),
};
static LPCTSTR m_szMainUI_SocketPos_KorEng[] =
{
	_T("CV1 In"),
	_T("CV2 Left"),
	_T("CV2 Right"),
	_T("CV3 Out"),
	_T("CV1 Buf"),
};

static LPCTSTR m_szMainUI_ProbePin_KorEng[] =
{
	_T("UP"),
	_T("DOWN"),
	_T("FOR"),
	_T("BACK"),	
};

static LPCTSTR m_szMainUI_RFID_KorEng[] =
{
	_T("RFID"),
	_T("RFID"),
	_T("INSPECTION"),
	_T("INSPECTION"),
};

static LPCTSTR m_szMainUI_Conv2Status_Kor[] =
{
	_T("예약 CONV1"),
	_T("예약 PICKER"),
	_T("작업 대기중"),	
};

static LPCTSTR m_szMainUI_Conv2Status_Eng[] =
{
	_T("Reserv Con1"),
	_T("Reserv Picker"),
	_T("Wait Work"),	
};
// Main Dlg End
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Teaching Dlg

static LPCTSTR m_szTeachTitleKor[] = 
{
	_T("모터 현재 위치값"),
	_T("조그 이동 설정"),
	_T("실린더"),
	_T("티칭 포지션"),
	_T("모터 속도 설정"),
	_T("도어 인터락"),
	_T("파워서플라이"),
};

static LPCTSTR m_szTeachTitleEng[] =
{
	_T("Motor Curruent Pos"),
	_T("Jog Move"),
	_T("Cylinder"),
	_T("Position Teaching"),
	_T("Motor Speed Setting"),
	_T("Door InterLock"),
	_T("Power Supply")
};

static LPCTSTR m_szTeachCurPosAxisnNameKor[] =
{
	_T("1번축"),
	_T("2번축"),
	_T("3번축"),
	_T("4번축"),
	_T("5번축"),
	_T("6번축"),
	_T("7번축"),
	_T("8번축"),
	_T("-"),
	_T("-"),
};

static LPCTSTR m_szTeachCurPosAxisnNameEng[] =
{
	_T("Axis 1"),
	_T("Axis 2"),
	_T("Axis 3"),
	_T("Axis 4"),
	_T("Axis 5"),
	_T("Axis 6"),
	_T("Axis 7"),
	_T("Axis 8"),
	_T("-"),
	_T("-"),
};

static LPCTSTR m_szTeachJogKor[] =
{
	_T("모드 선택"),
	_T("축 선택"),
	_T("이동 거리(mm)"),
	_T("이동 속도"),
};

static LPCTSTR m_szTeachJogEng[] =
{
	_T("Jog Mode"),
	_T("Select Axis"),
	_T("Distance Set(mm)"),
	_T("Speed Set"),
};

static LPCTSTR m_szTeachJogBtnKor[] =
{
	_T("속도 모드"),
	_T("상대 이동 모드"),

	_T("비네팅 X"),
	_T("비네팅 R"),
	_T("스크류 L"),
	_T("스크류 R"),
	_T("검사 R"),
	_T("축 6"),
	_T("축 7"),
	_T("축 8"),
	_T("축 9"),
	_T("축 10"),

	_T("1mm"),
	_T("3mm"),
	_T("5mm"),
	_T("10mm"),
	_T("이동거리 입력"),

	_T("저속"),
	_T("중속"),
	_T("고속"),

	_T("이동(-)"),
	_T("이동(+)"),

	_T("개별축 HOME 동작"),
};

static LPCTSTR m_szTeachJogBtnEng[] =
{
	_T("Velocity"),
	_T("Relative"),

	_T("Vignetting X"),
	_T("Vignetting R"),
	_T("Screw L"),
	_T("Screw R"),
	_T("Adjust R"),
	_T("Axis 6"),
	_T("Axis 7"),
	_T("Axis 8"),
	_T("Axis 9"),
	_T("Axis 10"),

	_T("1mm"),
	_T("3mm"),
	_T("5mm"),
	_T("10mm"),
	_T("Distance Input"),

	_T("Slow"),
	_T("Middle"),
	_T("High"),

	_T("Move(-)"),
	_T("move(+)"),

	_T("Each Axis Home"),
};

static LPCTSTR m_szTeachConvKor[] =
{
	_T("전면1 도어락"),
	_T("왼쪽1 도어락"),
	_T("왼쪽2 도어락"),
	_T("왼쪽3 도어락"),
	_T("오른쪽1 도어락"),
	_T("오른쪽2 도어락"),
	_T("오른쪽3 도어락"),
	_T("후면1 도어락"),
	_T("후면2 도어락"),


};

static LPCTSTR m_szTeachConvEng[] =
{
	_T("Front1 DoorLock"),
	_T("Side Left1 DoorLock"),
	_T("Side Left2 DoorLock"),
	_T("Side Left3 DoorLock"),
	_T("Side Right1 DoorLock"),
	_T("Side Right2 DoorLock"),
	_T("Side Right3 DoorLock"),
	_T("Rear1 DoorLock"),
	_T("Rear2 DoorLock"),
};

static LPCTSTR m_szTeachCylindeBtnKor[] =
{
	_T("그립"),
	_T("고정"),
	_T("전진"),
	_T("고정"),
	_T("고정"),

	_T("언그립"),
	_T("풀림"),
	_T("후진"),
	_T("풀림"),
	_T("풀림"),
};

static LPCTSTR m_szTeachCylindeBtnEng[] =
{
	_T("Grip"),
	_T("Lock"),
	_T("Fwd"),
	_T("Lock"),
	_T("Lock"),

	_T("Ungrip"),
	_T("Unlock"),
	_T("Bwd"),
	_T("Unlock"),
	_T("Unlock"),

};
static LPCTSTR m_szTeachSensnorKor[] =
{
	_T("제품 안착센서 L"),
	_T("제품 안착센서 R"),
	_T("제품고정 센서 L"),
	_T("제품고정 센서 R"),
	_T("조정 전진 센서"),
	_T("조정 후진 센서"),
	_T("L토크 지그 고정 센서 L"),
	_T("L토크 지그 고정 센서 R"),
	_T("L토크 지그 안착 센서"),
	_T("L토크 지그 전진 센서"),
	_T("L토크 지그 후진 센서"),
	_T("R토크 지그 고정 센서 L"),
	_T("R토크 지그 고정 센서 R"),
	_T("R토크 지그 안착 센서"),
	_T("R토크 지그 전진 센서"),
	_T("R토크 지그 후진 센서"),
};

static LPCTSTR m_szTeachSensorEng[] =
{
	_T("Product Sensor L"),
	_T("Product Sensor R"),
	_T("Socket Lock Sensor L"),
	_T("Socket Lock Sensor R"),
	_T("Chuck Fwd Sensnor"),
	_T("Chuck Bwd Sensnor"),
	_T("L Jin Lock L Sensnor"),
	_T("L Jin Lock R Sensnor"),
	_T("L Jin Check Sensnor"),
	_T("L Jin In Sensnor"),
	_T("L Jin Out Sensnor"),
	_T("R Jin Lock L Sensnor"),
	_T("R Jin Lock R Sensnor"),
	_T("R Jin Check Sensnor"),
	_T("R Jin In Sensnor"),
	_T("R Jin Out Sensnor"),

};
static LPCTSTR m_szTeachCylinderKor[] =
{
	_T("조정 그립/언그립"),
	_T("제품 고정"),
	_T("조정부 전/후"),
	_T("L토크지그 풀림/고정"),
	_T("R토크지그 풀림/고정"),
};

static LPCTSTR m_szTeachCylinderEng[] =
{
	_T("Chuck Grip/Ungrip"),
	_T("Socket Lock / Unlock"),
	_T("Chuck Fwd/Bwd"),
	_T("Left Jig Lock/Unlock"),
	_T("Right Jig Lock/Unlock"),
};

static LPCTSTR m_szTeachMotorSpeedKor[] =
{
	_T("모터 선택"),
	_T("모터 속도\n(RPM)"),
	
	_T("저속"),
	_T("중속"),
	_T("고속"),
	_T("가속도"),
	_T("감속도"),
};

static LPCTSTR m_szTeachMotorSpeedEng[] =
{
	_T("Motor Select"),
	_T("Motor Speed\n(RPM)"),

	_T("Slow"),
	_T("Middle"),
	_T("High"),
	_T("Acc"),
	_T("Dec"),
};

static LPCTSTR m_szTeachTrinity_CvList_Kor[] =
{
	_T("컨베이어1"),
	_T("컨베이어2"),
	_T("컨베이어3"),
};

static LPCTSTR m_szTeachTrinity_Chart_List_Kor[] =
{
	_T("46도"),
	_T("82.5도"),
	_T("180도"),
};

static LPCTSTR m_szTeachTrinity_CvList_Eng[] =
{
	_T("Conveyor1"),
	_T("Conveyor2"),
	_T("Conveyor3"),
};

static LPCTSTR m_szTeachTrinity_Chart_List_Eng[] =
{
	_T("46 Angle"),
	_T("82.5 Angle"),
	_T("180 Angle"),
};
static LPCTSTR m_szTeachTrinity_Vigneting_X_List_Kor[] =
{
	_T("대기(mm)"),
	_T("검사 1 (mm)"),
	_T("검사 2 (mm)"),
};
static LPCTSTR m_szTeachTrinity_Vigneting_X_List_Eng[] =
{
	_T("Wait(mm)"),
	_T("Inspection Pos1(mm)"),
	_T("Inspection Pos2(mm)"),
};
static LPCTSTR m_szTeachTrinity_Vigneting_R_List_Kor[] =
{
	_T("대기(mm)"),
	_T("검사 1 (mm)"),
	_T("검사 2 (mm)"),
};
static LPCTSTR m_szTeachTrinity_Vigneting_R_List_Eng[] =
{
	_T("Wait(mm)"),
	_T("Inspection Pos1(mm)"),
	_T("Inspection Pos2(mm)"),
};

static LPCTSTR m_szTeachTrinity_Screw_List_Kor[] =
{
	_T("대기 (mm)"),
	_T("시업점검 (mm)"),
	_T("검사 (mm)"),
};
static LPCTSTR m_szTeachTrinity_Screw_List_Eng[] =
{
	_T("Wait(mm)"),
	_T("Commencement(mm)"),
	_T("Work(mm)"),
};

static LPCTSTR m_szTeachTrinity_SPEC_List_Kor[] =
{
	_T("조정(pixel,±)"),
	_T("스크류 1차(pixel,±)"),
	_T("스크류 2차(pixel,±)"),
	_T("스크류 잠그고 검사 반복(EA)"),
	_T("조정 역방향 보정"),
	_T("조정 오프셋 Y"),
};
static LPCTSTR m_szTeachTrinity_SPEC_List_Eng[] =
{
	_T("Adjust(pixel,±)"),
	_T("Screw 1(pixel,±)"),
	_T("Screw 2(pixel,±)"),
	_T("Screw Retry Cnt(EA)"),
	_T("Adjust Reverse"),
	_T("Adjust Offset Y"),
};

static LPCTSTR m_szTeachTrinity_Adjust_List_Kor[] =
{
	_T("1픽셀 회전각도"),
	_T("검사 (mm)"),
};
static LPCTSTR m_szTeachTrinity_Adjust_List_Eng[] =
{
	_T("1 pixel rotation angle"),
	_T("Work(mm)"),
};


#if SET_INSPECTOR == SYS_DISTORTION
static LPCTSTR m_szTeachTrinity_AxisZList_Kor[] =
{
	_T("SOCKET Z"),

	_T("대기위치1(mm)"),
	_T("46, 82.5도 검사위치1(mm)"),
	_T("트레이 받는위치1(mm)"),
	_T("컨베이어 대기위치1(mm)"),
	_T(""),
	_T("180도 검사위치1(mm)"),
	
	_T("대기위치2(mm)"),
	_T("46, 82.5도 검사위치2(mm)"),
	_T("트레이 받는위치2(mm)"),
	_T("컨베이어 대기위치2(mm)"),
	_T(""),
	_T("180도 검사위치2(mm)"),
};

static LPCTSTR m_szTeachTrinity_AxisZList_Eng[] =
{
	_T("SOCKET Z"),

	_T("Waiting Pos1(mm)"),
	_T("46, 82.5 Angle Inspect Pos1(mm)"),
	_T("Tray Grap Pos1(mm)"),
	_T("Conveyor WaitPos1(mm)"),
	_T(""),
	_T("180 Angle Inspect Pos1(mm)"),
	

	_T("Waiting Pos2(mm)"),
	_T("46, 82.5 Angle Inspect Pos2(mm)"),
	_T("Tray Grap Pos2(mm)"),
	_T("Conveyor WaitPos2(mm)"),
	_T(""),
	_T("180 Angle Inspect Pos2(mm)"),
	
};
#elif SET_INSPECTOR == SYS_SFR_WIDE_H \
   || SET_INSPECTOR == SYS_SFR_WIDE_V
static LPCTSTR m_szTeachTrinity_AxisZList_Kor[] =
{
	_T("SOCKET Z"),

	_T("대기위치1(mm)"),
	_T("82.5도 검사위치1(mm)"),
	_T("트레이 받는위치1(mm)"),
	_T("컨베이어 대기위치1(mm)"),
	_T(""),
	_T("180도 검사위치1(mm)"),
	

	_T("대기위치2(mm)"),
	_T("82.5도 검사위치2(mm)"),
	_T("트레이 받는위치2(mm)"),
	_T("컨베이어 대기위치2(mm)"),
	_T(""),
	_T("180도 검사위치2(mm)"),	
};

static LPCTSTR m_szTeachTrinity_AxisZList_Eng[] =
{
	_T("SOCKET Z"),

	_T("Waiting Pos1(mm)"),
	_T("82.5 Angle Inspect Pos1(mm)"),
	_T("Tray Grap Pos1(mm)"),
	_T("Conveyor WaitPos1(mm)"),	
	_T(""),
	_T("180 Angle Inspect Pos1(mm)"),

	_T("Waiting Pos2(mm)"),
	_T("82.5 Angle Inspect Pos2(mm)"),
	_T("Tray Grap Pos2(mm)"),
	_T("Conveyor WaitPos2(mm)"),	
	_T(""),
	_T("180 Angle Inspect Pos2(mm)"),
};
#elif SET_INSPECTOR == SYS_COLOR_CAL
static LPCTSTR m_szTeachTrinity_AxisZList_Kor[] =
{
	_T("SOCKET Z"),

	_T("대기위치1(mm)"),
	_T("46도, 180도 검사위치1(mm)"),
	_T("트레이 받는위치1(mm)"),
	_T("컨베이어 대기위치1(mm)"),	
	_T("82.5도 검사위치1(mm)"),
	_T("CL500A 위치1(mm)"),

	_T("대기위치2(mm)"),
	_T("46도, 180도 검사위치2(mm)"),
	_T("트레이 받는위치2(mm)"),
	_T("컨베이어 대기위치2(mm)"),	
	_T("82.5도 검사위치2(mm)"),
	_T("CL500A 위치2(mm)"),
};

static LPCTSTR m_szTeachTrinity_AxisZList_Eng[] =
{
	_T("SOCKET Z"),

	_T("Waiting Pos1(mm)"),
	_T("46, 180 Angle Inspect Pos1(mm)"),
	_T("Tray Grap Pos1(mm)"),
	_T("Conveyor WaitPos1(mm)"),	
	_T("82.5 Angle Inspect Pos1(mm)"),
	_T("CL500A Measure Pos1(mm)"),

	_T("Waiting Pos2(mm)"),
	_T("46, 180 Angle Inspect Pos2(mm)"),	
	_T("Tray Grap Pos2(mm)"),
	_T("Conveyor WaitPos2(mm)"),	
	_T("82.5 Angle Inspect Pos2(mm)"),
	_T("CL500A Measure Pos2(mm)"),
};
#elif SET_INSPECTOR == SYS_HOTPIXEL
static LPCTSTR m_szTeachTrinity_AxisZList_Kor[] =
{
	_T("SOCKET Z"),

	_T("대기위치1(mm)"),
	_T("검사위치1(mm)"),
	_T("트레이 받는위치1(mm)"),
	_T("컨베이어 대기위치1(mm)"),
	_T(""),
	_T("CL200A 위치1(mm)"),

	_T("대기위치2(mm)"),
	_T("검사위치2(mm)"),
	_T("트레이 받는위치2(mm)"),
	_T("컨베이어 대기위치2(mm)"),
	_T(""),
	_T("CL200A 위치2(mm)"),
};

static LPCTSTR m_szTeachTrinity_AxisZList_Eng[] =
{
	_T("SOCKET Z"),

	_T("Waiting Pos1(mm)"),
	_T("Inspect Pos1(mm)"),
	_T("Tray Grap Pos1(mm)"),
	_T("Conveyor WaitPos1(mm)"),
	_T(""),
	_T("CL200A Measure Pos1(mm)"),

	_T("Waiting Pos2(mm)"),
	_T("Inspect Pos2(mm)"),
	_T("Tray Grap Pos2(mm)"),
	_T("Conveyor WaitPos2(mm)"),
	_T(""),
	_T("CL200A Measure Pos2(mm)"),
};
#else
static LPCTSTR m_szTeachTrinity_AxisZList_Kor[] =
{
	_T("SOCKET Z"),

	_T("대기위치1(mm)"),
	_T("검사위치1(mm)"),
	_T("트레이 받는위치1(mm)"),
	_T("컨베이어 대기위치1(mm)"),
	_T(""),
	_T(""),

	_T("대기위치2(mm)"),
	_T("검사위치2(mm)"),
	_T("트레이 받는위치2(mm)"),
	_T("컨베이어 대기위치2(mm)"),
	_T(""),
	_T(""),
};

static LPCTSTR m_szTeachTrinity_AxisZList_Eng[] =
{
	_T("SOCKET Z"),

	_T("Waiting Pos1(mm)"),
	_T("Inspect Pos1(mm)"),
	_T("Tray Grap Pos1(mm)"),
	_T("Conveyor WaitPos1(mm)"),
	_T(""),
	_T(""),

	_T("Waiting Pos2(mm)"),
	_T("Inspect Pos2(mm)"),
	_T("Tray Grap Pos2(mm)"),
	_T("Conveyor WaitPos2(mm)"),
	_T(""),
	_T(""),
};
#endif

static LPCTSTR m_szTeachTrinity_AxisStatus_List[] =
{
	_T("Error All"),
	_T("H/W +Limit"),
	_T("H/W -Limit"),
	_T("S/W +Limit"),
	_T("S/W -Limit"),
	_T("Reserved"),
	_T("Reserved"),
	_T("Err Pos Overflow"),
	_T("Err Over Current"),
	_T("Err Over Speed"),
	_T( "Err Pos Tracking"),
	_T("Err Over load"),
	_T("Err Over heat"),
	_T( "Err Back EMF"),
	_T("Err Motor Power"),
	_T("Err Inposition"),
	_T( "Emg Stop"),
	_T("Slow Stop"),
	_T("Org Returning"),
	_T( "Inposition"),
	_T("Servo On"),
	_T("Alarm Reset"),
	_T( "PT Stopped"),
	_T("Origin Sensor"),
	_T("Z Pulse"),
	_T( "Org Ret OK"),
	_T("Motion DIR"),
	_T("Motioning"),
	_T("Motion Pause"),
	_T("Motion Accel"),
	_T("Motion Decel"),
	_T("Motion Constant"),
};

static LPCTSTR m_szManualMoveList_Kor[] =
{
	_T("초기화"),
	_T("광원 OFF"),
	_T("광원 ON"),	

	_T("제품 고정"),
	_T("파워 서플라이 켜기"),
	_T("SEMCO 프로그램 Ready"),
	_T("SEMCO 프로그램 Preview Start"),
	_T("Screw 풀기"),
	_T("SEMCO 프로그램 Test TLA Start"),
	_T("SEMCO 프로그램 Test TLA 조정"),
	_T("Screw 잠그기 1"),
	_T("Screw 잠그기 2"),
	_T("조정 풀기"),

	_T("Vigneting 46 광원 이동 "),				// 버퍼 lift dn
	_T("SEMCO 프로그램 Test Vigneting 46 Start"),
	_T("Vigneting 82 광원 이동 "),				// 버퍼 lift dn
	_T("SEMCO 프로그램 Test Vigneting 82 Start"),
	_T("SEMCO 프로그램 Preview Stop"),
	_T("파워 서플라이 끄기"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
};

static LPCTSTR m_szManualMoveList_Eng[] =
{
	_T("Initialize"),
	_T("Light OFF"),
	_T("Light ON"),
	_T("Product Fixation"),
	_T("Power Supply On"),
	_T("SEMCO Ready"),
	_T("SEMCO Preview Start"),
	_T("Screw Bolt Unlock"),
	_T("SEMCO Test TLA Start"),
	_T("SEMCO Test TLA Adjust"),
	_T("Screw Bolt Lock 1"),
	_T("Screw Bolt Lock 2"),
	_T("Adjust Unlock"),

	_T("Vigneting 46 Light Move "),
	_T("SEMCO Test Vigneting 46 Start"),
	_T("Vigneting 82 Light Move "),
	_T("SEMCO Test Vigneting 82 Start"),
	_T("SEMCO Preview Stop"),
	_T("Power Supply Off"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),

};

// -------------------------------------------------------------------------
// DIO
// IO Dlg End
// -------------------------------------------------------------------------

//List Conroller
enum
{
	WAIT_COMM_LIST1,
	WAIT_COMM_LIST2,
	WAIT_COMM_LIST3,
	WAIT_COMM_LIST4,
	WAIT_COMM_LIST5,
	WAIT_COMM_LIST6,
	WAIT_COMM_MAX,
};
static LPCTSTR m_szWaitTimeList_Kor[] =
{
	_T("카메라 초기화"),
	_T("카메라 시작"),
	_T("이물검사"),
	_T("SFR검사"),
	_T("카메라 종료"),
	_T(""),
};
static LPCTSTR m_szWaitTimeList_Eng[] =
{
	_T("Camera Initialization"),
	_T("Camera Start"),
	_T("Foreign Inspection"),
	_T("SFR Inspection"),
	_T("Camera Quit"),
	_T(""),
};

//-------------------------------------------------------------------------
// Component 
static LPCTSTR m_szComponentWaitList_Kor[] =
{
	_T("통신 이름"),
	_T("대기 시간(ms)"),
};
static LPCTSTR m_szComponentWaitList_Eng[] =
{
	_T("Communication Name"),
	_T("WAIT TIME(ms)"),
};

static LPCTSTR m_szConfigurationButton_Kor[] =
{
	_T("시스템 설정"),
	_T("통신 설정"),
	_T("디바이스 설정"),
	_T("SEMCO Test"),
};

static LPCTSTR m_szConfigurationButton_Eng[] =
{
	_T("System"),
	_T("Communication"),
	_T("Device"),
	_T("SEMCO Test"),
};

static LPCTSTR m_szCommConfigurationTitle_Kor[] =
{
	_T("LAN 통신 연결"),
	_T("SERIAL 통신 연결"),
	_T("기타 통신 연결"),
};
static LPCTSTR m_szCommConfigurationTitle_Eng[] =
{
	_T("LAN Communication"),
	_T("SERIAL Communication"),
	_T("Etc Communication"),
};

static LPCTSTR m_szDeviceConfigurationTitle_Kor[] =
{
	_T("광원 밝기 설정"),
	_T("RFID 읽기"),
	_T("조도계 측정"),
};

static LPCTSTR m_szDeviceConfigurationTitle_Eng[] =
{
	_T("Light Value Setting"),
	_T("Read RFID"),
	_T("Measure Chroma Meter"),
};

static LPCTSTR m_szSystemConfigurationTitle_Kor[] =
{
	_T("시스템 설정"),
	_T("통신 대기시간 설정"),
};

static LPCTSTR m_szSystemConfigurationTitle_Eng[] =
{
	_T("System Setting"),
	_T("Wait Time Communication"),
};

static LPCTSTR m_szSystemConfigurationSubTitle_Kor[] =
{
	_T("설정 폴더 경로"),
	_T("설비 이름"),
	_T("Sleep 모드 시간 (Sec)"),
	_T("대기 시간"),
};

static LPCTSTR m_szSystemConfigurationSubTitle_Eng[] =
{
	_T("Setting Folder Path"),
	_T("EQP Name"),
	_T("Sleep Mode Time (Sec)"),
	_T("Wait Time"),
};

static LPCTSTR m_szComponentLan_Sub_Kor[] =
{
	_T("TESTER1 연결"),
	_T("TESTER2 연결"),
	_T("RFID 연결"),
	_T("물류제어 프로그램 연결"),
};
static LPCTSTR m_szComponentLan_Sub_Eng[] =
{
	_T("TESTER1 Connect"),
	_T("TESTER2 Connect"),
	_T("RFID Connect"),
	_T("Logistic Manager Connect"),
};

static LPCTSTR m_szComponentConnect_Kor[] =
{
	_T("연결"),
	_T("해제"),
};
static LPCTSTR m_szComponentConnect_Eng[] =
{
	_T("Connect"),
	_T("Disconnect"),
};

static LPCTSTR m_szComponentSerialSub_Kor[] =
{
	_T("파워 서플라이"),
	_T("광원 컨트롤러 1"),
	_T("광원 컨트롤러 2"),
	_T("광원 컨트롤러 3"),
	_T("바코드 1"),
	_T("바코드 2"),
	_T("CL-200A"),
	_T("CL-500A"),
	_T("Flare 광원 컨트롤러"),
};

static LPCTSTR m_szComponentSerialSub_Eng[] =
{
	_T("Power Supply"),
	_T("Light Controller 1"),
	_T("Light Controller 2"),
	_T("Light Controller 3"),
	_T("Barcode 1"),
	_T("Barcode 2"),
	_T("CL-200A"),
	_T("CL-500A"),
	_T("Flare Light Controller"),
};

static LPCTSTR m_szComponentSerial_Param[] =
{
	_T("Com Port"),
	_T("BaudRate"),
	_T("Data Bit"),
	_T("Parity Bit"),
	_T("Stop Bit"),
};

//-------------------------------------------------------------------------
// Error List
static LPCTSTR m_szErrList_Title_Kor[] =
{
	_T("에러리스트"),
	_T("해결 방법"),
	_T("에러 발생 위치"),
};
static LPCTSTR m_szErrList_Title_Eng[] =
{
	_T("Error List"),
	_T("Solution"),
	_T("Error Position"),
};

static LPCTSTR m_szErrList_TitleSub_Kor[] =
{
	_T("번호"),
	_T("에러번호"),
	_T("에러명"),

	_T("에러번호"),
	_T("해결방법"),
};
static LPCTSTR m_szErrList_TitleSub_Eng[] =
{
	_T("Num"),
	_T("Error Num"),
	_T("Error Name"),

	_T("Error Num"),
	_T("Error Solition"),
};

//-------------------------------------------------------------------------
// Key Pad Dialog
static LPCTSTR m_szKeyPadNujm_Param[] =
{
	_T("0"),
	_T("1"),
	_T("2"),
	_T("3"),
	_T("4"),
	_T("5"),
	_T("6"),
	_T("7"),
	_T("8"),
	_T("9"),
	_T("."),
	_T("DEL"),
	_T("+ / -"),
	_T("Ok"),
	_T("Cancel"),

};

//-------------------------------------------------------------------------
// 언어 설정 Dialog

static LPCTSTR m_szSelect_Language_Kor[] =
{
	_T("한국어"),
	_T("English"),
	_T("-"),
	_T("-"),
};

static LPCTSTR m_szSelect_Language_Eng[] =
{
	_T("한국어"),
	_T("English"),
	_T("-"),
	_T("-"),
};

//-------------------------------------------------------------------------
// 권한 설정 Dialog
static LPCTSTR m_szSelect_Admin_Kor[] =
{
	_T("작업자"),
	_T("메인터넌스"),
	_T("관리자"),
};
static LPCTSTR m_szSelect_Admin_Eng[] =
{
	_T("Operator"),
	_T("Maintenance"),
	_T("Administrator"),
};

//-------------------------------------------------------------------------
// TowerLamp
static LPCTSTR m_szTowerLamp[] =
{
	_T("RUN"),
	_T("STOP"),
	_T("ERROR"),
	_T("READY"),
	_T("CALL"),
	_T("PAUSE"),
};

static LPCTSTR m_szToweColor[] =
{
	_T("RED"),
	_T("YELLOW"),
	_T("GREEN"),
	_T("BUZZER"),
};

//-------------------------------------------------------------------------
// SFR Multi CL 광원 위치
static LPCTSTR m_szSFRMultiCL_LightPosition[] =
{
	_T("0F"),
	_T("0.5F L"),
	_T("0.7F LL"),
	_T("0.5F B"),
	_T("0.7F LR"),
	_T("0.5F R"),
	_T("0.7F UR"),
	_T("0.5F T"),
	_T("0.5F UL"),
};

//-------------------------------------------------------------------------
// Configuration
enum class enConfiguration
{
	System,
	Communication,
	Device,
	SEMCOTest
};


#if (20230905)
static LPCTSTR m_szComponentLan_Common_Sub_Eng[] =
{
	_T("SCREW Left Connect"),
	_T("SCREW Right Connect"),
	_T(""),
};
static LPCTSTR m_szComponentLan_Common_Sub_Kor[] =
{
	_T("SCREW 왼쪽 연결"),
	_T("SCREW 오른쪽 연결"),
	_T(""),
};
static LPCTSTR m_szIP_Common_UI_Text[] =
{
	_T("SCREW Left"),
	_T("SCREW Right"),
	_T(""),
};
enum {
	IP_COMMON_SCREW_LEFT,
	IP_COMMON_SCREW_RIGHT,
	IP_COMMON_MAX
};
static LPCTSTR m_szScrew_Parameter_Sub_Eng[] =
{
	_T("NULL"),
	_T("UNLOCK"),
	_T("LOCK 1"),
	_T("LOCK 2"),
	_T(""),
};
static LPCTSTR m_szScrew_Parameter_Sub_Kor[] =
{
	_T("NULL"),
	_T("UNLOCK"),
	_T("LOCK 1"),
	_T("LOCK 2"),
	_T(""),
};
static LPCTSTR m_szScrew_Parameter_UI_Text[] =
{
	_T("NULL"),
	_T("UNLOCK"),
	_T("LOCK 1"),
	_T("LOCK 2"),
	_T(""),
};
enum {
	SCREW_PARAMTER_0,
	SCREW_PARAMTER_1,
	SCREW_PARAMTER_2,
	SCREW_PARAMTER_3,
	SCREW_PARAMTER_Max,
};
#endif