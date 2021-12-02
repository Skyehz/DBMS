#pragma once

#define CST_PK 101
#define CST_UNIQUE 102
#define CST_NOTNULL 103
#define CST_DEFAULT 104
class ConstraintModel
{
public:
	CString name;	//约束名
	CString field;	//字段名
	int type;		//约束类型
	CString param;	//参数 库名+表名+时间

	ConstraintModel(CString m_name, CString m_field, int m_type, CString m_param );
	ConstraintModel(CString &str);
	~ConstraintModel();

	CString toString();

	//Get操作
	CString GetName() { return name; };
	CString GetField() { return field; };
	int GetType() { return type; };
	CString GetParam() { return param; };

	//Set操作
	void SetName(CString m_name) { name = m_name; };
	void SetField(CString m_field) { field = m_field; };
	void SetType(int m_type) { type = m_type; };
	void SetParam(CString m_param) { param = m_param; };
};

