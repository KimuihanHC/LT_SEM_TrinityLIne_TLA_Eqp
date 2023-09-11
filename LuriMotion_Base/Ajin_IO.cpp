#include "pch.h"
#include "Ajin_IO.h"
#include <mutex>

std::mutex mtx_lock;

IMPLEMENT_DYNAMIC(CAjinDio, CWnd)
CAjinDio::CAjinDio(void)
{

	for (int i = 0; i < 10; i++)
	{
		m_dwInputBuf[i] = 0;
		m_dwOutputBuf[i] = 0;
	}
}

CAjinDio::~CAjinDio(void)
{

}

bool CAjinDio::Start_AjinIO()
{
	DWORD dwResult;

	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	if (dwResult == AXT_RT_SUCCESS || dwResult == AXT_RT_OPEN_ALREADY)
	{
		dwResult = AxdInfoGetModuleCount(&m_lDIO_ModuleCnt);	// 먼저 IO 모듈의 갯수를 얻어오자.

		if (dwResult != AXT_RT_SUCCESS)
			return FALSE;

		// 1개도 없으면 리턴
		if (m_lDIO_ModuleCnt < 1)
			return FALSE;

		m_bThreadRun = TRUE;
		m_pThread = AfxBeginThread(Thread_DioRead, this);

		m_bInitDIO = TRUE;

		return TRUE;
	}

	return FALSE;
}

void CAjinDio::Terminate_DIO()
{
	if (!m_bInitDIO) return;

	if (m_bThreadRun)
	{
		m_bThreadRun = FALSE;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	}
	m_bThreadRun = NULL;

}


UINT CAjinDio::Thread_DioRead(LPVOID lpVoid)
{	
	int nErrorCount = 0;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	int nModuleCnt_Input = 3;
	nModuleCnt_Input = INPUT_IO_MAX_COUNT / INPUT_IO_READ_COUNT;

	int nModuleCnt_Output = 3;
	nModuleCnt_Output = OUTPUT_IO_MAX_COUNT / OUTPUT_IO_READ_COUNT;
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	int nModuleCnt_Input = 2;
	nModuleCnt_Input = INPUT_IO_MAX_COUNT / INPUT_IO_READ_COUNT;

	int nModuleCnt_Output = 2;
	nModuleCnt_Output = OUTPUT_IO_MAX_COUNT / OUTPUT_IO_READ_COUNT;
#endif
	while (AJIN_IO->m_bThreadRun)
	{
		for (int i = 0; i < nModuleCnt_Input; i++)
		{
			if (!AJIN_IO->GetInputState(i))
			{
			}
			else
			{
				nErrorCount = 0;
			}
		}

		for (int j = 0; j < nModuleCnt_Output; j++)
		{
			if (!AJIN_IO->GetOutputState(j))
			{
			}
			else
			{
				nErrorCount = 0;
			}
		}
		
		Sleep(5);
	}
	AJIN_IO->m_bThreadRun = FALSE;
	AJIN_IO->m_pThread = NULL;
	return 0;
}

bool CAjinDio::GetInputState(int PCICardCount)
{
	CString strLog = _T("");
	CString cs;
	long	lPortCnt = 0;

	DWORD	dwPortData;
	DWORD	dwChangeMask;

	DWORD	dwResult = NULL;
	DWORD64 dwValue = NULL;

	if (m_bInitDIO == FALSE)
	{
		return false;
	}

	int nModuelIdx = 0;
	// IO 읽어온다.
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	nModuelIdx = PCICardCount + 3;
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	nModuelIdx = PCICardCount;
#endif
	
// 	if (nModuelIdx == 2)
// 	{
// 		nModuelIdx = 4;
// 	}
	

	dwResult = AxdiReadInportDword(nModuelIdx, 0, &dwPortData);
	if (dwResult != AXT_RT_SUCCESS)
		return false;

	// IO 갯수가 맞는지 확인한다. (32점)
	dwValue |= dwPortData;
	dwResult = AxdInfoGetInputCount(nModuelIdx, &lPortCnt);
	if (dwResult != AXT_RT_SUCCESS)
		return FALSE;

	dwChangeMask = dwPortData ^ m_dwInputBuf[PCICardCount];
	if (dwChangeMask != 0x00)
	{
		m_dwInputBuf[PCICardCount] = dwPortData;
	}
	Sleep(5);

	return true;
}

bool CAjinDio::GetOutputState(int PCICardCount)
{
	long	lPortCnt = 0;
	CString strLog = _T("");
	CString	cs;

	DWORD	dwPortData;
	DWORD	dwChangeMask;

	DWORD	dwResult = NULL;
	DWORD64 dwValue = NULL;

	auto nModuleIdx = 0;

	

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	nModuleIdx = PCICardCount; 
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	nModuleIdx = PCICardCount+2;
#endif

	if (m_bInitDIO == FALSE)
	{
		return false;
	}

	dwResult = AxdoReadOutportDword(nModuleIdx, 0, &dwPortData);
	if (dwResult != AXT_RT_SUCCESS)
		return false;


	// IO 갯수가 맞는지 확인한다. (32점)
	dwValue |= dwPortData;
	dwResult = AxdInfoGetOutputCount(nModuleIdx, &lPortCnt);
	if (dwResult != AXT_RT_SUCCESS)
		return FALSE;

	dwChangeMask = dwPortData ^ m_dwOutputBuf[nModuleIdx];
	if (dwChangeMask != 0x00)
	{
		m_dwOutputBuf[nModuleIdx] = dwPortData;
	}
	Sleep(5);

	return true;
}

void CAjinDio::SendOutputBit(int nBitNumber, int bOnOff)
{
	int nBoardNo = 0;

	nBoardNo = nBitNumber / 32;

	nBitNumber = nBitNumber - (nBoardNo * 32);
	long				m_lToggleOnTimeDelay = 200;			// 토글 기능 On Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleOffTimeDelay = 200;		// 토글 기능 Off Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleCount = -1;				// 토글 횟수, -1의 경우 무한 토글
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AxdoWriteOutportBit((long)nBoardNo, nBitNumber, bOnOff);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	AxdoWriteOutportBit((long)nBoardNo+2, nBitNumber, bOnOff);
#endif



	
}

void CAjinDio::SendOutputBitSwap(int nBitNumber, BOOL bOnOff, int nSwapBit)
{
	int nBoardNo = 0;

	nBoardNo = nBitNumber / 32;
	nBitNumber = nBitNumber - (nBoardNo * 32);
	nSwapBit = nSwapBit - (nBoardNo * 32);
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	if (bOnOff == TRUE)
	{
		AxdoWriteOutportBit((long)nBoardNo, nSwapBit, !bOnOff);
		AxdoWriteOutportBit((long)nBoardNo, nBitNumber, bOnOff);
	}
	else
	{
		AxdoWriteOutportBit((long)nBoardNo, nBitNumber, bOnOff);
		AxdoWriteOutportBit((long)nBoardNo, nSwapBit, !bOnOff);
	}
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	if (bOnOff == TRUE)
	{
		AxdoWriteOutportBit((long)nBoardNo+2, nSwapBit, !bOnOff);
		AxdoWriteOutportBit((long)nBoardNo + 2, nBitNumber, bOnOff);
	}
	else
	{
		AxdoWriteOutportBit((long)nBoardNo + 2, nBitNumber, bOnOff);
		AxdoWriteOutportBit((long)nBoardNo + 2, nSwapBit, !bOnOff);
	}
#endif

}
void CAjinDio::BlinkStart(long lOffset, long lInitState, long lmSecOn, long lmSecOff, long lCount)
{
	long lModuleNo;
	long lBitNo;

	lModuleNo = lOffset / 32;

	lBitNo = lOffset - (lModuleNo * 32);
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AxdoToggleStart(lModuleNo, lBitNo, lInitState, lmSecOn, lmSecOff, lCount);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	AxdoToggleStart(lModuleNo+2, lBitNo, lInitState, lmSecOn, lmSecOff, lCount);
#endif

	
}

void CAjinDio::BlinkStop(long lOffset, DWORD uOnOff)
{
	long lModuleNo;
	long lBitNo;

	lModuleNo = lOffset / 32;

	lBitNo = lOffset - (lModuleNo * 32);
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	AxdoToggleStop(lModuleNo, lBitNo, uOnOff);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	AxdoToggleStop(lModuleNo+2, lBitNo, uOnOff);
#endif

	
}

BOOL CAjinDio::GetInputIoBit(long lBitNumber)
{
	m_csIO[0].Lock();
	BOOL bFlag;
	int nBoardNo;
	int nHighLow;
	int nBitNumber;
	DWORD	dwResult = NULL;

	nBoardNo = lBitNumber / 32;

// 	if ((lBitNumber % 32) >= 16)
// 	{
// 		nHighLow = 1;
// 	}
// 	else
// 	{
// 		nHighLow = 0;
// 	}


	if ((lBitNumber > 31) )
	{
		nHighLow = 1;
		nBitNumber = lBitNumber - 32 * nBoardNo;
	}
	else
	{
		nHighLow = 0;
		nBitNumber = lBitNumber;
	}


	//nBitNumber = (lBitNumber % 32) /*- (16 * nHighLow)*/;

	//nBitNumber = (lBitNumber % 32) - (16 * nHighLow);

	
// // 	dwResult = AxdiReadInportDword(nBoardNo, 0, &dwDataBit);
// // 
// // 	for (int i = 0; i < 32; i++)
// // 	{
// // 		bFlag = ((dwDataBit & 0x0001) ^ nHighLow);
// // 		dwDataBit = dwDataBit >> 1;
// // 
// // 		if (i == (int)nBitNumber)
// // 			return bFlag;
// // 	}
// 
// 	if (nBoardNo == 0)
// 	{
// 		dwResult = AxdiReadInportDword(nBoardNo, 0, &dwDataBit);
// 	}
// 	else
// 	{
// 		AxdiReadInportWord((long)nBoardNo + 1, (long)nHighLow, &dwDataBit);
// 	}
// 	//AxdiReadInportWord((long)nBoardNo + 1, (long)nHighLow, &dwDataBit);
// 
// 
// 	for (int i = 0; i < 32; i++)
// 	{
// 		bFlag = (dwDataBit & 0x0001);
// 		dwDataBit = dwDataBit >> 1;
// 
// 		if (i == (int)lBitNumber)
// 			return bFlag;
// 	}
	int nCnt = 3;
	int nMax = 32;
	int nReal_IO_Board_Number = 0;

	if (nBoardNo == 2)
		nMax = 32;
	else
		nMax = 32;

	for (int j = 0; j < nMax; j++)
	{
		if ((m_dwInputBuf[nBoardNo] >> j) & 0x01)
		{
			bFlag =  TRUE;
		}
		else
		{
			bFlag = FALSE;
		}

		if (j == nBitNumber)
		{
			m_csIO[0].Unlock();
			return bFlag;
			
		 }
	}

	m_csIO[0].Unlock();
	return bFlag;
	
}


BOOL CAjinDio::GetOutputIoBit(long lBitNumber)
{
	m_csIO[1].Lock();
	DWORD dwDataBit;
	BOOL bFlag;

	int nBoardNo;
	int nHighLow;

	nBoardNo = lBitNumber / 32;
	nHighLow = lBitNumber % 32;

	AxdoReadOutportDword((long)nBoardNo+1, 0, &dwDataBit);

	for (int i = 0; i < 32; i++)
	{
		bFlag = dwDataBit & 0x0001;
		dwDataBit = dwDataBit >> 1;

		if (i == (int)lBitNumber) return bFlag;
	}
	m_csIO[1].Unlock();
	return bFlag;
}

DWORD CAjinDio::GetOutputBit(long lBitNumber)
{
	int nBoardNo = 0;
	DWORD dwResult = FALSE;
	nBoardNo = lBitNumber / 32;

	lBitNumber = lBitNumber - (nBoardNo * 32);
	long				m_lToggleOnTimeDelay = 200;		// 토글 기능 On Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleOffTimeDelay = 200;		// 토글 기능 Off Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleCount = -1;		// 토글 횟수, -1의 경우 무한 토글

	AxdoReadOutportBit((long)nBoardNo , lBitNumber, &dwResult);
	return dwResult;
}
BOOL CAjinDio::IsON(long lBitNumber, BOOL bInOut)
{
	if (bInOut == FALSE) {	// Input
		return GetInputIoBit(lBitNumber);
	}
	else {					// Output
		return GetOutputIoBit(lBitNumber);
	}
}
BOOL CAjinDio::IsOFF(long lBitNumber, BOOL bInOut)
{
	if (bInOut == FALSE) {	// Input
		return !GetInputIoBit(lBitNumber);
	}
	else {					// Output
		return !GetOutputIoBit(lBitNumber);
	}
}
void CAjinDio::SetON(long lBitNumber)
{
	SendOutputBit(lBitNumber, TRUE);
}
void CAjinDio::SetOFF(long lBitNumber)
{
	SendOutputBit(lBitNumber, FALSE);
}