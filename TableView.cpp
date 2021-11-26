#include "pch.h"
#include "TableView.h"
#include "vector"

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
	/*ON_COMMAND(ID_32780, &CTableView::OnAddField)
	ON_COMMAND(ID_32782, &CTableView::OnDeleteField)*/
	//ON_NOTIFY_REFLECT(NM_RCLICK, &CTableView::OnNMRClick)
	/*ON_COMMAND(ID_32783, &CTableView::OnAddRecord)
	ON_COMMAND(ID_32785, &CTableView::OnDeleteRecord)
	ON_COMMAND(ID_32784, &CTableView::OnModifyRecord)
	ON_COMMAND(ID_32781, &CTableView::OnModifyField)*/
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
/*
void CTableView::DisplayRecords(vector<RecordModel>& rcdlist, vector<FieldModel>& fieldList)
{
	m_curView = TABLEVIEW_RECORD;	//视图类型设置为记录视图

	//清除表
	this->ClearTable();

	int columnNum = fieldList.size() + 1;
	//显示字段
	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);
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
		for (int j = 1; j < columnNum; j++)
		{
			m_ListCtrl->SetItemText(i, j, rcdlist[i].GetValue(fieldList[j - 1].GetName()));
		}
	}
}
*/