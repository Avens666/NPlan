//  File:        NProjectExcelData.cpp
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
#ifdef WIN32   //EXCEL导入导出仅在PC版提供

#include "NProjectExcelData.h"
#include "NProjectResourcePool.h"
#include "FilePath.h"

#define RESOURCE_GROUP_ROW (7)      //模板文件中资源组所在行号
#define RESOURCE_ROW (11)           //模板文件中资源所在行号
#define DEFAULT_RESOURCE_ROWS (5)   //模板文件中默认的资源空行数
#define DEFAULT_TASK_COUNT (2)      //模板文件中默认的任务数
#define DEFAULT_TASKCARD_COUNT (3)  //模板文件中每个任务下的子任务个数
#define DEFAULT_EVENT_COUNT (3)     //模板文件中事件默认行数
#define DEFAULT_MILESTONE_COUNT (3) //模板文件中里程碑默认行数

CNProjectExcelData::CNProjectExcelData( CNProjectData* pData ) : CNProjectBaseFile( pData ), CXExcel( false ),
    m_iRow( 0 )
{
}

CNProjectExcelData::~CNProjectExcelData( void )
{
}

int CNProjectExcelData::load( kn_string &filename, nplan_fileType file_type )
{
    int _result = ERROR_FUNCTION_FAILED;

	try
	{
		switch( file_type )
		{
		case XLS_TASK_FILE:
			_result = loadTasks( filename );
			break;
		case XLS_RESOURCE_FILE:
			_result = loadResourcePool( filename );
			break;
		case  XLS_PRJ_FILE:
			_result = loadPrjData( filename );
			break;
		default:
			break;
		}
	}
	catch(...)
	{
	}

    return _result;
}

//保存至EXCEL
int CNProjectExcelData::save( kn_string &filename, kn_char* lpTemplatefile /*= NULL*/ )
{
    int _result = NO_ERROR;

	kn_string str_template_file(lpTemplatefile);
	std::replace( str_template_file.begin(), str_template_file.end(), _T( '/' ), _T( '\\' ));

    if( _taccess( str_template_file.c_str(), 0 ) != 0 ) //文件不存在
    {
        //MessageBox( NULL, _T( "模版文件不存在！" ), _T( "导出失败！" ), MB_ICONWARNING );
        return ERROR_FILE_NOT_FOUND;
    }

    size_t stIndex = filename.rfind( _T( '/' ));  // 如果传入的是'\'，已经处理成'/'
    if ( stIndex != std::string::npos )
    {
        kn_string pathname = filename.substr( 0, stIndex +1 );
        if ( !IsDirectoryExist( pathname.c_str()))
        {
            return ERROR_PATH_NOT_FOUND;
        }
    }
    else
    {
        return ERROR_PATH_NOT_FOUND;
    }

	std::replace( filename.begin(), filename.end(), _T( '/' ), _T( '\\' ));

    CopySheetFromTemplate( str_template_file.c_str(), 1, 1, 43, 10 );

    saveBaseInfo();

    saveResourcePool();

    saveTaskBoard();

    saveTasks();

    saveEvents();

    saveMilestones();

    SetColumnWidth( long(0), 1.0 );  //第1列宽度设置为1
	SetColumnWidth( long(1), 15.0 );

    SetColumnWidth( long(3), 16.0 ); //第四列宽度设置为16

    SetColumnWidth( long(6), 10.0 );  //第七列宽度设置为10

	SetColumnWidth( long(8), 16.0 );
    SetColumnWidth( long(9), 1.0 );   //最后一列宽度设置为1

	try
	{
		if ( !SaveAs((kn_char*)filename.c_str()) )
		{
			return ERROR_CREATE_FAILED;
		}
	}
#ifdef _DEBUG
	catch(_com_error & e)
	{
		_bstr_t str_msg = e.Description();
		const TCHAR* lpError = e.ErrorMessage();
		_result = ERROR_CREATE_FAILED;
	}
#else
	catch(...)
	{
		_result = ERROR_CREATE_FAILED;
	}
#endif

	return _result;
}

bool CNProjectExcelData::saveBaseInfo()
{
    _ASSERT( m_pPrjData != NULL );

    CString strFmt, strValue;

    m_iRow = 4;

    CNProjectBaseInfo& base_info = m_pPrjData->m_baseinfo;

    strFmt.Format( _T( "C%d:D%d" ), m_iRow, m_iRow );
    WriteCell( strFmt, base_info.getName().c_str());

    strFmt.Format( _T( "F%d" ), m_iRow );
    WriteCell( strFmt, base_info.getVersion().c_str());

    strFmt.Format( _T( "H%d:I%d" ), m_iRow , m_iRow);
	int pm_id = base_info.getId();
	kn_string str_pm = m_pPrjData->m_respool.getResourceName( pm_id );
	WriteCell( strFmt, str_pm.c_str() );

    strFmt.Format( _T( "C%d:I%d" ), m_iRow+1, m_iRow+1 );
    WriteCell( strFmt, base_info.getProDescribe().c_str());

    return true;
}

bool CNProjectExcelData::saveResourcePool()
{
    CString strFmt;

    m_iRow = RESOURCE_GROUP_ROW;
    vector <CNProjectResGroup>& Groups = m_pPrjData->m_respool.getResourceGroups();
    if ( !Groups.empty())
    {
        //资源组
        kn_string strMembers;
        size_t stCount = Groups.size();
        for ( size_t i = 0; i < stCount; i++ )
        {
            CNProjectResGroup pGroup = Groups.at( i );
            if ( i != 0 )
            {
                strMembers += _T( "," );
            }

            strMembers += pGroup.getName();
        }

        strFmt.Format( _T( "C%d" ), m_iRow );
        WriteCell( strFmt, (kn_char*)strMembers.c_str());
    }

    m_iRow = RESOURCE_ROW;

    //保存所有资源项
    std::vector <CNProjectResource> &Resources = m_pPrjData->m_respool.getResources();
    size_t stCount = Resources.size();
    if ( !Resources.empty())
    {
        if ( stCount > DEFAULT_RESOURCE_ROWS )
        {
            //模板中行数不够，先增加行数
            const int diff = stCount - DEFAULT_RESOURCE_ROWS;
            InsertRow( RESOURCE_ROW+1, diff );
        }
		else if(stCount == 0)
		{
			m_iRow += DEFAULT_RESOURCE_ROWS;
		}

        for ( size_t i = 0; i < stCount; i++ )
        {
            CNProjectResource pResource = Resources.at( i );

            strFmt.Format( _T( "B%d" ), m_iRow );
            WriteCell( strFmt, (kn_char*)pResource.getName().c_str());

            strFmt.Format( _T( "C%d" ), m_iRow );
            WriteCell( strFmt, (kn_char*) m_pPrjData->m_respool.typeInt2Str( pResource.getType()).c_str());

            strFmt.Format( _T( "D%d:I%d" ), m_iRow, m_iRow );

            kn_string strGroupNames;
            const vector <int>& vectGroupIds = pResource.getGroups();
            size_t stCount = vectGroupIds.size();
            for ( size_t j = 0; j < stCount; j++ )
            {
                if ( j != 0 )
                {
                    strGroupNames += _T( "," );
                }

                strGroupNames += m_pPrjData->m_respool.getResourceGroupName( vectGroupIds[j] );
            }

            WriteCell( strFmt, (kn_char*)strGroupNames.c_str());

            m_iRow++;
        }
    }

    //如果资源数小于模板中的空行，自动换行
    if ( stCount < DEFAULT_RESOURCE_ROWS )
    {
        m_iRow += (DEFAULT_RESOURCE_ROWS - stCount);
    }

    m_iRow++;     //跳过空行

    return true;
}

//任务墙保存
bool CNProjectExcelData::saveTaskBoard()
{
    kn_string str = _T( "" );
    CString strFmt;

    strFmt.Format( _T( "C%d:I%d" ), m_iRow, m_iRow );
    vector <CNProjectTaskBoardItem>& vboard = m_pPrjData->m_taskboard.getBoardItems();
    vector <CNProjectTaskBoardItem >::iterator iter;
    for ( iter = vboard.begin(); iter != vboard.end(); ++iter )
    {
        str += iter->getName()+_T( "，" );
    }

    WriteCell( strFmt, (kn_char*)str.substr( 0, str.length()-1 ).c_str());

    m_iRow++; //空行
    return true;
}

bool CNProjectExcelData::saveTasks()
{
    bool _result = false;

    m_iRow++; //任务选项卡
    m_iRow++; //"任务"标题

    int temp_row = m_iRow;

    size_t stCount = m_pPrjData->m_tasks.size();

    //模板中的任务数目小于实际数据，拷贝插入
    if( stCount > DEFAULT_TASK_COUNT )
    {
        int beg_row = temp_row;

        int diff = stCount - DEFAULT_TASK_COUNT;
        for ( int i = 0; i < diff; i++ )
        {
            int end_row = beg_row + 3 + DEFAULT_TASKCARD_COUNT;
            CopyRows( beg_row, end_row, end_row+1 );
            beg_row = end_row + 1;
        }
    }
	else if(stCount==0)
	{
		m_iRow += (DEFAULT_TASK_COUNT * DEFAULT_TASKCARD_COUNT); //空行
		m_iRow += ( (4*DEFAULT_TASK_COUNT) -1 /*第一个"任务"标题已经下移了*/ ) ; //标题及备注所占列
	}

	CNProjectTask_LST::iterator it_task;
	for ( it_task = m_pPrjData->m_tasks.begin(); it_task != m_pPrjData->m_tasks.end(); it_task++ )
    {
        CNProjectTask pTask = *it_task;
        saveTask( pTask );
    }

    m_iRow++;  //跳过空行

    return _result;
}

//保存任务信息及其所属子任务信息
bool CNProjectExcelData::saveTask( CNProjectTask & task )
{
    CString strFmt, strValue;

	strFmt.Format( _T( "B%d" ), m_iRow );

	static int task_index = 1;

	TCHAR szBuff[16]={0};
	_stprintf(szBuff, _T("任务名称【%d】"), task_index++);
	WriteCell( strFmt, szBuff);

    m_iRow++;    //跳过标题，移到正文

    //任务名称
    strFmt.Format( _T( "B%d" ), m_iRow );
    WriteCell( strFmt, (kn_char*)task.getName().c_str());

    //负责人
    strFmt.Format( _T( "C%d" ), m_iRow );
    kn_string strPm = m_pPrjData->m_respool.getResourceName( task.getLeaderId());
    WriteCell( strFmt, (kn_char*)strPm.c_str());

    //任务描述信息
    strFmt.Format( _T( "C%d:I%d" ), m_iRow+1, m_iRow+1 );
    kn_string strdiscribe = task.getTaskDescribe();
    WriteCell( strFmt, (kn_char*)strdiscribe.c_str());

    //成员
    kn_string strMembers;
    size_t stCount = task.getMembers().size();
    for ( size_t i = 0; i < stCount; i++ )
    {
        int resId = task.getMembers()[i];
        if ( i != 0 )
        {
            strMembers += _T( "," );
        }

        strMembers += m_pPrjData->m_respool.getResourceName( resId );
    }

    strFmt.Format( _T( "D%d:E%d" ), m_iRow , m_iRow);
    WriteCell( strFmt, (kn_char*)strMembers.c_str());

    //开始日期
    strFmt.Format( _T( "F%d" ), m_iRow );
    string strTime = ptime2datestr( task.getStartTime());
    WriteCell( strFmt, (kn_char*)s2ns( strTime ).c_str());

    //开始时间
    strFmt.Format( _T( "G%d" ), m_iRow );
    strTime = ptime2timeStr( task.getStartTime());
    WriteCell( strFmt, (kn_char*)s2ns( strTime ).c_str());

    //结束日期
    strFmt.Format( _T( "H%d" ), m_iRow );
    strTime = ptime2datestr( task.getEndTime());
    WriteCell( strFmt, (kn_char*)s2ns( strTime ).c_str());

    //结束时间
    strFmt.Format( _T( "I%d" ), m_iRow );
    strTime = ptime2timeStr( task.getEndTime());
    WriteCell( strFmt, (kn_char*)s2ns( strTime ).c_str());

    m_iRow += 3; //跳过子任务的标题行

    //保存其所属的所有子任务信息
    CNProjectTaskCard_LST& TaskCards = task.getTaskCards();
    size_t stChildCount = TaskCards.size();
    if ( TaskCards.size() != 0 )
    {
		bool append_empty_line = false;

        //模板中行数不够，先增加行数
        if ( stChildCount >= DEFAULT_TASKCARD_COUNT )
        {
            int diff = stChildCount - DEFAULT_TASKCARD_COUNT;
            InsertRow( m_iRow+1, diff +1);//再加上一个空行

			append_empty_line = true;
        }

		CNProjectTaskCard_LST::iterator it;
		for (it = TaskCards.begin(); it != TaskCards.end(); ++it )
        {
            saveTaskCard( (*it) );
        }

		if(append_empty_line)
		{
			m_iRow++; //再加上一个空行
		}
    }

    //子任务少于3个，自动换行
    if ( stChildCount < DEFAULT_TASKCARD_COUNT )
    {
        int diff = DEFAULT_TASKCARD_COUNT - stChildCount;
        m_iRow += diff;
    }

    return true;
}

//保存子任务
bool CNProjectExcelData::saveTaskCard( CNProjectTaskCard & TaskCard )
{
    CString strFmt, strValue;

    strFmt.Format( _T( "B%d" ), m_iRow );
    WriteCell( strFmt, (kn_char*)TaskCard.getName().c_str());

    strFmt.Format( _T( "C%d" ), m_iRow );
    kn_string pm = m_pPrjData->m_respool.getResourceName( TaskCard.getLeaderId());
    WriteCell( strFmt, (kn_char*)pm.c_str());

    strFmt.Format( _T( "D%d" ), m_iRow );
    WriteCell( strFmt, TaskCard.getPlanDays());

    strFmt.Format( _T( "E%d" ), m_iRow );
    WriteCell( strFmt, TaskCard.getPlanHours());

	strFmt.Format( _T( "F%d" ), m_iRow );
	string str_time = ptime2str( TaskCard.getStartTime() );
	WriteCell( strFmt, s2ns(str_time).c_str() );

	strFmt.Format( _T( "G%d" ), m_iRow );
	str_time = ptime2str( TaskCard.getEndTime() );
	WriteCell( strFmt, s2ns(str_time).c_str() );

	strFmt.Format( _T( "H%d" ), m_iRow );
	WriteCell( strFmt, m_pPrjData->getTaskCardStateById( TaskCard.getState()).c_str());

	strFmt.Format( _T( "I%d" ), m_iRow );
	WriteCell( strFmt, TaskCard.getDifficulty() );

    m_iRow++;

    return true;
}

bool CNProjectExcelData::saveEvents()
{
    CString strFmt, strValue;

    m_iRow += 2; //跳过标题

    size_t stCount = m_pPrjData->m_events.size();

	//模板中行数不够，先增加行数
	if ( stCount > DEFAULT_EVENT_COUNT )
	{
		int diff = stCount - DEFAULT_EVENT_COUNT;
		InsertRow( m_iRow+1, diff );
	}
	else if(stCount == 0)
	{
		m_iRow += DEFAULT_EVENT_COUNT;
	}

    for ( size_t i = 0; i < stCount; i++ )
    {
        CNProjectEvent pEvent = m_pPrjData->m_events[i];

        strFmt.Format( _T( "B%d:C%d" ), m_iRow, m_iRow );
        WriteCell( strFmt, (kn_char*)pEvent.getName().c_str());

        strFmt.Format( _T( "D%d" ), m_iRow );
        WriteCell( strFmt, s2ns( ptime2str( pEvent.getTime())).c_str(), true );

        strFmt.Format( _T( "E%d:I%d" ), m_iRow, m_iRow );
        WriteCell( strFmt, (kn_char*)pEvent.getMemo().c_str());

        m_iRow++;
    }

    m_iRow++; //跳过空行

    return (stCount > 0);
}

bool CNProjectExcelData::saveMilestones()
{
    CString strFmt, strValue;

    m_iRow += 2; //跳过标题

    size_t stCount = m_pPrjData->m_milestones.size();

	//模板中行数不够，先增加行数
	if ( stCount > DEFAULT_MILESTONE_COUNT )
	{
		int diff = stCount - DEFAULT_MILESTONE_COUNT;
		//InsertRow( m_iRow+1, diff );
	}

	//最后的表格需要手动设置边框
	CNProjectMileStone_LST::iterator it;
	for( it = m_pPrjData->m_milestones.begin(); it != m_pPrjData->m_milestones.end(); it++ )
    {
        strFmt.Format( _T( "B%d:C%d" ), m_iRow, m_iRow );
        WriteCell( strFmt, (kn_char*)it->getName().c_str());
		SetRangeBorderEx( GetRange( strFmt )  );

        strFmt.Format( _T( "D%d" ), m_iRow );
        WriteCell( strFmt, s2ns( ptime2str( it->getTime())).c_str(), true );
		SetRangeBorderEx( GetRange( strFmt )  );

        strFmt.Format( _T( "E%d:I%d" ), m_iRow, m_iRow );
        WriteCell( strFmt, (kn_char*)it->getMemo().c_str());
		SetRangeBorderEx( GetRange( strFmt )  );

        m_iRow++;
    }

    return (stCount > 0);
}

int CNProjectExcelData::loadResourcePool( kn_string &filename )
{
    int _result = ERROR_FILE_INVALID;

    CXExcel xlsReader( false );

    if ( !xlsReader.open((kn_char*)filename.c_str()) )
    {
        return ERROR_OPEN_FAILED;
    }

    CString strValue, strType, strGroups, strFmt;

    xlsReader.getData( _T( "A1" ), strValue );
    xlsReader.getData( _T( "B1" ), strType );
    xlsReader.getData( _T( "C1" ), strGroups );

    //检查模板格式
    if ( strValue.Trim().IsEmpty()
         || strType.Trim().IsEmpty()
         || strGroups.Trim().IsEmpty())
    {
        return _result;
    }

    int m_iRow( 2 );

    while( 1 )
    {
        strFmt.Format( _T( "A%d" ), m_iRow );

        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        //连续两个空行，即认为文件结束
        if( strValue.Trim().IsEmpty())
        {
            m_iRow++;

            strFmt.Format( _T( "A%d" ), m_iRow );
            xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

            if( strValue.Trim().IsEmpty())
            {
                return NO_ERROR;
            }
        }

        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strType );

        if ( strType.Trim().IsEmpty())
        {
            return _result;
        }

        strFmt.Format( _T( "C%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strGroups );
        if ( strGroups.Trim().IsEmpty())
        {
            return _result;
        }

        CNProjectResource pRes = m_pPrjData->m_respool.addResource(
            strValue,
            _tstoi( strType.c_str())

            );

        if ( pRes.getId() != 0 )
        {
            m_pPrjData->m_respool.setResourceGroupIDs( pRes, strGroups.c_str());
        }

        m_iRow++;
    }

    return _result;
}

int CNProjectExcelData::loadTasks( kn_string &filename )
{
    int _result = ERROR_FILE_INVALID;

    CXExcel xlsReader( false );

    if( !xlsReader.open((kn_char*)filename.c_str()) )
    {
        return ERROR_OPEN_FAILED;
    }

    CString strValue, start_time, end_time, strFmt;

    xlsReader.getData( _T( "A1" ), strValue );
    xlsReader.getData( _T( "B1" ), start_time );
    xlsReader.getData( _T( "C1" ), end_time );

    //检查模板格式
    if ( strValue.Trim().IsEmpty()
         || start_time.Trim().IsEmpty()
         || end_time.Trim().IsEmpty())
    {
        return _result;
    }

    int m_iRow( 2 );
    while( 1 )
    {
        strFmt.Format( _T( "A%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        //连续两个空行，即认为文件结束
        if( strValue.Trim().IsEmpty())
        {
            m_iRow++;

            strFmt.Format( _T( "A%d" ), m_iRow );
            xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

            if( strValue.Trim().IsEmpty())
            {
                return NO_ERROR;
            }
        }

        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), start_time );

        if ( start_time.Trim().IsEmpty())
        {
            return _result;
        }

        strFmt.Format( _T( "C%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), end_time );
        if ( end_time.Trim().IsEmpty())
        {
            return _result;
        }

        m_pPrjData->addTask( CNProjectTask(strValue.c_str(), 0,
            start_time.c_str(), end_time.c_str())
            );

        m_iRow++;
    }

    return _result;
}

//导入完整的项目信息
int CNProjectExcelData::loadPrjData( kn_string &filename )
{
    int _result = NO_ERROR;

    CXExcel xlsReader( false );

    if ( !xlsReader.open((kn_char*)filename.c_str()) )
    {
        return ERROR_OPEN_FAILED;
    }

    CString strName, strVersion, strValue, strFmt, strDescribe;

	//先解析资源，再解析基本信息。以便将基本信息中的PM名称转化为ID

    // 资源组
    m_iRow = RESOURCE_GROUP_ROW;
    strFmt.Format( _T( "C%d" ), m_iRow );
    xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

    vector <kn_string> vResGroups = splitString2Vect( strValue.c_str());
    for ( int i = 0; i < vResGroups.size(); i++ )
    {
        m_pPrjData->m_respool.addResGroup( CNProjectResGroup( vResGroups[i].c_str()));
    }

    // 资源
    m_iRow = RESOURCE_ROW;
    for (;; )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        if ( strValue.Trim().IsEmpty())
        {
            break;
        }

        CNProjectResource res;
        res.setName( strValue.c_str());

        strFmt.Format( _T( "C%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        res.setType( strValue.c_str());

        //所属资源组
        strFmt.Format( _T( "D%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        vector <kn_string> vResource = splitString2Vect( strValue.c_str());
        for ( int i = 0; i < vResource.size(); i++ )
        {
            int res_id = m_pPrjData->m_respool.findResourceGroupID( vResource[i].c_str());
            res.setGroup( res_id );
        }

        m_pPrjData->m_respool.addResource( res );
        ++m_iRow;
    }

    //过滤模板中的空行
    for ( int i = 0; i < DEFAULT_RESOURCE_ROWS; i++ )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        if ( !strValue.Trim().IsEmpty())
        {
            break;
        }

        m_iRow++;
    }

	int iRow_taskboard = m_iRow; //备份

	 //项目基本信息
    m_iRow = 4;

    strFmt.Format( _T( "C%d" ), m_iRow );
    xlsReader.getData((kn_char*)strFmt.c_str(), strName );

    strFmt.Format( _T( "C%d" ), m_iRow+1 );
    xlsReader.getData((kn_char*)strFmt.c_str(), strDescribe );

    strFmt.Format( _T( "F%d" ), m_iRow );
    xlsReader.getData((kn_char*)strFmt.c_str(), strVersion );

    strFmt.Format( _T( "H%d" ), m_iRow );
    xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

	int pm_id = m_pPrjData->m_respool.findResourceID( strValue.c_str() );
    m_pPrjData->setBaseInfo((kn_char*)strName.c_str(), (kn_char*)strVersion.c_str(), pm_id, (kn_char*)strDescribe.c_str());

	m_iRow = iRow_taskboard;

    //任务墙类型
    strFmt.Format( _T( "C%d" ), m_iRow );
    xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

    vector <kn_string> vTaskState = splitString2Vect( strValue.c_str());

    for ( int i = 0; i < vTaskState.size(); i++ )
    {
        CNProjectTaskBoardItem TaskState( vTaskState[i].c_str());
        m_pPrjData->m_taskboard.addBoardItem( TaskState );
    }

    m_iRow += 3; //移动到任务选项卡的下一行

    parserTask( xlsReader );

    if ( parserEvents( xlsReader ))
    {
        parserMilestones( xlsReader );
    }

    return _result;
}

//保存任务信息及其所属子任务信息
bool CNProjectExcelData::parserTask( CXExcel &xlsReader )
{
    CString strFmt, strValue, strHours;

    CNProjectTask task;

    bool has_task( true );

    do
    {
        m_iRow++;    //跳过标题，移到正文

        //任务名称
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        task.setName( strValue.c_str());

        //任务描述
        strFmt.Format( _T( "C%d" ), m_iRow+1 );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        task.setTaskDescribe( strValue.c_str());

        //负责人
        strFmt.Format( _T( "C%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        int res_id = m_pPrjData->m_respool.findResourceID( strValue.c_str());

        task.setLeaderId( res_id );

        //成员
        strFmt.Format( _T( "D%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        vector <kn_string> vMembers = splitString2Vect( strValue.c_str());
        vector <int> vMemberIds( vMembers.size());
        for ( int i = 0; i < vMembers.size(); i++ )
        {
            vMemberIds[i] = m_pPrjData->m_respool.findResourceID( vMembers[i].c_str());
        }

        task.setMembers( vMemberIds );

        //开始日期
        strFmt.Format( _T( "F%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        //开始时间
        strFmt.Format( _T( "G%d" ), m_iRow );
        xlsReader.getTimeData((kn_char*)strFmt.c_str(), strHours );

        if ( !strValue.IsEmpty() && !strHours.IsEmpty())
        {
            strValue += _T( " " );
            strValue += strHours;
        }

        task.setStartTime( strValue.c_str());

        //结束日期
        strFmt.Format( _T( "H%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        //结束时间
        strFmt.Format( _T( "I%d" ), m_iRow );
        xlsReader.getTimeData((kn_char*)strFmt.c_str(), strHours );

        if ( !strValue.IsEmpty() && !strHours.IsEmpty())
        {
            strValue += _T( " " );
            strValue += strHours;
        }

        task.setEndTime( strValue.c_str());
        m_iRow++;
        task.setId( m_pPrjData->getUnusedTaskId());
        has_task = parserTaskCard( xlsReader, task );
        m_pPrjData->addTask( task );

        checkWarning(task);

        task.getTaskCards().clear();

    } while ( has_task );

    return true;
}

//保存子任务
bool CNProjectExcelData::parserTaskCard( CXExcel &xlsReader, CNProjectTask& task )
{
    CString strFmt, strValue;

    CNProjectTaskCard taskCard;

    m_iRow += 2;

    kn_string str_task_name = getStringManager()->GetStringByID(_T("task_resource_name"));
    boost::algorithm::trim( str_task_name );

    for (;; )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue ); //子任务名称
        if ( strValue.Trim().IsEmpty())
        {
            break;
        }

        if ( 0 == strValue.Trim().Compare( str_task_name.c_str() ))  //发现一组新任务(上一个任务无空行)
        {
            return true;
        }

        taskCard.setId( 0 ); //auto calc
        taskCard.setName( strValue.c_str());

        strFmt.Format( _T( "C%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue ); //负责人
        int res_id = m_pPrjData->m_respool.findResourceID( strValue.c_str());
        taskCard.setLeaderId( res_id );

        strFmt.Format( _T( "D%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );  //计划天数
        taskCard.setPlanDays( _ttoi( strValue.c_str()));

        strFmt.Format( _T( "E%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );  //计划小时数
        taskCard.setPlanHours( _ttoi( strValue.c_str()));

		strFmt.Format( _T( "F%d" ), m_iRow );
		xlsReader.getData((kn_char*)strFmt.c_str(), strValue );  //开始时间
		taskCard.setStartTime( strValue.c_str() );

		strFmt.Format( _T( "G%d" ), m_iRow );
		xlsReader.getData((kn_char*)strFmt.c_str(), strValue );  //结束时间
		taskCard.setEndTime( strValue.c_str() );

		strFmt.Format( _T( "H%d" ), m_iRow );
		xlsReader.getData((kn_char*)strFmt.c_str(), strValue );    //状态
        taskCard.setState( m_pPrjData->m_taskboard.findIdByName( strValue.c_str()));

		strFmt.Format( _T( "I%d" ), m_iRow );
		xlsReader.getData((kn_char*)strFmt.c_str(), strValue );    //任务困难度
		if( !strValue.empty() )
		{
			taskCard.setDifficulty( _ttoi(strValue.c_str()) );
		}

        task.addTaskCard( taskCard );
        m_iRow++;
    }

    //查找下一个不为空的对象，所在的行号（过滤一个或多个空行）
    bool find_next_obj( false );

    kn_string str_event = getStringManager()->GetStringByID(_T("event"));
    boost::algorithm::trim( str_event );

    for ( int i = 0; i <= DEFAULT_TASKCARD_COUNT; i++ )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        if ( -1 != strValue.Trim().Find( str_task_name.c_str() ))
        {
            find_next_obj = true;
            break;
        }
        else if ( -1 != strValue.Trim().Find( str_event.c_str() )) //最后一组任务
        {
            return false;
        }

        m_iRow++;
    }

    return find_next_obj;
}

//保存事件
bool CNProjectExcelData::parserEvents( CXExcel &xlsReader )
{
    CString strFmt, strValue;

    m_iRow += 2; //跳过标题

    for ( int i = 0;; i++ )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        if ( strValue.Trim().IsEmpty())
        {
            m_iRow++; //跳过空行
            break;
        }

        CNProjectEvent event;
        event.setName( strValue.c_str());

        strFmt.Format( _T( "D%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        event.setTime( strValue );

        strFmt.Format( _T( "E%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        event.setMemo( strValue.c_str());
        event.setId( 0 );
        m_pPrjData->addEvent( event );

        m_iRow++;
    }

    //查找下一个不为空的对象，所在的行号（过滤一个或多个空行）
    bool find_next_obj( false );

    for ( int i = 0; i <= DEFAULT_EVENT_COUNT; i++ )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        if ( !strValue.Trim().IsEmpty())
        {
            find_next_obj = true;
            break;
        }

        m_iRow++;
    }

    return (find_next_obj);
}

//保存里程碑
bool CNProjectExcelData::parserMilestones( CXExcel &xlsReader )
{
    CString strFmt, strValue;

    m_iRow += 2; //跳过标题

    for (;; )
    {
        strFmt.Format( _T( "B%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        if ( strValue.Trim().IsEmpty())
        {
            m_iRow++; //跳过空行
            break;
        }

        CNProjectMileStone milestone;
        milestone.setName( strValue.c_str());

        strFmt.Format( _T( "D%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );
        milestone.setTime( strValue );

        strFmt.Format( _T( "E%d" ), m_iRow );
        xlsReader.getData((kn_char*)strFmt.c_str(), strValue );

        milestone.setMemo( strValue.c_str());
        milestone.setId( 0 );
        m_pPrjData->addMilestone( milestone );

        m_iRow++;
    }

    return true;
}

#endif
