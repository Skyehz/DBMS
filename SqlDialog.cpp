// SqlDialog.cpp: 实现文件
//

#include "pch.h"
#include "myDBMS1.h"
#include "SqlDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "MainFrm.h"
#include "ParseSQL.h"

// SqlDialog 对话框

IMPLEMENT_DYNAMIC(SqlDialog, CDialogEx)

SqlDialog::SqlDialog(CWnd* pParent,CString &dbName /*=nullptr*/)
	: CDialogEx(ID_SQL, pParent),dbName(dbName)
{
	m_pTableView = (CTableView*)pParent;
}

SqlDialog::~SqlDialog()
{
}

void SqlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_sql);
}


BEGIN_MESSAGE_MAP(SqlDialog, CDialogEx)

	ON_BN_CLICKED(IDOK, &SqlDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// SqlDialog 消息处理程序

void SqlDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	CString cstr;
	GetDlgItem(IDC_EDIT1)->GetWindowText(cstr);
	ParseSQL parse;
	parse.setDB(dbName);
	parse.getSql(cstr);


	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->ClearTable();
	//FieldOp fieldop(GetSelectedDBName(), GetSelectedTBName());
	//pMainWnd->m_pTableView->DisplayFields(fieldop.queryFieldsModel(GetSelectedDBName(), GetSelectedTBName()));

}
