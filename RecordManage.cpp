#include "pch.h"
#include "RecordManage.h"
#include "DBModel.h"
#include <vector>
#include"FileOp.h"
#include <iostream>
using namespace std;


int RecordManage::insert(CString& dbName)
{
	//����
	CString message;
	message = "insert into ...values...";
	//����(����.trd�ļ���
	//�������ݿ�ʵ��
	CDBModel db(dbName);
	//Ϊ�����ݿⴴ���ļ���
	CString folderPath = CString("dbms_root/data") + CString("/") + dbName;
	vector<CString> init;
	init.push_back(message + dbName);
	CString logPath = folderPath + CString("/") + dbName + CString(".txt");
	/*FileOp f;
	f.WriteLog(CString("created database:") + dbName);*/
	bool crtDBFilelog = FileOp::WriteRecord(logPath, init);
	//���
	return 0;
}
