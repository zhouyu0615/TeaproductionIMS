// EditDeviceParaTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditDeviceParaTabDlg.h"
#include "afxdialogex.h"


// CEditDeviceParaTabDlg 对话框

IMPLEMENT_DYNAMIC(CEditDeviceParaTabDlg, CDialog)

CEditDeviceParaTabDlg::CEditDeviceParaTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDeviceParaTabDlg::IDD, pParent)
{

}

CEditDeviceParaTabDlg::~CEditDeviceParaTabDlg()
{
}

void CEditDeviceParaTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LI1_EDITDEVICEPARA_TABDLG, m_list1);
	DDX_Control(pDX, IDC_ED1_EDITDEVICEPARA_TABDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED2_EDITDEVICEPARA_TABDLG, m_ControlAddrEdit);
	DDX_Control(pDX, IDC_ED3_EDITDEVICEPARA_TABDLG, m_StateAddrEdit);
	DDX_Control(pDX, IDC_ED4_EDITDEVICEPARA_TABDLG, m_DescriptionEdit);
	DDX_Control(pDX, IDC_CO1_EDITDEVICEPARA_TABDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO2_EDITDEVICEPARA_TABDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO3_EDITDEVICEPARA_TABDLG, m_DeviceComoboBox);
	DDX_Control(pDX, IDC_CO4_EDITDEVICEPARA_TABDLG, m_PlcComboBox);
}


BEGIN_MESSAGE_MAP(CEditDeviceParaTabDlg, CDialog)
	ON_BN_CLICKED(IDC_BT1_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnBnClickedAddItem)
	ON_BN_CLICKED(IDC_BT2_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnBnClickedClearEdit)
	ON_BN_CLICKED(IDC_BT3_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnBnClickedClearAll)
	ON_CBN_SELCHANGE(IDC_CO1_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_CO2_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnCbnSelchangeModule)
	ON_NOTIFY(NM_RCLICK, IDC_LI1_EDITDEVICEPARA_TABDLG, &CEditDeviceParaTabDlg::OnNMRClickLi1EditdeviceparaTabdlg)
END_MESSAGE_MAP()


// CEditDeviceParaTabDlg 消息处理程序

//添加//
void CEditDeviceParaTabDlg::OnBnClickedAddItem()
{
	CDevicePara tempDevicePara;
	m_LineComboBox.GetWindowText(tempDevicePara.m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(tempDevicePara.m_strProcessModuleName);
	m_DeviceComoboBox.GetWindowText(tempDevicePara.m_strDeviceName);

	m_PlcComboBox.GetWindowText(tempDevicePara.m_strPlcName);
	
	m_NameEdit.GetWindowText(tempDevicePara.m_strParaName);
	m_ControlAddrEdit.GetWindowText(tempDevicePara.m_strControlAddrIndex);
	m_StateAddrEdit.GetWindowText(tempDevicePara.m_strStateAddrIndex);
	m_DescriptionEdit.GetWindowText(tempDevicePara.m_strDescription);

	if (m_ParaCheckUtil.DeviceParaCheck(tempDevicePara) == 0) //参数名的检查通过
	{
		m_pDataProvider->AddDeviceParaToDatabase(tempDevicePara);
		AddItemToList(tempDevicePara);

		int i = m_list1.GetItemCount() - 1;
		int index = m_pDataProvider->m_vectDevicePara.size() - 1;
		m_IndexMap[i] = index;
	}
		
}

//清空编辑框//
void CEditDeviceParaTabDlg::OnBnClickedClearEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	m_NameEdit.SetWindowText(_T(""));
	m_ControlAddrEdit.SetWindowText(_T(""));
	m_StateAddrEdit.SetWindowText(_T(""));
	m_DescriptionEdit.SetWindowText(_T(""));
}

//清空所有//
void CEditDeviceParaTabDlg::OnBnClickedClearAll()
{
	int nResult = MessageBox(_T("该操作将删除所有的设备参数，是否继续当前操作？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
	if (nResult == IDYES)
	{
		m_pDataProvider->m_vectDevicePara.clear();
		m_pDataProvider->DeleteDbTable(CDataProvider::tbDevicePara);
		OnBnClickedClearEdit();
		

		MyOnPaint();
	}

}


BOOL CEditDeviceParaTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MyOnPaint();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


int CEditDeviceParaTabDlg::MyOnPaint()
{
	//ListOnPaint(); //绘制列表框//

	LineComboxPaint();
	CString LineName,ModuleName;
	//m_LineComboBox.GetLBText(0,LineName);
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);

	//m_ModuleComboBox.GetLBText(0,ModuleName);
	m_ModuleComboBox.GetWindowText(ModuleName);
	DeviceComboxPaint(LineName,ModuleName);

	ListOnPaint(LineName, ModuleName);


	PlcComboxPaint();
	
	return 0;
}

int CEditDeviceParaTabDlg::LineComboxPaint()
{
	m_LineComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		m_LineComboBox.SetCurSel(0);
	}
	
	return 0;

}
int CEditDeviceParaTabDlg::ModuleComboxPaint(CString LineName)
{
	m_ModuleComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size();i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName==LineName)
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
			m_ModuleComboBox.SetCurSel(0);
		}
	}
	
	return 0;
}

int CEditDeviceParaTabDlg::DeviceComboxPaint(CString LineName, CString ModuleName)
{
	m_DeviceComoboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectDevice.size();i++)
	{
		if ((m_pDataProvider->m_vectDevice[i].m_strProductionLineName==LineName)
			&&(m_pDataProvider->m_vectDevice[i].m_strProcessModuleName==ModuleName))
		{
			m_DeviceComoboBox.AddString(m_pDataProvider->m_vectDevice[i].m_strDeviceName);
			m_DeviceComoboBox.SetCurSel(0);
		}
	}

	return 0;
}

int CEditDeviceParaTabDlg::PlcComboxPaint()
{
	m_PlcComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size();i++)
	{
		m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		m_PlcComboBox.SetCurSel(0);
	}
	
	return 0;
}

int CEditDeviceParaTabDlg::ListOnPaint(const CString &ProLineName, const CString &ProModuleName)
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
	CHeaderCtrl *pmyHeaderCtrl = m_list1.GetHeaderCtrl();
	int nCount = pmyHeaderCtrl->GetItemCount();
	for (int i = nCount - 1; i >= 0; i--)
		m_list1.DeleteColumn(i);

	m_list1.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_list1.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 17, -1);
	m_list1.InsertColumn(2, _T("所属生产线"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(3, _T("所属模块"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(4, _T("所属设备"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(5, _T("所属PLC"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(6, _T("参数名称"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(7, _T("控制信号地址"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(8, _T("运行信号地址"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(9, _T("备注"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	
	//填写表单内容//
	int length = m_pDataProvider->m_vectDevicePara.size();
	int index = 0; //表单索引//

	m_IndexMap.clear();
	for (size_t i = 0; i < length; i++)
	{
		if (ProLineName == m_pDataProvider->m_vectDevicePara[i].m_strProductionLineName
			&&ProModuleName == m_pDataProvider->m_vectDevicePara[i].m_strProcessModuleName)
		{
			litem.iItem = index;			
			m_list1.InsertItem(&litem);
			SetListItemText(index, m_pDataProvider->m_vectDevicePara[i]);

			m_IndexMap[index] = i;
			index++;
		}

	}
	return 0;
}


void CEditDeviceParaTabDlg::UpdateItemInList(int Index, CDevicePara &devicePara)
{
    
	SetListItemText(Index, devicePara);

	m_list1.EnsureVisible(Index, TRUE);

}


void CEditDeviceParaTabDlg::DeleteItemInList(int Index)
{
	m_list1.DeleteItem(Index);

	int length = m_list1.GetItemCount();
	//更新INDEX之后的编号//
	for (int i = Index; i <length ;i++)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_list1.SetItemText(i, 1, str);
	}
}


void CEditDeviceParaTabDlg::AddItemToList(CDevicePara &devicePara)
{

	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");

	int i =m_list1.GetItemCount();
	litem.iItem = i;
	m_list1.InsertItem(&litem);

	SetListItemText(i, devicePara);

	m_list1.EnsureVisible(i, TRUE);
	
}


void CEditDeviceParaTabDlg::SetListItemText(int Index, CDevicePara &devicePara)
{

	CString str;
	str.Format(_T("%d"), Index + 1);
	m_list1.SetItemText(Index, 1, str);
	m_list1.SetItemText(Index, 2, devicePara.m_strProductionLineName);
	m_list1.SetItemText(Index, 3, devicePara.m_strProcessModuleName);
	m_list1.SetItemText(Index, 4, devicePara.m_strDeviceName);
	m_list1.SetItemText(Index, 5, devicePara.m_strPlcName);
	m_list1.SetItemText(Index, 6, devicePara.m_strParaName);
	m_list1.SetItemText(Index, 7, devicePara.m_strControlAddrIndex);
	m_list1.SetItemText(Index, 8, devicePara.m_strStateAddrIndex);
	m_list1.SetItemText(Index, 9, devicePara.m_strDescription);	
}



void CEditDeviceParaTabDlg::OnCbnSelchangeLine()
{
	// TODO:  在此添加控件通知处理程序代码
	CString LineName,ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	
	ModuleComboxPaint(LineName);

	m_ModuleComboBox.GetWindowText(ModuleName);
	DeviceComboxPaint(LineName, ModuleName);

	ListOnPaint(LineName, ModuleName);
}


void CEditDeviceParaTabDlg::OnCbnSelchangeModule()
{
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);
	
	DeviceComboxPaint(LineName, ModuleName);

	ListOnPaint(LineName, ModuleName);

}


void CEditDeviceParaTabDlg::OnNMRClickLi1EditdeviceparaTabdlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_nSelectedItem = -1;
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		m_nSelectedItem = lpNMItemActivate->iItem;
	}
	if (m_nSelectedItem == -1)
	{
		return;
	}
	m_list1.SetItemState(m_nSelectedItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	CMenu menu, *pSubMenu;
	menu.LoadMenu(IDR_INITPARADLG_POP_MENU1);
	pSubMenu = menu.GetSubMenu(0);
	CPoint point1;        //存储鼠标位置的临时变量//
	GetCursorPos(&point1);//得到光标处//
	UINT nItem1 = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_TOPALIGN, point1.x, point1.y, GetParent());//确保右键点击在哪菜单出现在哪，并返回所选的菜单项//
	
	size_t DataIndex = m_IndexMap[m_nSelectedItem];
	switch (nItem1)
	{
	case ID_PARA_MODIFY:  //右键菜单：修改//
		m_DeviceParaDlg.m_nSelectedItem = DataIndex;
		m_DeviceParaDlg.DoModal();
		UpdateItemInList(m_nSelectedItem, m_pDataProvider->m_vectDevicePara[DataIndex]);

		break;
	case ID_PARA_DELETE:
		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbDevicePara, m_pDataProvider->m_vectDevicePara[DataIndex].m_Id);
		m_pDataProvider->m_vectDevicePara.erase(m_pDataProvider->m_vectDevicePara.begin() + DataIndex);

		DeleteItemInList(m_nSelectedItem);
		break;
	default:
		break;
	}

	//ListOnPaint();
	

}




void CEditDeviceParaTabDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
