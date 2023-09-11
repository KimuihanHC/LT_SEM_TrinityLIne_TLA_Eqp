#pragma once

#include <afxwin.h>
#include "Def_ErrorCode.h"

typedef enum
{
	Ready = 0,
	Testing,
	Done,
	Bypass,
}enSocketStatus;

typedef struct _tagSocketInfo
{
	enSocketStatus status;
	CString szRFID;
	CString szBCR;

	bool	bBypass;
	bool	bResultOk;
	bool	bSEMCO_Ack;
	int		nSEMCO_Code;
	
	_tagSocketInfo()
	{
		Reset();
	};

	_tagSocketInfo& operator= (const _tagSocketInfo& ref)
	{
		szRFID	= ref.szRFID;
		szBCR	= ref.szBCR;
		bBypass = ref.bBypass;
		nSEMCO_Code = ref.nSEMCO_Code;
		bSEMCO_Ack	= ref.bSEMCO_Ack;
		bResultOk = ref.bResultOk;
		status = ref.status;
		return *this;
	};

	void Reset()
	{
		szRFID.Empty();
		szBCR.Empty();
		bBypass = false;
		bSEMCO_Ack = false;
		nSEMCO_Code = -1;
		bResultOk = false;
		status = Ready;
	};

	bool IsTestOk()
	{
		if (bSEMCO_Ack == true)
		{
			if (nSEMCO_Code == 0)
				return true;
		}

		return false;
	};

	void SetTestResult(__in int nCode)
	{
		nSEMCO_Code = nCode;
	};

}ST_SocketInfo, *PST_SocketInfo;

typedef struct _tagErrorInfo
{
	enMCErrCode code;
	CString		szReason;

	_tagErrorInfo()
	{
		Reset();
	};

	_tagErrorInfo& operator= (const _tagErrorInfo& ref)
	{
		code = ref.code;
		szReason = ref.szReason;
		return *this;
	};

	void Reset()
	{
		code = enMCErrCode::MCEC_OK;
		szReason.Empty();
	};

	void SetErrorCode(__in enMCErrCode err)
	{
		code = err;
	};

	void SetReason(__in CString sz)
	{
		szReason = sz;
	};

	enMCErrCode GetErrorCode()
	{
		return code;
	};

	CString GetReason()
	{
		return szReason;
	};

}ST_ErrorInfo, *PST_ErrorInfo;