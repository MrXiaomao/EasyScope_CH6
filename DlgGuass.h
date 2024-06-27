#if !defined(AFX_DLGGUASS_H__AAE587BA_C75B_410E_94E2_DC386FAA8007__INCLUDED_)
#define AFX_DLGGUASS_H__AAE587BA_C75B_410E_94E2_DC386FAA8007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGuass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGuass dialog
class CDlgGuass : public CDialog
{
// Construction
public:
	CDlgGuass(CWnd* pParent = NULL);   // standard constructor
	void UpdateFit();

// Dialog Data
	//{{AFX_DATA(CDlgGuass)
	enum { IDD = IDD_DLG_GAUSS };
	double	m_Area1;
	double	m_Area2;
	double	m_Area3;
	double	m_Area4;
	double	m_Area5;
	BOOL	m_CheckSgm1;
	BOOL	m_CheckSgm2;
	BOOL	m_CheckSgm3;
	BOOL	m_CheckSgm4;
	BOOL	m_CheckSgm5;
	BOOL	m_CheckX1;
	BOOL	m_CheckX2;
	BOOL	m_CheckX3;
	BOOL	m_CheckX4;
	BOOL	m_CheckX5;
	double	m_Sgm1;
	double	m_Sgm2;
	double	m_Sgm3;
	double	m_Sgm4;
	double	m_Sgm5;
	int		m_StartCh;
	BOOL	m_Use1;
	BOOL	m_Use2;
	BOOL	m_Use3;
	BOOL	m_Use4;
	BOOL	m_Use5;
	double	m_X1;
	double	m_X2;
	double	m_X3;
	double	m_X4;
	double	m_X5;
	int		m_EndCh;
	double	m_AreaSgm1;
	double	m_AreaSgm2;
	double	m_AreaSgm3;
	double	m_AreaSgm4;
	double	m_AreaSgm5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGuass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGuass)
	virtual void OnOK();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGUASS_H__AAE587BA_C75B_410E_94E2_DC386FAA8007__INCLUDED_)
