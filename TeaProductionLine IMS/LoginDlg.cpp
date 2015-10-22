// LoginDlg.cpp : 实现文件
#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "resource.h"
// CLoginDlg 对话框
long  CLoginDlg::LoginUser_CurrentPermissionLevel ;		//声明静态全局变量：用户权限标志///
long  CLoginDlg::LoginUser_CurrentId;				   //声明静态全局变量：用户ID标志///
CString CLoginDlg::LoginUser_Name = _T(""); 

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_LoginUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EditPasswd);
	DDX_Control(pDX, IDC_CHECK_PASSWD, CheckPassWd);
	DDX_Control(pDX, IDC_CHECK_AUTOLOGIN, CheckAutoLogin);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, &CLoginDlg::OnCbnSelchangeComboUsername)
	ON_CBN_SELCHANGE(IDC_COMBO_USERRIGHT, &CLoginDlg::OnCbnSelchangeComboUserright)
END_MESSAGE_MAP()

// CLoginDlg 消息处理程序
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	
	//	///初始化权限组合框
	((CComboBox*)GetDlgItem(IDC_COMBO_USERRIGHT))->AddString(_T("操作员"));
	((CComboBox*)GetDlgItem(IDC_COMBO_USERRIGHT))->AddString(_T("管理员"));
	((CComboBox*)GetDlgItem(IDC_COMBO_USERRIGHT))->SetCurSel(0);		//默认为操作员///
//	m_LoginDataProvider->ReadLoginUserFormDatabase();					//读取数据库///
	for (int i = 0; i < m_LoginDataProvider->m_vectLoginUser.size(); i++)
	{
		if (m_LoginDataProvider->m_vectLoginUser[i].m_PermissionLevel == 0)
		{
								  //将操作员名字自动显示到下拉列表框中///
			m_LoginUserName.AddString(m_LoginDataProvider->m_vectLoginUser[i].m_strLoginName);
		}
	}
	return TRUE;				 // 除非将焦点设置到控件，否则返回 TRUE
}

//选择用户权限管理员和操作员的响应函数///
void CLoginDlg::OnCbnSelchangeComboUserright()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Super;
	GetDlgItem(IDC_COMBO_USERRIGHT)->GetWindowText(LoginUser_PermissionLevel);
	//m_LoginDataProvider->ReadLoginUserFormDatabase();
	m_LoginUserName.ResetContent();				//先将下拉的用户名清空///
	if (LoginUser_PermissionLevel == _T("管理员"))
	{
		for (int i = 0; i < m_LoginDataProvider->m_vectLoginUser.size();i++)
		{
			if (m_LoginDataProvider->m_vectLoginUser[i].m_PermissionLevel == 1)
			{
				m_LoginUserName.AddString(m_LoginDataProvider->m_vectLoginUser[i].m_strLoginName);
			}
		}
		m_LoginUserName.AddString(_T("Super"));
	}
	else
	{
		for (int i = 0; i < m_LoginDataProvider->m_vectLoginUser.size(); i++)
		{
			if (m_LoginDataProvider->m_vectLoginUser[i].m_PermissionLevel == 0)
			{
				m_LoginUserName.AddString(m_LoginDataProvider->m_vectLoginUser[i].m_strLoginName);
			}
		}
	}
	CheckAutoLogin.SetCheck(0);
	CheckPassWd.SetCheck(0);
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(_T(""));
}

//下拉编辑框下拉选中不同记住密码的用户名时，对应密码响应函数///
void CLoginDlg::OnCbnSelchangeComboUsername()
{
	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_COMBO_USERNAME)->GetWindowText(LoginUser.m_strLoginName);
	//m_LoginDataProvider->ReadLoginUserFormDatabase();
	if (LoginUser.m_strLoginName==_T("Super"))
	{
		GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(_T(""));
		CheckPassWd.SetCheck(0);
		CheckAutoLogin.SetCheck(0);
		return;
	}
	for (int i = 0; i < m_LoginDataProvider->m_vectLoginUser.size(); i++)
	{
		//若选中用户记住密码，则自动显示出密码///
		if (LoginUser.m_strLoginName == m_LoginDataProvider->m_vectLoginUser[i].m_strLoginName)
		{
			if (m_LoginDataProvider->m_vectLoginUser[i].m_IsRememberPasswd == 1)
			{
				GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(m_LoginDataProvider->m_vectLoginUser[i].m_strLoginPassWd);	//显示密码///
				CheckPassWd.SetCheck(TRUE);
			}
			else
			{
				GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(_T(""));
				CheckPassWd.SetCheck(0);
			}
			if (m_LoginDataProvider->m_vectLoginUser[i].m_IsAutoLogin)
				CheckAutoLogin.SetCheck(true);
			else
				CheckAutoLogin.SetCheck(0);
			break;
		}
	}
}

//登录按钮的消息响应函数///
void CLoginDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码_
	
	int UserNum ;										//         ///
	//m_LoginDataProvider->ReadUserFromDatabase();		//从数据里读取数据///
	GetDlgItem(IDC_COMBO_USERNAME)->GetWindowText(LoginUser.m_strLoginName);
	GetDlgItem(IDC_COMBO_USERRIGHT)->GetWindowText(LoginUser_PermissionLevel);
	GetDlgItem(IDC_COMBO_USERNAME)->GetWindowText(LoginUser.m_strLoginName);
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(LoginUser.m_strLoginPassWd);
	if (CheckPassWd.GetCheck())
	{
		LoginUser.m_IsRememberPasswd = 1;
	}
	else
	{
		LoginUser.m_IsRememberPasswd = 0;
	}
	if (CheckAutoLogin.GetCheck())					//判断是否选择自动登录///
	{
		LoginUser.m_IsAutoLogin = 1;
	}
	else
	{
		LoginUser.m_IsAutoLogin = 0;
	}
	if (LoginUser.m_strLoginName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空, 请重新输入!"));
		return;
	}
	if (LoginUser.m_strLoginPassWd.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空, 请重新输入!"));
		return;
	}
	//Super用户登录//////////////////////////////////////////////////////////////////////////
	if (LoginUser.m_strLoginName  == _T("Super"))          //如果是超级用户//
	{
		if (LoginUser.m_strLoginPassWd == _T("111111"))
		{
			LoginUser_CurrentPermissionLevel = 1;
			LoginUser_CurrentId = -1;
			LoginUser_Name = _T("Super");
			CDialog::OnOK();
			return;
		}
		else
		{
			AfxMessageBox(_T("用户密码不正确，请重新输入"));
			return;
		}
	}
	for ( UserNum = 0;UserNum < m_LoginDataProvider->m_vectLoginUser.size();UserNum++)
	{
		if (LoginUser.m_strLoginName == m_LoginDataProvider->m_vectLoginUser[UserNum].m_strLoginName)
		{
			if (LoginUser.m_strLoginPassWd == m_LoginDataProvider->m_vectLoginUser[UserNum].m_strLoginPassWd)
			{
				LoginUser_CurrentId = m_LoginDataProvider->m_vectLoginUser[UserNum].m_Id;		//记住当前登录用户的ID///
				LoginUser_CurrentPermissionLevel = m_LoginDataProvider->m_vectLoginUser[UserNum].m_PermissionLevel;	//记住用户权限///
				LoginUser_Name = m_LoginDataProvider->m_vectLoginUser[UserNum].m_strLoginName;
				m_LoginDataProvider->m_vectLoginUser[UserNum].m_LastLoginInTime = CTime::GetCurrentTime();	//记住用户登录时间///
				//若用户自动登录或记住密码发生修改,则更新数据库///
				if ((m_LoginDataProvider->m_vectLoginUser[UserNum].m_IsAutoLogin != LoginUser.m_IsAutoLogin) ||
					(m_LoginDataProvider->m_vectLoginUser[UserNum].m_IsRememberPasswd != LoginUser.m_IsRememberPasswd))
				{
					m_LoginDataProvider->m_vectLoginUser[UserNum].m_IsAutoLogin = LoginUser.m_IsAutoLogin;	//记住是否自动登录///
					m_LoginDataProvider->m_vectLoginUser[UserNum].m_IsRememberPasswd = LoginUser.m_IsRememberPasswd;//记住是否记住密码///
					m_LoginDataProvider->UpdateTableItem(m_LoginDataProvider->tbLoginUser, LoginUser_CurrentId); //更新数据库///
				}
                m_LoginDataProvider->UpdateUserLoginTime(LoginUser_CurrentId);				//更新用户登录时间///
				if (LoginUser.m_IsAutoLogin == 1)										//若该用户选择自动登录，则其他用户的自动登录取消///
				{
					for (int i = 0; i < m_LoginDataProvider->m_vectLoginUser.size(); i++)
					{
						if ((i != UserNum) && (m_LoginDataProvider->m_vectLoginUser[i].m_IsAutoLogin == 1))
						{
							m_LoginDataProvider->m_vectLoginUser[i].m_IsAutoLogin = 0;
							m_LoginDataProvider->UpdateTableItem(m_LoginDataProvider->tbLoginUser, m_LoginDataProvider->m_vectLoginUser[i].m_Id); //更新数据库///
						}
					}
				}
				
				CDialog::OnOK();
				return;
			}
			else
			{
				AfxMessageBox(_T("用户密码不正确，请重新输入"));
				return;
			}
		}		
	}
}
//取消按钮的消息响应函数///
void CLoginDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	PostQuitMessage(0);
	CDialog::OnCancel();
}

