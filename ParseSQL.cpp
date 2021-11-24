#include "pch.h"
#include "ParseSQL.h"
#include "FileOp.h"
#include "DBOp.h"
#include <vector>
using namespace std;

void ParseSQL::getSql(CString& statement) {
	vector<CString> init = FileOp::StrSplit(statement, CString(" "));

	//新建库或表
	if (init[0] == CString("create")) {
		CString type = init[1];
		if (type == CString("database")) {
			CString name = init[2];
			CDBOp dbOp;
			int code = dbOp.CreateDatabase(name);
		}
		else if (type == CString("table")) {

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

		}
		else
			return;
	}
	//打开数据库
	else if (init[0] == CString("use")) {
		CString name = init[1];

	}
	else if (init[0] == CString("insert")) {
		CString name = init[1];

	}

}
