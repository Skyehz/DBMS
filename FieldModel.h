#pragma once
class FieldModel
{

public:

	//.tdf
	int id;			//�ֶ�˳��,Ĭ��Ϊ0
	CString name;		//�ֶ����ƣ�Ĭ��Ϊ"NewField"
	int type;			//�ֶ�����  Ĭ��Ϊ1 ��int 2 ��varchar
	int param;			//�ֶ����Ͳ�����Ĭ�ϳ���Ϊ0
	CString mtime;		//����޸�ʱ�䣬��ǰʱ��
	int integrities;	//������Լ����Ϣ��һ��û��Լ��Ϊ0
	int primaryKey;  //������Ϣ Ĭ��Ϊ0��1Ϊ����
	int uniqueKey;   //Ψһ����Ĭ��Ϊ0��1ΪΨһ��
	CString defaultValue; //Ĭ��ֵ��Ĭ��Ϊ��null��
	CString notes;    //ע��,Ĭ��Ϊ��null��
	int empty;		//�Ƿ���Ϊ�գ�Ĭ��ֵ��Ϊ��0��1Ϊ����Ϊ��

	FieldModel(int order = 0, CString name = L"NewField", int type = 1, int param = 0, int integrities = 0,
		int primaryKey = 0, int uniqueKey = 0, CString defaultValue = L"null", CString notes = L"", int empty = 0);
	FieldModel(CString& str);
	~FieldModel(void);

	//�ж��ֶκ���
	bool IsPrimaryKey() { return this->primaryKey; };
	bool IsUniqueKey() { return this->uniqueKey; };
	bool IsEmpty() { return this->empty; };

	CString toString();
	//Get����
	int GetId() { return id; };
	CString GetName() { return name; };
	int GetType() { return type; };
	int GetParam() { return param; };
	CString GetMtime() { return mtime; };
	int GetIntegrities() { return integrities; };
	int GetPrimaryKey() { return primaryKey; };
	int GetUniqueKey() { return uniqueKey; };
	CString GetDefaultValue() { return defaultValue; };
	CString GetNotes() { return notes; };
	int GetEmpty() { return empty; };

	//Set����
	void SetId(int m_order) { id = m_order; };
	void SetName(CString m_name) { name = m_name; };
	void SetType(int m_type) { type = m_type; };
	void SetParam(int m_param) { param = m_param; };
	void GetMtime(CString m_mtime) { mtime = m_mtime; };
	void SetIntegrities(int m_integrities) { integrities = m_integrities; };

	void SetPrimaryKey() { primaryKey = true; };
	void CancelPrimaryKey() { primaryKey = false; }
	void SetUniqueKey() { uniqueKey = true; };
	void CancelUniqueKey() { uniqueKey = false; }
	void SetEmpty() { empty = true; };
	void CancelEmpty() { empty = false; }

	void SetPrimaryKey(int m_primaryKey) { primaryKey = m_primaryKey; };
	void SetUniqueKey(int m_uniqueKey) { uniqueKey = m_uniqueKey; };
	void SetDefaultValue(CString m_defaultValue) { defaultValue = m_defaultValue; };
	void SetNotes(CString m_notes) { notes = m_notes; };
	void SetEmpty(int m_empty) { empty = m_empty; };
};

