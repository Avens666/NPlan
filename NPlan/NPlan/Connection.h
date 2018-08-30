//  File:        Connection.h
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
#ifndef CONNECTION_H
#define CONNECTION_H
#include <WinSock2.h>
#include <string>
#include <vector>
using namespace std;
class Connection
{
public:
	Connection();
	~Connection();

public:
	int SetAddr(struct sockaddr_in addr);
	bool Connect();
	void Close();
	int RecvData(char *buf, int n);
	int SendData(const char *buf, int n);
	bool GetLine(string& line);

private:
	struct sockaddr_in m_addr;
	int m_sockfd;
};

#endif //CONNECTION_H
