#pragma once
#include "afxwin.h"


// CDlgSampleRateSet �Ի���

class CDlgSampleRateSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSampleRateSet)

public:
	CDlgSampleRateSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSampleRateSet();

// �Ի�������
	enum { IDD = IDD_DL_SAMPLERATE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	bool bSample250M;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	virtual BOOL OnInitDialog();
	CButton mRadio1;
//	int mRadio2;
	BOOL bSampleRate250M;
};
