#pragma once
#include "afxwin.h"
#include "DataProvider.h"


// CEditOwnerPopDlg 对话框

class CEditOwnerPopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditOwnerPopDlg)

public:
	CEditOwnerPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditOwnerPopDlg();

	int m_nSelectedItem;

// 对话框数据//
	enum { IDD = IDD_EDITOWNER_POPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	
	CEdit m_editOwerName;

	CEdit m_editOwerCode;
	CEdit m_editOwerPasswd;
	CEdit m_editDescription;
	afx_msg void OnBnClickedOk();

private:
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
