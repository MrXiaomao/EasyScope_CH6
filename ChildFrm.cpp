// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "mcbc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "mcbcDoc.h"
#include "mcbcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int nChildNum=0;
CChildFrame *m_pChildWnd[4];
CDialogBar *m_pDlgBarState[4];
extern	CDialogBar *m_pDlgBar;
extern	CDialogBar *m_pDlgBarCount;
extern	CMainFrame	*pWnd;
extern	bool	bWaveDisp[6];
extern	CMcbcView	*w1[4];

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_CAL_COUNT, &CChildFrame::OnBnClickedCalCount)
	ON_BN_CLICKED(IDC_CH1, &CChildFrame::OnBnClickedCh1)
	ON_BN_CLICKED(IDC_CH2, &CChildFrame::OnBnClickedCh2)
	ON_BN_CLICKED(IDC_CH3, &CChildFrame::OnBnClickedCh3)
	ON_BN_CLICKED(IDC_CH4, &CChildFrame::OnBnClickedCh4)
	ON_BN_CLICKED(IDC_CH5, &CChildFrame::OnBnClickedCh5)
	ON_BN_CLICKED(IDC_CH6, &CChildFrame::OnBnClickedCh6)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	IDChild=nChildNum;
	nChildNum++;
	m_pDlgBarState[IDChild] = NULL;
	m_pChildWnd[IDChild] = NULL;
}

CChildFrame::~CChildFrame()
{
	if(m_pDlgBarState[IDChild]) delete m_pDlgBarState[IDChild];
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &=~WS_SYSMENU;
//	cs.style &= ~(LONG)FWS_ADDTOTITLE;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CChildFrame::OnSysCommand( UINT nID, LPARAM lParam )
{

		CMDIChildWnd::OnSysCommand(nID,lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(!m_pChildWnd[IDChild])
	{
		m_pChildWnd[IDChild] = (CChildFrame*)GetActiveFrame();
	}
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
	m_pDlgBarState[IDChild] = new CDialogBar;
	m_pDlgBarState[IDChild]->Create(this,IDD_DIALOGBAR_STATE,CBRS_BOTTOM,102);
	m_pDlgBarState[IDChild]->ShowWindow(SW_SHOW);
	m_pDlgBarCount = new CDialogBar;
	m_pDlgBarCount->Create(this,IDD_DIALOGBAR_COUNT,CBRS_RIGHT,103);
	m_pDlgBarCount->ShowWindow(SW_SHOW);
	if(bWaveDisp[0])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH1)))->SetCheck(1);
	if(bWaveDisp[1])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH2)))->SetCheck(1);
	if(bWaveDisp[2])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH3)))->SetCheck(1);
	if(bWaveDisp[3])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH4)))->SetCheck(1);
	if(bWaveDisp[4])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH5)))->SetCheck(1);
	if(bWaveDisp[5])	
		((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH6)))->SetCheck(1);

	return TRUE;
}


void CChildFrame::OnBnClickedCalCount()
{
	// TODO: 在此添加控件通知处理程序代码
	pWnd->OnBnClickedCalCount();
}


void CChildFrame::OnBnClickedCh1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH1)))->GetCheck())
		bWaveDisp[0]=1;
	else
		bWaveDisp[0]=0;
	w1[0]->Invalidate(0);
}


void CChildFrame::OnBnClickedCh2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH2)))->GetCheck())
		bWaveDisp[1]=1;
	else
		bWaveDisp[1]=0;
	w1[0]->Invalidate(0);

}


void CChildFrame::OnBnClickedCh3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH3)))->GetCheck())
		bWaveDisp[2]=1;
	else
		bWaveDisp[2]=0;
	w1[0]->Invalidate(0);
}


void CChildFrame::OnBnClickedCh4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH4)))->GetCheck())
		bWaveDisp[3]=1;
	else
		bWaveDisp[3]=0;
	w1[0]->Invalidate(0);
}


void CChildFrame::OnBnClickedCh5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH5)))->GetCheck())
		bWaveDisp[4]=1;
	else
		bWaveDisp[4]=0;
	w1[0]->Invalidate(0);
}


void CChildFrame::OnBnClickedCh6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBarCount->GetDlgItem(IDC_CH6)))->GetCheck())
		bWaveDisp[5]=1;
	else
		bWaveDisp[5]=0;
	w1[0]->Invalidate(0);
}
