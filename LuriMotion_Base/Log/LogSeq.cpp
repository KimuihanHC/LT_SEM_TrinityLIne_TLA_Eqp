#include "../pch.h"

#include "LogSeq.h"


#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// CLogSeq
//////////////////////////////////////////////////////////////////////

CLogSeq::CLogSeq( LPCTSTR lpszServiceName ) :
	m_nExpirePeriod(-1), m_strServiceName(lpszServiceName), 
	m_bStart(FALSE), 
	m_bMonthlySplitFolder(FALSE), m_bUseTxtFile(TRUE), m_bUseCSVFile(FALSE),
	m_Time(CTime::GetCurrentTime())
{
}

CLogSeq::~CLogSeq()
{
	if (m_bStart)
		Stop();
}

void CLogSeq::GetLogPathName( CTime& Time, CExString& strLogPathName )
{
	int nYear = Time.GetYear();
	int nMonth = Time.GetMonth();
	int nDay = Time.GetDay();
	
	strLogPathName.Format( _T("%s\\%s_%04d_%02d_%02d.txt"), m_strLogRootPath, m_strServiceName, nYear, nMonth, nDay );
}

void CLogSeq::GetCSVPathName( CTime& Time, CExString& strCSVPathName )
{
	int nYear = Time.GetYear();
	int nMonth = Time.GetMonth();
	int nDay = Time.GetDay();
	
	strCSVPathName.Format( _T("%s\\%s_%04d_%02d_%02d.csv"), m_strLogRootPath, m_strServiceName, nYear, nMonth, nDay );
}

void CLogSeq::UseCSVFile( BOOL bUse, LPCTSTR lpszCSVHeader )
{
	m_bUseCSVFile = bUse;
	m_strCSVHeader.Format( _T("%s\r\n"), lpszCSVHeader );
}

void CLogSeq::UseTextFile( BOOL bUse )
{
	m_bUseTxtFile = bUse;
}

void CLogSeq::UseMonthlySplitFolder( BOOL bUse )
{
	m_bMonthlySplitFolder = bUse;
}

BOOL CLogSeq::Start()
{
	if ( m_strLogRootPath.IsEmpty() )
		return FALSE;

	CreateFolder( m_strLogRootPath );

	DeleteExpiredLogFile();
	DeleteExpiredCSVFile();
	
	m_Time = CTime::GetCurrentTime();

	CExString strLogPathName, strExcelPathName, strCSVPathName;
	GetLogPathName( m_Time, strLogPathName );
	GetCSVPathName( m_Time, strCSVPathName );
	
	if ( !OpenLogFile(strLogPathName) )
		return FALSE;

	if ( !OpenCSVFile( strCSVPathName ) )
		return FALSE;


	m_bStart = TRUE;
	
	return TRUE;
}

BOOL CLogSeq::Stop()
{
	{
		//CxCriticalSection::Owner Lock(m_csFile);
		m_File.Close();
		m_CSVFile.Close();
	}

	m_bStart = FALSE;
	return TRUE;
}

BOOL CLogSeq::OpenLogFile( CExString& strLogPathName )
{
	if ( !m_bUseTxtFile )
		return TRUE;

	USES_CONVERSION;

	if ( !m_File.Open(strLogPathName, CFile::modeRead|CFile::shareDenyWrite) )
	{
		if ( !m_File.Open(strLogPathName, CFile::modeCreate|CFile::modeWrite) )
			return FALSE;

		CTime CurTime = CTime::GetCurrentTime();
		CExString strTime;
		strTime.Format( _T("%04d/%02d/%02d"), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay() );
		CExString strComment;
		strComment.Format( _T("#################### Luritech Seq Log ####################\r\n")
						   _T("- Created by LogCommunication\r\n")
						   _T("- Service Name: %s\r\n")
						   _T("- Date: %s\r\n")
						   _T("#########################################################\r\n")
						  , m_strServiceName, strTime );
		m_File.Write( T2A( (LPTSTR)(LPCTSTR)strComment ), strComment.GetLength() );
	}
	m_File.Close();
	
	if ( !m_File.Open(strLogPathName, CFile::modeWrite|CFile::shareDenyWrite) )
		return FALSE;

	m_File.SeekToEnd();

	return TRUE;
}

BOOL CLogSeq::OpenCSVFile( CExString& strCSVPathName )
{
	if ( !m_bUseCSVFile )
		return TRUE;
	
	if ( !m_CSVFile.Open(strCSVPathName, CFile::modeRead|CFile::shareDenyWrite) )
	{
		if ( !m_CSVFile.Open(strCSVPathName, CFile::modeCreate|CFile::modeWrite) )
			return FALSE;

		USES_CONVERSION;

		if ( !m_strCSVHeader.IsEmpty() )
			m_CSVFile.Write( T2A((LPTSTR)(LPCTSTR)m_strCSVHeader), m_strCSVHeader.GetLength() );
	}
	m_CSVFile.Close();
	
	if ( !m_CSVFile.Open(strCSVPathName, CFile::modeWrite|CFile::shareDenyNone) )
		return FALSE;
	
	m_CSVFile.SeekToEnd();
	
	return TRUE;
}

void CLogSeq::LogOut( LPCTSTR lpszId, LPCTSTR lpszFormat, ... )
{
	if ( lpszFormat == NULL || !m_bStart )
	{
		return;
	}

	USES_CONVERSION;
	
	va_list argList;
	va_start(argList, lpszFormat);
	
	TCHAR lptszBuffer[LOG_BUFFER_SIZE];
	VERIFY(_vsntprintf(lptszBuffer, LOG_BUFFER_SIZE, lpszFormat, argList) <= LOG_BUFFER_SIZE );

	va_end(argList);

	CExString strLog = lptszBuffer;

	do 
	{
		//CxCriticalSection::Owner Lock(m_csFile);

		if ( m_File.m_hFile == NULL ) break;

		CTime CurTime = CTime::GetCurrentTime();
		if ( (CurTime.GetYear() != m_Time.GetYear()) ||
			(CurTime.GetMonth() != m_Time.GetMonth()) ||
			(CurTime.GetDay() != m_Time.GetDay()) )
		{
			m_File.Close();
			m_CSVFile.Close();
			CExString strLogPathName;
			CExString strCSVPathName;
	
			GetCSVPathName( CurTime, strCSVPathName );
			GetLogPathName( CurTime, strLogPathName );
			
			if ( !OpenLogFile(strLogPathName) )
			{
				break;
			}

			if ( !OpenCSVFile( strCSVPathName ) )
			{
				break;
			}

			DeleteExpiredLogFile();
			DeleteExpiredCSVFile();

			m_Time = CurTime;
		}

		int nLength = strLog.GetLength();
		CString EndLineCheck = strLog.Right(1);
		if (EndLineCheck != _T("\n"))
		{
			m_strCurrentLog.Format(L"%s,%s\n\r\n", lpszId, strLog);
		}
		else
			m_strCurrentLog.Format(L"%s,%s\r\n", lpszId, strLog);

		if ( lpszId == NULL ) { lpszId = _T("internal"); }

		if ( m_bUseCSVFile )
		{
			if ( m_CSVFile.m_hFile != NULL )
				m_CSVFile.Write( T2A((LPTSTR)(LPCTSTR)m_strCurrentLog), m_strCurrentLog.GetLength() );
		}

		if ( m_bUseTxtFile )
			m_File.Write( T2A((LPTSTR)(LPCTSTR)m_strCurrentLog), m_strCurrentLog.GetLength() );

	} while ( FALSE );
}

void CLogSeq::DeleteExpiredCSVFile()
{
	if ( m_nExpirePeriod < 0 || !m_bUseCSVFile )
		return;
	
	CExString strFilter;
	strFilter.Format( _T("%s\\%s*.csv"), m_strLogRootPath, m_strServiceName );
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
			
			CExString strServiceName = strDate.ExtractLeft( _T('_') );
			
			CExString strYear = strDate.ExtractLeft( _T('_') );
			//CString	  strYear_t = strDate.ExtractLeft(_T('_'));
			if (!isalnum(_ttoi(strYear)))
				strYear = strDate.ExtractLeft(_T('_'));
			if ( strYear.IsEmpty() ) break;
			
			CExString strMonth = strDate.ExtractLeft( _T('_') );
			//CString	  strMonth_t = strDate.ExtractLeft(_T('_'));
			if ( strMonth.IsEmpty() ) break;
			
			CExString strDay = strDate.ExtractLeft( _T('.') );
			//CString	  strDay_t = strDate.ExtractLeft(_T('_'));
			if ( strDay.IsEmpty() ) break;
			
			int nYear, nMonth, nDay;
			nYear = _ttoi(strYear);
			nMonth = _ttoi(strMonth);
			nDay = _ttoi(strDay);
			
			CTime FindFileTime( nYear, nMonth, nDay, 0, 0, 0 );
			CTimeSpan TimeSpan = m_Time - FindFileTime;
			if ( TimeSpan.GetDays() >= m_nExpirePeriod )
			{
				CExString strFullPathName = m_strLogRootPath;
				strFullPathName += _T('\\');
				strFullPathName += strFileName;
				DeleteFile( strFullPathName );
			}
		} while ( FALSE );
		
		if ( !FindNextFile( hFind, &FindFileData ) )
			break;
	}
	
	FindClose( hFind );
}

void CLogSeq::DeleteExpiredLogFile()
{
	if ( m_nExpirePeriod < 0 || !m_bUseTxtFile )
		return;

	CExString strFilter;
	strFilter.Format( _T("%s\\%s*.txt"), m_strLogRootPath, m_strServiceName );
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

			CExString strServiceName = strDate.ExtractLeft( _T('_') );

			CExString strYear = strDate.ExtractLeft( _T('_') );
			//CString	  strYear_t = strDate.ExtractLeft(_T('_'));
			if( !isalnum( _ttoi(strYear) ) )
				strYear = strDate.ExtractLeft(_T('_'));

			if ( strYear.IsEmpty() ) break;

			CExString strMonth = strDate.ExtractLeft( _T('_') );
			//CString	  strMonth_t = strDate.ExtractLeft(_T('_'));
			if ( strMonth.IsEmpty() ) break;

			CExString strDay = strDate.ExtractLeft( _T('.') );
			//CString	  strDay_t = strDate.ExtractLeft(_T('_'));
			if ( strDay.IsEmpty() ) break;

			int nYear, nMonth, nDay;
			nYear = _ttoi(strYear);
			nMonth = _ttoi(strMonth);
			nDay = _ttoi(strDay);

			CTime FindFileTime( nYear, nMonth, nDay, 0, 0, 0 );
			CTimeSpan TimeSpan = m_Time - FindFileTime;
			if ( TimeSpan.GetDays() >= m_nExpirePeriod )
			{
				CExString strFullPathName = m_strLogRootPath;
				strFullPathName += _T('\\');
				strFullPathName += strFileName;
				DeleteFile( strFullPathName );
			}
		} while ( FALSE );
		
		if ( !FindNextFile( hFind, &FindFileData ) )
			break;
	}

	FindClose( hFind );
}




void CLogSeq::CreateFolder(LPCTSTR lpszPath)
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