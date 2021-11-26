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

//�޸�dbms.sys������ݿ���Ϣ������Ϊ�µ����ݿ�����
bool ModifySysfileDB(CString& oldName, CString& newName, CString& sysFileName)
{
	vector<CString> strList = FileOp::ReadAll(sysFileName);
	if (!strList.empty())
	{
		for (vector<CString>::iterator ite = strList.begin() + 1; ite != strList.end(); ++ite)
		{
			vector<CString> tmpList = FileOp::StrSplit(*ite, L"#");
			if (tmpList[1] == oldName)
			{
				tmpList[1] = newName;
				*ite = tmpList[0] + CString("#") +
					tmpList[1] + CString("#") +
					tmpList[2] + CString("#") +
					tmpList[3] + CString("#") +
					tmpList[4];
				break;
			}
		}
	}
	return FileOp::WriteRecord(sysFileName, strList);
}

//�޸�.db�ļ�����
bool CDBOp::ModifyDirName(CString& oldName, CString& newName)
{
	CString oldDBFileName = CString("./dbms_root/") + oldName + ".db";
	CString newDBFileName = CString("./dbms_root/") + newName + ".db";
	vector<CString> strList = FileOp::ReadAll(oldDBFileName);
	if (!strList.empty())
	{
		//vector<CString>::iterator ite=strList.begin();
		CString first = strList[0];
		vector<CString> temp = FileOp::StrSplit(first, CString("#"));
		temp[1] = newName;
		first = temp[0] + CString("#") + temp[1];
		strList[0] = first;
	}
	if (FileOp::WriteRecord(oldDBFileName, strList) && FileOp::ModifyFileName(oldDBFileName, newDBFileName) )
		return true;
	else
		return false;

}

int CDBOp::ModifyDBName(CString& oldDBName, CString& newDBName) {
	//��ȡ��ǰ���ݿ��б�
	vector<CDBModel> dblist = GetDatabaseList();
	bool bExist = false;
	for (vector<CDBModel>::iterator ite = dblist.begin(); ite != dblist.end(); ++ite)
	{
		if (ite->GetName() == newDBName)
		{
			bExist = true;
			break;
		}
	}

	if (bExist)	//�����ݿ����Ѿ����ڣ������޸�
		return false;
	else        //�����ݿ��������ڣ������޸�
	{
		CString oldNametb = CString("./dbms_root/data/") + oldDBName + CString("/") + oldDBName + ".tb";
		CString newNametb = CString("./dbms_root/data/") + oldDBName + CString("/") + newDBName + ".tb";
		CString oldNamelog = CString("./dbms_root/data/") + oldDBName + CString("/") + oldDBName + ".log";
		CString newNamelog = CString("./dbms_root/data/") + oldDBName + CString("/") + newDBName + ".log";

		if (!ModifySysfileDB(oldDBName, newDBName, this->sysPath)	 //�޸�dbms.sys�еļ�¼
			||!ModifyDirName(oldDBName, newDBName)				//�޸�.db�ļ���
			||!FileOp::ModifyFileName(oldNametb, newNametb)		//�޸�.tb�ļ���
			||!FileOp::ModifyFileName(oldNamelog, newNamelog)	//�޸�.log�ļ���
			||MoveFile(CString("./dbms_root/data/") + oldDBName, CString("./dbms_root/data/") + newDBName) == 0)//�������ļ���
			return false;
		else
		{
			/*CSystemLogic sysLogic;
			sysLogic.WriteLog(CString("modified database name,") + oldDBName + CString("->") + newDBName);*/
			return true;
		}


	}
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



