// EditModulePopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditModulePopDlg.h"
#include "afxdialogex.h"
#include "ProcessModuleClass.h"

// CEditModulePopDlg 对话框

IMPLEMENT_DYNAMIC(CEditModulePopDlg, CDialog)

CEditModulePopDlg::CEditModulePopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditModulePopDlg::IDD, pParent)
{

}

CEditModulePopDlg::~CEditModulePopDlg()
{
}

void CEditModulePopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO1_EDITMODULE_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_ED1_EDITMODULE_POPDLG, m_ModuleNameEdit);
	DDX_Control(pDX, IDC_ED2_EDITMODULE_POPDLG, m_DescriptionEdit);
}


BEGIN_MESSAGE_MAP(CEditModulePopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditModulePopDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditModulePopDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CO1_EDITMODULE_POPDLG, &CEditModulePopDlg::OnCbnSelchangeCo1EditmodulePopdlg)
END_MESSAGE_MAP()


// CEditModulePopDlg 消息处理程序


BOOL CEditModulePopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CProcessModuleClass tempModule = m_pDataprovicer->m_vectProcessModule[m_nSelectedItem];

	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempModule.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//把剩余的可选生产线添加到LineComboBox当中去
	for (int i = 0; i < m_pDataprovicer->m_vectProductionLine.size();i++)
	{
		if (m_pDataprovicer->m_vectProductionLine[i].m_strLineName != tempModule.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataprovicer->m_vectProductionLine[i].m_strLineName);
		}
	}


	m_ModuleNameEdit.SetWindowText(tempModule.m_strProcessModuleName);
	m_DescriptionEdit.SetWindowText(tempModule.m_strDescription);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditModulePopDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	CString LineName, ModuleName, Description;
	m_LineComboBox.GetWindowText(LineName);
	m_ModuleNameEdit.GetWindowText(ModuleName);
	m_DescriptionEdit.GetWindowText(Description);

	m_pDataprovicer->m_vectProcessModule[m_nSelectedItem].m_strProductionLineName = LineName;
	m_pDataprovicer->m_vectProcessModule[m_nSelectedItem].m_ProcessLineId = m_pDataprovicer->FindProLineId(LineName);
	m_pDataprovicer->m_vectProcessModule[m_nSelectedItem].m_strProcessModuleName = ModuleName;
	m_pDataprovicer->m_vectProcessModule[m_nSelectedItem].m_strDescription = Description;


	//更新数据库中的条目//
	int Id = m_pDataprovicer->m_vectProcessModule[m_nSelectedItem].m_Id;
	m_pDataprovicer->UpdateTableItem(CDataProvider::tbProcessModule, Id);
	m_pDataprovicer->UpdateRelatedToModule(Id, ModuleName);

	CDialog::OnOK();
}


void CEditModulePopDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CEditModulePopDlg::OnCbnSelchangeCo1EditmodulePopdlg()
{
	// TODO:  在此添加控件通知处理程序代码


}


void CEditModulePopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
