// EditPlcPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "EditPlcPopDlg.h"
#include "afxdialogex.h"
#include "PlcClass.h"


// CEditPlcPopDlg 对话框

IMPLEMENT_DYNAMIC(CEditPlcPopDlg, CDialog)

CEditPlcPopDlg::CEditPlcPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPlcPopDlg::IDD, pParent)
{

}

CEditPlcPopDlg::~CEditPlcPopDlg()
{
}

void CEditPlcPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED1_EDITPLC_POPDLG, m_PlcNameEdit);
	DDX_Control(pDX, IDC_ED2_EDITPLC_POPDLG, m_IPAddrEdit);
	DDX_Control(pDX, IDC_ED3_EDITPLC_POPDLG, m_ReadAddrEdit);
	DDX_Control(pDX, IDC_ED4_EDITPLC_POPDLG, m_ReadLengthEdit);
	//DDX_Control(pDX, IDC_ED5_EDITPLC_POPDLG, m_WriteAddrEdit);
	//DDX_Control(pDX, IDC_ED6_EDITPLC_POPDLG, m_WriteLengthEdit);
}


BEGIN_MESSAGE_MAP(CEditPlcPopDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditPlcPopDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditPlcPopDlg 消息处理程序


void CEditPlcPopDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString tempStr;
	
	m_PlcNameEdit.GetWindowText(m_pDataProvider->m_vectPlc[m_nSelectedItem].m_strPlcName);
	m_IPAddrEdit.GetWindowText(m_pDataProvider->m_vectPlc[m_nSelectedItem].m_strIPAddr);

	m_ReadAddrEdit.GetWindowText(tempStr);
	m_pDataProvider->m_vectPlc[m_nSelectedItem].m_ReadStartAddr = _ttoi(tempStr);

	m_ReadLengthEdit.GetWindowText(tempStr);
	m_pDataProvider->m_vectPlc[m_nSelectedItem].m_ReadLength = _ttoi(tempStr);

	//m_WriteAddrEdit.GetWindowText(tempStr);
	//m_pDataProvider->m_vectPlc[m_nSelectedItem].m_WriteStartAddr = _ttoi(tempStr);
	//
	//m_WriteLengthEdit.GetWindowText(tempStr);
	//m_pDataProvider->m_vectPlc[m_nSelectedItem].m_WriteLength = _ttoi(tempStr);

	int PlcId = m_pDataProvider->m_vectPlc[m_nSelectedItem].m_Id;
	m_pDataProvider->UpdateTableItem(CDataProvider::tbPLc,PlcId );

	m_pDataProvider->UpdateRelatedToPlc(PlcId, m_pDataProvider->m_vectPlc[m_nSelectedItem].m_strPlcName);

	CDialog::OnOK();
}


BOOL CEditPlcPopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化//
	CPlcClass tempPlc;
	CString strtemp;
	tempPlc = m_pDataProvider->m_vectPlc[m_nSelectedItem];

	m_PlcNameEdit.SetWindowText(tempPlc.m_strPlcName);
	m_IPAddrEdit.SetWindowText(tempPlc.m_strIPAddr);
	strtemp.Format(_T("%d"), tempPlc.m_ReadStartAddr);
	m_ReadAddrEdit.SetWindowText(strtemp);
	strtemp.Format(_T("%d"), tempPlc.m_ReadLength);
	m_ReadLengthEdit.SetWindowText(strtemp);

	//strtemp.Format(_T("%d"), tempPlc.m_WriteStartAddr);
	//m_WriteAddrEdit.SetWindowText(strtemp);
	//strtemp.Format(_T("%d"), tempPlc.m_WriteLength);
	//m_WriteLengthEdit.SetWindowText(strtemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditPlcPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
