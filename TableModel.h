#pragma once
class TableModel
{
public:
	int id;				//���ݿ���
	CString name;		//������
	CString dbmspath;   //�ļ�·��
	CString ticpath;    //�������ļ�·��
	CString trdpath;    //��¼�ļ�·��
	CString tidpath;    //�����ļ�·��
	CString tdfpath;    //�����ļ�·��
	CString CreateTime;
	CString EditTime;
	int col_num;       //�ֶ���
	int record_num;    //��¼��
	CString dbmName;        //���ڿ�����

	TableModel(CString& str);
	~TableModel(void);
};
