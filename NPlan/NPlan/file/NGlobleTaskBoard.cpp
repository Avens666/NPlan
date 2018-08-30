//  File:        NGlobleTaskBoard.cpp
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
#include "NGlobleTaskBoard.h"

CNGlobleTaskBoard * CNGlobleTaskBoard::m_pInstance = NULL;

CNGlobleTaskBoard::CNGlobleTaskBoard( void )
{
}

CNGlobleTaskBoard::~CNGlobleTaskBoard( void )
{
}

int CNGlobleTaskBoard::loadFile( kn_string &filename )
{
    TiXmlDocument *pDoc = NULL;

    string file = ns2s( filename );
    pDoc = new TiXmlDocument( file.c_str());
    if ( !pDoc->LoadFile())
    {
        delete pDoc;
        return ERROR_FILE_INVALID;
    }

    TiXmlElement *pRootElement = pDoc->RootElement();
    if (NULL==pRootElement)
    {
        delete pDoc;
        return ERROR_FILE_INVALID;
    }

    TiXmlElement* pTaskBoardElement = pRootElement->FirstChildElement( "TaskBoard" );
    for ( pTaskBoardElement; pTaskBoardElement != NULL; pTaskBoardElement = pTaskBoardElement->NextSiblingElement())
    {
        CNGloblePrjTaskBoard gboard ;
        gboard.setName( GetAttributeString( pTaskBoardElement, "name" ).c_str());
        gboard.setId( GetAttributeInt( pTaskBoardElement, "id" ));
        TiXmlElement* pItemElement = pTaskBoardElement->FirstChildElement();
        for ( pItemElement; pItemElement != NULL; pItemElement = pItemElement->NextSiblingElement())
        {
            CNProjectTaskBoardItem board ;
            board.setName( GetAttributeString( pItemElement, "name" ).c_str());
            board.setId( GetAttributeInt( pItemElement, "id" ));
            gboard.addBoardItem(board);
        }
        m_globleboard.push_back(gboard);
    }
    delete pDoc;
    return NO_ERROR;
}

int CNGlobleTaskBoard::saveFile( kn_string &filename )
{
    string file = ns2s( filename );
	if (file.length() == 0)
	{
		return ERROR_INVALID_PARAMETER;
	}

    std::replace(filename.begin(), filename.end(), _T('\\'), _T('/'));

	size_t stIndex = filename.rfind(_T( '/' ));
	if ( stIndex != std::string::npos )
	{
		kn_string pathname = filename.substr( 0,stIndex +1);
		if (!IsDirectoryExist(pathname.c_str()))
		{
			return ERROR_PATH_NOT_FOUND;
		}
	}
	else
	{
		return ERROR_PATH_NOT_FOUND;
	}

    TiXmlDocument doc;  
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
    TiXmlElement*m_pRootElement = new TiXmlElement( "NProject" );

    vector <CNGloblePrjTaskBoard >::iterator it;
    for ( it = m_globleboard.begin(); it != m_globleboard.end(); ++it )
    {
        TiXmlElement *boardElement = new TiXmlElement ( "TaskBoard" );
        SetAttributeStr( boardElement, "name", it->getName());
        boardElement->SetAttribute( "id", it->getId());
        vector<CNProjectTaskBoardItem >& bditems=it->getBoardItems();
        vector <CNProjectTaskBoardItem >::iterator iter;
        for(iter=bditems.begin();iter!=bditems.end();++iter)
        {
            TiXmlElement *itemElement = new TiXmlElement ( "Item" );
            SetAttributeStr( itemElement, "name", iter->getName());
            itemElement->SetAttribute( "id", iter->getId());
            boardElement->LinkEndChild(itemElement);
        }
        m_pRootElement->LinkEndChild( boardElement );
    }

    doc.LinkEndChild( decl );

    doc.LinkEndChild( m_pRootElement );

    if ( !doc.SaveFile( file.c_str()) )
    {
        return ERROR_CREATE_FAILED;
    }

    return NO_ERROR;
}

void CNGlobleTaskBoard::addTaskBoard( vector <CNProjectTaskBoardItem> &boaraditem,const kn_char* _name, int id )
{
    CNGloblePrjTaskBoard  board ;
    board.setBoardItems(boaraditem);
    board.setName(_name);
    board.setId(id);
    addTaskBoard(board);
}

void CNGlobleTaskBoard::addTaskBoard( CNGloblePrjTaskBoard&  board )
{
    if ( m_globleboard.empty() )
    {
        board.setId( 1 );
        m_globleboard.push_back( board );
    } 
    //ID自己维护
    if ( board.getId() > m_globleboard.size())//大于vector长度的pushback
    {
        board.setId( m_globleboard.size() + 1 );
        m_globleboard.push_back( board );
    }
    else        //小于vector长度的,后面的id自加,然后pushback,再排序
    {
        int vsize = m_globleboard.size();
        for ( int i = board.getId(); i <= m_globleboard.size(); i++ )
        {
            m_globleboard[i-1].setId( i+1 );
        }
        m_globleboard.push_back( board );
        sort( m_globleboard.begin(), m_globleboard.end() );
    }
}

vector <CNGloblePrjTaskBoard >  &CNGlobleTaskBoard::getGlobleTaskBoard()
{
    return m_globleboard;
}

void CNGlobleTaskBoard::addItem( int id, CNProjectTaskBoardItem& board )
{
    if (id >0 && id <= m_globleboard.size())
    {     
        m_globleboard[id - 1].addBoardItem(board);
    }
}

kn_string CNGlobleTaskBoard::getBoardNameById( int id )
{
    kn_string str = _T( "" );

    if (id >0 && id <= m_globleboard.size())
    {     
        str =  m_globleboard[id - 1].getName();
    }
    return str;
}

kn_string CNGlobleTaskBoard::getItemNameById( int id, int _boardid )
{
    kn_string str = _T( "" );
    if (id >0 && id <= m_globleboard.size())
    {
        str =  m_globleboard[id - 1].getBoardItemName(_boardid);
    }

    return str;
}

vector <CNProjectTaskBoardItem> & CNGloblePrjTaskBoard::getBoardItems()
{
    return m_board;
}

void CNGloblePrjTaskBoard::setBoardItems( vector <CNProjectTaskBoardItem> &_board )
{
    m_board = _board;
}

int CNGloblePrjTaskBoard::addBoardItem( CNProjectTaskBoardItem & item )
{
    if ( m_board.empty() )
    {
        item.setId( START_TASK_STATE_ID );
        m_board.push_back( item );
        return START_TASK_STATE_ID;
    }
    else if ( item.getId() == START_TASK_STATE_ID-1 )
    {
        item.setId( getUnusedBoardID(m_board));
        m_board.push_back( item );
    }
    else
    {
        m_board.push_back( item );
    }

    return item.getId();

#if 0 //初步判定为无效代码
    //ID自己维护
    if ( item.getId() > m_board.size())//大于vector长度的pushback
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
            m_board[i-1].setId( i );
        }

        m_board.push_back( item );
        sort( m_board.begin(), m_board.end() );
        return item.getId();
    }
#endif
}

kn_string CNGloblePrjTaskBoard::getBoardItemName( int index )
{
    kn_string str = _T( "" );

    if ( index >= m_board.size()||(index < 0))
    {
        return str;
    }

    return m_board[index].getName();
}
