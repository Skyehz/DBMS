#pragma once
#include <vector>
using namespace std;

#define ALTER_CHANGE_FIELDNAME 1
#define ALTER_CHANGE_FIELDTYPE 2
#define ALTER_ADD_CONSTRANIT_PK 3
#define ALTER_ADD_CONSTRANIT_NOTNULL 4
#define ALTER_ADD_CONSTRANIT_UNIQUE 5

class ParseSQL
{
public:

	void setDB(CString& dbmName);
	void getSql(CString& statement);
	CString dbmName;
	bool alterOp(vector<CString> init);		//alter²Ù×÷
};


