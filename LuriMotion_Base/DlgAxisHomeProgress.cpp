// DlgAxisHomeProgress.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgAxisHomeProgress.h"
#include "afxdialogex.h"
#include "DlgAxisView.h"

namespace { const UINT TIMER_UPDATE_AXIS_STATUS = 10; }
namespace { const UINT TIMER_INITIAL_STATUS = 20; }

// CDlgAxisHomeProgress 대화 상자

IMPLEMENT_DYNAMIC(CDlgAxisHomeProgress, CDialogEx)

CDlgAxisHomeProgress::CDlgAxisHomeProgress( CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MOTOR_HOME_RATE, pParent)
{
}

CDlgAxisHomeProgress::~CDlgAxisHomeProgress()
{
}

void CDlgAxisHomeProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_INITIAL_1, m_stTitle);

	DDX_Control(pDX, IDC_BTN_INITIAL_1, m_Btn[eBtnInitial]);
	DDX_Control(pDX, IDC_BTN_INITIAL_2, m_Btn[eBtnStop]);
	DDX_Control(pDX, IDC_BTN_INITIAL_3, m_Btn[eBtnExit]);
}

BEGIN_MESSAGE_MAP(CDlgAxisHomeProgress, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INITIAL_1, &CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginall)
	ON_BN_CLICKED(IDC_BTN_INITIAL_2, &CDlgAxisHomeProgress::OnBnClickedButtonMotorStop)
	ON_BN_CLICKED(IDC_BTN_INITIAL_3, &CDlgAxisHomeProgress::OnBnClickedButtonMotorExit)
END_MESSAGE_MAP()

BOOL CDlgAxisHomeProgress::PreTranslateMessage(MSG* pMsg)
{
	//ALT + F4 차단
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	//ESC 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//Enter 차단
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
// CDlgTitle 메시지 처리기

BOOL CDlgAxisHomeProgress::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(112, 128, 144));		// Slate Gray
	//return TRUE;

	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	if (pParrentDlg->IsImageHandleValid(eBITMAP_INIT) ==TRUE) {
		pParrentDlg->EraseBkGnd_Image(this, pDC, eBITMAP_INIT);
		return TRUE;
	}
	else 
	{
		return CWnd::OnEraseBkgnd(pDC);
	}
}

// CDlgAxisHomeProgress 메시지 처리기

int CDlgAxisHomeProgress::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}
BOOL CDlgAxisHomeProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_AxisMoveHome(MOTOR_MAX_COUNT);

	for (int i = 0; i < MOTOR_MAX_COUNT; i++) 
	{
		m_pAxisView[i] = new CDlgAxisView();
		m_pAxisView[i]->Create(IDD_DLG_AXIS_VIEW, this);
	}

	CString str;
	RECT	rect;
	int		y1, y2;
	int		x1, x2;
	int		tray_y1, tray_y2;

	int		tray_cx, tray_cy;
	tray_cx = 380;
	tray_cy = 25;

	GetClientRect(&rect);

	tray_y1 = 40;
	tray_y2 = 365;

	x1 = 50;
	x2 = 50;

	// Pos
	for (int i = 0; i < MOTOR_MAX_COUNT; i++) 
	{
//		if (i < AXIS_PICKER_ULD_NG_Z)
//		{
			y1 = tray_y1 + (tray_cy + 4)*i;
			SetPos_AxisView(i, x1, y1);
//		}
// 		else
// 		{
// 			y2 = tray_y2 + (tray_cy + 4)*(i - AXIS_PICKER_ULD_NG_Z);
// 			SetPos_AxisView(i, x2, y2);
// 		}
	}
	// Name
	for (int i = 0; i < MOTOR_MAX_COUNT; i++) 
	{

		//str = m_szTeachJogBtnEng[2 + i];
		str = m_szMainMotorName_Eng[i];
		SetName_AxisView(i, str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.

}

void CDlgAxisHomeProgress::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		SetTimer(TIMER_UPDATE_AXIS_STATUS, 200, NULL);
	}
	else
	{
		KillTimer(TIMER_UPDATE_AXIS_STATUS);
	}
}

void CDlgAxisHomeProgress::InitComponent_AxisMoveHome(BYTE nAxis)
{
	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stTitle.SetFont_Gdip(L"Arial", 10.0F);
	m_stTitle.SetText(_T("STOPPED.."));
}

void CDlgAxisHomeProgress::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case	TIMER_UPDATE_AXIS_STATUS:
	{
// 		if (FAS_MO->m_bConnected == FALSE)
// 		{
// 			break;
// 		}
		// AXIS


		if (SEQUENCE->nhomeprogress == 1)
		{
			for (int i = 0; i < MOTOR_MAX_COUNT; i++)
			{
				m_pAxisView[i]->PrepareForDestroy();
				m_pAxisView[i]->DestroyWindow();
			}
			KillTimer(TIMER_UPDATE_AXIS_STATUS);
			KillTimer(TIMER_INITIAL_STATUS);
			OnOK();
		}
		UpdateStatus();
	}
	break;

	case	TIMER_INITIAL_STATUS:
	{
		if (SEQUENCE->GetRunMode() == eError)
		{
			m_stTitle.SetText(_T("STOPPED ON ERROR.."));
			m_Btn[eBtnInitial].EnableWindow(TRUE);
			m_Btn[eBtnExit].EnableWindow(TRUE);
			KillTimer(nIDEvent);
		}
		else if (SEQUENCE->GetRunMode() == eStop)
		{
			if (SEQUENCE->GetInitStatus() == eInit_Done)
			{
				m_stTitle.SetText(_T("SUCCESS.."));
				m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Green);
				m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Green);
				m_Btn[eBtnInitial].EnableWindow(FALSE);

			}
			else
			{
				m_stTitle.SetText(_T(" STOP BY USER.."));
				m_Btn[eBtnInitial].EnableWindow(TRUE);
			}
			m_Btn[eBtnStop].EnableWindow(FALSE);
			m_Btn[eBtnExit].EnableWindow(TRUE);
			KillTimer(nIDEvent);
			break;
		}
		else
		{
			m_stTitle.SetText(_T("IN PROGRESS..."));
		}

		if (m_nFlickCount++ < 5)
		{
			if(m_stTitle.GetBackColor() != Gdiplus::Color::Yellow)
				m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Yellow);
		}
		else if (m_nFlickCount < 10)
		{
			if (m_stTitle.GetBackColor() != Gdiplus::Color::White)
				m_stTitle.SetColorStyle(CVGStatic::ColorStyle_White);
		}
		else
		{
			m_nFlickCount = 0;
		}
	}
	break;
	}

}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorOriginall()
{
	CString strTemp = _T("");
	EQ_BASIC_PARAM	stEq = *DATAMANAGER->GetCurrentEqBasicData();

#ifndef DevMode_Simulator
	if (IO_I_EMO() == FALSE)
	{
		return;
	}

	if (IO_I_PowerOff() == FALSE)
	{
		return;
	}
	if (IO_I_AREA() == FALSE)
	{
		return;
	}
	if (IO_I_MainAirPress() == FALSE)
	{
		return;
	}
// 	if (FAS_MO->m_bConnected == FALSE)
// 	{
// 		if (stEq.nLanguage == 0)
// 			COMMON->ShowMsgDlg_Domodal(_T("모터를 연결해 주신후 초기화를 진행해 주십시오."), MODE_INFORMATION);
// 		else
// 			COMMON->ShowMsgDlg_Domodal(_T("Please connect the motor and start initialization.."), MODE_INFORMATION);
// 		return;
// 	}
#endif // !	
	if (stEq.nLanguage == 0)
		strTemp.Format(_T("초기화를 진행하시겠습니까?"));
	else
		strTemp.Format(_T("Will you start initialization?"));

	if (COMMON->ShowMsgDlg_Domodal(strTemp, MODE_QUESTION) == FALSE)
	{
		if (stEq.nLanguage == 0)
			strTemp.Format(_T("장비초기화 취소합니다."));
		else
			strTemp.Format(_T("Canceled the equipment initial."));
		COMMON->Write_Main_Log(strTemp);
	}
	else
	{
		SEQUENCE->SetInitStatus(eInit_None);
		SEQUENCE->SetStpInitial(0);
		SEQUENCE->Start_Initial();

		SetTimer(TIMER_INITIAL_STATUS, 250, NULL);

		m_Btn[eBtnInitial].EnableWindow(FALSE);
		m_Btn[eBtnExit].EnableWindow(FALSE);
	}
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorStop()
{
	SEQUENCE->Stop_Run(eStop);
}

void CDlgAxisHomeProgress::OnBnClickedButtonMotorExit()
{
	for (int i = 0; i < MOTOR_MAX_COUNT; i++) 
	{
		m_pAxisView[i]->PrepareForDestroy();
		m_pAxisView[i]->DestroyWindow();
	}
	KillTimer(TIMER_UPDATE_AXIS_STATUS);
	KillTimer(TIMER_INITIAL_STATUS);
	OnOK();
}


void CDlgAxisHomeProgress::UpdateStatus()
{
	for (int i = 0; i < MOTOR_MAX_COUNT; i++) 
	{
		// POS
		m_AxisData.dPos = AJIN_MO->GetActPos(i);
		// Origin
		m_AxisData.nData[0] = AJIN_MO->IsOrigin(i);
		// Servo
		m_AxisData.nData[1] = AJIN_MO->IsServoOn(i);
		// Alarm
		m_AxisData.nData[2] = AJIN_MO->m_bReadError[i];
		// -Limit Sensor
		m_AxisData.nData[3] = AJIN_MO->m_bReadLimitM[i];
		// Home Sensor
		m_AxisData.nData[4] = AJIN_MO->m_bReadHome[i];
		// +Limit Sensor
		m_AxisData.nData[5] = AJIN_MO->m_bReadLimitP[i];
		//  Refresh
		m_pAxisView[i]->SetData(&m_AxisData);
	}
}

void CDlgAxisHomeProgress::SetPos_AxisView(UINT nID, int x, int y, BOOL bShow)
{
	UINT	flag;
	if (TRUE == bShow) {
		flag = SWP_NOSIZE | SWP_SHOWWINDOW;
	}
	else {
		flag = SWP_NOSIZE | SWP_HIDEWINDOW;
	}
	m_pAxisView[nID]->SetWindowPos(&wndTop, x, y, 0, 0, flag);
}

void CDlgAxisHomeProgress::SetName_AxisView(UINT nID, CString str)
{
	m_pAxisView[nID]->SetText(1, str);
}

void CDlgAxisHomeProgress::SetData_AxisView(UINT nID, AXIS_DATA *pAX)
{
	m_pAxisView[nID]->SetData(pAX);
}
