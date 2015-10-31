#pragma once
#include "MsChart/mschart1.h"
#include "DataProvider.h"
#include "ProcessModulePara.h"
#include "afxcmn.h"
#include "afxwin.h"


// CHistoryChlDlg 对话框

class CHistoryChlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryChlDlg)

public:
	CHistoryChlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistoryChlDlg();

// 对话框数据
	enum { IDD = IDD_HISTORY_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboPara();

	CMschart1 m_Chart;


private:
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
	std::vector<CProcessPara> m_vRecordProPara; //需要历史记录的工艺参数//
	CProcessPara m_currentPara; //当前记录的工艺参数//

public:
	virtual BOOL OnInitDialog();

	void InitChart();
	CListCtrl m_list1;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_ParaComboBox;
};
