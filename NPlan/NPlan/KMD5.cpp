//  File:        KMD5.cpp
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
* 文件名称：KMD5.cpp
* 摘    要：MD5算法的实现

* 版    本：1.0.0
* 修改日期：2007-5-30
* 作    者：diwens

* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "KMD5.h"

/* Padding */
static kn_byte MD5_PADDING[64] = 
{
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* MD5_F, MD5_G and MD5_H are basic MD5 functions: selection, majority, parity */
#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#ifndef ROTATE_LEFT
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#endif

/* MD5_FF, MD5_GG, MD5_HH, and MD5_II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define MD5_FF(a, b, c, d, x, s, ac) {(a) += MD5_F ((b), (c), (d)) + (x) + (kn_UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_GG(a, b, c, d, x, s, ac) {(a) += MD5_G ((b), (c), (d)) + (x) + (kn_UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_HH(a, b, c, d, x, s, ac) {(a) += MD5_H ((b), (c), (d)) + (x) + (kn_UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_II(a, b, c, d, x, s, ac) {(a) += MD5_I ((b), (c), (d)) + (x) + (kn_UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

/* Constants for transformation */
#define MD5_S11 7  /* Round 1 */
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5  /* Round 2 */
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4  /* Round 3 */
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6  /* Round 4 */
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

/*
* 函数功能：Basic MD5 step. MD5_Transform buf based on in
* 参    数：
* 		buf [in]：
*		in []：
* 返 回 值：无
*/
void KMD5::MD5_Transform(kn_UINT4* buf, kn_UINT4* in)
{
	kn_UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	MD5_FF ( a, b, c, d, in[ 0], MD5_S11, (kn_UINT4) 3614090360u); /* 1 */
	MD5_FF ( d, a, b, c, in[ 1], MD5_S12, (kn_UINT4) 3905402710u); /* 2 */
	MD5_FF ( c, d, a, b, in[ 2], MD5_S13, (kn_UINT4)  606105819u); /* 3 */
	MD5_FF ( b, c, d, a, in[ 3], MD5_S14, (kn_UINT4) 3250441966u); /* 4 */
	MD5_FF ( a, b, c, d, in[ 4], MD5_S11, (kn_UINT4) 4118548399u); /* 5 */
	MD5_FF ( d, a, b, c, in[ 5], MD5_S12, (kn_UINT4) 1200080426u); /* 6 */
	MD5_FF ( c, d, a, b, in[ 6], MD5_S13, (kn_UINT4) 2821735955u); /* 7 */
	MD5_FF ( b, c, d, a, in[ 7], MD5_S14, (kn_UINT4) 4249261313u); /* 8 */
	MD5_FF ( a, b, c, d, in[ 8], MD5_S11, (kn_UINT4) 1770035416u); /* 9 */
	MD5_FF ( d, a, b, c, in[ 9], MD5_S12, (kn_UINT4) 2336552879u); /* 10 */
	MD5_FF ( c, d, a, b, in[10], MD5_S13, (kn_UINT4) 4294925233u); /* 11 */
	MD5_FF ( b, c, d, a, in[11], MD5_S14, (kn_UINT4) 2304563134u); /* 12 */
	MD5_FF ( a, b, c, d, in[12], MD5_S11, (kn_UINT4) 1804603682u); /* 13 */
	MD5_FF ( d, a, b, c, in[13], MD5_S12, (kn_UINT4) 4254626195u); /* 14 */
	MD5_FF ( c, d, a, b, in[14], MD5_S13, (kn_UINT4) 2792965006u); /* 15 */
	MD5_FF ( b, c, d, a, in[15], MD5_S14, (kn_UINT4) 1236535329u); /* 16 */

	/* Round 2 */
	MD5_GG ( a, b, c, d, in[ 1], MD5_S21, (kn_UINT4) 4129170786u); /* 17 */
	MD5_GG ( d, a, b, c, in[ 6], MD5_S22, (kn_UINT4) 3225465664u); /* 18 */
	MD5_GG ( c, d, a, b, in[11], MD5_S23, (kn_UINT4)  643717713u); /* 19 */
	MD5_GG ( b, c, d, a, in[ 0], MD5_S24, (kn_UINT4) 3921069994u); /* 20 */
	MD5_GG ( a, b, c, d, in[ 5], MD5_S21, (kn_UINT4) 3593408605u); /* 21 */
	MD5_GG ( d, a, b, c, in[10], MD5_S22, (kn_UINT4)   38016083u); /* 22 */
	MD5_GG ( c, d, a, b, in[15], MD5_S23, (kn_UINT4) 3634488961u); /* 23 */
	MD5_GG ( b, c, d, a, in[ 4], MD5_S24, (kn_UINT4) 3889429448u); /* 24 */
	MD5_GG ( a, b, c, d, in[ 9], MD5_S21, (kn_UINT4)  568446438u); /* 25 */
	MD5_GG ( d, a, b, c, in[14], MD5_S22, (kn_UINT4) 3275163606u); /* 26 */
	MD5_GG ( c, d, a, b, in[ 3], MD5_S23, (kn_UINT4) 4107603335u); /* 27 */
	MD5_GG ( b, c, d, a, in[ 8], MD5_S24, (kn_UINT4) 1163531501u); /* 28 */
	MD5_GG ( a, b, c, d, in[13], MD5_S21, (kn_UINT4) 2850285829u); /* 29 */
	MD5_GG ( d, a, b, c, in[ 2], MD5_S22, (kn_UINT4) 4243563512u); /* 30 */
	MD5_GG ( c, d, a, b, in[ 7], MD5_S23, (kn_UINT4) 1735328473u); /* 31 */
	MD5_GG ( b, c, d, a, in[12], MD5_S24, (kn_UINT4) 2368359562u); /* 32 */

	/* Round 3 */
	MD5_HH ( a, b, c, d, in[ 5], MD5_S31, (kn_UINT4) 4294588738u); /* 33 */
	MD5_HH ( d, a, b, c, in[ 8], MD5_S32, (kn_UINT4) 2272392833u); /* 34 */
	MD5_HH ( c, d, a, b, in[11], MD5_S33, (kn_UINT4) 1839030562u); /* 35 */
	MD5_HH ( b, c, d, a, in[14], MD5_S34, (kn_UINT4) 4259657740u); /* 36 */
	MD5_HH ( a, b, c, d, in[ 1], MD5_S31, (kn_UINT4) 2763975236u); /* 37 */
	MD5_HH ( d, a, b, c, in[ 4], MD5_S32, (kn_UINT4) 1272893353u); /* 38 */
	MD5_HH ( c, d, a, b, in[ 7], MD5_S33, (kn_UINT4) 4139469664u); /* 39 */
	MD5_HH ( b, c, d, a, in[10], MD5_S34, (kn_UINT4) 3200236656u); /* 40 */
	MD5_HH ( a, b, c, d, in[13], MD5_S31, (kn_UINT4)  681279174u); /* 41 */
	MD5_HH ( d, a, b, c, in[ 0], MD5_S32, (kn_UINT4) 3936430074u); /* 42 */
	MD5_HH ( c, d, a, b, in[ 3], MD5_S33, (kn_UINT4) 3572445317u); /* 43 */
	MD5_HH ( b, c, d, a, in[ 6], MD5_S34, (kn_UINT4)   76029189u); /* 44 */
	MD5_HH ( a, b, c, d, in[ 9], MD5_S31, (kn_UINT4) 3654602809u); /* 45 */
	MD5_HH ( d, a, b, c, in[12], MD5_S32, (kn_UINT4) 3873151461u); /* 46 */
	MD5_HH ( c, d, a, b, in[15], MD5_S33, (kn_UINT4)  530742520u); /* 47 */
	MD5_HH ( b, c, d, a, in[ 2], MD5_S34, (kn_UINT4) 3299628645u); /* 48 */

	/* Round 4 */
	MD5_II ( a, b, c, d, in[ 0], MD5_S41, (kn_UINT4) 4096336452u); /* 49 */
	MD5_II ( d, a, b, c, in[ 7], MD5_S42, (kn_UINT4) 1126891415u); /* 50 */
	MD5_II ( c, d, a, b, in[14], MD5_S43, (kn_UINT4) 2878612391u); /* 51 */
	MD5_II ( b, c, d, a, in[ 5], MD5_S44, (kn_UINT4) 4237533241u); /* 52 */
	MD5_II ( a, b, c, d, in[12], MD5_S41, (kn_UINT4) 1700485571u); /* 53 */
	MD5_II ( d, a, b, c, in[ 3], MD5_S42, (kn_UINT4) 2399980690u); /* 54 */
	MD5_II ( c, d, a, b, in[10], MD5_S43, (kn_UINT4) 4293915773u); /* 55 */
	MD5_II ( b, c, d, a, in[ 1], MD5_S44, (kn_UINT4) 2240044497u); /* 56 */
	MD5_II ( a, b, c, d, in[ 8], MD5_S41, (kn_UINT4) 1873313359u); /* 57 */
	MD5_II ( d, a, b, c, in[15], MD5_S42, (kn_UINT4) 4264355552u); /* 58 */
	MD5_II ( c, d, a, b, in[ 6], MD5_S43, (kn_UINT4) 2734768916u); /* 59 */
	MD5_II ( b, c, d, a, in[13], MD5_S44, (kn_UINT4) 1309151649u); /* 60 */
	MD5_II ( a, b, c, d, in[ 4], MD5_S41, (kn_UINT4) 4149444226u); /* 61 */
	MD5_II ( d, a, b, c, in[11], MD5_S42, (kn_UINT4) 3174756917u); /* 62 */
	MD5_II ( c, d, a, b, in[ 2], MD5_S43, (kn_UINT4)  718787259u); /* 63 */
	MD5_II ( b, c, d, a, in[ 9], MD5_S44, (kn_UINT4) 3951481745u); /* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

/*
* 函数功能：Set pseudoRandomNumber to zero for RFC MD5 implementation
* 参    数：
* 		pMd5Ctx [out]：MD5内容
*		ulPseudoRandomNumber []：
* 返 回 值：无
*/
void KMD5::MD5Init(MD5_CTX* pMd5Ctx, kn_ulong ulPseudoRandomNumber)
{
	pMd5Ctx->i[0] = pMd5Ctx->i[1] = (kn_UINT4)0;

	/* Load magic initialization constants */
	pMd5Ctx->buf[0] = (kn_UINT4)0x67452301 + (ulPseudoRandomNumber * 11);
	pMd5Ctx->buf[1] = (kn_UINT4)0xefcdab89 + (ulPseudoRandomNumber * 71);
	pMd5Ctx->buf[2] = (kn_UINT4)0x98badcfe + (ulPseudoRandomNumber * 37);
	pMd5Ctx->buf[3] = (kn_UINT4)0x10325476 + (ulPseudoRandomNumber * 97);
}

/*
* 函数功能：AES 128-bit block encryption routine
* 参    数：
* 		pMd5Ctx [out]：
*		pbyInBuf []：
*		inLen []：
* 返 回 值：无
*/
void KMD5::MD5Update(MD5_CTX* pMd5Ctx, kn_byte* pbyInBuf, kn_uint inLen)
{
	kn_UINT4 in[16];
	kn_int   nMdi = 0;
	kn_uint  i = 0, ii = 0;

	/* Compute number of bytes mod 64 */
	nMdi = (kn_int)((pMd5Ctx->i[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((pMd5Ctx->i[0] + ((kn_UINT4)inLen << 3)) < pMd5Ctx->i[0])
	{
		pMd5Ctx->i[1]++;
	}
	pMd5Ctx->i[0] += ((kn_UINT4)inLen << 3);
	pMd5Ctx->i[1] += ((kn_UINT4)inLen >> 29);

	while (inLen--)
	{
		/* Add new character to buffer, increment mdi */
		pMd5Ctx->in[nMdi++] = *pbyInBuf++;

		/* Transform if necessary */
		if (nMdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
			{
				in[i] = (((kn_UINT4)pMd5Ctx->in[ii+3]) << 24) |
					(((kn_UINT4)pMd5Ctx->in[ii+2]) << 16) |
					(((kn_UINT4)pMd5Ctx->in[ii+1]) << 8) |
					((kn_UINT4)pMd5Ctx->in[ii]);
			}
			MD5_Transform(pMd5Ctx->buf, in);
			nMdi = 0;
		}
	}
}

/*
* 函数功能：AES 128-bit block encryption routine
* 参    数：
* 		pMd5Ctx [in]：
* 返 回 值：无
*/
void KMD5::MD5Final(MD5_CTX* pMd5Ctx)
{
	kn_UINT4 in[16];
	kn_int   nMdi = 0;
	kn_uint  i = 0, ii = 0, padLen = 0;

	/* Save number of bits */
	in[14] = pMd5Ctx->i[0];
	in[15] = pMd5Ctx->i[1];

	/* Compute number of bytes mod 64 */
	nMdi = (kn_int)((pMd5Ctx->i[0] >> 3) & 0x3F);

	/* Pad out to 56 mod 64 */
	padLen = (nMdi < 56) ? (56 - nMdi) : (120 - nMdi);
	MD5Update (pMd5Ctx, MD5_PADDING, padLen);

	/* Append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
	{
		in[i] = (((kn_UINT4)pMd5Ctx->in[ii+3]) << 24) |
			(((kn_UINT4)pMd5Ctx->in[ii+2]) << 16) |
			(((kn_UINT4)pMd5Ctx->in[ii+1]) <<  8) |
			((kn_UINT4)pMd5Ctx->in[ii]);
	}
	MD5_Transform (pMd5Ctx->buf, in);

	/* Store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4)
	{
		pMd5Ctx->digest[ii]   = (kn_byte)( pMd5Ctx->buf[i]        & 0xFF);
		pMd5Ctx->digest[ii+1] = (kn_byte)((pMd5Ctx->buf[i] >>  8) & 0xFF);
		pMd5Ctx->digest[ii+2] = (kn_byte)((pMd5Ctx->buf[i] >> 16) & 0xFF);
		pMd5Ctx->digest[ii+3] = (kn_byte)((pMd5Ctx->buf[i] >> 24) & 0xFF);
	}
}

/***********************************************************************
* 函数功能：计算缓冲区数据的MD5值, 返回16位BYTES值
* 参    数：
* 		pbyBuf [in]：缓冲区的数据
* 		nBufLen [in]：缓冲区的大小
*		md5Value [out]：MD5值,
* 返 回 值：0表示成功
***********************************************************************/
kn_int KMD5::MD5Hash(kn_byte* pbyBuf, kn_int nBufLen, kn_byte md5Value[16])
{
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, pbyBuf, nBufLen);
	MD5Final(&context);
	memcpy(md5Value, context.digest, 16);
	return 0;
}

/***********************************************************************
* 函数功能：把BYTE[16] 转换成16进制32位字符串
* 参    数：
* 		bytes [in]：BYTE 型MD5值
* 		str [out]：6进制32位字符串MD5值, 如 3C4CAEF2CA3C89047846FA4EBF062A5F
* 返 回 值：0表示成功
***********************************************************************/
kn_int KMD5::Byte16ToChar32(kn_byte bytes[16], char str[33])
{
	memset(str, 0 , 33);
	sprintf(str, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7],
		 bytes[8], bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]);

	return 0;

}
