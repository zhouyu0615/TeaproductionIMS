// HistoryChlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "HistoryChlDlg.h"
#include "afxdialogex.h"


// CHistoryChlDlg �Ի���//

IMPLEMENT_DYNAMIC(CHistoryChlDlg, CDialogEx)

CHistoryChlDlg::CHistoryChlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryChlDlg::IDD, pParent)
	
{

}

CHistoryChlDlg::~CHistoryChlDlg()
{

}

void CHistoryChlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART1, m_Chart);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_COMBO_PARA2, m_LineComboBox);
	DDX_Control(pDX, IDC_COMBO_PARA3, m_ModuleComboBox);
	DDX_Control(pDX, IDC_COMBO_PARA, m_ParaComboBox);
}


BEGIN_MESSAGE_MAP(CHistoryChlDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA, &CHistoryChlDlg::OnCbnSelchangeComboPara)
END_MESSAGE_MAP()


// CHistoryChlDlg ��Ϣ�������//


void CHistoryChlDlg::OnCbnSelchangeComboPara()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


BOOL CHistoryChlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	size_t length = m_pDataProvider->m_vectProModulePara.size();
	for (size_t i = 0; i < length; i++)
	{
		if (m_pDataProvider->m_vectProModulePara[i].m_IsRecord == TRUE)
		{
			m_vRecordProPara.push_back(m_pDataProvider->m_vectProModulePara[i]);
		}
	}

	InitChart();

	return TRUE;
}



void CHistoryChlDlg::InitChart()
{

	m_Chart.put_TitleText(_T("��ʷ��¼"));

	m_Chart.put_ColumnCount(3);


	m_Chart.put_ShowLegend(TRUE);
	m_Chart.put_Column(1);
	m_Chart.put_ColumnLabel(_T("����1"));
	m_Chart.put_Column(2);
	m_Chart.put_ColumnLabel(_T("����2"));
	m_Chart.put_Column(3);
	m_Chart.put_ColumnLabel(_T("����3"));



	
 

}