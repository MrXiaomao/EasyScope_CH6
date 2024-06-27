// mcbc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mcbc.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CChildFrame *m_pChildWnd[4];
extern  bool	bAdcWorkOn[4];
extern	char	curDirectory[100];
extern	BOOL	bTongbu;
extern  unsigned	int	nPresetTime[6];
extern int		MaxChannel;
extern  char	strFileTxt[4];
extern	COLORREF mbkcolor,mforecolor,mredcolor,mcursorcolor;
extern McsSetData	mcsSetData[4];
extern CEnergyKedu energyKedu[4];
extern	StripData stripData;
extern bool	bPresetReal[4];
extern int		nComx;
extern CAutoData autoData;
extern unsigned char amplifier;
extern unsigned short  nHV;
extern CGuassData mGuassData;
extern	char	strCom[10][5];
extern	int	mAdcGain;
extern	bool	bAdcSet[3];//bAdcSet[0]: 0  直流  1 交流    bAdcSet[1]: 0 同向   1 反向   bAdcSet[2]:0 移码  1 补码
extern	int	fLD;
extern	int		Vort;
extern	int		fLD2;//基线
extern	int nWaveRepeat;
extern unsigned short   mHV;//高压限值
extern	bool	bWaveDisp[6];
extern	bool	bWaveShort;
extern	float fLDWave[6];

/////////////////////////////////////////////////////////////////////////////
// CMcbcApp

BEGIN_MESSAGE_MAP(CMcbcApp, CWinApp)
	//{{AFX_MSG_MAP(CMcbcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMcbcApp construction

CMcbcApp::CMcbcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMcbcApp object

CMcbcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMcbcApp initialization

BOOL CMcbcApp::InitInstance()
{
	HANDLE m_hMutex=CreateMutex(NULL,TRUE,m_pszAppName); 
	if(GetLastError()==ERROR_ALREADY_EXISTS)  
	{
		return FALSE;
	}
	CFile mfile;
	char temp[160];
	GetCurrentDirectory(MAX_PATH,curDirectory);
	sprintf(temp,"%s\\mcb.dat",curDirectory);
	if(mfile.Open(temp,CFile::modeRead))
	{
	mfile.Read(&bTongbu,sizeof(bTongbu));
	mfile.Read(nPresetTime,sizeof(nPresetTime));
	mfile.Read(&MaxChannel,sizeof(MaxChannel));
	mfile.Read(strFileTxt,4);
	mfile.Read(&mbkcolor,sizeof(mbkcolor));
	mfile.Read(&mforecolor,sizeof(mbkcolor));
	mfile.Read(&mredcolor,sizeof(mbkcolor));
	mfile.Read(&mcursorcolor,sizeof(mbkcolor));
	mfile.Read(bAdcWorkOn,sizeof(bAdcWorkOn));
	mfile.Read(mcsSetData,sizeof(mcsSetData));

	mfile.Read(energyKedu,sizeof(energyKedu));
	mfile.Read(&stripData,sizeof(stripData));
	mfile.Read(bPresetReal,sizeof(bPresetReal));
	mfile.Read(&nComx,sizeof(nComx));
	mfile.Read(&autoData,sizeof(autoData));
	mfile.Read(&amplifier,1);
	mfile.Read(&mHV,2);
	mfile.Read(&mGuassData,sizeof(CGuassData));

	mfile.Read(&strCom,sizeof(strCom));
	mfile.Read(&mAdcGain,sizeof(mAdcGain));
	mfile.Read(bAdcSet,sizeof(bAdcSet));
	mfile.Read(&fLD,sizeof(fLD));
	mfile.Read(&Vort,sizeof(Vort));
	mfile.Read(&nHV,sizeof(nHV));
	mfile.Read(&nWaveRepeat,sizeof(nWaveRepeat));
	mfile.Read(bWaveDisp,sizeof(bWaveDisp));
	mfile.Read(&bWaveShort,sizeof(bWaveShort));
	mfile.Read(fLDWave,sizeof(fLDWave));
	mfile.Close();
	}
	else

	{
		bTongbu=1;
		mforecolor = RGB(255,255,255);
		mbkcolor = RGB(0,0,127);
		mredcolor = RGB(255,0,0);
		mcursorcolor =RGB(255,0,255,);
	}
/*
	HWND hwnd;
	hwnd=FindWindow(NULL,"四通道多道分析器");
	if(hwnd)
	{
		ShowWindow(hwnd,SW_SHOWMAXIMIZED);
		::SetForegroundWindow (hwnd);
		return TRUE;
	}
*/
	AfxEnableControlContainer();
    SetDialogBkColor();        
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMcbcDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMcbcView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
/*
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
// The main window has been initialized, so show and update it.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
*/
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
//	m_pChildWnd[1]->ShowWindow(SW_HIDE);
//	m_pChildWnd[2]->ShowWindow(SW_HIDE);
//	m_pChildWnd[3]->ShowWindow(SW_HIDE);
    m_pChildWnd[0]->ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMcbcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMcbcApp message handlers



