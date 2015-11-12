#pragma once

#include "MsChart.h"
#include "DataProvider.h"
#include "ProcessModulePara.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Thread.h"



// CHistoryChlDlg 对话框

class CHistoryChlDlg : public CDialogEx,Runnable
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
	std::map<int, int> m_ParaIndexMap; //Key为m_vRecordProPara,Value为m_dataProider中m_vectProModulePara中的下标索引



	size_t m_CurrentParaDataIndex;
	CProcessPara m_currentPara; //当前记录的工艺参数//

	std::vector<CParaRecord>   m_vParaRecords; //记录的参数的值//

public:
	virtual BOOL OnInitDialog();

	void InitChart();
	CListCtrl m_list1;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_ParaComboBox;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnCbnSelchangeLine();
	afx_msg void OnCbnSelchangeModule();
	CComboBox m_TimeIntervalComboBox;

	void LineComboxPaint();
	void ModuleComboxPaint(const CString& LineName);
	void ParaComboxPaint(const CString& LineName, const CString& ModuleName);

private:
	void InitList();
	void SetListItemText(int Index, CParaRecord &paraRecord);
	void UpdateItemInList(int Index, CParaRecord &paraRecord);
	void AddItemToList(CParaRecord &paraRecord);


	BOOL SetCurrentPara();

	virtual void Run();
	CThread* m_pThread;

	
	
	//CArray<double, double> m_arrary;

	std::vector<CParaRecord> m_vChartData;
	const static int MAX_ROW_COUNT = 7;

	void AddDataPointToChart(const CParaRecord& paraRecord); //添加图表数据内容//
	void UpDateChartShow(); //更新图表显示界面//


public:
	afx_msg void OnBnClickedButtonClear();

	void StopRecordThread();

	BOOL m_bIsRecording = FALSE; //记录当前是否真正记录的标志位//
	afx_msg void OnBnClickedButtonExportRecord();
	afx_msg void OnBnClickedButtonImportRecord();
};
