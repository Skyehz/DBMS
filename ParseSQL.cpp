#include "pch.h"
#include "ParseSQL.h"
#include "FileOp.h"
#include "DBOp.h"
#include "FieldOp.h"
#include "TableOp.h"
#include "FieldModel.h"
#include "DataModel.h"
#include "DataOp.h"
#include <algorithm>
#include <vector>
#include <set>
#include"LogOp.h"
using namespace std;

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

			for (int i = 0; i < n; i++) {
				FieldModel fd;
				fd.SetId(i);
				CString len = FileOp::getbrakets(field[i]);
				fd.SetParam(_ttoi(len));
				CString fnt = FileOp::getbeforebrakets(field[i]);
				vector<CString>temp = FileOp::StrSplit(fnt, CString(" "));
				fd.SetName(temp[0]);
				int type = FileOp::GetTypeInt(temp[1]);
				fd.SetType(type);
				for (int i = 0; i < temp.size(); i++) {
					if (temp[i] == CString("primary") && temp[i + 1] == CString("key")) {
						fd.SetIntegrities(1);
						fd.SetPrimaryKey(1);
					}
					if (temp[i] == CString("unique")) {
						fd.SetIntegrities(1);
						fd.SetUniqueKey(1);
					}
					if (temp[i] == CString("not") && temp[i + 1] == CString("null")) {
						fd.SetIntegrities(1);
						fd.SetEmpty(1);
					}
					if (temp[i] == CString("comment")) {
						fd.SetIntegrities(1);
						fd.SetNotes(temp[i + 1].Mid(1, temp[i + 1].GetLength() - 2));
					}
					if (temp[i] == CString("default")) {
						fd.SetIntegrities(1);
						fd.SetNotes(temp[i + 1].Mid(1, temp[i + 1].GetLength() - 2));
					}
				}

				fieldlist.push_back(fd);
			}

			FieldOp fieldop(this->dbmName, name);
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
	else if (init[0] == CString("insert") && init[1] == CString("into")) {
		insertOp(init);
	}
	else if (init[0] == CString("select")) {
		selectOp(str);
	}
	else if (init[0] == CString("alter")) {
		CString type = init[1];
		if (type == CString("table")) {
			alterOp(init);
		}
	}


	LogOp logOp(this->dbmName);
	LogModel logModel(statement);
	if (!logOp.WriteOneLog(logModel))
		MessageBox(NULL, L"写入日志失败", CString("提示"), MB_OK);
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

vector<CString> ParseSQL::getDistinctList(vector<CDataModel>& whereRes, CString& field) {
	vector<CString> distinctList;
	bool isExist = false;
	if (!whereRes.empty()) {
		//按照field 筛选出whereRes中不重复的所有值
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
				if (isExist == false) 
					distinctList.push_back(content);
				isExist = false;
			}
		}
	}
	return distinctList;
}


//获得group by之后的结果
vector<vector<CDataModel>> ParseSQL::getGroupByRes(vector<CDataModel> &whereRes,CString &field) {
	vector<CString> distinctList = getDistinctList(whereRes,field);
	vector<vector<CDataModel>> groupbyRes;
	bool isExist = false;
	if (!whereRes.empty()&&!distinctList.empty()) {

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
vector<CDataModel> ParseSQL::getHavingRes(vector<vector<CDataModel>> &groupRes, vector<CDataModel> &res, CString& gField,CString &condition) {
	vector<CString> distinctList = getDistinctList(res, gField);
	int n = distinctList.size();
	int currId = 0;


	vector<CDataModel> havingRes;
	for (int i = 0; i < n; i++) {
		CDataModel new_model(i);
		havingRes.push_back(new_model);
	}


	
	vector<CString> con1 = FileOp::StrSplit(condition, CString(" or "));
	for (int i = 0; i < con1.size(); i++) {
		vector<CString> con2 = FileOp::StrSplit(con1[i], CString(" and "));
		for (int j = 0; j < con2.size(); j++) {

			if (con2[j].Find(CString("sum(")) != -1) {
				CString t = FileOp::StrSplit(con2[j], CString("("))[1];
				CString field = FileOp::StrSplit(t,CString(")"))[0];
				vector<CString> sumRes = getSumRes(distinctList, groupRes,field);
				for (int k = 0; k < n; k++) {
					vector<CString> tmp = FileOp::StrSplit(sumRes[k], CString("#"));
					map<CString, CString> m = havingRes[k].GetValues();
					m.insert(pair<CString, CString>(gField, tmp[0]));
					m.insert(pair<CString, CString>(CString("sum"), tmp[1]));
					havingRes[k].SetValues(m);//在原来map的基础上加一列
				}

			}
			else if (con2[j].Find(CString("avg(")) != -1) {
				CString t = FileOp::StrSplit(con2[j], CString("("))[1];
				CString field = FileOp::StrSplit(t, CString(")"))[0];
				vector<CString> avgRes = getAvgRes(distinctList, groupRes,field);
				for (int k = 0; k < n; k++) {
					vector<CString> tmp = FileOp::StrSplit(avgRes[k], CString("#"));
					map<CString, CString> m = havingRes[k].GetValues();
					m.insert(pair<CString, CString>(gField, tmp[0]));
					m.insert(pair<CString, CString>(CString("avg"), tmp[1]));
					havingRes[k].SetValues(m);//在原来map的基础上加一列
				}

			}
			else if (con2[j].Find(CString("count(")) != -1) {
				CString t = FileOp::StrSplit(con2[j], CString("("))[1];
				CString field = FileOp::StrSplit(t, CString(")"))[0];
				vector<CString> countRes = getCountRes(distinctList, groupRes, field);
				for (int k = 0; k < n; k++) {
					vector<CString> tmp = FileOp::StrSplit(countRes[k], CString("#"));
					map<CString, CString> m = havingRes[k].GetValues();
					m.insert(pair<CString, CString>(gField, tmp[0]));
					m.insert(pair<CString, CString>(CString("count"), tmp[1]));
					havingRes[k].SetValues(m);//在原来map的基础上加一列
				}

			}
		}
	}
	return havingRes;
}

//获得排序后的结果 (正序或倒序)
vector<CDataModel> ParseSQL::getOrderBy(vector<CDataModel> &list, CString &field, int type) {
	vector<CDataModel> orderRes;
	vector<CDataModel> tmpList = list;
	//倒序
	if (type == 1) {
		int max = INT_MIN;
		for (int count = 0; count < list.size(); count++) {
			//找到最大值
			for (vector<CDataModel>::iterator ite = tmpList.begin(); ite != tmpList.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now > max)
					max = now;
			}
			//将最大值对应的对象加入orderRes
			for (vector<CDataModel>::iterator ite = tmpList.begin(); ite != tmpList.end(); ++ite) {
				if (max == FileOp::StringToInteger(ite->GetValue(field))) {
					orderRes.push_back(*ite);
					tmpList.erase(ite);
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
			for (vector<CDataModel>::iterator ite = tmpList.begin(); ite != tmpList.end(); ++ite)
			{
				int now = FileOp::StringToInteger(ite->GetValue(field));
				if (now < min)
					min = now;
			}
			//将最小值对应的对象加入orderRes
			for (vector<CDataModel>::iterator ite = tmpList.begin(); ite != tmpList.end(); ++ite) {
				if (min == FileOp::StringToInteger(ite->GetValue(field))) {
					orderRes.push_back(*ite);
					tmpList.erase(ite);
					break;
				}
			}
			min = INT_MAX;
		}
	}
	return orderRes;
}

//获得经过group by之后的 聚组函数sum 结果 
vector<CString> ParseSQL::getSumRes(vector<CString> &distinctList,vector<vector<CDataModel>> &groupRes, CString &field) {
	vector<CString> sumRes;
	for (int i = 0; i < distinctList.size(); i++) {
		int sum = 0;
		vector<CDataModel> temp = groupRes[i];
		CString fieldValue = distinctList[i];
		for (int j = 0; j < temp.size(); j++) {
			sum += FileOp::StringToInteger(temp[j].GetValue(field));
		}
		CString str = fieldValue + CString("#") + FileOp::IntegerToString(sum);	//a#sum1   b#sum2   c#sum3
		sumRes.push_back(str);
	}

	return sumRes;
}

//获得经过group by之后的 聚组函数count   count(*)所有   count(列)列非空
vector<CString> ParseSQL::getCountRes(vector<CString> &distinctList, vector<vector<CDataModel>>& groupRes, CString& field) {
	int currID = 0;
	vector<CString> countRes;
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
		CString fieldValue = distinctList[i];
		CString str = fieldValue + CString("#") + FileOp::IntegerToString(count);	//a#count1   b#count2  c#count3
		countRes.push_back(str);
	}
	return countRes;
}

//获得经过group by之后的 聚组函数avg
vector<CString> ParseSQL::getAvgRes(vector<CString> &distinctList, vector<vector<CDataModel>>& groupRes, CString& field) {
	vector<CString> sumRes = getSumRes(distinctList,groupRes, field);
	vector<CString> countRes = getCountRes(distinctList,groupRes, CString("*"));

	int currID = 0;
	vector<CString> avgRes;
	for (int i = 0; i < groupRes.size(); i++) {
		vector<CDataModel> temp = groupRes[i];
		vector<CString> stmp = FileOp::StrSplit(sumRes[i], CString("#"));
		vector<CString> ctmp = FileOp::StrSplit(countRes[i], CString("#"));
		double avg = 0;
		int sum = FileOp::StringToInteger(stmp[1]);
		int count = FileOp::StringToInteger(ctmp[1]);
		avg =(double)sum /(double)count;

		CString fieldValue = distinctList[i];
		//不是int->CString!!!!!
		CString str = fieldValue + CString("#") + FileOp::IntegerToString(avg);	//a#avg1   b#avg2  c#avg3
		avgRes.push_back(str);
	}

	return avgRes;
}

bool ParseSQL::insertOp(vector<CString> init) {
	CString name = init[2];
	//按字段顺序插入记录
	if (init[3].Left(6) == CString("values")) {
		vector<vector<CString>>valuelist;
		CString v = init[3].Right(init[3].GetLength() - 6);
		vector<CString> values = FileOp::StrSplit(v, CString("),("));
		for (int i = 0; i < values.size(); i++) {
			if (values[i].Find(CString("(")) != -1)values[i] = values[i].Right(values[i].GetLength() - 1);
			if (values[i].Find(CString(")")) != -1)values[i] = values[i].Left(values[i].GetLength() - 1);
			vector<CString> value = FileOp::StrSplit(values[i], CString(","));
			valuelist.push_back(value);
		}
		FieldOp fp(this->dbmName, name);
		vector<FieldModel> fieldlist = fp.queryFieldsModel(this->dbmName, name);
		CDataOp dp(this->dbmName, name);
		for (int i = 0; i < valuelist.size(); i++) {
			CDataModel data;
			data.SetId(i);
			for (int j = 0; j < valuelist[i].size(); j++) {
				data.SetValue(fieldlist[j].GetName(), valuelist[i][j]);
			}
			dp.AddRecord(data, fieldlist);
		}
		return true;
	}
	//按指定字段插入记录
	else if (init[4].Left(6) == CString("values")) {
		CString fields = FileOp::getbrakets(init[3]);
		vector<CString>fieldlist = FileOp::StrSplit(fields, CString(","));
		vector<FieldModel>fieldmodellist;
		for (int i = 0; i < fieldlist.size(); i++) {
			FieldModel fm;
			fm.SetName(fieldlist[i]);
			fieldmodellist.push_back(fm);

		}
		vector<vector<CString>>valuelist;
		CString v = init[4].Right(init[4].GetLength() - 6);
		vector<CString> values = FileOp::StrSplit(v, CString("),("));
		for (int i = 0; i < values.size(); i++) {
			if (values[i].Find(CString("(")) != -1)values[i] = values[i].Right(values[i].GetLength() - 1);
			if (values[i].Find(CString(")")) != -1)values[i] = values[i].Left(values[i].GetLength() - 1);
			vector<CString> value = FileOp::StrSplit(values[i], CString(","));
			valuelist.push_back(value);
		}
		CDataOp dp(this->dbmName, name);
		for (int i = 0; i < valuelist.size(); i++) {
			CDataModel data;
			data.SetId(i);
			for (int j = 0; j < valuelist[i].size(); j++) {
				data.SetValue(fieldmodellist[j].GetName(), valuelist[i][j]);
			}
			dp.AddRecord(data, fieldmodellist);
		}

		return true;
	}
	else return false;
}

vector<CDataModel> ParseSQL::selectOp(CString str) {
	vector<CString> init = FileOp::StrSplit(str, CString(" "));
	vector<CString>tablelist;
	vector<CString>fieldlist;
	vector<FieldModel>fieldmodellist;
	vector<CDataModel> recordlist;
	vector<CDataModel> res;

	vector<vector<CDataModel>> groupRes;

	vector<CString>temp1 = FileOp::StrSplit(str, CString(" order by "));//temp1[1]存放order by后面的内容
	vector<CString>temp2 = FileOp::StrSplit(temp1[0], CString(" having "));//temp2[1]存放having后面的内容
	vector<CString>temp3 = FileOp::StrSplit(temp2[0], CString(" group by "));//temp3[1]存放group by后面的内容
	vector<CString>temp4 = FileOp::StrSplit(temp3[0], CString(" where "));//temp4[1]存放where后面的内容

	if (init[2] == CString("from")) {
		tablelist = FileOp::StrSplit(init[3], CString(","));
	}
	//只有一张表
	if (tablelist.size() == 1) {
		FieldOp fop(this->dbmName, tablelist[0]);
		CDataOp dop(this->dbmName, tablelist[0]);
		if (init[1] == "*") {
			fieldmodellist = fop.queryFieldsModel(this->dbmName, tablelist[0]);
			recordlist = dop.ReadDataList(fieldmodellist);
			res = recordlist;
		}
		else {
			fieldmodellist = fop.queryFieldsModel(this->dbmName, tablelist[0]);
			recordlist = dop.ReadDataList(fieldmodellist);
			fieldlist = FileOp::StrSplit(init[1], CString(","));//要显示的字段名
			res = recordlist;
		}

	}
	//数据来源于多张表
	else {
		if (init[1] == "*") {
			for (int i = 0; i < tablelist.size(); i++) {
				FieldOp fop(this->dbmName, tablelist[i]);
				CDataOp dop(this->dbmName, tablelist[i]);
				fieldmodellist = fop.queryFieldsModel(this->dbmName, tablelist[i]);
				vector<CDataModel> r = dop.ReadDataList(fieldmodellist);
				for (int j = 0; j < fieldmodellist.size(); j++) {
					fieldmodellist[i].SetName(tablelist[i] + CString(".") + fieldmodellist[j].GetName());
				}
				for (int k = 0; k < r.size(); k++) {
					CDataModel dm;
					dm.SetId(k);
					map<CString, CString> data = r[k].GetValues();

					map<CString, CString>::iterator it;
					for (it = data.begin(); it != data.end(); ++it) {
						dm.SetValue(tablelist[i] + CString(".") + it->first, it->second);
					}
					recordlist.push_back(dm);
				}
			}
		}
		else {
			map<CString, vector<CString>> fields;//存放表名和列名
			fieldlist = FileOp::StrSplit(init[1], CString(","));
			for (int i = 0; i < fieldlist.size(); i++) {
				vector<CString> t = FileOp::StrSplit(fieldlist[i], CString("."));
				map<CString, vector<CString>>::iterator it;
				for (it = fields.begin(); it != fields.end(); ++it) {
					if (it->first == t[0])it->second.push_back(t[1]);
					else {
						vector<CString> f;
						fields.insert(pair< CString, vector<CString>>(t[0], f));
					}
				}

			}
			for (int i = 0; i < tablelist.size(); i++) {
				FieldOp fop(this->dbmName, tablelist[i]);
				CDataOp dop(this->dbmName, tablelist[i]);
				fieldmodellist = fop.queryFieldsModel(this->dbmName, tablelist[i]);
				vector<CDataModel> r = dop.ReadDataList(fieldmodellist);
				for (int j = 0; j < fieldmodellist.size(); j++) {
					fieldmodellist[i].SetName(tablelist[i] + CString(".") + fieldmodellist[j].GetName());
				}
				for (int k = 0; k < r.size(); k++) {
					CDataModel dm;
					dm.SetId(k);
					map<CString, CString> data = r[k].GetValues();
					map<CString, CString>::iterator it;
					for (it = data.begin(); it != data.end(); ++it) {
						dm.SetValue(tablelist[i] + CString(".") + it->first, it->second);
					}
					recordlist.push_back(dm);
				}
			}
		}
		res = recordlist;
	}
	//where
	if (temp4.size() != 1) {
		res = whereOp(temp4[1], res);

	}
	
	//group by
	if (temp3.size() != 1) {
		CString groupField = temp3[1];
		groupRes = getGroupByRes(res, temp3[1]);
	}
	//having
	if (temp2.size() != 1) {
		
		res = getHavingRes(groupRes, res,temp3[1],temp2[1]);
		
	}
	//order by
	if (temp1.size() != 1) {
		//判断正序或倒序
		res = getOrderBy(res, temp1[1],1);
	}
	vector<CDataModel> res1;

	for (int i = 0; i < res.size(); i++) {
		CDataModel d;
		d.SetId(res[i].GetId());
		map<CString, CString> v = res[i].GetValues();
		for (int j = 0; j < fieldlist.size(); j++) {
			map<CString, CString>::iterator it;
			for (it = v.begin(); it != v.end(); ++it) {
				if (fieldlist[j] == it->first) {
					d.SetValue(it->first, it->second);
				}
			}
		}
		res1.push_back(d);

	}



	return res1;
}

vector<CDataModel> ParseSQL::whereOp(CString& str, vector<CDataModel> recordlist) {
	vector<CDataModel> res;
	vector<CString> con1 = FileOp::StrSplit(str, CString(" or "));
	for (int i = 0; i < con1.size(); i++) {
		vector<CString> con2 = FileOp::StrSplit(con1[i], CString(" and "));
		vector<CDataModel> res1 = recordlist;
		for (int j = 0; j < con2.size(); j++) {
			vector<CDataModel> res2;
			for (int k = 0; k < res1.size(); k++) {
				if (altOp(con2[j], res1[k])) {
					res2.push_back(res1[k]);
				}
			}
			res1 = res2;
		}
		res.insert(res.end(), res1.begin(), res1.end());
	}
	////res去掉重复元素
	//set<CDataModel>s(res.begin(), res.end());
	//res.assign(s.begin(), s.end());

	return res;
}


bool ParseSQL::altOp(CString& str, CDataModel& record) {
	map<CString, CString>r;
	if (str.Find(CString("=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 == value2)return true;

	}
	else if (str.Find(CString("<>")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<>"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 != value2)return true;
	}
	else if (str.Find(CString("<=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 < value2 || value1 == value2)return true;

	}
	else if (str.Find(CString(">=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString(">="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 > value2 || value1 == value2)return true;

	}
	else if (str.Find(CString(">")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString(">"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 > value2)return true;

	}
	else if (str.Find(CString("<")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (it = r.begin(); it != r.end(); ++it) {
			if (it->first == value1)value1 = it->second;
			if (it->first == value2)value2 = it->second;
		}
		if (value1 < value2)return true;

	}
	return false;
}

bool ParseSQL::deleteOp(vector<CString> init) {
	CString name = init[2];
	if (init[3] == CString("where")) {
		for (int i = 1; i < init.size(); i++) {
			init[i] = init[i - 1];
		}
		init[0] = CString("select");
		init[1] = CString("*");
		CString selectStr;
		for (int i = 0; i < init.size(); i++) {
			selectStr += init[0] + CString(" ");
		}
		selectStr = selectStr.Left(selectStr.GetLength() - 1);
		vector<CDataModel> res = selectOp(selectStr);
		CDataOp dp(this->dbmName, name);
		for (int i = 0; i < res.size(); i++) {
			dp.DeleteRecord(res[i].GetId());
		}
		return true;
	}
	if (init.size() == 3) {

		return true;
	}
	return false;
}

bool ParseSQL::updateOp(vector<CString> init) {
	CString name = init[2];
	if (init[3] == CString("set") && init[5] == CString("where")) {
		vector<CString> select;
		vector<CString>temp;
		temp = FileOp::StrSplit(init[4], CString("="));
		CString field = temp[0];
		CString value = temp[1];

		select.push_back(CString("select"));
		select.push_back(CString("*"));
		for (int i = 5; i < init.size(); i++) {
			select.push_back(init[i]);
		}
		CDataOp dp(this->dbmName, name);
		CString selectStr;
		for (int i = 0; i < select.size(); i++) {
			selectStr += select[0] + CString(" ");
		}
		selectStr = selectStr.Left(selectStr.GetLength() - 1);
		vector<CDataModel> records = selectOp(selectStr);
		for (int i = 0; i < records.size(); i++) {
			records[i].SetValue(field, value);
			dp.ModifyRecord(records[i]);
		}
		return true;
	}
	return false;
}