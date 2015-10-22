// DeviceContrlPopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "DeviceContrlPopDlg.h"
#include "afxdialogex.h"


// CDeviceContrlPopDlg 对话框

IMPLEMENT_DYNAMIC(CDeviceContrlPopDlg, CDialogEx)

CDeviceContrlPopDlg::CDeviceContrlPopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceContrlPopDlg::IDD, pParent)
{
	m_nIndex = -1;
	m_nOldState = 0;
}

CDeviceContrlPopDlg::~CDeviceContrlPopDlg()
{
}

void CDeviceContrlPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeviceContrlPopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_START, &CDeviceContrlPopDlg::OnBnClickedBtStart)
	ON_BN_CLICKED(IDC_BT_STOP, &CDeviceContrlPopDlg::OnBnClickedBtStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDeviceContrlPopDlg 消息处理程序


BOOL CDeviceContrlPopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_vDevicePara.empty())
	{
		for (int n = 0; n < m_pDataProvider->m_vectDevicePara.size();n++)
			if (m_vDevicePara[0].m_Id==m_pDataProvider->m_vectDevicePara[n].m_Id)
			{
				m_nIndex = n;
				break;
			}
		if (m_nIndex != -1)
		{
			if (m_pDataProvider->m_vectDevicePara[m_nIndex].m_StateValue == 1)
			{
				GetDlgItem(IDC_PIC_GREENLIGHT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_PIC_WHITELIGHT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_ST_STATE)->SetWindowText(_T("正在运行"));
				m_nOldState = 1;
			}
			else
			{
				GetDlgItem(IDC_PIC_GREENLIGHT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_PIC_WHITELIGHT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_ST_STATE)->SetWindowText(_T("停止"));
				m_nOldState = 0;
			}
		}
		GetDlgItem(IDC_ST_DEVICENAME)->SetWindowText(m_vDevicePara[0].m_strDeviceName);
		
	}
	SetTimer(2, 601, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDeviceContrlPopDlg::OnBnClickedBtStart() //启动按钮//
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nIndex != -1 && m_pDataProvider->m_vectDevicePara[m_nIndex].m_StateValue == 0 )
	{
		CPlcWriteRequest TempWriteQueue;
		TempWriteQueue.m_SetValue = 1;
		TempWriteQueue.m_PlcId = m_pDataProvider->m_vectDevicePara[m_nIndex].m_PlcId;
		TempWriteQueue.m_AddrType = CPlcWriteRequest::ADDR_TYPE_BIT;
		TempWriteQueue.m_ByteOffset = m_pDataProvider->m_vectDevicePara[m_nIndex].GetControlAddrBitOffSet();
		TempWriteQueue.m_WriteByteAddrIndex = m_pDataProvider->m_vectDevicePara[m_nIndex].GetControlAddrByteIndex();
		m_pDataProvider->m_wRequestQueue.PushBack(TempWriteQueue);
	}

}


void CDeviceContrlPopDlg::OnBnClickedBtStop()  //停止按钮//
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nIndex != -1 && m_pDataProvider->m_vectDevicePara[m_nIndex].m_StateValue == 1)
	{
		CPlcWriteRequest TempWriteQueue;
		TempWriteQueue.m_SetValue = 0;
		TempWriteQueue.m_PlcId = m_pDataProvider->m_vectDevicePara[m_nIndex].m_PlcId;
		TempWriteQueue.m_AddrType = CPlcWriteRequest::ADDR_TYPE_BIT;
		TempWriteQueue.m_ByteOffset = m_pDataProvider->m_vectDevicePara[m_nIndex].GetControlAddrBitOffSet();
		TempWriteQueue.m_WriteByteAddrIndex = m_pDataProvider->m_vectDevicePara[m_nIndex].GetControlAddrByteIndex();
		m_pDataProvider->m_wRequestQueue.PushBack(TempWriteQueue);
	}
}


void CDeviceContrlPopDlg::OnCancel()   //退出控制台//
{
	// TODO:  在此添加专用代码和/或调用基类
	KillTimer(2);
	CDialogEx::OnCancel();
}


void CDeviceContrlPopDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==2 && m_nIndex!=-1)
	{
		if (m_pDataProvider->m_vectDevicePara[m_nIndex].m_StateValue == 1)
		{
			if (m_nOldState != 1)
			{
				GetDlgItem(IDC_PIC_GREENLIGHT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_PIC_WHITELIGHT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_ST_STATE)->SetWindowText(_T("正在运行"));
				m_nOldState = 1;
			}
		}
		else
			if (m_nOldState!=0)
			{
				m_nOldState = 0;
			    GetDlgItem(IDC_PIC_GREENLIGHT)->ShowWindow(SW_HIDE);
			    GetDlgItem(IDC_PIC_WHITELIGHT)->ShowWindow(SW_SHOW);
			    GetDlgItem(IDC_ST_STATE)->SetWindowText(_T("停止"));
			}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDeviceContrlPopDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
}
