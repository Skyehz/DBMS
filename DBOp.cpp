#include "pch.h"
#include "DBOp.h"
#include "FileOp.h"
#include "DBModel.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

CDBOp::CDBOp() {
	this->sysPath = CString("./dbms_root/data/dbms.sys");
	//InitDatabase();
}

//��sys�ļ������һ�����ݿ���Ϣ
bool WriteAnDB(CDBModel db,CString sysPath) {
	CString Cid, Ctype;
	Cid.Format(_T("%d"), db.GetId());
	Cid.Format(_T("%d"), db.GetType());
	CString str = Cid + CString("#") + db.GetName() + CString("#") + Ctype + CString("#")  + CString("#") + db.GetCreateTime();
	return(FileOp::AddAnLine(sysPath, str));
}

//�����ļ��в�����.db�ļ�
bool InitDBFile(CString& dbName)
{
	//Ϊ�����ݿⴴ���ļ���
	CString folderPath = CString("./dbms_root/") + CString("data/") + dbName;
	bool crtFolder = CreateDirectory(folderPath, NULL);

	//�ڸ��ļ����´������ݿ������ļ��� ���ݿ���.db�ļ�
	CString dbPath = CString("./dbms_root/") + dbName + CString(".db");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtDBFile = FileOp::WriteRecord(dbPath, init);

	return crtFolder && crtDBFile;
}

//����.tb�ļ�
bool InitTBFile(CString& dbName) {
	CString tbPath = CString("./dbms_root/data/") + dbName + CString("/") + dbName + CString(".tb");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtTBFile = FileOp::WriteRecord(tbPath, init);

	return crtTBFile;
}

//����.log��־�ļ�
bool InitLogFile(CString& dbName) {
	CString logPath = CString("./dbms_root/data/") + dbName + CString("/") + dbName + CString(".log");
	CString info = CString("created database:") + dbName + CString("  ") + FileOp::GetCurrTime();
	bool crtLogFile = FileOp::WriteLog(info, logPath);

	return crtLogFile;
}

int CDBOp::InitDatabase() {
	//�������ݿ�dbms__root �ļ���
	CString folderPath = CString("./dbms_root");
	bool crtFolder = CreateDirectory(folderPath, NULL);
	//����data�ļ���
	CString folderDataPath = CString("./dbms_root/data");
	bool crtFolderData = CreateDirectory(folderDataPath, NULL);
	return 0;
}

//�������ݿ������������ݿ�
int CDBOp::CreateDatabase(CString& dbName) {
	//�������ݿ�ʵ��
	CDBModel db(0,dbName,1);

	//��dbms.sys�ļ���������ݿ���Ϣ
	if (!WriteAnDB(db, sysPath))
		return false;
	//��ʼ��.db�ļ��������ļ���
	if (!InitDBFile(dbName))
		return false;
	//�ڸ��ļ����´����������ļ��� ���ݿ���.tb�ļ�
	if (!InitTBFile(dbName))
		return false;
	//�ڸ��ļ����´�����־�ļ��� ���ݿ���.log�ļ�
	if(!InitLogFile(dbName))
		return false;


	return 0;
}

int DeleteDBRecord(CString& sysFileName, CString& dbname)
{
	vector<CString> list = FileOp::ReadAll(sysFileName);
	if (list.empty())
		return false;
	else
	{
		vector<CString>::iterator ite = list.begin();
		++ite;
		for (; ite != list.end(); ++ite)
		{
			vector<CString> temp = FileOp::StrSplit(*ite, CString("#"));
			//����ҵ�ָ���ļ�¼
			if (temp[1] == dbname)
			{
				list.erase(ite);
				break;
			}
		}
		if (FileOp::WriteRecord(sysFileName, list))
			return true;
		else
			return false;
	}

}



//ɾ��ָ�������ݿ�
int CDBOp::DropDatabase(CString& dbname)
{
	if (!DeleteDBRecord(this->sysPath, dbname) || !DeleteFile(CString("./dbms_root/") + dbname + CString(".db")) || !FileOp::DeleteFolder(CString("./dbms_root/data/") + dbname))
	{
		return false;
	}
	else
	{
		/*CSystemLogic sysLogic;
		sysLogic.WriteLog(CString("deleted database:") + dbname);*/
		return true;
	}
	return 0;
}

//��ȡ��ǰ���ݿ��б�
vector<CDBModel> CDBOp::GetDatabaseList()
{
	vector<CDBModel> res;
	vector<CString> strList = FileOp::ReadAll(CString("./dbms_root/data/dbms.sys"));
	//MessageBox(NULL,CUtil::IntegerToString(strList.size()),L"",MB_OK);
	if (!strList.empty())
	{
		vector<CString>::iterator ite = strList.begin();
		++ite;
		for (; ite != strList.end(); ++ite)
		{
			res.push_back(CDBModel(*ite));
		}
	}
	return res;
}



