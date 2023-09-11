#pragma once

#if(SET_INSPECTOR == SYS_COLOR_CAL)
#include "Singleton.h"

typedef void* DEVICE_HANDLE;

class CCL500ACommunicator : public CSington<CCL500ACommunicator>
{
private:
	CCL500ACommunicator(const CCL500ACommunicator &) = delete;
	CCL500ACommunicator & operator=(const CCL500ACommunicator &) = delete;

public:
	CCL500ACommunicator() = default;
	~CCL500ACommunicator();

	bool Connect();
	void Disconnect();

	bool ZeroCalibration();
	bool Measurement(float & Ev, float & Tcp);
	bool IsConnected() const;

private:
	bool PropertySetting();

	bool RemoteOffClose();
	bool Close();

private:
	DEVICE_HANDLE m_deviceHandle = nullptr;
};
#endif

