// FullScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "FullScreenDlg.h"
#include "afxdialogex.h"


// CFullScreenDlg 对话框

IMPLEMENT_DYNAMIC(CFullScreenDlg, CDialogEx)

CFullScreenDlg::CFullScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFullScreenDlg::IDD, pParent)
{
	m_nItem = -1;
}

CFullScreenDlg::~CFullScreenDlg()
{
}

void CFullScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REALPLAYXCTRL20, m_video);
}


BEGIN_MESSAGE_MAP(CFullScreenDlg, CDialogEx)
END_MESSAGE_MAP()


// CFullScreenDlg 消息处理程序


BOOL CFullScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//ModifyStyle(WS_CAPTION, 0, 0);
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);//全屏显示//
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL, 0, 0, x, y, SWP_SHOWWINDOW);
	CRect rect;
	GetClientRect(&rect);
	m_video.MoveWindow(rect);
	if (m_nItem>=0)
	{
		m_video.SetDeviceInfo(m_pDataProvider->m_vectVideo[m_nItem].m_strIPAddr, m_pDataProvider->m_vectVideo[m_nItem].m_port, 0, _T("admin"), _T("admin"));
		m_video.StartPlay();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CFullScreenDlg, CDialogEx)
	ON_EVENT(CFullScreenDlg, IDC_REALPLAYXCTRL20, DISPID_DBLCLICK, CFullScreenDlg::DblClickRealplayxctrl20, VTS_NONE)
END_EVENTSINK_MAP()


void CFullScreenDlg::DblClickRealplayxctrl20()//双击退出全屏//
{
	// TODO:  在此处添加消息处理程序代码
	m_video.StopPlay();
	OnCancel();
}
