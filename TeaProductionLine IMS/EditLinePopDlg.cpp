// EditLinePopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditLinePopDlg.h"
#include "afxdialogex.h"


// CEditLinePopDlg 对话框

IMPLEMENT_DYNAMIC(CEditLinePopDlg, CDialogEx)

CEditLinePopDlg::CEditLinePopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditLinePopDlg::IDD, pParent)
{

}

CEditLinePopDlg::~CEditLinePopDlg()
{
}

void CEditLinePopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED1_EDITLINE_POPDLG, m_editLineName);
	DDX_Control(pDX, IDC_ED2_EDITLINE_POPDLG, m_editCapacity);
	DDX_Control(pDX, IDC_ED3_EDITLINE_POPDLG, m_editDescription);
}


BEGIN_MESSAGE_MAP(CEditLinePopDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditLinePopDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditLinePopDlg 消息处理程序 


void CEditLinePopDlg::OnBnClickedOk()
{
	CString strLineName, strCapacity, strDescription;
	m_editLineName.GetWindowText(strLineName);
	m_editCapacity.GetWindowText(strCapacity);
	m_editDescription.GetWindowText(strDescription);

	m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strLineName = strLineName;
	m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strCapacity = strCapacity;
	m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strDescription = strDescription;
	int Id = m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_Id;
	m_pDataProvider->UpdateTableItem(CDataProvider::tbProductionLine,Id);
	m_pDataProvider->UpdateRelatedToLine(Id, strLineName);

	CDialogEx::OnOK();
}


BOOL CEditLinePopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_editLineName.SetWindowText(m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strLineName);
	m_editCapacity.SetWindowText(m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strCapacity);
	m_editDescription.SetWindowText(m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strDescription);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditLinePopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
