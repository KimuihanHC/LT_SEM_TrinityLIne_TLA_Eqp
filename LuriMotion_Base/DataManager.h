#pragma once
#include "pch.h"
#include "User_Define.h"
#include "Log/INI.h"

class CDataManager : public CSington<CDataManager>
{
public:
	CDataManager(void);
	~CDataManager(void);

private:
	BOOL	m_bInit;
	
	EQ_BASIC_PARAM		m_stEqBasicParam;
	EQ_TOWERLAMP		m_stTowerLamp;
	MOTION_PARAM		m_stMotion;
	TEACHING_PARAM		m_stTeach;
	TESTER_COMM			m_stTester;

	ERROR_LIST			m_stError;

	ST_OptSemiAuto		m_stSemiAuto;
	
public:
	void	Init();

	
	// EqBasic dlg
	void	LoadEqBasicInfo();										// ���α׷� ���۽� Loading �ϴ� �⺻ �Ķ��Ÿ
	void	SaveEqBasicParam(const LPCTSTR& lpszPath);				// EqBasic ������.
	void	LoadEqBasicParam(const LPCTSTR& lpszPath);				// EqBasic ������.
	EQ_BASIC_PARAM* GetCurrentEqBasicData();						// EqBasic ������.
	void SetEqData(const EQ_BASIC_PARAM& stEqParam);				// EqBasic ������.

	// TowerLamp Dlg
	void	SaveTowerLamp(const LPCTSTR& lpszPath);					// Towerlamp 
	void	LoadTowerLamp(const LPCTSTR& lpszPath);					// Towerlamp
	EQ_TOWERLAMP* GetCurrentTowerLampData();						// Towerlamp .
	void SetstTower(const EQ_TOWERLAMP& stTowerLamp);				// Towerlamp .

	// Tester Comm Dlg
	void	SaveTesterCommData(const LPCTSTR& lpszPath);			// Tester Comm Dlg
	void	LoadTesterCommData(const LPCTSTR& lpszPath);			// Tester Comm Dlg
	TESTER_COMM*	GetCurrentTesterCommData();						// Tester Comm Dlg
	void SetTesterCommData(const TESTER_COMM& stTester);			// Tester Comm Dlg
	
	/// Motion Dlg
	void	SaveMotionParam(const LPCTSTR& lpszPath);				// Motor Speed 
	void	LoadMotionParam(const LPCTSTR& lpszPath);				// Motor Speed 
	MOTION_PARAM*	GetCurrentMotionParam();						// Motor Speed 
	void SetMotionParam(const MOTION_PARAM& stMotion);				// Motor Speed 
	void WriteChangesMotionParamToSemcoLog(MOTION_PARAM stOldMotion, MOTION_PARAM stNewMotion);
	
	// Teaching Dlg
	void	SaveTeachingPosData(const LPCTSTR& lpszPath);			// Teaching Pos
	void	LoadTeachingPosData(const LPCTSTR& lpszPath);			// Teaching Pos
	TEACHING_PARAM*	GetCurrentTeachingData();						// Teaching Pos
	void SetTeachingData(const TEACHING_PARAM& stTeaching);			// Teaching Pos
	void WriteChangesTeachingDataToSemcoLog(TEACHING_PARAM stOldTeaching, TEACHING_PARAM stNewTeaching);

	//semi auto type
	ST_OptSemiAuto*	GetSemiAuto();
	void SetSemiAuto(__in ST_OptSemiAuto opt);

	void RecipeChange(CString& strRecipeName);

	// SaveAs �϶� �����ϴ� ��ƾ
	void SetSaveAs_TeachingData(const TEACHING_PARAM& stTeaching, CString strPathAndName);			// Teaching Pos_SaveAs
	void SetSaveAs_MotionParam(const MOTION_PARAM& stMotion, CString strPathAndName);				// MotionParam
	void SetSaveAs_TesterCommParam(const TESTER_COMM& stTester, CString strPathAndName);			// TestesrComm Param

	CString m_strCurrenctRecipeName;

	// Error Solution ���� ����ϴ� Error LIst
	void	LoadErrorList_SolutionList();
	void	SaveErrorList_SolutionList();
	ERROR_LIST*	GetCurrentErrorList();						// Teaching Pos
	void	SetErrorListData(const ERROR_LIST& stErr);
	CINI	m_ctrlLanguageFile;
};