#pragma once


// CDlgSetLD �Ի���

class CDlgSetLD : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetLD)

public:
	CDlgSetLD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetLD();

// �Ի�������
	enum { IDD = IDD_DLG_SET_LD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CString m_Ch1LD;
	float m_Ch1LD[6];
	virtual void OnOK();
};
