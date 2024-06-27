// mcbcDoc.h : interface of the CMcbcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCBCDOC_H__F19B1BAC_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
#define AFX_MCBCDOC_H__F19B1BAC_7809_11D4_B521_EBC39990CA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "define.h"

class CMcbcDoc : public CDocument
{
protected: // create from serialization only
	CMcbcDoc();
	DECLARE_DYNCREATE(CMcbcDoc)
	ChnHead chnHead;
	ChnEnd  chnEnd;
// Attributes
public:

// Operations
public:
	int		nCurView;
	int		oi1,oi2;
	bool	overlapbz;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcbcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMcbcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CString fileName;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMcbcDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveText();
	afx_msg void OnFileOpenText();
	afx_msg void OnFileSaveAll();
	afx_msg void OnFileLoadAll();
	afx_msg void OnMenuStrip();
	afx_msg void OnMenuSaveCansu();
	afx_msg void OnMenuReadCansu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCBCDOC_H__F19B1BAC_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
