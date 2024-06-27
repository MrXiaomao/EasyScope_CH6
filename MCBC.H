// mcbc.h : main header file for the MCBC application
//

#if !defined(AFX_MCBC_H__F19B1BA4_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
#define AFX_MCBC_H__F19B1BA4_7809_11D4_B521_EBC39990CA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMcbcApp:
// See mcbc.cpp for the implementation of this class
//

class CMcbcApp : public CWinApp
{
public:
	CMcbcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcbcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMcbcApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCBC_H__F19B1BA4_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
