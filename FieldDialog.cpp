// FieldDialog.cpp: 实现文件
//

#include "pch.h"
#include "myDBMS1.h"
#include "FieldDialog.h"
#include "DataOp.h"
#include "ParseSQL.h"
#include "afxdialogex.h"


// FieldDialog 对话框

IMPLEMENT_DYNAMIC(FieldDialog, CDialogEx)

FieldDialog::FieldDialog(CWnd* pParent,CString& dbName,CString& tbName,FieldModel& fieldEntity, int type)
	: CDialogEx(IDD_DIALOG1, pParent),  m_dbName(dbName), m_tbName(tbName),m_fieldEntity(fieldEntity), m_iType(type)
	, fieldName(_T(""))
	, fieldLength(_T(""))
	, m_primaryKey(FALSE)
	, m_notes(_T(""))
	, m_uniqueKey(FALSE)
	, m_defaultValue(_T(""))
	, m_notnull(FALSE)
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
	DDX_Check(pDX, IDC_CHECK1, m_primaryKey);
	DDX_Text(pDX, IDC_EDIT4, m_notes);
	DDX_Check(pDX, IDC_CHECK2, m_uniqueKey);
	DDX_Text(pDX, IDC_EDIT3, m_defaultValue);
	DDX_Check(pDX, IDC_CHECK3, m_notnull);
}


BEGIN_MESSAGE_MAP(FieldDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &FieldDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &FieldDialog::OnBnClickedPK)
END_MESSAGE_MAP()


// FieldDialog 消息处理程序

void FieldDialog::OnBnClickedPK()
{
	// TODO: 在此添加控件通知处理程序代码
	//更新值到变量
	UpdateData(TRUE);
	if (m_primaryKey)
	{
		m_notnull = true;
		m_uniqueKey = true;
		UpdateData(FALSE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK2)->EnableWindow();
		GetDlgItem(IDC_CHECK3)->EnableWindow();
	}
}

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
		m_NewField.SetPrimaryKey(m_primaryKey);
		m_NewField.SetUniqueKey(m_uniqueKey);
		m_NewField.SetDefaultValue(m_defaultValue);
		m_NewField.SetNotes(m_notes);
		m_NewField.SetEmpty(m_notnull);
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
	if(!havePK)
	{	
		//添加字段 alter table t2 add col_name type;
		CString statement = CString("alter table ") + m_tbName + CString(" add ") + m_NewField.GetName() +
			CString(" ") + FileOp::GetTypeCString(m_NewField.GetType()) + CString("(") + FileOp::IntegerToString(m_NewField.GetParam())+ CString(");");

		ParseSQL parseSql;
		parseSql.setDB(m_dbName);
		int m = parseSql.getSql(statement);

		if (m_NewField.IsPrimaryKey()) {
			//默认生成的主键约束名为cstPK@fieldName
			statement = CString("alter table ") + m_tbName + CString(" add constraint cstPK@") + m_NewField.GetName()
				+ CString(" primary key(") + m_NewField.GetName() + CString(");");
			parseSql.getSql(statement);
		}
		if (m_NewField.IsUniqueKey()) {
			//默认生成的唯一约束名为cstUQ@fieldName
			statement = CString("alter table ") + m_tbName + CString(" add constraint cstUQ@") + m_NewField.GetName()
				+ CString(" unique(") + m_NewField.GetName() + CString(");");
			parseSql.getSql(statement);
		}
		if (m_NewField.IsEmpty()) {
			//默认生成的非空约束名为cstNN@fieldName
			statement = CString("alter table ") + m_tbName + CString(" modify ") + m_NewField.GetName()+ FileOp::GetTypeCString(m_NewField.GetType())
				+CString("(") +FileOp::IntegerToString( m_NewField.GetParam()) +CString(") not null;");
			parseSql.getSql(statement);
		}
		if (m_NewField.GetDefaultValue() != CString("")) {
			//默认生成的默认值约束名为cstDEFAULT@fieldName
			statement = CString("alter table ") + m_tbName + CString(" change column ") + m_NewField.GetName() + FileOp::GetTypeCString(m_NewField.GetType())
				+ CString("(") + FileOp::IntegerToString(m_NewField.GetParam()) + CString(") default ")+m_NewField.GetDefaultValue();
			parseSql.getSql(statement);
		}

		if (m)
		{
			MessageBox(CString("添加成功"), CString("成功"), MB_OK);
			FieldOp fieldLogic(m_dbName, m_tbName);
			vector<FieldModel> fieldList = fieldLogic.queryFieldsModel(m_dbName, m_tbName);
			m_pTableView->DisplayFields(fieldList);
		}
		else
			MessageBox(CString("添加失败"), CString("错误"), MB_OK); 
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
		int code;
		if (m_NewField.GetName() != m_fieldEntity.GetName()) {	//字段名也进行了修改
			code = fieldLogic.ModifyField(m_NewField,1);
		}
		else {
			code = fieldLogic.ModifyField(m_NewField, 2);  //没有对字段名进行修改
		}
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