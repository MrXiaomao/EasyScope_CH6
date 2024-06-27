// Function.h: interface for the CFunction class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FUNCTION_H__920DC780_30EC_11D6_B52A_001088023E64__INCLUDED_)
#define AFX_FUNCTION_H__920DC780_30EC_11D6_B52A_001088023E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFunction  
{
public:
	CFunction();
	virtual ~CFunction();
	int		CalMain();  // 最小二乘法主程序
	void	DisplayHead(CDC *pDC,int *nTabPosition);
	int     adcNum;
	double	*pX,*pY,*pYSgm,*pYout;
	double	dbA[5],dbB[5],dbASgm[5],dbBSgm[5];
	double	dbA2[5],dbB2[5],dbC2[5],dbA2Sgm[5],dbB2Sgm[5],dbC2Sgm[5];
	double	SK1, SK2;  // SK1 = pV[i]求和　SK2 = pV2[i]求和
	double	RR1;
	double	acm0,acm0Sgm,acm1,acm1Sgm;
    double  beff0,geff0,beff1,geff1;
	double	kx,ky;
	int		nStarty;
	double	para[5],paraSgm[5];
	int		nDimension;//拟合参数个数
private:
	double	yTime,bTime; //yTime= 样品测量时间　bTime = 本底测量时间
	double	*pMatrix;   // Matrix * A = Matriy
	double	*pMatriy;
	double	*pw;        // 权重
	int		nDouble;   // =0( py[i]=n1[i] ) 
					   // =1( py[i]=n2[i] )
					   // =2( py[i]=nc[i] )
					   // =3( py[i]=z2[i] )
					   // =4( py[i]=fva[i] )
	int		nMaxData;  //x,y数据对个数
	double	*pA,*pASgm; //拟合参数和误差
	int		CalMatrix(double *pMatrix, double *pMatriy);
	int		CalMatrix_1(); //求矩阵逆
	int		CalA();      // 求拟合参数
	int		CalASgm();	// 求拟合参数误差
	void	CurveFit2();
public:
//	void	Display()
};

#endif // !defined(AFX_FUNCTION_H__920DC780_30EC_11D6_B52A_001088023E64__INCLUDED_)
