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
	vector<CString> strList = FileOp::StrSplit(str, L"#");
	USES_CONVERSION;
	id = atoi(T2A(strList[0]));		//CString -> char* -> int
	name = strList[1];
	type = atoi(T2A(strList[2]));
	crtime = strList[3];
}

CDBModel::~CDBModel() {
}