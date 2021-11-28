#include "pch.h"
#include "FieldModel.h"

#include "FileOp.h"

#include<atlconv.h>

FieldModel::FieldModel(int order, CString name, int type, int param, int integrities
	, int primaryKey, int uniqueKey, CString defaultValue, CString notes, int empty)
	:id(order), name(name), type(type), param(param), integrities(integrities)
	, uniqueKey(uniqueKey), defaultValue(defaultValue), notes(notes), empty(empty)
{
	mtime = FileOp::GetCurrTime();
}

FieldModel::FieldModel(CString& str)
{

	vector<CString> field = FileOp::StrSplit(str, L" ");
	USES_CONVERSION;
	int f_order = atoi(T2A(field.at(0)));
	int f_type = atoi(T2A(field.at(2)));
	int f_param = atoi(T2A(field.at(3)));
	int f_Integrities = atoi(T2A(field.at(5)));

	this->id = f_order;
	this->name = field.at(1);
	this->type = f_type;
	this->param = f_param;
	this->mtime = field.at(4);
	this->integrities = f_Integrities;


}

FieldModel::~FieldModel(void)
{
}

CString FieldModel::toString()
{
	CString Corder, CType, Cparam, Cintegrities;
	CString CprimaryKey, CuniqueKey, Cempty;
	Corder.Format(_T("%d"), this->GetId());
	CType.Format(_T("%d"), this->GetType());
	Cparam.Format(_T("%d"), this->GetParam());
	Cintegrities.Format(_T("%d"), this->GetIntegrities());
	CprimaryKey.Format(_T("%d"), this->primaryKey);
	CuniqueKey.Format(_T("%d"), this->uniqueKey);
	Cempty.Format(_T("%d"), this->empty);

	CString str = Corder + CString(" ") + this->GetName() + CString(" ") + CType +
		CString(" ") + Cparam + CString(" ") + this->mtime + CString(" ") + Cintegrities + CString(" ") +
		CprimaryKey + CString(" ") + CuniqueKey + CString(" ") + this->defaultValue + CString(" ") +
		this->notes + CString(" ") + Cempty;
	return str;
}