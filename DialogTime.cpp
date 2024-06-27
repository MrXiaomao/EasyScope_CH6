// DialogTime.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DialogTime.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern	unsigned	int	nPresetTime[6];
extern	bool	bPresetReal[4];
extern  BYTE		m_TxData[200];
extern  BYTE		*m_RxData;
extern CString  usbState;
extern HANDLE		hCom;
extern unsigned long	lpnumber;
extern	DWORD toBeWritten;
bool	bAdcWorkOn[4];
extern  bool	bHVOpen;
extern	int iClientSock;
void WriteFileTCP(int iClientSock,BYTE * m_TxData,int toBeWritten, unsigned long& lpnumber);

/////////////////////////////////////////////////////////////////////////////
// CDialogTime dialog

extern	int	mAdcGain;
CDialogTime::CDialogTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTime::IDD, pParent)
	, b50R(!bAdcWorkOn[0])
{
	//{{AFX_DATA_INIT(CDialogTime)
	m_PresetHV = nPresetTime[0];
	//}}AFX_DATA_INIT
}


void CDialogTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTime)
	DDX_Text(pDX, IDC_EDIT1, m_PresetHV);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON2, mHVClose);
	DDX_Control(pDX, IDC_BUTTON1, mHVOpen);
	DDX_Radio(pDX, IDC_RADIO1, b50R);
}


BEGIN_MESSAGE_MAP(CDialogTime, CDialog)
	//{{AFX_MSG_MAP(CDialogTime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogTime::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogTime::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CDialogTime::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO14, &CDialogTime::OnBnClickedRadio14)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTime message handlers

BOOL CDialogTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	mHVClose.EnableWindow(0);

	return TRUE; 
}

void CDialogTime::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	nPresetTime[0]=m_PresetHV;
	bAdcWorkOn[0] = b50R;
	CDialog::OnOK();
}


void CDialogTime::OnBnClickedButton1()//开高压
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	nPresetTime[0]=m_PresetHV;
	int nDAC=int(m_PresetHV*65536/1500.+0.5);
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
	bHVOpen=1;
	mHVOpen.EnableWindow(!bHVOpen);
	mHVClose.EnableWindow(bHVOpen);
}


void CDialogTime::OnBnClickedButton2()//关高压
{
	// TODO: 在此添加控件通知处理程序代码
	nPresetTime[0]=m_PresetHV;
	int nDAC=int(m_PresetHV*65536/1500.+0.5);
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
	bHVOpen=0;
	mHVOpen.EnableWindow(!bHVOpen);
	mHVClose.EnableWindow(bHVOpen);

}


void CDialogTime::OnBnClickedRadio1()//低阻
{
	// TODO: 在此添加控件通知处理程序代码
	b50R=1;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x07;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	bAdcWorkOn[0] = b50R;
}


void CDialogTime::OnBnClickedRadio14()//高阻
{
	// TODO: 在此添加控件通知处理程序代码
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x07;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0;
    m_TxData[8]=0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	b50R=0;
	bAdcWorkOn[0] = b50R;
}
