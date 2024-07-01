#pragma once

using namespace std;

class CLog
{
public:
	CLog(void);
	~CLog(void);

	//-��־�ļ�-
	//---���û��ָ������ΪEXE����·���µ�LOG\\EXEName.log	
	static short SetLogFile(LPCTSTR strPath=NULL);

	//-ǰ׺-
	//---������������ͬһ���ļ������־������Ϊÿ����������һ��ǰ׺---
	//---ǰ׺����������ʱ��֮ǰ---
	static short SetPrefix(CString strPrefix);

	//��ȡ������Ϣ���������ƺ��кţ�
	static short GetDugInfo(LPCTSTR pDugInfo=NULL);

	//�½��ļ�
	static void CreateNewFile();

	//-��־��Ϣ-
	//---д����־��Ϣ---
	static short  WriteMsg(CString strFormat = "");

	static short WriteDugMsg(CString strFormat);

protected:
	static CString s_errorInfo;
	static CString s_strLogFile;
	static CString s_strLogPrefix;
	static HANDLE  s_hWriteEvent;
};
