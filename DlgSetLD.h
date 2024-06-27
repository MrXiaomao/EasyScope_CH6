#pragma once


// CDlgSetLD 对话框

class CDlgSetLD : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetLD)

public:
	CDlgSetLD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetLD();

// 对话框数据
	enum { IDD = IDD_DLG_SET_LD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CString m_Ch1LD;
	float m_Ch1LD[6];
	virtual void OnOK();
};
