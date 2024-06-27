// DlgMcsSet.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgMcsSet.h"
#include <winioctl.h>
#include "MCB_AVR_WDMioctl.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	McsSetData	mcsSetData[4];
//extern	int		mcsStarCh[4],mcsEndCh[4],mcsTime[4],mcsAreaOrGross[4];
extern	HANDLE hDevice;
extern  int		MaxChannel;
extern  bool	bAdcWorkOn[4];

/////////////////////////////////////////////////////////////////////////////
// CDlgMcsSet dialog


CDlgMcsSet::CDlgMcsSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMcsSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMcsSet)
	m_McsTime1 = mcsSetData[0].mcsTime;
	m_McsTime2 = mcsSetData[1].mcsTime;
	m_McsTime3 = mcsSetData[2].mcsTime;
	m_McsTime4 = mcsSetData[3].mcsTime;
	m_McsLD1 = mcsSetData[0].mcsStarCh;
	m_McsLD2 = mcsSetData[1].mcsStarCh;
	m_McsLD3 = mcsSetData[2].mcsStarCh;
	m_McsLD4 = mcsSetData[3].mcsStarCh;
	m_McsUD1 = mcsSetData[0].mcsEndCh;
	m_McsUD2 = mcsSetData[1].mcsEndCh;
	m_McsUD3 = mcsSetData[2].mcsEndCh;
	m_McsUD4 = mcsSetData[3].mcsEndCh;
	m_McsArea1 = mcsSetData[0].mcsAreaOrGross;
	m_McsArea2 = mcsSetData[1].mcsAreaOrGross;
	m_McsArea3 = mcsSetData[2].mcsAreaOrGross;
	m_McsArea4 = mcsSetData[3].mcsAreaOrGross;
	m_RadioLevel = 0;
	//}}AFX_DATA_INIT
	bLowLevel=1;
}


void CDlgMcsSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMcsSet)
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_MCS_TIME1, m_McsTime1);
	DDX_Text(pDX, IDC_MCS_TIME2, m_McsTime2);
	DDX_Text(pDX, IDC_MCS_TIME3, m_McsTime3);
	DDX_Text(pDX, IDC_MCS_TIME4, m_McsTime4);
	DDX_Text(pDX, MCS_LD1, m_McsLD1);
	DDX_Text(pDX, MCS_LD2, m_McsLD2);
	DDX_Text(pDX, MCS_LD3, m_McsLD3);
	DDX_Text(pDX, MCS_LD4, m_McsLD4);
	DDX_Text(pDX, MCS_UD1, m_McsUD1);
	DDX_Text(pDX, MCS_UD2, m_McsUD2);
	DDX_Text(pDX, MCS_UD3, m_McsUD3);
	DDX_Text(pDX, MCS_UD4, m_McsUD4);
	DDX_Radio(pDX, IDC_RADIO1, m_McsArea1);
	DDX_Radio(pDX, IDC_RADIO6, m_McsArea2);
	DDX_Radio(pDX, IDC_RADIO8, m_McsArea3);
	DDX_Radio(pDX, IDC_RADIO10, m_McsArea4);
	DDX_Radio(pDX, IDC_RADIO12, m_RadioLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMcsSet, CDialog)
	//{{AFX_MSG_MAP(CDlgMcsSet)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMcsSet message handlers

void CDlgMcsSet::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	mcsSetData[0].mcsTime=m_McsTime1;
	mcsSetData[1].mcsTime=m_McsTime2;
	mcsSetData[2].mcsTime=m_McsTime3;
	mcsSetData[3].mcsTime=m_McsTime4;
	mcsSetData[0].mcsStarCh=m_McsLD1;
	mcsSetData[1].mcsStarCh=m_McsLD2;
	mcsSetData[2].mcsStarCh=m_McsLD3;
	mcsSetData[3].mcsStarCh=m_McsLD4;
	mcsSetData[0].mcsEndCh=m_McsUD1;
	mcsSetData[1].mcsEndCh=m_McsUD2;
	mcsSetData[2].mcsEndCh=m_McsUD3;
	mcsSetData[3].mcsEndCh=m_McsUD4;
	mcsSetData[0].mcsAreaOrGross = m_McsArea1;
	mcsSetData[1].mcsAreaOrGross = m_McsArea2;
	mcsSetData[2].mcsAreaOrGross = m_McsArea3;
	mcsSetData[3].mcsAreaOrGross = m_McsArea4;

	if(hDevice!=INVALID_HANDLE_VALUE)
	{
		int i;
		ULONG noutput=0;    // Count written to bufOutput
		ULONG input[30];
		input[0]=MaxChannel;
		for(i=0;i<1;i++)
			input[i+1]=bAdcWorkOn[i];
		for(i=0;i<1;i++)
			input[i+5]=mcsSetData[i].mcsStarCh;
		for(i=0;i<1;i++)
			input[i+9]=mcsSetData[i].mcsEndCh;
		for(i=0;i<1;i++)
			input[i+13]=mcsSetData[i].mcsTime;
		for(i=0;i<1;i++)
			input[i+17]=mcsSetData[i].mcsAreaOrGross;
		ULONG output;
/*
		DeviceIoControl(hDevice,
							 MCB_AVR_WDM_IOCTL_SET,
							 input,
							 sizeof(ULONG)*21,
							 &output,
							 sizeof(ULONG),
							 &noutput,
							 NULL);
*/
	}
	CDialog::OnOK();
}

BOOL CDlgMcsSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Spin2.SetRange(1,1000);
	m_Spin3.SetRange(1,1000);
	m_Spin4.SetRange(1,1000);
	m_Spin5.SetRange(1,1000);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMcsSet::OnRadio12() 
{
	// TODO: Add your control notification handler code here
	bLowLevel=1;
	m_RadioLevel=0;
}

void CDlgMcsSet::OnRadio13() 
{
	// TODO: Add your control notification handler code here
	bLowLevel=0;
	m_RadioLevel=1;
}
