#ifndef _LOGSEMCO_H__
#define _LOGSEMCO_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "ExString.h"

#define LOG_BUFFER_SIZE		10240

class CLogSemco
{
private:
	CFile		m_CSVFile;
	CExString	m_strLogRootPath;
	CExString	m_strServiceName;
	int			m_nExpirePeriod;
	CExString	m_strCurrentLog;
	CTime		m_csvTime;

	CRITICAL_SECTION   m_csFile;

	BOOL		m_bStart;

protected:
	//BOOL OpenLogFile( CExString& strLogPathName );
	void DeleteExpiredCSVFile();
	BOOL OpenCSVFile(CExString& strCSVPathName);
	void GetCSVPathName(CTime& Time, CExString& strCSVPathName);

public:
	CLogSemco( LPCTSTR lpszServiceName );
	virtual ~CLogSemco();

	void SetLogDirectory(LPCTSTR lpszPath) { m_strLogRootPath = lpszPath; }
	void SetExpirePeriod(int nDay) { m_nExpirePeriod = nDay; }

	void CsvOut(LPCTSTR lpszId, LPCTSTR lpszFormat, ...);

	BOOL Start();
	BOOL Stop();

	void CreateFolder(LPCTSTR lpszPath);

	void Init();
	void Exit();
};

#endif // __X_LOGSYSTEM_H__