// mcbcView.h : interface of the CMcbcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCBCVIEW_H__F19B1BAE_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
#define AFX_MCBCVIEW_H__F19B1BAE_7809_11D4_B521_EBC39990CA2A__INCLUDED_
#include "displaySpm.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DispSpm;
class CMcbcView : public CView
{
protected: // create from serialization only
	CMcbcView();
	DECLARE_DYNCREATE(CMcbcView)

// Attributes
public:
	CMcbcDoc* GetDocument();

// Operations
public:
	int RoiNum, RoiNum1,RoiCur,RoiCur1;
	int RoiStart[50],RoiStart1[50];
	int RoiEnd[50],RoiEnd1[50];
	DispSpm dispspm;
	CMcbcDoc	*m_pDoc;
	long			*pAdcbufold;
	bool	bSetRoiDlg;
	int nScrHeight,nScrWidth;
	int	xRange,yRange;
	int	xTextWidth;
	int yTextWidth;
	CPen	wavePen[7];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcbcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearCursor();
	void ClearDisp(CDC *pDC);
	int  DispGross(long *pdata);
	void DispCursor();
	virtual ~CMcbcView();
	void	mChangColor();
	CTime curTime;
	float	coolingTime;
	int		coolTimeStart;
	bool	bCoolTimeStart;
	void	TimerTick();
	void	SetRoiMenu();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	BYTE	viewNum;
	int		Roi;
	bool	clearbz,mcsbz, buttondown,boolRoiRect;
	CRect	rectMouseMove;
	CDC		*rectDC;
	CPen	whitePen,*oldPen,redPen;
	int		tmx3;
	double	kh;
	int		inBuf[10];
	CBrush	bkBrush;
	CBrush	adcBrush[6];
	BOOL	bSetRoi;
	BYTE	deviceIoChn;
	double  kx,ky;
	void	SetupLevel();
	long	netArea,crossArea;
	float	FwhmCh,FwhmEn,centerCh,centerEn;
// Generated message map functions
protected:
	void Paint(CDC *pDC);
	//{{AFX_MSG(CMcbcView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonExpand();
	afx_msg void OnButtonSmall();
	afx_msg void OnBtnAddy();
	afx_msg void OnBtnLogy();
	afx_msg void OnBtnSuby();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuSetroi();
	afx_msg void OnMenuClearroi();
	afx_msg void OnMenuClear();
	afx_msg void OnMenuClearAll();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateForce(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTiggerStart();
	afx_msg void OnMenuFindPeak();
	afx_msg void OnMenuSmooth();
	afx_msg void OnMenuRoiSet();
	//}}AFX_MSG
	public:
	afx_msg void OnButtonStop();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonStart();

	void	OnBtnAuto();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mcbcView.cpp
inline CMcbcDoc* CMcbcView::GetDocument()
   { return (CMcbcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCBCVIEW_H__F19B1BAE_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
