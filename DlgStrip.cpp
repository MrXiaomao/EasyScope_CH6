// DlgStrip.cpp : implementation file
//

#include "stdafx.h"
#include "mcbc.h"
#include "DlgStrip.h"
#include "define.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
StripData stripData;
/////////////////////////////////////////////////////////////////////////////
// CDlgStrip dialog

int CALLBACK _SHBrowseForFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	
	return	0;
}

CDlgStrip::CDlgStrip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStrip)
	m_Check1 = stripData.Check1;
	m_Radio1 = stripData.m_Radio1;
	m_Directory = _T("");
	//}}AFX_DATA_INIT
}


void CDlgStrip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrip)
	DDX_Control(pDX, IDC_COMBO3, m_Combo3);
	DDX_Control(pDX, IDC_CHECK1, m_CtrlCheck1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_CURIDIR, m_Directory);
	DDX_Text(pDX, IDC_EDIT1, m_Radio1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStrip, CDialog)
	//{{AFX_MSG_MAP(CDlgStrip)
	ON_BN_CLICKED(IDC_RADIO_FOUR, OnRadioFour)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_CBN_DBLCLK(IDC_COMBO3, OnDblclkCombo3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrip message handlers

BOOL CDlgStrip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	TCHAR szPath [MAX_PATH]; 
	::GetCurrentDirectory(MAX_PATH,szPath);
	char path[MAX_PATH+10];
	sprintf(path,"%s\\*.chn",szPath);
	int nReslt = DlgDirListComboBox( path,IDC_COMBO3,IDC_CURIDIR,DDL_READWRITE|DDL_READONLY| 
		DDL_ARCHIVE);
	m_Combo3.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStrip::OnRadioFour() 
{
	// TODO: Add your control notification handler code here
}

void CDlgStrip::OnRadio1() 
{
	// TODO: Add your control notification handler code here

}

void CDlgStrip::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	int x=m_CtrlCheck1.GetCheck();
	m_Edit1.EnableWindow(!x);
	
}

void CDlgStrip::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	stripData.Check1=m_Check1;
	stripData.m_Radio1 =  m_Radio1;
	DlgDirSelectComboBox(m_PathName.GetBuffer(100),IDC_COMBO3);	
	CDialog::OnOK();
}

void CDlgStrip::OnDblclkCombo3() 
{
	// TODO: Add your control notification handler code here
	char path[MAX_PATH+10];
	DlgDirSelectComboBox(path,IDC_COMBO3);
	TCHAR szPath [MAX_PATH]; 
	::GetCurrentDirectory(MAX_PATH,szPath);
	sprintf(path,"%s\\*.chn",szPath);
	int nReslt = DlgDirListComboBox (path,IDC_COMBO3,IDC_CURIDIR,DDL_READWRITE|DDL_READONLY| 
		DDL_ARCHIVE);
	m_Combo3.SetCurSel(0);
}

void CDlgStrip::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char temp[200];
	char curDirectory[100];
	UpdateData(TRUE);
	::GetCurrentDirectory(MAX_PATH,curDirectory);
	sprintf(temp,"%s",curDirectory);
	CString sFolderPath;
	BROWSEINFO bi;
	SHFILEINFO info; 

	char Buffer[MAX_PATH];
	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	//bi.pidlRoot = pidl;
	LPITEMIDLIST	lpidlRoot;
	::SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpidlRoot);	//ȡ��ѡ�����ļ�����
	bi.pidlRoot = lpidlRoot;

	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ���׾�����·��";
	bi.ulFlags = BIF_RETURNONLYFSDIRS |BIF_DONTGOBELOWDOMAIN;
//	bi.ulFlags	|= BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
	bi.lpfn			= _SHBrowseForFolderCallbackProc;
//	bi.lpfn = NULL;
	bi.iImage=IDR_MCBCTYPE;
	CString strIniFolder=temp;
	bi.lParam = (LPARAM)strIniFolder.GetBuffer(0);
	//��ʼ����ڲ���bi����


	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
	SHGetPathFromIDList(pIDList, Buffer);
	//ȡ���ļ���·����Buffer��
	m_PathName = Buffer;//��·��������һ��CString������
	strcpy(temp,m_PathName.GetBuffer(200));
	UpdateData(FALSE);
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();

	TCHAR szPath [MAX_PATH]; 
	::GetCurrentDirectory(MAX_PATH,szPath);
	char path[MAX_PATH+10];
	bool x=m_CtrlCheck1.GetCheck();
	sprintf(path,"%s\\*.chn",m_PathName.GetBuffer(MAX_PATH));
	int nReslt = DlgDirListComboBox (path,IDC_COMBO3,IDC_CURIDIR,DDL_READWRITE|DDL_READONLY| 
		DDL_ARCHIVE);
	m_Combo3.SetCurSel(0);

}
