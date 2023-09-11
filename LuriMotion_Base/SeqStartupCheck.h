#pragma once

#include "SeqBase.h"
#include "Def_DigitalIO.h"
#include "User_Define.h"

class CSeqStartupCheck : public CSeqBase
{
public:
	CSeqStartupCheck	();
	~CSeqStartupCheck	();

	void SetPortId(__in InspectionPortType port);
	InspectionPortType GetPortid();

	virtual bool Start();
	virtual UINT RunProcess();

	void StartThread(__in int nIdx);
	static UINT Thread_Func(__in LPVOID lpVoid);

	enMCErrCode EqpType_StartStandBy();

	virtual enMCErrCode StartOperation_Step();

	enMCErrCode EqpTypeStartupCheck			();
	enMCErrCode EqpType_Init_Finalize();
	enMCErrCode EqpType_MoveZInspectionPos	(__in InspectionPortType port);

	enMCErrCode Conveyor1Run				(__in enConv1SensorSelect mode);
	enMCErrCode Conveyor2Run				(__in enConv2SensorSelect mode);

	void SaveReportFile						();

	inline void SetNextStep(__in enSeqStartupCheckStep step)
	{
		SetStep(step);
	};

private:
	InspectionPortType m_Portid;
	CWinThread* m_pThread;
	enMCErrCode m_ThdErrorCode;

	BOOL m_bThread;
};

