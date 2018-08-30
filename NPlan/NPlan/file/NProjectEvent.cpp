//  File:        NProjectEvent.cpp
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
#include "NProjectEvent.h"

CNProjectEvent::CNProjectEvent( void ):CNProjectObject()
{
}

CNProjectEvent::CNProjectEvent( const kn_char* _name , const kn_char* _time,
    const kn_char* _memo, int _id /*= 0*/ ):CNProjectObject(_name, _id)
{
    setTime( time_from_string( ns2s( _time )));
    setMemo( _memo );
}

CNProjectEvent::~CNProjectEvent( void )
{
}

boost::posix_time::ptime CNProjectEvent::getTime() const
{
    return m_time;
}

kn_string CNProjectEvent::getMemo() const
{
    return m_memo;
}

void CNProjectEvent::setTime(kn_string & str_time )
{
    m_time = timestr2ptime( str_time );
}

void CNProjectEvent::setTime( const ptime& _time )
{
    m_time = _time;
}

void CNProjectEvent::setMemo( const kn_char* _memo )
{
    if( _memo != NULL )
    {
        m_memo = _memo;
    }
}
