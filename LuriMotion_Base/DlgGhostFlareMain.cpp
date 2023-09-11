#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgGhostFlareMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgGhostFlareMain 대화 상자
#define GHOSTFLARE_MAIN_TIMER_LIGHT_STATUS					300

IMPLEMENT_DYNAMIC(CDlgGhostFlareMain, CDialogEx)

CDlgGhostFlareMain::CDlgGhostFlareMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_GHOSTFLARE_MAIN, pParent)
{

}

CDlgGhostFlareMain::~CDlgGhostFlareMain()
{

}

void CDlgGhostFlareMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE)
	{
		SetTimer(GHOSTFLARE_MAIN_TIMER_LIGHT_STATUS, 250, NULL);
		if (COMMON->GetPermissionGrade() < 1)
			m_stGhostFlareMainLightCtrl_Buttons->EnableWindow(FALSE);
		else
			m_stGhostFlareMainLightCtrl_Buttons->EnableWindow(TRUE);
	}
	else
		KillTimer(GHOSTFLARE_MAIN_TIMER_LIGHT_STATUS);
}

void CDlgGhostFlareMain::ShowOperation(__in OperationMode mode)
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

ST_OptSemiAuto CDlgGhostFlareMain::GetOption()
{
	ST_OptSemiAuto opt;

	return opt;
}

BOOL CDlgGhostFlareMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgGhostFlareMain::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case GHOSTFLARE_MAIN_TIMER_LIGHT_STATUS:
		//Main Status
		ChangeDectectedUI_Status();
		break;
	}
}

void CDlgGhostFlareMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_TITLE, m_stGhostFlareMainLightCtrl_Title);

	DDX_Control(pDX, IDC_PWSP_ON2, m_stGhostFlareMainLightCtrl_Buttons[0]);
	DDX_Control(pDX, IDC_PWSP_ON3, m_stGhostFlareMainLightCtrl_Buttons[1]);
	DDX_Control(pDX, IDC_PWSP_ON4, m_stGhostFlareMainLightCtrl_Buttons[2]);
	DDX_Control(pDX, IDC_PWSP_ON5, m_stGhostFlareMainLightCtrl_Buttons[3]);
	DDX_Control(pDX, IDC_PWSP_ON6, m_stGhostFlareMainLightCtrl_Buttons[4]);
	DDX_Control(pDX, IDC_PWSP_ON7, m_stGhostFlareMainLightCtrl_Buttons[5]);
	DDX_Control(pDX, IDC_PWSP_ON8, m_stGhostFlareMainLightCtrl_Buttons[6]);
	DDX_Control(pDX, IDC_PWSP_ON9, m_stGhostFlareMainLightCtrl_Buttons[7]);
	DDX_Control(pDX, IDC_PWSP_ON10, m_stGhostFlareMainLightCtrl_Buttons[8]);
	DDX_Control(pDX, IDC_PWSP_ON11, m_stGhostFlareMainLightCtrl_Buttons[9]);
	DDX_Control(pDX, IDC_PWSP_OFF12, m_stGhostFlareMainLightCtrl_Buttons[10]);
}

BOOL CDlgGhostFlareMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgGhostFlareMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgGhostFlareMain, CDialogEx)
// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	ON_BN_CLICKED(IDC_PWSP_CONNECT2, &CDlgGhostFlareMain::OnBnClickedPwspConnect2)
// 	ON_BN_CLICKED(IDC_PWSP_ON2, &CDlgGhostFlareMain::OnBnClickedPwspOn2)
// 	ON_BN_CLICKED(IDC_PWSP_OFF2, &CDlgGhostFlareMain::OnBnClickedPwspOff2)
// 	ON_BN_CLICKED(IDC_PWSP_CONNECT1, &CDlgGhostFlareMain::OnBnClickedPwspConnect1)
// 	ON_BN_CLICKED(IDC_PWSP_ON1, &CDlgGhostFlareMain::OnBnClickedPwspOn1)
// 	ON_BN_CLICKED(IDC_PWSP_OFF1, &CDlgGhostFlareMain::OnBnClickedPwspOff1)
// 	ON_BN_CLICKED(IDC_PWSP_ON3, &CDlgGhostFlareMain::OnBnClickedPwspOn3)
// 	ON_BN_CLICKED(IDC_PWSP_OFF4, &CDlgGhostFlareMain::OnBnClickedPwspOff4)
// 	ON_BN_CLICKED(IDC_PWSP_ON7, &CDlgGhostFlareMain::OnBnClickedPwspOn7)
// 	ON_BN_CLICKED(IDC_PWSP_OFF8, &CDlgGhostFlareMain::OnBnClickedPwspOff8)
// 	ON_BN_CLICKED(IDC_PWSP_ON4, &CDlgGhostFlareMain::OnBnClickedPwspOn4)
// 	ON_BN_CLICKED(IDC_PWSP_OFF5, &CDlgGhostFlareMain::OnBnClickedPwspOff5)
// 	ON_BN_CLICKED(IDC_PWSP_ON8, &CDlgGhostFlareMain::OnBnClickedPwspOn8)
// 	ON_BN_CLICKED(IDC_PWSP_OFF9, &CDlgGhostFlareMain::OnBnClickedPwspOff9)
// 	ON_BN_CLICKED(IDC_PWSP_ON5, &CDlgGhostFlareMain::OnBnClickedPwspOn5)
// 	ON_BN_CLICKED(IDC_PWSP_OFF6, &CDlgGhostFlareMain::OnBnClickedPwspOff6)
// 	ON_BN_CLICKED(IDC_PWSP_ON10, &CDlgGhostFlareMain::OnBnClickedPwspOn10)
// 	ON_BN_CLICKED(IDC_PWSP_OFF11, &CDlgGhostFlareMain::OnBnClickedPwspOff11)
// 	ON_BN_CLICKED(IDC_PWSP_ON6, &CDlgGhostFlareMain::OnBnClickedPwspOn6)
// 	ON_BN_CLICKED(IDC_PWSP_OFF7, &CDlgGhostFlareMain::OnBnClickedPwspOff7)
// 	ON_BN_CLICKED(IDC_PWSP_ON9, &CDlgGhostFlareMain::OnBnClickedPwspOn9)
// 	ON_BN_CLICKED(IDC_PWSP_OFF10, &CDlgGhostFlareMain::OnBnClickedPwspOff10)
// 	ON_BN_CLICKED(IDC_PWSP_ON11, &CDlgGhostFlareMain::OnBnClickedPwspOn11)
// 	ON_BN_CLICKED(IDC_PWSP_OFF12, &CDlgGhostFlareMain::OnBnClickedPwspOff12)
//#endif
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgGhostFlareMain::OnBnClickedPwspConnect2()
{


}

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// void CDlgGhostFlareMain::OnBnClickedPwspOn2()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(0) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_1.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_1.Create();
// 			m_ctrlLight_SOL_3_1.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(0, m_ctrlLight_SOL_3_1.OpenPort(3, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(0) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[0].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// 		}
// 	}
// }
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff2()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F, FALSE);
// }
// 
// void CDlgGhostFlareMain::OnBnClickedPwspConnect1()
// {
// 	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();
// 
// 	if (m_ctrlLight_ODA_Q[0].m_bConnected == FALSE)
// 	{
// 		m_ctrlLight_ODA_Q[0].Create();
// 		m_ctrlLight_ODA_Q[0].Invalidate(0);
// 	}
// 
// 	if (m_ctrlLight_ODA_Q[0].OpenPort(stTester.nPort[0] + 1, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0) == FALSE)
// 	{
// 		WRITE_MAIN_LOG(_T("FAIL CONNECT"));
// 	}
// 	else
// 		WRITE_MAIN_LOG(_T("CONNECT SUCCESS"));
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn1()
// {
// 	if (m_ctrlLight_ODA_Q[0].Wrtie_ON())
// 	{
// 		WRITE_MAIN_LOG(_T("POWER ON SUCCESS\n"));
// 	}
// 	else
// 		WRITE_MAIN_LOG(_T("POWER ON FAILED\n"));
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff1()
// {
// 	if (m_ctrlLight_ODA_Q[0].Write_OFF())
// 	{
// 		WRITE_MAIN_LOG(_T("POWER OFF SUCCESS\n"));
// 	}
// 	else
// 		WRITE_MAIN_LOG(_T("POWER OFF FAILED\n"));
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn3()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(1) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_2.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_2.Create();
// 			m_ctrlLight_SOL_3_2.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(1, m_ctrlLight_SOL_3_2.OpenPort(7, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(1) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[1].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff4()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn7()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(8) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_9.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_9.Create();
// 			m_ctrlLight_SOL_3_9.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(8, m_ctrlLight_SOL_3_9.OpenPort(14, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(8) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[5].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[5].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff8()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn4()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(7) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_8.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_8.Create();
// 			m_ctrlLight_SOL_3_8.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(7, m_ctrlLight_SOL_3_8.OpenPort(13, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(7) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[2].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[2].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff5()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn8()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(6) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_7.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_7.Create();
// 			m_ctrlLight_SOL_3_7.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(6, m_ctrlLight_SOL_3_7.OpenPort(12, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(6) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[6].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, TRUE);
// 
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[6].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff9()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn5()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(5) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_6.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_6.Create();
// 			m_ctrlLight_SOL_3_6.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(5, m_ctrlLight_SOL_3_6.OpenPort(11, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(5) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[3].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[3].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff6()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn10()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(4) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_5.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_5.Create();
// 			m_ctrlLight_SOL_3_5.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(4, m_ctrlLight_SOL_3_5.OpenPort(10, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(4) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[8].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[8].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff11()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn6()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(3) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_4.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_4.Create();
// 			m_ctrlLight_SOL_3_4.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(3, m_ctrlLight_SOL_3_4.OpenPort(9, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(3) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[4].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[4].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff7()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn9()
// {
// 	return;
// 	if (COMMON->GetCtrlLightIsConnected(2) == FALSE)
// 	{
// 		if (m_ctrlLight_SOL_3_3.m_hWnd == NULL)
// 		{
// 			m_ctrlLight_SOL_3_3.Create();
// 			m_ctrlLight_SOL_3_3.Invalidate(0);
// 		}
// 		COMMON->SetCtrlLightConnected(2, m_ctrlLight_SOL_3_3.OpenPort(8, 0, WM_LIGHT_CONTROL_MSG, 0, 1, 0, 0));
// 		Sleep(100);
// 	}
// 	if (COMMON->GetCtrlLightIsConnected(2) == TRUE)
// 	{
// 		if (AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR) == FALSE)
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[7].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, TRUE);
// 		}
// 		else
// 		{
// 			m_stGhostFlareMainLightCtrl_Buttons[8].SetColorStyle(CVGStatic::ColorStyle_Green);
// 			AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// 		}
// 	}
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff10()
// {
// 	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR, FALSE);
// }
// 
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOn11()
// {
// 	m_stGhostFlareMainLightCtrl_Buttons[9].SetColorStyle(CVGStatic::ColorStyle_Green);
// 	m_stGhostFlareMainLightCtrl_Buttons[10].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	OnBnClickedPwspOn2();
// 	OnBnClickedPwspOn3();
// 	OnBnClickedPwspOn4();
// 	OnBnClickedPwspOn5();
// 	OnBnClickedPwspOn6();
// 	OnBnClickedPwspOn7();
// 	OnBnClickedPwspOn8();
// 	OnBnClickedPwspOn9();
// 	OnBnClickedPwspOn10();
// }
// 
// void CDlgGhostFlareMain::OnBnClickedPwspOff12()
// {
// 	m_stGhostFlareMainLightCtrl_Buttons[10].SetColorStyle(CVGStatic::ColorStyle_Green);
// 	m_stGhostFlareMainLightCtrl_Buttons[9].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	OnBnClickedPwspOff2();
// 	OnBnClickedPwspOff4();
// 	OnBnClickedPwspOff5();
// 	OnBnClickedPwspOff6();
// 	OnBnClickedPwspOff7();
// 	OnBnClickedPwspOff8();
// 	OnBnClickedPwspOff9();
// 	OnBnClickedPwspOff10();
// 	OnBnClickedPwspOff11();
// }
// #endif

void CDlgGhostFlareMain::InitComponent()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	OnBnClickedPwspOn11();
// #endif

	//광원 버튼들
	for (int i = 0; i < 11; i++)
	{
		m_stGhostFlareMainLightCtrl_Buttons[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stGhostFlareMainLightCtrl_Buttons[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stGhostFlareMainLightCtrl_Buttons[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stGhostFlareMainLightCtrl_Buttons[i].SetText(m_szMainLightCtrlBtn_Total[i]);
	}

	m_stGhostFlareMainLightCtrl_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stGhostFlareMainLightCtrl_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stGhostFlareMainLightCtrl_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stGhostFlareMainLightCtrl_Title.SetText(_T("광원 컨트롤러"));
	else
		m_stGhostFlareMainLightCtrl_Title.SetText(_T("Light Controller"));

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	OnBnClickedPwspOff12();
// #endif
}

void CDlgGhostFlareMain::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	m_stGhostFlareMainLightCtrl_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
	m_stGhostFlareMainLightCtrl_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_stGhostFlareMainLightCtrl_Title.SetFont_Gdip(L"Arial", 9.0F);
	if (stBasic.nLanguage == 0)
		m_stGhostFlareMainLightCtrl_Title.SetText(_T("광원 컨트롤러"));
	else
		m_stGhostFlareMainLightCtrl_Title.SetText(_T("Light Controller"));
}

void CDlgGhostFlareMain::ChangeDectectedUI_Status()
{
// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y0_1B_LIGHTCTRL_0F) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[0].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[0].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_14_LIGHTCTRL_05FB) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[1].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[1].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_1A_LIGHTCTRL_05FL) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[2].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[2].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_18_LIGHTCTRL_05FT) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[3].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[3].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_16_LIGHTCTRL_05FR) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[4].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[4].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_1B_LIGHTCTRL_1FLL) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[5].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[5].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_19_LIGHTCTRL_1FUL) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[6].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[6].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_15_LIGHTCTRL_1FLR) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[7].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[7].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// 	AJIN_IO->GetOutputBit(DIO_OUTPUT_Y1_17_LIGHTCTRL_1FUR) == TRUE ? m_stGhostFlareMainLightCtrl_Buttons[8].SetColorStyle(CVGStatic::ColorStyle_Green) : m_stGhostFlareMainLightCtrl_Buttons[8].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
// #endif
}
