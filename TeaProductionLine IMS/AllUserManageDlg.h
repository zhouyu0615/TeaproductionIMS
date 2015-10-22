#pragma once
#include "afxcmn.h"
#include "DataProvider.h"
#include "EditUserPopDlg.h"
#include "afxwin.h"


// CAllUserManageDlg 对话框

class CAllUserManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CAllUserManageDlg)

public:
	CAllUserManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAllUserManageDlg();

// 对话框数据
	enum { IDD = IDD_ALLUSERMANAGEDLG };
	int index;//用于暂存操作员单击列表框时鼠标所在的行标///
    //static long UserID;		//列表框中选中用户的ID///
	CDataProvider *m_pDataProvider=CDataProvider::getInstance(); //数据源///


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_UserInfo;			//用户管理界面列表框的成员变量///
	afx_msg void OnBnClickedBt1Adduser();
	virtual BOOL OnInitDialog();
	void MyOnPint();
	afx_msg void OnBnClickedBt2Clearall();
	afx_msg void OnNMRClickLi1Allusermanagedlg(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edUserName;
	CEdit m_edPassword;
};
