// UserManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "UserManageDlg.h"
#include "afxdialogex.h"
#include "MainDlg.h"

// CUserManageDlg 对话框

IMPLEMENT_DYNAMIC(CUserManageDlg, CDialog)

CUserManageDlg::CUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserManageDlg::IDD, pParent)
{

}

CUserManageDlg::~CUserManageDlg()
{
}

void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_ALLUSER, &CUserManageDlg::OnBnClickedBtAlluser)
	ON_BN_CLICKED(IDC_BT_EDITPASSWORD, &CUserManageDlg::OnBnClickedBtEditpassword)
	ON_BN_CLICKED(IDC_BT_LOGOUT, &CUserManageDlg::OnBnClickedBtLogout)
END_MESSAGE_MAP()


// CUserManageDlg 消息处理程序

//所有用户按钮响应函数//
void CUserManageDlg::OnBnClickedBtAlluser()
{
	// TODO:  在此添加控件通知处理程序代码
	//if (CMainDlg::s_nUserRight == 1)
	if (CLoginDlg::LoginUser_CurrentPermissionLevel)			//  ///
	{
		ShowWindow(SW_HIDE); 
		m_AllUserManageDlg.DoModal();
		ShowWindow(SW_SHOW);
	}
	else
		AfxMessageBox(_T("您没有权限进行该操作！"));
}

//修改密码按钮响应函数//
void CUserManageDlg::OnBnClickedBtEditpassword()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CLoginDlg::LoginUser_CurrentId==-1)
		AfxMessageBox(_T("超级用户信息不可修改"));
	else
	{
		ShowWindow(SW_HIDE);
		m_EditPasswordDlg.DoModal();
		ShowWindow(SW_SHOW);
	}
}

//切换用户按钮//
void CUserManageDlg::OnBnClickedBtLogout()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();
}
