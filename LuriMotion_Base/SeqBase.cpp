#include "pch.h"
#include "SeqBase.h"
#include <strsafe.h>

#include "LuriMotion_BaseDlg.h"

CSeqBase::CSeqBase()
{
	m_hThreadStart = NULL;
	m_bThreadStart = false;
	m_bThreadStop = false;
	m_bForceStop = true;

	m_status = Status_Ready;
	szLogBK = _T("");
}

CSeqBase::~CSeqBase()
{
}

void CSeqBase::StartTimer()
{
	m_dwTimer = GetTickCount();
};

DWORD CSeqBase::GetTimer()
{
	return m_dwTimer;
}

DWORD CSeqBase::GetElapTime()
{
	return GetTickCount() - GetTimer();
}

void CSeqBase::SetStep(__in int nStep)
{
	SetStatus(enSeqStatus::Status_Ready);
	m_nStep = nStep;
	UnLock();
};

int CSeqBase::GetStep()
{
	return m_nStep;
}

int CSeqBase::GetOldStep()
{
	return m_nOldStep;
}

bool CSeqBase::IsForceStop()
{
	return m_bForceStop;
}

bool CSeqBase::IsError()
{
	return m_bError;
};

void CSeqBase::Error(__in enMCErrCode err)
{
	m_bError = true;
	m_error.code = err;
}

void CSeqBase::Error(__in enMCErrCode err, __in CString szReason)
{
	m_bError = true;
	m_error.code = err;
	m_error.szReason = szReason;
}

void CSeqBase::ErrorClear()
{
	m_bError = false;
	m_error.Reset();
}

CString CSeqBase::GetErrorReason()
{
	return m_error.szReason;
}

void CSeqBase::SetErrorReason(__in CString sz)
{
	m_error.szReason = sz;
}

bool CSeqBase::Start()
{
	bool bRet = true;

	if (!m_bForceStop)
		return true;

	if (m_hThreadStart)
	{
		CloseHandle(m_hThreadStart);
		m_hThreadStart = NULL;
	}		

	// 
	if (m_hThreadStart == NULL)
	{
		m_bThreadStart	= true;
		m_bForceStop	= false;
		m_bThreadStop = false;
		m_hThreadStart	= HANDLE(_beginthreadex(NULL, 0, Thread_Run, this, 0, NULL));

//		m_hExecuteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		UnLock();
	}
	else
	{
		m_bThreadStart	= true;
		m_bForceStop	= false;
		m_bThreadStop	= false;
	}

	StartTimer();
	m_nOldStep = 0;
	
	return bRet;
}

bool CSeqBase::Stop()
{
	bool bRet = true;
	m_bThreadStop = true;
	m_bForceStop = false;
	return bRet;
}

bool CSeqBase::ForceStop()
{
	bool bRet = true;
	m_bForceStop = true;
	m_bThreadStart = false;
	return bRet;
}

bool CSeqBase::DeleteThread()
{
	if (m_hThreadStart)
	{
		DWORD dwExitCode = NULL;

		while (1)
		{
			GetExitCodeThread(m_hThreadStart, &dwExitCode);

			if (STILL_ACTIVE != dwExitCode)
				break;

			Sleep(10);
		}

		CloseHandle(m_hThreadStart);
		m_hThreadStart = NULL;
		m_bThreadStop = FALSE;
	}
	return true;
}

void CSeqBase::Reset()
{
	m_nStep = 0;
	ErrorClear();
}

void CSeqBase::Initialize_Step()
{

}

void CSeqBase::Finalize_Step()
{

}

void CSeqBase::Lock()
{
	ResetEvent(m_hExecuteEvent);
}

void CSeqBase::UnLock()
{
	SetEvent(m_hExecuteEvent);
}

void CSeqBase::Wait()
{
	WaitForSingleObject(m_hExecuteEvent, INFINITE);
}

bool CSeqBase::IsStatusComplete(__in int nStep)
{
	if (m_nStep == nStep && m_status == enSeqStatus::Status_Complete)
		return true;
	else
		return false;
}

bool CSeqBase::IsThreadStart()
{
	return  m_bThreadStart;
}

bool CSeqBase::IsThreadStop()
{
	return m_bThreadStop;
}

void CSeqBase::Log(__in LPCTSTR szLog, ...)
{
	if (szLogBK != szLog) // 같은 로그가 찍히지 않도록 
	{
		szLogBK = szLog;
		TCHAR *lpszBuffer = new TCHAR[4096];

		size_t cb = 0;
		va_list args;
		va_start(args, szLog);
		::StringCchVPrintfEx(lpszBuffer, 4096, NULL, &cb, 0, szLog, args);
		va_end(args);

		WRITE_MAIN_LOG(lpszBuffer);

		delete[] lpszBuffer;
	}

}

void CSeqBase::SemcoLog(__in LPCTSTR szCsv, ...)
{
	TCHAR *lpszBuffer = new TCHAR[4096];

	size_t cb = 0;
	va_list args;
	va_start(args, szCsv);
	::StringCchVPrintfEx(lpszBuffer, 4096, NULL, &cb, 0, szCsv, args);
	va_end(args);

	WRITE_SEMCO_LOG(lpszBuffer);

	delete[] lpszBuffer;
}

UINT CSeqBase::Thread_Run(__in LPVOID lParam)
{
	CSeqBase* pThis = (CSeqBase*)lParam;
	pThis->RunProcess();
	return 0;
}