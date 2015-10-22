#pragma once
#include "DataProvider.h"
#include "ProcessModulePara.h"
#include "afxwin.h"
#include "ParaCheckUtil.h"

// CEditModuleParaPopDlg 对话框

class CEditModuleParaPopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditModuleParaPopDlg)

public:
	CEditModuleParaPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditModuleParaPopDlg();

// 对话框数据
	enum { IDD = IDD_EDITMODULEPARA_POPDLG };
	int m_nSelectedItem;
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();

	CParaCheckUtil m_ParaCheckUtil;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_PlcComboBox;
	CEdit m_UnitEdit;
	CEdit m_NameEdit;
	CEdit m_ReadAddrIndexEdit;
	CEdit m_WriteAddrIndexEdit;
	CComboBox m_ValueTypeComboBox;

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeLine();
	virtual BOOL OnInitDialog();

	int ValTypeComboBoxInit();

	int LineComboxPaint();
	int ModuleComboxPaint(CString LineName);


	void ShowConfigState(BOOL IsConfig);
	void ShowVisibleState(BOOL IsVisible);
	BOOL GetConfigState();
	BOOL GetVisibleState();
	virtual void OnOK();
	CComboBox m_AddrTypeComboBox;
	
	void AddrTypeComboBoxInit();

	afx_msg void OnCbnSelchangeAddrType();
};
