// WaveDisplay.h: interface for the CWaveDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEDISPLAY_H__41935CA0_682A_11D6_ACD5_0000E2659362__INCLUDED_)
#define AFX_WAVEDISPLAY_H__41935CA0_682A_11D6_ACD5_0000E2659362__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaveDisplay  
{
public:
	CWaveDisplay();
	virtual ~CWaveDisplay();
	void	DrawLine();
	void	DrawData(int n);//n=1 draw ch1; n=2 draw ch2; n=3 draw ch1 and ch2
	CDC		*pDC;
	COLORREF mbkColor,mlineColor,mdataColor;
	COLORREF	leftColor,topColor;
	int		nScrOrgX,nScrOrgY;
	int		nScrWidth,nScrHeight;
	double	kx,ky;
protected:
	double	m_StartX,m_EndX,m_StartY,m_EndY;
	double	m_XUnit,m_YUnit;
	CString	xTitle,yTitle; 
	CPen	linePen,*oldPen,blackPen,thinPen;
	CPen	wavePen[4];
	CPen	dotPen,*pLinePen;
	CPen	printPen;
	CBrush	bkBrush,*oldBrush,whiteBrush;;
};

#endif // !defined(AFX_WAVEDISPLAY_H__41935CA0_682A_11D6_ACD5_0000E2659362__INCLUDED_)
