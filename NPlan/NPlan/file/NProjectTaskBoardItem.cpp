//  File:        NProjectTaskBoardItem.cpp
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
#include "NProjectTaskBoardItem.h"

CNProjectTaskBoardItem::CNProjectTaskBoardItem( void )
{
    setId(START_TASK_STATE_ID-1); //默认为无效值
}

CNProjectTaskBoardItem::CNProjectTaskBoardItem( const kn_char * _name, int _id ):CNProjectObject(_name, _id)
{
}

CNProjectTaskBoardItem::~CNProjectTaskBoardItem( void )
{
}

//获取未使用的任务状态编号，注意此处编号从0开始(其它对象编号从1开始)
int getUnusedBoardID(vector <CNProjectTaskBoardItem>& v_boards)
{
    vector <CNProjectTaskBoardItem>::iterator iter;
    vector <int> vecids;
    for ( iter = v_boards.begin(); iter != v_boards.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.empty() )
    {
        return START_TASK_STATE_ID;
    }
    else
    {
        for ( int i = 0; i < vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))  //没找到
            {
                return i;
            }
        }

        return ( vecids.size() );
    }
}
