#pragma once
class TableModel
{
public:
	int id;				
	CString name;		
	CString dbmspath;   
	CString ticpath;    
	CString trdpath;    
	CString tidpath;   
	CString tdfpath;    
	CString CreateTime;
	CString EditTime;
	int col_num;       
	int record_num;   
	CString dbmName;      

	TableModel(CString& str);
	~TableModel(void);
};
