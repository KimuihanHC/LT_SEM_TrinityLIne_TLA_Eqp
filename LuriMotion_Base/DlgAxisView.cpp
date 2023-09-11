// DlgAxisView.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgAxisView.h"
#include "afxdialogex.h"

namespace { const UINT nIDTimerUpdateStatus = 1; }

// CDlgAxisView 대화 상자

IMPLEMENT_DYNAMIC(CDlgAxisView, CDialogEx)

CDlgAxisView::CDlgAxisView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_AXIS_VIEW, pParent)
{
	m_strPosBk = _T("-1.0");
	m_bAxisBk[eOrigin] = -1;
	m_bAxisBk[eServo] = -1;
	m_bAxisBk[eAlarm] = -1;
}

CDlgAxisView::~CDlgAxisView()
{
}

void CDlgAxisView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_AX_VIEW_1, m_Stt[eSttName]);
	DDX_Control(pDX, IDC_STC_AX_VIEW_2, m_Stt[eSttOrigin]);
	DDX_Control(pDX, IDC_STC_AX_VIEW_3, m_Stt[eSttPos]);
	DDX_Control(pDX, IDC_STC_AX_VIEW_4, m_Stt[eSttServo]);
	DDX_Control(pDX, IDC_STC_AX_VIEW_5, m_Stt[eSttAlarm]);
}


BEGIN_MESSAGE_MAP(CDlgAxisView, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgLanguage 메시지 처리기
BOOL CDlgAxisView::PreTranslateMessage(MSG* pMsg)
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

int CDlgAxisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgAxisView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(112, 128, 144));		// Slate Gray
	pDC->FillSolidRect(rect, RGB(52, 102, 127));		// Slate Blue
	return TRUE;
}

BOOL CDlgAxisView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_AxView();

	SetTimer(nIDTimerUpdateStatus, 200, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgAxisView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		
	}
}

void CDlgAxisView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case nIDTimerUpdateStatus:
		UpdateCurPos();
		UpdateOrigin();
		UpdateServo();
		UpdateAlarm();
		UpdateSensor();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAxisView::InitComponent_AxView()
{
	CString str;
	// Static
	for (int i = 0; i < eStt_Max; i++)
	{
		if (i == eSttName || i == eSttPos)
		{
			m_Stt[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
			m_Stt[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			m_Stt[i].SetFont_Gdip(L"Arial", 10);
		}
		else
		{
			m_Stt[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
			m_Stt[i].SetFont_Gdip(L"Arial", 10);
		}

		if(i== eSttName)	m_Stt[i].SetTextAlignment(Gdiplus::StringAlignmentNear);
			
		if(i == eSttName)			str.Format(_T("AXIS_Z"));
		else if (i == eSttPos)		str.Format(_T("1.2345"));
		else						str.Format(_T(""));

		m_Stt[i].SetText(str);
	}
}
void CDlgAxisView::SetText(int offset, CString text)
{
	switch (offset) {
	case 1:
		m_Stt[eSttName].SetWindowText(text);
		break;
	}
}

void CDlgAxisView::SetData(AXIS_DATA *pAX)
{
	if (pAX == NULL) return;

	m_strPos.Format(L"%.3f", pAX->dPos / 1000);
	m_bAxis[eOrigin] = pAX->nData[0];
	m_bAxis[eServo] = pAX->nData[1];
	m_bAxis[eAlarm] = pAX->nData[2];
	//m_bAxisMinusLimit = pAX->nData[3];
	//m_bAxisHome = pAX->nData[4];
	//m_bAxisPlusLimit = pAX->nData[5];
}

void CDlgAxisView::PrepareForDestroy()
{
	KillTimer(nIDTimerUpdateStatus);
}

void CDlgAxisView::UpdateCurPos()
{
	if (m_strPosBk != m_strPos) {
		m_strPosBk = m_strPos;
		m_Stt[eSttPos].SetWindowText(m_strPosBk);
	}
}

void CDlgAxisView::UpdateOrigin()
{
	if (m_bAxisBk[eOrigin] != m_bAxis[eOrigin]) {
		m_bAxisBk[eOrigin] = m_bAxis[eOrigin];

		if (m_bAxisBk[eOrigin])
			m_Stt[eSttOrigin].SetBackColor(Gdiplus::Color::LightGreen);
		else
			m_Stt[eSttOrigin].SetBackColor(Gdiplus::Color::LightGray);
	}
}

void CDlgAxisView::UpdateServo()
{
	if (m_bAxisBk[eServo] != m_bAxis[eServo]) {
		m_bAxisBk[eServo] = m_bAxis[eServo];

		if (m_bAxisBk[eServo])
			m_Stt[eSttServo].SetBackColor(Gdiplus::Color::LightGreen);
		else
			m_Stt[eSttServo].SetBackColor(Gdiplus::Color::LightGray);
	}
}

void CDlgAxisView::UpdateAlarm()
{
	if (m_bAxisBk[eAlarm] != m_bAxis[eAlarm]) {
		m_bAxisBk[eAlarm] = m_bAxis[eAlarm];

		if (m_bAxisBk[eAlarm])
			m_Stt[eSttAlarm].SetBackColor(Gdiplus::Color::Red);
		else
			m_Stt[eSttAlarm].SetBackColor(Gdiplus::Color::LightGray);
	}
}

void CDlgAxisView::UpdateSensor()
{

	//if (m_bAxisMinusLimitBk != m_bAxisMinusLimit) {
	//	m_bAxisMinusLimitBk = m_bAxisMinusLimit;

	//	if (m_bAxisMinusLimitBk)
	//		m_ctrlAxisMinusLimit.SetBackColorInterior(PJC_COR_RED);
	//	else
	//		m_ctrlAxisMinusLimit.SetBackColorInterior(COLOR_OFF);
	//}

	//if (m_bAxisHomeBk != m_bAxisHome) {
	//	m_bAxisHomeBk = m_bAxisHome;

	//	if (m_bAxisHomeBk)
	//		m_ctrlAxisHome.SetBackColorInterior(PJC_COR_RED);
	//	else
	//		m_ctrlAxisHome.SetBackColorInterior(COLOR_OFF);
	//}

	//if (m_bAxisPlusLimitBk != m_bAxisPlusLimit) {
	//	m_bAxisPlusLimitBk = m_bAxisPlusLimit;

	//	if (m_bAxisPlusLimitBk)
	//		m_ctrlAxisPlusLimit.SetBackColorInterior(PJC_COR_RED);
	//	else
	//		m_ctrlAxisPlusLimit.SetBackColorInterior(COLOR_OFF);
	//}
}




