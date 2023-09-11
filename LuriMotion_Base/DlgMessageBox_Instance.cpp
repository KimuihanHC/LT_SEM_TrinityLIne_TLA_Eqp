// DlgMessageBox.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "LuriMotion_BaseDlg.h"
#include "DlgMessageBox_Instance.h"
#include "afxdialogex.h"

// CDlgMessageBox 대화 상자

IMPLEMENT_DYNAMIC(CDlgMessageBox_Instance, CDialogEx)
CDlgMessageBox_Instance *CDlgMessageBox_Instance::m_pInstance = NULL;

CDlgMessageBox_Instance *CDlgMessageBox_Instance::Get_Instance(CWnd *pParent)
{
	if (!m_pInstance) {
		m_pInstance = new CDlgMessageBox_Instance(pParent);
		if (!m_pInstance->m_hWnd) {
			m_pInstance->Create(IDD_DLG_MESSAGEBOX_INSTANCE, pParent);
		}
	}
	return m_pInstance;
}

void CDlgMessageBox_Instance::Delete_Instance()
{
	if (m_pInstance->m_hWnd) m_pInstance->DestroyWindow();
	if (m_pInstance) delete m_pInstance;
	m_pInstance = NULL;
}
CDlgMessageBox_Instance::CDlgMessageBox_Instance(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESSAGEBOX_INSTANCE, pParent)
{
	m_strError = _T("");
	//m_nMode = -1;
	m_nMode = MODE_INFORMATION;
}

CDlgMessageBox_Instance::~CDlgMessageBox_Instance()
{
}

void CDlgMessageBox_Instance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TEST_1, m_stTitle);
	DDX_Control(pDX, IDC_MESS_BOX_INST_BTN_1, m_ctrlButton[eBtnOk]);
	DDX_Control(pDX, IDC_MESS_BOX_INST_BTN_2, m_ctrlButton[eBtnCancel]);
	DDX_Control(pDX, IDC_MESS_BOX_INST_BTN_3, m_ctrlButton[eBtnBuzOff]);
	DDX_Control(pDX, IDC_MESS_BOX_INST_BTN_4, m_ctrlButton[eBtnDoorLock]);
}

BEGIN_MESSAGE_MAP(CDlgMessageBox_Instance, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_MESS_BOX_INST_BTN_1, &CDlgMessageBox_Instance::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_MESS_BOX_INST_BTN_2, &CDlgMessageBox_Instance::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_MESS_BOX_INST_BTN_3, &CDlgMessageBox_Instance::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_MESS_BOX_INST_BTN_4, &CDlgMessageBox_Instance::OnBnClickedBtn4)
END_MESSAGE_MAP()

BOOL CDlgMessageBox_Instance::PreTranslateMessage(MSG* pMsg)
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
int CDlgMessageBox_Instance::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//InitComponent_MessageBox();

	return 0;
}
// CDlgMessageBox 메시지 처리기
BOOL CDlgMessageBox_Instance::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(105, 186, 235));
	//pDC->FillSolidRect(rect, RGB(84, 169, 169));
	pDC->FillSolidRect(rect, RGB(70, 70, 70));		// Lime
	return TRUE;
}
BOOL CDlgMessageBox_Instance::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComponent_MessageBox();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMessageBox_Instance::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE)
	{
		ChangeComponent_MessageBox();
	}
	else
	{
		::SetEvent(COMMON->m_closeEvent);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

// CDlgMain 메시지 처리기
void CDlgMessageBox_Instance::InitComponent_MessageBox()
{
	CString strTemp = _T("");

	m_stTitle.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stTitle.SetColorStyle(CVGStatic::ColorStyle_Yellow);
	m_stTitle.SetFont_Gdip(L"Arial", 12.0F);
	m_stTitle.SetText(m_strError);
}
void CDlgMessageBox_Instance::ChangeComponent_MessageBox()
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
}

void CDlgMessageBox_Instance::SetMsg(CString str, int nMode)
{
	m_strError = str;
	m_nMode = nMode;
}

void CDlgMessageBox_Instance::OnBnClickedBtn1()
{
	m_nReturn = 1;
	::SetEvent(COMMON->m_closeEvent);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	EndDialog(IDOK);
}
void CDlgMessageBox_Instance::OnBnClickedBtn2()
{
	m_nReturn = 0;
	::SetEvent(COMMON->m_closeEvent);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	EndDialog(IDCANCEL);
}
void CDlgMessageBox_Instance::OnBnClickedBtn3()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
}

void CDlgMessageBox_Instance::OnBnClickedBtn4()
{
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0C_TOWERLAMP_BEEP1, FALSE);
	for (int i = 0; i < 5; i++)
		AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_0E_FRONT_DOOR1_INTERLOCK_ONOFF + i * 2 , TRUE);
}
