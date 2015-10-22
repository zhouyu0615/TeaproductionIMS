// FullScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "FullScreenDlg.h"
#include "afxdialogex.h"


// CFullScreenDlg �Ի���

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


// CFullScreenDlg ��Ϣ�������


BOOL CFullScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//ModifyStyle(WS_CAPTION, 0, 0);
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);//ȫ����ʾ//
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}
BEGIN_EVENTSINK_MAP(CFullScreenDlg, CDialogEx)
	ON_EVENT(CFullScreenDlg, IDC_REALPLAYXCTRL20, DISPID_DBLCLICK, CFullScreenDlg::DblClickRealplayxctrl20, VTS_NONE)
END_EVENTSINK_MAP()


void CFullScreenDlg::DblClickRealplayxctrl20()//˫���˳�ȫ��//
{
	// TODO:  �ڴ˴������Ϣ����������
	m_video.StopPlay();
	OnCancel();
}
