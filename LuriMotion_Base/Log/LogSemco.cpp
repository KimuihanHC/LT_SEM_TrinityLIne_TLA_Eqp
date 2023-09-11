#include "../pch.h"

#include "LogSemco.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// CLogSemco
//////////////////////////////////////////////////////////////////////

CLogSemco::CLogSemco( LPCTSTR lpszServiceName ) :
	m_nExpirePeriod(-1), m_strServiceName(lpszServiceName),
	m_bStart(FALSE),
	m_csvTime(CTime::GetCurrentTime())
{
	InitializeCriticalSection(&m_csFile);
}

CLogSemco::~CLogSemco()
{
	if (m_bStart)
		Stop();

	DeleteCriticalSection(&m_csFile);
}

void CLogSemco::GetCSVPathName(CTime& Time, CExString& strCSVPathName)
{
	int nYear = Time.GetYear();
	int nMonth = Time.GetMonth();
	int nDay = Time.GetDay();

	strCSVPathName.Format(_T("%s\\%04d%02d%02d.csv"), m_strLogRootPath, nYear, nMonth, nDay);
}

BOOL CLogSemco::Start()
{
	if (m_strLogRootPath.IsEmpty())
		return FALSE;

	CreateFolder(m_strLogRootPath);

	DeleteExpiredCSVFile();

	m_csvTime = CTime::GetCurrentTime();

	CExString strLogPathName, strExcelPathName, strCSVPathName;
	GetCSVPathName(m_csvTime, strCSVPathName);

	if (!OpenCSVFile(strCSVPathName))
		return FALSE;

	m_bStart = TRUE;

	return TRUE;
}

BOOL CLogSemco::Stop()
{
	m_CSVFile.Close();
	m_bStart = FALSE;
	return TRUE;
}

BOOL CLogSemco::OpenCSVFile(CExString& strCSVPathName)
{
	if (!m_CSVFile.Open(strCSVPathName, CFile::modeRead | CFile::shareDenyWrite))
	{
		if (!m_CSVFile.Open(strCSVPathName, CFile::modeCreate | CFile::modeWrite))
			return FALSE;

		USES_CONVERSION;
	}
	m_CSVFile.Close();

	if (!m_CSVFile.Open(strCSVPathName, CFile::modeWrite | CFile::shareDenyNone))
		return FALSE;

	m_CSVFile.SeekToEnd();

	return TRUE;
}

void CLogSemco::CsvOut(LPCTSTR lpszId, LPCTSTR lpszFormat, ...)
{
	EnterCriticalSection(&m_csFile);
	if (lpszFormat == NULL || !m_bStart)
	{
		LeaveCriticalSection(&m_csFile);
		return;
	}

	USES_CONVERSION;

	va_list argList;
	va_start(argList, lpszFormat);

	TCHAR lptszBuffer[LOG_BUFFER_SIZE];
	VERIFY(_vsntprintf(lptszBuffer, LOG_BUFFER_SIZE, lpszFormat, argList) <= LOG_BUFFER_SIZE);

	va_end(argList);

	CExString strLog = lptszBuffer;

	do
	{
		//CxCriticalSection::Owner Lock(m_csFile);

		if (m_CSVFile.m_hFile == NULL) break;

		CTime CurTime = CTime::GetCurrentTime();
		if ((CurTime.GetYear() != m_csvTime.GetYear()) ||
			(CurTime.GetMonth() != m_csvTime.GetMonth()) ||
			(CurTime.GetDay() != m_csvTime.GetDay()))
		{
			m_CSVFile.Close();
			CExString strCSVPathName;

			GetCSVPathName(CurTime, strCSVPathName);

			if (!OpenCSVFile(strCSVPathName))
			{
				break;
			}

			DeleteExpiredCSVFile();

			m_csvTime = CurTime;
		}

		int nLength = strLog.GetLength();
		CString EndLineCheck = strLog.Right(1);
		CExString strCurrentCsv;
		if (EndLineCheck != _T("\n"))
		{
			strCurrentCsv.Format(L"%s,%s\r\n", lpszId, strLog);
		}
		else
			strCurrentCsv.Format(L"%s,%s\r\n", lpszId, strLog);

		if (lpszId == NULL) { lpszId = _T("internal"); }

		if (m_CSVFile.m_hFile != NULL)
			m_CSVFile.Write(T2A((LPTSTR)(LPCTSTR)strCurrentCsv), strCurrentCsv.GetLength());

	} while (FALSE);

	LeaveCriticalSection(&m_csFile);
}

void CLogSemco::DeleteExpiredCSVFile()
{
	if (m_nExpirePeriod < 0)
		return;

	CExString strFilter;
	strFilter.Format(_T("%s\\%s*.csv"), m_strLogRootPath, m_strServiceName);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(strFilter, &FindFileData);

	CExString strFileName;
	CExString strDate;
	while (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			strFileName = FindFileData.cFileName;
			strDate = FindFileData.cFileName;

			CExString strYear = strDate.Mid(0, 4);
			if (strYear.IsEmpty()) break;

			CExString strMonth = strDate.Mid(4, 2);
			if (strMonth.IsEmpty()) break;

			CExString strDay = strDate.Mid(6, 2);
			if (strDay.IsEmpty()) break;

			int nYear, nMonth, nDay;
			nYear = _ttoi(strYear);
			nMonth = _ttoi(strMonth);
			nDay = _ttoi(strDay);

			CTime FindFileTime(nYear, nMonth, nDay, 0, 0, 0);
			CTimeSpan TimeSpan = m_csvTime - FindFileTime;
			if (TimeSpan.GetDays() >= m_nExpirePeriod)
			{
				CExString strFullPathName = m_strLogRootPath;
				strFullPathName += _T('\\');
				strFullPathName += strFileName;
				DeleteFile(strFullPathName);
			}
		} while (FALSE);

		if (!FindNextFile(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

void CLogSemco::CreateFolder(LPCTSTR lpszPath)
{
	CString strPath = lpszPath;

	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;

	while ((nEnd = strPath.Find('\\', nStart)) >= 0)
	{
		{
			CString csToken = strPath.Mid(nStart, nEnd - nStart);
			CreateDirectory(csPrefix + csToken, NULL);

			csPrefix += csToken;
			csPrefix += _T("/");
			nStart = nEnd + 1;
		}
	}

	csToken = strPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);
}

void  CLogSemco::Init()
{
	CString strTime;

	SYSTEMTIME tm;
	GetLocalTime(&tm);
	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	CString strLog;
	strLog.Format(_T("M,-,-,Idle,EVENT:PROGRAM_START,Start,"));

	CsvOut(strTime, strLog);
}

void CLogSemco::Exit()
{
	CString strTime;

	SYSTEMTIME tm;
	GetLocalTime(&tm);
	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	CString strLog;
	strLog.Format(_T("M,-,-,Idle,EVENT:PROGRAM_END,Start,"));

	CsvOut(strTime, strLog);
}
