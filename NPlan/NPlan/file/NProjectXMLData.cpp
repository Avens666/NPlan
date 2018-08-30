//  File:        NProjectXMLData.cpp
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
#include "NProjectXMLData.h"

CNProjectXMLData::CNProjectXMLData( CNProjectData* pData ) : CNProjectBaseFile( pData )
{
    m_pXmldoc = NULL;
    m_pRootElement = NULL;
}

CNProjectXMLData::~CNProjectXMLData( void )
{
    if ( m_pXmldoc )
    {
        delete m_pXmldoc;
    }
}

//加载xml文件
int CNProjectXMLData::load( kn_string &filename, nplan_fileType file_type /* = XML_PRJ_FILE*/ )
{
    string file = ns2s( filename );

    if ( file.length() == 0 )
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ( file_type == XML_PRJ_FILE )
    {
        m_pXmldoc = new TiXmlDocument( file.c_str() );
        if ( !m_pXmldoc->LoadFile() )
        {
            return ERROR_FILE_INVALID;
        }
    }
    else if ( file_type == XML_PRJ_BUFFER )
    {
        m_pXmldoc->Parse( file.c_str(), 0 );

        if ( m_pXmldoc->Error() )
        {
            return ERROR_FILE_INVALID;
        }
        else
        {
            return NO_ERROR;
        }
    }

    m_pRootElement = m_pXmldoc->RootElement();

	//先解析资源，再解析基本信息。以便将基本信息中的PM名称转化为ID
	parseResourcePool();

	parseCalendars();

	if ( parseBaseInfo() )
	{
		if ( parseTaskBoard() )
		{
			if ( parseTasks() )
			{
				parseEvents();

				parseMilestones();

				return NO_ERROR;
			}
		}
	}

    return ERROR_FILE_INVALID;
}

//保存xml文件
int CNProjectXMLData::save( kn_string &filename, kn_char* lpTemplatefile /* = NULL*/ )
{
    if ( filename.length() == 0 )
    {
        return ERROR_INVALID_PARAMETER;
    }

    string file = ns2s( filename );

    size_t stIndex = filename.rfind( _T( '/' ) );  // 如果传入的是'\'，已经处理成'/'
    if ( stIndex != std::string::npos )
    {
        kn_string pathname = filename.substr( 0, stIndex +1 );
        if ( !IsDirectoryExist( pathname.c_str() ))
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
    m_pRootElement = new TiXmlElement( "NProject" );
    if( -1 != m_pPrjData->getColor() )
    {
        m_pRootElement->SetAttribute( "color", m_pPrjData->getColor() );
    }

    saveBaseInfo();

    saveResourcePool();

    saveTaskBoard();

    saveTasks();

    saveEvents();

    saveMilestones();

    doc.LinkEndChild( decl );

    doc.LinkEndChild( m_pRootElement );

    if ( !doc.SaveFile( file.c_str() ) )
    {
        return ERROR_CREATE_FAILED;
    }

    return NO_ERROR;
}

bool CNProjectXMLData::parseCalendars()
{
    if ( m_pRootElement )
    {
        TiXmlElement* Calendars = m_pRootElement->FirstChildElement( "Calendars" );
        if ( Calendars )
        {
            CNProjectCalendar& PrjCalendar = m_pPrjData->getCalendar();

            TiXmlElement* Calendar = Calendars->FirstChildElement();
            for (; Calendar != NULL; Calendar = Calendar->NextSiblingElement() )
            {
                TiXmlElement* WorkDays = Calendar->FirstChildElement( "WorkDays" );
                if (NULL != WorkDays)
                {
                    TiXmlElement* WorkDay = WorkDays->FirstChildElement();
                    for (; WorkDay != NULL; WorkDay = WorkDay->NextSiblingElement() )
                    {
                        date da = from_string( WorkDay->GetText());
                        PrjCalendar.setWorkDay(da);
                    }
                }
                else
                {
                    TiXmlElement* Holidays = Calendar->FirstChildElement( "Holidays" );
                    if (NULL != Holidays)
                    {
                        TiXmlElement* Holiday = Holidays->FirstChildElement();
                        for (; Holiday != NULL; Holiday = Holiday->NextSiblingElement() )
                        {
                            date da = from_string( Holiday->GetText());
                            PrjCalendar.setHoliday(da);
                        }
                    }
                }
            }
        }
    }

	return (m_pRootElement != NULL);
}

//解析基本信息
bool CNProjectXMLData::parseBaseInfo()
{
    if ( m_pRootElement )
    {
        m_pPrjData->setColor( GetAttributeInt( m_pRootElement, "color" ) );

        TiXmlElement* pBaseInfoElement = m_pRootElement->FirstChildElement( "BaseInfo" );
        if ( pBaseInfoElement )
        {
			//m_pPrjData->m_baseinfo.setLeader( GetAttributeString( pBaseInfoElement, "leader" ).c_str() );
			//PM也是资源，应保存资源ID，而不是名称
			int pm_id = m_pPrjData->m_respool.findResourceID( GetAttributeString( pBaseInfoElement, "leader" ).c_str() );
			m_pPrjData->m_baseinfo.setId(pm_id);

            m_pPrjData->m_baseinfo.setName( GetAttributeString( pBaseInfoElement, "name" ).c_str() );
            m_pPrjData->m_baseinfo.setProDescribe( GetAttributeString( pBaseInfoElement, "proDescribe" ).c_str() );
            m_pPrjData->m_baseinfo.setVersion( GetAttributeString( pBaseInfoElement, "version" ).c_str() );
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool CNProjectXMLData::parseResourcePool()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement* pResPooElement = m_pRootElement->FirstChildElement( "ResourcePool" );

    if ( pResPooElement )
    {
        //解析ResourceGroup标签
        TiXmlElement* pResourceGroupElement = pResPooElement->FirstChildElement( "ResourceGroup" );
        if ( pResourceGroupElement )
        {
            TiXmlElement* pResGroupElement = pResourceGroupElement->FirstChildElement();  //resgroup标签
            for ( pResGroupElement; pResGroupElement != NULL; pResGroupElement = pResGroupElement->NextSiblingElement() )
            {
                CNProjectResGroup resgrp;
                resgrp.setName( GetAttributeString( pResGroupElement, "name" ).c_str() );
                resgrp.setId( GetAttributeInt( pResGroupElement, "id" ) );
                m_pPrjData->m_respool.addResGroup( resgrp );
            }
        }
        else
        {
            return false;
        }

        //解析Resources标签
        TiXmlElement* pRessElement = pResPooElement->FirstChildElement( "Resources" );
        if ( pRessElement )
        {
            TiXmlElement* pResElement = pRessElement->FirstChildElement();  //resgroup标签
            for ( pResElement; pResElement != NULL; pResElement = pResElement->NextSiblingElement() )
            {
                CNProjectResource res;
                res.setName( GetAttributeString( pResElement, "name" ).c_str() ); //name
                res.setId( GetAttributeInt( pResElement, "id" ) ); //id
                res.setType( GetAttributeInt( pResElement, "type" ) ); //type
                res.setGroups( GetAttribute2IntVector( pResElement, "group" ) );
                m_pPrjData->m_respool.addResource( res );
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CNProjectXMLData::parseTaskBoard()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement* pTaskBoardElement = m_pRootElement->FirstChildElement( "TaskBoard" );
    if ( !pTaskBoardElement )
    {
        return false;
    }

    TiXmlElement* pTaskElement = pTaskBoardElement->FirstChildElement();

    for ( pTaskElement; pTaskElement != NULL; pTaskElement = pTaskElement->NextSiblingElement() )
    {
        CNProjectTaskBoardItem board;
        board.setName( GetAttributeString( pTaskElement, "name" ).c_str() );
        board.setId( GetAttributeInt( pTaskElement, "id" ) );
        m_pPrjData->m_taskboard.addBoardItem( board );
    }

    return true;
}

bool CNProjectXMLData::parseTasks()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement* pTasksElement = m_pRootElement->FirstChildElement( "Tasks" );

    if ( pTasksElement )
    {
        TiXmlElement* pTaskElement = pTasksElement->FirstChildElement();

        for (; pTaskElement != NULL; pTaskElement = pTaskElement->NextSiblingElement() )
        {
            CNProjectTask task;

            CNProjectTaskCard_LST cards;

            TiXmlElement* pTaskEle	= pTaskElement->FirstChildElement();

            if (pTaskEle)
            {
                kn_string start_time = GetAttributeString( pTaskEle, "start_time" );
                kn_string end_time = GetAttributeString( pTaskEle, "end_time" );

                 if( !start_time.empty() )
                 {
                     task.setStartTime( start_time.c_str() );
                 }

                 if( !end_time.empty() )
                 {
                     task.setEndTime( end_time.c_str() );
                 }
            }

            task.setName( GetAttributeString( pTaskEle, "name" ).c_str() );
            task.setId( GetAttributeInt( pTaskEle, "id" ) );
            task.setLeaderId( GetAttributeInt( pTaskEle, "leader" ) );
            task.setMembers( GetAttribute2IntVector( pTaskEle, "members" ) );
            task.setTaskDescribe( GetAttributeString( pTaskEle, "task_describe" ).c_str() );

            TiXmlElement* pTaskCardElement = pTaskEle->NextSiblingElement();
            for (; pTaskCardElement != NULL; pTaskCardElement = pTaskCardElement->NextSiblingElement() )
            {
                CNProjectTaskCard card;
                card.setName( GetAttributeString( pTaskCardElement, "name" ).c_str() );
                card.setId( GetAttributeInt( pTaskCardElement, "id" ) );
                card.setLeaderId( GetAttributeInt( pTaskCardElement, "leader" ) );
                card.setPlanDays( GetAttributeInt( pTaskCardElement, "plan_days" ) );
                card.setPlanHours( GetAttributeInt( pTaskCardElement, "plan_hours" ) );
                card.setState( GetAttributeInt( pTaskCardElement, "state" ) );
				card.setScore( GetAttributeInt( pTaskCardElement, "score" ) );
				card.setDifficulty( GetAttributeInt( pTaskCardElement, "difficulty" ) );

				kn_string action_start_time = GetAttributeString( pTaskCardElement, "start_time" );
				kn_string action_end_time = GetAttributeString( pTaskCardElement, "end_time" );

				if( !action_start_time.empty() )
				{
					card.setStartTime( action_start_time.c_str() );
				}

				if( !action_end_time.empty() )
				{
					card.setEndTime( action_end_time.c_str() );
				}
                cards.push_back( card );
            }

            task.setTaskCards( cards );
            task.setColorType( GetAttributeInt( pTaskElement, "color" ) );
            m_pPrjData->addTask( task );

            checkWarning(task);
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool CNProjectXMLData::parseEvents()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement* pEventsElement = m_pRootElement->FirstChildElement( "Events" );
    if ( !pEventsElement )
    {
        return false;
    }

    TiXmlElement* pEventElement = pEventsElement->FirstChildElement();

    for ( pEventElement; pEventElement != NULL; pEventElement = pEventElement->NextSiblingElement() )
    {
        CNProjectEvent evt;
        evt.setName( GetAttributeString( pEventElement, "name" ).c_str() );
        evt.setId( GetAttributeInt( pEventElement, "id" ) );

        kn_string strAtt = GetAttributeString( pEventElement, "time" );
        evt.setTime(strAtt);
        evt.setMemo( GetAttributeString( pEventElement, "memo" ).c_str() );
        m_pPrjData->m_events.push_back( evt );
    }

    return true;
}

bool CNProjectXMLData::parseMilestones()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement* pMilesElement = m_pRootElement->FirstChildElement( "Milestones" );
    if ( !pMilesElement )
    {
        return false;
    }

    TiXmlElement* pMileElement = pMilesElement->FirstChildElement();

    for ( pMileElement; pMileElement != NULL; pMileElement = pMileElement->NextSiblingElement() )
    {
        CNProjectMileStone ms;

        kn_string strName = GetAttributeString( pMileElement, "name" );
        ms.setName(strName.c_str());
        ms.setId( GetAttributeInt( pMileElement, "id" ) );
        kn_string strTime =GetAttributeString( pMileElement, "time" );
        ms.setTime(strTime);
        ms.setMemo( GetAttributeString( pMileElement, "memo" ).c_str() );
        m_pPrjData->m_milestones.push_back( ms );
    }

    return true;
}

bool CNProjectXMLData::saveCalendars()
{
	TiXmlElement* calendars = add_node ( m_pRootElement, "Calendars", NULL );
    TiXmlElement* calendar = add_node ( calendars, "Calendar", NULL );

	const CNProjectCalendar_LST * pWorkDays = m_pPrjData->getCalendar().getWorkingdays();
	if(NULL != pWorkDays)
	{
		TiXmlElement* workdays = add_node ( calendar, "WorkDays", NULL );

		CNProjectCalendar_LST::const_iterator it = pWorkDays->begin();
		for(; it != pWorkDays->end(); ++it)
		{
			TiXmlElement* weekday = add_node ( workdays, "WorkDay", to_simple_string( it->getStartDate() ).c_str() );
		}
	}

	const CNProjectCalendar_LST * pHolidays = m_pPrjData->getCalendar().getHolidays();
	if(NULL != pWorkDays)
	{
		calendar = add_node ( calendars, "Calendar", NULL );
		TiXmlElement* holidays = add_node ( calendar, "Holidays", NULL );

		CNProjectCalendar_LST::const_iterator it = pHolidays->begin();
		for(; it != pHolidays->end(); ++it)
		{
			add_node ( holidays, "Holiday", to_simple_string( it->getStartDate() ).c_str() );
		}
	}

	return (calendars != NULL);
}

bool CNProjectXMLData::saveBaseInfo()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *baseInfoElement = new TiXmlElement ( "BaseInfo" );
    SetAttributeStr( baseInfoElement, "version", m_pPrjData->getVersion() );
    SetAttributeStr( baseInfoElement, "name", m_pPrjData->getName() );
    SetAttributeStr( baseInfoElement, "leader", m_pPrjData->getLeader() );
    SetAttributeStr( baseInfoElement, "proDescribe", m_pPrjData->getProDescribe() );

	//项目日历
	string prj_start_time = ptime2str( m_pPrjData->getProBeginTime(), true );
    string prj_end_time = ptime2str( m_pPrjData->getProEndTime(), true );
	add_node( m_pRootElement, "StartDate", prj_start_time.c_str() );
	add_node( m_pRootElement, "FinishDate",  prj_end_time.c_str() );

	CNProjectCalendar& calendar = m_pPrjData->getCalendar();
	
	time_duration default_start_time = calendar.getWorkingTime();
	string start_time = to_simple_string(default_start_time);

	time_duration default_end_time = calendar.getClosingTime();
	string end_time = to_simple_string(default_end_time);

	add_node( m_pRootElement, "DefaultStartTime", start_time.c_str() );
    add_node( m_pRootElement, "DefaultFinishTime", end_time.c_str() );

    add_node( m_pRootElement, "MinutesPerDay", "480" );
    add_node( m_pRootElement, "MinutesPerWeek", "2400" );
    add_node( m_pRootElement, "DaysPerMonth", "20" ); //TODO

	saveCalendars();

    m_pRootElement->LinkEndChild( baseInfoElement );
    return true;
}

bool CNProjectXMLData::saveResourcePool()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *respoolElement  = new TiXmlElement ( "ResourcePool" );
    TiXmlElement *resgroupElement = new TiXmlElement ( "ResourceGroup" );

    std::vector <CNProjectResGroup>& regrp = m_pPrjData->m_respool.getResourceGroups();
    vector <CNProjectResGroup>::iterator iter;
    for ( iter = regrp.begin(); iter != regrp.end(); ++iter )
    {
        TiXmlElement *resgrpElement = new TiXmlElement ( "ResGroup" );
        SetAttributeStr( resgrpElement, "name", iter->getName() );
        resgrpElement->SetAttribute( "id", iter->getId() );
        resgroupElement->LinkEndChild( resgrpElement );
    }

    TiXmlElement *resElement = new TiXmlElement ( "Resources" );
    std::vector <CNProjectResource>& res = m_pPrjData->m_respool.getResources();
    vector <CNProjectResource>::iterator ite;
    for ( ite = res.begin(); ite != res.end(); ++ite )
    {
        TiXmlElement *reElement = new TiXmlElement ( "Resource" );
        SetAttributeStr( reElement, "name", ite->getName() );
        reElement->SetAttribute( "id", ite->getId() );
        reElement->SetAttribute( "type", ite->getType() );

        vector <int> grps = ite->getGroups();

        reElement->SetAttribute( "group", IntVector2str( grps ).c_str() );
        resElement->LinkEndChild( reElement );
    }

    respoolElement->LinkEndChild( resgroupElement );
    respoolElement->LinkEndChild( resElement );
    m_pRootElement->LinkEndChild( respoolElement );
    return true;
}

bool CNProjectXMLData::saveTaskBoard()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *taskBoardElement = new TiXmlElement ( "TaskBoard" );

    vector <CNProjectTaskBoardItem >::iterator ite;
    vector <CNProjectTaskBoardItem>& boards = m_pPrjData->m_taskboard.getBoardItems();
    for ( ite = boards.begin(); ite != boards.end(); ++ite )
    {
        TiXmlElement *taskbdElement = new TiXmlElement ( "Item" );
        SetAttributeStr( taskbdElement, "name", ite->getName() );
        taskbdElement->SetAttribute( "id", ite->getId() );
        taskBoardElement->LinkEndChild( taskbdElement );
    }

    m_pRootElement->LinkEndChild( taskBoardElement );
    return true;
}

bool CNProjectXMLData::saveTasks()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *tasksElement = new TiXmlElement ( "Tasks" );

    CNProjectTask_LST::iterator iter;
    for ( iter = m_pPrjData->m_tasks.begin(); iter != m_pPrjData->m_tasks.end(); ++iter )
    {
        int colour = iter->getColorType();
        TiXmlElement *taskElement = new TiXmlElement ( "Task" );
        if( -1 != colour )
        {
            taskElement->SetAttribute( "color", colour );
        }

        TiXmlElement *taskinfoElement = new TiXmlElement ( "TaskInfo" );
        SetAttributeStr( taskinfoElement, "name", iter->getName() );
        taskinfoElement->SetAttribute( "id", iter->getId() );
        taskinfoElement->SetAttribute( "leader", iter->getLeaderId() );
        SetAttributeStr( taskinfoElement, "task_describe", iter->getTaskDescribe() );
        taskinfoElement->SetAttribute( "members", IntVector2str( iter->getMembers() ).c_str() );
        taskinfoElement->SetAttribute( "start_time", ptime2str( iter->getStartTime() ).c_str() );
        taskinfoElement->SetAttribute( "end_time", ptime2str( iter->getEndTime() ).c_str() );

        taskElement->LinkEndChild( taskinfoElement );

        CNProjectTaskCard_LST::iterator it;
        CNProjectTaskCard_LST& vec = iter->getTaskCards();
        for ( it = vec.begin(); it != vec.end(); ++it )
        {
            TiXmlElement *taskcardElement = new TiXmlElement( "TaskCard" );
            SetAttributeStr( taskcardElement, "name", it->getName() );
            taskcardElement->SetAttribute( "id", it->getId() );
            taskcardElement->SetAttribute( "leader", it->getLeaderId() );
            taskcardElement->SetAttribute( "plan_days", it->getPlanDays() );
            taskcardElement->SetAttribute( "plan_hours", it->getPlanHours() );
            taskcardElement->SetAttribute( "state", it->getState() );
            taskcardElement->SetAttribute( "score", it->getScore() );
			taskcardElement->SetAttribute( "difficulty", it->getDifficulty() );
			taskcardElement->SetAttribute( "start_time", ptime2str( it->getStartTime() ).c_str() );
			taskcardElement->SetAttribute( "end_time", ptime2str( it->getEndTime() ).c_str() );
            taskElement->LinkEndChild( taskcardElement );
        }

        tasksElement->LinkEndChild( taskElement );
    }

    m_pRootElement->LinkEndChild( tasksElement );

    return true;
}

bool CNProjectXMLData::saveMilestones()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *milestonesElement = new TiXmlElement ( "Milestones" );
    CNProjectMileStone_LST::iterator ite;
    for ( ite = m_pPrjData->m_milestones.begin(); ite != m_pPrjData->m_milestones.end(); ++ite )
    {
        TiXmlElement *milestoneElement = new TiXmlElement ( "Milestone" );
        SetAttributeStr( milestoneElement, "name", ite->getName() );
        milestoneElement->SetAttribute( "id", ite->getId() );
        milestoneElement->SetAttribute( "time", ptime2str( ite->getTime() ).c_str() );
        SetAttributeStr( milestoneElement, "memo", ite->getMemo() );
        milestonesElement->LinkEndChild( milestoneElement );
    }

    m_pRootElement->LinkEndChild( milestonesElement );
    return true;
}

bool CNProjectXMLData::saveEvents()
{
    if ( !m_pRootElement )
    {
        return false;
    }

    TiXmlElement *eventsElement = new TiXmlElement ( "Events" );

    vector <CNProjectEvent>::iterator ite;
    for ( ite = m_pPrjData->m_events.begin(); ite != m_pPrjData->m_events.end(); ++ite )
    {
        TiXmlElement *eventElement = new TiXmlElement ( "Event" );
        SetAttributeStr( eventElement, "name", ite->getName() );
        eventElement->SetAttribute( "id", ite->getId() );
        eventElement->SetAttribute( "time", ptime2str( ite->getTime() ).c_str() );
        SetAttributeStr( eventElement, "memo", ite->getMemo() );
        eventsElement->LinkEndChild( eventElement );
    }

    m_pRootElement->LinkEndChild( eventsElement );
    return true;
}
