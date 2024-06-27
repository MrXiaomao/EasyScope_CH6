// DlgRS232.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgRS232.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern	char	strCom[10][5];
extern	int		nComx;
extern	BYTE		m_TxData[200];
extern	BYTE		*m_RxData;
extern	bool	bOpenRs232,nOpenRs232;
extern	HANDLE		hCom;
extern	CString  usbState;


/////////////////////////////////////////////////////////////////////////////
// CDlgRS232 dialog


CDlgRS232::CDlgRS232(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRS232::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRS232)
	m_strCom = strCom[0];
	//}}AFX_DATA_INIT
}


void CDlgRS232::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRS232)
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON1, mRS232Open);
	DDX_Control(pDX, IDC_BUTTON2, mRS232Close);
}


BEGIN_MESSAGE_MAP(CDlgRS232, CDialog)
	//{{AFX_MSG_MAP(CDlgRS232)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRS232::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgRS232::OnBnClickedButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRS232 message handlers

BOOL CDlgRS232::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i;
	char temp[80];

	CRegKey RegKey;   
    int nCount = 0;   
	CString  pNameList[20];
	CString pPortList[20];
       
    if(RegKey.Open(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm",KEY_QUERY_VALUE|0x100) == ERROR_SUCCESS)   
    {   
        while(true)   
        {   
            char ValueName[_MAX_PATH];   
            unsigned char ValueData[_MAX_PATH];   
            DWORD nValueSize = _MAX_PATH;   
            DWORD nDataSize = _MAX_PATH;   
            DWORD nType;   
               
            if(::RegEnumValue(HKEY(RegKey), nCount, ValueName, &nValueSize, NULL, &nType, ValueData, &nDataSize) == ERROR_NO_MORE_ITEMS)   
            {   
                break;   
            }   
               
            if(pNameList)   
                pNameList[nCount] = ValueName;   
               
            if(pPortList)   
                strcpy(strCom[nCount],(char  *)ValueData);
			m_Combo2.AddString(strCom[nCount]);
            nCount++;   
        }   
    }
       
	m_Combo2.SetCurSel(nComx);
	mRS232Open.EnableWindow(!bOpenRs232);
	mRS232Close.EnableWindow(bOpenRs232);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRS232::OnOK() 
{
	// TODO: Add extra validation here
	nComx = m_Combo2.GetCurSel();
	UpdateData(TRUE);
	CDialog::OnOK();
}


void CDlgRS232::OnBnClickedButton1()//打开串口
{
	// TODO: 在此添加控件通知处理程序代码
	if(bOpenRs232)
	{
		CloseHandle(hCom);
	}
	nComx = m_Combo2.GetCurSel();
	int len=strlen(strCom[nComx]);
	if(len>0)
	{
		OpenRS232();
		mRS232Open.EnableWindow(!bOpenRs232);
		mRS232Close.EnableWindow(bOpenRs232);
	}
}


void CDlgRS232::OnBnClickedButton2()//关闭串口
{
	// TODO: 在此添加控件通知处理程序代码
//	nComx = m_Combo2.GetCurSel();
//	int len=strlen(strCom[nComx]);
	if(bOpenRs232)
	{
		CloseHandle(hCom);
		bOpenRs232=0;
		mRS232Open.EnableWindow(!bOpenRs232);
		mRS232Close.EnableWindow(bOpenRs232);
		CString str;
		usbState.Format("串口%s关闭!",strCom[nComx]);
//		usbState="串口关闭!";
	}
}
void  CDlgRS232::OpenRS232()
{
		DCB dcb;
		hCom = INVALID_HANDLE_VALUE;
		CString strcomOpen="\\\\.\\";
		strcomOpen+=strCom[nComx];
		hCom = CreateFile(strcomOpen.GetBuffer(100),GENERIC_READ|GENERIC_WRITE,0,NULL,
			OPEN_EXISTING,0,NULL);
		if(hCom==INVALID_HANDLE_VALUE)
		{
			CString str;
			usbState.Format("打开串口%s 错误",strCom[nComx]);
			bOpenRs232=0;
			nOpenRs232=0;
		}
		 //设置输入输出缓冲区
		else
		{
			bOpenRs232=1;
			SetupComm(hCom,16000,100);
			DCB dcb;
			GetCommState(hCom,&dcb);
//			dcb.BaudRate = CBR_115200;
			dcb.BaudRate = 256000;
			dcb.ByteSize = 8;
			dcb.StopBits = ONESTOPBIT;
			dcb.fParity = 0;   //无校验
			dcb.fAbortOnError = 0;
			dcb.Parity = NOPARITY;
			dcb.fBinary = 1;                                              
			SetCommState(hCom,&dcb);
			usbState.Format("打开串口%s",strCom[nComx]);
		}
}
