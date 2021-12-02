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

//���Լ��
bool ConstraintOp::AddConstraint(ConstraintModel& new_cst)
{
	vector<CString> strs = FileOp::ReadAll(ticPath);	//���е�����Լ��

	if (strs.empty()||!strs.empty()&&cstVerify(new_cst.GetName()))
	{	
		CString str = new_cst.toString();
		return (FileOp::AddAnLine(ticPath, str));;
	}
	else
		return false;

}

//ɾ��Լ��
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

//���Լ���Ƿ�Ϸ�(�ж�Լ���Ƿ����)
int ConstraintOp::cstVerify(CString& cstName) {
	if (getCstList().empty()) //Լ���б�Ϊ�գ�û���κ�Լ��
		return TRUE;
	vector<ConstraintModel> cstlist = getCstList();
	for (vector<ConstraintModel>::iterator ite = cstlist.begin(); ite != cstlist.end(); ++ite) {
		if (ite->GetName() == cstName) {	//��ͬ��Լ��������
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
