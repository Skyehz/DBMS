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


	//新建库或表
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
			//创建表
			TableOp tbop;
			tbop.CreateTable(name, this->dbmName);
			//获取字段类型
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
			MessageBox(NULL, CString("创建失败"), CString("提示"), MB_OK);
			return;
		}
	}
	//删除库或表
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

	//添加字段
	if (init[3] == CString("add") && init[4]!=CString("constraint")){
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);
		int curId = fieldList.back().GetId() + 1;
		FieldModel m_NewField(curId, init[4], FileOp::GetTypeInt(FileOp::getbeforebrakets(init[5])), FileOp::StringToInteger(FileOp::getbrakets(init[5])), 0);
		return fieldop.AddOneField(m_NewField);
	}

	//删除字段
	else if (init[3] == CString("drop") && init[4] == CString("column")) {
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);
		int curId = fieldList.back().GetId() + 1;
		bool dropF = fieldop.dropField(dbmName, tbname, init[5]);//删除字段
		//删除字段对应的记录
		bool dropR=true;

		return dropR && dropF;

	}

	//修改字段名 ALTER TABLE 表名 CHANGE 旧字段名 新字段名 字段类型(长度);
	else if (init[3] == CString("change") && init[4]!= CString("column")) {
		if (init.size() == 7) {
			FieldOp fieldop(dbmName, tbname);
			vector<CString> condition;
			condition.push_back(init[4]);	//旧字段名
			condition.push_back(init[5]);	//新字段名
			condition.push_back(init[6]);	//字段类型(长度);

			return fieldop.ModifyFieldSQL(condition, ALTER_CHANGE_FIELDNAME);
		}
	}

	//修改字段类型 ALTER TABLE table_name MODIFY COLUMN column_name datatype;
	else if (init[3] == CString("modify") && init[4] == CString("column")) {
		if (init.size() == 7) {
			FieldOp fieldop(dbmName, tbname);
			vector<CString> condition;
			condition.push_back(init[5]);	//字段名
			condition.push_back(init[6]);	//新字段类型(长度);

			return fieldop.ModifyFieldSQL(condition, ALTER_CHANGE_FIELDTYPE);
		}
	}

	//添加约束条件 设置默认值 ALTER TABLE 表名 CHANGE COLUMN 字段名 数据类型 DEFAULT 默认值
	else if (init[3] == CString("change") && init[4] == CString("column") && init[7] == CString("default") && init.size() == 9) {
		
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[5]);	//字段名
		condition.push_back(init[6]);	//数据类型
		condition.push_back(init[8]);	//默认值

		return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_DEFAULT);
		
	}

	//添加约束条件 设置为非空 ALTER TABLE table_name MODIFY column_name datatype NOT NULL;
	else if (init[3] == CString("modify") && init.size() == 8 && init[6] == CString("not") && init[7] == CString("null")) {
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[4]);	//字段名
		return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_NOTNULL);
	}

	//添加约束条件 添加主键、唯一值
	//  ALTER TABLE table_name ADD CONSTRAINT MyPrimaryKey PRIMARY KEY(column1, column2...);
	//	ALTER TABLE table_name ADD CONSTRAINT MyUniqueConstraint UNIQUE(column1, column2...);
	else if (init[3] == CString("add") && init[4] == CString("constraint")) {
		FieldOp fieldop(dbmName, tbname);
		vector<CString> condition;
		condition.push_back(init[5]);	//约束名
		if (init[6].Find(CString("unique")) >= 0) {		
			condition.push_back(FileOp::getbrakets(init[6])); //进行约束的字段：字段名1，字段名2，...
			return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_UNIQUE);
		}
		else if(init[6]==CString("primary")&& init[7].Find(CString("key")) >= 0){
			condition.push_back(FileOp::getbrakets(init[7])); //进行约束的字段：字段名1，字段名2，...
			return fieldop.ModifyFieldSQL(condition, ALTER_ADD_CONSTRANIT_PK);
		}
		
	}

	else
		return false;
}

//获得group by之后的结果
vector<vector<CDataModel>> getGroupByRes(vector<CDataModel> &whereRes,CString &field) {
	vector<CString> distinctList;
	vector<vector<CDataModel>> groupbyRes;
	bool isExist = false;
	if (!whereRes.empty()) {
		//筛选出whereRes中不重复的所有值
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

//筛选having条件之后的结果
vector<CDataModel> getHavingRes(vector<vector<CDataModel>> &groupRes,CString &condition) {
	if (condition.Find(CString("sum(")) != -1) {

	}
	return vector<CDataModel>();
}

//获得排序后的结果 (正序或倒序)
vector<CDataModel> getOrderBy(vector<CDataModel> &list, CString &field, int type) {
	vector<CDataModel> orderRes;

	//倒序
	if (type == 1) {
		int max = INT_MIN;
		for (int count = 0; count < list.size(); count++) {
			//找到最大值
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now > max)
					max = now;
			}
			//将最大值对应的对象加入orderRes
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
	//正序
	else {
		int min = INT_MAX;
		for (int count = 0; count < list.size(); count++) {
			//找到最小值
			for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now < min)
					min = now;
			}
			//将最小值对应的对象加入orderRes
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

//获得经过group by之后的 聚组函数sum 结果
vector<CDataModel> getSumRes(vector<vector<CDataModel>> &groupRes, CString &field) {
	int currID = 0;
	vector<CDataModel> sumRes;
	for (int i = 0; i < groupRes.size(); i++) {
		vector<CDataModel> temp = groupRes[i];
		int sum = 0;
		for (int j = 0; j < temp.size(); j++) {
			sum += FileOp::StringToInteger(temp[j].GetValue(field)); //将field字段的所有值求和
		}

		map<CString, CString> m = temp[0].GetValues();
		m.insert(pair<CString, CString>(CString("sum"),FileOp::IntegerToString(sum)));	//添加新字段 sum
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		sumRes.push_back(new_data);
	}

	return sumRes;
}


//获得经过group by之后的 聚组函数count   count(*)所有   count(列)列非空
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
				if (temp[j].GetValue(field) != CString(""))	//非空列
					count++;
			}
		}

		map<CString, CString> m = temp[0].GetValues();
		m.insert(pair<CString, CString>(CString("count"), FileOp::IntegerToString(count)));	//添加新字段 avg
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		countRes.push_back(new_data);
	}
	return countRes;
}

//获得经过group by之后的 聚组函数avg
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
		m.insert(pair<CString, CString>(CString("avg"), FileOp::IntegerToString(avg)));	//添加新字段 avg
		CDataModel new_data;
		new_data.SetId(currID++);
		new_data.SetValues(m);
		avgRes.push_back(new_data);
	}

	return avgRes;
}