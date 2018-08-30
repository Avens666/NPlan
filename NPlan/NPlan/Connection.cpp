//  File:        Connection.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/3
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "Connection.h"
#pragma comment(lib,"WS2_32.lib")

Connection::Connection()
{
	m_sockfd = INVALID_SOCKET;
	memset(&m_addr, 0x00, sizeof(m_addr));
}

Connection::~Connection()
{
}

int Connection::SetAddr(struct sockaddr_in addr)
{
	memcpy(&m_addr, &addr, sizeof(addr));
	return 1;
}

bool Connection::GetLine(string& line)
{
	int readSize = 0;
	int err;
	char c;
	line = "";

	while(1)
	{
		err = recv(m_sockfd, &c, 1, 0);
		if(err == SOCKET_ERROR)
		{
				return false;
		}
		else if(err == 0)
			break;

		if(c == '\r')
		{
			err = recv(m_sockfd, &c, 1, 0);
			if(err == SOCKET_ERROR)
			{
					return false;
			}
			else if(err == 0)
				break;

			if(c == '\n')
				break;

		}
		else if(c == '\n')
			break;

		line += c;
		readSize++;
	}

	return true;
}

bool Connection::Connect()
{
	struct timeval timeo = {30, 0};

	/* ��������TCP���ӵ��׽��� */
	m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sockfd == INVALID_SOCKET)
	{
		return false;
	}

	/* �����׽��ֵķ��ͼ����ճ�ʱ */
	setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeo, sizeof(timeo));
	setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));

	/* �����˷������� */
	if(connect(m_sockfd, (struct sockaddr*)&m_addr, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

void Connection::Close()
{
	if(m_sockfd != INVALID_SOCKET)
	{
		closesocket(m_sockfd);
	}
}
int Connection::RecvData(char *buf, int n)
{
	int nread;
	int nleft = n;
	char* ptr = buf;

	while(nleft > 0)
	{
		if((nread = recv(m_sockfd, ptr, nleft, 0)) == SOCKET_ERROR)
		{
				return (-1);
		}
		else if(nread == 0)
			break;		// EOF

		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}

int Connection::SendData(const char *buf, int n)
{
	int nwritten;
	int nleft = n;
	char* ptr = const_cast<char*>(buf);

	while(nleft > 0)
	{
		if((nwritten = send(m_sockfd, ptr, nleft, 0)) == SOCKET_ERROR)
		{
				return (-1);
		}
		else if(nwritten == 0)
			break;		// EOF

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n - nleft);
}
