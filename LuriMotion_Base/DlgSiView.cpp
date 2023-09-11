// DlgSInfoView.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSiView.h"
#include "afxdialogex.h"

namespace { const UINT nIDTimerUpdateStatus = 1; }

// CDlgSiView 대화 상자

IMPLEMENT_DYNAMIC(CDlgSiView, CDialogEx)

CDlgSiView::CDlgSiView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SI_VIEW, pParent)
{
	m_bExistBk = -1;
}

CDlgSiView::~CDlgSiView()
{
}

void CDlgSiView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_SI_VIEW_1, m_Stt[eSttName]);
	DDX_Control(pDX, IDC_STC_SI_VIEW_2, m_Stt[eSttSerialNo]);
	DDX_Control(pDX, IDC_STC_SI_VIEW_3, m_Stt[eSttRFID]);
	DDX_Control(pDX, IDC_STC_SI_VIEW_4, m_Stt[eSttCodeID]);
	DDX_Control(pDX, IDC_STC_SI_VIEW_5, m_Stt[eSttExist]);
}


BEGIN_MESSAGE_MAP(CDlgSiView, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgLanguage 메시지 처리기
BOOL CDlgSiView::PreTranslateMessage(MSG* pMsg)
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

int CDlgSiView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgSiView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(112, 128, 144));		// Slate Gray
	pDC->FillSolidRect(rect, RGB(52, 102, 127));		// Slate Blue
	return TRUE;
}

BOOL CDlgSiView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_SInfoView();

	SetTimer(nIDTimerUpdateStatus, 200, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSiView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		
	}
}

void CDlgSiView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case nIDTimerUpdateStatus:
		UpdateExist();
		UpdateSerialNo();
		UpdateRFID();
		UpdateCodeID();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgSiView::InitComponent_SInfoView()
{
	CString str;
	// Static
	for (int i = 0; i < eStt_Max; i++)
	{
		if (i != eSttExist )
		{
			m_Stt[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_Stt[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			m_Stt[i].SetFont_Gdip(L"Arial", 9);
		}
		else
		{
			m_Stt[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			m_Stt[i].SetFont_Gdip(L"Arial", 9);
		}

		if(i== eSttName)			m_Stt[i].SetTextAlignment(Gdiplus::StringAlignmentNear);
			
		if(i == eSttName)			str.Format(_T("INPUT"));
		else if (i == eSttSerialNo)	str.Format(_T(""));
		else if (i == eSttRFID)		str.Format(_T(""));
		else if (i == eSttCodeID)	str.Format(_T(""));
		else						str.Format(_T(""));

		m_Stt[i].SetText(str);
	}
}
void CDlgSiView::SetText(int offset, CString text)
{
	switch (offset) {
	case 1:
		m_Stt[eSttName].SetWindowText(text);
		break;
	}
}

void CDlgSiView::SetData(SOCKET_DATA *pSd)
{
	if (pSd == NULL) return;

	m_Str[eSerialNo] = pSd->str[0];
	m_Str[eRFID] = pSd->str[1];
	m_Str[eCodeID] = pSd->str[2];
	m_bExist = pSd->bExist;
}

void CDlgSiView::PrepareForDestroy()
{
	KillTimer(nIDTimerUpdateStatus);
}

void CDlgSiView::UpdateSerialNo()
{
	if (m_StrBk[eSerialNo] != m_Str[eSerialNo]) {
		m_StrBk[eSerialNo] = m_Str[eSerialNo];
		m_Stt[eSttSerialNo].SetWindowText(m_StrBk[eSerialNo]);
	}
}

void CDlgSiView::UpdateRFID()
{
	if (m_StrBk[eRFID] != m_Str[eRFID]) {
		m_StrBk[eRFID] = m_Str[eRFID];
		m_Stt[eSttRFID].SetWindowText(m_StrBk[eRFID]);
	}
}

void CDlgSiView::UpdateCodeID()
{
	if (m_StrBk[eCodeID] != m_Str[eCodeID]) {
		m_StrBk[eCodeID] = m_Str[eCodeID];
		m_Stt[eSttCodeID].SetWindowText(m_StrBk[eCodeID]);
	}
}

void CDlgSiView::UpdateExist()
{
	if (m_bExistBk != m_bExist) {
		m_bExistBk = m_bExist;

		if (m_bExistBk)
			m_Stt[eSttExist].SetBackColor(Gdiplus::Color::LawnGreen);
		else
			m_Stt[eSttExist].SetBackColor(Gdiplus::Color::LightGray);
	}
}
