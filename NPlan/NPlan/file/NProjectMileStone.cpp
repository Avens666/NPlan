//  File:        NProjectMileStone.cpp
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
#include "NProjectMileStone.h"

CNProjectMileStone::CNProjectMileStone( void ):CNProjectObject()
{
}

CNProjectMileStone::CNProjectMileStone( const kn_char * _name,
    const kn_char * _time, const kn_char * _memo, int _id ):CNProjectObject(_name, _id)
{
    setTime( time_from_string( ns2s( _time )));
    setMemo( _memo );
}

CNProjectMileStone::~CNProjectMileStone( void )
{
}

ptime CNProjectMileStone:: getTime()const
{
    return m_time;
}

kn_string CNProjectMileStone:: getMemo() const
{
    return m_memo;
}

void CNProjectMileStone:: setTime(kn_string &_time )
{
    m_time = timestr2ptime( _time );
}

void CNProjectMileStone:: setTime( const ptime &_time )
{
    m_time = _time;
}

void CNProjectMileStone:: setMemo( const kn_char* _memo )
{
    if( _memo != NULL )
    {
        m_memo = _memo;
    }
}
