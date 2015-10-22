#pragma once
#include <vector>
#include "DevicePara.h"
#include "DataProvider.h"
//#include "PlcWriteRequest.h"
// CDeviceContrlPopDlg 对话框

class CDeviceContrlPopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceContrlPopDlg)

public:
	CDeviceContrlPopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceContrlPopDlg();
	std::vector<CDevicePara> m_vDevicePara;             //容器，用于暂存当前被选中的设备参数//

// 对话框数据
	enum { IDD = IDD_DEVICECONTRL_POPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	int m_nIndex;      //当前选中的设备参数在数据源设备参数容器的下标//
	CDataProvider *m_pDataProvider = CDataProvider::getInstance(); //数据源//
	afx_msg void OnBnClickedBtStart();
	afx_msg void OnBnClickedBtStop();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	int m_nOldState; //之前设备的状态//
};
