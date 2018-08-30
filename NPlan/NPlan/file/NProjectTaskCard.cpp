//  File:        NProjectTaskCard.cpp
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
#include "NProjectTaskCard.h"
#include "NProjectTask.h"
#include "NProjectData.h"

CActivityChangeHistory::CActivityChangeHistory()
{
    m_nOldState   = 0;
    m_nNewState   = 0;
    m_change_time = not_a_date_time;
}

CActivityChangeHistory::CActivityChangeHistory( int old_state, int new_state, ptime* change_time /* = NULL*/ )
{
    m_nOldState = old_state;
    m_nNewState = new_state;
    if ( NULL == change_time ) //取当前时间为变更时间
    {
        m_change_time = second_clock::local_time();
    }
    else
    {
        m_change_time = *change_time;
    }
}

void CActivityChangeHistory::setOldState( int old_state )
{
    m_nOldState = old_state;
}

void CActivityChangeHistory::setNewState( int new_state )
{
    m_nNewState = new_state;
}

void CActivityChangeHistory::setChangeTime( const ptime & change_time )
{
    m_change_time = change_time;
}

int CActivityChangeHistory::getOldState() const
{
    return m_nOldState;
}

int CActivityChangeHistory::getNewState() const
{
    return m_nNewState;
}

ptime CActivityChangeHistory::getChangeTime() const
{
    return m_change_time;
}

CNProjectTaskCard::CNProjectTaskCard( void ) : CNProjectTaskCardBase(), m_pTask( NULL ), m_plan_days( 0 ), m_plan_hours( 0 ), m_state( 0 ), m_score( 0 )
{
    m_leader_id  = 0;
    m_task_type  = TaskCardBase_TASKCARD_TYPE;
    m_difficulty = 1;
	m_start_time = second_clock::local_time();
	m_end_time = m_start_time + weeks(1);
}

CNProjectTaskCard::CNProjectTaskCard( const kn_char * _name, int _leader_id, int days, int hours, int _state, int _id )
    : CNProjectTaskCardBase( _name, _id ), m_pTask( NULL ), m_score( 0 ), m_difficulty ( 1 )
{
    m_leader_id  = _leader_id;
    m_plan_days  = days;
    m_plan_hours = hours;
    m_state      = _state;
    m_task_type  = TaskCardBase_TASKCARD_TYPE;
}

CNProjectTaskCard::CNProjectTaskCard( const CNProjectTaskCard& other )
{
    m_task_type = TaskCardBase_TASK_TYPE;
    setId( other.getId());
    setName( other.getName().c_str());
    m_leader_id  = other.getLeaderId();
    m_start_time = other.getStartTime();
    m_end_time   = other.getEndTime();
    m_plan_hours = other.getPlanHours();
    m_plan_days  = other.getPlanDays();
    m_score      = other.getScore();
    m_state      = other.getState();
    m_difficulty = other.getDifficulty();

    m_pTask = (CNProjectTask* )other.getTask();
}

bool CNProjectTaskCard::operator == ( int card_id ) const
{
    return (getId() == card_id);
}

void CNProjectTaskCard::setTask( CNProjectTask* pTask )
{
    m_pTask = pTask;
}

CNProjectTaskCard::~CNProjectTaskCard( void )
{
}

//获取状态变更列表
const vector<CActivityChangeHistory>& CNProjectTaskCard::getStateChangeList() const
{
    return m_vChangeHistory;
}

const CNProjectTask* CNProjectTaskCard::getTask() const
{
    return m_pTask;
}

int CNProjectTaskCard::getLeaderId() const
{
    return m_leader_id;
}

kn_string CNProjectTaskCard::getLeaderName() const
{
    if ( m_pTask )
    {
        return m_pTask->getMembersNameById( m_leader_id );
    }

    return _T( "" );
}

int CNProjectTaskCard::getPlanDays() const
{
    return m_plan_days;
}

int CNProjectTaskCard::getPlanHours() const
{
    return m_plan_hours;
}

ptime CNProjectTaskCard::getStartTime() const
{
    return m_start_time;
}

ptime CNProjectTaskCard::getEndTime() const
{
    return m_end_time;
}

int CNProjectTaskCard::getState() const
{
    return m_state;
}

kn_bool CNProjectTaskCard::isDone() //任务是否完成 state为最大值
{
    if ( getTask() == NULL )
    {
        return 0;
    }

    return m_state == ((CNProjectTask*)getTask())->getProject()->getTaskBoardItems().size() -1;
}

void CNProjectTaskCard::setLeaderId( int _id )
{
    m_leader_id = _id;
	if (m_pTask) //zhic  update task member
	{
		m_pTask->addMembers(_id);
	}
}

void CNProjectTaskCard::setPlanDays( int _days )
{
    m_plan_days = _days;
}

void CNProjectTaskCard::setPlanHours( int _hours )
{
    m_plan_hours = _hours;
}

void CNProjectTaskCard::setPrjModify()
{
    if ( m_pTask )
    {
        if ( m_pTask->getProject())
        {
            m_pTask->getProject()->setModify( TRUE );
        }
    }
}

void CNProjectTaskCard::setStartTime( const ptime &_time )
{
    m_start_time = _time;
    setPrjModify();
}

void CNProjectTaskCard::setEndTime( const ptime &_time )
{
    m_end_time = _time;
    setPrjModify();
}

bool CNProjectTaskCard::setStartTime( const kn_char* _time )
{
    if( _time == NULL )
    {
        return false;
    }

    kn_string str( _time );
    m_start_time = timestr2ptime( str );

    setPrjModify();
    return true;
}

bool CNProjectTaskCard::setEndTime( const kn_char* _time )
{
    if( _time == NULL )
    {
        return false;
    }

    kn_string str( _time );
    m_end_time = timestr2ptime( str );

    setPrjModify();
    return true;
}

void CNProjectTaskCard::setState( int _state )
{
    if ( m_state != _state )
    {
        //保存变更记录
        m_vChangeHistory.push_back( CActivityChangeHistory( m_state, _state ));
        m_state = _state;

        setPrjModify();
    }
}

//获取任务得分
kn_uint CNProjectTaskCard::getScore() const
{
    return m_score;
}

//任务评分
void CNProjectTaskCard::setScore( kn_uint score )
{
    //TODO:每一个任务总分暂定为10分
    m_score = score;

    setPrjModify();
}

//任务难易程度（1-5）
kn_byte CNProjectTaskCard::getDifficulty() const
{
    return m_difficulty;
}

kn_bool CNProjectTaskCard::setDifficulty( kn_byte difficulty )
{
    kn_bool _result = false;

    if((difficulty >= 1) && (difficulty <= 5))
    {
        m_difficulty = difficulty;
        _result      = true;
    }

    return _result;
}
