#if !defined(AFX_DLGAUTOSETUP_H__D9ACE088_1DC5_4B69_B450_8BF8A20C2D48__INCLUDED_)
#define AFX_DLGAUTOSETUP_H__D9ACE088_1DC5_4B69_B450_8BF8A20C2D48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSetup dialog

class CDlgAutoSetup : public CDialog
{
// Construction
public:
	CDlgAutoSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoSetup)
	enum { IDD = IDD_DLG_AUTO_SETUP };
	int		m_GapTime;
	int		m_MeasureTime;
	int		m_Times;
	CString	m_No;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoSetup)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOSETUP_H__D9ACE088_1DC5_4B69_B450_8BF8A20C2D48__INCLUDED_)
