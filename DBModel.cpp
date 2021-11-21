#include "pch.h"
#include "DBModel.h"
#include "FileOp.h"
#pragma warning(disable:4996)


#include <vector>
using namespace std;

CDBModel::CDBModel(int id, CString name , int type , CString filepath) :
	id(id), name(name), type(type), filepath(filepath)
{
	crtime = FileOp::GetCurrTime();
}

CDBModel::CDBModel(CString& str) {
	id = 0;
	name = str;
	type = 1;
	filepath = CString("data/dbms.sys");
	crtime = FileOp::GetCurrTime();
}

CDBModel::~CDBModel() {
}