#pragma once
#include "DataProvider.h"
#include <vector>
#include "FormulaClass.h"
#include "ProcessModulePara.h"
#include "afxcmn.h"

// CAddConfigParaPopDlg 对话框

class CAddConfigParaPopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddConfigParaPopDlg)

public:
	CAddConfigParaPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddConfigParaPopDlg();

// 对话框数据
	enum { IDD = IDD_ADDCONFIGPARA_POPDLG };
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();    //数据源//
	//std::vector<CProcessPara> m_vectOldPara;         //已进入配方的参数//
	std::vector<CProcessPara> m_vectNewPara;           //可选参数//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CListCtrl m_list;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};
