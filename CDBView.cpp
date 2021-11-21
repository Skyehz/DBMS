#include "pch.h"
#include "CDBView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DBOp.h"

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
	//this->DisplayDBList();

	m_bAddDB = FALSE;
	m_bAddTB = FALSE;


}


void CDBView::DisplayDBList()
{
	CDBOp dbLogic;
	vector<CDBModel> dbList = dbLogic.GetDatabaseList();
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
		//HTREEITEM hRoot = m_pTreeCtrl->InsertItem(ite->GetName(), 0, 0, TVI_ROOT, TVI_LAST);
		//m_pTreeCtrl->SetItemData(hRoot, DBVIEW_DB_ITEM);
	}

}

/*
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
*/

void CDBView::OnCrtDB()
{
	// TODO: �ڴ���������������
	//CCrtDBDialog crt;
	//if(crt.DoModal()==IDOK)
	//{
	//	//MessageBox(L"success to Get "+crt.m_eDBName,L"",MB_OK);
	//	CDBLogic dbLogic;
	//	int code = dbLogic.CreateDatabase(crt.m_eDBName);
	//	if(code==YES)
	//	{
	//		//�������ݿ��б�
	//		this->DisplayDBList();
	//	}
	//	else
	//	{
	//		MessageBox(CUtil::GetError(code),CString("����"),MB_OK);
	//	}
	//};

	HTREEITEM hItem;
	hItem = m_pTreeCtrl->InsertItem(CString(""), 0, 1, TVI_ROOT, TVI_LAST);
	if (hItem != NULL)
	{
		m_bAddDB = TRUE;
		m_pTreeCtrl->SetItemData(hItem, DBVIEW_DB_ITEM);
		m_pTreeCtrl->EditLabel(hItem);
	}

}

/*
void CDBView::OnOpenDB()
{
	// TODO: �ڴ���������������

	//ɾ����List
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(m_hCurrDBItem);
	while (hChildItem != NULL)
	{
		hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		m_pTreeCtrl->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	CTableLogic tbLogic(this->GetSelectedDBName());
	vector<CTableEntity> tbList = tbLogic.GetTableList();
	for (int i = 0; i < tbList.size(); i++)
	{
		m_pTreeCtrl->SetItemData(m_pTreeCtrl->InsertItem(
			tbList[i].GetName(), 1, 1, m_hCurrDBItem), DBVIEW_TABLE_ITEM);
	}
	m_pTreeCtrl->Expand(m_hCurrDBItem, TVE_EXPAND);
}


void CDBView::OnDeleteDB()
{
	// TODO: �ڴ���������������

	CString dbName = this->GetSelectedDBName();
	if (MessageBox(CString("������ɾ�����ڸ����ݿ��������ݣ�ȷ��ɾ��") + dbName + CString("��"), CString("ɾ�����ݿ�"), MB_OKCANCEL) == IDOK)
	{
		//ִ��ɾ������
		CDBLogic dbLogic;
		int code = dbLogic.DeleteDatabase(dbName);
		if (code == YES)
		{
			//���νṹ����
			this->DisplayDBList();
		}
		else
		{
			MessageBox(CUtil::GetError(code), CString("����"), MB_OK);
		}
	}
}


void CDBView::OnCrtTable()
{
	// TODO: �ڴ���������������
	// ͬʱ�����ݿ�
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


CString CDBView::GetSelectedDBName()
{
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

CString CDBView::GetSelectedTBName()
{
	if (this->m_hCurrTBItem)
		return m_pTreeCtrl->GetItemText(m_hCurrTBItem);
	else
		return NULL;
}



/****************************
* �鿴���¼
****************************/
/*
void CDBView::OnLookTable()
{
	// TODO: �ڴ���������������
	CRecordLogic rcdLogic(GetSelectedDBName(), GetSelectedTBName());
	vector<CRecordEntity> rcdList = rcdLogic.GetRecordList();
	//��ձ�
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->m_pTableView->ClearTable();
	CFieldLogic fieldLogic(GetSelectedDBName(), GetSelectedTBName());
	//ˢ����ʾ��¼��
	pMainWnd->m_pTableView->DisplayRecords(rcdList, fieldLogic.GetFieldList());


}


/****************************
* ���ն��ֶ�������ѯ��¼
****************************/
/*
void CDBView::OnConditionQuery()
{
	// TODO: �ڴ���������������
	//CString tbName = m_pTreeCtrl->GetItemText(m_pTreeCtrl->GetSelectedItem());
	////CRecordLogic rcdLogic(this->GetSelectedDBName(),tbName);
	//
	//CTableLogic tbLogic(this->GetSelectedDBName());

	//vector<CFieldEntity> fieldList;
	//map<CString,CString> condition;
	//vector<CRecordEntity> rcdList=tbLogic.ConditionQuery(condition,tbName,fieldList);
	//for (vector<CRecordEntity>::iterator ite = rcdList.begin(); ite != rcdList.end(); ++ite)
	//{
	//	//����ļ�¼��ʾ�ڱ��


	//}

	CConditionQuery query(AfxGetMainWnd());
	query.DoModal();

}


void CDBView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	//MessageBox(CUtil::IntegerToString(m_pTreeCtrl->GetItemData(hItem)),L"",MB_OK);
	if (m_pTreeCtrl->GetItemData(hItem) == DBVIEW_DB_ITEM)          // ���ݿ�ڵ�
	{
		m_hCurrDBItem = hItem;


		// �����ֶκͼ�¼�˵���ˢ�²˵���
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
		GetParentFrame()->DrawMenuBar();

		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
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

		GetParentFrame()->GetMenu()->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		((CMainFrame*)GetParentFrame())->m_pTableView->ClearTable();
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		GetParentFrame()->DrawMenuBar();

		m_hCurrTBItem = hItem;
		m_hCurrDBItem = m_pTreeCtrl->GetParentItem(m_hCurrTBItem);
		//��ʾ��ǰ����ֶ���Ϣ
		this->OnDesignTable();
	}

	*pResult = 1;

}
*/
/*
void CDBView::OnDeleteTable()
{
	// TODO: �ڴ���������������
	CString tbName = this->GetSelectedTBName();
	if (MessageBox(CString("��ȷ��Ҫɾ����") + tbName + CString("���⽫ɾ���ñ������\n��¼�ͽṹ��"), CString("ɾ����"), MB_OKCANCEL) == IDOK)
	{
		CTableLogic tbLogic(this->GetSelectedDBName());
		if (tbName)
		{
			int code = tbLogic.DeleteTable(tbName);
			if (code == YES)
			{
				this->OnOpenDB();
			}
			else
				MessageBox(CUtil::GetError(code), CString("����"), MB_OK);
		}
	}

}


void CDBView::OnDesignTable()
{
	// TODO: �ڴ���������������
	CTableView* tbView = ((CMainFrame*)GetParentFrame())->m_pTableView;
	CFieldLogic fidLogic(this->GetSelectedDBName(), this->GetSelectedTBName());
	vector<CFieldEntity> fieldList = fidLogic.GetFieldList();
	tbView->DisplayFields(fieldList);
}


void CDBView::GetDBAndTableName(CString& dbName, CString& tbName)
{
	dbName = this->GetSelectedDBName();
	tbName = this->GetSelectedTBName();
}


void CDBView::OnRenameDB()
{
	// TODO: �ڴ���������������
	//CDBLogic dbLogic;
	//CString oldDBName = GetSelectedDBName();
	//dbLogic.ModifyDBName(oldDBName,);
	m_pTreeCtrl->EditLabel(m_pTreeCtrl->GetSelectedItem());
}


void CDBView::OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//���νṹ�е��Խڵ�༭����ʱִ�иú���
void CDBView::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
	CString name = pTVDispInfo->item.pszText;
	if (m_bAddDB)//״̬Ϊ����
	{
		if (name != "")
		{
			CDBLogic dbLogic;
			int code = dbLogic.CreateDatabase(name);
			if (code != YES)
				MessageBox(CUtil::GetError(code), CString("����"), MB_OK);
		}
		//�������ݿ��б�
		this->DisplayDBList();

		m_bAddDB = FALSE;
	}
	else if (m_bAddTB)
	{
		if (name != "")
		{
			CString dbName = this->GetSelectedDBName();
			CTableLogic tbLogic(dbName);
			int code = tbLogic.CreateTable(name);
			if (code != YES)
				MessageBox(CUtil::GetError(code), CString("����"), MB_OK);
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
				if (CUtil::ValidStr(newName))
				{
					CString oldName = GetSelectedDBName();
					CDBLogic dbLogic;
					if (dbLogic.ModifyDBName(oldName, newName))
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
				if (CUtil::ValidStr(newName))
				{
					CString oldName = GetSelectedTBName();
					CTableLogic tbLogic(GetSelectedDBName());
					if (tbLogic.ModifyTBName(oldName, newName))
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


//�����ݿ�������һ�ű�
void CDBView::OnAddTable()
{
	// TODO:  �ڴ���������������
	this->OnCrtTable();
}


//����ָ�������ݿ�
void CDBView::OnBackupDB()
{
	// TODO: �ڴ���������������
	CString m_FileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH];
	//szFolder[0] = _T('/0');
	memset(szFolder, '\0', sizeof(szFolder));
	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
			bRet = TRUE;
		IMalloc* pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
	}
	m_FileDir = szFolder;//ѡ����ļ���·��
	if (m_FileDir != L"")
	{
		CDBLogic dbLogic;
		if (dbLogic.BackupDatabase(GetSelectedDBName(), m_FileDir))
			MessageBox(L"���ݳɹ���", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		else
			MessageBox(L"����ʧ�ܣ�", L"��ʾ", MB_OK | MB_ICONWARNING);
	}


}

//�����ݿ��ļ����뵽�������¿�
void CDBView::OnRecoveryDB()
{
	// TODO: �ڴ���������������
	CDBLogic dbLogic;
	CTableLogic tbLogic(GetSelectedDBName());
	vector<CTableEntity> list = tbLogic.GetTableList();
	if (list.empty())
	{
		CString m_FileDir;
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = m_hWnd;
		//bi.ulFlags   = BIF_RETURNONLYFSDIRS;
		bi.ulFlags = BIF_BROWSEINCLUDEFILES;
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
		BOOL bRet = FALSE;
		TCHAR szFolder[MAX_PATH];
		//szFolder[0] = _T('/0');
		memset(szFolder, '\0', sizeof(szFolder));
		if (pidl)
		{
			if (SHGetPathFromIDList(pidl, szFolder))
				bRet = TRUE;
			IMalloc* pMalloc = NULL;
			if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
			{
				pMalloc->Free(pidl);
				pMalloc->Release();
			}
		}
		m_FileDir = szFolder;//ѡ����ļ���·��
		//94
		if (m_FileDir)
		{
			dbLogic.ImportDatabase(m_FileDir, GetSelectedDBName());
			this->OnOpenDB();
		}


	}
	else
		MessageBox(L"������Ŀ�����ǿյ����ݿ⣡", L"��ʾ", MB_OK);
}

//������е�����
void CDBView::OnClearTable()
{
	// TODO: �ڴ���������������
	if (MessageBox(L"ɾ���ñ����м�¼��ȷ��������", L"��ձ�", MB_OKCANCEL) == IDOK)
	{
		CRecordLogic rcdLogic(GetSelectedDBName(), GetSelectedTBName());
		int code = rcdLogic.DeleteAllRecord();
		if (code == YES)
			this->OnLookTable();
		else
			MessageBox(CUtil::GetError(code), CString("����"), MB_OK);;
	}


}


void CDBView::OnLookLog()
{
	// TODO: �ڴ���������������
	CSystemLogic sysLogic;
	sysLogic.LookLog();
}
*/