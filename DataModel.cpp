#include "pch.h"
#include "DataModel.h"
#include "FileOp.h"
#include "FieldModel.h"


CDataModel::CDataModel(int id) :
	m_id(id)
{
}

CDataModel::~CDataModel(void)
{

}

CString CDataModel::toString(CDataModel datamodel,vector<FieldModel> fieldList) {
	CString str = FileOp::IntegerToString(datamodel.GetId());
	for (vector<FieldModel>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite)
	{
		CString str1 = datamodel.GetValue(ite->GetName());
		if (str1.Find(CString("\"")) != -1)str1 = str1.Mid(1, str1.GetLength() - 2);
		if (str1.Find(CString("'")) != -1)str1 = str1.Mid(1, str1.GetLength() - 2);
		str += CString("#") + str1;
	}
	return str;
}


