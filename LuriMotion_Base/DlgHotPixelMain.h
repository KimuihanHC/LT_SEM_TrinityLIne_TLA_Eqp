#pragma once

#include "VGBase/VGStatic.h"

class CDlgHotPixelMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHotPixelMain)

public:
	CDlgHotPixelMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgHotPixelMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HOTPIXEL_MAIN };
#endif

	void ShowOperation(__in OperationMode mode);
	ST_OptSemiAuto GetOption	();

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void InitComponent();
	afx_msg void ChangeUI_Language();

	DECLARE_MESSAGE_MAP()

	CVGStatic m_stName[2];
	CVGStatic m_stData[2];

public:
	void Reset				();
	void SetCL200A		(__in InspectionPortType port, __in float fData, __in bool bReadOk);
	void ResetCL200A	(__in InspectionPortType port);

};
