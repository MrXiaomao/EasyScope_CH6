// mcbcView.cpp : implementation of the CMcbcView class
//

#include "stdafx.h"
#include "mcbc.h"

#include "dialogtime.h"
#include "MainFrm.h"
#include "mcbcDoc.h"
#include "DisplaySpm.h"
#include "mcbcView.h"
#include "ChildFrm.h"
#include "define.h"
#include "math.h"
#include <winioctl.h>
#include "MCB_AVR_WDMioctl.h"
#include "define.h"
#include "DlgEnergyKedu.h"
#include "DlgRoIDisplay.h"
#include "Mmsystem.h"
#include "DlgSetRoi.h"
#include "SiUSBXp.h"
#include "WaveDisplay.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int			viewID=0;
COLORREF	lineColor;
class		DispSpm;
extern	int		MaxChannel;
int		Gain[4]={MaxChannel,MaxChannel,MaxChannel,MaxChannel};
extern	CMcbcView	*w1[4];
extern	long		*pAdcBuf[8];
extern	long		*pDispAdcBuf[8];
extern	CDialogBar *m_pDlgBar;
extern	CChildFrame *m_pChildWnd[4];
extern	CDialogBar *m_pDlgBarState[4];
extern	bool linebz; // linebz=1 表示打印谱
extern	COLORREF mbkcolor,mforecolor,mredcolor,mcursorcolor;
extern  CDialogBar *m_pDlgBarTime[4];
extern	BOOL	bTongbu;
extern 	unsigned	int	nPresetTime[6];
extern	BOOL	transbz[4];
extern	int	nOldRealTime,nOldRealTime2;
extern	int	nOldRealTime3,nOldRealTime4;
DWORD cbByteReturned;
HANDLE hDevice;
int	nStartX=0;
int	nStartY=40;
BYTE nAddress = 0x61;
extern  int		bDispMca[8];
extern CEnergyKedu energyKedu[4];
extern	CDlgRoIDisplay *pDlgRoIDisplay;
extern	long	*pMemBuf[8];
extern	bool	bDispBuffer;
extern	CTime bufStartTime[4],hardSatrtTime[4];
extern	void findPeak(long *y,int H,int w,float factor);
extern	bool	bPresetReal[4];
extern	CDlgSetRoi   *pDlgSetRoi;
extern	HANDLE m_hUSBDevice;
extern  BYTE		m_TxData[200];
extern  BYTE		*m_RxData;
extern CString  usbState;
extern HANDLE		hCom;
extern unsigned long	lpnumber;
DWORD toBeWritten;
bool	bTrans;
extern	BOOL	bStart;
extern unsigned char comAddr;
extern	bool	bFirstStart,bFirstStop;
extern	int		Vort;
extern	int	mAdcGain;
extern  bool	bAdcWorkOn[4];
extern  int		fLD;//阈值
extern  int		fLD2;//基线
extern  int		Vort;
extern bool	bStopWave;
extern bool bWaitTriger;
int	yBaseLine=0;
extern	int	nCurWave;
extern	int nWaveRepeat;
extern	double   HV_k,HV_b,TH_k,TH_b;
extern	bool	bRevBegin,bRevEnd;
extern	bool	bWaveDisp[6];
COLORREF adcColor[6];
/////////////////////////////////////////////////////////////////////////////
// CMcbcView

IMPLEMENT_DYNCREATE(CMcbcView, CView)

BEGIN_MESSAGE_MAP(CMcbcView, CView)
	//{{AFX_MSG_MAP(CMcbcView)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_EXPAND, OnButtonExpand)
	ON_BN_CLICKED(IDC_BUTTON_SMALL, OnButtonSmall)
	ON_BN_CLICKED(IDC_BTN_ADDY, OnBtnAddy)
	ON_BN_CLICKED(IDC_BTN_LOGY, OnBtnLogy)
	ON_BN_CLICKED(IDC_BTN_SUBY, OnBtnSuby)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_SETROI, OnMenuSetroi)
	ON_COMMAND(ID_MENU_CLEARROI, OnMenuClearroi)
	ON_COMMAND(ID_MENU_CLEAR, OnMenuClear)
	ON_COMMAND(ID_MENU_CLEAR_ALL, OnMenuClearAll)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_START, OnUpdateButtonStart)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_STOP, OnUpdateButtonStop)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_CLEAR, OnUpdateButtonClear)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_TIGGER_START, OnTiggerStart)
	ON_COMMAND(ID_MENU_FIND_PEAK, OnMenuFindPeak)
	ON_COMMAND(ID_MENU_SMOOTH, OnMenuSmooth)
	ON_COMMAND(ID_MENU_ROI_SET, OnMenuRoiSet)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_COMMAND(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
//	ON_UPDATE_COMMAND_UI(IDC_FORCE, OnUpdateForce)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMcbcView construction/destruction

CMcbcView::CMcbcView()
{
	// TODO: add construction code here
	viewNum=viewID;
	viewID++;
	lineColor=RGB(255,255,255);
	Roi = 1;
	boolRoiRect = FALSE;
	whitePen.CreatePen(PS_SOLID,1,lineColor);
	redPen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	CFile mfile;
	CString str;
	str.Format("ROI%d.dat",viewNum);
	if(mfile.Open(str,CFile::modeRead))
	{
		mfile.Read(&RoiNum,sizeof(RoiNum));
		mfile.Read(RoiStart,sizeof(RoiStart));
		mfile.Read(RoiEnd,sizeof(RoiEnd));
		mfile.Read(&dispspm.spara,sizeof(dispspm.spara));
		mfile.Read(&curTime,sizeof(CTime));
		mfile.Close();
	}
	bkBrush.CreateSolidBrush(mbkcolor);
	rectDC = 0;
	bStart = FALSE;
	bSetRoi = FALSE;
	bCoolTimeStart=0;
	transbz[viewNum] = 1;
	bSetRoiDlg=0;
	adcColor[0]=RGB(255,0,0);
	adcColor[1]=RGB(0,255,0);
	adcColor[2]=RGB(0,0,255);
	adcColor[3]=RGB(255,255,0);
	adcColor[4]=RGB(255,0,255);
	adcColor[5]=RGB(0,255,255);
	wavePen[0].CreatePen(PS_SOLID,1,mredcolor);
	wavePen[1].CreatePen(PS_SOLID,1,RGB(255,0,0));
	wavePen[2].CreatePen(PS_SOLID,1,RGB(0,255,0));
	wavePen[3].CreatePen(PS_SOLID,1,RGB(0,0,255));
	wavePen[4].CreatePen(PS_SOLID,1,RGB(255,255,0));
	wavePen[5].CreatePen(PS_SOLID,1,RGB(255,0,255));
	wavePen[6].CreatePen(PS_SOLID,1,RGB(0,255,255));

}

CMcbcView::~CMcbcView()
{
	whitePen.DeleteObject();
	redPen.DeleteObject();
	wavePen[0].DeleteObject();
	wavePen[1].DeleteObject();
	wavePen[2].DeleteObject();
	wavePen[3].DeleteObject();
	wavePen[4].DeleteObject();
	wavePen[5].DeleteObject();
	wavePen[6].DeleteObject();

	CFile mfile;
	CString str;
	str.Format("ROI%d.dat",viewNum);
	mfile.Open(str,CFile::modeCreate | CFile::modeWrite);
	mfile.Write(&RoiNum,sizeof(RoiNum));
	mfile.Write(RoiStart,sizeof(RoiStart));
	mfile.Write(RoiEnd,sizeof(RoiEnd));
	mfile.Write(&dispspm.spara,sizeof(dispspm.spara));
	mfile.Write(&curTime,sizeof(CTime));
	mfile.Close();
	bkBrush.DeleteObject();
}

BOOL CMcbcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMcbcView drawing

void CMcbcView::OnDraw(CDC* pDC)
{
	CMcbcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;
	GetClientRect(&rect);

	int nWidth=rect.Width();
	int nHeight=rect.Height();

	CDC  MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);

	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	kx=1;
	ky=1;
	CBrush brush;
	brush.CreateStockObject(LTGRAY_BRUSH);
	if(!MemDC.IsPrinting())
		MemDC.FillRect(&rect,&brush);
	nScrHeight = int(rect.Height()-50*ky);
	xTextWidth=int(80*kx);
	yTextWidth=int(50*ky);
	yRange = nScrHeight-yTextWidth;
	nScrWidth = int(rect.Width()-90*kx);
	xRange = nScrWidth-xTextWidth;

	CWaveDisplay waveDisplay;
	waveDisplay.pDC = &MemDC;
	waveDisplay.kx=kx;
	waveDisplay.ky=ky;
	waveDisplay.nScrOrgX = int(rect.left+20*kx);
	waveDisplay.nScrOrgY = int(rect.top+40*ky);
	waveDisplay.nScrWidth = nScrWidth;
	waveDisplay.nScrHeight =nScrHeight;
	waveDisplay.DrawLine();
	if(MemDC.IsPrinting())
	{
		ky = pDC->GetDeviceCaps(VERTRES)/600./2.;
		kx = pDC->GetDeviceCaps(HORZRES)/800.;
		rect.right = int(pDC->GetDeviceCaps(HORZRES)-120*kx);
		rect.bottom = int(pDC->GetDeviceCaps(VERTRES)/2-50*ky);
		if(rect.right>8000) rect.right=8000;
		linebz =1;
		rect.left = 0;
		rect.top = 0;
		nStartX=60*kx;
		nStartY=40*ky;
		MemDC.SetViewportOrg(int(100*kx),int(80*ky));
	}
	else
	{
		linebz = 0;
		nStartX=waveDisplay.nScrOrgX+xTextWidth;
		nStartY=waveDisplay.nScrOrgY;
		kx=1;
		ky=1;
	}
	Paint(&MemDC);
	CString str;
	int nScrOrgX,nScrOrgY;
	nScrOrgX=waveDisplay.nScrOrgX+160;
	nScrOrgY=waveDisplay.nScrOrgY;
//******************************************************
	rect=CRect(CPoint(nScrOrgX,nScrOrgY-20*ky),
		CSize(nScrWidth/6.,60*ky));
	mAdcGain=1;
	if(mAdcGain==1)
		str="采样率:250M";
	else if(mAdcGain==2)
		str="采样率:500M";
	MemDC.DrawText(str,&rect,DT_SINGLELINE);
//**********************************************
	rect=CRect(CPoint(nScrWidth/6.+nScrOrgX,nScrOrgY-20*ky),
		CSize(nScrWidth/6.,60*ky));
	str.Format("高压：%d",nPresetTime[0]);
	MemDC.DrawText(str,&rect,DT_SINGLELINE);
//**********************************************
	rect=CRect(CPoint(nScrWidth/3.+nScrOrgX,nScrOrgY-20*ky),
		CSize(nScrWidth/6.,60*ky));
/*
	if(bAdcWorkOn[0])
		str="阻抗：低阻";
	else
		str="阻抗：高阻";
	MemDC.DrawText(str,&rect,DT_SINGLELINE);
*/
//***************************
	rect=CRect(CPoint(nScrWidth/3.+nScrOrgX,nScrOrgY-20*ky),
		CSize(nScrWidth/6.,60*ky));
	str.Format("阈值：%d",fLD);
	MemDC.DrawText(str,&rect,DT_SINGLELINE);
//*******************************************************
	rect=CRect(CPoint(nScrWidth*2/3.+nScrOrgX,nScrOrgY-20*ky),
		CSize(nScrWidth/6.,60*ky));
	if(bStopWave)
		str.Format("触发状态:停止");
	if(bWaitTriger)
		str.Format("触发状态:等待");
	else
		str.Format("触发状态:触发");
	MemDC.DrawText(str,&rect,DT_SINGLELINE);
//************************************************************
	rect=CRect(CPoint(nScrWidth-160,nScrHeight-80*ky),
		CSize(nScrWidth/7.2,60*ky));
	MemDC.SelectObject(&brush);
	MemDC.Rectangle(&rect);
	rect=CRect(CPoint(nScrWidth-140,nScrHeight-80*ky),
		CSize(nScrWidth/7.2,30*ky));
	float xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	xrange=dispspm.spara.Horz/1000000.*xrange;

	str.Format("X：%5.1f us /每格",xrange/8.);
	MemDC.DrawText(str,&rect,DT_SINGLELINE|DT_VCENTER);
	rect=CRect(CPoint(nScrWidth-140,nScrHeight-50*ky),
		CSize(nScrWidth/6.,30*ky));
	if(Vort==16)
		str.Format("Y：%5.1f V/每格",(pow(2.,Vort)*TH_k+TH_b)/5000.);
	else if(Vort>12)
		str.Format("Y：%5.0f mV/每格",(pow(2.,Vort)*TH_k+TH_b)/5.);
	else if(Vort>11)
		str.Format("Y：%5.1f mV/每格",(pow(2.,Vort)*TH_k+TH_b)/5.);
	else
		str.Format("Y：%5.3f mV/每格",(pow(2.,Vort)*TH_k+TH_b)/5.);


	MemDC.DrawText(str,&rect,DT_SINGLELINE|DT_VCENTER);

//************************************************************
	pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();


	if(boolRoiRect)
	{
	CBrush *oldBrush;
	oldBrush=(CBrush *)pDC->SelectStockObject(HOLLOW_BRUSH);
	oldPen = pDC->SelectObject(&whitePen);
//	boolRoiRect = TRUE;
	pDC->Rectangle(&rectMouseMove);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMcbcView printing

BOOL CMcbcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMcbcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMcbcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMcbcView diagnostics

#ifdef _DEBUG
void CMcbcView::AssertValid() const
{
	CView::AssertValid();
}

void CMcbcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMcbcDoc* CMcbcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMcbcDoc)));
	return (CMcbcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMcbcView message handlers

void CMcbcView::OnInitialUpdate() 
{
	CString str;
	CView::OnInitialUpdate();
	m_pDoc = GetDocument();
	w1[viewNum] = (CMcbcView *)this;

	CRect rect;
	GetClientRect(&rect);
	int tmx1=rect.right-rect.left;
	dispspm.spara.SpmW=tmx1-6;
	dispspm.spara.SpmH=rect.bottom-48-rect.top;

	dispspm.spara.Horz=Gain[0];
	dispspm.spara.Vort=Vort;
	dispspm.spara.StartChn=0;
	dispspm.spara.IsLog=0;
	dispspm.CursorChn=50;
	int	xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	str.Format("%d",int(xrange*dispspm.spara.Horz/(float)Gain[0]));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_X_RANGE)->SetWindowText(str);

	if(dispspm.spara.IsLog==1)
		str.Format("LOG");
	else
		str.Format("%d",int(pow(2.,dispspm.spara.Vort)*TH_k+TH_b));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_Y_RANGE)->SetWindowText(str);


}

BOOL CMcbcView::OnEraseBkgnd(CDC* pDC) 
{
//	::SetClassLong(this->m_hWnd,GCL_HBRBACKGROUND,(long)(HBRUSH)(3l));
//	::SetClassLong(this->m_hWnd,GCL_HBRBACKGROUND,(long)(HBRUSH)(bkBrush.operator HBRUSH()));
	return CView::OnEraseBkgnd(pDC);
}

void CMcbcView::Paint(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);
	int tmx1=rect.right-6-rect.left;
	mcsbz =0;
	dispspm.kx=kx;
	dispspm.ky=ky;
//	w1[viewNum] = (CMcbcView *)this;
	tmx1 = xRange+nStartX;
	if(tmx1>0)
	{
		dispspm.spara.SpmW = xRange;
		int tmx2 = rect.bottom-6-rect.top-nStartY;
		tmx2 = int(rect.Height()-50*ky-nStartY);
		tmx2 = yRange+nStartY+10*ky;
		if(tmx2>0)
		{
			dispspm.spara.SpmH=yRange;
			CPoint p0(2+nStartX,nStartY+10*ky),p1(tmx1-2,nStartY+10*ky),p2(tmx1-2,tmx2),p3(2+nStartX,tmx2);
			if(pDC->IsPrinting())
				oldPen = (CPen *)pDC->SelectStockObject(BLACK_PEN);
			else
				oldPen = (CPen *)pDC->SelectObject(&whitePen);
			pDC->MoveTo(p0);
			pDC->LineTo(p1);
			pDC->LineTo(p2);
			pDC->LineTo(p3);
			pDC->LineTo(p0);
			if(pDC->IsPrinting())
				oldPen = (CPen *)pDC->SelectStockObject(BLACK_PEN);
			else
				oldPen = pDC->SelectObject(&whitePen);
			pDC->SelectObject(&wavePen[0]);
			dispspm.DispCursor(pDC,0,pDispAdcBuf[0]);
			if(bWaveDisp[0])
			{
				dispspm.ReadyData(pDispAdcBuf[0]);
				dispspm.DispData(pDC,1,0,wavePen[1],wavePen[0]);
			}
			if(bWaveDisp[1])
			{
				dispspm.ReadyData(pDispAdcBuf[1]);
				dispspm.DispData(pDC,1,1,wavePen[2],wavePen[0]);
			}
			if(bWaveDisp[2])
			{
				dispspm.ReadyData(pDispAdcBuf[2]);
				dispspm.DispData(pDC,1,2,wavePen[3],wavePen[0]);
			}
			if(bWaveDisp[3])
			{
				dispspm.ReadyData(pDispAdcBuf[3]);
				dispspm.DispData(pDC,1,3,wavePen[4],wavePen[0]);
			}
			if(bWaveDisp[4])
			{
				dispspm.ReadyData(pDispAdcBuf[4]);
				dispspm.DispData(pDC,1,4,wavePen[5],wavePen[0]);
			}
			if(bWaveDisp[5])
			{
				dispspm.ReadyData(pDispAdcBuf[5]);
				dispspm.DispData(pDC,1,5,wavePen[6],wavePen[0]);
			}

			dispspm.DispCursor(pDC,1,pDispAdcBuf[0]);
			pDC->SetBkColor(RGB(192,192,192));
			dispspm.DispLabel(pDC);//显示y坐标标签
			dispspm.DispLabelX(pDC);//显示x坐标标签
			pDC->SelectObject(oldPen);
		}
		if(!pDC->IsPrinting())
			dispspm.paintbz=0;
	}

	if(pDlgSetRoi->IsWindowVisible()&&(bSetRoiDlg))
	{
		kh=(float)dispspm.spara.Horz/(float)dispspm.spara.SpmW;
		int leftx;
		leftx=int((pDlgSetRoi->m_RoiLeftCh-dispspm.spara.StartChn)/kh);
		pDC->MoveTo(leftx+3,0);
		pDC->LineTo(leftx+3,dispspm.spara.SpmH);
		leftx=int((pDlgSetRoi->m_RoiRightCh-dispspm.spara.StartChn)/kh);
		pDC->MoveTo(leftx+3,0);
		pDC->LineTo(leftx+3,dispspm.spara.SpmH);
		pDlgSetRoi->m_View=viewNum;

	}

}
int CMcbcView::DispGross(long *pdata)
{
	int i,tm1,tm2,tm4;
	CString str;
	int isroi=0;
	tm1=0;
	tm2=0;
	tm4=dispspm.CursorChn;
	if(RoiNum<1)
		return 0;
	tm1=RoiStart[0];
	tm2=RoiEnd[0];
	if(tm1>=tm2)
		return 0;
	{
		long bd1,bd2,maxCount=0,maxChannel=0;
		int minCount=0,minChannel=0;
		double sum=0,sumi=0;
		crossArea=0;
		bd1=0;
		int leftch1,rightch1;
		int leftch2,rightch2;
		int len=tm2-tm1+1;
		maxChannel=tm1+len/2.;
		float baseline=0;
		for(i=10;i<110;i++)
			baseline+=*(pdata+i);
		baseline=baseline/100.;
		maxCount=0;
		minCount=65536;
		float upTime;
		float downTime;
		for(i=0;i<len;i++)
		{
			if(maxCount<*(pdata+tm1+i))
			{
				maxCount=*(pdata+tm1+i);
				maxChannel=tm1+i;
			}
			if(minCount>*(pdata+tm1+i))
			{
				minCount=*(pdata+tm1+i);
				minChannel=tm1+i;
			}
		}
		if(maxCount-baseline>baseline-minCount)//正脉冲
		{
			leftch1=tm1;
			for(i=tm1;i<tm2;i++)
			{
				if(*(pdata+i)>baseline+(maxCount-baseline)*0.1)
				{
					leftch1=i;
					break;
				}
			}
			rightch1=tm2;
			for(i=tm1;i<tm2;i++)
			{
				if(*(pdata+i)>baseline+(maxCount-baseline)*0.9)
				{
					rightch1=i;
					break;
				}
			}
			upTime=(rightch1-leftch1);

			leftch2=tm2,rightch2=tm2;
			for(i=maxChannel;i<tm2;i++)
			{
				if(*(pdata+i)<baseline+(maxCount-baseline)*0.9)
				{
					leftch2=i;
					break;
				}
			}
			for(i=maxChannel;i<tm2;i++)
			{
				if(*(pdata+i)<baseline+(maxCount-baseline)*0.1)
				{
					rightch2=i;
					break;
				}
			}
			downTime=rightch2-leftch2;
		}
		else//负脉冲
		{
			leftch1=tm1;
			for(i=tm1;i<tm2;i++)
			{
				if(*(pdata+i)<baseline-(baseline-minCount)*0.1)
				{
					leftch1=i;
					break;
				}
			}
			rightch1=tm2;
			for(i=tm1;i<tm2;i++)
			{
				if(*(pdata+i)<baseline-(baseline-minCount)*0.9)
				{
					rightch1=i;
					break;
				}
			}
			upTime=(rightch1-leftch1);

			leftch2=tm2,rightch2=tm2;
			for(i=minChannel;i<tm2;i++)
			{
				if(*(pdata+i)>baseline-(baseline-minCount)*0.9)
				{
					leftch2=i;
					break;
				}
			}
			for(i=minChannel;i<tm2;i++)
			{
				if(*(pdata+i)>baseline-(baseline-minCount)*0.1)
				{
					rightch2=i;
					break;
				}
			}
			downTime=rightch2-leftch2;		}
		{

			str.Format("%5.0f",downTime);
			m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_NET)->SetWindowText(str);
		}
//		str.Format("%6.1f",FwhmCh);
		maxChannel=(rightch1+leftch2)/2.;
		if(maxChannel>0)
			str.Format("%6.1f",(float)maxChannel);
		else
			str.Format("  ");
		m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_CENTER)->SetWindowText(str);
	
		str.Format("%5.0f",upTime);
		m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_GROSS)->SetWindowText(str);
	}
	return isroi;
 }

void CMcbcView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	int i;
	dispspm.paintbz=1;
	m_pDoc->nCurView = viewNum;
}


void CMcbcView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDown(nFlags, point);

	CClientDC dc(this);
	int x,tmx1,tmx2,tmx4,i;
	BOOL oldbz;

	dispspm.DispCursor(&dc,0,pDispAdcBuf[viewNum]);

	x=point.x;
	buttondown=1;
	boolRoiRect = FALSE;
	rectDC = new CClientDC(this);
	tmx1=dispspm.spara.Horz;
	tmx2=dispspm.spara.SpmW;
	kh=(float)tmx1/(float)tmx2;
	tmx3=dispspm.spara.StartChn;
	if(x<nStartX)return;
	x=x-nStartX;
	if(x>tmx2+nStartX)
		x=tmx2+nStartX;
	tmx4 = int(x*kh+tmx3);
	if(tmx4 >= Gain[viewNum]-1)
		tmx4=Gain[viewNum]-1;
	if(tmx4<0)
		tmx4 = 0;
	w1[viewNum]->dispspm.CursorChn = tmx4;
	if(tmx3 > Gain[viewNum]-tmx1-1)
		dispspm.spara.StartChn=Gain[viewNum]-tmx1;
	oldbz=0;

	CBrush *oldBrush;
	oldBrush=(CBrush *)rectDC->SelectStockObject(HOLLOW_BRUSH);
	oldPen = rectDC->SelectObject(&whitePen);
	if(rectDC&&!bSetRoi)
	{
		rectDC->SetROP2(R2_XORPEN);
		rectDC->Rectangle(&rectMouseMove);
	}
	bSetRoi = FALSE;
	rectMouseMove.top = point.y;
	rectMouseMove.left = point.x;
	rectMouseMove.bottom = point.y;
	rectMouseMove.right = point.x;
	CString	str;

	long curdata;
	double energy;
	char temp[80];
	int RoiNum1=DispGross(pDispAdcBuf[viewNum]);
	if(RoiNum1>0)
		str.Format("%d*",dispspm.CursorChn);
	else
		str.Format("%d",dispspm.CursorChn);
	m_pDlgBarState[0]->GetDlgItem(IDC_STATIC_CURSOR)->SetWindowText(str);
	curdata=(*(pDispAdcBuf[0]+dispspm.CursorChn))&0x0ffffffl;
	str.Format("%ld",curdata);
	m_pDlgBarState[0]->GetDlgItem(IDC_ADC1)->SetWindowText(str);
	sprintf(temp,"%d",*(pDispAdcBuf[1]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC2,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[2]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC3,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[3]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC4,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[4]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC5,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[5]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC6,temp);
	if(energyKedu[0].Enum>=2)
	{
		energy=energyKedu[0].para[0]+energyKedu[0].para[1]*curdata;
		str.Format("%8.0f",energy);
		m_pDlgBarState[0]->GetDlgItem(IDC_ADC1_V)->SetWindowText(str);
	}
	int xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	energy=xrange/1000.*dispspm.CursorChn-xrange*1000/2.;
	str.Format("%8.0f",energy);
	m_pDlgBarState[0]->GetDlgItem(IDC_STATIC_ENERGY)->SetWindowText(str);
	dispspm.paintbz=1;
	Invalidate(0);
	dispspm.DispCursor(&dc,1,pDispAdcBuf[viewNum]);
}

void CMcbcView::OnButtonExpand() 
{
	CString	str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.Horz/=2;
	if(dispspm.spara.Horz<16)
		dispspm.spara.Horz=16;
	int curchn=dispspm.spara.Horz;
	curchn/=2;
	dispspm.spara.StartChn=(dispspm.CursorChn-curchn);
	if(dispspm.spara.StartChn>=Gain[viewNum]-dispspm.spara.Horz)
		dispspm.spara.StartChn=Gain[viewNum]-dispspm.spara.Horz;
	if(dispspm.spara.StartChn<0)
		dispspm.spara.StartChn=0;

	int	xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	str.Format("%d",int(xrange*dispspm.spara.Horz/(float)Gain[0]));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_X_RANGE)->SetWindowText(str);
/*
	char temp[20];
	sprintf(temp,"%d",dispspm.spara.Horz);
	w1[wid]->pdHorz->SetText(temp);*/
	dispspm.paintbz=1;
	Invalidate(0);
	SetFocus();
}

void CMcbcView::OnButtonSmall() 
{
	CString	str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.Horz*=2;
	if(dispspm.spara.Horz>Gain[viewNum])
		 dispspm.spara.Horz=Gain[viewNum];
	int fullchn=dispspm.spara.Horz;
	fullchn/=2;
	dispspm.spara.StartChn=(dispspm.CursorChn-fullchn);
	if(dispspm.spara.StartChn>=Gain[viewNum]-dispspm.spara.Horz)
		dispspm.spara.StartChn=Gain[viewNum]-dispspm.spara.Horz;
	if(dispspm.spara.StartChn<0)
		dispspm.spara.StartChn=0;
	int	xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	str.Format("%d",int(xrange*dispspm.spara.Horz/(float)Gain[0]));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_X_RANGE)->SetWindowText(str);
	dispspm.paintbz=1;
	Invalidate(0);
	SetFocus();
}

void CMcbcView::OnBtnAddy() 
{
	CString	str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.IsLog=0;
	dispspm.spara.Vort--;
	if(dispspm.spara.Vort<10)
		dispspm.spara.Vort=16;
	Vort=dispspm.spara.Vort;
	str.Format("%d",int(pow(2.,dispspm.spara.Vort)*TH_k+TH_b));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_Y_RANGE)->SetWindowText(str);
	dispspm.paintbz=1;
	Invalidate(0);
	SetFocus();
}

void CMcbcView::OnBtnSuby() 
{
	CString	str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.IsLog=0;
	dispspm.spara.Vort++;
	if(dispspm.spara.Vort>16)
		dispspm.spara.Vort=10;
	Vort=dispspm.spara.Vort;
	str.Format("%d",int(pow(2.,dispspm.spara.Vort)*TH_k+TH_b));
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_Y_RANGE)->SetWindowText(str);
	dispspm.paintbz=1;
	Invalidate(0);
	SetFocus();
}

void CMcbcView::OnBtnLogy() 
{
	CString str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.IsLog=1;
	dispspm.paintbz=1;
	str.Format("LOG");
	m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_Y_RANGE)->SetWindowText(str);
	Invalidate(0);
	SetFocus();
}

void CMcbcView::ClearDisp(CDC *pDC)
{
	dispspm.ClearData(pDC);
}

void CMcbcView::ClearCursor()
{
	CClientDC dc(this);
	dispspm.DispCursor(&dc,0,pDispAdcBuf[viewNum]);
}

void CMcbcView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	CClientDC dc(this);
	int i;
	switch(nChar)
	{
	case VK_RIGHT:
		ClearCursor();
		dispspm.CursorChn++;
		SetupLevel();
		if(dispspm.CursorChn>=Gain[viewNum]-1)
			dispspm.CursorChn=Gain[viewNum]-1;
		if(dispspm.CursorChn > dispspm.spara.Horz + dispspm.spara.StartChn)
			dispspm.spara.StartChn = dispspm.CursorChn - dispspm.spara.Horz;
		for(i=0;i<1;i++)
		{
			w1[i]->dispspm.CursorChn=dispspm.CursorChn;
			w1[i]->dispspm.spara.StartChn=dispspm.spara.StartChn;
			w1[i]->dispspm.paintbz=1;
			w1[i]->DispCursor();
		}
		Invalidate(0);
		break;
	case VK_LEFT:
		ClearCursor();
		dispspm.CursorChn--;
		SetupLevel();
		if(dispspm.CursorChn<0)
			dispspm.CursorChn=0;
		if(dispspm.CursorChn<dispspm.spara.StartChn)
			dispspm.spara.StartChn=dispspm.CursorChn;
		for(i=0;i<1;i++)
		{
			w1[i]->dispspm.CursorChn=dispspm.CursorChn;
			w1[i]->dispspm.spara.StartChn=dispspm.spara.StartChn;
			w1[i]->dispspm.paintbz=1;
			w1[i]->DispCursor();
		}
		Invalidate(0);
		break;
	case VK_PRIOR:
		ClearCursor();
		dispspm.CursorChn += dispspm.spara.Horz/8;
		SetupLevel();
		if(dispspm.CursorChn >= Gain[viewNum]-1)
			dispspm.CursorChn = Gain[viewNum]-1;
		if(dispspm.CursorChn > dispspm.spara.Horz + dispspm.spara.StartChn)
			dispspm.spara.StartChn = dispspm.CursorChn - dispspm.spara.Horz;
		DispCursor();
		Invalidate(0);
 		break;
	case VK_NEXT:
		ClearCursor();
		dispspm.CursorChn-=dispspm.spara.Horz/8;
		SetupLevel();
		if(dispspm.CursorChn<0)
			dispspm.CursorChn=0;
		if(dispspm.CursorChn<0)
			dispspm.CursorChn=0;
		if(dispspm.CursorChn<dispspm.spara.StartChn)
			dispspm.spara.StartChn=dispspm.CursorChn;
		DispCursor();
		Invalidate(0);
		break;
	case VK_UP:
		OnBtnAddy();
		if(Vort==16)
			yBaseLine=0;
		else if(Vort==15)
			yBaseLine=5;
		else if(Vort==14)
			yBaseLine=7;
		else
			yBaseLine=9;
		Invalidate(0);
		break;
	case VK_DOWN:
		OnBtnSuby();
		if(Vort==16)
			yBaseLine=0;
		else if(Vort==15)
			yBaseLine=5;
		else if(Vort==14)
			yBaseLine=7;
		else
			yBaseLine=9;
		Invalidate(0);
		break;
	case VK_ADD:
		OnButtonExpand();
		break;
	case VK_SUBTRACT:
		OnButtonSmall();
		break;
	case VK_HOME:
		ClearDisp(&dc);
		ClearCursor();
		dispspm.CursorChn=0;
		SetupLevel();
		dispspm.spara.StartChn=0;
		dispspm.paintbz=1;
		DispCursor();
		Invalidate(0);
		break;
	case VK_END:
		ClearDisp(&dc);
		ClearCursor();
		dispspm.CursorChn=Gain[viewNum]-1;
		SetupLevel();
		dispspm.spara.StartChn=Gain[viewNum]-dispspm.spara.Horz;
		if(dispspm.spara.StartChn<0)
			dispspm.spara.StartChn=0;
		dispspm.paintbz=1;
		DispCursor();
		Invalidate(0);
		break;
	case 0x41:
	case 'a':
		yBaseLine++;
		if(yBaseLine>19)
			yBaseLine=19;
		break;
	case 'Z':
	case 'z':
		yBaseLine--;
		if(yBaseLine<0)
			yBaseLine=0;
		break;
	case 76:
		OnBtnLogy();
		break;
	case VK_TAB:
/*
		if(((CButton *)(m_pDlgBar->GetDlgItem(IDC_RADIO_ONE)))->GetCheck())
		{
			((CMainFrame *)::AfxGetMainWnd())->MDIActivate(m_pChildWnd[1]);
						
		}
		else
		{
			((CMainFrame *)::AfxGetMainWnd())->MDIActivate(m_pChildWnd[0]);
		}
*/
		break;
	case VK_F1:
		OnBtnAuto();
	break;
	case VK_F2:
		break;
	case VK_F3:
		break;
	default:
		break;
	}
}

void CMcbcView::DispCursor()
{
	CString	str;
	long curdata;
	double energy;
	char temp[80];
	int RoiNum1=DispGross(pDispAdcBuf[viewNum]);
	if(RoiNum1>0)
		str.Format("%d*",dispspm.CursorChn);
	else
		str.Format("%d",dispspm.CursorChn);
	m_pDlgBarState[0]->GetDlgItem(IDC_STATIC_CURSOR)->SetWindowText(str);
	curdata=(*(pDispAdcBuf[0]+dispspm.CursorChn))&0x0ffffffl;
	str.Format("%ld",curdata);
	m_pDlgBarState[0]->GetDlgItem(IDC_ADC1)->SetWindowText(str);
	sprintf(temp,"%d",*(pDispAdcBuf[1]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC2,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[2]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC3,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[3]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC4,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[4]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC5,temp);
	sprintf(temp,"%d",*(pDispAdcBuf[5]+dispspm.CursorChn));
	m_pDlgBarState[0]->SetDlgItemTextA(IDC_ADC6,temp);
	//	if(energyKedu[viewNum].Enum>=2)
	{
		double energy;
		int xrange;
		if(mAdcGain==1)
			xrange=4000;
		else if(mAdcGain==2)
			xrange=160;
		else
			xrange=500;
		energy=xrange/1000.*dispspm.CursorChn-xrange*1000/2.;
		str.Format("%8.0f",energy);
		m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_ENERGY)->SetWindowText(str);
	}
//	Invalidate(0);

}

void CMcbcView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	buttondown=0;
	if(rectDC)
	{
		delete rectDC;
		rectDC = 0;
		bSetRoi = FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMcbcView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CView::OnMouseMove(nFlags, point);
	if(buttondown==1)
	{
//		CBrush *oldBrush;
//		oldBrush=(CBrush *)rectDC->SelectStockObject(HOLLOW_BRUSH);
//		oldPen = rectDC->SelectObject(&whitePen);
//		rectDC->SetROP2(R2_XORPEN);
//		rectDC->Rectangle(&rectMouseMove);
		rectMouseMove.right = point.x;
		rectMouseMove.bottom = point.y;
		boolRoiRect = TRUE;
//		if(rectDC)
//			rectDC->Rectangle(&rectMouseMove);
//		rectDC->SelectObject(oldPen);
//		rectDC->SelectObject(oldBrush);
	}
}

void CMcbcView::OnContextMenu(CWnd* pWnd, CPoint point) 
{

	CMenu menu;
	menu.LoadMenu(IDR_MCBCTYPE);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		point.x, point.y,this);

}

void CMcbcView::OnMenuSetroi() 
{
	int	i,minRoi,maxRoi;
	bool	oldbz=0;
	int		chn1,chn2,ochn1,ochn2;
	if(boolRoiRect)
	{
		boolRoiRect = FALSE;
		bSetRoi = TRUE;
		chn1 = int(kh*rectMouseMove.left)+tmx3-nStartX*kh;
		chn2 = int(kh*rectMouseMove.right)+tmx3-nStartX*kh;
		if(chn1>chn2)
		{
			int temp=chn2;
			chn2=chn1;
			chn1=temp;
		}
		RoiCur = 0;
		for(i=0;i<RoiNum;i++)
		{
			if(chn1 >= RoiEnd[i])
			{
				RoiCur=i+1;
			}

		}
		maxRoi = RoiCur;
		minRoi = RoiCur;
		oldbz= 0;

		for(i=RoiCur;i<RoiNum;i++)
		{
			ochn1 = RoiStart[i];
			ochn2 = RoiEnd[i];
			if((ochn1>=chn1)&&(ochn1<=chn2))
			{
				oldbz=1;
				if(maxRoi<i)maxRoi=i;
			}
			if((ochn2>=chn1)&&(ochn2<=chn2))
			{
				oldbz=1;
				if(maxRoi<i)maxRoi=i;
			}
		}
		int subNum;
		if(oldbz==0)
		{
			RoiNum++;
			if(RoiNum>49) RoiNum=49;
			for(i=RoiNum; i>RoiCur; i--)
			{
				RoiStart[i] = RoiStart[i-1];
				RoiEnd[i] = RoiEnd[i-1];
			}
		}
		else
		{
			if(chn1>RoiStart[minRoi])
				chn1 = RoiStart[minRoi];
			if(chn2<RoiEnd[maxRoi])
				chn2 = RoiEnd[maxRoi];
			subNum = maxRoi-minRoi;
			RoiNum = RoiNum - subNum;
			for(i=minRoi+1;i<RoiNum;i++)
			{
				RoiStart[i] = RoiStart[i+subNum];
				RoiEnd[i] = RoiEnd[i+subNum];
			}
		}
		RoiStart[RoiCur]=chn1;
		RoiEnd[RoiCur]=chn2;
		dispspm.paintbz=1;
	}
		Invalidate(0);
}

void CMcbcView::OnMenuClearroi() 
{
	if(RoiNum>=1)
	{
		int tmi,i,chn1;
		bool	oldbz=0;
		for(i=0;i<RoiNum;i++)
		{
			chn1 = int(kh*rectMouseMove.left)+tmx3;
			if((chn1>=RoiStart[i])&&(chn1<=RoiEnd[i]))
			{
				tmi=i;
				oldbz=1;
				break;
			}
		}
		if(oldbz==1)
		{
			RoiNum--;
			for(i=tmi;i<RoiNum;i++)
			{
				RoiStart[i]=RoiStart[i+1];
				RoiEnd[i]=RoiEnd[i+1];
			}
			dispspm.paintbz=1;
			Invalidate(0);
		}
	}
}

void CMcbcView::OnMenuClear() 
{
//	for(int i=0;i<MaxChannel;i++)
//		pAdcBuf[viewNum][i] = 0;
	dispspm.paintbz=1;
	ClearCursor();
	Invalidate(0);
}

void CMcbcView::OnMenuClearAll() 
{
	RoiNum = 0;
	memset(RoiStart,sizeof(RoiStart),0);
	memset(RoiEnd,sizeof(RoiStart),0);
	dispspm.paintbz=1;
	ClearCursor();
	Invalidate(0);
}

void CMcbcView::OnDestroy() 
{
	CView::OnDestroy();
}

void CMcbcView::OnButtonStart() 
{
	int i;
	bStart = TRUE;
	curTime = CTime::GetCurrentTime();
	CString sdate;
	sdate.Format("%02d.%02d.%02d",curTime.GetYear()-2000,
		curTime.GetMonth(),curTime.GetDay());

	CString stime;
	stime= curTime.Format("%X");
	m_pDlgBar->SetDlgItemText(IDC_STATIC_DATE2,sdate.GetBuffer(20));
	viewNum=0;
	w1[viewNum]->curTime=curTime;
	hardSatrtTime[viewNum]=curTime;
	m_pDlgBar->SetDlgItemText(IDC_STATIC_TIME2,stime.GetBuffer(20));
	transbz[viewNum] = TRUE;
	m_pDoc->SetTitle("");
	if(viewNum==0)
	{
		nOldRealTime=-1;
		nOldRealTime2=-2;
	}
	else
	{
		nOldRealTime3=-1;
		nOldRealTime4=-2;
	}


	ULONG input[6];
	input[0]=0x61 + viewNum;	// Input to device
	input[1]=nPresetTime[0];	// Input to device
	input[2]=nPresetTime[1];	// Input to device
	input[3]=nPresetTime[2];	// Input to device
	input[4]=nPresetTime[3];	// Input to device
	input[5]=bPresetReal[0];
	DWORD written;

	bFirstStart=1;
	bCoolTimeStart=0;

	SetFocus();
}

void CMcbcView::OnButtonStop() 
{
//		bStart = FALSE;
//		*(pAdcBuf[viewNum]+6)=0;
		m_pDoc->SetTitle("");
		ULONG input=0x61 + viewNum;	// Input to device
		if(bTongbu)
			input=0x65;
		else
			input=0x61 + viewNum;	// Input to device
		ULONG output=100;	    // Output from device
		ULONG noutput=0;    // Count written to bufOutput
	bCoolTimeStart=0;
	DWORD written;
	bFirstStop=1;
	*(pAdcBuf[viewNum]+6)=0x1;
	SetFocus();
}

void CMcbcView::OnUpdateButtonStart(CCmdUI* pCmdUI) 
{
//	int meas=*(pAdcBuf[0]+6);
//	bool bmeas;
//	if(meas==1)
//		bmeas=1;
//	else
//		bmeas=0;
	pCmdUI->Enable(!bStart);
 
}

void CMcbcView::OnUpdateButtonStop(CCmdUI* pCmdUI) 
{
//	int meas=*(pAdcBuf[0]+6);
//	bool bmeas;
//	if(meas==1)
//		bmeas=1;
//	else
//		bmeas=0;
	pCmdUI->Enable(bStart&&transbz[viewNum]);
}

void CMcbcView::OnButtonClear() 
{
	int i;
	ULONG input;

	input=0x61 + viewNum;	// Input to device
	if(bDispBuffer)
	{
		memset(pMemBuf[viewNum],0,8192*4);
	}
	else
	{
		memset(pAdcBuf[viewNum],0,8192*4);
	}
	memset(pAdcBuf[4],0,8192*4);
	ULONG output=100;	    // Output from device
	ULONG noutput=0;    // Count written to bufOutput
	w1[viewNum]->GetDocument()->fileName="";
	m_pDoc->SetTitle("");
	DWORD written;

    m_TxData[0]=0x12;
    m_TxData[1]=0x34;
    m_TxData[2]=0x0f;
    m_TxData[3]=0x0;
    m_TxData[4]=0x40;
    m_TxData[5]=0x20;
    m_TxData[6]=0x0;
    m_TxData[7]=0x0;
    m_TxData[8]=0x0;
    m_TxData[9]=0xab;
    m_TxData[10]=0xcd;
	toBeWritten=11;
	WriteFile(hCom,m_TxData,toBeWritten,&lpnumber,NULL);
	if(bStart)
		bFirstStart=1;

//	bTrans=1;
//	SI_Write(m_hUSBDevice, (LPVOID)m_TxData, toBeWritten, &written);
	SetFocus();
}


void CMcbcView::mChangColor()
{
	bkBrush.DeleteObject();
	bkBrush.CreateSolidBrush(mbkcolor);
	wavePen[0].DeleteObject();
	wavePen[1].DeleteObject();
	wavePen[0].CreatePen(PS_SOLID,1,mforecolor);
	wavePen[1].CreatePen(PS_SOLID,1,mredcolor);
	Invalidate(0);
}
void CMcbcView::OnBtnAuto()
{
	CString	str;
	CClientDC dc(this);
	ClearDisp(&dc);
	ClearCursor();
	dispspm.spara.IsLog=0;
	int maxData=0;
	for(int i=2;i<Gain[viewNum];i++)
	{
		if(*(pAdcBuf[viewNum]+i)>maxData)
		{
			maxData=*(pAdcBuf[viewNum]+i);
		}
	}
	if(maxData>0)
	{
		dispspm.spara.Vort=log(double(maxData))/log(2.)+1;
		Vort=dispspm.spara.Vort;
		str.Format("%d",int(pow(2.,dispspm.spara.Vort)*TH_k+TH_b));
		m_pDlgBarState[viewNum]->GetDlgItem(IDC_STATIC_Y_RANGE)->SetWindowText(str);
		dispspm.paintbz=1;
		Invalidate(0);
		SetFocus();
	}
}

void CMcbcView::OnUpdateButtonClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(transbz[viewNum]);
}
void  CMcbcView::SetupLevel() 
{
	if(pDlgSetRoi->IsWindowVisible())
	{
		pDlgSetRoi->UpdateData(TRUE);
		if(pDlgSetRoi->m_Left==0)
			pDlgSetRoi->m_RoiLeftCh=dispspm.CursorChn;
		else
			pDlgSetRoi->m_RoiRightCh=dispspm.CursorChn;
		pDlgSetRoi->UpdateData(FALSE);
	}
}

void CMcbcView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
	int x,tmx1,tmx2,tmx4;

//	dispspm.DispCursor(&dc,0,pDispAdcBuf[viewNum]);

	x=point.x;
	tmx1=dispspm.spara.Horz;
	tmx2=dispspm.spara.SpmW;
	kh=(float)tmx1/(float)tmx2;
	tmx3=dispspm.spara.StartChn;
	if(x<nStartX)return;
	x=x-nStartX;
	if(x>tmx2+nStartX)
		x=tmx2+nStartX;
	tmx4 = int(x*kh+tmx3);
	if(tmx4 >= Gain[viewNum]-1)
		tmx4=Gain[viewNum]-1;
	if(tmx4<0)
		tmx4 = 0;
	dispspm.CursorChn = tmx4;
	if(tmx3 > Gain[viewNum]-tmx1-1)
		dispspm.spara.StartChn=Gain[viewNum]-tmx1;

	CRect rect,rect1,rect2,rect3;
	pDlgRoIDisplay->GetWindowRect(&rect1);
	GetWindowRect(&rect2);
	GetParentFrame()->GetParentFrame()->GetWindowRect(&rect3);
	rect.left=rect2.left+point.x;
	rect.top=rect2.top+point.y;
	if(rect.left+rect1.Width()>rect3.right)
		rect.left=rect3.right-rect1.Width();
	if(rect.top+rect1.Height()>rect3.bottom)
		rect.top=rect3.bottom-rect1.Height();
	rect.right=rect.left+rect1.Width();
	rect.bottom=rect.top+rect1.Height();
	
	pDlgRoIDisplay->MoveWindow(&rect);
	int RoiNum1=DispGross(pDispAdcBuf[viewNum]);
	if(RoiNum1>0)
	{
		int tm1=RoiStart[RoiNum-1];
		int tm2=RoiEnd[RoiNum-1];
		pDlgRoIDisplay->m_RoiRangeCh.Format("%d---%d",tm1,tm2);
		if(energyKedu[viewNum].Enum>=2)
		{
			float en1=energyKedu[viewNum].para[0]+energyKedu[viewNum].para[1]*tm1;
			float en2=energyKedu[viewNum].para[0]+energyKedu[viewNum].para[1]*tm2;
			pDlgRoIDisplay->m_RoiRangeEn.Format("%7.2f---%7.2f",en1,en2);
			centerEn=energyKedu[viewNum].para[0]+energyKedu[viewNum].para[1]*centerCh;
			pDlgRoIDisplay->m_CenterEn=centerEn;
			FwhmEn=energyKedu[viewNum].para[1]*FwhmCh;
			pDlgRoIDisplay->m_FwmhEn=FwhmEn;
     		pDlgRoIDisplay->m_FWHM=int(FwhmEn/centerEn*100*1000)/1000.;

		}
		else
		{
			pDlgRoIDisplay->m_RoiRangeEn.Format("");
			pDlgRoIDisplay->m_CenterEn=-1;
			FwhmEn=-1;
			pDlgRoIDisplay->m_FwmhEn=-1;
		}
		pDlgRoIDisplay->m_CenterCh=centerCh;
		pDlgRoIDisplay->m_FwmhCh=FwhmCh;


		pDlgRoIDisplay->m_Gross=crossArea;
		pDlgRoIDisplay->m_NetArea=netArea;
		pDlgRoIDisplay->UpdateData(FALSE);
		pDlgRoIDisplay->ShowWindow(SW_SHOW);
	}
/*	else
	{
		pDlgRoIDisplay->ShowWindow(SW_HIDE);
		pDlgSetRoi->GetWindowRect(&rect1);
		if((viewNum==0)||(viewNum==2))
			rect.left=rect2.right;
		else
			rect.left=0;
		rect.right=rect.left+rect1.Width();
		rect.bottom=rect.top+rect1.Height();
		pDlgSetRoi->MoveWindow(&rect);
		pDlgSetRoi->ShowWindow(SW_SHOW);
		bSetRoiDlg=1;

	}
*/
	CView::OnLButtonDblClk(nFlags, point);
}
void CMcbcView::TimerTick() 
{
	// TODO: Add your command handler code here
	if(bCoolTimeStart)
	{
	coolingTime=(timeGetTime()-coolTimeStart)/1000.;
	CString str;
	str.Format("%6.1f",coolingTime);
	}

}

void CMcbcView::OnTiggerStart() 
{
	// TODO: Add your command handler code here
	coolingTime=0;
	bCoolTimeStart=1;
	coolTimeStart=timeGetTime();
}

void CMcbcView::OnMenuFindPeak() 
{
	// TODO: Add your command handler code here
	int i,j;
	float sum4;
	long sum8[2048];
	int  wid1=8;
	for(i=0;i<MaxChannel/wid1;i++)
	{	
		sum8[i]=0;
		for(j=0;j<wid1;j++)
			sum8[i]+=pMemBuf[viewNum][i*wid1+j];
	}
	int max8;
	max8=sum8[0];
	int peakNum,peakCh[100];
	peakNum=0;
	float radio1[10];
	bool b1[10];
	int bn;
	int wid2=wid1/2;
	for(i=wid1;i<MaxChannel/wid1;i++)
	{
		for(j=0;j<wid2;j++)
		{
			b1[j+wid2]=sum8[i-j]>sum8[i-j-1];
			b1[j]=sum8[i+j]>sum8[i+j+1];
		}
		bn=0;
		for(j=0;j<wid1;j++)
			bn+=b1[j];
		if(bn==wid1)
		{
			peakCh[peakNum]=i;
			peakNum++;
		}
		if(peakNum>49)
			break;
	}
	int x=peakNum;
	int lefti[100],righti[100],mini,maxi,maxi2;
	for(i=0;i<peakNum;i++)
	{
		maxi=peakCh[i];
		for(j=maxi-2;j>0;j--)
		{
			if((sum8[j]<sum8[j+1])||(sum8[j-1]<sum8[j]))
				continue;
			else
			{
				lefti[i]=j+2;
				break;
			}
		}
		if(i<peakNum-1)
			maxi2=peakCh[i+1];
		else
			maxi2=MaxChannel/wid1-2;
		for(j=maxi+2;j<maxi2;j++)
		{
			if((sum8[j]>sum8[j+1])||(sum8[j+1]<sum8[j+2]))
				continue;
			else
			{
				righti[i]=j;
				break;
			}
		}
	}
	int y=lefti[0];
	RoiNum=peakNum;
	for(i=0;i<peakNum;i++)
	{
	RoiStart[i]=lefti[i]*wid1;
	RoiEnd[i]=righti[i]*wid1;
	}
	for(i=0;i<peakNum-1;i++)
	{
		if(RoiEnd[i]>RoiStart[i+1])
		{
			int wdt=RoiEnd[i]-RoiStart[i+1];
			wdt=wdt/2+2;
			RoiEnd[i]-=wdt;
			RoiStart[i+1]+=wdt;
		}
	}
//	findPeak(pMemBuf[viewNum],24,49,4);
	Invalidate(0);
	
}

void CMcbcView::OnMenuSmooth() 
{
	// TODO: Add your command handler code here
	int i;
	long *py=pMemBuf[viewNum];
	long temp[8192];
	for(i=2;i<MaxChannel;i++)
	{
		temp[i]=(-3*py[i-2]+12*py[i-1]+17*py[i]+12*py[i+1]-3*py[i+2])/35.;
	}
	memcpy(py,temp,MaxChannel*4);
	Invalidate(0);
}

void CMcbcView::OnMenuRoiSet() 
{
	// TODO: Add your command handler code here
	pDlgSetRoi->ShowWindow(SW_SHOW);
}
void CMcbcView::SetRoiMenu()
{
	int	i,minRoi,maxRoi;
	bool	oldbz=0;
	int		chn1,chn2,ochn1,ochn2;
		chn1 = pDlgSetRoi->m_RoiLeftCh;
		chn2 = pDlgSetRoi->m_RoiRightCh;
		if(chn1>chn2)
		{
			int temp=chn2;
			chn2=chn1;
			chn1=temp;
		}
		RoiCur = 0;
		for(i=0;i<RoiNum;i++)
		{
			if(chn1 >= RoiEnd[i])
			{
				RoiCur=i+1;
			}

		}
		maxRoi = RoiCur;
		minRoi = RoiCur;
		oldbz= 0;

		for(i=RoiCur;i<RoiNum;i++)
		{
			ochn1 = RoiStart[i];
			ochn2 = RoiEnd[i];
			if((ochn1>=chn1)&&(ochn1<=chn2))
			{
				oldbz=1;
				if(maxRoi<i)maxRoi=i;
			}
			if((ochn2>=chn1)&&(ochn2<=chn2))
			{
				oldbz=1;
				if(maxRoi<i)maxRoi=i;
			}
		}
		int subNum;
		if(oldbz==0)
		{
			RoiNum++;
			if(RoiNum>49) RoiNum=49;
			for(i=RoiNum; i>RoiCur; i--)
			{
				RoiStart[i] = RoiStart[i-1];
				RoiEnd[i] = RoiEnd[i-1];
			}
		}
		else
		{
			if(chn1>RoiStart[minRoi])
				chn1 = RoiStart[minRoi];
			if(chn2<RoiEnd[maxRoi])
				chn2 = RoiEnd[maxRoi];
			subNum = maxRoi-minRoi;
			RoiNum = RoiNum - subNum;
			for(i=minRoi+1;i<RoiNum;i++)
			{
				RoiStart[i] = RoiStart[i+subNum];
				RoiEnd[i] = RoiEnd[i+subNum];
			}
		}
		RoiStart[RoiCur]=chn1;
		RoiEnd[RoiCur]=chn2;
		dispspm.paintbz=1;
}
void CMcbcView::OnUpdateForce(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!bRevBegin);
}
