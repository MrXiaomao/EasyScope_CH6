// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "mcbc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "define.h"
#include <stdio.h>
#include <winioctl.h>
#include "htmlhelp.h"
#include "MCB_AVR_WDMioctl.h"
#include "DialogTime.h"
#include "DlgEnergyKedu.h"
#include "define.h"
#include "Function.h"
#include "DlgRoIDisplay.h"
#include "DlgSetRoi.h"
#include "DlgRS232.h"
#include "SiUSBXp.h"
#include <afxmt.h>
#include <afxcoll.h>
#include <direct.h>
#include <conio.h>
#include "Mmsystem.h"
#include "DlgAutoSetup.h"
#include "DlgAmp.h"
#include "DlgGuass.h"
#include "DlgBaseLine.h"
#include "DlgSetLD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class	CChildFrame;
CDialogBar *m_pDlgBar;
CDialogBar *m_pDlgBarCount;
BOOL	bTongbu;
enum	ADCGAIN {Adc1=512,Adc2=1024,Adc3=2048,Adc4=4096,Adc5=8192};
int		MaxChannel;
BOOL	transbz[4];
unsigned	int	nPresetTime[6];
bool	bPresetReal[4];
ADCGAIN	AdcGain;
extern CChildFrame *m_pChildWnd[4];
extern	long	*pAdcBuf[8];
long		*pDispAdcBuf[8];
extern	COLORREF mbkcolor,mforecolor,mredcolor,mcursorcolor;
extern	DWORD cbByteReturned;
extern	HANDLE hDevice;
extern	CMcbcView	*w1[4];
extern  CDialogBar *m_pDlgBarTime[4];
extern	int		Gain[4];
extern	CDialogBar *m_pDlgBarState[4];
extern  char	strFileTxt[4];
int	nOldRealTime=-1,nOldRealTime2=-2;
int	nOldRealTime3=-1,nOldRealTime4=-2;
extern  bool	bAdcWorkOn[4];
McsSetData	mcsSetData[4];
//int		mcsStarCh[4],mcsEndCh[4],mcsTime[4],mcsAreaOrGross[4];
CDlgSetRoi   *pDlgSetRoi;
int		bDispMca[8];
long	*pMemBuf[8];
CEnergyKedu energyKedu[4];
CEnergyKedu1 energyKedu1[4];
CDlgRoIDisplay *pDlgRoIDisplay;
bool	bDispBuffer;
CTime bufStartTime[4],hardSatrtTime[4];
extern	StripData stripData;

HANDLE		hCom;
unsigned long	lpnumber;
char	strCom[10][5];
int		nComx;
BYTE		m_TxData[200];
BYTE		*m_RxData;
BYTE		*m_RxDataBuf;
bool	bRS232,bReceiveEnd;
CByteArray	inWaveBuf;
int timer100ms;
UINT wAccuracy;//定义分辨率
UINT TimerID;//定义定时器句柄
UINT wTimerRes;//定义时间间隔
//定义时钟分辨率，以ms为单位
const UINT TIMER_ACCURACY=1;
char	curDirectory[100];
int		mTimer;
CMainFrame	*pWnd;
int		nTimerTick;
HANDLE m_hUSBDevice;
bool   pumpOn,alarmOn,paperOn;
CString  usbState;
extern	DWORD toBeWritten;
extern	bool	bTrans;
unsigned char comAddr=0x60;
extern CAutoData autoData;
bool	bOpenRs232,nOpenRs232;
unsigned char amplifier;
extern CGuassData mGuassData;
int	  adcNum,adcStartTransCh;
int	mAdcGain;
BOOL	bStart;
bool	bFirstStart,bFirstStop;
bool	bAdcSet[3];//bAdcSet[0]: 0  直流  1 交流    bAdcSet[1]: 0 同向   1 反向   bAdcSet[2]:0 移码  1 补码
int		fLD;//阈值
unsigned short  nHV; //高压
unsigned short   mHV;//高压限值
int		Vort;
_int64   startAbsTime;
_int64   curAbsTime;
_int64   endAbsTime;
_int64   startDeadTime;		
_int64   curDeadTime;
_int64   endDeadTime;
float		fMeasTime;
float		fDeadTime;
bool	bStopWave;
bool bWaitTriger;
int	nCurWave;
int nWaveRepeat;
bool	bSendSingle;
bool	bHVOpen;
int		fLD2;//基线
double   HV_k,HV_b,TH_k,TH_b;
char	password[20];
void fitGass();
CDlgGuass * pDlgGuassFit;

#include< stdio.h >
#include< string.h >
#include< winsock.h >
#include< windows.h >
 
#pragma comment( lib, "ws2_32.lib" )
 
#define PORT 24
#define BACKLOG 10
#define TRUE 1
#define MAXDATASIZE 16384022
int		iClientSock;
struct	sockaddr_in ServerAddr;
bool	bTCPIPOK;
char serveIP[20]="192.168.10.16";
//char serveIP[20]="127.0.0.1";
int		nCurRevLen,nPreRevLen;
int		remainRevLen;//上次接收数据未处理的剩余数据长度
bool	bRevBegin,bRevEnd;
int		nPreDispLen;
bool	bWaveDisp[6];
//float	fLDWave[6]={34000,34000,34000,34000,34000,34000};
bool	bWaveShort;
bool	bHardware;
bool	bWaitHardware;
float	fLDWave[6];

void WriteFileTCP(int iClientSock,BYTE * m_TxData,int toBeWritten, unsigned long& lpnumber)
{
//	 lpnumber=send( iClientSock, (char *)m_TxData,toBeWritten, 0);
	if(bTCPIPOK)
		lpnumber = sendto( iClientSock, (char *)m_TxData,toBeWritten, 0, ( struct sockaddr * ) & ServerAddr, sizeof( struct sockaddr ) );
	else
		lpnumber=0;
}

void CALLBACK TimeProc(UINT wTimerId,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	mTimer++;
	pWnd=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(pWnd->GetSafeHwnd())
		pWnd->DoTimer();

}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)

	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_ADC_512, OnUpdateAdc512)
	ON_UPDATE_COMMAND_UI(ID_ADC_1024, OnUpdateAdc1024)
	ON_UPDATE_COMMAND_UI(ID_ADC_2048, OnUpdateAdc2048)
	ON_UPDATE_COMMAND_UI(ID_ADC_4096, OnUpdateAdc4096)
	ON_UPDATE_COMMAND_UI(ID_ADC_8192, OnUpdateAdc8192)
	ON_COMMAND(ID_MENU_BKCOLOR, OnMenuBkcolor)
	ON_COMMAND(ID_MENU_FORECOLOR, OnMenuForecolor)
	ON_COMMAND(ID_MENU_ROICOLOR, OnMenuRoicolor)
	ON_COMMAND(ID_MENU_CURSORCOLOR, OnMenuCursorcolor)
	ON_COMMAND(ID_TIME_PRESET, OnTimePreset)//基线刻度
	ON_COMMAND(ID_MENU_MCATOBUF, OnMenuMcatobuf)
	ON_COMMAND(ID_MENU_EKEDU, OnMenuEkedu)
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	ON_COMMAND(ID_MENU_CLOSE_INT, OnMenuCloseInt)
	ON_COMMAND(ID_MENU_RS232_SET, OnMenuRs232Set)
	ON_COMMAND(ID_MENU_AUTO_SETUP, OnMenuAutoSetup)
	ON_COMMAND(ID_MENU_AUTO_START, OnMenuAutoStart)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTO_START, OnUpdateMenuAutoStart)
	ON_COMMAND(ID_MENU_AUTO_STOP, OnMenuAutoStop)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTO_STOP, OnUpdateMenuAutoStop)
	ON_COMMAND(ID_MENU_AUTO_CLEAR, OnMenuAutoClear)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTO_CLEAR, OnUpdateMenuAutoClear)
	ON_COMMAND(ID_MENU_FIT, OnMenuFit)
	ON_COMMAND(ID_MENUITEM_CLOSE_RS232, OnMenuitemCloseRs232)
	ON_COMMAND(ID_MENUITEM_FORWARD, OnMenuitemForward)
	ON_COMMAND(ID_MENUITEM_REVERSE, OnMenuitemReverse)
	ON_COMMAND(ID_MENUITEM_DC, OnMenuitemDc)
	ON_COMMAND(ID_MENUITEM_AC, OnMenuitemAc)
	ON_COMMAND(ID_MENUITEM_SHIFT_CODE, OnMenuitemShiftCode)
	ON_COMMAND(ID_MENUITEM_COMP_CODE, OnMenuitemCompCode)
	ON_COMMAND(ID_MENUITEM_SET_LD, OnMenuitemSetLd)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_MENU_AUTO, &CMainFrame::OnMenuAuto)
	ON_COMMAND(ID_MENU_NORMAL, &CMainFrame::OnMenuNormal)
	ON_COMMAND(ID_MENU_SINGLE, &CMainFrame::OnMenuSingle)
	ON_COMMAND(ID_MENU_STOP, &CMainFrame::OnMenuStop)
	ON_COMMAND(ID_MENU_FORCE, &CMainFrame::OnMenuForce)
	ON_COMMAND(ID_MENU_TRIP_UP, &CMainFrame::OnMenuTripUp)
	ON_COMMAND(ID_MENU_TRIP_DOWN, &CMainFrame::OnMenuTripDown)
	ON_COMMAND(ID_MENU_TRIP_LEVEL, &CMainFrame::OnMenuTripLevel)
	ON_COMMAND(ID_MENU_50, &CMainFrame::OnMenu50)
	ON_COMMAND(ID_MENU_10K, &CMainFrame::OnMenu10k)
	ON_COMMAND(ID_MENU_BASELINE, &CMainFrame::OnMenuBaseline)
	ON_COMMAND(ID_MENUITEM_SET_LD2, &CMainFrame::OnMenuitemSetLd2)
	ON_BN_CLICKED(ID_MENUITEM_DC2, &CMainFrame::OnBnClickedMenuitemDc2)
	ON_BN_CLICKED(ID_MENUITEM_FORWARD2, &CMainFrame::OnBnClickedMenuitemForward2)
	ON_BN_CLICKED(ID_MENUITEM_SHIFT_CODE2, &CMainFrame::OnBnClickedMenuitemShiftCode2)
	ON_COMMAND(ID_MENUITEM_SET_HV, &CMainFrame::OnMenuitemSetHv)
	ON_BN_CLICKED(IDC_RUN, &CMainFrame::OnBnClickedRun)
	ON_BN_CLICKED(IDC_STOP, &CMainFrame::OnBnClickedStop)
	ON_BN_CLICKED(IDC_FORCE, &CMainFrame::OnBnClickedForce)
	ON_BN_CLICKED(IDC_HV_OPEN, &CMainFrame::OnBnClickedHvOpen)
	ON_UPDATE_COMMAND_UI(IDC_RUN, OnUpdateRun)
	ON_UPDATE_COMMAND_UI(IDC_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(IDC_FORCE, OnUpdateForce)
	ON_BN_CLICKED(IDC_CH1, &CMainFrame::OnBnClickedCh1)
	ON_BN_CLICKED(IDC_CH2, &CMainFrame::OnBnClickedCh2)
	ON_BN_CLICKED(IDC_CH3, &CMainFrame::OnBnClickedCh3)
	ON_BN_CLICKED(IDC_CH4, &CMainFrame::OnBnClickedCh4)
	ON_BN_CLICKED(IDC_CH5, &CMainFrame::OnBnClickedCh5)
	ON_BN_CLICKED(IDC_CH6, &CMainFrame::OnBnClickedCh6)
	ON_BN_CLICKED(IDC_CAL_COUNT, &CMainFrame::OnBnClickedCalCount)
	ON_COMMAND(ID_CAL_COUNT, &CMainFrame::OnCalCount)
	ON_BN_CLICKED(IDC_RADIO_WAVE_SHORT, &CMainFrame::OnBnClickedRadioWaveShort)
	ON_BN_CLICKED(IDC_RADIO_WAVE_LONG, &CMainFrame::OnBnClickedRadioWaveLong)
	ON_BN_CLICKED(IDC_RADIO_WAVE_SHORT2, &CMainFrame::OnBnClickedRadioWaveShort2)
	ON_COMMAND(ID_32871, &CMainFrame::On32871)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	int i,j;
	m_pDlgBar = NULL;
	timerID = 0;
	initPresetTime = TRUE;
	nTimerNo = 0;
	bRS232=1;	
	timer100ms=0;
	bReceiveEnd=1;
	bChannelStart=0;
	nReceiveNum=0;
	char t1[10]="北";
	m_RxData = new BYTE[MAXDATASIZE];
	m_RxDataBuf= new BYTE[MAXDATASIZE];
	CFile mfile;
	char temp[160];
	bStopWave=0;
	GetCurrentDirectory(MAX_PATH,curDirectory);
	nCurRevLen=0;
	nPreRevLen=0;
/*
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
	mfile.Read(&c51Data,sizeof(c51Data));
	mfile.Read(&amplifier,1);
	mfile.Read(&mHV,2);
	mfile.Read(&mGuassData,sizeof(CGuassData));
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
	autoData.bStart=0;

	energyKedu[0].Enum=0;
	energyKedu[1].Enum=0;
	energyKedu[2].Enum=0;
	energyKedu[3].Enum=0;
*/
//	MaxChannel = AdcGain;
	if(bWaveShort)
		MaxChannel=4000;
	else
		MaxChannel=1000000;
	Gain[0] = MaxChannel;
	Gain[1] = MaxChannel;
	Gain[2] = MaxChannel;
	Gain[3] = MaxChannel;
	for(i=0;i<8;i++)
	{
		pAdcBuf[i]=new long[2000000];
//		memset(pAdcBuf[i],0,2000000*sizeof(long));
	}
	for(i=0;i<1000000;i++)
	{
		pAdcBuf[0][i]=5000+i/100;
		pAdcBuf[1][i]=10000+i/100;
		pAdcBuf[2][i]=15000+i/100;
		pAdcBuf[3][i]=20000+i/100;
		pAdcBuf[4][i]=25000+i/100;
		pAdcBuf[5][i]=30000+i/100;
	}
	for(i=0;i<8;i++)
	{
		pMemBuf[i]=new long[2000000];
		memset(pMemBuf[i],0,2000000*sizeof(long));
	}
	pDlgRoIDisplay = new CDlgRoIDisplay;
	pDlgSetRoi = new CDlgSetRoi;

	for(i=0;i<8;i++)
	{
		pDispAdcBuf[i]=pAdcBuf[i];
		bDispMca[i]=1;
	}
	bFirst=1;
	bDispBuffer=0;
	pumpOn=0;
	alarmOn=0;
	paperOn=0;
//	pDlgAmp = new CDlgAmp;
	pDlgGuassFit =new CDlgGuass;
	adcStartTransCh=0;
	adcNum=0;
	nCurWave=0;
	bWaitHardware=0;
}

CMainFrame::~CMainFrame()
{
	delete []m_RxData;
	delete []m_RxDataBuf;
	if(m_pDlgBar) 
		delete m_pDlgBar;
	CFile mfile;
	int i,j,maxj;
	for(i=0;i<1;i++)
	{
		energyKedu1[i].Enum=energyKedu[i].Enum;
		energyKedu1[i].fitMode=energyKedu[i].fitMode;
	}
	for(i=0;i<1;i++)
	{
		maxj=energyKedu1[i].Enum;
		for(j=0;j<maxj;j++)
		{
			energyKedu1[i].channel[j]=energyKedu[i].channel[j];
			energyKedu1[i].energy[j]=energyKedu[i].energy[j];
		}
	}
	char temp[160];
	sprintf(temp,"%s\\mcb.dat",curDirectory);
	if(mfile.Open(temp,CFile::modeWrite|CFile::modeCreate))
	{
		mfile.Write(&bTongbu,sizeof(bTongbu));
		mfile.Write(nPresetTime,sizeof(nPresetTime));
		mfile.Write(&MaxChannel,sizeof(MaxChannel));
		mfile.Write(strFileTxt,4);
		mfile.Write(&mbkcolor,sizeof(mbkcolor));
		mfile.Write(&mforecolor,sizeof(mbkcolor));
		mfile.Write(&mredcolor,sizeof(mbkcolor));
		mfile.Write(&mcursorcolor,sizeof(mbkcolor));
		mfile.Write(bAdcWorkOn,sizeof(bAdcWorkOn));
		mfile.Write(mcsSetData,sizeof(mcsSetData));

		mfile.Write(energyKedu,sizeof(energyKedu));
		mfile.Write(&stripData,sizeof(stripData));
		mfile.Write(bPresetReal,sizeof(bPresetReal));
		mfile.Write(&nComx,sizeof(nComx));
		mfile.Write(&autoData,sizeof(autoData));
		mfile.Write(&amplifier,1);
		mfile.Write(&mHV,2);
		mfile.Write(&mGuassData,sizeof(CGuassData));

		mfile.Write(&strCom,sizeof(strCom));
		mfile.Write(&mAdcGain,sizeof(mAdcGain));
		mfile.Write(bAdcSet,sizeof(bAdcSet));
		mfile.Write(&fLD,sizeof(fLD));
		mfile.Write(&Vort,sizeof(Vort));
		mfile.Write(&nHV,sizeof(nHV));
		mfile.Write(&nWaveRepeat,sizeof(nWaveRepeat));
		mfile.Write(bWaveDisp,sizeof(bWaveDisp));
		mfile.Write(&bWaveShort,sizeof(bWaveShort));
		mfile.Write(fLDWave,sizeof(fLDWave));
		mfile.Close();

	
	mfile.Close();
	}
	if(bTCPIPOK)
	{
		closesocket( iClientSock );
		WSACleanup( );
	}
	delete 	pDlgRoIDisplay;
	delete pDlgSetRoi;
//	delete pDlgAmp;
	delete pDlgGuassFit;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int i;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1);;
//		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	int x=pDlgSetRoi->m_Left;

	int scrx = GetSystemMetrics(SM_CXMAXIMIZED);
	m_wndStatusBar.SetPaneInfo(1,WM_TIMER,SBPS_POPOUT,scrx*1/2);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_pDlgBar = new CDialogBar;
	m_pDlgBar->Create(this,IDD_DIALOGBAR,CBRS_RIGHT,101);
	m_pDlgBar->ShowWindow(SW_SHOW);

	oldday = -1;
	oldsecond = -1;
	pDlgRoIDisplay->Create(IDD_DLG_ROI);
	pDlgSetRoi->Create(IDD_DLG_SET_ROI);

	CStdioFile mfile;
	CString strPara;
	if(mfile.Open("para.dat",CFile::modeRead)!=NULL)
	{
		mfile.ReadString(strPara);
		mfile.ReadString(strPara);
		sscanf(strPara.GetBuffer(80),"%lf %lf",&HV_k,&HV_b);
		mfile.ReadString(strPara);
		sscanf(strPara.GetBuffer(80),"%lf %lf",&TH_k,&TH_b);
		mfile.ReadString(password,20);
		mfile.Close();
	}


	CFunction fun;
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

	GetCurrentDirectory(MAX_PATH,curDirectory);
	bDispBuffer=0;
//	OpenRS232();

	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
  	timerID = SetTimer(2,1000,NULL);

	wTimerRes=100;
	TIMECAPS tc;	//该结构包含计数器精度
	//利用函数timeGevDevCaps取出系统分辨率的取值范围，如无错则继续
	if(timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR)	
	{
		int wPeriodMin=max(tc.wPeriodMin,TIMER_ACCURACY);
		int wPeriodMax=tc.wPeriodMax;
		wAccuracy=min(wPeriodMin,wPeriodMax);		//分辨率不能超过系统取值范围
		timeBeginPeriod(wAccuracy);//调用timeBeginPeriod函数设置定时器的分辨率
		
		if ((TimerID=timeSetEvent(wTimerRes,wAccuracy,
		(LPTIMECALLBACK) TimeProc,(DWORD)this,TIME_PERIODIC))==0)
		{
			AfxMessageBox("不能进行定时!",MB_OK|MB_ICONASTERISK);
		}
//		else
//		MessageBox("计数器加载成功!");
	}
//	pDlgAmp->Create(IDD_DLG_AMP);
	pDlgGuassFit->Create(IDD_DLG_GAUSS);
	if(bWaveShort)
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_RADIO_WAVE_SHORT)))->SetCheck(1);
	else
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_RADIO_WAVE_LONG)))->SetCheck(1);
/*
	if(bWaveDisp[0])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH1)))->SetCheck(1);
	if(bWaveDisp[1])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH2)))->SetCheck(1);
	if(bWaveDisp[2])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH3)))->SetCheck(1);
	if(bWaveDisp[3])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH4)))->SetCheck(1);
	if(bWaveDisp[4])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH5)))->SetCheck(1);
	if(bWaveDisp[5])	
		((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH6)))->SetCheck(1);
*/
	initTCP();

	Init();
	setWaveMode(bWaveShort,0,0);
	OnMenuBaseline();  
	OnMenuitemSetLd();
//	OnMenuSingle();
//	bWaitTriger=1;
//	OnMenuNormal();
	OnMenuitemForward();
	if(bAdcWorkOn[0])
		OnMenu50();
	else
		OnMenu10k();
	mAdcGain=1;
	SetSampleRate(mAdcGain);
	bWaitTriger=0;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(LONG)FWS_ADDTOTITLE;
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnRadioOne() 
{
	
}

void CMainFrame::OnRadioTwo() 
{
}

void CMainFrame::OnRadioHardWare() 
{
	int i,nMca;
	nMca=0;
	CString sdate;
	CString stime;
	for(i=0;i<8;i++)
		pDispAdcBuf[i]=pAdcBuf[i];
	i=0;
		w1[i]->curTime=hardSatrtTime[i];
		sdate.Format("%02d.%02d.%02d",w1[i]->curTime.GetYear()-2000,
			w1[i]->curTime.GetMonth(),w1[i]->curTime.GetDay());
		m_pDlgBar->SetDlgItemText(IDC_STATIC_DATE2,sdate.GetBuffer(20));
		stime= w1[i]->curTime.Format("%X");
		m_pDlgBar->SetDlgItemText(IDC_STATIC_TIME2,stime.GetBuffer(20));
		w1[i]->Invalidate(0);
	bDispBuffer=0;
}
void CMainFrame::OnRadioBuffer() 
{
	int i,nMca;
	if(bDispMca[0])
		nMca=0;
	else
		nMca=4;
	CString sdate;
	CString stime;
	for(i=0;i<8;i++)
		pDispAdcBuf[i]=pMemBuf[i];
	i=0;
		w1[i]->curTime=bufStartTime[i];
		sdate.Format("%02d.%02d.%02d",w1[i]->curTime.GetYear()-2000,
			w1[i]->curTime.GetMonth(),w1[i]->curTime.GetDay());
		m_pDlgBar->SetDlgItemText(IDC_STATIC_DATE2,sdate.GetBuffer(20));
		stime= w1[i]->curTime.Format("%X");
		m_pDlgBar->SetDlgItemText(IDC_STATIC_TIME2,stime.GetBuffer(20));
		w1[i]->Invalidate(0);

	bDispBuffer=1;
}

void CMainFrame::DoTimer1s() 
{
int	i;
		CTime curTime = CTime::GetCurrentTime();
		year = curTime.GetYear();
		month = curTime.GetMonth();
		day = curTime.GetDay();
		hour = curTime.GetHour();
		minute = curTime.GetMinute();
		second = curTime.GetSecond();
		if(day!=oldday)
		{
			oldday = day;
			CString sdate;
			sdate.Format("%02d.%02d.%02d",curTime.GetYear()-2000,
				curTime.GetMonth(),curTime.GetDay());
			m_pDlgBar->SetDlgItemText(IDC_STATIC_DATE,sdate.GetBuffer(20));
		}
		if(second!=oldsecond)
		{
			oldsecond = second;
			CString stime;

			stime= curTime.Format("%X");
			m_pDlgBar->SetDlgItemText(IDC_STATIC_TIME,stime.GetBuffer(20));
		}
}

void CMainFrame::OnDestroy() 
{
	int i;
	if(timerID) KillTimer(2);
	timeKillEvent(TimerID);
	timeEndPeriod(wAccuracy);
//	Sleep(1000);
	if(bOpenRs232)
		CloseHandle(hCom);

	for(i=0;i<8;i++)
	{
		delete [] pAdcBuf[i];
	}

	for(i=0;i<8;i++)
	{
		delete [] pMemBuf[i];
	}
	CMDIFrameWnd::OnDestroy();
}

void CMainFrame::AdcChange()
{
	Gain[0] = MaxChannel;
	Gain[1] = MaxChannel;
	Gain[2] = MaxChannel;
	Gain[3] = MaxChannel;
//	MaxChannel = AdcGain;
	CString str;
	int	xrange;
	if(mAdcGain==1)
		xrange=100;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;

	str.Format("%d",xrange);
	w1[0]->dispspm.spara.StartChn = 0;
	for(int i=0;i<1;i++)
	{
		w1[i]->dispspm.spara.Horz = Gain[i];
		w1[i]->dispspm.spara.StartChn = 0;
		m_pDlgBarState[i]->GetDlgItem(IDC_STATIC_X_RANGE)->SetWindowText(str);
		w1[i]->Invalidate(0);
	}
}
void CMainFrame::OnUpdateAdc512(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(AdcGain==512);
//	pCmdUI->Enable(!bStart&&!bStart);
}

void CMainFrame::OnUpdateAdc1024(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(AdcGain==1024);	
//	pCmdUI->Enable(!bStart&&!w1[1]->bStart);
}

void CMainFrame::OnUpdateAdc2048(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(AdcGain==2048);	
//	pCmdUI->Enable(!bStart&&!w1[1]->bStart);
}

void CMainFrame::OnUpdateAdc4096(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(AdcGain==4096);	
//	pCmdUI->Enable(!bStart&&!w1[1]->bStart);
}

void CMainFrame::OnUpdateAdc8192(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(AdcGain==8192);	
//	pCmdUI->Enable(!bStart&&!w1[1]->bStart);
}

void CMainFrame::OnMenuBkcolor() 
{
	int i;
	CColorDialog dlg(0,CC_FULLOPEN|CC_RGBINIT,this);
	if(dlg.DoModal()==IDOK)
	{
		mbkcolor=dlg.GetColor();
		for(i=0;i<1;i++)
			w1[i]->mChangColor();
	}
}
void CMainFrame::OnMenuForecolor() 
{
	CColorDialog dlg(0,CC_FULLOPEN|CC_RGBINIT,this);
	if(dlg.DoModal()==IDOK)
	{
		mforecolor=dlg.GetColor();
		w1[0]->mChangColor();
//		w1[0]->Invalidate(0);
	}
	
}
void CMainFrame::OnMenuRoicolor() 
{
	CColorDialog dlg(0,CC_FULLOPEN|CC_RGBINIT,this);
	if(dlg.DoModal()==IDOK)
	{
		mredcolor=dlg.GetColor();
		w1[0]->Invalidate(0);
	}
}

void CMainFrame::OnMenuCursorcolor() 
{
	CColorDialog dlg(0,CC_FULLOPEN|CC_RGBINIT,this);
	if(dlg.DoModal()==IDOK)
	{
		mcursorcolor=dlg.GetColor();
		w1[0]->Invalidate(0);
	}
}

void CMainFrame::OnTimePreset() //基线刻度
{
	// TODO: Add your command handler code here
	int i;
	CDlgBaseLine dlg;
//	dlg.m_Preset_Time = nPresetTime[viewNum];
	if(dlg.DoModal()==IDOK)
	{
	}
}


void CMainFrame::OnMenuMcatobuf() 
{
	// TODO: Add your command handler code here
	int i;
	for(i=0;i<1;i++)
	{
		memcpy(pMemBuf[i],pAdcBuf[i],8200*4);
	}

	for(i=0;i<1;i++)
	{
		bufStartTime[i]=hardSatrtTime[i];
		w1[i]->Invalidate(0);
	}
}

void CMainFrame::OnMenuEkedu() 
{
	// TODO: Add your command handler code here
	CRect rect,rect1,rect2;
	GetWindowRect(&rect);
	rect2.left=rect.right-rect1.Width();
	rect2.top=(rect.bottom-rect1.Height())/2;
	rect2.right=rect2.left+rect1.Width();
	rect2.bottom=rect2.top+rect1.Height();
	
	
}



void CMainFrame::OnMenuHelp() 
{
	// TODO: Add your command handler code here
//	HtmlHelp("help\\mca1help.chm",HH_DISPLAY_TOPIC);	
	
}

void CMainFrame::OnMenuCloseInt() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnMenuRs232Set() 
{
	// TODO: Add your command handler code here

	CDlgRS232 dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(bOpenRs232)
		{
		Init();
		OnMenuBaseline();  
		OnMenuitemSetLd();
		OnMenuNormal();
		OnMenuitemForward();
		if(bAdcWorkOn[0])
			OnMenu50();
		else
			OnMenu10k();
		SetSampleRate(mAdcGain);
		}
		//CloseHandle(hCom);
//		m_pDlgBar->SetDlgItemText(IDC_STATIC_COM,strCom[nComx]);
//		OpenRS232();
	}
/*
	DWORD device=0;
	SI_STATUS status = SI_Open(device, &m_hUSBDevice);
	// setup the COM Port
	if (status == SI_SUCCESS)
	{
		// Clear the CP210x device's buffers
		status = SI_FlushBuffers(m_hUSBDevice, TRUE, TRUE);
		status = SI_SetBaudRate(m_hUSBDevice, 115200);
		SI_SetTimeouts(400,400);
	}
*/
}
void CMainFrame::ReceiveData()
{
}
void CMainFrame::ReceiveDataWave()
{
int		i;
unsigned long nRxLength;
BYTE	checksum;
bool	bWave=0;
	nRxLength = ReadCommData(hCom,m_RxData);
	if(nRxLength<=30)
	{
		nReceiveNum++ ;
		if(nReceiveNum>=3)
		{
//			usbState="RS232通讯错误!";
			nReceiveNum = 0;

		}
		return;
	}
	else 
	{
		int j=0;
		bWaitTriger=0;
		nReceiveNum=0;
//        int k1=inBuf.GetSize()/3;
		int k1=(nRxLength-16)/2;
		timer100ms=0;

//		if(k1>=1024)
		{

			int adcNum0=m_RxData[9];
			int adcStartTransCh0=m_RxData[10];
			if(adcStartTransCh0>3)
				adcStartTransCh0=3;
 		    int nStart=0;
			
			if(adcNum0>3)
				adcNum0=3;
			short byteLen;
			BYTE bdata[300];
			BYTE b0,b1,b3,b4;
			int waveLen=4096;
			for(i=0;i<4096;i++)
			{
				b0=m_RxData[i];
				b1=m_RxData[i+1];
				if(b0==0x11&&b1==0x22)
				{
				b3=m_RxData[i+waveLen*2+14];
				b4=m_RxData[i+waveLen*2+15];
				if(b3==0xaa&&b4==0xbb)
				{
					bWave=1;
					break;
				}
				}
			}
			nStart=i;
			if(bWave)
			{
				for(i=0;i<waveLen;i++)
				{
						pAdcBuf[nCurWave][i]=m_RxData[2*i+15+nStart]+256*m_RxData[2*i+14+nStart];
				}
				nCurWave++;
				if(nCurWave>=32)
					nCurWave=0;
				w1[0]->Invalidate(0);
			}
			bReceiveEnd=1;
			usbState="RS232通讯正常!";


		}
	}
}
void CMainFrame::ReceiveDataWaveTCP2(int iClientSock) //长波形
{
	int nRxLength;
	int i,nRevBegin;
	nRxLength=ReadTCPData(iClientSock,m_RxData);
	if(nRxLength>0)
	{
		if(!bRevBegin)
		{
			if(nRxLength>22)
			{
				nRevBegin=nRxLength-1;
				for(i=0;i<nRxLength-1;i++)
					if(m_RxData[i]==0x11&&m_RxData[i+1]==0x22)
					{
						bRevBegin=1;
						nRevBegin=i+2;
						break;
					}
				if(bRevBegin)
				{
					nCurRevLen=(nRxLength-nRevBegin-18)/8;
					remainRevLen=(nRxLength-nRevBegin-18)%8;
					if(nCurRevLen>0)
					{
						for(i=0;i<nCurRevLen;i++)
						{
							pAdcBuf[0][i*2]=m_RxData[i*8+20]*256+m_RxData[i*8+21]-20000;
							pAdcBuf[0][i*2+1]=m_RxData[i*8+22]*256+m_RxData[i*8+23]-20000;
							pAdcBuf[1][i*2]=m_RxData[i*8+24]*256+m_RxData[i*8+25];
							pAdcBuf[1][i*2+1]=m_RxData[i*8+26]*256+m_RxData[i*8+27];
						}
						w1[0]->Invalidate(0);
						nPreRevLen=nCurRevLen;
					}
					for(i=0;i<remainRevLen;i++)
						m_RxDataBuf[i]=m_RxData[nRxLength-remainRevLen+i];
	//				memcpy(m_RxDataBuf,m_RxData+nRxLength-remainRevLen,remainRevLen);
					nPreDispLen=nRxLength-20;
					memcpy(m_RxDataBuf,m_RxData+20,nRxLength-20);
				}
			}
		}
		else
		{
			memcpy(m_RxDataBuf+nPreDispLen,m_RxData,nRxLength);
			nCurRevLen=nRxLength/8;
			if(nCurRevLen+nPreRevLen>5e5)
			{
				nCurRevLen=1e6-nPreRevLen;
				bRevBegin=0;
				bWaitHardware=0;
				OnBnClickedCalCount();
			}
			BYTE *pRxData=m_RxDataBuf+nPreRevLen*8;
			for(i=0;i<nCurRevLen;i++)
			{
				pAdcBuf[0][nPreRevLen*2+i*2]=pRxData[i*8]*256+pRxData[i*8+1]-20000;
				pAdcBuf[0][nPreRevLen*2+i*2+1]=pRxData[i*8+2]*256+pRxData[i*8+3]-20000;
				pAdcBuf[1][nPreRevLen*2+i*2]=pRxData[i*8+4]*256+pRxData[i*8+5];
				pAdcBuf[1][nPreRevLen*2+i*2+1]=pRxData[i*8+6]*256+pRxData[i*8+7];
			}
			w1[0]->Invalidate(0);
			m_pDlgBarState[0]->SetDlgItemInt(IDC_REV_LEN,nRxLength);
			nPreRevLen+=nCurRevLen;
			nPreDispLen=nPreDispLen+nRxLength;
		}	
	}
	pAdcBuf[0][0]=nRxLength;
//	m_pDlgBarState[0]->SetDlgItemInt(IDC_REV_LEN,nRxLength);
}

void CMainFrame::ReceiveDataWaveTCP(int iClientSock) //短波形
{
	static int recLen;
	int nRxLength;
	int i,nRevBegin;
	bool	bRxEnd=0;
	BYTE	temp[8300];
	nRxLength=ReadTCPData(iClientSock,m_RxData);
	if(nRxLength>0)
	{
		if(!bRevBegin)
		{
			if(nRxLength>22)
			{
				nRevBegin=nRxLength-1;
				for(i=0;i<nRxLength-1;i++)
					if(m_RxData[i]==0x11&&m_RxData[i+1]==0x22)
					{
						bRevBegin=1;
						nRevBegin=i+2;
						recLen=nRxLength-nRevBegin+2;
						break;
					}
					memcpy(m_RxDataBuf,m_RxData,nRxLength-nRevBegin+2);
					nPreDispLen=nRxLength-nRevBegin+2;
					m_pDlgBarState[0]->SetDlgItemInt(IDC_REV_LEN,nRxLength);
					if(nPreDispLen>=8214)
						bRxEnd=1;

				}
			}
			else
			{
				memcpy(m_RxDataBuf+nPreDispLen,m_RxData,nRxLength);
				nPreDispLen+=nRxLength;
				if(nPreDispLen>=8214)
					bRxEnd=1;
			}
		}
	if(bRxEnd)
	{
		bRevBegin=0;
		bWaitHardware=0;
		memcpy(temp,m_RxDataBuf,8214);
		for(i=0;i<4096;i++)
		{
			pAdcBuf[0][i]=m_RxDataBuf[i*2+20]*256+m_RxDataBuf[i*2+21];
//			pAdcBuf[1][i]=m_RxDataBuf[4096+i*2]*256+m_RxDataBuf[4096+i*2+1];
		}
		w1[0]->Invalidate(0);
		m_pDlgBarState[0]->SetDlgItemInt(IDC_REV_LEN,nPreDispLen);
	}
	pAdcBuf[0][0]=nRxLength;
//	m_pDlgBarState[0]->SetDlgItemInt(IDC_REV_LEN,nRxLength);
}
int CMainFrame::ReadTCPData(int iClientSock,BYTE *m_RxData)
{
	int   nLength;
	BYTE data[9000];
	nLength=recv(iClientSock,(char *)m_RxData,20000,0);
	if(nLength<0)
		nLength=0;
//	memcpy(data,m_RxData,9000);
//	if(nLength==-1)
//		WSACleanup( );
	return nLength;
}
int CMainFrame::ReadCommData(HANDLE hCom,BYTE *m_RxData)
{

	BOOL	fReadStat;
	COMSTAT ComStat;
	DWORD   dwErrorFlags,dwLength;
	ClearCommError(hCom,&dwErrorFlags,&ComStat);
	dwLength=ComStat.cbInQue;
	lpnumber=0;
	if(dwLength>0)
	{
		if(dwLength>8300*2)
			dwLength=8300*2;
		fReadStat = ReadFile(hCom,m_RxData,dwLength,&lpnumber,NULL);
	}
	return lpnumber;
/*
	SI_STATUS status;
	DWORD read;
	DWORD toBeRead;
	DWORD lpdwNumBytesInQueue;
	DWORD lpdwQueueStatus[500];
	SI_CheckRXQueue(m_hUSBDevice,&lpdwNumBytesInQueue,lpdwQueueStatus);
	toBeRead=lpdwNumBytesInQueue;
	read=0;
	if(toBeRead>0)
		SI_Read(m_hUSBDevice, m_RxData, toBeRead, &read);
	return read;
*/
}
void CMainFrame::SendData()
{
	bool success = false;
	DWORD written;
	if(bTrans)
	{
		bTrans=0;
		m_TxData[0]=0x12;
		m_TxData[1]=0x34;
//		m_TxData[2]=0x0f;
		m_TxData[9]=0xab;
		m_TxData[10]=0xcd;
		toBeWritten=11;
		WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	}
/*
	else
	{
		if(!bWaitTriger)
		{
			OnMenuSingle();
			bWaitTriger=1;
		}
	}
*/
}
void CMainFrame::DoTimer() 
{
	nTimerTick++;
	bSendSingle=!bSendSingle;
	if(bOpenRs232&&!bStopWave)
	{
		if(bSendSingle)
		{
			if(!bWaitTriger)
			{
				OnMenuSingle();
				bWaitTriger=1;
			}
		}
		else
		{
			ReceiveDataWave();
		}
	}
	if(bTCPIPOK)
	{
		if(bWaveShort)
			ReceiveDataWaveTCP(iClientSock);  //短波形
		else
			ReceiveDataWaveTCP2(iClientSock); //长波形
	}
	timer100ms++;
   if(timer100ms>9)
   {
	timer100ms=0;
   }
	if(nTimerTick>=(1000/wTimerRes))
	{
		nTimerTick=0;
		DoTimer1s();
	}	
}

void CMainFrame::OnMenuPaperStart() 
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnMenuPaperStop() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnMenuPumpOn() 
{
	// TODO: Add your command handler code here
	DWORD written;
	SI_STATUS status;
	m_TxData[3]=0xc1;
	m_TxData[4]=0;
	m_TxData[5]=0;
	m_TxData[6]=0;
	m_TxData[7]=0;
	bTrans=1;
//	status = SI_Write(m_hUSBDevice, (LPVOID)m_TxData, toBeWritten, &written);
 	bReceiveEnd=0;
	pumpOn=1;
	
}
void CMainFrame::OnUpdateMenuPaperStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!paperOn);
	
}

void CMainFrame::OnUpdateMenuPaperStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(paperOn);
	
}

void CMainFrame::OnMenuPumpOff() 
{
	// TODO: Add your command handler code here
	DWORD written;
	SI_STATUS status;
	m_TxData[3]=0xc2;
	m_TxData[4]=0;
	m_TxData[5]=0;
	m_TxData[6]=0;
	m_TxData[7]=0;
	bTrans=1;
//	status = SI_Write(m_hUSBDevice, (LPVOID)m_TxData, toBeWritten, &written);
 	bReceiveEnd=0;
	pumpOn=0;
	
}

void CMainFrame::OnUpdateMenuPumpOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pumpOn);
	
}

void CMainFrame::OnUpdateMenuPumpOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!pumpOn);
	
}


void CMainFrame::OnMenuAlarmOn() 
{
	// TODO: Add your command handler code here
	DWORD written;
	SI_STATUS status;
	m_TxData[3]=0xd1;
	m_TxData[4]=0;
	m_TxData[5]=0;
	m_TxData[6]=0;
	m_TxData[7]=0;
	bTrans=1;
//	status = SI_Write(m_hUSBDevice, (LPVOID)m_TxData, toBeWritten, &written);
 	bReceiveEnd=0;
	alarmOn=1;
}

void CMainFrame::OnUpdateMenuAlarmOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!alarmOn);
	
}

void CMainFrame::OnMenuAlarmOff() 
{
	// TODO: Add your command handler code here
	DWORD written;
	SI_STATUS status;
	m_TxData[3]=0xd2;
	m_TxData[4]=0;
	m_TxData[5]=0;
	m_TxData[6]=0;
	m_TxData[7]=0;
	bTrans=1;
//	status = SI_Write(m_hUSBDevice, (LPVOID)m_TxData, toBeWritten, &written);
 	bReceiveEnd=0;
	alarmOn=0;
}

void CMainFrame::OnUpdateMenuAlarmOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(alarmOn);
	
}

void CMainFrame::OnPaperV() 
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnMenuAutoSetup() 
{
	// TODO: Add your command handler code here
	CDlgAutoSetup dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnMenuAutoStart() 
{
	
}
void CMainFrame::OnUpdateMenuAutoStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!autoData.bStart);
	
}

void CMainFrame::OnUpdateMenuAutoStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(autoData.bStart);
	
}

void CMainFrame::OnMenuAutoClear() 
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateMenuAutoClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!autoData.bStart);
	
}
void CMainFrame::OnMenuAutoStop() 
{
	// TODO: Add your command handler code here
	autoData.bStart=0;
}

void CMainFrame::DoTimerAuto() 
{
}
struct CSetdDta
{
	unsigned char	m_AlarmPaper; //是否走纸故障报警  1  是  0 否
	unsigned char	m_AlarmFlux;//是否流量故障报警
	unsigned char	m_AlarmBeta;//是否Beta计数越限报警
	unsigned char	m_AlarmAlpha;//是否Alpha计数越限报警
	unsigned short	m_Alpha1;//报警下限
	unsigned short	m_Alpha2;//报警上限
	unsigned short	m_Beta1;//报警下限
	unsigned short 	m_Beta2;//报警上限
	unsigned short	m_Flux1;//报警下限
	unsigned short	m_Flux2;//报警上限
	unsigned short	m_Paper1;//报警周期
	unsigned short	m_Rn220H;//上阈
	unsigned short	m_Rn220L;//下阈
	unsigned short	m_Rn222H;//上阈
	unsigned short	m_Rn222L;//下阈
	unsigned short	m_Pu239H;//上阈
	unsigned short	m_Pu239L;//下阈
	unsigned short	m_U235H;//上阈
	unsigned short	m_U235L;//下阈
	unsigned short	m_BetaH;//上阈
	unsigned short	m_BetaL;//下阈
	unsigned short	m_Pu239S1;
	unsigned short	m_Pu239S2;
	unsigned short	m_Pu239S3;
	unsigned short	m_Pu239S4;
	unsigned short	m_Pu239S5;
	unsigned short	m_Rn220S1;
	unsigned short	m_Rn220S2;
	unsigned short	m_Rn220S3;
	unsigned short	m_Rn220S4;
	unsigned short	m_Rn220S5;
	unsigned short	m_Rn222S1;
	unsigned short	m_Rn222S2;
	unsigned short	m_Rn222S3;
	unsigned short	m_Rn222S4;
	unsigned short	m_Rn222S5;
	unsigned short	m_U235S1;
	unsigned short	m_U235S2;
	unsigned short	m_U235S3;
	unsigned short	m_U235S4;
	unsigned short	m_U235S5;
	unsigned short	m_Address;
	unsigned short	m_Period;
	unsigned short	baudrate;
	float	m_BetaEff;
};
struct CSetdDta  setData;


void CMainFrame::OnMenuSetC51() 
{
	// TODO: Add your command handler code here
/*
	CDlgSetC51 dlg;
	int x=sizeof(setData);
	if(dlg.DoModal()==IDOK)
	{
		m_TxData[3]=0xe1;
		int i;
		unsigned char *pData;
		pData=(unsigned char *)&c51Data;
		for(i=0;i<90;i++)
			m_TxData[4+i]=*(pData+i);
		bTrans=1;
	}
	*/
}



void CMainFrame::OnMenuAmplifier() 
{
	// TODO: Add your command handler code here
//	if(dlg.DoModal()==IDOK)
//	pDlgAmp->ShowWindow(SW_SHOW);
/*
	{
		m_TxData[3]=0xa6;
		m_TxData[4]=amplifier;
		m_TxData[5]=0;
		m_TxData[6]=0;
		m_TxData[7]=0;
		bTrans=1;
	}
*/
}

void CMainFrame::OnMenuFit() 
{
	// TODO: Add your command handler code here
//	fitGass();
//	w1[0]->Invalidate(0);
	pDlgGuassFit->ShowWindow(SW_SHOW);
	
}

void CMainFrame::OnMenuitemCloseRs232() 
{
	// TODO: Add your command handler code here
	if(bRS232)
	{
		CloseHandle(hCom);
		bRS232=0;
	}
	else
	{
		OpenRS232();
		bRS232=1;

	}

}
void  CMainFrame::OpenRS232()
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


void CMainFrame::OnMenuitemDc() 
{
	// TODO: Add your command handler code here
	bAdcSet[0]=0;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x20*bAdcSet[2]+0x40*bAdcSet[1]+0x80*bAdcSet[0];
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
//	bTrans=1;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	
}

void CMainFrame::OnMenuitemAc() 
{
	// TODO: Add your command handler code here
	bAdcSet[0]=1;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x20*bAdcSet[2]+0x40*bAdcSet[1]+0x80*bAdcSet[0];
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
//	bTrans=1;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	
}
void CMainFrame::OnMenuitemForward() 
{
	// TODO: Add your command handler code here
	bAdcSet[1]=0;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
//	bTrans=1;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}

void CMainFrame::OnMenuitemReverse() 
{
	// TODO: Add your command handler code here
	bAdcSet[1]=1;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x20*bAdcSet[2]+0x40*bAdcSet[1]+0x80*bAdcSet[0];
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
//	bTrans=1;
}

void CMainFrame::OnMenuitemShiftCode() 
{
	// TODO: Add your command handler code here
	bAdcSet[2]=0;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x20*bAdcSet[2]+0x40*bAdcSet[1]+0x80*bAdcSet[0];
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
//	bTrans=1;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}

void CMainFrame::OnMenuitemCompCode() 
{
	// TODO: Add your command handler code here
	bAdcSet[2]=1;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x70;
    m_TxData[5]=0x20*bAdcSet[2]+0x40*bAdcSet[1]+0x80*bAdcSet[0];
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
//	bTrans=1;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}

void CMainFrame::OnMenuitemSetLd() //调阈值
{
	// TODO: Add your command handler code here
	int fLD_DAC=(fLD-TH_b)/TH_k;
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x010;
    m_TxData[5]=0x06;
    m_TxData[6]=0x0;
    m_TxData[7]=fLD_DAC/256;
    m_TxData[8]=fLD_DAC%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	
}
void CMainFrame::Init()
{
	BYTE transData[20];
	char temp[200];
	FILE *fp;
	fp=fopen("initWaveCH6.dat","rt");
	if(fp!=NULL)
	{
		int num=0;
		while(!feof(fp))
		{
		fgets(temp,200,fp);
		sscanf(temp,"%x %x %x %x %x %x %x %x %x %x %x",&m_TxData[0],&m_TxData[1],&m_TxData[2],&m_TxData[3],&m_TxData[4],
			&m_TxData[5],&m_TxData[6],&m_TxData[7],&m_TxData[8],&m_TxData[9],&m_TxData[10]);
		toBeWritten=11;
		WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
		num++;
		}
	fclose(fp);	
	}
}

void CMainFrame::OnMenuAuto()//
{
	// TODO: 在此添加命令处理程序代码
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x11;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}


void CMainFrame::OnMenuNormal()
{
	// TODO: 在此添加命令处理程序代码
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x11;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x02;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}


void CMainFrame::OnMenuSingle()
{
	// TODO: 在此添加命令处理程序代码

	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x11;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}


void CMainFrame::OnMenuStop()
{
	// TODO: 在此添加命令处理程序代码
	bStopWave=!bStopWave;
	if(bStopWave)
		m_pDlgBar->SetDlgItemTextA(ID_MENU_STOP,"RUN");
	else
		m_pDlgBar->SetDlgItemTextA(ID_MENU_STOP,"Stop");
}


void CMainFrame::OnMenuForce()
{
	// TODO: 在此添加命令处理程序代码
//	12 34 0F 00 04 03 00 00 00 AB CD


    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x03;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	bRevBegin=0;
	bTrans=1;
	
/*
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x06;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x03;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
*/
}


void CMainFrame::OnMenuTripUp()
{
	// TODO: 在此添加命令处理程序代码
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x11;
    m_TxData[5]=0x02;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}


void CMainFrame::OnMenuTripDown()
{
	// TODO: 在此添加命令处理程序代码
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x11;
    m_TxData[5]=0x02;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}


void CMainFrame::OnMenuTripLevel()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenu50()
{
	// TODO: 在此添加命令处理程序代码
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x07;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	bAdcWorkOn[0] = 1;

}


void CMainFrame::OnMenu10k()
{
	// TODO: 在此添加命令处理程序代码
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x07;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=0;
    m_TxData[8]=0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
	bAdcWorkOn[0] = 0;

}


void CMainFrame::OnMenuBaseline()
{
	// TODO: 在此添加命令处理程序代码
	fLD2=35000;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x60;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=fLD2/256;
    m_TxData[8]=fLD2%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);

}

void CMainFrame::OnMenuitemSetLd2()
{
	// TODO: 在此添加命令处理程序代码
	fLD2=35000;
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x60;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=fLD2/256;
    m_TxData[8]=fLD2%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);

}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_wndStatusBar.SetPaneText(1,usbState);
	CMDIFrameWnd::OnTimer(nIDEvent);
}




void CMainFrame::OnBnClickedMenuitemDc2()//采样率41.7M
{
	// TODO: 在此添加控件通知处理程序代码
	mAdcGain=1;
	SetSampleRate(1);

}
void CMainFrame::OnBnClickedMenuitemForward2()//采样率25M
{
	// TODO: 在此添加控件通知处理程序代码
	mAdcGain=2;
	SetSampleRate(2);
}
void CMainFrame::OnBnClickedMenuitemShiftCode2()//采样率7.8M
{
	// TODO: 在此添加控件通知处理程序代码
	mAdcGain=3;
	SetSampleRate(3);
}

void CMainFrame::SetSampleRate(int n)//采样率设置 n=1: 41.7M  n=2:  25M   n=3:  7.8M
{
//指令1
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0;
    m_TxData[7]=0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
//指令2
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x05;
    m_TxData[5]=0x0;
    m_TxData[6]=0;
    m_TxData[7]=0;
	if(n==3)//7.8M
		m_TxData[8]=0x07;
	else
		m_TxData[8]=0x03;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
//指令3
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x50;
    m_TxData[5]=0x0;
    m_TxData[6]=0;
    m_TxData[7]=0xb0;
	if(n==1)//41.7M
		m_TxData[8]=0x02;	
	else if(n==2)//25M
		m_TxData[8]=0x04;	
	else   //7.8M
		m_TxData[8]=0x07;	
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
//指令4
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0;
    m_TxData[7]=0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
}



void CMainFrame::OnMenuitemSetHv()//设置高压
{
	// TODO: 在此添加命令处理程序代码
	CDlgAmp dlg;
	dlg.DoModal();
}


void CMainFrame::OnBnClickedRun()//外部触发开始
{
	// TODO: 在此添加控件通知处理程序代码
//	bStopWave=0;
	bHardware=1;
	bWaitHardware=1;
	setWaveMode(bWaveShort,bHardware,1);
/*
	Init();
	OnMenuBaseline();  
	OnMenuitemSetLd();
//	OnMenuSingle();
	OnMenuitemForward();
	OnMenuNormal();
*/


}


void CMainFrame::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	bHardware=0;
	bWaitHardware=0;
	bRevBegin=0;
}


void CMainFrame::OnBnClickedForce()
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuForce();
}


void CMainFrame::OnBnClickedHvOpen()//开关高压
{
	// TODO: 在此添加控件通知处理程序代码
	int nDAC=100;
	m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x01;
    m_TxData[4]=0x60;
    m_TxData[5]=0x0;
    m_TxData[6]=0x0;
    m_TxData[7]=nDAC/256;
    m_TxData[8]=nDAC%256;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);

}
void CMainFrame::OnUpdateForce(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!bRevBegin);
}
void CMainFrame::OnUpdateRun(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!bWaitHardware);
}
void CMainFrame::OnUpdateStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(bWaitHardware);
}
void CMainFrame::initTCP()
{
// TCP-IP初始化开始**************************************************
	WSADATA WSAData;
	bTCPIPOK=1;
	if( WSAStartup( MAKEWORD( 2, 2 ), &WSAData ) )
	{
//	printf( "initializationing error!\n" );
	WSACleanup( );
	bTCPIPOK=0;
//	exit( 0 );
	}
	if(bTCPIPOK)
	{
		if( ( iClientSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
		{
	//printf( "创建套接字失败!\n" );
		WSACleanup( );
		bTCPIPOK=0;
	//exit( 0 );
		}
	}
		int iMode = 1;
		int retVal;
	if(bTCPIPOK)
	{
//		retVal= ioctlsocket(iClientSock, FIONBIO, (u_long FAR*)&iMode);  //设置非阻塞模式
	DWORD TimeOut=6000; //设置发送超时6秒
	if(::setsockopt(iClientSock,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
	return ;
	}
	TimeOut=6000;//设置接收超时6秒
	if(::setsockopt(iClientSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
	return ;
	}
	//设置非阻塞方式连接
	unsigned long ul = 1;
	retVal = ioctlsocket(iClientSock, FIONBIO, (unsigned long*)&ul);
	if(retVal==SOCKET_ERROR)return ;

		retVal=0;
		if(retVal==SOCKET_ERROR)
		{
			bTCPIPOK=0;
			WSACleanup( );
		}
		else
		{
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(24);
			ServerAddr.sin_addr.s_addr = inet_addr(serveIP );//记得换IP
			memset( &( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );
			if( connect( iClientSock, ( struct sockaddr * ) & ServerAddr, sizeof( struct sockaddr ) ) == -1 )
			{
	//		printf( "connect失败!" );
				WSACleanup( );
	//		exit( 0 );
				bTCPIPOK=0;
			}
			usbState.Format("TCP/IP连接成功！");
		}
	}
	retVal= ioctlsocket(iClientSock, FIONBIO, (u_long FAR*)&iMode);  //设置非阻塞模式
	if(!bTCPIPOK)
		usbState.Format("TCP/IP连接失败！");
// TCP-IP初始化结束**************************************************
}
int CMainFrame::getCount(int ch)
{
	int i;
	bool	bUp,bDown;
	int		nUp,nDown;
	int		nCount=0;
	bUp=0;
	nUp=0;
	bDown=0;
	for(i=0;i<1000000;i++)
	{
		if(bUp)
		{
			if(pAdcBuf[ch][i]>fLDWave[ch])
				nUp++;
			else
			{
				if(nUp<10)
				{
					bUp=0;
					nUp=0;
				}
				if(bUp)
				{
					if(bDown)
					{
						nDown++;
						if(nUp>30&&nDown>30)
						{
							nCount++;
							bUp=0;
							bDown=0;
						}
					}
					else
					{
						bDown=1;
						nDown=1;
					}
				}
			}
		}
		else
		{
			if(pAdcBuf[ch][i]>fLDWave[ch])
			{
				bUp=1;
				nUp=1;
			}
		}
	}
	return nCount;
}

void CMainFrame::OnBnClickedCh1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH1)))->GetCheck())
		bWaveDisp[0]=1;
	else
		bWaveDisp[0]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCh2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH2)))->GetCheck())
		bWaveDisp[1]=1;
	else
		bWaveDisp[1]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCh3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH3)))->GetCheck())
		bWaveDisp[2]=1;
	else
		bWaveDisp[2]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCh4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH4)))->GetCheck())
		bWaveDisp[3]=1;
	else
		bWaveDisp[3]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCh5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH5)))->GetCheck())
		bWaveDisp[4]=1;
	else
		bWaveDisp[4]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCh6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_CH6)))->GetCheck())
		bWaveDisp[5]=1;
	else
		bWaveDisp[5]=0;
	w1[0]->Invalidate(0);
}


void CMainFrame::OnBnClickedCalCount()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount1;
	nCount1=getCount(0);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH1,nCount1);
	nCount1=getCount(1);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH2,nCount1);
	nCount1=getCount(2);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH3,nCount1);
	nCount1=getCount(3);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH4,nCount1);
	nCount1=getCount(4);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH5,nCount1);
	nCount1=getCount(5);
	m_pDlgBarCount->SetDlgItemInt(IDC_COUNT_CH6,nCount1);
}


void CMainFrame::OnCalCount()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedCalCount();
}


void CMainFrame::OnBnClickedRadioWaveShort()//短波形
{
	// TODO: 在此添加控件通知处理程序代码
	bWaveShort=1;
	MaxChannel=4000;
	AdcChange();
	setWaveMode(bWaveShort,0,1);
}


void CMainFrame::OnBnClickedRadioWaveLong()
{
	// TODO: 在此添加控件通知处理程序代码
	bWaveShort=0;
	MaxChannel=1000000;
	AdcChange();
	setWaveMode(bWaveShort,0,0);
}


void CMainFrame::OnBnClickedRadioWaveShort2()
{
	// TODO: 在此添加控件通知处理程序代码
	bWaveShort=1;
	MaxChannel=4000;
	AdcChange();
	setWaveMode(bWaveShort,0,0);
}

void CMainFrame::setWaveMode(bool bShortWave,bool bHardware,bool bCh1) 
{
    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x01;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[5]=0x0;
	if(bShortWave)
	{
		if(bHardware)
			m_TxData[8]=0x11;
		else
		{
			if(bCh1)
				m_TxData[8]=0x21;
			else
				m_TxData[8]=0x22;
		}
	}
	else
	{
		if(bHardware)
			m_TxData[8]=0x14;
		else
			m_TxData[8]=0x24;
	}
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[4]=0x03;
	if(bShortWave)
	{
//		if(bHardware)
//		    m_TxData[8]=0x01;
//		else
		if(bCh1)
			m_TxData[8]=0x01;
		else
			m_TxData[8]=0x02;

	}
	else
	{
	    m_TxData[8]=0x04;
	}
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);
    m_TxData[4]=0x04;
    m_TxData[5]=0x01;
    m_TxData[8]=0x00;
	WriteFileTCP(iClientSock,m_TxData,toBeWritten,lpnumber);

}


void CMainFrame::On32871()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSetLD dlg;
	dlg.DoModal();
}
