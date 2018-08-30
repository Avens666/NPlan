//  File:        NProjectTaskCard.h
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
#ifndef NProjectTaskCard_h__
#define NProjectTaskCard_h__

#include "utility.h"
#include "NProjectObject.h"
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost:: posix_time;

//子任务状态变更信息
class CActivityChangeHistory
{
public:
    CActivityChangeHistory();
    CActivityChangeHistory( int old_state, int new_state, ptime* change_time = NULL );
    void setOldState( int old_state );
    void setNewState( int new_state );
    void setChangeTime( const ptime & change_time );
    int getOldState() const;
    int getNewState() const;
    ptime getChangeTime() const;
private:
    int m_nOldState, m_nNewState;
    ptime m_change_time;
};

class CNProjectTask;
#define TaskCardBase_TASK_TYPE 0
#define TaskCardBase_TASKCARD_TYPE 1
class CNProjectTaskCardBase : public CNProjectObject
{
public:
	CNProjectTaskCardBase(){};
	CNProjectTaskCardBase( const kn_char*_name, int _id = 0 ):CNProjectObject(_name, _id) {};
	virtual ~CNProjectTaskCardBase(){};

	virtual int getLeaderId() const = 0;
	virtual kn_string getLeaderName() const  = 0;
	virtual ptime getStartTime() const = 0;
	virtual ptime getEndTime() const = 0;
	virtual void setLeaderId( int _id ) = 0;
	virtual void setStartTime(const ptime &_time) = 0;
	virtual void setEndTime(const ptime &_time) = 0;
	virtual bool setStartTime( const kn_char* _time ) = 0;
	virtual bool setEndTime( const kn_char* _time ) = 0;
protected:

	//责任人编号
	int m_leader_id;
	//活动开始执行时间
	ptime m_start_time;

	//活动结束时间
	ptime m_end_time;

	//判断是taskcard还是task
	int m_task_type;
};

class CNProjectTaskCard : public CNProjectTaskCardBase
{
public:
    CNProjectTaskCard();

    CNProjectTaskCard( const kn_char * _name, int _leader_id, int days, int hours, int _state = 0, int _id = 0 );
	CNProjectTaskCard( const CNProjectTaskCard& other );

    virtual ~CNProjectTaskCard( void );

	virtual int getLeaderId() const;
	virtual kn_string getLeaderName() const;
	virtual ptime getStartTime() const;
	virtual ptime getEndTime() const;
	virtual void setLeaderId( int _id );
	virtual void setStartTime(const ptime &_time);
	virtual void setEndTime(const ptime &_time);
	virtual bool setStartTime( const kn_char* _time );
	virtual bool setEndTime( const kn_char* _time );

    bool operator == (int card_id) const;

    const CNProjectTask* getTask() const;

    int getPlanDays() const;

    int getPlanHours() const;

    int getState() const;

	kn_byte getDifficulty() const ;

	kn_bool isDone();
    //获取任务得分
    kn_uint getScore() const;

    //获取状态变更列表
    const vector <CActivityChangeHistory>& getStateChangeList() const;

    void setTask( CNProjectTask* pTask );

    void setPlanDays( int _days );

	void setPlanHours( int _hours );

    void setState( int _state );

	bool setDifficulty( kn_byte difficulty );

    //任务评分
    void setScore(kn_uint score);
	void setPrjModify();

protected:
    int m_plan_days;//计划天数
    int m_plan_hours;//计划小时数
    int m_state;	//任务执行状态

	kn_byte m_difficulty; //任务难易程度（1-5）
    kn_uint m_score;//任务评分

    CNProjectTask*  m_pTask;

    //状态变更记录
    vector <CActivityChangeHistory> m_vChangeHistory;
};

#define CNProjectTaskCard_LST std::list<CNProjectTaskCard>
//指针链表，用于按人员分组的action
#define CNProjectTaskCard_Point_LST std::list<CNProjectTaskCard*>

#endif
