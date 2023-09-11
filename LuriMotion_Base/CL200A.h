#pragma once

#if(SET_INSPECTOR == SYS_HOTPIXEL)
#include "Singleton.h"
#include "BaseChromaMeter.h"
#include "Def_Chroma.h"

class CCL200A : public CBaseChromaMeter, public CSington<CCL200A>
{
private:
	CCL200A(const CCL200A &) = delete;
	CCL200A & operator=(const CCL200A &) = delete;

public:
	CCL200A();
	~CCL200A();

	enCL200AErr GetLastError	(__out LPCTSTR& sz);

	BOOL Connect				(UINT InPort, DWORD inBaudRate, BYTE inParity, BYTE inStopBits, BYTE inByteSize);
	BOOL Disconnect				();
	BOOL SetPCConnectMode		(__in BOOL bConnect = TRUE);
	BOOL SetMeasureMode			(__in eHeadNumber enHeader, __out CHROMA_PACKET& pk);
	BOOL GetMeasureData			(__in eHeadNumber enHeader, __in eCommandType enCmd, __out CHROMA_PACKET& pk);

	BOOL IsPCMeasureOn()		{ return m_bPCMeasureOn; }

protected:
	UINT		m_nErrorCode = 0;

	enCL200AErr	m_Error = Cl200A_Err_OK;

	BOOL		m_bPCConnectOn = FALSE;
	BOOL		m_bPCMeasureOn = FALSE;
};
#endif
