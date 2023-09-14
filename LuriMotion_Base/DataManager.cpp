#include "pch.h"
#include "DataManager.h"
#include "DlgLog.h"

CDataManager::CDataManager(void) : m_bInit(FALSE)
{

}


CDataManager::~CDataManager(void)
{
}


void CDataManager::Init()
{
	LoadEqBasicInfo();

	m_bInit = TRUE;
}

void CDataManager::LoadEqBasicInfo()
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	CFileFind FindFile;
	BOOL bRet = FindFile.FindFile(strSystemFile);
	if (!bRet)
	{
		SaveTowerLamp(strSystemFile);
		SaveEqBasicParam(strSystemFile);
	}
	else
	{
		LoadTowerLamp(strSystemFile);
		LoadEqBasicParam(strSystemFile);
	}
	
	//Motor Speed
	CString strMotionFile;
	strMotionFile.Format(_T("%s\\%s\\MotionParam.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	bRet = FindFile.FindFile(strMotionFile);
	if (!bRet)
		SaveMotionParam(strMotionFile);
	else
		LoadMotionParam(strMotionFile);

	// Tester Comm 
	CString strTesterCommFile;
	strTesterCommFile.Format(_T("%s\\%s\\TesterComm.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	bRet = FindFile.FindFile(strTesterCommFile);
	if (!bRet)
		SaveTesterCommData(strTesterCommFile);
	else
		LoadTesterCommData(strTesterCommFile);


	CString strTeachionFile;
	strTeachionFile.Format(_T("%s\\%s\\%s.Mod"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe, m_stEqBasicParam.strLastRecipe);

	// Teaching Pos
	bRet = FindFile.FindFile(strTeachionFile);
	if (!bRet)
		SaveTeachingPosData(strTeachionFile);
	else
		LoadTeachingPosData(strTeachionFile);


	CString strErrorListFile;
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	bRet = FindFile.FindFile(strErrorListFile);
	if (!bRet)
	{
		SaveErrorList_SolutionList();
		LoadErrorList_SolutionList();
	}		
	else
		LoadErrorList_SolutionList();
	
}

void CDataManager::SaveEqBasicParam(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	CString strApp = _T("EQ_BASIC");
	CString strKey, strTemp;

	strKey.Format(_T("LAST_RECIPE_NAME"));
	strTemp.Format(_T("%s"), m_stEqBasicParam.strLastRecipe);
	WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

	strKey.Format(_T("SELECT_LANGUAGE"));
	strTemp.Format(_T("%d"), m_stEqBasicParam.nLanguage);
	WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);


	// [21.1017.6] Modified
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		strKey.Format(_T("USER_ID_%02d"), i);
		strTemp.Format(_T("%s"), m_stEqBasicParam.stUserData[i].strUserID);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

		strKey.Format(_T("USER_PW_%02d"), i);
		strTemp.Format(_T("%s"), m_stEqBasicParam.stUserData[i].strPassword);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

		strKey.Format(_T("USER_PERMISSION_%02d"), i);
		strTemp.Format(_T("%d"), m_stEqBasicParam.stUserData[i].nPermission);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);
	}
}

void CDataManager::LoadEqBasicParam(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("EQ_BASIC");
	CString strKey;

	strKey.Format(_T("LAST_RECIPE_NAME"));
	GetPrivateProfileString(strApp, strKey, _T("Default_Recipe"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.strLastRecipe = (LPCTSTR)strTemp;

	strKey.Format(_T("SELECT_LANGUAGE"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.nLanguage = _ttoi(strTemp);

	strKey.Format(_T("IO_VERSION"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.nDeviceIoVersion = _ttoi(strTemp);

	// [21.1017.7] Modified
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		strKey.Format(_T("USER_ID_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].strUserID = strTemp;

		strKey.Format(_T("USER_PW_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].strPassword = strTemp;

		strKey.Format(_T("USER_PERMISSION_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].nPermission = _ttoi(strTemp);
	}
}

EQ_BASIC_PARAM* CDataManager::GetCurrentEqBasicData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stEqBasicParam;
}

void CDataManager::SetEqData(const EQ_BASIC_PARAM& stEqParam)
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	m_stEqBasicParam = stEqParam;
	SaveEqBasicParam(strSystemFile);
}


//===========================================================================================================
// TOWER LAMP
void CDataManager::SaveTowerLamp(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	CString strApp = _T("TOWER_LAMP");
	CString strKey, strTemp;

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			strKey.Format(_T("TOWER_LAMP_%02d_%02d"), i,j);
			strTemp.Format(_T("%d"), m_stTowerLamp.nTowerLampSetting[i][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);
		}
	}
}
void CDataManager::LoadTowerLamp(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TOWER_LAMP");
	CString strKey;

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			strKey.Format(_T("TOWER_LAMP_%02d_%02d"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
			m_stTowerLamp.nTowerLampSetting[i][j] = _ttoi(strTemp);
		}
	}
}

EQ_TOWERLAMP* CDataManager::GetCurrentTowerLampData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTowerLamp;
}
void CDataManager::SetstTower(const EQ_TOWERLAMP& stTowerLamp)
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	m_stTowerLamp = stTowerLamp;
	SaveTowerLamp(strSystemFile);
}


//===========================================================================================================
// TESTER CONNECT 
void CDataManager::SaveTesterCommData(const LPCTSTR& lpszPath)
{
	CString strTesterFile;
	strTesterFile = lpszPath;

	CString strApp = _T("TESTER_CONNECT");
	CString strKey, strTemp, strTotal;

	strKey.Format(_T("SYSTEM_FOLDER_PATH"));
	strTemp.Format(_T("%s"), m_stTester.strSystemFolderPath);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("EQP_NAME"));
	strTemp.Format(_T("%s"), m_stTester.strEQPName);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	for (int i = 0; i < WAITLIST_COMM_MAX; i++)
	{
		strKey.Format(_T("COMM_WATITIME_COLUMN%02d"), i);
		strTemp.Format(_T("%s"), m_stTester.m_strWaitTimeLIst[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	strKey.Format(_T("SLEEPMODE_WATITIME"));
	strTemp.Format(_T("%d"), m_stTester.nSleepModeTimer);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	for (int i = 0; i < 4; i++)
	{
		strKey.Format(_T("TESTER_IP_%02d"), i);
		strTemp.Format(_T("%s"), m_stTester.strTesterConnect[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_IP_DWORD_%02d"), i);
		strTemp.Format(_T("%u"), m_stTester.dwTesterIP[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_PORT_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nTesterPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_USE_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nUseMode[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_SERVERMODE_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nServerMode[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	for (int i = 0; i < 7; i++)
	{
		strKey.Format(_T("LIGHT_%02d_PORT"), i);
		strTemp.Format(_T("%d"), m_stTester.nPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_BAUD"), i);
		strTemp.Format(_T("%d"), m_stTester.nBaudRate[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_DATA"), i);
		strTemp.Format(_T("%d"), m_stTester.nDataBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_STOP"), i);
		strTemp.Format(_T("%d"), m_stTester.nStopBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_PARITY"), i);
		strTemp.Format(_T("%d"), m_stTester.nParityBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	}

	// 광원 밝기 사용
	for (int i = 0; i < 20; i++)
	{
		strKey.Format(_T("LIGHT_%02d_VOLTAGE_LURI"), i);
		strTemp.Format(_T("%d"), m_stTester.nLightValue[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	// COLOR CAL SEMCO 사용 광원 채널
	for (int i = 0; i < 4; i++)
	{
		strKey.Format(_T("COLORCAL_SEMCO_USE_LIGHT_CH_%d"), i);
		strTemp.Format(_T("%d"), m_stTester.nColorCal_SemcoLight[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}
}
void CDataManager::LoadTesterCommData(const LPCTSTR& lpszPath)
{
	CString strTesterCommFile;
	strTesterCommFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TESTER_CONNECT");
	CString strKey;

	strKey.Format(_T("SYSTEM_FOLDER_PATH"));
	GetPrivateProfileString(strApp, strKey, _T("C:\\Exec_Trinity"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.strSystemFolderPath = strTemp;

	strKey.Format(_T("EQP_NAME"));
	GetPrivateProfileString(strApp, strKey, g_szInsptrSysType[SET_INSPECTOR], strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.strEQPName = strTemp;

	for (int i = 0; i < WAITLIST_COMM_MAX; i++)
	{
		strKey.Format(_T("COMM_WATITIME_COLUMN%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.m_strWaitTimeLIst[i] = strTemp;
	}

	strKey.Format(_T("SLEEPMODE_WATITIME"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nSleepModeTimer = _ttoi(strTemp);

	for (int i = 0; i < 4; i++)
	{
		strKey.Format(_T("TESTER_IP_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.strTesterConnect[i] = strTemp;

		strKey.Format(_T("TESTER_IP_DWORD_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.dwTesterIP[i] = (DWORD)_ttof(strTemp);

		strKey.Format(_T("TESTER_PORT_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nTesterPort[i] = _ttoi(strTemp);

		strKey.Format(_T("TESTER_USE_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nUseMode[i] = _ttoi(strTemp);

		strKey.Format(_T("TESTER_SERVERMODE_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nServerMode[i] = _ttoi(strTemp);
	}

	for (int i = 0; i < 7; i++)
	{
		strKey.Format(_T("LIGHT_%02d_PORT"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nPort[i] = _ttoi(strTemp);
		m_stTester.nPort[i] = max(m_stTester.nPort[i], 0);

		strKey.Format(_T("LIGHT_%02d_BAUD"), i);
		GetPrivateProfileString(strApp, strKey, _T("9600"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nBaudRate[i] = _ttoi(strTemp);
		m_stTester.nBaudRate[i] = max(m_stTester.nBaudRate[i], 0);

		strKey.Format(_T("LIGHT_%02d_DATA"), i);
		GetPrivateProfileString(strApp, strKey, _T("8"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nDataBit[i] = _ttoi(strTemp);
		m_stTester.nDataBit[i] = max(m_stTester.nDataBit[i], 0);

		strKey.Format(_T("LIGHT_%02d_STOP"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nStopBit[i] = _ttoi(strTemp);
		m_stTester.nStopBit[i] = max(m_stTester.nStopBit[i], 0);

		strKey.Format(_T("LIGHT_%02d_PARITY"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nParityBit[i] = _ttoi(strTemp);
		m_stTester.nParityBit[i] = max(m_stTester.nParityBit[i], 0);
	}

	for (int i = 0; i < 20; i++)
	{
		strKey.Format(_T("LIGHT_%02d_VOLTAGE_LURI"), i);
		GetPrivateProfileString(strApp, strKey, _T("12000"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nLightValue[i] = _ttoi(strTemp);
	}

	// COLOR CAL SEMCO 사용 광원 채널
	for (int i = 0; i < 4; i++)
	{
		strKey.Format(_T("COLORCAL_SEMCO_USE_LIGHT_CH_%d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nColorCal_SemcoLight[i] = _ttoi(strTemp);
	}
}

TESTER_COMM* CDataManager::GetCurrentTesterCommData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTester;
}

void CDataManager::SetTesterCommData(const TESTER_COMM& stTester)
{
	CString strTesterFile;
	strTesterFile.Format(_T("%s\\%s\\TesterComm.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);
	
	m_stTester = stTester;
	SaveTesterCommData(strTesterFile);
}


//===========================================================================================================
// MOTION SPEED
void CDataManager::SaveMotionParam(const LPCTSTR& lpszPath)
{
	CString strMotionFile;
	strMotionFile = lpszPath;

	CString strApp = _T("MOTION_PARAM");
	CString strKey, strTemp, strTotal;

	for (int i = 0; i < 1; i++)
	{
		strKey.Format(_T("MOTOR_%02d_COMPORT"), i);
		strTemp.Format(_T("%d"), m_stMotion.nPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

		strKey.Format(_T("MOTOR_%02d_BAUDRATE"), i);
		strTemp.Format(_T("%d"), m_stMotion.nBaudRate[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			strKey.Format(_T("MOTION_SPEED_%02d_%02d"), i, j);
			strTemp.Format(_T("%d"), m_stMotion.nMotorSpeed[i][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);
		}
	}

	strKey.Format(_T("CHANGE_NEW"));
	strTemp.Format(_T("%s"), m_stMotion.strChangeTime_New);
	WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

	strKey.Format(_T("CHANGE_LAST"));
	strTemp.Format(_T("%s"), m_stMotion.strChangeTime_Last);
	WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			if (m_stMotion.strDiff[i][j] != _T(""))
			{
				strTemp.Format(_T("[%d][%d] : [%s], "), i, j, m_stMotion.strDiff[i][j]);
				strTotal = strTotal + strTemp;
			}			
		}
	}
	strKey.Format(_T("CHANGE_PARAMETER"));
	WritePrivateProfileString(strApp, strKey, strTotal, strMotionFile);
}
void CDataManager::LoadMotionParam(const LPCTSTR& lpszPath)
{
	CString strMotionFile;
	strMotionFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("MOTION_PARAM");
	CString strKey;

	for (int i = 0; i < 1; i++)
	{
		strKey.Format(_T("MOTOR_%02d_COMPORT"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
		m_stMotion.nPort[i] = _ttoi(strTemp);


		strKey.Format(_T("MOTOR_%02d_BAUDRATE"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
		m_stMotion.nBaudRate[i] = _ttoi(strTemp);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			strKey.Format(_T("MOTION_SPEED_%02d_%02d"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
			m_stMotion.nMotorSpeed[i][j] = _ttoi(strTemp);
		}
	}

	strKey.Format(_T("CHANGE_NEW"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
	m_stMotion.strChangeTime_New = strTemp;

	strKey.Format(_T("CHANGE_LAST"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
	m_stMotion.strChangeTime_Last = strTemp;
}
MOTION_PARAM*	CDataManager::GetCurrentMotionParam()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stMotion;
}
void CDataManager::SetMotionParam(const MOTION_PARAM& stMotion)
{
	CString strMotionFile;
	strMotionFile.Format(_T("%s\\%s\\MotionParam.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	WriteChangesMotionParamToSemcoLog(m_stMotion, stMotion);
	m_stMotion = stMotion;
	SaveMotionParam(strMotionFile);
}

void CDataManager::WriteChangesMotionParamToSemcoLog(MOTION_PARAM stOldMotion, MOTION_PARAM stNewMotion)
{
	CString csvStr;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			if (stOldMotion.nMotorSpeed[i][j] != stNewMotion.nMotorSpeed[i][j])
			{
				csvStr.Format(_T("P,,,MOTION_SPEED_%02d_%02d,%d,%d,%s"), i, j, stOldMotion.nMotorSpeed[i][j], stNewMotion.nMotorSpeed[i][j], COMMON->GetPermissionId());
				WRITE_SEMCO_LOG(csvStr);
			}
		}
	}
}


//===========================================================================================================
// Teaching
void CDataManager::SaveTeachingPosData(const LPCTSTR& lpszPath)
{
	CString strTeachingFile;
	strTeachingFile = lpszPath;

	CString strApp = _T("TACHING_PARAM");
	CString strKey, strTemp, strTotal;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	//VIGNETTING_X
	strKey.Format(_T("AXIS_VIGNETTING_X_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_X_Wait);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_VIGNETTING_X_TEST_01_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_X_Test_01);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_VIGNETTING_X_TEST_02_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_X_Test_02);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	//VIGNETTING_R
	strKey.Format(_T("AXIS_VIGNETTING_R_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_R_Wait);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_VIGNETTING_R_TEST_01_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_R_Test_01);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_VIGNETTING_R_TEST_02_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nVignetting_R_Test_02);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_ADJUST_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	strKey.Format(_T("AXIS_ADJUST_SCREW1_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Screw1_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	strKey.Format(_T("AXIS_ADJUST_SCREW2_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Screw2_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_ADJUST_ReTryCnt"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_RetryCnt);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);


	strKey.Format(_T("ADJUST_OFFSET_Y"));
	strTemp.Format(_T("%0.1f"), m_stTeach.dAdjust_Offset_Y);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	
#endif
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	//SCREW L
	strKey.Format(_T("AXIS_SCREW_L_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_L_Wait);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SCREW_L_COMMENCEMENT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_L_Commencement);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SCREW_L_WORK_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_L_Work);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	//SCREW R
	strKey.Format(_T("AXIS_SCREW_R_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_R_Wait);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SCREW_R_COMMENCEMENT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_R_Commencement);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SCREW_R_WORK_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nScrew_R_Work);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	//ADJUST
	strKey.Format(_T("AXIS_ADJUST_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Wait);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_ADJUST_WORK_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Work);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);


	strKey.Format(_T("AXIS_ADJUST_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	strKey.Format(_T("AXIS_ADJUST_SCREW1_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Screw1_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	strKey.Format(_T("AXIS_ADJUST_SCREW2_SPEC_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Screw2_Spec);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_ADJUST_ReTryCnt"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_RetryCnt);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	strKey.Format(_T("AXIS_ADJUST_Reverse"));
	strTemp.Format(_T("%d"), m_stTeach.nAdjust_Reverse);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);


#endif

	strKey.Format(_T("BarCodeChek"));
	strTemp.Format(_T("%d"), m_stTeach.nBarcode);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
}

void CDataManager::LoadTeachingPosData(const LPCTSTR& lpszPath)
{
	CString strTeachingFile;
	strTeachingFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TACHING_PARAM");
	CString strKey;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	//VIGNETTING_X
	strKey.Format(_T("AXIS_VIGNETTING_X_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_X_Wait = _ttoi(strTemp);
	
	strKey.Format(_T("AXIS_VIGNETTING_X_TEST_01_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_X_Test_01 = _ttoi(strTemp);
	strKey.Format(_T("AXIS_VIGNETTING_X_TEST_02_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_X_Test_02 = _ttoi(strTemp);

	//VIGNETTING_R
	strKey.Format(_T("AXIS_VIGNETTING_R_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_R_Wait = _ttoi(strTemp);

	strKey.Format(_T("AXIS_VIGNETTING_R_TEST_01_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_R_Test_01 = _ttoi(strTemp);

	strKey.Format(_T("AXIS_VIGNETTING_R_TEST_02_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nVignetting_R_Test_02 = _ttoi(strTemp);
	strKey.Format(_T("AXIS_ADJUST_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_SCREW1_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Screw1_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_SCREW2_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Screw2_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_ReTryCnt"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_RetryCnt = _ttoi(strTemp);
	strKey.Format(_T("ADJUST_OFFSET_Y"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.dAdjust_Offset_Y = _ttof(strTemp);




#endif
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	//SCREW L
	strKey.Format(_T("AXIS_SCREW_L_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_L_Wait = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SCREW_L_COMMENCEMENT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_L_Commencement = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SCREW_L_WORK_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_L_Work = _ttoi(strTemp);


	//SCREW R

	strKey.Format(_T("AXIS_SCREW_R_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_R_Wait = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SCREW_R_COMMENCEMENT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_R_Commencement = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SCREW_R_WORK_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nScrew_R_Work = _ttoi(strTemp);

	//ADJUST
	strKey.Format(_T("AXIS_ADJUST_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Wait = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_WORK_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Work = _ttoi(strTemp);




	strKey.Format(_T("AXIS_ADJUST_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_SCREW1_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Screw1_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_SCREW2_SPEC_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Screw2_Spec = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_ReTryCnt"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_RetryCnt = _ttoi(strTemp);

	strKey.Format(_T("AXIS_ADJUST_Reverse"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nAdjust_Reverse = _ttoi(strTemp);

#endif

	strKey.Format(_T("BarCodeChek"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nBarcode = _ttoi(strTemp);

}

TEACHING_PARAM*	CDataManager::GetCurrentTeachingData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTeach;
}
void CDataManager::SetTeachingData(const TEACHING_PARAM& stTeaching)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strPath = _T("C:\\Exec_Trinity\\Recipe");
	CString strFullPath = _T("");

	CString strFolder = _T("");
	strFolder = strPath + _T("\\") + m_stEqBasicParam.strLastRecipe;

	CreateDirectory(strPath, NULL);
	CreateDirectory(strFolder, NULL);
	
	strFullPath.Format(_T("%s\\%s\\%s.Mod"), strPath, m_stEqBasicParam.strLastRecipe, m_stEqBasicParam.strLastRecipe);

	WriteChangesTeachingDataToSemcoLog(m_stTeach, stTeaching);
	m_stTeach = stTeaching;
	SaveTeachingPosData(strFullPath);
}

void CDataManager::WriteChangesTeachingDataToSemcoLog(TEACHING_PARAM stOldTeaching, TEACHING_PARAM stNewTeaching)
{
	CString csvStr;
	if (stOldTeaching.dAdjust_Offset_Y != stNewTeaching.dAdjust_Offset_Y)
	{
		csvStr.Format(_T("P,,,Adjust_Offset_Y,%0.3f,%0.3f,%s"), stOldTeaching.dAdjust_Offset_Y, stNewTeaching.dAdjust_Offset_Y, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_RetryCnt != stNewTeaching.nAdjust_RetryCnt)
	{
		csvStr.Format(_T("P,,,Adjust_RetryCnt,%d,%d,%s"), stOldTeaching.nAdjust_RetryCnt, stNewTeaching.nAdjust_RetryCnt, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Reverse != stNewTeaching.nAdjust_Reverse)
	{
		csvStr.Format(_T("P,,,Adjust_Reverse,%d,%d,%s"), stOldTeaching.nAdjust_Reverse, stNewTeaching.nAdjust_Reverse, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Screw1_Spec != stNewTeaching.nAdjust_Screw1_Spec)
	{
		csvStr.Format(_T("P,,,Adjust_Screw1_Spec,%d,%d,%s"), stOldTeaching.nAdjust_Screw1_Spec, stNewTeaching.nAdjust_Screw1_Spec, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Screw2_Spec != stNewTeaching.nAdjust_Screw2_Spec)
	{
		csvStr.Format(_T("P,,,Adjust_Screw2_Spec,%d,%d,%s"), stOldTeaching.nAdjust_Screw2_Spec, stNewTeaching.nAdjust_Screw2_Spec, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Spec != stNewTeaching.nAdjust_Spec)
	{
		csvStr.Format(_T("P,,,Adjust_Spec,%d,%d,%s"), stOldTeaching.nAdjust_Spec, stNewTeaching.nAdjust_Spec, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Wait != stNewTeaching.nAdjust_Wait)
	{
		csvStr.Format(_T("P,,,Adjust_Wait,%0.3f,%0.3f,%s"), stOldTeaching.nAdjust_Wait, stNewTeaching.nAdjust_Wait, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nAdjust_Work != stNewTeaching.nAdjust_Work)
	{
		csvStr.Format(_T("P,,,Adjust_Work,%d,%d,%s"), stOldTeaching.nAdjust_Work, stNewTeaching.nAdjust_Work, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nBarcode != stNewTeaching.nBarcode)
	{
		csvStr.Format(_T("P,,,Barcode,%d,%d,%s"), stOldTeaching.nBarcode, stNewTeaching.nBarcode, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nScrew_L_Commencement != stNewTeaching.nScrew_L_Commencement)
	{
		csvStr.Format(_T("P,,,Screw_L_Commencement,%d,%d,%s"), stOldTeaching.nScrew_L_Commencement, stNewTeaching.nScrew_L_Commencement, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nScrew_L_Wait != stNewTeaching.nScrew_L_Wait)
	{
		csvStr.Format(_T("P,,,Screw_L_Wait,%d,%d,%s"), stOldTeaching.nScrew_L_Wait, stNewTeaching.nScrew_L_Wait, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nScrew_L_Work != stNewTeaching.nScrew_L_Work)
	{
		csvStr.Format(_T("P,,,Screw_L_Work,%d,%d,%s"), stOldTeaching.nScrew_L_Work, stNewTeaching.nScrew_L_Work, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nScrew_R_Commencement != stNewTeaching.nScrew_R_Commencement)
	{
		csvStr.Format(_T("P,,,Screw_R_Commencement,%d,%d,%s"), stOldTeaching.nScrew_R_Commencement, stNewTeaching.nScrew_R_Commencement, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_R_Test_01 != stNewTeaching.nVignetting_R_Test_01)
	{
		csvStr.Format(_T("P,,,Vignetting_R_Test_01,%d,%d,%s"), stOldTeaching.nVignetting_R_Test_01, stNewTeaching.nVignetting_R_Test_01, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_R_Test_02 != stNewTeaching.nVignetting_R_Test_02)
	{
		csvStr.Format(_T("P,,,Vignetting_R_Test_02,%d,%d,%s"), stOldTeaching.nVignetting_R_Test_02, stNewTeaching.nVignetting_R_Test_02, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_R_Wait != stNewTeaching.nVignetting_R_Wait)
	{
		csvStr.Format(_T("P,,,Vignetting_R_Wait,%d,%d,%s"), stOldTeaching.nVignetting_R_Wait, stNewTeaching.nVignetting_R_Wait, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_X_Test_01 != stNewTeaching.nVignetting_X_Test_01)
	{
		csvStr.Format(_T("P,,,Vignetting_X_Test_01,%d,%d,%s"), stOldTeaching.nVignetting_X_Test_01, stNewTeaching.nVignetting_X_Test_01, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_X_Test_02 != stNewTeaching.nVignetting_X_Test_02)
	{
		csvStr.Format(_T("P,,,Vignetting_X_Test_02,%d,%d,%s"), stOldTeaching.nVignetting_X_Test_02, stNewTeaching.nVignetting_X_Test_02, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}

	if (stOldTeaching.nVignetting_X_Wait != stNewTeaching.nVignetting_X_Wait)
	{
		csvStr.Format(_T("P,,,Vignetting_X_Wait,%d,%d,%s"), stOldTeaching.nVignetting_X_Wait, stNewTeaching.nVignetting_X_Wait, COMMON->GetPermissionId());
		WRITE_SEMCO_LOG(csvStr);
	}
}

ST_OptSemiAuto* CDataManager::GetSemiAuto()
{
	return &m_stSemiAuto;
}

void CDataManager::SetSemiAuto(__in ST_OptSemiAuto opt)
{
	m_stSemiAuto = opt;
}

void CDataManager::RecipeChange(CString& strRecipeName)
{
	CString strPath = _T("C:\\Exec_Trinity\\Recipe\\");
	CString strFullPath = _T("");

	m_strCurrenctRecipeName = strRecipeName;
	strFullPath.Format(_T("%s%s\\%s.Mod"), strPath, strRecipeName, strRecipeName);

	// TEaching Data
	LoadTeachingPosData(strFullPath);


	// Comm Data
	CString strTesterCommFile;
	strTesterCommFile.Format(_T("%s%s\\TesterComm.ini"), strPath, strRecipeName);
	LoadTesterCommData(strTesterCommFile);
}

void CDataManager::SetSaveAs_TeachingData(const TEACHING_PARAM& stTeaching, CString strPathAndName)
{
	m_stTeach = stTeaching;
	SaveTeachingPosData(strPathAndName);
}
void CDataManager::SetSaveAs_MotionParam(const MOTION_PARAM& stMotion, CString strstrPathAndNamePath)
{
	m_stMotion = stMotion;
	SaveMotionParam(strstrPathAndNamePath);
}
void CDataManager::SetSaveAs_TesterCommParam(const TESTER_COMM& stTester, CString strPathAndName)
{
	m_stTester = stTester;
	SaveTesterCommData(strPathAndName);
}

void CDataManager::LoadErrorList_SolutionList()
{
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CString strErrorListFile;
	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("ERROR_LIST");
	CString strKey;

	strKey.Format(_T("TOTAL_ERROR_COUNT"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strErrorListFile);
	m_stError.nErrorCount = _ttoi(strTemp);

	// 에러카운트가 없으면 호출할 데이터가 없으므로.. 리턴하자.
	if (m_stError.nErrorCount != 0)
	{
		// 발생한 날짜의 에러를 받아오자.
		for (int i = 0; i < m_stError.nErrorCount; i++)
		{
			strKey.Format(_T("%03d_ErrNum"), i + 1);			// 에러번호 입력을 1번부터 한다.
			GetPrivateProfileString(strApp, strKey, _T("1000"), strTemp, sizeof(strTemp), strErrorListFile);
			m_stError.vtErrorNum.push_back(strTemp);
		}
	}

	// 에러명리스트를 받아오자.
	CString strLanguageFilePath = _T("");
	CString strList = _T("");
	CString strGetKey = _T("");
	strLanguageFilePath.Format(_T("%s\\Language\\Language.txt"), _PROGRAM_ROOT_DIR);

	
	int nStartErrorNum_Part1 = 1000;
	int nStartErrorNum_Part2 = 2000;
	int nStartErrorNum_Part3 = 3000;
	int nRepeatCnt = 0;

	m_stError.vtErr_Kor.clear();
	m_stError.vtErr_Eng.clear();
	m_stError.vtErr_Viet.clear();

	m_stError.vtSol_Kor.clear();
	m_stError.vtSol_Eng.clear();
	m_stError.vtSol_Viet.clear();

	if (m_ctrlLanguageFile.Open(strLanguageFilePath))
	{
		// 1000번대 읽어오기
		strGetKey.Format(_T("LANGUAGE_1000"));
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();
			
			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_1000 = nRepeatCnt;

			Sleep(10);
		}

		strGetKey.Format(_T("LANGUAGE_2000"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_2000 = nRepeatCnt;

			Sleep(10);
		}

		strGetKey.Format(_T("LANGUAGE_3000"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_3000 = nRepeatCnt;
			Sleep(10);
		}

		strGetKey.Format(_T("SOLUTION_1000"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("SOLUTION_%d_KOR"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("SOLUTION_%d_ENG"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("SOLUTION_%d_VIET"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nSolutionNameCount_1000 = nRepeatCnt;
			Sleep(10);
		}

		strGetKey.Format(_T("SOLUTION_2000"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("SOLUTION_%d_KOR"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("SOLUTION_%d_ENG"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("SOLUTION_%d_VIET"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nSolutionNameCount_2000 = nRepeatCnt;
			Sleep(10);
		}

		strGetKey.Format(_T("SOLUTION_3000"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("SOLUTION_%d_KOR"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("SOLUTION_%d_ENG"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("SOLUTION_%d_VIET"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtSol_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nSolutionNameCount_3000 = nRepeatCnt;
			Sleep(10);
		}
	}

}
void CDataManager::SaveErrorList_SolutionList()
{
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CString strErrorListFile;
	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	const int nLength = 256;

	CString strApp = _T("ERROR_LIST");
	CString strKey, strTemp, strTotal;

	strKey.Format(_T("TOTAL_ERROR_COUNT"));
	strTemp.Format(_T("%d"), m_stError.nErrorCount);
	WritePrivateProfileString(strApp, strKey, strTemp, strErrorListFile);


	if (m_stError.nErrorCount > 0)
	{
		strKey.Format(_T("%03d_ErrNum"), m_stError.nErrorCount);
		strTemp.Format(_T("%s"), m_stError.vtErrorNum.at( m_stError.nErrorCount - 1) );
		WritePrivateProfileString(strApp, strKey, strTemp, strErrorListFile);
	}

}
void CDataManager::SetErrorListData(const ERROR_LIST& stErr)
{
	m_stError = stErr;
	SaveErrorList_SolutionList();
}
ERROR_LIST*	CDataManager::GetCurrentErrorList()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stError;
}