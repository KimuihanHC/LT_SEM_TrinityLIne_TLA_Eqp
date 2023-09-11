#pragma once

#include "VGBase/VGStatic.h"

class CDlgDistortionMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDistortionMain)

public:
	CDlgDistortionMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgDistortionMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DISTORTION_MAIN };
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

private:
	int m_iAngleType;
	int m_iSocketType;
};
