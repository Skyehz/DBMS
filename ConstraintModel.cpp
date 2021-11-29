#include "pch.h"
#include "ConstraintModel.h"
#include "FileOp.h"
#include <vector>
using namespace std;

ConstraintModel::ConstraintModel(CString m_name, CString m_field, int m_type, CString m_param)
{
	name = m_name;
	field = m_field;
	type = m_type;
	param = m_param;
}

ConstraintModel::ConstraintModel(CString& str)
{
	vector<CString> init = FileOp::StrSplit(str, CString("#"));
	this->name = init[0];
	this->field = init[1];
	this->type = FileOp::StringToInteger(init[2]);
	this->param = init[3];
}

ConstraintModel::~ConstraintModel()
{
}
