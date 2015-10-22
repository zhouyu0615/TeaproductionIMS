#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataProvider.h"
#include "EditModuleParaPopDlg.h"
#include "ProcessModulePara.h"
#include "ParaCheckUtil.h"

// CEditModuleParaTabDlg 对话框

class CEditModuleParaTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditModuleParaTabDlg)

public:
	CEditModuleParaTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditModuleParaTabDlg();

// 对话框数据
	enum { IDD = IDD_EDITMODULEPARA_TABDLG };
	int m_nSelectedItem;
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
	CEditModuleParaPopDlg m_ModuleParaPopDlg;
	CParaCheckUtil m_ParaCheckUtil;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_PlcComboBox;
	CEdit m_UnitEdit;
	CEdit m_NameEdit;
	CEdit m_ReadAddrIndexEdit;
	CComboBox m_ValueTypeComboBox;

	CEdit m_WriteAddrIndex;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddItem();
	afx_msg void OnBnClickedClearEdit();
	afx_msg void OnBnClickedClearAll();
	afx_msg void OnCbnSelchangeCo2EditmoduleparaTabdlg();
	afx_msg void OnNMRClickLi1EditmoduleparaTabdlg(NMHDR *pNMHDR, LRESULT *pResult);

	int MyOnPaint();
	int ListOnPaint();
	int LineComboxPaint();
	int ModuleComboxPaint(CString LineName);
	int PlcComboxPaint();

	int ValTypeComboBoxInit();

	void ShowConfigState(BOOL IsConfig);
	void ShowVisibleState(BOOL IsVisible);

	BOOL GetConfigState();
	BOOL GetVisibleState();
	virtual void OnOK();
	CComboBox m_AddrTypeComboBox;

	void AddrTypeComboBoxInit();
	afx_msg void OnCbnSelchangeAddrType();
};
