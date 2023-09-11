#pragma once

#include "VGbase/VGStatic.h"

// CDlgDeviceConfiguration 대화 상자

class CDlgDeviceConfiguration : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDeviceConfiguration)

public:
	CDlgDeviceConfiguration(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgDeviceConfiguration();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DEVICE_CONFIGURATION };
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

	afx_msg void OnbnClickedBtnLightOnOff(UINT nID);
	afx_msg void OnBnClickedBtnReadRFID();
	afx_msg void OnBnClickedBtnChromaMeterMeasure();

	void InitComponent_Component();
	void InspectorCtrlSetting();
	void VisibleLightCtrls(int count);

	void LightOn(int index);
	void LightOff(int index);

private:
	CVGStatic		m_stComponent_Title[3];					// Title

	CVGStatic		m_stComponent_Light_ValueName[20];		// 광원 20개 Value Name
	CEdit			m_edComponent_Light_Value[20];			// 광원 20개 Value
	CMFCButton		m_btnComponent_Light_On[20];			// Light On
	CMFCButton		m_btnComponent_Light_Off[20];			// Light Off

	CMFCButton		m_btnComponent_Read_RFID;				// RFID 읽기
	CEdit			m_edComponent_Read_RFID;

	CMFCButton		m_btnComponent_Measure_ChromaMeter;		// 조도계 측정
	CEdit			m_edChromaMeterMeasurementData;

	// COLOR CAL 용
	CVGStatic		m_stComponent_Colorcal_SemcoLight[4];
	CEdit			m_edComponent_Colorcal_SemcoLight[4];

	DECLARE_MESSAGE_MAP()
public:
};
