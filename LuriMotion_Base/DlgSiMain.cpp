// DlgAxisHomeProgress.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgSiMain.h"
#include "DlgSiView.h"
#include "afxdialogex.h"

namespace { const UINT TIMER_UPDATE_SOCKET_STATUS = 10; }

// CDlgSiMain 대화 상자

IMPLEMENT_DYNAMIC(CDlgSiMain, CDialogEx)

CDlgSiMain::CDlgSiMain( CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SI_MAIN, pParent)
{
}

CDlgSiMain::CDlgSiMain(CWnd* pParent , CRect rt)
	: CDialogEx(IDD_DLG_SI_MAIN, pParent)
{
	m_rt = rt;
}

CDlgSiMain::~CDlgSiMain()
{
}

void CDlgSiMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgSiMain, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgSiMain::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSiMain::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDlgSiMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgSiMain::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect Rect, ParentRect;
	GetClientRect(&Rect);
	GetParent()->GetClientRect(&ParentRect);

	CPoint ptLeftTop = CPoint(0, 0);
	ClientToScreen(&ptLeftTop);
	GetParent()->ScreenToClient(&ptLeftTop);

	CDC MemDC;
	CBitmap Bmp;

	MemDC.CreateCompatibleDC(NULL);
	Bmp.CreateBitmap(ParentRect.Width(), ParentRect.Height(), MemDC.GetDeviceCaps(PLANES), MemDC.GetDeviceCaps(BITSPIXEL), NULL);
	CBitmap * pOldBmp = MemDC.SelectObject(&Bmp);

	GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)MemDC.m_hDC);

	CLuriMotion_BaseDlg *pParrentDlg = (CLuriMotion_BaseDlg*)AfxGetApp()->GetMainWnd();

	if (pParrentDlg->IsImageHandleValid(eBITMAP_SOCKET_INFO) ==TRUE) {
		pParrentDlg->EraseBkGnd_Image(this, pDC, eBITMAP_SOCKET_INFO);

		MemDC.SelectObject(pOldBmp);

		return TRUE;
	}
	else {
		return CWnd::OnEraseBkgnd(pDC);
	}
}

// CDlgSiMain 메시지 처리기

int CDlgSiMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}
BOOL CDlgSiMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent_SiMain();
	
	for (int i = 0; i < eSPL_MAX; i++) 
	{
		m_pSiView[i] = new CDlgSiView();
		m_pSiView[i]->Create(IDD_DLG_SI_VIEW, this);		
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

	x1 = 10;
	x2 = 10;

	// Pos
	for (int i = 0; i < eSPL_MAX; i++) {
		y1 = tray_y1 + (tray_cy + 4)*i;
		SetPos_SiView(i, x1, y1);

	}
	// Name
	for (int i = 0; i < eSPL_MAX; i++) {

		str = m_szSocketPosNameEng[i];
		SetName_SiView(i, str);
	}

	// 
	MoveWindow(&m_rt);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.

}

void CDlgSiMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		SetTimer(TIMER_UPDATE_SOCKET_STATUS, 200, NULL);

	}
	else
	{
		KillTimer(TIMER_UPDATE_SOCKET_STATUS);
	}
}

void CDlgSiMain::InitComponent_SiMain()
{
}

void CDlgSiMain::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case	TIMER_UPDATE_SOCKET_STATUS:
	{
		UpdateStatus();
	}
	break;
	}
}

void CDlgSiMain::UpdateStatus()
{
	SOCKET_INFO tsi;

	for (int i = 0; i < eSPL_MAX; i++) 
	{
		SEQUENCE->GetSocketInfo((ESOCKET_POS_LIST)i, tsi);

		// SOCKET EXIST
		m_SiData.bExist = tsi.bExist;

		// SERIAL NO
		m_SiData.str[0] = tsi.strSerialNo;

		// RF ID
		m_SiData.str[1] = tsi.strRFID;

		// Q CODE ID
		m_SiData.str[2] = tsi.stModule.strModuleID;

		m_pSiView[i]->SetData(&m_SiData);
	}

}

void CDlgSiMain::SetPos_SiView(UINT nID, int x, int y, BOOL bShow)
{
	UINT	flag;
	if (TRUE == bShow) {
		flag = SWP_NOSIZE | SWP_SHOWWINDOW;
	}
	else {
		flag = SWP_NOSIZE | SWP_HIDEWINDOW;
	}
	m_pSiView[nID]->SetWindowPos(&wndTop, x, y, 0, 0, flag);
}

void CDlgSiMain::SetName_SiView(UINT nID, CString str)
{
	m_pSiView[nID]->SetText(1, str);
}

void CDlgSiMain::SetData_SiView(UINT nID, SOCKET_DATA *pSD)
{
	m_pSiView[nID]->SetData(pSD);
}

void CDlgSiMain::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(FALSE);

	CDialogEx::OnOK();
}

void CDlgSiMain::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
