// DlgGuass.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgGuass.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CGuassData mGuassData;
extern char	curDirectory[100];

void fitGass();
/////////////////////////////////////////////////////////////////////////////
// CDlgGuass dialog


CDlgGuass::CDlgGuass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGuass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGuass)
	m_Area1 = mGuassData.m_Area[0];
	m_Area2 = mGuassData.m_Area[1];
	m_Area3 = mGuassData.m_Area[2];
	m_Area4 = mGuassData.m_Area[3];
	m_Area5 = mGuassData.m_Area[4];
	m_CheckSgm1 = mGuassData.m_CheckSgm[0];
	m_CheckSgm2 = mGuassData.m_CheckSgm[1];
	m_CheckSgm3 = mGuassData.m_CheckSgm[2];
	m_CheckSgm4 = mGuassData.m_CheckSgm[3];
	m_CheckSgm5 = mGuassData.m_CheckSgm[4];
	m_CheckX1 = mGuassData.m_CheckX[0];
	m_CheckX2 = mGuassData.m_CheckX[1];
	m_CheckX3 = mGuassData.m_CheckX[2];
	m_CheckX4 = mGuassData.m_CheckX[3];
	m_CheckX5 = mGuassData.m_CheckX[4];
	m_Sgm1 = mGuassData.m_Sgm[0];
	m_Sgm2 = mGuassData.m_Sgm[1];
	m_Sgm3 = mGuassData.m_Sgm[2];
	m_Sgm4 = mGuassData.m_Sgm[3];
	m_Sgm5 = mGuassData.m_Sgm[4];
	m_StartCh = mGuassData.startCh;
	m_Use1 = mGuassData.m_Use[0];
	m_Use2 = mGuassData.m_Use[1];
	m_Use3 = mGuassData.m_Use[2];
	m_Use4 = mGuassData.m_Use[3];
	m_Use5 = mGuassData.m_Use[4];
	m_X1 = mGuassData.m_X0[0];
	m_X2 = mGuassData.m_X0[1];
	m_X3 = mGuassData.m_X0[2];
	m_X4 = mGuassData.m_X0[3];
	m_X5 = mGuassData.m_X0[4];
	m_EndCh = mGuassData.endCh;
	m_AreaSgm1 = mGuassData.m_AreaSgm[0];
	m_AreaSgm2 = mGuassData.m_AreaSgm[1];
	m_AreaSgm3 = mGuassData.m_AreaSgm[2];
	m_AreaSgm4 = mGuassData.m_AreaSgm[3];
	m_AreaSgm5 = mGuassData.m_AreaSgm[4];
	//}}AFX_DATA_INIT
}


void CDlgGuass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGuass)
	DDX_Text(pDX, IDC_AREA1, m_Area1);
	DDX_Text(pDX, IDC_AREA2, m_Area2);
	DDX_Text(pDX, IDC_AREA3, m_Area3);
	DDX_Text(pDX, IDC_AREA4, m_Area4);
	DDX_Text(pDX, IDC_AREA5, m_Area5);
	DDX_Check(pDX, IDC_CHECK_SGM1, m_CheckSgm1);
	DDX_Check(pDX, IDC_CHECK_SGM2, m_CheckSgm2);
	DDX_Check(pDX, IDC_CHECK_SGM3, m_CheckSgm3);
	DDX_Check(pDX, IDC_CHECK_SGM4, m_CheckSgm4);
	DDX_Check(pDX, IDC_CHECK_SGM5, m_CheckSgm5);
	DDX_Check(pDX, IDC_CHECK_X1, m_CheckX1);
	DDX_Check(pDX, IDC_CHECK_X2, m_CheckX2);
	DDX_Check(pDX, IDC_CHECK_X3, m_CheckX3);
	DDX_Check(pDX, IDC_CHECK_X4, m_CheckX4);
	DDX_Check(pDX, IDC_CHECK_X5, m_CheckX5);
	DDX_Text(pDX, IDC_SGM1, m_Sgm1);
	DDX_Text(pDX, IDC_SGM2, m_Sgm2);
	DDX_Text(pDX, IDC_SGM3, m_Sgm3);
	DDX_Text(pDX, IDC_SGM4, m_Sgm4);
	DDX_Text(pDX, IDC_SGM5, m_Sgm5);
	DDX_Text(pDX, IDC_STARTCH, m_StartCh);
	DDX_Check(pDX, IDC_USE1, m_Use1);
	DDX_Check(pDX, IDC_USE2, m_Use2);
	DDX_Check(pDX, IDC_USE3, m_Use3);
	DDX_Check(pDX, IDC_USE4, m_Use4);
	DDX_Check(pDX, IDC_USE5, m_Use5);
	DDX_Text(pDX, IDC_X1, m_X1);
	DDX_Text(pDX, IDC_X2, m_X2);
	DDX_Text(pDX, IDC_X3, m_X3);
	DDX_Text(pDX, IDC_X4, m_X4);
	DDX_Text(pDX, IDC_X5, m_X5);
	DDX_Text(pDX, IDC_ENDCH, m_EndCh);
	DDX_Text(pDX, IDC_AREASGM1, m_AreaSgm1);
	DDX_Text(pDX, IDC_AREASGM2, m_AreaSgm2);
	DDX_Text(pDX, IDC_AREASGM3, m_AreaSgm3);
	DDX_Text(pDX, IDC_AREASGM4, m_AreaSgm4);
	DDX_Text(pDX, IDC_AREASGM5, m_AreaSgm5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGuass, CDialog)
	//{{AFX_MSG_MAP(CDlgGuass)
	ON_BN_CLICKED(IDSAVE, OnSave)
	ON_BN_CLICKED(IDLOAD, OnLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGuass message handlers

void CDlgGuass::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	mGuassData.m_Area[0] = m_Area1;
	mGuassData.m_Area[1] = m_Area2;
	mGuassData.m_Area[2] = m_Area3;
	mGuassData.m_Area[3] = m_Area4;
	mGuassData.m_Area[4] = m_Area5;
	mGuassData.m_AreaSgm[0]=m_AreaSgm1;
	mGuassData.m_AreaSgm[1]=m_AreaSgm2;
	mGuassData.m_AreaSgm[2]=m_AreaSgm3;
	mGuassData.m_AreaSgm[3]=m_AreaSgm4;
	mGuassData.m_AreaSgm[4]=m_AreaSgm5;
	mGuassData.m_CheckSgm[0] = m_CheckSgm1;
	mGuassData.m_CheckSgm[1] = m_CheckSgm2;
	mGuassData.m_CheckSgm[2] = m_CheckSgm3;
	mGuassData.m_CheckSgm[3] = m_CheckSgm4;
	mGuassData.m_CheckSgm[4] = m_CheckSgm5;
	mGuassData.m_CheckX[0] = m_CheckX1;
	mGuassData.m_CheckX[1] = m_CheckX2;
	mGuassData.m_CheckX[2] = m_CheckX3;
	mGuassData.m_CheckX[3] = m_CheckX4;
	mGuassData.m_CheckX[4] = m_CheckX5;
	mGuassData.m_Sgm[0] = m_Sgm1;
	mGuassData.m_Sgm[1] = m_Sgm2;
	mGuassData.m_Sgm[2] = m_Sgm3;
	mGuassData.m_Sgm[3] = m_Sgm4;
	mGuassData.m_Sgm[4] = m_Sgm5;
	mGuassData.startCh = m_StartCh;
	mGuassData.m_Use[0]  =m_Use1;
	mGuassData.m_Use[1]  =m_Use2;
	mGuassData.m_Use[2]  =m_Use3;
	mGuassData.m_Use[3]  =m_Use4;
	mGuassData.m_Use[4]  =m_Use5;
	mGuassData.m_X0[0] = m_X1;
	mGuassData.m_X0[1] = m_X2;
	mGuassData.m_X0[2] = m_X3;
	mGuassData.m_X0[3] = m_X4;
	mGuassData.m_X0[4] = m_X5;
	mGuassData.endCh = m_EndCh;
	fitGass();
//	CDialog::OnOK();
}
void CDlgGuass::UpdateFit()
{
	m_Area1 = int(mGuassData.m_Area[0]*100)/100.;
	m_Area2 = int(mGuassData.m_Area[1]*100)/100.;
	m_Area3 = int(mGuassData.m_Area[2]*100)/100.;
	m_Area4 = int(mGuassData.m_Area[3]*100)/100.;
	m_Area5 = int(mGuassData.m_Area[4]*100)/100.;
	m_AreaSgm1 = int(mGuassData.m_AreaSgm[0]*100)/100.;
	m_AreaSgm2 = int(mGuassData.m_AreaSgm[1]*100)/100.;
	m_AreaSgm3 = int(mGuassData.m_AreaSgm[2]*100)/100.;
	m_AreaSgm4 = int(mGuassData.m_AreaSgm[3]*100)/100.;
	m_AreaSgm5 = int(mGuassData.m_AreaSgm[4]*100)/100.;
	m_CheckSgm1 = mGuassData.m_CheckSgm[0];
	m_CheckSgm2 = mGuassData.m_CheckSgm[1];
	m_CheckSgm3 = mGuassData.m_CheckSgm[2];
	m_CheckSgm4 = mGuassData.m_CheckSgm[3];
	m_CheckSgm5 = mGuassData.m_CheckSgm[4];
	m_CheckX1 = mGuassData.m_CheckX[0];
	m_CheckX2 = mGuassData.m_CheckX[1];
	m_CheckX3 = mGuassData.m_CheckX[2];
	m_CheckX4 = mGuassData.m_CheckX[3];
	m_CheckX5 = mGuassData.m_CheckX[4];
	m_Sgm1 = mGuassData.m_Sgm[0];
	m_Sgm2 = mGuassData.m_Sgm[1];
	m_Sgm3 = mGuassData.m_Sgm[2];
	m_Sgm4 = mGuassData.m_Sgm[3];
	m_Sgm5 = mGuassData.m_Sgm[4];
	m_StartCh = mGuassData.startCh;
	m_Use1 = mGuassData.m_Use[0];
	m_Use2 = mGuassData.m_Use[1];
	m_Use3 = mGuassData.m_Use[2];
	m_Use4 = mGuassData.m_Use[3];
	m_Use5 = mGuassData.m_Use[4];
	m_X1 = mGuassData.m_X0[0];
	m_X2 = mGuassData.m_X0[1];
	m_X3 = mGuassData.m_X0[2];
	m_X4 = mGuassData.m_X0[3];
	m_X5 = mGuassData.m_X0[4];
	m_EndCh = mGuassData.endCh;
	UpdateData(FALSE);
}

void CDlgGuass::OnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	mGuassData.m_Area[0] = m_Area1;
	mGuassData.m_Area[1] = m_Area2;
	mGuassData.m_Area[2] = m_Area3;
	mGuassData.m_Area[3] = m_Area4;
	mGuassData.m_Area[4] = m_Area5;
	mGuassData.m_AreaSgm[0]=m_AreaSgm1;
	mGuassData.m_AreaSgm[1]=m_AreaSgm2;
	mGuassData.m_AreaSgm[2]=m_AreaSgm3;
	mGuassData.m_AreaSgm[3]=m_AreaSgm4;
	mGuassData.m_AreaSgm[4]=m_AreaSgm5;
	mGuassData.m_CheckSgm[0] = m_CheckSgm1;
	mGuassData.m_CheckSgm[1] = m_CheckSgm2;
	mGuassData.m_CheckSgm[2] = m_CheckSgm3;
	mGuassData.m_CheckSgm[3] = m_CheckSgm4;
	mGuassData.m_CheckSgm[4] = m_CheckSgm5;
	mGuassData.m_CheckX[0] = m_CheckX1;
	mGuassData.m_CheckX[1] = m_CheckX2;
	mGuassData.m_CheckX[2] = m_CheckX3;
	mGuassData.m_CheckX[3] = m_CheckX4;
	mGuassData.m_CheckX[4] = m_CheckX5;
	mGuassData.m_Sgm[0] = m_Sgm1;
	mGuassData.m_Sgm[1] = m_Sgm2;
	mGuassData.m_Sgm[2] = m_Sgm3;
	mGuassData.m_Sgm[3] = m_Sgm4;
	mGuassData.m_Sgm[4] = m_Sgm5;
	mGuassData.startCh = m_StartCh;
	mGuassData.m_Use[0]  =m_Use1;
	mGuassData.m_Use[1]  =m_Use2;
	mGuassData.m_Use[2]  =m_Use3;
	mGuassData.m_Use[3]  =m_Use4;
	mGuassData.m_Use[4]  =m_Use5;
	mGuassData.m_X0[0] = m_X1;
	mGuassData.m_X0[1] = m_X2;
	mGuassData.m_X0[2] = m_X3;
	mGuassData.m_X0[3] = m_X4;
	mGuassData.m_X0[4] = m_X5;
	mGuassData.endCh = m_EndCh;
	CFile mfile;
	char temp[160];
	sprintf(temp,"%s\\fit.dat",curDirectory);
	if(mfile.Open(temp,CFile::modeWrite|CFile::modeCreate))
	{
	mfile.Write(&mGuassData,sizeof(CGuassData));
	mfile.Close();
	}
	
}

void CDlgGuass::OnLoad() 
{
	// TODO: Add your control notification handler code here
	CFile mfile;
	char temp[160];
	sprintf(temp,"%s\\fit.dat",curDirectory);
	if(mfile.Open(temp,CFile::modeRead))
	{
	mfile.Read(&mGuassData,sizeof(CGuassData));
	mfile.Close();
	}
	UpdateFit();
}
