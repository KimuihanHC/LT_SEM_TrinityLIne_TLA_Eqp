// DlgSocketStatus.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSocket.h"
#include "afxdialogex.h"

// CDlgSocket 대화 상자

IMPLEMENT_DYNAMIC(CDlgSocket, CDialogEx)

CDlgSocket::CDlgSocket(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SOCKET, pParent)
{

}

CDlgSocket::~CDlgSocket()
{

}

void CDlgSocket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_SOCKET_STATUS, m_stcStatus);

	DDX_Control(pDX, IDC_STC_SOCKET_OK, m_stcResultOk);
	DDX_Control(pDX, IDC_STC_SOCKET_NG, m_stcResultNg);

	DDX_Control(pDX, IDC_STC_SOCKET_RFID, m_stcRfid);
	DDX_Control(pDX, IDC_STC_SOCKET_BARCODE, m_stcBarcode);
}

BEGIN_MESSAGE_MAP(CDlgSocket, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CDlgSocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgSocket::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

void CDlgSocket::InitComponent()
{
	m_stcStatus.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stcStatus.SetColorStyle(CVGStatic::ColorStyle_SlateGray);
	m_stcStatus.SetFont_Gdip(L"Arial", 9.0F);
	m_stcStatus.SetText(_T("ready"));

	m_stcResultOk.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stcResultOk.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stcResultOk.SetFont_Gdip(L"Arial", 9.0F);

	m_stcResultNg.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stcResultNg.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stcResultNg.SetFont_Gdip(L"Arial", 9.0F);

	m_stcRfid.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stcRfid.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stcRfid.SetFont_Gdip(L"Arial", 9.0F);
	m_stcRfid.SetText(_T("rfid"));

	m_stcBarcode.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_stcBarcode.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stcBarcode.SetFont_Gdip(L"Arial", 9.0F);
	m_stcBarcode.SetText(_T("barcode"));
}

// CDlgSocket 메시지 처리기
void CDlgSocket::SetSocket(__in ST_SocketInfo socket)
{
	CString sz;

	m_stcResultOk.SetColorStyle(CVGStatic::ColorStyle_White);
	m_stcResultNg.SetColorStyle(CVGStatic::ColorStyle_White);

	switch (socket.status)
	{
	case Ready:
		m_stcStatus.SetText(_T("ready"));
		m_stcResultNg.SetText(_T(""));
		break;

	case Testing:
		m_stcStatus.SetText(_T("testing"));
		break;

	case Done:
		m_stcStatus.SetText(_T("done"));

		if (socket.nSEMCO_Code == 0)
		{
			m_stcResultOk.SetColorStyle(CVGStatic::ColorStyle_Green);
			m_stcResultNg.SetColorStyle(CVGStatic::ColorStyle_White);
		}			
		else
		{
			CString sz;
			sz.Format(_T("%d"), socket.nSEMCO_Code);
			m_stcResultOk.SetColorStyle(CVGStatic::ColorStyle_White);
			m_stcResultNg.SetColorStyle(CVGStatic::ColorStyle_Red);
			m_stcResultNg.SetText(sz);
		}
		break;

	case Bypass:
		m_stcStatus.SetText(_T("bypass"));
		break;

	default:
		break;
	}

	sz.Format(_T("%s"), socket.szRFID);
	m_stcRfid.SetText(sz);

	sz.Format(_T("%s"), socket.szBCR);
	m_stcBarcode.SetText(sz);
}
