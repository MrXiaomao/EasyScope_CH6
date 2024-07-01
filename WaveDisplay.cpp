// WaveDisplay.cpp: implementation of the CWaveDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mcbc.h"
#include "WaveDisplay.h"
#include "define.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern	long		*pDispAdcBuf[8];

int data0Len;
unsigned char *GPibBuffer[4];
double	timeRange,vRange[4];
int yOffset[4]; 

 int waveLen;
int	mIsTDS;
extern	double   HV_k,HV_b,TH_k,TH_b;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int maxy(unsigned char *buf,int z0,int len)
{
	int i;
	int max=0,min=255,maxi=0,mini=0;
	for(i=0;i<len;i++)
	{
		if(max<*(buf+i))
		{
			max=*(buf+i);
			maxi=i;
		}
		if(min>*(buf+i))
		{
			min=*(buf+i);
			mini=i;
		}
	}
	if(max-z0>z0-min)
	{
/*		max=0;
		for(i=0;i<10;i++)
		{
			max+=*(buf+maxi-5+i);
		}
		max/=10;*/
		max=max-z0;
	}
	else
	{
/*		max=0;
		for(i=0;i<10;i++)
		{
			max+=*(buf+mini-5+i);
		}
		max/=10;
*/
		max=z0-min;
	}
	return max;
}
CWaveDisplay::CWaveDisplay()
{
	m_StartX = 0;
	m_StartY = 0;
	m_EndX = waveLen;
	m_EndY = 256;
	nScrOrgX = 0;
	nScrOrgY = 0;
	nScrWidth = 400;
	nScrHeight = 400;
	mlineColor = RGB(0,127,0);
	mlineColor = RGB(127,127,127);
	mbkColor=RGB(0,0,0);
	leftColor = RGB(220,220,220);
	topColor = RGB(128,128,128);
	linePen.CreatePen(PS_SOLID,1,mlineColor);
	blackPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	bkBrush.CreateSolidBrush(mbkColor);
	whiteBrush.CreateSolidBrush(RGB(255,255,255));
	wavePen[0].CreatePen(PS_SOLID,1,RGB(255,255,0));
	wavePen[1].CreatePen(PS_SOLID,1,RGB(0,255,255));
	wavePen[2].CreatePen(PS_SOLID,1,RGB(255,0,128));
	wavePen[3].CreatePen(PS_SOLID,1,RGB(0,255,0));
//	thinPen.CreatePen(PS_SOLID,1,RGB(0,64,0));
	thinPen.CreatePen(PS_SOLID,1,RGB(0,64,0));
	dotPen.CreatePen(PS_DOT,5,RGB(0,64,0));
	printPen.CreatePen(PS_SOLID,5,RGB(0,0,0));
}

CWaveDisplay::~CWaveDisplay()
{
	linePen.DeleteObject();
	blackPen.DeleteObject();
	bkBrush.DeleteObject();
	whiteBrush.DeleteObject();
	wavePen[0].DeleteObject();
	wavePen[1].DeleteObject();
	wavePen[2].DeleteObject();
	wavePen[3].DeleteObject();
	thinPen.DeleteObject();
	dotPen.DeleteObject();
	printPen.DeleteObject();
}
void CWaveDisplay::DrawLine()
{
	int	xTextWidth=int(86*kx),yTextWidth=int(50*ky);
	if(!pDC->IsPrinting())
		oldBrush = (CBrush *)pDC->SelectObject(&bkBrush);
	else
		oldBrush = (CBrush *)pDC->SelectObject(&whiteBrush);
	
	CRect rect(CPoint(nScrOrgX+xTextWidth,nScrOrgY+10*ky),
		CSize(nScrWidth-xTextWidth,nScrHeight-yTextWidth));
	pDC->Rectangle(&rect);


	rect=CRect(CPoint(nScrOrgX,nScrOrgY-30),
		CSize(10,nScrHeight+30));
	if(!pDC->IsPrinting())
		pDC->FillSolidRect(&rect,leftColor);//left

	rect=CRect(CPoint(nScrOrgX,nScrOrgY-33*ky),
		CSize(nScrWidth+43*kx,3*ky));
	if(!pDC->IsPrinting())
		pDC->FillSolidRect(&rect,leftColor);//top

	rect=CRect(CPoint(nScrOrgX,nScrOrgY+nScrHeight),
		CSize(nScrWidth+43*kx,3*ky));
	if(!pDC->IsPrinting())
		pDC->FillSolidRect(&rect,topColor);//bottom

	rect=CRect(CPoint(nScrOrgX+nScrWidth+40*kx,nScrOrgY-30),
		CSize(4*kx,nScrHeight+30));
	if(!pDC->IsPrinting())
		pDC->FillSolidRect(&rect,topColor);//right

	double	xUnit = (nScrWidth-xTextWidth)/8.;
//	double	yUnit = (nScrHeight-yTextWidth)/8.;
	double	yUnit = (nScrHeight-yTextWidth)/4.;
	double	xUnit5 = (nScrWidth-xTextWidth)/8./5.;
//	double	yUnit5 = (nScrHeight-yTextWidth)/8./5.;
	double	yUnit5 = (nScrHeight-yTextWidth)/10./5.;
	if(mIsTDS==0)
		yUnit5 = (nScrHeight-yTextWidth)/8./5.;
	int		i;

	oldPen = (CPen *)pDC->SelectObject(&thinPen);
	if(!pDC->IsPrinting())
		pLinePen = &linePen;
	else
		pLinePen = &dotPen;
	for(i=0;i<8;i++)//显示垂直粗线
	{
		pDC->SelectObject(pLinePen);
		pDC->MoveTo(nScrOrgX+xTextWidth+int(i*xUnit),nScrOrgY+10*ky);
		pDC->LineTo(nScrOrgX+xTextWidth+int(i*xUnit),
			nScrOrgY+nScrHeight-yTextWidth+10*ky);
	}

	for(i=0;i<4;i++)//显示水平粗线
	{
		pDC->SelectObject(pLinePen);
		pDC->MoveTo(nScrOrgX+xTextWidth,nScrOrgY+10*ky+int(i*yUnit));
		pDC->LineTo(nScrOrgX+nScrWidth,nScrOrgY+10*ky+int(i*yUnit));
	}
	
	pDC->SelectObject(&blackPen);
	for(i=0;i<6;i++)//显示垂直黑线(在X)
	{
		pDC->MoveTo(nScrOrgX+xTextWidth+int(i*xUnit),
			nScrOrgY+nScrHeight-yTextWidth+15*ky);
		pDC->LineTo(nScrOrgX+xTextWidth+int(i*xUnit),
			nScrOrgY+nScrHeight-yTextWidth+20*ky);
	}

	for(i=0;i<5;i++)
	{
		pDC->MoveTo(int(nScrOrgX+xTextWidth-10*kx),int(nScrOrgY+10*ky+i*yUnit));
		pDC->LineTo(int(nScrOrgX+xTextWidth-5*kx),int(nScrOrgY+10*ky+i*yUnit));
	}
	
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}
void CWaveDisplay::DrawData(int n)//n=1 draw ch1; n=2 draw ch2; n=3 draw ch1 and ch2
{
int	i;
int	xTextWidth=int(80*kx),yTextWidth=int(50*ky);
int	xPt,yPt;
double	xUnit = (nScrWidth-xTextWidth)/8.;
double	yUnit = (nScrHeight-yTextWidth)/10.;
//long *GBuffer=pDispAdcBuf[n]+data0Len;
long *GBuffer=pDispAdcBuf[n];
	if(!pDC->IsPrinting())
	{
		oldPen = (CPen *)pDC->SelectObject(&wavePen[n]);
	}
	else
		oldPen = (CPen *)pDC->SelectObject(&printPen);
	int	xRange = nScrWidth-xTextWidth;
	int	yRange = nScrHeight-yTextWidth;
	int xOrg = nScrOrgX+xTextWidth;
	int yOrg = int(nScrOrgY+10*ky);
	xPt = int(xOrg);
	yPt = int(yOrg+(256-*(GBuffer))/256.*yRange);
	if(mIsTDS==0)
		yPt = int(yOrg+(225-*(GBuffer))/200.*yRange);
	pDC->MoveTo(xPt,yPt);
	for(i=1;i<waveLen-1;i++)
	{
		xPt = int(xOrg+i/(float)waveLen*xRange);
		yPt = int(yOrg+(256-*((GBuffer)+i))/256.*yRange);
		if(mIsTDS==0)
			yPt = int(yOrg+(228-*(GBuffer+i))/200.*yRange);
		pDC->LineTo(xPt,yPt);
//		pDC->Ellipse(xPt-10,yPt-10,xPt+10,yPt+10);
	}
	CString str;
	str.Format("%d",n+1);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(xPt+int(5*kx),yPt,str);
	pDC->SetTextColor(RGB(0,0,0));

	pDC->SelectObject(oldPen);
		
}
