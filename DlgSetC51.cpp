// DlgSetC51.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgSetC51.h"
#include "define.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "mcbc.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "DisplaySpm.h"
#include "math.h"
#include "DlgSetRoi.h"

extern CMcbcView *w1[4];

/////////////////////////////////////////////////////////////////////////////
// CDlgSetC51 dialog
extern	long	*pAdcBuf[8];
C51DATA c51Data;
extern	long	*pMemBuf[8];
int brinv(double *a, int n) ;

CDlgSetC51::CDlgSetC51(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetC51::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetC51)
	m_AlarmPaper = c51Data.m_AlarmPaper;
	m_AlarmFlux = c51Data.m_AlarmFlux;
	m_AlarmBeta = c51Data.m_AlarmBeta;
	m_AlarmAlpha= c51Data.m_AlarmAlpha;
	m_Alpha1 = c51Data.m_Alpha1;
	m_Alpha2 = c51Data.m_Alpha2;
	m_BetaAcc =c51Data.m_BetaAcc;
	m_BetaEff = c51Data.m_BetaEff;
	m_BetaFile = c51Data.m_BetaFile;
	m_Beta1 = c51Data.m_Beta1;
	m_Beta2 = c51Data.m_Beta2;
	m_BetaH = c51Data.m_BetaH;
	m_BetaL = c51Data.m_BetaL;
	m_Flux1 = c51Data.m_Flux1;
	m_Flux2 = c51Data.m_Flux2;
	m_Paper1 = c51Data.m_Paper1;
	m_Pu239File = c51Data.m_Pu239File;
	m_Pu239S1 = c51Data.m_Pu239S1;
	m_Pu239S2 = c51Data.m_Pu239S2;
	m_Pu239S3 = c51Data.m_Pu239S3;
	m_Pu239S4 = c51Data.m_Pu239S4;
	m_Pu239S5 = c51Data.m_Pu239S5;
	m_Pu239H = c51Data.m_Pu239H;
	m_Pu239L = c51Data.m_Pu239L;
	m_Rn220File = c51Data.m_Rn220File;
	m_Rn220S1 = c51Data.m_Rn220S1;
	m_Rn220S2 = c51Data.m_Rn220S2;
	m_Rn220S3 = c51Data.m_Rn220S3;
	m_Rn220S4 = c51Data.m_Rn220S4;
	m_Rn220S5 = c51Data.m_Rn220S5;
	m_Rn222S1 = c51Data.m_Rn222S1;
	m_Rn222S2 = c51Data.m_Rn222S2;
	m_Rn222S3 = c51Data.m_Rn222S3;
	m_Rn222S4 = c51Data.m_Rn222S4;
	m_Rn222S5 = c51Data.m_Rn222S5;
	m_U235S1 = c51Data.m_U235S1;
	m_U235S2 = c51Data.m_U235S2;
	m_U235S3 = c51Data.m_U235S3;
	m_U235S4 = c51Data.m_U235S4;
	m_U235S5 = c51Data.m_U235S5;
	m_Rn220H = c51Data.m_Rn220H;
	m_Rn220L =c51Data.m_Rn220L;
	m_Rn222File = c51Data.m_Rn222File;
	m_Rn222H = c51Data.m_Rn222H;
	m_Rn222L = c51Data.m_Rn222L;
	m_U235File = c51Data.m_U235File;
	m_U235H = c51Data.m_U235H;
	m_U235L = c51Data.m_U235L;
	m_S1 = 0.0f;
	m_S2 = 0.0f;
	m_S3 = 0.0f;
	m_S4 = 0.0f;
	m_S5 = 0.0f;
	m_Address = c51Data.m_Address;
	m_Period = c51Data.m_Period;
	m_BendiFile =c51Data.m_BendiFile;
	m_BendiS1 = c51Data.m_BendiS1;
	m_BendiS2 = c51Data.m_BendiS2;
	m_BendiS3 =c51Data.m_BendiS3;
	m_BendiS4 = c51Data.m_BendiS4;
	m_BendiS5 = c51Data.m_BendiS5;
	//}}AFX_DATA_INIT
}


void CDlgSetC51::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetC51)
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_Check(pDX, IDC_ALAEM_PAPER, m_AlarmPaper);
	DDX_Check(pDX, IDC_ALARM_FLUX, m_AlarmFlux);
	DDX_Check(pDX, IDC_ALARM_BETA, m_AlarmBeta);
	DDX_Check(pDX, IDC_ALARM_ALPHA, m_AlarmAlpha);
	DDX_Text(pDX, IDC_ALPHA1, m_Alpha1);
	DDX_Text(pDX, IDC_ALPHA2, m_Alpha2);
	DDX_Text(pDX, IDC_BETA_ACC, m_BetaAcc);
	DDX_Text(pDX, IDC_BETA_EFF, m_BetaEff);
	DDX_Text(pDX, IDC_BETA_FILE, m_BetaFile);
	DDX_Text(pDX, IDC_BETA1, m_Beta1);
	DDX_Text(pDX, IDC_BETA2, m_Beta2);
	DDX_Text(pDX, IDC_BETAH, m_BetaH);
	DDX_Text(pDX, IDC_BETAL, m_BetaL);
	DDX_Text(pDX, IDC_FLUX1, m_Flux1);
	DDX_Text(pDX, IDC_FLUX2, m_Flux2);
	DDX_Text(pDX, IDC_PAPER1, m_Paper1);
	DDX_Text(pDX, IDC_PU239_FILE, m_Pu239File);
	DDX_Text(pDX, IDC_PU239_S1, m_Pu239S1);
	DDX_Text(pDX, IDC_PU239_S2, m_Pu239S2);
	DDX_Text(pDX, IDC_PU239_S3, m_Pu239S3);
	DDX_Text(pDX, IDC_PU239_S4, m_Pu239S4);
	DDX_Text(pDX, IDC_PU239_S5, m_Pu239S5);
	DDX_Text(pDX, IDC_PU239H, m_Pu239H);
	DDX_Text(pDX, IDC_PU239L, m_Pu239L);
	DDX_Text(pDX, IDC_RN220_FILE, m_Rn220File);
	DDX_Text(pDX, IDC_RN220_S1, m_Rn220S1);
	DDX_Text(pDX, IDC_RN220_S2, m_Rn220S2);
	DDX_Text(pDX, IDC_RN220_S3, m_Rn220S3);
	DDX_Text(pDX, IDC_RN220_S4, m_Rn220S4);
	DDX_Text(pDX, IDC_RN220_S5, m_Rn220S5);
	DDX_Text(pDX, IDC_RN222_S1, m_Rn222S1);
	DDX_Text(pDX, IDC_RN222_S2, m_Rn222S2);
	DDX_Text(pDX, IDC_RN222_S3, m_Rn222S3);
	DDX_Text(pDX, IDC_RN222_S4, m_Rn222S4);
	DDX_Text(pDX, IDC_RN222_S5, m_Rn222S5);
	DDX_Text(pDX, IDC_U235_S1, m_U235S1);
	DDX_Text(pDX, IDC_U235_S2, m_U235S2);
	DDX_Text(pDX, IDC_U235_S3, m_U235S3);
	DDX_Text(pDX, IDC_U235_S4, m_U235S4);
	DDX_Text(pDX, IDC_U235_S5, m_U235S5);
	DDX_Text(pDX, IDC_RN220H, m_Rn220H);
	DDX_Text(pDX, IDC_RN220L, m_Rn220L);
	DDX_Text(pDX, IDC_RN222_FILE, m_Rn222File);
	DDX_Text(pDX, IDC_RN222H, m_Rn222H);
	DDX_Text(pDX, IDC_RN222L, m_Rn222L);
	DDX_Text(pDX, IDC_U235_FILE, m_U235File);
	DDX_Text(pDX, IDC_U235H, m_U235H);
	DDX_Text(pDX, IDC_U235L, m_U235L);
	DDX_Text(pDX, IDC_S1, m_S1);
	DDX_Text(pDX, IDC_S2, m_S2);
	DDX_Text(pDX, IDC_S3, m_S3);
	DDX_Text(pDX, IDC_S4, m_S4);
	DDX_Text(pDX, IDC_S5, m_S5);
	DDX_Text(pDX, IDC_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_PERIOD, m_Period);
	DDX_Text(pDX, IDC_BENDI_FILE, m_BendiFile);
	DDX_Text(pDX, IDC_BENDI_S1, m_BendiS1);
	DDX_Text(pDX, IDC_BENDI_S2, m_BendiS2);
	DDX_Text(pDX, IDC_BENDI_S3, m_BendiS3);
	DDX_Text(pDX, IDC_BENDI_S4, m_BendiS4);
	DDX_Text(pDX, IDC_BENDI_S5, m_BendiS5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetC51, CDialog)
	//{{AFX_MSG_MAP(CDlgSetC51)
	ON_BN_CLICKED(IDC_BTN_RN220, OnBtnRn220)
	ON_BN_CLICKED(IDC_BTN_RN222, OnBtnRn222)
	ON_BN_CLICKED(IDC_BTN_PU239, OnBtnPu239)
	ON_BN_CLICKED(IDC_BTN_U235, OnBtnU235)
	ON_BN_CLICKED(IDC_BTN_BETA, OnBtnBeta)
	ON_BN_CLICKED(IDC_BTN_CAL, OnBtnCal)
	ON_BN_CLICKED(IDC_BTN_CAL2, OnBtnCal2)
	ON_BN_CLICKED(IDC_BTN_BENDI, OnBtnBendi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetC51 message handlers

void CDlgSetC51::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	c51Data.m_AlarmPaper=m_AlarmPaper;
	c51Data.m_AlarmFlux=m_AlarmFlux;
	c51Data.m_AlarmBeta=m_AlarmBeta;
	c51Data.m_AlarmAlpha=m_AlarmAlpha;
	c51Data.m_Alpha1=m_Alpha1;
	c51Data.m_Alpha2=m_Alpha2;
	c51Data.m_BetaAcc =m_BetaAcc;
	c51Data.m_BetaEff=m_BetaEff;
	strcpy(c51Data.m_BetaFile,m_BetaFile.GetBuffer(200));
	c51Data.m_Beta1=m_Beta1;
	c51Data.m_Beta2=m_Beta2;
	c51Data.m_BetaH=m_BetaH;
	c51Data.m_BetaL=m_BetaL;
	c51Data.m_Flux1=m_Flux1;
	c51Data.m_Flux2=m_Flux2;
	c51Data.m_Paper1=m_Paper1;
	strcpy(c51Data.m_Pu239File,m_Pu239File.GetBuffer(200));
	c51Data.m_Pu239S1=m_Pu239S1;
	c51Data.m_Pu239S2=m_Pu239S2;
	c51Data.m_Pu239S3=m_Pu239S3;
	c51Data.m_Pu239S4=m_Pu239S4;
	c51Data.m_Pu239S5=m_Pu239S5;
	c51Data.m_Pu239H=m_Pu239H;
	c51Data.m_Pu239L=m_Pu239L;
	strcpy(c51Data.m_Rn220File,m_Rn220File.GetBuffer(200));
	c51Data.m_Rn220S1=m_Rn220S1;
	c51Data.m_Rn220S2=m_Rn220S2;
	c51Data.m_Rn220S3=m_Rn220S3;
	c51Data.m_Rn220S4=m_Rn220S4;
	c51Data.m_Rn220S5=m_Rn220S5;
	c51Data.m_Rn222S1=m_Rn222S1;
	c51Data.m_Rn222S2=m_Rn222S2;
	c51Data.m_Rn222S3=m_Rn222S3;
	c51Data.m_Rn222S4=m_Rn222S4;
	c51Data.m_Rn222S5=m_Rn222S5;
	c51Data.m_U235S1=m_U235S1;
	c51Data.m_U235S2=m_U235S2;
	c51Data.m_U235S3=m_U235S3;
	c51Data.m_U235S4=m_U235S4;
	c51Data.m_U235S5=m_U235S5;
	c51Data.m_Rn220H=m_Rn220H;
	c51Data.m_Rn220L =m_Rn220L;
	strcpy(c51Data.m_Rn222File,m_Rn222File.GetBuffer(200));
	c51Data.m_Rn222H=m_Rn222H;
	c51Data.m_Rn222L=m_Rn222L;
	strcpy(c51Data.m_U235File,m_U235File.GetBuffer(200));
	c51Data.m_U235H=m_U235H;
	c51Data.m_U235L=m_U235L;	
	c51Data.baudrate=m_Combo2.GetCurSel();
	c51Data.m_Period=m_Period;
	c51Data.m_Address=m_Address;
	strcpy(c51Data.m_BendiFile,m_BendiFile.GetBuffer(200));
	c51Data.m_BendiS1 = m_BendiS1;
	c51Data.m_BendiS2 = m_BendiS2;
	c51Data.m_BendiS3 =m_BendiS3;
	c51Data.m_BendiS4 = m_BendiS4;
	c51Data.m_BendiS5 = m_BendiS5;
	CDialog::OnOK();
}

void CDlgSetC51::OnBtnRn220() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_Rn220File=str;
			UpdateData(FALSE);
		}
	
}

void CDlgSetC51::OnBtnRn222() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_Rn222File=str;
			UpdateData(FALSE);
		}
	
}

void CDlgSetC51::OnBtnPu239() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_Pu239File=str;
			UpdateData(FALSE);
		}
	
}

void CDlgSetC51::OnBtnU235() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_U235File=str;
			UpdateData(FALSE);
		}
	
}

void CDlgSetC51::OnBtnBeta() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_BetaFile=str;
			UpdateData(FALSE);
		}
	
}

void CDlgSetC51::OnBtnCal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFile mfile;
	int *pTempBuf;
	pTempBuf=new int[2048];
	ChnHead chnHead;
	ChnEnd  chnEnd;
	if(mfile.Open(m_Rn220File.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		m_Rn220S1=0;
		m_Rn220S2=0;
		m_Rn220S3=0;
		m_Rn220S4=0;
		m_Rn220S5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_Rn220S1+=pTempBuf[i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_Rn220S2+=pTempBuf[i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_Rn220S3+=pTempBuf[i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_Rn220S4+=pTempBuf[i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_Rn220S5+=pTempBuf[i];
		totCount1=0;
		for(i=32;i<=2000;i++)
			totCount1+=pTempBuf[i];
	}
	if(mfile.Open(m_Rn222File.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		m_Rn222S1=0;
		m_Rn222S2=0;
		m_Rn222S3=0;
		m_Rn222S4=0;
		m_Rn222S5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_Rn222S1+=pTempBuf[i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_Rn222S2+=pTempBuf[i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_Rn222S3+=pTempBuf[i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_Rn222S4+=pTempBuf[i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_Rn222S5+=pTempBuf[i];
		totCount2=0;
		for(i=32;i<=2000;i++)
			totCount2+=pTempBuf[i];
	}
	if(mfile.Open(m_Pu239File.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		m_Pu239S1=0;
		m_Pu239S2=0;
		m_Pu239S3=0;
		m_Pu239S4=0;
		m_Pu239S5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_Pu239S1+=pTempBuf[i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_Pu239S2+=pTempBuf[i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_Pu239S3+=pTempBuf[i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_Pu239S4+=pTempBuf[i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_Pu239S5+=pTempBuf[i];
		totCount3=0;
		for(i=32;i<=2000;i++)
			totCount3+=pTempBuf[i];
	}
	if(mfile.Open(m_U235File.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		m_U235S1=0;
		m_U235S2=0;
		m_U235S3=0;
		m_U235S4=0;
		m_U235S5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_U235S1+=pTempBuf[i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_U235S2+=pTempBuf[i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_U235S3+=pTempBuf[i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_U235S4+=pTempBuf[i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_U235S5+=pTempBuf[i];
		totCount4=0;
		for(i=32;i<=2000;i++)
			totCount4+=pTempBuf[i];
	}
	if(mfile.Open(m_BendiFile.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		m_BendiS1=0;
		m_BendiS2=0;
		m_BendiS3=0;
		m_BendiS4=0;
		m_BendiS5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_BendiS1+=pTempBuf[i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_BendiS2+=pTempBuf[i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_BendiS3+=pTempBuf[i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_BendiS4+=pTempBuf[i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_BendiS5+=pTempBuf[i];
		m_BendiS1/=liveTime;
		m_BendiS2/=liveTime;
		m_BendiS3/=liveTime;
		m_BendiS4/=liveTime;
		m_BendiS5/=liveTime;
		totCount1/=liveTime;
		for(i=32;i<=2000;i++)
			totCount1+=pTempBuf[i];
	}
	if(mfile.Open(m_BetaFile.GetBuffer(150),CFile::modeRead))
	{
		mfile.Read(&chnHead,32);
		mfile.Read(pTempBuf,4*2048);
		mfile.Read(&chnEnd,512);
		mfile.Close();
		float liveTime=chnHead.liveTime/50.;
		int i;
		float betaArea=0;
		for(i=m_BetaL;i<=m_BetaH;i++)
			betaArea+=pTempBuf[i];
		m_BetaEff=betaArea/m_BetaAcc*100;
	}

	delete []pTempBuf;
	w1[0]->RoiNum=5;
	w1[0]->RoiStart[0]=m_BetaL;
	w1[0]->RoiEnd[0]=m_BetaH;
	w1[0]->RoiStart[1]=m_U235L;
	w1[0]->RoiEnd[1]=m_U235H;
	w1[0]->RoiStart[2]=m_Pu239L;
	w1[0]->RoiEnd[2]=m_Pu239H;
	w1[0]->RoiStart[3]=m_Rn222L;
	w1[0]->RoiEnd[3]=m_Rn222H;
	w1[0]->RoiStart[4]=m_Rn220L;
	w1[0]->RoiEnd[4]=m_Rn220H;
	UpdateData(FALSE);
}

void CDlgSetC51::OnBtnCal2() 
{
	// TODO: Add your control notification handler code here
	int i;
	OnBtnCal();
		m_S1=0;
		m_S2=0;
		m_S3=0;
		m_S4=0;
		m_S5=0;
		for(i=m_Rn220L;i<=m_Rn220H;i++)
			m_S1+=pMemBuf[0][i];
		for(i=m_Rn222L;i<=m_Rn222H;i++)
			m_S2+=pMemBuf[0][i];
		for(i=m_Pu239L;i<=m_Pu239H;i++)
			m_S3+=pMemBuf[0][i];
		for(i=m_U235L;i<=m_U235H;i++)
			m_S4+=pMemBuf[0][i];
		for(i=m_BetaL;i<=m_BetaH;i++)
			m_S5+=pMemBuf[0][i];
		double *pa=new double[16];
		*(pa+0)=m_Rn220S1;
		*(pa+1)=m_Rn222S1;
		*(pa+2)=m_Pu239S1;
		*(pa+3)=m_U235S1;
		*(pa+4)=m_Rn220S2;
		*(pa+5)=m_Rn222S2;
		*(pa+6)=m_Pu239S2;
		*(pa+7)=m_U235S2;
		*(pa+8)=m_Rn220S3;
		*(pa+9)=m_Rn222S3;
		*(pa+10)=m_Pu239S3;
		*(pa+11)=m_U235S3;
		*(pa+12)=m_Rn220S4;
		*(pa+13)=m_Rn222S4;
		*(pa+14)=m_Pu239S4;
		*(pa+15)=m_U235S4;
		brinv(pa,4);
		double s1,s2,s3,s4;
		double a1,a2,a3,a4;
		s1=*(pa+0)*m_S1+*(pa+1)*m_S2+*(pa+2)*m_S3+*(pa+3)*m_S4;
		s2=*(pa+4)*m_S1+*(pa+5)*m_S2+*(pa+6)*m_S3+*(pa+7)*m_S4;
		s3=*(pa+8)*m_S1+*(pa+9)*m_S2+*(pa+10)*m_S3+*(pa+11)*m_S4;
		s4=*(pa+12)*m_S1+*(pa+13)*m_S2+*(pa+14)*m_S3+*(pa+15)*m_S4;
		delete []pa;
		m_S1=s1*totCount1;
		m_S2=s2*totCount2;
		m_S3=s3*totCount3;
		m_S4=s4*totCount4;
		m_S5=m_S5-s1*c51Data.m_Rn220S5-s2*c51Data.m_Rn222S5
			  -s3*c51Data.m_Pu239S5-s4*c51Data.m_U235S5;
	UpdateData(FALSE);
}

BOOL CDlgSetC51::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Combo2.SetCurSel(c51Data.baudrate);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetC51::OnBtnBendi() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"ORTEC 多道谱格式(*.chn)|*.chn|PCA 多道谱格式(*.spm)|*.spm||");
		char curpath[150];
		CString str;
		::GetCurrentDirectory(150,curpath);
		dlg.m_ofn.lpstrInitialDir=curpath;
		if(dlg.DoModal()==IDOK)
		{
			str = dlg.GetPathName();
			m_BendiFile=str;
			UpdateData(FALSE);
		}
	
}
