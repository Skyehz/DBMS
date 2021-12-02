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

	int getOneCst(CString& field, CString &cstName,int type);//����field��ȡԼ��
	bool AddConstraint(ConstraintModel &new_cst);	//���һ����Լ��
	bool DeleteConstraint(ConstraintModel& cst);	//ɾ��Լ��
	int cstVerify(CString& cstName);	//��֤Լ���Ϸ���
	vector<ConstraintModel> getCstList();	//���ļ��ж����е�Լ��
};

