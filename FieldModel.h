#pragma once
class FieldModel
{
public:

	//.tdf
	int order;			//字段顺序
	CString name;		//字段名称
	int type;			//字段类型 
	int param;			//字段类型参数
	CString mtime;		//最后修改时间
	int integrities;	//完整性约束信息

	FieldModel(int order = 1, CString name = L"NewField", int type = 1, int param = 1, int integrities);
	FieldModel(CString& str);
	~FieldModel(void);


	//Get操作
	int GetOrder() { return order; };
	CString GetName() { return name; };
	int GetType() { return type; };
	int GetParam() { return param; };
	CString GetMtime() { return mtime; };
	int GetIntegrities() { return integrities; };

	//Set操作
	void SetOrder(int m_order) { order = m_order; };
	void SetName(CString m_name) { name = m_name; };
	void SetType(int m_type) { type = m_type; };
	void SetParam(int m_param) { param = m_param; };
	void GetMtime(CString m_mtime) {  mtime=m_mtime; };
	void SetIntegrities(int m_integrities) {integrities=m_integrities; };

};

