#if !defined(AFX_DLGENERGYKEDU_H__5D2F0285_DA8C_480A_A2EC_47BA7EA1705A__INCLUDED_)
#define AFX_DLGENERGYKEDU_H__5D2F0285_DA8C_480A_A2EC_47BA7EA1705A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnergyKedu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEnergyKedu dialog

class CDlgEnergyKedu : public CDialog
{
// Construction
public:
	CDlgEnergyKedu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEnergyKedu)
	enum { IDD = IDD_DLG_ENERGY_KEDU };
	CEdit	m_CtrlEn;
	CEdit	m_CtrlCh;
	int		m_Channel;
	float	m_Energy;
	CString	m_ADC;
	int		m_fitMode;
	//}}AFX_DATA
	CListCtrl	m_List[4];
	int		adcNum;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnergyKedu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnergyKedu)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	afx_msg void OnDelete();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnAdd();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENERGYKEDU_H__5D2F0285_DA8C_480A_A2EC_47BA7EA1705A__INCLUDED_)
