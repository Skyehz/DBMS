#pragma once
#include "DataModel.h"
#include <vector>
#include "TableView.h"
#include "FileOp.h"
#include "FieldDialog.h"
#include "DataOp.h"

// DataInsertDlg 对话框

class DataInsertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DataInsertDlg)

public:
	DataInsertDlg(CWnd* pParent, vector<FieldModel>& fieldList, CString& dbName, CString& tbName, int operType, vector<CString>& rcd);   // 标准构造函数
	virtual ~DataInsertDlg();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CDataModel* m_datamodel;

	int m_iOperType;
	//vector<CString> m_rcd;
	CListCtrl m_listCtl;
	CTableView* m_pTableView;
	vector<FieldModel> m_vFieldList;
	CDataModel* m_record;

	vector<CString> m_rcd;

	CString m_sDBName;
	CString m_sTableName;
	afx_msg void OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnKillfocusEdit();
	void RefreshRecord();	//刷新表
	CEdit m_edit;
	int m_nRow;
	int m_nColumn;
};
