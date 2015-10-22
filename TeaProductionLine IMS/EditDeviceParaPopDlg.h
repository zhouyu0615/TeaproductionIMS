#pragma once
#include "afxwin.h"
#include "DataProvider.h"
#include "DevicePara.h"
#include "ParaCheckUtil.h"


// CEditDeviceParaPopDlg 对话框

class CEditDeviceParaPopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditDeviceParaPopDlg)

public:
	CEditDeviceParaPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditDeviceParaPopDlg();

// 对话框数据
	enum { IDD = IDD_EDITDEVICEPARA_POPDLG };
	int m_nSelectedItem;
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
	
	CParaCheckUtil m_ParaCheckUtil;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_DeviceComboBox;
	CComboBox m_PlcComboBox;
	CEdit m_NameEdit;
	CEdit m_ControlAddrEdit;
	CEdit m_StateAddrEdit;
	CEdit m_DescriptionEdit;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	int LineComboxPaint();
	int ModuleComboxPaint(CString LineName);
	int DeviceComboxPaint(CString LineName, CString ModuleName);
	afx_msg void OnCbnSelchangeLine();
	afx_msg void OnCbnSelchangeModule();

	virtual void OnOK();
	afx_msg void OnCbnSelchangeCo3EditdeviceparaPopdlg();
};
