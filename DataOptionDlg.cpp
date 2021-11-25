// DataOption.cpp: 实现文件
//

#include "pch.h"
#include "myDBMS1.h"
#include "DataOptionDlg.h"
#include "afxdialogex.h"
#include<conio.h>


// DataOption 对话框

IMPLEMENT_DYNAMIC(DataOptionDlg, CDialogEx)

DataOptionDlg::DataOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

DataOptionDlg::~DataOptionDlg()
{
}

void DataOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_insert, Insert);
}


BEGIN_MESSAGE_MAP(DataOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &DataOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// DataOption 消息处理程序


void DataOptionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	
	GetDlgItem(IDC_insert)->GetWindowTextW(message);
	/*AllocConsole();
	_cprintf("%s", message);
	HANDLE hdlWrite = GetStdHandle(STD_OUTPUT_HANDLE); //取得控制台的句柄
	WriteConsole(hdlWrite, message, wcslen(message), NULL, NULL); //向DOS窗口里输出字符串
	FreeConsole();    //析0构函数*/
}
void DataOptionDlg::setMessage(CString &str)
{
	str = message;
}
