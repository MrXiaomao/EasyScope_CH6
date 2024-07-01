// DlgSampleRateSet.cpp : 实现文件
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgSampleRateSet.h"
#include "afxdialogex.h"

extern	bool	bWaveShort;


// CDlgSampleRateSet 对话框

IMPLEMENT_DYNAMIC(CDlgSampleRateSet, CDialogEx)

CDlgSampleRateSet::CDlgSampleRateSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSampleRateSet::IDD, pParent)
	
	, bSampleRate250M(bWaveShort)
{
}

CDlgSampleRateSet::~CDlgSampleRateSet()
{
}

void CDlgSampleRateSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, mRadio1);
	DDX_Radio(pDX, IDC_RADIO1, bSampleRate250M);
}


BEGIN_MESSAGE_MAP(CDlgSampleRateSet, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgSampleRateSet::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgSampleRateSet::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CDlgSampleRateSet 消息处理程序


void CDlgSampleRateSet::OnBnClickedRadio1()//长波形
{
	// TODO: 在此添加控件通知处理程序代码
	bSampleRate250M=1;
	bWaveShort=0;
}


void CDlgSampleRateSet::OnBnClickedRadio4()//短波形
{
	// TODO: 在此添加控件通知处理程序代码
	bSampleRate250M=0;
	bWaveShort=1;
}


BOOL CDlgSampleRateSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	mRadio1.SetCheck(bWaveShort);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
