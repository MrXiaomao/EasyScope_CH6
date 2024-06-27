#if !defined(AFX_DLGPAPERV_H__5CEB1996_F7B1_4842_AE60_144E1EC18ADC__INCLUDED_)
#define AFX_DLGPAPERV_H__5CEB1996_F7B1_4842_AE60_144E1EC18ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPaperV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperV dialog

class CDlgPaperV : public CDialog
{
// Construction
public:
	CDlgPaperV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPaperV)
	enum { IDD = IDD_DLG_PAPER_V };
	int		m_PaperV;
	CString	m_PaperV2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPaperV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPaperV)
	afx_msg void OnButton1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAPERV_H__5CEB1996_F7B1_4842_AE60_144E1EC18ADC__INCLUDED_)
