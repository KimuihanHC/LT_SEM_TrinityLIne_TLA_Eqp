#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSFRNarrowMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgSFRNarrowMain 대화 상자
IMPLEMENT_DYNAMIC(CDlgSFRNarrowMain, CDialogEx)

CDlgSFRNarrowMain::CDlgSFRNarrowMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SFR_NARROW_MAIN, pParent)
{

}

CDlgSFRNarrowMain::~CDlgSFRNarrowMain()
{
}

void CDlgSFRNarrowMain::ShowOperation(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
		break;
	case OperationMode::Manual:
		break;
	case OperationMode::SemiAuto:
		break;
	case OperationMode::StartUpCheck:
		break;
	case OperationMode::DryRun:
		break;
	case OperationMode::Loto:
		break;
	default:
		break;
	}
}

ST_OptSemiAuto CDlgSFRNarrowMain::GetOption()
{
	ST_OptSemiAuto opt;

	return opt;
}

BOOL CDlgSFRNarrowMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSFRNarrowMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CDlgSFRNarrowMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgSFRNarrowMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgSFRNarrowMain, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgSFRNarrowMain::InitComponent()
{
}

void CDlgSFRNarrowMain::ChangeUI_Language()
{
}
