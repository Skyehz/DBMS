#pragma once
class TableModel
{
public:
	int id;				//数据库编号
	CString name;		//表名称
	CString dbmspath;   //文件路径
	CString ticpath;    //完整性文件路径
	CString trdpath;    //记录文件路径
	CString tidpath;    //索引文件路径
	CString tdfpath;    //定义文件路径
	CString CreateTime;
	CString EditTime;
	int col_num;       //字段数
	int record_num;    //记录数
	CString dbmName;        //所在库名字

	TableModel(CString& str);
	~TableModel(void);
};
