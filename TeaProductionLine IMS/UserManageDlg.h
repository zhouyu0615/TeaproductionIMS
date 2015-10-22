#pragma once
#include "AllUserManageDlg.h"
#include "EditPassWordDlg.h"

// CUserManageDlg 对话框

class CUserManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserManageDlg)

public:
	CUserManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserManageDlg();
	CAllUserManageDlg m_AllUserManageDlg;   //所有用户管理界面//
	CEditPassWordDlg m_EditPasswordDlg;     //修改密码界面//

// 对话框数据
	enum { IDD = IDD_USERMANAGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtAlluser();
	afx_msg void OnBnClickedBtEditpassword();
	afx_msg void OnBnClickedBtLogout();
};
