#include "pch.h"

#if(SET_INSPECTOR == SYS_COLOR_CAL)
#include "CL500ACommunicator.h"
#include "CL-SDK/include/CLAPI.h"
#include "DlgLog.h"

CCL500ACommunicator::~CCL500ACommunicator()
{
	Disconnect();
}

bool CCL500ACommunicator::Connect()
{
	static constexpr auto FUNCTION_NAME = _T("CL500A Connect");
	CString strLog;

	strLog.Format(_T("%s Start"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);

	// Acquire the device handle
	auto result = CLOpenDevice(&m_deviceHandle);
	if (result != SUCCESS)
	{
		strLog.Format(_T("%s CLOpenDevice Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		Disconnect();
		return false;
	}

	// Set remote mode on
	result = CLSetRemoteMode(m_deviceHandle, CL_RMODE_ON);
	if (result != SUCCESS)
	{
		strLog.Format(_T("%s CLSetRemoteMode Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		Disconnect();
		return false;
	}

	// Property setting
	if (!PropertySetting())
	{
		Disconnect();
		return false;
	}

	strLog.Format(_T("%s Succeeded"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);
	return true;
}

void CCL500ACommunicator::Disconnect()
{
	if (!RemoteOffClose())
		Close();
}

bool CCL500ACommunicator::ZeroCalibration()
{
	static constexpr auto FUNCTION_NAME = _T("CL500A ZeroCalibration");
	CString strLog;

	if (!IsConnected())
		return false;

	strLog.Format(_T("%s Start"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);

	// Perform a zero calibration
	auto result = CLDoCalibration(m_deviceHandle);
	if (result != SUCCESS) 
	{
		strLog.Format(_T("%s CLDoCalibration Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		return false;
	}

	for (auto status = CL_CALIBMEAS_FREE; status != CL_CALIBMEAS_FINISH;)
	{
		Sleep(1000);

		result = CLPollingCalibration(m_deviceHandle, &status);
		if (result == SUCCESS)
		{
			strLog.Format(_T("%s CLPollingCalibration in progress. code: %d"), FUNCTION_NAME, result);
			WRITE_ERR_LOG(strLog);
		}
		else
		{
			strLog.Format(_T("%s CLPollingCalibration Failed. code: %d"), FUNCTION_NAME, result);
			WRITE_ERR_LOG(strLog);
			return false;
		}
	}

	strLog.Format(_T("%s Succeeded"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);
	return true;
}

bool CCL500ACommunicator::Measurement(float & Ev, float & Tcp)
{
	static constexpr auto FUNCTION_NAME = _T("CL500A Measurement");
	CString strLog;

	if (!IsConnected())
		return false;

	strLog.Format(_T("%s Start"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);

	// Perform a measurement
	int time;
	auto result = CLDoMeasurement(m_deviceHandle, &time);
	if (result == WARNING)
	{
		WR_CODE warningCode;
		CLGetWarning(m_deviceHandle, &warningCode);
		strLog.Format(_T("%s CLDoMeasurement Warning. warning code: %d"), FUNCTION_NAME, warningCode);
		WRITE_COMM_LOG(strLog);
	}
	else if (result > WARNING)
	{
		strLog.Format(_T("%s CLDoMeasurement Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		return false;
	}

	strLog.Format(_T("%s MeasureTime: %dms"), FUNCTION_NAME, time / 10);
	WRITE_COMM_LOG(strLog);

	// Polling until completing a measurement
	for(auto status = CL_MEAS_FREE; status != CL_MEAS_FINISH; Sleep(1000))
	{
		result = CLPollingMeasure(m_deviceHandle, &status);
		if (result != SUCCESS) 
		{
			strLog.Format(_T("%s CLPollingMeasure Failed. code: %d"), FUNCTION_NAME, result);
			WRITE_ERR_LOG(strLog);
			return false;
		}
	}

	// Getting the measurement data(Ev/Tcp/duv)
	CL_MEASDATA measureData;
	result = CLGetMeasData(m_deviceHandle, CL_COLORSPACE_EVTCPJISDUV, &measureData);
	if (result == WARNING)
	{
		WR_CODE warningCode;
		CLGetWarning(m_deviceHandle, &warningCode);
		strLog.Format(_T("%s CLDoMeasurement Warning. warning code: %d"), FUNCTION_NAME, warningCode);
		WRITE_COMM_LOG(strLog);
	}
	else if (result > WARNING)
	{
		strLog.Format(_T("%s CLGetMeasData Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		return false;
	}

	Ev = measureData.EvTduv.Ev;
	Tcp = measureData.EvTduv.T;

	strLog.Format(_T("%s Succeeded, Ev: %.4f, Tcp: %.4f"), FUNCTION_NAME, Ev, Tcp);
	WRITE_COMM_LOG(strLog);
	return true;
}

bool CCL500ACommunicator::IsConnected() const
{
	return m_deviceHandle != nullptr;
}

bool CCL500ACommunicator::PropertySetting()
{
	static constexpr auto FUNCTION_NAME = _T("CL500A PropertySetting");
	CString strLog;

	if (!IsConnected())
		return false;

	strLog.Format(_T("%s Start"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);

	// Property setting(Observer)
	auto result = CLSetProperty(m_deviceHandle, CL_PR_OBSERVER, CL_OBS_02DEG);
	if (result != SUCCESS) 
	{
		strLog.Format(_T("%s CLSetProperty(Observer) Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		return false;
	}

	// Property setting(Illuminant unit)
	result = CLSetProperty(m_deviceHandle, CL_PR_ILLUNIT, CL_ILLUNIT_LX);
	if (result != SUCCESS) 
	{
		strLog.Format(_T("%s CLSetProperty(Illuminant Unit) Failed. code: %d"), FUNCTION_NAME, result);
		WRITE_ERR_LOG(strLog);
		return false;
	}

	strLog.Format(_T("%s Succeeded"), FUNCTION_NAME);
	WRITE_COMM_LOG(strLog);
	return true;
}

bool CCL500ACommunicator::RemoteOffClose()
{
	if (!IsConnected())
		return false;

	auto result = CLSetRemoteMode(m_deviceHandle, CL_RMODE_OFF);

	if (result != SUCCESS)
		return false;

	return Close();
}

bool CCL500ACommunicator::Close()
{
	if (!IsConnected())
		return false;

	auto result = CLCloseDevice(m_deviceHandle);

	m_deviceHandle = nullptr;

	if (result != SUCCESS)
		return false;

	return true;
}
#endif
