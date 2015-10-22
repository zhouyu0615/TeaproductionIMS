// EditVedioPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditVedioPopDlg.h"
#include "afxdialogex.h"
#include "VideoClass.h"


// CEditVedioPopDlg 对话框

IMPLEMENT_DYNAMIC(CEditVedioPopDlg, CDialog)

CEditVedioPopDlg::CEditVedioPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditVedioPopDlg::IDD, pParent)
{

}

CEditVedioPopDlg::~CEditVedioPopDlg()
{
}

void CEditVedioPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CO1_EDITVEDIO_POPDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO2_EDITVEDIO_POPDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_ED1_EDITVEDIO_POPDLG, m_VideoNameEdit);
	DDX_Control(pDX, IDC_ED2_EDITVEDIO_POPDLG, m_IPAddrEdit);
	DDX_Control(pDX, IDC_ED3_EDITVEDIO_POPDLG, m_PortEdit);
}


BEGIN_MESSAGE_MAP(CEditVedioPopDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CO1_EDITVEDIO_POPDLG, &CEditVedioPopDlg::OnCbnSelchangeCo1EditvedioPopdlg)
	ON_BN_CLICKED(IDOK, &CEditVedioPopDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditVedioPopDlg 消息处理程序


void CEditVedioPopDlg::OnCbnSelchangeCo1EditvedioPopdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ModuleComboBox.ResetContent();

	CString LineName;
	m_LineComboBox.GetWindowText(LineName);

	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == LineName)
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}
	m_ModuleComboBox.SetCurSel(0);

}


void CEditVedioPopDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_LineComboBox.GetWindowText(m_pDataProvider->m_vectVideo[m_nSelectedItem].m_strProductionLineName);
	m_ModuleComboBox.GetWindowText(m_pDataProvider->m_vectVideo[m_nSelectedItem].m_strProcessModuleName);

	m_VideoNameEdit.GetWindowText(m_pDataProvider->m_vectVideo[m_nSelectedItem].m_strVideoName);
	CString str;
	m_PortEdit.GetWindowText(str);

	m_IPAddrEdit.GetWindowText(m_pDataProvider->m_vectVideo[m_nSelectedItem].m_strIPAddr);

	m_pDataProvider->m_vectVideo[m_nSelectedItem].m_port = _ttoi(str);

	m_pDataProvider->UpdateTableItem(CDataProvider::tbVideo, m_pDataProvider->m_vectVideo[m_nSelectedItem].m_Id);

	CDialog::OnOK();
}


BOOL CEditVedioPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CVideoClass tempVideo = m_pDataProvider->m_vectVideo[m_nSelectedItem];
	m_LineComboBox.ResetContent();
	m_LineComboBox.AddString(tempVideo.m_strProductionLineName);
	m_LineComboBox.SetCurSel(0);
	//把剩余的生产线添加到LineComboBox中
	for (int i = 0; i < m_pDataProvider->m_vectProductionLine.size(); i++)
	{
		if (m_pDataProvider->m_vectProductionLine[i].m_strLineName != tempVideo.m_strProductionLineName)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[i].m_strLineName);
		}
	}

	m_ModuleComboBox.ResetContent();
	m_ModuleComboBox.AddString(tempVideo.m_strProcessModuleName);
	m_ModuleComboBox.SetCurSel(0);
	//把选中的生产线下的可供选择的模块加入到ModuleComboBox当中去
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
	{
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == tempVideo.m_strProductionLineName && (m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName != tempVideo.m_strProcessModuleName))
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
		}
	}


	m_VideoNameEdit.SetWindowText(tempVideo.m_strVideoName);
	m_IPAddrEdit.SetWindowText(tempVideo.m_strIPAddr);
	CString str;
	str.Format(_T("%d"), tempVideo.m_port);
	m_PortEdit.SetWindowText(str);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditVedioPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
