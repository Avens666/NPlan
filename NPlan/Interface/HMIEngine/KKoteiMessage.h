//  File:        KKoteiMessage.h
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
* 文件名称：KKoteiMessage.cpp
* 摘    要：
*
* 版    本：1.0
* 创建日期：15-01-2014 10:12:35
* 作    者：jiangfengh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/


#ifndef __KKOTEI_MESSAGE_H__
#define __KKOTEI_MESSAGE_H__

#include "NE_pubhead.h"
#include "message.h"

//#define WM_NAVI_BASE (WM_USER + 1000)	//	消息起始
//enum KKoteiMessage
//{
//	WM_NAVI_HMI_SETSPECBOOK_FINISHED = 1,		//	通知HMI设置特殊收藏点完成
//    WM_NAVI_GET_POSITION,
//    WM_NAVI_SET_POSITION
//};


kn_dword SendKMessage(KMessage* pMsg);
kn_dword PostKMessage(KMessage* pMsg);
KMessage* GetKMessage();



// 自定义消息队列
class KMsgQueue
{
public:

	// 加入用户消息
    void AddUserMessage(KMessage* pMsg);

	// 获取用户消息
    KMessage* GetUserMessage();

 

protected:

    // 自定义消息列表
    deque<KMessage*> m_dqUserMsg;


	// 操作消息的互斥
    //KMutex m_mutex;
    boost::shared_mutex m_mutex;
 

};


#endif // __KKOTEI_MESSAGE_H__
