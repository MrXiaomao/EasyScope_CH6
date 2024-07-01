// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F19B1BA8_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
#define AFX_MAINFRM_H__F19B1BA8_7809_11D4_B521_EBC39990CA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DisplaySpm.h"
#include "define.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

	void	DoTimer();
	void	DoTimer1s();
	bool    bChannelStart;
// Operations
public:

	CStatusBar  m_wndStatusBar;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
	CTime curTime;
	ChnHead chnHead;
	ChnEnd  chnEnd;
	int year,month,day,oldday;
	int hour,minute,second,oldsecond;
	UINT	timerID;
	BOOL	initPresetTime;
	int		nTimerNo;
	SpmPara	mcaPara[4],mcsPara[4];
	bool	bFirst;
	CString	m_output;
// Generated message map functions
public:
	CStatic m_dlgbarDate;
	void	AdcChange();
	void	OpenRS232();
	BOOL	bComBusy;
	int		nSendComNum;
	int		nReceiveNum;
	int		nReceiveVnum;
	int		nSendComByte;
	int		nSendComLen;
	void	SendData(int i);
	int		ReadCommData(HANDLE hCom,BYTE *m_RxData);
	void	ReceiveData();
	void	ReceiveDataWave(int nTCP );
	void	ReceiveDataWaveTCP(int nTCP);
	void	ReceiveDataWaveTCP2(int nTCP);
	int		ReadTCPData(int iClientSock,BYTE *m_RxData);
	void	initTCP(int nTCP,bool &bTCPIPOK);
	int		getCount(int ch);
	void	setWaveMode(bool bShortWave,bool bHardware,bool bCh1);

	bool Transfer(HANDLE* hDev1, HANDLE* hDev2);
	void DoTimerAuto();
	void SetSampleRate(int n);//采样率设置 n=1: 41.7M  n=2:  25M   n=3:  7.8M
	HICON m_hIcon;
	void Init();
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRadioOne();
	afx_msg void OnRadioTwo();
	afx_msg void OnRadioHardWare();
	afx_msg void OnRadioBuffer();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateAdc512(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdc1024(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdc2048(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdc4096(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdc8192(CCmdUI* pCmdUI);
	afx_msg void OnMenuBkcolor();
	afx_msg void OnMenuForecolor();
	afx_msg void OnMenuRoicolor();
	afx_msg void OnMenuCursorcolor();
	afx_msg void OnTimePreset();
	afx_msg void OnMenuMcatobuf();
	afx_msg void OnMenuEkedu();
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuCloseInt();
	afx_msg void OnMenuRs232Set();
	afx_msg void OnMenuPaperStart();
	afx_msg void OnMenuPaperStop();
	afx_msg void OnMenuPumpOn();
	afx_msg void OnMenuPumpOff();
	afx_msg void OnUpdateMenuPumpOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPumpOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPaperStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPaperStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuAlarmOn();
	afx_msg void OnUpdateMenuAlarmOn(CCmdUI* pCmdUI);
	afx_msg void OnMenuAlarmOff();
	afx_msg void OnUpdateMenuAlarmOff(CCmdUI* pCmdUI);
	afx_msg void OnPaperV();
	afx_msg void OnMenuAutoSetup();
	afx_msg void OnMenuAutoStart();
	afx_msg void OnUpdateMenuAutoStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateForce(CCmdUI* pCmdUI);
	afx_msg void OnMenuAutoStop();
	afx_msg void OnUpdateMenuAutoStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuAutoClear();
	afx_msg void OnUpdateMenuAutoClear(CCmdUI* pCmdUI);
	afx_msg void OnMenuSetC51();
	afx_msg void OnMenuAmplifier();
	afx_msg void OnMenuFit();
	afx_msg void OnMenuitemCloseRs232();
	afx_msg void OnMenuitemForward();
	afx_msg void OnMenuitemReverse();
	afx_msg void OnMenuitemDc();
	afx_msg void OnMenuitemAc();
	afx_msg void OnMenuitemShiftCode();
	afx_msg void OnMenuitemCompCode();
	afx_msg void OnMenuitemSetLd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuAuto();
	afx_msg void OnMenuNormal();
	afx_msg void OnMenuSingle();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuForce();
	afx_msg void OnMenuTripUp();
	afx_msg void OnMenuTripDown();
	afx_msg void OnMenuTripLevel();
	afx_msg void OnMenu50();
	afx_msg void OnMenu10k();
	afx_msg void OnMenuBaseline();
	afx_msg void OnMenuitemSetLd2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedMenuitemDc2();
	afx_msg void OnBnClickedMenuitemForward2();
	afx_msg void OnBnClickedMenuitemShiftCode2();
	afx_msg void OnMenuitemSetHv();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedForce();
	afx_msg void OnBnClickedHvOpen();
	afx_msg void OnBnClickedCh1();
	afx_msg void OnBnClickedCh2();
	afx_msg void OnBnClickedCh3();
	afx_msg void OnBnClickedCh4();
	afx_msg void OnBnClickedCh5();
	afx_msg void OnBnClickedCh6();
	afx_msg void OnBnClickedCalCount();
	afx_msg void OnCalCount();
	afx_msg void OnBnClickedRadioWaveShort();
	afx_msg void OnBnClickedRadioWaveShort2();
	void OnUpdateRun(CCmdUI* pCmdUI);
	void OnUpdateStop(CCmdUI* pCmdUI);

	afx_msg void On32871();
	afx_msg void OnMenu();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F19B1BA8_7809_11D4_B521_EBC39990CA2A__INCLUDED_)
