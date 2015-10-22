#pragma once
#include "afxcmn.h"
#include "EditDeviceParaTabDlg.h"
#include "EditErrorParaTabDlg.h"
#include "EditModuleParaTabDlg.h"
#include "EditStateParaTabDlg.h"


// CEditPlcParaDlg 对话框

class CEditPlcParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditPlcParaDlg)

public:
	CEditPlcParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPlcParaDlg();

// 对话框数据
	enum { IDD = IDD_EDITPLCPARADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab1;
	virtual BOOL OnInitDialog();
	CEditDeviceParaTabDlg m_EditDeviceParaTabDlg;     //四个标签界面//
	CEditErrorParaTabDlg m_EditErrorParaTabDlg;
	CEditModuleParaTabDlg m_EditModuleParaTabDlg;
	CEditStateParaTabDlg m_EditStateParaTabDlg;
	afx_msg void OnTcnSelchangeTabEditplcparadlg(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
};
