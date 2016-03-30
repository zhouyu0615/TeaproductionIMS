#pragma once
#include "DataProvider.h"
#include "Thread.h"
#include "PlcClass.h"
#include "SendBuffQueue.h"
#include "PlcWriteRequest.h"


#define FCT_READ_COILS		        0x01	// 读线圈
#define FCT_READ_HOLD_REG	        0x03	// 读保持寄存器 
#define FCT_WRITE_SINGLE_COILS      0x05	// 写线圈 
#define FCT_WRITE_SINGLE_HOLD_REG   0x06	// 写单个寄存器 
#define FCT_WRITE_MULTIPLE_REG	    0x10	// 写多个寄存器

#define RECEIVE_BUFF_SIZE  300

#define  MAX_READ_BYTE_LEN  240     //每一帧读取240个字节//

#define  READ_PLC_READ_MEMORY      0X01 //读取PLC读入地址的事务标识符//
#define  READ_PLC_WRITE_MEMORY     0x02  //读取PLC写入地址的事务标识符//

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

	SOCKET m_Socket;                     //通讯Socket句柄

	CString m_strRemoteHost;             //远程访问IP地址
	int m_nPort;                         //远程访问端口，modbus固定访问端口：502
	bool m_bIsconnected;                 //Socket连接状态
	bool m_bIsopen;                      //Socket打开状态
	CDataProvider *m_pDataP;             //指向全局数据存储类

	int m_PlcId;

	CSendBuffQueue m_ReadFrameQueue;   //存储几条查询PLC数据的帧
	CSendBuffQueue m_WriteFrameQueue;  //存储需要写入到PLC的帧

public:
	bool Open();                         //打开Socket以及启动通讯事件线程，接收Socket消息
	bool Connect();                      //连接Socket
	bool Close();                        //关闭Socket

	bool SendData(const char *p_SendBuff, int BuffLen);


	/*保护m_vRecvBuff的临界区变量*/
	CRITICAL_SECTION m_RBCS;

	/**
	运行的线程函数，可以使用派生类重写此函数
	**/
	void CreatePollThread();
	void StartPollThread();
	void ResumePollThread();
	void SuspendPollThread();
	void Run();

	CThread *m_pWorkThread;


	void AddWriteRequest(CPlcWriteRequest& request);

	//根据modbus协议把写请求转化为TCP要发送的字节流//
	CTcpSendBuffFrame ConstructModbusWriteFrame(CPlcWriteRequest& WriteRequest);
	//创建读功能数据帧//
	void  ConstructModbusReadFrame();  

	std::vector<BYTE> m_vRecvBuff;
	void DealRecvData(char* pcRcvData, int rcvLen);
	void DispatchReadResponse();
};

