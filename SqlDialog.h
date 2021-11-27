#pragma once


// SqlDialog 对话框

class SqlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SqlDialog)

public:
	SqlDialog(CWnd* pParent, CString &dbName);   // 标准构造函数
	virtual ~SqlDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_SQL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString dbName;
	CEdit Edit_sql;
	afx_msg void OnBnClickedOk();
};
