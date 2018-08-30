//  File:        NProjectMPPXMLData.cpp
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
#include "NProjectMPPXMLData.h"
#include "../manager/KProjectManager.h"
#include <numeric>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost:: posix_time;

CNProjectMPPXMLData::CNProjectMPPXMLData ( CNProjectData* pData ) : CNProjectBaseFile( pData )
{
    m_p_xmldoc = NULL;
    m_p_root   = NULL;
    m_is_nplan_format = false;
}

CNProjectMPPXMLData::~CNProjectMPPXMLData( void )
{
    if ( m_p_xmldoc )
    {
        delete m_p_xmldoc;
    }
}

//加载xml文件
int CNProjectMPPXMLData::load( kn_string &filename )
{
    int _result = ERROR_FUNCTION_FAILED;

    string file = ns2s( filename );
    m_p_xmldoc = new TiXmlDocument( file.c_str());
    if( NULL == m_p_xmldoc )
    {
        return ERROR_FILE_NOT_FOUND;
    }

    if ( !m_p_xmldoc->LoadFile())
    {
        return ERROR_FILE_INVALID;
    }

    m_p_root = m_p_xmldoc->RootElement();

    if( m_pPrjData != NULL)
    {
		//先解析资源，再解析基本信息。以便将基本信息中的PM名称转化为ID
		parseResources();

		parseBaseInfo();

		_result = parseTasks();

        if ( _result != NO_ERROR )
        {
            return _result;
        }

        parseAssignment();
    }

    return NO_ERROR;
}

//解析项目基本信息
bool CNProjectMPPXMLData:: parseBaseInfo()
{
    if( m_p_root == NULL )
    {
        return false;
    }

   /* kn_string str_name = getChildNodeText( m_p_root, "Title" );
    if( !str_name.empty() && (kn_string::npos == str_name.find( _T( "MSProj" )) ))
    {
        m_pPrjData->setName( str_name.c_str());
    }*/

    //标识此文件格式为NPlan导出的扩展格式
    kn_string str_name = getChildNodeText( m_p_root, "NPlan" );
    m_is_nplan_format = ( str_name == _T("1") );

    //m_pPrjData->m_baseinfo.setLeader( getChildNodeText( m_p_root, "Manager" ).c_str());
	//PM也是资源，应保存资源ID，而不是名称
	int pm_id = m_pPrjData->m_respool.findResourceID( getChildNodeText( m_p_root, "Manager" ).c_str() );
	m_pPrjData->m_baseinfo.setId(pm_id);

    return true;
}

//解析资源
bool CNProjectMPPXMLData:: parseResources()
{
    bool _result = false;

    if( NULL == m_p_root )
    {
        return _result;
    }

    TiXmlElement* p_resources = m_p_root->FirstChildElement( "Resources" );
    if( NULL == p_resources )
    {
        return _result;
    }

    TiXmlElement* p_res = p_resources->FirstChildElement( "Resource" );
    if( NULL == p_res )
    {
        return _result;
    }

    _result = true;

	CNProjectResourcePool* p_res_pool = getProjectManager()->getGlobalResource();

    for (; p_res != NULL; p_res = p_res->NextSiblingElement())
    {
        kn_string str_text = getChildNodeText( p_res, "Name" );
        if( str_text.empty())
        {
            continue;
        }

		CNProjectResource res( str_text.c_str(), getChildNodeValue( p_res, "UID" ), -1 );

		//全局资源池中去查找匹配的资源，以便获取资源类型
		if(p_res_pool)
		{
			if( res.getId() == p_res_pool->findResourceGroupID( res.getName().c_str() ) )
			{
				res.setType( p_res_pool->getResourceType( res.getId() ) );
			}
		}

        m_pPrjData->m_respool.addResource( res );
    }

    return _result;
}

//查找第一个有效的任务节点
TiXmlElement* CNProjectMPPXMLData::findFirstValidTaskElem()
{
    TiXmlElement* first_task_element = NULL;

    TiXmlElement* p_tasks = m_p_root->FirstChildElement( "Tasks" );

    if( NULL == p_tasks )
    {
        return first_task_element;
    }

    TiXmlElement* p_task = p_tasks->FirstChildElement();

    if ( p_task && (0 == getChildNodeValue( p_task, "OutlineLevel" ))) //MS Project中的非显示用对象
    {
        m_pPrjData->setName( getChildNodeText( p_task, "Name" ).c_str() );
        m_pPrjData->m_baseinfo.setProDescribe( getChildNodeText( p_task, "Notes" ).c_str() );
        p_task = p_task->NextSiblingElement();
    }

    if( NULL == p_task )
    {
        return first_task_element;
    }

    first_task_element = p_task;

    vector <int> vect_level;

    for (; p_task != NULL; p_task = p_task->NextSiblingElement())
    {
        int level = getChildNodeValue( p_task, "OutlineLevel" ); //任务层数
        if( level > 3 )
        {
            //因为NPlan中只支持任务及子任务两层结构，任务层数>3的格式不支持
            first_task_element = NULL;
            return first_task_element;
        }

        vect_level.push_back( level );
    }

    vector <int>::iterator f = std::max_element( vect_level.begin(), vect_level.end());
    if( f != vect_level.end())
    {
        if((*f) == 3 )
        {
            //任务层数=3，但是在在多个1级任务的，也不支持(1级任务只能当作项目名称，且仅有一个)
            if ( std::count( vect_level.begin(), vect_level.end(), 1 ) > 1 )
            {
                first_task_element = NULL;
                return first_task_element;
            }
            else
            {
                //自动过滤1级节点（项目名称）
                first_task_element = first_task_element->NextSiblingElement();
            }
        }
    }

    return first_task_element;
}

//计算两个时间之间的间隔，返回间隔天数和小时数
void CNProjectMPPXMLData::calcTimeInterval( kn_string &start_time, kn_string &end_time, OUT int& days, OUT int& hours )
{
    days = hours = 0; //clear first

    if( start_time.empty() || end_time.empty())
    {
        return;
    }

    ptime begin_time = timestr2ptime( start_time );
    if( begin_time.is_not_a_date_time())
    {
        return;
    }

    ptime finish_time = timestr2ptime( end_time );
    if( finish_time.is_not_a_date_time())
    {
        return;
    }

    boost::posix_time::time_duration apart_time = finish_time -begin_time;

    days  = apart_time.hours() / 24;
    hours = apart_time.hours() % 24;
}

int CNProjectMPPXMLData::calcActionStateByPercent(int precent)
{
    int action_state = 0;

    if( precent <= 0 )
    {
        action_state = 0; //TODO
    }
    else if( precent == 100 )
    {
        action_state = 3; //DONE
    }
    else 
    {
        if(m_is_nplan_format)
        {
            if ( precent <= 40 )
            {
                action_state = 1; //IN PROGRESS
            }
            else if( precent >= 80)
            {
                action_state = 2; //VERIFY
            }
        }
        else
        {
            if( precent >= 50 )
            {
                action_state = 2; //VERIFY
            }
            else
            {
                precent = 1; //IN PROGRESS
            }
        }
    }

    return action_state;
}

//解析任务列表
int CNProjectMPPXMLData::parseTasks()
{
    int _result = NO_ERROR;

    if( NULL == m_p_root )
    {
        return _result;
    }

    TiXmlElement* p_task = findFirstValidTaskElem();
    if( NULL == p_task )
    {
        return ERROR_FILE_INVALID;
    }

    CNProjectMileStone milestone;
    CNProjectTask task;
    CNProjectTaskCard action;

    bool first_time = true;

    int days( 0 ), hours( 0 );

    int outline_level = getChildNodeValue( p_task, "OutlineLevel" ); //TASK的级别

    for (; p_task != NULL; p_task = p_task->NextSiblingElement())
    {
        kn_string str_text = getChildNodeText( p_task, "Name" );
        if( str_text.empty())
        {
            continue;
        }

        int level = getChildNodeValue( p_task, "OutlineLevel" ); //任务层数
        if( outline_level == level )
        {
            if( !first_time ) //插入完task card后，再添加进数组
            {
                m_pPrjData->addTask( task );
                task.clearTaskCards();
                task.setName( _T( "" )); //清空名称，用来判断是否已经保存
            }

            //TODO:MS PROJECT 一级任务没有责任人
            task.setName( str_text.c_str());
            task.setId( getChildNodeValue( p_task, "UID" ));
            task.setStartTime( getChildNodeText( p_task, "Start" ).c_str());
            task.setEndTime( getChildNodeText( p_task, "Finish" ).c_str());
            task.setTaskDescribe( getChildNodeText( p_task, "Notes" ).c_str());

            first_time = false;
        }
        else
        {
            action.setName( str_text.c_str());
            action.setId( getChildNodeValue( p_task, "UID" ));

            kn_string start_time = getChildNodeText( p_task, "Start" );
            kn_string end_time   = getChildNodeText( p_task, "Finish" );

            if(m_is_nplan_format)//NPlan导出的扩展MS Project格式
            {
                action.setPlanDays( getChildNodeValue( p_task, "PlanDays" ) );
                action.setPlanHours( getChildNodeValue( p_task, "PlanHours" ) );
				action.setDifficulty( getChildNodeValue( p_task, "Difficulty" ) );
            }

            calcTimeInterval( start_time, end_time, days, hours );

            action.setPlanDays( days );
            action.setPlanHours( hours );
            action.setStartTime( start_time.c_str() );
            action.setEndTime( end_time.c_str() );

            int done_precent = getChildNodeValue( p_task, "PercentComplete" );
            done_precent = calcActionStateByPercent(done_precent);

            action.setState( done_precent );

            task.addTaskCard( action );
        }

        if( 1 == getChildNodeValue( p_task, "Milestone" ))
        {
            if ( outline_level == level )
            {
                milestone.setName( task.getName().c_str());
                milestone.setMemo( task.getTaskDescribe().c_str());
            }
            else
            {
                milestone.setName( action.getName().c_str());
            }

            milestone.setId( 0 ); //force to auto calc
            m_pPrjData->addMilestone( milestone );
        }
    }

    if( !task.getName().empty())
    {
        m_pPrjData->addTask( task );
    }

    return _result;
}

//解析任务分配,将taskID转化为resourceID
bool CNProjectMPPXMLData:: parseAssignment()
{
    bool _result = false;

    if( NULL == m_p_root )
    {
        return _result;
    }

    TiXmlElement* p_assigns = m_p_root->FirstChildElement( "Assignments" );
    if( NULL == p_assigns )
    {
        return _result;
    }

    TiXmlElement* p_assign = p_assigns->FirstChildElement( "Assignment" );
    if( NULL == p_assign )
    {
        return _result;
    }

    _result = true;

    CNProjectTask_LST::iterator task_iter;

    CNProjectTask_LST& list_tasks = m_pPrjData->getTasks();

    for(; p_assign != NULL; p_assign = p_assign->NextSiblingElement())
    {
        int task_id = getChildNodeValue( p_assign, "TaskUID" );
        int res_id  = getChildNodeValue( p_assign, "ResourceUID" );

        for( task_iter = list_tasks.begin(); task_iter != list_tasks.end(); task_iter++ )
        {
            CNProjectTaskCard_LST& list_cards  = task_iter->getTaskCards();
            CNProjectTaskCard_LST::iterator it = find( list_cards.begin(), list_cards.end(), task_id );
            if( it != list_cards.end())
            {
                it->setLeaderId( res_id );
            }
        }
    }

    return _result;
}

bool CNProjectMPPXMLData::save_all_resource( TiXmlElement* p_resources )
{
    bool _result = false;

    if((NULL == m_pPrjData) || (NULL == p_resources))
    {
        return _result;
    }

    typedef std::vector <CNProjectResource> VectResource;

    CNProjectResourcePool &ResPool = m_pPrjData->getResourcePool();

    VectResource &v_Res = ResPool.getResources();

    char szBuff[32] = {0};

    VectResource::iterator it_res = v_Res.begin();
    for (; it_res != v_Res.end(); ++it_res )
    {
        TiXmlElement* p_resource = add_node( NULL, "Resource", NULL );
        if( p_resource )
        {
            sprintf( szBuff, "%d", it_res->getId());

            add_node( p_resource, "UID", szBuff );
            add_node( p_resource, "ID", szBuff );
            kn_string wstrName = it_res->getName();
            add_node( p_resource, "Name", wstrName);
            res_type res_type = it_res->getType();

            switch (res_type)
            {
            case res_man:
            case res_woman:
                add_node( p_resource, "Type", "1" ); //对应Project中的工时
                break;
            case res_device:
                add_node( p_resource, "Type", "0" ); //对应Project中的材料
                break;
            default:
                break;
            }

            p_resources->LinkEndChild(p_resource);
        }
    }

    return _result;
}

void CNProjectMPPXMLData::save_assignment(TiXmlElement* assignments, ptime &action_stime, ptime &action_etime, vector<kn_uint> &vect_hours, CNProjectTaskCard_LST::iterator iter_taskcard, const string& str_uid)
{
    if(assignments==NULL)
    { //预防性编码，调用时控制了参数的有效性
        return;
    }

	CNProjectCalendar& calendar = m_pPrjData->getCalendar();

    TiXmlElement* assignment = add_node( assignments, "Assignment", NULL );
    if( assignment )
    {
        add_node( assignment, "UID", str_uid.c_str() );
        add_node( assignment, "TaskUID", str_uid.c_str() );

        if(iter_taskcard->getLeaderId() < 1)
        {	//未分配任务
            add_node( assignment, "ResourceUID", "-65535" );
        }
        else
        {
            char szBuff[32]={0};
            sprintf( szBuff, "%d", iter_taskcard->getLeaderId());
            add_node( assignment, "ResourceUID", szBuff );
        }

		ptime action_time;
		date action_date;
		time_duration action_td;
		boost::gregorian::days one_day( 1 );

		const size_t st_count = vect_hours.size();	
		for(size_t i = 0; i < st_count; i++)
		{
			TiXmlElement* date = add_node(assignment, "TimephasedData", NULL);
			add_node( date, "Type", "1" );
			add_node( date, "UID", str_uid.c_str() );

			if(0 == i) 
			{
				action_time = action_stime;
			}
			else
			{
				action_td = calendar.getWorkingTime();
				action_time = ptime( action_date, action_td );
			}
			string str_time = ptime2str ( action_time, true ) ;
			add_node ( date, "Start", str_time.c_str() ); 

			if(st_count-1 == i)
			{
				action_time = action_etime;
			}
			else
			{
				action_td = calendar.getClosingTime();
				action_time = ptime( action_date, action_td );
			}

			str_time = ptime2str( action_time, true);
			add_node( date, "Finish", str_time.c_str() );

			add_node( date, "Unit", "2" );

			if(0 < vect_hours[i] )
			{
				char szBuff[32]={0};
				sprintf( szBuff, "PT%dH0M0S", vect_hours[i] );
				add_node( date, "Value", szBuff );
			}

			action_date += one_day;
		}
    }
}

bool CNProjectMPPXMLData::save_all_task( TiXmlElement* tasks, TiXmlElement* assignments )
{
    bool _result = false;

    if((NULL == m_pPrjData) || (NULL == tasks) || (NULL == assignments))
    {
        return _result;
    }

    CNProjectTask_LST* p_v_task = m_pPrjData->getpTasks();
    if( p_v_task == NULL )
    {
        return _result;
    }

    ptime time_begin = m_pPrjData->getProBeginTime();
    ptime time_end = m_pPrjData->getProEndTime();

    string prj_start_time = ptime2str(time_begin, true );
    string prj_end_time = ptime2str(time_end, true );

    kn_string strName = m_pPrjData->getName();
    kn_string strPrjData = m_pPrjData->getProDescribe();

    TiXmlElement* task = add_node( tasks, "Task", NULL );
    if( task )
    {
        add_node( task, "UID", "0" );//MPP中非显示对象
        add_node( task, "ID", "0" );
        add_node( task, "Name", strName);
        add_node( task, "WBS", "0" );
        add_node( task, "OutlineNumber", "0" );
        add_node( task, "OutlineLevel", "0" );
        add_node( task, "Start", prj_start_time.c_str());
        add_node( task, "Finish", prj_end_time.c_str());
        add_node( task, "ConstraintType", "0" ); //不加此字段，时间显示会混乱
        add_node( task, "Notes", strPrjData); //项目备注
    }

    task = add_node( tasks, "Task", NULL );
    if( task )
    {
        //一级节点
        add_node( task, "UID", "1" );
        add_node( task, "ID", "1" );
        add_node( task, "Name", m_pPrjData->getName() ); //项目名称
        add_node( task, "WBS", "1" );
        add_node( task, "OutlineNumber", "1" );
        add_node( task, "OutlineLevel", "1" );
        add_node( task, "Start", prj_start_time.c_str());
        add_node( task, "Finish", prj_end_time.c_str());
        add_node( task, "ConstraintType", "0" ); //不加此字段，时间显示会混乱
    }

    UINT uid( 2 ), level1_id(1);
    char szBuff[32]     = {0};
    char szWBSBuff[256] = {0};

    string str_wbs;

	CNProjectCalendar& calendar = m_pPrjData->getCalendar();

	CNProjectTask_LST::iterator iter_task;
    for ( iter_task = p_v_task->begin(); iter_task != p_v_task->end(); ++iter_task )
    {
        TiXmlElement* task = add_node( tasks, "Task", NULL );
        if( task == NULL )
        {
            continue;
        }

        //二级节点
        memset( szBuff, 0, sizeof(char)*32 );
        sprintf( szBuff, "%d", uid++ ); 
        add_node( task, "UID", szBuff );
        add_node( task, "ID", szBuff );
        add_node( task, "Name", iter_task->getName());

        memset( szWBSBuff, 0, sizeof(char)*256 );
        sprintf( szWBSBuff, "1.%s", szBuff ); //目前WBS从1.2开始编号，但不影响显示，所以暂不修改
        add_node( task, "WBS", szWBSBuff );
        str_wbs = szWBSBuff;
        add_node( task, "OutlineNumber", szWBSBuff );
        add_node( task, "OutlineLevel", "2" );  //NPlan都只支持二级格式
        
        string start_time = ptime2str ( iter_task->getStartTime(), true );
        string end_time = ptime2str ( iter_task->getEndTime(), true );
        add_node( task, "Start", start_time.c_str());
        add_node( task, "Finish", end_time.c_str());
        add_node( task, "Estimated", "1" );        
        add_node( task, "ConstraintType", "0" ); //不加此字段，时间显示会混乱

        kn_string str = iter_task->getTaskDescribe();
        if( !str.empty())
        {
            add_node( task, "Notes", str );
        }
        
        CNProjectTaskCard_LST &lst_cards = iter_task->getTaskCards();
        CNProjectTaskCard_LST::iterator iter_taskcard;
        
        size_t action_count = lst_cards.size();
        
        int index( 1 );
        for ( iter_taskcard = lst_cards.begin(); iter_taskcard != lst_cards.end(); iter_taskcard++ )
        {
            TiXmlElement* task = add_node( tasks, "Task", NULL );
            if( task == NULL )
            {
                continue;
            }

            //三级节点
            memset( szBuff, 0, sizeof(char)*32 );
            sprintf( szBuff, "%d", uid++ );

            const string str_uid = szBuff;
            add_node( task, "UID", szBuff );
            add_node( task, "ID", szBuff );
            add_node( task, "Name", iter_taskcard->getName());

            memset( szWBSBuff, 0, sizeof(char)*256 );
            sprintf( szWBSBuff, "%s.%d", str_wbs.c_str(), index );
            add_node( task, "WBS", szWBSBuff );
            add_node( task, "OutlineNumber", szWBSBuff );
            add_node( task, "OutlineLevel", "3" );

            ptime action_stime, action_etime;
			action_stime = iter_taskcard->getStartTime();
			action_etime = iter_taskcard->getEndTime();

			if( !action_stime.is_not_a_date_time() )
			{
				time_duration action_td = action_stime.time_of_day();
				if( (action_td.hours() == 0) && (action_td.minutes() == 0) && (action_td.seconds() == 0) )
				{  //未填写时间，开始时间默认为工作日的开始上班时间
					action_stime += calendar.getWorkingTime();
				}
			}

			if( !action_etime.is_not_a_date_time() )
			{
				time_duration action_td = action_etime.time_of_day();
				if( (action_td.hours() == 0) && (action_td.minutes() == 0) && (action_td.seconds() == 0) )
				{
					//未填写时间，线束时间默认为工作日的结束上班时间
					action_etime += calendar.getClosingTime();
				}
			}

			string str_start_time = ptime2str( action_stime, true );
			string str_end_time = ptime2str( action_etime, true );

			add_node ( task, "Start", str_start_time.c_str() );
			add_node ( task, "Finish", str_end_time.c_str() );

			//NPlan导出的扩展MS Project格式
			int days = iter_taskcard->getPlanDays();
			int hours = iter_taskcard->getPlanHours();
			ostringstream oss;
			oss << days;
			add_node ( task, "PlanDays", oss.str().c_str() );
			oss.str("");

			oss << hours;
			add_node ( task, "PlanHours", oss.str().c_str() );
			oss.str("");

			oss << iter_taskcard->getDifficulty();
			add_node ( task, "Difficulty", oss.str().c_str() );
			oss.str("");

			vector<kn_uint> vect_hours = calendar.getWorkHours(action_stime , action_etime);
			const kn_uint work_hours = std::accumulate(vect_hours.begin(), vect_hours.end(), 0);
			oss << "PT" << work_hours << "H0M0S";

			float actual_work_hours = work_hours;
            string str_precent = "0";
            int state = iter_taskcard->getState();
            switch(state)
            {
            case 0:
                str_precent = "0";
				actual_work_hours = 0;
                break;
            case 1:
                str_precent = "40";
				actual_work_hours *= 0.4;
                break;
            case 2:
                str_precent = "80";
				actual_work_hours *= 0.8;
                break;
			case 3:
				str_precent = "100";	
				add_node ( task, "ActualFinish", str_end_time.c_str() );
				break;
            default:
                break;
            }

			//add_node ( task, "PercentWorkComplete", str_precent.c_str() ); //写入也无效，PROJECT会重新计算
			add_node ( task, "ActualStart", str_start_time.c_str() );

			//Percent Work Complete = (Actual Work / Work) * 100
			add_node ( task, "Work", oss.str().c_str() );	
			oss.str("");

			const int i_act_work = actual_work_hours;
			const int i_act_work_minutes = (actual_work_hours - i_act_work) * 60;
			oss << "PT" << i_act_work <<  "H" << i_act_work_minutes << "M0S";

			add_node ( task, "ActualWork", oss.str().c_str() );
			add_node ( task, "ActualDuration", oss.str().c_str() );
			oss.str("");

			add_node( task, "Estimated", "1" );
			//不加此字段，时间显示会混乱	
            (index == 1) ? add_node( task, "ConstraintType", "4" ):add_node( task, "ConstraintType", "6" );

            //assignment
            save_assignment(assignments, action_stime, action_etime , vect_hours ,iter_taskcard, str_uid);

            index++ ;
        }
    }

    return _result;
}

void CNProjectMPPXMLData::save_calendars()
{
    //TODO:目前无此项数据，仅写入DummyNODE
    TiXmlElement* calendars = add_node ( m_p_root, "Calendars", NULL );
    TiXmlElement* calendar = add_node ( calendars, "Calendar", NULL );
    add_node ( calendar, "UID", "1" );
    add_node ( calendar, "Name", kn_string(_T("标准")) );
    add_node ( calendar, "IsBaseCalendar", "1" );
    add_node ( calendar, "BaseCalendarUID", "-1" );
    TiXmlElement* weekdays = add_node ( calendar, "WeekDays", NULL );
    
    for ( int i = 1; i <= 7; i++ ) //默认星期日为每周第一天
    {
        TiXmlElement* weekday = add_node ( weekdays, "WeekDay", NULL );
        
        char szBuff[12] = {0};
        sprintf ( szBuff, "%d", i );
        add_node ( weekday, "DayType", szBuff );
        
        if ( ( i == 1 ) || ( i == 7 ) )
        {
            add_node ( weekday, "DayWorking", "0" ); //休息日
        }
        else
        {
            add_node ( weekday, "DayWorking", "1" );
            
            TiXmlElement* workingtimes = add_node ( weekday, "WorkingTimes", NULL );
            
            TiXmlElement* workingtime = add_node ( workingtimes, "WorkingTime", NULL );
            add_node ( workingtime, "FromTime", "09:00:00" );
            add_node ( workingtime, "ToTime", "12:00:00" );
            
            workingtime = add_node ( workingtimes, "WorkingTime", NULL );
            add_node ( workingtime, "FromTime", "13:00:00" );
            add_node ( workingtime, "ToTime", "18:00:00" );
        }
    }
}

int CNProjectMPPXMLData:: save( kn_string &filename )
{
    TiXmlDocument doc;

    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "yes" );

    if( decl )
    {
        doc.LinkEndChild( decl );
    }

    m_p_root = new TiXmlElement( "Project" );
    if( !m_p_root )
    {
        return ERROR_FUNCTION_FAILED;
    }

    m_p_root->SetAttribute( "xmlns", "http://schemas.microsoft.com/project" );

    size_t pos = filename.find_last_of( '/' );
    if( kn_string::npos != pos )
    {
        kn_string file = filename.substr( pos+1 );
        add_node( m_p_root, "Name", file );
    }

    add_node( m_p_root, "SaveVersion", "12" );
    add_node( m_p_root, "Title", "NPlan Export" );
    add_node( m_p_root, "Manager", m_pPrjData->getLeader());

	string prj_start_time = ptime2str( m_pPrjData->getProBeginTime(), true );
    string prj_end_time = ptime2str( m_pPrjData->getProEndTime(), true );
	add_node( m_p_root, "StartDate", prj_start_time.c_str() );
	add_node( m_p_root, "FinishDate",  prj_end_time.c_str() );

	CNProjectCalendar& calendar = m_pPrjData->getCalendar();
	
	time_duration default_start_time = calendar.getWorkingTime();
	string start_time = to_simple_string(default_start_time);

	time_duration default_end_time = calendar.getClosingTime();
	string end_time = to_simple_string(default_end_time);

	add_node( m_p_root, "DefaultStartTime", start_time.c_str() );
    add_node( m_p_root, "DefaultFinishTime", end_time.c_str() );

    add_node( m_p_root, "MinutesPerDay", "480" );
    add_node( m_p_root, "MinutesPerWeek", "2400" );
    add_node( m_p_root, "DaysPerMonth", "20" ); //TODO

    add_node( m_p_root, "NPlan", "1" ); //标识此文件格式为NPlan导出的扩展格式
    add_node( m_p_root, "ProjectExternallyEdited", "0" );

    save_calendars();

    //先不挂在父节点上，数据设置完毕后，最后将其挂在父节点上就行了
    TiXmlElement* assignments = add_node( NULL, "Assignments", NULL );

    TiXmlElement* tasks = add_node( m_p_root, "Tasks", NULL );
    if(tasks)
    {
        save_all_task( tasks, assignments );
    }

    TiXmlElement* p_resources = add_node( NULL, "Resources", NULL );
    if(p_resources)
    {
        save_all_resource( p_resources );
        m_p_root->LinkEndChild( p_resources );
    }

    if(assignments)
    {
        m_p_root->LinkEndChild( assignments ); //Assignments
    }

    doc.LinkEndChild( m_p_root );

    if ( !doc.SaveFile( ns2s( filename ).c_str()))
    {
        return ERROR_CREATE_FAILED;
    }

    return NO_ERROR;
}
