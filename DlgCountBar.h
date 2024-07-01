#pragma once

#include "resource.h"

// CDlgCountBar 对话框

class CDlgCountBar : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgCountBar)

public:
	CDlgCountBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCountBar();

// 对话框数据
	enum { IDD = IDD_DIALOGBAR_COUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
