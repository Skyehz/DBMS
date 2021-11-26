﻿
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "myDBMS1.h"
//#include "stdafx.h"
#include "Resource.h"
#include "SqlDialog.h"
#include "MainFrm.h"
#include "DBOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_32771, &CMainFrame::OnCrtDB)
	ON_COMMAND(ID_32774, &CMainFrame::OnDropDB)
	ON_COMMAND(ID_32772, &CMainFrame::OnOpenDB)
	ON_COMMAND(ID_32791, &CMainFrame::OnInputSql)
	ON_COMMAND(ID_32773, &CMainFrame::OnRenameDB)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	*/
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 如果不需要可停靠工具栏，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

//创建窗口（左右）
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDBView), CSize(160, 200), pContext))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CTableView), CSize(160, 200), pContext))
		return FALSE;

	m_pTableView = (CTableView*)m_wndSplitter.GetPane(0, 1);
	m_pDBView = (CDBView*)m_wndSplitter.GetPane(0, 0);

	return TRUE;
}

void CMainFrame::OnCrtDB()
{
	// TODO: 在此添加命令处理程序代码
	m_pDBView->OnCrtDB();
}


void CMainFrame::OnDropDB()
{
	// TODO: 在此添加命令处理程序代码
	m_pDBView->OnDropDB();
}


void CMainFrame::OnInputSql()
{
	// TODO: 在此添加命令处理程序代码
	SqlDialog sqlDialog;
	sqlDialog.DoModal();
}


void CMainFrame::OnOpenDB()
{
	// TODO: 在此添加命令处理程序代码
	m_pDBView->OnOpenDB();
}



void CMainFrame::OnRenameDB()
{
	// TODO: 在此添加命令处理程序代码
	m_pDBView->OnRenameDB();
}
