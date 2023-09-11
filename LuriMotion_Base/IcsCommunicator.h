#pragma once

#include "Singleton.h"
#include "TypeDecl.h"
#include "IcsDef.h"
#include "Event/IcsCommEvent.h"

namespace lt
{
	class CDebugLogDispatcher;
	class ILogDispatcher;
	class ILogData;
	class CLogDispatcher;
	class ILoggable;
	class CLogger;
	class CTcpCommunicator;
	class CIcsClient;
	class CIcsClientTester;
	class ICommunicator;
	struct SCommOption;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
}

class CIcsCommunicator final : public CSington<CIcsCommunicator>
{
private:
	CIcsCommunicator(const CIcsCommunicator &) = delete;

	CIcsCommunicator & operator=(const CIcsCommunicator &) = delete;

public:
	CIcsCommunicator();
	~CIcsCommunicator();

	bool Connect(LPCTSTR strAddress, USHORT port);
	void Disconnect();

	bool IsConnected();
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	// ���� ID
	bool SendEquipmentId(LPCTSTR equipmentId);
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	// � ��� ����
	bool SendOperationMode(OperationMode mode);

	// ���� ���� ���� ��û
	bool SendSocketRequest(LPCTSTR rfid);

	// �˻� ��� ����
	bool SendTestResult(LPCTSTR rfid,
						INT32 resultCode,
						InspectionPortType type);

	// ��Ʈ Ŭ����
	bool SendPortClear(PortType type);

	// ���� ���� ����
	bool SendEquipmentStatus(EquipmentStatus status,
							 INT32 alarmCode,
							 LPCTSTR alarmReason);

	// ��Ʈ ���� ����
	bool SendPortStatus(PortType type,
						PortStatus status,
						LPCTSTR rfid,
						LPCTSTR productSn);

	// ��Ʈ ���� ��û
	bool SendPortStatusRequest(PortType type,
							   PortStatus & status,
							   CString & rfid,
							   CString & productSn);

	// �����̾� ���� ����
	bool SendConveyorStatus(ConveyorType type,
							ConveyorStatus1 status1,
							ConveyorStatus2 status2,
							LPCTSTR rfid,
							LPCTSTR productSn);

	// �����̾� ���� ��û
	bool SendConveyorStatusRequest(ConveyorType type,
								   ConveyorStatus1 & status1,
								   ConveyorStatus2 & status2,
								   CString & rfid,
								   CString & productSn);
	//
	//////////////////////////////////////////////////////////////////////////////////

private:
	// Log
	lt::CLogger & GetLogger();
	lt::ILoggable & GetLogger() const;

	using LogAddedEventArgs =
		lt::IEventArgs<lt::ILogDispatcher, lt::ILogData>;

	void OnLogAddedEvent(LogAddedEventArgs & eventArgs);

	// Comm
	lt::CTcpCommunicator & GetComm();
	const lt::CTcpCommunicator & GetComm() const;

	using CommConnectionEventArgs =
		lt::IEventArgs<lt::ICommunicator, const lt::SCommOption>;

	void OnCommConnectedEvent(CommConnectionEventArgs & eventArgs);
	void OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs);

	// Client
	lt::CIcsClientTester & GetClient();
	const lt::CIcsClientTester & GetClient() const;

	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	// �ð� ����
	using TimeSyncEventArgs =
		lt::TimeSyncEventArgs<lt::CIcsClient>;

	void OnTimeSyncEvent(TimeSyncEventArgs & eventArgs);

	// �����
	using UserLevelEventArgs =
		lt::UserLevelEventArgs<lt::CIcsClient>;

	void OnUserLevelEvent(UserLevelEventArgs & eventArgs);

	// ���
	using LanguageEventArgs =
		lt::LanguageEventArgs<lt::CIcsClient>;

	void OnLanguageEvent(LanguageEventArgs & eventArgs);

	// ��
	using ModelEventArgs =
		lt::ModelEventArgs<lt::CIcsClientTester>;

	void OnModelEvent(ModelEventArgs & eventArgs);
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	// �ʱ�ȭ
	using InitializationEventArgs =
		lt::InitializationEventArgs<lt::CIcsClient>;

	void OnInitializationEvent(InitializationEventArgs & eventArgs);

	// ����
	using ResetEventArgs =
		lt::ResetEventArgs<lt::CIcsClient>;

	void OnResetEvent(ResetEventArgs & eventArgs);

	// ����
	using RunEventArgs =
		lt::RunEventArgs<lt::CIcsClient>;

	void OnRunEvent(RunEventArgs & eventArgs);

	// ����
	using StopEventArgs =
		lt::StopEventArgs<lt::CIcsClient>;

	void OnStopEvent(StopEventArgs & eventArgs);

	// ���� ����
	using BuzzerOffEventArgs =
		lt::BuzzerOffEventArgs<lt::CIcsClient>;

	void OnBuzzerOffEvent(BuzzerOffEventArgs & eventArgs);
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	// � ���
	// ���� ���� ����
	using SocketAcceptingEventArgs =
		lt::SocketAcceptingEventArgs<lt::CIcsClient>;

	void OnSocketAcceptingEvent(SocketAcceptingEventArgs & eventArgs);

	// ���� ����
	using EjectAcceptingEventArgs =
		lt::EjectAcceptingEventArgs<lt::CIcsClient>;

	void OnEjectAcceptingEvent(EjectAcceptingEventArgs & eventArgs);

	// Product End
	using ProductionStartEndEventArgs =
		lt::ProductionStartEndEventArgs<lt::CIcsClient>;

	void OnProductStartEndEvent(ProductionStartEndEventArgs & eventArgs);

	//
	//////////////////////////////////////////////////////////////////////////////////

private:
	// Log
	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;

	using LogAddedEventListener =
		lt::CEventMfnListener<CIcsCommunicator, LogAddedEventArgs>;

	LogAddedEventListener * m_pLogAddedEventListener = nullptr;

	lt::CLogDispatcher * m_pLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	// Comm
	lt::CTcpCommunicator * m_pComm = nullptr;

	using CommConnectionEventListener =
		lt::CEventMfnListener<CIcsCommunicator, CommConnectionEventArgs>;

	CommConnectionEventListener * m_pCommConnectedEventListener = nullptr;
	CommConnectionEventListener * m_pCommDisconnectingEventListener = nullptr;

	// Client
	lt::CIcsClientTester * m_pClient = nullptr;

	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	using TimeSyncEventListener =
		lt::CEventMfnListener<CIcsCommunicator, TimeSyncEventArgs>;

	TimeSyncEventListener * m_pTimeSyncEventListener = nullptr;

	using UserLevelEventListener =
		lt::CEventMfnListener<CIcsCommunicator, UserLevelEventArgs>;

	UserLevelEventListener * m_pUserLevelEventListener = nullptr;

	using LanguageEventListener =
		lt::CEventMfnListener<CIcsCommunicator, LanguageEventArgs>;

	LanguageEventListener * m_pLanguageEventListener = nullptr;

	using ModelEventListener =
		lt::CEventMfnListener<CIcsCommunicator, ModelEventArgs>;

	ModelEventListener * m_pModelEventListener = nullptr;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	using InitializationEventListener =
		lt::CEventMfnListener<CIcsCommunicator, InitializationEventArgs>;

	InitializationEventListener * m_pInitializationEventListener = nullptr;

	using ResetEventListener =
		lt::CEventMfnListener<CIcsCommunicator, ResetEventArgs>;

	ResetEventListener * m_pResetEventListener = nullptr;

	using RunEventListener =
		lt::CEventMfnListener<CIcsCommunicator, RunEventArgs>;

	RunEventListener * m_pRunEventListener = nullptr;

	using StopEventListener =
		lt::CEventMfnListener<CIcsCommunicator, StopEventArgs>;

	StopEventListener * m_pStopEventListener = nullptr;

	using BuzzerOffEventListener =
		lt::CEventMfnListener<CIcsCommunicator, BuzzerOffEventArgs>;

	BuzzerOffEventListener * m_pBuzzerOffEventListener = nullptr;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	using SocketAcceptingEventListener =
		lt::CEventMfnListener<CIcsCommunicator, SocketAcceptingEventArgs>;

	SocketAcceptingEventListener * m_pSocketAcceptingEventListener = nullptr;

	using EjectAcceptingEventListener =
		lt::CEventMfnListener<CIcsCommunicator, EjectAcceptingEventArgs>;

	EjectAcceptingEventListener * m_pEjectAcceptingEventListener = nullptr;

	// product start/end
	using ProductStartEndEventListener =
		lt::CEventMfnListener<CIcsCommunicator, ProductionStartEndEventArgs>;

	ProductStartEndEventListener * m_pProductStartEndEventListener = nullptr;

	//
	//////////////////////////////////////////////////////////////////////////////////
};
