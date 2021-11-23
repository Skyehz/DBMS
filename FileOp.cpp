#include "pch.h"
#include "FileOp.h"

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