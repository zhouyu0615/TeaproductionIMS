#pragma once
#include "afxcmn.h"
#include "DataProvider.h"
#include "PlcWriteRequest.h"

// CAlarmDlg �Ի���

class CAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmDlg)

public:
	CAlarmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarmDlg();

// �Ի�������
	enum { IDD = IDD_ALARMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtClearalarm();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CListCtrl m_listAlarm;
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();      //����Դ//
};
