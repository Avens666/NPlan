//  File:        UserCountSuggestion.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/24
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "UserCountSuggestion.h"
#include "KMACInfo.h"
#include "KMD5.h"
using namespace std;

UserCountSuggestion::UserCountSuggestion()
{
	/*api.map.baidu.com*/
	char addr[50];
	memset(addr, 0, 50);
	sprintf(addr, "%d.%d.%d.%d", 121,43,153,190);
	m_str_host = addr; //"192.168.5.47";
	ParseHost();
}

UserCountSuggestion::~UserCountSuggestion()
{
}

/* ����������ȡ��ip��ַ */
bool UserCountSuggestion::ParseHost()
{
	struct hostent *hptr = NULL;
	struct sockaddr_in addr;

	/* ����������ȡ��Ӧ��ip��ַ */
	if((hptr = gethostbyname(m_str_host.c_str())) == NULL)
	{
		return false;
	}
	//addr.sin_addr.S_addr=hptr->h_addr_list[0];
	addr.sin_addr = *((struct in_addr*)hptr->h_addr);

	addr.sin_family = AF_INET;	//ָ��ͨ��ip����ͨ��
	addr.sin_port = htons(803);	//HTTPĬ�϶˿���80������ת���������ʽ�����浽m_Addr��

	/* �������Ӷ���ĵ�ַ */
	m_conn.SetAddr(addr);

	return true;
}

bool UserCountSuggestion::SetRequest()
{
	const int BUFFER_SIZE = 1024;
	const int BUFFER_TOTAL_SIZE = 4096;
	// ��ȡmac��ַ
	BYTE byMAC[BUFFER_SIZE];
	UINT uMacLen = 0;
	memset (byMAC, 0, BUFFER_SIZE);
	KMACInfo macInfo;
	macInfo.GetAdapterByDLL(byMAC, uMacLen);

	// ��ȡmd5ֵ
	BYTE byDeviceID[BUFFER_TOTAL_SIZE] = {0};
	UINT uDeviceIDLen = 0;
	BYTE szMD5[32] = {0};
	KMD5 md;
	memcpy(byDeviceID, byMAC, uMacLen);
	uDeviceIDLen = uMacLen;
	memcpy(byDeviceID + uMacLen,  "koteiNui", 8);
	uDeviceIDLen += 8;
	md.MD5Hash (byDeviceID, uDeviceIDLen, szMD5);
	char szMD5Out [128]= {0}; 
	md.Byte16ToChar32 (szMD5, szMD5Out);

	// start_time
	SYSTEMTIME m_time;
	GetLocalTime(&m_time);
	char cur_time[30];
	_snprintf_s(cur_time, sizeof(cur_time), "%d-%d-%d%%20%d:%d:%d", m_time.wYear, m_time.wMonth, m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond); 
	
	//http://dangkui/index.php?g=admin&m=Nui&a=statistics&mac=50-E5-49-1D-AE-9E&md5=B11DB1C56E312CA12421A26AA30E4FE5&start_time=2015-07-23 11:30:27

	const char *format = 
		"GET /index.php?g=admin&m=Nui&a=statistics&mac=%s&md5=%s&start_time=%s HTTP/1.1\r\n" \
		"Host: dangkui\r\n" \
		"Connection: keep-alive\r\n" \
		"Cache-Control: max-age=0\r\n" \
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" \
		"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.111 Safari/537.36\r\n" \
		"Accept-Encoding: gzip,deflate,sdch\r\n" \
		"Accept-Language: zh-CN,zh;q=0.8\r\n" \
		"\r\n";

	char dest[10240];
	_snprintf_s(dest, sizeof(dest), format, byMAC, szMD5Out, cur_time);
	m_str_request = dest;

	return true;
}

int UserCountSuggestion::QueryRequest()
{
	int ret = 0;
	std::string response;

	/* ͬ�������������� */
	if(!m_conn.Connect())
	{
		return (-1);
	}

	/* ����Http�������� */
	SetRequest();

	/* ����Http�������� */
	ret = m_conn.SendData(m_str_request.c_str(), m_str_request.length());
	if(ret < 0)
	{
		return (-1);
	}


	//string line;
	//while(1)
	//{
	//	m_conn.GetLine(line);
	//	if(line == "")
	//	{
	//		break;
	//	}
	//}
	//char buf[100];
	//ret = m_conn.RecvData(buf, 100);

	m_conn.Close();
	return 0;
}
