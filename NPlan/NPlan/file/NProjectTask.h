//  File:        NProjectTask.h
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
#ifndef NProjectTask_h__
#define NProjectTask_h__

#include "utility.h"
#include "NProjectTaskCard.h"
#include "NProjectObject.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "NProjectResource.h"
using namespace boost:: posix_time;

class CNProjectData;
class CNProjectTask : public CNProjectTaskCardBase
{
public:

    CNProjectTask();

    CNProjectTask(const CNProjectTask& other);

    CNProjectTask( const kn_char*_name, int _id, const kn_char*s_time, const kn_char*e_time, int _leader_id = 0 );

    virtual ~CNProjectTask( void );

protected:

    int m_color;

    //任务卡列表
    CNProjectTaskCard_LST m_taskcards;

	//任务卡指针链表，用于按人员分组的action
	CNProjectTaskCard_Point_LST m_taskcard_points;

    //任务描述
    kn_string m_task_describe;

    //任务参与人员列表
    std::vector <int> m_members;



    //成员名字,
    vector <kn_string> m_MembersName;

    CNProjectData *m_npdata;

public:
	virtual int getLeaderId() const;
	virtual kn_string getLeaderName() const;
	virtual ptime getStartTime() const;
	virtual ptime getEndTime() const;
	virtual void setLeaderId( int _id );
	virtual void setStartTime(const ptime &_time);
	virtual void setEndTime(const ptime &_time);
	virtual bool setStartTime( const kn_char* _time );
	virtual bool setEndTime( const kn_char* _time );

    CNProjectTask &operator=(const CNProjectTask &d1);

    //获得在任务vector中的位置
    int getVectorId();
	//获得action列表
    CNProjectTaskCard_LST& getTaskCards();
	//获得用于人员按分组的action列表，人员分组存储指针，指向其它task内部的action
	CNProjectTaskCard_Point_LST& getTaskCardsByMan();

    void setTaskCards( CNProjectTaskCard_LST& card );

    void setTaskInfo( const kn_char* _name, int _id, const kn_char* s_time, const kn_char* e_time, int _leader_id = 0 );

    void setTaskInfo( const kn_char* _name, int _id, const ptime &s_time, const ptime &e_time, int _leader_id = 0 );

    const vector <int>  &getMembers() const;

    void setMembers(const vector <int>& vectMembers );

	void addMembers(int id );
     
    vector <int>&  getMembers();

    void addTaskCard( kn_char* _name, int _leader_id, int days, int hours, int _state, int id = 0 );

    void addTaskCard( kn_char* _name, kn_char* _leader_name, int days, int hours, kn_char* _state, int id = 0);

    void addTaskCard( CNProjectTaskCard & pTaskCard );

	void delTaskCard( CNProjectTaskCard & pTaskCard );

	void addTaskCardPoint( CNProjectTaskCard* pTaskCard );

	void offsetTaskCardTime(time_duration td);

	//获取输入state值的任务卡数量
	int getTaskCardCount( int state);
	//根据人员分组时统计任务卡数量 m_taskcard_points
	int getManCardCount( int state);

//    int getLeaderId()  const;

//    void setLeaderId( int _id );

//    ptime getStartTime() const;

//    ptime getEndTime() const;

 //   bool setStartTime(const kn_char* _time );
	//bool setStartTime( ptime& _time );
	//bool setEndTime(const kn_char* _time );
	//bool setEndTime( ptime& _time );
    
	bool setEndTime(kn_string _time );



    void setDataPointer( CNProjectData *pdata );
	CNProjectData* getProject();

//    kn_string getLeaderName() const;

    void setLeaderName( const kn_char* _leader) ;

    int getColorType() const;

    void  setColorType( int type );

    //update MembersName by id
    void updateMembersName() const;

    const vector <kn_string>& getMembersName() const;

    kn_string getMembersNameStr( bool b_hide_leader = FALSE ) const ;

    kn_string getTaskDescribe() const;

    void setTaskDescribe(const kn_char* describe );

	kn_string getMembersNameById(kn_int _id);

	kn_string getStateStrById(kn_int _id);

	kn_int getResourceIdByName( const kn_char* name );

    void clearTaskCards();

	vector<CNProjectResource>& getProjectMember();

	res_type getResourceTypeById(kn_int _id);

	void setModify(kn_bool);
private:

    int getUnusedCardId();
};

#define CNProjectTask_LST std::list<CNProjectTask>


#endif
