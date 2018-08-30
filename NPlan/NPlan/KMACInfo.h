//  File:        KMACInfo.h
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
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <memory>

#include "NE_pubhead.h"

#ifdef WIN32
#include "windows.h"
#include "tchar.h"
#endif

using namespace std;

class KMACInfo
{
public:
	KMACInfo(void);
	~KMACInfo(void);
	//----------------------------------------------------
	// MAC �ṹ���� for VC6
	//	VS2005 �¿��� #include <Iphlpapi.h> & link Iphlpapi.lib
	//
	//

	#define   MAX_ADAPTER_DESCRIPTION_LENGTH	128   //   arb.   
	#define   MAX_ADAPTER_NAME_LENGTH			256   //   arb.   
	#define   MAX_ADAPTER_ADDRESS_LENGTH		8  	 //   arb.  
	//   
	//   IP_ADDRESS_STRING - store an IP address as a dotted decimal string   
	//   
	typedef   struct
	{
		char String[4 * 4];
	}   IP_ADDRESS_STRING, * PIP_ADDRESS_STRING, IP_MASK_STRING, * PIP_MASK_STRING;   

	//   
	//   IP_ADDR_STRING - store an IP address with its corresponding subnet mask,   
	//   both as dotted decimal strings   
	//   
	typedef struct _IP_ADDR_STRING
	{
		struct _IP_ADDR_STRING* Next;   
		IP_ADDRESS_STRING IpAddress;   
		IP_MASK_STRING IpMask;   
		DWORD Context;
	}   IP_ADDR_STRING, * PIP_ADDR_STRING;   


	//   
	//   ADAPTER_INFO   -   per-adapter   information.   All   IP   addresses   are   stored   as   
	//   strings   
	//   
	typedef struct _IP_ADAPTER_INFO
	{
		struct _IP_ADAPTER_INFO* Next;   
		DWORD ComboIndex;   
		char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];   
		char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];   
		UINT AddressLength;   
		BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];   
		DWORD Index;   
		UINT Type;   
		UINT DhcpEnabled;   
		PIP_ADDR_STRING CurrentIpAddress;   
		IP_ADDR_STRING IpAddressList;   
		IP_ADDR_STRING GatewayList;   
		IP_ADDR_STRING DhcpServer;   
		BOOL HaveWins;   
		IP_ADDR_STRING PrimaryWinsServer;   
		IP_ADDR_STRING SecondaryWinsServer;   
		time_t LeaseObtained;   
		time_t LeaseExpires;
	}   IP_ADAPTER_INFO, * PIP_ADAPTER_INFO;   

	//	����ָ��, �õ�������Ϣ
	typedef DWORD (WINAPI* GetAdaptersInfoFunc)(PIP_ADAPTER_INFO pAdapterInfo,     //   buffer to receive data   
		PULONG pOutBufLen 							  //   size of data returned   
		);  
	//	�õ�����MAC, for VC6 
	int GetAdapterByDLL(BYTE* bySystemInfo, UINT& uSystemInfoLen);

private:

	//	�õ���һ��������MAC��ַ
	int GetMACInfo(BYTE* byMACInfo, UINT& uMACLen, GetAdaptersInfoFunc GetAdaptersInfo);

	//	��Byteת����16���Ʊ�ʾ, 
	void FormatBytetoHex(const BYTE* byteSrc, char* strDes, int iSrcLength);
};

