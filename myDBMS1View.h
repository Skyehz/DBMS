
// myDBMS1View.h: CmyDBMS1View 类的接口
//

#pragma once


class CmyDBMS1View : public CView
{
protected: // 仅从序列化创建
	CmyDBMS1View() noexcept;
	DECLARE_DYNCREATE(CmyDBMS1View)

// 特性
public:
	CmyDBMS1Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmyDBMS1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // myDBMS1View.cpp 中的调试版本
inline CmyDBMS1Doc* CmyDBMS1View::GetDocument() const
   { return reinterpret_cast<CmyDBMS1Doc*>(m_pDocument); }
#endif

