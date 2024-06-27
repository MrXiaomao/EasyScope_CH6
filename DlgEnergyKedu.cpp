// DlgEnergyKedu.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgEnergyKedu.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "define.h"

extern CEnergyKedu energyKedu[4];
extern	CMcbcView	*w1[4];
/////////////////////////////////////////////////////////////////////////////
// CDlgEnergyKedu dialog


CDlgEnergyKedu::CDlgEnergyKedu(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnergyKedu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnergyKedu)
	m_Channel = 0;
	m_Energy = 0.0f;
	m_ADC = "ADC1";
	//}}AFX_DATA_INIT
	adcNum=0;
	m_fitMode = energyKedu[0].fitMode;
}


void CDlgEnergyKedu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnergyKedu)
	DDX_Control(pDX, ID_ENERGY, m_CtrlEn);
	DDX_Control(pDX, ID_CHANNEL, m_CtrlCh);
	DDX_Text(pDX, ID_CHANNEL, m_Channel);
	DDX_Text(pDX, ID_ENERGY, m_Energy);
	DDX_Text(pDX, IDC_STATIC_ADC, m_ADC);
	DDX_Radio(pDX, IDC_RADIO2, m_fitMode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST1, m_List[0]);
	DDX_Control(pDX, IDC_LIST2, m_List[1]);
	DDX_Control(pDX, IDC_LIST3, m_List[2]);
	DDX_Control(pDX, IDC_LIST4, m_List[3]);
}


BEGIN_MESSAGE_MAP(CDlgEnergyKedu, CDialog)
	//{{AFX_MSG_MAP(CDlgEnergyKedu)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, OnClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnergyKedu message handlers

BOOL CDlgEnergyKedu::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	int i,j;
	for(i=0;i<1;i++)
	{
	m_List[i].SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_List[i].GetClientRect (&rect);
	m_List[i].InsertColumn (0, "±àºÅ", LVCFMT_LEFT,rect.Width() * 0.25);
	m_List[i].InsertColumn (1, "ADCÖµ", LVCFMT_LEFT,rect.Width() * 0.3);
	m_List[i].InsertColumn (2, "·ù¶È", LVCFMT_LEFT,rect.Width() * 0.45);
	}
	for(i=1;i<1;i++)
		m_List[i].ShowWindow(SW_HIDE);
	char temp[80];
	for(j=0;j<1;j++)
	{
	for(int i=0;i<energyKedu[j].Enum;i++)
	{
		sprintf(temp,"%d",i+1);
		m_List[j].InsertItem(i,temp);
		sprintf(temp,"%4.0f",energyKedu[j].channel[i]);
		m_List[j].SetItemText(i,1,temp);
		sprintf(temp,"%8.3f",energyKedu[j].energy[i]);
		m_List[j].SetItemText(i,2,temp);
	}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgEnergyKedu::OnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
/*
	char temp[80];
	int i=energyKedu[adcNum].Enum;
	energyKedu[adcNum].Enum++;
	energyKedu[adcNum].channel[i]=m_Channel;
	energyKedu[adcNum].energy[i]=m_Energy;
	sprintf(temp,"%d",i+1);
	m_List[adcNum].InsertItem(i,temp);
	sprintf(temp,"%d",energyKedu[adcNum].channel[i]);
	m_List[adcNum].SetItemText(i,1,temp);
	sprintf(temp,"%d",energyKedu[adcNum].energy[i]);
	m_List[adcNum].SetItemText(i,2,temp);
*/
	char temp[20];
	int i,j;
	int maxn=energyKedu[adcNum].Enum;
	m_CtrlCh.GetWindowText(temp,10);
    energyKedu[adcNum].channel[maxn]=atof(temp);
	m_CtrlEn.GetWindowText(temp,20);
	energyKedu[adcNum].energy[maxn]=atof(temp);
	energyKedu[adcNum].Enum++;
	if(maxn==0)
	{
		sprintf(temp,"%d",maxn+1);
		m_List[adcNum].InsertItem(maxn,temp);
		sprintf(temp,"%4.0f",energyKedu[adcNum].channel[maxn]);
		m_List[adcNum].SetItemText(maxn,1,temp);
		sprintf(temp,"%8.3f",energyKedu[adcNum].energy[maxn]);
		m_List[adcNum].SetItemText(maxn,2,temp);
	}
	else if(energyKedu[adcNum].channel[maxn-1]<energyKedu[adcNum].channel[maxn])
	{
		sprintf(temp,"%d",maxn+1);
		m_List[adcNum].InsertItem(maxn,temp);
		sprintf(temp,"%4.0f",energyKedu[adcNum].channel[maxn]);
		m_List[adcNum].SetItemText(maxn,1,temp);
		sprintf(temp,"%8.3f",energyKedu[adcNum].energy[maxn]);
		m_List[adcNum].SetItemText(maxn,2,temp);
	}
	else
	{
		for(i=0;i<maxn;i++)
		{
			if(energyKedu[adcNum].channel[maxn]<energyKedu[adcNum].channel[i])
				break;
		}
		int temp1=energyKedu[adcNum].channel[maxn];
		int temp2=energyKedu[adcNum].energy[maxn];
		for(j=maxn;j>i;j--)
		{
			energyKedu[adcNum].channel[j]=energyKedu[adcNum].channel[j-1];
			energyKedu[adcNum].energy[j]=energyKedu[adcNum].energy[j-1];
		}
		energyKedu[adcNum].channel[i]=temp1;
		energyKedu[adcNum].energy[i]=temp2;
	}
	m_List[adcNum].DeleteAllItems();
	for(i=0;i<maxn+1;i++)
	{
		sprintf(temp,"%d",i+1);
		m_List[adcNum].InsertItem(i,temp);
		sprintf(temp,"%4.0f",energyKedu[adcNum].channel[i]);
		m_List[adcNum].SetItemText(i,1,temp);
		sprintf(temp,"%8.3f",energyKedu[adcNum].energy[i]);
		m_List[adcNum].SetItemText(i,2,temp);
	}
}

void CDlgEnergyKedu::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List[adcNum].GetFirstSelectedItemPosition();
	if(pos)
	{
		 int nItem = m_List[adcNum].GetNextSelectedItem(pos);
		 char temp[20];
		 m_CtrlCh.GetWindowText(temp,20);
		 m_List[adcNum].SetItemText(nItem,1,temp);
		 energyKedu[adcNum].channel[nItem]=atof(temp);
		 m_CtrlEn.GetWindowText(temp,20);
		 m_List[adcNum].SetItemText(nItem,2,temp);
		 energyKedu[adcNum].energy[nItem]=atof(temp);
	}
}

void CDlgEnergyKedu::OnDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List[adcNum].GetFirstSelectedItemPosition();
	int i,nSelect;
	char temp[80];
	if(pos)
	{
	    int nItem = m_List[adcNum].GetNextSelectedItem(pos);
		m_List[adcNum].DeleteItem(nItem);
		energyKedu[adcNum].Enum--;
		for(i=nItem;i<energyKedu[adcNum].Enum;i++)
		{
			energyKedu[adcNum].channel[i]=energyKedu[adcNum].channel[i+1];
		}
		m_List[adcNum].DeleteAllItems();
		for(i=0;i<energyKedu[adcNum].Enum;i++)
		{
			sprintf(temp,"%d",i+1);
			m_List[adcNum].InsertItem(i,temp);
			sprintf(temp,"%4.0f",energyKedu[adcNum].channel[i]);
			m_List[adcNum].SetItemText(i,1,temp);
			sprintf(temp,"%8.3f",energyKedu[adcNum].energy[i]);
			m_List[adcNum].SetItemText(i,2,temp);
		}
	}
	
}

void CDlgEnergyKedu::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_fitMode=0;
	energyKedu[adcNum].fitMode=0;
}

void CDlgEnergyKedu::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_fitMode=1;
	energyKedu[adcNum].fitMode=1;
}


void CDlgEnergyKedu::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_List[adcNum].GetFirstSelectedItemPosition();
	if(pos)
	{
		 int nItem = m_List[adcNum].GetNextSelectedItem(pos);
		 char temp[20];
		 m_List[adcNum].GetItemText(nItem,1,temp,10);
		 m_CtrlCh.SetWindowText(temp);
		 w1[adcNum]->dispspm.CursorChn=atof(temp);
		 m_List[adcNum].GetItemText(nItem,2,temp,10);
		 m_CtrlEn.SetWindowText(temp);
	     w1[adcNum]->DispCursor();
	}

	*pResult = 0;
}


void CDlgEnergyKedu::OnOK() 
{
	// TODO: Add extra validation here
	CFunction fun;
	int i;
	for(i=0;i<1;i++)
	{
		fun.pX=energyKedu[i].channel;
		fun.pY=energyKedu[i].energy;
		fun.adcNum=i;
		if(energyKedu[i].fitMode==0)
			fun.nDimension=2;
		else
			fun.nDimension=3;
		fun.CalMain();
		energyKedu[i].para[0]=fun.para[0];
		energyKedu[i].para[1]=fun.para[1];
		energyKedu[i].para[2]=fun.para[2];
	}
	CDialog::OnOK();
}
