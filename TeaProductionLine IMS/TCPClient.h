#pragma once


#define FCT_READ_COILS		        0x01	// ����Ȧ
#define FCT_READ_HOLD_REG	        0x03	// �����ּĴ��� 
#define FCT_WRITE_SINGLE_COILS      0x05	// д��Ȧ 
#define FCT_WRITE_SINGLE_HOLD_REG   0x06	// д�����Ĵ��� 
#define FCT_WRITE_MULTIPLE_REG	    0x10	// д����Ĵ���

#define RECEIVE_BUFF_SIZE  300

#include <winsock.h>
#include "DataProvider.h"
#include "Thread.h"
#include "vector"
#include "PlcClass.h"
#include "SendBuffQueue.h"


class CTCPClient:Runnable
{
public:
	CTCPClient();
	virtual ~CTCPClient();
public:
	CString m_strRemoteHost;             //Զ�̷���IP��ַ
	int m_nPort;                         //Զ�̷��ʶ˿ڣ�modbus�̶����ʶ˿ڣ�502
	bool m_bIsconnected;                 //Socket����״̬
	bool m_bIsopen;                      //Socket��״̬
	CDataProvider *m_pDataP;             //ָ��ȫ�����ݴ洢��

	int m_PlcId;

private:
	SOCKET m_Socket;                     //ͨѶSocket���
	//HANDLE m_hExitThreadEvent;           //�˳�ͨѶ�߳��¼��ľ��
	CWinThread* m_WndTCPThread;          //ͨѶ�߳̾��

	CWnd* m_pWndOwner;                   //�����ھ��

public:
	bool Open();                         //��Socket�Լ�����ͨѶ�¼��̣߳�����Socket��Ϣ
	bool Connect();                      //����Socket
	bool Close();                        //�ر�Socket


	CSendBuffQueue m_ReadFrameQueue;//�洢������ѯPLC���ݵ�֡
	CSendBuffQueue m_WriteFrameQueue; //�洢��Ҫд�뵽PLC��֡

	/*����m_vRecvBuff���ٽ�������*/
	CRITICAL_SECTION m_RBCS;
	std::vector<BYTE> m_vRecvBuff;
	void DealRecvData(char* pcRcvData);



	/**
	���е��̺߳���������ʹ����������д�˺���
	**/
	void CreatePollThread();
	void StartPollThread();
	void ResumePollThread();
	void SuspendPollThread();
	virtual void Run(); 
	CThread *m_pThread;

private:
	//static UINT SocketThreadFunc(LPVOID lparam); //ͨѶ�߳����к���

	bool SendData(const char *p_SendBuff, int BuffLen);
	int GetPlcClassIndex();            //��ȡ��ǰPLC�ڹ������������е�λ������


};

