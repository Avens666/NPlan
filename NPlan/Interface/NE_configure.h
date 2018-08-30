//  File:        NE_configure.h
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
* 文件名称：configure.h
* 摘    要：编译预处理相关配置
*
* 版    本：1.0
* 创建日期：2010/03/09
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _NE_CONFIGURE_H_
#define _NE_CONFIGURE_H_

#define _LINUX_PLATFORM		0x01
#define _WIN32_PLATFORM		0x02
#define _WINCE_PLATFORM		0x03

// OS Platform
#if defined(_WIN32_WCE)
	#define _CURRENT_PLATFORM _WINCE_PLATFORM
	#ifndef WIN32
		#define WIN32
	#endif
#elif defined(_WINDOWS)
    #define _CURRENT_PLATFORM _WIN32_PLATFORM
    #ifndef WIN32
        #define WIN32
    #endif
#else
    #define _CURRENT_PLATFORM _LINUX_PLATFORM
    #define LINUX
#endif // OS Platform

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

    #include <windows.h>
    #include <tchar.h>
#endif

// C++ Compiler
#if   _CURRENT_PLATFORM == _LINUX_PLATFORM
    #ifndef KOTEI_GCC
		#include "LINUXTCHAR.h"
        #define KOTEI_GCC
    #endif
#elif _CURRENT_PLATFORM == _WIN32_PLATFORM
    #ifndef KOTEI_VC
        #define KOTEI_VC
    #endif
#elif _CURRENT_PLATFORM == _WINCE_PLATFORM
    #ifndef KOTEI_EVC
        #define KOTEI_EVC
    #endif
#endif // C++ Compiler

// 定义DEBUG和_DEBUG宏
#if defined(KOTEI_EVC) && defined(DEBUG) && !defined(_DEBUG)
    #define _DEBUG
#endif

#if defined(KOTEI_VC) && defined(_DEBUG) && !defined(DEBUG)
    #define DEBUG
#endif // DEBUG

#ifdef IOS_DEV
	#include "LINUXTCHAR.h"
#endif
#endif // _NE_CONFIGURE_H_
