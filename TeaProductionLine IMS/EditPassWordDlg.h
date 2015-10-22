#pragma once
#include "DataProvider.h"
#include "LoginUser.h"
#include "LoginDlg.h"
// CEditPassWordDlg 对话框

class CEditPassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditPassWordDlg)

public:
	CEditPassWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPassWordDlg();

// 对话框数据
	enum { IDD = IDD_EDITPASSWORDDLG };

	CDataProvider *m_pDataProvider = CDataProvider::getInstance(); //数据源///
	long CurrentId;		//将当前登录用户的ID赋给CurrentId///
	int UserNumber;											//记住用户查找个数///

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBt1SaveEditpassworddlg();
	static long LoginUser_CurrentId;				//当前登录用户的ID号,用户修改密码时将用ID查找///

	virtual BOOL OnInitDialog();
};
