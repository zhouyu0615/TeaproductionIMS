#pragma once
#include <vector>
#include "DevicePara.h"
#include "DataProvider.h"
//#include "PlcWriteRequest.h"
// CDeviceContrlPopDlg �Ի���

class CDeviceContrlPopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceContrlPopDlg)

public:
	CDeviceContrlPopDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeviceContrlPopDlg();
	std::vector<CDevicePara> m_vDevicePara;             //�����������ݴ浱ǰ��ѡ�е��豸����//

// �Ի�������
	enum { IDD = IDD_DEVICECONTRL_POPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	int m_nIndex;      //��ǰѡ�е��豸����������Դ�豸�����������±�//
	CDataProvider *m_pDataProvider = CDataProvider::getInstance(); //����Դ//
	afx_msg void OnBnClickedBtStart();
	afx_msg void OnBnClickedBtStop();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	int m_nOldState; //֮ǰ�豸��״̬//
};
