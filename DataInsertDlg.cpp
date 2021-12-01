// DataInsertDlg.cpp: 实现文件
#include "pch.h"
#include "myDBMS1.h"
#include "DataInsertDlg.h"
#include "afxdialogex.h"
#include "ParseSQL.h"

// DataInsertDlg 对话框

IMPLEMENT_DYNAMIC(DataInsertDlg, CDialogEx)


DataInsertDlg::DataInsertDlg(CWnd* pParent, vector<FieldModel>& fieldList, CString& dbName, CString& tbName, int operType, vector<CString>& rcd)
	: CDialogEx(IDD_INSERT, pParent)
{
	this->m_pTableView = (CTableView*)pParent;
	this->m_vFieldList = fieldList;
	this->m_sDBName = dbName;
	this->m_sTableName = tbName;
	this->m_iOperType = operType;
	this->m_rcd = rcd;
}

DataInsertDlg::~DataInsertDlg()
{

}

void DataInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST2, m_ctllist);
	DDX_Control(pDX, IDC_LIST1, m_listCtl);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(DataInsertDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &DataInsertDlg::OnNMClickList)
	ON_BN_CLICKED(IDOK, &DataInsertDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT1, &DataInsertDlg::OnEnKillfocusEdit)
END_MESSAGE_MAP()


// DataInsertDlg 消息处理程序
BOOL DataInsertDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	if (m_iOperType == OPERATE_ADD)
		this->SetWindowTextW(_T("添加记录"));
	else
		this->SetWindowTextW(_T("修改记录"));

	m_edit.ShowWindow(false);//将输入框设置为不可见

	LONG lStyle;
	lStyle = GetWindowLong(m_listCtl.m_hWnd, GWL_STYLE);  // 获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK;                              // 清除显示方式位
	lStyle |= LVS_REPORT;
	SetWindowLong(m_listCtl.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_listCtl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;                      // 选中某行使整行高亮
	dwStyle |= LVS_EX_GRIDLINES;                          // 网格线
	m_listCtl.SetExtendedStyle(dwStyle);                  // 设置扩展风格

	m_listCtl.InsertColumn(0, CString("字段名"), LVCFMT_LEFT, 70);
	m_listCtl.InsertColumn(1, CString("类型"), LVCFMT_LEFT, 70);
	m_listCtl.InsertColumn(2, CString("主键"), LVCFMT_LEFT, 50);
	m_listCtl.InsertColumn(3, CString("允许空"), LVCFMT_LEFT, 50);
	m_listCtl.InsertColumn(4, CString("唯一"), LVCFMT_LEFT, 50);
	m_listCtl.InsertColumn(5, CString("值"), LVCFMT_LEFT, 100);
	//for (int i = 0; i < 5; ++i)
	//{
	//	m_listCtl.InsertItem(i, _T("name"));
	//	m_listCtl.SetItemText(i, 1, _T("name"));
	//	m_listCtl.SetItemText(i, 2, _T("name"));
	//	m_listCtl.SetItemText(i, 3, _T("name"));
	//	m_listCtl.SetItemText(i, 4, _T("name"));
	//	//m_listCtl.SetItemText(i, 5, m_rcd[i + 1]);
	//	/*if (m_iOperType == OPERATE_ADD)
	//		m_listCtl.SetItemText(i, 5, m_vFieldList[i].GetDefault());
	//	else
	//		m_listCtl.SetItemText(i, 5, m_rcd[i + 1]);*/
	//}
	for (int i = 0; i < m_vFieldList.size(); ++i)
	{
		m_listCtl.InsertItem(i, m_vFieldList[i].GetName());
		m_listCtl.SetItemText(i, 1, FileOp::GetTypeCString(m_vFieldList[i].GetType()));
		m_listCtl.SetItemText(i, 2, FileOp::BoolToString(m_vFieldList[i].IsPrimaryKey()));
		m_listCtl.SetItemText(i, 3, FileOp::BoolToString(m_vFieldList[i].IsEmpty()));
		m_listCtl.SetItemText(i, 4, FileOp::BoolToString(m_vFieldList[i].IsUniqueKey()));
		if (m_iOperType == OPERATE_ADD)
			m_listCtl.SetItemText(i, 5, m_vFieldList[i].GetDefaultValue());
		else
			m_listCtl.SetItemText(i, 5, m_rcd[i + 1]);

	}

	//初始值设置为5
	m_nColumn = 5;


	return TRUE;
}

void DataInsertDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_record = new CDataModel();
	bool isOK = true;

	/*if (isOK)
	{
		m_record->SetId(FileOp::StringToInteger(m_rcd[0]));
	}*/
	CDataOp dataOp(m_sDBName, m_sTableName);

	//bool isOK = true;
	//验证合法性
	for (int i = 0; i < m_vFieldList.size(); ++i)
	{
		CString val = m_listCtl.GetItemText(i, m_nColumn);
		m_record->SetValue(m_vFieldList[i].GetName(), val);
	}
	if (isOK)
	{
		if (m_iOperType == OPERATE_ADD)
		{
			//插入数据
			//dataOp.AddRecord(*m_record, m_vFieldList);
			CString record;
			for (vector<FieldModel>::iterator ite = m_vFieldList.begin(); ite != m_vFieldList.end(); ++ite)
			{
				record += m_record->GetValue(ite->GetName())+ CString(",");
			}
			record = record.Left(record.GetLength() - 1);
			CString statement;
			statement = CString("insert into ") + m_sTableName + CString(" values") + CString("(") + record+ CString(");");
			ParseSQL parseSql;
			parseSql.setDB(m_sDBName);
			parseSql.getSql(statement);
			
			RefreshRecord();
		}
		else
		{
			m_record->SetId(FileOp::StringToInteger(m_rcd[0]));
			//修改数据
			//dataOp.ModifyRecord(*m_record);
			CString record;
			CString statement;
			for (vector<FieldModel>::iterator ite = m_vFieldList.begin(); ite != m_vFieldList.end(); ++ite)
			{
				record = m_record->GetValue(ite->GetName());
				statement = CString("update ") + m_sTableName + CString(" set ") + ite->GetName()+CString("=")+record+CString(" where ")+CString("#=")+FileOp::IntegerToString(m_record->GetId()) + CString(";");
			}
			record = record.Left(record.GetLength() - 1);
			
			ParseSQL parseSql;
			parseSql.setDB(m_sDBName);
			parseSql.getSql(statement);
			RefreshRecord();
		}



	}
	CDialogEx::OnOK();

}

void DataInsertDlg::OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nRow = pNMListView->iItem;
	m_nColumn = pNMListView->iSubItem;
	DWORD nCount = m_listCtl.GetItemCount();

	if (m_nRow >= 0 && m_nColumn == 5 && m_nRow <= nCount)
	{
		// 设置接收框到列表对应位置
		CRect rtEdit, rtClient;
		m_listCtl.GetWindowRect(rtClient);
		ScreenToClient(rtClient);
		m_listCtl.GetSubItemRect(m_nRow, m_nColumn, LVIR_LABEL, rtEdit);
		rtEdit.left += rtClient.left + 0;
		rtEdit.top += rtClient.top + 0;
		rtEdit.bottom += 0;

		CString strTemp = m_listCtl.GetItemText(m_nRow, m_nColumn);
		int nLength = strTemp.GetLength();
		m_edit.SetWindowText(strTemp);
		m_edit.SetWindowPos(&wndTop, rtEdit.left, rtEdit.top, 100, 20, SWP_SHOWWINDOW);
		m_edit.SetSel(nLength, nLength);
		m_edit.ShowWindow(TRUE);
		m_edit.SetFocus();
	}

	*pResult = 0;
}
void DataInsertDlg::OnEnKillfocusEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strValue;
	m_edit.GetWindowText(strValue);
	m_edit.ShowWindow(SW_HIDE);

	//把用户输入的内容同步到列表上
	m_listCtl.SetItemText(m_nRow, m_nColumn, strValue);
}

void DataInsertDlg::RefreshRecord() {
	
	FieldOp fieldop(m_sDBName, m_sTableName);//获取字段
	vector<FieldModel> fieldList = fieldop.queryFieldsModel(m_sDBName, m_sTableName);
	CDataOp dataop(m_sDBName, m_sTableName);
	vector<CDataModel> dataList = dataop.ReadDataList(fieldList);	//获取记录
	//清空表
	m_pTableView->ClearTable();
	m_pTableView->DisplayRecords(dataList, fieldList);
	
}