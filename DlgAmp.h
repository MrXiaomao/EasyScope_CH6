#include "afxwin.h"
#if !defined(AFX_DLGAMP_H__381E2B03_19DC_4800_99CB_EB6D67251758__INCLUDED_)
#define AFX_DLGAMP_H__381E2B03_19DC_4800_99CB_EB6D67251758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAmp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAmp dialog

class CDlgAmp : public CDialog
{
// Construction
public:
	CDlgAmp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAmp)
	enum { IDD = IDD_DLG_AMP };
	CSpinButtonCtrl	m_Spin7;
	CButton	m_CtrlClose;
	CButton	m_ctrlOpen;
	CEdit	m_HVCtrl;
	CSpinButtonCtrl	m_Spin6;
	int		m_HV;
	int		m_Yuzhi;
	int		m_maxHV;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAmp)
	virtual void OnOK();
	afx_msg void OnRadio1();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeHv();
	afx_msg void OnSetYuzhi();
	afx_msg void OnHvOpen();
	afx_msg void OnHvClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_maxHV_Ctrl;
	afx_msg void OnBnClickedPasswordOK();
	CString m_Password;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAMP_H__381E2B03_19DC_4800_99CB_EB6D67251758__INCLUDED_)
