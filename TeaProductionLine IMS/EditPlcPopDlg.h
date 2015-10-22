#pragma once
#include "afxwin.h"
#include "DataProvider.h"


// CEditPlcPopDlg 对话框

class CEditPlcPopDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditPlcPopDlg)

public:
	CEditPlcPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPlcPopDlg();

// 对话框数据
	enum { IDD = IDD_EDITPLC_POPDLG };
	int m_nSelectedItem;
	CDataProvider * m_pDataProvider = CDataProvider::getInstance();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_PlcNameEdit; 
	CEdit m_IPAddrEdit;
	CEdit m_ReadAddrEdit;
	CEdit m_ReadLengthEdit;
	//CEdit m_WriteAddrEdit;
	//CEdit m_WriteLengthEdit;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
