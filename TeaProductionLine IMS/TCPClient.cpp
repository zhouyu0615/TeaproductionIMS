#include "stdafx.h"
#include "TCPClient.h"
#include <atlconv.h>
#include <afxpriv.h>

CTCPClient::CTCPClient()
{
	//��ʼ��Socket����
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 1), &wd);
	m_nPort = 502;
	m_bIsconnected = FALSE;
	m_bIsopen = FALSE;
	m_PlcId = 0;
	//�����˳��߳��¼��ľ��,��ʼ״̬Ϊ���źţ��Զ���ԭ��������ҪReestEvent�������ֹ���ԭ�ź�����
	//	m_hExitThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//��ȡ�����ھ��
	m_pWndOwner = AfxGetMainWnd();
	//��ʼ��ȫ��������
	m_pDataP = CDataProvider::getInstance();
	m_pThread = NULL;
	InitializeCriticalSection(&m_RBCS); //��ʼ���ٽ���
}


CTCPClient::~CTCPClient()
{
	//�ͷ�Socket��Դ
	WSACleanup();
	//�ر��˳��߳��¼��ľ��
	//	SetEvent(m_hExitThreadEvent);
	//	DeleteCriticalSection(&m_RBCS);
}


void CTCPClient::CreatePollThread()
{
	m_pThread = new CThread(this);
	m_pThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);

}

void CTCPClient::StartPollThread()
{
	m_pThread->Start();
	m_pThread->SetThreadName(m_strRemoteHost);
}
void CTCPClient::ResumePollThread()
{
	m_pThread->Resume();
}
void CTCPClient::SuspendPollThread()
{
	m_pThread->Suspend();
}

void CTCPClient::Run()
{
	fd_set fdRead;
	int nRet;
	//�����¼��ȴ�ʱ��
	TIMEVAL tvTime;
	tvTime.tv_sec = 1;
	tvTime.tv_usec = 0;
	while (TRUE)
	{
		if (m_bIsconnected == FALSE)
		{
			TRACE("����ʧ��");
			Sleep(500);
			continue;
		}
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
			char *pRead =(char *) tempReadFrame.m_BSendBuff;
			m_ReadFrameQueue.PushBack(tempReadFrame); //����һ��ѭ�����У��Ѷ��е�һ��Ԫ�طŵ�����ĩβ//
			SendData(pRead, tempReadFrame.m_nLen);		
		}


		//�յ��˳��¼��������߳�
		//if (WaitForSingleObject(m_hExitThreadEvent, 0) == WAIT_OBJECT_0)
		//{
		//	break;
		//}
		//��fdRead�¼�Ϊ��
		FD_ZERO(&fdRead);
		//���ͻ���Socket���ö��¼�
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
			m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
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
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
					//break;
				}
				else if (nRecvLen == 0)
				{
					//�����Ͽ������¼�
					TRACE("���ݽ��յȴ������������ж�\n");
					//�رտͻ���socket
					closesocket(m_Socket);

					m_bIsconnected = FALSE;
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
					//break;
				}
				else
				{
					//�������ݽ����¼�����sendmessage���ƽ����յ����鷢�͵����״���
					//SetReadDataPacket(m_strRemoteHost, cRecvBuf);
					DealRecvData(cRecvBuf);
					m_bIsconnected = TRUE;
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);
					TRACE("%d ���ݽ��ճɹ�\n", m_pThread->GetThreadID());
				}
			}
		}

		Sleep(50);
	}


}

//============================================//
//���ܣ����ڴ򿪿ͻ���Socket
//��ڣ���
//���ڣ���
//���أ�TRUE--�򿪳ɹ���FALSE--��ʧ��
//============================================//
bool CTCPClient::Open()
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
bool CTCPClient::Connect()
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
	//�������ӵ�Socket�ͻ��ˣ�����Ҫbind()
	//����ͬ�����ӷ�ʽ��connectֱ�ӷ��سɹ�����ʧ��
	nErr = connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr));
	if (nErr == SOCKET_ERROR)
	{
		TRACE("����ʧ��\n");
		m_bIsconnected = FALSE;
		m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
		//Close();
		return FALSE;
	}
	//����ͨѶģʽΪ�첽ģʽ
	DWORD dwUl = 1;
	ioctlsocket(m_Socket, FIONBIO, &dwUl);
	TRACE("���ӳɹ�\n");
	m_bIsconnected = TRUE;
	m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);

	return TRUE;
}



//============================================//
//���ܣ����ڹرտͻ���Socket
//��ڣ���
//���ڣ���
//���أ�TRUE--�رճɹ���FALSE--�ر�ʧ��
//============================================//
bool CTCPClient::Close()
{
	//����ͨѶ�߳̽����¼�
	//	SetEvent(m_hExitThreadEvent);
	//	Sleep(100);
	//�ر�Socket���ͷ���Դ
	int nErr = closesocket(m_Socket);
	if (nErr == SOCKET_ERROR)
	{
		return FALSE;
	}
	m_bIsconnected = FALSE;
	m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
	return TRUE;
}

//============================================//
//���ܣ���������˷�������
//��ڣ�c_pcSendBuf:�����͵�����
//      nLen:���������ݵĳ���
//���ڣ���
//���أ�TRUE���������ݳɹ���FALSE����������ʧ��
//============================================//
bool CTCPClient::SendData(const char *p_SendBuff, int BuffLen)
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
		m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
		return FALSE;
	}

	TRACE("�������ݳɹ�\n");
	m_bIsconnected = TRUE;
	m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);
	return TRUE;
}



//�����ɹ����յ����ַ������뵽m_vRecvBuff�У���MODBUS�����ݷַ��߳̽��ж�ȡ�ַ�
void CTCPClient::DealRecvData(char* pcRcvData)
{
	EnterCriticalSection(&m_RBCS);
	m_vRecvBuff.clear();
	for (int i = 0; i < RECEIVE_BUFF_SIZE; i++)
	{
		m_vRecvBuff.push_back(*pcRcvData++);
	}
	LeaveCriticalSection(&m_RBCS);


}








//==================================================================//
//���ܣ���ȡ��ǰPLC�ڹ������������е�λ������
//��ڣ���
//���ڣ���
//���أ�����ֵ��ǰPLC����
//==================================================================//
int CTCPClient::GetPlcClassIndex()
{
	int nIndex=0;
	for (int i = 0; i < m_pDataP->m_vectPlc.size(); i++)
	{
		if (m_pDataP->m_vectPlc[i].m_strIPAddr == m_strRemoteHost)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}