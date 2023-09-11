#pragma once

#include "SeqBase.h"

class CSeqPicker : public CSeqBase
{
public:
	CSeqPicker();
	~CSeqPicker();

	virtual void Reset();
	inline void SetPortId(__in InspectionPortType nId)
	{
		m_nPortId = nId;

		if (m_nPortId == InspectionPortType::Left)
			m_PortType = PortType::Left;
		else
			m_PortType = PortType::Right;
	};
		
	inline InspectionPortType GetPortId()
	{
		return m_nPortId;
	};

	inline void SetPortSkip(__in bool bSkip)
	{
		m_bSkip =bSkip;
	};
		
	inline bool GetPortSkip()
	{
		return m_bSkip;
	};

// 	inline void SetNextStep(__in enSeqPickerStep step)
// 	{
// 		SetStep(step);
// 	};
	
	virtual UINT RunProcess						();
	virtual enMCErrCode StartOperation_Step		();

	enMCErrCode StartOperation_Inspection	(__in InspectionPortType port);

	void		EqpType_Initialize			();
	enMCErrCode EqpType_MoveZInspectionPos	(__in InspectionPortType port);
	void		EqpType_Finalize			();

	inline void InputSocket(__in ST_SocketInfo socket)
	{
//		m_list.push_back(socket);

		m_bSocketEmptyInput = false;
		m_SocketInput = socket;
	};

	inline void OutputSocket(__in ST_SocketInfo& OutSocket)
	{		
		if (m_bSocketEmptyInput) return;

// 		OutSocket = m_list.front();
// 		m_list.pop_front();		

		m_bSocketEmptyInput = true;
		OutSocket = m_SocketInput;
		m_SocketInput.Reset();
	};

	inline bool IsInputSocket()
	{
		bool bRet = false;

		if (!m_bSocketEmptyInput)
			bRet = true;
		else
			bRet = false;

		return bRet;
	};

// 	inline int GetSocketCount()
// 	{
// 		return (int)m_list.size();
// 	};

	void SetEject(int nResult)
	{
		m_bRecvEject = true;
		m_nEjectResult = nResult;
	};

	bool GetRecvEject()
	{
		return m_bRecvEject;
	};

	void SetTestResult(__in int nCode)
	{
// 		auto * pSocket = &m_list.front();
// 		pSocket->SetTestResult(nCode);

		m_SocketInput.SetTestResult(nCode);
	};

	void SetSocketStatus(__in enSocketStatus status)
	{
		if (m_bSocketEmptyInput)
			return;

// 		auto * pSocket = &m_list.front();
// 		pSocket->status = status;
		m_SocketInput.status = status;
	};
	
	inline bool GetSocketInfo(__out ST_SocketInfo& info)
	{
		if (!m_bSocketEmptyInput)
		{
		//	info = m_list.front();
			info = m_SocketInput;
			return true;
		}
		return false;
	};

private:
	InspectionPortType	m_nPortId;
	PortType			m_PortType;

	bool	m_bSkip;

	bool	m_bRecvEject;
	int		m_nEjectResult;

//	std::list<ST_SocketInfo>	m_list;

	bool			m_bSocketEmptyInput;
	ST_SocketInfo	m_SocketInput;
};

