#pragma once
#include <afxcview.h>

#define DBVIEW_DB_ITEM		0
#define DBVIEW_TABLE_ITEM	1

class CDBView :public CTreeView
{
	DECLARE_DYNCREATE(CDBView)

protected:
	CDBView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDBView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
private:
	CImageList m_treeImageList;
public:

	HTREEITEM m_hCurrDBItem;
	HTREEITEM m_hCurrTBItem;

	CTreeCtrl* m_pTreeCtrl;

	bool m_bAddDB;  //״̬�Ƿ�Ϊ������ݿ�
	bool m_bAddTB;  //״̬�Ƿ�Ϊ��ӱ�

	CString GetSelectedDBName();//�õ���ǰ��ѡ������ݿ���
	CString GetSelectedTBName();//�õ���ǰ��ѡ��ı���
	void DisplayDBList();		//��ʾ���ݿ��б�

	void GetDBAndTableName(CString& dbName, CString& tbName);

	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCrtDB();
	afx_msg void OnOpenDB();
	afx_msg void OnDeleteDB();
	afx_msg void OnCrtTable();
	afx_msg void OnLookTable();
	afx_msg void OnConditionQuery();
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteTable();
	afx_msg void OnDesignTable();
	afx_msg void OnRenameDB();
	afx_msg void OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddTable();
	afx_msg void OnBackupDB();
	afx_msg void OnRecoveryDB();
	afx_msg void OnClearTable();
	afx_msg void OnLookLog();

};

