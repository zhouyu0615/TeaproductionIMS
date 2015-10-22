// EditUserPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "AllUserManageDlg.h"
#include "EditUserPopDlg.h"
#include "afxdialogex.h"


// CEditUserPopDlg 对话框


IMPLEMENT_DYNAMIC(CEditUserPopDlg, CDialog)

CEditUserPopDlg::CEditUserPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUserPopDlg::IDD, pParent)
	, EditUserId(0)
	, ManagerId(0)
{

}

CEditUserPopDlg::~CEditUserPopDlg()
{
}

void CEditUserPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditUserPopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditUserPopDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CEditUserPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//将选中用户信息自动显示到修改信息的框中///
	for (int i = 0; i < m_pDataProvider->m_vectLoginUser.size(); i++)
	{
		if (EditUserId == m_pDataProvider->m_vectLoginUser[i].m_Id)
		{
			GetDlgItem(IDC_ED1_USERNAME)->SetWindowText(m_pDataProvider->m_vectLoginUser[i].m_strLoginName);		//获取用户的名字///
			GetDlgItem(IDC_ED2_PSAAWORD)->SetWindowText(m_pDataProvider->m_vectLoginUser[i].m_strLoginPassWd);		//获取用户的密码///
			break;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// CEditUserPopDlg 消息处理程序

void CEditUserPopDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString NewUserName;
	CString NewUserPassWd;
	ManagerId = CLoginDlg::LoginUser_CurrentId;		//将当前用户ID赋给管理员ID///
	GetDlgItem(IDC_ED1_USERNAME)->GetWindowText(NewUserName);		//获取用户新名字///
	GetDlgItem(IDC_ED2_PSAAWORD)->GetWindowText(NewUserPassWd);		//获取用户新密码///
	if (NewUserName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空,请重新输入!"));
		return;
	}
	if (NewUserPassWd.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空,请重新输入!"));
		return;
	}
	if (NewUserName==_T("Super"))
	{
		AfxMessageBox(_T("非法用户名，请重新输入！"));
		return;
	}

	for (int i = 0; i < m_pDataProvider->m_vectLoginUser.size(); i++)			
	{
		//判断除了修改后的用户名与其他用户是否重名///
		if ((NewUserName == m_pDataProvider->m_vectLoginUser[i].m_strLoginName)&&
			(EditUserId !=m_pDataProvider->m_vectLoginUser[i].m_Id))
		{
			AfxMessageBox(_T("该用户名已被使用,请重新输入!"));
			return;
		}
	}
	for (EditUserIndex = 0; EditUserIndex < m_pDataProvider->m_vectLoginUser.size(); EditUserIndex++)
	{
		if (EditUserId == m_pDataProvider->m_vectLoginUser[EditUserIndex].m_Id)
		{
			//判断用户名和密码是否和原来一样///
			if((NewUserName == m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginName)&&
				(NewUserPassWd == m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginPassWd))
			{
				AfxMessageBox(_T("用户信息修改成功!"));
				CDialog::OnOK();
				return;
			}
			if (EditUserId == ManagerId)	//若选中用户ID为管理员自己的ID///
			{
				int nResult;
				nResult = MessageBox(_T("该操作无法恢复，是否继续？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
				if (nResult == IDYES)
				{
					m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginName = NewUserName;
					m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginPassWd = NewUserPassWd;
					//记录修改时间///
					m_pDataProvider->m_vectLoginUser[EditUserIndex].m_LastUpdatedDateTime = CTime::GetCurrentTime();
					m_pDataProvider->UpdateTableItem(m_pDataProvider->tbLoginUser, EditUserId);		//更新数据库///
					AfxMessageBox(_T("用户信息修改成功!"));
					CDialog::OnOK();
				}
			}
			else
			{
				m_pDataProvider->m_vectLoginUser[EditUserIndex].m_IsAutoLogin = 0;
				m_pDataProvider->m_vectLoginUser[EditUserIndex].m_IsRememberPasswd = 0;
				m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginName = NewUserName;
				m_pDataProvider->m_vectLoginUser[EditUserIndex].m_strLoginPassWd = NewUserPassWd;
				//记录修改时间///
				m_pDataProvider->m_vectLoginUser[EditUserIndex].m_LastUpdatedDateTime = CTime::GetCurrentTime();
				m_pDataProvider->UpdateTableItem(m_pDataProvider->tbLoginUser, EditUserId);		//更新数据库///
				AfxMessageBox(_T("用户信息修改成功!"));
				CDialog::OnOK();
			}
		}
	}	
}



