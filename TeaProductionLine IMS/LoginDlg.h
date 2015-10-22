#pragma once
#include "afxwin.h"
#include "DataProvider.h"
#include "LoginUser.h"

// CLoginDlg 对话框

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_LOGINDLG };

	CDataProvider *m_LoginDataProvider = CDataProvider::getInstance(); //数据源///

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	HICON m_hIcon;			//  ///

	//生产的消息映射函数///
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	static long m_PermissionLevel;                 //用户权限标识，0代表操作员，1代表管理员//
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CComboBox m_LoginUserName;
	CLoginUser LoginUser;					//新增用户变量///
	CString LoginUser_PermissionLevel;		//确认用户权限是管理员还是操作员///

	static long LoginUser_CurrentId;				//当前登录用户的ID号,用户修改密码时将用ID查找///
	static long LoginUser_CurrentPermissionLevel;	//当前登录用户的权限，用于判断用户有无管理所有用户权限///
	static CString LoginUser_Name;
	 
	CEdit m_EditPasswd;
	afx_msg void OnCbnSelchangeComboUsername();
	CButton CheckPassWd;
	CButton CheckAutoLogin;


//	afx_msg void OnCbnEditchangeComboUsername();
	afx_msg void OnCbnSelchangeComboUserright();
};
