// DlgPaperV.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgPaperV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int mPaperV;
/////////////////////////////////////////////////////////////////////////////
// CDlgPaperV dialog


CDlgPaperV::CDlgPaperV(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPaperV)
	m_PaperV = mPaperV;
//	unsigned short sPaperV=(65536-1843200/m_PaperV/2);
//	unsigned char paperVH,paperVL;
//	paperVH=sPaperV/256;
//	paperVL=sPaperV%256;
//	float v2=1843200./(65536-sPaperV)/2;
//	m_PaperV2.Format("%8.3f",v2);
	//}}AFX_DATA_INIT
}


void CDlgPaperV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPaperV)
	DDX_Text(pDX, IDC_PAPER_V, m_PaperV);
	DDV_MinMaxInt(pDX, m_PaperV, 15, 15000);
	DDX_Text(pDX, IDC_PAPER_V2, m_PaperV2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPaperV, CDialog)
	//{{AFX_MSG_MAP(CDlgPaperV)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperV message handlers

void CDlgPaperV::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	mPaperV=m_PaperV;
	unsigned short sPaperV=(65536-1843200/mPaperV);
	unsigned char paperVH,paperVL;
	paperVH=sPaperV/256;
	paperVL=sPaperV%256;
	float v2=1843200./(65536-sPaperV);
	m_PaperV2.Format("%8.3f",v2);
	UpdateData(FALSE);

}

void CDlgPaperV::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	mPaperV=m_PaperV;
	CDialog::OnOK();
}
