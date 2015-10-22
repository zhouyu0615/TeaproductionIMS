#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataProvider.h"
#include "FaultPara.h"
#include "EditErrorParaPopDlg.h"
#include "ParaCheckUtil.h"


// CEditErrorParaTabDlg 对话框

class CEditErrorParaTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditErrorParaTabDlg)

public:
	CEditErrorParaTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditErrorParaTabDlg();

// 对话框数据
	enum { IDD = IDD_EDITERRORPARA_TABDLG };
	int m_nSelectedItem;
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();

	CEditErrorParaPopDlg m_FaultParaPopDlg;

	CParaCheckUtil m_ParaCheckUtil;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_DeviceComboBox;
	CComboBox m_PlcComboBox;
	afx_msg void OnBnClickedAddItem();
	afx_msg void OnBnClickedClearEdit();
	afx_msg void OnBnClickedClearAllTabdlg();
	CEdit m_NameEdit;
	CEdit m_AddrIndexEdit;
	CEdit m_DescriptionEdit;
	afx_msg void OnCbnSelchangeLine();
	afx_msg void OnCbnSelchangeModule();
	virtual BOOL OnInitDialog();

	int MyOnPaint();
	int ListOnPaint();
	int LineComboxPaint();
	int ModuleComboxPaint(CString LineName);
	int DeviceComboxPaint(CString LineName, CString ModuleName);
	int PlcComboxPaint();
	afx_msg void OnNMRClickLi1EditerrorparaTabdlg(NMHDR *pNMHDR, LRESULT *pResult);


	virtual void OnOK();
};
