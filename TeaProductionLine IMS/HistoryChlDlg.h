#pragma once

#include "MsChart.h"
#include "DataProvider.h"
#include "ProcessModulePara.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Thread.h"



// CHistoryChlDlg �Ի���

class CHistoryChlDlg : public CDialogEx,Runnable
{
	DECLARE_DYNAMIC(CHistoryChlDlg)

public:
	CHistoryChlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistoryChlDlg();

// �Ի�������
	enum { IDD = IDD_HISTORY_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboPara();

	CMschart1 m_Chart;


private:
	CDataProvider* m_pDataProvider = CDataProvider::getInstance();
	std::vector<CProcessPara> m_vRecordProPara; //��Ҫ��ʷ��¼�Ĺ��ղ���//
	std::map<int, int> m_ParaIndexMap; //KeyΪm_vRecordProPara,ValueΪm_dataProider��m_vectProModulePara�е��±�����



	size_t m_CurrentParaDataIndex;
	CProcessPara m_currentPara; //��ǰ��¼�Ĺ��ղ���//

	std::vector<CParaRecord>   m_vParaRecords; //��¼�Ĳ�����ֵ//

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

	void AddDataPointToChart(const CParaRecord& paraRecord); //���ͼ����������//
	void UpDateChartShow(); //����ͼ����ʾ����//


public:
	afx_msg void OnBnClickedButtonClear();

	void StopRecordThread();

	BOOL m_bIsRecording = FALSE; //��¼��ǰ�Ƿ�������¼�ı�־λ//
	afx_msg void OnBnClickedButtonExportRecord();
	afx_msg void OnBnClickedButtonImportRecord();
};
