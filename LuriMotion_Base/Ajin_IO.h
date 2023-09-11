#pragma once
#include "pch.h"
#include "User_Define.h"
#define NUM_IO_DIVIDE	32 
class CAjinDio : public CWnd, public CSington<CAjinDio>
{
	DECLARE_DYNAMIC(CAjinDio)

public:
	CAjinDio();
	virtual ~CAjinDio();

protected:
	int m_nIONo;



	BOOL m_bThreadRun;
	HANDLE m_hThread;       // this thread's HANDLE
	CWinThread*	m_pThread;
	static UINT	Thread_DioRead(LPVOID lpVoid);




public:
	long m_lDIO_ModuleCnt;
	BOOL m_bInitDIO;
	
	DWORD m_dwInputBuf[10];
	DWORD m_dwOutputBuf[10];

	bool Start_AjinIO();
	void Terminate_DIO();

	bool GetInputState(int PCICardCount);
	bool GetOutputState(int PCICardCount);

	BOOL GetInputIoBit(long lBitNumber);
	BOOL GetOutputIoBit(long lBitNumber);
	DWORD GetOutputBit(long lBitNumber);
	void SendOutputBit(int nBitNumber, int bOnOff);
	void SendOutputBitSwap(int nBitNumber, BOOL bOnOff, int nSwapBit);
	void BlinkStart(long lOffset, long lInitState, long lmSecOn, long lmSecOff, long lCount = -1);
	void BlinkStop(long lOffset, DWORD uOnOff = 0);


	BOOL IsON(long lBitNumber , BOOL bInOut = FALSE);
	BOOL IsOFF(long lBitNumber, BOOL bInOut = FALSE);

	
	void SetON(long lBitNumber);
	void SetOFF(long lBitNumber);

	CCriticalSection	m_csIO[10];
};
