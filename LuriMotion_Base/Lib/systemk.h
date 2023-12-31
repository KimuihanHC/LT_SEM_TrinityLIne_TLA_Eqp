#ifndef _RFID_H_
#define _RFID_H_

#ifdef __cplusplus
	extern "C" {
#endif

#define RFID_OK				 0 //에러 없음
#define RFID_LIB_ERR		-1 //Library에러
#define RFID_NO_CARD		-2 //Card없음
#define RFID_READ_FAIL		-3 //읽기 실패
#define RFID_TCP_SEND_ERR	-4 //TCP송신 에러
#define RFID_TCP_RECV_ERR	-5 //TCP수신 에러
#define RFID_SOCKET_ERR		-6 //TCP소켓 에러
#define RFID_NO_RESP		-7 //응답없음
#define RFID_INVALID_PARAM  -8 //함수에 전달된 인자오류
#define RFID_WR_FAIL		-9 //RFID쓰기(메모리) 실패
//------------------------------------------------------------------------------
short WINAPI RFID_Connect(short nDevNo, short nField0, short nField1, short nField2, short nField3, short nPort, short nTimeOut);
short WINAPI RFID_CloseDevice(short nDevNo);
short WINAPI RFID_EventRead(short nDevNo, short nBlockID, short nID, char *szRetData, short nBufSize, short nTimeOut);
short WINAPI RFID_SetAutoRead(short nDevNo, short nMode, short nBlockID, short nID, short nIntervalTm);
short WINAPI RFID_SetTrigger(short nDevNo, short nMode, short nBlockID, short nID);
short WINAPI RFID_SetReadIRQ(short (CALLBACK *func)(short nDevNo, short nID, char *szTagID));
short WINAPI RFID_SetConnectIRQ(short (CALLBACK *func)(short nDevNo, bool bConnect));
short WINAPI RFID_WriteData(short nDevNo, short nBlockID, short nID, char *szWrData, short nDataSize, short nTimeOut);
short WINAPI RFID_PingCheck(short nField0, short nField1, short nField2, short nField3, short nPort, int lWaitTime);
short WINAPI RFID_SetLogConfig(bool bLogging, char *szLogFile);

#ifdef __cplusplus
	}
#endif

#endif
