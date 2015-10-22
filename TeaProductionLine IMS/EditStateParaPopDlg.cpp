// EditStateParaPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditStateParaPopDlg.h"
#include "afxdialogex.h"


// CEditStateParaPopDlg 对话框

IMPLEMENT_DYNAMIC(CEditStateParaPopDlg, CDialog)

CEditStateParaPopDlg::CEditStateParaPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStateParaPopDlg::IDD, pParent)
{

}

CEditStateParaPopDlg::~CEditStateParaPopDlg()
{
}

void CEditStateParaPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO2_EDITSTATEPARA_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO3_EDITSTATEPARA_POPDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO4_EDITSTATEPARA_POPDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED1_EDITSTATEPARA_POPDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED2_EDITSTATEPARA_POPDLG, m_AddrIndexEdit);
	DDX_Control(pDX, IDC_ED3_EDITSTATEPARA_POPDLG, m_NoteEdit);
}


BEGIN_MESSAGE_MAP(CEditStateParaPopDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CO2_EDITSTATEPARA_POPDLG, &CEditStateParaPopDlg::OnCbnSelchangeLine)
	ON_BN_CLICKED(IDOK, &CEditStateParaPopDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CO3_EDITSTATEPARA_POPDLG, &CEditStateParaPopDlg::OnCbnSelchangeCo3EditstateparaPopdlg)
END_MESSAGE_MAP()


// CEditStateParaPopDlg 消息处理程序


void CEditStateParaPopDlg::OnCbnSelchangeLine()
{
	CString LineName;
	m_LineComboBox.GetWindowText(LineName);

	ModuleComboxPaint(LineName);
}


void CEditStateParaPopDlg::OnBnClickedOk()
{
	CStatePara tempStatePara = m_pDataProvider->m_vectStatePara[m_nSelectedItem];
	m_LineComboBox.GetWindowText(tempStatePara.m_strProductionLineName);

	m_ModuleComboBox.GetWindowText(tempStatePara.m_strProcessModuleName);

	m_PlcComboBox.GetWindowText(tempStatePara.m_strPlcName);

	m_NameEdit.GetWindowText(tempStatePara.m_strParaName);
	m_NoteEdit.GetWindowText(tempStatePara.m_strDescription);

	m_AddrIndexEdit.GetWindowText(tempStatePara.m_strAddressIndex);

	if (!m_ParaCheckUtil.StateParaCheck(tempStatePara, m_nSelectedItem))
	{
		m_pDataProvider->m_vectStatePara[m_nSelectedItem] = tempStatePara;
		m_pDataProvider->UpdateTableItem(CDataProvider::tbStatePara, m_pDataProvider->m_vectStatePara[m_nSelectedItem].m_Id);
	}

	CDialog::OnOK();
}


BOOL CEditStateParaPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CStatePara tempStatePara = m_pDataProvider->m_vectStatePara[m_nSelectedItem];

	m_NameEdit.SetWindowText(tempStatePara.m_strParaName);
	m_AddrIndexEdit.SetWindowText(tempStatePara.m_strAddressIndex);
	m_NoteEdit.SetWindowText(tempStatePara.m_strDescription);

	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempStatePara.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//把剩余的可选生产线加入到Combobox中
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		if (m_pDataProvider->m_vectProductionLine[i].m_strLineName != tempStatePara.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		}
	}

	m_ModuleComboBox.ResetContent();
	m_ModuleComboBox.AddString(tempStatePara.m_strProcessModuleName);
	m_ModuleComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName != tempStatePara.m_strProcessModuleName && (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == tempStatePara.m_strProductionLineName))
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}


	m_PlcComboBox.ResetContent();
	m_PlcComboBox.AddString(tempStatePara.m_strPlcName);
	m_PlcComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		if (m_pDataProvider->m_vectPlc[i].m_strPlcName != tempStatePara.m_strPlcName)
		{
			m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		}

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


int CEditStateParaPopDlg::ModuleComboxPaint(CString LineName)
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


void CEditStateParaPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


void CEditStateParaPopDlg::OnCbnSelchangeCo3EditstateparaPopdlg()
{
	// TODO:  在此添加控件通知处理程序代码
}
