#pragma once
#include "FieldModel.h"
#include "FieldOp.h"
#include "TableView.h"
// FieldDialog 对话框
#define OPERATE_ADD 1		//增加操作
#define OPERATE_MODIFY 2	//修改操作
class FieldDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FieldDialog)

public:
	FieldDialog(CWnd* pParent, CString& dbname, CString& tbName, FieldModel& fieldModel, int type);     // 标准构造函数
	virtual ~FieldDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	CString m_dbName;
	CString m_tbName;
	int m_iType;
	FieldModel m_fieldEntity;
	FieldModel m_NewField;
	CTableView* m_pTableView;
	CString fieldName;
	CString fieldLength;

	CComboBox m_combType;
	int m_iSelType;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void AddField();

	void ModifyField();

};
