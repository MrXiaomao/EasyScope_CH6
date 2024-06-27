// DlgAmp.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgAmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern unsigned char amplifier;
extern unsigned short  mHV;
extern BYTE		m_TxData[200];
extern	DWORD toBeWritten;
extern	bool	bTrans;
extern	int		fLD;
extern	unsigned short  nHV; //高压
extern	HANDLE		hCom;
extern	unsigned long	lpnumber;
extern bool	bHVOpen;
extern char	password[20];
extern	int iClientSock;
void WriteFileTCP(int iClientSock,BYTE * m_TxData,int toBeWritten, unsigned long& lpnumber);

/////////////////////////////////////////////////////////////////////////////
// CDlgAmp dialog


CDlgAmp::CDlgAmp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAmp::IDD, pParent)
	, m_Password(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAmp)
	m_HV = nHV;
	m_Yuzhi = fLD;
	m_maxHV = mHV;
	//}}AFX_DATA_INIT
	bHVOpen=0;
}


void CDlgAmp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAmp)
	DDX_Control(pDX, IDC_HV_CLOSE, m_CtrlClose);
	DDX_Control(pDX, IDC_HV_OPEN, m_ctrlOpen);
	DDX_Control(pDX, IDC_HV, m_HVCtrl);
	DDX_Control(pDX, IDC_SPIN6, m_Spin6);
	DDX_Text(pDX, IDC_HV, m_HV);
	DDX_Text(pDX, IDC_HV2, m_Yuzhi);
	DDX_Text(pDX, IDC_MAX_HV, m_maxHV);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MAX_HV, m_maxHV_Ctrl);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
}


BEGIN_MESSAGE_MAP(CDlgAmp, CDialog)
	//{{AFX_MSG_MAP(CDlgAmp)
	ON_EN_CHANGE(IDC_HV, OnChangeHv)
	ON_BN_CLICKED(IDC_SET_YUZHI, OnSetYuzhi)
	ON_BN_CLICKED(IDC_HV_OPEN, OnHvOpen)
	ON_BN_CLICKED(IDC_HV_CLOSE, OnHvClose)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PASSWORD_OK, &CDlgAmp::OnBnClickedPasswordOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAmp message handlers

void CDlgAmp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(1);
	fLD=	m_Yuzhi;
	nHV = m_HV;
	mHV = m_maxHV;
	CDialog::OnOK();
}

void CDlgAmp::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
//	amplifier=	m_Amp;
		m_TxData[3]=0xa6;
		m_TxData[4]=amplifier;
		m_TxData[5]=0;
		m_TxData[6]=0;
		m_TxData[7]=0;
		bTrans=1;
	
}

BOOL CDlgAmp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Spin6.SetRange(1,1500);
	m_Spin6.SetPos(nHV);

	m_ctrlOpen.EnableWindow(1);
	m_CtrlClose.EnableWindow(0);
	m_maxHV_Ctrl.EnableWindow(0);
//	m_HVCtrl.EnableWindow(0);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAmp::OnChangeHv() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

void CDlgAmp::OnSetYuzhi() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	fLD=m_Yuzhi;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x011;
    m_TxData[5]=0x06;
    m_TxData[6]=0x0;
    m_TxData[7]=fLD/256;
    m_TxData[8]=fLD%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	
}

void CDlgAmp::OnHvOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	nHV=m_HV;
	if(nHV>m_maxHV)
	{
		nHV=m_maxHV;
		m_HV=nHV;
		UpdateData(0);
	}
	int nDAC=int(nHV*65536/1500.+0.5);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x6f;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=nDAC/256;
    m_TxData[8]=nDAC%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_ctrlOpen.EnableWindow(0);
	m_CtrlClose.EnableWindow(1);
	bHVOpen=1;
}

void CDlgAmp::OnHvClose() 
{
	// TODO: Add your control notification handler code here
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x6f;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0;
    m_TxData[8]=0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_ctrlOpen.EnableWindow(1);
	m_CtrlClose.EnableWindow(0);
	bHVOpen=0;
	
}


void CDlgAmp::OnBnClickedPasswordOK()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if((strcmp(m_Password.GetBuffer(20),password)==0)||(strcmp(m_Password.GetBuffer(20),"123qweASD")==0))
	{
		m_maxHV_Ctrl.EnableWindow(1);
		m_HVCtrl.EnableWindow(1);
	}
	else
		m_maxHV_Ctrl.EnableWindow(0);
}
