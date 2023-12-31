#pragma once

#include "Comm/IcsRemote.h"

namespace lt
{
	class LTICS_LIB_CLASS CIcsRemoteTester final : public CIcsRemote
	{
	public:
		CIcsRemoteTester();
		CIcsRemoteTester(const CIcsRemoteTester &) = delete;
		~CIcsRemoteTester();

		CIcsRemoteTester & operator=(const CIcsRemoteTester &) = delete;

		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		uint32 CommandModel(const SModel & model,
							uint64 timeout);
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		TestResultEventNotifier<CIcsRemoteTester> &
			GetTestResultEventNotifier();
		//
		//////////////////////////////////////////////////////////////////////////////////

	private:
		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		bool ProcRequestTestResult(const DataCntr & dataCntr,
								   DataCntr & resultDataCntr);

		bool RaiseTestResultEvent(TestResultArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////

	private:
		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		using TestResultEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteTester, TestResultArgs>;

		TestResultEventNotifierImpl * m_pTestResultEventNotifier = nullptr;
		//
		//////////////////////////////////////////////////////////////////////////////////
	};
}
