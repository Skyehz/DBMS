#pragma once
#include <afxcview.h>
#include"FieldModel.h"
#include<vector>
using namespace std;
#define TABLEVIEW_VALID  0
#define TABLEVIEW_FIELD  1
#define TABLEVIEW_RECORD 2

class CTableView : public CListView
{
	DECLARE_DYNCREATE(CTableView)

protected:
	CTableView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTableView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	CListCtrl* m_ListCtrl;
public:
	virtual void OnInitialUpdate();
	int m_curView;       //判断右边的表是字段还是记录

	int m_iRow;
	int m_iColumn;
	int m_iCount;

	//void DisplayRecords(vector<CString>& rcdlist, vector<FieldModel>& fieldList);
	void DisplayFields(vector<FieldModel>& fieldList);
	void ClearTable();
	afx_msg void OnAddField();
	afx_msg void OnDeleteField();
	afx_msg void OnModifyField();
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddRecord();
	afx_msg void OnDeleteRecord();
	afx_msg void OnModifyRecord();
};

