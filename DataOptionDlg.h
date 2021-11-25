#pragma once


// DataOption 对话框

class DataOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DataOptionDlg)

public:
	DataOptionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DataOptionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	CEdit Insert;
	afx_msg void OnBnClickedOk();
	CEdit test;
	CString message;
	void setMessage(CString &str);
};
