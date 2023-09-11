// DlgSemcoComTest.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgSemcoComTest.h"
#include "afxdialogex.h"

#include "DlgMain.h"


// CDlgSemcoComTest 대화 상자

IMPLEMENT_DYNAMIC(CDlgSemcoComTest, CDialogEx)

CDlgSemcoComTest::CDlgSemcoComTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SEMCO_TEST, pParent)
{
	m_PortType = InspectionPortType::Left;
}

CDlgSemcoComTest::~CDlgSemcoComTest()
{
}

void CDlgSemcoComTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Radio(pDX, IDC_RADIO_SEMCO_PORT_LEFT, (int&)m_PortType);
	DDX_Text(pDX, IDC_EDIT_SEMCO_SEND_BARCODE, m_szSendBcr);
	DDX_Text(pDX, IDC_EDIT_SEMCO_RECV_BARCODE, m_szRecvBcr);
}

BOOL CDlgSemcoComTest::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgSemcoComTest::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgSemcoComTest, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_SEMCO_READY, &CDlgSemcoComTest::OnBnClickedBtnSemcoReady)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_BCR, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendBcr)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_PREVIEW_START, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendPreviewStart)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_TLA_INSPECTION, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendTLAInspection)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_VIGNETING_INSPECTION_46, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendVignetingInspection46)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_VIGNETING_INSPECTION_82, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendVignetingInspection82)

	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_DISTORTION_CAPTURE, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendDistortionCapture)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_DISTORTION_CALCULATE, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendDistortionCalculate)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_FLARE_INSPECTION, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendFlareInspection)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_PREVIEW_STOP, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendPreviewStop)
	ON_BN_CLICKED(IDC_BTN_SEMCO_COLORCAL_READY, &CDlgSemcoComTest::OnBnClickedBtnSemcoColorcalReady)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_COLORCAL_PREVIEW_START, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalPreviewStart)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_COLORCAL_INSPECTION, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalInspection)
	ON_BN_CLICKED(IDC_BTN_SEMCO_SEND_COLORCAL_PREVIEW_STOP, &CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalPreviewStop)
END_MESSAGE_MAP()

// CDlgSemcoComTest 메시지 처리기
void CDlgSemcoComTest::OnBnClickedBtnSemcoReady()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendReadyStart(m_PortType);
	
	SEMCO_WaitReadyStartAck(m_PortType, 1000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendBcr()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	m_szRecvBcr.Empty();

	SEMCO_Reset(m_PortType);

	bRet = SEMCO_SendBarcode(m_PortType, m_szSendBcr);

	SEMCO_WaitBarcodeAck(m_PortType, 1000, m_szRecvBcr);

	CString sz;
	sz.Format(_T("Return : %s"), m_szRecvBcr);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendPreviewStart()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendPreviewStart(m_PortType);

	SEMCO_WaitPreviewStartAck(m_PortType, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendTLAInspection()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;
	CString str; str.Empty();

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);
	m_szRecvBcr.Empty();
	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType , eComMode_Insp_TLA);

	SEMCO_WaitStartTestAck(m_PortType, eComMode_Insp_TLA, 10000, nRetCode);

	
	str.Format(_T("Pixel Y : %0.3f"), m_UdpTester[(int)m_PortType].m_dTesterFlag_TLA_PixelY);
	m_szRecvBcr = str;
	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendVignetingInspection46()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType, eComMode_Insp_Vigneting46);

	SEMCO_WaitStartTestAck(m_PortType, eComMode_Insp_Vigneting46, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}


void CDlgSemcoComTest::OnBnClickedBtnSemcoSendVignetingInspection82()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType, eComMode_Insp_Vigneting82);

	SEMCO_WaitStartTestAck(m_PortType, eComMode_Insp_Vigneting82, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}


void CDlgSemcoComTest::OnBnClickedBtnSemcoSendDistortionCapture()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType, 1);

	SEMCO_WaitStartTestAck(m_PortType, 1, 30000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendDistortionCalculate()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType);

	SEMCO_WaitStartTestAck(m_PortType, 0, 30000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendFlareInspection()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	IO_O_FlareLightAllOff();

	for (int i = 0; i < 10; i++)
	{
		IO_O_FlareLightOn(i);

		bRet = SEMCO_SendStartTest(m_PortType, i);

		SEMCO_WaitStartTestAck(m_PortType, i, 30000, nRetCode);

		CString sz;
		sz.Format(_T("ReturnCode : %d"), nRetCode);
		WRITE_MAIN_LOG(sz);

		IO_O_FlareLightOff(i);
	}
	
	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendPreviewStop()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendPreviewStop(m_PortType);

	SEMCO_WaitPreviewStopAck(m_PortType, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoColorcalReady()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendReadyStart(m_PortType);

	SEMCO_WaitReadyStartAck(m_PortType, 1000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalPreviewStart()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendPreviewStart(m_PortType);

	SEMCO_WaitPreviewStartAck(m_PortType, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalInspection()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendStartTest(m_PortType);

	SEMCO_WaitStartTestAck(m_PortType, 0, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

void CDlgSemcoComTest::OnBnClickedBtnSemcoSendColorcalPreviewStop()
{
	UpdateData(TRUE);

	bool bRet = false;
	UINT nRetCode = 0;

	UINT nID = GetFocus()->GetDlgCtrlID();
	GetDlgItem(nID)->EnableWindow(FALSE);

	SEMCO_Reset(m_PortType);
	bRet = SEMCO_SendPreviewStop(m_PortType);

	SEMCO_WaitPreviewStopAck(m_PortType, 10000, nRetCode);

	CString sz;
	sz.Format(_T("ReturnCode : %d"), nRetCode);
	WRITE_MAIN_LOG(sz);

	GetDlgItem(nID)->EnableWindow(TRUE);
}

