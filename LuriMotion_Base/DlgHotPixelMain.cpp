#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgHotPixelMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgHotPixelMain 대화 상자
IMPLEMENT_DYNAMIC(CDlgHotPixelMain, CDialogEx)

CDlgHotPixelMain::CDlgHotPixelMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_HOTPIXEL_MAIN, pParent)
{

}

CDlgHotPixelMain::~CDlgHotPixelMain()
{
}

void CDlgHotPixelMain::ShowOperation(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
	case OperationMode::Manual:
	case OperationMode::SemiAuto:
	case OperationMode::StartUpCheck:
		GetDlgItem(IDC_STC_HOTPIXEL_LEFT_CL200A_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_HOTPIXEL_LEFT_CL200A_DATA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_HOTPIXEL_RIGHT_CL200A_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_HOTPIXEL_RIGHT_CL200A_DATA)->ShowWindow(SW_SHOW);
		break;
	default:
		GetDlgItem(IDC_STC_HOTPIXEL_LEFT_CL200A_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_HOTPIXEL_LEFT_CL200A_DATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_HOTPIXEL_RIGHT_CL200A_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_HOTPIXEL_RIGHT_CL200A_DATA)->ShowWindow(SW_HIDE);
		break;
	}
}

ST_OptSemiAuto CDlgHotPixelMain::GetOption()
{
	ST_OptSemiAuto opt;

	return opt;
}

BOOL CDlgHotPixelMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgHotPixelMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_HOTPIXEL_LEFT_CL200A_NAME, m_stName[0]);
	DDX_Control(pDX, IDC_STC_HOTPIXEL_RIGHT_CL200A_NAME, m_stName[1]);

	DDX_Control(pDX, IDC_STC_HOTPIXEL_LEFT_CL200A_DATA, m_stData[0]);
	DDX_Control(pDX, IDC_STC_HOTPIXEL_RIGHT_CL200A_DATA, m_stData[1]);
}

BOOL CDlgHotPixelMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgHotPixelMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgHotPixelMain, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgHotPixelMain::InitComponent()
{
	for (auto i = 0; i < 2; i++)
	{
		m_stName[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stName[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stName[i].SetFont_Gdip(L"Arial", 12.0F);

		m_stData[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stData[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stData[i].SetFont_Gdip(L"Arial", 12.0F);
	}

	m_stName[0].SetText(_T("(L) CL200A"));
	m_stName[1].SetText(_T("(R) CL200A"));
	Reset();
}

void CDlgHotPixelMain::ChangeUI_Language()
{
}

void CDlgHotPixelMain::Reset()
{
	for(auto i = 0; i<2; i++)
		m_stData[i].SetText(_T("0"));
}

void CDlgHotPixelMain::SetCL200A(__in InspectionPortType port, __in float fData, __in bool bReadOk)
{
	int nIdx = (int)port;

	CString sz;
	if (bReadOk)
		sz.Format(_T("%f"), fData);
	else
		sz = _T("READ FAIL");

	m_stData[nIdx].SetText(sz);
}

void CDlgHotPixelMain::ResetCL200A(__in InspectionPortType port)
{
	int nIdx = (int)port;
	CString sz;
	sz = _T("READY");
	m_stData[nIdx].SetText(sz);
}

