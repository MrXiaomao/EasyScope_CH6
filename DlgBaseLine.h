#pragma once


// CDlgBaseLine �Ի���

class CDlgBaseLine : public CDialog
{
	DECLARE_DYNAMIC(CDlgBaseLine)

public:
	CDlgBaseLine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBaseLine();

// �Ի�������
	enum { IDD = IDD_DLG_ENERGY_KEDU1 };
	CEdit	m_CtrlEn;
	CEdit	m_CtrlCh;
	int		m_Channel;
	float	m_Energy;
	CString	m_ADC;
	int		m_fitMode;
	//}}AFX_DATA
	CListCtrl	m_List1;
	int		adcNum;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	afx_msg void OnDelete();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnAdd();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
