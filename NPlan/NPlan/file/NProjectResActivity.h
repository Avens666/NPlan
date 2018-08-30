//  File:        NProjectResActivity.h
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
#ifndef NProjectResActivity_h__
#define NProjectResActivity_h__

#include "NProjectTaskCard.h"
#include "NE_type.h"
#include <vector>
using std::vector;

typedef  vector<CNProjectTaskCardBase*> TaskCard_LST;
//资源所在子任务信息
class CResourceActivity
{
public:
     CResourceActivity();

     bool operator == (const CResourceActivity& other);

     void setResourceName(const kn_string &name);

     kn_string getResourceName() const;

     const TaskCard_LST* getAllTaskCard() const ;

     void addTaskCardRef(CNProjectTaskCard* pActivity);

private:
    kn_string m_resource_name;

    //该资源所属的所有子任务
    TaskCard_LST m_v_pActivity;
};

typedef vector<CResourceActivity> vectResourceActivity;


#endif // NProjectResActivity_h__
