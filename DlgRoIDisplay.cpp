// DlgRoIDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgRoIDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRoIDisplay dialog


CDlgRoIDisplay::CDlgRoIDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRoIDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRoIDisplay)
	m_CenterCh = 0.0f;
	m_CenterEn = 0.0f;
	m_FwmhCh = 0.0f;
	m_FwmhEn = 0.0f;
	m_Gross = 0.0f;
	m_NetArea = 0.0f;
	m_RoiRangeCh = _T("");
	m_RoiRangeEn = _T("");
	m_FWHM = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgRoIDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRoIDisplay)
	DDX_Text(pDX, IDC_CENTERCH, m_CenterCh);
	DDX_Text(pDX, IDC_CENTEREN, m_CenterEn);
	DDX_Text(pDX, IDC_FWMHCH, m_FwmhCh);
	DDX_Text(pDX, IDC_FWMHEN, m_FwmhEn);
	DDX_Text(pDX, IDC_GROSS, m_Gross);
	DDX_Text(pDX, IDC_NETAREA, m_NetArea);
	DDX_Text(pDX, IDC_ROI_RANGE_CH, m_RoiRangeCh);
	DDX_Text(pDX, IDC_ROI_RANGE_EN, m_RoiRangeEn);
	DDX_Text(pDX, IDC_FWMHEN2, m_FWHM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRoIDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgRoIDisplay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRoIDisplay message handlers
