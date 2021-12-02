#include "pch.h"
#include "CDBView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DBOp.h"
#include "DataOp.h"
#include "DataModel.h"
#include "ParseSQL.h"
#include "TableModel.h"
#include "TableOp.h"
#include "FieldOp.h"
#include "SqlDialog.h"
#include"LogOp.h"
// CDBView

IMPLEMENT_DYNCREATE(CDBView, CTreeView)

CDBView::CDBView()
{

}

CDBView::~CDBView()
{
}

BEGIN_MESSAGE_MAP(CDBView, CTreeView)
	ON_COMMAND(ID_32771, &CDBView::OnCrtDB)
	ON_COMMAND(ID_32772, &CDBView::OnOpenDB)
	ON_COMMAND(ID_32774, &CDBView::OnDropDB)

	ON_COMMAND(ID_32791, &CDBView::OnInputSql)

	ON_NOTIFY_REFLECT(NM_RCLICK, &CDBView::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDBView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CDBView::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CDBView::OnTvnBeginlabeledit)
	ON_COMMAND(ID_32788, &CDBView::OnRefresh)
	ON_COMMAND(ID_32775, &CDBView::OnCrtTable)
	ON_COMMAND(ID_32776, &CDBView::OnbtnATable)
	ON_COMMAND(ID_32777, &CDBView::OnbtnDTable)
	ON_COMMAND(ID_32781, &CDBView::OnQueryFields)
	ON_COMMAND(ID_32792, &CDBView::OnQueryTable)
	ON_COMMAND(ID_32793, &CDBView::OnBackup)
	ON_COMMAND(ID_32794, &CDBView::OnRestore)
END_MESSAGE_MAP()


// CDBView 诊断

#ifdef _DEBUG
void CDBView::AssertValid() const
{
	CTreeView::AssertValid();
	CString ss;
}

#ifndef _WIN32_WCE
void CDBView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBView 消息处理程序


void CDBView::OnInitialUpdate()
{

	// TODO: 在此添加专用代码和/或调用基类
	CTreeView::OnInitialUpdate();

	//Get the treeCtrl associated with CTreeView
	m_pTreeCtrl = &GetTreeCtrl();

	DWORD dwStyles = GetWindowLong(m_pTreeCtrl->m_hWnd, GWL_STYLE);
	dwStyles |= TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_pTreeCtrl->m_hWnd, GWL_STYLE, dwStyles);


	//Associate ICON with the control
	m_treeImageList.Create(16, 16, ILC_MASK, 4, 1);
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDR_myDBMS1TYPE));
	m_treeImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_pTreeCtrl->SetImageList(&m_treeImageList, TVSIL_NORMAL);

	//初始化为NULL
	m_hCurrDBItem = NULL;
	m_hCurrTBItem = NULL;

	//显示已有的数据库列表
	this->DisplayDBList();

	m_bAddDB = FALSE;
	m_bAddTB = FALSE;


}


void CDBView::OnInputSql()
{
	// TODO: 在此添加命令处理程序代码
	SqlDialog sqlDialog(NULL,GetSelectedDBName());
	sqlDialog.DoModal();
}


CString CDBView::GetSelectedDBName() {
	//m_hCurrDBItem= GetTreeCtrl().GetSelectedItem();
	if (m_hCurrDBItem)
		return GetTreeCtrl().GetItemText(m_hCurrDBItem);
	else
		//如果当前没有选择database，则返回第一个数据库名
		return GetTreeCtrl().GetItemText(m_pTreeCtrl->GetRootItem());;
}

CString CDBView::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
}
void CDBView::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}

void CDBView::DisplayDBList()
{
	CDBOp dbOp;
	vector<CDBModel> dbList = dbOp.GetDatabaseList();
	//删除树状图中的数据库显示
	HTREEITEM hNextItem;
	HTREEITEM hPreItem = m_pTreeCtrl->GetRootItem();
	while (hPreItem != NULL)
	{
		hNextItem = m_pTreeCtrl->GetNextItem(hPreItem, TVGN_NEXT);
		m_pTreeCtrl->DeleteItem(hPreItem);
		hPreItem = hNextItem;
	}

	for (vector<CDBModel>::iterator ite = dbList.begin(); ite != dbList.end(); ++ite)
	{
		//MessageBox(CUtil::IntegerToString(ite->GetId()),L"",MB_OK);
		HTREEITEM hRoot = m_pTreeCtrl->InsertItem(ite->GetName(), 0, 0, TVI_ROOT, TVI_LAST);
		m_pTreeCtrl->SetItemData(hRoot, DBVIEW_DB_ITEM);
	}

}

//创建数据库
void CDBView::OnCrtDB()
{
	HTREEITEM hItem;
	hItem = m_pTreeCtrl->InsertItem(CString(""), 0, 1, TVI_ROOT, TVI_LAST);
	if (hItem != NULL)
	{
		m_bAddDB = TRUE;
		m_pTreeCtrl->SetItemData(hItem, DBVIEW_DB_ITEM);
		m_pTreeCtrl->EditLabel(hItem);
	}
}

//删除数据库
void CDBView::OnDropDB() {
	CString dbName = this->GetSelectedDBName();
	if (MessageBox(CString("操作会删除关于该数据库所有数据，确定删除") + dbName + CString("？"), CString("删除数据库"), MB_OKCANCEL) == IDOK)
	{
		//执行删除操作
		/*CDBOp dbOp;
		int code = dbOp.DropDatabase(dbName);*/
		CString Sql = CString("drop database ") + dbName;
		ParseSQL parse;
		parse.getSql(Sql);
		//树形结构更新
		this->DisplayDBList();
		
	}
}

//重命名数据库
void CDBView::OnRenameDB() {
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
}

//打开数据库
void CDBView::OnOpenDB() {

	//删除表List
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(m_hCurrDBItem);
	while (hChildItem != NULL)
	{
		hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		m_pTreeCtrl->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	//获得当前数据库名和该数据库下所有表的list
	TableOp tbOp;
	CString name = this->GetSelectedDBName();
	vector<TableModel> tbList = tbOp.GetTableMess(name);
	for (int i = 0; i < tbList.size(); i++)
	{
		m_pTreeCtrl->SetItemData(m_pTreeCtrl->InsertItem(
			tbList[i].name, 1, 1, m_hCurrDBItem), DBVIEW_TABLE_ITEM);
	}
	m_pTreeCtrl->Expand(m_hCurrDBItem, TVE_EXPAND);
}



//树形结构中当对节点编辑结束时执行该函数
void CDBView::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	CString name = pTVDispInfo->item.pszText;
	if (m_bAddDB)//状态为增加数据库
	{
		if (name != "")
		{
			//生成create语句，解析sql语句，创建数据库
			CString Sql = CString("create database ") + name;
			ParseSQL parse;
			parse.getSql(Sql);
			/*CDBOp dbOp;
			int code = dbOp.CreateDatabase(name);*/
		}
		//更新数据库列表
		this->DisplayDBList();

		m_bAddDB = FALSE;
	}
	else if (m_bAddTB)//状态为增加表
	{


		//生成语句，增加表操作
		if (name != "")
		{
			//TableOp tbOp;
			//tbOp.CreateTable(name, GetSelectedDBName());
			CString sql = CString("create table ") + name + CString(" (@ int)");
			ParseSQL p;
			p.setDB(GetSelectedDBName());
			p.getSql(sql);
		}
		//更新数据库列表
		this->OnOpenDB();

		m_bAddTB = FALSE;
	}
	else
	{
		CString newName = pTVDispInfo->item.pszText;
		if (hItem == m_hCurrDBItem) //数据库节点
		{
			if (newName != GetSelectedDBName() && newName.GetLength() > 0)	//不为空
			{
				if (newName == newName.SpanExcluding(L"!@#$%^&*()_+-={}[]:\";'<>?/"))	//满足命名规范
				{
					CString oldName = GetSelectedDBName();

					CDBOp dbOp;
					if (dbOp.ModifyDBName(oldName, newName))
					{
						m_pTreeCtrl->SetItemText(hItem, newName);
					}
					else
					{
						MessageBox(L"重命名数据库失败！", L"错误", MB_OK | MB_ICONWARNING);
					}

				}
				else
					MessageBox(L"不合法字符！", L"警告", MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			if (newName != GetSelectedTBName() && newName.GetLength() > 0)
			{
				//m_pTreeCtrl->SetItemText(hItem,newName);
				if (newName == newName.SpanExcluding(L"!@#$%^&*()_+-={}[]:\";'<>?/"))
				{
					CString oldName = GetSelectedTBName();
					CString sql = CString("rename table ") + oldName + " to " + newName;
					ParseSQL p;
					p.setDB(GetSelectedDBName());
					p.getSql(sql);
					if (true)
					{
						m_pTreeCtrl->SetItemText(hItem, newName);
					}
					else
					{
						MessageBox(L"重命名表失败！", L"错误", MB_OK | MB_ICONWARNING);
					}

				}
				else
					MessageBox(L"不合法字符！", L"警告", MB_OK | MB_ICONWARNING);
			}
		}
	}
	*pResult = 0;
}


void CDBView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint point(GetMessagePos());
	unsigned int nFlags = 0;
	m_pTreeCtrl->ScreenToClient(&point);
	HTREEITEM hItem = m_pTreeCtrl->HitTest(point, &nFlags);

	if ((nFlags & TVHT_ONITEM) && (hItem != NULL))
	{
		m_pTreeCtrl->SelectItem(hItem);
		CMenu MyMenu;
		MyMenu.LoadMenu(IDR_MAINFRAME);
		GetCursorPos(&point);
		if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)
		{
			CMenu* popup = MyMenu.GetSubMenu(1);
			popup->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)
		{
			CMenu* popup = MyMenu.GetSubMenu(2);
			popup->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
	*pResult = 0;
}
void CDBView::OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDBView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	//MessageBox(CUtil::IntegerToString(m_pTreeCtrl->GetItemData(hItem)),L"",MB_OK);
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // 数据库节点
	{
		m_hCurrDBItem = hItem;

	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)   // 表节点
	{
		
		m_hCurrTBItem = hItem;
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);
		//显示当前表的字段信息
		//this->OnDesignTable();
	}

	*pResult = 1;
}



void CDBView::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	this->DisplayDBList();
}


void CDBView::OnCrtTable()
{
	this->OnOpenDB();

	HTREEITEM hTableItem = m_pTreeCtrl->InsertItem(_T(""), 2, 2, m_hCurrDBItem, TVI_LAST);
	if (hTableItem != NULL)
	{
		m_pTreeCtrl->SetItemData(hTableItem, DBVIEW_TABLE_ITEM);
		m_pTreeCtrl->Expand(m_hCurrDBItem, TVE_EXPAND);
		m_bAddTB = TRUE;
		m_pTreeCtrl->EditLabel(hTableItem);
	}
}

//修改表名
void CDBView::OnbtnATable()
{
	// TODO: 在此添加命令处理程序代码
	//this->OnCrtTable();
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());

}



void CDBView::OnbtnDTable()
{
	// TODO: 在此添加命令处理程序代码
	CString dbName = this->GetSelectedDBName();
	CString tbName = this->GetSelectedTBName();
	if (MessageBox(CString("操作会删除关于该表及表中所有数据，确定删除") + tbName + CString("？"), CString("删除表"), MB_OKCANCEL) == IDOK)
	{
		//执行删除操作
		/*TableOp tbOp;
		tbOp.DropTable(tbName, dbName);*/
		ParseSQL p;
		p.setDB(dbName);
		CString sql = CString("drop table ") + tbName;
		p.getSql(sql);
		//树形结构更新
		this->DisplayDBList();

	}
}


void CDBView::OnQueryFields()
{
	// TODO: 在此添加命令处理程序代码
	//清空表
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	//pMainWnd->m_pTableView->ClearTable();
	FieldOp fieldop(GetSelectedDBName(),GetSelectedTBName());
	pMainWnd->m_pTableView->DisplayFields(fieldop.queryFieldsModel(GetSelectedDBName(), GetSelectedTBName()));
}


void CDBView::OnQueryTable()
{
	// TODO: 在此添加命令处理程序代码

	FieldOp fieldop(GetSelectedDBName(), GetSelectedTBName());//获取字段
	vector<FieldModel> fieldList = fieldop.queryFieldsModel(GetSelectedDBName(), GetSelectedTBName());
	CDataOp dataop(GetSelectedDBName(), GetSelectedTBName());
	vector<CDataModel> dataList = dataop.ReadDataList(fieldList);	//获取记录
	//清空表
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->ClearTable();


	//刷新显示记录表
	pMainWnd->m_pTableView->DisplayRecords(dataList, fieldList);
}


void CDBView::OnBackup()
{
	// TODO: 在此添加命令处理程序代码

	LogOp logOp(GetSelectedDBName());
	if (!logOp.BackupDB()) {
		MessageBox(L"备份失败");
	}
}


void CDBView::OnRestore()
{
	// TODO: 在此添加命令处理程序代码
	LogOp logOp(GetSelectedDBName());
	logOp.Restore();
}
