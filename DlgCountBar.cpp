// DlgCountBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgCountBar.h"
#include "afxdialogex.h"
#include "resource.h"

// CDlgCountBar �Ի���

IMPLEMENT_DYNAMIC(CDlgCountBar, CDialogBar)

CDlgCountBar::CDlgCountBar(CWnd* pParent /*=NULL*/)
	: CDialogBar(CDlgCountBar::IDD, pParent)
{

}

CDlgCountBar::~CDlgCountBar()
{
}

void CDlgCountBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCountBar, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgCountBar ��Ϣ�������


BOOL CDlgCountBar::OnInitDialog()
{
	CDialogBar::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CDlgCountBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID()==IDC_CH1&&CTLCOLOR_STATIC==nCtlColor)
	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetTextColor(RGB(255,0,0));
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
