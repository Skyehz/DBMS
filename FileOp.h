#pragma once
#include<vector>
using namespace std;
class FileOp
{
public:
	static vector<CString> ReadAll(CString& fileName);	//������ȡȫ���ļ�������
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//��������¼һ����д���ļ�


	bool WriteLog(CString& oper);	//д��־�ļ�
};

