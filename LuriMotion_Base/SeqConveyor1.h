#pragma once

#include "SeqBase.h"
#include "Def_DigitalIO.h"
#include "User_Define.h"

#include <afxtempl.h>
#include <list>

class CSeqConveyor1 : public CSeqBase
{
public:
	CSeqConveyor1();
	~CSeqConveyor1();

	virtual	void Reset();

private:
	bool WaitForDetectSignal		(__in enConv1SensorSelect sensor, __in bool bStatus = true);
	bool WaitForSocketToOut			();

public:
	inline void ResetAccept()
	{
		m_Accept = 0;
	}

	inline void SetAccept(__in int Accept)
	{
		m_Accept = Accept;

		if (Accept == 3)
		{
// 			ST_SocketInfo * socket;
// 			socket = &m_list.front();
// 			socket->bBypass = true;
			m_SocketInput.bBypass = true;
		}
	};

	inline int GetAccept()
	{
		return m_Accept;
	};

	inline void SetBarcode(__in CString sz)
	{
// 		if (m_list.empty())
// 			return;

		if (m_bSocketEmptyInput)
			return;
		
// 		ST_SocketInfo * socket;
// 		socket = &m_list.front();
// 		socket->szBCR = sz;		
		m_SocketInput.szBCR = sz;
	};

	inline void SetRFID(__in CString sz)
	{
// 		if (m_list.empty())
// 			return;
// 
// 		ST_SocketInfo * socket;
// 		socket = &m_list.front();
// 		socket->szRFID = sz;

		if (m_bSocketEmptyInput)
			return;

		m_SocketInput.szRFID = sz;
	};

	inline void SetNextStep(__in enSeqCv1Step step)
	{
		SetStep(step);
	};
	
	inline bool IsInputSocket()
	{
		if (!m_bSocketEmptyInput)
			return true;
		else
			return false;
	};

	inline bool IsInputBufferSocket()
	{
		if (!m_bSocketEmptyBuffer)
			return true;
		else
			return false;
	};

	inline void InputSocket()
	{
		ST_SocketInfo socket;
		socket.szRFID = _T("");
		socket.szBCR = _T("");

		m_Accept = 0;
		
		m_SocketInput = socket;
		m_bSocketEmptyInput = false;
	};

	inline bool InputSocket(__in ST_SocketInfo socket)
	{
		m_Accept = 0;

		if (m_bSocketEmptyInput)
		{
			m_SocketInput = socket;
			m_bSocketEmptyInput = false;
			return true;
		}

		return false;
	};

	inline void OutputSocketToConv2(__out ST_SocketInfo& OutSocket)
	{
		if (m_bSocketEmptyInput) return;

// 		OutSocket = m_list.front();
// 		m_list.pop_front();

		OutSocket = m_SocketInput;
		m_bSocketEmptyInput = true;
		m_SocketInput.Reset();
	};

	inline void OutputBufferSocketToConv1(__out ST_SocketInfo& OutSocket)
	{
		if (m_bSocketEmptyBuffer) return;

// 		OutSocket = m_list_Buffer.front();
// 		m_list_Buffer.pop_front();

		OutSocket = m_SocketBuffer;
		m_bSocketEmptyBuffer = true;
		m_SocketBuffer.Reset();
	};

	inline void InputBufferSocket(__in ST_SocketInfo socket)
	{
		if (m_bSocketEmptyBuffer)
		{
			m_SocketBuffer = socket;
			m_bSocketEmptyBuffer = false;
		}			
	};

	inline bool IsBypassSocket()
	{
		if (m_bSocketEmptyInput)
			return false;

//		ST_SocketInfo socket = m_list.front();
		return m_SocketInput.bBypass;
	};

	inline void SetUseRFIDStop(__in bool bUse)
	{
		m_bUseRFIDStop = bUse;
	};

	bool GetUseRFIDStop()
	{
		return m_bUseRFIDStop;
	};

	inline bool GetInputSocketInfo(__out ST_SocketInfo& info)
	{
// 		if (!m_list.empty())
// 		{
// 			info = m_list.front();
// 			return true;
// 		}

		if (!m_bSocketEmptyInput)
		{
//			info = m_list.front();
			info = m_SocketInput;
			return true;
		}

		return false;		
	};

	inline bool GetBufferSocketInfo(__out ST_SocketInfo& info)
	{
		if (!m_bSocketEmptyBuffer)
		{
//			info = m_list_Buffer.front();
			info = m_SocketBuffer;
			return true;
		}

		return false;
	};
	   
	virtual	UINT RunProcess();
	virtual enMCErrCode StartOperation_Step();

	enMCErrCode ConveyorRun(__in enConv1SensorSelect mode);

private:
	int m_Accept;

	bool m_bUseRFIDStop;

	// µð¹ö±ë ¿É¼Ç
// 	std::list<ST_SocketInfo>	m_list;
// 	std::list<ST_SocketInfo>	m_list_Buffer;

	bool			m_bSocketEmptyInput;
	ST_SocketInfo	m_SocketInput;

	bool			m_bSocketEmptyBuffer;
	ST_SocketInfo	m_SocketBuffer;
};

