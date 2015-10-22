// EditModuleParaPopDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditModuleParaPopDlg.h"
#include "afxdialogex.h"


// CEditModuleParaPopDlg �Ի���

IMPLEMENT_DYNAMIC(CEditModuleParaPopDlg, CDialog)

CEditModuleParaPopDlg::CEditModuleParaPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditModuleParaPopDlg::IDD, pParent)
{

}

CEditModuleParaPopDlg::~CEditModuleParaPopDlg()
{
}

void CEditModuleParaPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO2_EDITMODULEPARA_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO3_EDITMODULEPARA_POPDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO4_EDITMODULEPARA_POPDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED4_EDITMODULEPARA_POPDLG, m_UnitEdit);
	DDX_Control(pDX, IDC_ED1_EDITMODULEPARA_POPDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED3_EDITMODULEPARA_POPDLG, m_ReadAddrIndexEdit);
	DDX_Control(pDX, IDC_ED3_EDITMODULEPARA_POPDLG2, m_WriteAddrIndexEdit);
	DDX_Control(pDX, IDC_CO1_EDITMODULEPARA_POPDLG, m_ValueTypeComboBox);
	DDX_Control(pDX, IDC_COMBO1, m_AddrTypeComboBox);
}


BEGIN_MESSAGE_MAP(CEditModuleParaPopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditModuleParaPopDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CO2_EDITMODULEPARA_POPDLG, &CEditModuleParaPopDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEditModuleParaPopDlg::OnCbnSelchangeAddrType)
END_MESSAGE_MAP()


// CEditModuleParaPopDlg ��Ϣ�������


void CEditModuleParaPopDlg::OnBnClickedOk()
{
	CProcessPara tempProcessPara = m_pDataProvider->m_vectProModulePara[m_nSelectedItem];
	m_LineComboBox.GetWindowText(tempProcessPara.m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(tempProcessPara.m_strProcessModuleName);

	m_PlcComboBox.GetWindowText(tempProcessPara.m_strPlcName);

	m_UnitEdit.GetWindowText(tempProcessPara.m_strUnit);

	m_NameEdit.GetWindowText(tempProcessPara.m_strParaName);
	m_ReadAddrIndexEdit.GetWindowText(tempProcessPara.m_strReadAddrIndex);
	m_WriteAddrIndexEdit.GetWindowText(tempProcessPara.m_strWriteAddrIndex);

	m_AddrTypeComboBox.GetWindowText(tempProcessPara.m_strAddressType);



	CString strValType;
	m_ValueTypeComboBox.GetWindowText(strValType);
	if (strValType == _T("Bool"))
	{
		tempProcessPara.m_ValueType = CProcessPara::VALUETYPE_BOOL;
	}
	else if (strValType == _T("Byte"))
	{
		tempProcessPara.m_ValueType = CProcessPara::VALUETYPE_BYTE;
	}
	else if (strValType == _T("Short"))
	{
		tempProcessPara.m_ValueType = CProcessPara::VALUETYPE_SHORT;
	}
	else if (strValType == _T("Float"))
	{
		tempProcessPara.m_ValueType = CProcessPara::VALUETYPE_FLOAT;
	}


	tempProcessPara.m_IsConfig = GetConfigState();
	tempProcessPara.m_IsVisible = GetVisibleState();

	if (!m_ParaCheckUtil.ProcessParaCheck(tempProcessPara, m_nSelectedItem))
	{
		m_pDataProvider->m_vectProModulePara[m_nSelectedItem] = tempProcessPara;
		m_pDataProvider->UpdateTableItem(CDataProvider::tbProcessPara, m_pDataProvider->m_vectProModulePara[m_nSelectedItem].m_Id);
	}


	CDialog::OnOK();
}


void CEditModuleParaPopDlg::OnCbnSelchangeLine()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString LineName;
	m_LineComboBox.GetWindowText(LineName);
	ModuleComboxPaint(LineName);

}




BOOL CEditModuleParaPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CProcessPara tempProPara = m_pDataProvider->m_vectProModulePara[m_nSelectedItem];

	m_NameEdit.SetWindowText(tempProPara.m_strParaName);
	m_UnitEdit.SetWindowText(tempProPara.m_strUnit);
	m_ReadAddrIndexEdit.SetWindowText(tempProPara.m_strReadAddrIndex);
	m_WriteAddrIndexEdit.SetWindowText(tempProPara.m_strWriteAddrIndex);

	ShowConfigState(tempProPara.m_IsConfig);
	ShowVisibleState(tempProPara.m_IsVisible);

	AddrTypeComboBoxInit();
	m_AddrTypeComboBox.SetCurSel(tempProPara.GetAddrTypeInEmType());

	OnCbnSelchangeAddrType(); //���ݵ�ַ������ʾ��������

	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempProPara.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//��ʣ��Ŀ�ѡ�����߼��뵽Combobox��
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		if (m_pDataProvider->m_vectProductionLine[i].m_strLineName != tempProPara.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		}
	}

	m_ModuleComboBox.ResetContent();
	m_ModuleComboBox.AddString(tempProPara.m_strProcessModuleName);
	m_ModuleComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName != tempProPara.m_strProcessModuleName && (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == tempProPara.m_strProductionLineName))
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}

	m_PlcComboBox.ResetContent();
	m_PlcComboBox.AddString(tempProPara.m_strPlcName);
	m_PlcComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		if (m_pDataProvider->m_vectPlc[i].m_strPlcName != tempProPara.m_strPlcName)
		{
			m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

int CEditModuleParaPopDlg::ValTypeComboBoxInit()
{
	//m_ValueTypeComboBox.ResetContent();
	//m_ValueTypeComboBox.AddString(_T("Bool"));
	//m_ValueTypeComboBox.AddString(_T("Byte"));
	//m_ValueTypeComboBox.AddString(_T("Short"));
	//m_ValueTypeComboBox.AddString(_T("Long"));
	//m_ValueTypeComboBox.AddString(_T("Float"));

	return 0;
}



void CEditModuleParaPopDlg::AddrTypeComboBoxInit()
{
	m_AddrTypeComboBox.ResetContent();
	m_AddrTypeComboBox.AddString(_T("MX"));
	m_AddrTypeComboBox.AddString(_T("MB"));
	m_AddrTypeComboBox.AddString(_T("MW"));
	m_AddrTypeComboBox.AddString(_T("MD"));

}

int CEditModuleParaPopDlg::ModuleComboxPaint(CString LineName)
{
	m_ModuleComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == LineName)
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}
	m_ModuleComboBox.SetCurSel(0);
	return 0;
}


void CEditModuleParaPopDlg::ShowConfigState(BOOL IsConfig)
{
	if (IsConfig) //��
	{
		((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_POPDLG))->SetCheck(TRUE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA2_EDITMODULEPARA_POPDLG))->SetCheck(FALSE);// ��ѡ��
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_POPDLG))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RA2_EDITMODULEPARA_POPDLG))->SetCheck(TRUE);
	}

}
void CEditModuleParaPopDlg::ShowVisibleState(BOOL IsVisible)
{
	if (IsVisible)
	{
		((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_POPDLG))->SetCheck(TRUE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA4_EDITMODULEPARA_POPDLG))->SetCheck(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_POPDLG))->SetCheck(FALSE);//ѡ��
		((CButton *)GetDlgItem(IDC_RA4_EDITMODULEPARA_POPDLG))->SetCheck(TRUE);
	}

}

BOOL CEditModuleParaPopDlg::GetConfigState()
{
	if (((CButton *)GetDlgItem(IDC_RA1_EDITMODULEPARA_POPDLG))->GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CEditModuleParaPopDlg::GetVisibleState()
{
	if (((CButton *)GetDlgItem(IDC_RA3_EDITMODULEPARA_POPDLG))->GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}



void CEditModuleParaPopDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}


//����ѡ��ĵ�ַ����ȷ����������//
void CEditModuleParaPopDlg::OnCbnSelchangeAddrType()
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
