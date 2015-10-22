#pragma once
#include "realplayxctrl1.h"
#include "DataProvider.h"
#include <vector>
#include "FullScreenDlg.h"
// CVedioChiDlg �Ի���


class CVedioChiDlg : public CDialog
{
	DECLARE_DYNAMIC(CVedioChiDlg)

public:
	CVedioChiDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVedioChiDlg();

// �Ի�������
	enum { IDD = IDD_VEDIO_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);    //�����С�����仯ʱ����Ӧ����//
	POINT m_OldSize;                                    //�����С�����仯ǰ�ĳߴ�//
	bool m_bHaveInit;                 //�Ƿ��Ѿ���ʼ��//
	bool m_bHaveRemenberOldSize;       //�Ƿ��Ѿ���ס��ʼ�ߴ�//
	std::vector<CRect> m_vectOldSize;  //��Ÿ����ؼ��ĳ�ʼ�ߴ�//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);

	std::vector<CRect> m_vectVideoRect;  //�������ͷ���ڵ�λ��//
	void getVideoRect(int nPicStyle);    //������ʾ��������video����λ��//

	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedBtPlay();
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnBnClickedButton3();
	int m_nPicStyle;              //�������־��1�����桢4��ʾ�Ļ��棬9��ʾ�Ż��棬16��ʾʮ������//
	int m_nNumOfPage;             //����Ҫ��ҳ��ʱ����ǰ��ʾҳ���ҳ��//

	CRealplayxctrl1 m_vedio1;     //16����Ƶ������//
	CRealplayxctrl1 m_vedio2;
	CRealplayxctrl1 m_vedio3;
	CRealplayxctrl1 m_vedio4;
	CRealplayxctrl1 m_vedio5;
	CRealplayxctrl1 m_vedio6;
	CRealplayxctrl1 m_vedio7;
	CRealplayxctrl1 m_vedio8;
	CRealplayxctrl1 m_vedio9;
	CRealplayxctrl1 m_vedio10;
	CRealplayxctrl1 m_vedio11;
	CRealplayxctrl1 m_vedio12;
	CRealplayxctrl1 m_vedio13;
	CRealplayxctrl1 m_vedio14;
	CRealplayxctrl1 m_vedio15;
	CRealplayxctrl1 m_vedio16;
	afx_msg void OnBnClickedBtSinglepic();
	afx_msg void OnBnClickedBtFourpic();
	afx_msg void OnBnClickedBtNinepic();
	afx_msg void OnBnClickedBt16pic();
	void ShowOneVedio(CRealplayxctrl1 &vedio);           //�Զ��庯������ʾָ����������ͷ//
	void ShowFourVedio(CRealplayxctrl1 &vedio1, CRealplayxctrl1 &vedio2, CRealplayxctrl1 &vedio3, CRealplayxctrl1 &vedio4);//��ʾָ�����ĸ�����ͷ//
	void ShowNineVedio(int nPage);     //�Ż�����ʾ����ͷ//
	CRealplayxctrl1 m_vedio17;
	CRealplayxctrl1 m_vedio18;
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();    //����Դ//
	void connectVedio();          //���¶�ȡ���ݿ⣬��������ͷ//
	bool m_bIsVedioPlay;          //����ͷ�Ƿ����ڲ��ű�־��//
	int m_nNumOfVedio;            //����ͷ����//
	afx_msg void OnBnClickedBtNext();
	afx_msg void OnBnClickedBtLast();
	std::vector<CRealplayxctrl1*> m_vectVedio;//�洢���������ͷ��ActiveX�ؼ�//
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	void DblClickRealplayxctrl1();
	static UINT ThreadFunc(LPVOID lpParam);
	static UINT ThreadConnectVideo(LPVOID lpParam);
	std::vector<bool> m_vectIsVideoPlay;         //��������ͷ�Ƿ�������//
	void ClickRealplayxctrl1();
	void DblClickRealplayxctrl2();
	void DblClickRealplayxctrl3();
	void DblClickRealplayxctrl4();
	void DblClickRealplayxctrl5();
	void DblClickRealplayxctrl6();
	void DblClickRealplayxctrl7();
	void DblClickRealplayxctrl8();
	void DblClickRealplayxctrl9();
	void DblClickRealplayxctrl10();
	void DblClickRealplayxctrl11();
	void DblClickRealplayxctrl12();
	void DblClickRealplayxctrl13();
	void DblClickRealplayxctrl14();
	void DblClickRealplayxctrl15();
	void DblClickRealplayxctrl16();
};
