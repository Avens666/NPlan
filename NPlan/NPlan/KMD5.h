//  File:        KMD5.h
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
/**
* Copyright (C) 2003-2007，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KMD5.h
* 摘    要：MD5算法的实现
* 版    本：1.0.0
* 修改日期：2007-5-30
* 作    者：diwens
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/


#ifndef ___KMD5_H___
#define ___KMD5_H___

#include "NE_pubhead.h"

/* Typedef a 32 bit type */
#ifndef kn_UINT4
typedef unsigned long int kn_UINT4;
#endif

class KMD5
{
	
	/* Data structure for MD5 (Message Digest) computation */
	typedef struct 
	{
		kn_UINT4 i[2];                   /* Number of _bits_ handled mod 2^64 */
		kn_UINT4 buf[4];                                    /* Scratch buffer */
		kn_byte  in[64];                              /* Input buffer */
		kn_byte  digest[16];     /* Actual digest after MD5Final call */
	} MD5_CTX;
	
	// Basic MD5 step. MD5_Transform buf based on in
	void MD5_Transform (kn_UINT4* buf, kn_UINT4* in);
	// Set pseudoRandomNumber to zero for RFC MD5 implementation
	void MD5Init(MD5_CTX* pMd5Ctx, kn_ulong ulPseudoRandomNumber = 0);
	// AES 128-bit block encryption routine
	void MD5Update(MD5_CTX* pMd5Ctx, kn_byte* pbyInBuf, kn_uint inLen);
	// AES 128-bit block encryption routine
	void MD5Final(MD5_CTX* pMd5Ctx);
	
public:

	//	计算缓冲区数据的MD5值, 返回16位BYTES值
	kn_int MD5Hash (kn_byte* pbyBuf, kn_int nBufLen, kn_byte md5Value[16]);

	//	把BYTE[16] 转换成16进制32位字符串
	kn_int Byte16ToChar32(kn_byte bytes[16], char str[33]);
	
};

#endif /* ___KMD5_H___ included */
