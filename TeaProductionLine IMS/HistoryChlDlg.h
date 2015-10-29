#pragma once
#include "mschart1.h"


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
};
