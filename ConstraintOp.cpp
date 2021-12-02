#include "pch.h"
#include "ConstraintOp.h"
#include "FieldModel.h"
#include "FileOp.h"
#include "ParseSQL.h"

ConstraintOp::ConstraintOp(CString m_dbName, CString m_tbName)
{
	this->dbName = m_dbName;
	this->tbName = m_tbName;
	this->ticPath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tbName + CString(".tic");
}

ConstraintOp::~ConstraintOp()
{
}

int ConstraintOp::getOneCst(CString &field,CString &cstName,int type) {
	
	if (type == ALTER_ADD_CONSTRANIT_PK) {
		CString param = dbName + CString("+") + tbName + CString("+") + FileOp::GetCurrTime();
		ConstraintModel cst(cstName, field, type, param);
		return AddConstraint(cst);
	}
	else if(type == ALTER_ADD_CONSTRANIT_UNIQUE) {
		CString param = dbName + CString("+") + tbName + CString("+") + FileOp::GetCurrTime();
		ConstraintModel cst(cstName, field, type, param);
		return AddConstraint(cst);
	}
	else if (type == ALTER_ADD_CONSTRANIT_NOTNULL) {
		CString param = dbName + CString("+") + tbName + CString("+") + FileOp::GetCurrTime();
		ConstraintModel cst(cstName, field, type, param);
		return AddConstraint(cst);
	}
	else if (type == ALTER_ADD_CONSTRANIT_DEFAULT) {
	
		CString param = dbName + CString("+") + tbName + CString("+") + FileOp::GetCurrTime();
		ConstraintModel cst(cstName, field, type, param);
		return AddConstraint(cst);
	}
}

//添加约束
bool ConstraintOp::AddConstraint(ConstraintModel& new_cst)
{
	vector<CString> strs = FileOp::ReadAll(ticPath);	//现有的所有约束

	if (strs.empty()||!strs.empty()&&cstVerify(new_cst.GetName()))
	{	
		CString str = new_cst.toString();
		return (FileOp::AddAnLine(ticPath, str));;
	}
	else
		return false;

}

//删除约束
bool ConstraintOp::DeleteConstraint(ConstraintModel& cst) {
	vector<CString> strs = FileOp::ReadAll(ticPath);	//现有的所有约束
	if (!strs.empty())
	{
		for (vector<CString>::iterator ite = strs.begin(); ite != strs.end(); ++ite)
		{
			vector<CString> now_cst = FileOp::StrSplit(*ite, CString("#"));
			//约束名相同或约束已存在，则返回
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

//检查约束是否合法(判断约束是否存在)
int ConstraintOp::cstVerify(CString& cstName) {
	if (getCstList().empty()) //约束列表为空，没有任何约束
		return TRUE;
	vector<ConstraintModel> cstlist = getCstList();
	for (vector<ConstraintModel>::iterator ite = cstlist.begin(); ite != cstlist.end(); ++ite) {
		if (ite->GetName() == cstName) {	//相同的约束名存在
			return false;
		}
	}
	return true;
}


vector<ConstraintModel> ConstraintOp::getCstList() {
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tbName + CString(".tic");
	vector<CString>	constraints = FileOp::ReadAll(filePath);
	vector<ConstraintModel> modelCsts;
	for (int i = 0; i < constraints.size(); i++)
	{
		CString cst = constraints.at(i);
		ConstraintModel cstModel(cst);
		modelCsts.push_back(cstModel);
	}
	return modelCsts;
}
