#include "pch.h"
#include "FileOp.h"
//#include<string>
#include <stack>

//��ȡ��ǰϵͳʱ��
CString FileOp::GetCurrTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	return t.Format("%Y-%m-%d %H:%M:%S");
}

//�ָ��ַ���
vector<CString> FileOp::StrSplit(CString str, CString split)
{
	vector<CString> res;
	string::size_type index = 0;
	str += split;
	while (str.GetLength() != 0)
	{
		index = str.Find(split);
		res.push_back(str.Left(index));
		str.Delete(0, index + 1);
	}
	return res;
}

//int -> cstring
CString FileOp::IntegerToString(int number)
{
	if (number == NULL)
		return CString("0");
	else
	{
		char buff[1024];
		sprintf_s(buff, "%d", number);
		return CString(buff);
	}
}

int FileOp::StringToInteger(CString cstring)
{
	if (!cstring)
		return NULL;
	//���Unicode�����ת������
	USES_CONVERSION;
	return atoi(T2A(cstring));
}

CString FileOp::GetTypeCString(int type) {
	switch (type)
	{
	case 1:
		return CString("Integer");
	case 2:
		return CString("Bool");
	case 3:
		return CString("Double");
	case 4:
		return CString("Varchar");
	case 5:
		return CString("DateTime");
	default:
		return CString("δ֪");
	}
}
int FileOp::GetTypeInt(CString type) {
	if (type == CString("Integer")) {
		return 1;
	}
	else if (type == CString("Bool")) {
		return 2;
	}
	else if (type == CString("Double")) {
		return 3;
	}
	else if (type == CString("Varchar")) {
		return 4;
	}
	else if (type == CString("DateTime")) {
		return 5;
	}
	else {
		return 6;
	}
}


CString FileOp::semicolon(CString& str) {
	CString final = str.Right(1);
	int n = str.GetLength();
	if (final == CString(";")) {
		CString s = str.Left(n - 1);
		return s;

	}
	else {
		MessageBox(NULL, CString("SQL�������ԡ�������β"), CString("����"), MB_OK);
		return str;
	}
}

CString FileOp::getbrakets(CString& str) {
	int begin = str.Find(CString("("));
	int end = str.GetLength();
	CString str1 = str.Right(end - begin - 1);
	str1 = str1.Left(end - begin - 2);

	return str1;
}

CString FileOp::getbeforebrakets(CString& str) {
	int begin = str.Find(CString("("));
	CString str1 = str.Left(begin);
	return str1;
}

bool FileOp::paren(CString& str)
{
	int n = str.GetLength();
	char* s;
	s = (char*)str.GetBuffer(0);
	stack<char> pi;
	for (int i = 0; i < n; i++) {
		if (s[i] == '(')  pi.push(s[i]);
		else if (!pi.empty())  pi.pop();
		else return false;
	}
	return pi.empty();
}



//д��־�ļ�
bool FileOp::WriteLog(CString &info, CString &fileName)
{
	info += "\r\n";

	/*CString in = CString("created successful");*/
	CFile writefile;
	bool openWrite = writefile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	if (openWrite) {
		writefile.SeekToEnd();
		int num = info.GetLength() *2;
		writefile.Write(info, num);
		writefile.Close();
	}
	else
		return false;


	return true;

}

//��ȡ�ļ��е�ȫ������
vector<CString> FileOp::ReadAll(CString& fileName) {
	vector<CString> content;
	TCHAR con[256];
	ZeroMemory(con, sizeof(con));

	char len[4];
	//�Զ�������ʽ���ļ�
	CFile readfile;
	bool openRead = readfile.Open(fileName, CFile::modeRead | CFile::typeBinary);
	if (openRead) {

		while (readfile.Read(len, 4) != 0) {
			long num = atoi(len);
			readfile.Read(con, num);
			CString temp = con;
			content.push_back(temp);
			ZeroMemory(con, sizeof(con));
		}
		readfile.Close();
	}
	else
	{
		MessageBox(NULL, CString("���ļ�ʧ�ܣ�"), CString("����"), MB_OK | MB_ICONWARNING);
	}

	return content;
}

//��������¼д���ļ�
bool FileOp::WriteRecord(CString& fileName, vector<CString>& str) {
	bool res = true;
	CFile writefile;
	bool openWrite = writefile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary/*|CFile::modeNoTruncate*/);
	if (openWrite) {
		//writefile.SeekToEnd();
		for (int i = 0; i < str.size(); i++) {
			int num = str[i].GetLength() * 2;
			char len[4];
			ZeroMemory(len, sizeof(len));
			sprintf_s(len, "%d", num);
			writefile.Write(len, 4);
			writefile.Write(str[i], num);
		}
		writefile.Close();
	}
	else
	{
		res = false;
	}
	return res;
}


//���ļ�ĩβ����һ����¼
bool FileOp::AddAnLine(CString& fileName, CString& str) {
	CFile writefile;
	bool openWrite = writefile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::modeNoTruncate);
	if (openWrite) {
		writefile.SeekToEnd();
		int num = str.GetLength() * 2;
		char len[4];
		ZeroMemory(len, sizeof(len));
		sprintf_s(len, "%d", num);
		writefile.Write(len, 4);
		writefile.Write(str, num);
		writefile.Close();
	}
	else
	{
		return false;
	}

	return true;
}

//ɾ���ļ��к�.db�ļ�
bool FileOp::DeleteFolder(CString& folderName)
{
	CFileFind finder;
	CString path;
	path.Format(CString("%s/*.*"), folderName);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{//�����ļ���
			DeleteFolder(finder.GetFilePath()); //�ݹ�ɾ���ļ���
			RemoveDirectory(finder.GetFilePath());
		}
		else
		{//�����ļ�
			DeleteFile(finder.GetFilePath());
		}
	}
	if (!RemoveDirectory(folderName))
		return false;

	return true;
}

//�޸��ļ���
bool FileOp::ModifyFileName(CString& oldname, CString& newname)
{
	try
	{
		CFile::Rename(oldname, newname);
	}
	catch (CFileException* pEx)
	{
		MessageBox(NULL, CString("�޸�ʧ�ܣ�") + pEx->m_strFileName, CString("�޸��ļ�����"), MB_OK);
		return false;
	}

	return true;
}