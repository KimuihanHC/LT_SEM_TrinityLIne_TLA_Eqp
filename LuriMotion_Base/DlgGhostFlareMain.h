#pragma once

#include "VGBase/VGStatic.h"

class CDlgGhostFlareMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGhostFlareMain)

public:
	CDlgGhostFlareMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgGhostFlareMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GHOSTFLARE_MAIN };
#endif

	void ShowOperation(__in OperationMode mode);
	ST_OptSemiAuto GetOption();

private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedPwspConnect1();
	afx_msg void OnBnClickedPwspConnect2();

// #if(SET_INSPECTOR == SYS_GHOSTFLARE)
// 	afx_msg void OnBnClickedPwspOn1();		//x
// 	afx_msg void OnBnClickedPwspOn2();		//0F
// 	afx_msg void OnBnClickedPwspOn3();		//0.5F B
// 	afx_msg void OnBnClickedPwspOn4();	    //0.5F L
// 	afx_msg void OnBnClickedPwspOn5();	    //0.5F T
// 	afx_msg void OnBnClickedPwspOn6();	    //0.5F R
// 	afx_msg void OnBnClickedPwspOn7();		//1F LL
// 	afx_msg void OnBnClickedPwspOn8();		//1F UL
// 	afx_msg void OnBnClickedPwspOn9();		//1F LR
// 	afx_msg void OnBnClickedPwspOn10();	//1F UR
// 	afx_msg void OnBnClickedPwspOn11();
// 
// 	afx_msg void OnBnClickedPwspOff1();		  //x
// 	afx_msg void OnBnClickedPwspOff2();		  //0F
// 	afx_msg void OnBnClickedPwspOff4();		  //0.5F B
// 	afx_msg void OnBnClickedPwspOff5();		  //0.5F L
// 	afx_msg void OnBnClickedPwspOff6();		  //0.5F T
// 	afx_msg void OnBnClickedPwspOff7();		  //0.5F R
// 	afx_msg void OnBnClickedPwspOff8();		  //1F LL
// 	afx_msg void OnBnClickedPwspOff9();		  //1F UL
// 	afx_msg void OnBnClickedPwspOff10();	  //1F LR
// 	afx_msg void OnBnClickedPwspOff11();		  //1F UR
// 	afx_msg void OnBnClickedPwspOff12();
// #endif

	afx_msg void InitComponent();
	afx_msg void ChangeUI_Language();
	afx_msg void ChangeDectectedUI_Status();

private:
	CVGStatic			m_stGhostFlareMainLightCtrl_Title;					// 메인 광원컨트롤러_ 타이틀
	CVGStatic			m_stGhostFlareMainLightCtrl_Buttons[11];			// 메인 광원컨트롤러 개별색상

	DECLARE_MESSAGE_MAP()
};
