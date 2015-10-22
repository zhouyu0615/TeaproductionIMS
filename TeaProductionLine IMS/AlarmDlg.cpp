// AlarmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "AlarmDlg.h"
#include "afxdialogex.h"


// CAlarmDlg 对话框

IMPLEMENT_DYNAMIC(CAlarmDlg, CDialogEx)

CAlarmDlg::CAlarmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmDlg::IDD, pParent)
{

}

CAlarmDlg::~CAlarmDlg()
{
}

void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTALARM, m_listAlarm);
}


BEGIN_MESSAGE_MAP(CAlarmDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_CLEARALARM, &CAlarmDlg::OnBnClickedBtClearalarm)
END_MESSAGE_MAP()


// CAlarmDlg 消息处理程序


void CAlarmDlg::OnBnClickedBtClearalarm()    //清除警报按钮//
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pDataProvider->m_vectAlarmClass.empty())   //如果没有任何警报//
		return;
	CPlcWriteRequest tempWriteQueue;
	CString strClearTime = CTime::GetCurrentTime().Format("%m//%d %H:%M:%S");
	bool bHaveClear=false;    //是否有清除故障的写入//
	for (int a = 0; a < m_pDataProvider->m_vectPlc.size();a++)
		if (m_pDataProvider->m_vectPlc[a].m_bIsConnected)  //如果这台PLC已连接//
			for (int x = 0; x < m_pDataProvider->m_vectAlarmClass.size(); x++)
				if (m_pDataProvider->m_vectAlarmClass[x].m_nPlcId == m_pDataProvider->m_vectPlc[a].m_Id && m_pDataProvider->m_vectAlarmClass[x].m_nState!=2)
				{
					tempWriteQueue.m_PlcId = m_pDataProvider->m_vectAlarmClass[x].m_nPlcId;
					tempWriteQueue.m_SetValue = 1;
					tempWriteQueue.m_AddrType = CPlcWriteRequest::ADDR_TYPE_BIT;
					tempWriteQueue.m_ByteOffset = 2;
					tempWriteQueue.m_WriteByteAddrIndex = 0;
					m_pDataProvider->m_wRequestQueue.PushBack(tempWriteQueue);
					m_listAlarm.SetItemText(x, 3, _T("已清除"));
					m_pDataProvider->m_vectAlarmClass[x].m_nState = 2;
					m_listAlarm.SetItemText(x, 6, strClearTime);
					m_pDataProvider->m_vectFaultPara[m_pDataProvider->m_vectAlarmClass[x].m_nIndexInErrorParaVect].m_bIsIgnore = false;
					m_pDataProvider->m_vectFaultPara[m_pDataProvider->m_vectAlarmClass[x].m_nIndexInErrorParaVect].m_nIndexInAlarmVect = -1;
					bHaveClear = true;
				}
	if (bHaveClear)
	    AfxMessageBox(_T("故障已清除！"));
}


BOOL CAlarmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectList1;
	m_listAlarm.GetWindowRect(&rectList1);
	m_listAlarm.SetExtendedStyle(m_listAlarm.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE);
	m_listAlarm.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listAlarm.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList1.Width() / 21.5, -1);
	m_listAlarm.InsertColumn(2, _T("报警"), LVCFMT_CENTER, rectList1.Width() / 21.5*5.0, -1);
	m_listAlarm.InsertColumn(3, _T("状态"), LVCFMT_CENTER, rectList1.Width() / 21.5*2.5, -1);
	m_listAlarm.InsertColumn(4, _T("说明"), LVCFMT_CENTER, rectList1.Width() / 21.5*5.0, -1);
	m_listAlarm.InsertColumn(5, _T("报警时间"), LVCFMT_CENTER, rectList1.Width() / 21.5*4.0, -1);
	m_listAlarm.InsertColumn(6, _T("报警清除时间"), LVCFMT_CENTER, rectList1.Width() / 21.5*4.0, -1);
	CString strTemp;
	for (int n = 0; n < m_pDataProvider->m_vectAlarmClass.size(); n++) //填写列表内容//
	{
		strTemp.Format(_T("%d"), n + 1);
		m_listAlarm.InsertItem(n, _T(""));
		m_listAlarm.SetItemText(n, 1, strTemp);
		m_listAlarm.SetItemText(n, 2, m_pDataProvider->m_vectAlarmClass[n].m_strAlarmName);
		switch (m_pDataProvider->m_vectAlarmClass[n].m_nState)
		{
		case 0:
			m_listAlarm.SetItemText(n, 3, _T("未处理"));
			break;
		case 1:
			m_listAlarm.SetItemText(n, 3, _T("忽略"));
			break;
		case 2:
			m_listAlarm.SetItemText(n, 3, _T("已清除"));
			break;
		default:
			break;
		}
		m_listAlarm.SetItemText(n, 4, m_pDataProvider->m_vectAlarmClass[n].m_strNote);
		m_listAlarm.SetItemText(n, 5, m_pDataProvider->m_vectAlarmClass[n].m_strAlarmTime);
		if (m_pDataProvider->m_vectAlarmClass[n].m_strClearTime==_T(""))
			m_listAlarm.SetItemText(n, 6, _T("未清除"));
		else
		    m_listAlarm.SetItemText(n, 6, m_pDataProvider->m_vectAlarmClass[n].m_strClearTime);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAlarmDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
}


void CAlarmDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}