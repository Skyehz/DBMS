#include "pch.h"
#include "TableView.h"
#include "Resource.h"
#include "FieldOp.h"
#include "vector"
#include "FieldDialog.h"
#include "DataInsertDlg.h"
#include "FileOp.h"
#include "MainFrm.h"
#include <stdlib.h>
using namespace std;
// CTableView

IMPLEMENT_DYNCREATE(CTableView, CListView)

CTableView::CTableView()
{

}

CTableView::~CTableView()
{
}

BEGIN_MESSAGE_MAP(CTableView, CListView)
	ON_COMMAND(ID_32778, &CTableView::OnAddField)
	ON_COMMAND(ID_32779, &CTableView::OnDeleteField)
	ON_COMMAND(ID_32780, &CTableView::OnModifyField)
	//ON_COMMAND(ID_32782, &CTableView::OnDeleteField)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CTableView::OnNMRClick)
	/*ON_COMMAND(ID_32783, &CTableView::OnAddRecord)
	ON_COMMAND(ID_32785, &CTableView::OnDeleteRecord)
	ON_COMMAND(ID_32784, &CTableView::OnModifyRecord)*/
	ON_COMMAND(ID_32782, &CTableView::OnAddRecord)
	ON_COMMAND(ID_32783, &CTableView::OnDeleteRecord)
	ON_COMMAND(ID_32784, &CTableView::OnModifyRecord)
END_MESSAGE_MAP()


// CTableView 诊断

#ifdef _DEBUG
void CTableView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CTableView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTableView 消息处理程序


void CTableView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_ListCtrl = &GetListCtrl();

	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE);   // 获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK;                              // 清除显示方式位
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCtrl->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;                      // 选中某行使整行高亮
	dwStyle |= LVS_EX_GRIDLINES;                          // 网格线
	m_ListCtrl->SetExtendedStyle(dwStyle);                   // 设置扩展风格

	//初始化的视图类型为非法视图
	m_curView = TABLEVIEW_VALID;

}

void CTableView::ClearTable()
{
	//清空表数据及表头
	m_ListCtrl->DeleteAllItems();
	while (m_ListCtrl->GetHeaderCtrl()->GetItemCount())
	{
		m_ListCtrl->DeleteColumn(0);
	}

}


void CTableView::DisplayRecords(vector<CDataModel>& rcdlist, vector<FieldModel>& fieldList)
{
	m_curView = TABLEVIEW_RECORD;	//视图类型设置为记录视图

	//清除表
	this->ClearTable();

	int columnNum = fieldList.size() + 1;
	//显示字段
	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 20);
	for (int i = 1; i < columnNum; i++)
	{
		m_ListCtrl->InsertColumn(i, fieldList[i - 1].GetName(), LVCFMT_LEFT, 150);
	}
	
	//显示记录
	int rcdNum = rcdlist.size();
	for (int i = 0; i < rcdNum; i++)
	{
		CString cstr;
		cstr.Format(_T("%d"), rcdlist[i].GetId());
		m_ListCtrl->InsertItem(i, cstr);
		m_ListCtrl->SetItemText(i, 0 , FileOp::IntegerToString(rcdlist[i].GetId()));
		for (int j = 1; j < columnNum; j++)
		{
			m_ListCtrl->SetItemText(i, j, rcdlist[i].GetValue(fieldList[j - 1].GetName()));
		}
	}
}


void CTableView::DisplayFields(vector<FieldModel>& fieldList)
{
	m_curView = TABLEVIEW_FIELD; //视图类型设置为字段视图

	this->ClearTable();

	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);
	m_ListCtrl->InsertColumn(1, CString("顺序"), LVCFMT_LEFT, 70);
	m_ListCtrl->InsertColumn(2, CString("字段名"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(3, CString("类型"), LVCFMT_LEFT, 80);
	m_ListCtrl->InsertColumn(4, CString("长度"), LVCFMT_LEFT, 50);
	/*m_ListCtrl->InsertColumn(5, CString("最小值"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(6, CString("最大值"), LVCFMT_LEFT, 100);*/
	m_ListCtrl->InsertColumn(5, CString("默认值"), LVCFMT_LEFT, 100);
	m_ListCtrl->InsertColumn(6, CString("主键"), LVCFMT_LEFT, 40);
	m_ListCtrl->InsertColumn(7, CString("允许空值"), LVCFMT_LEFT, 70);
	m_ListCtrl->InsertColumn(8, CString("唯一值"), LVCFMT_LEFT, 60);
	m_ListCtrl->InsertColumn(9, CString("注释"), LVCFMT_LEFT, 100);

	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);

	for (int i = 0; i < fieldList.size(); ++i)
	{
		m_ListCtrl->InsertItem(i, FileOp::IntegerToString(fieldList[i].GetId()));
		m_ListCtrl->SetItemText(i, 2, FileOp::IntegerToString(fieldList[i].GetId()));
		m_ListCtrl->SetItemText(i, 3, fieldList[i].GetName());
		m_ListCtrl->SetItemText(i, 4, FileOp::GetTypeCString(fieldList[i].GetType()));
		m_ListCtrl->SetItemText(i, 5,  FileOp::IntegerToString(fieldList[i].GetParam()));
	}
	
}

void CTableView::OnAddField()
{
	CString dbName, tbName;
	((CMainFrame*)GetParentFrame())->m_pDBView->GetDBAndTableName(dbName, tbName);

	FieldDialog addFieldDlg(this, dbName, tbName, FieldModel(), OPERATE_ADD);
	addFieldDlg.DoModal();
}

void CTableView::OnDeleteField()
{
	CString fieldName;
	POSITION pos = m_ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		if (MessageBox(CString("确定要删除该字段？"), CString("删除字段"), MB_OKCANCEL) == IDOK)
		{
			int nItem = m_ListCtrl->GetNextSelectedItem(pos);
			fieldName = m_ListCtrl->GetItemText(nItem, 3);

			CString dbName, tbName;
			((CMainFrame*)GetParentFrame())->m_pDBView->GetDBAndTableName(dbName, tbName);
			FieldOp fieldLogic(dbName, tbName);

			int code = fieldLogic.dropField(dbName, tbName,fieldName);
			if (code == 1)
			{
				vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(dbName, tbName);
				this->DisplayFields(fieldList);
			}
			else
			{
				MessageBox(CString("删除字段错误！"), CString("错误"), MB_OK);
			}
		}
	}
}

void CTableView::OnModifyField()
{
	POSITION pos = m_ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL) {
		if (/*m_iRow < m_iCount && m_iRow >= 0 &&*/ m_curView == TABLEVIEW_FIELD)
		{
			int nItem = m_ListCtrl->GetNextSelectedItem(pos);

			CString dbName, tbName;
			((CMainFrame*)GetParentFrame())->m_pDBView->GetDBAndTableName(dbName, tbName);

			FieldModel field(FileOp::StringToInteger(m_ListCtrl->GetItemText(nItem, 2)),
				m_ListCtrl->GetItemText(nItem, 3),
				FileOp::GetTypeInt(m_ListCtrl->GetItemText(nItem, 4)),
				FileOp::StringToInteger(m_ListCtrl->GetItemText(nItem, 5)),
				1);


			/*field.SetMin(CUtil::StringToInteger(m_ListCtrl->GetItemText(m_iRow, 5)));
			field.SetMax(CUtil::StringToInteger(m_ListCtrl->GetItemText(m_iRow, 6)));
			field.SetDefault(m_ListCtrl->GetItemText(m_iRow, 7));*/

			//MessageBox(CUtil::DoubleToString(field.GetMin()),L"",MB_OK);

			FieldDialog modifyFieldDlg(this, dbName, tbName, field, OPERATE_MODIFY);
			modifyFieldDlg.DoModal();
		}
	}
}


void CTableView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_iRow = pNMListView->iItem;
	m_iColumn = pNMListView->iSubItem;
	m_iCount = m_ListCtrl->GetItemCount();


	CPoint point;
	GetCursorPos(&point);
	CMenu MyMenu;
	MyMenu.LoadMenu(IDR_MAINFRAME);

	if (m_iRow >= 0 && m_iRow < m_iCount)
	{
		m_ListCtrl->SetItemState(m_iRow, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_ListCtrl->SetSelectionMark(m_iRow);

	}
	else
	{
		if (m_curView == TABLEVIEW_FIELD)
		{
			MyMenu.GetSubMenu(3)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED);
		}
		else if (m_curView == TABLEVIEW_RECORD)
		{
			MyMenu.GetSubMenu(4)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED);
		}
	}

	if (m_curView == TABLEVIEW_FIELD)
	{
		CMenu* popup = MyMenu.GetSubMenu(3);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());
	}
	else if (m_curView == TABLEVIEW_RECORD)
	{
		CMenu* popup = MyMenu.GetSubMenu(4);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());
	}

	*pResult = 0;
}


void CTableView::OnAddRecord()
{
	// TODO: 在此添加命令处理程序代码
	CString dbName, tbName;
	((CMainFrame*)GetParentFrame())->m_pDBView->GetDBAndTableName(dbName, tbName);

	FieldOp fieldOp(dbName, tbName);
	DataInsertDlg addRcdDlg(this,
		fieldOp.queryFieldsModel(dbName, tbName),
		dbName,
		tbName,
		OPERATE_ADD,
		vector<CString>());

	addRcdDlg.DoModal();
}


void CTableView::OnModifyRecord()
{
	POSITION pos = m_ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL) {

		// TODO: 在此添加命令处理程序代码
		if (m_curView == TABLEVIEW_RECORD)
		{
			int nItem = m_ListCtrl->GetNextSelectedItem(pos);

			int columnNum = m_ListCtrl->GetHeaderCtrl()->GetItemCount();
			vector<CString> list;
			for (int i = 0; i < columnNum; i++)
			{
				list.push_back(m_ListCtrl->GetItemText(nItem, i));
			}
			CString dbName, tbName;
			((CMainFrame*)GetParentFrame())->m_pDBView->GetDBAndTableName(dbName, tbName);
			FieldOp fieldOption(dbName, tbName);
			DataInsertDlg modifyRcdDlg(this,
				fieldOption.queryFieldsModel(dbName, tbName),
				dbName,
				tbName,
				OPERATE_MODIFY,
				list);
			modifyRcdDlg.DoModal();
		}
	}
}




void CTableView::OnDeleteRecord()
{
	// TODO: 在此添加命令处理程序代码
	if (m_curView == TABLEVIEW_RECORD)
	{
		if (MessageBox(CString("确定删除该记录？"), CString("删除记录"), MB_OKCANCEL) == IDOK)
		{
			CString dbName, tbName;
			CDBView* pDBView = ((CMainFrame*)GetParentFrame())->m_pDBView;
			pDBView->GetDBAndTableName(dbName, tbName);

			CDataOp rcdLogic(dbName, tbName);
			int code = rcdLogic.DeleteRecord(FileOp::StringToInteger(m_ListCtrl->GetItemText(m_iRow, 0)));
			if (code == YES)
			{
				pDBView->OnQueryTable();
			}

		}

	}
}
