#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgDistortionMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgDistortionMain 대화 상자
IMPLEMENT_DYNAMIC(CDlgDistortionMain, CDialogEx)

CDlgDistortionMain::CDlgDistortionMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DISTORTION_MAIN, pParent)
{

}

CDlgDistortionMain::~CDlgDistortionMain()
{
}

void CDlgDistortionMain::ShowOperation(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
	case OperationMode::Manual:
	case OperationMode::SemiAuto:
	case OperationMode::StartUpCheck:
		GetDlgItem(IDC_RADIO_46_ANGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_82_5_ANGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_180_ANGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SOCKET_H)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SOCKET_V)->ShowWindow(SW_SHOW);

		break;		
	default:
		GetDlgItem(IDC_RADIO_46_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_82_5_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_180_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SOCKET_H)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SOCKET_V)->ShowWindow(SW_HIDE);
		break;
	}
}

ST_OptSemiAuto CDlgDistortionMain::GetOption()
{
	ST_OptSemiAuto opt;

	UpdateData(TRUE);

	opt.Angle = (enAngleType)m_iAngleType;
	opt.Socket = (enSocketType)m_iSocketType;

	return opt;
}

BOOL CDlgDistortionMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgDistortionMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_46_ANGLE, m_iAngleType);
	DDX_Radio(pDX, IDC_RADIO_SOCKET_H, m_iSocketType);
}

BOOL CDlgDistortionMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgDistortionMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgDistortionMain, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgDistortionMain::InitComponent()
{
}

void CDlgDistortionMain::ChangeUI_Language()
{
}

