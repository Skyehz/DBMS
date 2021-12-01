#pragma once
#include<vector>
#include <string>
using namespace std;
class FileOp
{
public:
	static CString GetCurrTime();		//��ȡ��ǰϵͳʱ��
	static CString setOneline(CString& str);
	static vector<CString> StrSplit(CString str, CString split);
	static CString IntegerToString(int number);
	static int StringToInteger(CString cstring);
	static CString BoolToString(bool bl);
	static bool StringToBool(CString str);
	static CString GetTypeCString(int type);
	static int GetTypeInt(CString type);
	static CString semicolon(CString& str);//����ֺ�
	static CString getbrakets(CString& str);//��������е�����
	static CString getbeforebrakets(CString& str);//�������(ǰ������
	static bool paren(CString& str);//�ж�����ƥ��S



	static vector<CString> ReadAll(CString& fileName);	//������ȡȫ���ļ�������
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//��������¼һ����д���ļ�
	static bool WriteLog(CString &info, CString &fileName);		//д��־�ļ�
	static bool AddAnLine(CString& fileName, CString& str);	//���ļ�ĩβ����һ����¼
	static bool DeleteFolder(CString& folderName);	//ɾ���ļ���
	static bool ModifyFileName(CString& oldname, CString& newname);
	static CString FileOp::ReadFirstLine(CString& fileName);//��ȡ��һ���ļ�������
	static bool FileOp::SaveCounter(CString& filePath, int counter);
};

