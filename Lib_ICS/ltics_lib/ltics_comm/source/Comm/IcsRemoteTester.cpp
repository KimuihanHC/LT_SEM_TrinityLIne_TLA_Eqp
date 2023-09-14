#include "pch.h"
#include "Comm/IcsRemoteTester.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct SendingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	static constexpr uint16 Model = 0x0104;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

struct ReceivingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 TestResult = 0x0322;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

CIcsRemoteTester::CIcsRemoteTester()
: CIcsRemote()
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pTestResultEventNotifier = new TestResultEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::TestResult,
						reinterpret_cast<RequestProc>(
							&CIcsRemoteTester::ProcRequestTestResult));
	//
	//////////////////////////////////////////////////////////////////////////////////
}

CIcsRemoteTester::~CIcsRemoteTester()
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	delete m_pTestResultEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////
}

uint32 CIcsRemoteTester::CommandModel(const SModel & model,
									  uint64 timeout)
{
	return ProcCommand(SendingCommand::Model,
					   {
						   Format("%d", model.model),
						   Format("%d", model.socketType)
					   },
					   timeout);
}

bool CIcsRemoteTester::ProcRequestTestResult(const DataCntr & dataCntr,
											 DataCntr & resultDataCntr)
{
	if(dataCntr.size() < 3)
		return false;

	return RaiseTestResultEvent(
		{
			ToTypeChar(dataCntr[0]).c_str(),
			static_cast<int32>(strtoul(dataCntr[1].c_str(), nullptr, 10)),
			static_cast<uint32>(strtoul(dataCntr[2].c_str(), nullptr, 10))
		}
	);
}

bool CIcsRemoteTester::RaiseTestResultEvent(TestResultArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pTestResultEventNotifier,
					  args);
}

TestResultEventNotifier<CIcsRemoteTester> &
CIcsRemoteTester::GetTestResultEventNotifier()
{
	return *m_pTestResultEventNotifier;
}
