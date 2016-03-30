#pragma once
#include "DataProvider.h"
#include "Thread.h"
#include "PlcClass.h"
#include "SendBuffQueue.h"
#include "PlcWriteRequest.h"


#define FCT_READ_COILS		        0x01	// ����Ȧ
#define FCT_READ_HOLD_REG	        0x03	// �����ּĴ��� 
#define FCT_WRITE_SINGLE_COILS      0x05	// д��Ȧ 
#define FCT_WRITE_SINGLE_HOLD_REG   0x06	// д�����Ĵ��� 
#define FCT_WRITE_MULTIPLE_REG	    0x10	// д����Ĵ���

#define RECEIVE_BUFF_SIZE  300

#define  MAX_READ_BYTE_LEN  240     //ÿһ֡��ȡ240���ֽ�//

#define  READ_PLC_READ_MEMORY      0X01 //��ȡPLC�����ַ�������ʶ��//
#define  READ_PLC_WRITE_MEMORY     0x02  //��ȡPLCд���ַ�������ʶ��//

class CMobusClient:public Runnable
{
private:
	CMobusClient();
public:	
	CMobusClient(CPlcClass& plc);
	~CMobusClient();
	
	void InitModbusClient();
	
private:
	CPlcClass& m_Plc;

	SOCKET m_Socket;                     //ͨѶSocket���

	CString m_strRemoteHost;             //Զ�̷���IP��ַ
	int m_nPort;                         //Զ�̷��ʶ˿ڣ�modbus�̶����ʶ˿ڣ�502
	bool m_bIsconnected;                 //Socket����״̬
	bool m_bIsopen;                      //Socket��״̬
	CDataProvider *m_pDataP;             //ָ��ȫ�����ݴ洢��

	int m_PlcId;

	CSendBuffQueue m_ReadFrameQueue;   //�洢������ѯPLC���ݵ�֡
	CSendBuffQueue m_WriteFrameQueue;  //�洢��Ҫд�뵽PLC��֡

public:
	bool Open();                         //��Socket�Լ�����ͨѶ�¼��̣߳�����Socket��Ϣ
	bool Connect();                      //����Socket
	bool Close();                        //�ر�Socket

	bool SendData(const char *p_SendBuff, int BuffLen);


	/*����m_vRecvBuff���ٽ�������*/
	CRITICAL_SECTION m_RBCS;

	/**
	���е��̺߳���������ʹ����������д�˺���
	**/
	void CreatePollThread();
	void StartPollThread();
	void ResumePollThread();
	void SuspendPollThread();
	void Run();

	CThread *m_pWorkThread;


	void AddWriteRequest(CPlcWriteRequest& request);

	//����modbusЭ���д����ת��ΪTCPҪ���͵��ֽ���//
	CTcpSendBuffFrame ConstructModbusWriteFrame(CPlcWriteRequest& WriteRequest);
	//��������������֡//
	void  ConstructModbusReadFrame();  

	std::vector<BYTE> m_vRecvBuff;
	void DealRecvData(char* pcRcvData, int rcvLen);
	void DispatchReadResponse();
};

