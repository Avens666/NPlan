//  File:        NProjectTaskBoard.cpp
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
#include "NProjectTaskBoard.h"

CNProjectTaskBoard::CNProjectTaskBoard( void )
{
}

CNProjectTaskBoard::~CNProjectTaskBoard( void )
{
}

vector <CNProjectTaskBoardItem> & CNProjectTaskBoard::getBoardItems()
{
    return m_board;
}

void CNProjectTaskBoard::setBoardItems( const vector <CNProjectTaskBoardItem> &_board )
{
    m_board.clear();
    std::copy( _board.begin(), _board.end(), std::back_inserter( m_board ));
    sort( m_board.begin(), m_board.end() );
}

bool CNProjectTaskBoard::setBoardItems( const kn_char* _itemstr )
{
    vector <kn_string> vitem = splitString2Vect( _itemstr );

    //列表为空
    if ( vitem.empty() )
    {
        return false;
    }

    m_board.clear();
    vector <kn_string>::iterator it;
    for ( it = vitem.begin(); it != vitem.end(); ++it )
    {
        CNProjectTaskBoardItem bi(it->c_str(), START_TASK_STATE_ID-1/*无效值*/ );
        addBoardItem(bi);
    }

    return true;
}

//返回新加对象的ID
int CNProjectTaskBoard::addBoardItem( CNProjectTaskBoardItem& item )
{
    if ( m_board.empty() )
    {
        item.setId( START_TASK_STATE_ID );
        m_board.push_back( item );
        return START_TASK_STATE_ID;
    }
    else if ( item.getId() == START_TASK_STATE_ID-1 )
    {
        item.setId( getUnusedBoardID(m_board) );
        m_board.push_back( item );
    }
    else
    {
        m_board.push_back( item );
    }
    return item.getId();

#if 0 //初步判定为无效代码
     //ID自己维护
    if ( item.getId() > m_board.size()) //大于vector长度的pushback
    {
        item.setId( m_board.size() + 1 );

        m_board.push_back( item );

        return m_board.size();
    }
    else       //小于vector长度的,后面的id自加,然后pushback,再排序
    {
        int vsize = m_board.size();
        for ( int i = item.getId(); i <= m_board.size(); i++ )
        {
            m_board[ i-1].setId( i+1 ) ;
        }

        m_board.push_back( item );
        sort( m_board.begin(), m_board.end() );
        return item.getId();
    }
#endif

}

kn_string CNProjectTaskBoard::getBoardItemName( int _boardindex )
{
    kn_string str = _T( "" );

    if (_boardindex < 0 || _boardindex >= m_board.size())
    {
        return str;
    }

    CNProjectTaskBoardItem& bditem = m_board.at( _boardindex );
    return bditem.getName();
}

#if 0 //废弃代码
bool CNProjectTaskBoard::changeItemPos( int srcid, int desid )
{
    if ((srcid > m_board.size()) || (desid > m_board.size()) || (srcid == desid))
    {
        return false;
    }

    if ((srcid < 1) || (desid < 1))
    {
        return false;
    }

    CNProjectTaskBoardItem& bditem1 = m_board.at( srcid-1 );
    CNProjectTaskBoardItem& bditem2 = m_board.at( desid-1 );
    kn_string tempname = bditem1.getName();
    bditem1.setName( bditem2.getName().c_str());
    bditem2.setName( tempname.c_str());
    return true;
}
#endif

int CNProjectTaskBoard::findIdByName( const kn_char* _name )
{
    kn_string name;
    //board列表为空或者name为空
    if ((m_board.empty()) || (NULL == _name) || (_name[0] == _T( '\0' )))
    {
        return 0;
    }

    if( NULL != _name )
    {
        name = _name;
    }

    int index = 0;
    vector <CNProjectTaskBoardItem>::iterator it;
    for ( it = m_board.begin(); it != m_board.end(); ++it )
    {
        if (it->getName() == name )
        {
            index = it->getId();
            break;
        }
    }

    return index;
}


