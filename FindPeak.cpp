#include "stdafx.h"
#include "stdio.h"
#include <math.h>
extern	int		MaxChannel;

void findPeak(long *y,int H,int w,float factor)
{
 int long *pp;
 double  *yymax;
 double *ss;
 unsigned char *spm;
 double *ssmax,*ssmaxe;
 char *f1;
 char n1[10];
 int long f2,f3;
 float A;
 int i,j,k=0,l,n,m;
 double X[50],Y[50],Y0[50],*W,F[250],FT[250],
	       C[250],D[25],E[250],G[5],d[5];
 double x,y1,a1=0,a2=0,b,eps=0.01,z,am=0,T1,T2,T3,T4,Area,Areas=0;
 double b1=0,c,db,da=0,ue,uf=0,de,df=0,sdf;
 int t;
	int peakl[100];
	int peakr[100];
	int peakm[100];
	yymax= new double [8200];
	ssmax= new double[8200];
	ssmaxe= new double[8200];
	ss= new double[8200];

   m=(w-1)/2;
   A=(float)(-m);
   for(i=0;i<w;i++)
    {b1=A/H;
     c=-2.7*(b1*b1);
     db=exp(c);
	 da=da+db;
     A++;}
     da=da/(double)w;
   for(j=m;j<MaxChannel-m;j++)
   { ss[j]=0;
     yymax[j]=0;
     df=0;
     uf=0;
     A=(float)(-m);
     for(i=0;i<w;i++)
      {b1=A/H;
       c=-2.7*(b1*b1);
       db=exp(c);
	 ue=(db-da)*y[(int)(j+A)];
	 uf=uf+ue;
	 de=(db-da)*(db-da)*y[(int)(j+A)];
       df=df+de;
       A++;
      }
     sdf=sqrt(df);
     ss[j]=uf/sdf;
     yymax[j]=uf;
   }
    for(j=0;j<MaxChannel;j++)
     {
      if(ss[j]>factor)
			ssmax[j]=ss[j];
      else ssmax[j]=0;
     }
    for(j=4;j<MaxChannel-4;j++)
     {if(ssmax[j]>=ssmax[j+1]&&ssmax[j]>ssmax[j+2]&&ssmax[j]>ssmax[j+3]&&ssmax[j]>ssmax[j+4]&&ssmax[j]>=ssmax[j-1]&&ssmax[j]>ssmax[j-2]&&ssmax[j]>ssmax[j-3]&&ssmax[j]>ss[j-4])	
           ssmaxe[j]=ss[j];
      else ssmaxe[j]=0;
     }
	int peaknum=0;
    for(j=w;j<MaxChannel-w;j++)
     {
		peakl[peaknum]=0;
		peakr[peaknum]=0;
		if(ssmaxe[j]!=0)
		{
			peakm[peaknum]=j;
			for(i=j;(yymax[i-1]<yymax[i])||(yymax[i-2]<yymax[i-1])||(yymax[i-3]<yymax[i-2]);i--)
			{
				peakl[peaknum]=peakl[peaknum]+1;
			}
			for(i=j;(yymax[i+1]<yymax[i])||(yymax[i+2]<yymax[i+1])||(yymax[i+3]<yymax[i+2]);i++)
			{
				peakr[peaknum]=peakr[peaknum]+1;
			}
			peaknum++;
		}
		if(peaknum>49)
			break;
      }
/*
	printf("                        Find the peaks  \n ");
	printf("  -----------------------------------------------------------------\n");
	printf("         n   |    yymax     |    ss     |    peakl   |   peakr     \n");
	printf("   -----------------------------------------------------------------\n");
   for(j=4;j<MaxChannel-4;j++)
      {if(ssmaxe[j]!=0)
	{
	  printf("      %4d     %8.2f      %8.3f      %4d        %4d\n",j,yymax[j],ss[j],peakl[j],peakr[j]);
	  printf("   -----------------------------------------------------------------\n");
    
    }
   }
*/
	delete [] yymax;
	delete [] ssmax;
	delete [] ssmaxe;
	delete [] ss;

}