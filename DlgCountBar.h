#pragma once

#include "resource.h"

// CDlgCountBar �Ի���

class CDlgCountBar : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgCountBar)

public:
	CDlgCountBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCountBar();

// �Ի�������
	enum { IDD = IDD_DIALOGBAR_COUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
