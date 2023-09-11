#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgDeviceConfiguration.h"
#include "afxdialogex.h"
#include "Device.h"
#include "DlgLog.h"

// CDlgDeviceConfiguration 대화 상자

IMPLEMENT_DYNAMIC(CDlgDeviceConfiguration, CDialogEx)

CDlgDeviceConfiguration::CDlgDeviceConfiguration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DEVICE_CONFIGURATION, pParent)
{

}

CDlgDeviceConfiguration::~CDlgDeviceConfiguration()
{
}

void CDlgDeviceConfiguration::SaveConfiguration()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int i = 0; i < 20; i++)
	{
		CString strData;
		m_edComponent_Light_Value[i].GetWindowText(strData);
		stTester.nLightValue[i] = _ttoi(strData);
	}

	for (int i = 0; i < 4; i++)
	{
		CString strData;
		m_edComponent_Colorcal_SemcoLight[i].GetWindowText(strData);
		stTester.nColorCal_SemcoLight[i] = _ttoi(strData);
	}

	DATAMANAGER->SetTesterCommData(stTester);
}

void CDlgDeviceConfiguration::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");

	for (int i = 0; i < 3; i++)
	{
		if (stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szDeviceConfigurationTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szDeviceConfigurationTitle_Eng[i]);
	}

	if (stBasic.nLanguage == 0)
	{
		m_btnComponent_Read_RFID.SetWindowText(_T("읽기"));
		m_btnComponent_Measure_ChromaMeter.SetWindowText(_T("측정"));
	}
	else
	{
		m_btnComponent_Read_RFID.SetWindowText(_T("Read"));
		m_btnComponent_Measure_ChromaMeter.SetWindowText(_T("Measure"));
	}
}

void CDlgDeviceConfiguration::Load_ComponentDlg()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int i = 0; i < 20; i++)
	{
		CString strData;
		strData.Format(_T("%d"), stTester.nLightValue[i]);
		m_edComponent_Light_Value[i].SetWindowText(strData);
	}

	for (int i = 0; i < 4; i++)
	{
		CString strData;
		strData.Format(_T("%d"), stTester.nColorCal_SemcoLight[i]);
		m_edComponent_Colorcal_SemcoLight[i].SetWindowText(strData);
	}
}

BOOL CDlgDeviceConfiguration::OnInitDialog()
{
	__super::OnInitDialog();

	InitComponent_Component();
	InspectorCtrlSetting();

	return TRUE;
}

void CDlgDeviceConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_LIGHT, m_stComponent_Title[0]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_RFID, m_stComponent_Title[1]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_CHROMAMETER, m_stComponent_Title[2]);

	for (auto index = 0; index < 20; index++)
	{
		DDX_Control(pDX, IDC_STC_LIGHT_VALUE1 + index * 4, m_stComponent_Light_ValueName[index]);
		DDX_Control(pDX, IDC_ED_LIGHT_VALUE1 + index * 4, m_edComponent_Light_Value[index]);
		DDX_Control(pDX, IDC_BTN_LIGHT_ON1 + index * 4, m_btnComponent_Light_On[index]);
		DDX_Control(pDX, IDC_BTN_LIGHT_OFF1 + index * 4, m_btnComponent_Light_Off[index]);
	}

	for (auto index = 0; index < 4; index++)
	{
		DDX_Control(pDX, IDC_STC_COLORCAL_SEMCOM_LIGHT_CH1 + index, m_stComponent_Colorcal_SemcoLight[index]);
		DDX_Control(pDX, IDC_ED_COLORCAL_SEMCO_LIGHT_VALUE_CH1 + index, m_edComponent_Colorcal_SemcoLight[index]);
	}

	DDX_Control(pDX, IDC_BTN_READ_RFID, m_btnComponent_Read_RFID);
	DDX_Control(pDX, IDC_ED_READ_RFID, m_edComponent_Read_RFID);

	DDX_Control(pDX, IDC_BTN_MEASURE_CHROMAMETER, m_btnComponent_Measure_ChromaMeter);
	DDX_Control(pDX, IDC_ED_CHROMAMETER_MEASUREMENT_DATA, m_edChromaMeterMeasurementData);
}

// CDlgDeviceConfiguration 메시지 처리기
BEGIN_MESSAGE_MAP(CDlgDeviceConfiguration, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LIGHT_ON1, IDC_BTN_LIGHT_OFF9, &CDlgDeviceConfiguration::OnbnClickedBtnLightOnOff)
	ON_BN_CLICKED(IDC_BTN_READ_RFID, &CDlgDeviceConfiguration::OnBnClickedBtnReadRFID)
	ON_BN_CLICKED(IDC_BTN_MEASURE_CHROMAMETER, &CDlgDeviceConfiguration::OnBnClickedBtnChromaMeterMeasure)
END_MESSAGE_MAP()

BOOL CDlgDeviceConfiguration::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgDeviceConfiguration::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

void CDlgDeviceConfiguration::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		Load_ComponentDlg();
}

void CDlgDeviceConfiguration::InitComponent_Component()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);
	}

	for (int i = 0; i < 20; i++)
	{
		m_stComponent_Light_ValueName[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Light_ValueName[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Light_ValueName[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Light_ValueName[i].SetText(_T("Value"));

		m_btnComponent_Light_On[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Light_On[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Light_On[i].m_bTransparent = false;
		m_btnComponent_Light_On[i].SetTextColor(COLOR_BLACK);
		m_btnComponent_Light_On[i].SetFaceColor(RGB(225, 225, 225), true);
		m_btnComponent_Light_On[i].SetWindowText(_T("On"));

		m_btnComponent_Light_Off[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Light_Off[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Light_Off[i].m_bTransparent = false;
		m_btnComponent_Light_Off[i].SetFaceColor(COLOR_GREEN1, true);
		m_btnComponent_Light_Off[i].SetWindowText(_T("Off"));
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Colorcal_SemcoLight[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Colorcal_SemcoLight[i].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_stComponent_Colorcal_SemcoLight[i].SetFont_Gdip(L"Arial", 9.0F);

		CString sz;
		sz.Format(_T("CH_%d"), i + 1);
		m_stComponent_Colorcal_SemcoLight[i].SetText(sz);
	}

	m_btnComponent_Read_RFID.EnableWindowsTheming(FALSE);
	m_btnComponent_Read_RFID.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnComponent_Read_RFID.m_bTransparent = false;
	m_btnComponent_Read_RFID.SetTextColor(COLOR_BLACK);
	m_btnComponent_Read_RFID.SetFaceColor(RGB(225, 225, 225), true);

	m_btnComponent_Measure_ChromaMeter.EnableWindowsTheming(FALSE);
	m_btnComponent_Measure_ChromaMeter.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnComponent_Measure_ChromaMeter.m_bTransparent = false;
	m_btnComponent_Measure_ChromaMeter.SetTextColor(COLOR_BLACK);
	m_btnComponent_Measure_ChromaMeter.SetFaceColor(RGB(225, 225, 225), true);

	ChangeUI_Language();
}

void CDlgDeviceConfiguration::InspectorCtrlSetting()
{
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	VisibleLightCtrls(2);
	m_stComponent_Light_ValueName[0].SetWindowText(_T("MAIN"));
	m_stComponent_Light_ValueName[1].SetWindowText(_T("Vigneting"));
	//m_stComponent_Light_ValueName[2].SetWindowText(_T("Right"));
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	VisibleLightCtrls(2);
	m_stComponent_Light_ValueName[0].SetWindowText(_T("MAIN"));
	m_stComponent_Light_ValueName[1].SetWindowText(_T("Vigneting"));
	//m_stComponent_Light_ValueName[2].SetWindowText(_T("Right"));
#endif



// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	GetDlgItem(IDC_STC_COMPONENT_TITLE_CHROMAMETER)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_MEASURE_CHROMAMETER)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_ED_CHROMAMETER_MEASUREMENT_DATA)->ShowWindow(SW_SHOW);
// 	int count = 20;
// 	VisibleLightCtrls(count);
// 
// 	CString strChannel;
// 	for (auto i = 0; i < count; i++)
// 	{
// 		strChannel.Format(_T("Ch %02d"), i + 1);
// 		m_stComponent_Light_ValueName[i].SetWindowText(strChannel);
// 		m_edComponent_Light_Value[i].EnableWindow(FALSE);
// 	}
// #elif(SET_INSPECTOR == SYS_DISTORTION)
// 	VisibleLightCtrls(3);
// 	m_stComponent_Light_ValueName[0].SetWindowText(_T("Left"));
// 	m_stComponent_Light_ValueName[1].SetWindowText(_T("Center"));
// 	m_stComponent_Light_ValueName[2].SetWindowText(_T("Right"));
// #elif(SET_INSPECTOR == SYS_GHOSTFLARE)
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// 	GetDlgItem(IDC_STC_COMPONENT_TITLE_CHROMAMETER)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_BTN_MEASURE_CHROMAMETER)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_ED_CHROMAMETER_MEASUREMENT_DATA)->ShowWindow(SW_SHOW);
// #elif(SET_INSPECTOR == SYS_SFR_NARROW)
// 	VisibleLightCtrls(1);
// 	m_stComponent_Light_ValueName[0].SetWindowText(_T("Chart"));
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_H)
// 	VisibleLightCtrls(9);
// 	for (auto index = 0; index < 9; index++)
// 		m_stComponent_Light_ValueName[index].SetWindowText(m_szSFRMultiCL_LightPosition[index]);
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	VisibleLightCtrls(9);
// 	for (auto index = 0; index < 9; index++)
// 		m_stComponent_Light_ValueName[index].SetWindowText(m_szSFRMultiCL_LightPosition[index]);
// #endif
}

void CDlgDeviceConfiguration::VisibleLightCtrls(int count)
{
	for (auto i = 0; i < count; i++)
	{
		m_stComponent_Light_ValueName[i].ShowWindow(SW_SHOW);
		m_edComponent_Light_Value[i].ShowWindow(SW_SHOW);
		m_btnComponent_Light_On[i].ShowWindow(SW_SHOW);
		m_btnComponent_Light_Off[i].ShowWindow(SW_SHOW);
	}
}

void CDlgDeviceConfiguration::OnbnClickedBtnLightOnOff(UINT nID)
{
	UpdateData(TRUE);

	if ((nID - IDC_BTN_LIGHT_ON1) % 4 == 0)
	{
		int index = (nID - IDC_BTN_LIGHT_ON1) / 4;
		LightOn(index);
	}
	else if ((nID - IDC_BTN_LIGHT_OFF1) % 4 == 0)
	{
		int index = (nID - IDC_BTN_LIGHT_OFF1) / 4;
		LightOff(index);
	}
}

void CDlgDeviceConfiguration::OnBnClickedBtnReadRFID()
{
	CDevice device;
	CString strData;

	if (!device.RFID_Read(strData))
	{
		WRITE_ERR_LOG(_T("RFID read failure"));
		m_edComponent_Read_RFID.SetWindowText(_T("Read failed."));
		return;
	}
	m_edComponent_Read_RFID.SetWindowText(strData);
}

void CDlgDeviceConfiguration::OnBnClickedBtnChromaMeterMeasure()
{
//#if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	m_edChromaMeterMeasurementData.SetWindowText(_T("Measuring..."));
// 
// 	float Ev = 0, Tcp = 0;
// 	if (!CL500A_COMM->Measurement(Ev, Tcp))
// 	{
// 		WRITE_ERR_LOG(_T("CL-500A measurement failed"));
// 		m_edChromaMeterMeasurementData.SetWindowText(_T("Measurement failed."));
// 		return;
// 	}
// 
// 	CString strData;
// 	strData.Format(_T("Ev: %.4f, Tcp: %.4f"), Ev, Tcp);
// 	m_edChromaMeterMeasurementData.SetWindowText(strData);
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// 	m_edChromaMeterMeasurementData.SetWindowText(_T("Measuring..."));
// 
// 	CHROMA_PACKET pk = {};
// 	if (!CL200A_COMM->GetMeasureData(NUMBER_0, CT_Evxy, pk))
// 	{
// 		WRITE_ERR_LOG(_T("CL-200A measurement failed"));
// 		m_edChromaMeterMeasurementData.SetWindowText(_T("Measurement failed."));
// 		return;
// 	}
// 
// 	CString strData;
// 	strData.Format(_T("Lux Data: %.4f"), atof(pk.cData3));
// 	m_edChromaMeterMeasurementData.SetWindowText(strData);
// #endif
}

void CDlgDeviceConfiguration::LightOn(int index)
{
	CDevice device;
	bool isOn = false;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	isOn = device.LIGHT_Distortion_On(index);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	isOn = device.LIGHT_Distortion_On(index);
#endif

// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	isOn = device.LIGHT_Color_Cal_On(index);
// #elif(SET_INSPECTOR == SYS_DISTORTION)
// 	if (index >= 3)
// 		return;
// 
// 	isOn = device.LIGHT_Distortion_On(index);
// #elif(SET_INSPECTOR == SYS_GHOSTFLARE || \
// 	  SET_INSPECTOR == SYS_HOTPIXEL)
// 	return;
// #elif(SET_INSPECTOR == SYS_SFR_NARROW)
// 	if (index != 0)
// 		return;
// 
// 	isOn = device.LIGHT_SFR_Narrow_On();
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_H || \
// 	  SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	if (index >= 9)
// 		return;
// 
// 	isOn = device.LIGHT_SFR_Wide_On(index + 1);
// #endif

	auto * pActiveButton = isOn ? &m_btnComponent_Light_On : &m_btnComponent_Light_Off;
	auto * pDisableButton = isOn ? &m_btnComponent_Light_Off : &m_btnComponent_Light_On;

	(*pActiveButton)[index].SetFaceColor(COLOR_GREEN1, true);
	(*pDisableButton)[index].SetTextColor(COLOR_BLACK);
	(*pDisableButton)[index].SetFaceColor(RGB(225, 225, 225), true);
}

void CDlgDeviceConfiguration::LightOff(int index)
{
	CDevice device;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	device.LIGHT_Distortion_Off(index);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	device.LIGHT_Distortion_Off(index);
#endif
// 	auto EqpType = SET_INSPECTOR;
// 
// 	switch (EqpType)
// 	{
// 	case SYS_COLOR_CAL:
// 		device.LIGHT_Color_Cal_Off(index);
// 		break;
// 
// 	case SYS_DISTORTION:
// 		device.LIGHT_Distortion_Off(index);
// 		break;
// 	case SYS_GHOSTFLARE:
// 	case SYS_HOTPIXEL:
// 	{
// 		return;
// 	}
// 		break;
// 
// 	case SYS_SFR_NARROW:
// 		device.LIGHT_SFR_Narrow_Off();
// 		break;
// 
// 	case SYS_SFR_WIDE_H:
// 	case SYS_SFR_WIDE_V:
// 	{
// 		if (index >= 9)
// 			return;
// 
// 		device.LIGHT_SFR_Wide_Off(index + 1);
// 	}
// 		break;
// 
// 	default:
// 		break;
// 	}

// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	device.LIGHT_Color_Cal_Off(index);
// #elif(SET_INSPECTOR == SYS_DISTORTION)
// 	if (index >= 3)
// 		return;
// 
// 	device.LIGHT_Distortion_Off(index);
// #elif(SET_INSPECTOR == SYS_GHOSTFLARE || \
// 	  SET_INSPECTOR == SYS_HOTPIXEL)
// 	return;
// #elif(SET_INSPECTOR == SYS_SFR_NARROW)
// 	if (index != 0)
// 		return;
// 
// 	device.LIGHT_SFR_Narrow_Off();
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_H || \
// 	  SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	if (index >= 9)
// 		return;
// 
// 	device.LIGHT_SFR_Wide_Off(index + 1);
// #endif

	m_btnComponent_Light_On[index].SetTextColor(COLOR_BLACK);
	m_btnComponent_Light_On[index].SetFaceColor(RGB(225, 225, 225), true);
	m_btnComponent_Light_Off[index].SetFaceColor(COLOR_GREEN1, true);
}
