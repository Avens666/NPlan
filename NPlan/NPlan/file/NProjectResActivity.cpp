//  File:        NProjectResActivity.cpp
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
#include "NProjectResActivity.h"

CResourceActivity::CResourceActivity() : m_resource_name(_T(""))
{
}

bool CResourceActivity::operator == (const CResourceActivity& other)
{
    return (other.m_resource_name == m_resource_name);
}

kn_string CResourceActivity::getResourceName() const
{
    return m_resource_name;
}

void CResourceActivity::setResourceName(const kn_string &name)
{
    m_resource_name = name;
}

const TaskCard_LST*  CResourceActivity::getAllTaskCard() const
{
    return (&m_v_pActivity);
}

void CResourceActivity::addTaskCardRef(CNProjectTaskCard* pActivity)
{       
    if (NULL != pActivity)
    {
        m_v_pActivity.push_back(pActivity);
    }
}



