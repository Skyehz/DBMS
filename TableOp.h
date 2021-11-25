#pragma once

#include "TableModel.h" 

class TableOp
{
	
public:
	TableModel* table;
	TableOp();
	int CreateTable(CString& tableName, CString& dbmName);
	int DropTable(CString& tableName, CString& dbmName);
	int AlterTable(CString& tableName, CString& dbmName,int col_num,int record_num); 
};

