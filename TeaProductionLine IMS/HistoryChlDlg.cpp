// HistoryChlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "HistoryChlDlg.h"
#include "afxdialogex.h"


#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"



// CHistoryChlDlg 对话框//

IMPLEMENT_DYNAMIC(CHistoryChlDlg, CDialogEx)

CHistoryChlDlg::CHistoryChlDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CHistoryChlDlg::IDD, pParent), m_pThread(NULL)
	
{
	//m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); //手动复原，初始状态无信号
}

CHistoryChlDlg::~CHistoryChlDlg()
{
	//free(m_pThread);
	//CloseHandle(m_hEvent);
}

void CHistoryChlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART1, m_Chart);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_COMBO_PARA2, m_LineComboBox);
	DDX_Control(pDX, IDC_COMBO_PARA3, m_ModuleComboBox);
	DDX_Control(pDX, IDC_COMBO_PARA, m_ParaComboBox);
	DDX_Control(pDX, IDC_BUTTON_START_RECORD, m_BtnStart);
}


BEGIN_MESSAGE_MAP(CHistoryChlDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA, &CHistoryChlDlg::OnCbnSelchangeComboPara)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CHistoryChlDlg::OnBnClickedButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA2, &CHistoryChlDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA3, &CHistoryChlDlg::OnCbnSelchangeModule)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CHistoryChlDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_RECORD, &CHistoryChlDlg::OnBnClickedButtonExportRecord)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_RECORD, &CHistoryChlDlg::OnBnClickedButtonImportRecord)
	ON_BN_CLICKED(IDC_BUTTON_START_RECORD, &CHistoryChlDlg::OnBnClickedButtonStartRecord)
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

	m_bIsSelectedPara = FALSE; //选中的参数有变动//

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
	
	if (m_bIsSelectedPara==TRUE) //当前选中的参数并未改变//
	{
		return;
	}
	m_bIsSelectedPara = SetCurrentPara();

	if (m_bIsSelectedPara)
	{
		//清空列表//
		m_list1.DeleteAllItems();

		CString tbName;
		tbName.Format(_T("tbParaRecord%d"), m_currentPara.m_Id);
		m_pDataProvider->ReadRecentParaRecords(tbName);

		int length = m_pDataProvider->m_vCurrentParaRecordes.size();
		for (int i = 0; i < length;i++)
		{
			AddItemToList(m_pDataProvider->m_vCurrentParaRecordes[i]);
			AddDataPointToChart(m_pDataProvider->m_vCurrentParaRecordes[i]);
		}
		UpDateChartShow();
	}
	else
	{
		AfxMessageBox(_T("没有确定当前记录的参数，请确定！"));
	}
	
}

void CHistoryChlDlg::OnBnClickedButtonStartRecord()
{
	OnBnClickedButtonOk();//调用确认参数的按钮
	if (m_bIsSelectedPara==FALSE)
	{
		return;
	}

	if (m_pThread == NULL)
	{
		m_pThread = new CThread(this);
		m_pThread->Start(); //创建线程，并等待
	}


	if (m_bIsRecording==FALSE) //停止状态切换到开始记录状态
	{
		//SetEvent(m_hEvent);

		m_bIsRecording = TRUE;
		m_pThread->Resume();
		m_BtnStart.SetWindowText(_T("停止记录"));

	}
	else //记录状态切换到停止记录状态
	{
		//ResetEvent(m_hEvent);
		m_pThread->Suspend();
		m_bIsRecording = FALSE;
		m_BtnStart.SetWindowText(_T("开始记录"));
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
		
		//WaitForSingleObject(m_hEvent, INFINITE);


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
	OnBnClickedButtonOk();//调用确认参数的按钮
	if (m_bIsSelectedPara == FALSE)
	{
		return;
	}


	CString tbName;
	tbName.Format(_T("tbParaRecord%d"), m_currentPara.m_Id);
	m_pDataProvider->ClearParaRecords(tbName);

	m_vChartData.clear();
	
	m_Chart.put_RowCount(1);  //清除图表
	InitChart();

	m_list1.DeleteAllItems();

	
}



void CHistoryChlDlg::StopRecordThread()
{
	
	if (m_pThread!=NULL)
	{
		m_pThread->Suspend();
	}
	
}



void CHistoryChlDlg::GetCellName(int nRow, int nCol, CString &strName)
{
	int nSeed = nCol;
	CString strRow;
	char cCell = 'A' + nCol - 1;
	strName.Format(_T("%c"), cCell);
	strRow.Format(_T("%d "), nRow);
	strName += strRow;
}


void CHistoryChlDlg::OnBnClickedButtonExportRecord()
{

	OnBnClickedButtonOk();//调用确认参数的按钮
	if (m_bIsSelectedPara==FALSE)
	{
		return;
	}


	BOOL isOpen = FALSE;
	CFileDialog FileDlg(isOpen, _T("(*.xlsx)"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("(*.xlsx)|*.xlsx||"), NULL);

	if (FileDlg.DoModal() == IDOK)
	{
		//获取路径
		CString strFileName = FileDlg.GetPathName();
		COleVariant
			covTrue((short)TRUE),
			covFalse((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

		CApplication app;
		CWorkbook book;
		CWorkbooks books;
		CWorksheet sheet;
		CWorksheets sheets;
		CRange range;
		CFont0 font;

		if (!app.CreateDispatch(_T("Excel.Application")))
		{
			MessageBox(_T("创建失败!"));
		}

		books = app.get_Workbooks();
		book = books.Add(covOptional);
		sheets = book.get_Worksheets();
		sheet = sheets.get_Item(COleVariant((short)1));

		//range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1")));
		//range.put_Value2(COleVariant(_T("hanhan")));

		CHeaderCtrl   *pmyHeaderCtrl;
		pmyHeaderCtrl = m_list1.GetHeaderCtrl();//此句取得CListCtrl控件的列表頭

		int   iRow, iCol;
		int   m_cols = pmyHeaderCtrl->GetItemCount();
		int   m_rows = m_list1.GetItemCount();
		HDITEM   hdi;
		TCHAR     lpBuffer[256];
		bool       fFound = false;
		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;
		CString   colname;
		CString strTemp;

		for (iCol = 0; iCol < m_cols; iCol++)//将列表的标题头写入EXCEL
		{
			GetCellName(1, iCol + 1, colname);
			range = sheet.get_Range(COleVariant(colname), COleVariant(colname));
			pmyHeaderCtrl->GetItem(iCol, &hdi);
			range.put_Value2(COleVariant(hdi.pszText));

			int   nWidth = m_list1.GetColumnWidth(iCol) / 6;
			//得到第iCol+1列  
			range.AttachDispatch(range.get_Item(_variant_t((long)(iCol + 1)), vtMissing).pdispVal, true);

			//设置列宽 
			range.put_ColumnWidth(_variant_t((long)nWidth));
		}

		range = sheet.get_Range(COleVariant(_T("A1 ")), COleVariant(colname));
		range.put_RowHeight(_variant_t((long)50));//设置行的高度
		font = range.get_Font();
		font.put_Bold(covTrue);
		range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108
		COleSafeArray   saRet;
		DWORD   numElements[] = { m_rows, m_cols };       //5x2   element   array
		saRet.Create(VT_BSTR, 2, numElements);
		range = sheet.get_Range(COleVariant(_T("A2 ")), covOptional);
		range = range.get_Resize(COleVariant((short)m_rows), COleVariant((short)m_cols));
		
		long   index[2];
		range = sheet.get_Range(COleVariant(_T("A2 ")), covOptional);

		range = range.get_Resize(COleVariant((short)m_rows), COleVariant((short)m_cols));
		for (iRow = 1; iRow <= m_rows; iRow++)//将列表内容写入EXCEL
		{
			for (iCol = 1; iCol <= m_cols; iCol++)
			{
				index[0] = iRow - 1;
				index[1] = iCol - 1;
				CString   szTemp;
				szTemp = m_list1.GetItemText(iRow - 1, iCol - 1);
				BSTR   bstr = szTemp.AllocSysString();
				saRet.PutElement(index, bstr);
				SysFreeString(bstr);
			}

		}
		range.put_Value2(COleVariant(saRet));
		saRet.Detach();
		book.SaveCopyAs(COleVariant(strFileName));
		book.put_Saved(true);
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();
	}
}


void CHistoryChlDlg::OnBnClickedButtonImportRecord()
{
	OnBnClickedButtonOk();//调用确认参数的按钮
	if (m_bIsSelectedPara == FALSE)
	{
		return;
	}



	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir =_T("C:\\Users\\Administrator\\Desktop");	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"文件 (*.xls)|*.xls||";	//文件过滤的类型//
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);

	openFileDlg.m_ofn.lpstrFileTitle = _T("导入文件");
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.doc";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
}







