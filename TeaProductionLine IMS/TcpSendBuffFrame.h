#pragma once
class CTcpSendBuffFrame
{
public:
	CTcpSendBuffFrame();
	~CTcpSendBuffFrame();
	int m_nLen;
	BYTE m_BSendBuff[50];
};

