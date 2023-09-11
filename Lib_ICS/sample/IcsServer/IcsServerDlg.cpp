
// IcsServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "IcsServer.h"
#include "IcsServerDlg.h"
#include "afxdialogex.h"
#include "RichEditCtrlEx.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/Logger.h"
#include "Logger/LogDispatcher.h"
#include "Comm/IcsTcpServer.h"
#include "Comm/SocketCommOption.h"
#include "Comm/IcsRemoteLoUnloader.h"
#include "Comm/IcsRemoteTester.h"
#include "Event/EventMfnListener.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/BasicTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIcsServerDlg 대화 상자



CIcsServerDlg::CIcsServerDlg(CWnd* pParent /*=nullptr*/)
: CDialogEx(IDD_ICSSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pRichEditCtrl = new CRichEditCtrlEx();

	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(_T("ICS_SERVER"), lt::LogFilter::Detail);
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Server begins =============================")
	);

	m_pLogAddedEventListener =
		new LogAddedEventListener(this, &CIcsServerDlg::OnLogAddedEvent);

	m_pLogDispatcher = new lt::CLogDispatcher();
	m_pLogDispatcher->SetLogFilter(lt::LogFilter::Detail);
	m_pLogDispatcher->GetLogAddedEventNotifier() +=
		m_pLogAddedEventListener;

	m_pLogger = new lt::CLogger();
	m_pLogger->Attach(m_pLogDispatcher);

	m_pServer = new lt::CIcsTcpServer();
	m_pServer->SetLogDispatcher(*m_pDebugLogDispatcher);

	m_pRemoteMutex = new lt::StdMutex();

	// Server
	m_pRemoteConnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsServerDlg::OnRemoteConnectedEvent);
	m_pRemoteDisconnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsServerDlg::OnRemoteDisconnectedEvent);

	m_pServer->GetRemoteConnectedEventNotifier() +=
		m_pRemoteConnectedEventListener;
	m_pServer->GetRemoteDisconnectedEventNotifier() +=
		m_pRemoteDisconnectedEventListener;

	// Common
	m_pEquipmentIdEventListener =
		new EquipmentIdEventListener(this, &CIcsServerDlg::OnEquipmentIdEvent);
	m_pEquipmentStatusEventListener =
		new EquipmentStatusEventListener(this, &CIcsServerDlg::OnEquipmentStatusEvent);
	m_pOperationModeEventListener =
		new OperationModeEventListener(this, &CIcsServerDlg::OnOperationModeEvent);
	m_pSocketRequestEventListener =
		new SocketRequestEventListener(this, &CIcsServerDlg::OnSocketRequestEvent);
	m_pPortClearEventListener =
		new PortClearEventListener(this, &CIcsServerDlg::OnPortClearEvent);
	m_pPortStatusEventListener =
		new PortStatusEventListener(this, &CIcsServerDlg::OnPortStatusEvent);
	m_pPortStatusRequestEventListener =
		new PortStatusRequestEventListener(this, &CIcsServerDlg::OnPortStatusRequestEvent);
	m_pConveyorStatusEventListener =
		new ConveyorStatusEventListener(this, &CIcsServerDlg::OnConveyorStatusEvent);
	m_pConveyorStatusRequestEventListener =
		new ConveyorStatusRequestEventListener(this, &CIcsServerDlg::OnConveyorStatusRequestEvent);

	// LoUnloader
	m_pLoadingEventListener =
		new LoadingEventListener(this, &CIcsServerDlg::OnLoadingEvent);
	m_pTestResultRequestEventListener =
		new TestResultRequestEventListener(this, &CIcsServerDlg::OnTestResultRequestEvent);
	m_pUnloadingEventListener =
		new UnloadingEventListener(this, &CIcsServerDlg::OnUnloadingEvent);

	// Tester
	m_pTestResultEventListener =
		new TestResultEventListener(this, &CIcsServerDlg::OnTestResultEvent);
}

CIcsServerDlg::~CIcsServerDlg()
{
	// Common
	delete m_pEquipmentIdEventListener;
	delete m_pEquipmentStatusEventListener;
	delete m_pOperationModeEventListener;
	delete m_pSocketRequestEventListener;
	delete m_pPortClearEventListener;
	delete m_pPortStatusEventListener;
	delete m_pPortStatusRequestEventListener;
	delete m_pConveyorStatusEventListener;
	delete m_pConveyorStatusRequestEventListener;

	// LoUnloader
	delete m_pLoadingEventListener;
	delete m_pTestResultRequestEventListener;
	delete m_pUnloadingEventListener;

	// Tester
	delete m_pTestResultEventListener;

	// Server
	m_pServer->GetRemoteConnectedEventNotifier() -=
		m_pRemoteConnectedEventListener;
	m_pServer->GetRemoteDisconnectedEventNotifier() -=
		m_pRemoteDisconnectedEventListener;

	delete m_pRemoteConnectedEventListener;
	delete m_pRemoteDisconnectedEventListener;

	delete m_pRemoteMutex;

	m_pServer->SetLogDispatcher(nullptr);
	delete m_pServer;

	m_pLogger->Detach();
	delete m_pLogger;

	m_pLogDispatcher->GetLogAddedEventNotifier() -=
		m_pLogAddedEventListener;
	delete m_pLogDispatcher;

	delete m_pLogAddedEventListener;

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Server ends =============================")
	);
	Sleep(100);
	delete m_pDebugLogDispatcher;

	delete m_pRichEditCtrl;
}

void CIcsServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RICHEDIT_LOG, *m_pRichEditCtrl);
}

BEGIN_MESSAGE_MAP(CIcsServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CIcsServerDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CIcsServerDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_TIME_SYNC, &CIcsServerDlg::OnBnClickedButtonTimeSync)
	ON_BN_CLICKED(IDC_BUTTON_USER_LEVEL, &CIcsServerDlg::OnBnClickedButtonUserLevel)
	ON_BN_CLICKED(IDC_BUTTON_LANGUAGE, &CIcsServerDlg::OnBnClickedButtonLanguage)
	ON_BN_CLICKED(IDC_BUTTON_MODEL, &CIcsServerDlg::OnBnClickedButtonModel)
	ON_BN_CLICKED(IDC_BUTTON_INITIALIZE, &CIcsServerDlg::OnBnClickedButtonInitialize)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CIcsServerDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CIcsServerDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CIcsServerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_BUZZER_OFF, &CIcsServerDlg::OnBnClickedButtonBuzzerOff)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, &CIcsServerDlg::OnBnClickedButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, &CIcsServerDlg::OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_ACCEPT, &CIcsServerDlg::OnBnClickedButtonSocketAccept)
	ON_BN_CLICKED(IDC_BUTTON_EJECT_ACCEPT, &CIcsServerDlg::OnBnClickedButtonEjectAccept)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCTION_START_END, &CIcsServerDlg::OnBnClickedButtonProductionStartEnd)
	ON_BN_CLICKED(IDC_BUTTON_FORCE_EJECT, &CIcsServerDlg::OnBnClickedButtonForceEject)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE, &CIcsServerDlg::OnBnClickedButtonActive)
	ON_BN_CLICKED(IDC_BUTTON_INACTIVE, &CIcsServerDlg::OnBnClickedButtonInactive)
END_MESSAGE_MAP()


// CIcsServerDlg 메시지 처리기

BOOL CIcsServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	EnableConnectionCtrls(true);
	EnableCommunicationCtrls(false);

	SetDlgItemInt(IDC_EDIT_PORT, 5000);
	static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_EQUIPMENT))->SetCurSel(0);
	m_pRichEditCtrl->SetReadOnly();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CIcsServerDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pServer->IsOpened())
		OnBnClickedButtonClose();

	RemoveRemote();

	for(lt::CBasicTimer timer;;)
	{
		MSG msg = {};

		if(!::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) > 100)
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return CDialogEx::DestroyWindow();
}

void CIcsServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIcsServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIcsServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIcsServerDlg::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	auto equipmentType = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_EQUIPMENT))->GetCurSel();

	CreateRemote(equipmentType);

	auto port = static_cast<lt::ushort>(GetDlgItemInt(IDC_EDIT_PORT));
	size_t sizeOfRxBuffer = 1024;

	if(!m_pServer->Open(nullptr, port, sizeOfRxBuffer))
	{
		RemoveRemote();
		return;
	}

	EnableConnectionCtrls(false);

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The server opened (equipment = %d, port = %d, rx buffer = %d)"),
				equipmentType,
				port,
				sizeOfRxBuffer);
}

void CIcsServerDlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pServer->Close();

	RemoveRemote();

	EnableConnectionCtrls(true);

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The server closed"));
}

void CIcsServerDlg::OnBnClickedButtonTimeSync()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendTimeSync, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonUserLevel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendUserLevel, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonLanguage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendLanguage, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendModel, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonInitialize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendInitialize, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendReset, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendRun, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendStop, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonBuzzerOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendBuzzerOff, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonMinimize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendUiVisible, this, SW_MINIMIZE)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonRestore()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendUiVisible, this, SW_RESTORE)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonSocketAccept()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendSocketAccept, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonEjectAccept()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendEjectAccept, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonProductionStartEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendProductionStartEnd, this)
		.detach();
}

void CIcsServerDlg::OnBnClickedButtonForceEject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsServerDlg::SendForceEject, this)
		.detach();
}

template<typename TypeRes, typename... TypeArgs>
TypeRes CIcsServerDlg::InvokeRemote(TypeRes (lt::CIcsRemote::*fn)(TypeArgs...),
									TypeArgs&&... args)
{
	SimpleLockPtr(m_pRemoteMutex);

	return (m_pRemote->*fn)(std::forward<TypeArgs>(args)...);
}

bool CIcsServerDlg::CreateRemote(int equipmentType)
{
	SimpleLockPtr(m_pRemoteMutex);

	if(m_pRemote != nullptr)
		return false;

	if(equipmentType == 0)
	{
		auto remotePtr = new lt::CIcsRemoteLoUnloader();

		remotePtr->GetLoadingEventNotifier() +=
			m_pLoadingEventListener;
		remotePtr->GetTestResultRequestEventNotifier() +=
			m_pTestResultRequestEventListener;
		remotePtr->GetUnloadingEventNotifier() +=
			m_pUnloadingEventListener;

		m_pRemote = remotePtr;
	}
	else if(equipmentType == 1)
	{
		auto remotePtr = new lt::CIcsRemoteTester();

		remotePtr->GetTestResultEventNotifier() +=
			m_pTestResultEventListener;

		m_pRemote = remotePtr;
	}
	else
	{
		m_pRemote = new lt::CIcsRemote();
	}

	if(m_pRemote == nullptr)
		return false;

	m_pRemote->GetEquipmentIdEventNotifier() +=
		m_pEquipmentIdEventListener;
	m_pRemote->GetEquipmentStatusEventNotifier() +=
		m_pEquipmentStatusEventListener;
	m_pRemote->GetOperationModeEventNotifier() +=
		m_pOperationModeEventListener;
	m_pRemote->GetSocketRequestEventNotifier() +=
		m_pSocketRequestEventListener;
	m_pRemote->GetPortClearEventNotifier() +=
		m_pPortClearEventListener;
	m_pRemote->GetPortStatusEventNotifier() +=
		m_pPortStatusEventListener;
	m_pRemote->GetPortStatusRequestEventNotifier() +=
		m_pPortStatusRequestEventListener;
	m_pRemote->GetConveyorStatusEventNotifier() +=
		m_pConveyorStatusEventListener;
	m_pRemote->GetConveyorStatusRequestEventNotifier() +=
		m_pConveyorStatusRequestEventListener;
	m_pRemote->SetLogDispatcher(m_pLogDispatcher);

	m_equipmentType = equipmentType;

	return true;
}

void CIcsServerDlg::RemoveRemote()
{
	SimpleLockPtr(m_pRemoteMutex);

	if(m_pRemote == nullptr)
		return;

	m_pRemote->GetEquipmentIdEventNotifier() -=
		m_pEquipmentIdEventListener;
	m_pRemote->GetEquipmentStatusEventNotifier() -=
		m_pEquipmentStatusEventListener;
	m_pRemote->GetOperationModeEventNotifier() -=
		m_pOperationModeEventListener;
	m_pRemote->GetSocketRequestEventNotifier() -=
		m_pSocketRequestEventListener;
	m_pRemote->GetPortClearEventNotifier() -=
		m_pPortClearEventListener;
	m_pRemote->GetPortStatusEventNotifier() -=
		m_pPortStatusEventListener;
	m_pRemote->GetPortStatusRequestEventNotifier() -=
		m_pPortStatusRequestEventListener;
	m_pRemote->GetConveyorStatusEventNotifier() -=
		m_pConveyorStatusEventListener;
	m_pRemote->GetConveyorStatusRequestEventNotifier() -=
		m_pConveyorStatusRequestEventListener;

	if(dynamic_cast<lt::CIcsRemoteLoUnloader *>(m_pRemote) != nullptr)
	{
		auto remotePtr = dynamic_cast<lt::CIcsRemoteLoUnloader *>(m_pRemote);

		remotePtr->GetLoadingEventNotifier() -=
			m_pLoadingEventListener;
		remotePtr->GetTestResultRequestEventNotifier() -=
			m_pTestResultRequestEventListener;
		remotePtr->GetUnloadingEventNotifier() -=
			m_pUnloadingEventListener;
	}
	else if(dynamic_cast<lt::CIcsRemoteTester *>(m_pRemote) != nullptr)
	{
		auto remotePtr = dynamic_cast<lt::CIcsRemoteTester *>(m_pRemote);

		remotePtr->GetTestResultEventNotifier() -=
			m_pTestResultEventListener;

	}

	m_pRemote->SetLogDispatcher(nullptr);
	delete m_pRemote;
	m_pRemote = nullptr;
}

void CIcsServerDlg::EnableConnectionCtrls(bool enable)
{
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_EQUIPMENT)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(!enable);
}

void CIcsServerDlg::EnableCommunicationCtrls(bool enable)
{
	static constexpr struct
	{
		int id = 0;
		bool enable[3] = {};
	} Ctrls[] =
	{
		{ IDC_BUTTON_TIME_SYNC, true, true, true },
		{ IDC_EDIT_USER_LEVEL, true, true, true },
		{ IDC_EDIT_USER_ID, true, true, true },
		{ IDC_BUTTON_USER_LEVEL, true, true, true },
		{ IDC_EDIT_LANGUAGE, true, true, true },
		{ IDC_BUTTON_LANGUAGE, true, true, true },
		{ IDC_EDIT_EQUIPMENT_ID, true, true, true },
		{ IDC_EDIT_MODEL, true, true, true },
		{ IDC_EDIT_SOCKET_TYPE, true, true, true },
		{ IDC_BUTTON_MODEL, true, true, true },
		{ IDC_BUTTON_INITIALIZE, true, true, true },
		{ IDC_BUTTON_RESET, true, true, true },
		{ IDC_BUTTON_RUN, true, true, true },
		{ IDC_BUTTON_STOP, true, true, true },
		{ IDC_BUTTON_BUZZER_OFF, true, true, true },
		{ IDC_EDIT_OPERATION_MODE, true, true, true },
		{ IDC_EDIT_RFID_REGISTERED_RFID, true, false, false },
		{ IDC_EDIT_RFID_REGISTERED_BARCODE, true, false, false },
		{ IDC_EDIT_RFID_TEST_RESULT_RFID, false, true, false },
		{ IDC_EDIT_RFID_TEST_RESULT_ERROR, false, true, false },
		{ IDC_EDIT_RFID_TEST_RESULT_PARA, false, true, false },
		{ IDC_EDIT_SOCKET_REQUEST_RFID, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_RFID, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_FLAG, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_BARCODE, true, true, true },
		{ IDC_BUTTON_SOCKET_ACCEPT, true, true, true },
		{ IDC_EDIT_EJECT_ACCEPT_RFID, true, true, true },
		{ IDC_BUTTON_EJECT_ACCEPT, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS_ERROR, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS_REASON, true, true, true },
		{ IDC_EDIT_PORT_CLEAR, true, true, true },
		{ IDC_EDIT_PORT_STATUS_1, true, true, true },
		{ IDC_EDIT_PORT_STATUS_2, true, true, true },
		{ IDC_EDIT_PORT_STATUS_3, true, true, true },
		{ IDC_EDIT_PORT_STATUS_4, true, true, false },
		{ IDC_EDIT_PORT_STATUS_5, true, false, false },
		{ IDC_EDIT_PORT_STATUS_6, true, false, false },
		{ IDC_EDIT_PORT_STATUS_7, true, false, false },
		{ IDC_EDIT_PORT_STATUS_8, true, false, false },
		{ IDC_EDIT_PORT_STATUS_9, true, false, false },
		{ IDC_EDIT_PORT_STATUS_10, true, false, false },
		{ IDC_EDIT_PORT_STATUS_11, true, false, false },
		{ IDC_EDIT_PORT_RFID_1, true, true, true },
		{ IDC_EDIT_PORT_RFID_2, true, true, true },
		{ IDC_EDIT_PORT_RFID_3, true, true, true },
		{ IDC_EDIT_PORT_RFID_4, true, true, false },
		{ IDC_EDIT_PORT_RFID_5, true, false, false },
		{ IDC_EDIT_PORT_RFID_6, true, false, false },
		{ IDC_EDIT_PORT_RFID_7, true, false, false },
		{ IDC_EDIT_PORT_RFID_8, true, false, false },
		{ IDC_EDIT_PORT_RFID_9, true, false, false },
		{ IDC_EDIT_PORT_RFID_10, true, false, false },
		{ IDC_EDIT_PORT_RFID_11, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_1, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS1_2, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS1_3, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS1_4, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_5, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_6, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_7, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_8, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_9, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS1_10, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_1, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS2_2, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS2_3, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS2_4, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_5, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_6, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_7, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_8, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_9, true, false, false },
		{ IDC_EDIT_CONVEYOR_STATUS2_10, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_1, true, true, true },
		{ IDC_EDIT_CONVEYOR_RFID_2, true, true, true },
		{ IDC_EDIT_CONVEYOR_RFID_3, true, true, true },
		{ IDC_EDIT_CONVEYOR_RFID_4, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_5, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_6, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_7, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_8, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_9, true, false, false },
		{ IDC_EDIT_CONVEYOR_RFID_10, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_1, true, true, true },
		{ IDC_EDIT_CONVEYOR_BARCODE_2, true, true, true },
		{ IDC_EDIT_CONVEYOR_BARCODE_3, true, true, true },
		{ IDC_EDIT_CONVEYOR_BARCODE_4, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_5, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_6, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_7, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_8, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_9, true, false, false },
		{ IDC_EDIT_CONVEYOR_BARCODE_10, true, false, false },
		{ IDC_BUTTON_PRODUCTION_START_END, true, true, true },
		{ IDC_EDIT_FORCE_EJECT, true, true, true },
		{ IDC_BUTTON_FORCE_EJECT, true, true, true },
		{ IDC_BUTTON_MINIMIZE, true, true, true },
		{ IDC_BUTTON_RESTORE, true, true, true },
		{ IDC_EDIT_PRODUCTION_START_END, true, true, true }
	};

	for(auto index = 0; index < _countof(Ctrls); ++index)
	{
		auto ctrlPtr = GetDlgItem(Ctrls[index].id);

		if(ctrlPtr == nullptr)
			continue;

		ctrlPtr->EnableWindow(enable && Ctrls[index].enable[m_equipmentType]);
	}

	if(!enable)
		ClearData();
}

void CIcsServerDlg::ClearData()
{
	static constexpr int Ids[] =
	{
		IDC_EDIT_USER_LEVEL,
		IDC_EDIT_USER_ID,
		IDC_EDIT_LANGUAGE,
		IDC_EDIT_EQUIPMENT_ID,
		IDC_EDIT_MODEL,
		IDC_EDIT_SOCKET_TYPE,
		IDC_EDIT_OPERATION_MODE,
		IDC_EDIT_RFID_REGISTERED_RFID,
		IDC_EDIT_RFID_REGISTERED_BARCODE,
		IDC_EDIT_RFID_TEST_RESULT_RFID,
		IDC_EDIT_RFID_TEST_RESULT_ERROR,
		IDC_EDIT_RFID_TEST_RESULT_PARA,
		IDC_EDIT_SOCKET_REQUEST_RFID,
		IDC_EDIT_SOCKET_ACCEPT_RFID,
		IDC_EDIT_SOCKET_ACCEPT_FLAG,
		IDC_EDIT_SOCKET_ACCEPT_BARCODE,
		IDC_EDIT_EJECT_ACCEPT_RFID,
		IDC_EDIT_EQUIPMENT_STATUS,
		IDC_EDIT_EQUIPMENT_STATUS_ERROR,
		IDC_EDIT_EQUIPMENT_STATUS_REASON,
		IDC_EDIT_PORT_CLEAR,
		IDC_EDIT_PORT_STATUS_1,
		IDC_EDIT_PORT_STATUS_2,
		IDC_EDIT_PORT_STATUS_3,
		IDC_EDIT_PORT_STATUS_4,
		IDC_EDIT_PORT_STATUS_5,
		IDC_EDIT_PORT_STATUS_6,
		IDC_EDIT_PORT_STATUS_7,
		IDC_EDIT_PORT_STATUS_8,
		IDC_EDIT_PORT_STATUS_9,
		IDC_EDIT_PORT_STATUS_10,
		IDC_EDIT_PORT_STATUS_11,
		IDC_EDIT_PORT_RFID_1,
		IDC_EDIT_PORT_RFID_2,
		IDC_EDIT_PORT_RFID_3,
		IDC_EDIT_PORT_RFID_4,
		IDC_EDIT_PORT_RFID_5,
		IDC_EDIT_PORT_RFID_6,
		IDC_EDIT_PORT_RFID_7,
		IDC_EDIT_PORT_RFID_8,
		IDC_EDIT_PORT_RFID_9,
		IDC_EDIT_PORT_RFID_10,
		IDC_EDIT_PORT_RFID_11,
		IDC_EDIT_PORT_BARCODE_1,
		IDC_EDIT_PORT_BARCODE_2,
		IDC_EDIT_PORT_BARCODE_3,
		IDC_EDIT_PORT_BARCODE_4,
		IDC_EDIT_PORT_BARCODE_5,
		IDC_EDIT_PORT_BARCODE_6,
		IDC_EDIT_PORT_BARCODE_7,
		IDC_EDIT_PORT_BARCODE_8,
		IDC_EDIT_PORT_BARCODE_9,
		IDC_EDIT_PORT_BARCODE_10,
		IDC_EDIT_PORT_BARCODE_11,
		IDC_EDIT_CONVEYOR_STATUS1_1,
		IDC_EDIT_CONVEYOR_STATUS1_2,
		IDC_EDIT_CONVEYOR_STATUS1_3,
		IDC_EDIT_CONVEYOR_STATUS1_4,
		IDC_EDIT_CONVEYOR_STATUS1_5,
		IDC_EDIT_CONVEYOR_STATUS1_6,
		IDC_EDIT_CONVEYOR_STATUS1_7,
		IDC_EDIT_CONVEYOR_STATUS1_8,
		IDC_EDIT_CONVEYOR_STATUS1_9,
		IDC_EDIT_CONVEYOR_STATUS1_10,
		IDC_EDIT_CONVEYOR_STATUS2_1,
		IDC_EDIT_CONVEYOR_STATUS2_2,
		IDC_EDIT_CONVEYOR_STATUS2_3,
		IDC_EDIT_CONVEYOR_STATUS2_4,
		IDC_EDIT_CONVEYOR_STATUS2_5,
		IDC_EDIT_CONVEYOR_STATUS2_6,
		IDC_EDIT_CONVEYOR_STATUS2_7,
		IDC_EDIT_CONVEYOR_STATUS2_8,
		IDC_EDIT_CONVEYOR_STATUS2_9,
		IDC_EDIT_CONVEYOR_STATUS2_10,
		IDC_EDIT_CONVEYOR_RFID_1,
		IDC_EDIT_CONVEYOR_RFID_2,
		IDC_EDIT_CONVEYOR_RFID_3,
		IDC_EDIT_CONVEYOR_RFID_4,
		IDC_EDIT_CONVEYOR_RFID_5,
		IDC_EDIT_CONVEYOR_RFID_6,
		IDC_EDIT_CONVEYOR_RFID_7,
		IDC_EDIT_CONVEYOR_RFID_8,
		IDC_EDIT_CONVEYOR_RFID_9,
		IDC_EDIT_CONVEYOR_RFID_10,
		IDC_EDIT_CONVEYOR_BARCODE_1,
		IDC_EDIT_CONVEYOR_BARCODE_2,
		IDC_EDIT_CONVEYOR_BARCODE_3,
		IDC_EDIT_CONVEYOR_BARCODE_4,
		IDC_EDIT_CONVEYOR_BARCODE_5,
		IDC_EDIT_CONVEYOR_BARCODE_6,
		IDC_EDIT_CONVEYOR_BARCODE_7,
		IDC_EDIT_CONVEYOR_BARCODE_8,
		IDC_EDIT_CONVEYOR_BARCODE_9,
		IDC_EDIT_CONVEYOR_BARCODE_10,
		IDC_EDIT_FORCE_EJECT,
		IDC_EDIT_PRODUCTION_START_END
	};

	for(auto index = 0; index < _countof(Ids); ++index)
	{
		auto ctrlPtr = GetDlgItem(Ids[index]);

		if(ctrlPtr == nullptr)
			continue;

		ctrlPtr->SetWindowText(nullptr);
	}
}

void CIcsServerDlg::ClearData(LPCTSTR rfid)
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_RFID_REGISTERED_RFID, strRfid);

	if(strRfid.Compare(rfid) == 0)
	{
		SetDlgItemText(IDC_EDIT_RFID_REGISTERED_RFID, nullptr);
		SetDlgItemText(IDC_EDIT_RFID_REGISTERED_BARCODE, nullptr);
	}

	GetDlgItemText(IDC_EDIT_SOCKET_REQUEST_RFID, strRfid);

	if(strRfid.Compare(rfid) == 0)
	{
		SetDlgItemText(IDC_EDIT_SOCKET_REQUEST_RFID, nullptr);
	}

	GetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_RFID, strRfid);

	if(strRfid.Compare(rfid) == 0)
	{
		SetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_RFID, nullptr);
		SetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_ERROR, nullptr);
		SetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_PARA, nullptr);
	}
}

void CIcsServerDlg::OnLogAddedEvent(LogAddedEventArgs & eventArgs)
{
	static constexpr DWORD CharFormatMask = CFM_BOLD | CFM_FACE | CFM_COLOR | CFM_SIZE;
	static constexpr LONG CharHeight = 210;
	static constexpr struct
	{
		LPCTSTR strLogLevel = nullptr;
		CHARFORMAT charFormat = {};
	} LogLevelFormats[] =
	{
		{
			_T("None"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 100, 100), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Info"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 100, 100), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Proc"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				0, // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Noti"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 60, 20), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Warn"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				CFE_BOLD, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				0, // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Erro"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				CFE_BOLD, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(255, 0, 0), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Pass"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(0, 0, 255), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Fail"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(255, 0, 0), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		}
	};

	auto & logData = eventArgs.GetArgs();
	auto & loggingTime = logData.GetLoggingTime();
	auto & logLevelFormat = LogLevelFormats[static_cast<int>(logData.GetLogLevel())];
	CString strLog;

	strLog.Format(_T("%02d:%02d:%02d.%03d (%s) %s\r\n"),
				  loggingTime.wHour, loggingTime.wMinute,
				  loggingTime.wSecond, loggingTime.wMilliseconds,
				  logLevelFormat.strLogLevel,
				  logData.GetLogString());

	m_pRichEditCtrl->SetSel(-1, -1);
	m_pRichEditCtrl->SetSelectionCharFormat(CHARFORMAT(logLevelFormat.charFormat));
	m_pRichEditCtrl->ReplaceSel(strLog);
}

void CIcsServerDlg::OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs)
{
	SimpleLockPtr(m_pRemoteMutex);

	if(m_pRemote == nullptr)
		return;

	auto & comm = eventArgs.GetArgs();
	auto & commOption = reinterpret_cast<const lt::SSocketCommOption &>(comm.GetOption());

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d connected"),
				commOption.remoteAddress,
				commOption.remotePort);

	m_pRemote->Attach(std::addressof(comm));

	EnableCommunicationCtrls(true);
}

void CIcsServerDlg::OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs)
{
	SimpleLockPtr(m_pRemoteMutex);

	if(m_pRemote == nullptr)
		return;

	auto & comm = eventArgs.GetArgs();

	if(*m_pRemote != std::addressof(comm))
		return;

	EnableCommunicationCtrls(false);

	auto & tcpCommOption = reinterpret_cast<const lt::SSocketCommOption &>(comm.GetOption());

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d disconnected"),
				tcpCommOption.remoteAddress,
				tcpCommOption.remotePort);

	m_pRemote->Detach();
}

void CIcsServerDlg::SendTimeSync()
{
	SYSTEMTIME time = {};

	::GetLocalTime(&time);

	lt::STimeSync timeSync =
	{
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		time.wSecond
	};

	InvokeRemote(&lt::CIcsRemote::CommandTimeSync,
				 std::forward<const lt::STimeSync &>(timeSync),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendUserLevel()
{
	CString strUserId;
	GetDlgItemText(IDC_EDIT_USER_ID, strUserId);

	lt::SUserLevel userLevel =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_USER_LEVEL)),
		strUserId
	};

	InvokeRemote(&lt::CIcsRemote::CommandUserLevel,
				 std::forward<const lt::SUserLevel &>(userLevel),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendLanguage()
{
	lt::SLanguage language =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_LANGUAGE))
	};

	InvokeRemote(&lt::CIcsRemote::CommandLanguage,
				 std::forward<const lt::SLanguage &>(language),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendModel()
{
	lt::SModel model =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_MODEL)),
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_SOCKET_TYPE))
	};

	InvokeRemote(&lt::CIcsRemote::CommandModel,
				 std::forward<const lt::SModel &>(model),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendInitialize()
{
	InvokeRemote(&lt::CIcsRemoteTester::CommandInitialization,
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendReset()
{
	InvokeRemote(&lt::CIcsRemoteTester::CommandReset,
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendRun()
{
	InvokeRemote(&lt::CIcsRemoteTester::CommandRun,
				 std::forward<lt::uint64>(5000));
}

void CIcsServerDlg::SendStop()
{
	InvokeRemote(&lt::CIcsRemoteTester::CommandStop,
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendBuzzerOff()
{
	InvokeRemote(&lt::CIcsRemoteTester::CommandBuzzerOff,
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendUiVisible(int cmdShow)
{
	lt::SUiVisible uiVisible =
	{
		cmdShow
	};

	InvokeRemote(&lt::CIcsRemoteTester::CommandUiVisible,
				 std::forward<const lt::SUiVisible &>(uiVisible),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendSocketAccept()
{
	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_SOCKET_ACCEPT_RFID, strRfid);
	GetDlgItemText(IDC_EDIT_SOCKET_ACCEPT_BARCODE, strProductSn);

	lt::SSocketAccepting socketAccepting =
	{
		strRfid,
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_SOCKET_ACCEPT_FLAG)),
		strProductSn
	};

	InvokeRemote(&lt::CIcsRemote::CommandSocketAccepting,
				 std::forward<const lt::SSocketAccepting &>(socketAccepting),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendEjectAccept()
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_EJECT_ACCEPT_RFID, strRfid);

	lt::SSocketRfid socketRfid =
	{
		strRfid
	};

	InvokeRemote(&lt::CIcsRemote::CommandEjectAccepting,
				 std::forward<const lt::SSocketRfid &>(socketRfid),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendProductionStartEnd()
{
	lt::SProductionStartEnd productionStartEnd =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_PRODUCTION_START_END))
	};

	InvokeRemote(&lt::CIcsRemote::CommandProductionStartEnd,
				 std::forward<const lt::SProductionStartEnd &>(productionStartEnd),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::SendForceEject()
{
	lt::SForceEject forceEject =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_FORCE_EJECT)) 
	};

	InvokeRemote(&lt::CIcsRemote::CommandForceEject,
				 std::forward<const lt::SForceEject &>(forceEject),
				 std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::OnEquipmentIdEvent(EquipmentIdEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Equipment Id (equipment id = %s)"),
				reqArgs.equipmentId);

	SetDlgItemText(IDC_EDIT_EQUIPMENT_ID, reqArgs.equipmentId);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnEquipmentStatusEvent(EquipmentStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Equipment Status (status = %d, alarm = %d, info = %s)"),
				reqArgs.status,
				reqArgs.alarmCode,
				reqArgs.info);

	SetDlgItemInt(IDC_EDIT_EQUIPMENT_STATUS, static_cast<UINT>(reqArgs.status));
	SetDlgItemInt(IDC_EDIT_EQUIPMENT_STATUS_ERROR, static_cast<UINT>(reqArgs.alarmCode));
	SetDlgItemText(IDC_EDIT_EQUIPMENT_STATUS_REASON, reqArgs.info);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnOperationModeEvent(OperationModeEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Operation Mode (mode = %d)"),
				reqArgs.mode);

	SetDlgItemInt(IDC_EDIT_OPERATION_MODE, static_cast<UINT>(reqArgs.mode));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnSocketRequestEvent(SocketRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Socket Request (rfid = %s)"),
				reqArgs.rfid);

	SetDlgItemText(IDC_EDIT_SOCKET_REQUEST_RFID, reqArgs.rfid);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnPortClearEvent(PortClearEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Clear (para = %d)"),
				reqArgs.port);

	SetDlgItemInt(IDC_EDIT_PORT_CLEAR, static_cast<UINT>(reqArgs.port));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnPortStatusEvent(PortStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Status (port = %d, status = %d, rfid = %s, productSn = %s)"),
				reqArgs.port,
				reqArgs.status,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	SetDlgItemInt(IDC_EDIT_PORT_STATUS_1 + reqArgs.port,
				  static_cast<UINT>(reqArgs.status));
	SetDlgItemText(IDC_EDIT_PORT_RFID_1 + reqArgs.port,
				   reqArgs.socketInfo.rfid);
	SetDlgItemText(IDC_EDIT_PORT_BARCODE_1 + reqArgs.port,
				   reqArgs.socketInfo.productSn);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnPortStatusRequestEvent(PortStatusRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_PORT_RFID_1 + reqArgs.port, strRfid);
	GetDlgItemText(IDC_EDIT_PORT_BARCODE_1 + reqArgs.port, strProductSn);

	reqArgs.status = static_cast<decltype(reqArgs.status)>(
		GetDlgItemInt(IDC_EDIT_PORT_STATUS_1 + reqArgs.port));
	_tcscpy_s(reqArgs.socketInfo.rfid,
			  _countof(reqArgs.socketInfo.rfid),
			  strRfid);
	_tcscpy_s(reqArgs.socketInfo.productSn,
			  _countof(reqArgs.socketInfo.productSn),
			  strProductSn);

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Status Request (port = %d, status = %d, rfid = %s, productSn = %s)"),
				reqArgs.port,
				reqArgs.status,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnConveyorStatusEvent(ConveyorStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Conveyor Status (conveyor = %d, status1 = %d, status2 = %d, rfid = %s, productSn = %s)"),
				reqArgs.conveyor,
				reqArgs.status1,
				reqArgs.status2,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	SetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS1_1 + reqArgs.conveyor,
				  static_cast<UINT>(reqArgs.status1));
	SetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS2_1 + reqArgs.conveyor,
				  static_cast<UINT>(reqArgs.status2));
	SetDlgItemText(IDC_EDIT_CONVEYOR_RFID_1 + reqArgs.conveyor,
				   reqArgs.socketInfo.rfid);
	SetDlgItemText(IDC_EDIT_CONVEYOR_BARCODE_1 + reqArgs.conveyor,
				   reqArgs.socketInfo.productSn);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnConveyorStatusRequestEvent(ConveyorStatusRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_CONVEYOR_RFID_1 + reqArgs.conveyor, strRfid);
	GetDlgItemText(IDC_EDIT_CONVEYOR_BARCODE_1 + reqArgs.conveyor, strProductSn);

	reqArgs.status1 = static_cast<decltype(reqArgs.status1)>(
		GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS1_1 + reqArgs.conveyor));
	reqArgs.status2 = static_cast<decltype(reqArgs.status2)>(
		GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS2_1 + reqArgs.conveyor));
	_tcscpy_s(reqArgs.socketInfo.rfid,
			  _countof(reqArgs.socketInfo.rfid),
			  strRfid);
	_tcscpy_s(reqArgs.socketInfo.productSn,
			  _countof(reqArgs.socketInfo.productSn),
			  strProductSn);

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Conveyor Status Request (conveyor = %d, status1 = %d, status2 = %d, rfid = %s, productSn = %s)"),
				reqArgs.conveyor,
				reqArgs.status1,
				reqArgs.status2,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnLoadingEvent(LoadingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Loading (rfid = %s, productSn = %s)"),
				reqArgs.rfid,
				reqArgs.productSn);

	ClearData(reqArgs.rfid);

	SetDlgItemText(IDC_EDIT_RFID_REGISTERED_RFID, reqArgs.rfid);
	SetDlgItemText(IDC_EDIT_RFID_REGISTERED_BARCODE, reqArgs.productSn);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnTestResultRequestEvent(TestResultRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	CString strRfid;

	GetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_RFID, strRfid);

	if(strRfid.Compare(reqArgs.rfid) == 0)
	{
		reqArgs.result = static_cast<decltype(reqArgs.result)>(
			GetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_ERROR));
		reqArgs.para = static_cast<decltype(reqArgs.para)>(
			GetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_PARA));
		_tcscpy_s(reqArgs.equipmentName, _countof(reqArgs.equipmentName), _T("Equipment"));
		args.SetResult(true);
	}
	else
	{
		reqArgs.result = 100;
	}

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Test Result Request (rfid = %s, result = %d, para = %d, equip = %s)"),
				reqArgs.rfid,
				reqArgs.result,
				reqArgs.para,
				reqArgs.equipmentName);

	eventArgs.Cancel();
}

void CIcsServerDlg::OnUnloadingEvent(UnloadingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Unloading (rfid = %s)"),
				reqArgs.rfid);

	ClearData(reqArgs.rfid);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnTestResultEvent(TestResultEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	SetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_RFID, reqArgs.rfid);
	SetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_ERROR, reqArgs.result);
	SetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_PARA, reqArgs.para);

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Test Result (rfid = %s, result = %d, para = %d)"),
				reqArgs.rfid,
				reqArgs.result,
				reqArgs.para);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsServerDlg::OnBnClickedButtonActive()
{
	lt::SOperationActiveStatus language =
	{
		static_cast<lt::uint32>(1)
	};

	InvokeRemote(&lt::CIcsRemote::CommandOperationActiveStatus,
		std::forward<const lt::SOperationActiveStatus &>(language),
		std::forward<lt::uint64>(1000));
}

void CIcsServerDlg::OnBnClickedButtonInactive()
{
	lt::SOperationActiveStatus language =
	{
		static_cast<lt::uint32>(0)
	};

	InvokeRemote(&lt::CIcsRemote::CommandOperationActiveStatus,
		std::forward<const lt::SOperationActiveStatus &>(language),
		std::forward<lt::uint64>(1000));
}
