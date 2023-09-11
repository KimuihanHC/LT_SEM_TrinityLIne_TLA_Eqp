#pragma once

#include "VGBase/VGStatic.h"

class CDlgColorCalMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgColorCalMain)

public:
	CDlgColorCalMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgColorCalMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_COLOR_CAL_MAIN };
#endif

	void ShowOperation(__in OperationMode mode);
	ST_OptSemiAuto GetOption();

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void InitComponent();
	afx_msg void ChangeUI_Language();
	   
	DECLARE_MESSAGE_MAP()

	CVGStatic m_stTitleName[2];

	CVGStatic m_stLightChName[2][20];
	CVGStatic m_stLightChLux[2][20];
	CVGStatic m_stLightChColorTemp[2][20];

public:
	void Reset			();
	void SetCL500A		(__in InspectionPortType port, __in int nLightIdx, __in float fColorTemper, __in float fData, __in bool bReadOk);
	void ResetCL500A	(__in InspectionPortType port);

private:
	int m_iAngleType;
	int m_iSocketType;
};
