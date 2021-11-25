#include "pch.h"
#include "CDBView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DBOp.h"
#include "TableOp.h"
#include "ParseSQL.h"
#include "TableOp.h"

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
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDBView::OnTvnSelchanged)
	
END_MESSAGE_MAP()


// CDBView ���

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


// CDBView ��Ϣ�������


void CDBView::OnInitialUpdate()
{

	// TODO: �ڴ����ר�ô����/����û���
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

	//��ʼ��ΪNULL
	m_hCurrDBItem = NULL;
	m_hCurrTBItem = NULL;

	//��ʾ���е����ݿ��б�
	this->DisplayDBList();

	m_bAddDB = FALSE;
	m_bAddTB = FALSE;


}

void CDBView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	//MessageBox(CUtil::IntegerToString(m_pTreeCtrl->GetItemData(hItem)),L"",MB_OK);
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)
	{
		m_hCurrDBItem = hItem;

	}

	*pResult = 1;
}
CString CDBView::GetSelectedDBName() {
	m_hCurrDBItem = GetTreeCtrl().GetSelectedItem();
	if (m_hCurrDBItem)
	{
		return GetTreeCtrl().GetItemText(m_hCurrDBItem);
	}
	else
	{
		//�����ǰû��ѡ��database���򷵻ص�һ�����ݿ���
		return GetTreeCtrl().GetItemText(m_pTreeCtrl->GetRootItem());;

	}
}

CString CDBView::GetSelectedTBName() {
	m_hCurrTBItem = GetTreeCtrl().GetSelectedItem();
	if (m_hCurrTBItem)
	{
		return GetTreeCtrl().GetItemText(m_hCurrTBItem);
	}
	else
	{
		return NULL;

	}
}
void CDBView::DisplayDBList()
{
	CDBOp dbOp;

	vector<CDBModel> dbList = dbOp.GetDatabaseList();
	//ɾ����״ͼ�е����ݿ���ʾ
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

//�������ݿ�
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

//ɾ�����ݿ�
void CDBView::OnDropDB() {
	CString dbName = this->GetSelectedDBName();
	if (MessageBox(CString("������ɾ�����ڸ����ݿ��������ݣ�ȷ��ɾ��") + dbName + CString("��"), CString("ɾ�����ݿ�"), MB_OKCANCEL) == IDOK)
	{
		//ִ��ɾ������
		CDBOp dbOp;
		int code = dbOp.DropDatabase(dbName);
		//���νṹ����
		this->DisplayDBList();
		
	}
}



//���νṹ�е��Խڵ�༭����ʱִ�иú���
void CDBView::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	CString name = pTVDispInfo->item.pszText;
	if (m_bAddDB)//״̬Ϊ�������ݿ�
	{
		if (name != "")
		{
			//����create��䣬����sql��䣬�������ݿ�
			CString crtSql = CString("create database ") + name;
			ParseSQL::getSql(crtSql);
			/*CDBOp dbOp;
			int code = dbOp.CreateDatabase(name);*/
		}
		//�������ݿ��б�
		this->DisplayDBList();

		m_bAddDB = FALSE;
	}
	if (m_bAddTB)//״̬Ϊ���ӱ�
	{
		//������䣬���ӱ����
		TableOp tbop;
		tbop.CreateTable(name, this->GetSelectedDBName());
		m_bAddTB = FALSE;
	}



	*pResult = 0;

}

//�����ݿ�
void CDBView::OnOpenDB() {

	//ɾ����List
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(m_hCurrDBItem);
	while (hChildItem != NULL)
	{
		hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		m_pTreeCtrl->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	//��õ�ǰ���ݿ����͸����ݿ������б��list
	CString currentDB = this->GetSelectedDBName();
	TableOp tbOp;
	vector<TableModel> tbList = tbOp.GetTableMess(currentDB);
	for (int i = 0; i < tbList.size(); i++)
	{
	 m_pTreeCtrl->SetItemData(m_pTreeCtrl->InsertItem(
	  //����
	  tbList[i].name, 1, 1, m_hCurrDBItem), DBVIEW_TABLE_ITEM);
	}
	m_pTreeCtrl->Expand(m_hCurrDBItem, TVE_EXPAND);
}

void CDBView::OnbtnCTable() {
	this->OnOpenDB();
	HTREEITEM hItem;
	hItem = m_pTreeCtrl->InsertItem(CString(""), GetTreeCtrl().GetSelectedItem());
	if (hItem != NULL)
	{
		m_bAddTB = TRUE;
		m_pTreeCtrl->SetItemData(hItem, DBVIEW_DB_ITEM);
		m_pTreeCtrl->EditLabel(hItem);
	}

}

void CDBView::OnbtnATable()
{
	// TODO: �ڴ���������������


}


void CDBView::OnbtnDTable()
{
	CString dbName = this->GetSelectedDBName();
	CString tbName = this->GetSelectedTBName();
	if (MessageBox(CString("������ɾ�����ڸñ������������ݣ�ȷ��ɾ��") + tbName + CString("��"), CString("ɾ����"), MB_OKCANCEL) == IDOK)
	{
		//ִ��ɾ������
		TableOp tbOp;
		tbOp.DropTable(tbName,dbName);
		//���νṹ����
		this->DisplayDBList();

	}

}