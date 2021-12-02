#pragma once
#include "ConstraintModel.h"
#include <vector>
using namespace std;
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

	int getOneCst(CString& field, CString &cstName,int type);//根据field获取约束
	bool AddConstraint(ConstraintModel &new_cst);	//添加一个新约束
	bool DeleteConstraint(ConstraintModel& cst);	//删除约束
	int cstVerify(CString& cstName);	//验证约束合法性
	vector<ConstraintModel> getCstList();	//从文件中读所有的约束
};

