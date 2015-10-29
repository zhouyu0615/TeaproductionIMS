// HistoryChlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "HistoryChlDlg.h"
#include "afxdialogex.h"


// CHistoryChlDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryChlDlg, CDialogEx)

CHistoryChlDlg::CHistoryChlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryChlDlg::IDD, pParent)
	, m_Chart(0)
{

}

CHistoryChlDlg::~CHistoryChlDlg()
{
}

void CHistoryChlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART1, m_Chart);
}


BEGIN_MESSAGE_MAP(CHistoryChlDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA, &CHistoryChlDlg::OnCbnSelchangeComboPara)
END_MESSAGE_MAP()


// CHistoryChlDlg 消息处理程序


void CHistoryChlDlg::OnCbnSelchangeComboPara()
{
	// TODO:  在此添加控件通知处理程序代码
}
