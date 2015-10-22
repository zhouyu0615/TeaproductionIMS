#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataProvider.h"
#include "DevicePara.h"
#include "EditDeviceParaPopDlg.h"
#include "ParaCheckUtil.h"


// CEditDeviceParaTabDlg 对话框

class CEditDeviceParaTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditDeviceParaTabDlg)

public:
	CEditDeviceParaTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditDeviceParaTabDlg();

// 对话框数据
	enum { IDD = IDD_EDITDEVICEPARA_TABDLG };

	

	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	int m_nSelectedItem;
	CEditDeviceParaPopDlg m_DeviceParaDlg;

	CParaCheckUtil m_ParaCheckUtil;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	CEdit m_NameEdit;
	CEdit m_ControlAddrEdit;
	CEdit m_StateAddrEdit;
	CEdit m_DescriptionEdit;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_DeviceComoboBox;
	CComboBox m_PlcComboBox;
	afx_msg void OnBnClickedAddItem();
	afx_msg void OnBnClickedClearEdit();
	afx_msg void OnBnClickedClearAll();
	virtual BOOL OnInitDialog();
	int MyOnPaint();

	int ListOnPaint();
	int LineComboxPaint();
	int ModuleComboxPaint(CString LineName);
	int DeviceComboxPaint(CString LineName, CString ModuleName);
	int PlcComboxPaint();



	afx_msg void OnCbnSelchangeLine();
	afx_msg void OnCbnSelchangeModule();
	afx_msg void OnNMRClickLi1EditdeviceparaTabdlg(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
};
