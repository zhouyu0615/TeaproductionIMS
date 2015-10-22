// EditDeviceParaPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditDeviceParaPopDlg.h"
#include "afxdialogex.h"


// CEditDeviceParaPopDlg 对话框

IMPLEMENT_DYNAMIC(CEditDeviceParaPopDlg, CDialog)

CEditDeviceParaPopDlg::CEditDeviceParaPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDeviceParaPopDlg::IDD, pParent)
{

}

CEditDeviceParaPopDlg::~CEditDeviceParaPopDlg()
{
}

void CEditDeviceParaPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO1_EDITDEVICEPARA_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO2_EDITDEVICEPARA_POPDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_CO3_EDITDEVICEPARA_POPDLG, m_DeviceComboBox);
	DDX_Control(pDX, IDC_CO4_EDITDEVICEPARA_POPDLG, m_PlcComboBox);
	DDX_Control(pDX, IDC_ED1_EDITDEVICEPARA_POPDLG, m_NameEdit);
	DDX_Control(pDX, IDC_ED2_EDITDEVICEPARA_POPDLG, m_ControlAddrEdit);
	DDX_Control(pDX, IDC_ED3_EDITDEVICEPARA_POPDLG, m_StateAddrEdit);
	DDX_Control(pDX, IDC_ED4_EDITDEVICEPARA_POPDLG, m_DescriptionEdit);
}


BEGIN_MESSAGE_MAP(CEditDeviceParaPopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditDeviceParaPopDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CO1_EDITDEVICEPARA_POPDLG, &CEditDeviceParaPopDlg::OnCbnSelchangeLine)
	ON_CBN_SELCHANGE(IDC_CO2_EDITDEVICEPARA_POPDLG, &CEditDeviceParaPopDlg::OnCbnSelchangeModule)
	ON_CBN_SELCHANGE(IDC_CO3_EDITDEVICEPARA_POPDLG, &CEditDeviceParaPopDlg::OnCbnSelchangeCo3EditdeviceparaPopdlg)
END_MESSAGE_MAP()


// CEditDeviceParaPopDlg 消息处理程序


void CEditDeviceParaPopDlg::OnBnClickedOk()
{
	CDevicePara tempDevicePara;
	tempDevicePara = m_pDataProvider->m_vectDevicePara[m_nSelectedItem];

	m_LineComboBox.GetWindowText(tempDevicePara.m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(tempDevicePara.m_strProcessModuleName);

	m_DeviceComboBox.GetWindowText(tempDevicePara.m_strDeviceName);
	m_PlcComboBox.GetWindowText(tempDevicePara.m_strPlcName);

	m_NameEdit.GetWindowText(tempDevicePara.m_strParaName);
	m_ControlAddrEdit.GetWindowText(tempDevicePara.m_strControlAddrIndex);
	m_StateAddrEdit.GetWindowText(tempDevicePara.m_strStateAddrIndex);
	m_DescriptionEdit.GetWindowText(tempDevicePara.m_strDescription);

	if (!m_ParaCheckUtil.DeviceParaCheck(tempDevicePara, m_nSelectedItem))
	{
		m_pDataProvider->m_vectDevicePara[m_nSelectedItem] = tempDevicePara;
		m_pDataProvider->UpdateTableItem(CDataProvider::tbDevicePara, m_pDataProvider->m_vectDevicePara[m_nSelectedItem].m_Id);
	}
	CDialog::OnOK();
}


BOOL CEditDeviceParaPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDevicePara tempDevicePara;
	tempDevicePara = m_pDataProvider->m_vectDevicePara[m_nSelectedItem];

	m_NameEdit.SetWindowText(tempDevicePara.m_strParaName);
	m_ControlAddrEdit.SetWindowText(tempDevicePara.m_strControlAddrIndex);
	m_StateAddrEdit.SetWindowText(tempDevicePara.m_strStateAddrIndex);
	m_DescriptionEdit.SetWindowText(tempDevicePara.m_strDescription);

	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempDevicePara.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//把剩余的可选生产线加入到Combobox中
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size();i++)
	{
		if (m_pDataProvider->m_vectProductionLine[i].m_strLineName!=tempDevicePara.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		}
	}

	m_ModuleComboBox.ResetContent();
	m_ModuleComboBox.AddString(tempDevicePara.m_strProcessModuleName);
	m_ModuleComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size();i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName != tempDevicePara.m_strProcessModuleName&&(m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName==tempDevicePara.m_strProductionLineName))
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}

	m_DeviceComboBox.ResetContent();
	m_DeviceComboBox.AddString(tempDevicePara.m_strDeviceName);
	m_DeviceComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectDevice.size();i++)
	{
		
		if ((m_pDataProvider->m_vectDevice[i].m_strProductionLineName == tempDevicePara.m_strProductionLineName) && (m_pDataProvider->m_vectDevice[i].m_strProcessModuleName == tempDevicePara.m_strProcessModuleName) && m_pDataProvider->m_vectDevice[i].m_strDeviceName != tempDevicePara.m_strDeviceName)
		{

			m_DeviceComboBox.AddString(m_pDataProvider->m_vectDevice[i].m_strDeviceName);
		}
	}

	m_PlcComboBox.ResetContent();
	m_PlcComboBox.AddString(tempDevicePara.m_strPlcName);
	m_PlcComboBox.SetCurSel(0);
	for (int i = 0; i < m_pDataProvider->m_vectPlc.size(); i++)
	{
		if (m_pDataProvider->m_vectPlc[i].m_strPlcName!=tempDevicePara.m_strPlcName)
		{
			m_PlcComboBox.AddString(m_pDataProvider->m_vectPlc[i].m_strPlcName);
		}
		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditDeviceParaPopDlg::OnCbnSelchangeLine()
{
	// TODO:  在此添加控件通知处理程序代码
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);

	ModuleComboxPaint(LineName);

	m_ModuleComboBox.GetWindowText(ModuleName);
	DeviceComboxPaint(LineName, ModuleName);
}
 

void CEditDeviceParaPopDlg::OnCbnSelchangeModule()
{
	// TODO:  在此添加控件通知处理程序代码
	CString LineName, ModuleName;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleComboBox.GetWindowText(ModuleName);

	DeviceComboxPaint(LineName, ModuleName);

}



int CEditDeviceParaPopDlg::ModuleComboxPaint(CString LineName)
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

int CEditDeviceParaPopDlg::DeviceComboxPaint(CString LineName, CString ModuleName)
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




void CEditDeviceParaPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


void CEditDeviceParaPopDlg::OnCbnSelchangeCo3EditdeviceparaPopdlg()
{
	// TODO:  在此添加控件通知处理程序代码
}
