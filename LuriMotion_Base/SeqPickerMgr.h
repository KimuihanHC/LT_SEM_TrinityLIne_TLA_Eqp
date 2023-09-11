#pragma once

#include "SeqBase.h"
#include "SeqPicker.h"
//#include "DlgSide.h"

typedef enum
{
	Use_LR = 0,
	Use_RL,
	Use_L_Skip_R,
	Use_R_Skip_L,
}enPickUseType;

class CSeqPickerMgr : public CSeqBase
{
public:
	CSeqPickerMgr();
	~CSeqPickerMgr();
//	CDlgSide*					m_pDlgSide;
	void SetMode();

	virtual bool Start		();
	virtual bool Stop		();

			void ErrorClear	();
	virtual void Reset		();

	virtual bool ForceStop	();

	virtual UINT RunProcess();
	enMCErrCode WaitCompletePickerStep(__in InspectionPortType port, __in enSeqPickerStep step);

	virtual enMCErrCode StartOperation_Step();

	virtual enMCErrCode StartReady();
	virtual enMCErrCode StartLock();
	virtual enMCErrCode StartInsp_Power();
	virtual enMCErrCode StartInsp_Prewiew();
	virtual enMCErrCode StartInsp_OC();
	virtual enMCErrCode StartInsp_Vignetting_46();
	virtual enMCErrCode StartInsp_Vignetting_82();

	inline enMCErrCode GetPickerErrorCode()
	{
		enMCErrCode err = MCEC_OK;
		err = GetErrorCode();
		return err;
	};

	inline enMCErrCode GetPickerErrorCode(__in InspectionPortType port)
	{
		enMCErrCode err = MCEC_OK;
		err = (err != MCEC_OK) ? m_SeqPicker[(int)port].GetErrorCode() : err;
		return err;
	};

	inline enMCErrCode GetPickerError(__in InspectionPortType port)
	{
		enMCErrCode err;
		err = m_SeqPicker[(int)port].GetErrorCode();
		return err;
	};

	inline bool IsErrorPicker(__in InspectionPortType port)
	{
		return m_SeqPicker[(int)port].IsError();
	};

	inline int GetPickerStep(__in InspectionPortType port)
	{
		return m_SeqPicker[(int)port].GetStep();
	};

	inline void SetNextStep(__in enSeqPickerMgrStep step)
	{
		SetStep(step);
	};

	bool IsPickerUnloadReqeust();
	bool IsPickerWorkEndRFID();
	bool IsReadyWorkEnd();

	inline int GetEmptyPortId()		// -1 : 모두 사용중, 0 : left, 1 : right
	{
		int nPortId = -1;

// 		if (m_SeqPicker[0].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET)
// 			nPortId = 0;
// 
// 		if (m_SeqPicker[1].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET)
// 			nPortId = 1;

		return nPortId;
	};

	inline bool IsSocketAtUnloadPort(__out InspectionPortType& Port)
	{
		Port = m_UnloadingPort;
		return m_SeqPicker[(int)m_UnloadingPort].IsInputSocket();
	};

	inline void OutputSocketFromUnload(__out ST_SocketInfo& OuputSocket)
	{
		m_SeqPicker[(int)m_UnloadingPort].OutputSocket(OuputSocket);
	};

	inline void InputSocketToLoad(__in ST_SocketInfo socket)
	{
		int nPortId = -1;
// 
// 		if (m_SeqPicker[0].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET)
// 			nPortId = 0;
// 
// 		if (m_SeqPicker[1].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_WAIT_SOCKET)
// 			nPortId = 1;

		m_SeqPicker[nPortId].InputSocket(socket);
	};

// 	inline int GetSocketCount()
// 	{
// 		int Count = 0;
// 
// 		Count += m_SeqPicker[0].GetSocketCount();
// 		Count += m_SeqPicker[1].GetSocketCount();
// 	};

	inline bool IsUsePortSkip()
	{
		bool bSkip = false;
		
		if (m_bPortSkip[0] || m_bPortSkip[1])
			bSkip = true;
		
		return bSkip;
	};

	inline void SetMode(__in enPickUseType nMode)
	{
		switch (nMode)
		{
		case Use_LR:
			m_LoadingPort	= InspectionPortType::Left;
			m_UnloadingPort = InspectionPortType::Right;

			m_bPortSkip[0] = false;
			m_bPortSkip[1] = false;
			break;

		case Use_RL:
			m_LoadingPort	= InspectionPortType::Right;
			m_UnloadingPort = InspectionPortType::Left;

			m_bPortSkip[0] = false;
			m_bPortSkip[1] = false;
			break;

		case Use_L_Skip_R:
			m_LoadingPort	= InspectionPortType::Left;
			m_UnloadingPort = InspectionPortType::Right;
			
			m_bPortSkip[0] = false;
			m_bPortSkip[1] = true;
			break;

		case Use_R_Skip_L:
			m_LoadingPort = InspectionPortType::Right;
			m_UnloadingPort = InspectionPortType::Left;

			m_bPortSkip[0] = true;
			m_bPortSkip[1] = false;
			break;

		default:
			break;
		}

		m_PickUseType = nMode;
	};

	inline enPickUseType GetMode()
	{
		return m_PickUseType;
	};

	inline void SetAutoChange(__in bool bUse)
	{
		m_bAutoChange = bUse;
	};

	inline bool GetAutoChange()
	{
		return m_bAutoChange;
	};

	inline void SetEject(__in int nResult)
	{
// 		int nPortId = -1;
// 
// 		if (m_SeqPicker[0].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_REQEUST)
// 			nPortId = 0;
// 
// 		if (m_SeqPicker[1].GetStep() == enSeqPickerStep::SEQ_PICKER_STEP_UNLOAD_REQEUST)
// 			nPortId = 1;

		m_SeqPicker[0].SetEject(nResult);
		m_SeqPicker[1].SetEject(nResult);
	};

	inline bool GetSocketInfo(__in InspectionPortType port,  __out ST_SocketInfo& socket)
	{
		bool bRet = false;
		switch (port)
		{
		case InspectionPortType::Left:
			bRet = m_SeqPicker[0].GetSocketInfo(socket);
			break;

		case InspectionPortType::Right:
			bRet = m_SeqPicker[1].GetSocketInfo(socket);
			break;

		default:
			break;
		}

		return bRet;
	};

	inline void SetWorkEnd()
	{
		m_bWorkEnd = true;
	};

	inline void ResetWorkEnd()
	{
		m_bWorkEnd = false;
	};

	inline bool GetWorkEnd()
	{
		return m_bWorkEnd;
	};
	
	clock_t startmMotionChk = clock(); // 모션 관련 
	int reTry;

private:
	CSeqPicker		m_SeqPicker[2];		// 0 : left, 1 : right

	bool			m_bPortSkip[2];

	InspectionPortType	m_LoadingPort;
	InspectionPortType	m_UnloadingPort;

	enPickUseType m_PickUseType;

	bool m_bWorkEnd;
	bool m_bAutoChange;

	enMCErrCode m_PickErrorCode;
	bool m_bPickErrorCode;
};

