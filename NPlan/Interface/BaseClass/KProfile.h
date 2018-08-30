//  File:        KProfile.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/2/27
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KProfile.h
* 摘    要：配置文件读取类
*
* 版    本：1.0
* 创建日期：2010/03/29
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _KPROFILE_H__
#define _KPROFILE_H__

#include "KFile.h"

class KNAPI KProfile  
{
public:
	KProfile();
	virtual ~KProfile();

	// 打开配置文件
    kn_int Open(const kn_string& strFilename);
	// 关闭配置文件
    void Close(void);

	// 复位文件位置
    kn_int Reset();
	// 保存文件当前位置
    kn_int Mark();
	// 返回先前保存的位置
    kn_int Back();

	// 取得Section名称
    kn_int GetSection(const char* pKey);
	// 读取指定名称的字符串
    kn_int GetString(const char* pKey, char* pBuff, kn_int nCount);
	// 读取指定名称的整型值
    kn_int GetValue(const char* pKey, kn_int& nVvalue);
    // 读取指定名称的整型值
    kn_int GetValue(const char* pKey, kn_ulong& nVvalue);
	// 读取指定名称的双精度值
    kn_int GetValue(const char* pKey, kn_double& dValue);

	// 拆分字符串
    static void Split(const char *pBuff, ...);

	// 读取一行
	kn_int GetLine(char* pBuff, kn_int nCount);
protected:

	// 字符串比较，不区分大小写
    kn_int strnicmp2(const char* pStr1, const char* pStr2, kn_int nCount) const;

    KFile m_File;		// 配置文件
    kn_ulong m_ulMark;	// 保存文件读取的位置
};


#ifdef NAVI_ENGINE
 
#endif

#endif // _KPROFILE_H__
