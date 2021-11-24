#include "pch.h"
#include "FieldModel.h"
#include "FileOp.h"
FieldModel::FieldModel(int order, CString name, int type, int param, int integrities):
	order(order),name(name),type(type),param(param),integrities(integrities)
{
	mtime = FileOp::GetCurrTime();
}

FieldModel::FieldModel(CString& str)
{
	//处理这些字符串
}

FieldModel::~FieldModel(void)
{
}
