#pragma once
#include "realplayxctrl1.h"
#include "DataProvider.h"

// CFullScreenDlg �Ի���

class CFullScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenDlg)

public:
	CFullScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFullScreenDlg();

// �Ի�������
	enum { IDD = IDD_FULLSCREENDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CRealplayxctrl1 m_video;
	int m_nItem; //ȫ����ʾ����ͷ������Դ������λ��//
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void DblClickRealplayxctrl20();
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();    //����Դ//
};
