//  File:        KMACInfo.cpp
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
#include "KMACInfo.h"

//	��ӦASCII�ĵ������
void static MyTraceA(char* strFormat, ...)
{
#ifdef _DEBUG
	const int BUFFER_SIZE = 4096;
	TCHAR szBuffer [BUFFER_SIZE];
	memset (szBuffer, 0, sizeof (szBuffer));
	char   strBuffer [BUFFER_SIZE] = {0};

	va_list pArgList ;

	// The va_start macro (defined in STDARG.H) is usually equivalent to:
	// pArgList = (char *) &szFormat + sizeof (szFormat) ;
	va_start (pArgList, strFormat) ;

	// The last argument to wvsprintf points to the arguments
	_vsnprintf ( strBuffer, sizeof (strBuffer) / sizeof (char), strFormat, pArgList) ;

	// The va_end macro just zeroes out pArgList for no good reason
	va_end (pArgList);
	int result = mbstowcs(szBuffer, strBuffer, strlen (strBuffer)); 
	OutputDebugString (szBuffer);

#endif
}

KMACInfo::KMACInfo(void)
{
}


KMACInfo::~KMACInfo(void)
{
}

int KMACInfo::GetAdapterByDLL( BYTE* bySystemInfo, UINT& uSystemInfoLen )
{
	HMODULE hLib;   
	GetAdaptersInfoFunc GetAdaptersInfo = NULL;   

	PIP_ADAPTER_INFO pai = NULL;   
	DWORD dwSize = 0;   
	// 	CHAR szMac[64];   

	hLib = LoadLibrary(_T("Iphlpapi.dll"));   
	if (NULL == hLib)
	{
		MyTraceA("Failed to load Iphlpapi.dll\n");   
		return -11;
	}   

	GetAdaptersInfo = (GetAdaptersInfoFunc) GetProcAddress(hLib, "GetAdaptersInfo");   
	if (NULL == GetAdaptersInfo)
	{
		MyTraceA("Failed to load GetAdaptersInfo in Iphlpapi.dll\n");
		return -12;
	}   

	int iRet = GetMACInfo(bySystemInfo, uSystemInfoLen, GetAdaptersInfo);

	FreeLibrary(hLib);

	return iRet;
}

int KMACInfo::GetMACInfo( BYTE* byMACInfo, UINT& uMACLen, GetAdaptersInfoFunc GetAdaptersInfo )
{
	int iRet = -1;
	uMACLen = 0;

	const DWORD MAC_SIZE = 256;


	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	DWORD dwRetVal = -1;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen = 0U;

	int iAdapterCount = 0;	//	������

	//	�õ��������ĳ���
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);


		//	�õ�������Ϣ
		if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;

			//	�������
			if (pAdapter != NULL)
			{
					//	��ȡ��1������
				FormatBytetoHex(pAdapter->Address, (char*)byMACInfo, pAdapter->AddressLength);
				uMACLen = strlen((const char*)byMACInfo);
				MyTraceA("MAC = %s\n", byMACInfo); 
				MyTraceA("Description = %s\n", pAdapter->Description);   
				MyTraceA("---------------------------------\n");
				iRet = 0;
			}

			if (uMACLen == 0)
			{
				MyTraceA("No Network Adapter!\n");
				iRet = -3;
			}
			else
			{
				MyTraceA("MAC Lenth = %d\n", uMACLen);
			}

		}
		else
		{
			iRet = -2;
			MyTraceA("Call to GetAdaptersInfo failed.\n");
		}
	}

	free(pAdapterInfo);
	pAdapterInfo = NULL;




	return iRet;
}

void KMACInfo::FormatBytetoHex( const BYTE* byteSrc, char* strDes, int iSrcLength )
{
	char* p = strDes;   
	for (int i = 0; i < iSrcLength; i++)
	{
		p += sprintf(p, i ? "-%02X" : "%02X", byteSrc[i]);
	}
}
