// DlgBaseLine.cpp : 实现文件
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgBaseLine.h"
#include "afxdialogex.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "Function.h"
#include "define.h"

extern CEnergyKedu energyKedu[4];
extern	CMcbcView	*w1[4];
// CDlgBaseLine 对话框

IMPLEMENT_DYNAMIC(CDlgBaseLine, CDialog)

CDlgBaseLine::CDlgBaseLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBaseLine::IDD, pParent)
{
	m_Channel = 0;
	m_Energy = 0.0f;
	adcNum=1;
	m_fitMode = energyKedu[1].fitMode;

}

CDlgBaseLine::~CDlgBaseLine()
{
}

void CDlgBaseLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ENERGY, m_CtrlEn);
	DDX_Control(pDX, ID_CHANNEL, m_CtrlCh);
	DDX_Text(pDX, ID_CHANNEL, m_Channel);
	DDX_Text(pDX, ID_ENERGY, m_Energy);
	DDX_Radio(pDX, IDC_RADIO2, m_fitMode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST1, m_List1);
}


BEGIN_MESSAGE_MAP(CDlgBaseLine, CDialog)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
END_MESSAGE_MAP()


// CDlgBaseLine 消息处理程序
BOOL CDlgBaseLine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	int i,j;
//	for(i=0;i<1;i++)
	{
	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List1.GetClientRect (&rect);
	m_List1.InsertColumn (0, "编号", LVCFMT_LEFT,rect.Width() * 0.25);
	m_List1.InsertColumn (1, "ADC值", LVCFMT_LEFT,rect.Width() * 0.3);
	m_List1.InsertColumn (2, "幅度", LVCFMT_LEFT,rect.Width() * 0.45);
	}
	char temp[80];
	j=1;
	//	for(j=0;j<4;j++)
	{
	for(int i=0;i<energyKedu[j].Enum;i++)
	{
		sprintf(temp,"%d",i+1);
		m_List1.InsertItem(i,temp);
		sprintf(temp,"%4.0f",energyKedu[j].channel[i]);
		m_List1.SetItemText(i,1,temp);
		sprintf(temp,"%8.3f",energyKedu[j].energy[i]);
		m_List1.SetItemText(i,2,temp);
	}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgBaseLine::OnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char temp[20];
	int i,j;
	int maxn=energyKedu[1].Enum;
	m_CtrlCh.GetWindowText(temp,10);
    energyKedu[1].channel[maxn]=atof(temp);
	m_CtrlEn.GetWindowText(temp,20);
	energyKedu[1].energy[maxn]=atof(temp);
	energyKedu[1].Enum++;
	if(maxn==0)
	{
		sprintf(temp,"%d",maxn+1);
		m_List1.InsertItem(maxn,temp);
		sprintf(temp,"%4.0f",energyKedu[1].channel[maxn]);
		m_List1.SetItemText(maxn,1,temp);
		sprintf(temp,"%8.3f",energyKedu[1].energy[maxn]);
		m_List1.SetItemText(maxn,2,temp);
	}
	else if(energyKedu[1].channel[maxn-1]<energyKedu[1].channel[maxn])
	{
		sprintf(temp,"%d",maxn+1);
		m_List1.InsertItem(maxn,temp);
		sprintf(temp,"%4.0f",energyKedu[1].channel[maxn]);
		m_List1.SetItemText(maxn,1,temp);
		sprintf(temp,"%8.3f",energyKedu[1].energy[maxn]);
		m_List1.SetItemText(maxn,2,temp);
	}
	else
	{
		for(i=0;i<maxn;i++)
		{
			if(energyKedu[1].channel[maxn]<energyKedu[1].channel[i])
				break;
		}
		int temp1=energyKedu[1].channel[maxn];
		int temp2=energyKedu[1].energy[maxn];
		for(j=maxn;j>i;j--)
		{
			energyKedu[1].channel[j]=energyKedu[1].channel[j-1];
			energyKedu[1].energy[j]=energyKedu[1].energy[j-1];
		}
		energyKedu[1].channel[i]=temp1;
		energyKedu[1].energy[i]=temp2;
	}
	m_List1.DeleteAllItems();
	for(i=0;i<maxn+1;i++)
	{
		sprintf(temp,"%d",i+1);
		m_List1.InsertItem(i,temp);
		sprintf(temp,"%4.0f",energyKedu[1].channel[i]);
		m_List1.SetItemText(i,1,temp);
		sprintf(temp,"%8.3f",energyKedu[1].energy[i]);
		m_List1.SetItemText(i,2,temp);
	}
}

void CDlgBaseLine::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List1.GetFirstSelectedItemPosition();
	if(pos)
	{
		 int nItem = m_List1.GetNextSelectedItem(pos);
		 char temp[20];
		 m_CtrlCh.GetWindowText(temp,20);
		 m_List1.SetItemText(nItem,1,temp);
		 energyKedu[1].channel[nItem]=atof(temp);
		 m_CtrlEn.GetWindowText(temp,20);
		 m_List1.SetItemText(nItem,2,temp);
		 energyKedu[1].energy[nItem]=atof(temp);
	}
}

void CDlgBaseLine::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List1.GetFirstSelectedItemPosition();
	int i,nSelect;
	char temp[80];
	if(pos)
	{
	    int nItem = m_List1.GetNextSelectedItem(pos);
		m_List1.DeleteItem(nItem);
		energyKedu[1].Enum--;
		for(i=nItem;i<energyKedu[1].Enum;i++)
		{
			energyKedu[1].channel[i]=energyKedu[1].channel[i+1];
		}
		m_List1.DeleteAllItems();
		for(i=0;i<energyKedu[1].Enum;i++)
		{
			sprintf(temp,"%d",i+1);
			m_List1.InsertItem(i,temp);
			sprintf(temp,"%4.0f",energyKedu[1].channel[i]);
			m_List1.SetItemText(i,1,temp);
			sprintf(temp,"%8.3f",energyKedu[1].energy[i]);
			m_List1.SetItemText(i,2,temp);
		}
	}
	
}
void CDlgBaseLine::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_fitMode=0;
	energyKedu[1].fitMode=0;
}

void CDlgBaseLine::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_fitMode=1;
	energyKedu[1].fitMode=1;
}


void CDlgBaseLine::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List1.GetFirstSelectedItemPosition();
	if(pos)
	{
		int adcNum=0;
		 int nItem = m_List1.GetNextSelectedItem(pos);
		 char temp[20];
		 m_List1.GetItemText(nItem,1,temp,10);
		 m_CtrlCh.SetWindowText(temp);
		 w1[adcNum]->dispspm.CursorChn=atof(temp);
		 m_List1.GetItemText(nItem,2,temp,10);
		 m_CtrlEn.SetWindowText(temp);
	     w1[adcNum]->DispCursor();
	}

	*pResult = 0;
}


void CDlgBaseLine::OnOK() 
{
	// TODO: Add extra validation here
	CFunction fun;
	fun.pX=energyKedu[1].channel;
	fun.pY=energyKedu[1].energy;
	fun.adcNum=1;
	if(energyKedu[1].fitMode==0)
		fun.nDimension=2;
	else
		fun.nDimension=3;
	fun.CalMain();
	energyKedu[1].para[0]=fun.para[0];
	energyKedu[1].para[1]=fun.para[1];
	energyKedu[1].para[2]=fun.para[2];
	CDialog::OnOK();
}
