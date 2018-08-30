//  File:        NE_debug.h
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
* 文件名称：debug.h
* 摘    要：调试用的相关头文件声明
*
* 版    本：1.0
* 创建日期：2010/03/09
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _NE_DEBUG_H_
#define _NE_DEBUG_H_

#include <assert.h>

// Define ASSERT
#ifndef ASSERT
    
    #ifdef NDEBUG // for release
//         #define ASSERT(expr) \
//             if (!(expr)) LOG_ASSERT(__FILE__, __LINE__, __FUNCTION__, #expr);
			#if defined(QT_LINUX_LIB)
				#define  ASSERT(X) Q_ASSERT(X)
			#else
				#define  ASSERT(X)
			#endif
    #else
        #ifdef KOTEI_VC
            #include <crtdbg.h>
            #define ASSERT(X) _ASSERT(X)
        #else
			#define _ASSERT(X) assert(X) 
            #define ASSERT(X) assert(X)
        #endif // KOTEI_VC
    #endif	//	NDEBUG
#endif	//	ASSERT

// DEFINE _SYSTEMMONITOR
//#ifdef _DEBUG
//    #ifdef WIN32
//		#ifndef _SYSTEMMONITOR
//		#define _SYSTEMMONITOR // 启用监测系统
//		#endif
//    #endif
//#endif

#endif // _NE_DEBUG_H_
