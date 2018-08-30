//  File:        NProjectTask.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/15
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NProjectTask.h"
#include "NProjectData.h"

CNProjectTask::CNProjectTask( void )
{
	m_task_describe = _T("");
    m_npdata = NULL;
    m_color = 0;
	m_leader_id = 0;
	m_task_type = TaskCardBase_TASK_TYPE;
	m_start_time = second_clock::local_time();
	m_end_time = m_start_time + months(1);
}

CNProjectTask::CNProjectTask( const CNProjectTask& other )
{
	m_task_type = TaskCardBase_TASK_TYPE;
    setId( other.getId() );
    setName( other.getName().c_str() );
    m_leader_id	       = other.getLeaderId();
    m_start_time       = other.getStartTime();
    m_end_time         = other.getEndTime();
    m_color            = other.getColorType();
    m_members          = other.getMembers();
    m_MembersName      = other.getMembersName();
    m_task_describe    = other.getTaskDescribe();
    m_npdata           = other.m_npdata;
    CNProjectTask& ref = const_cast <CNProjectTask&>(other);
    m_taskcards.assign( ref.getTaskCards().begin(), ref.getTaskCards().end());

	CNProjectTaskCard_LST::iterator it_task;
	for ( it_task = m_taskcards.begin(); it_task != m_taskcards.end(); it_task++ )
	{
		it_task->setTask(this);
	}

}

CNProjectTask& CNProjectTask::operator=(const CNProjectTask &other)
{
	m_task_type = TaskCardBase_TASK_TYPE;
	setId( other.getId() );
	setName( other.getName().c_str() );
	m_leader_id	       = other.getLeaderId();
	m_start_time       = other.getStartTime();
	m_end_time         = other.getEndTime();
	m_color            = other.getColorType();
	m_members          = other.getMembers();
	m_MembersName      = other.getMembersName();
	m_task_describe    = other.getTaskDescribe();
	m_npdata           = other.m_npdata;
	CNProjectTask& ref = const_cast <CNProjectTask&>(other);
	m_taskcards.assign( ref.getTaskCards().begin(), ref.getTaskCards().end());

	CNProjectTaskCard_LST::iterator it_task;
	for ( it_task = m_taskcards.begin(); it_task != m_taskcards.end(); it_task++ )
	{
		it_task->setTask(this);
	}

	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}

	return *this;
}

CNProjectData* CNProjectTask::getProject()
{
	return m_npdata;
}

CNProjectTask::~CNProjectTask( void )
{
}

CNProjectTask::CNProjectTask( const kn_char*_name, int _id, const kn_char*s_time, const kn_char*e_time, int _leader_id )
{
    setTaskInfo( _name, _id, s_time, e_time, _leader_id );
}

CNProjectTaskCard_LST & CNProjectTask::getTaskCards()
{
    return m_taskcards;
}

void CNProjectTask::setTaskCards( CNProjectTaskCard_LST& card )
{
    m_taskcards.clear();
    std::copy( card.begin(), card.end(), std::back_inserter( m_taskcards ));
}

void CNProjectTask::setTaskInfo( const kn_char* _name, int _id, const kn_char* s_time, const kn_char* e_time,
                                 int _leader_id )
{
    setName( _name );
    setId( _id );

    if ( s_time != NULL )
    {
        setStartTime( s_time );
    }

    if ( e_time != NULL )
    {
        setEndTime( e_time );
    }

    setLeaderId( _leader_id );
}

void CNProjectTask::setTaskInfo( const kn_char* _name, int _id,const ptime & s_time, const ptime & e_time, int _leader_id )
{
    setName( _name );
    setId( _id );
    m_leader_id	 = _leader_id;
    m_start_time = s_time;
    m_end_time = e_time;
}

void CNProjectTask::setMembers(const vector <int>& vectMembers )
{
    m_members.clear();
    std::copy( vectMembers.begin(), vectMembers.end(), std::back_inserter( m_members ));
}

void CNProjectTask::addMembers(int id )
{
	if (m_npdata)
	{
		if (m_npdata->getResourcePool().checkResourceByID(id) == FALSE)
		{
			return;
		}
	}
	//防止重复
	vector<int>::iterator result = find( m_members.begin( ), m_members.end( ), id );
	if (  result == m_members.end( ) ) // not found
	{
		m_members.push_back(id);
	}

}

vector <int>&  CNProjectTask::getMembers()
{
    return m_members;
}

void CNProjectTask::addTaskCard( kn_char* _name, int _leader_id, int days, int hours, int _state, int id )
{
    if ( id == 0 )
    {
        id = (getId()<<16) + getUnusedCardId();
    }

    m_taskcards.push_back( CNProjectTaskCard( _name, _leader_id, days, hours, _state, id ));
	m_taskcards.rbegin()->setTask(this);
}

void CNProjectTask::addTaskCard( CNProjectTaskCard & taskCard )
{
    if ( taskCard.getId() == 0 )
    {
        taskCard.setId((getId()<<16) + getUnusedCardId());
    }

    m_taskcards.push_back( taskCard );
	m_taskcards.rbegin()->setTask(this);

	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
}

void CNProjectTask::delTaskCard( CNProjectTaskCard & pTaskCard )
{
	if ( pTaskCard.getId() == 0 )
	{
		pTaskCard.setId((getId()<<16) + getUnusedCardId());
	}

	CNProjectTaskCard_LST::iterator itr;
	kn_int i=0;
	for (itr = m_taskcards.begin(); itr != m_taskcards.end(); ++itr )
	{
		if ( (*itr).getId() == pTaskCard.getId())
		{
			itr = m_taskcards.erase(itr);
			break;
		}
		i++;
	}

	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
}

void CNProjectTask::addTaskCard( kn_char* _name, kn_char* _leader_name, int days, int hours, kn_char* _state, int id /*= 0*/ )
{
    if ( id == 0 )
    {
        id = (getId()<<16) + getUnusedCardId();
    }

    int leader_id = m_npdata->getResourcePool().findResourceID( _leader_name );
    int state = m_npdata->m_taskboard.findIdByName( _state );
    m_taskcards.push_back( CNProjectTaskCard( _name, leader_id, days, hours, state, id ));
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
}

int CNProjectTask::getLeaderId() const
{
    return m_leader_id;
}

ptime CNProjectTask::getStartTime() const
{
    return m_start_time;
}

ptime CNProjectTask::getEndTime() const
{
    return m_end_time;
}

const vector <int> &CNProjectTask::getMembers() const
{
    return m_members;
}

int CNProjectTask::getUnusedCardId()
{
    if ( m_taskcards.empty() )
    {
        return 1;
    }
    else
    {
        CNProjectTaskCard_LST::iterator iter;
        vector <int> vecids;
        for ( iter = m_taskcards.begin(); iter != m_taskcards.end(); ++iter )
        {
            int tempid = iter->getId() & 0xFFFF;
            vecids.push_back( tempid );
        }

        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))  //没找到
            {
                return i;
            }
        }

        return (vecids.size() + 1);
    }
}

void CNProjectTask::setDataPointer( CNProjectData *pdata )
{
    m_npdata = pdata;
}

kn_string CNProjectTask::getLeaderName() const
{
    if ( m_npdata )
    {
        return m_npdata->getResourceNameById( getLeaderId());
    }
    else
    {
        kn_string str = _T( "" );
        return str;
    }
}

void CNProjectTask::setStartTime( const ptime& _time )
{
	if (m_start_time.is_not_a_date_time() == FALSE && _time.is_not_a_date_time() == FALSE )
	{
		offsetTaskCardTime( _time - m_start_time );
	}
    m_start_time = _time;
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
    return ;
}

bool CNProjectTask::setStartTime( const kn_char* _time )
{
    kn_string str( _time );
	ptime t = timestr2ptime( str );
	if (m_start_time.is_not_a_date_time() == FALSE && t.is_not_a_date_time() == FALSE)
	{
		offsetTaskCardTime( t - m_start_time );
	}
	m_start_time = t;
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
    return TRUE;
}

void CNProjectTask::offsetTaskCardTime(time_duration td)
{
	CNProjectTaskCard_LST::iterator iter;

	for ( iter = m_taskcards.begin(); iter != m_taskcards.end(); ++iter )
	{
		(*iter).setStartTime( (*iter).getStartTime() + td );
		(*iter).setEndTime( (*iter).getEndTime() + td );
	}
}

void CNProjectTask::setEndTime(const ptime& _time )
{
    m_end_time = _time;
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
    return ;
}

bool CNProjectTask::setEndTime( const kn_char* _time )
{
    kn_string str( _time );

    m_end_time = timestr2ptime( str );
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
    return true;
}

int CNProjectTask::getColorType() const
{
    return m_color;
}

void CNProjectTask::setColorType( int type )
{
    m_color = type;
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
}

void CNProjectTask::updateMembersName() const
{
    {
        const vector <kn_string>* pNames = &m_MembersName;
        if ( pNames != NULL )
        {
            vector <kn_string>* pMemName = const_cast <vector <kn_string>*>(pNames);
			pMemName->clear();
            vector <int>::const_iterator iter;
            for ( iter = getMembers().begin(); iter != getMembers().end(); ++iter )
            {
                 if( NULL != m_npdata )
                {
                    pMemName->push_back( m_npdata->getResourceNameById( *iter ));
                }
            }
        }
    }
}

void CNProjectTask::setLeaderId( int _id )
{
    m_leader_id = _id;
	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
		addMembers(_id);
	}
}

kn_string CNProjectTask::getMembersNameStr( bool b_hide_leader ) const
{
    kn_string members_name_str;

    updateMembersName();

    kn_string str = getLeaderName();

    vector <kn_string>::const_iterator iter;
    for ( iter = m_MembersName.begin(); iter != m_MembersName.end(); ++iter )
    {
        if ( b_hide_leader && ((*iter) == str))
        {
            continue;
        }

        members_name_str += ((*iter) +_T( " " ));
    }

    return members_name_str;
}

int CNProjectTask::getVectorId()
{
    int i = 0;

    if (m_npdata != NULL)
    {
        CNProjectTask_LST::iterator it_task;   
        for ( it_task = m_npdata->m_tasks.begin(); it_task != m_npdata->m_tasks.end(); it_task++ )
        {
            if ( it_task->getId() == getId() )
            {
                break;
            }

            i++;
        }
    }

    return i;
}

kn_string CNProjectTask::getTaskDescribe( ) const
{
    return m_task_describe;
}

void CNProjectTask::setTaskDescribe( const kn_char* describe )
{
    if ( NULL != describe )
    {
        m_task_describe = describe;

		if (m_npdata)
		{
			m_npdata->setModify(TRUE);
		}
    }
}

void CNProjectTask::setLeaderName( const kn_char* _leader )
{
    if( _leader != NULL )
    {
        int _leader_Id = m_npdata->getResourcePool().findResourceID( _leader );
        m_leader_id = _leader_Id;

		if (m_npdata)
		{
			m_npdata->setModify(TRUE);
		}
    }
}

const vector <kn_string>& CNProjectTask::getMembersName() const
{
    return m_MembersName;
}

int CNProjectTask::getTaskCardCount( int state )
{
    CNProjectTaskCard_LST::iterator iter;
    int count = 0;

    for ( iter = m_taskcards.begin(); iter != m_taskcards.end(); ++iter )
    {
        if (iter->getState() == state )
        {
            count++;
        }
    }

    return count;
}

kn_string CNProjectTask::getMembersNameById(kn_int _id)
{
	return m_npdata->getResourceNameById(_id);
}

kn_int CNProjectTask::getResourceIdByName( const kn_char* name )
{
	return m_npdata->getResourceIdByName(name);
}
res_type CNProjectTask::getResourceTypeById(kn_int _id)
{
	return m_npdata->m_respool.getResourceType(_id);
}

kn_string CNProjectTask::getStateStrById(kn_int _id)
{
	return m_npdata->m_taskboard.getBoardItemName(_id);
}

CNProjectTaskCard_Point_LST& CNProjectTask::getTaskCardsByMan()
{
	return m_taskcard_points;
}

void CNProjectTask::addTaskCardPoint( CNProjectTaskCard* pTaskCard )
{
	m_taskcard_points.push_back(pTaskCard);
}

int CNProjectTask::getManCardCount( int state )
{
	CNProjectTaskCard_Point_LST::iterator iter;
	int count = 0;

	for ( iter = m_taskcard_points.begin(); iter != m_taskcard_points.end(); ++iter )
	{
		if ( (*iter)->getState() == state )
		{
			count++;
		}
	}

	return count;
}

void CNProjectTask::clearTaskCards()
{
    m_taskcards.clear();

	if (m_npdata)
	{
		m_npdata->setModify(TRUE);
	}
}
vector<CNProjectResource>& CNProjectTask::getProjectMember()
{
	ASSERT (m_npdata);
	return m_npdata->m_respool.getResources();
}

void CNProjectTask::setModify( kn_bool b)
{
	if (m_npdata)
	{
		m_npdata->setModify(b);
	}
}


