#if !defined(AFX_DLGMCSSET_H__D7EE0886_B1DD_4124_9436_2CDD96291B4B__INCLUDED_)
#define AFX_DLGMCSSET_H__D7EE0886_B1DD_4124_9436_2CDD96291B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMcsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMcsSet dialog

class CDlgMcsSet : public CDialog
{
// Construction
public:
	CDlgMcsSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMcsSet)
	enum { IDD = IDD_DLG_MCS_SET };
	CSpinButtonCtrl	m_Spin5;
	CSpinButtonCtrl	m_Spin4;
	CSpinButtonCtrl	m_Spin3;
	CSpinButtonCtrl	m_Spin2;
	int		m_McsTime1;
	int		m_McsTime2;
	int		m_McsTime3;
	int		m_McsTime4;
	int		m_McsLD1;
	int		m_McsLD2;
	int		m_McsLD3;
	int		m_McsLD4;
	int		m_McsUD1;
	int		m_McsUD2;
	int		m_McsUD3;
	int		m_McsUD4;
	int		m_McsArea1;
	int		m_McsArea2;
	int		m_McsArea3;
	int		m_McsArea4;
	int		m_RadioLevel;
	//}}AFX_DATA
	bool	bLowLevel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMcsSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMcsSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio12();
	afx_msg void OnRadio13();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMCSSET_H__D7EE0886_B1DD_4124_9436_2CDD96291B4B__INCLUDED_)
