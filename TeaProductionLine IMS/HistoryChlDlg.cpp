// HistoryChlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "HistoryChlDlg.h"
#include "afxdialogex.h"


// CHistoryChlDlg 对话框//

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
	DDX_Control(pDX, IDC_COMBO_TIME_INTERVAL, m_TimeIntervalComboBox);
}


BEGIN_MESSAGE_MAP(CHistoryChlDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA, &CHistoryChlDlg::OnCbnSelchangeComboPara)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CHistoryChlDlg::OnBnClickedButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA2, &CHistoryChlDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA3, &CHistoryChlDlg::OnCbnSelchangeModule)
END_MESSAGE_MAP()


// CHistoryChlDlg 消息处理程序//


void CHistoryChlDlg::OnCbnSelchangeComboPara()
{
	// TODO:  在此添加控件通知处理程序代码
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

	LineComboxPaint();
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);
	
	ParaComboxPaint(LineName, ModuleName);



	InitChart();

	InitList();

	return TRUE;
}


void CHistoryChlDlg::InitList()
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");
	//设置列表控件风格//
	CRect rect1;
	m_list1.GetWindowRect(&rect1);
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE);

	//清空列表//
	m_list1.DeleteAllItems();

	m_list1.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_list1.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 8, -1);
	m_list1.InsertColumn(2, _T("参数名称"), LVCFMT_CENTER, rect1.Width() / 8 * 3, -1);
	m_list1.InsertColumn(3, _T("参数值"), LVCFMT_CENTER, rect1.Width() / 8 * 2, -1);
	m_list1.InsertColumn(4, _T("单位"), LVCFMT_CENTER, rect1.Width() / 8 * 2, -1);

	for (size_t i = 0; i < m_vParaRecords.size();i++)
	{
		litem.iItem = i;
		m_list1.InsertItem(&litem);
		SetListItemText(i, m_vParaRecords[i]);
	}

}


void CHistoryChlDlg::LineComboxPaint()
{
	std::set<CString> LineNamePool;
	m_LineComboBox.ResetContent();
	for (int i = 0; i < m_vRecordProPara.size();i++)
	{
		CString LineName = m_vRecordProPara[i].m_strProductionLineName;
		if (LineNamePool.count(LineName) == 0)
		{
			LineNamePool.insert(LineName);
			m_LineComboBox.AddString(LineName);
		}
	}
	m_LineComboBox.SetCurSel(0);
}


void CHistoryChlDlg::ModuleComboxPaint(const CString& LineName)
{
	std::set<CString> ModuleNamePool;
	m_ModuleComboBox.ResetContent();

	for (int i = 0; i < m_vRecordProPara.size(); i++)
	{
		if (m_vRecordProPara[i].m_strProductionLineName==LineName)
		{
			CString ModuleName = m_vRecordProPara[i].m_strProcessModuleName;
			if (ModuleNamePool.count(ModuleName) == 0)
			{
				ModuleNamePool.insert(ModuleName);
				m_ModuleComboBox.AddString(ModuleName);
			}
		}
	}
	m_ModuleComboBox.SetCurSel(0);

}
void CHistoryChlDlg::ParaComboxPaint(const CString& LineName, const CString& ModuleName)
{
	m_ParaComboBox.ResetContent();
	for (int i = 0; i < m_vRecordProPara.size();i++)
	{
		if (m_vRecordProPara[i].m_strProductionLineName==LineName&&
			m_vRecordProPara[i].m_strProcessModuleName==ModuleName)
		{
			m_ParaComboBox.AddString(m_vRecordProPara[i].m_strParaName);
			m_ParaComboBox.SetCurSel(0);
		}
	}
	

}


void CHistoryChlDlg::InitChart()
{

	m_Chart.put_TitleText(_T("历史记录"));

	m_Chart.put_ColumnCount(1);


	m_Chart.put_ShowLegend(FALSE);
	m_Chart.put_Column(1);
	m_Chart.put_ColumnLabel(_T("参数1"));





	
}

void CHistoryChlDlg::OnBnClickedButtonOk()
{

	SetCurrentPara();





}

void CHistoryChlDlg::SetCurrentPara()
{
	CString LineName, ModuleName, paraName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);
	m_ParaComboBox.GetWindowText(paraName);

	for (size_t i = 0; i < m_vRecordProPara.size(); i++)
	{
		if (LineName == m_vRecordProPara[i].m_strProductionLineName
			&&ModuleName == m_vRecordProPara[i].m_strProcessModuleName
			&&paraName == m_vRecordProPara[i].m_strParaName)
		{
			m_currentPara = m_vRecordProPara[i];
			return;
		}
	}
}


void CHistoryChlDlg::OnCbnSelchangeLine()
{
	
	CString LineName,ModuleName;
	m_LineComboBox.GetWindowText(LineName);
    
	ModuleComboxPaint(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);

	ParaComboxPaint(LineName, ModuleName);
}


void CHistoryChlDlg::OnCbnSelchangeModule()
{
	
	CString LineName,  ModuleName;

	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);

	ParaComboxPaint(LineName,ModuleName);
}


void CHistoryChlDlg::SetListItemText(int Index, CParaRecord &paraRecord)
{
	CString str;
	str.Format(_T("%d"), Index + 1);
	m_list1.SetItemText(Index, 1, str);
	m_list1.SetItemText(Index, 2, m_currentPara.m_strParaName);
	str.Format(_T("%.2f"), paraRecord.m_fParaValue);
	m_list1.SetItemText(Index, 3, str);
	m_list1.SetItemText(Index, 4, m_currentPara.m_strUnit);

}
