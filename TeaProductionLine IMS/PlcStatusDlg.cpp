// PlcStatusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "PlcStatusDlg.h"
#include "afxdialogex.h"




// CPlcStatusDlg 对话框

IMPLEMENT_DYNAMIC(CPlcStatusDlg, CDialogEx)

CPlcStatusDlg::CPlcStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlcStatusDlg::IDD, pParent)
{
	m_pDataProvider = CDataProvider::getInstance();
}

CPlcStatusDlg::~CPlcStatusDlg()
{
}

void CPlcStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_PlcStatusList);

}


BEGIN_MESSAGE_MAP(CPlcStatusDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlcStatusDlg 消息处理程序











void CPlcStatusDlg::UpdatePlcState()
{
	
	int length = m_pDataProvider->m_vectPlc.size();
	for (int i = 0; i < length; i++)
	{
	m_PlcStatusList.SetItemText(i, 4, m_pDataProvider->m_vectPlc[i].GetConectedString());
	}
}

void CPlcStatusDlg::OnListPaint()
{
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");
	//设置列表控件风格//
	CRect rect1;
	m_PlcStatusList.GetWindowRect(&rect1);
	m_PlcStatusList.SetExtendedStyle(m_PlcStatusList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE);

	//清空列表//
	m_PlcStatusList.DeleteAllItems();
	CHeaderCtrl *pmyHeaderCtrl = m_PlcStatusList.GetHeaderCtrl();
	int nCount = pmyHeaderCtrl->GetItemCount();
	for (int i = nCount - 1; i >= 0; i--)
		m_PlcStatusList.DeleteColumn(i);

	m_PlcStatusList.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_PlcStatusList.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 8, -1);
	m_PlcStatusList.InsertColumn(2, _T("PLC名"), LVCFMT_CENTER, rect1.Width() / 8 * 2, -1);
	m_PlcStatusList.InsertColumn(3, _T("IP地址"), LVCFMT_CENTER, rect1.Width() / 8 * 3, -1);
	m_PlcStatusList.InsertColumn(4, _T("连接状态"), LVCFMT_CENTER, rect1.Width() / 8 * 2, -1);
	//填写表单内容//
	int length = m_pDataProvider->m_vectPlc.size();
	for (int i = 0; i < length; i++)
	{
		litem.iItem = i;
		CString str;
		str.Format(_T("%d"), i + 1);
		m_PlcStatusList.InsertItem(&litem);
		m_PlcStatusList.SetItemText(i, 1, str);
		m_PlcStatusList.SetItemText(i, 2, m_pDataProvider->m_vectPlc[i].m_strPlcName);
		m_PlcStatusList.SetItemText(i, 3, m_pDataProvider->m_vectPlc[i].m_strIPAddr);
		m_PlcStatusList.SetItemText(i, 4, m_pDataProvider->m_vectPlc[i].GetConectedString());				
	}
}

BOOL CPlcStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化



	SetTimer(1, 1000,NULL); //设置刷新界面的定时器


	OnListPaint();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}



void CPlcStatusDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CPlcStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

  UpdatePlcState();

	
	CDialogEx::OnTimer(nIDEvent);
}
