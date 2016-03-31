#include "stdafx.h"
#include "MobusClient.h"
#include <winsock.h>
#include <atlconv.h>
#include <afxpriv.h>




CMobusClient::CMobusClient(CPlcClass* Plc) :m_Plc(Plc)
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 1), &wd);
	m_nPort = 502;
	m_bIsconnected = FALSE;
	m_bIsopen = FALSE;
	m_PlcId = 0;
	//初始化全局数据类_

	m_strRemoteHost = m_Plc->m_strIPAddr;

	m_pDataP = CDataProvider::getInstance();
	m_pWorkThread = NULL;
	InitializeCriticalSection(&m_RBCS); //初始化临界区

}

CMobusClient::~CMobusClient()
{
	//释放Socket资源
	WSACleanup();
}

void CMobusClient::InitModbusClient()
{
	m_Plc->InitPlcMemory();
	ConstructModbusReadFrame();
}



void CMobusClient::CreatePollThread()
{
	m_pWorkThread = new CThread(this);
	m_pWorkThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);

}

void CMobusClient::StartPollThread()
{
	m_pWorkThread->Start();
	m_pWorkThread->SetThreadName(m_Plc->m_strIPAddr);
}
void CMobusClient::ResumePollThread()
{
	m_pWorkThread->Resume();
}
void CMobusClient::SuspendPollThread()
{
	m_pWorkThread->Suspend();
}

bool CMobusClient::GetConnectedState()
{
	return m_bIsconnected;
}

//线程工作函数//
void CMobusClient::Run()
{
	fd_set fdRead;
	int nRet;
	//定义事件等待时间
	TIMEVAL tvTime;
	tvTime.tv_sec = 1;
	tvTime.tv_usec = 0;
	while (TRUE)
	{
		if (m_bIsconnected == FALSE) //没有建立连接
		{
			TRACE("连接失败");
			Close();
			Open();
			Connect();
			if (m_bIsconnected == FALSE)
			{
				Sleep(500);
				continue;
			}
		}
		else  //建立连接，进行通信//
		{
			//当写入队列有数据时，优先把数据写入到PLC去//
			if (!m_WriteFrameQueue.IsEmpty())
			{
				CTcpSendBuffFrame tempWriteFrame = m_WriteFrameQueue.PopFront();
				char *pWrite = (char *)tempWriteFrame.m_BSendBuff;
				SendData(pWrite, tempWriteFrame.m_nLen);
			}
			else
			{
				CTcpSendBuffFrame tempReadFrame = m_ReadFrameQueue.PopFront();
				char *pRead = (char *)tempReadFrame.m_BSendBuff;
				m_ReadFrameQueue.PushBack(tempReadFrame); //构成一个循环队列，把队列第一个元素放到队列末尾//
				SendData(pRead, tempReadFrame.m_nLen);
			}

			//置fdRead事件为空
			FD_ZERO(&fdRead);
			FD_SET(this->m_Socket, &fdRead);
			//调用select函数，判断是否有读事件发生
			nRet = select(32, &fdRead, NULL, NULL, &tvTime);

			if (nRet == SOCKET_ERROR)
			{
				WSAGetLastError();
				//读取文件描述符失败
				TRACE("读取文件描述符失败\n");
				//关闭客户端socket
				closesocket(m_Socket);
				WSACleanup();
				m_bIsconnected = FALSE;
				m_Plc->SetConectedState(FALSE);
				//break;
			}

			if (nRet > 0)
			{
				if (FD_ISSET(m_Socket, &fdRead))
				{
					//检查fdset联系的文件句柄fd是否可读写，当>0表示可读写，则
					//发生读事件
					char cRecvBuf[RECEIVE_BUFF_SIZE];
					int nRecvLen;
					ZeroMemory(cRecvBuf, RECEIVE_BUFF_SIZE);
					TRACE("数据读取事件触发，执行读操作\n");
					//接收数据
					nRecvLen = recv(m_Socket, cRecvBuf, RECEIVE_BUFF_SIZE, 0);
					if (nRecvLen == SOCKET_ERROR)
					{
						int nError = WSAGetLastError();
						//数据接收操作失败
						TRACE("数据接收操作失败\n");
						//关闭客户端socket
						closesocket(m_Socket);
						m_bIsconnected = FALSE;
						m_Plc->SetConectedState(FALSE);
						//break;
					}
					else if (nRecvLen == 0)
					{					
						TRACE("数据接收等待过程中网络中断\n");					
						closesocket(m_Socket);
						m_bIsconnected = FALSE;
						m_Plc->SetConectedState(FALSE);
						//break;
					}
					else
					{
						m_bIsconnected = TRUE;
						m_Plc->SetConectedState(TRUE);

						DealRecvData(cRecvBuf,nRecvLen);
						DispatchReadResponse();		

						TRACE("%d 数据接收成功\n", m_pWorkThread->GetThreadID());
					}
				}
			}
			Sleep(50);
		}//end of else
	}
}

//============================================//
//功能：用于打开客户端Socket
//入口：无
//出口：无
//返回：TRUE--打开成功；FALSE--打开失败
//============================================//
bool CMobusClient::Open()
{
	//复位线程退出事件
	//	ResetEvent(m_hExitThreadEvent);
	//创建TCP套接字
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == SOCKET_ERROR)
	{
		m_bIsopen = FALSE;
		return FALSE;
	}

	m_bIsopen = TRUE;
	return TRUE;
}

//============================================//
//功能：用于建立与TCP服务器连接
//入口：无
//出口：无
//返回：TRUE--建立连接成功；FALSE--建立连接失败
//============================================//
bool CMobusClient::Connect()
{
	struct sockaddr_in addr;
	int nErr;
	char cAnsiRemoteHost[255];

	//定位当前连接的PLC在公共数据类中的索引位置

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	//此处要将双字节转换成单字节
	ZeroMemory(cAnsiRemoteHost, 255);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_strRemoteHost, wcslen(m_strRemoteHost), cAnsiRemoteHost, wcslen(m_strRemoteHost), NULL, NULL);
	addr.sin_addr.S_un.S_addr = inet_addr(cAnsiRemoteHost);

	//设置连接超时时间//
	DWORD TimeOut = 600;
	setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//面向连接的Socket客户端，不需要bind()
	//采用同步连接方式，connect直接返回成功或者失败
	nErr = connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr));
	if (nErr == SOCKET_ERROR)
	{
		TRACE("连接失败\n");
		m_bIsconnected = FALSE;
		m_Plc->SetConectedState(FALSE);
		//Close();
		return FALSE;
	}
	//设置通讯模式为异步模式
	DWORD dwUl = 1;
	ioctlsocket(m_Socket, FIONBIO, &dwUl);
	TRACE("连接成功\n");
	m_bIsconnected = TRUE;
	m_Plc->SetConectedState(TRUE);

	return TRUE;
}



//============================================//
//功能：用于关闭客户端Socket
//返回：TRUE--关闭成功；FALSE--关闭失败
//============================================//
bool CMobusClient::Close()
{
	int nErr = closesocket(m_Socket);
	if (nErr == SOCKET_ERROR)
	{
		return FALSE;
	}
	m_bIsconnected = FALSE;
	m_Plc->SetConectedState(FALSE);
	return TRUE;
}

//============================================//
//功能：向Plc端发送数据
//入口：c_pcSendBuf:待发送的数据
//      nLen:待发送数据的长度
//出口：无
//返回：TRUE；发送数据成功；FALSE：发送数据失败
//============================================//
bool CMobusClient::SendData(const char *p_SendBuff, int BuffLen)
{
	int nBytes = 0;

	nBytes = send(m_Socket, p_SendBuff, BuffLen, 0);
	if (nBytes == SOCKET_ERROR)
	{
		int nErrorCode = WSAGetLastError();
		//发送数据失败
		TRACE("发送数据失败\n");
		//关闭Socket
		Close();
		m_bIsconnected = FALSE;
		m_Plc->SetConectedState(FALSE);
		return FALSE;
	}

	TRACE("发送数据成功\n");
	m_bIsconnected = TRUE;
	m_Plc->SetConectedState(TRUE);
	return TRUE;
}



//处理到成功接收到的字符，移入到m_vRecvBuff中，待MODBUS的数据分发线程进行读取分发
void CMobusClient::DealRecvData(char* pcRcvData,int rcvLen)
{
	EnterCriticalSection(&m_RBCS);
	m_vRecvBuff.clear();
	for (int i = 0; i < rcvLen; i++)
	{
		m_vRecvBuff.push_back(*pcRcvData++);
	}
	LeaveCriticalSection(&m_RBCS);

}


//分发各个成功连接的TCP线程读取到的数据//
void CMobusClient::DispatchReadResponse()
{
	int nSizeOfResponseReadData = 0;   //字节个数//
	int sizeOfRecvBuff=0;
	
		sizeOfRecvBuff = m_vRecvBuff.size();
		if (m_bIsconnected&&sizeOfRecvBuff > 9) //当前客户端成功连接
		{
			if (m_vRecvBuff[7] == FCT_READ_HOLD_REG)
			{
				//如果解析的代码是读操作功能返回的代码//
				nSizeOfResponseReadData = m_vRecvBuff[8];

			
				int PlcMemoryType = m_vRecvBuff[0];
				int Respone_Frame_Index = m_vRecvBuff[1];

				int RegIndex = 0;
				switch (PlcMemoryType)
				{
				case READ_PLC_READ_MEMORY:
					for (int i = 0; i < nSizeOfResponseReadData - 1;)
					{
						RegIndex = (Respone_Frame_Index*MAX_READ_BYTE_LEN + i) / 2;
						m_Plc->m_PlcReadMemory[RegIndex].reg_Byte.high_byte = m_vRecvBuff[9 + i];
						++i;
						m_Plc->m_PlcReadMemory[RegIndex].reg_Byte.low_byte = m_vRecvBuff[9 + i];
						++i;
					}
					break;
				default:
					break;
				}

				//把该PLC下面的参数分派到各个参数记录中//
				m_Plc->DispatchPara();
			}
		}	
}



void CMobusClient::AddWriteRequest( CPlcWriteRequest& request)
{
	CTcpSendBuffFrame tempFrame = ConstructModbusWriteFrame(request);
	m_WriteFrameQueue.PushBack(tempFrame);
	return;
}

void  CMobusClient::ConstructModbusReadFrame()
{
	CTcpSendBuffFrame tempReadFrame;

	int nFrameCount = 0; //需要构建多少帧的数目//
	int FrameStartAddr;  //帧读取的起始地址//
	int FrameReadLength; //帧读取的数据字节长度//

	//plcClass.m_ReadLength中字节个数，MODBUS保持寄存器按照字读，需要转换为字节
	int SumReadLen = m_Plc->m_ReadLength;  //总共还需要读写的长度//
	int NumOfReg = 0;  //读取的寄存器个数//

	nFrameCount = SumReadLen / MAX_READ_BYTE_LEN;
	if (SumReadLen > nFrameCount*MAX_READ_BYTE_LEN)
	{
		nFrameCount++;
	} //nFrameCount把向上取整


	FrameStartAddr = m_Plc->m_ReadStartAddr / 2;
	//把	PLC里面读取内容读取出来//
	for (int index = 0; index < nFrameCount; index++)
	{
		FrameStartAddr = FrameStartAddr + NumOfReg;
		if (SumReadLen > MAX_READ_BYTE_LEN)//如果剩余的发送字节还大于每一帧最大能请求的字节数//
		{
			SumReadLen = SumReadLen - MAX_READ_BYTE_LEN;
			FrameReadLength = MAX_READ_BYTE_LEN;
		}
		else //否则，把剩余的需要发送字节数一次性发送
		{
			FrameReadLength = SumReadLen;
		}

		if (FrameReadLength % 2 == 0) //偶数
		{
			NumOfReg = FrameReadLength / 2;
		}
		else
		{
			NumOfReg = FrameReadLength / 2 + 1;
		}


		tempReadFrame.m_BSendBuff[0] = READ_PLC_READ_MEMORY;     //事务处理标识符，高低位，占两字节//
		tempReadFrame.m_BSendBuff[1] = index; //读取顺序的索引
		tempReadFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempReadFrame.m_BSendBuff[3] = 0x00;
		tempReadFrame.m_BSendBuff[4] = 0x00;               //后续字节数据长度，高低位，占两字节，读功能数据包为常量：0x0006
		tempReadFrame.m_BSendBuff[5] = 0x06;
		tempReadFrame.m_BSendBuff[6] = 0xff;               //单元标识符，占一字节，采用TCP/IP时，通过IP地址寻址，忽略此标识符，常量：0xff
		tempReadFrame.m_BSendBuff[7] = FCT_READ_HOLD_REG;  //功能代码,占一字节，上述宏定义//
		tempReadFrame.m_BSendBuff[8] = FrameStartAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempReadFrame.m_BSendBuff[9] = FrameStartAddr;
		tempReadFrame.m_BSendBuff[10] = NumOfReg >> 8;   //读取寄存器个数，高低位，占两字节
		tempReadFrame.m_BSendBuff[11] = NumOfReg;

		tempReadFrame.m_nLen = 12;

		m_ReadFrameQueue.PushBack(tempReadFrame);
	}

}


//把WriteRequest，按照MODBUS协议转化为Modbus协议帧//
CTcpSendBuffFrame CMobusClient::ConstructModbusWriteFrame(CPlcWriteRequest& WriteRequest)
{
	CTcpSendBuffFrame tempSendFrame;
	int And_mask;
	int Or_mask;
	int StartRegAddr; //起始寄存器//
	int BitOffset = 0;   //位操作时的位偏移//
	int ByteOffset = 0;
	int WordIndex = 0;
	WORD wValue = 0;


	switch (WriteRequest.m_AddrType)
	{
	case CPlcWriteRequest::ADDR_TYPE_BIT:
		//掩码寄存器操作 结果=（寄存器当前内容AND And_Mask）OR(Or_mask AND And_mask)//
		//对应位置0，需要And_Mask,Or_mask的位都为0
		//对应位置1，需要And_mask对应位为0，Or_mask对应位为1
		//保持不变的位，And_mask为1，Or_mask的位置0
		/*
		if (WriteRequest.m_WriteByteAddrIndex%2==0) //字节顺序为偶数，在Modbus大端字节序中为寄存器的高字节
		{
		BitOffset = WriteRequest.m_ByteOffset + 8;
		}
		else //奇数字节位保持寄存器的低字节//
		{
		BitOffset = WriteRequest.m_ByteOffset;
		}

		//以功能码的方式写保持寄存器的某一位
		And_mask = 0xFFFF;
		Or_mask = 0;
		if ((BOOL)WriteRequest.m_SetValue==TRUE) //对应位置1
		{
		And_mask &= ~(1 << BitOffset);
		Or_mask |= (1 << BitOffset);
		}
		else //对应位置0，And_mask，Or_mask对应位都为0.
		{
		And_mask &= ~(1 << BitOffset);
		Or_mask &= ~(1 << BitOffset);
		}

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x08;
		tempSendFrame.m_BSendBuff[6] = 0xff;
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MASK_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (And_mask & 0xff00) >> 8;  //And_mask屏蔽高位
		tempSendFrame.m_BSendBuff[11] = And_mask & 0x00ff; //And_mask屏蔽低位//
		tempSendFrame.m_BSendBuff[12] = (Or_mask & 0xff00) >> 8;  //Or_mask屏蔽高位
		tempSendFrame.m_BSendBuff[13] = Or_mask & 0x00ff; //Or_mask屏蔽地低位//
		tempSendFrame.m_nLen = 14;
		*/

		//以写寄存器，或的方式写入寄存器的位 2015/09/11

		StartRegAddr = (int)WriteRequest.m_WriteByteAddrIndex / 2;

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //单元标识符，占一字节，采用TCP/IP时，通过IP地址寻址，忽略此标识符，常量：0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;


		if (WriteRequest.m_WriteByteAddrIndex % 2 == 0)
		{
			BitOffset = WriteRequest.m_ByteOffset;
		}
		else //奇数字节位保持寄存器的低字节//
		{
			BitOffset = WriteRequest.m_ByteOffset + 8;
		}

		for (int i = 0; i < m_pDataP->m_vectPlc.size(); i++)
		{
			if (WriteRequest.m_PlcId == m_pDataP->m_vectPlc[i].m_Id)
			{
				wValue = m_pDataP->m_vectPlc[i].GetReadMemoryWord(StartRegAddr);
				break;
			}
		}

		if (WriteRequest.m_SetValue == 1)
		{
			wValue = wValue | (1 << BitOffset);
		}
		else
		{
			wValue = (wValue & ~(1 << BitOffset));
		}
		tempSendFrame.m_BSendBuff[10] = (wValue & 0xff00) >> 8;
		tempSendFrame.m_BSendBuff[11] = wValue & 0x00ff; //字节数//

		tempSendFrame.m_nLen = 12;

		break;
	case CPlcWriteRequest::ADDR_TYPE_BYTE:
		/*
		if (WriteRequest.m_WriteByteAddrIndex%2==0) //偶数，为操作16位寄存器的高字节//
		{
		And_mask = 0x00FF;
		Or_mask = 0;
		Or_mask |= ((WriteRequest.m_SetValue & 0xFF) << 8);
		}
		else //奇数，16位寄存器的低字节//
		{
		And_mask = 0xFF00;
		Or_mask = 0;
		Or_mask |= (WriteRequest.m_SetValue & 0xFF);
		}
		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x08;
		tempSendFrame.m_BSendBuff[6] = 0xff;
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MASK_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节//
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (And_mask & 0xff00) >> 8;  //And_mask屏蔽高字节
		tempSendFrame.m_BSendBuff[11] = And_mask & 0x00ff; //And_mask屏蔽低字节//
		tempSendFrame.m_BSendBuff[12] = (Or_mask & 0xff00) >> 8;  //Or_mask屏蔽高字节
		tempSendFrame.m_BSendBuff[13] = Or_mask & 0x00ff; //Or_mask屏蔽地低位//

		tempSendFrame.m_nLen = 14;
		*/

		StartRegAddr = (int)(WriteRequest.m_WriteByteAddrIndex / 2);

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //单元标识符，占一字节，采用TCP/IP时，通过IP地址寻址，忽略此标识符，常量：0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;


		if (WriteRequest.m_WriteByteAddrIndex % 2 == 0) //字节顺序为偶数，在Modbus大端字节序中为寄存器的高字节
		{
			ByteOffset = 0;
		}
		else //奇数字节位保持寄存器的低字节//
		{
			ByteOffset = 8;
		}

		for (int i = 0; i < m_pDataP->m_vectPlc.size(); i++)
		{
			if (WriteRequest.m_PlcId == m_pDataP->m_vectPlc[i].m_Id)
			{
				wValue = m_pDataP->m_vectPlc[i].GetReadMemoryWord(StartRegAddr);
				break;
			}
		}

		wValue &= ~(0xFF << ByteOffset);
		wValue |= (WriteRequest.m_SetValue << ByteOffset);

		tempSendFrame.m_BSendBuff[10] = (wValue & 0xff00) >> 8;
		tempSendFrame.m_BSendBuff[11] = wValue & 0x00ff;

		tempSendFrame.m_nLen = 12;

		break;
	case CPlcWriteRequest::ADDR_TYPE_WORD:
		StartRegAddr = WriteRequest.m_WriteByteAddrIndex;

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //单元标识符，占一字节，采用TCP/IP时，通过IP地址寻址，忽略此标识符，常量：0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (WriteRequest.m_SetValue & 0x0000ff00) >> 8;
		tempSendFrame.m_BSendBuff[11] = WriteRequest.m_SetValue & 0x000000ff; //字节数//

		tempSendFrame.m_nLen = 12;

		break;
	case CPlcWriteRequest::ADDR_TYPE_DWORD:		//在此只有Float为双字类型，传输float

		StartRegAddr = WriteRequest.m_WriteByteAddrIndex * 2;

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //事务处理标识符，高低位，占两字节//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //协议标识符，高低位，占两字节，常量，为0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x0B;              //协议请求数据长度，//
		tempSendFrame.m_BSendBuff[6] = 0xFF;              //单元标识符，占一字节，采用TCP/IP时，通过IP地址寻址，忽略此标识符，常量：0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MULTIPLE_REG;  //功能代码,占一字节，上述宏定义//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //开始寄存器地址，高低位，占两字节
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = 0;            //寄存器数量Hi//  
		tempSendFrame.m_BSendBuff[11] = 2;			//寄存器数量Lo//  
		tempSendFrame.m_BSendBuff[12] = 4; //字节数//
		tempSendFrame.m_BSendBuff[13] = WriteRequest.GetFloatValueByte(1);
		tempSendFrame.m_BSendBuff[14] = WriteRequest.GetFloatValueByte(0);
		tempSendFrame.m_BSendBuff[15] = WriteRequest.GetFloatValueByte(3);
		tempSendFrame.m_BSendBuff[16] = WriteRequest.GetFloatValueByte(2);
		tempSendFrame.m_nLen = 17; //写入长度
		break;
	default:
		break;
	}

	return tempSendFrame;
}



