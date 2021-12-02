#pragma once
#include <vector>
#define INVALID 0
#include <map>
#include "FieldModel.h"
using namespace std;
class CDataModel
{
private:
	int m_id;
	map<CString, CString> m_mValues;
public:
	CDataModel(int id = INVALID);
	~CDataModel(void);

	int GetId() { return m_id; };
	CString GetValue(CString propety) { return m_mValues[propety]; };
	map<CString, CString> GetValues() { return m_mValues; };
	void SetValues(map<CString, CString> m_map) { m_mValues = m_map; }
	void SetId(int id) { m_id = id; };
	void SetValue(CString propety, CString value) { m_mValues[propety] = value; };
	CString toString(CDataModel datamodel,vector<FieldModel> fieldmodellist);
};



