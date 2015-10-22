// OldConfigGraChiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "OldConfigGraChiDlg.h"
#include "afxdialogex.h"


// COldConfigGraChiDlg 对话框

IMPLEMENT_DYNAMIC(COldConfigGraChiDlg, CDialogEx)

COldConfigGraChiDlg::COldConfigGraChiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COldConfigGraChiDlg::IDD, pParent)
{
	m_bIsNameChange=false;                 //当前编辑区内的配方是否发生了更改//
	m_bIsParaValueChange=false;            //是否修改了配方参数的值//
	m_bIsNoteChange=false;                 //是否修改了配方的备注//
	m_bIsDelePara=false;                   //是否删除了配方参数//
	m_bIsAddPara=false;                    //是否添加了配方参数//
	m_strOldName=_T("");                  //修改之前的配方名//
	m_strOldNote = _T("");                 //修改之前的备注//
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
	m_bIsCurConfigChange = false;
}

COldConfigGraChiDlg::~COldConfigGraChiDlg()
{
}

void COldConfigGraChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listAllConfig);
	DDX_Control(pDX, IDC_LIST1, m_listConfigPara);
	DDX_Control(pDX, IDC_TREE1, m_treeConfig);
	DDX_Control(pDX, IDC_EDIT4, m_edConfigName);
	DDX_Control(pDX, IDC_EDIT5, m_edConfigLine);
	DDX_Control(pDX, IDC_EDIT6, m_edNote);
}


BEGIN_MESSAGE_MAP(COldConfigGraChiDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &COldConfigGraChiDlg::OnTvnSelchangedTree1)
//	ON_EN_CHANGE(IDC_EDIT4, &COldConfigGraChiDlg::OnEnChangeEdit4)
ON_NOTIFY(NM_RCLICK, IDC_LIST1, &COldConfigGraChiDlg::OnNMRClickList1)
ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &COldConfigGraChiDlg::OnTvnSelchangingTree1)
//ON_EN_KILLFOCUS(IDC_EDIT4, &COldConfigGraChiDlg::OnEnKillfocusEdit4)
//ON_EN_KILLFOCUS(IDC_EDIT6, &COldConfigGraChiDlg::OnEnKillfocusEdit6)
ON_BN_CLICKED(IDC_BUTTON6, &COldConfigGraChiDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON2, &COldConfigGraChiDlg::OnBnClickedButton2)
ON_NOTIFY(NM_CLICK, IDC_LIST1, &COldConfigGraChiDlg::OnNMClickList1)
ON_EN_KILLFOCUS(IDC_EDIT_CREATE1, &COldConfigGraChiDlg::OnEnKillfocusEditCreate1)
ON_BN_CLICKED(IDC_BUTTON1, &COldConfigGraChiDlg::OnBnClickedButton1)
ON_EN_KILLFOCUS(IDC_EDIT4, &COldConfigGraChiDlg::OnEnKillfocusEdit4)
ON_BN_CLICKED(IDC_BUTTON7, &COldConfigGraChiDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON3, &COldConfigGraChiDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON4, &COldConfigGraChiDlg::OnBnClickedButton4)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTTON8, &COldConfigGraChiDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// COldConfigGraChiDlg 消息处理程序


BOOL COldConfigGraChiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bHaveInit = true;

	// TODO:  在此添加额外的初始化
	//初始化两个列表的表头//
	CRect rectList1;
	m_listAllConfig.GetWindowRect(&rectList1);
	m_listAllConfig.SetExtendedStyle(m_listAllConfig.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE | LVS_EX_GRIDLINES);
	m_listAllConfig.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listAllConfig.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList1.Width() / 11.0, -1);
	m_listAllConfig.InsertColumn(2, _T("生产线名称"), LVCFMT_CENTER, rectList1.Width() / 11.0*4.0, -1);
	m_listAllConfig.InsertColumn(3, _T("正在使用的配方"), LVCFMT_CENTER, rectList1.Width() / 11.0*3.0, -1);
	m_listAllConfig.InsertColumn(4, _T("默认配方"), LVCFMT_CENTER, rectList1.Width() / 11.0*3.0, -1);

	CRect rectList2;
	m_listConfigPara.GetWindowRect(&rectList2);
	m_listConfigPara.SetExtendedStyle(m_listConfigPara.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LBS_OWNERDRAWVARIABLE);
	m_listConfigPara.InsertColumn(0, _T(""), LVCFMT_CENTER, rectList2.Width() / 24.0, -1);
	m_listConfigPara.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList2.Width() / 9.0, -1);
	m_listConfigPara.InsertColumn(2, _T("参数名称"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 4.0, -1);
	m_listConfigPara.InsertColumn(3, _T("设定值"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 1.8, -1);
	m_listConfigPara.InsertColumn(4, _T("单位"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 1.8, -1);
	HICON hIcon[3];      // 图标句柄数组//
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICONTREE);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON4);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON3);
	// 创建图像序列CImageList对象   
	m_imageList.Create(24, 24, ILC_COLOR32, 3, 3);
	m_imageList.Add(hIcon[0]);
	m_imageList.Add(hIcon[1]);
	m_imageList.Add(hIcon[2]);
	InitTreeContrl();                      //填写树形区域//
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size();n++)          //填写列表框1//
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);

	CRect rectTemp;                //记住原始尺寸//
	if (!m_bHaveRemenberOldSize)
	{
		GetClientRect(&rectTemp);
		m_OldSize.x = rectTemp.right - rectTemp.left;
		m_OldSize.y = rectTemp.bottom - rectTemp.top;
		RememberOldSize(m_vectOldSize);
		m_bHaveRemenberOldSize = true;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//填写树形区域//
void COldConfigGraChiDlg::InitTreeContrl()
{
	m_treeConfig.DeleteAllItems();
	HTREEITEM hRoot;     // 树的根节点的句柄//   
	HTREEITEM hFirItem;  // 可表示任一一级节点的句柄//
	// 为树形控件设置图像序列//  
	m_treeConfig.SetImageList(&m_imageList, TVSIL_NORMAL);
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)
	{
		hRoot = m_treeConfig.InsertItem(m_pDataProvider->m_vectProductionLine[n].m_strLineName, 0, 0, NULL, TVI_LAST);
		m_treeConfig.SetItemData(hRoot, m_pDataProvider->m_vectProductionLine[n].m_Id);
		searchConfigFromLine(m_pDataProvider->m_vectProductionLine[n].m_Id);
		for (int i = 0; i < m_vectConfigID.size();i++)
		{
			hFirItem = m_treeConfig.InsertItem(m_vectConfigName[i], 1, 2, hRoot, TVI_LAST);
			m_treeConfig.SetItemData(hFirItem,m_vectConfigID[i]);
		}
	}
}

//根据生产线ID找到旗下的所有配方，并将配方ID和名称对应存入set容器//
void COldConfigGraChiDlg::searchConfigFromLine(int nLineID)
{
	m_vectConfigID.clear();
	m_vectConfigName.clear();
	bool bHaveAdd;                             //是否已添加//
	for (int k = 0; k < m_pDataProvider->m_vectFormula.size();k++)
	{
		if (nLineID==m_pDataProvider->m_vectFormula[k].m_ProductionLineId)         //如果该记录的所属生产线与当前生产线一致//
		{
			bHaveAdd = false;
			for (int m = 0; m < m_vectConfigID.size();m++)                         //判断该记录的配方是否已被添加//
			{
				if (m_pDataProvider->m_vectFormula[k].m_FormulaId==m_vectConfigID[m])       //如果配方ID已经添加过了//
			    {
					bHaveAdd = true;
					break;
			    }
			}
			if (!bHaveAdd)         //如果未添加//
			{
				m_vectConfigID.push_back(m_pDataProvider->m_vectFormula[k].m_FormulaId);
				m_vectConfigName.push_back(m_pDataProvider->m_vectFormula[k].m_strFormulaName);
			}
		}
	}
}

//填写list1的第nItem+1行//
void COldConfigGraChiDlg::WriteToList1(int nLineID, CString strLineName, int nItem)
{
	CString strItem;
	strItem.Format(_T("%d"), nItem + 1);
	m_listAllConfig.InsertItem(nItem, _T(""));
	m_listAllConfig.SetItemText(nItem, 1, strItem);
	m_listAllConfig.SetItemText(nItem, 2, strLineName);
	bool bHaveFindDefault = false;         //该生产线是否设定了默认配方//
	bool bHaveFindCurrent = false;         //该生产线是否正在使用某配方//
	for (int n = 0; n<m_pDataProvider->m_vectFormula.size(); n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_ProductionLineId == nLineID && m_pDataProvider->m_vectFormula[n].m_IsCurrentFormula != 0)
		{
			m_listAllConfig.SetItemText(nItem, 3, m_pDataProvider->m_vectFormula[n].m_strFormulaName);
			bHaveFindCurrent = true;
			break;
		}
	}
	if (!bHaveFindCurrent)                //如果尚未指定使用的配方//
		m_listAllConfig.SetItemText(nItem, 3, _T("未指定"));
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size();n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_ProductionLineId == nLineID && m_pDataProvider->m_vectFormula[n].m_IsDefaultFormula!=0)
		{
			m_listAllConfig.SetItemText(nItem, 4, m_pDataProvider->m_vectFormula[n].m_strFormulaName);
			bHaveFindDefault = true;
			break;
		}
	}
	if (!bHaveFindDefault)            //如果尚未指定默认配方//
		m_listAllConfig.SetItemText(nItem, 4, _T("未指定"));
}

//树形控件选中条目发生改变时//
void COldConfigGraChiDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hCurSelItem = m_treeConfig.GetSelectedItem();
	UINT TempID = m_treeConfig.GetItemData(hCurSelItem);
	HTREEITEM hParentItem = m_treeConfig.GetParentItem(hCurSelItem);
	bool bHaveWriteLineName=false;         //是否已经填写了生产线名称、备注和配方名称//
	m_listConfigPara.DeleteAllItems();     //清空配方参数列表//
	m_vectConfigPara.clear();              //清空容器//
	CString strItem;
	int nTempData;
	if (hParentItem == NULL)                             //如果选中的是生产线节点//
	{
		m_edConfigLine.SetWindowText(_T(""));
		m_edConfigName.SetWindowText(_T(""));
		m_edNote.SetWindowText(_T(""));
		m_strOldName = _T("");
		m_strOldNote = _T("");
	}
	else                        //如果选中的是配方节点//
	{
		for (int n = 0; n < m_pDataProvider->m_vectFormula.size();n++)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId==TempID && !bHaveWriteLineName)  //填写配方名称、使用生产线、备注//
			{
				m_edNote.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strNote);
				m_strOldNote = m_pDataProvider->m_vectFormula[n].m_strNote;
				m_edConfigName.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strFormulaName);
				m_strOldName = m_pDataProvider->m_vectFormula[n].m_strFormulaName;
				m_edConfigLine.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strProductionLineName);
				bHaveWriteLineName = true;
			}
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == TempID)   //将该配方的所有参数写到m_vectConfigPara//
				m_vectConfigPara.push_back(m_pDataProvider->m_vectFormula[n]);
		}
		for (int n = 0; n < m_vectConfigPara.size();n++)                //填写list2//
		{
			m_listConfigPara.InsertItem(n, _T(""));
			strItem.Format(_T("%d"), n + 1);
			m_listConfigPara.SetItemText(n, 1, strItem);
			m_listConfigPara.SetItemText(n, 2, m_vectConfigPara[n].m_strProcessParaName);
			switch (m_pDataProvider->searchModuleParaType(m_vectConfigPara[n].m_ProcessParaId))
			{
			case 0:
				if (m_vectConfigPara[n].m_DefaultValue==0)
					m_listConfigPara.SetItemText(n, 3, _T("0"));
				else
					m_listConfigPara.SetItemText(n, 3, _T("1"));
				break;
			case 1:
			case 2:
			case 3:
				nTempData = m_vectConfigPara[n].m_DefaultValue;
				strItem.Format(_T("%d"), nTempData);
				m_listConfigPara.SetItemText(n, 3, strItem);
				break;
			case 4:
			case 5:
				strItem.Format(_T("%.2f"), m_vectConfigPara[n].m_DefaultValue);
				m_listConfigPara.SetItemText(n, 3, strItem);
				break;
			default:
				break;
			}		
			m_listConfigPara.SetItemText(n, 4, m_vectConfigPara[n].m_strParaValueUnit);
		}
	}
}

//右击list2//
void COldConfigGraChiDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem == -1)
		return;
	CMenu menu, *pSubMenu;
	menu.LoadMenu(IDR_MENU_OLDCONFIG);
	pSubMenu = menu.GetSubMenu(0);
	CPoint point1;          //存储鼠标位置的临时变量//
	GetCursorPos(&point1);  //得到光标处//
	UINT nResult = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_TOPALIGN, point1.x, point1.y, GetParent());//确保右键点击在哪菜单出现在哪，并返回所选的菜单项//
	CString strItem;
	switch (nResult)
	{
	case ID_ALLSEL:
		for (int n = 0; n < m_listConfigPara.GetItemCount();n++)
			m_listConfigPara.SetCheck(n);
		break;
	case ID_KILLSEL:
		for (int n = 0; n < m_listConfigPara.GetItemCount(); n++)
			m_listConfigPara.SetCheck(n,0);
		break;
	case ID_DELETEPARA:
		for (int n = m_vectConfigPara.size() - 1; n >= 0;n--)   //如果参数被选中则从容器和列表内去掉//
		{
			if (m_listConfigPara.GetCheck(n))
			{
				m_vectConfigPara.erase(m_vectConfigPara.begin() + n);
				m_listConfigPara.DeleteItem(n);
				m_bIsDelePara = true;
			}
		}
		if (m_bIsDelePara)
		    for (int n = 0; n < m_listConfigPara.GetItemCount();n++)  //修正列表框内显示的序号//
		    {
			    strItem.Format(_T("%d"), n + 1);
			    m_listConfigPara.SetItemText(n, 1, strItem);
		     }
		break;
	case ID_ADDPARA:              //添加新参数到配方//
		AddParaToConfig();
		break;
	default:
		break;
	}
	*pResult = 0;
}

//树形区选中项即将发生变化//
void COldConfigGraChiDlg::OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	if (m_bHaveEditCreate == true)                                               //如果之前创建了编辑框就销毁掉//
	{
		distroyEdit(&m_listConfigPara, &m_Edit, m_OldItem, m_OldSubItem);                 //销毁单元格编辑框对象//
		m_bHaveEditCreate = false;
	}
	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;

	int nResult;
	if (m_bIsAddPara||m_bIsDelePara||m_bIsNameChange||m_bIsNoteChange||m_bIsParaValueChange)  //如果有未保存的修改则弹出提醒//
	{
		nResult = MessageBox(_T("配方信息已被修改，是否保存？"), _T("提示"), MB_YESNO | MB_ICONEXCLAMATION);
		if (nResult==IDYES)
			saveChange();//保存修改//
		m_bIsNameChange = false;                 //当前编辑区内的配方是否发生了更改//
		m_bIsParaValueChange = false;            //是否修改了配方参数的值//
		m_bIsNoteChange = false;                 //是否修改了配方的备注//
		m_bIsDelePara = false;                   //是否删除了配方参数//
		m_bIsAddPara = false;                    //是否添加了配方参数//
	} 
	*pResult = 0;
}

//保存修改,并将修改标志位都设为否//
void COldConfigGraChiDlg::saveChange()
{
	//判断是否有改动//
	if (!m_bIsAddPara && !m_bIsDelePara && !m_bIsNameChange && !m_bIsNoteChange && !m_bIsParaValueChange)
		return;
	
	//判断新配方名称是否合法//
	CString strNewName;
	m_edConfigName.GetWindowText(strNewName);
	strNewName = strNewName.Trim();
	if (strNewName.IsEmpty())
	{
		AfxMessageBox(_T("保存失败，配方名称不能为空！"));
		return;
	}
	if (m_bIsNameChange)
	{
		for (int q = 0; q < m_pDataProvider->m_vectFormula.size();q++)
		{
			if (m_pDataProvider->m_vectFormula[q].m_strFormulaName == strNewName)
			{
				AfxMessageBox(_T("保存失败！该名称已被使用。"));
				return;
			}
		}
	}

	//判断配方是否为空//
	if (m_vectConfigPara.empty())
	{
		AfxMessageBox(_T("保存失败，请先为配方添加参数！"));
		return;
	}

	//判断配方参数是否都具有设定值//
	for (int n = 0; n < m_vectConfigPara.size();n++)
	{
		if (m_vectConfigPara[n].m_DefaultValue==-3001)
		{
			AfxMessageBox(_T("保存失败，请为每个配方参数添加预设值！"));
			return;
		}
	}

	bool bIsBeDele;     //该参数是否被删除了//
	if (m_bIsDelePara)  //如果有配方参数被删除了//
	{
		for (int n = m_pDataProvider->m_vectFormula.size() - 1; n >= 0;n--)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem()))
			{
				bIsBeDele = true;
				for (int i = 0; i < m_vectConfigPara.size(); i++)   //查询该参数是否被删除了//
					if (m_pDataProvider->m_vectFormula[n].m_Id == m_vectConfigPara[i].m_Id)
					{
						bIsBeDele = false;
						break;
					}
				if (bIsBeDele)           //如果该参数已被删除，则从数据源容器和数据库删除//
				{
					m_pDataProvider->DeleteDbTableItem(CDataProvider::tbFormula, m_pDataProvider->m_vectFormula[n].m_Id);
					m_pDataProvider->m_vectFormula.erase(m_pDataProvider->m_vectFormula.begin() + n);
				}
			}
		}
		
	}

	if (m_bIsAddPara)   //如果新增了参数//
	{
		for (int n = 0; n < m_vectConfigPara.size();n++)
			if (m_vectConfigPara[n].m_Id==0)           //如果该参数不在配方里，则添加//
				m_pDataProvider->AddFormulaToDatabase(m_vectConfigPara[n]);
		
	}

	if (m_bIsParaValueChange)    //如果有参数的值被修改//
	{
		for (int n = 0; n < m_vectConfigPara.size();n++)
		{
			for (int i = 0; i < m_pDataProvider->m_vectFormula.size();i++)
			{
				if (m_pDataProvider->m_vectFormula[i].m_FormulaId == m_vectConfigPara[n].m_FormulaId && m_vectConfigPara[n].m_ProcessParaId == m_pDataProvider->m_vectFormula[i].m_ProcessParaId)
				{
					m_pDataProvider->m_vectFormula[i].m_DefaultValue = m_vectConfigPara[n].m_DefaultValue;
					m_pDataProvider->UpdateTableItem(CDataProvider::tbFormula, m_pDataProvider->m_vectFormula[i].m_Id);
					break;
				}
			}
		}
		
	}

	if (m_vectConfigPara[0].m_IsCurrentFormula != 0 && (m_bIsDelePara || m_bIsAddPara || m_bIsParaValueChange))  //如果是当前使用的配方，并且它的设定值或参数数量有改动//
	{
		m_pDataProvider->SetCurConfig(m_vectConfigPara[0].m_ProductionLineId, m_vectConfigPara[0].m_FormulaId);
		m_bIsCurConfigChange = true;
	}
	m_bIsDelePara = false;                   //是否删除了配方参数置0//
	m_bIsParaValueChange = false;            //是否修改了配方参数的值置0//
	m_bIsAddPara = false;                          //是否添加了配方参数，置0//


	if (m_bIsNameChange)        //如果配方名称被修改//
	{
		for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem()))
			{
				m_pDataProvider->m_vectFormula[n].m_strFormulaName = strNewName;
				m_pDataProvider->UpdateTableItem(CDataProvider::tbFormula, m_pDataProvider->m_vectFormula[n].m_Id);
			}
		}
		m_treeConfig.SetItemText(m_treeConfig.GetSelectedItem(), strNewName);  //修改树形区//
		m_strOldName = strNewName;             
		m_bIsNameChange = false;                 //配方名称是否被修改，置0//
	}

	CString strNewNote;
	if (m_bIsNoteChange)        //如果配方的备注被修改//
	{
		m_edNote.GetWindowText(strNewNote);         //获取新的备注名称//
		strNewNote = strNewNote.Trim();
		for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem()))
			{
				m_pDataProvider->m_vectFormula[n].m_strNote = strNewNote;
				m_pDataProvider->UpdateTableItem(CDataProvider::tbFormula, m_pDataProvider->m_vectFormula[n].m_Id);
			}
		}
		m_strOldNote = strNewNote;
		m_bIsNoteChange = false;                     //是否修改了配方的备注，置0//
	}
	AfxMessageBox(_T("保存成功！"));
}

//添加参数到配方//
void COldConfigGraChiDlg::AddParaToConfig()
{
	HTREEITEM hCurSelItem = m_treeConfig.GetSelectedItem();   //获得当前树形区的选中节点//
	if (hCurSelItem==NULL)
		return;
	UINT TempID = m_treeConfig.GetItemData(hCurSelItem);
	HTREEITEM hParentItem = m_treeConfig.GetParentItem(hCurSelItem);
	if (hParentItem == NULL)             //如果树形区选中的是生产线节点，则不作处理//
		return;
	m_AddConfigParaPopDlg.m_vectNewPara.clear();              //清空弹出界面的容器//
		
	UINT nLineID = m_treeConfig.GetItemData(hParentItem);     //选中配方所属生产线的ID//

	//将该生产线下可进入配方但未进入配方的参数传给弹出界面//
	bool bIsInConfig = false;
	for (int n = 0; n < m_pDataProvider->m_vectProModulePara.size(); n++)
	{
		if (m_pDataProvider->m_vectProModulePara[n].m_ProductionLineId == nLineID && m_pDataProvider->m_vectProModulePara[n].m_IsConfig)
		{
			bIsInConfig = false;
			for (int i = 0; i < m_vectConfigPara.size(); i++)
			{
				if (m_vectConfigPara[i].m_ProcessParaId == m_pDataProvider->m_vectProModulePara[n].m_Id)
				{
					bIsInConfig = true;
					break;
				}
			}
			if (!bIsInConfig)
			{
				m_AddConfigParaPopDlg.m_vectNewPara.push_back(m_pDataProvider->m_vectProModulePara[n]);
			}
		}
	}
	if (m_AddConfigParaPopDlg.m_vectNewPara.empty())
	{
		AfxMessageBox(_T("没有可供添加的参数！"));
		return;
	}
	m_AddConfigParaPopDlg.DoModal();   //弹出添加参数的界面//

	if (m_pDataProvider->m_vectTempProModulePara.empty())   //如果没有新添加参数则退出函数，如果添加了则将标志位置1//
		return;
	else
		m_bIsAddPara = true;
	
	CFormulaClass TempConfigPara;
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)    //将当前配方的基本信息赋值给新增的参数//
	{
		if (m_pDataProvider->m_vectFormula[n].m_FormulaId==TempID)
		{
			TempConfigPara.m_IsCurrentFormula = m_pDataProvider->m_vectFormula[n].m_IsCurrentFormula;
			TempConfigPara.m_IsDefaultFormula = m_pDataProvider->m_vectFormula[n].m_IsDefaultFormula;
			TempConfigPara.m_strFormulaName = m_pDataProvider->m_vectFormula[n].m_strFormulaName;
			TempConfigPara.m_strNote = m_pDataProvider->m_vectFormula[n].m_strNote;
			TempConfigPara.m_ProductionLineId = m_pDataProvider->m_vectFormula[n].m_ProductionLineId;
			TempConfigPara.m_strProductionLineName = m_pDataProvider->m_vectFormula[n].m_strProductionLineName;
			TempConfigPara.m_DefaultValue = -3001;
			TempConfigPara.m_FormulaId = TempID;
			TempConfigPara.m_Id = 0;
			break;
		}
	}

	//如果用户在弹出界面添加了参数，则将这些参数传入m_vectConfigPara,并显示//
	CString strItem1;
	int nItem;
	for (int m = 0; m < m_pDataProvider->m_vectTempProModulePara.size();m++)
	{
		//TempConfigPara.m_DefaultValue = m_pDataProvider->m_vectTempProModulePara[m].m_fSetValue;

		TempConfigPara.m_ProcessParaId = m_pDataProvider->m_vectTempProModulePara[m].m_Id;		
		TempConfigPara.m_strParaValueUnit = m_pDataProvider->m_vectTempProModulePara[m].m_strUnit;
		TempConfigPara.m_strProcessParaName = m_pDataProvider->m_vectTempProModulePara[m].m_strParaName;
		m_vectConfigPara.push_back(TempConfigPara);
		nItem = m_listConfigPara.GetItemCount();
		m_listConfigPara.InsertItem(nItem, _T(""));
		strItem1.Format(_T("%d"), nItem + 1);
		m_listConfigPara.SetItemText(nItem, 1, strItem1);
		m_listConfigPara.SetItemText(nItem, 2, TempConfigPara.m_strProcessParaName);
		m_listConfigPara.SetItemText(nItem, 3, _T(""));
		m_listConfigPara.SetItemText(nItem, 4, TempConfigPara.m_strParaValueUnit);
	}
	m_pDataProvider->m_vectTempProModulePara.clear();         //清空数据源内的临时容器//
}


//添加参数按钮//
void COldConfigGraChiDlg::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	AddParaToConfig();
}

//删除配方按钮//
void COldConfigGraChiDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	
	UINT TempID;
	if (m_strOldName!=_T(""))
	{
		TempID = m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem());
		for (int n = 0; n < m_pDataProvider->m_vectFormula.size();n++)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == TempID && m_pDataProvider->m_vectFormula[n].m_IsCurrentFormula)
			{
				AfxMessageBox(_T("该配方正在使用中，无法删除，请先停用配方！"));
				return;
			}
		}
		int nResult = MessageBox(_T("该操作无法恢复，是否继续？"), _T("警告"), MB_YESNO | MB_ICONEXCLAMATION);
		if (nResult==IDYES)
		{
			m_pDataProvider->DeleteFormula(TempID);         //从数据源容器和数据库删除该配方//
			ReInitDlg();//重新绘制整个窗口，清空标志位//		
		}
	}
}


void COldConfigGraChiDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	m_listConfigPara.SetFocus();
}

//单击列表框2//
void COldConfigGraChiDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pEditCtrl->iItem == -1 || pEditCtrl->iSubItem != 3)                                                //点击到非工作区//
	{
		if (m_bHaveEditCreate == true)                                            //如果之前创建了编辑框就销毁掉//
		{
			distroyEdit(&m_listConfigPara, &m_Edit, m_OldItem, m_OldSubItem);              //销毁单元格编辑框对象//
			m_bHaveEditCreate = false;
		}
	}
	else
	{
		if (m_bHaveEditCreate == true)
		{
			if (!(m_OldItem == pEditCtrl->iItem && m_OldSubItem == pEditCtrl->iSubItem))    //如果点中的单元格不是之前创建好的//
			{
				distroyEdit(&m_listConfigPara, &m_Edit, m_OldItem, m_OldSubItem);
				m_bHaveEditCreate = false;
				createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);      //创建编辑框//
			}
			else                                                                       //点中的单元格是之前创建好的//
			{
				m_Edit.SetFocus();                                                     //设置为焦点//
			}
		}
		else
		{
			m_OldItem = pEditCtrl->iItem;                                                 //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
			m_OldSubItem = pEditCtrl->iSubItem;                                           //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
			createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);         //创建编辑框//
		}
	}

	*pResult = 0;
}


//创建编辑框//
void COldConfigGraChiDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)
//pEditCtrl为列表对象指针，createdit为编辑框指针对象//
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准//
{
	Item = pEditCtrl->iItem;                     //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
	SubItem = pEditCtrl->iSubItem;               //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_CENTER | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_CREATE1);  //创建编辑框对象,IDC_EDIT_CREATE为控件ID号//
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);                   //设置字体//
	createdit->SetParent(&m_listConfigPara);                      //将listcontrol设置为父窗口//
	CRect  EditRect;
	m_listConfigPara.GetSubItemRect(m_OldItem, 3, LVIR_LABEL, EditRect);      //获取单元格的空间位置信息//
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_listConfigPara.GetColumnWidth(3) - 1, EditRect.bottom - 1);//+1和-1让编辑框不至于挡住列表框中的网格线//
	CString strTemp;
	strTemp = m_listConfigPara.GetItemText(m_OldItem, 3);
	createdit->SetWindowText(strTemp);
	createdit->MoveWindow(&EditRect);                                      //将编辑框位置放在相应单元格上//
	createdit->ShowWindow(SW_SHOW);                                        //显示编辑框在单元格上面//
	createdit->SetFocus();                                                 //设置为焦点//
	createdit->SetSel(-1);                                                 //设置光标在文本框文字的最后//
}

//销毁编辑框//
void COldConfigGraChiDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString strEditData;
	float tempfloat;
	int tempValue;
	distroyedit->GetWindowText(strEditData);
	strEditData = strEditData.Trim();
	int nDataType = m_pDataProvider->searchModuleParaType(m_vectConfigPara[Item].m_ProcessParaId);
	if (strEditData.IsEmpty())             //如果输入为空，则销毁编辑框//
	{
		distroyedit->DestroyWindow();
		if (m_vectConfigPara[Item].m_DefaultValue!=-3001)
		{
			m_vectConfigPara[Item].m_DefaultValue = -3001;
			m_bIsParaValueChange = true;
		}
		list->SetItemText(Item, 3, _T(""));
		return;
	}
	if ( nDataType== 0 && (strEditData != _T("0") && strEditData != _T("1")))
	{
		AfxMessageBox(_T("非法操作，该参数值只能为“0”或“1”！"));
		distroyedit->DestroyWindow();                          //销毁对象//
		return;
	}

	if (strEditData == strEditData.SpanIncluding(_T("0123456789.")))
	{
		switch (nDataType)
		{
		case 0:
			if (strEditData == _T("0"))
			{
				list->SetItemText(Item, 3, _T("0"));                 //将修改写入列表框//
				if (m_vectConfigPara[Item].m_DefaultValue != 0)
				{
					m_vectConfigPara[Item].m_DefaultValue = 0;
					m_bIsParaValueChange = true;
				}
			}
			else
			{
				list->SetItemText(Item, 3, _T("1"));                 //将修改写入列表框//
				if (m_vectConfigPara[Item].m_DefaultValue != 1)
				{
					m_vectConfigPara[Item].m_DefaultValue = 1;
					m_bIsParaValueChange = true;
				}
			}
			break;
		case 1:
		case 2:
		case 3:
			tempValue = (float)_wtof(strEditData);
			strEditData.Format(_T("%d"), tempValue);
			list->SetItemText(Item, 3, strEditData);
			if (tempValue != ((int)m_vectConfigPara[Item].m_DefaultValue))
			{
				m_vectConfigPara[Item].m_DefaultValue = tempValue;
				m_bIsParaValueChange = true;
			}
			break;
		case 4:
		case 5:
			tempfloat = (float)_wtof(strEditData);
			strEditData.Format(_T("%.2f"), tempfloat);
			list->SetItemText(Item, 3, strEditData);
			if (tempfloat != m_vectConfigPara[Item].m_DefaultValue)  //判断值是否发生了变化//
			{
				m_vectConfigPara[Item].m_DefaultValue = tempfloat;    //将修改后的值存入容器//
				m_bIsParaValueChange = true;
			}
			break;
		default:
			break;
		}	
	}
	else
		AfxMessageBox(_T("非法操作，请输入数字！"));
	distroyedit->DestroyWindow();                          //销毁对象//
}

//编辑框失去焦点//
void COldConfigGraChiDlg::OnEnKillfocusEditCreate1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bHaveEditCreate == true)                                               //如果之前创建了编辑框就销毁掉//
	{
		distroyEdit(&m_listConfigPara, &m_Edit, m_OldItem, m_OldSubItem);                 //销毁单元格编辑框对象//
		m_bHaveEditCreate = false;
	}
}


void COldConfigGraChiDlg::ReInitDlg()   //重新初始化窗口//
{
	//重新绘制整个窗口，清空标志位//
	//修改标志位置0//
	m_bIsNameChange = false;                 //当前编辑区内的配方是否发生了更改//
	m_bIsParaValueChange = false;            //是否修改了配方参数的值//
	m_bIsNoteChange = false;                 //是否修改了配方的备注//
	m_bIsDelePara = false;                   //是否删除了配方参数//
	m_bIsAddPara = false;                    //是否添加了配方参数//
	m_strOldName = _T("");
	m_strOldNote = _T("");
	m_edConfigLine.SetWindowText(_T(""));
	m_edNote.SetWindowText(_T(""));
	m_edConfigName.SetWindowText(_T(""));
	m_vectConfigPara.clear();
	m_listConfigPara.DeleteAllItems();

	InitTreeContrl();                               //重新填写树形区域//
	m_listAllConfig.DeleteAllItems();
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)          //重新填写列表框1//
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);
}

//保存修改按钮//
void COldConfigGraChiDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;
	//保存//
	saveChange();
}

//名称编辑框失去焦点//
void COldConfigGraChiDlg::OnEnKillfocusEdit4()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strCurName;
	m_edConfigName.GetWindowText(strCurName);
	strCurName = strCurName.Trim();
	if (strCurName.IsEmpty()&&!m_strOldName.IsEmpty())
	{
		AfxMessageBox(_T("警告！配方名称不能为空！"));
	}
}

//撤销更改按钮//
void COldConfigGraChiDlg::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;

	//判断是否有改动//
	if (!m_bIsAddPara && !m_bIsDelePara && !m_bIsNameChange && !m_bIsNoteChange && !m_bIsParaValueChange)
		return;

	//重新填写编辑区//
	m_listConfigPara.DeleteAllItems();
	m_vectConfigPara.clear();
	HTREEITEM hCurSelItem = m_treeConfig.GetSelectedItem();
	UINT TempID = m_treeConfig.GetItemData(hCurSelItem);     //得到当前选中配方的ID//
	bool bHaveWriteLineName = false;         //是否已经填写了生产线名称、备注和配方名称//
	CString strItem;
	if (m_treeConfig.GetParentItem(hCurSelItem)==NULL)                             //如果选中的是生产线节点//
	{
		m_edConfigLine.SetWindowText(_T(""));
		m_edConfigName.SetWindowText(_T(""));
		m_edNote.SetWindowText(_T(""));
		m_strOldName = _T("");
		m_strOldNote = _T("");
	}
	else                        //如果选中的是配方节点//
	{
		for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
		{
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == TempID && !bHaveWriteLineName)  //填写配方名称、使用生产线、备注//
			{
				m_edNote.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strNote);
				m_strOldNote = m_pDataProvider->m_vectFormula[n].m_strNote;
				m_edConfigName.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strFormulaName);
				m_strOldName = m_pDataProvider->m_vectFormula[n].m_strFormulaName;
				m_edConfigLine.SetWindowText(m_pDataProvider->m_vectFormula[n].m_strProductionLineName);
				bHaveWriteLineName = true;
			}
			if (m_pDataProvider->m_vectFormula[n].m_FormulaId == TempID)   //将该配方的所有参数写到m_vectConfigPara//
				m_vectConfigPara.push_back(m_pDataProvider->m_vectFormula[n]);
		}
		for (int n = 0; n < m_vectConfigPara.size(); n++)                //填写list2//
		{
			m_listConfigPara.InsertItem(n, _T(""));
			strItem.Format(_T("%d"), n + 1);
			m_listConfigPara.SetItemText(n, 1, strItem);
			m_listConfigPara.SetItemText(n, 2, m_vectConfigPara[n].m_strProcessParaName);
			switch (m_pDataProvider->searchModuleParaType(m_vectConfigPara[n].m_ProcessParaId))
			{
			case 0:
				if (m_vectConfigPara[n].m_DefaultValue==0)
					m_listConfigPara.SetItemText(n, 3, _T("0"));
				else
					m_listConfigPara.SetItemText(n, 3, _T("1"));
				break;
			case 1:
			case 2:
			case 3:
				strItem.Format(_T("%d"), ((int)m_vectConfigPara[n].m_DefaultValue));
				m_listConfigPara.SetItemText(n, 3, strItem);
				break;
			case 4:
			case 5:
				strItem.Format(_T("%.2f"), m_vectConfigPara[n].m_DefaultValue);
				m_listConfigPara.SetItemText(n, 3, strItem);
				break;
			default:
				break;
			}		
			m_listConfigPara.SetItemText(n, 4, m_vectConfigPara[n].m_strParaValueUnit);
		}
	}
	m_bIsNameChange = false;                 //当前编辑区内的配方是否发生了更改//
	m_bIsParaValueChange = false;            //是否修改了配方参数的值//
	m_bIsNoteChange = false;                 //是否修改了配方的备注//
	m_bIsDelePara = false;                   //是否删除了配方参数//
	m_bIsAddPara = false;                    //是否添加了配方参数//
}

//使用配方按钮//
void COldConfigGraChiDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;

	if (m_bIsAddPara || m_bIsDelePara || m_bIsNameChange || m_bIsNoteChange || m_bIsParaValueChange) //如果配方被修改了尚未保存，则禁止设置//
	{
		AfxMessageBox(_T("请先保存修改！"));
		return;
	}
	int nIDofCurConfig;   //当前树形区选中配方的ID//
	int nIDofLine;        //当前选中配方所属生产线的ID//
	if (m_treeConfig.GetSelectedItem() == NULL)
		return;
	nIDofCurConfig = m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem());
	if (m_treeConfig.GetParentItem(m_treeConfig.GetSelectedItem()) == NULL)  //如果选中的是生产线节点，则不做处理//
		return;
	if (!IsPlcConnected(nIDofCurConfig))
	{
		AfxMessageBox(_T("检测到与该配方相关的设备未连接，无法使用配方！请连接后重试。"));
		return;
	}
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size();n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_FormulaId==nIDofCurConfig)
		{
			nIDofLine = m_pDataProvider->m_vectFormula[n].m_ProductionLineId;
			break;
		}
	}
	m_pDataProvider->SetCurConfig(nIDofLine, nIDofCurConfig);
	m_listAllConfig.DeleteAllItems();    //重新填写列表框1//
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)          
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);
	m_bIsCurConfigChange = true;  //正在使用的配方发生了变化//
	AfxMessageBox(_T("使用配方成功！"));
}

//设为默认配方按钮//
void COldConfigGraChiDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;

	if (m_bIsAddPara || m_bIsDelePara || m_bIsNameChange || m_bIsNoteChange || m_bIsParaValueChange) //如果配方被修改了尚未保存，则禁止设置//
	{
		AfxMessageBox(_T("请先保存修改！"));
		return;
	}
	int nIDofCurConfig;   //当前树形区选中配方的ID//
	int nIDofLine;        //当前选中配方所属生产线的ID//
	if (m_treeConfig.GetSelectedItem() == NULL)
		return;
	nIDofCurConfig = m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem());
	if (m_treeConfig.GetParentItem(m_treeConfig.GetSelectedItem()) == NULL)  //如果选中的是生产线节点，则不做处理//
		return;
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_FormulaId == nIDofCurConfig)
		{
			nIDofLine = m_pDataProvider->m_vectFormula[n].m_ProductionLineId;
			break;
		}
	}
	m_pDataProvider->SetDefaultConfig(nIDofLine, nIDofCurConfig);
	m_listAllConfig.DeleteAllItems();    //重新填写列表框1//
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);
	AfxMessageBox(_T("设置默认配方成功！"));
}


void COldConfigGraChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect rectList1;
			m_listAllConfig.GetWindowRect(&rectList1);
			m_listAllConfig.SetColumnWidth(1, rectList1.Width() / 11.0);
			m_listAllConfig.SetColumnWidth(2, rectList1.Width() / 11.0 * 3.95);
			m_listAllConfig.SetColumnWidth(3, rectList1.Width() / 11.0 * 3.0);
			m_listAllConfig.SetColumnWidth(4, rectList1.Width() / 11.0 * 3.0);
			CRect rectList2;
			m_listConfigPara.GetWindowRect(&rectList2);
			m_listConfigPara.SetColumnWidth(0, rectList2.Width() / 24.0);
			m_listConfigPara.SetColumnWidth(1, rectList2.Width() / 24.0*2.7);
			m_listConfigPara.SetColumnWidth(2, rectList2.Width() / 24.0 * 10.65);
			m_listConfigPara.SetColumnWidth(3, rectList2.Width() / 24.0 * 4.8);
			m_listConfigPara.SetColumnWidth(4, rectList2.Width() / 24.0 * 4.8);
			Invalidate();
		}
}


void COldConfigGraChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
{
	vectOldSize.clear();
	CRect Rect;
	int woc;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件//    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //取得ID//  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		vectOldSize.push_back(Rect);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void COldConfigGraChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
{
	CRect Rect;
	int woc;
	CPoint  BRPoint, TLPoint;
	double fsp[2];                                    //缩放比例//
	POINT recNewSize;                                 //获取现在对话框的大小//  
	CRect TempRect;
	GetClientRect(&TempRect);                         //取客户区大小//    
	recNewSize.x = TempRect.right - TempRect.left;
	recNewSize.y = TempRect.bottom - TempRect.top;
	fsp[0] = (double)recNewSize.x / OldSizeOfClient.x;
	fsp[1] = (double)recNewSize.y / OldSizeOfClient.y;

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件//    
	for (int n = 0; n < vectOldSize.size(); n++)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //取得ID//  
		TLPoint.x = double(vectOldSize[n].left*fsp[0]);
		TLPoint.y = double(vectOldSize[n].top*fsp[1]);
		BRPoint.x = double(vectOldSize[n].right *fsp[0]);
		BRPoint.y = double(vectOldSize[n].bottom*fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

bool COldConfigGraChiDlg::IsPlcConnected(int nFormulaId)    //判断所有配方参数所属的PLC是否连接//
{
	for (int m = 0; m < m_pDataProvider->m_vectFormula.size();m++)
		if (nFormulaId == m_pDataProvider->m_vectFormula[m].m_FormulaId)
			for (int i = 0; i < m_pDataProvider->m_vectProModulePara.size(); i++)
				if (m_pDataProvider->m_vectFormula[m].m_ProcessParaId == m_pDataProvider->m_vectProModulePara[i].m_Id)
				{
					if (!IsPlcConnectedByPlcId(m_pDataProvider->m_vectProModulePara[i].m_PlcId))
						return false;
					break;
				}
	return true;
}

bool COldConfigGraChiDlg::IsPlcConnectedByPlcId(int nPlcId)    //判断某台PLC是否连接//
{
	for (int n = 0; n < m_pDataProvider->m_vectPlc.size(); n++)
		if (m_pDataProvider->m_vectPlc[n].m_Id == nPlcId)
			if (m_pDataProvider->m_vectPlc[n].m_bIsConnected)
				return true;
			else
				return false;
	return false;
}

void COldConfigGraChiDlg::OnBnClickedButton8()  //取消默认按钮响应函数//
{
	// TODO:  在此添加控件通知处理程序代码
	//判断配方名称和备注是否发生变化//
	CString strCurName, strCurNote;
	m_edConfigName.GetWindowText(strCurName);
	m_edNote.GetWindowText(strCurNote);
	strCurName = strCurName.Trim();
	strCurNote = strCurNote.Trim();
	if (strCurName != m_strOldName && m_strOldName != _T(""))
		m_bIsNameChange = true;
	else
		m_bIsNameChange = false;
	if (strCurNote != m_strOldNote && m_strOldName != _T(""))
		m_bIsNoteChange = true;
	else
		m_bIsNoteChange = false;

	if (m_bIsAddPara || m_bIsDelePara || m_bIsNameChange || m_bIsNoteChange || m_bIsParaValueChange) //如果配方被修改了尚未保存，则禁止设置//
	{
		AfxMessageBox(_T("请先保存修改！"));
		return;
	}
	int nIDofCurConfig;   //当前树形区选中配方的ID//
	int nIDofLine;        //当前选中配方所属生产线的ID//
	if (m_treeConfig.GetSelectedItem() == NULL)
		return;
	nIDofCurConfig = m_treeConfig.GetItemData(m_treeConfig.GetSelectedItem());
	if (m_treeConfig.GetParentItem(m_treeConfig.GetSelectedItem()) == NULL)  //如果选中的是生产线节点，则不做处理//
		return;
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_FormulaId == nIDofCurConfig)
		{
			if (m_pDataProvider->m_vectFormula[n].m_IsDefaultFormula == 0)//如果当前选中配方并非默认配方//
				return;
			nIDofLine = m_pDataProvider->m_vectFormula[n].m_ProductionLineId;
			break;
		}
	}
	m_pDataProvider->SetDefaultConfig(nIDofLine, 0);
	m_listAllConfig.DeleteAllItems();    //重新填写列表框1//
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);
	AfxMessageBox(_T("取消默认配方成功！"));
}


void COldConfigGraChiDlg::reInitList1()//重新填写列表框1//
{
	m_listAllConfig.DeleteAllItems();
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)          //重新填写列表框1//
		WriteToList1(m_pDataProvider->m_vectProductionLine[n].m_Id, m_pDataProvider->m_vectProductionLine[n].m_strLineName, n);
}