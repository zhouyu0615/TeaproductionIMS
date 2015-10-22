// ConfigChiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "ConfigChiDlg.h"
#include "afxdialogex.h"


// CConfigChiDlg �Ի���

IMPLEMENT_DYNAMIC(CConfigChiDlg, CDialogEx)

CConfigChiDlg::CConfigChiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigChiDlg::IDD, pParent)
{
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
}

CConfigChiDlg::~CConfigChiDlg()
{
}

void CConfigChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabConfig);
}


BEGIN_MESSAGE_MAP(CConfigChiDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CConfigChiDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CConfigChiDlg ��Ϣ�������


BOOL CConfigChiDlg::OnInitDialog()
{
	m_bHaveInit = true;
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ����ǩ�ؼ�//
	m_tabConfig.InsertItem(0, _T("�����䷽"));
	m_tabConfig.InsertItem(1, _T("�½��䷽"));
	m_OldConfigGraChiDlg.Create(IDD_OLDCONFIG_GRACHIDLG, &m_tabConfig);
	m_NewConfigGraChiDlg.Create(IDD_NEWCONFIG_GRACHIDLG, &m_tabConfig);
	CRect tabRect;                                                       // ��ǩ�ؼ��ͻ�����λ�úʹ�С//	
	m_tabConfig.GetClientRect(&tabRect);                                 // ��ȡ��ǩ�ؼ��ͻ���Rect //
	tabRect.left += 1;                                                   // ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ //  
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	m_OldConfigGraChiDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);// ���ݵ����õ�tabRect�����ӶԻ���//
	m_NewConfigGraChiDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	CRect rectTemp;                //��סԭʼ�ߴ�//
	if (!m_bHaveRemenberOldSize)
	{
		GetClientRect(&rectTemp);
		m_OldSize.x = rectTemp.right - rectTemp.left;
		m_OldSize.y = rectTemp.bottom - rectTemp.top;
		RememberOldSize(m_vectOldSize);
		m_bHaveRemenberOldSize = true;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�л���ǩ����//
void CConfigChiDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	switch (m_tabConfig.GetCurSel())  //����ѡ��ı�ǩ����ʾ��Ӧ�Ի���//
	{
	case 0:
		if (m_NewConfigGraChiDlg.m_bIsAddConfig==true)
		{
			m_OldConfigGraChiDlg.ReInitDlg();
			m_NewConfigGraChiDlg.m_bIsAddConfig = false;
		}
		m_NewConfigGraChiDlg.ShowWindow(SW_HIDE);
		m_OldConfigGraChiDlg.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_NewConfigGraChiDlg.ShowWindow(SW_SHOW);
		m_OldConfigGraChiDlg.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	*pResult = 0;
}


void CConfigChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect tabRect;
			m_tabConfig.GetClientRect(&tabRect);
			tabRect.left += 1;
			tabRect.right -= 1;
			tabRect.top += 25;
			tabRect.bottom -= 1;
			m_OldConfigGraChiDlg.MoveWindow(tabRect);
			m_NewConfigGraChiDlg.MoveWindow(tabRect);
		}
}

void CConfigChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
{
	vectOldSize.clear();
	CRect Rect;
	int woc;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�//    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //ȡ��ID//  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		vectOldSize.push_back(Rect);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void CConfigChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
{
	CRect Rect;
	int woc;
	CPoint  BRPoint, TLPoint;
	double fsp[2];                                    //���ű���//
	POINT recNewSize;                                 //��ȡ���ڶԻ���Ĵ�С//  
	CRect TempRect;
	GetClientRect(&TempRect);                         //ȡ�ͻ�����С//    
	recNewSize.x = TempRect.right - TempRect.left;
	recNewSize.y = TempRect.bottom - TempRect.top;
	fsp[0] = (double)recNewSize.x / OldSizeOfClient.x;
	fsp[1] = (double)recNewSize.y / OldSizeOfClient.y;

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�//    
	for (int n = 0; n < vectOldSize.size(); n++)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //ȡ��ID//  
		TLPoint.x = double(vectOldSize[n].left*fsp[0]);
		TLPoint.y = double(vectOldSize[n].top*fsp[1]);
		BRPoint.x = double(vectOldSize[n].right *fsp[0]);
		BRPoint.y = double(vectOldSize[n].bottom*fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}