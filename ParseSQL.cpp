#include "pch.h"
#include "ParseSQL.h"
#include "FileOp.h"
#include "DBOp.h"
#include "FieldOp.h"
#include "TableOp.h"
#include "FieldModel.h"
#include "DataModel.h"
#include <map>

void ParseSQL::setDB(CString& dbmName) {
	this->dbmName = dbmName;
}
void ParseSQL::getSql(CString& statement) {
	statement.MakeLower();
	CString str = FileOp::semicolon(statement);
	vector<CString> init = FileOp::StrSplit(str, CString(" "));


	//�½�����
	if (init[0] == CString("create")) {
		CString type = init[1];
		if (type == CString("database")) {
			CString name = init[2];
			CDBOp dbOp;
			int code = dbOp.CreateDatabase(name);
		}
		else if (type == CString("table")) {
			CString fields = FileOp::getbrakets(str);
			CString tname = FileOp::getbeforebrakets(str);

			vector<CString> init1 = FileOp::StrSplit(tname, CString(" "));
			CString name = init1[2];
			//������
			TableOp tbop;
			tbop.CreateTable(name, this->dbmName);
			//��ȡ�ֶ�����
			vector<CString>field = FileOp::StrSplit(fields, CString(","));
			vector<FieldModel> fieldlist;
			int n = field.size();
			FieldModel fd;
			for (int i = 0; i < n; i++) {
				fd.SetId(i);
				CString len = FileOp::getbrakets(field[i]);
				fd.SetParam(_ttoi(len));
				CString fnt = FileOp::getbeforebrakets(field[i]);
				vector<CString>temp = FileOp::StrSplit(fnt, CString(" "));
				fd.SetName(temp[0]);
				int type = FileOp::GetTypeInt(temp[1]);
				fd.SetType(type);
				fieldlist.push_back(fd);
			}

			FieldOp fieldop(this->dbmName,name);
			fieldop.AddFields(fieldlist);

		}
		else
		{
			MessageBox(NULL, CString("����ʧ��"), CString("��ʾ"), MB_OK);
			return;
		}
	}
	//ɾ������
	else if (init[0] == CString("drop")) {
		CString type = init[1];
		if (type == CString("database")) {
			CString name = init[2];
			CDBOp dbOp;
			int code = dbOp.DropDatabase(name);
		}
		else if (type == CString("table")) {
			CString name = init[2];
			TableOp tbOp;
			int code = tbOp.DropTable(name, this->dbmName);

		}
		else
			return;
	}
	else if (init[0] == CString("rename")) {
		if (init[1] == CString("table")) {
			CString currentName = init[3];
			if (init[4] == CString("to")) {
				CString tableName = init[5];
				TableOp tbop;
				tbop.AlterTable(currentName, tableName, this->dbmName);
			}
		}
	}
	else if (init[0] == CString("insert")) {
		CString name = init[1];

	}
	else if (init[0] == CString("alter")) {
		CString type = init[1];
		if (type == CString("table")) {
			alterOp(init);
		}
	}

	return;
}


bool ParseSQL::alterOp(vector<CString> init) {
	CString tbname = init[2];

	//����ֶ�
	if (init[3] == CString("add") && init[4]!=CString("constraint")){
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);
		int curId = fieldList.back().GetId() + 1;
		FieldModel m_NewField(curId, init[4], FileOp::GetTypeInt(FileOp::getbeforebrakets(init[5])), FileOp::StringToInteger(FileOp::getbrakets(init[5])), 0);
		return fieldop.AddOneField(m_NewField);
	}

	//ɾ���ֶ�
	else if (init[3] == CString("drop") && init[4] == CString("column")) {
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);
		int curId = fieldList.back().GetId() + 1;
		bool dropF = fieldop.dropField(dbmName, tbname, init[5]);//ɾ���ֶ�
		//ɾ���ֶζ�Ӧ�ļ�¼
		bool dropR=true;

		return dropR && dropF;

	}

	//�޸��ֶ��� ALTER TABLE ���� CHANGE ���ֶ��� ���ֶ��� �ֶ�����(����);
	else if (init[3] == CString("change") && init[4]!= CString("column")) {
		if (init.size() == 7) {
			FieldOp fieldop(dbmName, tbname);
			vector<CString> condition;
			condition.push_back(init[4]);	//���ֶ���
			condition.push_back(init[5]);	//���ֶ���
			condition.push_back(init[6]);	//�ֶ�����(����);

			return fieldop.ModifyFieldSQL(condition, ALTER_CHANGE_FIELDNAME);
		}
	}

	//�޸��ֶ����� ALTER TABLE table_name MODIFY COLUMN column_name datatype;
	else if (init[3] == CString("modify") && init[4] == CString("column")) {
		if (init.size() == 7) {
			FieldOp fieldop(dbmName, tbname);
			vector<CString> condition;
			condition.push_back(init[5]);	//�ֶ���
			condition.push_back(init[6]);	//���ֶ�����(����);

			return fieldop.ModifyFieldSQL(condition, ALTER_CHANGE_FIELDTYPE);
		}
	}

	//���Լ������ ����Ĭ��ֵ ALTER TABLE ���� CHANGE COLUMN �ֶ��� �������� DEFAULT Ĭ��ֵ
	else if (init[3] == CString("change") && init[4] == CString("column") && init[7] == CString("default") && init.size() == 9) {
		
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[5]);	//�ֶ���
		condition.push_back(init[6]);	//��������
		condition.push_back(init[8]);	//Ĭ��ֵ

		return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_DEFAULT);
		
	}

	//���Լ������ ����Ϊ�ǿ� ALTER TABLE table_name MODIFY column_name datatype NOT NULL;
	else if (init[3] == CString("modify") && init.size() == 8 && init[6] == CString("not") && init[7] == CString("null")) {
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[4]);	//�ֶ���
		return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_NOTNULL);
	}

	//���Լ������ ���������Ψһֵ
	//  ALTER TABLE table_name ADD CONSTRAINT MyPrimaryKey PRIMARY KEY(column1, column2...);
	//	ALTER TABLE table_name ADD CONSTRAINT MyUniqueConstraint UNIQUE(column1, column2...);
	else if (init[3] == CString("add") && init[4] == CString("constraint")) {
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[5]);	//Լ����
		if (init[6].Find(CString("unique")) >= 0) {		
			condition.push_back(FileOp::getbrakets(init[6])); //����Լ�����ֶΣ��ֶ���1���ֶ���2��...
			return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_UNIQUE);
		}
		else if(init[6]==CString("primary")&& init[7].Find(CString("key")) >= 0){
			condition.push_back(FileOp::getbrakets(init[7])); //����Լ�����ֶΣ��ֶ���1���ֶ���2��...
			return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_PK);
		}
		
	}

	else
		return false;
}

//���group by֮��Ľ��
vector<vector<CDataModel>> getGroupByRes(vector<CDataModel> &whereRes,CString &field) {
	vector<CString> distinctList;
	vector<vector<CDataModel>> groupbyRes;
	bool isExist = false;
	if (!whereRes.empty()) {
		//ɸѡ��whereRes�в��ظ�������ֵ
		for (vector<CDataModel>::iterator ite = whereRes.begin(); ite != whereRes.end(); ++ite) {
			CString content = ite->GetValue(field);
			if (distinctList.empty())
				distinctList.push_back(content);
			else {
				for (int j = 0; j < distinctList.size(); j++) {
					if (content == distinctList.at(j)) {
						isExist = true;
						break;
					}
				}
				if (isExist == false) distinctList.push_back(content);
			}
		}
		
		vector<CDataModel> whereRest = whereRes;
		for (int i = 0; i < distinctList.size();i++) {
			vector<CDataModel> temp;
			for (int j = 0; j < whereRest.size();j++) {
				if (distinctList[i] == whereRest[j].GetValue(field)) {
					temp.push_back(whereRest[j]);
					//whereRest.erase(whereRest[j]);
				}
			}
			groupbyRes.push_back(temp);
		}

		return groupbyRes;
	}
	else 
		return groupbyRes;
}

//ɸѡhaving����֮��Ľ��
vector<CDataModel> getHavingRes(vector<vector<CDataModel>> &groupRes,CString &condition) {
	if (condition.Find(CString("sum(")) != -1) {

	}
	return vector<CDataModel>();
}

//��������Ľ�� (�������)
vector<CDataModel> getOrderBy(vector<CDataModel> &list, CString &field, int type) {
	vector<CDataModel> orderRes;

	//����
	if (type == 1) {
		int max = INT_MIN;
		for (int count = 0; count < list.size(); count++) {
			//�ҵ����ֵ
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now > max)
					max = now;
			}
			//�����ֵ��Ӧ�Ķ������orderRes
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite) {
				if (max == FileOp::StringToInteger(ite->GetValue(field))) {
					orderRes.push_back(*ite);
					list.erase(ite);
					break;
				}
			}
			max = INT_MIN;
		}
	}
	//����
	else {
		int min = INT_MAX;
		for (int count = 0; count < list.size(); count++) {
			//�ҵ���Сֵ
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now < min)
					min = now;
			}
			//����Сֵ��Ӧ�Ķ������orderRes
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite) {
				if (min == FileOp::StringToInteger(ite->GetValue(field))) {
					orderRes.push_back(*ite);
					list.erase(ite);
					break;
				}
			}
			min = INT_MAX;
		}
	}
	return orderRes;
}

//��þ���group by֮��� ���麯��sum ���
vector<CDataModel> getSumRes(vector<vector<CDataModel>> &groupRes, CString &field) {
	int currID = 0;
	vector<CDataModel> sumRes;
	for (int i = 0; i < groupRes.size(); i++) {
		vector<CDataModel> temp = groupRes[i];
		int sum = 0;
		for (int j = 0; j < temp.size(); j++) {
			sum += FileOp::StringToInteger(temp[j].GetValue(field)); //��field�ֶε�����ֵ���
		}

		map<CString, CString> m = temp[0].GetValues();
		m.insert(pair<CString, CString>(CString("sum"),FileOp::IntegerToString(sum)));	//������ֶ� sum
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		sumRes.push_back(new_data);
	}

	return sumRes;
}


//��þ���group by֮��� ���麯��count   count(*)����   count(��)�зǿ�
vector<CDataModel> getCountRes(vector<vector<CDataModel>> &groupRes, CString &field) {
	int currID = 0;
	vector<CDataModel> countRes;
	for (int i = 0; i < groupRes.size(); i++) {
		vector<CDataModel> temp = groupRes[i];
		int count = 0;
		if (field == CString("*")) {
			count = temp.size();
		}
		else {
			for (int j = 0; j < temp.size(); j++) {
				if (temp[j].GetValue(field) != CString(""))	//�ǿ���
					count++;
			}
		}

		map<CString, CString> m = temp[0].GetValues();
		m.insert(pair<CString, CString>(CString("count"), FileOp::IntegerToString(count)));	//������ֶ� avg
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		countRes.push_back(new_data);
	}
	return countRes;
}

//��þ���group by֮��� ���麯��avg
vector<CDataModel> getAvgRes(vector<vector<CDataModel>> &groupRes, CString &field) {
	vector<CDataModel> sumRes = getSumRes(groupRes, field);
	vector<CDataModel> countRes = getSumRes(groupRes, CString("*"));

	int currID = 0;
	vector<CDataModel> avgRes;
	for (int i = 0; i < groupRes.size(); i++) {
		vector<CDataModel> temp = groupRes[i];
		double avg = 0;
		int sum = FileOp::StringToInteger(sumRes[i].GetValue(CString("sum")));
		int count = FileOp::StringToInteger(sumRes[i].GetValue(CString("count")));
		avg =(double)sum /(double)count;

		map<CString, CString> m = temp[0].GetValues();
		m.insert(pair<CString, CString>(CString("avg"), FileOp::IntegerToString(avg)));	//������ֶ� avg
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		avgRes.push_back(new_data);
	}

	return avgRes;
}