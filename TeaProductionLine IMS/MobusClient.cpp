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
	//��ʼ��ȫ��������_

	m_strRemoteHost = m_Plc->m_strIPAddr;

	m_pDataP = CDataProvider::getInstance();
	m_pWorkThread = NULL;
	InitializeCriticalSection(&m_RBCS); //��ʼ���ٽ���

}

CMobusClient::~CMobusClient()
{
	//�ͷ�Socket��Դ
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

//�̹߳�������//
void CMobusClient::Run()
{
	fd_set fdRead;
	int nRet;
	//�����¼��ȴ�ʱ��
	TIMEVAL tvTime;
	tvTime.tv_sec = 1;
	tvTime.tv_usec = 0;
	while (TRUE)
	{
		if (m_bIsconnected == FALSE) //û�н�������
		{
			TRACE("����ʧ��");
			Close();
			Open();
			Connect();
			if (m_bIsconnected == FALSE)
			{
				Sleep(500);
				continue;
			}
		}
		else  //�������ӣ�����ͨ��//
		{
			//��д�����������ʱ�����Ȱ�����д�뵽PLCȥ//
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
				m_ReadFrameQueue.PushBack(tempReadFrame); //����һ��ѭ�����У��Ѷ��е�һ��Ԫ�طŵ�����ĩβ//
				SendData(pRead, tempReadFrame.m_nLen);
			}

			//��fdRead�¼�Ϊ��
			FD_ZERO(&fdRead);
			FD_SET(this->m_Socket, &fdRead);
			//����select�������ж��Ƿ��ж��¼�����
			nRet = select(32, &fdRead, NULL, NULL, &tvTime);

			if (nRet == SOCKET_ERROR)
			{
				WSAGetLastError();
				//��ȡ�ļ�������ʧ��
				TRACE("��ȡ�ļ�������ʧ��\n");
				//�رտͻ���socket
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
					//���fdset��ϵ���ļ����fd�Ƿ�ɶ�д����>0��ʾ�ɶ�д����
					//�������¼�
					char cRecvBuf[RECEIVE_BUFF_SIZE];
					int nRecvLen;
					ZeroMemory(cRecvBuf, RECEIVE_BUFF_SIZE);
					TRACE("���ݶ�ȡ�¼�������ִ�ж�����\n");
					//��������
					nRecvLen = recv(m_Socket, cRecvBuf, RECEIVE_BUFF_SIZE, 0);
					if (nRecvLen == SOCKET_ERROR)
					{
						int nError = WSAGetLastError();
						//���ݽ��ղ���ʧ��
						TRACE("���ݽ��ղ���ʧ��\n");
						//�رտͻ���socket
						closesocket(m_Socket);
						m_bIsconnected = FALSE;
						m_Plc->SetConectedState(FALSE);
						//break;
					}
					else if (nRecvLen == 0)
					{					
						TRACE("���ݽ��յȴ������������ж�\n");					
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

						TRACE("%d ���ݽ��ճɹ�\n", m_pWorkThread->GetThreadID());
					}
				}
			}
			Sleep(50);
		}//end of else
	}
}

//============================================//
//���ܣ����ڴ򿪿ͻ���Socket
//��ڣ���
//���ڣ���
//���أ�TRUE--�򿪳ɹ���FALSE--��ʧ��
//============================================//
bool CMobusClient::Open()
{
	//��λ�߳��˳��¼�
	//	ResetEvent(m_hExitThreadEvent);
	//����TCP�׽���
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
//���ܣ����ڽ�����TCP����������
//��ڣ���
//���ڣ���
//���أ�TRUE--�������ӳɹ���FALSE--��������ʧ��
//============================================//
bool CMobusClient::Connect()
{
	struct sockaddr_in addr;
	int nErr;
	char cAnsiRemoteHost[255];

	//��λ��ǰ���ӵ�PLC�ڹ����������е�����λ��

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	//�˴�Ҫ��˫�ֽ�ת���ɵ��ֽ�
	ZeroMemory(cAnsiRemoteHost, 255);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_strRemoteHost, wcslen(m_strRemoteHost), cAnsiRemoteHost, wcslen(m_strRemoteHost), NULL, NULL);
	addr.sin_addr.S_un.S_addr = inet_addr(cAnsiRemoteHost);

	//�������ӳ�ʱʱ��//
	DWORD TimeOut = 600;
	setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//�������ӵ�Socket�ͻ��ˣ�����Ҫbind()
	//����ͬ�����ӷ�ʽ��connectֱ�ӷ��سɹ�����ʧ��
	nErr = connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr));
	if (nErr == SOCKET_ERROR)
	{
		TRACE("����ʧ��\n");
		m_bIsconnected = FALSE;
		m_Plc->SetConectedState(FALSE);
		//Close();
		return FALSE;
	}
	//����ͨѶģʽΪ�첽ģʽ
	DWORD dwUl = 1;
	ioctlsocket(m_Socket, FIONBIO, &dwUl);
	TRACE("���ӳɹ�\n");
	m_bIsconnected = TRUE;
	m_Plc->SetConectedState(TRUE);

	return TRUE;
}



//============================================//
//���ܣ����ڹرտͻ���Socket
//���أ�TRUE--�رճɹ���FALSE--�ر�ʧ��
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
//���ܣ���Plc�˷�������
//��ڣ�c_pcSendBuf:�����͵�����
//      nLen:���������ݵĳ���
//���ڣ���
//���أ�TRUE���������ݳɹ���FALSE����������ʧ��
//============================================//
bool CMobusClient::SendData(const char *p_SendBuff, int BuffLen)
{
	int nBytes = 0;

	nBytes = send(m_Socket, p_SendBuff, BuffLen, 0);
	if (nBytes == SOCKET_ERROR)
	{
		int nErrorCode = WSAGetLastError();
		//��������ʧ��
		TRACE("��������ʧ��\n");
		//�ر�Socket
		Close();
		m_bIsconnected = FALSE;
		m_Plc->SetConectedState(FALSE);
		return FALSE;
	}

	TRACE("�������ݳɹ�\n");
	m_bIsconnected = TRUE;
	m_Plc->SetConectedState(TRUE);
	return TRUE;
}



//�����ɹ����յ����ַ������뵽m_vRecvBuff�У���MODBUS�����ݷַ��߳̽��ж�ȡ�ַ�
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


//�ַ������ɹ����ӵ�TCP�̶߳�ȡ��������//
void CMobusClient::DispatchReadResponse()
{
	int nSizeOfResponseReadData = 0;   //�ֽڸ���//
	int sizeOfRecvBuff=0;
	
		sizeOfRecvBuff = m_vRecvBuff.size();
		if (m_bIsconnected&&sizeOfRecvBuff > 9) //��ǰ�ͻ��˳ɹ�����
		{
			if (m_vRecvBuff[7] == FCT_READ_HOLD_REG)
			{
				//��������Ĵ����Ƕ��������ܷ��صĴ���//
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

				//�Ѹ�PLC����Ĳ������ɵ�����������¼��//
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

	int nFrameCount = 0; //��Ҫ��������֡����Ŀ//
	int FrameStartAddr;  //֡��ȡ����ʼ��ַ//
	int FrameReadLength; //֡��ȡ�������ֽڳ���//

	//plcClass.m_ReadLength���ֽڸ�����MODBUS���ּĴ��������ֶ�����Ҫת��Ϊ�ֽ�
	int SumReadLen = m_Plc->m_ReadLength;  //�ܹ�����Ҫ��д�ĳ���//
	int NumOfReg = 0;  //��ȡ�ļĴ�������//

	nFrameCount = SumReadLen / MAX_READ_BYTE_LEN;
	if (SumReadLen > nFrameCount*MAX_READ_BYTE_LEN)
	{
		nFrameCount++;
	} //nFrameCount������ȡ��


	FrameStartAddr = m_Plc->m_ReadStartAddr / 2;
	//��	PLC�����ȡ���ݶ�ȡ����//
	for (int index = 0; index < nFrameCount; index++)
	{
		FrameStartAddr = FrameStartAddr + NumOfReg;
		if (SumReadLen > MAX_READ_BYTE_LEN)//���ʣ��ķ����ֽڻ�����ÿһ֡�����������ֽ���//
		{
			SumReadLen = SumReadLen - MAX_READ_BYTE_LEN;
			FrameReadLength = MAX_READ_BYTE_LEN;
		}
		else //���򣬰�ʣ�����Ҫ�����ֽ���һ���Է���
		{
			FrameReadLength = SumReadLen;
		}

		if (FrameReadLength % 2 == 0) //ż��
		{
			NumOfReg = FrameReadLength / 2;
		}
		else
		{
			NumOfReg = FrameReadLength / 2 + 1;
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

		m_ReadFrameQueue.PushBack(tempReadFrame);
	}

}


//��WriteRequest������MODBUSЭ��ת��ΪModbusЭ��֡//
CTcpSendBuffFrame CMobusClient::ConstructModbusWriteFrame(CPlcWriteRequest& WriteRequest)
{
	CTcpSendBuffFrame tempSendFrame;
	int And_mask;
	int Or_mask;
	int StartRegAddr; //��ʼ�Ĵ���//
	int BitOffset = 0;   //λ����ʱ��λƫ��//
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

		StartRegAddr = (int)WriteRequest.m_WriteByteAddrIndex / 2;

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

		StartRegAddr = (int)(WriteRequest.m_WriteByteAddrIndex / 2);

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

		StartRegAddr = WriteRequest.m_WriteByteAddrIndex * 2;

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



