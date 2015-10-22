#pragma once
#include "LoginDlg.h"

// CEditUserPopDlg 对话框

class CEditUserPopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUserPopDlg)

public:
	CEditUserPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditUserPopDlg();

// 对话框数据
	enum { IDD = IDD_EDITUSER_POPDLG };
	CDataProvider *m_pDataProvider = CDataProvider::getInstance(); //数据源///

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	long EditUserId;		//列表框选中用户的ID///
	long ManagerId;			//存储管理员的ID///
	int EditUserIndex;
	virtual BOOL OnInitDialog();
};
