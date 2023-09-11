// DlgMessageBox.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgMessageBox.h"
#include "afxdialogex.h"


// CDlgMessageBox 대화 상자

IMPLEMENT_DYNAMIC(CDlgMessageBox, CDialogEx)

CDlgMessageBox::CDlgMessageBox( CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESSAGEBOX, pParent)
{
	m_strError = _T("");
	m_nMode = -1;
}

CDlgMessageBox::~CDlgMessageBox()
{
}

void CDlgMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TEST_1, m_stTitle);
	DDX_Control(pDX, IDC_BTN_1_ALARM, m_ctrlButton[eBtnOk]);
	DDX_Control(pDX, IDC_BTN_2_ALARM, m_ctrlButton[eBtnCancel]);
	DDX_Control(pDX, IDC_BTN_3_ALARM, m_ctrlButton[eBtnBuzOff]);
	DDX_Control(pDX, IDC_BTN_4_ALARM, m_ctrlButton[eBtnDoorLock]);
	
}


BEGIN_MESSAGE_MAP(CDlgMessageBox, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_1_ALARM, &CDlgMessageBox::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2_ALARM, &CDlgMessageBox::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3_ALARM, &CDlgMessageBox::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN_4_ALARM, &CDlgMessageBox::OnBnClickedBtn4)
END_MESSAGE_MAP()

void CDlgMessageBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		//Sleep(500);

		//COMMON->DoEvent(500);
		SetTimer(TIMER_END_DIALOG, 2000, NULL);
	}
	else
	{
		KillTimer(TIMER_END_DIALOG);
	}
}

BOOL CDlgMessageBox::PreTranslateMessage(MSG* pMsg)
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
int CDlgMessageBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgMessageBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	//pDC->FillSolidRect(rect, RGB(84, 169, 169));
	pDC->FillSolidRect(rect, RGB(70, 70, 70));		// Lime
	return TRUE;
}
BOOL CDlgMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//InitComponent_MessageBox();
	InitComponent_MessageBox();

	ChangeComponent_MessageBox();



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CDlgMain 메시지 처리기
void CDlgMessageBox::InitComponent_MessageBox()
{
	CString strTemp = _T("");

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	//m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Yellow);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);
}
void CDlgMessageBox::ChangeComponent_MessageBox()
{
	CString strTemp = _T("");


	if (m_nMode == (int)MODE_INFORMATION)
	{
		SetWindowText(_T("INFORMATION"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_SlateGray);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_OK;
	}
	else if (m_nMode == (int)MODE_QUESTION)
	{
		SetWindowText(_T("QUESTION"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_White);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_OK | BUTTON_CANCEL;
	}
	else if (m_nMode == (int)MODE_ERROR)
	{
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, TRUE);
		SetWindowText(_T("ERROR"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Orange);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_OK | BUTTON_BUZZER_OFF | BUTTON_DOOR_LOCK;
	}
	else if (m_nMode == (int)MODE_PASS)
	{
	//	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, TRUE);
		SetWindowText(_T("PASS"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Green);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_OK;
	}
	else if (m_nMode == (int)MODE_FAIL)
	{
		//AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, TRUE);
		SetWindowText(_T("FAIL"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Red);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_OK;
	}
	else
	{
		SetWindowText(_T("WARNING"));
		m_stTitle.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stTitle.SetText(m_strError);
		m_nButton = BUTTON_ALL;
	}

	for (int i = 0; i < eBtn_Max; i++)
	{
		if ((BUTTON_OK & (m_nButton >> i)) == BUTTON_OK)
			m_ctrlButton[i].ShowWindow(TRUE);
		else
			m_ctrlButton[i].ShowWindow(FALSE);
	}

// 	if (m_nMode == (int)MODE_PASS)
// 	{
// 		COMMON->DoEvent(500);
// 		EndDialog(IDOK);
// 	}
// 	if (m_nMode == (int)MODE_FAIL)
// 	{
// 		COMMON->DoEvent(500);
// 		EndDialog(IDOK);
// 	}

}



void CDlgMessageBox::SetMsg(CString str, int nMode)
{
	m_strError = str;
	m_nMode = nMode;

	//ChangeComponent_MessageBox();
}

void CDlgMessageBox::OnBnClickedBtn1()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	KillTimer(TIMER_END_DIALOG);
	EndDialog(IDOK);
}
void CDlgMessageBox::OnBnClickedBtn2()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	KillTimer(TIMER_END_DIALOG);
	EndDialog(IDCANCEL);

}
void CDlgMessageBox::OnBnClickedBtn3()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	KillTimer(TIMER_END_DIALOG);
}

void CDlgMessageBox::OnBnClickedBtn4()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	for (int i = 0; i < 5; i++)
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF + i * 2, TRUE);

	KillTimer(TIMER_END_DIALOG);
}

void CDlgMessageBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_END_DIALOG:
		if (m_nMode == (int)MODE_PASS)
		{
			Sleep(500);
			//COMMON->DoEvent(500);
			EndDialog(IDOK);
			KillTimer(TIMER_END_DIALOG);
		}
		else if (m_nMode == (int)MODE_FAIL)
		{
			Sleep(500);
			EndDialog(IDOK);
			KillTimer(TIMER_END_DIALOG);
		}
		else {
			KillTimer(TIMER_END_DIALOG);

		}
		break;
	};
}