//  File:        NProjectData.cpp
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
#include "NProjectData.h"
#include "NGlobleResourcePool.h"
#include "NGlobleTaskBoard.h"
#include "NProjectBaseFile.h"
#include "NProjectXMLData.h"
#include "NProjectExcelData.h"
#include "NProjectXLSXMLData.h"
#include "NProjectMPPXMLData.h"
#include "../manager/KStringManager.h"
#define  APART_DAYS 5

CNProjectData::CNProjectData( void )
{
    m_color = rand()*10;
	m_b_modify = FALSE;
}

CNProjectData::CNProjectData( const CNProjectData& other )
{
    //任务墙
    m_taskboard = other.getTaskBoard();

    m_color = other.getColor();
	m_b_modify = FALSE;
    //任务列表
    m_tasks.assign( other.m_tasks.begin(), other.m_tasks.end());
    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_tasks.begin(); it_task != m_tasks.end(); it_task++ )
    {
        it_task->setDataPointer( this );
    }

    //事件列表
    m_events.assign( other.m_events.begin(), other.m_events.end());

    //里程碑列表
    m_milestones.assign( other.m_milestones.begin(), other.m_milestones.end());

    //资源池
    CNProjectData& ref = const_cast <CNProjectData&>(other);
    m_respool = ref.getResourcePool();

    //基本信息
    m_baseinfo.setId( other.m_baseinfo.getId() );
    m_baseinfo.setName( other.getName().c_str() );
    m_baseinfo.setProDescribe( other.getProDescribe().c_str() );
    m_baseinfo.setVersion( other.getVersion().c_str() );
	
	//项目日历
	m_prj_calendar = other.m_prj_calendar; 
}

CNProjectData& CNProjectData::operator=( CNProjectData &other )
{
    //任务墙
    m_taskboard = other.getTaskBoard();

    m_color = other.getColor();

    //任务列表
    m_tasks.assign( other.m_tasks.begin(), other.m_tasks.end());
    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_tasks.begin(); it_task != m_tasks.end(); it_task++ )
    {
        it_task->setDataPointer( this );
    }

    //事件列表
    m_events.assign( other.m_events.begin(), other.m_events.end());

    //里程碑列表
    m_milestones.assign( other.m_milestones.begin(), other.m_milestones.end());

    //资源池
    CNProjectData& ref = const_cast <CNProjectData&>(other);
    m_respool = ref.getResourcePool();

    //基本信息
    m_baseinfo.setId( other.m_baseinfo.getId() );
    m_baseinfo.setName( other.getName().c_str() );
    m_baseinfo.setProDescribe( other.getProDescribe().c_str() );
    m_baseinfo.setVersion( other.getVersion().c_str() );

	//项目日历
	m_prj_calendar = other.m_prj_calendar; 

    m_filename = other.m_filename;
	setModify(TRUE);
    return *this;
}

CNProjectData::~CNProjectData( void )
{
}

// 获取项目资源池
CNProjectResourcePool & CNProjectData::getResourcePool()
{
    return m_respool;
}

//设置项目基本信息
bool CNProjectData::setBaseInfo( kn_char* prjName, kn_char* dataFormatVersion, int leader_id, kn_char* proDescribe )
{
    m_baseinfo.setBaseInfo( dataFormatVersion, prjName, leader_id, proDescribe );
	setModify(TRUE);
    return true;
}

//增加项目任务(返回新加任务ID)
int CNProjectData::addTask( CNProjectTask & task )
{
    if ( task.getId() == 0 )
    {
        task.setId( getUnusedTaskId());
    }

    task.setDataPointer( this );

    //测试vector 代码
    //CNProjectTask* p, *p1, *p2;
    //if (m_tasks.size() > 3)
    //{
    //	p = &m_tasks[1];
    //}

    m_tasks.push_back( task );
	setModify(TRUE);
    //if (m_tasks.size() > 3)
    //{
    //	p = &m_tasks[1];
    //	p2 = &m_tasks[2];
    //	m_tasks.erase( m_tasks.begin()+1 );
    //	p1  = &m_tasks[1];
    //}
    return task.getId();
}

int CNProjectData::addEvent( CNProjectEvent & event )
{
    if ( event.getId() == 0 )
    {
        event.setId( getUnusedEventId());
    }

    m_events.push_back( event );

    return (event.getId());
}

int CNProjectData::addEvent( kn_char* _name, kn_char* _time, kn_char* _memo, int id /*=0*/ )
{
    if ( id == 0 )
    {
        id = getUnusedEventId();
    }

    m_events.push_back( CNProjectEvent( _name, _time, _memo, id ));
	setModify(TRUE);
    return (id);
}

int CNProjectData::addMilestone( CNProjectMileStone & milestone )
{
    if ( milestone.getId() == 0 )
    {
        milestone.setId( getUnusedMilestoneId());
    }

    m_milestones.push_back( milestone );
	setModify(TRUE);
    return (milestone.getId());
}

int CNProjectData::addMilestone( kn_char* _name, kn_char* _time, kn_char* _memo, int id /*=0*/ )
{
    if ( id == 0 )
    {
        id = getUnusedMilestoneId();
    }

    m_milestones.push_back( CNProjectMileStone( _name, _time, _memo, id ));
	setModify(TRUE);
    return id;
}

nplan_fileType CNProjectData::getFileType( kn_string& filename )
{
    nplan_fileType file_type = UNKNOWN_PRJ_FILE;

    size_t stIndex = filename.rfind( _T( '.' ));

    if ( stIndex != std::string::npos )
    {
        kn_string strExt = filename.substr( stIndex+1 );
        if ((0 == _tcsicmp( strExt.c_str(), _T( "xls" )))
            || (0 == _tcsicmp( strExt.c_str(), _T( "xlsx" ))))
        {
            file_type = XLS_PRJ_FILE;
        }
        else if ( 0 == _tcsicmp( strExt.c_str(), _T( "nprj" )))
        {
            file_type = XML_PRJ_FILE;
        }
        else if( 0 == _tcsicmp( strExt.c_str(), _T( "xml" )))
        {
            string file = ns2s( filename );

            TiXmlDocument* p_xmldoc = new TiXmlDocument( file.c_str());
            if( p_xmldoc && p_xmldoc->LoadFile())
            {
                TiXmlElement* p_root = p_xmldoc->RootElement();
                if( p_root && (NULL != p_root->FirstChildElement( "ExcelWorkbook" )) )  //EXCEL xml
                {
                    file_type = XLS_XML_FILE;
                }
                else if(p_root && ( p_root->ValueTStr() == "Project") )
                {
                    file_type = MPP_XML_FILE;
                }
            }
            SAFE_DELETE(p_xmldoc);
        }
    }

    return file_type;
}

void CNProjectData::reload()
{
	if(!IsFileExist(m_filename.c_str()))
	{
		return ;
	}
	m_taskboard.getBoardItems().clear();
	m_respool.clear();
	m_tasks.clear();
	m_man_tasks.clear();

	//事件列表
	m_events.clear();

	//里程碑列表
	m_milestones.clear();
	m_warnings.clear();

	loadPrjFile(m_filename);
}
//导入项目文件
int CNProjectData::loadPrjFile( kn_string &filename )
{
    int _result = ERROR_FUNCTION_FAILED;

    //if( _taccess( filename.c_str(), 0 ) != 0 ) //文件不存在
    if(!IsFileExist(filename.c_str()))
    {
        return ERROR_FILE_NOT_FOUND;
    }

    nplan_fileType file_type = getFileType( filename );

    std::replace( filename.begin(), filename.end(), _T( '\\' ), _T( '/' ));

    m_filename = filename;

    switch( file_type )
    {
    case XML_PRJ_FILE:
        {
            CNProjectXMLData xmlData( this );
            _result = xmlData.load( filename );
        }
        break;
    case XML_PRJ_BUFFER:
        {
            CNProjectXMLData xmlData( this );
            _result = xmlData.load( filename, XML_PRJ_BUFFER );
        }
        break;
        //case XML_RES_FILE:
        //    {
        //    CNGlobleResourcePool * pResPool = CNGlobleResourcePool::getGlobleResPoolInstance();
        //    _result = pResPool->loadFile( filename );
        //    }
        //    break;
    case XML_TASKBOARD_FILE:
        {
            CNGlobleTaskBoard * globoard = CNGlobleTaskBoard::getGlobleTaskBoardInstance();
            _result = globoard->loadFile( filename );
        }
        break;
    case XLS_XML_FILE:
        {
            CNProjectXLSXMLData xlsxmlData( this );
            _result = xlsxmlData.load( filename );
        }
        break;
    case MPP_XML_FILE:
        {
            CNProjectMPPXMLData mppxmlData( this );
            _result = mppxmlData.load( filename );
        }
        break;

#ifdef WIN32

        //EXCEL导入导出仅在PC版提供
    case XLS_TASK_FILE:
        {
            CNProjectExcelData xlsData( this );
            _result = xlsData.load( filename, file_type );
        }
        break;
    case XLS_RESOURCE_FILE:
        {
            CNProjectExcelData xlsData( this );
            _result = xlsData.load( filename, file_type );
        }
        break;
    case  XLS_PRJ_FILE:
        {
            CNProjectExcelData xlsData( this );
            _result = xlsData.load( filename, file_type );
        }
        break;
#endif
    default:
        break;
    }
	setModify(FALSE);
    return _result;
}

int CNProjectData::saveAsPrjFile( kn_string &filename, nplan_fileType file_type, kn_char* lpTemplatefile /*= NULL*/ )
{
    int _result = NO_ERROR;

    std::replace( filename.begin(), filename.end(), _T( '\\' ), _T( '/' ));

	switch( file_type )
	{
	case XML_PRJ_FILE:
		{       //NPlan自己的XML格式
			CNProjectXMLData xmlData( this );
			_result = xmlData.save( filename );
		}
		break;
#ifdef WIN32
		//EXCEL导入导出仅在PC版提供
	case XLS_PRJ_FILE:
		{
			try
			{
				CNProjectExcelData xlsData( this );
				_result = xlsData.save( filename, lpTemplatefile );
			}
			catch (...)
			{
				_result = ERROR_FUNCTION_FAILED;
			}
		}
		break;
#endif
	case MPP_XML_FILE:
		{       //导出MS Project的XML格式
			CNProjectMPPXMLData mpp_xml_data( this );
			_result = mpp_xml_data.save( filename );
		}
		break;
	case XLS_XML_FILE:
		{       //导出Excel的XML格式(基于我们定义的模板)
			CNProjectXLSXMLData xls_xml_data( this );
			_result = xls_xml_data.save( filename );
		}
		break;
	default:
		break;
	}

    return _result;
}

kn_string CNProjectData::getPrjFilePath()
{
	return m_filename;
}

//将之前打开的项目文件重新保存， 必须先调用loadPrjFile方法
//保存NPlan自定义的XML文件，后缀名为.nprj
int CNProjectData::savePrjFile()
{
    int _result = NO_ERROR;

    if ( m_filename.empty()) // 必须先调用loadPrjFile方法
    {
        return ERROR_FUNCTION_FAILED;
    }

    CNProjectXMLData xmlData( this );
    _result = xmlData.save( m_filename );
	setModify(FALSE);
    return _result;
}

//通过任务卡中的任务状态ID获取对应的任务状态名称
kn_string CNProjectData::getTaskCardStateById( int state_id )
{
    kn_string strStateName( _T( "" ));

    if ((state_id < 0) || (state_id > m_taskboard.getBoardItems().size()))
    {
        return strStateName;
    }

    vector <CNProjectTaskBoardItem>& boards = m_taskboard.getBoardItems();
    vector <CNProjectTaskBoardItem>::iterator iter;
    for ( iter = boards.begin(); iter != boards.end(); ++iter )
    {
        if ( iter->getId() == state_id )
        {
            return iter->getName();
        }
    }

    return strStateName;
}

//设置里程碑列表
void CNProjectData::setMilestones( const CNProjectMileStone_LST &milestones )
{
    m_milestones.clear();
    std::copy( milestones.begin(), milestones.end(), std::back_inserter( m_milestones ));
	setModify(TRUE);
}

//读取项目完成进度
kn_uint CNProjectData::getDonePercentage()
{
    if ( m_tasks.empty())
    {
        return 0;
    }

    const int max_score = m_taskboard.getBoardItems().size() -1;  //第一项为0分
    if ( max_score <= 0 )
    {
        return 0;
    }

    int tastcard_count = 0;
    int card_score     = 0;
    int percent        = 0;
	int total_score =0;
	int state = 0;
	int workday = 0;
    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_tasks.begin(); it_task != m_tasks.end(); it_task++ )
    {
        CNProjectTaskCard_LST& taskcard = it_task->getTaskCards();

        CNProjectTaskCard_LST::iterator it_taskcard;
        for ( it_taskcard = taskcard.begin(); it_taskcard != taskcard.end(); it_taskcard++ )
        {
            tastcard_count++;     //子任务的个数

            state = it_taskcard->getState();
			workday = it_taskcard->getPlanDays()*8+it_taskcard->getPlanHours();

            // 第一个状态定为0分，之后每一个状态加一分，如TODO(未开始)为0分，
            // IN Progress(正在进行中) 为1分， ....
            if((state > 0) && (state <= max_score))
            {
                card_score += state*workday;   //将任务状态转化为得分
            }
			//增加工时的计算
			total_score += max_score* workday;
        }
    }

    if ( tastcard_count == 0 )  //项目还没有指定任何小任务
    {
        return 0;
    }

//    int total_score = tastcard_count * max_score;
	if (total_score < 1)
	{
		percent = 0;
	}
	else
	{
		percent = card_score*100 / total_score;
	}
    return percent;
}

//获得项目开始时间
ptime CNProjectData::getProBeginTime()
{
    ptime min_time;

    if ( m_tasks.empty())
    {
        return min_time;
    }

    //min_time = m_tasks.front().getStartTime();
    ptime max_time( max_date_time );
    min_time = max_time;

    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_tasks.begin(); it_task != m_tasks.end(); it_task++ )
    {
        if ( it_task->getStartTime().is_not_a_date_time())
        {
            continue;
        }

        if( it_task->getStartTime() < min_time )
        {
            min_time = it_task->getStartTime();
        }
    }

	//加上里程碑的遍历
	CNProjectMileStone_LST::iterator it_mile;
	for ( it_mile = m_milestones.begin(); it_mile != m_milestones.end(); it_mile++ )
	{
		if ( it_mile->getTime().is_not_a_date_time())
		{
			continue;
		}

		if( it_mile->getTime() < min_time )
		{
			min_time = it_mile->getTime();
		}
	}
    if ( max_time == min_time ) //都为无效值  则返回无效值
    {
        ptime no_time;
        return no_time;
    }

    return min_time;
}

//获得项目结束时间
ptime CNProjectData::getProEndTime()
{
    ptime max_time;

    if ( m_tasks.empty())
    {
        return max_time;
    }

    //设置为最小时间，防止第一个时间无效 zhic
    ptime min_time( min_date_time );
    max_time = min_time;

    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_tasks.begin(); it_task != m_tasks.end(); it_task++ )
    {
        if ( it_task->getEndTime().is_not_a_date_time())
        {
            continue;
        }

        if( it_task->getEndTime() > max_time )
        {
            max_time = it_task->getEndTime();
        }
    }

	//加上里程碑的遍历
	CNProjectMileStone_LST::iterator it_mile;
	for ( it_mile = m_milestones.begin(); it_mile != m_milestones.end(); it_mile++ )
	{
		if ( it_mile->getTime().is_not_a_date_time())
		{
			continue;
		}

		if( it_mile->getTime() > max_time )
		{
			max_time = it_mile->getTime();
		}
	}

    if ( max_time == min_time ) //都为无效值  则返回无效值
    {
        ptime no_time;
        return no_time;
    }

    return max_time;
}

std::vector <CNProjectTaskBoardItem> & CNProjectData::getTaskBoardItems()
{
    return m_taskboard.getBoardItems();
}

const CNProjectTaskBoard & CNProjectData::getTaskBoard() const
{
    return m_taskboard;
}

void CNProjectData::setTaskBoard( const std::vector <CNProjectTaskBoardItem> &taskboards )
{
    m_taskboard.setBoardItems( taskboards );
	setModify(TRUE);
}

void CNProjectData::setTaskBoard( CNProjectTaskBoard & taskboard )
{
    m_taskboard = taskboard;
	setModify(TRUE);
}

//更新子任务中的任务指针
void CNProjectData::updateTask()
{
    CNProjectTask_LST::iterator it = m_tasks.begin();
    for (; it != m_tasks.end(); ++it )
    {
        CNProjectTaskCard_LST& v_cards = it->getTaskCards();
        if( !v_cards.empty())
        {
            if( NULL == v_cards.front().getTask())
            {
                CNProjectTaskCard_LST::iterator card_it;

                //for ( size_t i = 0; i < v_cards.size(); i++ )
                for( card_it = v_cards.begin(); card_it != v_cards.end(); ++card_it )
                {
                    (*card_it).setTask( &(*it));
                }
            }
        }
    }
}

CNProjectTask_LST & CNProjectData::getTasks()
{
    updateTask();
    return m_tasks;
}

CNProjectTask_LST * CNProjectData::getpTasks()
{
    updateTask();
    return &m_tasks;
}

/*const*/ std::vector <CNProjectEvent> & CNProjectData::getEvents() /*const*/
{
    return m_events;
}

/*const*/ CNProjectMileStone_LST & CNProjectData::getMilestones() /*const*/
{
    return m_milestones;
}

//获取项目文件的版本信息
kn_string CNProjectData::getVersion() const
{
    return m_baseinfo.getVersion();
}

//设置项目文件的版本信息
void CNProjectData:: setVersion( const kn_char* version )
{
    if( version != NULL )
    {
        m_baseinfo.setVersion( version );
    }
}

//获取项目名称
kn_string CNProjectData::getName() const
{
    return m_baseinfo.getName();
}

//设置项目名称
void CNProjectData::setName( const kn_char* name )
{
    if( name != NULL )
    {
        m_baseinfo.setName( name );
    }
	setModify(TRUE);
}

//获取项目描述信息
kn_string CNProjectData::getProDescribe() const
{
    return m_baseinfo.getProDescribe();
}

//获取项目负责人
kn_string CNProjectData::getLeader()
{
	int pm_id = m_baseinfo.getId();
	kn_string str_pm = getResourcePool().getResourceName(pm_id);
	return str_pm;
    //return m_baseinfo.getLeader();
}

//获取项目负责人ID
int CNProjectData::getLeaderId()
{
	return  m_baseinfo.getId();
}

void CNProjectData::setLeaderId(int leader_id)
{
	m_baseinfo.setId(leader_id);
	setModify(TRUE);
}

void CNProjectData::setLeader(const kn_string & str_leader)
{
	int leader_id = getResourcePool().findResourceID( str_leader.c_str() );
	setLeaderId(leader_id);
}

int CNProjectData::getUnusedTaskId()
{
    CNProjectTask_LST::iterator iter;
    vector <int> vecids;
    for ( iter = m_tasks.begin(); iter != m_tasks.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.size() == 0 )
    {
        return 1;
    }
    else
    {
        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))     //没找到
            {
                return i;
            }
        }

        return (vecids.size() + 1);
    }
}

int CNProjectData::getUnusedEventId()
{
    vector <CNProjectEvent>::iterator iter;
    vector <int> vecids;
    for ( iter = m_events.begin(); iter != m_events.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.size() == 0 )
    {
        return 1;
    }
    else
    {
        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))     //没找到
            {
                return i;
            }
        }

        return (vecids.size() + 1);
    }
}

int CNProjectData::getUnusedMilestoneId()
{
    CNProjectMileStone_LST::iterator iter;
    vector <int> vecids;
    for ( iter = m_milestones.begin(); iter != m_milestones.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.size() == 0 )
    {
        return 1;
    }
    else
    {
        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))     //没找到
            {
                return i;
            }
        }

        return (vecids.size()+1);
    }
}

kn_string CNProjectData::getResourceNameById( int resid )
{
    return m_respool.getResourceName( resid );
}

kn_int CNProjectData::getResourceIdByName( const kn_char* name )
{
    return m_respool.findResourceID( name );
}

bool CNProjectData::setResourcePool( CNProjectResourcePool & resourcepool )
{
    m_respool = resourcepool;
    return true;
}

bool CNProjectData::deleteEvent( int index )
{
    //todo
    return false;
}

bool CNProjectData::modifyEvent( int index, CNProjectEvent& event )
{
    //todo
    return false;
}

int CNProjectData::getColor() const
{
    return m_color;
}

void CNProjectData::setColor( int _color )
{
    m_color = _color;
	setModify(TRUE);
}

kn_string CNProjectData::getResMembersNameStr( bool b_hide_leader )
{
    if ( b_hide_leader )
    {
        return m_respool.getResMembersNameStr( m_baseinfo.getId());
    }

    int dummy_id(-1);
    return m_respool.getResMembersNameStr( dummy_id );
}

TaskCard_LST CNProjectData::getActionGroupByDate(date d, int mode)
{
	CNProjectTask_LST& p_v_task = m_tasks;
	TaskCard_LST taskcard_lst;
	CNProjectTask_LST::iterator iter;

	for ( iter = p_v_task.begin(); iter != p_v_task.end(); ++iter )
	{
		if (mode == 1)
		{
			if ( (*iter).getStartTime().date() == d || (*iter).getEndTime().date() == d )
			{
				taskcard_lst.push_back( &(*iter) );
			}
		}
		else
		{
			CNProjectTaskCard_LST& cards = iter->getTaskCards();
			CNProjectTaskCard_LST::iterator it;
			for ( it = cards.begin(); it != cards.end(); ++it )
			{
				const int res_id = (*it).getLeaderId();
				if ( (*it).getStartTime().date() == d || (*it).getEndTime().date() == d )
				{
					taskcard_lst.push_back( &(*it) );
				}
			}
		}
	}
	return taskcard_lst;
}

//按资源对所有子任务进行分组
vectResourceActivity CNProjectData::getActionGroupByResource()
{
    vectResourceActivity v_ResActivity;
    /*  vectResourceActivity::iterator finder;

       CNProjectTask_LST& p_v_task = getTasks();

       CNProjectTask_LST::iterator iter;
       for ( iter = p_v_task.begin(); iter != p_v_task.end(); ++iter )
       {
          CNProjectTaskCard_LST& cards = iter->getTaskCards();

          for ( size_t i = 0; i < cards.size(); i++ )
          {
              CResourceActivity res_activity;

              const int res_id = cards[i].getLeaderId();
              if ( res_id > 0 )
              {
                  res_activity.setResourceName( m_respool.getResourceName( res_id ));

                  finder = find( v_ResActivity.begin(), v_ResActivity.end(), res_activity );
                  if ( finder != v_ResActivity.end())
                  {
                      finder->addTaskCardRef( &cards[i] );
                  }
                  else
                  {
                      res_activity.addTaskCardRef( &cards[i] );
                      v_ResActivity.push_back( res_activity );
                  }
              }
          }
       }
     */
    return v_ResActivity;
}

CNProjectTask_LST*  CNProjectData::getpManTasks()
{
    m_man_tasks.clear();
    CNProjectTask_LST::iterator iter, it;

    for ( iter = m_tasks.begin(); iter != m_tasks.end(); ++iter )
    {
        CNProjectTaskCard_LST& cards = iter->getTaskCards();
        CNProjectTaskCard_LST::iterator card;

        //for ( size_t i = 0; i < cards.size(); i++ )
        for ( card = cards.begin(); card != cards.end(); ++card )
        {
            const int res_id = (*card).getLeaderId();
            bool b_find      = FALSE;
            for ( it = m_man_tasks.begin(); it != m_man_tasks.end(); ++it )
            {
                if ( it->getLeaderId() == res_id )
                {
                    b_find = TRUE;
                    break;
                }
            }

            if ( b_find )
            {
                it->addTaskCardPoint( &(*card));
            }
            else
            {
                CNProjectTask task;
                m_man_tasks.push_back( task );
                m_man_tasks.rbegin()->setLeaderId( res_id );
                m_man_tasks.rbegin()->setName((*card).getLeaderName().c_str());
                m_man_tasks.rbegin()->setDataPointer( this );
                m_man_tasks.rbegin()->addTaskCardPoint( &(*card));
            }
        }
    }

    return &m_man_tasks;
}

kn_string CNProjectData::getWarning()
{
    kn_string name_str                    = _T( "" );
    kn_string time_str                    = _T( "" );
    kn_string aparttime_str               = _T( "" );
    boost::posix_time::time_duration days = hours( APART_DAYS*24 );
    ptime ptNow( second_clock::local_time());

    if( !ptNow.is_not_a_date_time())
    {
        vector <CNProjectEvent>::iterator event_iter;
        for( event_iter = m_events.begin(); event_iter != m_events.end(); event_iter++ )
        {
            boost::posix_time::ptime time = event_iter->getTime();
            if( !time.is_not_a_date_time() && (time >= ptNow))
            {
                boost::posix_time::time_duration apart_time = time-ptNow;
                if((apart_time <= days))
                {
                    float hours = apart_time.hours()/24.0;
                    if( hours < 1 )
                    {
                        name_str = _T( "明天(" )+time_str +_T( " " ) + event_iter->getName()+_T( ")" );
                    }
                    else
                    {
                        char buff[16] = {0};
                        sprintf( buff, "%.1f天后", hours );
                        boost::posix_time::ptime bpp_time = event_iter->getTime();
                        string str_gettingtime = ptime2str(bpp_time);
                        time_str = s2ns(str_gettingtime);
                        name_str = s2ns( buff )+_T( "(" )+time_str +_T( " " ) + event_iter->getName()+_T( ")" );
                    }

                    return name_str;
                }
            }
        }

        CNProjectMileStone_LST::iterator mile_iter;
        for( mile_iter = m_milestones.begin(); mile_iter != m_milestones.end(); mile_iter++ )
        {
            boost::posix_time::ptime time = mile_iter->getTime();
            if( !time.is_not_a_date_time() && (time >= ptNow))
            {
                boost::posix_time::time_duration apart_time = time-ptNow;
                if(((time-ptNow) <= days))
                {
                    float hours = apart_time.hours()/24.0;
                    if( hours < 1 )
                    {
                        name_str = _T( "明天(" )+time_str +_T( " " ) + event_iter->getName()+_T( ")" );
                    }
                    else
                    {
                        char buff[16] = {0};
                        sprintf( buff, "%.1f天后", hours );
                        //time_str = s2ns( ptime2str( event_iter->getTime()));
                        boost::posix_time::ptime bpp_time = event_iter->getTime();
                        string str_gettingtime = ptime2str(bpp_time);
                        time_str = s2ns(str_gettingtime);
                        name_str = s2ns( buff )+_T( "(" )+time_str +_T( " " ) + event_iter->getName()+_T( ")" );
                    }

                    return name_str;
                }
            }
        }
    }

    return name_str;
}


int CNProjectData::getTaskCardCount( int state )
{
	CNProjectTask_LST::iterator iter;
	int count = 0;

	for ( iter = m_tasks.begin(); iter != m_tasks.end(); ++iter )
	{
		count+= iter->getTaskCardCount(state);
	}

	return count;
}

bool CNProjectData::isModify()
{
	return m_b_modify;
}

void CNProjectData::setModify(bool b)
{
	m_b_modify = b;
}

CNProjectMileStone*  CNProjectData::getNearMilestone( ptime t, BOOL b_dir)
{
	CNProjectMileStone_LST::iterator it_mile;
	CNProjectMileStone* ms = NULL;

	if (b_dir >0)
	{
		for ( it_mile = m_milestones.begin(); it_mile != m_milestones.end(); it_mile++ )
		{
			if ( it_mile->getTime().is_not_a_date_time())
			{
				continue;
			}

			if( it_mile->getTime() > t )
			{
				if (ms)
				{
					if (it_mile->getTime() < ms->getTime())
					{
						ms = &(*it_mile);
					}
				}
				else
				{
					ms = &(*it_mile);
				}
			}
		}

	}
	else
	{
		for ( it_mile = m_milestones.begin(); it_mile != m_milestones.end(); it_mile++ )
		{
			if ( it_mile->getTime().is_not_a_date_time())
			{
				continue;
			}

			if( it_mile->getTime() < t )
			{
				if (ms)
				{
					if (it_mile->getTime() > ms->getTime())
					{
						ms = &(*it_mile);
					}
				}
				else
				{
					ms = &(*it_mile);
				}
			}
		}
	}

	return ms;
}

//获取项目日历
CNProjectCalendar& CNProjectData::getCalendar()
{
    return m_prj_calendar;
}

//设置项目日历
void CNProjectData::setCalendar(const CNProjectCalendar& other)
{
    m_prj_calendar = other;
}

CNProjectWarning_LST* CNProjectData::getAllWarning()
{
    return (&m_warnings);
}

//获取某种类型的警告信息列表,默认获得task的所有信息列表,id为0则获取该类型所有的warning
CNProjectWarning_LST  CNProjectData::getWarning(warning_source source/*= WS_TaskCard*/, int id/* = 0*/ )
{
    CNProjectWarning_LST lst_warnings;

    if( m_warnings.empty() )
    {
        return lst_warnings;
    }

    for(CNProjectWarning_LST::iterator it = m_warnings.begin(); it != m_warnings.end(); it++)
    {
        if ( it->getWarningSource() != source )
        {
            continue;
        }

        if( id == 0 )
        {
            lst_warnings.push_back(*it);
        }
        else
        {
            if(it->getId() == id)
            {
                lst_warnings.push_back(*it);
            }
        }
    }

    return lst_warnings;
}
