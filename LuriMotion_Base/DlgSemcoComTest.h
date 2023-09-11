#pragma once

#include "Device.h"
#include "IO.h"
// CDlgSemcoComTest 대화 상자

class CDlgSemcoComTest : public CDialogEx, public CDevice, public CIO
{
	DECLARE_DYNAMIC(CDlgSemcoComTest)

public:
	CDlgSemcoComTest(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSemcoComTest();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SEMCO_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSemcoReady();
	afx_msg void OnBnClickedBtnSemcoSendBcr();
	afx_msg void OnBnClickedBtnSemcoSendPreviewStart();
	afx_msg void OnBnClickedBtnSemcoSendTLAInspection();
	afx_msg void OnBnClickedBtnSemcoSendVignetingInspection46();
	afx_msg void OnBnClickedBtnSemcoSendVignetingInspection82();
	afx_msg void OnBnClickedBtnSemcoSendDistortionCapture();
	afx_msg void OnBnClickedBtnSemcoSendDistortionCalculate();
	afx_msg void OnBnClickedBtnSemcoSendFlareInspection();
	afx_msg void OnBnClickedBtnSemcoSendPreviewStop();
	afx_msg void OnBnClickedBtnSemcoColorcalReady();
	afx_msg void OnBnClickedBtnSemcoSendColorcalPreviewStart();
	afx_msg void OnBnClickedBtnSemcoSendColorcalInspection();
	afx_msg void OnBnClickedBtnSemcoSendColorcalPreviewStop();

private:
	InspectionPortType	m_PortType;
	CString m_szSendBcr;
	CString m_szRecvBcr;
};
