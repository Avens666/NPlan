//  File:        KHWRjt.h
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
* Copyright (C) 2003-2008，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KHWRpc.h
* 摘    要：对话框基类
*
* 版    本：3.0.0
* 创建日期：2008.4.9
* 作    者：xiaojingd
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _KHWRPC_H_
#define _KHWRPC_H_

#include "IHandwritingRecog.h"
#include <vector>


typedef std::vector<HWPOINT> HWPOINTLIST;

extern "C" IHandwritingRecog * GetHWRSinglton();

class KHWRjt : public IHandwritingRecog
{
public:
	KHWRjt();
	virtual ~KHWRjt();
	
	// 设置参数
	void SetParam(const HWRPARAM* pParam);

	// 初始化识别引擎
	kn_bool InitRecog();
	
	// 退出识别
	void ExitRecog();
	
	// 开始一次新的识别(一个字)
	kn_bool BeginRecog();
	
 
	// 处理输入，进行识别
	kn_string Process(const short* ppt, kn_int iCount);
	
  

private:
	HWRPARAM m_hwrParam;
 
	kn_string m_strResults;

	kn_bool m_b_init;
};

#endif // _KHWRPC_H_
