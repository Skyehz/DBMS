#pragma once

#include "TableModel.h" 
#include <vector>
using namespace std;

class TableOp
{

public:
	CString dbmName;
	CString tableName;
	TableOp();
	vector <TableModel> GetTableMess(CString& dbmName);
	int CreateTable(CString& tableName, CString& dbmName);
	int DropTable(CString& tableName, CString& dbmName);
	bool AlterTable(CString& currentName,CString& tableName, CString& dbmName);
	int UpDateTable(CString& tableName, CString& dbmName, int col_num, int record_num);
};
