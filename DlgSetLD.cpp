// DlgSetLD.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "DlgSetLD.h"
#include "afxdialogex.h"
extern	float fLDWave[6];
extern int	mPeakPoint;//寻峰连续上升和下降点数

// CDlgSetLD 对话框

IMPLEMENT_DYNAMIC(CDlgSetLD, CDialogEx)

CDlgSetLD::CDlgSetLD(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetLD::IDD, pParent)
	, m_PeakPoint(mPeakPoint)
{

	//  m_Ch1LD = _T("");
	for(int i=0;i<6;i++)
		m_Ch1LD[i] = fLDWave[i];
}

CDlgSetLD::~CDlgSetLD()
{
}

void CDlgSetLD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_CH1_LD, m_Ch1LD);
	DDX_Text(pDX, IDC_CH1_LD, m_Ch1LD[0]);
	DDX_Text(pDX, IDC_CH1_LD2, m_Ch1LD[1]);
	DDX_Text(pDX, IDC_CH1_LD3, m_Ch1LD[2]);
	DDX_Text(pDX, IDC_CH1_LD4, m_Ch1LD[3]);
	DDX_Text(pDX, IDC_CH1_LD5, m_Ch1LD[4]);
	DDX_Text(pDX, IDC_CH1_LD6, m_Ch1LD[5]);
	DDX_Text(pDX, IDC_PEAK_POINT, m_PeakPoint);
}


BEGIN_MESSAGE_MAP(CDlgSetLD, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetLD 消息处理程序


BOOL CDlgSetLD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSetLD::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(1);
	for(int i=0;i<6;i++)
		fLDWave[i] = m_Ch1LD[i];
	mPeakPoint=m_PeakPoint;
	CDialogEx::OnOK();
}
