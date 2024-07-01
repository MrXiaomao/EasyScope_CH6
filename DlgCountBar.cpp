// DlgCountBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCountBar.h"
#include "afxdialogex.h"
#include "resource.h"

// CDlgCountBar 对话框

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


// CDlgCountBar 消息处理程序


BOOL CDlgCountBar::OnInitDialog()
{
	CDialogBar::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDlgCountBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID()==IDC_CH1&&CTLCOLOR_STATIC==nCtlColor)
	// TODO:  在此更改 DC 的任何特性
	pDC->SetTextColor(RGB(255,0,0));
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
