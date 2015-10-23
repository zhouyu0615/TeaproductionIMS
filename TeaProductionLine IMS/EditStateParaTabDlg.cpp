// EditStateParaTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditStateParaTabDlg.h"
#include "afxdialogex.h"


// CEditStateParaTabDlg 对话框

IMPLEMENT_DYNAMIC(CEditStateParaTabDlg, CDialog)

CEditStateParaTabDlg::CEditStateParaTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStateParaTabDlg::IDD, pParent)
{

}

CEditStateParaTabDlg::~CEditStateParaTabDlg()
{
}

void CEditStateParaTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_CO2_EDITSTATEPARA_TABDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO3_EDITSTATEPARA_TABDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO4_EDITSTATEPARA_TABDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED1_EDITSTATEPARA_TABDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED2_EDITSTATEPARA_TABDLG, m_AddrIndexEdit);
	DDX_Control(pDX, IDC_ED3_EDITSTATEPARA_TABDLG, m_NoteEdit);
}


BEGIN_MESSAGE_MAP(CEditStateParaTabDlg, CDialog)
	ON_BN_CLICKED(IDC_BT1_EDITSTATEPARA_TABDLG, &CEditStateParaTabDlg::OnBnClickedAddItem)
	ON_BN_CLICKED(IDC_BT2_EDITSTATEPARA_TABDLG, &CEditStateParaTabDlg::OnBnClickedClearEdit)
	ON_BN_CLICKED(IDC_BT3_EDITSTATEPARA_TABDLG, &CEditStateParaTabDlg::OnBnClickedClearAll)
	ON_CBN_SELCHANGE(IDC_CO2_EDITSTATEPARA_TABDLG, &CEditStateParaTabDlg::OnCbnSelchangeLine)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CEditStateParaTabDlg::OnNMRClickList1)
END_MESSAGE_MAP()


// CEditStateParaTabDlg 消息处理程序


void CEditStateParaTabDlg::OnBnClickedAddItem()
{
	CStatePara tempStatePara;
	m_LineComboBox.GetWindowText(tempStatePara.m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(tempStatePara.m_strProcessModuleName);
	m_PlcComboBox.GetWindowText(tempStatePara.m_strPlcName);

	m_NameEdit.GetWindowText(tempStatePara.m_strParaName);
	m_AddrIndexEdit.GetWindowText(tempStatePara.m_strAddressIndex);
	m_NoteEdit.GetWindowText(tempStatePara.m_strDescription);

	if (!m_ParaCheckUtil.StateParaCheck(tempStatePara))
	{
		m_pDataProvider->AddStateParaToDatabase(tempStatePara);
		AddItemToList(tempStatePara);
	}

	//ListOnPaint();

}


BOOL CEditStateParaTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化//

	MyOnPaint();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


int CEditStateParaTabDlg::MyOnPaint()
{
	ListOnPaint();
	LineComboxPaint();
	CString LineName;
	//m_LineComboBox.GetLBText(0, LineName);
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);
	PlcComboxPaint();

	return 0;
}

void CEditStateParaTabDlg::OnBnClickedClearEdit()
{
	m_NameEdit.SetWindowText(_T(""));
	m_AddrIndexEdit.SetWindowText(_T(""));
	m_NoteEdit.SetWindowText(_T(""));
}


void CEditStateParaTabDlg::OnBnClickedClearAll()
{
	int nResult = MessageBox(_T("该操作将删除所有的状态参数，是否继续当前操作？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
	if (nResult == IDYES)
	{ 
		m_pDataProvider->m_vectStatePara.clear();
		m_pDataProvider->DeleteDbTable(CDataProvider::tbStatePara);
		ListOnPaint();
		OnBnClickedClearEdit();
	}

}


void CEditStateParaTabDlg::OnCbnSelchangeLine()
{
	CString LineName;
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);
}


int CEditStateParaTabDlg::LineComboxPaint()
{
	m_LineComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		m_LineComboBox.SetCurSel(0);
	}
	
	return 0;
}

int CEditStateParaTabDlg::ModuleComboxPaint(CString LineName)
{
	m_ModuleComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == LineName)
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
			m_ModuleComboBox.SetCurSel(0);
		}
	}
	
	return 0;
}

int CEditStateParaTabDlg::PlcComboxPaint()
{
	m_PlcComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		m_PlcComboBox.SetCurSel(0);
	}
	
	return 0;
}

int CEditStateParaTabDlg::ListOnPaint()
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
	m_list1.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 13, -1);
	m_list1.InsertColumn(2, _T("所属生产线"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	m_list1.InsertColumn(3, _T("所属模块"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	m_list1.InsertColumn(4, _T("所属PLC"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	m_list1.InsertColumn(5, _T("参数名称"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	m_list1.InsertColumn(6, _T("参数地址"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	m_list1.InsertColumn(7, _T("备注"), LVCFMT_CENTER, rect1.Width() / 13 * 2, -1);
	//填写表单内容//
	int length = m_pDataProvider->m_vectStatePara.size();
	for (int i = 0; i < length; i++)
	{
		litem.iItem = i;
		m_list1.InsertItem(&litem);
		
		SetListItemText(i, m_pDataProvider->m_vectStatePara[i]);
	}
	return 0;
}


void CEditStateParaTabDlg::SetListItemText(int Index, CStatePara &statePara)
{
	CString str;
	int i = Index;
	str.Format(_T("%d"), i + 1);
	m_list1.SetItemText(i, 1, str);
	m_list1.SetItemText(i, 2, m_pDataProvider->m_vectStatePara[i].m_strProductionLineName);
	m_list1.SetItemText(i, 3, m_pDataProvider->m_vectStatePara[i].m_strProcessModuleName);
	m_list1.SetItemText(i, 4, m_pDataProvider->m_vectStatePara[i].m_strPlcName);
	m_list1.SetItemText(i, 5, m_pDataProvider->m_vectStatePara[i].m_strParaName);
	m_list1.SetItemText(i, 6, m_pDataProvider->m_vectStatePara[i].m_strAddressIndex);
	m_list1.SetItemText(i, 7, m_pDataProvider->m_vectStatePara[i].m_strDescription);

}

void CEditStateParaTabDlg::AddItemToList(CStatePara &statePara)
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");

	int i = m_list1.GetItemCount();
	litem.iItem = i;
	m_list1.InsertItem(&litem);

	SetListItemText(i, statePara);

	m_list1.EnsureVisible(i, TRUE);
}


void CEditStateParaTabDlg::UpdateItemInList(int Index, CStatePara &statePara)
{
	SetListItemText(Index, statePara);

	m_list1.EnsureVisible(Index, TRUE);

}
void CEditStateParaTabDlg::DeleteItemInList(int Index)
{
	m_list1.DeleteItem(Index);

	size_t length = m_pDataProvider->m_vectStatePara.size();

	for (int i = Index; i < length;i++)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_list1.SetItemText(i, 1, str);
	}
}




void CEditStateParaTabDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
	
	switch (nItem1)
	{
	case ID_PARA_MODIFY:  //右键菜单：修改//
		m_StateParaPopDlg.m_nSelectedItem = m_nSelectedItem;
		m_StateParaPopDlg.DoModal();
		UpdateItemInList(m_nSelectedItem, m_pDataProvider->m_vectStatePara[m_nSelectedItem]);

		break;
	case ID_PARA_DELETE:
		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbStatePara, m_pDataProvider->m_vectStatePara[m_nSelectedItem].m_Id);
		m_pDataProvider->m_vectStatePara.erase(m_pDataProvider->m_vectStatePara.begin() + m_nSelectedItem);
		DeleteItemInList(m_nSelectedItem);

		break;
	default:
		break;
	}

	//ListOnPaint();


}


void CEditStateParaTabDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
