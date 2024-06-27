#include "stdafx.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include "stdafx.h"
#include "mcbc.h"
#include "mcbcDoc.h"
#include "mcbcView.h"
#include "DisplaySpm.h"
#include "math.h"
#include "DlgSetRoi.h"

CMcbcView *w1[4];
bool linebz=0; // linebz=1 表示打印谱
COLORREF mbkcolor,mforecolor,mredcolor,mcursorcolor;
extern "C" double log(double x);
extern	int		MaxChannel;
extern int	nStartX;
extern	int	nStartY;
class CDlgSetRoi;
extern	CDlgSetRoi   *pDlgSetRoi;
CPen	redPen;
extern	int	yBaseLine;
extern	int	mAdcGain;//采用率
extern	CEnergyKedu energyKedu[4];
extern	double   HV_k,HV_b,TH_k,TH_b;
extern	COLORREF adcColor[6];

long findmax(long far *pd,int ld)
	 {
		long xmax;
		xmax=(long)*pd;
		if(ld<2) return xmax;
		pd++;
		for(int i=1;i<=ld;i++)
			 {
			 if(*pd>xmax) xmax=*pd;
			 pd++;
			 }
	 if(xmax<0)xmax=0;
		return xmax;
	 }
DispSpm::DispSpm()
{
	DispBuf = new short[1000000];
	OldBuf = new short[1000000];
}
DispSpm::~DispSpm()
{
	delete [] DispBuf;
	delete [] OldBuf;
}
void DispSpm::ReadyData(long far *pdata)
 {
  int i;
  int xstep1;
  short  *ps;
  unsigned long ty;
  kh=(float)spara.Horz/(float)spara.SpmW;
  ps=DispBuf;
  pdata+=spara.StartChn;
  int Vort  = spara.Vort;
  double y2max;
  y2max=1.;
  for(i=0;i<spara.Vort;i++)
	y2max=y2max*2.;
  int	xcur=CursorChn-spara.StartChn;
  cursorY = (float)*(pdata+xcur);
  int yStartLine=65536*yBaseLine/20.;

  if(kh>1.)  //x方向屏幕可显示点数小于谱道数
	  {
		largebz=1;
		CursorX=int((CursorChn-spara.StartChn)/kh);
		if(spara.IsLog==1)
			 {
				for(i=1;i<spara.SpmW;i++)
				{
				xstep1=(int)((i+1)*kh)-(int)(i*kh);
				ty = findmax(pdata,xstep1);
				if(ty==0)
					*ps = spara.SpmH;
				else
				{
					*ps=int(spara.SpmH-log(float(ty+1))*(float)spara.SpmH/24);
				}
				pdata+=xstep1;ps++;
				}
			 }
		else
			{
			for(i=1;i<spara.SpmW;i++)
				{
				xstep1=(int)((i+1)*kh)-(int)(i*kh);
				*ps=spara.SpmH-int(findmax(pdata,xstep1)-yStartLine)/(y2max)*spara.SpmH;
				if(*ps<5) *ps=5;
				pdata+=xstep1;ps++;
				}
			}
	  }
  else  //x方向屏幕可显示点数大于谱道数
	{
		largebz=0;
		CursorX=int((CursorChn-spara.StartChn)/kh);
		if(spara.IsLog==1)
		 {
			for(i=1;i<spara.SpmW;i++)
				{
				if(i>spara.Horz)break;
				if(((*pdata)&0x7fffffff)==0)
					*ps = spara.SpmH;
				else
					*ps=int(spara.SpmH-log((float)((*pdata)&0x7fffffff)+1)*(float)spara.SpmH/24);
				pdata++;ps++;
				}
		 }
		else
			{
			for(i=1;i<spara.SpmW;i++)
				{
				if(i>spara.Horz)break;
				*ps=spara.SpmH-((*pdata)-yStartLine)/(y2max)*spara.SpmH;
				if(*ps<5)*ps=5;
				pdata++;ps++;
				}
			}
	}
 }
void DispSpm::DispData(CDC *pDC,int wid,int ch,CPen &mforePen,CPen &redPen)
{
	int start[50],end1[50],roibz,i;
	int xPt,yPt;
	char temp[80];
	kh=(float)spara.Horz/(float)spara.SpmW;
	roibz=0;
	COLORREF tempColor;
//	if(!linebz)  //不为打印
	if(!linebz&&!wid)  //不为打印
	{
		if(w1[0]->RoiNum>0)   //计算ROI 显示的X位置
		{
			roibz=1;
			for(i=0;i<w1[0]->RoiNum;i++)
			{
				if(kh>1.)
				{
					start[i]=int((w1[0]->RoiStart[i]-spara.StartChn)/kh);
					end1[i]=int((w1[0]->RoiEnd[i]-spara.StartChn)/kh);
				}
				else
				{
					start[i]=(w1[0]->RoiStart[i]-spara.StartChn);
					end1[i]=(w1[0]->RoiEnd[i]-spara.StartChn);
				}
			}
		}
		tempColor=mforecolor;
		xPt=3+nStartX;
		yPt=nStartY+10+DispBuf[0];
		pDC->MoveTo(xPt,yPt);
		for(i=0;i<spara.SpmW;i++)
		{
			BOOL redbz;
			redbz=0;
			if(roibz==1)  //有ROI
			{
				{
					for(int j=0;j<w1[0]->RoiNum;j++)
					{
						if((i>=start[j])&&(i<=end1[j])) //在ROI区内
						{
							pDC->SelectObject(&redPen);
							tempColor=mredcolor;
							break;
						}
						else
						{
							pDC->SelectObject(&mforePen);
							tempColor=mforecolor;
						}
					}
				}
				if(kh>1)
				{
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX+1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX-1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i]+1,tempColor);
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i]-1,tempColor);
					xPt=i+3+nStartX;
					yPt=nStartY+10+DispBuf[i];
				}
				else
				{
					if(i/kh+1>spara.SpmW) break;
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX)+1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX)-1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i]+1,tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i]-1,tempColor);
					xPt=int(i/kh+3+nStartX);
					yPt=nStartY+10+DispBuf[i];
				}
				redbz=1;
				pDC->LineTo(xPt,yPt);
			}
			else // if((DispBuf[i]!=OldBuf[i])||(paintbz==1))//无ROI
			{
//				if(wid)
//					tempColor=RGB(255,0,255);
//				else
				pDC->SelectObject(&mforePen);
				tempColor=mforecolor;
				pDC->SelectObject(&mforePen);

				yPt=nStartY+10+DispBuf[i];
//				pDC->LineTo(xPt,yPt);

				if(kh>1)
				{
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX+1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX-1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i]+1,tempColor);
					pDC->SetPixel(i+3+nStartX,nStartY+10+DispBuf[i]-1,tempColor);
					xPt=i+3+nStartX;
				}
				else
				{
					if(int(i/kh+1)>spara.SpmW) break;
					xPt=int(i/kh+3+nStartX);
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX)+1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX)-1,nStartY+10+DispBuf[i],tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i]+1,tempColor);
					pDC->SetPixel(int(i/kh+3+nStartX),nStartY+10+DispBuf[i]-1,tempColor);
				}
				yPt=nStartY+10+DispBuf[i];
				pDC->LineTo(xPt,yPt);
			}
			OldBuf[i]=DispBuf[i];
		}
	}
	else if(wid) //为打印
	{
		int sx,sy,x1,y1;
		sx=1;sy=1;
		pDC->SelectObject(&mforePen);
		pDC->MoveTo(int(sx+3+nStartX),int(sy+DispBuf[0]));
		for(i=1;i<spara.SpmW-1;i++)
		{
			if(kh>1)
			{
				x1=int(sx+(i+3+nStartX));
				y1=int(sy+DispBuf[i]);
				pDC->LineTo(x1,y1);
			}
			else
			{
				if(int(i/kh+1)>spara.SpmW) break;
				x1=int(sx+(i/kh+3+nStartX));
				y1=int(sy+DispBuf[i]);
				pDC->LineTo(x1,y1);
			}
		}
		pDC->SetBkColor(RGB(192,192,192));
		pDC->SetTextColor(adcColor[ch]);
		sprintf(temp,"CH%d",ch+1);
		pDC->TextOutA(x1+4,y1-10,temp);
		pDC->SetTextColor(RGB(0,0,0,));
	}
/*
	else  //为打印
	{
		int sx,sy;
		sx=1;sy=1;
		pDC->SelectObject(&mforePen);
		pDC->MoveTo(int(sx+3+nStartX),int(sy+DispBuf[0]));
		for(i=5;i<spara.SpmW-1;i++)
		{
			if(kh>1)
			{
				pDC->LineTo(int(sx+(i+3+nStartX)),int(sy+DispBuf[i]));
			}
			else
			{
				if(int(i/kh+1)>spara.SpmW) break;
				pDC->LineTo(int(sx+(i/kh+3+nStartX)),int(sy+DispBuf[i]));
			}
		}
	}
*/
}
void DispSpm::ClearData(CDC *pDC)
{
	int i;
	int  xstep2;
	kh=(float)spara.Horz/(float)spara.SpmW;
	xstep2=spara.SpmW/spara.Horz;
	if(xstep2<1) xstep2=1;
	for(i=0;i<spara.SpmW;i++)
	{
		if(kh>1)
			pDC->SetPixel(i+3+nStartX,OldBuf[i],mbkcolor);
		else
		{
			if(int(i/kh+1)>spara.SpmW) break;
			pDC->SetPixel(int(i/kh+3),OldBuf[i],mbkcolor);
		}
	}
}
void DispSpm::DispCursor(CDC *pDC,bool cbz,long *pdata)
{
int y1,y2,i;
int xcur,ycur,ystep1;
int Vort  = spara.Vort;
double ymax;
/*
kh=(float)spara.Horz/(float)spara.SpmW;
  ymax=1;
  for(i=0;i<spara.Vort;i++)
  ymax=ymax*2;
  if(kh>1.)  //x方向屏幕可显示点数小于谱道数
	{
		xcur=CursorChn-spara.StartChn;
//		double ty = (float)*(pdata+xcur);	
		if(spara.IsLog==1)
		{
			if(cursorY==0)
				ycur = spara.SpmH;
			else
				ycur=int(spara.SpmH-log(cursorY)*(float)spara.SpmH/24);
		}
		else
		{
			ycur=spara.SpmH-cursorY/ymax*spara.SpmH;
		}
		if(ycur<3)ycur=3;			
		if(ycur<30)
		{
			y1=ycur+1;
			y2=y1+30;
		}
		else
		{
			y2=ycur-1;
			y1=y2-30;
		}
		xcur=CursorX;
	}
	else
	{
		xcur=(CursorChn-spara.StartChn);
		if(DispBuf[xcur]<30)
		{
			y1=DispBuf[xcur]+1;
			y2=y1+30;
		}
		else
		{
			y2=DispBuf[xcur]-1;
			y1=y2-30;
		}
	}
*/
	mbkcolor=RGB(0,0, 0);
	if(!pDC->IsPrinting())
	{
		if(cbz)
			for(int i=5;i<spara.SpmH;i++)
			{
				pDC->SetPixel(CursorX+3+nStartX,nStartY+10+i,mcursorcolor);
//				pDC->SetPixel(CursorX+3+nStartX,y1+i,mcursorcolor);
//				pDC->SetPixel(CursorX+3-1,y1+i,mcursorcolor);
//				pDC->SetPixel(CursorX+3+1,y1+i,mcursorcolor);
			}
		else
			for(int i=5;i<spara.SpmH;i++)
			{
				pDC->SetPixel(CursorX+3+nStartX,nStartY+10+i,mbkcolor);
//				pDC->SetPixel(CursorX+3-1,y1+i,mbkcolor);
//				pDC->SetPixel(CursorX+3+1,y1+i,mbkcolor);
			}
	}
 
}
void DispSpm::DispLabel(CDC *pDC)
{
	int i;
	CString str,str1;
	for(i=0;i<6;i++)
	{
		pDC->MoveTo(3+nStartX,nStartY+10+(spara.SpmH)*i/5);
		pDC->LineTo(3+nStartX-10*kx,nStartY+10+(spara.SpmH)*i/5);
	}
	double ymax;
	ymax=spara.SpmH*TH_k+TH_b;
	if(spara.IsLog==1)
	{
		ymax=24;
		for(i=0;i<6;i++)
		{
			str.Format("%4d",int(pow(2,ymax*i/5.)*TH_k+TH_b));
			pDC->TextOut(nStartX-70*kx,nStartY+10-5*ky+(spara.SpmH)*(5-i)/5,str);
		}
	}
	else
	{
		ymax=1;
		double yStartLine=65536*yBaseLine/20.;
		yStartLine=yStartLine*TH_k+TH_b;
		float ydata1,energy;
		for(i=0;i<spara.Vort;i++)
			ymax=ymax*2;
		ymax=ymax*TH_k+TH_b;
		for(i=0;i<6;i++)
		{
			ydata1=ymax-i/5.*ymax+yStartLine;
			str.Format("%5.1f",ydata1);
			pDC->TextOut(nStartX-70*kx,nStartY+10-5*ky+(spara.SpmH)*i/5,str);
		}
	}
}
void DispSpm::DispLabelX(CDC *pDC)
{
	int i;
	CString str,str1;
/*
	for(i=0;i<9;i++)
	{
		pDC->MoveTo(3+nStartX+spara.SpmW*i/8.,spara.SpmH);
		pDC->LineTo(3+nStartX+spara.SpmW*i/8.,spara.SpmH-10*ky);
	}
*/
	int	xmin;
	char c5;
	CString str2;
//	xmin=(spara.StartChn+1)*controlData.m_VacuumTime;
	int	xrange;
	if(mAdcGain==1)
		xrange=4000;
	else if(mAdcGain==2)
		xrange=160;
	else
		xrange=500;
	xrange=spara.Horz/1000000.*xrange;
	xmin=(spara.StartChn)/(float)spara.Horz*xrange;
	for(i=0;i<9;i++)
	{
		str.Format("%5.2f",xmin+xrange*i/8.-xrange/2.);
		pDC->TextOut(3+nStartX+spara.SpmW*i/8.-10*kx,nStartY+10+spara.SpmH+15*ky,str);
	}

//	str.Format("s");
//	pDC->TextOut(3+nStartX+spara.SpmW-10*kx,spara.SpmH+15*ky,str);
}
