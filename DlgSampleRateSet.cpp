// DlgSampleRateSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgSampleRateSet.h"
#include "afxdialogex.h"

extern	bool	bWaveShort;


// CDlgSampleRateSet �Ի���

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


// CDlgSampleRateSet ��Ϣ�������


void CDlgSampleRateSet::OnBnClickedRadio1()//������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bSampleRate250M=1;
	bWaveShort=0;
}


void CDlgSampleRateSet::OnBnClickedRadio4()//�̲���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bSampleRate250M=0;
	bWaveShort=1;
}


BOOL CDlgSampleRateSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//	mRadio1.SetCheck(bWaveShort);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
