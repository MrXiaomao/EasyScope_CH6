#pragma once
#include "afxwin.h"


// CDlgSampleRateSet 对话框

class CDlgSampleRateSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSampleRateSet)

public:
	CDlgSampleRateSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSampleRateSet();

// 对话框数据
	enum { IDD = IDD_DL_SAMPLERATE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
