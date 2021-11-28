// FieldDialog.cpp: 实现文件
//

#include "pch.h"
#include "myDBMS1.h"
#include "FieldDialog.h"
#include "DataOp.h"
#include "afxdialogex.h"


// FieldDialog 对话框

IMPLEMENT_DYNAMIC(FieldDialog, CDialogEx)

FieldDialog::FieldDialog(CWnd* pParent,CString& dbName,CString& tbName,FieldModel& fieldEntity, int type)
	: CDialogEx(IDD_DIALOG1, pParent),  m_dbName(dbName), m_tbName(tbName),m_fieldEntity(fieldEntity), m_iType(type)
	, fieldName(_T(""))
	, fieldLength(_T(""))
{
	m_pTableView = (CTableView*)pParent;
}

FieldDialog::~FieldDialog()
{
}

void FieldDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, fieldName);
	DDX_Text(pDX, IDC_EDIT2, fieldLength);
	DDX_Control(pDX, IDC_COMBO1, m_combType);
}


BEGIN_MESSAGE_MAP(FieldDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &FieldDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// FieldDialog 消息处理程序


void FieldDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	UpdateData(true);
	m_iSelType = m_combType.GetCurSel() + 1;
	if (fieldName == "" || m_iSelType == 0)
	{
		MessageBox(CString("字段名和类型不能为空！"), CString("警告"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		m_NewField.SetName(fieldName);
		m_NewField.SetType(m_iSelType);
		USES_CONVERSION;
		m_NewField.SetParam(atoi(T2A(fieldLength)));
	}


	if (m_iType==OPERATE_ADD) {
		this->AddField();
	}
	else {
		this->ModifyField();
	}

}

void FieldDialog::AddField()
{
	bool havePK = false;
	if (m_NewField.IsPrimaryKey())	//新字段设置了主键
	{
		FieldOp fieldLogic(m_dbName, m_tbName);
		vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
		for (vector<FieldModel>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite)
		{
			if (ite->IsPrimaryKey())
			{

				MessageBox(L"该表已经有了主键！", L"错误", MB_OK);
				havePK = true;
				break;

			}
		}
	}
	else {	//新字段没有要设置主键
		FieldOp fieldLogic(m_dbName, m_tbName);
		vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
		if (fieldList.empty()) {
			m_NewField.SetId(0);
		}
		else {
			int curId = fieldList.back().GetId() + 1;
			m_NewField.SetId(curId);
		}

		//int code = fieldLogic.addField(m_dbName, m_tbName,m_NewField.GetName(), m_NewField.GetOrder(), m_NewField.GetType(),m_NewField.GetParam());
		int code = fieldLogic.AddOneField(m_NewField);
		if (code == true)
		{
			vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
			m_pTableView->DisplayFields(fieldList);
		}
		else
			MessageBox(CString("添加错误"), CString("错误"), MB_OK);
	}
}

void FieldDialog::ModifyField() {
	bool havePK = false;
	FieldOp fieldLogic(m_dbName, m_tbName);
	vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
	if (m_NewField.IsPrimaryKey())
	{
		for (vector<FieldModel>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite)
		{
			if (ite->IsPrimaryKey() && ite->GetId() != m_fieldEntity.GetId())
			{
				MessageBox(L"该表已经有了主键！", L"错误", MB_OK);
				havePK = true; break;
			}
		}
	}
	bool chageType = false;
	CDataOp dataop(m_dbName, m_tbName);
	if (!dataop.ReadDataList(fieldList).empty())
	{
		if (m_NewField.GetType() != m_fieldEntity.GetType())
		{
			MessageBox(L"当记录不为空时无法修改字段类型！", L"提示", MB_OK);
			chageType = true;
		}
	}
	if (!havePK&&!chageType) {
		m_NewField.SetId(m_fieldEntity.GetId());
		//m_NewField.SetDefault(m_sDefault);
		FieldOp fieldLogic(m_dbName, m_tbName);
		vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);

		int code = fieldLogic.ModifyField(m_NewField);
		if (code == true)
		{
			vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
			m_pTableView->DisplayFields(fieldList);
		}
		else
			MessageBox(CString("修改错误"), CString("错误"), MB_OK);
	}
}

BOOL FieldDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	//刷新初始化的值到控件


	m_combType.InsertString(0, CString("Integer"));
	m_combType.InsertString(1, CString("Bool"));
	m_combType.InsertString(2, CString("Double"));
	m_combType.InsertString(3, CString("Varchar"));
	m_combType.InsertString(4, CString("DateTime"));


	if (m_iType == OPERATE_MODIFY)
	{
		this->SetWindowTextW(L"修改字段");

		fieldName = m_fieldEntity.GetName();
		m_combType.SetCurSel(m_fieldEntity.GetType() - 1);
		m_iSelType = m_fieldEntity.GetType() - 1;
	}
	else {
		this->SetWindowTextW(L"增加字段");
		m_combType.SetCurSel(3);	//默认类型为varchar
		m_iSelType = 3;
	}
	
	return true;
}