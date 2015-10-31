#pragma once
#include "MsChart/mschart1.h"
#include "DataProvider.h"
#include "ProcessModulePara.h"
#include "afxcmn.h"
#include "afxwin.h"


// CHistoryChlDlg �Ի���

class CHistoryChlDlg : public CDialogEx
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
	CProcessPara m_currentPara; //��ǰ��¼�Ĺ��ղ���//

public:
	virtual BOOL OnInitDialog();

	void InitChart();
	CListCtrl m_list1;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CComboBox m_ParaComboBox;
};
