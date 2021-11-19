#include "pch.h"
#include "DBOp.h"
#include"FileOp.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

int CDBOp::InitDatabase() {
	//�������ݿ�dbms__root �ļ���
	CString folderPath = CString("./dbms_root");
	bool crtFolder = CreateDirectory(folderPath, NULL);

	return 0;
}

int CDBOp::CreateDatabase(CString& dbName) {
	//�������ݿ�ʵ��
	CDBModel db(dbName);

	//Ϊ�����ݿⴴ���ļ���
	CString folderPath = CString("./dbms_root/") + CString("/") + dbName;
	bool crtFolder = CreateDirectory(folderPath, NULL);


	//�ڸ��ļ����´������ݿ������ļ��� ���ݿ���.db�ļ�
	CString dbPath = CString("./dbms_root/") + CString("/") + dbName + CString(".db");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtDBFile = FileOp::WriteRecord(dbPath, init);

	//�ڸ��ļ����´����������ļ��� ���ݿ���.tb�ļ�
	CString tbPath = folderPath + CString("/") + dbName + CString(".tb");
	init.push_back(CString("0#") + dbName);
	bool crtDBFiletb = FileOp::WriteRecord(tbPath, init);

	//�ڸ��ļ����´�����־�ļ��� ���ݿ���.log�ļ�
	init.push_back(CString("0#") + dbName);
	CString logPath = folderPath + CString("/") + dbName + CString(".log");
	/*FileOp f;
	f.WriteLog(CString("created database:") + dbName);*/
	bool crtDBFilelog = FileOp::WriteRecord(logPath, init);

	return 0;
}