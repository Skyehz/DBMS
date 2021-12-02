#pragma once
class FieldModel
{

public:

	//.tdf
	int id;			//字段顺序,默认为0
	CString name;		//字段名称，默认为"NewField"
	int type;			//字段类型  默认为1 是int 2 是varchar
	int param;			//字段类型参数，默认长度为0
	CString mtime;		//最后修改时间，当前时间
	int integrities;	//完整性约束信息，一般没有约束为0
	int primaryKey;  //主键信息 默认为0，1为主键
	int uniqueKey;   //唯一键，默认为0，1为唯一键
	CString defaultValue; //默认值，默认为“null”
	CString notes;    //注释,默认为“null”
	int empty;		//是否能为空，默认值能为空0，1为不能为空

	FieldModel(int order = 0, CString name = L"NewField", int type = 1, int param = 0, int integrities = 0,
		int primaryKey = 0, int uniqueKey = 0, CString defaultValue = L"null", CString notes = L"", int empty = 0);
	FieldModel(CString& str);
	~FieldModel(void);

	//判断字段函数
	bool IsPrimaryKey() { return this->primaryKey; };
	bool IsUniqueKey() { return this->uniqueKey; };
	bool IsEmpty() { return this->empty; };

	CString toString();
	//Get操作
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

	//Set操作
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

