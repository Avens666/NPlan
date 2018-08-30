//  File:        NProjectResource.cpp
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
#include "NProjectResource.h"

CNProjectResource::CNProjectResource( void ):CNProjectObject()
{
    m_type = res_man;
}

CNProjectResource::CNProjectResource( const kn_char * _name,
    int _id, int _type, int groupId ):CNProjectObject(_name, _id)
{
    m_type = convertResType( _type );
    if ( groupId != 0 )
    {
        setGroup( groupId );
    }
}

CNProjectResource::~CNProjectResource( void )
{
}

void CNProjectResource::setType( const kn_char* _type )
{
    if( _type != NULL )
    {
        m_type = convertResType( _type );
    }
}

void CNProjectResource::setType( int _type )
{
    m_type = convertResType( _type );
}

void CNProjectResource::setGroup( int groupId )
{
    m_group_ids.push_back( groupId );
}

void CNProjectResource::setGroups(const vector <int>& vectIds )
{
    std::copy( vectIds.begin(), vectIds.end(), std::back_inserter( m_group_ids ));
}

res_type CNProjectResource::getType()
{
    return m_type;
}

vector <int> CNProjectResource::getGroups()
{
    return m_group_ids;
}

res_type CNProjectResource::convertResType( int _type )
{
    res_type type = res_unknown;

    if( _type <= res_unknown  && _type >= 0)
    {
        type = (res_type)_type;
    }

    return type;
}

kn_string  CNProjectResource::resTypeConvertStr( res_type _type )
{
    kn_string type = _T("res_unknown");

    switch ( _type )
    {
    case res_man:
        type = getStringManager()->GetStringByID(_T("man"));
        break;
    case res_woman:
        type = getStringManager()->GetStringByID(_T("woman"));
        break;
    case res_device:
        type = getStringManager()->GetStringByID(_T("device"));
        break;
    default:
        type = RES_NAME_UNKNOWN;
        break;
    }

    return type;
}

res_type CNProjectResource::convertResType( const kn_char* _type )
{
    kn_string typestr = _T( "" );

    if( _type != NULL )
    {
        typestr = _type;
    }

    res_type type = res_unknown;
    if ((_type == NULL) || (typestr == _T( "" ))) //空值判断
    {
        type = res_unknown;
    }
    else if( typestr == getStringManager()->GetStringByID(_T("woman")) )
    {
        type = res_woman;
    }
    else if ( typestr == getStringManager()->GetStringByID(_T("man")) )
    {
        type = res_man;
    }
    else if ( typestr == getStringManager()->GetStringByID(_T("device")) )
    {
        type = res_device;
    }
    else
    {
        type = res_unknown;
    }

    return type;
}
