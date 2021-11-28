#pragma once
#include <vector>
using namespace std;
class ParseSQL
{
public:

	void setDB(CString& dbmName);
	void getSql(CString& statement);
	CString dbmName;
	bool alterOp(vector<CString> init);		//alter²Ù×÷
};


