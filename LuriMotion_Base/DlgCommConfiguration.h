#pragma once

#include "VGbase/VGStatic.h"

// CDlgCommConfiguration 대화 상자

class CDlgCommConfiguration : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommConfiguration)

public:
	CDlgCommConfiguration(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgCommConfiguration();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_COMMUNICATION_CONFIGURATION };
#endif

public:
	void SaveConfiguration();
	void ChangeUI_Language();
	void Load_ComponentDlg();

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void ChangeUi_Lan();
	void ChangeUi_Serial();

	void InitComponent_Component();
	void InspectorCtrlSetting();

	// Lan
	afx_msg void OnBnClickedBtnLanConnectTester1();
	afx_msg void OnBnClickedBtnDisconnectTester1();
	afx_msg void OnBnClickedBtnLanConnectTester2();
	afx_msg void OnBnClickedBtnDisconnectTester2();
	afx_msg void OnBnClickedBtnLanConnectRFID();
	afx_msg void OnBnClickedBtnDisconnectRFID();
	afx_msg void OnBnClickedBtnLanConnectLogisticManager();
	afx_msg void OnBnClickedBtnDisconnectLogisticManager();

	// Serial
	afx_msg void OnBnClickedBtnSerialConnectPowerSupply();
	afx_msg void OnBnClickedBtnSerialDisconnectPowerSupply();
	afx_msg void OnBnClickedBtnSerialConnectLightController1();
	afx_msg void OnBnClickedBtnSerialDisconnectLightController1();
	afx_msg void OnBnClickedBtnSerialConnectLightController2();
	afx_msg void OnBnClickedBtnSerialDisconnectLightController2();
	afx_msg void OnBnClickedBtnSerialConnectLightController3();
	afx_msg void OnBnClickedBtnSerialDisconnectLightController3();

	// Etc
#if(SET_INSPECTOR == SYS_HOTPIXEL)
	CEdit m_edCL200AMeasurementData;
	afx_msg void OnBnClickedBtnCL200AConnect();
	afx_msg void OnBnClickedBtnCL200ADisconnect();
	afx_msg void OnBnClickedBtnCL200AMeasure();
#elif(SET_INSPECTOR == SYS_COLOR_CAL)
	CEdit m_edCL500AMeasurementData;
	afx_msg void OnBnClickedBtnCL500AConnect();
	afx_msg void OnBnClickedBtnCL500ADisconnect();
	afx_msg void OnBnClickedBtnCL500AMeasure();
#endif

private:
	CVGStatic		m_stComponent_Title[3];					// lan & serial title
	CVGStatic		m_stComponent_Lan_Sub[4];				// lan sub title
	CVGStatic		m_stComponent_Lan_Ip[4];				// Ip static
	CMFCButton		m_btnComponent_Lan_Connect[4];			// Lan 통신 연결
	CMFCButton		m_btnComponent_Lan_DisConnect[4];		// Lan 통신 g해제

	CEdit			m_edComponent_Lan_Port[4];

	CVGStatic		m_stComponent_Serial_Sub[8];
	CVGStatic		m_stComponent_Serial_Controller[5][7];	// Serial Comport 설정 UI
	CMFCButton		m_btnComponent_Serial_Connect[8];		// Serial 연결
	CMFCButton		m_btnComponent_Serial_DisConnect[8];	// Serial 해제

	CMFCButton		m_btnComponent_Measure_CL_200A;			// CL-200A 측정
	CMFCButton		m_btnComponent_Measure_CL_500A;			// CL-500A 측정

	CComboBox		m_cbSerial_Comport[7];
	CComboBox		m_cbSerial_Baudrate[7];
	CComboBox		m_cbSerial_DataBit[7];
	CComboBox		m_cbSerial_ParityBit[7];
	CComboBox		m_cbSerial_StopBit[7];

	CIPAddressCtrl	m_lanIpCtrl[4];

	DECLARE_MESSAGE_MAP()
};
