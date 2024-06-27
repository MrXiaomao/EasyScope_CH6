#ifndef CENERGY
#define CENERGY
struct CEnergyKedu
{
	int Enum;
	double channel[20];
	double	energy[20];
	int fitMode;
	double para[3];
};
struct CEnergyKedu1
{
	int Enum;
	double channel[20];
	double	energy[20];
	int fitMode;
	double para[3];
};
struct ChnHead
{
	unsigned char head[6];
	char startSecond[2];
	int  realTime;
	int  liveTime;
	unsigned char day[2];
	unsigned char month[3];
	unsigned char year[2];
	char bank;
	char hour[2];
	char minute[2];
	short  offset;
	short channel;
};
struct ChnEnd
{
	unsigned char head[4];
	float   enZero;
	float   enSlope;
	float   fwhmZero;
	float   fwhmSlope;
	CTime	startTime;
	char	reserved[488];
};

#endif

#ifndef MCSSETDATA
#define MCSSETDATA
struct McsSetData
{
	int		mcsStarCh;
	int		mcsEndCh;
	int		mcsTime;
	int		mcsAreaOrGross;
};
#endif

#ifndef STRIP
#define STRIP
struct StripData
{
	bool oneOrFour;
	int nAdc;
	bool Check1;
	float m_Radio1;
};
#endif

#ifndef AUTODATA
#define AUTODATA
struct CAutoData
{
	int		m_GapTime;
	int		m_MeasureTime;
	int		m_Times;
	char	m_No[100];
	bool	bStart;
	int		m_CurTimes;
	int	    m_curMeasTime;
	int		m_curGapTime;
	bool	bcurStart;
};
#endif

#ifndef F8051DATA
#define F8051DATA
struct C51DATA
{
	unsigned char	m_AlarmPaper; //是否走纸故障报警  1  是  0 否
	unsigned char	m_AlarmFlux;//是否流量故障报警
	unsigned char	m_AlarmBeta;//是否Beta计数越限报警
	unsigned char	m_AlarmAlpha;//是否Alpha计数越限报警
	unsigned short	m_Alpha1;//报警下限
	unsigned short	m_Alpha2;//报警上限
	unsigned short	m_Beta1;//报警下限
	unsigned short 	m_Beta2;//报警上限
	unsigned short	m_Flux1;//报警下限
	unsigned short	m_Flux2;//报警上限
	unsigned short	m_Paper1;//报警周期
	unsigned short	m_Rn220H;//上阈
	unsigned short	m_Rn220L;//下阈
	unsigned short	m_Rn222H;//上阈
	unsigned short	m_Rn222L;//下阈
	unsigned short	m_Pu239H;//上阈
	unsigned short	m_Pu239L;//下阈
	unsigned short	m_U235H;//上阈
	unsigned short	m_U235L;//下阈
	unsigned short	m_BetaH;//上阈
	unsigned short	m_BetaL;//下阈
	unsigned short	m_Pu239S1;
	unsigned short	m_Pu239S2;
	unsigned short	m_Pu239S3;
	unsigned short	m_Pu239S4;
	unsigned short	m_Pu239S5;
	unsigned short	m_Rn220S1;
	unsigned short	m_Rn220S2;
	unsigned short	m_Rn220S3;
	unsigned short	m_Rn220S4;
	unsigned short	m_Rn220S5;
	unsigned short	m_Rn222S1;
	unsigned short	m_Rn222S2;
	unsigned short	m_Rn222S3;
	unsigned short	m_Rn222S4;
	unsigned short	m_Rn222S5;
	unsigned short	m_U235S1;
	unsigned short	m_U235S2;
	unsigned short	m_U235S3;
	unsigned short	m_U235S4;
	unsigned short	m_U235S5;
	unsigned short	m_Address;
	unsigned short	m_Period;
	unsigned short	baudrate;
	float	m_BendiS1;
	float	m_BendiS2;
	float	m_BendiS3;
	float	m_BendiS4;
	float	m_BendiS5;
	float	m_BetaEff;
	unsigned char beteColor;
	unsigned char UColor;
	unsigned char PuColor;
	unsigned char RnColor;
	unsigned char ThColor;
	float	m_BetaAcc;
	char	m_Rn220File[200];
	char	m_Rn222File[200];
	char	m_U235File[200];
	char	m_Pu239File[200];
	char	m_BetaFile[200];
	char	m_BendiFile[200];
};
#endif

#ifndef CGUSSDATA
#define CGUSSDATA
struct CGuassData
{
	int startCh;
	int endCh;
	bool m_Use[10];
	bool m_CheckX[10];
	bool m_CheckSgm[10];
	double m_X0[10];
	double m_Sgm[10];
	double m_Area[10];
	double m_AreaSgm[10];
};
#endif
