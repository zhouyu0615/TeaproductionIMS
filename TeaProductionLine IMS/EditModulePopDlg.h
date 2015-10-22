#pragma once
#include "afxwin.h"
#include "DataProvider.h"


// CEditModulePopDlg 对话框

class CEditModulePopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditModulePopDlg)

public:
	CEditModulePopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditModulePopDlg();

	int m_nSelectedItem;
	CDataProvider * m_pDataprovicer = CDataProvider::getInstance();


// 对话框数据
	enum { IDD = IDD_EDITMODULE_POPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	CComboBox m_LineComboBox;
	CEdit m_ModuleNameEdit;
	CEdit m_DescriptionEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCo1EditmodulePopdlg();
	virtual void OnOK();
};
