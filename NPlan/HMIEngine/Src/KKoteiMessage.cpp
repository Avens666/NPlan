//  File:        KKoteiMessage.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         2
//  Created:     2018/8/27
//  Last edit:   2018/8/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KoteiMessage.h
* 摘    要：
*
* 版    本：1.0
* 创建日期：15-01-2014 10:12:35
* 作    者：jiangfengh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/
 

#include "KKoteiMessage.h"
#include "DataSync.h"



// 添加用户消息
void KMsgQueue::AddUserMessage(KMessage* pMsg)
{
    writeLock lock(m_mutex);
    m_dqUserMsg.push_back(pMsg);

}

// 获取用户消息
KMessage* KMsgQueue::GetUserMessage()
{
	//	快速判断
	if(m_dqUserMsg.empty())
	{
		return false;
	}



    //KAutoMutex amLock(m_mutex);

    writeLock lock(m_mutex);

    KMessage* pMsg = NULL;

	if(!m_dqUserMsg.empty())
	{
        pMsg = m_dqUserMsg.front();
        m_dqUserMsg.pop_front();

	}

    return pMsg;

}
 
// 自定义消息队列
KMsgQueue g_UserMsgQueue;


//extern kn_hwnd g_hWndNavi;
kn_dword SendKMessage(KMessage* pMsg)
{
    return PostKMessage(pMsg);


}

kn_dword PostKMessage(KMessage* pMsg)
{

    g_UserMsgQueue.AddUserMessage(pMsg);

	return 0;
}


KMessage* GetKMessage()
{

    return g_UserMsgQueue.GetUserMessage();
}
 
