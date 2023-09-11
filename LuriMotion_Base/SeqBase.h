#pragma once

#include "Motion.h"
#include "Device.h"
#include "UpdateUI.h"

typedef struct
{
	LPVOID		pOwner;
	UINT		nPort;
	UINT		nArg;
}stThreadParam;

class CSeqBase : public CDevice, public CUpdateUI, public CMotion
{
public:
	CSeqBase();
	~CSeqBase();

	static UINT Thread_Run(__in LPVOID lParam);

	void StartTimer();
	DWORD GetTimer();
	DWORD GetElapTime();	

	int GetStep();	
	int GetOldStep();

	bool IsForceStop();
	bool IsError();
	void Error(__in enMCErrCode err);	
	void Error(__in enMCErrCode err, __in CString szReason);
	void ErrorClear();

	CString GetErrorReason();
	void SetErrorReason(__in CString sz);

	virtual bool Start();
	virtual bool Stop();
	virtual bool ForceStop();

	bool DeleteThread();

	virtual void Reset();

	virtual	UINT RunProcess						()	= 0;
	virtual void Initialize_Step				();
	virtual enMCErrCode StartOperation_Step		()	= 0;
	virtual void Finalize_Step					();

	inline enMCErrCode GetErrorCode()
	{
		return m_error.code;
	}

	inline ST_ErrorInfo GetError()
	{
		return m_error;
	}

	void Lock();
	void UnLock();
	void Wait();

	inline bool IsChangeStep()
	{
		if (m_nStep != m_nOldStep)
		{
			SetStatus(enSeqStatus::Status_Ready);
			m_nOldStep = m_nStep;
			return true;
		}
		else
		{
			return false;
		}
	}

	inline void SetStatus(__in enSeqStatus status)
	{
		m_status = status;
	};

	inline enSeqStatus GetStatus()
	{
		return m_status;
	};

	inline bool IsStatusComplete()
	{
		if (m_status == enSeqStatus::Status_Complete)
			return true;
		else
			return false;
	};

	bool IsStatusComplete(__in int nStep);	

	bool IsThreadStart();
	bool IsThreadStop();

	void Log(__in LPCTSTR szLog, ...);
	void SemcoLog(__in LPCTSTR szLog, ...);

	LPCTSTR szLogBK;
	int nJobCheck[6];// 0 : Lock , 1: Power , 2: Preview, 3:OC 4:vigne 46 5:vigne82


protected:
	void SetStep(__in int nStep);

private:
	DWORD m_dwTimer;
	int m_nStep;
	bool m_bMoveComplete;
	bool m_bError;

	int m_nOldStep;

	enSeqStatus m_status;

	ST_ErrorInfo	m_error;

	bool		m_bThreadStart;
	bool		m_bThreadStop;
	bool		m_bForceStop;


	HANDLE		m_hThreadStart;
	HANDLE		m_hExecuteEvent;
};

