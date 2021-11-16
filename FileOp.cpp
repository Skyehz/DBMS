#include "pch.h"
#include "FileOp.h"

vector<CString> FileOp::ReadAll(CString& fileName) {
	vector<CString> content;
	TCHAR con[256];
	ZeroMemory(con, sizeof(con));

	char len[4];
	//以二进制形式打开文件
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
		MessageBox(NULL, CString("打开文件失败！"), CString("错误"), MB_OK | MB_ICONWARNING);
	}

	return content;
}

//将多条记录写进文件
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



bool FileOp::WriteLog(CString& oper)
{
	/*
	CString info("");
	CSystemDAO sysDao;
	info += sysDao.GetCurrUserName(CString("log/u.temp"));
	info += CString("  ") + oper;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	CTime t(time);
	info += CString("  ") + t.Format("%Y-%m-%d %H:%M:%S");
	return sysDao.WriteLog(info, CString("log/sys.log"));
	*/
	return true;
}
