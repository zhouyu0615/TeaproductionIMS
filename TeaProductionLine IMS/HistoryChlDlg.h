#pragma once
#include "mschart1.h"


// CHistoryChlDlg �Ի���

class CHistoryChlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryChlDlg)

public:
	CHistoryChlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistoryChlDlg();

// �Ի�������
	enum { IDD = IDD_HISTORY_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboPara();

	CMschart1 m_Chart;
};
