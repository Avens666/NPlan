//  File:        NProjectObject.cpp
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
#include "NProjectObject.h"

CNProjectObject::CNProjectObject( void )
{
    m_id = 0;
}

CNProjectObject::CNProjectObject( const kn_char*_name, int _id )
{
    setName( _name );
    setId( _id );
}

CNProjectObject::~CNProjectObject( void )
{
}

int CNProjectObject::getId() const
{
    return m_id;
}

void CNProjectObject::setId( int _id )
{
    m_id = _id;
}

void CNProjectObject::setId( const kn_char* _id )
{
    if( _id != NULL )
    {
        setId( _tstoi( _id ) );
    }
}

kn_string CNProjectObject::getName() const
{
    return m_name;
}

void CNProjectObject::setName( const kn_char* _name )
{
    if( _name != NULL )
    {
        m_name = _name;
    }
}

bool CNProjectObject::operator< (const CNProjectObject& other) const 
{
    return getId() < other.getId();
}

