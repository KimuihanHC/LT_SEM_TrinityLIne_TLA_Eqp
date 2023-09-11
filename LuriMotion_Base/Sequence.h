#pragma once

#include "pch.h"
#include "User_Define.h"

#include "Def_Sequence.h"

#include "UpdateUI.h"
#include "Device.h"
#include "Motion.h"

#include "SeqStartupCheck.h"
#include "SeqSemiAuto.h"
#include "SeqConveyor1.h"
#include "SeqConveyor2.h"
#include "SeqPickerMgr.h"
#include "SeqConveyorReturn.h"

// [21.1029.4] Modified
enum RUN_MODE		{ eNone =-1, eStandby =0, eRunning, ePause, eError, eStop, eEStop, eInitializing };
enum INIT_STATUS	{ eInit_None, eInit_Working, eInit_Done };
enum TRAY_STATUS	{ eTray_None, eTray_Before, eTray_Working, eTray_Done };

class CSequence : public CWnd, public CSington<CSequence>, public CUpdateUI, public CDevice, public CMotion
{

public:
	CSequence();
	virtual ~CSequence();

	enum ETHREAD_ID
	{
		THREAD_RUN = 0,
		THREAD_MANUAL,
		THREAD_INITIAL,
		THREAD_INTLK,
		THREAD_MAX
	};
	int PickMgrStep();
	void PickMgrSetNextStep(__in enSeqPickerMgrStep step);
	void LogSeq(__in CString sz);
	void LogError(__in CString sz);
	int PickMgrJob0();
	int PickMgrJob1();
	int PickMgrJob2();
	int PickMgrJob3();
	int PickMgrJob4();
	void DoEvents();
	void DoEvents(DWORD dwMiliSeconds);


	enMCErrCode CheckMCError();

	bool GetSocketInfo(__in ESOCKET_POS_LIST spl, __out SOCKET_INFO & socket);
//	bool GetSocketInfo(__in SocketPos pos, __out ST_SocketInfo & socket);

	INIT_STATUS GetInitStatus();
	void SetInitStatus(INT16 is);
	int GetStpInitial();
	void SetStpInitial(int step);
	void Start_Initial();
	int	m_nbuttonFlagIs;
	int	m_nStop_buttonFlagIs;

	int nADJUST_Reverse;
	inline bool IsThread_SemiAuto()
	{
		return m_bThreadSemiAuto;
	};

	inline bool IsThread_StartupCheck()
	{
		return m_bThreadStartupCheck;
	};
private:
	
	BOOL		m_bThreadRun;
	CWinThread*	m_pThreadRun;
	BOOL		m_bThreadRunStop;
	CWinThread*	m_pThreadRunStop;

	static UINT	Thread_Run			(LPVOID lpParam);
	static UINT	Thread_RunStop		(LPVOID lpParam);

	static UINT Thread_SemiAuto		(__in LPVOID lParam);
	static UINT Thread_StartupCheck	(__in LPVOID lParam);

	void Run_Conveyor1			();
	void Run_Conveyor2			();
	void Run_ConveyorReturn		();
	void Run_Picker				();
	void Run_SemiAuto			();
	void Run_StartupCheck		();

	bool IsTesting_AutoRun		();
	bool IsTesting_SemiAuto		();
	bool IsTesting_StartupCheck	();	

	

	void		Initialize_Run();
	enMCErrCode StartOperation_Run				();
	void		Finalize_Run					(__in enMCErrCode err);
	bool		IsAutoRunStop					();

	void		Initialize_SemiAuto				(__in InspectionPortType port, __in UINT nIdx);
	enMCErrCode	StartOperation_SemiAuto			();
	void		Finalize_SemiAuto				(__in enMCErrCode err, __in int nIdx);

	void		Initialize_StartupCheck			(__in InspectionPortType port, __in UINT nIdx);
	enMCErrCode	StartOpertation_StartupCheck	();
	void		Finalize_StartupCheck			(__in enMCErrCode err, __in int nIdx);

	enMCErrCode CheckStart_Auto					();
	enMCErrCode CheckStart_SemiAuto				();
	enMCErrCode CheckStart_StartupCheck			();

	INT16	m_bRunMode;

	int		m_nFlareShotCnt;				// 검사기 검사 횟수

// 	CSeqConveyor1			m_SeqConveyor1;
// 	CSeqConveyor2			m_SeqConveyor2;
// 	CSeqConveyorReturn		m_SeqConveyorReturn;
	CSeqPickerMgr			m_SeqPickerMgr;

	// 세미오토 모드 동작
	CSeqSemiAuto			m_SeqSemiAuto;
	HANDLE					m_hThreadSemiAuto;
	bool					m_bThreadSemiAuto;

	// 시업점검 모드
	HANDLE					m_hThreadStartupCheck;
	bool					m_bThreadStartupCheck;
	CSeqStartupCheck		m_SeqStartupCheck;
	
	INT16   m_nInitStatus;
	stRunInfo	m_RunInfo[THREAD_MAX];

	INT16	m_nRunMode;

protected:
	BOOL		m_bThread[THREAD_MAX];
	CWinThread	*m_pThread[THREAD_MAX];

	static UINT	Thread_Initial(LPVOID lpParam);

	bool Begin_Thread(ETHREAD_ID tid);
	void End_Thread(ETHREAD_ID tid);

	BOOL	Run_Total_Origin();
	
public:
	bool Begin_RunThread();
	void End_RunThread();	
	bool Begin_RunThreadStop();
	void End_RunThreadStop();

	void Start_Run();
	void Stop_Run();
	void Stop_Run(RUN_MODE rm);
	void Pause_Run();
	
	void Emergency_Run();
	void Init_Compleate();
	void Reset_SeqFlags();

	// Getter Setter
	RUN_MODE	GetRunMode();
	void		SetRunMode(INT16 rm);

	BOOL		IsThreadRun() const;
	void		SetThreadRun(BOOL val);

	CWinThread* GetWinThreadRun() const;
	void		SetWinThreadRun(CWinThread * val);

	BOOL		IsThreadRunStop() const;
	void		SetThreadRunStop(BOOL val);

	CWinThread* GetWinThreadRunStop() const;
	void		SetWinThreadRunStop(CWinThread * val);

	int			GetFlareShotCnt() const;
	void		SetFlareShotCnt(int val);

	BOOL		Get_ThreadRun() { return IsThreadRun(); }

	void		Start_SemiAuto		(__in InspectionPortType port, __in int nIdx);
	void		Stop_SemiAuto		();

	void		Start_StartupCheck	(__in InspectionPortType port, __in int nIdx);
	void		Stop_StartupCheck	();

	inline BOOL IsDryRun()
	{
		return (COMMON->GetOperationMode() == OperationMode::DryRun) ? TRUE : FALSE;
	};

	inline BOOL IsSemiAuto()
	{
		return (COMMON->GetOperationMode() == OperationMode::SemiAuto) ? TRUE : FALSE;
	};

// 	CSeqConveyor1*		GetPtrSeqConveyor1()
// 	{
// 		return &m_SeqConveyor1;
// 	};
// 
// 	CSeqConveyor2*		GetPtrSeqConveyor2()
// 	{
// 		return &m_SeqConveyor2;
// 	};
// 
// 	CSeqConveyorReturn*		GetPtrSeqConveyorReturn()
// 	{
// 		return &m_SeqConveyorReturn;
// 	};

	CSeqPickerMgr*		GetPtrSeqPickerMgr()
	{
		return &m_SeqPickerMgr;
	};
	public:
		INT nPASS_NG;
		int nhomeprogress;
		CString StrNG;

};
