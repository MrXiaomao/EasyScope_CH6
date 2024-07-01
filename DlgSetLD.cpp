// DlgSetLD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "DlgSetLD.h"
#include "afxdialogex.h"
extern	float fLDWave[6];
extern int	mPeakPoint;//Ѱ�������������½�����

// CDlgSetLD �Ի���

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


// CDlgSetLD ��Ϣ�������


BOOL CDlgSetLD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSetLD::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(1);
	for(int i=0;i<6;i++)
		fLDWave[i] = m_Ch1LD[i];
	mPeakPoint=m_PeakPoint;
	CDialogEx::OnOK();
}
