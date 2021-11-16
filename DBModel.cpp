#include "pch.h"
#include "DBModel.h"

#pragma warning(disable:4996)
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>


#include <vector>
using namespace std;

CDBModel::CDBModel(int id, CString name , int type , CString filepath) :
	id(id), name(name), type(type), filepath(filepath)
{

	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	//תΪstring->CString
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%Y/%m%/d %H:%M:%S");
	std::string str_time = ss.str();
	crtime = str_time.c_str();
}

CDBModel::CDBModel(CString& str) {
	id = 0;
	name = str;
	type = 1;
	filepath = CString("data/dbms.sys");
/*	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//תΪstring->CString
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%Y/%m%/d %H:%M:%S");
	std::string str_time = ss.str();*/
	string str_time = "2021/11/15 22:46";
	crtime = str_time.c_str();

}

CDBModel::~CDBModel() {
}