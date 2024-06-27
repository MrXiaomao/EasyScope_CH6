#include "stdafx.h"
#include <math.h>
#include "define.h"
#include "mcbc.h"
#include "DlgGuass.h"
extern	long		*pDispAdcBuf[8];
extern CGuassData mGuassData;
extern CDlgGuass * pDlgGuassFit;
struct GaussPara
{
	double H,x0,sgm;
};
GaussPara *gaussPara;
void GetYSubY0(double *X, double *Y,int n,GaussPara *gaussPara ,int nPeak,double b1,double b2,double b3,double *Y0,double nStart)
{
	int i,j;
	double x1,x2,x3,x4;
	for(i=0;i<n;i++)
	{
		x3=0;
		for(j=0;j<nPeak;j++)
		{
		x1=X[i]-gaussPara[j].x0;
		x2=x1/gaussPara[j].sgm;
		x3+=gaussPara[j].H*exp(-x2*x2/2.);
		}
		x4=b1+b2*(X[i]-nStart)+b3*(X[i]-nStart)*(X[i]-nStart);
		Y0[i]=x3+x4;
		Y0[i]=Y[i]-Y0[i];
	 }
}

void GetMatF(double *F,int n,int m,GaussPara *gaussPara ,int nPeak,double *X,double nStart)
{double x1,x2,x3,x4,x5,x6,x7,x8;
 int i,j,k1;
 for(i=0;i<n;i++)
 {
	 F[i*m+0]=1;
	 F[i*m+1]=X[i]-nStart;
	 F[i*m+2]=(X[i]-nStart)*(X[i]-nStart);
	 k1=3;
	 for(j=0;j<nPeak;j++)
	 {

		 x1=X[i]-gaussPara[j].x0;
		 x2=gaussPara[j].sgm;
		 x3=x1/x2;
		F[i*m+k1++]=exp(-x3*x3/2.);
		 if(!mGuassData.m_CheckX[j])
			F[i*m+k1++]=x3/x2*gaussPara[j].H*exp(-x3*x3/2.);
		 if(!mGuassData.m_CheckSgm[j])
			 F[i*m+k1++]=x3*x3/x2*gaussPara[j].H*exp(-x3*x3/2.);
	 }
 }
}

void GetMATTra(double *F,int n,int m,double *FT)
{int i,j;
 for(i=0;i<n;i++)
 {
  for(j=0;j<m;j++)
  FT[j*n+i]=F[i*m+j];
 }
}

void MATMUL(double *A,double *B,int n,int m,int k,double *C)
{int i,j,l,u;
 for(i=0;i<n;i++)
  {
   for(j=0;j<k;j++)
    {
     u=i*k+j;
     C[u]=0.0;
     for(l=0;l<m;l++)
     C[u]=C[u]+A[i*m+l]*B[l*k+j];
    }
  }
}

void MATW(double *Y,int n,double *W)
{int i,j;
 for(i=0;i<n;i++)
  {for(j=0;j<n;j++)
   W[i*n+j]=0.0;
  }
 for(i=0;i<n;i++)
	 if(Y[i]==0)
		 W[i*n+i]=1;
	 else
		W[i*n+i]=1/Y[i];
  return;
}


int dcinv(double *a,int n)
{int *is,*js,i,j,k,l,u,v;
 double d,p;
 is=new int[n*sizeof(int)];
 js=new int[n*sizeof(int)];
 for(k=0;k<=n-1;k++)
  {d=0.0;
   for(i=k;i<=n-1;i++)
   for(j=k;j<=n-1;j++)
    {l=i*n+j;
     p=fabs(a[l]);
     if(p>d)
      {d=p;
       is[k]=i;
       js[k]=j;
      }
    }
   if(d+1.0==1.0)
    {
		delete [] is;
		delete [] js;
     printf("err**not inv\n");
     return(0);
    }
   if(is[k]!=k)
    for(j=0;j<=n-1;j++)
     {u=k*n+j;
      v=is[k]*n+j;
      p=a[u];
      a[u]=a[v];
      a[v]=p;
     }
   if(js[k]!=k)
    for(i=0;i<=n-1;i++)
     {u=i*n+k;
      v=i*n+js[k];
      p=a[u];
      a[u]=a[v];
      a[v]=p;
     }
  l=k*n+k;
  a[l]=1.0/a[l];
  for(j=0;j<=n-1;j++)
   if(j!=k)
    {u=k*n+j;
     a[u]=a[u]*a[l];
    }
  for(i=0;i<=n-1;i++)
  if(i!=k)
   for(j=0;j<=n-1;j++)
    if(j!=k)
     {u=i*n+j;
      a[u]=a[u]-a[i*n+k]*a[k*n+j];
     }
  for(i=0;i<=n-1;i++)
   if(i!=k)
    {u=i*n+k;
     a[u]=-a[u]*a[l];
    }
 }
  for(k=n-1;k>=0;k--)
   {if(js[k]!=k)
     for(j=0;j<=n-1;j++)
      {u=k*n+j;
       v=js[k]*n+j;
       p=a[u];
       a[u]=a[v];
       a[v]=p;
      }
    if(is[k]!=k)
     for(i=0;i<=n-1;i++)
      {u=i*n+k;
       v=i*n+is[k];
       p=a[u];
       a[u]=a[v];
       a[v]=p;
      }
   }
   delete [] is;
   delete [] js;
   return(1);
}
void fitGass()
{
	int i,j,n,m,k,k2;
	double *X1,*Y1,*Y0,*F,*FT,*W,*C,*E;
	double H=1000;
	double x0=800;
	double sgm=100;
	double b1=1,b2=0,b3=0;
	double am=0,eps=0.001;
	double D[1000],d[12],G[12];
	double z,RX0,RX1,alpha;
	int nPeak;
	int nStart=mGuassData.startCh;
	int nx1;
	int kindex;

	nPeak=0;
	for(i=0;i<5;i++)
		nPeak+=mGuassData.m_Use[i];
	GaussPara gaussPara[10]={{1000,1312,10},{1000,1493,10},{360,1640,10}};
	gaussPara[0].H=1000;
	m=0;
	for(i=0;i<nPeak;i++)
	{
		gaussPara[i].x0=mGuassData.m_X0[i];
		gaussPara[i].sgm=mGuassData.m_Sgm[i]/2.355;
		nx1=mGuassData.m_X0[i];
    	gaussPara[i].H=pDispAdcBuf[0][nx1];
		if(!mGuassData.m_CheckSgm[i])
			m++;
		if(!mGuassData.m_CheckX[i])
			m++;
		m++;

	}	
	n=mGuassData.endCh-mGuassData.startCh+1;
	m+=3;

	X1=new double[1000*8];
	Y1=new double[1000*8];
	Y0=new double[1000*8];
	F=new double[1000*8*33];
	FT=new double[1000*8*33];
	W=new double[1000*1000*33];
	C=new double[1000*8*33];
	E=new double[1000*8*33];

	for(i=0;i<n;i++)
	{
		X1[i]=nStart+i;
		Y1[i]=pDispAdcBuf[0][nStart+i];
	}
   for(i=0;i<m;i++)
	   d[i]=1.0;
	MATW(Y1,n,W);
	for(i=0;i<m;i++)
	{
		z=fabs(d[i]);
	    if(z>eps)
			am=z;
    }
	GetYSubY0(X1,Y1,n,gaussPara,nPeak,b1,b2,b3,Y0,nStart);
	alpha=0.3;
    for(k=1000;k>0;k--)
	{
	    if(am>=eps)
		 {
			am=0;
			GetMatF(F,n,m,gaussPara,nPeak,X1,nStart);
			GetMATTra(F,n,m,FT);
			MATMUL(FT,W,m,n,n,C);
			MATMUL(C,F,m,n,m,D);
			dcinv(D,m);
			MATMUL(D,FT,m,m,n,C);
			MATMUL(C,W,m,n,n,E);
			MATMUL(E,Y0,m,n,1,G);
			RX0=0;
			for(i=0;i<n;i++)
				RX0+=W[i*n+i]*Y0[i]*Y0[i];
			b1=b1+alpha*G[0];
			b2=b2+alpha*G[1];
			b3=b3+alpha*G[2];
			kindex=3;
			for(k2=0;k2<nPeak;k2++)
			{
				gaussPara[k2].H+=alpha*G[kindex++];
				if(!mGuassData.m_CheckX[k2])
					gaussPara[k2].x0+=alpha*G[kindex++];
				if(!mGuassData.m_CheckSgm[k2])
					gaussPara[k2].sgm+=alpha*G[kindex++];
			}
			GetYSubY0(X1,Y1,n,gaussPara,nPeak,b1,b2,b3,Y0,nStart);
			RX1=0;
			for(i=0;i<n;i++)
				RX1+=W[i*n+i]*Y0[i]*Y0[i];

/*
			if(RX1>RX0)
				alpha=1;
			else
				alpha=alpha/2.;
*/
			for(i=0;i<nPeak;i++)
			{
				mGuassData.m_X0[i]=gaussPara[i].x0;
				mGuassData.m_Sgm[i]=gaussPara[i].sgm*2.355;
				mGuassData.m_Area[i]=gaussPara[i].H*gaussPara[i].sgm*2.5068;
				mGuassData.m_AreaSgm[i]=sqrt(RX1/(n-m))*D[i*m+i]*2.5068;
			}
			pDlgGuassFit->UpdateFit();
			d[0]=G[0]/b1;
			d[1]=G[1]/b2;
			d[2]=G[2]/b3;
			kindex=3;
			for(k2=0;k2<nPeak;k2++)
			{
				d[kindex]=G[kindex++]/gaussPara[k2].H;
				if(!mGuassData.m_CheckX[k2])
					d[kindex]=G[kindex++]/gaussPara[k2].x0;
				if(!mGuassData.m_CheckSgm[k2])
					d[kindex]=G[kindex++]/gaussPara[k2].sgm;
			}
			for(i=0;i<m;i++)
			{
				z=fabs(d[i]);
				if(z>eps)
					am=z;
			}
		}
			else break;
	  }
	for(i=0;i<n;i++)
	{
		z=0;
		for(k2=0;k2<nPeak;k2++)
		{
			z+=gaussPara[k2].H*exp(-(X1[i]-gaussPara[k2].x0)*(X1[i]-gaussPara[k2].x0)
				/2./gaussPara[k2].sgm/gaussPara[k2].sgm);
		}
		pDispAdcBuf[4][nStart+i]=z+b1+b2*(X1[i]-nStart)+b3*(X1[i]-nStart)*(X1[i]-nStart);
	}
	delete [] X1;
	delete [] Y1;
	delete [] Y0;
	delete [] F;
	delete [] FT;
	delete [] W;
	delete [] C;
	delete [] E;

}