// EditPassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditPassWordDlg.h"
#include "afxdialogex.h"
#include "DataProvider.h"
// CEditPassWordDlg 对话框


IMPLEMENT_DYNAMIC(CEditPassWordDlg, CDialog)

CEditPassWordDlg::CEditPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPassWordDlg::IDD, pParent)
{
}

CEditPassWordDlg::~CEditPassWordDlg()
{
}

void CEditPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
									
	
}



BEGIN_MESSAGE_MAP(CEditPassWordDlg, CDialog)
	ON_BN_CLICKED(IDC_BT1_SAVE_EDITPASSWORDDLG, &CEditPassWordDlg::OnBnClickedBt1SaveEditpassworddlg)
END_MESSAGE_MAP()

BOOL CEditPassWordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_pDataProvider->ReadLoginUserFormDatabase();		//从数据库里读取数据///
	CurrentId = CLoginDlg::LoginUser_CurrentId;			//将当前登录用户的ID赋给CurrentId///

	for (UserNumber = 0; UserNumber < m_pDataProvider->m_vectLoginUser.size(); UserNumber++)
	{
		if (m_pDataProvider->m_vectLoginUser[UserNumber].m_Id == CurrentId)
		{
			GetDlgItem(IDC_ED1_USERNAME_EDITPASSWORDDLG)->SetWindowText(m_pDataProvider->m_vectLoginUser[UserNumber].m_strLoginName);
			break;
		}
	}
	return TRUE;
	// return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

	// CEditPassWordDlg 消息处理程序
void CEditPassWordDlg::OnBnClickedBt1SaveEditpassworddlg()
{

	CLoginUser tempLoginUser;				//新增用户变量///
	CString NewPassWord1;					//新密码///
	CString NewPassWord;					//再次输入的新密码///
	(GetDlgItem(IDC_ED2_OLDPASSWORD))->GetWindowText(tempLoginUser.m_strLoginPassWd);
	(GetDlgItem(IDC_ED3_NEWPASSWORD1))->GetWindowText(NewPassWord1);
	(GetDlgItem(IDC_ED4_NEWPASSWORD))->GetWindowText(NewPassWord);
	if(tempLoginUser.m_strLoginPassWd.IsEmpty())
	{
		AfxMessageBox(_T("输入密码不能为空,请输入密码!"));
		return;
	}
	if (tempLoginUser.m_strLoginPassWd == m_pDataProvider->m_vectLoginUser[UserNumber].m_strLoginPassWd)
	{
		if (NewPassWord.IsEmpty()||NewPassWord1.IsEmpty())
		{
			AfxMessageBox(_T("新密码不能为空,请重新输入!"));
			return;
		}
		if (NewPassWord1 == NewPassWord)
		{
			if (NewPassWord == m_pDataProvider->m_vectLoginUser[UserNumber].m_strLoginPassWd)
			{
				AfxMessageBox(_T("密码修改成功!请牢记您的新密码!"));
				CDialog::OnCancel();
			}
			else
			{
				
				m_pDataProvider->m_vectLoginUser[UserNumber].m_strLoginPassWd = NewPassWord;						//记录修改密码///
				m_pDataProvider->m_vectLoginUser[UserNumber].m_LastUpdatedDateTime = CTime::GetCurrentTime();			//记录修改时间///
				m_pDataProvider->UpdateTableItem(m_pDataProvider->tbLoginUser, m_pDataProvider->m_vectLoginUser[UserNumber].m_Id);	//更新数据库///
				AfxMessageBox(_T("密码修改成功!请牢记您的新密码!"));
				CDialog::OnCancel();
			}
		}
		else
		{
			AfxMessageBox(_T("两次输入的新密码不一致!请重新输入!"));
			return;
		}
	}
	else
	{
		AfxMessageBox(_T("旧密码不正确!请重新输入!"));
		return;
	}
}



