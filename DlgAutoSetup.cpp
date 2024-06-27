// DlgAutoSetup.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgAutoSetup.h"
#include "define.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CAutoData autoData;
/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSetup dialog


CDlgAutoSetup::CDlgAutoSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoSetup)
	m_GapTime = autoData.m_GapTime;
	m_MeasureTime = autoData.m_MeasureTime;
	m_Times = autoData.m_Times;
	m_No = autoData.m_No;
	//}}AFX_DATA_INIT
}


void CDlgAutoSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoSetup)
	DDX_Text(pDX, IDC_GAP_TIME, m_GapTime);
	DDX_Text(pDX, IDC_MEASURE_TIME, m_MeasureTime);
	DDX_Text(pDX, IDC_TIMES, m_Times);
	DDX_Text(pDX, IDC_NO1, m_No);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSetup message handlers

void CDlgAutoSetup::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	autoData.m_GapTime=m_GapTime ;
	autoData.m_MeasureTime=m_MeasureTime;
	strcpy(autoData.m_No,m_No.GetBuffer(50));
	autoData.m_Times=m_Times;

	CDialog::OnOK();
}
