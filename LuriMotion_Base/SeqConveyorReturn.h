#pragma once

#include "SeqBase.h"

class CSeqConveyorReturn : public CSeqBase
{
public:
	CSeqConveyorReturn();
	~CSeqConveyorReturn();

	virtual void Reset();
	bool WaitForDetectSignal(__in enConvRetSensorSelect sensor, __in bool bStatus = true);

	bool WaitForSocketToOut		();
	bool WaitForSocketFromIn	();

	inline void SetNextStep(__in enSeqCvrStep step)
	{
		SetStep(step);
	};
	
	virtual UINT RunProcess();
	virtual enMCErrCode StartOperation_Step();

	enMCErrCode ConveyorRun(__in enConvRetSensorSelect mode);
	inline bool IsInputSocket()
	{
		if (!m_bSocketEmptyInput)
			return true;
		else
			return false;
	};

	inline void InputSocket(__in ST_SocketInfo socket)
	{
	//	m_list.push_back(socket);
		m_bSocketEmptyInput = false;
		m_SocketInput = socket;
	};

	inline void OutputSocket(__out ST_SocketInfo& OutSocket)
	{
		if (m_bSocketEmptyInput)
			return;

// 		OutSocket = m_list.front();
// 		m_list.pop_front();
		OutSocket = m_SocketInput;
		m_bSocketEmptyInput = true;
	};

	inline bool GetSocketInfo(__out ST_SocketInfo& info)
	{
		if (!m_bSocketEmptyInput)
		{
//			info = m_list.front();
			info = m_SocketInput;
			return true;
		}

		return false;
	};
private:

//	std::list<ST_SocketInfo>	m_list;

	bool			m_bSocketEmptyInput;
	ST_SocketInfo	m_SocketInput;
};

