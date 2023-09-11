#include "pch.h"
#include "Comm/IcsClientTester.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct SendingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 TestResult = 0x0322;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

struct ReceivingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	static constexpr uint16 Model = 0x0104;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

CIcsClientTester::CIcsClientTester()
: CIcsClient()
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	m_pModelEventNotifier = new ModelEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::Model,
						reinterpret_cast<RequestProc>(
							&CIcsClientTester::ProcRequestModel));
	//
	//////////////////////////////////////////////////////////////////////////////////
}

CIcsClientTester::~CIcsClientTester()
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	delete m_pModelEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////
}

uint32 CIcsClientTester::CommandTestResult(const STestResult & testResult,
										   uint64 timeout)
{
	return ProcCommand(SendingCommand::TestResult,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(testResult.rfid),
						   #else
						   testResult.rfid,
						   #endif
						   Format("%d", testResult.result),
						   Format("%d", testResult.para)
					   },
					   timeout);
}

bool CIcsClientTester::ProcRequestModel(const DataCntr & dataCntr,
										DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 2)
		return false;

	return RaiseModelEvent(
		{
			static_cast<uint16>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(dataCntr[1].c_str(), nullptr, 10))
		}
	);
}

bool CIcsClientTester::RaiseModelEvent(ModelArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pModelEventNotifier,
					  args);
}

ModelEventNotifier<CIcsClientTester> &
CIcsClientTester::GetModelEventNotifier()
{
	return *m_pModelEventNotifier;
}
