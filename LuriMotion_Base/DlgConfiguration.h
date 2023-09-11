#pragma once

class CDlgSystemConfiguration;
class CDlgCommConfiguration;
class CDlgDeviceConfiguration;
class CDlgSemcoComTest;
enum class enConfiguration;

// CDlgConfiguration 대화 상자

class CDlgConfiguration : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfiguration)

public:
	CDlgConfiguration(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgConfiguration();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_CONFIGURATION };

	void Load_ComponentDlg();
	void ChangeUI_Language();

private:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void EqpTypeShowControl();
	afx_msg void OnBnClickedBtnSaveConfiguration();
	afx_msg void OnBnClickedBtnSystemConfiguration();
	afx_msg void OnBnClickedBtnCommunicationConfiguration();
	afx_msg void OnBnClickedBtnDeviceConfiguration();
	afx_msg void OnBnClickedBtnSemcoTest();

	void InitButton();
	void InitConfigDlg();
	void ChangeConfigView(enConfiguration config);

private:
	CBitmap		m_bitmapSave;
	CMFCButton	m_btnSaveConfiguration;
	CMFCButton	m_btnConfiguration[4];

	CDlgSystemConfiguration * m_pSystemConfig = nullptr;
	CDlgCommConfiguration * m_pCommConfig = nullptr;
	CDlgDeviceConfiguration * m_pDeviceConfig = nullptr;
	CDlgSemcoComTest * m_pSemcoComTest = nullptr;

	DECLARE_MESSAGE_MAP()
};
