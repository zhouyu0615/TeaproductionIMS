
// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "DlgProxy.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeaProductionLineIMSDlgAutoProxy

IMPLEMENT_DYNCREATE(CTeaProductionLineIMSDlgAutoProxy, CCmdTarget)

CTeaProductionLineIMSDlgAutoProxy::CTeaProductionLineIMSDlgAutoProxy()
{
	EnableAutomation();
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CMainDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainDlg)))
		{
			m_pDialog = reinterpret_cast<CMainDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CTeaProductionLineIMSDlgAutoProxy::~CTeaProductionLineIMSDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CTeaProductionLineIMSDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CTeaProductionLineIMSDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTeaProductionLineIMSDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_ITeaProductionLineIMS 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {E6E392E5-4C19-4ED0-B641-A0A468860781}
static const IID IID_ITeaProductionLineIMS =
{ 0xE6E392E5, 0x4C19, 0x4ED0, { 0xB6, 0x41, 0xA0, 0xA4, 0x68, 0x86, 0x7, 0x81 } };

BEGIN_INTERFACE_MAP(CTeaProductionLineIMSDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CTeaProductionLineIMSDlgAutoProxy, IID_ITeaProductionLineIMS, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {1C8FCBEA-5A91-49C9-BCE6-A437D8025D2E}
IMPLEMENT_OLECREATE2(CTeaProductionLineIMSDlgAutoProxy, "TeaProductionLineIMS.Application", 0x1c8fcbea, 0x5a91, 0x49c9, 0xbc, 0xe6, 0xa4, 0x37, 0xd8, 0x2, 0x5d, 0x2e)


// CTeaProductionLineIMSDlgAutoProxy 消息处理程序
