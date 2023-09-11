#include "pch.h"
#include "IcsCommunicator.h"
#include "Def_CompileOption.h"
#include "Def_Enum.h"
#include "DlgLog.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/Logger.h"
#include "Logger/LogDispatcher.h"
#include "Comm/TcpCommunicator.h"
#include "Comm/SocketCommOption.h"
#include "Comm/IcsClientTester.h"
#include "Event/EventMfnListener.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/StringUtil.hpp"

#include "System_Define.h"

CIcsCommunicator::CIcsCommunicator()
: CSington<CIcsCommunicator>()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	// Log
	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(stTester.strEQPName, lt::LogFilter::Detail);

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s begins ============================="),
				   stTester.strEQPName).c_str()
	);

	m_pLogAddedEventListener =
		new LogAddedEventListener(this, &CIcsCommunicator::OnLogAddedEvent);

	m_pLogDispatcher = new lt::CLogDispatcher();

	m_pLogDispatcher->SetLogFilter(lt::LogFilter::Normal);
	m_pLogDispatcher->GetLogAddedEventNotifier() +=
		m_pLogAddedEventListener;

	m_pLogger = new lt::CLogger();

	m_pLogger->Attach(m_pLogDispatcher);

	// Comm
	m_pComm = new lt::CTcpCommunicator(_T(""));

	m_pComm->SetLogDispatcher(*m_pDebugLogDispatcher);

	m_pCommConnectedEventListener =
		new CommConnectionEventListener(this, &CIcsCommunicator::OnCommConnectedEvent);
	m_pCommDisconnectingEventListener =
		new CommConnectionEventListener(this, &CIcsCommunicator::OnCommDisconnectingEvent);

	m_pComm->GetCommConnectedEventNotifier() +=
		m_pCommConnectedEventListener;
	m_pComm->GetCommDisconnectingEventNotifier() +=
		m_pCommDisconnectingEventListener;

	// Client
	m_pClient = new lt::CIcsClientTester();

	m_pClient->SetLogDispatcher(m_pLogDispatcher);

	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	m_pTimeSyncEventListener =
		new TimeSyncEventListener(this, &CIcsCommunicator::OnTimeSyncEvent);
	m_pUserLevelEventListener =
		new UserLevelEventListener(this, &CIcsCommunicator::OnUserLevelEvent);
	m_pLanguageEventListener =
		new LanguageEventListener(this, &CIcsCommunicator::OnLanguageEvent);
	m_pModelEventListener =
		new ModelEventListener(this, &CIcsCommunicator::OnModelEvent);

	m_pClient->GetTimeSyncEventNotifier() +=
		m_pTimeSyncEventListener;
	m_pClient->GetUserLevelEventNotifier() +=
		m_pUserLevelEventListener;
	m_pClient->GetLanguageEventNotifier() +=
		m_pLanguageEventListener;
	//m_pClient->GetModelEventNotifier() +=
	//	m_pModelEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	m_pInitializationEventListener =
		new InitializationEventListener(this, &CIcsCommunicator::OnInitializationEvent);
	m_pResetEventListener =
		new ResetEventListener(this, &CIcsCommunicator::OnResetEvent);
	m_pRunEventListener =
		new RunEventListener(this, &CIcsCommunicator::OnRunEvent);
	m_pStopEventListener =
		new StopEventListener(this, &CIcsCommunicator::OnStopEvent);
	m_pBuzzerOffEventListener =
		new BuzzerOffEventListener(this, &CIcsCommunicator::OnBuzzerOffEvent);

	m_pClient->GetInitializationEventNotifier() +=
		m_pInitializationEventListener;
	m_pClient->GetResetEventNotifier() +=
		m_pResetEventListener;
	m_pClient->GetRunEventNotifier() +=
		m_pRunEventListener;
	m_pClient->GetStopEventNotifier() +=
		m_pStopEventListener;
	m_pClient->GetBuzzerOffEventNotifier() +=
		m_pBuzzerOffEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pSocketAcceptingEventListener =
		new SocketAcceptingEventListener(this, &CIcsCommunicator::OnSocketAcceptingEvent);
	m_pEjectAcceptingEventListener =
		new EjectAcceptingEventListener(this, &CIcsCommunicator::OnEjectAcceptingEvent);
	m_pProductStartEndEventListener =
		new ProductStartEndEventListener(this, &CIcsCommunicator::OnProductStartEndEvent);

	m_pClient->GetSocketAcceptingEventNotifier() +=
		m_pSocketAcceptingEventListener;
	m_pClient->GetEjectAcceptingEventNotifier() +=
		m_pEjectAcceptingEventListener;
	m_pClient->GetProductionStartEndEventNotifier() +=
		m_pProductStartEndEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////
}

CIcsCommunicator::~CIcsCommunicator()
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	m_pClient->GetTimeSyncEventNotifier() -=
		m_pTimeSyncEventListener;
	m_pClient->GetUserLevelEventNotifier() -=
		m_pUserLevelEventListener;
	m_pClient->GetLanguageEventNotifier() -=
		m_pLanguageEventListener;
	//m_pClient->GetModelEventNotifier() -=
	//	m_pModelEventListener;

	delete m_pTimeSyncEventListener;
	delete m_pUserLevelEventListener;
	delete m_pLanguageEventListener;
	delete m_pModelEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	m_pClient->GetInitializationEventNotifier() -=
		m_pInitializationEventListener;
	m_pClient->GetResetEventNotifier() -=
		m_pResetEventListener;
	m_pClient->GetRunEventNotifier() -=
		m_pRunEventListener;
	m_pClient->GetStopEventNotifier() -=
		m_pStopEventListener;
	m_pClient->GetBuzzerOffEventNotifier() -=
		m_pBuzzerOffEventListener;

	delete m_pInitializationEventListener;
	delete m_pResetEventListener;
	delete m_pRunEventListener;
	delete m_pStopEventListener;
	delete m_pBuzzerOffEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pClient->GetSocketAcceptingEventNotifier() -=
		m_pSocketAcceptingEventListener;
	m_pClient->GetEjectAcceptingEventNotifier() -=
		m_pEjectAcceptingEventListener;

	delete m_pSocketAcceptingEventListener;
	delete m_pEjectAcceptingEventListener;
	//
	//////////////////////////////////////////////////////////////////////////////////

	// Client
	m_pClient->SetLogDispatcher(nullptr);

	delete m_pClient;

	// Comm
	m_pComm->GetCommConnectedEventNotifier() -=
		m_pCommConnectedEventListener;
	m_pComm->GetCommDisconnectingEventNotifier() -=
		m_pCommDisconnectingEventListener;

	delete m_pCommConnectedEventListener;
	delete m_pCommDisconnectingEventListener;

	m_pComm->SetLogDispatcher(nullptr);

	delete m_pComm;

	// Log
	m_pLogger->Detach();

	delete m_pLogger;

	m_pLogDispatcher->GetLogAddedEventNotifier() -=
		m_pLogAddedEventListener;

	delete m_pLogDispatcher;

	delete m_pLogAddedEventListener;

	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s ends ============================="),
				   stTester.strEQPName).c_str()
	);
	Sleep(100);

	delete m_pDebugLogDispatcher;
}

bool CIcsCommunicator::Connect(LPCTSTR strAddress, USHORT port)
{
	lt::SSocketCommOption commOption = {};

	commOption.sizeOfStructure = sizeof(commOption);
	commOption.remotePort = port;
	commOption.sizeOfRxBuffer = 1000;
	_tcscpy_s(commOption.remoteAddress, _countof(commOption.remoteAddress), strAddress);

	return GetComm().Connect(commOption);
}

void CIcsCommunicator::Disconnect()
{
	GetComm().Disconnect();
}

bool CIcsCommunicator::IsConnected()
{
	return GetComm().IsConnected();
}

bool CIcsCommunicator::SendEquipmentId(LPCTSTR equipmentId)
{
	auto result = GetClient().CommandEquipmentId(
		{
			equipmentId
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandEquipmentId failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendOperationMode(OperationMode mode)
{
	auto result = GetClient().CommandOperationMode(
		{
			static_cast<lt::uint32>(mode)
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandOperationMode failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendSocketRequest(LPCTSTR rfid)
{
	auto result = GetClient().CommandSocketRequest(
		{
			rfid
		},
		10000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandSocketRequest failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendTestResult(LPCTSTR rfid,
									  INT32 resultCode,
									  InspectionPortType type)
{
	auto result = GetClient().CommandTestResult(
		{
			rfid,
			resultCode,
			static_cast<lt::uint32>(type)
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClientTester::CommandTestResult failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendPortClear(PortType type)
{
	auto result = GetClient().CommandPortClear(
		{
			static_cast<lt::uint32>(type)
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandPortClear failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendEquipmentStatus(EquipmentStatus status,
										   INT32 alarmCode,
										   LPCTSTR alarmReason)
{
	auto result = GetClient().CommandEquipmentStatus(
		{
			static_cast<lt::uint32>(status),
			alarmCode,
			alarmReason
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandEquipmentStatus failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendPortStatus(PortType type,
									  PortStatus status,
									  LPCTSTR rfid,
									  LPCTSTR productSn)
{
	auto result = GetClient().CommandPortStatus(
		{
			static_cast<lt::uint32>(type),
			static_cast<lt::uint32>(status),
			rfid,
			productSn
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandPortStatus failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendPortStatusRequest(PortType type,
											 PortStatus & status,
											 CString & rfid,
											 CString & productSn)
{
	lt::SPortStatus2 portStatus = { static_cast<lt::uint32>(type) };
	auto result = GetClient().CommandPortStatusRequest(
		portStatus,
		1000
	);

	if(result == ICS_COMM_OK)
	{
		status = static_cast<std::remove_reference_t<decltype(status)>>(portStatus.status);
		rfid = portStatus.socketInfo.rfid;
		productSn = portStatus.socketInfo.productSn;

		return true;
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandPortStatusRequest failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendConveyorStatus(ConveyorType type,
										  ConveyorStatus1 status1,
										  ConveyorStatus2 status2,
										  LPCTSTR rfid,
										  LPCTSTR productSn)
{
	auto result = GetClient().CommandConveyorStatus(
		{
			static_cast<lt::uint32>(type),
			static_cast<lt::uint32>(status1),
			static_cast<lt::uint32>(status2),
			rfid,
			productSn
		},
		1000
	);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandConveyorStatus failed %d"),
				result);

	return false;
}

bool CIcsCommunicator::SendConveyorStatusRequest(ConveyorType type,
												 ConveyorStatus1 & status1,
												 ConveyorStatus2 & status2,
												 CString & rfid,
												 CString & productSn)
{
	lt::SConveyorStatus2 conveyorStatus = { static_cast<lt::uint32>(type) };
	auto result = GetClient().CommandConveyorStatusRequest(
		conveyorStatus,
		1000
	);

	if(result == ICS_COMM_OK)
	{
		status1 = static_cast<std::remove_reference_t<
			decltype(status1)>>(conveyorStatus.status1);
		status2 = static_cast<std::remove_reference_t<
			decltype(status2)>>(conveyorStatus.status2);
		rfid = conveyorStatus.socketInfo.rfid;
		productSn = conveyorStatus.socketInfo.productSn;

		return true;
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("lt::CIcsClient::CommandConveyorStatusRequest failed %d"),
				result);

	return false;
}

lt::CLogger & CIcsCommunicator::GetLogger()
{
	return *m_pLogger;
}

lt::ILoggable & CIcsCommunicator::GetLogger() const
{
	return *m_pLogger;
}

void CIcsCommunicator::OnLogAddedEvent(LogAddedEventArgs & eventArgs)
{
	static constexpr LPCTSTR StrLogLevels[] =
	{
		_T("None"),
		_T("Info"),
		_T("Proc"),
		_T("Noti"),
		_T("Warn"),
		_T("Erro"),
		_T("Pass"),
		_T("Fail")
	};

	auto & logData = eventArgs.GetArgs();
	auto & loggingTime = logData.GetLoggingTime();
	CString strLog;

	strLog.Format(_T("%02d:%02d:%02d.%03d (%s) %s\r\n"),
				  loggingTime.wHour, loggingTime.wMinute,
				  loggingTime.wSecond, loggingTime.wMilliseconds,
				  StrLogLevels[static_cast<int>(logData.GetLogLevel())],
				  logData.GetLogString());

	WRITE_ICSCOMM_LOG(strLog);
}

lt::CTcpCommunicator & CIcsCommunicator::GetComm()
{
	return *m_pComm;
}

const lt::CTcpCommunicator & CIcsCommunicator::GetComm() const
{
	return *m_pComm;
}

void CIcsCommunicator::OnCommConnectedEvent(CommConnectionEventArgs & eventArgs)
{
	auto & tcpCommOption = reinterpret_cast<const lt::SSocketCommOption &>(eventArgs.GetArgs());

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d connected to the ICS"),
				tcpCommOption.remoteAddress,
				tcpCommOption.remotePort);

	GetClient().Attach(std::addressof(eventArgs.GetSender()));
}

void CIcsCommunicator::OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs)
{
	auto & tcpCommOption = reinterpret_cast<const lt::SSocketCommOption &>(eventArgs.GetArgs());

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d disconnected from the ICS"),
				tcpCommOption.remoteAddress,
				tcpCommOption.remotePort);

//	if(GetClient() != std::addressof(eventArgs.GetSender()))
//		return;

	GetClient().Detach();
}

lt::CIcsClientTester & CIcsCommunicator::GetClient()
{
	return *m_pClient;
}

const lt::CIcsClientTester & CIcsCommunicator::GetClient() const
{
	return *m_pClient;
}

void CIcsCommunicator::OnTimeSyncEvent(TimeSyncEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Time Sync (year = %d, month = %02d, day = %02d, hour = %02d, minute = %02d, second = %02d)"),
				reqArgs.year,
				reqArgs.month,
				reqArgs.day,
				reqArgs.hour,
				reqArgs.minute,
				reqArgs.second);

	SYSTEMTIME tm;
	GetLocalTime(&tm);

	tm.wYear = reqArgs.year;
	tm.wMonth = reqArgs.month;
	tm.wDay = reqArgs.day;
	tm.wHour = reqArgs.hour;
	tm.wMinute = reqArgs.minute;
	tm.wSecond = reqArgs.second;

	SetLocalTime(&tm);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnUserLevelEvent(UserLevelEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("User Level (user level = %d)"),
				reqArgs.userLevel);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnLanguageEvent(LanguageEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Language (language = %d)"),
				reqArgs.language);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnModelEvent(ModelEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Model (model = %d, socket type = %d)"),
				reqArgs.model,
				reqArgs.socketType);

	auto opt = DATAMANAGER->GetSemiAuto();
	opt->Angle = (enAngleType)reqArgs.model;
	opt->Socket = (enSocketType)reqArgs.socketType;

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnInitializationEvent(InitializationEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Initialization"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnResetEvent(ResetEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Reset"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnRunEvent(RunEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Run"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnStopEvent(StopEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Stop"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnBuzzerOffEvent(BuzzerOffEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Buzzer off"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnSocketAcceptingEvent(SocketAcceptingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Socket accepting (rfid = %s, flag = %d, info = %s)"),
				reqArgs.rfid,
				reqArgs.flag,
				reqArgs.info);

	//auto * p = SEQUENCE->GetPtrSeqConveyor1();
		
// 	p->SetBarcode(reqArgs.info);
// 	p->SetRFID(reqArgs.rfid);
// 
// 	p->SetAccept(reqArgs.flag);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnEjectAcceptingEvent(EjectAcceptingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Eject accepting (rfid = %s)"),
				reqArgs.rfid);

	auto p = SEQUENCE->GetPtrSeqPickerMgr();

	p->SetEject(true);
	
	args.SetResult(true);

	CString sz;
	sz = _T("Set Eject Command From ICS");
	WRITE_MAIN_LOG(sz);
	eventArgs.Cancel();
}

void CIcsCommunicator::OnProductStartEndEvent(ProductionStartEndEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Product start/end (flag = %d)"),
				reqArgs.flag);

	auto * p = SEQUENCE->GetPtrSeqPickerMgr();

	p->SetWorkEnd();

	args.SetResult(true);
	eventArgs.Cancel();
}
