// WaveView.cpp : implementation file
//

#include "stdafx.h"
#include "a208.h"
#include "WaveView.h"
#include "WaveDisplay.h"
#include "MainFrm.h"
#include "PulseSet.h"
#include "define.h"
#include "PrintReportFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int data0Len;

extern	char m_nCurNo[18];
extern unsigned char *GPibBuffer[4];
extern	SCurPulseSet CurPulseSet;
extern	double	timeRange,vRange[4];
extern	ControlCom	controlCom;
extern	ControlData	controlData;
extern	CPrintReportFun cprintReportFun;
extern	bool bPrintPreview;
extern	bool	bHasSaved;
extern	bool linebz; // linebz=1 ±íÊ¾´òÓ¡Æ×
extern	int	bStateBar;
extern	int yOffset[4]; 
extern	ScurSetData *pCurSetData;
extern	int nDetectorSel;
//extern  float   *pDTWaveFwhm[2];

extern	CDialogBar *m_pDlgBarCurrent;
extern  CDialogBar *m_pDlgBarHv;
extern  CDialogBar *m_pDlgBarStart;
extern	CDialogBar *m_pDlgBarCursor;
extern	CDialogBar *m_pDlgBarWave;
extern	char	curPath[200];
extern	char	dataPath[200];
extern int waveLen;
extern int	mIsTDS;

int maxy(unsigned char *buf,int z0,int len);
float GetFwhm(int detector);
float GetAreas(int detector);
int wavebd(unsigned char *buf)
{
	int i;
	int sum=0;
	for(i=10;i<60;i++)
	{
		sum+=*(buf+i);

	}
	return sum/50.;
}
/////////////////////////////////////////////////////////////////////////////
// CWaveView

IMPLEMENT_DYNCREATE(CWaveView, CFormView)

CWaveView::CWaveView()
	: CFormView(CWaveView::IDD)
{
	//{{AFX_DATA_INIT(CWaveView)
	m_Num = 0;
	m_Ch1On = TRUE;
	m_Ch2On = TRUE;
	m_Radio1 = -1;
	m_Total = _T("");
	m_Ch3On = TRUE;
	m_Ch4On = TRUE;
	//}}AFX_DATA_INIT
	bCursor=0;
	curX0=5;
	curX=5;
	cursorPen.CreatePen(PS_SOLID,2,RGB(0,0,255));
}

CWaveView::~CWaveView()
{
//	m_PulseSet.Close();
//	m_PulseSet2.Close();
	cursorPen.DeleteObject();

}

void CWaveView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveView)
	DDX_Control(pDX, IDC_RADIO1, m_Radio);
	DDX_Control(pDX, IDC_EDIT_NUM, m_Edit1);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_COMBO_NO2, m_ComboNo2);
	DDX_Control(pDX, IDC_COMBO_NO, m_ComboNo);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT_NUM, m_Num);
	DDX_Check(pDX, IDC_CH1_ON, m_Ch1On);
	DDX_Check(pDX, IDC_CH2_ON, m_Ch2On);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio1);
	DDX_Text(pDX, IDC_TOTAL, m_Total);
	DDX_Check(pDX, IDC_CH3_ON, m_Ch3On);
	DDX_Check(pDX, IDC_CH4_ON, m_Ch4On);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveView, CFormView)
	//{{AFX_MSG_MAP(CWaveView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_CBN_SELCHANGE(IDC_COMBO_NO, OnSelchangeComboNo)
	ON_BN_CLICKED(IDC_CH1_ON, OnCh1On)
	ON_BN_CLICKED(IDC_CH2_ON, OnCh2On)
	ON_CBN_SELCHANGE(IDC_COMBO_NO2, OnSelchangeComboNo2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CH3_ON, OnCh3On)
	ON_BN_CLICKED(IDC_CH4_ON, OnCh4On)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveView diagnostics

#ifdef _DEBUG
void CWaveView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWaveView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveView message handlers

void CWaveView::OnDraw(CDC* pDC) 
{
	int i;
	CRect rect;
	GetClientRect(&rect);
	if(pDC->IsPrinting())
	{
		ky = pDC->GetDeviceCaps(VERTRES)/600.;
		kx = pDC->GetDeviceCaps(HORZRES)/800./1.5;
		linebz=1;
	}
	else
	{
		ky = 1;
		kx = 1;
		linebz=0;
	}
	rect.right = int(rect.right*kx);
	rect.bottom = int(rect.bottom*ky);
	CBrush brush;
	brush.CreateStockObject(LTGRAY_BRUSH);
	if(!pDC->IsPrinting())
		pDC->FillRect(&rect,&brush);

	nScrOrgX = int(rect.left+20*kx);
	nScrOrgY = int(rect.top+20*ky);
	nScrWidth = int(rect.Width()-180*kx);
	nScrHeight = int(rect.Height()-50*ky);
	int	xTextWidth=int(80*kx),yTextWidth=int(50*ky);
	double	xUnit = (nScrWidth-xTextWidth)/10.;
	double	yUnit = (nScrHeight-yTextWidth)/10.;

	CWaveDisplay waveDisplay;
	waveDisplay.pDC = pDC;
	waveDisplay.kx=kx;
	waveDisplay.ky=ky;
	waveDisplay.nScrOrgX = int(rect.left+20*kx);
	waveDisplay.nScrOrgY = int(rect.top+20*ky);
	waveDisplay.nScrWidth = int(rect.Width()-180*kx);
	waveDisplay.nScrHeight = int(rect.Height()-50*ky);
	waveDisplay.DrawLine();
//	UpdateData(TRUE);
	if(!pDC->IsPrinting())
	{
		CPen *pOldPen=(CPen *)pDC->SelectObject(&cursorPen);
		pDC->MoveTo(curX*kx,nScrOrgY+10*ky);
		pDC->LineTo(curX*kx,nScrOrgY+nScrHeight-yTextWidth+10*ky);
		pDC->SelectObject(pOldPen);
	}
	if(m_Ch1On)
		waveDisplay.DrawData(0);
	if(m_Ch2On)
		waveDisplay.DrawData(1);
	if(m_Ch3On)
		waveDisplay.DrawData(2);
	if(m_Ch4On)
		waveDisplay.DrawData(3);

	CString str;

	if(timeRange<1e-6)
	{
		str.Format("%d ns/div",int(timeRange*1.0e9));
		timeRangeDisp=timeRange*1.0e9;
		strcpy(timeRangeUnit,"ns");
	}
	else if(timeRange<1e-3)
	{
		double tx= timeRange*(double)1.0e6;
		str.Format("%3.0lf us/div",tx);
		timeRangeDisp=timeRange*1.0e6;
		strcpy(timeRangeUnit,"us");
	}
	else
	{
		str.Format("%d ms/div",int(timeRange*1e3));
		timeRangeDisp=timeRange*1.0e3;
		strcpy(timeRangeUnit,"ms");
	}
	pDC->SetBkColor(RGB(192,192,192));
	pDC->TextOut(int(nScrOrgX+10*kx+5*xUnit),nScrOrgY+nScrHeight-17*ky,str);
	CString str1[4],str2[4];	
	for(i=0;i<4;i++)
	{
		if(vRange[i]<1e-3)
		{
			str1[i].Format("%d",int(vRange[i]*1e6));
			str2[i] = "uV/div";
		}
		else if(vRange[i]<1)
		{
			str1[i].Format("%d",int(vRange[i]*1e3));
			str2[i] = "mV/div";
		}
		else
		{
			str1[i].Format("%d",int(vRange[i]));
			str2[i] = "V/div";
		}
	}
	int chn=CurPulseSet.m_SetWaveCh1+1;
	if(chn>4)chn=4;
	if(chn<1)chn=1;
	str.Format("CH%d",chn);
	pDC->TextOut(int(nScrOrgX+15*kx),int(nScrOrgY+yUnit*1),str);
	pDC->TextOut(int(nScrOrgX+12*kx),int(nScrOrgY+yUnit*1.5),"°ÐÑ¹²¨ÐÎ",8);
	pDC->TextOut(int(nScrOrgX+10*kx),int(nScrOrgY+yUnit*2.0),str1[chn-1]+str2[chn-1]);

	chn=CurPulseSet.m_SetWaveCh2+1;
	if(chn>4)chn=4;
	if(chn<1)chn=1;
	str.Format("CH%d",chn);
	pDC->TextOut(int(nScrOrgX+15*kx),int(nScrOrgY+yUnit*3),str);
	pDC->TextOut(int(nScrOrgX+12*kx),int(nScrOrgY+yUnit*3.5),"Ô´Á÷²¨ÐÎ",8);
	pDC->TextOut(int(nScrOrgX+10*kx),int(nScrOrgY+yUnit*4.0),str1[chn-1]+str2[chn-1]);

	chn=CurPulseSet.m_SetWaveCh3+1;
	if(chn>4)chn=4;
	if(chn<1)chn=1;
	str.Format("CH%d",chn);
	pDC->TextOut(int(nScrOrgX+15*kx),int(nScrOrgY+yUnit*5),str);
	pDC->TextOut(int(nScrOrgX+12*kx),int(nScrOrgY+yUnit*5.5),"Ì½²âÆ÷S1",8);
	pDC->TextOut(int(nScrOrgX+10*kx),int(nScrOrgY+yUnit*6.0),str1[chn-1]+str2[chn-1]);

	chn=CurPulseSet.m_SetWaveCh4+1;
	if(chn>4)chn=4;
	if(chn<1)chn=1;
	str.Format("CH%d",chn);
	pDC->TextOut(int(nScrOrgX+15*kx),int(nScrOrgY+yUnit*7),str);
	pDC->TextOut(int(nScrOrgX+12*kx),int(nScrOrgY+yUnit*7.5),"Ì½²âÆ÷S2",8);
	pDC->TextOut(int(nScrOrgX+10*kx),int(nScrOrgY+yUnit*8.0),str1[chn-1]+str2[chn-1]);

	for(i=0;i<11;i++)
	{
		str.Format("%d",i);
		pDC->TextOut(nScrOrgX+xTextWidth+int(i*xUnit-5*kx),
			int(nScrOrgY+nScrHeight-yTextWidth+20*ky),
			str);
	}
	
	int	pos;
	if(m_Radio1==0)
		str.Format("¼ì²â±àºÅ£º%s",m_nCurNo);
	else
		str.Format("±¾µ×±àºÅ£º%s",m_nCurNo);
	if(pDC->IsPrinting())
	{
		pDC->TextOut(150*kx,waveDisplay.nScrHeight+20*ky,str);
		pos=m_Spin1.GetPos();
		str.Format("´ÎÊý£º%d",pos);
		pDC->TextOut(300*kx,waveDisplay.nScrHeight+20*ky,str);
	}
	brush.DeleteObject();
}
void CWaveView::OpenDatabase()
{
CString str1;
BOOL	hasOpen=1;
int		i,mcurNo=-1,mcurNo2=-1;
	m_PulseSet.m_strSort = CString("[No] DESC");
	m_PulseSet.m_strFilter = "";
	try
	{
    	m_PulseSet.Open(	);
	}
	catch( CDaoException* e )
	{
		 AfxMessageBox( 
				e->m_pErrorInfo->m_strDescription, 
				MB_ICONEXCLAMATION );
		// Delete the incomplete recordset object
		e->Delete();
		hasOpen  = 0;
   }
	i = 0;
	if(hasOpen)
	{
	if(m_PulseSet.GetRecordCount())
	{
		while(!m_PulseSet.IsEOF())
		{
			if(bInit)
			{
				m_ComboNo.AddString(m_PulseSet.m_No.GetBuffer(18));
				if(m_PulseSet.m_No==m_nCurNo)
				{
					m_ComboNo.SetCurSel(i);
					mcurNo=i;
					m_Radio1=0;
					if(controlCom.nState!=1)
					{
						int i,j;
						CString str;
						j=0;
						BYTE *pTemp=(BYTE *)m_PulseSet.m_data.GetData();
						memcpy(pCurSetData,pTemp,m_PulseSet.m_data.GetSize());
						CurPulseSet.m_setNum = m_PulseSet.m_setNum;
						timeRange = pCurSetData->m_timeRange;
						for(i=0;i<4;i++)
						{
							vRange[i] = pCurSetData->m_vRange[i];
							yOffset[i]=pCurSetData->m_ChOffset[i];
						}
						CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
						pMainFrame->fileName.Format("%s\\wave\\%s.sam",dataPath,m_nCurNo);
						int nTotal=0;
						if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
						{
							nTotal = pMainFrame->mWaveFile.GetLength()/waveLen/4;
							pMainFrame->mWaveFile.Close();
						}
						m_Spin1.SetRange(1,nTotal);
						m_Total.Format("%d",nTotal);
						CurPulseSet.m_setNum = nTotal;
					}

				}
			}
			m_PulseSet.MoveNext();
			i++;
		}
		if(bInit)
		{
			m_PulseSet.MoveFirst();
			for(i=0;i<mcurNo;i++)
				m_PulseSet.MoveNext();
		}
	}
	if(bInit)
	{
		if(m_Radio1==0)
		{
			m_Num=1;
			OnButton1();
		}
	}
	}
}
void CWaveView::OpenBackDatabase()
{
CString str1;
BOOL	hasOpen=1;
int		i,mcurNo=-1,mcurNo2=-1;
	m_PulseSet2.m_strSort = CString("[No] DESC");
	m_PulseSet2.m_strFilter = "";
	try
	{
    	m_PulseSet2.Open(	);
	}
	catch( CDaoException* e )
	{
		 AfxMessageBox( 
				e->m_pErrorInfo->m_strDescription, 
				MB_ICONEXCLAMATION );
		// Delete the incomplete recordset object
		e->Delete();
		hasOpen  = 0;
   }
	i = 0;
	if(hasOpen)
	{
	if(m_PulseSet2.GetRecordCount())
	{

		while(!m_PulseSet2.IsEOF())
		{
		if(bInit)
		{
			m_ComboNo2.AddString(m_PulseSet2.m_No.GetBuffer(18));
			if(m_PulseSet2.m_No==m_nCurNo)
			{
				m_ComboNo2.SetCurSel(i);
				mcurNo2=i;
				m_Radio1=1;
				if(controlCom.nState!=1)
				{
					BYTE *pTemp=(BYTE *)m_PulseSet2.m_data.GetData();
					memcpy(pCurSetData,pTemp,m_PulseSet.m_data.GetSize());
					CurPulseSet.m_setNum = m_PulseSet.m_setNum;
					timeRange = pCurSetData->m_timeRange;
					vRange[0] = pCurSetData->m_vRange[0];
					vRange[1] = pCurSetData->m_vRange[1];
					vRange[2] = pCurSetData->m_vRange[2];
					vRange[3] = pCurSetData->m_vRange[3];
					CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
					pMainFrame->fileName.Format("%s\\wave\\%s.bac",dataPath,m_nCurNo);
					int nTotal=0;
					if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
					{
						nTotal = pMainFrame->mWaveFile.GetLength()/waveLen/4;
						pMainFrame->mWaveFile.Close();
					}
					m_Spin1.SetRange(1,nTotal);
					m_Total.Format("%d",nTotal);
					CurPulseSet.m_setNum = nTotal;
				}

			}
		}
		m_PulseSet2.MoveNext();
		i++;
		}
		if(bInit)
		{
			m_PulseSet2.MoveFirst();
			for(i=0;i<mcurNo2;i++)
				m_PulseSet2.MoveNext();
		}

	}
	if(bInit)
	if(m_Radio1==1)
	{
		m_Num=1;
		OnButton1();
		m_Total.Format("%d",m_PulseSet2.m_setNum);
	}
	}

}
void CWaveView::OnInitialUpdate1() 
{
	CFormView::OnInitialUpdate();
	m_Spin1.SetRange(1,1000);	
	m_Spin1.SetPos(1);
	bInit=1;
	if(controlCom.nState!=1)
	{
		OpenDatabase();
		m_PulseSet.m_data.RemoveAll();
		m_PulseSet.Close();
	}
	bInit=0;
	UpdateData(FALSE);
}
void CWaveView::OnInitialUpdate2() 
{

	m_Spin1.SetRange(1,1000);	
	m_Spin1.SetPos(1);
	bInit=1;
	if(controlCom.nState!=1)
	{
		OpenBackDatabase();
		m_PulseSet2.Close();
	}
	bInit=0;
	UpdateData(FALSE);

}
void CWaveView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ModifyStyle(0, WS_SIZEBOX);

	m_Spin1.SetRange(1,1000);	
	m_Spin1.SetPos(1);
	bInit=1;
	if(controlCom.nState!=1&&bHasSaved==1)
	{
		OpenBackDatabase();
		OpenDatabase();
		m_PulseSet.m_data.RemoveAll();
		m_PulseSet.Close();
		m_PulseSet2.m_data.RemoveAll();
		m_PulseSet2.Close();
	}
	bInit=0;
	mEnable();
/*	
	CRect rect1,rect2;
	this->GetParent()->GetClientRect(&rect1);
	m_pDlgBar->GetClientRect(&rect2);
	CWnd *pWndCtrl;
	pWndCtrl=this->GetDlgItem(IDC_COMBO_NO);
*/
	UpdateData(FALSE);

}

void CWaveView::mEnable()
{
	if(controlCom.nState == 1)
	{
		m_Button1.EnableWindow(0);
		m_ComboNo.EnableWindow(0);
		m_ComboNo2.EnableWindow(0);
		m_Spin1.EnableWindow(0);
		m_Edit1.EnableWindow(0);
		m_Radio.EnableWindow(0);
		GetDlgItem(IDC_RADIO2)->EnableWindow(0);
	}
	else
	{
		m_Button1.EnableWindow(1);
		m_ComboNo.EnableWindow(1);
		m_ComboNo2.EnableWindow(1);
		m_Spin1.EnableWindow(1);
		m_Edit1.EnableWindow(1);
		m_Radio.EnableWindow(1);
		GetDlgItem(IDC_RADIO2)->EnableWindow(1);
	}
}
void CWaveView::OnButton1()
{
	CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
	if(bHasSaved==0)
	{
		pMainFrame->fileName.Format("%s\\current.tmp",curPath);
	}
	else
	{
		if(m_Radio1==0)
			pMainFrame->fileName.Format("%s\\wave\\%s.sam",dataPath,m_nCurNo);
		else
			pMainFrame->fileName.Format("%s\\wave\\%s.bac",dataPath,m_nCurNo);
	}
	if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
	{
		int pos=m_Spin1.GetPos();
		pos=CurPulseSet.m_setNum;
		if(pos>CurPulseSet.m_setNum)
		{
			pos = CurPulseSet.m_setNum;
			m_Spin1.SetPos(pos);
		}
		pMainFrame->mWaveFile.Seek((pos-1)*waveLen*4,CFile::begin);
		for(int i=0;i<4;i++)
			pMainFrame->mWaveFile.Read(GPibBuffer[i]+data0Len,waveLen);
		pMainFrame->mWaveFile.Close();
		if(bStateBar&1)
		{
			CString str;

			str.Format("%d",pos);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_CUR_NUM,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_Yield[nDetectorSel]);
			str=str.Left(6)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_YIELD,str);
			str.Format("%7.3f",(pCurSetData+pos-1)->m_area[nDetectorSel]);
			if(nDetectorSel==3)
			{
				float mdata=0;
				for(int j=0;j<9;j++)
					mdata+=(pCurSetData+pos-1)->m_area[4+i];

				str.Format("%7.3f",mdata);
			}
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_AREA,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_VacuumData);
			str=str.Left(5)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_VACUUM,str);
			str.Format("%6.2f",(pCurSetData+pos-1)->m_TargetV);
			m_pDlgBarCurrent->SetDlgItemText(IDC_THIS_TARGET_V,str);
			int i;
			float area9[12];
			area9[9]=0;
			for(i=0;i<9;i++)
			{
				area9[i]=(pCurSetData+pos-1)->m_area[4+i];
				area9[9]+=area9[i];
			}
			str.Format("1:%5.0f        %5.0f        %5.0f        %5.0f        5:%5.0f     %5.0f      %5.0f      %5.0f   %5.0f      total:%8.0f",area9[0],
				area9[1],area9[2],area9[3],area9[4],area9[5],area9[6],area9[7],area9[8],area9[9]);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_9BF3_COUNT,str);
		}
		Invalidate();
	}
}
void  CWaveView::displayCurState()
{
		int pos=controlCom.nCurNum;
		if(bStateBar&1)
		{
			CString str;

			str.Format("%d",pos);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_CUR_NUM,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_Yield[nDetectorSel]);
			str=str.Left(6)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_YIELD,str);
			str.Format("%7.3f",(pCurSetData+pos-1)->m_area[nDetectorSel]);
			if(nDetectorSel==3)
			{
				float mdata=0;
				for(int j=0;j<9;j++)
					mdata+=(pCurSetData+pos-1)->m_area[4+j];

				str.Format("%7.3f",mdata);
			}
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_AREA,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_VacuumData);
			str=str.Left(5)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_VACUUM,str);
			str.Format("%6.2f",(pCurSetData+pos-1)->m_TargetV);
			m_pDlgBarCurrent->SetDlgItemText(IDC_THIS_TARGET_V,str);
			int i;
			float area9[12];
			area9[9]=0;
			for(i=0;i<9;i++)
			{
				area9[i]=(pCurSetData+pos-1)->m_area[4+i];
				area9[9]+=area9[i];
			}
			str.Format("1:%5.0f        %5.0f        %5.0f        %5.0f        5:%5.0f     %5.0f      %5.0f      %5.0f   %5.0f      total:%8.0f",area9[0],
				area9[1],area9[2],area9[3],area9[4],area9[5],area9[6],area9[7],area9[8],area9[9]);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_9BF3_COUNT,str);
		}
}
void CWaveView::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int pos = GetDlgItemInt(IDC_EDIT_NUM);
	int i;
	if (pNMUpDown->iDelta < 0)
		pos = pos-1;
	else
		pos = pos+1;
	if(pos<1)
		pos = 1;
	if(pos>CurPulseSet.m_setNum)
		pos = CurPulseSet.m_setNum;

	CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
	if(bHasSaved==0)
	{
		pMainFrame->fileName.Format("%s\\current.tmp",curPath);
	}
	else
	{
		if(m_Radio1==0)
			pMainFrame->fileName.Format("%s\\wave\\%s.sam",dataPath,m_nCurNo);
		else
			pMainFrame->fileName.Format("%s\\wave\\%s.bac",dataPath,m_nCurNo);
	}
	if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
	{
		pMainFrame->mWaveFile.Seek((pos-1)*waveLen*4,CFile::begin);
		for(i=0;i<4;i++)
		{
		if(pMainFrame->mWaveFile.Read(GPibBuffer[i]+data0Len,waveLen)!=waveLen)
			memset(GPibBuffer[i]+data0Len,128,waveLen);
		}
		pMainFrame->mWaveFile.Close();
	}
		for(i=0;i<4;i++)
			yOffset[i]=(pCurSetData+pos-1)->m_ChOffset[i];
		(pCurSetData+pos-1)->m_PulseWidth[0]=GetFwhm(1);
		(pCurSetData+pos-1)->m_PulseWidth[1]=GetFwhm(2);
		(pCurSetData+pos-1)->m_area[0]=GetAreas(1);
		(pCurSetData+pos-1)->m_area[1]=GetAreas(2);
		CString str;
	if(bStateBar&1)
	{
			str.Format("%d",pos);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_CUR_NUM,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_Yield[nDetectorSel]);
			str=str.Left(6)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_YIELD,str);
			str.Format("%7.3f",(pCurSetData+pos-1)->m_area[nDetectorSel]);
			if(nDetectorSel==3)
			{
				float mdata=0;
				for(int j=0;j<9;j++)
					mdata+=(pCurSetData+pos-1)->m_area[4+j];

				str.Format("%7.3f",mdata);
			}
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_AREA,str);
			str.Format("%8.2e",(pCurSetData+pos-1)->m_VacuumData);
			str=str.Left(6)+str.Right(1);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_THIS_VACUUM,str);
			str.Format("%6.2f",(pCurSetData+pos-1)->m_TargetV);
			m_pDlgBarCurrent->SetDlgItemText(IDC_THIS_TARGET_V,str);
			int i;
			float area9[12];
			area9[9]=0;
			for(i=0;i<10;i++)
			{
				area9[i]=(pCurSetData+pos-1)->m_area[4+i];
				area9[9]+=area9[i];
			}
			str.Format("1:%5.0f        %5.0f        %5.0f        %5.0f        %5.0f        6:%5.0f     %5.0f      %5.0f      %5.0f   %5.0f      total:%8.0f",area9[0],
				area9[1],area9[2],area9[3],area9[4],area9[5],area9[6],area9[7],area9[8],area9[9],area9[10]);
			m_pDlgBarCurrent->SetDlgItemText(IDC_STATIC_9BF3_COUNT,str);

	}
		if(bStateBar&4)
		{
			str.Format("%6.3f",(pCurSetData+pos-1)->m_PulseWidth[0]);
			m_pDlgBarStart->SetDlgItemText(IDC_STATIC_FWHM1,str);
			str.Format("%6.3f",(pCurSetData+pos-1)->m_PulseWidth[1]);
			m_pDlgBarStart->SetDlgItemText(IDC_STATIC_FWHM2,str);
			str.Format("%6.3f",GetFwhm(3));
			m_pDlgBarStart->SetDlgItemText(IDC_STATIC_FWHM3,str);
		}
	DispCursor();
	Invalidate();
	*pResult = 0;
}

BOOL CWaveView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;
	CWinApp* pApp = AfxGetApp();

	if (TRUE)
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			{
				if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						return FALSE;
			}

			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				if (pInfo->m_pPD->CreatePrinterDC() == NULL)
					return FALSE;
			}
		}

		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	}
	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;
	
	LPDEVNAMES ptrdevnames = (LPDEVNAMES) GlobalLock (pInfo->m_pPD->m_pd.hDevNames);
	LPDEVMODE  ptrdevmode  = (LPDEVMODE)  GlobalLock (pInfo->m_pPD->m_pd.hDevMode);
	CString driver ((LPSTR) ptrdevnames + ptrdevnames->wDriverOffset);
	CString device ((LPSTR) ptrdevnames + ptrdevnames->wDeviceOffset);
	CString output ((LPSTR) ptrdevnames + ptrdevnames->wOutputOffset);
	ptrdevmode->dmOrientation = 2;
	ptrdevmode->dmPaperSize = 256;
	ptrdevmode->dmPaperLength = int(29.70*100) ;
	ptrdevmode->dmPaperWidth = int(21.0 * 100 );
	GlobalUnlock (ptrdevmode);
	GlobalUnlock (ptrdevnames);
	pInfo->m_pPD->m_pd.hDC=CreateDC (driver, device, output, ptrdevmode);
	if (pInfo->m_pPD->m_pd.hDC == NULL) return FALSE;

	pInfo->m_nNumPreviewPages = pApp->m_nNumPreviewPages;
	VERIFY(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	pInfo->SetMaxPage(1);
	return TRUE;
}


void CWaveView::OnSelchangeComboNo() 
{
	int n = m_ComboNo.GetCurSel();
	OpenDatabase();
	m_PulseSet.MoveFirst();
	for(int i=0;i<n;i++)
		m_PulseSet.MoveNext();
	memcpy(pCurSetData,m_PulseSet.m_data.GetData(),2048*sizeof(ScurSetData));
	timeRange = pCurSetData->m_timeRange;
	vRange[0] = pCurSetData->m_vRange[0];
	vRange[1] = pCurSetData->m_vRange[1];
	vRange[2] = pCurSetData->m_vRange[2];
	vRange[3] = pCurSetData->m_vRange[3];
	CurPulseSet.m_setNum = m_PulseSet.m_setNum;
	strcpy(m_nCurNo,m_PulseSet.m_No.GetBuffer(18));
	m_PulseSet.m_data.RemoveAll();
	m_PulseSet.Close();

	CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
	pMainFrame->fileName.Format("%s\\wave\\%s.sam",dataPath,m_nCurNo);
	long nTotal=0;
	if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
	{
		nTotal = pMainFrame->mWaveFile.GetLength()/waveLen/4;
		pMainFrame->mWaveFile.Close();
	}
	m_Spin1.SetRange(1,nTotal);
	m_Total.Format("%d",nTotal);
	CurPulseSet.m_setNum = nTotal;
	m_Radio1=0;
	UpdateData(FALSE);
}

void CWaveView::OnCh1On() 
{
	UpdateData(TRUE);
	Invalidate();	
}

void CWaveView::OnCh2On() 
{	
	UpdateData(TRUE);
	Invalidate();	
	
}

void CWaveView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
//	CString str;
//	str.Format("±àºÅ£º%s",m_nCurNo);
//	pDC->TextOut(150*kx,350*ky,str);
	CFormView::OnPrint(pDC, pInfo);
}

void CWaveView::OnSelchangeComboNo2() 
{
	int n = m_ComboNo2.GetCurSel();
	OpenBackDatabase();
	m_PulseSet2.MoveFirst();
	for(int i=0;i<n;i++)
		m_PulseSet2.MoveNext();
	CurPulseSet.m_setNum = m_PulseSet2.m_setNum;

	memcpy(pCurSetData,m_PulseSet2.m_data.GetData(),2048*sizeof(ScurSetData));
	timeRange = pCurSetData->m_timeRange;
	vRange[0] = pCurSetData->m_vRange[0];
	vRange[1] = pCurSetData->m_vRange[1];
	vRange[2] = pCurSetData->m_vRange[2];
	vRange[3] = pCurSetData->m_vRange[3];
	strcpy(m_nCurNo,m_PulseSet2.m_No.GetBuffer(18));
	m_PulseSet2.Close();
	CMainFrame * pMainFrame = ((CMainFrame *)(::AfxGetMainWnd()));
	pMainFrame->fileName.Format("%s\\wave\\%s.bac",dataPath,m_nCurNo);
	int nTotal=0;
	if(pMainFrame->mWaveFile.Open(pMainFrame->fileName.GetBuffer(80),CFile::modeRead))
	{
		nTotal = pMainFrame->mWaveFile.GetLength()/waveLen/4;
		pMainFrame->mWaveFile.Close();
	}
	m_Spin1.SetRange(1,nTotal);
	m_Total.Format("%d",nTotal);
	CurPulseSet.m_setNum = nTotal;
	m_Radio1=1;
	UpdateData(FALSE);
}

void CWaveView::OnRadio1() 
{
	OnSelchangeComboNo();
}

void CWaveView::OnRadio2() 
{
	OnSelchangeComboNo2();
}

void CWaveView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	if(pInfo->m_bPreview)
	{
		bPrintPreview=0;
		m_pDlgBar->ShowWindow(SW_SHOW);
	}
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CWaveView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	CRect rect1,rect2;
	GetParent()->GetClientRect(&rect1);
	m_pDlgBar->GetClientRect(&rect2);
	int xw=rect1.right-rect2.right;
	
	// TODO: Add your message handler code here
	CWnd *pWndCtrl;
    CRect rectCtrl;
	int i;
	pWndCtrl=GetDlgItem(IDC_RADIO1);
    if ((NULL != pWndCtrl) && IsWindow(pWndCtrl->GetSafeHwnd()))
	{
		pWndCtrl->GetWindowRect(rectCtrl);
		ScreenToClient(rectCtrl);
		int shift=xw-rectCtrl.right-20;
		int width=rectCtrl.Width();
		rectCtrl.right+=shift;
		rectCtrl.left+=shift;
		pWndCtrl->MoveWindow(&rectCtrl);
		for(i=0;i<14;i++)
		{
			pWndCtrl=GetDlgItem(IDC_RADIO1+1+i);
			pWndCtrl->GetWindowRect(rectCtrl);
			ScreenToClient(rectCtrl);
			rectCtrl.right+=shift;
			rectCtrl.left+=shift;
			pWndCtrl->MoveWindow(&rectCtrl);
		}
	}

}

void CWaveView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnLButtonDown(nFlags, point);
	curX0=curX;
	int	xTextWidth=80,yTextWidth=50;
	unsigned char *GBuffer=GPibBuffer[0]+data0Len;
	unsigned char *GBuffer2=GPibBuffer[1]+data0Len;
	int	xRange = nScrWidth-xTextWidth;
	int	yRange = nScrHeight-yTextWidth;
	int xOrg = nScrOrgX+xTextWidth;
	int yOrg = int(nScrOrgY+10);
	int i;
	if((point.x>nScrOrgX)&&(point.x<nScrWidth*waveLen/xRange))
	{
	curX=point.x;
	cursorTime=(curX-xOrg)*waveLen/xRange;
	DispCursor();
	}

}

void CWaveView::OnCh3On() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Invalidate();	
	
}

void CWaveView::OnCh4On() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Invalidate();	
	
}

void CWaveView::DispCursor() 
{
	int	xTextWidth=80,yTextWidth=50;
	int	xRange = nScrWidth-xTextWidth;
	int	yRange = nScrHeight-yTextWidth;
	int xOrg = nScrOrgX+xTextWidth;
	int yOrg = int(nScrOrgY+10);
	int i;
	if(cursorTime<0)
		cursorTime=0;
//	if(cursorTime>
	for(i=0;i<4;i++)
	{
		cursorData[i] = yOrg+(256-*(GPibBuffer[i]+data0Len+cursorTime))/256.*yRange;
		cursorData[i] = *(GPibBuffer[i]+data0Len+cursorTime);
		cursorChV[i]= (cursorData[i]-yOffset[i])/256.*10*vRange[i];

	}
	CString str;
	str.Format("%d",cursorTime);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CUR_NUM,str.GetBuffer(6));
	if(mIsTDS==0)
		str.Format("%5.2f %s",cursorTime*timeRangeDisp/100./2.5,timeRangeUnit);
	else
		str.Format("%5.2f %s",cursorTime*timeRangeDisp/100.,timeRangeUnit);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CUR_TIME,str.GetBuffer(10));
	str.Format("%5.2f V",cursorChV[0]);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CH1_V,str.GetBuffer(10));
	str.Format("%5.2f V",cursorChV[1]);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CH2_V,str.GetBuffer(10));
	str.Format("%5.2f V",cursorChV[2]);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CH3_V,str.GetBuffer(10));
	str.Format("%5.2f V",cursorChV[3]);
	m_pDlgBarWave->SetDlgItemText(IDC_STATIC_CH4_V,str.GetBuffer(10));

	CRect rect;
	if(curX>curX0)
	{
	rect.left=curX0-5;
	rect.right=curX+5;
	}
	else
	{
	rect.left=curX-5;
	rect.right=curX0+5;
	}
	rect.top=yOrg;
	rect.bottom=yOrg+yRange;
	InvalidateRect(&rect);
}

BOOL CWaveView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	 if(pMsg->message   ==   WM_KEYDOWN)   
     {   
		curX0=curX;
		int	xTextWidth=80,yTextWidth=50;
		unsigned char *GBuffer=GPibBuffer[0]+data0Len;
		unsigned char *GBuffer2=GPibBuffer[1]+data0Len;
		int	xRange = nScrWidth-xTextWidth;
		int	yRange = nScrHeight-yTextWidth;
		int xOrg = nScrOrgX+xTextWidth;
		int yOrg = int(nScrOrgY+10);
		switch(pMsg->wParam)
		{
		case VK_RIGHT:
			cursorTime++;
			if(cursorTime>waveLen-1)
				cursorTime=waveLen-1;
			curX=cursorTime*xRange/(float)waveLen+xOrg;
			DispCursor();
			break;
		case VK_LEFT:
			cursorTime--;
			if(cursorTime<1)
				cursorTime=0;
			curX=cursorTime*xRange/(float)waveLen+xOrg;
			DispCursor();
			break;
		case VK_PRIOR:
 			cursorTime+=50;
			if(cursorTime>waveLen-1)
				cursorTime=waveLen-1;
			curX=cursorTime*xRange/(float)waveLen+xOrg;
			DispCursor();
			break;
		case VK_NEXT:
			cursorTime-=50;
			if(cursorTime<0)
				cursorTime=0;
			curX=cursorTime*xRange/(float)waveLen+xOrg;
			DispCursor();
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_ADD:
			break;
		case VK_SUBTRACT:
			break;
		case 76:
			break;
		case VK_HOME:
			Invalidate(0);
			break;
		case VK_END:
			Invalidate(0);
			break;
		case VK_TAB:
			break;
		case VK_F1:
			break;
		case VK_F2:
			break;
		case VK_F3:
			break;
		default:
			break;
		}
	 }
	return CFormView::PreTranslateMessage(pMsg);
}
