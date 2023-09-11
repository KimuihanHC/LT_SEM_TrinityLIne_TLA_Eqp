#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSFRWideVMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgSFRWideVMain 대화 상자
IMPLEMENT_DYNAMIC(CDlgSFRWideVMain, CDialogEx)

CDlgSFRWideVMain::CDlgSFRWideVMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SFR_WIDE_V_MAIN, pParent)
{

}

CDlgSFRWideVMain::~CDlgSFRWideVMain()
{
}

void CDlgSFRWideVMain::ShowOperation(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
	case OperationMode::Manual:
	case OperationMode::SemiAuto:
	case OperationMode::StartUpCheck:
		GetDlgItem(IDC_RADIO_SFR_WIDE_V_82_5_ANGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SFR_WIDE_V_180_ANGLE)->ShowWindow(SW_SHOW);
		break;
	default:
		GetDlgItem(IDC_RADIO_SFR_WIDE_V_46_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SFR_WIDE_V_82_5_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SFR_WIDE_V_180_ANGLE)->ShowWindow(SW_HIDE);
		break;
	}
}

ST_OptSemiAuto CDlgSFRWideVMain::GetOption()
{
	ST_OptSemiAuto opt;

	UpdateData(TRUE);

	opt.Angle = (enAngleType)m_iAngleType;

	return opt;
}

BOOL CDlgSFRWideVMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();
	m_iAngleType = (int)(enAngleType::Angle_82_5);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSFRWideVMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SFR_WIDE_V_46_ANGLE, m_iAngleType);
}

BOOL CDlgSFRWideVMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgSFRWideVMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgSFRWideVMain, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgSFRWideVMain::InitComponent()
{
}

void CDlgSFRWideVMain::ChangeUI_Language()
{
}
