#include "pch.h"
#include "FieldModel.h"

#include "FileOp.h"

#include<atlconv.h>


FieldModel::FieldModel(int order, CString name, int type, int param, int integrities) :
	id(order), name(name), type(type), param(param), integrities(integrities)
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
