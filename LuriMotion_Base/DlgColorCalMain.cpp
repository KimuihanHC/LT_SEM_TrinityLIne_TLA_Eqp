#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgColorCalMain.h"
#include "afxdialogex.h"
#include "DlgLog.h"

// CDlgColorCalMain 대화 상자
IMPLEMENT_DYNAMIC(CDlgColorCalMain, CDialogEx)

CDlgColorCalMain::CDlgColorCalMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_COLOR_CAL_MAIN, pParent)
{

}

CDlgColorCalMain::~CDlgColorCalMain()
{
}

void CDlgColorCalMain::ShowOperation(__in OperationMode mode)
{
	switch (mode)
	{
	case OperationMode::Auto:
	case OperationMode::Manual:
	case OperationMode::SemiAuto:
	{
		GetDlgItem(IDC_STC_COLORCAL_LEFT_CL500A_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_COLORCAL_RIGHT_CL500A_NAME)->ShowWindow(SW_HIDE);

		for (int i = 0; i < 20; i++)
		{
			for (int port = 0; port < 2; port++)
			{
				m_stLightChName[port][i].ShowWindow(SW_HIDE);
				m_stLightChColorTemp[port][i].ShowWindow(SW_HIDE);
				m_stLightChLux[port][i].ShowWindow(SW_HIDE);
			}
		}
	}
	break;

	case OperationMode::StartUpCheck:
		GetDlgItem(IDC_STC_COLORCAL_LEFT_CL500A_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_COLORCAL_RIGHT_CL500A_NAME)->ShowWindow(SW_SHOW);

		for (int i = 0; i < 20; i++)
		{
			for (int port = 0; port < 2; port++)
			{
				m_stLightChName[port][i].ShowWindow(SW_SHOW);
				m_stLightChColorTemp[port][i].ShowWindow(SW_SHOW);
				m_stLightChLux[port][i].ShowWindow(SW_SHOW);
			}			
		}
		break;
	default:
		GetDlgItem(IDC_STC_COLORCAL_LEFT_CL500A_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_COLORCAL_RIGHT_CL500A_NAME)->ShowWindow(SW_HIDE);

		for (int i = 0; i < 20; i++)
		{
			for (int port = 0; port < 2; port++)
			{
				m_stLightChName[port][i].ShowWindow(SW_HIDE);
				m_stLightChColorTemp[port][i].ShowWindow(SW_HIDE);
				m_stLightChLux[port][i].ShowWindow(SW_HIDE);
			}
		}
		break;
	}
}

ST_OptSemiAuto CDlgColorCalMain::GetOption()
{
	ST_OptSemiAuto opt;

	UpdateData(TRUE);

	opt.Angle = (enAngleType)m_iAngleType;
	opt.Socket = (enSocketType)m_iSocketType;
	
	return opt;
}

BOOL CDlgColorCalMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgColorCalMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_COLORCAL_46_ANGLE, m_iAngleType);
	DDX_Radio(pDX, IDC_RADIO_COLORCAL__SOCKET_H, m_iSocketType);

	int i = 0;

	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME, m_stTitleName[0]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME, m_stTitleName[1]);

	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH1, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH1, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH1, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH1, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH1, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH1, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH2, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH2, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH2, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH2, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH2, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH2, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH3, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH3, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH3, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH3, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH3, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH3, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH4, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH4, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH4, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH4, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH4, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH4, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH5, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH5, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH5, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH5, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH5, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH5, m_stLightChLux[1][i]);
	
	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH6, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH6, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH6, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH6, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH6, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH6, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH7, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH7, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH7, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH7, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH7, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH7, m_stLightChLux[1][i]);
	
	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH8, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH8, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH8, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH8, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH8, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH8, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH9, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH9, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH9, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH9, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH9, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH9, m_stLightChLux[1][i]);


	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH10, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH10, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH10, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH10, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH10, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH10, m_stLightChLux[1][i]);


	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH11, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH11, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH11, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH11, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH11, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH11, m_stLightChLux[1][i]);


	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH12, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH12, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH12, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH12, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH12, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH12, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH13, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH13, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH13, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH13, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH13, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH13, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH14, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH14, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH14, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH14, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH14, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH14, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH15, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH15, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH15, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH15, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH15, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH15, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH16, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH16, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH16, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH16, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH16, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH16, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH17, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH17, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH17, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH17, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH17, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH17, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH18, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH18, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH18, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH18, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH18, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH18, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH19, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH19, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH19, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH19, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH19, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH19, m_stLightChLux[1][i]);

	i++;
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_NAME_CH20, m_stLightChName[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_TEMPER_DATA_CH20, m_stLightChColorTemp[0][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_LEFT_CL500A_LUX_DATA_CH20, m_stLightChLux[0][i]);

	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_NAME_CH20, m_stLightChName[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_TEMPER_DATA_CH20, m_stLightChColorTemp[1][i]);
	DDX_Control(pDX, IDC_STC_COLORCAL_RIGHT_CL500A_LUX_DATA_CH20, m_stLightChLux[1][i]);
}

BOOL CDlgColorCalMain::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgColorCalMain::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgColorCalMain, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDlgColorCalMain::InitComponent()
{
	CString sz;

	for (auto i = 0; i < 2; i++)
	{
		m_stTitleName[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stTitleName[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stTitleName[i].SetFont_Gdip(L"Arial", 12.0F);
	}

	for (auto i = 0; i < 20; i++)
	{
		for (auto port = 0; port < 2; port++)
		{
			m_stLightChName[port][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stLightChName[port][i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_stLightChName[port][i].SetFont_Gdip(L"Arial", 12.0F);

			sz.Format(_T("CH%d"), i + 1);
			m_stLightChName[port][i].SetText(sz);

			m_stLightChColorTemp[port][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stLightChColorTemp[port][i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_stLightChColorTemp[port][i].SetFont_Gdip(L"Arial", 12.0F);

			m_stLightChLux[port][i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stLightChLux[port][i].SetColorStyle(CVGStatic::ColorStyle_SlateGray);
			m_stLightChLux[port][i].SetFont_Gdip(L"Arial", 12.0F);
		}		
	}

	m_stTitleName[0].SetText(_T("(L) CL500A"));
	m_stTitleName[1].SetText(_T("(R) CL500A"));
	Reset();
}

void CDlgColorCalMain::ChangeUI_Language()
{
}

void CDlgColorCalMain::Reset()
{
	for (auto i = 0; i < 20; i++)
	{
		for (auto port = 0; port < 2; port++)
		{
			m_stLightChColorTemp[port][i].SetText(_T("0"));
			m_stLightChLux[port][i].SetText(_T("0"));
		}
	}
}

void CDlgColorCalMain::SetCL500A(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk)
{
	int nIdx = (int)port;

	CString sz;
	
	if (bReadOk)
	{
		sz.Format(_T("%0.2f k"), fColorTemper);
		m_stLightChColorTemp[(int)port][nLightIdx].SetText(sz);

		sz.Format(_T("%0.2f lux"), fData);
		m_stLightChLux[(int)port][nLightIdx].SetText(sz);
	}
	else
	{
		m_stLightChColorTemp[(int)port][nLightIdx].SetText(_T("READ FAIL"));
		m_stLightChLux[(int)port][nLightIdx].SetText(_T("READ FAIL"));
	}	
}

void CDlgColorCalMain::ResetCL500A(__in InspectionPortType port)
{
	int nIdx = (int)port;

	for (auto i = 0; i < 20; i++)
	{	
		m_stLightChColorTemp[nIdx][i].SetText(_T("READY"));
		m_stLightChLux[nIdx][i].SetText(_T("READY"));
	}
}

