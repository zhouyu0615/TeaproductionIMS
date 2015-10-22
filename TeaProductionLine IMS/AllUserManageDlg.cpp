// AllUserManageDlg.cpp : 实现文件

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "AllUserManageDlg.h"
#include "afxdialogex.h"

// CAllUserManageDlg 对话框

IMPLEMENT_DYNAMIC(CAllUserManageDlg, CDialog)

CAllUserManageDlg::CAllUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllUserManageDlg::IDD, pParent)
{
	m_pDataProvider = CDataProvider::getInstance();
	m_pDataProvider->ReadLoginUserFormDatabase();
	

}

CAllUserManageDlg::~CAllUserManageDlg()
{
}

void CAllUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LI1_ALLUSERMANAGEDLG, m_list_UserInfo);
	DDX_Control(pDX, IDC_ED1_USERNAME, m_edUserName);
	DDX_Control(pDX, IDC_ED2_PASSWORD, m_edPassword);
}

BEGIN_MESSAGE_MAP(CAllUserManageDlg, CDialog)
	ON_BN_CLICKED(IDC_BT1_ADDUSER, &CAllUserManageDlg::OnBnClickedBt1Adduser)
	ON_BN_CLICKED(IDC_BT2_CLEARALL, &CAllUserManageDlg::OnBnClickedBt2Clearall)
	ON_NOTIFY(NM_RCLICK, IDC_LI1_ALLUSERMANAGEDLG, &CAllUserManageDlg::OnNMRClickLi1Allusermanagedlg)
END_MESSAGE_MAP()

// CAllUserManageDlg 消息处理程序

void CAllUserManageDlg::OnBnClickedBt1Adduser()
{
	// TODO:  在此添加控件通知处理程序代码///
	CLoginUser tempLoginUser;		//新增用户变量///

	// 获取编辑框中的新增用户名和密码///
	GetDlgItem(IDC_ED1_USERNAME)->GetWindowText(tempLoginUser.m_strLoginName);
	GetDlgItem(IDC_ED2_PASSWORD)->GetWindowText(tempLoginUser.m_strLoginPassWd);

	if (tempLoginUser.m_strLoginName==_T("Super"))
	{
		AfxMessageBox(_T("该用户名不可使用！"));
		return;
	}
	
	//判断用户名是否为空，为空则不提示信息为空///
	if (tempLoginUser.m_strLoginName.IsEmpty())
    {
		AfxMessageBox(_T("用户名为空,请输入用户名!"));
		return;
    }
	else if (tempLoginUser.m_strLoginPassWd.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空，请输入密码!"));
		return;
	}
	else
	{
		m_pDataProvider->ReadUserFromDatabase();
		
		for (int i = 0; i < m_pDataProvider->m_vectLoginUser.size();i++)
		{
			if (tempLoginUser.m_strLoginName == m_pDataProvider->m_vectLoginUser[i].m_strLoginName)
			{
				AfxMessageBox(_T("该用户名已被使用，请重新输入!"));
				return;
			}	
		}
	}
	tempLoginUser.m_PermissionLevel = 0;						//操作员权限为0，管理员和super权限为1  ///
	m_pDataProvider->AddLoginUserToDatabase(tempLoginUser);		//新增一个用户///
	AfxMessageBox(_T("添加成功!"));
	m_edUserName.SetWindowText(_T(""));
	m_edPassword.SetWindowText(_T(""));
	
	MyOnPint();													//每修改一次刷新一次列表框///
}

BOOL CAllUserManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
							// TODO:  在此添加额外的初始化
	MyOnPint();				//列表框显示用户信息///
	return TRUE;		 // return TRUE unless you set the focus to a control
						// 异常:  OCX 属性页应返回 FALSE
}
void CAllUserManageDlg::MyOnPint()
{
	CPaintDC dc(this); // device context for painting
	m_pDataProvider->ReadLoginUserFormDatabase();
						//设置列表控件风格///
	CRect rect1;
	m_list_UserInfo.GetWindowRect(&rect1);
	m_list_UserInfo.SetExtendedStyle(m_list_UserInfo.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE);

	m_list_UserInfo.DeleteAllItems();		//先清空列表框中元素///
	CHeaderCtrl *pmyHeaderCtrl = m_list_UserInfo.GetHeaderCtrl();
	int nCount = pmyHeaderCtrl->GetItemCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		m_list_UserInfo.DeleteColumn(i);
	}
	int temp = 0;
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");
	//初始化编辑区///
	m_list_UserInfo.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_list_UserInfo.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 35 * 3, -1);
	m_list_UserInfo.InsertColumn(2, _T("用户名"), LVCFMT_CENTER, rect1.Width() / 35 * 3, -1);
	m_list_UserInfo.InsertColumn(3, _T("密码"), LVCFMT_CENTER, rect1.Width() / 35 * 3, -1);
	m_list_UserInfo.InsertColumn(4, _T("权限"), LVCFMT_CENTER, rect1.Width() / 35 * 3, -1);
	m_list_UserInfo.InsertColumn(5, _T("创建时间"), LVCFMT_CENTER, rect1.Width() / 35 * 8, -1);
	m_list_UserInfo.InsertColumn(6, _T("上次登录时间"), LVCFMT_CENTER, rect1.Width() / 35 * 8, -1);
	m_list_UserInfo.InsertColumn(7, _T("最近修改时间"), LVCFMT_CENTER, rect1.Width() / 35 * 8, -1);

	//填写表单内容,将用户信息显示到列表框中///
	temp = m_pDataProvider->m_vectLoginUser.size();
	for (int i = 0; i < temp; i++)
	{
		litem.iItem = i;
		CString str;
		str.Format(_T("%d"), i + 1);
		m_list_UserInfo.InsertItem(&litem);
		m_list_UserInfo.SetItemText(i, 1, str);
		m_list_UserInfo.SetItemText(i, 2, (m_pDataProvider->m_vectLoginUser[i].m_strLoginName));
		m_list_UserInfo.SetItemText(i, 3, (m_pDataProvider->m_vectLoginUser[i].m_strLoginPassWd));
		m_list_UserInfo.SetItemText(i, 5, (m_pDataProvider->m_vectLoginUser[i].m_strCreatedDateTime));
		if (m_pDataProvider->m_vectLoginUser[i].m_strLastLoginInTime==_T("1970-01-01 08:00:00"))
		{
			m_list_UserInfo.SetItemText(i, 6, _T("尚未登录"));
		}
		else
		    m_list_UserInfo.SetItemText(i, 6, (m_pDataProvider->m_vectLoginUser[i].m_strLastLoginInTime));
		m_list_UserInfo.SetItemText(i, 7, (m_pDataProvider->m_vectLoginUser[i].m_strLastUpdatedDateTime));
		if (m_pDataProvider->m_vectLoginUser[i].m_PermissionLevel==0)
		{
			m_list_UserInfo.SetItemText(i, 4, _T("操作员"));
		}
		else if (m_pDataProvider->m_vectLoginUser[i].m_PermissionLevel == 1)
		{
			m_list_UserInfo.SetItemText(i, 4, _T("管理员"));
		}
	}
}
//管理员右击菜单删除和修改的响应函数///
void CAllUserManageDlg::OnNMRClickLi1Allusermanagedlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	index = pNMItemActivate->iItem;  //用户右击的行标//
	//CEditUserPopDlg.EditUserId= m_pDataProvider->m_vectLoginUser[index].m_Id;		//列表框中用户ID///
	if (index == -1)
	{
		return;
	}
	//添加相应菜单项///
	CMenu menu, *pSubMenu;
	menu.LoadMenu(IDR_MENU_MANAGE);
	pSubMenu = menu.GetSubMenu(0);			//0表示自定义的菜单项并不只有1个,0表示第1个///
	CPoint cpoint;							//存储鼠标位置的临时变量//
	GetCursorPos(&cpoint);					//得到光标处//
	//确保右键点击在哪菜单出现在哪，并返回所选的菜单项//
	UINT  nItem1 = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_TOPALIGN, cpoint.x, cpoint.y, GetParent());
	if (nItem1 == ID_DELETE)
	{
		int nResult;
		nResult = MessageBox(_T("该操作无法恢复，是否继续？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
		if (nResult == IDYES)
		{
			if (1 == m_pDataProvider->m_vectLoginUser[index].m_PermissionLevel)		//若当前删除的用户ID为管理员，则提示不能删除///
			{
				AfxMessageBox(_T("对不起,您不能删除管理员用户!"));
				return;
			}
			m_pDataProvider->DeleteDbTableItem(m_pDataProvider->tbLoginUser, m_pDataProvider->m_vectLoginUser[index].m_Id);
			//将容器里的第index条删除，或者重新读数据库///
			m_pDataProvider->m_vectLoginUser.erase(m_pDataProvider->m_vectLoginUser.begin()+index);		
			AfxMessageBox(_T("已删除"));
			MyOnPint();
		}
	}
	if (nItem1 == ID_MODIFY)
	{
		//弹出修改用户的界面///
		CEditUserPopDlg dlgUserPop;
		dlgUserPop.EditUserId = m_pDataProvider->m_vectLoginUser[index].m_Id;	//列表框选中用户的ID///
		dlgUserPop.DoModal();
		MyOnPint();
	}
}

	//清空操作的响应函数///
void CAllUserManageDlg::OnBnClickedBt2Clearall()
{
	// TODO:  在此添加控件通知处理程序代码
	int nResult;
	nResult = MessageBox(_T("该操作会删除所有操作员账号,且数据无法恢复，是否继续？"),
		_T("警告"), MB_ICONEXCLAMATION | MB_YESNO);
	if (nResult == IDYES)
	{
		for (int i = m_pDataProvider->m_vectLoginUser.size()-1; i >=0; i--)
		{
			if (m_pDataProvider->m_vectLoginUser[i].m_PermissionLevel==0)
			{
				m_pDataProvider->DeleteDbTableItem(m_pDataProvider->tbLoginUser, m_pDataProvider->m_vectLoginUser[i].m_Id);
				//将容器里的第i条记录清除，或者for循环结束后重新读数据库///
				m_pDataProvider->m_vectLoginUser.erase(m_pDataProvider->m_vectLoginUser.begin() + i);
			}
		}
		AfxMessageBox(_T("已清空"));
		MyOnPint();
	}
}