#include "stdafx.h"
#include "Modbus.h"


CModbus::CModbus()
{
	//��ʼ�����л���
	//�����˳���ѯ�߳��¼����,��ʼ״̬Ϊ���źţ��Զ���ԭ��������ҪReestEvent�������ֹ���ԭ�ź�����
	//m_hExitThreadCycle = CreateEvent(NULL, FALSE, FALSE, NULL);
	//��ʼ��ȫ��������
	m_pDataP = CDataProvider::getInstance();

	m_WndCycleThread = NULL;

	m_DispatchThread = NULL;
}


CModbus::~CModbus()
{
	//�ر��˳���ѯ�߳��¼����

	//CloseHandle(m_WndCycleThread);
	//CloseHandle(m_hExitThreadCycle);
	delete(m_DispatchThread);
}



//====================================================================================================//
//���ܣ��������ݿ��ж�ȡ��PLC��Ϣ������CTCPClient����������modbus��ѯ�̣߳�ʵʱ���������ݴ�������дmodbus����      
//��ڣ�vPlcClass:�����ӵ�����PLC���࣬����PLCIP��ַ����ȡ��������С��              
//���ڣ���
//���أ���
//====================================================================================================//
void CModbus::CycleStart()
{
	//����PLC�������CTCPClient����
	m_vMultipleTCPClient.clear();

	//��ʼ��PLC���ڴ�洢��//
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

	//��λ�˳���ѯ�߳��¼�	
	//	ResetEvent(m_hExitThreadCycle);

	//������ѯ�߳�


	m_WndCycleThread = AfxBeginThread(CycleThreadFunc, this, THREAD_PRIORITY_ABOVE_NORMAL);


	if (!m_WndCycleThread)
	{
		//�߳��������ɹ�
		m_bIsCycling = FALSE;
		m_bModbusRead = FALSE;
	}
	else
	{
		//�߳������ɹ�����ʼ��ѯ
		m_bIsCycling = TRUE;
		m_bModbusRead = TRUE;
	}

	//�������ݷַ��߳�
	m_DispatchThread = new CThread(this);
	m_DispatchThread->Start();

}

//====================================================================================================//
//���ܣ�ֹͣ��ѯ�̣߳���ֹͣ��PLC�Ķ���д�����Ͽ�PLC������
//��ڣ���         
//���ڣ���
//���أ���
//====================================================================================================//
void CModbus::CycleDisconnect()
{
	//������ѯ�߳��˳��¼�
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
//���ܣ���������������֡
//��ڣ�plcClass��CPlcClass�࣬�洢��Ŀ���IP����ȡ����ʼ��ַ�Լ��Ĵ����������������ɶ�ȡ����֡
//      nPlcIndex����ǵ�ǰPLC��PLC�б��е��������������ڶ�λ
//���ڣ���
//���أ�BYTE�������飬���ڷ�����װ�õ�����֡��ַ
//====================================================================================================//

//����Plc�ṩ�Ķ�ȡ��ʼ��ַ����ȡ���ȵ����ݴ�������������֡,��д�뵽TcpClient�Ķ�ȡ������//
void CModbus::ConstructModbusReadFrame(CPlcClass plcClass, int nPlcIndex)
{
	CTcpSendBuffFrame tempReadFrame;
	

	int nFrameCount = 0; //��Ҫ��������֡����Ŀ//
	int FrameStartAddr;  //֡��ȡ����ʼ��ַ//
	int FrameReadLength; //֡��ȡ�������ֽڳ���//

	//plcClass.m_ReadLength���ֽڸ�����MODBUS���ּĴ��������ֶ�����Ҫת��Ϊ�ֽ�
	int SumReadLen = plcClass.m_ReadLength;  //�ܹ�����Ҫ��д�ĳ���//
	int NumOfReg=0;  //��ȡ�ļĴ�������//

	nFrameCount = SumReadLen / MAX_READ_BYTE_LEN;
	if (SumReadLen>nFrameCount*MAX_READ_BYTE_LEN)
	{
		nFrameCount++;
	} //nFrameCount������ȡ��


	FrameStartAddr = plcClass.m_ReadStartAddr/2;
	//��	PLC�����ȡ���ݶ�ȡ����//
	for (int  index = 0; index < nFrameCount; index++)
	{
		FrameStartAddr = FrameStartAddr + NumOfReg;
		if (SumReadLen>MAX_READ_BYTE_LEN)//���ʣ��ķ����ֽڻ�����ÿһ֡�����������ֽ���//
		{
			SumReadLen = SumReadLen - MAX_READ_BYTE_LEN;
			FrameReadLength = MAX_READ_BYTE_LEN;
		}
		else //���򣬰�ʣ�����Ҫ�����ֽ���һ���Է���
		{
			FrameReadLength = SumReadLen;
		}	

		if (FrameReadLength%2==0) //ż��
		{
			NumOfReg = FrameReadLength / 2;
		}
		else
		{
			NumOfReg = FrameReadLength / 2+1;
		}
	

		tempReadFrame.m_BSendBuff[0] = READ_PLC_READ_MEMORY;     //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempReadFrame.m_BSendBuff[1] = index; //��ȡ˳�������
		tempReadFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempReadFrame.m_BSendBuff[3] = 0x00;
		tempReadFrame.m_BSendBuff[4] = 0x00;               //�����ֽ����ݳ��ȣ��ߵ�λ��ռ���ֽڣ����������ݰ�Ϊ������0x0006
		tempReadFrame.m_BSendBuff[5] = 0x06;
		tempReadFrame.m_BSendBuff[6] = 0xff;               //��Ԫ��ʶ����ռһ�ֽڣ�����TCP/IPʱ��ͨ��IP��ַѰַ�����Դ˱�ʶ����������0xff
		tempReadFrame.m_BSendBuff[7] = FCT_READ_HOLD_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempReadFrame.m_BSendBuff[8] = FrameStartAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempReadFrame.m_BSendBuff[9] = FrameStartAddr;
		tempReadFrame.m_BSendBuff[10] = NumOfReg >> 8;   //��ȡ�Ĵ����������ߵ�λ��ռ���ֽ�
		tempReadFrame.m_BSendBuff[11] = NumOfReg;

		tempReadFrame.m_nLen = 12;
		m_vMultipleTCPClient[nPlcIndex].m_ReadFrameQueue.PushBack(tempReadFrame);
	}

	
   

}



//==================================================================//
//���ܣ����߳�������ѯPLC���򿪡����ӡ���ȡ��д������
//��ڣ�lparam:������ָ�룬����ͨ���˲��������߳��д�����Ҫ�õ�����Դ
//      �˴���CModbus��ʵ��ָ�봫��
//      nLen:���������ݵĳ���
//���ڣ���
//���أ�����ֵ�����壬����0
//==================================================================//
UINT CModbus::CycleThreadFunc(LPVOID lparam)
{
	CModbus* pmd;
	pmd = (CModbus*)lparam;
	pmd->m_nIndex = 0;
	pmd->m_nWriteIndex = 0;
	//�߳��������򿪶�ʱ���������Ը�λ��д״̬

	while (TRUE)
	{
		//�յ��˳��¼��������߳�//
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
					//�����ȡPLC�ڴ������֡��д�뵽TCPCLIENT�̷߳��Ͷ����У������߳�//
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

//�ַ������ɹ����ӵ�TCP�̶߳�ȡ��������//
void CModbus::DispatchReadResponse()
{
	int nSizeOfResponseReadData=0;   //�ֽڸ���//
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
		if (m_vMultipleTCPClient[TcpIndex].m_bIsconnected&&sizeOfRecvBuff > 9) //��ǰ�ͻ��˳ɹ�����
		{
			if (m_vMultipleTCPClient[TcpIndex].m_vRecvBuff[7] == FCT_READ_HOLD_REG)
			{
				//��������Ĵ����Ƕ��������ܷ��صĴ���//
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
				
			    //�Ѹ�PLC����Ĳ������ɵ�����������¼��//
				m_pDataP->DispatchParaValue(PlcIndex);
			}
			
		}
		LeaveCriticalSection(&m_vMultipleTCPClient[TcpIndex].m_RBCS);
	}
}

//�������ݵ�������
//index ΪУ���������m_vMultipleTCPClient�е�����
//У��ͨ������0�����򷵻�1
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
		TRACE("���ݷַ��߳����ڷַ�");
		DispatchWriteRequest();

		DispatchReadResponse();
		Sleep(30);
	}

}

//��д����WriteRequest�ַ�������TcpClinetͨ���߳�//
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

//��WriteRequest������MODBUSЭ��ת��ΪModbusЭ��֡//
CTcpSendBuffFrame CModbus::ConstructModbusWriteFrame(CPlcWriteRequest WriteRequest)
{
	CTcpSendBuffFrame tempSendFrame;
	int And_mask ;
	int Or_mask;
	int StartRegAddr; //��ʼ�Ĵ���//
	int BitOffset=0;   //λ����ʱ��λƫ��//
	int ByteOffset = 0;
	int WordIndex = 0;
	WORD wValue = 0;


	
	

	switch (WriteRequest.m_AddrType)
	{
	case CPlcWriteRequest::ADDR_TYPE_BIT:
		//����Ĵ������� ���=���Ĵ�����ǰ����AND And_Mask��OR(Or_mask AND And_mask)//
		//��Ӧλ��0����ҪAnd_Mask,Or_mask��λ��Ϊ0
		//��Ӧλ��1����ҪAnd_mask��ӦλΪ0��Or_mask��ӦλΪ1
		//���ֲ����λ��And_maskΪ1��Or_mask��λ��0
/*
		if (WriteRequest.m_WriteByteAddrIndex%2==0) //�ֽ�˳��Ϊż������Modbus����ֽ�����Ϊ�Ĵ����ĸ��ֽ�
		{
			BitOffset = WriteRequest.m_ByteOffset + 8;
		}
		else //�����ֽ�λ���ּĴ����ĵ��ֽ�//
		{
			BitOffset = WriteRequest.m_ByteOffset;
		}

//�Թ�����ķ�ʽд���ּĴ�����ĳһλ
		And_mask = 0xFFFF;
		Or_mask = 0;
		if ((BOOL)WriteRequest.m_SetValue==TRUE) //��Ӧλ��1
		{
			And_mask &= ~(1 << BitOffset);
			Or_mask |= (1 << BitOffset);
		}
		else //��Ӧλ��0��And_mask��Or_mask��Ӧλ��Ϊ0. 
		{
			And_mask &= ~(1 << BitOffset);
			Or_mask &= ~(1 << BitOffset);
		}

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x08;
		tempSendFrame.m_BSendBuff[6] = 0xff;             
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MASK_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (And_mask & 0xff00) >> 8;  //And_mask���θ�λ
		tempSendFrame.m_BSendBuff[11] = And_mask & 0x00ff; //And_mask���ε�λ//
		tempSendFrame.m_BSendBuff[12] = (Or_mask & 0xff00) >> 8;  //Or_mask���θ�λ
		tempSendFrame.m_BSendBuff[13] = Or_mask & 0x00ff; //Or_mask���εص�λ//
		tempSendFrame.m_nLen = 14;
*/

//��д�Ĵ�������ķ�ʽд��Ĵ�����λ 2015/09/11
	    
		StartRegAddr =(int)WriteRequest.m_WriteByteAddrIndex/2;

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //��Ԫ��ʶ����ռһ�ֽڣ�����TCP/IPʱ��ͨ��IP��ַѰַ�����Դ˱�ʶ����������0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;


		if (WriteRequest.m_WriteByteAddrIndex % 2 == 0)
		{
			BitOffset = WriteRequest.m_ByteOffset;				
		}
		else //�����ֽ�λ���ּĴ����ĵ��ֽ�//
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
		tempSendFrame.m_BSendBuff[11] = wValue & 0x00ff; //�ֽ���//

		tempSendFrame.m_nLen = 12;

		break;
	case CPlcWriteRequest::ADDR_TYPE_BYTE:
/*
		if (WriteRequest.m_WriteByteAddrIndex%2==0) //ż����Ϊ����16λ�Ĵ����ĸ��ֽ�//
		{
			And_mask = 0x00FF;
			Or_mask = 0;
			Or_mask |= ((WriteRequest.m_SetValue & 0xFF) << 8);	
		}
		else //������16λ�Ĵ����ĵ��ֽ�//
		{
			And_mask = 0xFF00;
			Or_mask = 0;
			Or_mask |= (WriteRequest.m_SetValue & 0xFF);
		}
		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x08;
		tempSendFrame.m_BSendBuff[6] = 0xff;
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MASK_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (And_mask & 0xff00) >> 8;  //And_mask���θ��ֽ�
		tempSendFrame.m_BSendBuff[11] = And_mask & 0x00ff; //And_mask���ε��ֽ�//
		tempSendFrame.m_BSendBuff[12] = (Or_mask & 0xff00) >> 8;  //Or_mask���θ��ֽ�
		tempSendFrame.m_BSendBuff[13] = Or_mask & 0x00ff; //Or_mask���εص�λ//

		tempSendFrame.m_nLen = 14;
*/

		StartRegAddr = (int)(WriteRequest.m_WriteByteAddrIndex/2);

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //��Ԫ��ʶ����ռһ�ֽڣ�����TCP/IPʱ��ͨ��IP��ַѰַ�����Դ˱�ʶ����������0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;


		if (WriteRequest.m_WriteByteAddrIndex % 2 == 0) //�ֽ�˳��Ϊż������Modbus����ֽ�����Ϊ�Ĵ����ĸ��ֽ�
		{
			ByteOffset = 0;
		}
		else //�����ֽ�λ���ּĴ����ĵ��ֽ�//
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

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x06;
		tempSendFrame.m_BSendBuff[6] = 0xff;              //��Ԫ��ʶ����ռһ�ֽڣ�����TCP/IPʱ��ͨ��IP��ַѰַ�����Դ˱�ʶ����������0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_SINGLE_HOLD_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = (WriteRequest.m_SetValue & 0x0000ff00) >> 8;
		tempSendFrame.m_BSendBuff[11] = WriteRequest.m_SetValue & 0x000000ff; //�ֽ���//
		
		tempSendFrame.m_nLen = 12;

		break;
	case CPlcWriteRequest::ADDR_TYPE_DWORD:		//�ڴ�ֻ��FloatΪ˫�����ͣ�����float
		
		StartRegAddr = WriteRequest.m_WriteByteAddrIndex*2;

		tempSendFrame.m_BSendBuff[0] = WriteRequest.m_PlcId >> 8; //�������ʶ�����ߵ�λ��ռ���ֽ�//
		tempSendFrame.m_BSendBuff[1] = WriteRequest.m_PlcId;
		tempSendFrame.m_BSendBuff[2] = 0x00;               //Э���ʶ�����ߵ�λ��ռ���ֽڣ�������Ϊ0x0000
		tempSendFrame.m_BSendBuff[3] = 0x00;
		tempSendFrame.m_BSendBuff[4] = 0x00;
		tempSendFrame.m_BSendBuff[5] = 0x0B;              //Э���������ݳ��ȣ�//
		tempSendFrame.m_BSendBuff[6] = 0xFF;              //��Ԫ��ʶ����ռһ�ֽڣ�����TCP/IPʱ��ͨ��IP��ַѰַ�����Դ˱�ʶ����������0xff
		tempSendFrame.m_BSendBuff[7] = FCT_WRITE_MULTIPLE_REG;  //���ܴ���,ռһ�ֽڣ������궨��//
		tempSendFrame.m_BSendBuff[8] = StartRegAddr >> 8;   //��ʼ�Ĵ�����ַ���ߵ�λ��ռ���ֽ�
		tempSendFrame.m_BSendBuff[9] = StartRegAddr;
		tempSendFrame.m_BSendBuff[10] = 0;            //�Ĵ�������Hi//  
		tempSendFrame.m_BSendBuff[11] = 2;			//�Ĵ�������Lo//  
		tempSendFrame.m_BSendBuff[12] = 4; //�ֽ���//
		tempSendFrame.m_BSendBuff[13] = WriteRequest.GetFloatValueByte(1);
		tempSendFrame.m_BSendBuff[14] = WriteRequest.GetFloatValueByte(0); 
		tempSendFrame.m_BSendBuff[15] = WriteRequest.GetFloatValueByte(3);
		tempSendFrame.m_BSendBuff[16] = WriteRequest.GetFloatValueByte(2);
		tempSendFrame.m_nLen = 17; //д�볤��
		break;
	default:
		break;
	}

	return tempSendFrame;
}


