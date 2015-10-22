#pragma once
#include "afxwin.h"
#include "DataProvider.h"


// CEditLinePopDlg 对话框

class CEditLinePopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditLinePopDlg)

public:
	CEditLinePopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditLinePopDlg();

// 对话框数据
	enum { IDD = IDD_EDITLINE_POPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nSelectedItem;
	CEdit m_editLineName;
	CEdit m_editCapacity;
	CEdit m_editDescription;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();

	virtual void OnOK();
};
