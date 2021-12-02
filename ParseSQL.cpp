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


int ParseSQL::getSql(CString& statement) {
	statement.MakeLower();
	CString str = FileOp::semicolon(statement);
	vector<CString> init = FileOp::StrSplit(str, CString(" "));

	vector<CDataModel> res;

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
				if (field[i].Find(CString("primary key")) == 0) {
					CString fieldname = FileOp::getbrakets(field[i]);
					FieldOp fop(this->dbmName, name);
					for (int j = 0; j < fieldlist.size(); j++) {
						if (fieldlist[j].GetName() == fieldname) {
							fieldlist[j].SetPrimaryKey(1);
							fieldlist[j].SetEmpty(1);
							fieldlist[j].SetUniqueKey(1);
							fieldlist[j].SetIntegrities(1);
						}
					}
					
				}
				else if (field[i].Find(CString("unique")) == 0) {
					CString fieldname = FileOp::getbrakets(field[i]);
					FieldOp fop(this->dbmName, name);
					for (int j = 0; j < fieldlist.size(); j++) {
						if (fieldlist[j].GetName() == fieldname) {
							fieldlist[j].SetUniqueKey(1);
							fieldlist[j].SetIntegrities(1);
						}
					}
					
				}
				else {
					FieldModel fd;
					fd.SetId(i);	//id从1开始设置
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
							fd.SetEmpty(1);
							fd.SetUniqueKey(1);
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
			}

			FieldOp fieldop(this->dbmName, name);

			fieldop.AddFields(fieldlist);

		}
		else
		{
			MessageBox(NULL, CString("创建失败"), CString("提示"), MB_OK);
			//return res;
		}
	}
	//删除库或表
	else if (init[0] == CString("drop")) {
		CString type = init[1];
		if (type == CString("database")) {
			CString name = init[2];
			CDBOp dbOp;
			int code = dbOp.DropDatabase(name);
			//return res;
		}
		else if (type == CString("table")) {
			CString name = init[2];
			TableOp tbOp;
			int code = tbOp.DropTable(name, this->dbmName);
			//return res;
		}
		//else
			//return res;
	}
	//重命名表
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
	//插入数据
	else if (init[0] == CString("insert") && init[1] == CString("into")) {
		insertOp(init);
	}
	//修改表
	else if (init[0] == CString("alter")) {
		CString type = init[1];
		if (type == CString("table")) {
			alterOp(init);
		}
	}
	//删除记录
	else if (init[0] == CString("delete") && init[1] == CString("from")) {
		deleteOp(init);
	}
	//更新记录
	else if (init[0] == CString("update")) {
		updateOp(init);
	}

	LogOp logOp(this->dbmName);
	LogModel logModel(1,statement);
	//MessageBox(NULL, statement, CString("提示"), MB_OK);
	logOp.WriteOneLog(logModel);
	return true;
}

vector<CDataModel> ParseSQL::getSelectSql(CString &statement) {
	statement.MakeLower();
	CString str = FileOp::semicolon(statement);
	vector<CString> init = FileOp::StrSplit(str, CString(" "));
	vector<CDataModel> res;
	
	res = selectOp(str);
	return res;
}


bool ParseSQL::alterOp(vector<CString> init) {
	CString tbname = init[2];
	//添加字段 alter table t2 add col_name type;
	if (init[3] == CString("add") && init[4]!=CString("constraint")){
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);

		int curId = fieldList.back().GetId() + 1;
		FieldModel m_NewField(curId, init[4], FileOp::GetTypeInt(FileOp::getbeforebrakets(init[5])), FileOp::StringToInteger(FileOp::getbrakets(init[5])), 0);
		return fieldop.AddOneField(m_NewField);
	}

	//删除字段 ALTER TABLE table_name DROP COLUMN column_name
	else if (init[3] == CString("drop") && init[4] == CString("column")) {
		FieldOp fieldop(dbmName, tbname);
		vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbmName, tbname);
		bool dropF = fieldop.DeleteField(init[5]);//删除字段
		//删除字段对应的记录
		//bool dropR=true;

		return /*dropR && */dropF;
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


//select的列中有聚组函数，则将所需要的聚组函数进行运算加到原表之后
vector<CDataModel> ParseSQL::getAllGroupRes(vector<vector<CDataModel>>& groupRes, vector<CDataModel>& res, CString& gField, vector<CString>& condition) {
	vector<CDataModel> allRes;
	vector<CString> distinctList = getDistinctList(res, gField);
	int n = distinctList.size();
	//map<CString, CString> m;
	for (int i = 0; i < n; i++) {
		map<CString, CString> m;
		CDataModel new_model(i);
		new_model.SetValue(CString("@"), FileOp::IntegerToString(i));
		m.insert(pair<CString, CString>(gField, distinctList[i]));
		new_model.SetValues(m);
		allRes.push_back(new_model);
	}

	
	//m.insert(pair<CString, CString>(gField, tmp[0]));
	for (int i = 0; i < condition.size(); i++) {
		if (condition[i].Find(CString("sum(")) != -1) {
			CString groupByfield = FileOp::getbrakets(condition[i]);
			vector<CString> sumRes = getSumRes(distinctList, groupRes, groupByfield);
			for (int k = 0; k < n; k++) {
				vector<CString> tmp = FileOp::StrSplit(sumRes[k], CString("#"));
				map<CString, CString> m = allRes[k].GetValues();
				m.insert(pair<CString, CString>(CString("sum(") + groupByfield + CString(")"), tmp[1]));//在原来map的基础上加一列
				allRes[k].SetValues(m);
			}
		}
		else if (condition[i].Find(CString("avg(")) != -1) {
			CString groupByfield = FileOp::getbrakets(condition[i]);
			vector<CString> avgRes = getAvgRes(distinctList, groupRes, groupByfield);
			for (int k = 0; k < n; k++) {
				vector<CString> tmp = FileOp::StrSplit(avgRes[k], CString("#"));
				//m.insert(pair<CString, CString>(gField, tmp[0]));
				map<CString, CString> m = allRes[k].GetValues();
				m.insert(pair<CString, CString>(CString("avg(") + groupByfield + CString(")"), tmp[1]));
				allRes[k].SetValues(m);//在原来map的基础上加一列
			}
		}
		else if (condition[i].Find(CString("count(")) != -1) {
			CString groupByfield = FileOp::getbrakets(condition[i]);
			vector<CString> countRes = getCountRes(distinctList, groupRes, groupByfield);
			for (int k = 0; k < n; k++) {
				vector<CString> tmp = FileOp::StrSplit(countRes[k], CString("#"));
				//m.insert(pair<CString, CString>(gField, tmp[0]));
				map<CString, CString> m = allRes[k].GetValues();
				m.insert(pair<CString, CString>(CString("count(") + groupByfield + CString(")"), tmp[1]));
				allRes[k].SetValues(m);//在原来map的基础上加一列
			}
		}


	}

	return allRes;
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
					m.insert(pair<CString, CString>(CString("sum(")+field+CString(")"), tmp[1]));
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
					m.insert(pair<CString, CString>(CString("avg") + field + CString(")"), tmp[1]));
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
					m.insert(pair<CString, CString>(CString("count") + field + CString(")"), tmp[1]));
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

			vector<CDataModel> dataList = dp.ReadDataList(fieldlist);
			int curId;
			if (dataList.empty())
				curId = 0;
			else
				curId = dataList.back().GetId() + 1;

			data.SetId(curId+i);
			data.SetValue(CString("@"), FileOp::IntegerToString(curId+i));///////////////////////////
			for (int j = 1; j < fieldlist.size(); j++) {
				data.SetValue(fieldlist[j].GetName(), valuelist[i][j-1]);
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
		FieldOp fp(this->dbmName, name);
		vector<CDataModel> dataList = dp.ReadDataList(fp.queryFieldsModel(this->dbmName,name));
		int curId;
		if (dataList.empty())
			curId = 0;
		else
			curId = dataList.back().GetId() + 1;
		
		for (int i = 0; i < valuelist.size(); i++) {
			CDataModel data;
			data.SetValue(CString("@"), FileOp::IntegerToString(curId + i));

			data.SetId(i+curId);
			for (int j = 0; j < fieldmodellist.size(); j++) {
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
	vector<CString> groupFunList; //此条语句中所需要的聚组函数

	vector<CString>temp1 = FileOp::StrSplit(str, CString(" order by "));//temp1[1]存放order by后面的内容
	vector<CString>temp2 = FileOp::StrSplit(temp1[0], CString(" having "));//temp2[1]存放having后面的内容
	vector<CString>temp3 = FileOp::StrSplit(temp2[0], CString(" group by "));//temp3[1]存放group by后面的内容
	vector<CString>temp4 = FileOp::StrSplit(temp3[0], CString(" where "));//temp4[1]存放where后面的内容
	vector<CString>temp5 = FileOp::StrSplit(init[1], CString(",")); //要查询的列名

	//筛选出所有的聚组函数
	for (int i = 0; i < temp5.size(); i++) {
		if (temp5[i].Find(CString("sum(")) != -1 || temp5[i].Find(CString("count(")) != -1 || temp5[i].Find(CString("avg(")) != -1) {
			groupFunList.push_back(temp5[i]);
		}
	}
	if (temp2.size() != 1) { //having之后的聚组函数
		vector<CString> con1 = FileOp::StrSplit(temp2[1], CString(" or "));
		for (int i = 0; i < con1.size(); i++) {
			vector<CString> con2 = FileOp::StrSplit(con1[i], CString(" and "));
			for (int j = 0; j < con2.size(); j++) {
				if (con2[j].Find(CString("sum(")) != -1 || con2[j].Find(CString("count(")) != -1 || con2[j].Find(CString("avg(")) != -1) {
					groupFunList.push_back(FileOp::StrSplit(con2[j], CString(")"))[0] + CString(")"));
				}
			}
		}
	}

	if (init[2] == CString("from")) {
		tablelist = FileOp::StrSplit(init[3], CString(","));
	}
	//只有一张表
	if (tablelist.size() == 1) {
		FieldOp fop(this->dbmName, tablelist[0]);
		CDataOp dop(this->dbmName, tablelist[0]);
		if (init[1] == "*") {
			fieldmodellist = fop.queryFieldsModel(this->dbmName, tablelist[0]);
			fieldlist = fop.queryFields(this->dbmName, tablelist[0]);
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
				vector<FieldModel>f = fop.queryFieldsModel(this->dbmName, tablelist[i]);
				for (int j = 0; j < f.size(); j++) {
					f[j].SetName(tablelist[i] + CString(".") + f[j].GetName());
				}
				fieldmodellist.insert(fieldmodellist.end(), f.begin(), f.end());
				vector<CDataModel> r = dop.ReadDataList(f);

				for (int k = 0; k < r.size(); k++) {
					CDataModel dm;
					dm.SetId(k);
					map<CString, CString> data = r[k].GetValues();

					map<CString, CString>::iterator it;
					for (it = data.begin(); it != data.end(); ++it) {
						dm.SetValue(it->first, it->second);
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
				vector<FieldModel>f = fop.queryFieldsModel(this->dbmName, tablelist[i]);
				for (int j = 0; j < f.size(); j++) {
					f[j].SetName(tablelist[i] + CString(".") + f[j].GetName());
				}
				fieldmodellist.insert(fieldmodellist.end(), f.begin(), f.end());
				vector<CDataModel> r = dop.ReadDataList(f);

				for (int k = 0; k < r.size(); k++) {
					CDataModel dm;
					dm.SetId(k);
					map<CString, CString> data = r[k].GetValues();

					map<CString, CString>::iterator it;
					for (it = data.begin(); it != data.end(); ++it) {
						dm.SetValue(it->first, it->second);
					}
					recordlist.push_back(dm);
				}
			}
		}
		res = recordlist;
	}
	//where
	if (temp4.size() != 1) {
		if (temp4.size() == 3) {
			CString con1 = temp4[1] + " where " + temp4[2];
			CString con = FileOp::getbrakets(con1);
			vector<CDataModel> subres = selectOp(con);
			CString value = subres[0].GetValues().begin()->second;
			con1.Replace(CString("(") + con + CString(")"), value);
			res = whereOp(con1, res, fieldmodellist);
		}
		res = whereOp(temp4[1], res, fieldmodellist);

	}

	//group by
	if (temp3.size() != 1) {
		CString groupField = temp3[1];
		groupRes = getGroupByRes(res, temp3[1]);
		res = getAllGroupRes(groupRes, res, groupField, groupFunList);
	}
	//having
	if (temp2.size() != 1) {
		//res = getHavingRes(groupRes, res,temp3[1],temp2[1]);
		vector<FieldModel> flist;
		for (int i = 0; i<groupFunList.size(); i++) {
			FieldModel field;
			field.SetName(groupFunList[i]);
			field.SetType(1);
			flist.push_back(field);
		}
		res = whereOp(temp2[1],res, flist);
	}
	//order by
	if (temp1.size() != 1) {
		if (temp1[1].Find(CString("desc")) != -1) {
			CString fname = FileOp::StrSplit(temp1[1], CString(" "))[0];
			res = getOrderBy(res, fname, 1);
		}
		else
			res = getOrderBy(res, temp1[1], 0);
		
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
		if(d.GetValues().size()!=0)res1.push_back(d);

	}

	return res1;
}

vector<CDataModel> ParseSQL::whereOp(CString& str, vector<CDataModel> recordlist, vector<FieldModel>& fieldmodellist) {
	vector<CDataModel> res;
	vector<CString> con1 = FileOp::StrSplit(str, CString(" or "));
	for (int i = 0; i < con1.size(); i++) {
		vector<CString> con2 = FileOp::StrSplit(con1[i], CString(" and "));
		vector<CDataModel> res1 = recordlist;
		for (int j = 0; j < con2.size(); j++) {
			vector<CDataModel> res2;
			for (int k = 0; k < res1.size(); k++) {
				if (altOp(con2[j], res1[k], fieldmodellist, recordlist)) {
					res2.push_back(res1[k]);
				}
			}
			res1 = res2;
		}
		
			
			for (int k = 0; k < res1.size(); k++) {
				bool re = false;
				for (int i = 0; i < res.size(); i++) {
					if (res1[k].toString(res1[k], fieldmodellist) == res[i].toString(res[i], fieldmodellist))re = true;
				}
				if (!re || res.size() == 0)res.push_back(res1[k]);
		}
		//res.insert(res.end(), res1.begin(), res1.end());
	}


	return res;
}

bool ParseSQL::altOp(CString& str, CDataModel& record, vector<FieldModel>& fieldmodellist, vector<CDataModel>& recordlist) {
	map<CString, CString>r;
	bool field1 = false;//是否为字段名
	bool field2 = false;
	bool num = false;//是否为数字
	CString field11;
	CString field12;
	double v1;
	double v2;


	if (str.Find(CString("=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();//当前record的值
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 == v2)return true;
							}
							else {
								if (value1 == recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 == v2)return true;
							}
							else {
								if (value2 == recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);

			if (v1 == v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 == value2)return true;
		else return false;


	}
	else if (str.Find(CString("<>")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<>"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 != v2)return true;
							}
							else {
								if (value1 != recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 != v2)return true;
							}
							else {
								if (value2 != recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);
			if (v1 != v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 != value2)return true;
		else return false;
	}
	else if (str.Find(CString("<=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 == v2 || v1 < v2)return true;
							}
							else {
								if (value1 == recordlist[i].GetValue(value2) || value1 < recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 == v2 || v1 < v2)return true;
							}
							else {
								if (value2 == recordlist[i].GetValue(value1) || value2 > recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);

			if (v1 == v2 || v1 < v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 == value2 || value1 < value2)return true;
		else return false;

	}
	else if (str.Find(CString(">=")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString(">="));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 == v2 || v1 > v2)return true;
							}
							else {
								if (value1 == recordlist[i].GetValue(value2) || value1 > recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 == v2 || v1 > v2)return true;
							}
							else {
								if (value2 == recordlist[i].GetValue(value1) || value2 < recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);

			if (v1 == v2 || v1 > v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 == value2 || value1 > value2)return true;
		else return false;

	}
	else if (str.Find(CString(">")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString(">"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 > v2)return true;
							}
							else {
								if (value1 > recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 > v2)return true;
							}
							else {
								if (value2 < recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);

			if (v1 > v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 > value2)return true;
		else return false;

	}
	else if (str.Find(CString("<")) != -1) {
		vector<CString>temp = FileOp::StrSplit(str, CString("<"));
		CString value1 = temp[0];
		CString value2 = temp[1];
		r = record.GetValues();
		map<CString, CString>::iterator it;
		for (int i = 0; i < fieldmodellist.size(); i++) {
			if (fieldmodellist[i].GetName() == value1) {
				field1 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
			if (fieldmodellist[i].GetName() == value2) {
				field2 = true;
				if (fieldmodellist[i].GetType() == 1 || fieldmodellist[i].GetType() == 3)num = true;
			}
		}

		for (it = r.begin(); it != r.end(); ++it) {
			FieldModel fm;
			if (it->first == value1) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value1) {
						fm = fieldmodellist[i];
					}
					if (field1) { if (IntegrityVerify(it->second, fm)) { field11 = value1; value1 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(value1);
								v2 = _ttof(recordlist[i].GetValue(value2));
								if (v1 < v2)return true;
							}
							else {
								if (value1 < recordlist[i].GetValue(value2))
									return true;
							}
						}
						return false;
					}
				}
			}
			if (it->first == value2) {
				for (int i = 0; i < fieldmodellist.size(); i++) {
					if (fieldmodellist[i].GetName() == value2) {
						fm = fieldmodellist[i];
					}
					if (field2) { if (IntegrityVerify(it->second, fm)) { field12 = value2; value2 = it->second; } }
					if (field1 && field2) {
						for (int i = 0; i < recordlist.size(); i++) {
							if (num) {
								v1 = _ttof(recordlist[i].GetValue(value1));
								v2 = _ttof(value2);
								if (v1 < v2)return true;
							}
							else {
								if (value2 > recordlist[i].GetValue(value1))
									return true;
							}
						}
						return false;
					}
				}

			}
		}
		if (num) {
			v1 = _ttof(value1);
			v2 = _ttof(value2);

			if (v1 < v2)return true;
			else return false;
		}
		if (!field1) {
			if (value1.Find(CString("\"")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
			if (value1.Find(CString("\'")) != -1)value2 = value1.Mid(1, value1.GetLength() - 2);
		}
		if (!field2) {
			if (value2.Find(CString("\"")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
			if (value2.Find(CString("\'")) != -1)value2 = value2.Mid(1, value2.GetLength() - 2);
		}
		if (value1 < value2)return true;
		else return false;

	}
	return false;
}


bool ParseSQL::deleteOp(vector<CString> init) {
	CString name = init[2];
	if (init[3] == CString("where")) {
		for (int i = 1; i<init.size(); i++) {
			init[i-1] = init[i];
		}
		init[0] = CString("select");
		init[1] = CString("*");
		CString selectStr;
		for (int i = 0; i < init.size()-1; i++) {
			if (i == 2) {
				selectStr += CString("from ") + name + CString(" ");
			}
			selectStr += init[i] + CString(" ");
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

//update t1 set name=ccc where id=1;
bool ParseSQL::updateOp(vector<CString> init) {
	CString name = init[1];
	if (init[2] == CString("set") && init[4] == CString("where")) {
		vector<CString> select;
		vector<CString>temp;
		temp = FileOp::StrSplit(init[3], CString("="));
		CString field = temp[0];
		CString value = temp[1];

		select.push_back(CString("select"));
		select.push_back(CString("*"));
		select.push_back(CString("from"));
		select.push_back(name);
		for (int i = 4; i < init.size(); i++) {
			select.push_back(init[i]);
		}
		CDataOp dp(this->dbmName, name);
		CString selectStr;
		for (int i = 0; i < select.size(); i++) {

			selectStr += select[i] + CString(" ");
		}
		selectStr = selectStr.Left(selectStr.GetLength() - 1);
		vector<CDataModel> records = selectOp(selectStr);
		if (value.Find(CString("\"")) == 0)value=value.Mid(1, value.GetLength() - 2);
		for (int i = 0; i < records.size(); i++) {
			records[i].SetValue(field, value);
			dp.ModifyRecord(records[i]);
		}
		return true;
	}
	return false;
}


bool OnlyHaveNumber(CString& str)
{
	bool isOK = TRUE;
	if (str != str.SpanIncluding(_T("1234567890")))
		isOK = FALSE;
	return isOK;
}
bool OnlyHaveNumberAndDot(CString& str)
{
	bool isOK = TRUE;
	if (str != str.SpanIncluding(_T("1234567890.")))
		isOK = FALSE;
	return isOK;
}


int ParseSQL::IntegrityVerify(CString& val, FieldModel& field)
{
	CString sAftText = _T("");
	for (int i = 0; i < val.GetLength(); i++)
	{
		if (val.Mid(i, 2) != _T("\"\""))
		{
			sAftText += val.Mid(i, 2);
		}
		else
		{
			sAftText += _T("");
		}
	}

	//完整性
	switch (field.GetType())
	{
	case 1://integer
	{
		if (OnlyHaveNumber(sAftText))
		{
			//(判断是否符合范围大小)
			int iVal = FileOp::StringToInteger(sAftText);
			//return 数字范围错误
		}
		else
		{
			return INTEGRITY_ERROR_TYPE;
		}
		break;
	}
	case 2://布尔
	{
		if (sAftText != "true" && sAftText != "false")
			return INTEGRITY_ERROR_TYPE;
		break;
	}
	case 3://double类型
	{
		if (OnlyHaveNumberAndDot(sAftText))
		{
			//判断double范围是否符合（待）
		}
		else
			return INTEGRITY_ERROR_TYPE;

		break;
	}
	case 4://varchar*类型
	{

		if (val.Find(CString("\"")) == 0) {
			if (val.GetLength() - 2 > field.GetParam())
			{
				return INTEGRITY_TOO_LONG;
			}
		}
		else
			return INTEGRITY_ERROR_TYPE;
		break;
	}
	case 5://datatime类型
	{
		if (sAftText != sAftText.SpanIncluding(_T("1234567890-")) || sAftText.GetLength() != 10)
			return INTEGRITY_ERROR_TYPE;
		else
		{
			//2014-09-02
			for (int i = 0; i < 10; i++)
			{
				if (i != 4 && i != 7)
				{
					if (sAftText[i] > '9' || sAftText[i] < '0')
						return INTEGRITY_ERROR_DATETYPE;
				}
			}

			if (sAftText[4] != '-' || sAftText[7] != '-')
				return INTEGRITY_ERROR_DATETYPE;
			CString date = sAftText;
			vector<CString> ymd = FileOp::StrSplit(date, CString("-"));
			int m = FileOp::StringToInteger(ymd[1]);
			int d = FileOp::StringToInteger(ymd[2]);
			if (m > 12 || (m == 2 && d > 28))
				return INTEGRITY_ERROR_DATETYPE;

			if (((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && d > 31) ||
				((m == 2 || m == 4 || m == 6 || m == 9 || m == 11) && d > 30))
				return INTEGRITY_ERROR_DATETYPE;
		}
		break;
	}
	default:
		break;
	}


	return TRUE;
}