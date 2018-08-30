//  File:        NGlobleTaskBoard.h
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
#ifndef _NPROJECT_GLOBLE_TASKBOARDITEM_H
#define _NPROJECT_GLOBLE_TASKBOARDITEM_H
#include "NProjectTaskBoardItem.h"
#include "NProjectDataStruct.h"
#include "utility.h"
#include "FilePath.h"
#include "NProjectObject.h"

//全局任务墙
class CNGloblePrjTaskBoard : public CNProjectObject
{
public:
    vector <CNProjectTaskBoardItem> & getBoardItems();

    void setBoardItems( vector <CNProjectTaskBoardItem> &_board );

    //增加一个Item
    int addBoardItem( CNProjectTaskBoardItem& item );

    //获取任务墙中id对应的名称
    kn_string getBoardItemName( int _boardindex );

private:

    vector <CNProjectTaskBoardItem> m_board;
};

class CNGlobleTaskBoard
{
public:

    CNGlobleTaskBoard( void );

    ~CNGlobleTaskBoard( void );

    static CNGlobleTaskBoard* getGlobleTaskBoardInstance()
    {
        if ( m_pInstance == NULL )  //判断是否第一次调用
        {
            m_pInstance = new CNGlobleTaskBoard();
        }

        return m_pInstance;
    }

    int loadFile( kn_string &filename );

    int saveFile( kn_string &filename );

    vector <CNGloblePrjTaskBoard > &getGlobleTaskBoard();

    void addTaskBoard( vector <CNProjectTaskBoardItem> &boaraditem, const kn_char* _name, int id );

    //增加一个任务墙
    void addTaskBoard( CNGloblePrjTaskBoard& board );

    //增加一个Item
    void addItem( int id, CNProjectTaskBoardItem& board );

    //获取任务墙名称
    kn_string getBoardNameById( int id );

    //获取任务墙Item的名称
    kn_string getItemNameById( int id, int _boardid );

private:

    static CNGlobleTaskBoard * m_pInstance;

    std::vector <CNGloblePrjTaskBoard > m_globleboard;
};

#endif
