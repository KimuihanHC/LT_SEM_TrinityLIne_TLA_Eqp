#pragma once

#include "SeqBase.h"

class CSeqSemiAuto : public CSeqBase
{
public:
	CSeqSemiAuto();
	~CSeqSemiAuto();

	void SetPortId(__in InspectionPortType port);
	InspectionPortType GetPortid();

	virtual bool Start();
	virtual UINT RunProcess();

	virtual enMCErrCode StartOperation_Step();

	enMCErrCode Conveyor1Run(__in enConv1SensorSelect mode);
	enMCErrCode Conveyor2Run(__in enConv2SensorSelect mode);

	void StartThread(__in int nIdx);
	static UINT Thread_Func(__in LPVOID lpVoid);

	enMCErrCode EqpType_StartStandBy	();
	enMCErrCode EqpType_Init_Finalize	();
	enMCErrCode EqpType_Inspection		();

	enMCErrCode EqpType_MoveZInspectionPos	(__in InspectionPortType port);
	inline void SetNextStep(__in enSeqSemiAutoStep step)
	{
		SetStep(step);
	};
	
private:
	InspectionPortType m_Portid;
	CWinThread* m_pThread;
	enMCErrCode m_ThdErrorCode;

	BOOL m_bThread;
	int nRetryChk;
};

