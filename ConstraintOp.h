#pragma once
#include "ConstraintModel.h"
class ConstraintOp
{
private:
	CString ticPath;
	CString dbName;
	CString tbName;
public:
	ConstraintOp(CString m_dbName,CString m_tbName);
	~ConstraintOp();
public:
	bool AddConstraint(ConstraintModel &new_cst);	//���һ����Լ��
};

