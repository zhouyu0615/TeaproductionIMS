#include "stdafx.h"
#include "TcpSendBuffFrame.h"


CTcpSendBuffFrame::CTcpSendBuffFrame()
{
	m_nLen = 0;
	ZeroMemory(m_BSendBuff, sizeof(m_BSendBuff));
}


CTcpSendBuffFrame::~CTcpSendBuffFrame()
{
}
