// DlgSetRoi.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgSetRoi.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
extern	CMcbcView	*w1[4];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetRoi dialog


CDlgSetRoi::CDlgSetRoi(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetRoi::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetRoi)
	m_RoiLeftCh = 0;
	m_RoiRightCh = 0;
	m_Left = 0;
	//}}AFX_DATA_INIT
	m_Set=0;
}


void CDlgSetRoi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetRoi)
	DDX_Text(pDX, IDC_EDIT_LEFT, m_RoiLeftCh);
	DDV_MinMaxInt(pDX, m_RoiLeftCh, 0, 8192);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_RoiRightCh);
	DDV_MinMaxInt(pDX, m_RoiRightCh, 0, 8192);
	DDX_Radio(pDX, IDC_RADIO1, m_Left);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetRoi, CDialog)
	//{{AFX_MSG_MAP(CDlgSetRoi)
	ON_BN_CLICKED(IDC_RETURN, OnReturn)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetRoi message handlers

void CDlgSetRoi::OnReturn() 
{
	// TODO: Add your control notification handler code here
	w1[m_View]->bSetRoiDlg=0;
	w1[m_View]->Invalidate(0);
	OnCancel();
}

void CDlgSetRoi::OnSet() 
{
	// TODO: Add your control notification handler code here
	w1[m_View]->SetRoiMenu();
	w1[m_View]->bSetRoiDlg=0;
	w1[m_View]->Invalidate(0);
	OnCancel();
}

void CDlgSetRoi::OnClear() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
