#pragma once

// CDlgSocket 대화 상자
#include "VGBase/VGStatic.h"

class CDlgSocket : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSocket)

public:
	CDlgSocket(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSocket();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SOCKET};
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
	
	void InitComponent();

public:
	void SetSocket(__in ST_SocketInfo socket);

private:
	CVGStatic m_stcStatus;
	CVGStatic m_stcResultOk;
	CVGStatic m_stcResultNg;
	CVGStatic m_stcRfid;
	CVGStatic m_stcBarcode;
};
