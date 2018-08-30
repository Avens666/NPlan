//  File:        NProjectWarning.cpp
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
#include "NProjectWarning.h"

CNProjectWarning::CNProjectWarning() : CNProjectObject()
{
}

CNProjectWarning::CNProjectWarning(int id, const kn_char* msg, warning_source source, warning_type _type)
{
    setId(id);
    setMsg(msg);
    m_warn_type = _type;
    m_warn_source = source;
}

CNProjectWarning::~CNProjectWarning( void )
{
}

void CNProjectWarning::setMsg( const kn_char* msg )
{
    setName( msg );
}

kn_string CNProjectWarning::getMsg()
{
    return getName();
}

warning_type CNProjectWarning::getWarningType()
{
    return m_warn_type;
}

void CNProjectWarning::setWarnningType( warning_type _type )
{
    m_warn_type = _type;
}

warning_source CNProjectWarning::getWarningSource()
{
    return m_warn_source;
}

void CNProjectWarning::setWarningSource( warning_source source )
{
    m_warn_source = source;
}

//kn_string CNProjectWarning::getTypeStr( warning_type _type )
//{
//    kn_string typestr;
//
//    switch( _type )
//    {
//    case NP_Task:
//        typestr = KEYWORD_TASK;
//        break;
//    case NP_TaskCard:
//        typestr = KEYWORD_SUBTASK;
//        break;
//    default:
//        typestr = KEYWORD_UNDEFINE;
//        break;
//    }
//
//    return typestr;
//}
//
kn_string CNProjectWarning::getWarningTypeStr( )
{
    kn_string str_warning_type;
//
//    switch( _level )
//    {
//    case NP_Warning_Suggest:
//        levelstr = WARNING_LEVEL_SUGGEST;
//        break;
//    case NP_Warning_Info:
//        levelstr = WARNING_LEVEL_INFO;
//        break;
//    case NP_Warning_Warning:
//        levelstr = WARNING_LEVEL_WARNING;
//        break;
//    case NP_Warning_Error:
//        levelstr = WARNING_LEVEL_ERROR;
//        break;
//    case NP_Warning_FatalError:
//        levelstr = WARNING_LEVEL_FATALERROR;
//        break;
//    default:
//        levelstr = KEYWORD_UNDEFINE;
//        break;
//    }
//
    return str_warning_type;
}
