#pragma once
#include <afxcview.h>

#define DBVIEW_DB_ITEM		0
#define DBVIEW_TABLE_ITEM	1

class CDBView :public CTreeView
{
	DECLARE_DYNCREATE(CDBView)

protected:
	CDBView();           // 动态创建所使用的受保护的构造函数
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

	HTREEITEM m_hCurrDBItem;	//当前选中的数据库名称
	HTREEITEM m_hCurrTBItem;	//当前表的数据库名称

	CTreeCtrl* m_pTreeCtrl;

	bool m_bAddDB;  //状态是否为添加数据库
	bool m_bAddTB;  //状态是否为添加表

	CString GetSelectedDBName();//得到当前被选择的数据库名
	CString GetSelectedTBName();//得到当前被选择的表名
	void DisplayDBList();		//显示数据库列表
	void GetDBAndTableName(CString& dbName, CString& tbName);
	
	afx_msg void OnCrtDB();
	afx_msg void OnDropDB();
	afx_msg void OnOpenDB();
	afx_msg void OnRenameDB();
	afx_msg void OnInputSql();
	afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnRefresh();
	afx_msg void OnCrtTable();
	afx_msg void OnbtnATable();
	afx_msg void OnbtnDTable();
	afx_msg void OnQueryFields();
	afx_msg void OnQueryTable();
};

