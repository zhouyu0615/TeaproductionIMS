// DeviceChiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "DeviceChiDlg.h"
#include "afxdialogex.h"



// CDeviceChiDlg �Ի���

IMPLEMENT_DYNAMIC(CDeviceChiDlg, CDialog)

CDeviceChiDlg::CDeviceChiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceChiDlg::IDD, pParent)
{
	m_nCurSelModuleId = 0;//��ʼ����������������ǰѡ��ģ��ID//
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
	m_bHaveEditCreate = false;
}

CDeviceChiDlg::~CDeviceChiDlg()
{
}

void CDeviceChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE_CHIDLG, m_treeDevice);
	DDX_Control(pDX, IDC_LI1_MODULEPARA, m_listModulePara);
	DDX_Control(pDX, IDC_LI2_DEVICEPARA, m_listDevicePara);
	DDX_Control(pDX, IDC_ST_CURPOSITION, m_textCurPosition);
	DDX_Control(pDX, IDC_REALPLAYXCTRL1, m_video);
}


BEGIN_MESSAGE_MAP(CDeviceChiDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEVICE_CHIDLG, &CDeviceChiDlg::OnTvnSelchangedTreeDeviceChidlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LI2_DEVICEPARA, &CDeviceChiDlg::OnNMDblclkLi2Devicepara)
	ON_NOTIFY(NM_DBLCLK, IDC_LI1_MODULEPARA, &CDeviceChiDlg::OnNMDblclkLi1Modulepara)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &CDeviceChiDlg::OnEnKillfocusEditCreateid)
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_DEVICE_CHIDLG, &CDeviceChiDlg::OnTvnSelchangingTreeDeviceChidlg)
END_MESSAGE_MAP()


// CDeviceChiDlg ��Ϣ�������

//�����С�����仯����Ӧ����//
void CDeviceChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect rectList1;                        //�����趨�б���п�//
			m_listModulePara.GetWindowRect(&rectList1);
			m_listModulePara.SetColumnWidth(1, rectList1.Width() / 27.0*2.0);
			m_listModulePara.SetColumnWidth(2, rectList1.Width() / 27.0 *10.85);
			m_listModulePara.SetColumnWidth(3, rectList1.Width() / 27.0 * 4.8);
			m_listModulePara.SetColumnWidth(4, rectList1.Width() / 27.0 * 4.8);
			m_listModulePara.SetColumnWidth(5, rectList1.Width() / 27.0 * 4.5);
			CRect rectList2;
			m_listDevicePara.GetWindowRect(&rectList2);
			m_listDevicePara.SetColumnWidth(1, rectList2.Width() / 7.0*1.16);
			m_listDevicePara.SetColumnWidth(2, rectList2.Width() / 7.0 * 3.90);
			m_listDevicePara.SetColumnWidth(3, rectList2.Width() / 7.0 * 1.88);
			Invalidate();
		}
}


BOOL CDeviceChiDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	m_bHaveInit = true;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HICON hIcon[3];      // ͼ��������//
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICONTREE);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON4);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON3);
	// ����ͼ������CImageList����   
	m_imageList.Create(24, 24, ILC_COLOR32, 3, 3);
	m_imageList.Add(hIcon[0]);
	m_imageList.Add(hIcon[1]);
	m_imageList.Add(hIcon[2]);
	InitTreeContrl();   //��ʼ��������//

	SetTimer(3, 1001, NULL); //���ø��½���Ķ�ʱ��
	
	//��ʼ��2���б��//
	m_bHaveEditCreate = false;            //��ʼ����־λ����ʾ��û�д����༭��//
	CRect rectList1;
	m_listModulePara.GetWindowRect(&rectList1);
	m_listModulePara.SetExtendedStyle(m_listModulePara.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE );
	m_listModulePara.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listModulePara.InsertColumn(1, _T("���"), LVCFMT_CENTER, rectList1.Width() / 27.0*2.0, -1);
	m_listModulePara.InsertColumn(2, _T("���ղ���"), LVCFMT_CENTER, rectList1.Width() / 27.0 *10.9, -1);
	m_listModulePara.InsertColumn(3, _T("��ǰֵ"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.8, -1);
	m_listModulePara.InsertColumn(4, _T("�趨ֵ"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.8, -1);
	m_listModulePara.InsertColumn(5, _T("��λ"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.5, -1);

	CRect rectList2;
	m_listDevicePara.GetWindowRect(&rectList2);
	m_listDevicePara.SetExtendedStyle(m_listDevicePara.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE);
	m_listDevicePara.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listDevicePara.InsertColumn(1, _T("���"), LVCFMT_CENTER, rectList2.Width() / 7.0*1.2, -1);
	m_listDevicePara.InsertColumn(2, _T("�豸"), LVCFMT_CENTER, rectList2.Width() / 7.0 * 3.92, -1);
	m_listDevicePara.InsertColumn(3, _T("��ǰ״̬"), LVCFMT_CENTER, rectList2.Width() / 7.0 * 1.88, -1);


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

//���³�ʼ�����οؼ�//
void CDeviceChiDlg::InitTreeContrl()
{
	m_treeDevice.DeleteAllItems();
	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��//   
	HTREEITEM hFirItem;  // �ɱ�ʾ��һһ���ڵ�ľ��//
	// Ϊ���οؼ�����ͼ������//  
	m_treeDevice.SetImageList(&m_imageList, TVSIL_NORMAL);
	for (int nLine = 0; nLine < m_pDataProvider->m_vectProductionLine.size();nLine++)
	{
		hRoot = m_treeDevice.InsertItem(m_pDataProvider->m_vectProductionLine[nLine].m_strLineName, 0, 0, TVI_ROOT, TVI_LAST);
		m_treeDevice.SetItemData(hRoot, m_pDataProvider->m_vectProductionLine[nLine].m_Id);
		for (int nModule = 0; nModule < m_pDataProvider->m_vectProcessModule.size();nModule++)
		{
			if (m_pDataProvider->m_vectProcessModule[nModule].m_ProcessLineId == m_pDataProvider->m_vectProductionLine[nLine].m_Id)
			{
				hFirItem = m_treeDevice.InsertItem(m_pDataProvider->m_vectProcessModule[nModule].m_strProcessModuleName, 1, 2, hRoot, TVI_LAST);
				m_treeDevice.SetItemData(hFirItem, m_pDataProvider->m_vectProcessModule[nModule].m_Id);
			}
		}
	}
}

//�����οؼ�ѡ����Ŀ�����仯ʱ����Ӧ����//
void CDeviceChiDlg::OnTvnSelchangedTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//ɾ�������б��ؼ�������//
	m_listDevicePara.DeleteAllItems();
	m_listModulePara.DeleteAllItems();

	m_video.StopPlay();   //��Ƶ����ֹͣ//

	HTREEITEM hCurSelItem = m_treeDevice.GetSelectedItem();
	UINT TempID = m_treeDevice.GetItemData(hCurSelItem);
	HTREEITEM hParentItem = m_treeDevice.GetParentItem(hCurSelItem);
	CString strCurPosition;
	if (hParentItem!=NULL)                              //���ѡ�е��ǹ���ģ��ڵ�//
	{
		m_nCurSelModuleId = TempID;                                                                 //��ǰѡ��ģ���ID//
		CString strLineName = m_treeDevice.GetItemText(hParentItem);    //��������������//
		strCurPosition = _T("��ǰλ�ã� ") + strLineName + _T(" >> ") + m_treeDevice.GetItemText(hCurSelItem)+_T(" >> ");  //λ��������//
		m_textCurPosition.SetWindowText(strCurPosition);  
		
		SearchFromDataProvider(m_nCurSelModuleId);    //����ǰѡ��ģ���µ�����ͷ�����ղ������豸�����浽��ʱ����//
		WriteToDeviceList();                          //��д�����б��//
		WriteToModuleList();
		if (!m_vectTempVideo.empty())                 //�����ģ������Ƶ��أ�����ʾ//
			AfxBeginThread(ThreadFunc, this);
	}
	else                                             //���ѡ�е��������߽ڵ�//
	{
		strCurPosition = _T("��ǰλ�ã� ") + m_treeDevice.GetItemText(hCurSelItem) + _T(" >> ");
		m_textCurPosition.SetWindowText(strCurPosition);
		m_listModulePara.DeleteAllItems();
		m_listDevicePara.DeleteAllItems();
		m_vectTempDevicePara.clear();
		m_vectTempModulePara.clear();
		m_vectTempVideo.clear();
	}

}

//����ǰѡ��ģ���µ�����ͷ�����ղ������豸�����浽��ʱ����//
void CDeviceChiDlg::SearchFromDataProvider(UINT nCerSelModuleId)
{
	m_vectTempDevicePara.clear();
	m_vectTempModulePara.clear();
	m_vectTempVideo.clear();
	for (int n = 0; n < m_pDataProvider->m_vectDevicePara.size();n++)  //�豸����//
	{
		if (m_pDataProvider->m_vectDevicePara[n].m_ProcessModuleId==nCerSelModuleId)
		{
			m_vectTempDevicePara.push_back(m_pDataProvider->m_vectDevicePara[n]);
		}
	}
	for (int i = 0; i < m_pDataProvider->m_vectVideo.size();i++)      //����ͷ//
    {
		if (m_pDataProvider->m_vectVideo[i].m_ModuleId==nCerSelModuleId)
		{
			m_vectTempVideo.push_back(m_pDataProvider->m_vectVideo[i]);
			break;
		}
    }
	for (int j = 0; j < m_pDataProvider->m_vectProModulePara.size();j++)    //���ղ���//
	{
		if (m_pDataProvider->m_vectProModulePara[j].m_ProcessModuleId==nCerSelModuleId && m_pDataProvider->m_vectProModulePara[j].m_IsVisible)
		{
			m_vectTempModulePara.push_back(m_pDataProvider->m_vectProModulePara[j]);
		}
	}
}

//��д���ղ����б��//
void CDeviceChiDlg::WriteToModuleList()
{
	m_listModulePara.DeleteAllItems();
	CString strItem;                                               //ģ���µĹ��ղ�����ʾ��list1//
	for (int n = 0; n < m_vectTempModulePara.size(); n++)
	{
		strItem.Format(_T("%d"), n + 1);
		m_listModulePara.InsertItem(n, _T(""));
		m_listModulePara.SetItemText(n, 1, strItem);
		m_listModulePara.SetItemText(n, 2, m_vectTempModulePara[n].m_strParaName);
		m_listModulePara.SetItemText(n, 5, m_vectTempModulePara[n].m_strUnit);
	}  
}   

//��д�豸�����б��//
void CDeviceChiDlg::WriteToDeviceList()                          
{
	m_listDevicePara.DeleteAllItems();
	CString strItem;
	for (int n = 0; n < m_vectTempDevicePara.size(); n++)
	{
		m_listDevicePara.InsertItem(n, _T(""));
		strItem.Format(_T("%d"), n + 1);
		m_listDevicePara.SetItemText(n, 1, strItem);
		m_listDevicePara.SetItemText(n, 2, m_vectTempDevicePara[n].m_strDeviceName);
	}
}

void CDeviceChiDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	//CRect rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect, RGB(0, 0, 250));   //����Ϊ��ɫ����
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}


HBRUSH CDeviceChiDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//˫���豸�����б����Ӧ����//
void CDeviceChiDlg::OnNMDblclkLi2Devicepara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_nDClkLineItem = pNMItemActivate->iItem;
	if (m_nDClkLineItem == -1)
		return;
	if (!IsPlcConnected(m_vectTempDevicePara[m_nDClkLineItem].m_PlcId))
	{
		AfxMessageBox(_T("���������豸��"));
		return;
	}
	CDeviceContrlPopDlg DeviceContrlPopDlg;          //�豸����̨��������//
	DeviceContrlPopDlg.m_vDevicePara.push_back(m_vectTempDevicePara[m_nDClkLineItem]);        //��ѡ���豸����������������//
	DeviceContrlPopDlg.DoModal();

	*pResult = 0;
}

//˫�����ղ����б����Ӧ����//
void CDeviceChiDlg::OnNMDblclkLi1Modulepara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������//
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pEditCtrl->iItem == -1)                                                //������ǹ�����//
	{
		if (m_bHaveEditCreate == true)                                            //���֮ǰ�����˱༭������ٵ�//
		{
			distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);        //���ٵ�Ԫ��༭�����//
			m_bHaveEditCreate = false;
		}
	}
	else
	{
		if (m_bHaveEditCreate == true)
		{
			if (!(m_OldItem == pEditCtrl->iItem && m_OldSubItem == pEditCtrl->iSubItem))    //������еĵ�Ԫ����֮ǰ�����õ�//
			{
				distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);
				m_bHaveEditCreate = false;
				createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);      //�����༭��//
			}
			else                                                                       //���еĵ�Ԫ����֮ǰ�����õ�//
			{
				m_Edit.SetFocus();                                                     //����Ϊ����//
			}
		}
		else
		{
			if (!IsPlcConnected(m_vectTempModulePara[pEditCtrl->iItem].m_PlcId))      //����ò�������PLCδ����//
			{
				AfxMessageBox(_T("���������豸��"));
				return;
			}
			m_OldItem = pEditCtrl->iItem;                                                 //�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���//
			m_OldSubItem = pEditCtrl->iSubItem;                                           //�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���//
			createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);         //�����༭��//
		}
	}
	*pResult = 0;
}

//������Ԫ��༭����//
void CDeviceChiDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)
//pEditCtrlΪ�б����ָ�룬createditΪ�༭��ָ�����//
//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼//
{
	Item = pEditCtrl->iItem;                     //�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���//
	SubItem = pEditCtrl->iSubItem;               //�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���//
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_CENTER | ES_WANTRETURN,CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);  //�����༭�����,IDC_EDIT_CREATEIDΪ�ؼ�ID��//
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);                   //��������//
	createdit->SetParent(&m_listModulePara);                      //��listcontrol����Ϊ������//
	CRect  EditRect;
	m_listModulePara.GetSubItemRect(m_OldItem, 4, LVIR_LABEL, EditRect);      //��ȡ��Ԫ��Ŀռ�λ����Ϣ//
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_listModulePara.GetColumnWidth(4) - 1, EditRect.bottom - 1);//+1��-1�ñ༭�����ڵ�ס�б���е�������
	CString strItem = m_listModulePara.GetItemText(m_OldItem, 4);             //��õ�ǰ�趨ֵ//
	createdit->SetWindowText(strItem);                                     //����Ԫ���ַ���ʾ�ڱ༭����//
	createdit->MoveWindow(&EditRect);                                      //���༭��λ�÷�����Ӧ��Ԫ����//
	createdit->ShowWindow(SW_SHOW);                                        //��ʾ�༭���ڵ�Ԫ������//
	createdit->SetFocus();                                                 //����Ϊ����//
	createdit->SetSel(-1);                                                 //���ù�����ı������ֵ����//
}

void CDeviceChiDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString strEditData;
	distroyedit->GetWindowText(strEditData);
	strEditData = strEditData.Trim();
	if (strEditData.IsEmpty())             //�������Ϊ�գ������ٱ༭��//
	{
		AfxMessageBox(_T("�Ƿ����������벻��Ϊ�գ�"));
		distroyedit->DestroyWindow();
		return;
	}
	if (m_vectTempModulePara[Item].m_ValueType == CProcessPara::VALUETYPE_BOOL && (strEditData != _T("0") && strEditData != _T("1")))
	{
		AfxMessageBox(_T("�Ƿ��������ò���ֵֻ��Ϊ��0����1����"));
		distroyedit->DestroyWindow();                          //���ٶ���//
		return;
	}
	if (strEditData == strEditData.SpanIncluding(_T("0123456789.")))
	{
		if(IDYES==MessageBox(_T("�Ƿ񱣴���޸ģ�"), _T("��ʾ"), MB_ICONQUESTION | MB_YESNO))
		{
			//list->SetItemText(Item, 4, strEditData);      //���޸�д���б��//
			//����ӣ����޸�д�����//
			CPlcWriteRequest tempRequest;
			switch (m_vectTempModulePara[Item].m_ValueType)   //�����������͸�ֵ//
			{
			case 0:
				if (strEditData==_T("0"))
					tempRequest.m_SetValue = 0;
				else
					tempRequest.m_SetValue = 1;
				break;
			case 1:
			case 2:
			case 3:
				tempRequest.m_SetValue = (float)_wtof(strEditData);
				break;
			case 4:
			case 5:
				tempRequest.m_fSetValue = (float)_wtof(strEditData);
				break;
			default:
				break;
			}
			tempRequest.m_PlcId = m_vectTempModulePara[Item].m_PlcId;
			tempRequest.m_AddrType = m_vectTempModulePara[Item].GetAddrTypeInEmType();
			tempRequest.m_ByteOffset = m_vectTempModulePara[Item].GetWriteBitOffSet();
			tempRequest.m_WriteByteAddrIndex = m_vectTempModulePara[Item].GetWriteAddrByteIndex();
			m_pDataProvider->m_wRequestQueue.PushBack(tempRequest);
		}
	}             
	else
		AfxMessageBox(_T("�Ƿ����������������֣�"));
	distroyedit->DestroyWindow();                          //���ٶ���//
}

//�����麯��//
void CDeviceChiDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	m_listModulePara.SetFocus();
}

//�༭��ʧȥ����//
void CDeviceChiDlg::OnEnKillfocusEditCreateid()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bHaveEditCreate == true)                                               //���֮ǰ�����˱༭������ٵ�//
	{
		distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);           //���ٵ�Ԫ��༭�����//
		m_bHaveEditCreate = false;
	}
}

void CDeviceChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
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

void CDeviceChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
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

UINT CDeviceChiDlg::ThreadFunc(LPVOID lpParam)
{
	((CDeviceChiDlg*)lpParam)->m_video.SetDeviceInfo(((CDeviceChiDlg*)lpParam)->m_vectTempVideo[0].m_strIPAddr, ((CDeviceChiDlg*)lpParam)->m_vectTempVideo[0].m_port, 0, _T("admin"), _T("admin"));
	((CDeviceChiDlg*)lpParam)->m_video.StartPlay();
	return 1;
}

void CDeviceChiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (nIDEvent == 3)
	{
		if (!m_vectTempModulePara.empty())
		    UpdateModuleParaValueShow();            //���¹��ղ����б��//
		if (!m_vectTempDevicePara.empty())
			UpdateDeviceParaValueShow();           //�����豸�����б��//
	}  

	CDialog::OnTimer(nIDEvent);
}


void CDeviceChiDlg::UpdateModuleParaValueShow() //���¹��ղ����ĵ�ǰֵ���趨ֵ//
{
	//m_listModulePara.SetRedraw(FALSE);
	CString strTempSetValue, strTempStateValue,strCurSetValue,strCurStateValue;
	int nTempSetValue, nTempStateValue;
	for (int n = 0; n < m_vectTempModulePara.size(); n++)
	{
		for (int i = 0; i < m_pDataProvider->m_vectProModulePara.size(); i++)
		{
			if (m_vectTempModulePara[n].m_Id == m_pDataProvider->m_vectProModulePara[i].m_Id)
			{
				strCurSetValue = m_listModulePara.GetItemText(n, 4);
				strCurStateValue = m_listModulePara.GetItemText(n, 3);
				if (IsPlcConnected(m_pDataProvider->m_vectProModulePara[i].m_PlcId))   //�ж�PLC�Ƿ�����//
                {
					switch (m_pDataProvider->m_vectProModulePara[i].m_ValueType)     //����ֵ������д�б��//
					{
					case 0:
						if (m_pDataProvider->m_vectProModulePara[i].m_ParaValue == 0)
						{
							if (strCurStateValue != _T("0"))
								m_listModulePara.SetItemText(n, 3, _T("0"));
						}
						else
							if (strCurStateValue != _T("1"))
							    m_listModulePara.SetItemText(n, 3, _T("1"));

						if (m_pDataProvider->m_vectProModulePara[i].m_fSetValue == 0)
						{
							if (strCurSetValue != _T("0"))
								m_listModulePara.SetItemText(n, 4, _T("0"));
						}
						else
							if (strCurSetValue != _T("1"))
							    m_listModulePara.SetItemText(n, 4, _T("1"));

						break;
					case 1:
					case 2:
					case 3:
						nTempSetValue = m_pDataProvider->m_vectProModulePara[i].m_fSetValue;
						nTempStateValue = m_pDataProvider->m_vectProModulePara[i].m_ParaValue;
						strTempSetValue.Format(_T("%d"), nTempSetValue);
						strTempStateValue.Format(_T("%d"), nTempStateValue);
						if (strCurStateValue!=strTempStateValue)
						    m_listModulePara.SetItemText(n, 3, strTempStateValue);
						if (strCurSetValue!=strTempSetValue)
						    m_listModulePara.SetItemText(n, 4, strTempSetValue);
						break;
					case 4:
					case 5:
						strTempSetValue.Format(_T("%.2f"), m_pDataProvider->m_vectProModulePara[i].m_fSetValue);
						strTempStateValue.Format(_T("%.2f"), m_pDataProvider->m_vectProModulePara[i].m_ParaValue);
						if (strCurStateValue != strTempStateValue)
						    m_listModulePara.SetItemText(n, 3, strTempStateValue);
						if (strCurSetValue != strTempSetValue)
						    m_listModulePara.SetItemText(n, 4, strTempSetValue);
						break;
					default:
						break;
					}
                }
				else
				{
					if (strCurStateValue != _T("δ����"))
					    m_listModulePara.SetItemText(n, 3, _T("δ����"));
					if (strCurSetValue != _T("δ����"))
					    m_listModulePara.SetItemText(n, 4, _T("δ����"));
				}
				break;
			}
		}
	}
	//m_listModulePara.SetRedraw(TRUE);
//	m_listModulePara.UpdateWindow();
}

bool CDeviceChiDlg::IsPlcConnected(int nPlcId)    //�ж�PLC�Ƿ�����//
{
	for (int n = 0; n < m_pDataProvider->m_vectPlc.size();n++)
		if (m_pDataProvider->m_vectPlc[n].m_Id==nPlcId)
			if (m_pDataProvider->m_vectPlc[n].m_bIsConnected)
				return true;
		    else
				return false;
	return false;
}

void CDeviceChiDlg::UpdateDeviceParaValueShow()    //�����豸��ͣ״̬//
{
	//m_listDevicePara.SetRedraw(FALSE);
	CString strCurValue;
	for (int n = 0; n < m_vectTempDevicePara.size();n++)
		for (int i = 0; i < m_pDataProvider->m_vectDevicePara.size();i++)
			if (m_vectTempDevicePara[n].m_Id == m_pDataProvider->m_vectDevicePara[i].m_Id)
			{
				strCurValue = m_listDevicePara.GetItemText(n, 3);
				if (IsPlcConnected(m_vectTempDevicePara[n].m_PlcId))          //�жϸ��豸�Ƿ�����//
				{
					if (m_pDataProvider->m_vectDevicePara[i].m_StateValue)
					{
						if (strCurValue != _T("��������"))
							m_listDevicePara.SetItemText(n, 3, _T("��������"));
					}
					else
					{
						if (strCurValue != _T("ֹͣ"))
							m_listDevicePara.SetItemText(n, 3, _T("ֹͣ"));
					}
				}
				else
					if (strCurValue != _T("δ����"))
					    m_listDevicePara.SetItemText(n, 3, _T("δ����"));
				break;
			}
	//m_listDevicePara.SetRedraw(TRUE);
	//m_listDevicePara.UpdateWindow();
}

//������ѡ����Ŀ���������仯//
void CDeviceChiDlg::OnTvnSelchangingTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bHaveEditCreate == true)                                               //���֮ǰ�����˱༭������ٵ�//
	{
		distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);           //���ٵ�Ԫ��༭�����//
		m_bHaveEditCreate = false;
	}
	*pResult = 0;
}
