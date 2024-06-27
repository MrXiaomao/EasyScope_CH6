#if !defined(AFX_DLGSTRIP_H__4C22C56D_5822_47F9_A9C2_119BC7C85AD0__INCLUDED_)
#define AFX_DLGSTRIP_H__4C22C56D_5822_47F9_A9C2_119BC7C85AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStrip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStrip dialog

class CDlgStrip : public CDialog
{
// Construction
public:
	CDlgStrip(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStrip)
	enum { IDD = IDD_DLG_STRIP };
	CComboBox	m_Combo3;
	CButton	m_CtrlCheck1;
	CListCtrl	m_List3;
	CEdit	m_Edit1;
	BOOL	m_Check1;
	CString	m_Directory;
	CString m_PathName;
	float	m_Radio1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStrip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStrip)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioFour();
	afx_msg void OnRadio1();
	afx_msg void OnCheck1();
	virtual void OnOK();
	afx_msg void OnDblclkCombo3();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRIP_H__4C22C56D_5822_47F9_A9C2_119BC7C85AD0__INCLUDED_)
