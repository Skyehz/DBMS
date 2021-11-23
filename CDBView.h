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
	
	afx_msg void OnCrtDB();
	afx_msg void OnDropDB();
	afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

};

