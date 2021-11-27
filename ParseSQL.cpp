#include "pch.h"
#include "ParseSQL.h"
#include "FileOp.h"
#include "DBOp.h"
#include "FieldOp.h"
#include "TableOp.h"
#include <vector>
#include "FieldModel.h"
using namespace std;

void ParseSQL::setDB(CString& dbmName) {
	this->dbmName = dbmName;
}
void ParseSQL::getSql(CString& statement) {
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



}

