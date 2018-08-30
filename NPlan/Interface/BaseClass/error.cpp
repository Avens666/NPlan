//  File:        error.cpp
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
* Copyright (C) 2005-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KoteiNaviSysError.h
* 摘    要：导航系统错误分类及代码标识.
*
* 版    本：1.0
* 创建日期：2010.02.26
* 作    者: 黄志方
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#include "NE_pubhead.h"
#include "NE_error.h"

KNRESULT g_LatestErrorCode = RESULT_OK;

// 取得最后错误码
KNRESULT GetLastErrorCode()
{
    return g_LatestErrorCode;
}

// 设置最后错误码
void SetLastErrorCode(KNRESULT theErrorCode)
{
    g_LatestErrorCode = theErrorCode;
}

kn_bool operator==(const KNRESULT& lhs, const KNRESULT& rhs)
{
    if (lhs.dwClass == rhs.dwClass && lhs.dwCode == rhs.dwCode && lhs.dwComponent == rhs.dwComponent && lhs.dwFacility == rhs.dwFacility)
    {
        return true;
    }
    else
    {
        return false;
    }
}

kn_bool operator!=(const KNRESULT& lhs, const KNRESULT& rhs)
{
    return !(lhs == rhs);
}
