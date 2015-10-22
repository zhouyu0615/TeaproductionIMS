#pragma once
#include "afxcmn.h"

#include "PlcClass.h"
#include "DataProvider.h"
#include "afxwin.h"


// CPlcStatusDlg �Ի���

class CPlcStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlcStatusDlg)

public:
	CPlcStatusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlcStatusDlg();

// �Ի�������
	enum { IDD = IDD_PLCSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CDataProvider * m_pDataProvider;
	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_PlcStatusList;

	void OnListPaint();
	void UpdatePlcState();


	
	virtual BOOL OnInitDialog();


	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
