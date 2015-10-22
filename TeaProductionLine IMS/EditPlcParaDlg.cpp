// EditPlcParaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditPlcParaDlg.h"
#include "afxdialogex.h"


// CEditPlcParaDlg 对话框

IMPLEMENT_DYNAMIC(CEditPlcParaDlg, CDialog)

CEditPlcParaDlg::CEditPlcParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPlcParaDlg::IDD, pParent)
{

}

CEditPlcParaDlg::~CEditPlcParaDlg()
{
}

void CEditPlcParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_EDITPLCPARADLG, m_tab1);
}


BEGIN_MESSAGE_MAP(CEditPlcParaDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EDITPLCPARADLG, &CEditPlcParaDlg::OnTcnSelchangeTabEditplcparadlg)
END_MESSAGE_MAP()


// CEditPlcParaDlg 消息处理程序


BOOL CEditPlcParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化标签控件//
	m_tab1.InsertItem(0, _T("工艺参数"));
	m_tab1.InsertItem(1, _T("设备参数"));
	m_tab1.InsertItem(2, _T("故障参数"));
	m_tab1.InsertItem(3, _T("状态参数"));
	m_EditModuleParaTabDlg.Create(IDD_EDITMODULEPARA_TABDLG, &m_tab1);
	m_EditDeviceParaTabDlg.Create(IDD_EDITDEVICEPARA_TABDLG, &m_tab1);
	m_EditErrorParaTabDlg.Create(IDD_EDITERRORPARA_TABDLG, &m_tab1);
	m_EditStateParaTabDlg.Create(IDD_EDITSTATEPARA_TABDLG, &m_tab1);
	CRect tabRect;          // 标签控件客户区的位置和大小//	
	m_tab1.GetClientRect(&tabRect);    // 获取标签控件客户区Rect //
	tabRect.left += 1;      // 调整tabRect，使其覆盖范围适合放置标签页 //  
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;	   
	m_EditModuleParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);// 根据调整好的tabRect放置子对话框//
	m_EditDeviceParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_EditErrorParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_EditStateParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//标签控件选中条目发生改变时的响应函数//
void CEditPlcParaDlg::OnTcnSelchangeTabEditplcparadlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	CRect tabRect;               // 标签控件客户区的Rect//

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页//
	m_tab1.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_tab1.GetCurSel())  //根据选择的标签，显示相应对话框//
	{
	case 0:
		m_EditModuleParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_EditDeviceParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditErrorParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditStateParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1:
		m_EditModuleParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditDeviceParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_EditErrorParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditStateParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_EditModuleParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditDeviceParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditErrorParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_EditStateParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_EditModuleParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditDeviceParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditErrorParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_EditStateParaTabDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}


void CEditPlcParaDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
