#include "pch.h"
#include "CDBView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DBOp.h"
#include "ParseSQL.h"
#include "TableModel.h"
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
	ON_COMMAND(ID_32771, &CDBView::OnCrtDB)
	ON_COMMAND(ID_32772, &CDBView::OnOpenDB)
	ON_COMMAND(ID_32774, &CDBView::OnDropDB)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CDBView::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDBView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CDBView::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CDBView::OnTvnBeginlabeledit)
	ON_COMMAND(ID_32788, &CDBView::OnRefresh)
	ON_COMMAND(ID_32775, &CDBView::OnCrtTable)
	ON_COMMAND(ID_32776, &CDBView::OnbtnATable)
	ON_COMMAND(ID_32777, &CDBView::OnbtnDTable)
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

CString CDBView::GetSelectedDBName() {
	//m_hCurrDBItem= GetTreeCtrl().GetSelectedItem();
	if (m_hCurrDBItem)
		return GetTreeCtrl().GetItemText(m_hCurrDBItem);
	else
		//�����ǰû��ѡ��database���򷵻ص�һ�����ݿ���
		return GetTreeCtrl().GetItemText(m_pTreeCtrl->GetRootItem());;
}

CString CDBView::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
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
		/*CDBOp dbOp;
		int code = dbOp.DropDatabase(dbName);*/
		CString crtSql = CString("drop database ") + dbName;
		ParseSQL::getSql(crtSql);
		//���νṹ����
		this->DisplayDBList();
		
	}
}

//���������ݿ�
void CDBView::OnRenameDB() {
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
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
	else if (m_bAddTB)//״̬Ϊ���ӱ�
	{


		//������䣬���ӱ����
		if (name != "")
		{
			TableOp tbOp;
			tbOp.CreateTable(name, GetSelectedDBName());
		}
		//�������ݿ��б�
		this->OnOpenDB();

		m_bAddTB = FALSE;
	}
	else
	{
		CString newName = pTVDispInfo->item.pszText;
		if (hItem == m_hCurrDBItem) //���ݿ�ڵ�
		{
			if (newName != GetSelectedDBName() && newName.GetLength() > 0)
			{
				if (newName == newName.SpanExcluding(L"!@#$%^&*()_+-={}[]:\";'<>?/"))
				{
					CString oldName = GetSelectedDBName();

					CDBOp dbOp;
					if (dbOp.ModifyDBName(oldName, newName))
					{
						m_pTreeCtrl->SetItemText(hItem, newName);
					}
					else
					{
						MessageBox(L"���������ݿ�ʧ�ܣ�", L"����", MB_OK | MB_ICONWARNING);
					}

				}
				else
					MessageBox(L"���Ϸ��ַ���", L"����", MB_OK | MB_ICONWARNING);
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
					
					TableOp tbOp;
					if (tbOp.AlterTable(oldName, newName, GetSelectedDBName()))
					{
						m_pTreeCtrl->SetItemText(hItem, newName);
					}
					else
					{
						MessageBox(L"��������ʧ�ܣ�", L"����", MB_OK | MB_ICONWARNING);
					}

				}
				else
					MessageBox(L"���Ϸ��ַ���", L"����", MB_OK | MB_ICONWARNING);
			}
		}
	}
	*pResult = 0;
}


void CDBView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDBView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	//MessageBox(CUtil::IntegerToString(m_pTreeCtrl->GetItemData(hItem)),L"",MB_OK);
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // ���ݿ�ڵ�
	{
		m_hCurrDBItem = hItem;


		//// �����ֶκͼ�¼�˵���ˢ�²˵���
		//GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->DrawMenuBar();

		//((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
	}
	else if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_TABLE_ITEM)   // ��ڵ�
	{
		// �������ת��������ֶκͼ�¼�˵���ˢ�²˵���
		//if (m_hCurrTBItem == hItem)
		//{
			//GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
			//GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
			//((CMainFrame *)GetParentFrame())->m_pTableView->ClearTable();
		//}

		//GetParentFrame()->GetMenu()->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
		//GetParentFrame()->GetMenu()->GetSubMenu(1)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED);

		/*
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->DrawMenuBar();
		*/
		m_hCurrTBItem = hItem;
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);
		//��ʾ��ǰ����ֶ���Ϣ
		//this->OnDesignTable();
	}

	*pResult = 1;
}



void CDBView::OnRefresh()
{
	// TODO: �ڴ���������������
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

//�޸ı���
void CDBView::OnbtnATable()
{
	// TODO: �ڴ���������������
	//this->OnCrtTable();
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
}



void CDBView::OnbtnDTable()
{
	// TODO: �ڴ���������������
	CString dbName = this->GetSelectedDBName();
	CString tbName = this->GetSelectedTBName();
	if (MessageBox(CString("������ɾ�����ڸñ������������ݣ�ȷ��ɾ��") + tbName + CString("��"), CString("ɾ����"), MB_OKCANCEL) == IDOK)
	{
		//ִ��ɾ������
		TableOp tbOp;
		tbOp.DropTable(tbName, dbName);
		//���νṹ����
		this->DisplayDBList();

	}
}
