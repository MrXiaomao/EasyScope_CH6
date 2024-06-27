#ifndef SPMPARA
#define SPMPARA
struct SpmPara
{
	int Horz;
	int Vort;
	int SpmH,SpmW;
	int StartChn;
	BOOL IsLog;
};
#endif
#ifndef DISPSPM
#define DISPSPM
class DispSpm
{
	public:
		DispSpm();
		~DispSpm();
		SpmPara spara;
		bool paintbz,largebz;
		short *DispBuf;
		short *OldBuf;
		int CursorX,CursorChn,CursorX0;
		void ReadyData(long *pdata);
		void DispData(CDC *pDC,int wid,int viewNum,CPen &mforePen,CPen &redPen);
		void ClearData(CDC *pDC);
		void DispCursor(CDC *pDC,bool cl,long *pdata);
		void DispLabel(CDC *pDC);
		void DispLabelX(CDC *pDC);
		double kh,kx,ky;
		double cursorY;
  };
#endif
