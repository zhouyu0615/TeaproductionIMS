// EditModuleParaTabDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditModuleParaTabDlg.h"
#include "afxdialogex.h"


// CEditModuleParaTabDlg �Ի���

IMPLEMENT_DYNAMIC(CEditModuleParaTabDlg, CDialog)

CEditModuleParaTabDlg::CEditModuleParaTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditModuleParaTabDlg::IDD, pParent)
{

}

CEditModuleParaTabDlg::~CEditModuleParaTabDlg()
{
}

void CEditModuleParaTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LI1_EDITMODULEPARA_TABDLG, m_list1);
	DDX_Control(pDX, IDC_CO2_EDITMODULEPARA_TABDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO3_EDITMODULEPARA_TABDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO4_EDITMODULEPARA_TABDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED4_EDITMODULEPARA_TABDLG, m_UnitEdit);
	DDX_Control(pDX, IDC_ED1_EDITMODULEPARA_TABDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED3_EDITMODULEPARA_TABDLG, m_ReadAddrIndexEdit);
	DDX_Control(pDX, IDC_CO1_EDITMODULEPARA_TABDLG, m_ValueTypeComboBox);
	DDX_Control(pDX, IDC_ED3_EDITMODULEPARA_TABDLG2, m_WriteAddrIndex);
	DDX_Control(pDX, IDC_COMBO1, m_AddrTypeComboBox);
}


BEGIN_MESSAGE_MAP(CEditModuleParaTabDlg, CDialog)
	ON_BN_CLICKED(IDC_BT1_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnBnClickedAddItem)
	ON_BN_CLICKED(IDC_BT2_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnBnClickedClearEdit)
	ON_BN_CLICKED(IDC_BT3_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnBnClickedClearAll)
	ON_CBN_SELCHANGE(IDC_CO2_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnCbnSelchangeLine)
	ON_NOTIFY(NM_RCLICK, IDC_LI1_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnNMRClickLi1EditmoduleparaTabdlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEditModuleParaTabDlg::OnCbnSelchangeAddrType)
	ON_CBN_SELCHANGE(IDC_CO3_EDITMODULEPARA_TABDLG, &CEditModuleParaTabDlg::OnCbnSelchangeModule)
END_MESSAGE_MAP()


// CEditModuleParaTabDlg ��Ϣ�������//


BOOL CEditModuleParaTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	MyOnPaint();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CEditModuleParaTabDlg::OnBnClickedAddItem()
{
	CProcessPara tempProPara;
	m_LineComboBox.GetWindowText(tempProPara.m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(tempProPara.m_strProcessModuleName);
	m_PlcComboBox.GetWindowText(tempProPara.m_strPlcName);
	m_UnitEdit.GetWindowText(tempProPara.m_strUnit);

	m_NameEdit.GetWindowText(tempProPara.m_strParaName);
	m_ReadAddrIndexEdit.GetWindowText(tempProPara.m_strReadAddrIndex);
	m_WriteAddrIndex.GetWindowText(tempProPara.m_strWriteAddrIndex);


	CString strValType;
	m_ValueTypeComboBox.GetWindowText(strValType);
	if (strValType == _T("Bool"))
	{
		tempProPara.m_ValueType = CProcessPara::VALUETYPE_BOOL;
	}
	else if (strValType == _T("Byte"))
	{
		tempProPara.m_ValueType = CProcessPara::VALUETYPE_BYTE;
	}
	else if (strValType == _T("Short"))
	{
		tempProPara.m_ValueType = CProcessPara::VALUETYPE_SHORT;
	}
	else if (strValType == _T("Float"))
	{
		tempProPara.m_ValueType = CProcessPara::VALUETYPE_FLOAT;
	}

	

	m_AddrTypeComboBox.GetWindowText(tempProPara.m_strAddressType);

	tempProPara.m_IsConfig = GetConfigState();
	tempProPara.m_IsVisible = GetVisibleState();

	tempProPara.m_IsRecord = GetRecordState(); //�Ƿ������ʷ��¼//
	
	if (!m_ParaCheckUtil.ProcessParaCheck(tempProPara))
	{
		m_pDataProvider->AddProcessParaToDatabase(tempProPara);
		AddItemToList(tempProPara);

		int i = m_list1.GetItemCount()-1;
		int index = m_pDataProvider->m_vectProModulePara.size()-1;
		m_IndexMap[i] = index;

	}

	//ListOnPaint();
}

void CEditModuleParaTabDlg::OnBnClickedClearEdit()
{
	m_NameEdit.SetWindowText(_T(""));
	m_UnitEdit.SetWindowText(_T(""));
	m_ReadAddrIndexEdit.SetWindowText(_T(""));
	m_WriteAddrIndex.SetWindowText(_T(""));

	
}


void CEditModuleParaTabDlg::OnBnClickedClearAll()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nResult = MessageBox(_T("�ò�����ɾ�����еĹ��ղ������Ƿ������ǰ������"), _T("����"), MB_ICONEXCLAMATION | MB_YESNO);//����//
	if (nResult == IDYES)
	{
		m_pDataProvider->m_vectProModulePara.clear();
		m_pDataProvider->DeleteDbTable(CDataProvider::tbProcessPara);		
		
		MyOnPaint();
	}
}



void CEditModuleParaTabDlg::OnCbnSelchangeLine()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString LineName,ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);

	m_ModuleComboBox.GetWindowText(ModuleName);

	ListOnPaint(LineName, ModuleName);

}

void CEditModuleParaTabDlg::OnCbnSelchangeModule()
{
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);

	ListOnPaint(LineName, ModuleName); //�����б��//
}



void CEditModuleParaTabDlg::OnNMRClickLi1EditmoduleparaTabdlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	CPoint point1;        //�洢���λ�õ���ʱ����//
	GetCursorPos(&point1);//�õ���괦//
	UINT nItem1 = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_TOPALIGN, point1.x, point1.y, GetParent());//ȷ���Ҽ�������Ĳ˵��������ģ���������ѡ�Ĳ˵���//

	int DataIndex = m_IndexMap[m_nSelectedItem];
	switch (nItem1)
	{
	case ID_PARA_MODIFY:  //�Ҽ��˵����޸�//
		m_ModuleParaPopDlg.m_DataIndex = DataIndex;
		m_ModuleParaPopDlg.DoModal();
		UpdateItemInList(m_nSelectedItem, m_pDataProvider->m_vectProModulePara[DataIndex]);
		break;
	case ID_PARA_DELETE:
		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbProcessPara, m_pDataProvider->m_vectProModulePara[DataIndex].m_Id);
		m_pDataProvider->m_vectProModulePara.erase(m_pDataProvider->m_vectProModulePara.begin() + DataIndex);
		
		DeleteItemInList(m_nSelectedItem);

		break;
	default:
		break;
	}

	//ListOnPaint();

}



int CEditModuleParaTabDlg::MyOnPaint()
{
	

	ShowConfigState(TRUE);
	ShowVisibleState(TRUE);
	
	ShowRecordState(FALSE);

	AddrTypeComboBoxInit();
	OnCbnSelchangeAddrType();
	

	LineComboxPaint();
	CString LineName, ModuleName;
	//m_LineComboBox.GetLBText(0, LineName);
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);

	m_ModuleComboBox.GetWindowText(ModuleName);

	ListOnPaint(LineName, ModuleName);

	PlcComboxPaint();

	return 0;
}

int CEditModuleParaTabDlg::LineComboxPaint()
{
	m_LineComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		m_LineComboBox.SetCurSel(0);
	}
	
	return 0;
}

int CEditModuleParaTabDlg::ModuleComboxPaint(CString LineName)
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

int CEditModuleParaTabDlg::PlcComboxPaint()
{
	m_PlcComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		m_PlcComboBox.SetCurSel(0);
	}
	
	return 0;
}

int CEditModuleParaTabDlg::ValTypeComboBoxInit()
{
	//m_ValueTypeComboBox.ResetContent();
	//m_ValueTypeComboBox.AddString(_T("Bool"));
	//m_ValueTypeComboBox.AddString(_T("Byte"));
	//m_ValueTypeComboBox.AddString(_T("Short"));
	//m_ValueTypeComboBox.AddString(_T("Long"));
	//m_ValueTypeComboBox.AddString(_T("Float"));
	//m_ValueTypeComboBox.AddString(_T("Double"));

	//m_ValueTypeComboBox.SetCurSel(3);

	return 0;
}

void CEditModuleParaTabDlg::AddrTypeComboBoxInit()
{
	m_AddrTypeComboBox.ResetContent();
	m_AddrTypeComboBox.AddString(_T("MX"));
	m_AddrTypeComboBox.AddString(_T("MB"));
	m_AddrTypeComboBox.AddString(_T("MW"));
	m_AddrTypeComboBox.AddString(_T("MD"));

	m_AddrTypeComboBox.SetCurSel(2);
}

int CEditModuleParaTabDlg::ListOnPaint(const CString &ProLineName, const CString &ProModuleName)
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");
	//�����б�ؼ����//
	CRect rect1;
	m_list1.GetWindowRect(&rect1);
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE);

	//����б�//
	m_list1.DeleteAllItems();
	CHeaderCtrl *pmyHeaderCtrl = m_list1.GetHeaderCtrl();
	int nCount = pmyHeaderCtrl->GetItemCount();
	for (int i = nCount - 1; i >= 0; i--)
		m_list1.DeleteColumn(i);

	m_list1.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_list1.InsertColumn(1, _T("���"), LVCFMT_CENTER, rect1.Width() / 17, -1);
	m_list1.InsertColumn(2, _T("����������"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(3, _T("����ģ��"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(4, _T("����PLC"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(5, _T("��������"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(6, _T("��ַ����"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(7, _T("����ַ"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(8, _T("д��ַ"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(9, _T("��������"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(10, _T("�Ƿ��䷽"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(11, _T("�Ƿ�ɼ�"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	m_list1.InsertColumn(12, _T("�Ƿ��¼"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);

	m_list1.InsertColumn(13, _T("��λ"), LVCFMT_CENTER, rect1.Width() / 17 * 2, -1);
	//��д������//
	int length = m_pDataProvider->m_vectProModulePara.size();
	int index = 0; //������//

	m_IndexMap.clear(); //��չ���������//
	for (size_t i = 0; i < length; i++)
	{
		if (ProLineName == m_pDataProvider->m_vectProModulePara[i].m_strProductionLineName
			&&ProModuleName == m_pDataProvider->m_vectProModulePara[i].m_strProcessModuleName)
		{
			litem.iItem = index;
			m_list1.InsertItem(&litem);
			SetListItemText(index, m_pDataProvider->m_vectProModulePara[i]);
			m_IndexMap[index] = i;
			index++;
		}

	}
	return 0;
}


void CEditModuleParaTabDlg::AddItemToList(CProcessPara &processPara)
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");

	int i = m_list1.GetItemCount();
	litem.iItem = i;
	m_list1.InsertItem(&litem);

	SetListItemText(i, processPara);

	m_list1.EnsureVisible(i, TRUE);


}


void CEditModuleParaTabDlg::UpdateItemInList(int Index, CProcessPara &processPara)
{
	SetListItemText(Index, processPara);

	m_list1.EnsureVisible(Index, TRUE);
	

}
void CEditModuleParaTabDlg::DeleteItemInList(int Index)
{
	m_list1.DeleteItem(Index);

	int length = m_list1.GetItemCount();
	//����INDEX֮��ı��//
	for (int i = Index; i < length; i++)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_list1.SetItemText(i, 1, str);
	}
}


void CEditModuleParaTabDlg::SetListItemText(int Index, CProcessPara &processPara)
{
	CString str;
	str.Format(_T("%d"), Index + 1);
	m_list1.SetItemText(Index, 1, str);
	m_list1.SetItemText(Index, 2, processPara.m_strProductionLineName);
	m_list1.SetItemText(Index, 3, processPara.m_strProcessModuleName);
	m_list1.SetItemText(Index, 4, processPara.m_strPlcName);
	m_list1.SetItemText(Index, 5, processPara.m_strParaName);
	m_list1.SetItemText(Index, 6, processPara.m_strAddressType);
	m_list1.SetItemText(Index, 7, processPara.m_strReadAddrIndex);
	m_list1.SetItemText(Index, 8, processPara.m_strWriteAddrIndex);
	m_list1.SetItemText(Index, 9, processPara.ConvertValTypeToString());
	m_list1.SetItemText(Index, 10, processPara.ConvertIsConfigToString());
	m_list1.SetItemText(Index, 11, processPara.ConvertIsVisibleToString());

	m_list1.SetItemText(Index, 12, processPara.ConvertIsRecordToString());

	m_list1.SetItemText(Index, 13, processPara.m_strUnit);
}





void CEditModuleParaTabDlg::ShowConfigState(BOOL IsConfig)
{
	if (IsConfig) //��
	{
		((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA2_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);// ��ѡ��
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RA2_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);
	}

}
void CEditModuleParaTabDlg::ShowVisibleState(BOOL IsVisible)
{
	if (IsVisible)
	{
		((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA4_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA4_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);
	}

}

BOOL CEditModuleParaTabDlg::GetConfigState()
{
	if (((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_TABDLG))->GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CEditModuleParaTabDlg::GetVisibleState()
{
	if (((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_TABDLG))->GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}



void CEditModuleParaTabDlg::ShowRecordState(BOOL IsRecord)
{

	if (IsRecord) //��
	{
		((CButton *)GetDlgItem(IDC_RA5_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA6_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);// ��ѡ��
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RA5_EDITMODULEPARA_TABDLG))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RA6_EDITMODULEPARA_TABDLG))->SetCheck(TRUE);
	}

}
BOOL CEditModuleParaTabDlg::GetRecordState()
{
	if (((CButton *)GetDlgItem(IDC_RA5_EDITMODULEPARA_TABDLG))->GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}








void CEditModuleParaTabDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}



//����ѡ��ĵ�ַ���ͣ���ȷ��ֵ��������Щ����ѡ��//
void CEditModuleParaTabDlg::OnCbnSelchangeAddrType()
{
	CString strAddrType;
	m_AddrTypeComboBox.GetWindowText(strAddrType);
	if (strAddrType == _T("MX"))//
	{
		m_ValueTypeComboBox.ResetContent();
		m_ValueTypeComboBox.AddString(_T("Bool"));
		m_ValueTypeComboBox.SetCurSel(0);
	}
	if (strAddrType == _T("MB"))//
	{
		m_ValueTypeComboBox.ResetContent();
		m_ValueTypeComboBox.AddString(_T("Byte"));
		m_ValueTypeComboBox.SetCurSel(0);
	}

	if (strAddrType == _T("MW"))//
	{
		m_ValueTypeComboBox.ResetContent();
		m_ValueTypeComboBox.AddString(_T("Short"));
		m_ValueTypeComboBox.SetCurSel(0);
	}

	if (strAddrType == _T("MD"))
	{
		m_ValueTypeComboBox.ResetContent();
		m_ValueTypeComboBox.AddString(_T("Float"));
		m_ValueTypeComboBox.SetCurSel(0);
	}
}


