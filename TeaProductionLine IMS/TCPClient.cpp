#include "stdafx.h"
#include "TCPClient.h"
#include <atlconv.h>
#include <afxpriv.h>

CTCPClient::CTCPClient()
{
	//初始化Socket环境
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 1), &wd);
	m_nPort = 502;
	m_bIsconnected = FALSE;
	m_bIsopen = FALSE;
	m_PlcId = 0;
	//创建退出线程事件的句柄,初始状态为无信号，自动复原（即不需要ReestEvent函数来手工复原信号量）
	//	m_hExitThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//获取父窗口句柄
	m_pWndOwner = AfxGetMainWnd();
	//初始化全局数据类
	m_pDataP = CDataProvider::getInstance();
	m_pThread = NULL;
	InitializeCriticalSection(&m_RBCS); //初始化临界区
}


CTCPClient::~CTCPClient()
{
	//释放Socket资源
	WSACleanup();
	//关闭退出线程事件的句柄
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
	//定义事件等待时间
	TIMEVAL tvTime;
	tvTime.tv_sec = 1;
	tvTime.tv_usec = 0;
	while (TRUE)
	{
		if (m_bIsconnected == FALSE)
		{
			TRACE("连接失败");
			Sleep(500);
			continue;
		}
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
			char *pRead =(char *) tempReadFrame.m_BSendBuff;
			m_ReadFrameQueue.PushBack(tempReadFrame); //构成一个循环队列，把队列第一个元素放到队列末尾//
			SendData(pRead, tempReadFrame.m_nLen);		
		}


		//收到退出事件，结束线程
		//if (WaitForSingleObject(m_hExitThreadEvent, 0) == WAIT_OBJECT_0)
		//{
		//	break;
		//}
		//置fdRead事件为空
		FD_ZERO(&fdRead);
		//给客户端Socket设置读事件
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
			m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
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
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
					//break;
				}
				else if (nRecvLen == 0)
				{
					//触发断开连接事件
					TRACE("数据接收等待过程中网络中断\n");
					//关闭客户端socket
					closesocket(m_Socket);

					m_bIsconnected = FALSE;
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
					//break;
				}
				else
				{
					//触发数据接收事件采用sendmessage机制将接收的数组发送到父亲窗口
					//SetReadDataPacket(m_strRemoteHost, cRecvBuf);
					DealRecvData(cRecvBuf);
					m_bIsconnected = TRUE;
					m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);
					TRACE("%d 数据接收成功\n", m_pThread->GetThreadID());
				}
			}
		}

		Sleep(50);
	}


}

//============================================//
//功能：用于打开客户端Socket
//入口：无
//出口：无
//返回：TRUE--打开成功；FALSE--打开失败
//============================================//
bool CTCPClient::Open()
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
bool CTCPClient::Connect()
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
	//面向连接的Socket客户端，不需要bind()
	//采用同步连接方式，connect直接返回成功或者失败
	nErr = connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr));
	if (nErr == SOCKET_ERROR)
	{
		TRACE("连接失败\n");
		m_bIsconnected = FALSE;
		m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
		//Close();
		return FALSE;
	}
	//设置通讯模式为异步模式
	DWORD dwUl = 1;
	ioctlsocket(m_Socket, FIONBIO, &dwUl);
	TRACE("连接成功\n");
	m_bIsconnected = TRUE;
	m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);

	return TRUE;
}



//============================================//
//功能：用于关闭客户端Socket
//入口：无
//出口：无
//返回：TRUE--关闭成功；FALSE--关闭失败
//============================================//
bool CTCPClient::Close()
{
	//发送通讯线程结束事件
	//	SetEvent(m_hExitThreadEvent);
	//	Sleep(100);
	//关闭Socket，释放资源
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
//功能：向服务器端发送数据
//入口：c_pcSendBuf:待发送的数据
//      nLen:待发送数据的长度
//出口：无
//返回：TRUE；发送数据成功；FALSE：发送数据失败
//============================================//
bool CTCPClient::SendData(const char *p_SendBuff, int BuffLen)
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
		m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(FALSE);
		return FALSE;
	}

	TRACE("发送数据成功\n");
	m_bIsconnected = TRUE;
	m_pDataP->m_vectPlc[GetPlcClassIndex()].SetConectedState(TRUE);
	return TRUE;
}



//处理到成功接收到的字符，移入到m_vRecvBuff中，待MODBUS的数据分发线程进行读取分发
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
//功能：获取当前PLC在公共数据容器中的位置索引
//入口：无
//出口：无
//返回：返回值当前PLC索引
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