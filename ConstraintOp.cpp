#include "pch.h"
#include "ConstraintOp.h"
#include "FileOp.h"
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
	vector<CString> strs = FileOp::ReadAll(ticPath);	//���е�����Լ��

	if (!strs.empty())
	{
		for (vector<CString>::iterator ite = strs.begin(); ite != strs.end(); ++ite)
		{
			vector<CString> now_cst = FileOp::StrSplit(*ite, CString("#"));
			//Լ������ͬ��Լ���Ѵ��ڣ��򷵻�
			if (now_cst[0] == new_cst.GetName() || now_cst[1] == new_cst.GetField() && now_cst[2] == new_cst.GetType())
				return false;
		}
		
		CString str = new_cst.toString();
		return (FileOp::AddAnLine(ticPath, str));;
	}
	else
		return false;

}

bool ConstraintOp::DeleteConstraint(ConstraintModel& cst) {
	vector<CString> strs = FileOp::ReadAll(ticPath);	//���е�����Լ��
	if (!strs.empty())
	{
		for (vector<CString>::iterator ite = strs.begin(); ite != strs.end(); ++ite)
		{
			vector<CString> now_cst = FileOp::StrSplit(*ite, CString("#"));
			//Լ������ͬ��Լ���Ѵ��ڣ��򷵻�
			if (now_cst[1] == cst.GetField() && now_cst[2] == cst.GetType()) {
				strs.erase(ite);
				break;
			}
		}
		return FileOp::WriteRecord(ticPath, strs);
	}
	else
		return false;
}
