//  File:        KLog.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/24
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KLog.h
* 摘    要：日志类
*
* 版    本：1.0
* 创建日期：2010/04/01
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _KLOG_H_
#define _KLOG_H_

void BeginLog();

void EndLog();

void WriteLogNoTime(const char *szFormat, ...);

void WriteLog(const char *szFormat, ...);

void LogAssert(char* szFileName, long iLine, char* szFuncName);

void MyTrace(LPCTSTR lpsz, ...);
//  auto log
class AutoLog
{
private:

    char m_szFuncName[256];
public:

    AutoLog(const char* szFuncName);


     ~AutoLog();

};

#if defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)
void LogInfo(const char *szFormat, ...);

void LogInfo(const wchar_t *szFormat, ...);
#endif

#define LOG WriteLog
#define LOG_ASSERT LogAssert
#define LOGNT WriteLogNoTime
#define LOG_FUNC AutoLog logAutoFuncc (__FUNCTION__) ;

#endif // _KLOG_H_
