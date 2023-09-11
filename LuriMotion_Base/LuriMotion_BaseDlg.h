
// LuriMotionUIDlg.h: 헤더 파일
#pragma once

#include "Def_CompileOption.h"
#include "Def_Enum.h"
#include "Def_DigitalIO.h"

#include "Device.h"
#include "IO.h"

#include "IcsCommunicator.h"

#include "DlgMain.h"
#include "DlgSide.h"
#include "DlgTitle.h"
#include "DlgBottom.h"
#include "DlgMotorTeach.h"
#include "DlgIo.h"
#include "DlgConfiguration.h"
#include "DlgMessageBox.h"
#include "DlgMessageBox_Instance.h"
#include "DlgErrorSolution.h"
#include "DlgLog.h"

#include <afxwin.h>
#include "VGbase/VGStatic.h"

// CLuriMotionUIDlg 대화 상자
class CLuriMotion_BaseDlg : public CDialogEx, public CDevice, public CIO
{
// 생성입니다.
public:
	CLuriMotion_BaseDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LT_BASE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer								(UINT_PTR nIDEvent);
	afx_msg		LRESULT		OnRecvLightArgFromSEMCO		(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	// CVG 사용하기 위한 GDI 초기화
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

	CStatic		m_ctrlStatic;
	CFont			m_font_List;
	CFont			m_font_Large;
	CFont			m_font_Default;
	CFont			m_font_Normal;

	// Bitmap
	int							m_nBitmap[eBITMAP_COUNT];
	CBitmap						m_cbitBack[eBITMAP_COUNT];
	HICON						m_IconBack[eICON_COUNT];

public:
	CDlgMain*					m_pDlgMain;
	CDlgSide*					m_pDlgSide;
	CDlgBottom*					m_pDlgBtm;
	CDlgTitle*					m_pDlgTitle;
	
	CDlgMotorTeach*				m_pDlgTeach;
	CDlgIo*						m_pDlgIO;
	CDlgConfiguration*			m_pDlgConfiguration;
	CDlgErrorSolution*			m_pDlgErrSolution;
	CDlgLog*					m_pDlgLog;

	CDlgMessageBox_Instance*		m_pMsgBox_Instance;
	
	BOOL EraseBkGnd_Image		(CWnd * pWnd, CDC * pDC, /*CBitmap * pBmp*/EBITMAP_LIST nImageId, int nBit = 1);
	BOOL IsImageHandleValid		(EBITMAP_LIST nImageId);
	BOOL IsIconHandleValid		(EICON_LIST nIconId);
	HICON GetIconHandle			(EICON_LIST nIconId);

	afx_msg void CreateDlg();
	afx_msg void Dlg_MovePosition();

	afx_msg void OnClose();

	afx_msg void ChangeMainUi(int nPart);
	afx_msg void RecipeChange_Ui_Refresh();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

public:
	CDlgMessageBox	m_DlgMessage;
	afx_msg BOOL RunMessageDlg_Domodal(CString strInfo, int nMode);

	afx_msg void RunMessageDlg_Instance(CString strInfo, int nMode);
	afx_msg void RunMessageDlg_Instance_Show();
	bool m_bMessageBlock;

public:
	void ChangeUi_ByLanguage();
	void ChangeUi_ByLevel();

public:
	afx_msg BOOL Connect_Tester();

public:
	afx_msg void Hide_Program();
	LRESULT OnWindow2Top(WPARAM wParam, LPARAM lParam);
	LRESULT OnStartup2ui(WPARAM wParam, LPARAM lParam);

	void InitBitmap		();
	void InitProgram	();
	void InitDevice		();

public:
	BOOL	m_bStartCompleate;
};
