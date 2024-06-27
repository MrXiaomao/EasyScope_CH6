// mcbcDoc.cpp : implementation of the CMcbcDoc class
//

#include "stdafx.h"
#include "mcbc.h"

#include "mcbcDoc.h"
#include "mcbcView.h"
#include "define.h"
#include "ChildFrm.h"
#include "DlgStrip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
long	*pAdcBuf[8];
extern	CMcbcView	*w1[4];
extern	int		MaxChannel;
extern	BOOL	transbz[4];
extern CChildFrame *m_pChildWnd[4];
int			docID=0;
char	strFileTxt[4]="chn";
extern  CDialogBar *m_pDlgBarTime[2];
extern	CDialogBar *m_pDlgBarState[4];
extern	long	*pMemBuf[8];
extern	long		*pDispAdcBuf[8];
char month[12][4]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
extern	CEnergyKedu energyKedu[4];
extern	CDialogBar *m_pDlgBar;
extern  int		bDispMca[8];
extern	McsSetData	mcsSetData[4];
extern	bool	bDispBuffer;
extern	CTime bufStartTime[4],hardSatrtTime[4];
extern	StripData stripData;

/////////////////////////////////////////////////////////////////////////////
// CMcbcDoc

IMPLEMENT_DYNCREATE(CMcbcDoc, CDocument)

BEGIN_MESSAGE_MAP(CMcbcDoc, CDocument)
	//{{AFX_MSG_MAP(CMcbcDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_TEXT, OnFileSaveText)
	ON_COMMAND(ID_FILE_OPEN_TEXT, OnFileOpenText)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_COMMAND(ID_FILE_LOAD_ALL, OnFileLoadAll)
	ON_COMMAND(ID_MENU_STRIP, OnMenuStrip)
	ON_COMMAND(ID_MENU_SAVE_CANSU, OnMenuSaveCansu)
	ON_COMMAND(ID_MENU_READ_CANSU, OnMenuReadCansu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMcbcDoc construction/destruction

CMcbcDoc::CMcbcDoc()
{
	nCurView = docID;
	fileName="";
	docID++;
}

CMcbcDoc::~CMcbcDoc()
{
}

BOOL CMcbcDoc::OnNewDocument()
{
//	memset(pAdcBuf[0],0,MaxChannel*4);
//	memset(pAdcBuf[1],0,MaxChannel*4);
//	memset(pAdcBuf[2],0,MaxChannel*4);
//	memset(pAdcBuf[3],0,MaxChannel*4);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMcbcDoc serialization

void CMcbcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMcbcDoc diagnostics

#ifdef _DEBUG
void CMcbcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMcbcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMcbcDoc commands

void CMcbcDoc::OnFileOpen() 
{
//	CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"二进制文件(*.chn)|*.chn|文本文件(*.txt)|*.txt|双通道二进制文件(*.two)|*.two||");
	CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"二进制文件(*.chn)|*.chn||");
	char curpath[50];
	int i,nMca;
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if (strcmp(strFileTxt,"chn")==0)
		dlg.m_ofn.nFilterIndex = 1;
	else if (strcmp(strFileTxt,"txt")==0)
		dlg.m_ofn.nFilterIndex = 2;
	else if (strcmp(strFileTxt,"two")==0)
		dlg.m_ofn.nFilterIndex = 3;
	else
		dlg.m_ofn.nFilterIndex = 1;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		strcpy(strFileTxt,dlg.GetFileExt());
		if((strcmp(strFileTxt,"chn")==0)||(strcmp(strFileTxt,"CHN")==0)||(strcmp(strFileTxt,"Chn")==0))
		{
			int spmLen=w1[nCurView]->dispspm.spara.Horz;
			if(mfile.Open(fileName.GetBuffer(50),CFile::modeRead))
			{
				spmLen=(mfile.GetLength()-32-512)/8;
//				bDispBuffer=1;
				mfile.Read(&chnHead,32);
				mfile.Read((void *)pAdcBuf[0],UINT(spmLen*4));
				mfile.Read((void *)pAdcBuf[1],UINT(spmLen*4));
				mfile.Read(&chnEnd,512);
				mfile.Close();
				fileName = dlg.GetFileName();
				SetTitle("");
				w1[nCurView]->curTime=chnEnd.startTime;
				bufStartTime[nCurView]=chnEnd.startTime;

				pMemBuf[nCurView][1]=chnHead.realTime/50.;
				pMemBuf[nCurView][2]=chnHead.liveTime/50.;
				pDispAdcBuf[nCurView][1]=chnHead.realTime/50.;
				pDispAdcBuf[nCurView][2]=chnHead.liveTime/50.;
				nMca=0;
				for(i=0;i<1;i++)
				{
//					pDispAdcBuf[i]=pMemBuf[i];
				}
			}
		}
		else if(strcmp(strFileTxt,"txt")==0)
		{
			mfile.Open(fileName.GetBuffer(50),CFile::modeRead);
			for(int i=0;i<MaxChannel;i++)
			{
				mfile.Read(str.GetBuffer(30),18);
				*(pAdcBuf[nCurView]+i)=atoi(str.GetBuffer(18)+8);
			}
			mfile.Close();
		}
	}
	if(MaxChannel<8192)
	{
		for(int i=MaxChannel;i<8192;i++)
		{
			*(pAdcBuf[nCurView]+i)=0;
		}
	}
	w1[0]->Invalidate();
}

void CMcbcDoc::OnFileSave() 
{
	int i,nMca;
	if(bDispMca[0])
		nMca=0;
	else
		nMca=4;
	chnHead.head[0]=0xff;
	chnHead.head[1]=0xff;
	chnHead.head[2]=0;
	chnHead.head[3]=0;
	chnHead.head[4]=0;
	chnHead.head[5]=0;
	chnHead.startSecond[0]=w1[nCurView]->curTime.GetSecond()/10+0x30;
	chnHead.startSecond[1]=w1[nCurView]->curTime.GetSecond()%10+0x30;
	chnHead.realTime=pDispAdcBuf[nCurView][1]*50;
	chnHead.liveTime=pDispAdcBuf[nCurView][2]*50;
	chnHead.day[0]=w1[nCurView]->curTime.GetDay()/10+0x30;
	chnHead.day[1]=w1[nCurView]->curTime.GetDay()%10+0x30;
	int mmm=w1[nCurView]->curTime.GetMonth();
	chnHead.month[0]=month[mmm][0];
	chnHead.month[1]=month[mmm][1];
	chnHead.month[2]=month[mmm][2];
	int yy=w1[nCurView]->curTime.GetYear();
	yy=yy%100;
	chnHead.year[0]=yy/10+0x30;
	chnHead.year[1]=yy%10+0x30;
	chnHead.bank=0x20;
	int hh=w1[nCurView]->curTime.GetHour();
	chnHead.hour[0]=hh/10+0x30;
	chnHead.hour[1]=hh%10+0x30;
	int mm=w1[nCurView]->curTime.GetMinute();
	chnHead.minute[0]=mm/10+0x30;
	chnHead.minute[1]=mm%10+0x30;
	chnHead.offset=0;
	chnHead.channel=MaxChannel;
	chnEnd.head[0]=0x9b;
	chnEnd.head[1]=0xff;
	chnEnd.head[2]=0x64;
	chnEnd.head[3]=0x21;
	chnEnd.enZero=energyKedu[nCurView].para[0];
	chnEnd.enSlope=energyKedu[nCurView].para[1];
	chnEnd.fwhmZero=0;
	chnEnd.fwhmSlope=0;
	chnEnd.startTime=w1[nCurView]->curTime;
	memset(chnEnd.reserved,0,488);
//	CFileDialog dlg(FALSE,".chn",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"二进制文件(*.chn)|*.chn|文本文件(*.txt)|*.txt|双通道二进制文件(*.two)|*.two||");
	CFileDialog dlg(FALSE,".chn",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"二进制文件(*.chn)|*.chn|文本文件(*.txt)|*.txt||");
	char curpath[50];
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if (strcmp(strFileTxt,"chn")==0)
		dlg.m_ofn.nFilterIndex = 1;
	else if (strcmp(strFileTxt,"txt")==0)
		dlg.m_ofn.nFilterIndex = 2;
	else if (strcmp(strFileTxt,"two")==0)
		dlg.m_ofn.nFilterIndex = 3;
	else
		dlg.m_ofn.nFilterIndex = 1;
	int a=0x20;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		strcpy(strFileTxt,dlg.GetFileExt());
		if((strcmp(strFileTxt,"chn")==0)||(strcmp(strFileTxt,"CHN")==0))
		{
			int spmLen=w1[nCurView]->dispspm.spara.Horz;
			mfile.Open(fileName.GetBuffer(50),CFile::modeCreate|CFile::modeWrite);
			mfile.Write(&chnHead,32);
/*			if(bDispBuffer)
			{
				mfile.Write((void *)pMemBuf[0],UINT(spmLen*4));
				mfile.Write((void *)pMemBuf[1],UINT(spmLen*4));
			}
			else*/
			{
				mfile.Write((void *)pAdcBuf[0],UINT(spmLen*4));
				mfile.Write((void *)pAdcBuf[1],UINT(spmLen*4));
			}
			mfile.Write(&chnEnd,512);
			mfile.Close();
			SetTitle("");
		}
		else if((strcmp(strFileTxt,"txt")==0)||(strcmp(strFileTxt,"TXT")==0))
		{
			int i;
			mfile.Open(fileName.GetBuffer(50),CFile::modeCreate|CFile::modeWrite);
			if(bDispBuffer)
			{
				for(i=0;i<MaxChannel;i++)
				{
					str.Format("%8d%8d\xd\n",i,*(pMemBuf[nCurView]+i));
					mfile.Write(str.GetBuffer(20),str.GetLength());
				}
			}
			else
			{
				for(i=0;i<MaxChannel;i++)
				{
					str.Format("%8d%8d\xd\n",i,*(pAdcBuf[nCurView]+i));
					mfile.Write(str.GetBuffer(20),str.GetLength());
				}
			}
			mfile.Close();
		}
	}
}

void CMcbcDoc::OnFileSaveText() 
{
	CFileDialog dlg(FALSE,".txt",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt||");
	char curpath[50];
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	int a=0x20;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		CString str;
		mfile.Open(fileName.GetBuffer(50),CFile::modeCreate|CFile::modeWrite);
		for(int i=0;i<MaxChannel;i++)
		{
			str.Format("%8d%8d\xd\n",*(pAdcBuf[nCurView]+i),i);
			mfile.Write(str.GetBuffer(20),str.GetLength());
		}
		mfile.Close();
	}
}

void CMcbcDoc::OnFileOpenText() 
{
	CFileDialog dlg(TRUE,".txt",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt||");
	char curpath[50];
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	int a=0x20;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		CString str;
		mfile.Open(fileName.GetBuffer(50),CFile::modeRead);
		for(int i=0;i<MaxChannel;i++)
		{
			mfile.Read(str.GetBuffer(30),18);
			*(pAdcBuf[nCurView]+i)=atoi(str);
		}
		mfile.Close();
	}
}

void CMcbcDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString str,str1,str2;
/*	
	if(w1[nCurView])
	{
		if(w1[nCurView]->bStart)
			str1 = "测量状态";
		else
			str1 = "停止状态";
	}
	else
		str1 = "停止状态";
	if(transbz[nCurView])
		str2="允许传输";
	else
		str2= "禁止传输";
	str2="";
*/
	str.Format("%s",fileName);
//	str.Format("%s",fileName);
	CDocument::SetTitle(str);
//	CDocument::SetTitle(lpszTitle);
}


void CMcbcDoc::OnFileLoadAll() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,NULL,NULL,OFN_READONLY,"4通道文件(*.4s)|*.4s||");
	char curpath[50];
	int	i;
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		mfile.Open(fileName.GetBuffer(50),CFile::modeRead);
		bDispBuffer=1;
		if(bDispBuffer)
		{
			
			for(i=0;i<1;i++)
			{
				mfile.Read(&(w1[i]->curTime),sizeof(CTime));
				bufStartTime[i]=w1[i]->curTime;
				mfile.Read((void *)pMemBuf[i],UINT(MaxChannel*4));
			}
			for(i=4;i<8;i++)
				mfile.Read((void *)pMemBuf[i],UINT(8192*4));
		}
/*
		else
		{
			for(i=0;i<4;i++)
			{
				mfile.Read(&(w1[i]->curTime),sizeof(CTime));
				mfile.Read((void *)pAdcBuf[i],UINT(MaxChannel*4));
			}
			for(i=4;i<8;i++)
				mfile.Read((void *)pAdcBuf[i],UINT(8192*4));
		}
*/
		for(i=0;i<1;i++)
		{
			mfile.Read(&w1[i]->RoiNum,sizeof(w1[i]->RoiNum));
			mfile.Read(w1[i]->RoiStart,sizeof(w1[i]->RoiStart));
			mfile.Read(w1[i]->RoiEnd,sizeof(w1[i]->RoiEnd));
		}
		mfile.Read(mcsSetData,sizeof(mcsSetData));
		mfile.Read(energyKedu,sizeof(energyKedu));
		mfile.Close();
		int nMca;
		if(bDispMca[0])
			nMca=0;
		else
			nMca=4;
		for(i=0;i<1;i++)
		{
			pDispAdcBuf[i]=pMemBuf[nMca+i];
		}
//		((CButton *)(m_pDlgBar->GetDlgItem(IDC_RADIO_ONE2)))->SetCheck(0);
//		((CButton *)(m_pDlgBar->GetDlgItem(IDC_RADIO_TWO2)))->SetCheck(1);

		CString sdate;
		CString stime;
		for(i=0;i<1;i++)
		{
			sdate.Format("%02d.%02d.%02d",w1[i]->curTime.GetYear()-2000,
				w1[i]->curTime.GetMonth(),w1[i]->curTime.GetDay());
			m_pDlgBar->SetDlgItemText(IDC_STATIC_DATE2,sdate.GetBuffer(20));
			stime= w1[i]->curTime.Format("%X");
			m_pDlgBar->SetDlgItemText(IDC_STATIC_TIME2,stime.GetBuffer(20));
			((CMcbcDoc *)w1[i]->GetDocument())->fileName=fileName;
			w1[i]->Invalidate();
		}

	}
	
}

void CMcbcDoc::OnFileSaveAll() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,".4s",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"4通道文件(*.4s)|*.4s||");
	char curpath[50];
	int	i;
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		mfile.Open(fileName.GetBuffer(50),CFile::modeCreate|CFile::modeWrite);
		if(bDispBuffer)
		{
			
			for(i=0;i<1;i++)
			{
				mfile.Write(&(w1[i]->curTime),sizeof(CTime));
				mfile.Write((void *)pMemBuf[i],UINT(MaxChannel*4));
			}
//			for(i=4;i<8;i++)
//				mfile.Write((void *)pMemBuf[i],UINT(8192*4));
		}
		else
		{
			for(i=0;i<1;i++)
			{
				mfile.Write(&(w1[i]->curTime),sizeof(CTime));
				mfile.Write((void *)pAdcBuf[i],UINT(MaxChannel*4));
			}
//			for(i=4;i<8;i++)
//				mfile.Write((void *)pAdcBuf[i],UINT(8192*4));
		}
		for(i=0;i<1;i++)
		{
			mfile.Write(&w1[i]->RoiNum,sizeof(w1[i]->RoiNum));
			mfile.Write(w1[i]->RoiStart,sizeof(w1[i]->RoiStart));
			mfile.Write(w1[i]->RoiEnd,sizeof(w1[i]->RoiEnd));
			((CMcbcDoc *)w1[i]->GetDocument())->fileName=fileName;
		}
		mfile.Write(mcsSetData,sizeof(mcsSetData));
		mfile.Write(energyKedu,sizeof(energyKedu));
		mfile.Close();
	}
	
}

void CMcbcDoc::OnMenuStrip() 
{
	// TODO: Add your command handler code here
	CDlgStrip dlg;
	if(dlg.DoModal()==IDOK)
	{
		char temp[200];
		float radio1;
		sprintf(temp,"%s\\%s",dlg.m_Directory.GetBuffer(100),dlg.m_PathName.GetBuffer(100));
		fileName=temp;
		CFile mfile;
		mfile.Open(fileName.GetBuffer(200),CFile::modeRead);
		{
			CTime tempTime;
			long *pTempBuf[8];
			int i,j;
			for(i=0;i<1;i++)
			{
				pTempBuf[i]= new long[8200];
			}
			int nStripAdc=stripData.nAdc;
			int spmLen=(mfile.GetLength()-32-512)/4;
			mfile.Read(&chnHead,32);
			mfile.Read((void *)pTempBuf[nStripAdc],UINT(spmLen*4));
			mfile.Read(&chnEnd,512);
			if(!dlg.m_Check1)
				radio1=dlg.m_Radio1;
			else
			{
				if(pTempBuf[nStripAdc][1]>1)
					radio1=pMemBuf[nStripAdc][1]/pTempBuf[nStripAdc][1];
				else
					radio1=1;
				
			}
			for(i=0;i<spmLen;i++)
			{
				pMemBuf[nStripAdc][i]-=pTempBuf[nStripAdc][i]*radio1;
				if(pMemBuf[nStripAdc][i]<0)
					pMemBuf[nStripAdc][i]=0;
			}
			for(i=0;i<1;i++)
			{
				delete []pTempBuf[i];
			}
			for(i=0;i<1;i++)
			{
				pDispAdcBuf[i]=pMemBuf[i];
			}

			for(i=0;i<1;i++)
			{
				w1[i]->Invalidate();
			}
		}
		mfile.Close();
	}
}

void CMcbcDoc::OnMenuSaveCansu() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,".ren",NULL,OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,"加亮区和能量刻度参数(*.ren)|*.ren||");
	char curpath[50];
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		mfile.Open(fileName.GetBuffer(50),CFile::modeCreate|CFile::modeWrite);
		mfile.Write(&energyKedu[nCurView],sizeof(CEnergyKedu));
		mfile.Write(&w1[nCurView]->RoiNum,sizeof(int));
		mfile.Write(w1[nCurView]->RoiStart,sizeof(int)*15);
		mfile.Write(w1[nCurView]->RoiEnd,sizeof(int)*15);
		mfile.Close();
	}
}

void CMcbcDoc::OnMenuReadCansu() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_READONLY,"加亮区和能量刻度参数(*.ren)|*.ren||");
	char curpath[50];
	int i,nMca;
	CString str;
	::GetCurrentDirectory(50,curpath);
	dlg.m_ofn.lpstrInitialDir=curpath;
	if(dlg.DoModal()==IDOK)
	{
		fileName = dlg.GetPathName();
		CFile mfile;
		if(mfile.Open(fileName.GetBuffer(50),CFile::modeRead))
		{
				mfile.Read(&energyKedu[nCurView],sizeof(CEnergyKedu));
				mfile.Read(&w1[nCurView]->RoiNum,sizeof(int));
				mfile.Read(w1[nCurView]->RoiStart,sizeof(int)*15);
				mfile.Read(w1[nCurView]->RoiEnd,sizeof(int)*15);
				mfile.Close();
		}
	}
	
}


