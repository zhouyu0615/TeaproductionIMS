// HistoryChlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "HistoryChlDlg.h"
#include "afxdialogex.h"


// CHistoryChlDlg 对话框//

IMPLEMENT_DYNAMIC(CHistoryChlDlg, CDialogEx)

CHistoryChlDlg::CHistoryChlDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CHistoryChlDlg::IDD, pParent), m_pThread(NULL)
	
{

}

CHistoryChlDlg::~CHistoryChlDlg()
{
	free(m_pThread);
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
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CHistoryChlDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_RECORD, &CHistoryChlDlg::OnBnClickedButtonExportRecord)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_RECORD, &CHistoryChlDlg::OnBnClickedButtonImportRecord)
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
	int index = 0;
	m_ParaIndexMap.clear();
	for (size_t i = 0; i < length; i++)
	{
		if (m_pDataProvider->m_vectProModulePara[i].m_IsRecord == TRUE)
		{
			m_vRecordProPara.push_back(m_pDataProvider->m_vectProModulePara[i]);
			
			m_ParaIndexMap[index] = i;
			index++;
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


	m_Chart.put_ColumnCount(10);

	m_Chart.put_ShowLegend(FALSE);
	m_Chart.put_RandomFill(FALSE);
	//m_Chart.put_Column(1);
	//m_Chart.put_ColumnLabel(_T("参数1"));

	CVcTitle vctitle;
	CVcFont	 vcFont;
	CVcColor vcColor;
	vctitle = m_Chart.get_Title();
	vcFont = vctitle.get_VtFont();
	vcColor = vcFont.get_VtColor();
	vcColor.Set(0, 255, 0);
	vcFont.put_Size(20);

	m_Chart.put_TitleText(_T("历史记录"));



	CVcBackdrop vcbackdrop;
	CVcFill vcfill;
	vcbackdrop = m_Chart.get_Backdrop();
	vcfill = vcbackdrop.get_Fill();
	vcfill.put_Style(1);

	CVcBrush vcbrush;
	vcbrush = vcfill.get_Brush();
	vcColor = vcbrush.get_FillColor();
	vcColor.Set(100, 100, 100);


	m_Chart.put_RowCount(MAX_ROW_COUNT);

	m_Chart.put_Row(1);
	m_Chart.put_RowLabel((LPCTSTR)(_T("12")));

	m_Chart.put_Row(2);
	m_Chart.put_RowLabel((LPCTSTR)(_T("24")));


	m_Chart.put_Stacking(TRUE);


	VARIANT var;

	CVcPlot vcplot;
	CVcAxis vcaxis;
	CVcAxisTitle vcAxistitle;
	CVcTextLayout textlayout;
	

	vcplot = m_Chart.get_Plot();
	vcaxis = vcplot.get_Axis(0, var);
	vcAxistitle = vcaxis.get_AxisTitle();
	vcAxistitle.put_Text(_T("时间(s)"));

	vcaxis = vcplot.get_Axis(1, var);
	vcAxistitle = vcaxis.get_AxisTitle();  
	textlayout = vcAxistitle.get_TextLayout();
	textlayout.put_Orientation(1); //设置字体横竖排放//
	vcAxistitle.put_Text(_T("温度"));
	

	CVcPlot vcPlot = m_Chart.get_Plot();
	CVcAxis vcAxis = vcPlot.get_Axis(0, var);
	CVcCategoryScale vccat = vcAxis.get_CategoryScale();
	vccat.put_Auto(FALSE);
	vccat.put_DivisionsPerLabel(1);
	vccat.put_DivisionsPerTick(1);




	vcAxis = vcPlot.get_Axis(1, var);
	CVcValueScale vcvalue = vcAxis.get_ValueScale();
	vcvalue.put_MajorDivision(10);
	//vcvalue.put_Maximum(100);
	//vcvalue.put_Minimum(0);


	CVcSeriesCollection vcseriescollection;
	vcseriescollection = vcplot.get_SeriesCollection();
	CVcSeries vcseries = vcseriescollection.get_Item(1);



	CVcPen vcpen = vcseries.get_Pen();
	vcColor = vcpen.get_VtColor();
	vcColor.Set(255, 0, 0);
	vcpen.put_Width(25);



	CVcDataPoints datapoints;
	CVcDataPoint datatpont;
	datapoints = vcseries.get_DataPoints();
	CVcDataPointLabel vcdatapointslabel;
	datatpont = datapoints.get_Item(-1);
	vcdatapointslabel = datatpont.get_DataPointLabel();
	vcdatapointslabel.put_LocationType(1);


	CVcSeriesMarker vcseriesmarker;
	vcseriesmarker = vcseries.get_SeriesMarker();
	vcseriesmarker.put_Auto(FALSE);


	vcFont = vcdatapointslabel.get_VtFont();
	vcColor = vcFont.get_VtColor();
	vcColor.Set(255, 255, 0);


	CVcMarker vcmarker = datatpont.get_Marker();
	vcmarker.put_Size(150);
	vcmarker.put_Visible(TRUE);

	vcmarker.put_Style(10);



	vcColor = vcmarker.get_FillColor();
	//	vcpen = vcmarker.get_Pen();
	//	vcColor = vcpen.get_VtColor();
	vcColor.Set(0, 255, 255);

	vcpen = vcmarker.get_Pen();
	vcColor = vcpen.get_VtColor();
	vcColor.Set(0, 255, 255);



	vcpen = vcAxis.get_Pen();
	vcColor = vcpen.get_VtColor();
	vcColor.Set(0, 255, 0);


	CVcLabels vclabels;
	vclabels = vcAxis.get_Labels();
	CVcLabel vclabel;
	vclabel = vclabels.get_Item(1);
	vcFont = vclabel.get_VtFont();
	vcColor = vcFont.get_VtColor();
	vcColor.Set(255, 255, 0);



	vcAxis = vcplot.get_Axis(1, var);
	vcvalue = vcAxis.get_ValueScale();
	vcvalue.put_Auto(TRUE);
	//vcvalue.put_Maximum(300);
	//vcvalue.put_Minimum(-100);
	vcvalue.put_MajorDivision(20);
	vcvalue.put_MinorDivision(1);
	
}

void CHistoryChlDlg::OnBnClickedButtonOk()
{

	if (m_bIsRecording==TRUE)
	{
		return;
	}
	BOOL Init_Success = SetCurrentPara();
	if (Init_Success)
	{
		CString tbName;
		tbName.Format(_T("tbParaRecord%d"), m_currentPara.m_Id);
		m_pDataProvider->ReadRecentParaRecords(tbName);

		int length = m_pDataProvider->m_vCurrentParaRecordes.size();
		for (int i = 0; i < length;i++)
		{
			AddItemToList(m_pDataProvider->m_vCurrentParaRecordes[i]);
			AddDataPointToChart(m_pDataProvider->m_vCurrentParaRecordes[i]);
		}


		if (m_pThread==NULL)
		{
			m_pThread = new CThread(this);
			m_pThread->Start();
		}

		m_bIsRecording = TRUE;
	}
	
}

/*
返回值：
 True 确定了当前观察的工艺参数
 False 确定参数失败
*/
BOOL CHistoryChlDlg::SetCurrentPara()
{
	CString LineName, ModuleName, paraName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);
	m_ParaComboBox.GetWindowText(paraName);

	if (paraName=="")
	{
		AfxMessageBox(_T("未选定参数！"));
		return FALSE;
	}

	for (size_t i = 0; i < m_vRecordProPara.size(); i++)
	{
		if (LineName == m_vRecordProPara[i].m_strProductionLineName
			&&ModuleName == m_vRecordProPara[i].m_strProcessModuleName
			&&paraName == m_vRecordProPara[i].m_strParaName)
		{
			m_currentPara = m_vRecordProPara[i];
			m_CurrentParaDataIndex = m_ParaIndexMap[i];

			return TRUE;
		}
	}

	return FALSE;
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


void CHistoryChlDlg::AddItemToList(CParaRecord &paraRecord)
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");

	size_t index = m_list1.GetItemCount();
	litem.iItem = index;
	m_list1.InsertItem(&litem);

	SetListItemText(index, paraRecord);
	m_list1.EnsureVisible(index,TRUE);

}

void CHistoryChlDlg::Run()
{
	std::map<int, int>::const_iterator pIter;
	while (TRUE)
	{

		//for (pIter = m_ParaIndexMap.begin(); pIter != m_ParaIndexMap.end();++pIter)
		//{
		//	CParaRecord tParaRecord;
		//	CProcessPara proPara = m_pDataProvider->m_vectProModulePara[pIter->second];
		//	tParaRecord.m_ProParaId = proPara.m_Id;
		//	tParaRecord.m_fParaValue = proPara.m_ParaValue;
		//	CString tbName;
		//	tbName.Format(_T("tbParaRecord%d"), tParaRecord.m_ProParaId);
		//	m_pDataProvider->AddParaReordToTb(tbName, tParaRecord);

		//	if (m_CurrentParaDataIndex == pIter->second)
		//	{
		//		AddItemToList(tParaRecord);
		//		AddDataPointToChart(tParaRecord);
		//		UpDateChartShow();
		//	}		
		//}

			CParaRecord tParaRecord;
			CProcessPara proPara = m_pDataProvider->m_vectProModulePara[m_CurrentParaDataIndex];
			tParaRecord.m_ProParaId = proPara.m_Id;
			tParaRecord.m_fParaValue = proPara.m_ParaValue;
			tParaRecord.m_CreateTime = CTime::GetCurrentTime();
			CString tbName;
			tbName.Format(_T("tbParaRecord%d"), tParaRecord.m_ProParaId);
			m_pDataProvider->AddParaReordToTb(tbName, tParaRecord);

			AddItemToList(tParaRecord);

			AddDataPointToChart(tParaRecord);
			UpDateChartShow();
			


		
		Sleep(3 * 1000);

	}

}



//更新图表数据内容//
void CHistoryChlDlg::AddDataPointToChart(const CParaRecord& paraRecord)
{
	int length = m_vChartData.size();
	if (length<MAX_ROW_COUNT)
	{
		m_vChartData.push_back(paraRecord);
	}
	else
	{
		m_vChartData.erase(m_vChartData.begin());
		m_vChartData.push_back(paraRecord);
	}
}



void CHistoryChlDlg::UpDateChartShow()
{

	m_Chart.SetRedraw(FALSE);
	CRgn rgn;
	m_Chart.GetUpdateRgn(&rgn);

	int length = m_vChartData.size();
	for (int i = 1; i <= length; i++)
	{
		CVcDataGrid vcDataGrid = m_Chart.get_DataGrid();
		vcDataGrid.SetData(i, 1, m_vChartData[i-1].m_fParaValue, 0);

		m_Chart.put_Row(i);
		m_Chart.put_RowLabel(m_vChartData[i - 1].getCreateTimeString());
		
	}
	m_Chart.SetRedraw(TRUE);
	m_Chart.InvalidateRgn(&rgn, FALSE);
}


void CHistoryChlDlg::OnBnClickedButtonClear()
{
	CString tbName;
	tbName.Format(_T("tbParaRecord%d"), m_currentPara.m_Id);
	m_pDataProvider->ClearParaRecords(tbName);


	m_vChartData.clear();
	
	m_Chart.put_RowCount(1);  //清除图表//
	InitChart();

	m_list1.DeleteAllItems();

	
}



void CHistoryChlDlg::StopRecordThread()
{
	if (m_pThread !=NULL)
	{
		m_pThread->Suspend();
	}
	
}


void CHistoryChlDlg::OnBnClickedButtonExportRecord()
{

	

}


void CHistoryChlDlg::OnBnClickedButtonImportRecord()
{
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = L"E:\\FileTest";	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"文件 (*.xls)|*.xls||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"E:\\FileTest\\test.doc";
	openFileDlg.GetOFN().lpstrFileTitle = L"导入文件";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.doc";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
}




