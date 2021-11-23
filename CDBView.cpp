#include "pch.h"
#include "CDBView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DBOp.h"
#include "ParseSQL.h"

// CDBView

IMPLEMENT_DYNCREATE(CDBView, CTreeView)

CDBView::CDBView()
{

}

CDBView::~CDBView()
{
}

BEGIN_MESSAGE_MAP(CDBView, CTreeView)
	//ON_NOTIFY_REFLECT(NM_RCLICK, &CDBView::OnNMRClick)
	//ON_COMMAND(ID_32771, &CDBView::OnCrtDB)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CDBView::OnTvnEndlabeledit)
	
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

CString CDBView::GetSelectedDBName() {
	CString dbName;

	return dbName;
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
		CDBOp dbOp;
		int code = dbOp.DropDatabase(dbName);
		//树形结构更新
		this->DisplayDBList();
		
	}
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
			CString crtSql = CString("create database ") + name;
			ParseSQL::getSql(crtSql);
			/*CDBOp dbOp;
			int code = dbOp.CreateDatabase(name);*/
		}
		//更新数据库列表
		this->DisplayDBList();

		m_bAddDB = FALSE;
	}
	if (m_bAddTB)//状态为增加表
	{
		//生成语句，增加表操作

		m_bAddTB = FALSE;
	}



	*pResult = 0;
}
