#pragma once
class FieldOp
{
public:
	//FieldOp();
	//FieldOp(CString& tdfPath);
	//CString tdfpath;
	//
	//�Ա��ֶε���ɾ�Ĳ� 
	bool addFiled(CString& dbName,CString& tableName,CString& fieldName,int fieldOrder,int fieldType,int fieldParam,int fieldIntegrities=-1);//����ֶ�
																																			 //�����ֶ���Ϣ
	bool addFieldInfo(CString& dbName, CString& tableName,FieldModel field);//�����ֶ���Ϣ
	
	bool modifyFiled(CString& dbName, CString& tableName, CString& fieldName,
		CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities = -1);
	//�޸ı����ֶ���Ϣ
	
	bool dropFiled(CString& dbName, CString& tableName,CString& fieldName);

	CString queryFiled(CString& dbName, CString& tableName, CString& fieldName);//��ѯ�����ֶ���Ϣ�������ַ���
	vector<CString> queryFields(CString& dbName, CString& tableName);// ��ѯ���������ֶ���


	bool IsTableExist(CString& dbName, CString& tableName);//�жϱ����ļ��Ƿ����
	int  IsFiledExist(CString& dbName, CString& tableName, CString& fieldName);//�鿴����tdf�ֶ��Ƿ����
};

