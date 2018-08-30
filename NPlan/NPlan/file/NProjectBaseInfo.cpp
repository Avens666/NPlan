//  File:        NProjectBaseInfo.cpp
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
#include "NProjectBaseInfo.h"

CNProjectBaseInfo::CNProjectBaseInfo( void ):CNProjectObject()
{
    m_dataFormatVersion = _T( "1.0" );
}

CNProjectBaseInfo::CNProjectBaseInfo( const kn_char * _version, const kn_char * _name, int leader_id )
{
    _version = _T( "1.0" );
    setBaseInfo( _version, _name, leader_id, _T( "" ));
}

CNProjectBaseInfo::~CNProjectBaseInfo( void )
{
}

kn_string CNProjectBaseInfo::getVersion() const
{
    return m_dataFormatVersion;
}

void CNProjectBaseInfo::setVersion( const kn_char* _version )
{
    if ( _version != NULL )
    {
        m_dataFormatVersion = _version;
    }
}

void CNProjectBaseInfo::setBaseInfo( const kn_char* _version, const kn_char* _name, int leader_id,
                                     const kn_char* _describe )
{
	setVersion( _version );
	setId( leader_id );
	setName( _name );
	setProDescribe( _describe );
}

kn_string CNProjectBaseInfo::getProDescribe() const
{
    return m_ProDescribe;
}

void CNProjectBaseInfo::setProDescribe( const kn_char * _prodescribe )
{
    if ( _prodescribe != NULL )
    {
        m_ProDescribe = _prodescribe;
    }
}
