#pragma once
#include<vector>
using namespace std;
class FileOp
{
public:
	static CString GetCurrTime();		//��ȡ��ǰϵͳʱ��
	static vector<CString> ReadAll(CString& fileName);	//������ȡȫ���ļ�������
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//��������¼һ����д���ļ�
	static bool WriteLog(CString &info, CString &fileName);		//д��־�ļ�
	static bool AddAnLine(CString& fileName, CString& str);	//���ļ�ĩβ����һ����¼
};

