//  File:        NProjectBaseFile.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/25
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NProjectBaseFile.h"


CNProjectBaseFile::CNProjectBaseFile( CNProjectData* pData ) : m_pPrjData( pData )
{
}

CNProjectBaseFile::~CNProjectBaseFile( void )
{

}

void CNProjectBaseFile::setProjectFileType( nplan_fileType fileType )
{
    m_FileType = fileType;
}

int CNProjectBaseFile:: load( kn_string &filename )
{
    return NO_ERROR;
}

int CNProjectBaseFile:: save( kn_string &filename )
{
    return NO_ERROR;
}

int CNProjectBaseFile::load( kn_string &filename, nplan_fileType file_type )
{
    return NO_ERROR;
}

int CNProjectBaseFile::save( kn_string &filename, kn_char* lpTemplatefile /*= NULL */ )
{
    return NO_ERROR;
}

bool CNProjectBaseFile::saveWarning()
{
    bool _result = false;
#ifdef WIN32
    std::wofstream ofs( _T( "warning.txt" ) );


    switch ( getConfigManager()->getLanguage() )
	{
	case LAN_CN:
		ofs.imbue( std::locale( "chs" ));
		break;
	case LAN_JP:
		ofs.imbue( std::locale( "jpn" ));
		break;
	default:
		break;
	}

#else
    std::wofstream ofs("warning.txt");  // for linux
#endif

    if( !ofs )
    {
        return _result;
    }

    if( (m_pPrjData != NULL) && (!m_pPrjData->m_warnings.empty()) )
    {
        kn_char szBuff[512]={0};

        int index = 0;
        CNProjectWarning_LST::iterator it = m_pPrjData->m_warnings.begin();
        for( ; it != m_pPrjData->m_warnings.end(); it++, index++ )
        {
            memset(szBuff, 0, sizeof(kn_char)*512);
            _stprintf(szBuff, _T("%d,%s:%s\n"), index, it->getWarningTypeStr().c_str(), it->getMsg().c_str() );
            ofs << szBuff;
        }

        ofs.flush();
    }

    ofs.close();

    return _result;
};

bool CNProjectBaseFile::checkWarning(CNProjectTask& task)
{
    bool _result = true;

    ptime s_time = task.getStartTime();
    ptime e_time = task.getEndTime();

    kn_string str_msg;

    CNProjectWarning warning( task.getId(), str_msg.c_str(), WS_Task, WT_Warning_Error );

    kn_bool valid_time = true;

    if( s_time.is_not_a_date_time() )
    {
        valid_time = false;
        warning.setMsg( str_msg.c_str() );
        m_pPrjData->m_warnings.push_back( warning );
    }

    if( e_time.is_not_a_date_time() )
    {
        valid_time = false;
        warning.setMsg( str_msg.c_str() );
        m_pPrjData->m_warnings.push_back( warning );
    }

    const CNProjectTaskCard_LST& actions = task.getTaskCards();
    for (CNProjectTaskCard_LST::const_iterator it = actions.begin(); it != actions.end() ; ++it)
    {
        const CNProjectTaskCard& action = *it;

        CNProjectWarning warning( it->getId(), str_msg.c_str(), WS_TaskCard, WT_Warning_Error );

        if ( ( 0 <= action.getPlanDays() ) && ( 0 <= action.getPlanHours() ) )
        {
            kn_string str_msg /*= _T( "任务天数和小时数均为0。" )*/;
            warning.setMsg( str_msg.c_str() );
            m_pPrjData->m_warnings.push_back( warning );
            _result = false;
        }

        if( action.getName().empty() )
        {
            kn_string str_msg /*= _T( "任务天数和小时数均为0。" )*/;
            warning.setMsg( str_msg.c_str() );
            m_pPrjData->m_warnings.push_back( warning );

            _result = false;
        }

        if( action.getStartTime().is_not_a_date_time() )
        {
            _result = false;
        }

        if( action.getEndTime().is_not_a_date_time() )
        {
            _result = false;
        }

        if(_result )
        {
            if( valid_time )
            {
                if(s_time < action.getStartTime())
                {
                    _result = false;
                }

                if(e_time > action.getEndTime() )
                {
                    _result = false;
                }
            }
        }
    }

    return _result;
}


