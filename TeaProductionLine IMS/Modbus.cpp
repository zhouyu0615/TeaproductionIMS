#include "stdafx.h"
#include "Modbus.h"


CModbus::CModbus()
{
	//初始化运行环境
	//创建退出轮询线程事件句柄,初始状态为无信号，自动复原（即不需要ReestEvent函数来手工复原信号量）
	//m_hExitThreadCycle = CreateEvent(NULL, FALSE, FALSE, NULL);
	//初始化全局数据类
	m_pDataP = CDataProvider::getInstance();

	m_WndCycleThread = NULL;

	m_DispatchThread = NULL;
}


CModbus::~CModbus()
{
	//关闭退出轮询线程事件句柄

	//CloseHandle(m_WndCycleThread);
	//CloseHandle(m_hExitThreadCycle);
	delete(m_DispatchThread);
}



//====================================================================================================//
//功能：根据数据库中读取的PLC信息，创建CTCPClient容器，启动modbus轮询线程，实时读，并根据窗口需求写modbus数据      
//入口：vPlcClass:待连接的所有PLC的类，包括PLCIP地址，读取缓冲区大小等              
//出口：无
//返回：无
//====================================================================================================//
void CModbus::CycleStart()
{
	//根据PLC数量添加CTCPClient容器
	m_vMultipleTCPClient.clear();

	//初始化PLC的内存存储区//
	for (int i = 0; i < m_pDataP->m_vectPlc.size();i++)
	{
		m_pDataP->m_vectPlc[i].InitPlcMemory();
	}

	m_vPlcClass = m_pDataP->m_vectPlc;
	m_nCountTCPClient = m_vPlcClass.size();
	for (int i = 0; i < m_nCountTCPClient; i++)
	{
		m_pTCPClient = new CTCPClient;
		m_pTCPClient->m_strRemoteHost = m_vPlcClass[i].m_strIPAddr;
		m_pTCPClient->m_PlcId = m_vPlcClass[i].m_Id;
		m_vMultipleTCPClient.push_back(*m_pTCPClient);
	}

	//复位退出轮询线程事件	
	//	ResetEvent(m_hExitThreadCycle);

	//启动轮询线程


	m_WndCycleThread = AfxBeginThread(CycleThreadFunc, this, THREAD_PRIORITY_ABOVE_NORMAL);


	if (!m_WndCycleThread)
	{
		//线程启动不成功
		m_bIsCycling = FALSE;
		m_bModbusRead = FALSE;
	}
	else
	{
		//线程启动成功，开始轮询
		m_bIsCycling = TRUE;
		m_bModbusRead = TRUE;
	}

	//启动数据分发线程
	m_DispatchThread = new CThread(this);
	m_DispatchThread->Start();

}

//====================================================================================================//
//功能：停止轮询线程，即停止对PLC的读和写，并断开PLC的连接
//入口：无         
//出口：无
//返回：无
//====================================================================================================//
void CModbus::CycleDisconnect()
{
	//发送轮询线程退出事件
	//SetEvent(m_hExitThreadCycle);
	m_WndCycleThread->SuspendThread();
	m_DispatchThread->Suspend();
	for (int i = 0; i < m_nCountTCPClient; i++)
	{
		if (m_vMultipleTCPClient[i].m_bIsconnected == TRUE)
		{
			m_vMultipleTCPClient[i].SuspendPollThread();
			m_vMultipleTCPClient[i].Close();
		}
	}
	m_bIsCycling = FALSE;
	m_bModbusRead = FALSE;
}


//====================================================================================================//
//功能：创建读功能数据帧
//入口：plcClass：CPlcClass类，存储读目标的IP，读取的起始地址以及寄存器数量，便于生成读取数据帧
//      nPlcIndex：标记当前PLC在PLC列表中的序列索引，便于定位
//出口：无
//返回：BYTE类型数组，用于返回组装好的数据帧地址
//====================================================================================================//

//根据Plc提供的读取起始地址，读取长度等内容创建读功能数据帧,并写入到TcpClient的读取队列中//
void CModbus::ConstructModbusReadFrame(CPlcClass plcClass, int nPlcIndex)
{
	CTcpSendBuffFrame tempReadFrame;
	

	int nFrameCount = 0; //需要构建多少帧的数目//
	int FrameStartAddr;  //帧读取的起始地址//
	int FrameReadLength; //帧读取的数据字节长度//

	//plcClass.m_ReadLength中字节个数，MODBUS保持寄存器按照字读，需要转换为字节
	int SumReadLen = plcClass.m_ReadLength;  //总共还需要读写的长度//
	int NumOfReg=0;  //读取的寄存器个数//

	nFrameCount = SumReadLen / MAX_READ_BYTE_LEN;
	if (SumReadLen>nFrameCount*MAX_READ_BYTE_LEN)
	{
		nFrameCount++;
	} //nFrameCount把向上取整


	FrameStartAddr = plcClass.m_ReadStartAddr/2;
	//把	PLC里面读取内容读取出来//
	for (int  index = 0; index < nFrameCount; index++)
	{
		FrameStartAddr = FrameStartAddr + NumOfReg;
		if (SumReadLen>MAX_READ_BYTE_LEN)//如果剩余的发送字节还大于每一帧最大能请求的字节数//
		{
			SumReadLen = SumReadLen - MAX_READ_BYTE_LEN;
			FrameReadLength = MAX_READ_BYTE_LEN;
		}
		else //否则，把剩余的需要发送字节数一次性发送
		{
			FrameReadLength = SumReadLen;
		}	

		if (FrameReadLength%2==0) //偶数
		{
			NumOfReg = FrameReadLength / 2;
		}
		else
		{
			NumOfReg = FrameReadLength / 2+1;
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
		m_vMultipleTCPClient[nPlcIndex].m_ReadFrameQueue.PushBack(tempReadFrame);
	}

	
   

}



//==================================================================//
//功能：此线程用于轮询PLC，打开、连接、读取和写入数据
//入口：lparam:无类型指针，可以通过此参数，向线程中传入需要用到的资源
//      此处将CModbus类实例指针传入
//      nLen:待发送数据的长度
//出口：无
//返回：返回值无意义，返回0
//==================================================================//
UINT CModbus::CycleThreadFunc(LPVOID lparam)
{
	CModbus* pmd;
	pmd = (CModbus*)lparam;
	pmd->m_nIndex = 0;
	pmd->m_nWriteIndex = 0;
	//线程启动，打开定时器，周期性复位读写状态

	while (TRUE)
	{
		//收到退出事件，结束线程//
		//if (WaitForSingleObject(pmd->m_hExitThreadCycle, 0) == WAIT_OBJECT_0)
		//{
		//	break;
		//}

		if (pmd->m_vMultipleTCPClient.size() <= 0)
		{
			continue;
		}

		for (int i = 0; i < pmd->m_vMultipleTCPClient.size(); i++)
		{
			if (pmd->m_vMultipleTCPClient[i].m_bIsconnected == FALSE)
			{
				pmd->m_vMultipleTCPClient[i].Close();
				pmd->m_vMultipleTCPClient[i].Open();
				pmd->m_vMultipleTCPClient[i].Connect();
				if (pmd->m_vMultipleTCPClient[i].m_bIsconnected == TRUE)
				{
					//构造读取PLC内存的数据帧，写入到TCPCLIENT线程发送队列中，启动线程//
					pmd->ConstructModbusReadFrame(pmd->m_vPlcClass[i], i);
					if (pmd->m_vMultipleTCPClient[i].m_pThread == NULL)
					{
						pmd->m_vMultipleTCPClient[i].CreatePollThread();
						pmd->m_vMultipleTCPClient[i].StartPollThread();
					}
					else
					{
						pmd->m_vMultipleTCPClient[i].ResumePollThread();
					}
				}

			}
		}
		Sleep(200);
	}
	return 0;
}

//分发各个成功连接的TCP线程读取到的数据//
void CModbus::DispatchReadResponse()
{
	int nSizeOfResponseReadData=0;   //字节个数//
	int lenth = m_vMultipleTCPClient.size();
	int sizeOfRecvBuff;
	int PlcIndex;
	int TcpIndex;
	for (TcpIndex = 0; TcpIndex < lenth; TcpIndex++)
	{
		if (m_vMultipleTCPClient[TcpIndex].m_bIsconnected == FALSE)
		{
			continue;
		}
		EnterCriticalSection(&m_vMultipleTCPClient[TcpIndex].m_RBCS);
		sizeOfRecvBuff = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff.size();
		if (m_vMultipleTCPClient[TcpIndex].m_bIsconnected&&sizeOfRecvBuff > 9) //当前客户端成功连接
		{
			if (m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[7] == FCT_READ_HOLD_REG)
			{
				//如果解析的代码是读操作功能返回的代码//
				nSizeOfResponseReadData = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[8];

				PlcIndex = GetPlcClassIndex(m_vMultipleTCPClient[TcpIndex].m_strRemoteHost);

				int PlcMemoryType = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[0];
				int Respone_Frame_Index = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[1];

				int RegIndex = 0;
				switch (PlcMemoryType)
				{
				case READ_PLC_READ_MEMORY:

					for (int i = 0; i < nSizeOfResponseReadData-1;)
					{
						RegIndex = (Respone_Frame_Index*MAX_READ_BYTE_LEN + i) / 2;

						m_pDataP->m_vectPlc[PlcIndex].m_PlcReadMemory[RegIndex].reg_Byte.high_byte = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[9 + i];

						++i;
						m_pDataP->m_vectPlc[PlcIndex].m_PlcReadMemory[RegIndex].reg_Byte.low_byte = m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[9 + i];
						++i;
					}
					break;

				default:
					break;
				}
				
			    //把该PLC下面的参数分派到各个参数记录中//
				m_pDataP->DispatchParaValue(PlcIndex);
			}
			
		}
		LeaveCriticalSection(&m_vMultipleTCPClient[TcpIndex].m_RBCS);
	}
}

//检验数据的完整性
//index 为校验的数据在m_vMultipleTCPClient中的索引
//校验通过返回0，否则返回1
BOOL CModbus::VerifyRecvData(int Index)
{
	return 0;
}

int CModbus::GetPlcClassIndex(CString strIpAddr)
{
	int PlcIndex;
	for (PlcIndex = 0; PlcIndex < m_pDataP->m_vectPlc.size(); PlcIndex++)
	{
		if (strIpAddr == m_pDataP->m_vectPlc[PlcIndex].m_strIPAddr)
		{
			return PlcIndex;
		}
	}
	return 0;

}


void CModbus::Run()
{
	while (TRUE)
	{
		TRACE("数据分发线程正在分发");
		DispatchWriteRequest();

		DispatchReadResponse();
		Sleep(30);
	}

}

//把写请求WriteRequest分发到各个TcpClinet通信线程//
void CModbus::DispatchWriteRequest()
{
	CPlcWriteRequest tempRequest;
	while (!(m_pDataP->m_wRequestQueue.IsEmpty()))
	{
		tempRequest = m_pDataP->m_wRequestQueue.PopFront();
		for (int i = 0; i < m_vMultipleTCPClient.size(); i++)
		{
			if (tempRequest.m_PlcId == m_vMultipleTCPClient[i].m_PlcId)
			{
				if (m_vMultipleTCPClient[i].m_bIsconnected == TRUE)
				{
					CTcpSendBuffFrame tempFrame = ConstructModbusWriteFrame(tempRequest);
					m_vMultipleTCPClient[i].m_WriteFrameQueue.PushBack(tempFrame);
				}

			}
		}
	}

}

//把WriteRequest，按照MODBUS协议转化为Modbus协议帧//
CTcpSendBuffFrame CModbus::ConstructModbusWriteFrame(CPlcWriteRequest WriteRequest)
{
	CTcpSendBuffFrame tempSendFrame;
	int And_mask ;
	int Or_mask;
	int StartRegAddr; //起始寄存器//
	int BitOffset=0;   //位操作时的位偏移//
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
	    
		StartRegAddr =(int)WriteRequest.m_WriteByteAddrIndex/2;

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
			BitOffset = WriteRequest.m_ByteOffset+8;
		}

		for (int i = 0; i < m_pDataP->m_vectPlc.size(); i++)
		{
			if (WriteRequest.m_PlcId == m_pDataP->m_vectPlc[i].m_Id)
			{
				wValue = m_pDataP->m_vectPlc[i].GetReadMemoryWord(StartRegAddr);
				break;
			}
		}

		if (WriteRequest.m_SetValue==1)
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

		StartRegAddr = (int)(WriteRequest.m_WriteByteAddrIndex/2);

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
		
		StartRegAddr = WriteRequest.m_WriteByteAddrIndex*2;

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


