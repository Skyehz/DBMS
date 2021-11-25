#include "pch.h"
#include "DataOp.h"
#include "DBModel.h"

#include"FileOp.h"
#include <iostream>
#include "DataOptionDlg.h"

using namespace std;

void DataOp::getMessage()
{
	DataOptionDlg* dataOpDlg = new DataOptionDlg();
	dataOpDlg->setMessage(message);
}
void DataOp::getSql(CString& tableName) {
	getMessage();
	vector<CString> init = FileOp::StrSplit(message, CString(" "));
	//数据操作类型
	if (init[0] == CString("insert")) {
		if (init[2] == tableName) {
            USES_CONVERSION;
            string str(W2A(message));
            stringstream ss(str);
			FileOp* file = new FileOp();
			file->WriteRecord(tableName, );
            //打开文件，追加写，写入文件中
   //         tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "a");
   //         for (int i = 0; i < tab[pos]->type.size(); i++) {
   //             string tmp = tab[pos]->type[i];
   //             //cout << "tmp:" << tmp << endl;
   //             if (tmp == "int") {
   //                 int x; ss >> x;
   //                 fprintf(tab[pos]->fp, "%d", x);
   //             }
   //             else if (tmp == "float") {
   //                 float x; ss >> x;
   //                 fprintf(tab[pos]->fp, "%f", x);
   //             }
   //             else if (tmp == "double") {
   //                 double x; ss >> x;
   //                 fprintf(tab[pos]->fp, "%f", x);
   //             }
   //             else if (tmp == "char") {
   //                 if (tab[pos]->size[i] == 1) {
   //                     char x; ss >> x;
   //                     fprintf(tab[pos]->fp, "%c", x);
   //                 }
   //                 else {
   //                     int cnt = tab[pos]->size[i];
   //                     char* x = new char[cnt];
   //                     ss >> x;
   //                     fprintf(tab[pos]->fp, "%s", x);
   //                     delete x;
   //                 }
   //             }
   //             if (i != tab[pos]->type.size() - 1)
   //                 fprintf(tab[pos]->fp, "%c", ' ');
   //         }
			//
			//for (int i = 0; i < str.length(); i++)
			//	if (str[i] == ',' || str[i] == '(' || str[i] == ')')
			//		str[i] = ' ';

		}
	}
	else if (init[0] == CString("update")) {
		if (init[1] == tableName) {

		}

	}
	else if (init[0] == CString("select")) {
		if (init[3] == tableName) {

		}
	}
	else if (init[0] == CString("delete")) {
		if (init[2] == tableName) {

		}
	}
}


