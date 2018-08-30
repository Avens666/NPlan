//  File:        NProjectTaskBoardItem.h
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
#ifndef NProjectTaskBoardItem_h__
#define NProjectTaskBoardItem_h__

#include "utility.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "NProjectObject.h"

using namespace boost:: posix_time;

//注意此处编号从0开始(其它对象编号从1开始)
#ifndef START_TASK_STATE_ID
#define START_TASK_STATE_ID 0
#endif

class CNProjectTaskBoardItem : public CNProjectObject
{
public:
    CNProjectTaskBoardItem( void );

    CNProjectTaskBoardItem( const kn_char * _name, int _id = START_TASK_STATE_ID-1 /*默认为无效值*/ );

    ~CNProjectTaskBoardItem( void );
};

//获取未使用的任务状态编号，注意此处编号从0开始(其它对象编号从1开始)
int getUnusedBoardID(vector <CNProjectTaskBoardItem>& v_boards);

#endif
