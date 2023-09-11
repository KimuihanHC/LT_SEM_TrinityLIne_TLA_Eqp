#pragma once

#include "SeqBase.h"

class CSeqConveyor2 : public CSeqBase
{
public:
	CSeqConveyor2();
	~CSeqConveyor2();
	
	virtual void Reset();
private:
	bool WaitForDetectSignal		(__in enConv2SensorSelect sensor, __in bool bStatus = true);
	bool WaitForDetectSlowSignal	(__in InspectionPortType port, __in bool bStatus);
	bool WaitForDetectStopSignal	(__in InspectionPortType port, __in bool bStatus);
	bool WaitForSocketToOut();
	bool WaitForSocketFromIn		();
public:
	virtual	UINT RunProcess			();		
	virtual enMCErrCode StartOperation_Step();

	enMCErrCode ConveyorRun(__in enConv2SensorSelect mode, __in enConveyorSpeed spd = High);
	inline void SetNextStep(__in enSeqCv2Step step)
	{
		SetStep(step);
	};

	inline void SetWorkPort(__in UINT nPort)
	{
		m_WorkPort = (InspectionPortType)nPort;
	};

	inline InspectionPortType GetWorkPort()
	{
		return m_WorkPort;
	};

	inline bool IsInputSocket()
	{
		if (!m_bSocketEmptyInput)
			return true;
		else
			return false;
	};

	inline void InputSocket(__in ST_SocketInfo socket)
	{
//		m_list.push_back(socket);
		m_SocketInput = socket;
		m_bSocketEmptyInput = false;
	};

	inline void OutputSocket(__out ST_SocketInfo& OutSocket)
	{
		if (m_bSocketEmptyInput)
			return;

// 		OutSocket = m_list.front();
// 		m_list.pop_front();

		m_bSocketEmptyInput = true;
		OutSocket = m_SocketInput;
		m_SocketInput.Reset();
	};

	inline void InputUnloadSocket(__in ST_SocketInfo socket)
	{
//		m_listUnload.push_back(socket);

		m_bSocketEmptyUnload = false;
		m_SocketUnload = socket;
	};

	inline void OutputUnloadSocket(__out ST_SocketInfo& OutSocket)
	{
		if (m_bSocketEmptyUnload)
			return;

// 		OutSocket = m_listUnload.front();
// 		m_listUnload.pop_front();

		OutSocket = m_SocketUnload;
		m_bSocketEmptyUnload = true;
		m_SocketUnload.Reset();
	};

// 	inline int GetSocketCount()
// 	{
// 		return (int)m_list.size();
// 	};
	
	inline bool GetSocketInfo(__out ST_SocketInfo& info)
	{
		if (!m_bSocketEmptyInput)
		{
			info = m_SocketInput;
			return true;
		}

		if (!m_bSocketEmptyUnload)
		{
			info = m_SocketUnload;
			return true;
		}
		return false;
	};

private:
	InspectionPortType m_WorkPort;

// 	std::list<ST_SocketInfo>	m_list;
// 	std::list<ST_SocketInfo>	m_listUnload;

	bool			m_bSocketEmptyInput;
	ST_SocketInfo	m_SocketInput;

	bool			m_bSocketEmptyUnload;
	ST_SocketInfo	m_SocketUnload;
};

