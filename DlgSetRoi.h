#if !defined(AFX_DLGSETROI_H__1BCD11A0_A752_4C2F_8FFC_0C8E95DDC383__INCLUDED_)
#define AFX_DLGSETROI_H__1BCD11A0_A752_4C2F_8FFC_0C8E95DDC383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetRoi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetRoi dialog

class CDlgSetRoi : public CDialog
{
// Construction
public:
	CDlgSetRoi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetRoi)
	enum { IDD = IDD_DLG_SET_ROI };
	int		m_RoiLeftCh;
	int		m_RoiRightCh;
	int		m_Left;
	//}}AFX_DATA
	int m_Set;
	int m_View;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetRoi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetRoi)
	afx_msg void OnReturn();
	afx_msg void OnSet();
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETROI_H__1BCD11A0_A752_4C2F_8FFC_0C8E95DDC383__INCLUDED_)
