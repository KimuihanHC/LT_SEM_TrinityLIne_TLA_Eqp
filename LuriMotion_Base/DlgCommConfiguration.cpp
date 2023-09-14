// CDlgCommConfiguration.cpp: 구현 파일
//

#include "pch.h"
#include "LuriMotion_Base.h"
#include "DlgCommConfiguration.h"
#include "afxdialogex.h"
#include "SerialParam.h"
#include "PComm.h"
#include "DlgLog.h"

// CDlgCommConfiguration 대화 상자

IMPLEMENT_DYNAMIC(CDlgCommConfiguration, CDialogEx)

CDlgCommConfiguration::CDlgCommConfiguration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_COMMUNICATION_CONFIGURATION, pParent)
{

}

CDlgCommConfiguration::~CDlgCommConfiguration()
{

}

void CDlgCommConfiguration::SaveConfiguration()
{
	// Save
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	CString strIp = _T("");
	CString strPort = _T("");
	CString strTemp = _T("");
	BYTE byIp[4] = { 0x00, };

	DWORD dwIp = 0;

	for (int i = 0; i < 4; i++)
	{
		m_lanIpCtrl[i].GetAddress(byIp[0], byIp[1], byIp[2], byIp[3]);
		strTemp.Format(_T("%d.%d.%d.%d"), byIp[0], byIp[1], byIp[2], byIp[3]);
		stTester.strTesterConnect[i] = strTemp;

		m_edComponent_Lan_Port[i].GetWindowText(strTemp);
		stTester.nTesterPort[i] = _ttoi(strTemp);

		m_lanIpCtrl[i].GetAddress(dwIp);
		stTester.dwTesterIP[i] = dwIp;

		stTester.nServerMode[i] = 1;
	}

	int index = 0;
	strTemp = _T("");
	int nTempData = 0;

	for (int i = 0; i < 7; i++)
	{
		stTester.nPort[i] = m_cbSerial_Comport[i].GetCurSel();
		stTester.nBaudRate[i] = m_cbSerial_Baudrate[i].GetCurSel();
		stTester.nDataBit[i] = m_cbSerial_DataBit[i].GetCurSel();
		stTester.nParityBit[i] = m_cbSerial_ParityBit[i].GetCurSel();
		stTester.nStopBit[i] = m_cbSerial_StopBit[i].GetCurSel();
	}

	DATAMANAGER->SetTesterCommData(stTester);
}

BOOL CDlgCommConfiguration::OnInitDialog()
{
	__super::OnInitDialog();

	InitComponent_Component();
	InspectorCtrlSetting();

	return TRUE;
}

void CDlgCommConfiguration::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_LAN, m_stComponent_Title[0]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_SERIAL, m_stComponent_Title[1]);
	DDX_Control(pDX, IDC_STC_COMPONENT_TITLE_ETC, m_stComponent_Title[2]);

	DDX_Control(pDX, IDC_STC_LAN_CONNECT_TESTER1, m_stComponent_Lan_Sub[0]);
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_TESTER2, m_stComponent_Lan_Sub[1]);
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_RFID, m_stComponent_Lan_Sub[2]);
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_LOGISTIC_MANAGER, m_stComponent_Lan_Sub[3]);

	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_TESTER1, m_stComponent_Lan_Ip[0]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_TESTER2, m_stComponent_Lan_Ip[1]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_RFID, m_stComponent_Lan_Ip[2]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_LOGISTIC_MANAGER, m_stComponent_Lan_Ip[3]);

	DDX_Control(pDX, IDC_IPADDRESS_LAN_TESTER1, m_lanIpCtrl[0]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_TESTER2, m_lanIpCtrl[1]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_RFID, m_lanIpCtrl[2]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_LOGISTIC_MANAGER, m_lanIpCtrl[3]);

	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_TESTER1, m_edComponent_Lan_Port[0]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_TESTER2, m_edComponent_Lan_Port[1]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_RFID, m_edComponent_Lan_Port[2]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_LOGISTIC_MANAGER, m_edComponent_Lan_Port[3]);

	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_TESTER1, m_btnComponent_Lan_Connect[0]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_TESTER2, m_btnComponent_Lan_Connect[1]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_RFID, m_btnComponent_Lan_Connect[2]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_LOGISTIC_MANAGER, m_btnComponent_Lan_Connect[3]);

	DDX_Control(pDX, IDC_BTN_DISCONNECT_TESTER1, m_btnComponent_Lan_DisConnect[0]);
	DDX_Control(pDX, IDC_BTN_DISCONNECT_TESTER2, m_btnComponent_Lan_DisConnect[1]);
	DDX_Control(pDX, IDC_BTN_DISCONNECT_RFID, m_btnComponent_Lan_DisConnect[2]);
	DDX_Control(pDX, IDC_BTN_DISCONNECT_LOGISTIC_MANAGER, m_btnComponent_Lan_DisConnect[3]);

	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_POWER_SUPPLY, m_stComponent_Serial_Sub[0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_LIGHT_CONTROLLER_1, m_stComponent_Serial_Sub[1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_LIGHT_CONTROLLER_2, m_stComponent_Serial_Sub[2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_LIGHT_CONTROLLER_3, m_stComponent_Serial_Sub[3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_BARCODE_1, m_stComponent_Serial_Sub[4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_BARCODE_2, m_stComponent_Serial_Sub[5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_CL_200A, m_stComponent_Serial_Sub[6]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_TITLE_CL_500A, m_stComponent_Serial_Sub[7]);

	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_1, m_stComponent_Serial_Controller[0][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_2, m_stComponent_Serial_Controller[1][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_3, m_stComponent_Serial_Controller[2][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_4, m_stComponent_Serial_Controller[3][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_5, m_stComponent_Serial_Controller[4][0]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_6, m_stComponent_Serial_Controller[0][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_7, m_stComponent_Serial_Controller[1][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_8, m_stComponent_Serial_Controller[2][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_9, m_stComponent_Serial_Controller[3][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_10, m_stComponent_Serial_Controller[4][1]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_11, m_stComponent_Serial_Controller[0][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_12, m_stComponent_Serial_Controller[1][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_13, m_stComponent_Serial_Controller[2][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_14, m_stComponent_Serial_Controller[3][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_15, m_stComponent_Serial_Controller[4][2]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_16, m_stComponent_Serial_Controller[0][3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_17, m_stComponent_Serial_Controller[1][3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_18, m_stComponent_Serial_Controller[2][3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_19, m_stComponent_Serial_Controller[3][3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_20, m_stComponent_Serial_Controller[4][3]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_21, m_stComponent_Serial_Controller[0][4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_22, m_stComponent_Serial_Controller[1][4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_23, m_stComponent_Serial_Controller[2][4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_24, m_stComponent_Serial_Controller[3][4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_25, m_stComponent_Serial_Controller[4][4]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_26, m_stComponent_Serial_Controller[0][5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_27, m_stComponent_Serial_Controller[1][5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_28, m_stComponent_Serial_Controller[2][5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_29, m_stComponent_Serial_Controller[3][5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_30, m_stComponent_Serial_Controller[4][5]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_31, m_stComponent_Serial_Controller[0][6]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_32, m_stComponent_Serial_Controller[1][6]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_33, m_stComponent_Serial_Controller[2][6]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_34, m_stComponent_Serial_Controller[3][6]);
	DDX_Control(pDX, IDC_STC_SERIAL_CONNECT_35, m_stComponent_Serial_Controller[4][6]);

	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_POWER_SUPPLY, m_btnComponent_Serial_DisConnect[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_1, m_btnComponent_Serial_DisConnect[1]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_2, m_btnComponent_Serial_DisConnect[2]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_3, m_btnComponent_Serial_DisConnect[3]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_BARCODE_1, m_btnComponent_Serial_DisConnect[4]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_BARCODE_2, m_btnComponent_Serial_DisConnect[5]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_CL_200A, m_btnComponent_Serial_DisConnect[6]);
	DDX_Control(pDX, IDC_BTN_SERIAL_DISCONNECT_CL_500A, m_btnComponent_Serial_DisConnect[7]);

	DDX_Control(pDX, IDC_BTN_SERIAL_MEASURE_CL_200A, m_btnComponent_Measure_CL_200A);
	DDX_Control(pDX, IDC_BTN_SERIAL_MEASURE_CL_500A, m_btnComponent_Measure_CL_500A);

	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_POWER_SUPPLY, m_btnComponent_Serial_Connect[0]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_1, m_btnComponent_Serial_Connect[1]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_2, m_btnComponent_Serial_Connect[2]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_3, m_btnComponent_Serial_Connect[3]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_BARCODE_1, m_btnComponent_Serial_Connect[4]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_BARCODE_2, m_btnComponent_Serial_Connect[5]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_CL_200A, m_btnComponent_Serial_Connect[6]);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_CL_500A, m_btnComponent_Serial_Connect[7]);

	DDX_Control(pDX, IDC_COMBO_SERIAL_1, m_cbSerial_Comport[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_2, m_cbSerial_Baudrate[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_3, m_cbSerial_DataBit[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_4, m_cbSerial_ParityBit[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_5, m_cbSerial_StopBit[0]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_6, m_cbSerial_Comport[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_7, m_cbSerial_Baudrate[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_8, m_cbSerial_DataBit[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_9, m_cbSerial_ParityBit[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_10, m_cbSerial_StopBit[1]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_11, m_cbSerial_Comport[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_12, m_cbSerial_Baudrate[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_13, m_cbSerial_DataBit[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_14, m_cbSerial_ParityBit[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_15, m_cbSerial_StopBit[2]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_16, m_cbSerial_Comport[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_17, m_cbSerial_Baudrate[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_18, m_cbSerial_DataBit[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_19, m_cbSerial_ParityBit[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_20, m_cbSerial_StopBit[3]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_21, m_cbSerial_Comport[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_22, m_cbSerial_Baudrate[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_23, m_cbSerial_DataBit[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_24, m_cbSerial_ParityBit[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_25, m_cbSerial_StopBit[4]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_26, m_cbSerial_Comport[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_27, m_cbSerial_Baudrate[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_28, m_cbSerial_DataBit[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_29, m_cbSerial_ParityBit[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_30, m_cbSerial_StopBit[5]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_31, m_cbSerial_Comport[6]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_32, m_cbSerial_Baudrate[6]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_33, m_cbSerial_DataBit[6]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_34, m_cbSerial_ParityBit[6]);
	DDX_Control(pDX, IDC_COMBO_SERIAL_35, m_cbSerial_StopBit[6]);

#if (20230905)
	DDX_Control(pDX, IDC_STC_LAN_CONNECT_COMMON1, m_stComponent_Lan_Common_Sub[IP_COMMON_SCREW_LEFT]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_COMMON1, m_stComponent_Lan_Common_Ip[IP_COMMON_SCREW_LEFT]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_COMMON1, m_btnComponent_Lan_Common_Connect[IP_COMMON_SCREW_LEFT]);
	DDX_Control(pDX, IDC_BTN_LAN_DISCONNECT_COMMON1, m_btnComponent_Lan_Common_DisConnect[IP_COMMON_SCREW_LEFT]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_COMMON1, m_lanIpCommonCtrl[IP_COMMON_SCREW_LEFT]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_COMMON1, m_edComponent_Lan_Common_Port[IP_COMMON_SCREW_LEFT]);


	DDX_Control(pDX, IDC_STC_LAN_CONNECT_COMMON2, m_stComponent_Lan_Common_Sub[IP_COMMON_SCREW_RIGHT]);
	DDX_Control(pDX, IDC_STC_CONNECT_LAN_IP_COMMON2, m_stComponent_Lan_Common_Ip[IP_COMMON_SCREW_RIGHT]);
	DDX_Control(pDX, IDC_BTN_LAN_CONNECT_COMMON2, m_btnComponent_Lan_Common_Connect[IP_COMMON_SCREW_RIGHT]);
	DDX_Control(pDX, IDC_BTN_LAN_DISCONNECT_COMMON2, m_btnComponent_Lan_Common_DisConnect[IP_COMMON_SCREW_RIGHT]);
	DDX_Control(pDX, IDC_IPADDRESS_LAN_COMMON2, m_lanIpCommonCtrl[IP_COMMON_SCREW_RIGHT]);
	DDX_Control(pDX, IDC_EDIT_LAN_PORT_INPUT_COMMON2, m_edComponent_Lan_Common_Port[IP_COMMON_SCREW_RIGHT]);
#endif
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	DDX_Control(pDX, IDC_ED_CL_500A_MEASUREMENT_DATA, m_edCL500AMeasurementData);
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// 	DDX_Control(pDX, IDC_ED_CL_200A_MEASUREMENT_DATA, m_edCL200AMeasurementData);
// #endif
}


BOOL CDlgCommConfiguration::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgCommConfiguration::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(84, 169, 169));
	return TRUE;
}

void CDlgCommConfiguration::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		Load_ComponentDlg();
}

void CDlgCommConfiguration::ChangeUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	CRect rt;

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szCommConfigurationTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szCommConfigurationTitle_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Lan_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Lan_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Kor[i]);
		else
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Lan_Ip[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Ip[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Lan_Ip[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Lan_Ip[i].SetText(_T("IP"));
	}

	for (int i = 0; i < 4; i++)
	{
		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Lan_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Lan_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	for (int i = 0; i < 8; i++)
	{
		m_stComponent_Serial_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Serial_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Kor[i]);
		else
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Eng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			m_stComponent_Serial_Controller[i][j].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stComponent_Serial_Controller[i][j].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_stComponent_Serial_Controller[i][j].SetFont_Gdip(L"Arial", 9.0F);
			m_stComponent_Serial_Controller[i][j].SetText(m_szComponentSerial_Param[i]);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	if (stBasic.nLanguage == 0)
	{
		m_btnComponent_Measure_CL_200A.SetWindowText(_T("측정"));
		m_btnComponent_Measure_CL_500A.SetWindowText(_T("측정"));
	}
	else
	{
		m_btnComponent_Measure_CL_200A.SetWindowText(_T("Measure"));
		m_btnComponent_Measure_CL_500A.SetWindowText(_T("Measure"));
	}
#if (20230905)
	ChangeCommonUI_Language();
#endif
	UpdateData(FALSE);
}

void CDlgCommConfiguration::ChangeUi_Lan()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	CString strTemp = _T("");
	CString strPart = _T("");
	BYTE byIp[4] = { 0x00, };
	TCHAR tchSep = _T('.');

	for (int i = 0; i < 4; i++)
	{
		strTemp.Format(_T("%s"), stTester.strTesterConnect[i]);
		for (int j = 0; j < 4; j++)
		{
			AfxExtractSubString(strPart, strTemp, j, tchSep);
			byIp[j] = _ttoi(strPart);

		}
		m_lanIpCtrl[i].SetAddress(byIp[0], byIp[1], byIp[2], byIp[3]);

		strTemp.Format(_T("%d"), stTester.nTesterPort[i]);
		m_edComponent_Lan_Port[i].SetWindowText(strTemp);
	}
#if (20230905)
	ChangeCommonUi_Lan();
#endif
	UpdateData(FALSE);
}

void CDlgCommConfiguration::ChangeUi_Serial()
{
	TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();

	for (int i = 0; i < 7; i++)
	{
		m_cbSerial_Comport[i].SetCurSel(stTester.nPort[i]);
		m_cbSerial_Baudrate[i].SetCurSel(stTester.nBaudRate[i]);
		m_cbSerial_DataBit[i].SetCurSel(stTester.nDataBit[i]);
		m_cbSerial_ParityBit[i].SetCurSel(stTester.nParityBit[i]);
		m_cbSerial_StopBit[i].SetCurSel(stTester.nStopBit[i]);
	}

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDlgCommConfiguration, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()

	// Lan
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_TESTER1, &CDlgCommConfiguration::OnBnClickedBtnLanConnectTester1)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_TESTER1, &CDlgCommConfiguration::OnBnClickedBtnDisconnectTester1)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_TESTER2, &CDlgCommConfiguration::OnBnClickedBtnLanConnectTester2)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_TESTER2, &CDlgCommConfiguration::OnBnClickedBtnDisconnectTester2)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_RFID, &CDlgCommConfiguration::OnBnClickedBtnLanConnectRFID)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_RFID, &CDlgCommConfiguration::OnBnClickedBtnDisconnectRFID)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_LOGISTIC_MANAGER, &CDlgCommConfiguration::OnBnClickedBtnLanConnectLogisticManager)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_LOGISTIC_MANAGER, &CDlgCommConfiguration::OnBnClickedBtnDisconnectLogisticManager)

	// Serial
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_POWER_SUPPLY, &CDlgCommConfiguration::OnBnClickedBtnSerialConnectPowerSupply)
	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_POWER_SUPPLY, &CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectPowerSupply)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_1, &CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_1, &CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_2, &CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_2, &CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_3, &CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_3, &CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController3)
#if (20230905)
	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_COMMON1, &CDlgCommConfiguration::OnBnClickedBtnLanConnectCommon1)
	ON_BN_CLICKED(IDC_BTN_LAN_DISCONNECT_COMMON1, &CDlgCommConfiguration::OnBnClickedBtnDisconnectCommon1)
	ON_BN_CLICKED(IDC_BTN_LAN_SUB_COMMON_PSET1, &CDlgCommConfiguration::OnBnClickedBtnLanSubCommonPset1)
	ON_BN_CLICKED(IDC_BTN_LAN_SUB_COMMON_TIGHTERNING_RESULT1, &CDlgCommConfiguration::OnBnClickedBtnLanSubCommonTeghteringResult1)

	ON_BN_CLICKED(IDC_BTN_LAN_CONNECT_COMMON2, &CDlgCommConfiguration::OnBnClickedBtnLanConnectCommon2)
	ON_BN_CLICKED(IDC_BTN_LAN_DISCONNECT_COMMON2, &CDlgCommConfiguration::OnBnClickedBtnLanDisconnectCommon2)
	ON_BN_CLICKED(IDC_BTN_LAN_SUB_COMMON_PSET2, &CDlgCommConfiguration::OnBnClickedBtnLanSubCommonPSet2)
#endif
	// Etc
// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_CL_500A, &CDlgCommConfiguration::OnBnClickedBtnCL500AConnect)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_CL_500A, &CDlgCommConfiguration::OnBnClickedBtnCL500ADisconnect)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_MEASURE_CL_500A, &CDlgCommConfiguration::OnBnClickedBtnCL500AMeasure)
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_MEASURE_CL_200A, &CDlgCommConfiguration::OnBnClickedBtnCL200AMeasure)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_CL_200A, &CDlgCommConfiguration::OnBnClickedBtnCL200AConnect)
// 	ON_BN_CLICKED(IDC_BTN_SERIAL_DISCONNECT_CL_200A, &CDlgCommConfiguration::OnBnClickedBtnCL200ADisconnect)
// #endif




ON_BN_CLICKED(IDC_BTN_LAN_SUB_COMMON_TIGHTERNING_RESULT2, &CDlgCommConfiguration::OnBnClickedBtnLanSubCommonTighterningResult2)
END_MESSAGE_MAP()

void CDlgCommConfiguration::InitComponent_Component()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	CRect rt;

	for (int i = 0; i < 3; i++)
	{
		m_stComponent_Title[i].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_stComponent_Title[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Title[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Title[i].SetText(m_szCommConfigurationTitle_Kor[i]);
		else
			m_stComponent_Title[i].SetText(m_szCommConfigurationTitle_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Lan_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Lan_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Kor[i]);
		else
			m_stComponent_Lan_Sub[i].SetText(m_szComponentLan_Sub_Eng[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_stComponent_Lan_Ip[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Ip[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Lan_Ip[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Lan_Ip[i].SetText(_T("IP"));
	}

	for (int i = 0; i < 4; i++)
	{
		m_btnComponent_Lan_Connect[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Lan_Connect[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Lan_Connect[i].m_bTransparent = false;
		m_btnComponent_Lan_Connect[i].SetTextColor(COLOR_BLACK);
		m_btnComponent_Lan_Connect[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Lan_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		m_btnComponent_Lan_DisConnect[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Lan_DisConnect[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Lan_DisConnect[i].m_bTransparent = false;
		m_btnComponent_Lan_DisConnect[i].SetTextColor(COLOR_BLACK);
		m_btnComponent_Lan_DisConnect[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Lan_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	for (int i = 0; i < 8; i++)
	{
		m_stComponent_Serial_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Serial_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Serial_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Kor[i]);
		else
			m_stComponent_Serial_Sub[i].SetText(m_szComponentSerialSub_Eng[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			m_stComponent_Serial_Controller[i][j].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
			m_stComponent_Serial_Controller[i][j].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
			m_stComponent_Serial_Controller[i][j].SetFont_Gdip(L"Arial", 9.0F);
			m_stComponent_Serial_Controller[i][j].SetText(m_szComponentSerial_Param[i]);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		m_btnComponent_Serial_Connect[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Serial_Connect[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Serial_Connect[i].m_bTransparent = false;
		m_btnComponent_Serial_Connect[i].SetTextColor(COLOR_BLACK);
		m_btnComponent_Serial_Connect[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Serial_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		m_btnComponent_Serial_DisConnect[i].EnableWindowsTheming(FALSE);
		m_btnComponent_Serial_DisConnect[i].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_btnComponent_Serial_DisConnect[i].m_bTransparent = false;
		m_btnComponent_Serial_DisConnect[i].SetTextColor(COLOR_BLACK);
		m_btnComponent_Serial_DisConnect[i].SetFaceColor(RGB(225, 225, 225), true);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Serial_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}

	m_btnComponent_Measure_CL_200A.EnableWindowsTheming(FALSE);
	m_btnComponent_Measure_CL_200A.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnComponent_Measure_CL_200A.m_bTransparent = false;
	m_btnComponent_Measure_CL_200A.SetTextColor(COLOR_BLACK);
	m_btnComponent_Measure_CL_200A.SetFaceColor(RGB(225, 225, 225), true);

	m_btnComponent_Measure_CL_500A.EnableWindowsTheming(FALSE);
	m_btnComponent_Measure_CL_500A.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_btnComponent_Measure_CL_500A.m_bTransparent = false;
	m_btnComponent_Measure_CL_500A.SetTextColor(COLOR_BLACK);
	m_btnComponent_Measure_CL_500A.SetFaceColor(RGB(225, 225, 225), true);

	// SERIAL
	for (int i = 0; i < 7; i++)
	{
		m_cbSerial_Comport[i].ResetContent();
		m_cbSerial_Baudrate[i].ResetContent();
		m_cbSerial_DataBit[i].ResetContent();
		m_cbSerial_ParityBit[i].ResetContent();
		m_cbSerial_StopBit[i].ResetContent();
	}

	COMMCONFIG lpCC;
	CString str, str1;
	unsigned long ccsize = sizeof(COMMCONFIG);
	int index = 0;
	BOOL bExistFlag = FALSE;

	// Port
	for (int i = 1; i < 15; i++)
	{
		str = _T("COM");
		str1.Format(_T("%d"), i);
		str += str1;
		bExistFlag = FALSE;

		// 중요 포인트 return값이 TRUE이면 COMPORT존재
		bExistFlag = GetDefaultCommConfig(str, &lpCC, &ccsize);

		for (int i = 0; i < 7; i++)
		{
			index = m_cbSerial_Comport[i].AddString(str);
			m_cbSerial_Comport[i].SetItemData(index, i);
		}
	}

	//Baudrate
	for (int i = 0; i < 5; i++)
	{
		str.Format(_T("%s"), BaudList[i]);
		for (int i = 0; i < 7; i++)
		{
			index = m_cbSerial_Baudrate[i].AddString(str);
			m_cbSerial_Baudrate[i].SetItemData(index, i);
		}
	}

	//Data bit
	for (int i = 0; i < 2; i++)
	{
		str.Format(_T("%s"), DataBitList[i]);
		for (int i = 0; i < 7; i++)
		{
			index = m_cbSerial_DataBit[i].AddString(str);
			m_cbSerial_DataBit[i].SetItemData(index, i);
		}
	}

	//Parity bit
	for (int i = 0; i < 3; i++)
	{
		str.Format(_T("%s"), ParityList[i]);
		for (int i = 0; i < 7; i++)
		{
			index = m_cbSerial_ParityBit[i].AddString(str);
			m_cbSerial_ParityBit[i].SetItemData(index, i);
		}
	}

	//Stop bit
	for (int i = 0; i < 2; i++)
	{
		str.Format(_T("%s"), StopBitList[i]);
		for (int i = 0; i < 7; i++)
		{
			index = m_cbSerial_StopBit[i].AddString(str);
			m_cbSerial_StopBit[i].SetItemData(index, i);
		}
	}

	ChangeUI_Language();
}

void CDlgCommConfiguration::Load_ComponentDlg()
{
	ChangeUi_Lan();
	ChangeUi_Serial();
}

void CDlgCommConfiguration::InspectorCtrlSetting()
{
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE) \
||(SET_INSPECTOR == SYS_TLA_OQC)
	GetDlgItem(IDC_STC_SERIAL_CONNECT_TITLE_LIGHT_CONTROLLER_2)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_STC_SERIAL_CONNECT_TITLE_LIGHT_CONTROLLER_3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STC_SERIAL_CONNECT_11)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STC_SERIAL_CONNECT_12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STC_SERIAL_CONNECT_13)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STC_SERIAL_CONNECT_14)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STC_SERIAL_CONNECT_15)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STC_SERIAL_CONNECT_16)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STC_SERIAL_CONNECT_17)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STC_SERIAL_CONNECT_18)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STC_SERIAL_CONNECT_19)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STC_SERIAL_CONNECT_20)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_SERIAL_11)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_SERIAL_12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_SERIAL_13)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_SERIAL_14)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_SERIAL_15)->ShowWindow(SW_SHOW);

// 	GetDlgItem(IDC_COMBO_SERIAL_16)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_COMBO_SERIAL_17)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_COMBO_SERIAL_18)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_COMBO_SERIAL_19)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_COMBO_SERIAL_20)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_2)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_BTN_SERIAL_CONNECT_LIGHT_CONTROLLER_3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_2)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_BTN_SERIAL_DISCONNECT_LIGHT_CONTROLLER_3)->ShowWindow(SW_SHOW);
#endif
}

void CDlgCommConfiguration::OnBnClickedBtnLanConnectTester1()
{
	UpdateData(TRUE);

	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_UdpTester[0].m_bConnected)
	{
		//m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
	}
	else
	{
// 		m_UdpTester[0].Thread_Stop();
// 		COMMON->DoEvent(100);
// 		m_UdpTester[0].Thread_Start();
// 		COMMON->DoEvent(100);
// 		m_UdpTester[0].InitVariable(0);
// 		COMMON->DoEvent(100);
		m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);

		COMMON->DoEvent(100);
		//m_UdpTester[0].Send_Tester_Communication(eComMode_Ready);
		m_UdpTester[0].Send_Tester_Communication(eComMode_PreviewStop);

		m_UdpTester[0].SetOwnerHWND(this->m_hWnd);


		m_UdpTester[0].SetWmRecv(UM_RECV_SEMCO_LIGHT_CONTROL);

	}
// 	if (stTester.nUseMode[0] == FALSE)
// 	{
// 		m_ctrlOthers[0].CloseSocket();
// 		Sleep(100);
// 		//if (m_ctrlOthers[i].Connect(stTester.strTesterConnect_Single[i], stTester.nTesterPort_Single[i]) == FALSE)
// 		m_ctrlOthers[0].CreateSocket(stTester.dwTesterIP[0], stTester.nTesterPort[0], 1);
// 
// 
// 		if (COMMON->Wait_Time(2000, &m_ctrlOthers[0].m_bConnected) == FALSE)
// 		{
// 			COMMON->m_bTesterConnectFlag[0] = FALSE;
// 
// 			strTemp.Format(_T("IP[%s],Port[%d] Connect Fail. "), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
// 			//WRITE_COM_FILE(_COMM_LOG, strTemp);
// 			WRITE_COMM_LOG(strTemp);
// 		}
// 		else
// 		{
// 			COMMON->m_bTesterConnectFlag[0] = TRUE;
// 
// 			strTemp.Format(_T("IP[%s],Port[%d] Connect Success."), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
// 			//WRITE_COM_FILE(_COMM_LOG, strTemp);
// 			WRITE_COMM_LOG(strTemp);
// 		}
// 	}
}

void CDlgCommConfiguration::OnBnClickedBtnDisconnectTester1()
{
	UpdateData(TRUE);
	m_UdpTester[0].CloseSocket();
	return;
	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	int nCount = 0;

		
	if (stTester.nUseMode[0] == TRUE)
	{
		m_ctrlOthers[0].CloseSocket();

		strTemp.Format(_T("IP[%s],Port[%d] Disconnect"), stTester.strTesterConnect[0], stTester.nTesterPort[0]);
		//WRITE_COM_FILE(_COMM_LOG, strTemp);
		WRITE_COMM_LOG(strTemp);
	}
}

void CDlgCommConfiguration::OnBnClickedBtnLanConnectTester2()
{
	UpdateData(TRUE);

	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (stTester.nUseMode[1] == TRUE)
	{
		m_ctrlOthers[1].CloseSocket();
		Sleep(100);
		//if (m_ctrlOthers[i].Connect(stTester.strTesterConnect_Single[i], stTester.nTesterPort_Single[i]) == FALSE)
		m_ctrlOthers[1].CreateSocket(stTester.dwTesterIP[1], stTester.nTesterPort[1], 1);


		if (COMMON->Wait_Time(2000, &m_ctrlOthers[1].m_bConnected) == FALSE)
		{
			COMMON->m_bTesterConnectFlag[1] = FALSE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Fail. "), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
		else
		{
			COMMON->m_bTesterConnectFlag[1] = TRUE;

			strTemp.Format(_T("IP[%s],Port[%d] Connect Success."), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
			//WRITE_COM_FILE(_COMM_LOG, strTemp);
			WRITE_COMM_LOG(strTemp);
		}
	}
}

void CDlgCommConfiguration::OnBnClickedBtnDisconnectTester2()
{
	UpdateData(TRUE);

	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (stTester.nUseMode[1] == TRUE)
	{
		m_ctrlOthers[1].CloseSocket();

		strTemp.Format(_T("IP[%s],Port[%d] Disconnect"), stTester.strTesterConnect[1], stTester.nTesterPort[1]);
		//WRITE_COM_FILE(_COMM_LOG, strTemp);
		WRITE_COMM_LOG(strTemp);
	}
}

void CDlgCommConfiguration::OnBnClickedBtnLanConnectLogisticManager()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

}

void CDlgCommConfiguration::OnBnClickedBtnLanConnectRFID()
{
	UpdateData(TRUE);

	CDevice device;
	device.RFID_Connect();
}

void CDlgCommConfiguration::OnBnClickedBtnDisconnectRFID()
{
	UpdateData(TRUE);

	CDevice device;
	device.RFID_Disconnect();
}

void CDlgCommConfiguration::OnBnClickedBtnDisconnectLogisticManager()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

}

void CDlgCommConfiguration::OnBnClickedBtnSerialConnectPowerSupply()
{
	UpdateData(TRUE);

	CDevice device;
	device.PowerSupply_Connect();
}

void CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectPowerSupply()
{
	UpdateData(TRUE);

	CDevice device;
	device.PowerSupply_Disconnect();
}

void CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController1()
{
	UpdateData(TRUE);

	CDevice device;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	device.LIGHT_Distortion_Connect(0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	device.LIGHT_Distortion_Connect(0);
#endif


// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	device.LIGHT_Color_Cal_Connect();
// #elif(SET_INSPECTOR == SYS_DISTORTION)
// 	device.LIGHT_Distortion_Connect(0);
// #elif(SET_INSPECTOR == SYS_GHOSTFLARE)
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// #elif(SET_INSPECTOR == SYS_SFR_NARROW)
// 	device.LIGHT_SFR_Narrow_Connect();
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_H || \
// 	  SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	device.LIGHT_SFR_Wide_Connect();
// #endif
}

void CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController1()
{
	UpdateData(TRUE);

	CDevice device;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	device.LIGHT_Distortion_Disconnect(0);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	device.LIGHT_Distortion_Disconnect(0);
#endif

// #if(SET_INSPECTOR == SYS_COLOR_CAL)
// 	device.LIGHT_Color_Cal_Disconnect();
// #elif(SET_INSPECTOR == SYS_DISTORTION)
// 	device.LIGHT_Distortion_Disconnect(0);
// #elif(SET_INSPECTOR == SYS_GHOSTFLARE)
// #elif(SET_INSPECTOR == SYS_HOTPIXEL)
// #elif(SET_INSPECTOR == SYS_SFR_NARROW)
// 	device.LIGHT_SFR_Narrow_Disconnect();
// #elif(SET_INSPECTOR == SYS_SFR_WIDE_H || \
// 	  SET_INSPECTOR == SYS_SFR_WIDE_V)
// 	device.LIGHT_SFR_Wide_Disconnect();
// #endif
}

void CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController2()
{
	UpdateData(TRUE);

	CDevice device;

#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	device.LIGHT_Distortion_Connect(1);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	device.LIGHT_Distortion_Connect(1);
#endif
}

void CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController2()
{
	UpdateData(TRUE);

	CDevice device;
#if(SET_INSPECTOR == SYS_TLA_ASSEMBLE)
	device.LIGHT_Distortion_Disconnect(1);
#elif(SET_INSPECTOR == SYS_TLA_OQC)
	device.LIGHT_Distortion_Disconnect(1);
#endif
// #if(SET_INSPECTOR == SYS_DISTORTION)
// 	device.LIGHT_Distortion_Disconnect(1);
// #endif
}

void CDlgCommConfiguration::OnBnClickedBtnSerialConnectLightController3()
{
	UpdateData(TRUE);

	CDevice device;

// #if(SET_INSPECTOR == SYS_DISTORTION)
// 	device.LIGHT_Distortion_Connect(2);
// #endif
}

void CDlgCommConfiguration::OnBnClickedBtnSerialDisconnectLightController3()
{
	UpdateData(TRUE);

	CDevice device;

// #if(SET_INSPECTOR == SYS_DISTORTION)
// 	device.LIGHT_Distortion_Disconnect(2);
// #endif
}

// #if(SET_INSPECTOR == SYS_HOTPIXEL)
// void CDlgCommConfiguration::OnBnClickedBtnCL200AMeasure()
// {
// 	UpdateData(TRUE);
// 
// 	m_edCL200AMeasurementData.SetWindowText(_T("Measuring..."));
// 
// // 	CHROMA_PACKET pk = {};
// // 	if (!CL200A_COMM->GetMeasureData(NUMBER_0, CT_Evxy, pk))
// // 	{
// // 		WRITE_ERR_LOG(_T("CL-200A measurement failed"));
// // 		m_edCL200AMeasurementData.SetWindowText(_T("Measurement failed."));
// // 		return;
// // 	}
// 
// // 	CString strData;
// // 	strData.Format(_T("Lux Data: %.4f"), atof(pk.cData3));
// // 	m_edCL200AMeasurementData.SetWindowText(strData);
// }
// 
// void CDlgCommConfiguration::OnBnClickedBtnCL200AConnect()
// {
// 	UpdateData(TRUE);
// 
// 	CDevice device;
// 	if (!device.CL200A_HOTPIXEL_Connect())
// 		return;
// 
// 	static constexpr auto CL200A_INDEX = 6;
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetTextColor(COLOR_BLACK);
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// void CDlgCommConfiguration::OnBnClickedBtnCL200ADisconnect()
// {
// 	UpdateData(TRUE);
// 
// 	CDevice device;
// 	device.CL200A_HOTPIXEL_Disconnect();
// 
// 	static constexpr auto CL200A_INDEX = 6;
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetTextColor(COLOR_BLACK);
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetFaceColor(RGB(225, 225, 225), true);
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetFaceColor(COLOR_GREEN1, true);
// }
// #elif(SET_INSPECTOR == SYS_COLOR_CAL)
// void CDlgCommConfiguration::OnBnClickedBtnCL500AConnect()
// {
// 	UpdateData(TRUE);
// 
// 	CDevice device;
// 	if (!device.CL500A_COLORCAL_Connect())
// 		return;
// 
// 	static constexpr auto CL200A_INDEX = 7;
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetFaceColor(COLOR_GREEN1, true);
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetTextColor(COLOR_BLACK);
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetFaceColor(RGB(225, 225, 225), true);
// }
// 
// void CDlgCommConfiguration::OnBnClickedBtnCL500ADisconnect()
// {
// 	UpdateData(TRUE);
// 
// 	CDevice device;
// 	device.CL500A_COLORCAL_Disconnect();
// 
// 	static constexpr auto CL200A_INDEX = 7;
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetFaceColor(COLOR_BLACK, true);
// 	m_btnComponent_Serial_Connect[CL200A_INDEX].SetTextColor(RGB(225, 225, 225));
// 	m_btnComponent_Serial_DisConnect[CL200A_INDEX].SetFaceColor(COLOR_GREEN1, true);
// }
// 
// void CDlgCommConfiguration::OnBnClickedBtnCL500AMeasure()
// {
// 	UpdateData(TRUE);
// 
// 	m_edCL500AMeasurementData.SetWindowText(_T("Measuring..."));
// 
// 	float Ev = 0, Tcp = 0;
// // 	if (!CL500A_COMM->Measurement(Ev, Tcp))
// // 	{
// // 		WRITE_ERR_LOG(_T("CL-500A measurement failed"));
// // 		m_edCL500AMeasurementData.SetWindowText(_T("Measurement failed."));
// // 		return;
// // 	}
// 
// 	CString strData;
// 	strData.Format(_T("Ev: %.4f, Tcp: %.4f"), Ev, Tcp);
// 	m_edCL500AMeasurementData.SetWindowText(strData);
// }
// #endif


#if (20230905)
void CDlgCommConfiguration::ChangeCommonUI_Language()
{
	EQ_BASIC_PARAM	stBasic = *DATAMANAGER->GetCurrentEqBasicData();

	CString strTemp = _T("");
	CRect rt;

	for (int i = 0; i < IP_COMMON_MAX; i++)
	{
		m_stComponent_Lan_Common_Sub[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Common_Sub[i].SetColorStyle(CVGStatic::ColorStyle_Turquois);
		m_stComponent_Lan_Common_Sub[i].SetFont_Gdip(L"Arial", 9.0F);

		if (stBasic.nLanguage == 0)
			m_stComponent_Lan_Common_Sub[i].SetText(m_szComponentLan_Common_Sub_Kor[i]);
		else
			m_stComponent_Lan_Common_Sub[i].SetText(m_szComponentLan_Common_Sub_Eng[i]);
	}

	for (int i = 0; i < IP_COMMON_MAX; i++)
	{
		m_stComponent_Lan_Common_Ip[i].SetStaticStyle(CVGStatic::StaticStyle_Title_Sub);
		m_stComponent_Lan_Common_Ip[i].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_stComponent_Lan_Common_Ip[i].SetFont_Gdip(L"Arial", 9.0F);
		m_stComponent_Lan_Common_Ip[i].SetText(_T("IP"));
	}

	for (int i = 0; i < IP_COMMON_MAX; i++)
	{
		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Common_Connect[i].SetWindowText(m_szComponentConnect_Kor[0]);
		else
			m_btnComponent_Lan_Common_Connect[i].SetWindowText(m_szComponentConnect_Eng[0]);

		if (stBasic.nLanguage == 0)
			m_btnComponent_Lan_Common_DisConnect[i].SetWindowText(m_szComponentConnect_Kor[1]);
		else
			m_btnComponent_Lan_Common_DisConnect[i].SetWindowText(m_szComponentConnect_Eng[1]);
	}
}

void CDlgCommConfiguration::ChangeCommonUi_Lan()
{
	//TESTER_COMM	stTester = *DATAMANAGER->GetCurrentTesterCommData();
	CString strTemp = _T("");
	CString strPart = _T("");
	BYTE byIp[4] = { 0x00, };
	TCHAR tchSep = _T('.');

	for (int i = 0; i < IP_COMMON_MAX; i++)
	{
		//strTemp.Format(_T("%s"), stTester.strTesterConnect[i]);
		strTemp = _T("192.168.0.2");
		for (int j = 0; j < 4; j++)
		{
			AfxExtractSubString(strPart, strTemp, j, tchSep);
			byIp[j] = _ttoi(strPart);

		}
		m_lanIpCommonCtrl[i].SetAddress(byIp[0], byIp[1], byIp[2], byIp[3]);

		//strTemp.Format(_T("%d"), stTester.nTesterPort[i]);
		strTemp = _T("4545");
		m_edComponent_Lan_Common_Port[i].SetWindowText(strTemp);
	}

}



void CDlgCommConfiguration::OnBnClickedBtnLanConnectCommon1()
{
	UpdateData(TRUE);

	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_TCPCommon[IP_COMMON_SCREW_LEFT].m_bConnected)
	{
		m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_Communication_Start);
	}
	else
	{
		strTemp = "192.168.0.2";
		DWORD dwTesterIP;
		int nTesterPort = 4545;
		dwTesterIP = (DWORD)_ttof(strTemp);
		CString strTesterConnect;
		m_TCPCommon[IP_COMMON_SCREW_LEFT].Connect_Common(strTemp, nTesterPort);

		COMMON->DoEvent(100);
		m_TCPCommon[IP_COMMON_SCREW_LEFT].InitVariable(IP_COMMON_SCREW_LEFT);
		m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_Communication_Start);

		m_TCPCommon[IP_COMMON_SCREW_LEFT].SetOwnerHWND(this->m_hWnd);
	}
}

void CDlgCommConfiguration::OnBnClickedBtnDisconnectCommon1()
{
	UpdateData(TRUE);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_Communication_Stop);
	m_TCPCommon[IP_COMMON_SCREW_LEFT].CloseSocket();
}
#endif

void CDlgCommConfiguration::OnBnClickedBtnLanSubCommonPset1()
{
	// TODO: Add your control notification handler code here
	if (m_TCPCommon[IP_COMMON_SCREW_LEFT].m_bConnected)
	  m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_DataMessageSubscribe, 15);
}


void CDlgCommConfiguration::OnBnClickedBtnLanSubCommonTeghteringResult1()
{
	// TODO: Add your control notification handler code here
	if (m_TCPCommon[IP_COMMON_SCREW_LEFT].m_bConnected)
		m_TCPCommon[IP_COMMON_SCREW_LEFT].Send_Common_Communication(eScrew_DataMessageSubscribe, 1201);
}


void CDlgCommConfiguration::OnBnClickedBtnLanConnectCommon2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strTemp = _T("");

	TESTER_COMM stTester = *DATAMANAGER->GetCurrentTesterCommData();

	if (m_TCPCommon[IP_COMMON_SCREW_RIGHT].m_bConnected)
	{
		//m_UdpTester[0].Connect_Tester(stTester.dwTesterIP[0], stTester.nTesterPort[0], stTester.strTesterConnect[0], 1);
		m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_Communication_Start);
	}
	else
	{
		strTemp = "192.168.1.2";
		DWORD dwTesterIP;
		int nTesterPort = 4545;
		dwTesterIP = (DWORD)_ttof(strTemp);
		CString strTesterConnect;
		m_TCPCommon[IP_COMMON_SCREW_RIGHT].Connect_Common(strTemp, nTesterPort);

		COMMON->DoEvent(100);
		m_TCPCommon[IP_COMMON_SCREW_RIGHT].InitVariable(IP_COMMON_SCREW_RIGHT);
		m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_Communication_Start);

		m_TCPCommon[IP_COMMON_SCREW_RIGHT].SetOwnerHWND(this->m_hWnd);
	}
}


void CDlgCommConfiguration::OnBnClickedBtnLanDisconnectCommon2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_Communication_Stop);
	m_TCPCommon[IP_COMMON_SCREW_RIGHT].CloseSocket();
}


void CDlgCommConfiguration::OnBnClickedBtnLanSubCommonPSet2()
{
	// TODO: Add your control notification handler code here
	if (m_TCPCommon[IP_COMMON_SCREW_RIGHT].m_bConnected)
		m_TCPCommon[IP_COMMON_SCREW_RIGHT].Send_Common_Communication(eScrew_DataMessageSubscribe, 15);
}


void CDlgCommConfiguration::OnBnClickedBtnLanSubCommonTighterningResult2()
{
	// TODO: Add your control notification handler code here
}
