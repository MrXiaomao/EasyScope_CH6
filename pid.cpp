#include "stdafx.h"
//#include <avr/io.h>
//#include "PID.h"
//static PID sPID;
//static PID *sptr=&sPID;
//TADD Tadd1;
//TADD Tadd2;
typedef struct
{
 float SetPoint;   //设定目标值
 float SumError; //误差累计
 
 //int Proportion;
 //int Integral; //积分常数
 //int Derivative; //微分常数
 
 float LastError;  //Error[-1]
 float PrevError;  //Error[-2]
}PID;
#define PID_Proportion 900    //比例常数
#define PID_Integral 20  //积分常数
#define PID_Derivative 2  //微分常数
#define PWM_T 100
#define MAX_T 80   //加热的最大温度 
typedef struct
{
 PID spid; //PID控制器
 unsigned char pwm_H;//输出
 unsigned char EnAdd;//加热使能
 float real_T;   //实际温度值
 unsigned char Taddset[3]; //加热的设定温度
 unsigned char set_NO;   //加热的档数
 unsigned char errorflg;
 unsigned char addokflg;
}TADD;

void IncPIDInit(PID *sptr)
{
 sptr->SumError=0;
 sptr->LastError=0;
 sptr->PrevError=0;
 
 //sptr->Proportion = 900;
 //sptr->Integral=20;
 //sptr->Derivative = 2;
 
 //sptr->SetPoint = 0;
}

 


void LocPIDCalc(TADD *sptr)
{
 float iError,dError;
 float result;
 iError = (sptr->spid.SetPoint*4) - (sptr->real_T/4);
 //sptr->spid.LastError = iError;
 if(iError>-2&&iError<4)
 {
  sptr->addokflg=1;
  sptr->spid.SumError +=iError;
  dError = iError-sptr->spid.LastError;
  sptr->spid.LastError = iError;
  result=(PID_Proportion * iError +
   PID_Integral * sptr->spid.SumError +
   PID_Derivative * dError)/20;
 }
 else
 {
  sptr->spid.SumError =0;
  sptr->spid.LastError=0;
  if(iError>0)
   sptr->addokflg=0;
  result=(PID_Proportion * iError)/20;
 }
 if(result>PWM_T)
  result=PWM_T;
 else if(result<0)
  result=0;
 if((sptr->real_T*16)>MAX_T||sptr->errorflg==0)
  result=0;
 sptr->pwm_H=(unsigned char) result;
 //return (unsigned char) result;
}

 