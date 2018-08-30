//  File:        NProjectTaskBoard.h
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
#ifndef NProjectTaskBoard_h__
#define NProjectTaskBoard_h__

#include "utility.h"
#include "NProjectTaskBoardItem.h"
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost:: posix_time;

class CNProjectTaskBoard
{
public:
    CNProjectTaskBoard( void );

    ~CNProjectTaskBoard( void );

private:

    //任务墙Item列表
    vector <CNProjectTaskBoardItem> m_board;

public:

    vector <CNProjectTaskBoardItem> & getBoardItems();

    void setBoardItems( const vector <CNProjectTaskBoardItem> &_board );

    bool setBoardItems( const kn_char* _itemstr );

    //增加一个Item
    int addBoardItem( CNProjectTaskBoardItem& _boarditem );

    //获取任务墙中id对应的名称
    kn_string getBoardItemName( int _boardindex );

#if 0 //废弃代码
    //互换任务墙两个item的位置
    bool changeItemPos( int srcid, int desid );
#endif

    //根据TaskBoardItem名称查找对应的ID
    int findIdByName( const kn_char* _name );
};

#endif
