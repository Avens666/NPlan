//  File:        NProjectData.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/16
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
/**
 * Copyright (C) 2003-2013，光庭信息有限公司(http://www.kotei-info.com)
 * All rights reserved.
 *
 * 文件名称：NProjectData.h
 * 摘    要：NProject数据类
 *
 * 版    本：1.0
 * 创建日期：2013年7月31日13:42:05
 * 作    者：mingg
 *
 * 修改历史：
 * [修改序列][修改日期][修改者][修改内容]
 **/

#ifndef _NPROJECT_DATA_H
#define _NPROJECT_DATA_H

#include "NProjectDataStruct.h"
#include "NProjectTaskBoard.h"
#include "NProjectBaseInfo.h"
#include "NProjectResourcePool.h"
#include "NProjectEvent.h"
#include "NProjectMileStone.h"
#include "NProjectTask.h"
#include "NE_type.h"
#include "NProjectResActivity.h"
#include "NProjectCalendar.h"
#include "NProjectWarning.h"

/**
 * NProjectData相关接口，与其他模块交互
 * @author mingg
 * @version 1.0
 * @created 2013年7月31日13:43:35
 */
class CNProjectData
{
public:

    //初始化
    CNProjectData( void );

    CNProjectData( const CNProjectData& other );

    ~CNProjectData( void );

    //导入项目文件
    int loadPrjFile( kn_string &filename );

    //导出项目文件
    int savePrjFile();

    //导出另存为文件
    int saveAsPrjFile( kn_string &filename, nplan_fileType file_type, kn_char* lpTemplatefile = NULL );

    //获取项目文件的版本信息
    kn_string getVersion() const;

    //设置项目文件的版本信息
    void setVersion( const kn_char* version );

    //获取项目名称
    kn_string getName() const;

    //设置项目名称
    void setName( const kn_char* name );

    //获取项目描述信息
    kn_string getProDescribe() const;

    //获取项目负责人
    kn_string getLeader();

	 //获取项目负责人ID
    int getLeaderId();

	void setLeaderId(int leader_id);

    void setLeader(const kn_string & str_leader);

    //设置项目基本信息
    bool setBaseInfo( CNProjectBaseInfo *baseinfo );

    //设置项目基本信息
    bool setBaseInfo( kn_char* prjName, kn_char* dataFormatVersion, int leader_id, kn_char* describe );

    //获取项目资源池
    CNProjectResourcePool &getResourcePool();

    //设置项目资源池
    bool setResourcePool( CNProjectResourcePool & resourcepool );

    //获取项目任务墙列表
    std::vector <CNProjectTaskBoardItem> & getTaskBoardItems();

    const CNProjectTaskBoard & getTaskBoard() const;

    //设置项目任务墙
    void setTaskBoard( const std::vector <CNProjectTaskBoardItem> &taskboards );

    void setTaskBoard( CNProjectTaskBoard & taskboard );

    //增加任务墙条目
    void addTaskBoard( CNProjectTaskBoardItem & taskboard );

    //获取项目任务列表
    CNProjectTask_LST & getTasks();

    //获取项目按资源分组的action列表指针
    CNProjectTask_LST * getpTasks();

	//获取项目任务列表指针
	CNProjectTask_LST * getpManTasks();

    //通过任务卡的任务状态ID获取对应的任务状态名称
    kn_string getTaskCardStateById( int state );

    //设置项目任务列表
    bool setTasks( CNProjectTask_LST &tasks );

    //增加项目任务, 返回新添加的任务的ID, 失败返回0
    int addTask( CNProjectTask & task );

    //获取项目事件列表

    /*const*/ std::vector <CNProjectEvent> & getEvents() /*const*/;

    //添加事件,返回新添加的事件的ID, 失败返回0
    int addEvent( CNProjectEvent & event );

    //添加事件，返回新添加的事件ID
    int addEvent( kn_char* _name, kn_char* _time, kn_char* _memo, int id = 0 );

    //修改项目事件
    // TODO,该方法需尽快实现
    bool modifyEvent( int index, CNProjectEvent& event );

    //删除项目事件
    // TODO,该方法需尽快实现
    bool deleteEvent( int index );

    //获取里程碑列表

    CNProjectMileStone_LST & getMilestones() /*const*/;

    //设置里程碑列表
    void setMilestones(  const CNProjectMileStone_LST &milestones );

    //添加里程碑,返回新添加的里程碑的ID, 失败返回0
    int addMilestone( CNProjectMileStone & milestone );

    //添加里程碑,返回新添加的里程碑的ID, 失败返回0
    int addMilestone( kn_char* _name, kn_char* _time, kn_char* _memo, int id = 0 );

    //读取项目完成进度
    kn_uint getDonePercentage();

    //获得项目开始时间
    ptime getProBeginTime();

    //获得项目结束时间
    ptime getProEndTime();

    //通过资源的ID获取资源对应的名字
    kn_string getResourceNameById( int resid );

	kn_int getResourceIdByName( const kn_char* name );

    int getColor() const;

    void setColor( int _color );

    kn_string getResMembersNameStr( bool b_hide_leader = FALSE );

    kn_string getWarning();

    //按资源对所有子任务进行分组
    vectResourceActivity getActionGroupByResource();
	//根据时间取子任务
	TaskCard_LST getActionGroupByDate(date d, int mode = 1);
	

	CNProjectData &operator=(CNProjectData &d1);

	//统计项目任务卡完成情况
	int getTaskCardCount( int state );

	//获取指定时间之前或者之后最近的里程碑   dir=0 向前检索 >0 向后检索
	CNProjectMileStone* getNearMilestone( ptime t, BOOL b_dir);
    //获取所有的Warning信息
    CNProjectWarning_LST* getAllWarning();
    //获取某种类型的警告信息列表,默认获得子任务的信息,id为0则获取该类型所有的warning
    CNProjectWarning_LST getWarning(warning_source source = WS_TaskCard ,int id = 0);
	//重新加载文件
	void reload();
public:

    //项目基本信息
    CNProjectBaseInfo m_baseinfo;

    // 资源池
    CNProjectResourcePool m_respool;

    //任务墙
    CNProjectTaskBoard m_taskboard;

    //任务列表
    CNProjectTask_LST m_tasks;

	//将任务按人员分组后的列表
	CNProjectTask_LST m_man_tasks;

    //事件列表
    std::vector <CNProjectEvent> m_events;

    //里程碑列表
    CNProjectMileStone_LST m_milestones;

    CNProjectWarning_LST m_warnings;

protected:

    nplan_fileType getFileType( kn_string& filename );

    //更新子任务中的任务指针
    void updateTask();

public:

    //获取未使用的任务ID
    int getUnusedTaskId();

    //获取未使用的事件ID
    int getUnusedEventId();

    //获取未使用的里程碑ID
    int getUnusedMilestoneId();

	kn_string getFileName(){return m_filename;};
	void setFileName(kn_string fileName){m_filename=fileName;};
	bool isModify();
	void setModify(bool b);

	CNProjectCalendar& getCalendar();                 //获取项目日历
	void setCalendar(const CNProjectCalendar& other); //设置项目日历
	kn_string getPrjFilePath();
protected:

    int m_color;

    kn_string m_filename;

	bool m_b_modify;//是否被编辑

	CNProjectCalendar   m_prj_calendar; //项目日历
};



#endif
