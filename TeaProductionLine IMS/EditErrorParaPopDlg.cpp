// EditErrorParaPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditErrorParaPopDlg.h"
#include "afxdialogex.h"


// CEditErrorParaPopDlg 对话框

IMPLEMENT_DYNAMIC(CEditErrorParaPopDlg, CDialog)

CEditErrorParaPopDlg::CEditErrorParaPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditErrorParaPopDlg::IDD, pParent)
{

}

CEditErrorParaPopDlg::~CEditErrorParaPopDlg()
{
}

void CEditErrorParaPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO1_EDITERRORPARA_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO2_EDITERRORPARA_POPDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO3_EDITERRORPARA_POPDLG, m_DeviceComboBox);
	DDX_Control(pDX, IDC_CO4_EDITERRORPARA_POPDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED1_EDITERRORPARA_POPDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED2_EDITERRORPARA_POPDLG, m_AddrIndexEdit);
	DDX_Control(pDX, IDC_ED3_EDITERRORPARA_POPDLG, m_NoteEdit);
}


BEGIN_MESSAGE_MAP(CEditErrorParaPopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditErrorParaPopDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditErrorParaPopDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CO1_EDITERRORPARA_POPDLG, &CEditErrorParaPopDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_CO2_EDITERRORPARA_POPDLG, &CEditErrorParaPopDlg::OnCbnSelchangeModule)
END_MESSAGE_MAP()


// CEditErrorParaPopDlg 消息处理程序


void CEditErrorParaPopDlg::OnBnClickedOk()
{
	CFaultPara tempFautPara = m_pDataProvider->m_vectFaultPara[m_nSelectedItem];
	m_LineComboBox.GetWindowText(tempFautPara.m_strProductionLineName);

	m_ModuleComboBox.GetWindowText(tempFautPara.m_strProcessName);

	m_DeviceComboBox.GetWindowText(tempFautPara.m_strDeviceName);
	m_PlcComboBox.GetWindowText(tempFautPara.m_strPlcName);

	m_NameEdit.GetWindowText(tempFautPara.m_strParaName);

	m_NoteEdit.GetWindowText(tempFautPara.m_strDescription);

	m_AddrIndexEdit.GetWindowText(tempFautPara.m_strAddressIndex);

	if (!m_ParaCheckUtil.FaultParaCheck(tempFautPara, m_nSelectedItem))
	{
		m_pDataProvider->m_vectFaultPara[m_nSelectedItem] = tempFautPara;
		m_pDataProvider->UpdateTableItem(CDataProvider::tbFaultPara, m_pDataProvider->m_vectFaultPara[m_nSelectedItem].m_Id);
	}
	CDialog::OnOK();
}


void CEditErrorParaPopDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL CEditErrorParaPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFaultPara tempFaultPara = m_pDataProvider->m_vectFaultPara[m_nSelectedItem];

	m_NameEdit.SetWindowText(tempFaultPara.m_strParaName);
	m_AddrIndexEdit.SetWindowText(tempFaultPara.m_strAddressIndex);
	m_NoteEdit.SetWindowText(tempFaultPara.m_strDescription);

	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempFaultPara.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//把剩余的可选生产线加入到Combobox中
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		if (m_pDataProvider->m_vectProductionLine[i].m_strLineName != tempFaultPara.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		}
	}

	m_ModuleComboBox.ResetContent();
	m_ModuleComboBox.AddString(tempFaultPara.m_strProcessName);
	m_ModuleComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName != tempFaultPara.m_strProcessName && (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == tempFaultPara.m_strProductionLineName))
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}

	m_DeviceComboBox.ResetContent();
	m_DeviceComboBox.AddString(tempFaultPara.m_strDeviceName);
	m_DeviceComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectDevice.size(); i++)
	{
		if ((m_pDataProvider->m_vectDevice[i].m_strProductionLineName == tempFaultPara.m_strProductionLineName) && (m_pDataProvider->m_vectDevice[i].m_strProcessModuleName == tempFaultPara.m_strProcessName) && m_pDataProvider->m_vectDevice[i].m_strDeviceName != tempFaultPara.m_strDeviceName)
		{
			m_DeviceComboBox.AddString(tempFaultPara.m_strDeviceName);
		}
	}

	m_PlcComboBox.ResetContent();
	m_PlcComboBox.AddString(tempFaultPara.m_strPlcName);
	m_PlcComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		if (m_pDataProvider->m_vectPlc[i].m_strPlcName != tempFaultPara.m_strPlcName)
		{
			m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditErrorParaPopDlg::OnCbnSelchangeLine()
{
	// TODO:  在此添加控件通知处理程序代码
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);

	ModuleComboxPaint(LineName);

	m_ModuleComboBox.GetWindowText(ModuleName);
	DeviceComboxPaint(LineName, ModuleName);
}


void CEditErrorParaPopDlg::OnCbnSelchangeModule()
{
	// TODO:  在此添加控件通知处理程序代码
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);

	DeviceComboxPaint(LineName, ModuleName);

}


int CEditErrorParaPopDlg::ModuleComboxPaint(CString LineName)
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

int CEditErrorParaPopDlg::DeviceComboxPaint(CString LineName, CString ModuleName)
{
	m_DeviceComboBox.ResetContent();
	for (int i = 0; i < m_pDataProvider->m_vectDevice.size(); i++)
	{
		if ((m_pDataProvider->m_vectDevice[i].m_strProductionLineName == LineName)
			&& (m_pDataProvider->m_vectDevice[i].m_strProcessModuleName == ModuleName))
		{
			m_DeviceComboBox.AddString(m_pDataProvider->m_vectDevice[i].m_strDeviceName);
		}
	}
	m_DeviceComboBox.SetCurSel(0);
	return 0;
}


void CEditErrorParaPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
