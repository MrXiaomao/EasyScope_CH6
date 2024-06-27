// Function.cpp: implementation of the CFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Function.h"
#include "mcbc.h"
#include <math.h>
#include <memory.h> 
#include "define.h"
extern	CEnergyKedu energyKedu[4];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunction::CFunction()
{
	pMatrix = new double[100];
	pMatriy = new double[100];
//	pX = new double[100];
	pYout = new double[100];
	pw = new double[100*sizeof(double)];  //权重
	pA = para;
	pASgm = paraSgm;
	nDimension = 0;
	nMaxData = 0;
}

CFunction::~CFunction()
{
	delete [] pMatrix;
	delete [] pMatriy;
	delete [] pYout;
	delete [] pw;

}

int CFunction::CalMatrix(double *pMatrix, double *pMatriy)
{
	int i,i1,i2,j,k;
	double spw;
	double xk;

	if(nDimension<1) return -1;
	if(nMaxData<=nDimension)return -1;
	spw = 0;
/*
	for(i=0;i<nMaxData;i++)
	{
		*(pw+i) = 1./(*(pYSgm+i)*(*(pYSgm+i)));
		spw += *(pw+i);
	}
*/
	for(i=0;i<nMaxData;i++)
	{
//		*(pw+i) = *(pw+i)/spw;
		*(pw+i) = 1;
	}

	for(i1=0;i1<nDimension;i1++)
	{
		for(i2=0;i2<nDimension;i2++)
		{
			*(pMatrix+i1*nDimension+i2) = 0;
			for(j=0;j<nMaxData;j++)
			{
				xk=1;
				for(k=0;k<i1+i2;k++)
					xk=xk*(*(pX+j));
				*(pMatrix+i1*nDimension+i2) += *(pw+j) * xk;
			}
		}
	}
	for(i=0;i<nDimension;i++)
	{
		*(pMatriy+i) = 0;
		for(j=0;j<nMaxData;j++)
		{
			xk=1;
			for(k=0;k<i;k++)
				xk=xk*(*(pX+j));
			*(pMatriy+i) += *(pw+j) * xk *(*(pY+j));
		}
	}
	return 1;
}

int CFunction::CalMatrix_1()
{
	int i,j,k,m;
	double w,g;
//	double b[200];
	double *b;
	if(nDimension<1)
		return -2;
	b = new double[nDimension*sizeof(double)];
	for(k=0;k<nDimension;k++)
	{
		w=*pMatrix;
		if(fabs(w)==0)
		{
			delete [] b;
			return -2;
		}
		m = nDimension-k-1;
		for(i=1;i<nDimension;i++)
		{
			g=*(pMatrix+i*nDimension);
			*(b+i) = g/w;
			if(i<=m)
				*(b+i)=-*(b+i);
			for(j=1;j<=i;j++)
				*(pMatrix+(i-1)*nDimension+j-1) = 
				*(pMatrix+i*nDimension+j) + g*(*(b+j));
		}
		*(pMatrix+nDimension*nDimension-1) = 1./w;
		for(i=1;i<nDimension;i++)
			*(pMatrix+(nDimension-1)*nDimension+i-1) = *(b+i);
	}
	for(i=0;i<nDimension-1;i++)
		for(j=i+1;j<nDimension;j++)
			*(pMatrix+i*nDimension+j) =  *(pMatrix+j*nDimension+i);
	delete [] b;

	return 2;
}
int CFunction::CalA()
{
	int i,j;
	if(nDimension<1)
		return -2;
	for(i=0;i<nDimension;i++)
	{
		*(pA+i)=0;
		for(j=0;j<nDimension;j++)
		{
		*(pA+i) += *(pMatrix+i*nDimension+j)*
			(*(pMatriy+j));
		}
	}
	return 1;
}
int CFunction::CalASgm()
{
	int i;
	double	deltay;
	double	sgmy;
	if(nMaxData<=nDimension)return -1;
	double xx[20];
	double pyy,rrr,syy,uuu;

	for(i=0;i<nMaxData;i++)
	{
		xx[0]=1;
		 *(pYout+i)= *pA;
		for(int j=1;j<nDimension;j++)
		{
			xx[j] = xx[j-1]*(*(pX+i));
			*(pYout+i) += *(pA+j)*xx[j];
		}
	}
	sgmy = 0;
	pyy=0;
	for(i=0;i<nMaxData;i++)
	{
		deltay = *(pY+i)-*(pYout+i);
		pyy+=*(pY+i);
		sgmy += *(pw+i) * deltay * deltay;
	}
	pyy=pyy/nMaxData;
	syy=0;
	uuu=0;
	for(i=0;i<nMaxData;i++)
	{
		syy+=*(pw+i)*(*(pY+i)-pyy)*(*(pY+i)-pyy);
		uuu+=*(pw+i)*(*(pYout+i)-pyy)*(*(pYout+i)-pyy);
	}
	if(nDimension==2)
	{
		SK1 = sgmy;
	}
	if(nDimension==3)
	{
		SK2 = sgmy;
	}
	RR1=sqrt(uuu/syy);
	sgmy = sqrt(sgmy/(nMaxData-nDimension));
	for(i=0;i<nDimension;i++)
	{
		*(pASgm+i) = sgmy*sqrt(*(pMatrix+i*nDimension+i));
	}
	return 1;
}


int CFunction::CalMain()
{
//	nDimension = 2;
	nDouble = 0;
	nMaxData = energyKedu[adcNum].Enum;
	if(nMaxData<nDimension)
		return -1;
	if(nMaxData==nDimension)
	{
		para[0]=pY[0]-(pY[1]-pY[0])/(pX[1]-pX[0])*pX[0];
		para[1]=(pY[1]-pY[0])/(pX[1]-pX[0]);
		dbA[0] = para[0];
		dbB[0] = para[1];
		return 1;
	}
	CalMatrix(pMatrix, pMatriy);
	CalMatrix_1();
	CalA();
	CalASgm();
	dbA[0] = para[0];
	dbB[0] = para[1];
/*
	dbASgm[0] = paraSgm[0];
	dbBSgm[0] = paraSgm[1];

	nDimension = 3;
	if(nMaxData<=nDimension)return -1;
	CalMatrix(pMatrix, pMatriy);
	CalMatrix_1();
	CalA();
	CalASgm();
	dbA2[0] = para[0];
	dbB2[0] = para[1];
	dbC2[0] = para[2];
	dbA2Sgm[0] = paraSgm[0];
	dbB2Sgm[0] = paraSgm[1];
	dbC2Sgm[0] = paraSgm[2];
*/
//	CurveFit2();
	return 1;
}

void CFunction::DisplayHead(CDC *pDC,int *nTabPosition)
{
	int		tx0, tx1, tx2, tx3, tx4;
	CString str0,str1;
	tx0 = nTabPosition[0];
	tx1 = int(nTabPosition[1]*.5);
	tx2 = nTabPosition[3];
	tx3 = int(nTabPosition[1]*5.2);
	tx4 = int(nTabPosition[7]*1.05);
	pDC->SetTextAlign(TA_LEFT);
/*
	str0.Format("样品数据(%s)",strMethod[mSetData.m_nCoinMethod-1]);
	pDC->TextOut(tx2, int((nStarty)*ky),str0);
//	if(!pDC->IsPrinting())
	{
		pDC->MoveTo(tx0,int((nStarty+25)*ky));
		pDC->LineTo(tx4,int((nStarty+25)*ky));
	}
	str0.Format("测量编号: %s",mSetData.m_CerNo);
	pDC->TextOut(tx1, int((nStarty+30)*ky),str0);
	str0.Format("核素名称: %s",mSetData.m_DUT);
	pDC->TextOut(tx2, int((nStarty+30)*ky),str0);
	str0.Format("源编号: %s",mSetData.m_SSourceNo);
	pDC->TextOut(tx3, int((nStarty+30)*ky),str0);

	str0.Format("样品质量: %6.2f %s",mSetData.m_M, strMUnit[mSetData.m_mUnit]);
	pDC->TextOut(tx1, int((nStarty+60)*ky),str0);
	str0.Format("稀释倍数: %6.2f",mSetData.m_DF);
	pDC->TextOut(tx2, int((nStarty+60)*ky),str0);
	str0.Format("半衰期: %8.3e %s",mSetData.m_T12, strT12Unit[mSetData.m_T12Unit]);
	pDC->TextOut(tx3, int((nStarty+60)*ky),str0);

	str0.Format("X轴系数(BG): %6.2f",mSetData.m_XFactor);
	pDC->TextOut(tx1, int((nStarty+90)*ky),str0);
	str0.Format("本底编号: %s",mSetData.m_BendiNo);
	pDC->TextOut(tx2, int((nStarty+90)*ky),str0);
	str0.Format("测量日期: %s",mSetData.m_CalDate);
	pDC->TextOut(tx3, int((nStarty+90)*ky),str0);

	str0.Format("β死时间: %6.2f μs",mSetData.m_dtBeta);
	pDC->TextOut(tx1, int((nStarty+120)*ky),str0);
	str0.Format("γ死时间: %6.2f μs",mSetData.m_dtGamma);
	pDC->TextOut(tx2, int((nStarty+120)*ky),str0);
	str0.Format("符合死时间: %6.2f μs",mSetData.m_dtCoin);
	pDC->TextOut(tx3, int((nStarty+120)*ky),str0);

//	if(!pDC->IsPrinting())
	{
		pDC->MoveTo(tx0,int((nStarty+145)*ky));
		pDC->LineTo(tx4, int((nStarty+145)*ky));
	}
	str0.Format("示踪核素: %s",mSetData.m_SNuclide);
	pDC->TextOut(tx1, int((nStarty+150)*ky),str0);
	str0.Format("比活度: %6.2f",mSetData.m_SAC);
	pDC->TextOut(tx2, int((nStarty+150)*ky),str0);
	str0.Format("质  量: %6.2f %s",mSetData.m_SM, strMUnit[mSetData.m_SMUnit]);
	pDC->TextOut(tx3, int((nStarty+150)*ky),str0);
	str0.Format("半衰期: %8.3e %s",mSetData.m_ST12,strT12Unit[mSetData.m_ST12Unit]);
	pDC->TextOut(tx1, int((nStarty+180)*ky),str0);
*/
//	if(!pDC->IsPrinting())
	{
		pDC->MoveTo(tx0,int((nStarty+205)*ky));
		pDC->LineTo(tx4,int((nStarty+205)*ky));
	}
}
void CFunction::CurveFit2()
{
	double sp, x0,x2,x3,x4;
	double	py,xy,px2y;
	int	i;
	sp = 0;
	x0=0;	x2=0;	x3=0;	x4=0;
	py=0;	xy=0;   px2y=0;
	for(i=0;i<nMaxData;i++)
	{
		sp += *(pw+i);
		x0 += *(pw+i)*(*(pX+i));
		x2 += *(pw+i)*(*(pX+i))*(*(pX+i));
		x3 += *(pw+i)*(*(pX+i))*(*(pX+i))*(*(pX+i));
		x4 += *(pw+i)*(*(pX+i))*(*(pX+i))*(*(pX+i))*(*(pX+i));
		py +=  *(pw+i)*(*(pY+i));
		xy += *(pw+i)*(*(pX+i))*(*(pY+i));
		px2y += *(pw+i)*(*(pX+i))*(*(pX+i))*(*(pY+i));
	}
	double d = sp*x2 - x0*x0;
	double delta;
	dbA[0] = (x2 * py - x0 * xy) / d;
	dbB[0] = (sp * xy - x0 * py) / d;
	SK1 = 0;
	for(i=0;i<nMaxData;i++)
	{
		delta = *(pY+i) - (dbA[0] + dbB[0]*(*(pX+i)));
		SK1 += *(pw+i) * delta * delta;
	}
	double sgm0 = sqrt(SK1/(nMaxData-2));
	dbASgm[0] = sgm0 * sqrt(x2 / d);
	dbBSgm[0] = sgm0 * sqrt(sp / d);
	double d2,na,nb,nc;
	d2 = sp * x2 * x4 + 2 * x0 * x2 * x3 - x2 * x2 * x2 - 
		x0 * x0 * x4 - x3 * x3 * sp;
	na = py * x2 * x4 + xy *x3 * x2 + px2y * x3 * x0 -
		x2 * x2 * px2y - x0 * xy * x4 - py * x3 * x3;
	nb = sp * xy * x4 + x0 * px2y * x2 + x2 * x3 * py -
		x2 * x2 * xy - py * x0 * x4 - sp * x3 * px2y;
	nc = sp * x2 * px2y + x0 * x3 * py + x2 * xy * x0 -
		py * x2 * x2 - x0 * x0 * px2y - sp * x3 * xy;
	dbA2[0] = na / d2;
	dbB2[0] = nb / d2;
	dbC2[0] = nc / d2;
	SK2 = 0;
	for(i=0;i<nMaxData;i++)
	{
		delta = *(pY+i) - (dbA2[0] + dbB2[0]*(*(pX+i))
			+dbC2[0]*(*(pX+i))*(*(pX+i)));
		SK2 += *(pw+i) * delta * delta;
	}
	sgm0 = sqrt(SK2/(nMaxData-3));
	double sgm2 = sgm0 * sgm0 / d2;
	dbA2Sgm[0] = sqrt(sgm2* (x2 * x4 - x3 * x3));
	dbB2Sgm[0] = sqrt(sgm2* (sp * x4 - x2 * x2));
	dbC2Sgm[0] = sqrt(sgm2* (sp * x2 - x0 * x0));
}

