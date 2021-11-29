#include "pch.h"
#include "ConstraintOp.h"
#include <vector>
using namespace std;

ConstraintOp::ConstraintOp(CString m_dbName, CString m_tbName)
{
	this->dbName = m_dbName;
	this->tbName = m_tbName;
	this->ticPath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tbName + CString(".tic");
}

ConstraintOp::~ConstraintOp()
{
}

bool ConstraintOp::AddConstraint(ConstraintModel& new_cst)
{
	//读出所有约束条件

	//判断能否添加该约束

	//添加

	return false;
}
