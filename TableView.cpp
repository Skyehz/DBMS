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


// CTableView ���

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


// CTableView ��Ϣ�������


void CTableView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	m_ListCtrl = &GetListCtrl();

	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE);   // ��ȡ��ǰ���ڷ��
	lStyle &= ~LVS_TYPEMASK;                              // �����ʾ��ʽλ
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ListCtrl->m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCtrl->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;                      // ѡ��ĳ��ʹ���и���
	dwStyle |= LVS_EX_GRIDLINES;                          // ������
	m_ListCtrl->SetExtendedStyle(dwStyle);                   // ������չ���

	//��ʼ������ͼ����Ϊ�Ƿ���ͼ
	m_curView = TABLEVIEW_VALID;

}
/*
void CTableView::DisplayRecords(vector<RecordModel>& rcdlist, vector<FieldModel>& fieldList)
{
	m_curView = TABLEVIEW_RECORD;	//��ͼ��������Ϊ��¼��ͼ

	//�����
	this->ClearTable();

	int columnNum = fieldList.size() + 1;
	//��ʾ�ֶ�
	m_ListCtrl->InsertColumn(0, CString("#"), LVCFMT_LEFT, 0);
	for (int i = 1; i < columnNum; i++)
	{
		m_ListCtrl->InsertColumn(i, fieldList[i - 1].GetName(), LVCFMT_LEFT, 150);
	}
	//��ʾ��¼
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