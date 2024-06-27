#if !defined(AFX_DLGSETC51_H__99E678D8_80F1_4EFA_AB11_C1DCF62C37E9__INCLUDED_)
#define AFX_DLGSETC51_H__99E678D8_80F1_4EFA_AB11_C1DCF62C37E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetC51.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetC51 dialog

class CDlgSetC51 : public CDialog
{
// Construction
public:
	CDlgSetC51(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetC51)
	enum { IDD = IDD_DLG_SET_C51 };
	CComboBox	m_Combo2;
	BOOL	m_AlarmPaper;
	BOOL	m_AlarmFlux;
	BOOL	m_AlarmBeta;
	BOOL	m_AlarmAlpha;
	int		m_Alpha1;
	int		m_Alpha2;
	float	m_BetaAcc;
	float	m_BetaEff;
	CString	m_BetaFile;
	int		m_Beta1;
	int		m_Beta2;
	int		m_BetaH;
	int		m_BetaL;
	int		m_Flux1;
	int		m_Flux2;
	int		m_Paper1;
	CString	m_Pu239File;
	float	m_Pu239S1;
	float	m_Pu239S2;
	float	m_Pu239S3;
	float	m_Pu239S4;
	float	m_Pu239S5;
	int		m_Pu239H;
	int		m_Pu239L;
	CString	m_Rn220File;
	float	m_Rn220S1;
	float	m_Rn220S2;
	float	m_Rn220S3;
	float	m_Rn220S4;
	float	m_Rn220S5;
	float	m_Rn222S1;
	float	m_Rn222S2;
	float	m_Rn222S3;
	float	m_Rn222S4;
	float	m_Rn222S5;
	float	m_U235S1;
	float	m_U235S2;
	float	m_U235S3;
	float	m_U235S4;
	float	m_U235S5;
	int		m_Rn220H;
	int		m_Rn220L;
	CString	m_Rn222File;
	int		m_Rn222H;
	int		m_Rn222L;
	CString	m_U235File;
	int		m_U235H;
	int		m_U235L;
	float	m_S1;
	float	m_S2;
	float	m_S3;
	float	m_S4;
	float	m_S5;
	int		m_Address;
	int		m_Period;
	CString	m_BendiFile;
	float	m_BendiS1;
	float	m_BendiS2;
	float	m_BendiS3;
	float	m_BendiS4;
	float	m_BendiS5;
	//}}AFX_DATA
	double totCount1,totCount2,totCount3,totCount4;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetC51)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetC51)
	virtual void OnOK();
	afx_msg void OnBtnRn220();
	afx_msg void OnBtnRn222();
	afx_msg void OnBtnPu239();
	afx_msg void OnBtnU235();
	afx_msg void OnBtnBeta();
	afx_msg void OnBtnCal();
	afx_msg void OnBtnCal2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBendi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETC51_H__99E678D8_80F1_4EFA_AB11_C1DCF62C37E9__INCLUDED_)
