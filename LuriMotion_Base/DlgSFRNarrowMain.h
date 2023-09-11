#pragma once

#include "VGBase/VGStatic.h"

class CDlgSFRNarrowMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSFRNarrowMain)

public:
	CDlgSFRNarrowMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSFRNarrowMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SFR_NARROW_MAIN };
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
};
