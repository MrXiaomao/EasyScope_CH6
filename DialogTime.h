#include "afxwin.h"
#if !defined(AFX_DIALOGTIME_H__C54AE6A3_CC2A_11D4_B523_001088023E64__INCLUDED_)
#define AFX_DIALOGTIME_H__C54AE6A3_CC2A_11D4_B523_001088023E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTime dialog

class CDialogTime : public CDialog
{
// Construction
	void EnableChannel();
public:
	CDialogTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTime)
	enum { IDD = IDD_DIALOG_TIME_PRESET };
	int		m_PresetHV;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTime)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton mHVClose;
	CButton mHVOpen;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio14();
	BOOL b50R;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTIME_H__C54AE6A3_CC2A_11D4_B523_001088023E64__INCLUDED_)
