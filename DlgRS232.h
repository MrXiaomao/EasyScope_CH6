#include "afxwin.h"
#if !defined(AFX_DLGRS232_H__190D745F_864B_4F1E_907F_B7A60EA067A9__INCLUDED_)
#define AFX_DLGRS232_H__190D745F_864B_4F1E_907F_B7A60EA067A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRS232.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRS232 dialog

class CDlgRS232 : public CDialog
{
// Construction
public:
	CDlgRS232(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRS232)
	enum { IDD = IDD_DLG_RS232 };
	CComboBox	m_Combo2;
	CComboBox	m_Combo1;
	CString	m_strCom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRS232)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRS232)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void  OpenRS232();
	CButton mRS232Open;
	CButton mRS232Close;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRS232_H__190D745F_864B_4F1E_907F_B7A60EA067A9__INCLUDED_)
