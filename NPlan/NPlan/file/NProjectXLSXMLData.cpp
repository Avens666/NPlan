//  File:        NProjectXLSXMLData.cpp
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
#include "NProjectXLSXMLData.h"
#include <stdio.h>

#define DATA_COLS 8   //数据列总共有8列
#define STYLE_DATE_ONLY "s1"                   //日期格式(只有日期)
#define STYLE_TIME_ONLY "s2"                   //时间格式(只有时间)
#define STYLE_DATE_TIME "s3"                   //时间和日期样式
#define STYLE_EMPTY_LINE "s4"                  //空行样式
#define STYLE_TITLE_1 "s5"                     //标题样式(绿色)
#define STYLE_TITLE_2 "s6"                     //标题样式(黄色)

MyTiXmlDeclaration::MyTiXmlDeclaration( const char *_progid)
{
    progid = _progid;
}

void MyTiXmlDeclaration::Print(FILE *cfile, int /*depth*/, TiXmlString *str) const
{
	if ( cfile )
		fprintf( cfile, "<?mso-application " );
	if ( str )	 (*str) += "<?mso-application ";
	if ( !progid.empty() ) {
		if ( cfile ) fprintf (cfile, "progid=\"%s\" ", progid.c_str ());
		if ( str ) { (*str) += "progid=\""; (*str) += progid; (*str) += "\" "; }
	}
	if ( cfile ) fprintf( cfile, "?>" );
	if ( str )	 (*str) += "?>";
}

CNProjectXLSXMLData::CNProjectXLSXMLData ( CNProjectData* pData ) : CNProjectBaseFile( pData )
{
    m_pXmldoc     = NULL;
    m_p_root      = NULL;
    m_pRowElement = NULL;
    m_pTable      = NULL;
    m_filetype    = UNKNOWN_Type;
}

CNProjectXLSXMLData::~CNProjectXLSXMLData( void )
{
    if ( m_pXmldoc )
    {
        delete m_pXmldoc;
    }
}

//加载xml文件
int CNProjectXLSXMLData::load( kn_string &filename )
{
	m_strPM = _T("");

    int _result = NO_ERROR;

    string file = ns2s( filename );

    if ( file.length() == 0 )
    {
        return ERROR_INVALID_PARAMETER;
    }

    m_pXmldoc = new TiXmlDocument( file.c_str());
    if( !m_pXmldoc )
    {
        return ERROR_FILE_NOT_FOUND;
    }
    if ( !m_pXmldoc->LoadFile())
    {
        return ERROR_FILE_INVALID;
    }

    if( m_pXmldoc )
    {
        m_p_root = m_pXmldoc->RootElement();
    }

    //不支持的文件类型
    if( !isSupportedType())
    {
        return ERROR_FILE_INVALID;
    }

    if( m_pPrjData != NULL)
    {
        _result = parsePrjBaseInfo();
        if( NO_ERROR != _result )
        {
            return ERROR_FILE_INVALID;
        }

        parseResourceGroup();

        parseResource();

		//PM也是资源，应保存资源ID，而不是名称
		int pm_id = m_pPrjData->m_respool.findResourceID(m_strPM.c_str() );
		m_pPrjData->m_baseinfo.setId(pm_id);

        parseTaskBoard();

        parseTasks();

        parseMilestoneEvent( true );

        parseMilestoneEvent( false );
    }

    return _result;
}

//保存xml文件
int CNProjectXLSXMLData::save( kn_string &filename )
{
    if ( filename.length() == 0 )
    {
        return ERROR_INVALID_PARAMETER;
    }

    string file = ns2s( filename );

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

    TiXmlDocument doc;

    TiXmlDeclaration * decl     = new TiXmlDeclaration( "1.0", "", "" );
    MyTiXmlDeclaration * mydecl = new MyTiXmlDeclaration( "Excel.Sheet" );

    //保存根节点及旗下几个excel特有的节点。
    m_p_root = new TiXmlElement( "Workbook" );
    if( m_p_root == NULL )
    {
        return ERROR_FUNCTION_FAILED;
    }

    //保存特有属性
    SetAttributeStr( m_p_root, "xmlns", "urn:schemas-microsoft-com:office:spreadsheet" );
    SetAttributeStr( m_p_root, "xmlns:o", "urn:schemas-microsoft-com:office:office" );
    SetAttributeStr( m_p_root, "xmlns:x", "urn:schemas-microsoft-com:office:excel" );
    SetAttributeStr( m_p_root, "xmlns:ss", "urn:schemas-microsoft-com:office:spreadsheet" );
    SetAttributeStr( m_p_root, "xmlns:html", "http://www.w3.org/TR/REC-html40" );

    //保存文档属性
	TiXmlElement *p_elem  = new TiXmlElement( "DocumentProperties" );
	if( p_elem )
	{
		SetAttributeStr( p_elem, "xmlns", "urn:schemas-microsoft-com:office:office" );
		ptime now( second_clock::local_time() );
		string str_time = ptime2str(now, true);
		add_node( p_elem, "Created", str_time.c_str() );
		add_node( p_elem, "LastSaved", str_time.c_str() );
		add_node( p_elem, "Version", "12.00" );
		m_p_root->LinkEndChild( p_elem );
	}

    //保存Office文档设置
	p_elem = add_node(m_p_root, "OfficeDocumentSettings", NULL);
	if( p_elem )
	{
		SetAttributeStr( p_elem, "xmlns", "urn:schemas-microsoft-com:office:office" );
	}

	p_elem = add_node(NULL, "ExcelWorkbook", NULL);
	if( p_elem )
	{
		SetAttributeStr( p_elem, "xmlns", "urn:schemas-microsoft-com:office:excel" );
		m_p_root->LinkEndChild( p_elem );
	}

    TiXmlElement* p_sheet = new TiXmlElement( "Worksheet" );
    SetAttributeStr( p_sheet, "ss:Name", "NPlan" );

    m_pTable = new TiXmlElement( "Table" );
    if( m_pTable )
    {
        saveStyles();

        saveBaseInfo();

        saveResourceGroup();

        saveResource();

        saveTaskBoard();

        saveTasks();

        saveEvents();

        saveMilestones();

        p_sheet->LinkEndChild( m_pTable );

        m_p_root->LinkEndChild( p_sheet );

        doc.LinkEndChild( decl );
        doc.LinkEndChild( mydecl );
        doc.LinkEndChild( m_p_root );

        if ( !doc.SaveFile( file.c_str()))
        {
            return ERROR_CREATE_FAILED;
        }
    }

    return NO_ERROR;
}

//查询该文件是否是本类支持解析的xml文件
bool CNProjectXLSXMLData::isSupportedType()
{
    bool _result = false;

    if( !m_p_root )
    {
        return _result;
    }

    if( NULL != m_p_root->FirstChildElement( "OfficeDocumentSettings" ))
    {
        m_filetype = XLS_XML_Office;
        _result    = true;
    }
    else
    {
        TiXmlElement * p_elem = m_p_root->FirstChildElement( "CustomDocumentProperties" );

        if( p_elem && p_elem->FirstChildElement( "KSOProductBuildVer" ))
        {
            m_filetype = XLS_XML_WPS;
            _result    = true;
        }
    }

    return _result;
}

//获取一行的所有列的内容
bool CNProjectXLSXMLData::getRowValue( TiXmlElement* row, vector <kn_string>& v_cols )
{
    bool _result = false;

    v_cols.clear();

    if( row != NULL )
    {
        TiXmlElement* cell = row->FirstChildElement( "Cell" );

        bool first_col( true );

        for(; cell != NULL; cell = cell->NextSiblingElement())
        {
            _result = true;

            //Data或ss:Data
            TiXmlElement* data_elem = cell->FirstChildElement();
            if( data_elem )
            {
                kn_string value = getXmlElementValue( data_elem );
                boost::algorithm::trim( value ); //去首尾空格

               TiXmlNode *child = data_elem->FirstChild();
               if(child) child = data_elem->IterateChildren(child);
               while(child)
               {
                   //TIXML_STRING a =child->ValueTStr();
                   int type = child->Type();
                   if (type == TiXmlNode::ELEMENT)
                   {
                       value +=  getXmlElementValue( child->ToElement() );
                   }
                   else if (type == TiXmlNode::TEXT)
                   {
                       const char* text = child->ToText()->Value();
                       if(text != NULL)
                       {
                           value += UTF8ToUnicode(text);
                       }

                       //value += s2ns(text);
                   }

                   child = data_elem->IterateChildren(child);
               }

                v_cols.push_back( value );
            }
            else
            {
                //第一列肯定是空列(模板定义的)，非数据项
                if( !first_col )
                {
                    v_cols.push_back( _T( "" ));     //有些数据项，没填值时，是没有DATA节点的
                }
            }

            first_col = false;
        }
    }

    return ( _result && !v_cols.empty() );
}

//解析项目基本信息
int CNProjectXLSXMLData::parsePrjBaseInfo()
{
    int _result = ERROR_FILE_INVALID;

    if( !m_p_root )
    {
        return _result;
    }

    m_pRowElement = NULL;
    TiXmlElement* p_sheet = m_p_root->FirstChildElement( "Worksheet" );
    if( p_sheet )
    {
        TiXmlElement* pTable = p_sheet->FirstChildElement( "Table" );
        if( pTable )
        {
            m_pRowElement = pTable->FirstChildElement( "Row" );
        }
    }

    if( !m_pRowElement )
    {
        return _result;
    }

    kn_string str_project_name = getStringManager()->GetStringByID(_T("project_name"));
    boost::algorithm::trim( str_project_name );

    vector <kn_string> v_cols;

    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if ( v_cols.front() == str_project_name )
            {
                if( v_cols.size() < 6 )
                {
                    return _result;
                }

                m_pPrjData->setName( v_cols[1].c_str());
                m_pPrjData->setVersion( v_cols[3].c_str());
				m_strPM = v_cols[5];
				//m_pPrjData->m_baseinfo.setLeader( v_cols[5].c_str());
                _result = NO_ERROR;
                break;
            }
        }
    }

    if( NO_ERROR == _result ) //项目描述
    {
        kn_string str_project_desc = getStringManager()->GetStringByID(_T("project_desc"));
        boost::algorithm::trim( str_project_desc );

        m_pRowElement = m_pRowElement->NextSiblingElement();
        if(m_pRowElement && getRowValue( m_pRowElement, v_cols ))
        {
            if((v_cols.size() >= 2) && (v_cols[0] == str_project_desc))
            {
                m_pPrjData->m_baseinfo.setProDescribe( v_cols[1].c_str());
            }

            m_pRowElement = m_pRowElement->NextSiblingElement();
        }
    }

    return _result;
}

//解析资源组
bool CNProjectXLSXMLData::parseResourceGroup()
{
    bool _result = false;

    vector <kn_string> v_cols;

    kn_string str_resource_group = getStringManager()->GetStringByID(_T("resource_group"));
    boost::algorithm::trim( str_resource_group );

    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if((v_cols.size() >= 2) && (v_cols[0] == str_resource_group))
            {
                vector <kn_string> v_res_gp = splitString2Vect( v_cols[1].c_str());
                for( vector <kn_string>::iterator it = v_res_gp.begin(); it != v_res_gp.end(); ++it )
                {
                    if( it->length() > 0 )
                    {
                        CNProjectResGroup res_group( it->c_str());
                        m_pPrjData->m_respool.addResGroup( res_group );
                    }
                }

                m_pRowElement = m_pRowElement->NextSiblingElement();
                _result = true;
                break;
            }
        }
    }

    return _result;
}

//解析资源
bool CNProjectXLSXMLData::parseResource()
{
    bool _result = false;

    kn_string str_resource_item = getStringManager()->GetStringByID(_T("resource_item"));
    boost::algorithm::trim( str_resource_item );

    vector <kn_string> v_cols;
    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if ( v_cols.end() != find( v_cols.begin(), v_cols.end(), str_resource_item ))
            {
                m_pRowElement = m_pRowElement->NextSiblingElement(); //跳过标题
                if(m_pRowElement)
                {
                    m_pRowElement = m_pRowElement->NextSiblingElement(); //跳过标题
                }
                _result       = true;
                break;
            }
        }
    }

    if( !_result )
    {
        return _result;
    }

    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            const int iLen = v_cols.size();
            if ( iLen >= 3 )
            {
                if( !v_cols[0].empty())
                {
                    CNProjectResource res;
                    res.setName( v_cols[0].c_str());
                    res.setType( v_cols[1].c_str());

                    vector <kn_string> vResource = splitString2Vect( v_cols[2].c_str());
                    for ( size_t i = 0; i < vResource.size(); i++ )
                    {
                        int res_id = m_pPrjData->m_respool.findResourceGroupID( vResource[i].c_str());
                        res.setGroup( res_id );
                    }

                    m_pPrjData->m_respool.addResource( res );
                }
                else if( iLen ==  count(v_cols.begin(), v_cols.end(), _T("")) ) //TODO
                {
                    break; //空行结束
                }
            }
            else
            {
                break; //非资源行
            }
        }
        else
        {
            break; //空行结束
        }
    }

    return _result;
}

//解析任务墙类型
bool CNProjectXLSXMLData::parseTaskBoard()
{
    bool _result = false;

    vector <kn_string> v_cols;

     kn_string str_task_borad_type = getStringManager()->GetStringByID(_T("task_borad_type"));
    boost::algorithm::trim( str_task_borad_type );

    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if((v_cols.size() >= 2) && (v_cols[0] == str_task_borad_type))
            {
                vector <kn_string> vTaskState = splitString2Vect( v_cols[1].c_str());
                for ( size_t i = 0; i < vTaskState.size(); i++ )
                {
                    CNProjectTaskBoardItem TaskState( vTaskState[i].c_str());
                    m_pPrjData->m_taskboard.addBoardItem( TaskState );
                }

                m_pRowElement = m_pRowElement->NextSiblingElement();
                _result       = true;
                break;
            }
        }
    }

    return _result;
}

ptime CNProjectXLSXMLData::parserDataTime( kn_string &date, kn_string &time )
{
    ptime task_time;

    if( !date.empty())
    {
        if(m_filetype == XLS_XML_WPS)
        {
            return excelTime2ptime(_ttoi(date.c_str()), _tstof(time.c_str()));
        }
        else //XLS_XML_Office
        {
            //只取日期部分
            size_t pos = date.find(_T("T"));
            if( pos != kn_string::npos )
            {
                date = date.substr(0,pos); //1899-12-31T18:00:00 过滤掉无效日期
            }

            if( time.empty())  //只有日期
            {
                return timestr2ptime( date );
            }
            else
            {   //只取时间部分
                size_t pos = time.find(_T("T"));
                if( pos != kn_string::npos )
                {
                    time = time.substr(pos+1); //1899-12-31T18:00:00 过滤掉无效日期
                }

                kn_string str= date + _T( " " ) + time;
                return timestr2ptime(str);
            }
        }
    }

    return task_time;
}

bool CNProjectXLSXMLData::parserTask(CNProjectTask &task)
{
    if( m_pRowElement == NULL )
    {
        return false;
    }

    vector <kn_string> v_cols;
    if( getRowValue( m_pRowElement, v_cols ))//任务名称
    {
		if(v_cols.size() == 1 && v_cols[0].empty() )
		{
			 m_pRowElement = m_pRowElement->NextSiblingElement();
			 getRowValue( m_pRowElement, v_cols ); //过滤空行
		}

        kn_string str_task_name = getStringManager()->GetStringByID(_T("task_resource_name"));
        boost::algorithm::trim( str_task_name );

        if( !v_cols.empty() && v_cols[0].find( str_task_name ) == kn_string::npos )
        {
            return false;  //格式错误
        }
    }
    else
    {   //一个CELL节点没有，或者只有一个空的CELL节点，这种情况一般为WPS保存的空行
        return false;
    }

    m_pRowElement = m_pRowElement->NextSiblingElement();
    if( !getRowValue( m_pRowElement, v_cols ) )
    {
        return false;   //格式错误
    }

    //任务信息
    if( v_cols.size() >=7 )
    {
        task.setId( m_pPrjData->getUnusedTaskId() );
        task.setName( v_cols[0].c_str() );

        int res_id = m_pPrjData->m_respool.findResourceID( v_cols[1].c_str());
        task.setLeaderId( res_id );

        //成员
        vector <kn_string> vMembers = splitString2Vect( v_cols[2].c_str());
        vector <int> vMemberIds( vMembers.size());
        for ( size_t i = 0; i < vMembers.size(); i++ )
        {
            vMemberIds[i] = m_pPrjData->m_respool.findResourceID( vMembers[i].c_str());
        }

        task.setMembers( vMemberIds );

        task.setStartTime( parserDataTime( v_cols[3], v_cols[4] ));
        task.setEndTime( parserDataTime( v_cols[5], v_cols[6] ));
    }

    m_pRowElement = m_pRowElement->NextSiblingElement();
    if( !getRowValue( m_pRowElement, v_cols ) )
    {
        return false;
    }

    kn_string str_task_desc = getStringManager()->GetStringByID(_T("task_desc"));
    boost::algorithm::trim( str_task_desc );

    if(v_cols[0] == str_task_desc )
    {
        task.setTaskDescribe( v_cols[1].c_str() );
        m_pRowElement = m_pRowElement->NextSiblingElement();
    }

    return true;
}

//解析任务列表
bool CNProjectXLSXMLData::parseTasks()
{
    bool _result = false;

    kn_string str_task = getStringManager()->GetStringByID(_T("task"));
    boost::algorithm::trim( str_task );

    vector <kn_string> v_cols;
    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if ( v_cols.end() != find( v_cols.begin(), v_cols.end(), str_task ))
            {
                m_pRowElement = m_pRowElement->NextSiblingElement(); //跳过标题
                _result       = true;
                break;
            }
        }
    }

    CNProjectTask task;
    CNProjectTaskCard taskcard;

    kn_string str_task_name = getStringManager()->GetStringByID(_T("task_resource_name"));
    boost::algorithm::trim( str_task_name );

    kn_string str_action_name = getStringManager()->GetStringByID(_T("action_name"));
    boost::algorithm::trim( str_action_name );

    while( parserTask( task ))
    {
        if( getRowValue( m_pRowElement, v_cols ) &&  v_cols[0] == str_action_name)
        {
            m_pRowElement = m_pRowElement->NextSiblingElement();

            while( getRowValue( m_pRowElement, v_cols )
                && (v_cols.size() >=7)
                &&(v_cols[0].find( str_task_name ) == kn_string::npos ) )
            {
                taskcard.setId( 0 ); //auto calc
                taskcard.setName( v_cols[0].c_str());

                int res_id = m_pPrjData->m_respool.findResourceID( v_cols[1].c_str());
                taskcard.setLeaderId( res_id );

                taskcard.setPlanDays( _ttoi( v_cols[2].c_str()));
                taskcard.setPlanHours( _ttoi( v_cols[3].c_str()));
				taskcard.setStartTime(  v_cols[4].c_str() );
				taskcard.setEndTime( v_cols[5].c_str()  );

                taskcard.setState( m_pPrjData->m_taskboard.findIdByName( v_cols[6].c_str()));

                task.addTaskCard( taskcard );
                m_pRowElement = m_pRowElement->NextSiblingElement();
            }

            m_pPrjData->addTask(task);
            checkWarning(task);
        }
        //else 空行或线束

        task.clearTaskCards();
    }

    return _result;
}

bool CNProjectXLSXMLData::addEvent( const vector <kn_string> &v_cols )
{
    kn_string str_mile_stone = getStringManager()->GetStringByID(_T("mile_stone"));
    boost::algorithm::trim( str_mile_stone );

    if( v_cols.size() < 3 || (v_cols[0] == str_mile_stone) )
    {
        return false;
    }

    if( !v_cols[0].empty())
    {
        CNProjectEvent event;
        event.setName( v_cols[0].c_str());
        event.setId( 0 );
        event.setMemo( v_cols[2].c_str());

        kn_string timestr = v_cols[1];
        if( m_filetype == XLS_XML_WPS )
        {
            if( !timestr.empty())
            {
                event.setTime( excelTime2ptime( _ttoi( timestr.c_str()), 0 ));
            }
        }
        else
        {
            event.setTime( timestr );
        }

        m_pPrjData->addEvent( event );
    }

    return true;
}

bool CNProjectXLSXMLData::addMileStone( const vector <kn_string> &v_cols )
{
    if( v_cols.size() < 3 )
    {
        return false;
    }

    if( !v_cols[0].empty())
    {
        CNProjectMileStone milestone;
        milestone.setName( v_cols[0].c_str());
        milestone.setId( 0 );
        milestone.setMemo( v_cols[2].c_str());

        kn_string timestr = v_cols[1];
        if( m_filetype == XLS_XML_WPS )
        {
            if( !timestr.empty())
            {
                milestone.setTime( excelTime2ptime( _ttoi( timestr.c_str()), 0 ));
            }
        }
        else
        {
            milestone.setTime( timestr );
        }

        m_pPrjData->addMilestone( milestone );
    }

    return true;
}

//解析事件列表
bool CNProjectXLSXMLData::parseMilestoneEvent( bool is_event )
{
    bool _result = false;

    kn_string str_mile_stone = getStringManager()->GetStringByID(_T("mile_stone"));
    boost::algorithm::trim( str_mile_stone );

    kn_string str_event = getStringManager()->GetStringByID(_T("event"));
    boost::algorithm::trim( str_event );

    const kn_string str_key = is_event ? str_event : str_mile_stone;

    TiXmlElement * p_row_elem = NULL;

    vector <kn_string> v_cols;
    for (; m_pRowElement != NULL; m_pRowElement = m_pRowElement->NextSiblingElement())
    {
        if( getRowValue( m_pRowElement, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            if( str_key == v_cols.front())
            {
                p_row_elem = m_pRowElement->NextSiblingElement(); //跳过标题
                if(p_row_elem)
                {
                    p_row_elem = p_row_elem->NextSiblingElement(); //跳过标题
                }
                _result = true;
                break;
            }
        }
    }

    bool is_good;

    for (; p_row_elem != NULL; p_row_elem = p_row_elem->NextSiblingElement())
    {
        if( getRowValue( p_row_elem, v_cols )) //非数据行，Cell节点下没有Data节点
        {
            is_event ? is_good = addEvent( v_cols ) : is_good = addMileStone( v_cols );
            if( !is_good )
            {
                break; //非当前数据块
            }
        }
        else
        {
            break; //空行
        }
    }

    m_pRowElement = p_row_elem;
    return _result;
}

//保存项目基本信息
bool CNProjectXLSXMLData::saveBaseInfo()
{
    if( !m_pTable )
    {
        return false;
    }

    addRow( m_pTable, NULL );
    addRow( m_pTable, newTiXmlText( L"NPlan 项目模板" ), DATA_COLS-1 );
    addRow( m_pTable, NULL );

    m_pRowElement = addRow( NULL, newTiXmlText( L"项目名称" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( m_pPrjData->getName()), 1 );
    addCell( m_pRowElement, newTiXmlText( L"版本号" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( m_pPrjData->getVersion()));
    addCell( m_pRowElement, newTiXmlText( L"项目经理" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( m_pPrjData->getLeader()), 1);
    m_pTable->LinkEndChild( m_pRowElement );

    m_pRowElement = addRow( NULL, newTiXmlText( L"项目描述" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( m_pPrjData->getProDescribe()), DATA_COLS-2 );
    m_pTable->LinkEndChild( m_pRowElement );

    addRow( m_pTable, NULL ); //空行

    return true;
}

//保存资源组
bool CNProjectXLSXMLData::saveResourceGroup()
{
    if( !m_pTable )
    {
        return false;
    }

    m_pRowElement = addRow( NULL, newTiXmlText( L"资源组" ));

    vector <CNProjectResGroup>& Groups = m_pPrjData->m_respool.getResourceGroups();
    if ( !Groups.empty())
    {
        //资源组
        kn_string strMembers;
        size_t stCount = Groups.size();
        for ( size_t i = 0; i < stCount; i++ )
        {
            CNProjectResGroup& pGroup = Groups.at( i );
            if ( i != 0 )
            {
                strMembers += _T( "," );
            }

            strMembers += pGroup.getName();
        }

        addCell( m_pRowElement, newTiXmlText( strMembers ), DATA_COLS-2 );
    }

    m_pTable->LinkEndChild( m_pRowElement );

    m_pRowElement = addRow( m_pTable, NULL );

    return true;
}

//保存资源
bool CNProjectXLSXMLData::saveResource()
{
    if( !m_pTable )
    {
        return false;
    }

    addRow( m_pTable, newTiXmlText( L"资源项" ), DATA_COLS-1, STYLE_TITLE_1 );

    m_pRowElement = addRow( NULL, newTiXmlText( L"名称" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"类型" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"所属资源组" ), 5, STYLE_TITLE_1 );
    m_pTable->LinkEndChild( m_pRowElement );

    //数据行
    std::vector <CNProjectResource> &Resources = m_pPrjData->m_respool.getResources();
    size_t stCount                             = Resources.size();
    for ( size_t i = 0; i < stCount; i++ )
    {
        CNProjectResource& res = Resources.at( i );

        m_pRowElement = addRow( NULL, newTiXmlText( res.getName()));
        addCell( m_pRowElement, newTiXmlText( res.resTypeConvertStr( res.getType())));

        //所属资源组，拼接
        kn_string strGroupNames;
        const vector <int>& vectGroupIds = res.getGroups();
        size_t stCount                   = vectGroupIds.size();
        for ( size_t j = 0; j < stCount; j++ )
        {
            if ( j != 0 )
            {
                strGroupNames += _T( "," );
            }

            strGroupNames += m_pPrjData->m_respool.getResourceGroupName( vectGroupIds[j] );
        }

        addCell( m_pRowElement, newTiXmlText( strGroupNames ), 5 );

        m_pTable->LinkEndChild( m_pRowElement );
    }

    addRow( m_pTable, NULL );

    return true;
}

//保存任务墙
bool CNProjectXLSXMLData::saveTaskBoard()
{
    if( !m_pTable )
    {
        return false;
    }

    m_pRowElement = addRow( NULL, newTiXmlText( L"任务墙类型" ), 0, STYLE_TITLE_1 );

    vector <CNProjectTaskBoardItem>& vboard = m_pPrjData->m_taskboard.getBoardItems();
    if ( !vboard.empty())
    {
        //资源组
        kn_string str;
        vector <CNProjectTaskBoardItem >::iterator iter;
        for ( iter = vboard.begin(); iter != vboard.end(); ++iter )
        {
            str += iter->getName()+_T( "," );
        }

        addCell( m_pRowElement, newTiXmlText( str.substr( 0, str.length()-1 )), DATA_COLS-2 );
    }

    m_pTable->LinkEndChild( m_pRowElement );

    m_pRowElement = addRow( m_pTable, NULL );

    return true;
}

void CNProjectXLSXMLData::writeDataTime(string& str_data_time)
{
	if( str_data_time.empty() )
	{
		addCell(m_pRowElement, NULL, 0);
	}
	else
	{
		if( string::npos != str_data_time.find("00:00:00") )
		{
			addCell( m_pRowElement, newTiXmlText( str_data_time.c_str() ), 0, STYLE_DATE_ONLY , "DateTime");
		}
		else
		{
			addCell( m_pRowElement, newTiXmlText( str_data_time.c_str() ), 0, STYLE_DATE_TIME , "DateTime");
		}
	}
}

//保存任务
bool CNProjectXLSXMLData::saveTasks()
{
    if( !m_pTable )
    {
        return false;
    }

    addRow( m_pTable, newTiXmlText( L"任务" ), DATA_COLS-1, STYLE_TITLE_1 );

	int task_index(0);

    //保存task
    CNProjectTask_LST::iterator it_task;
    for ( it_task = m_pPrjData->m_tasks.begin(); it_task != m_pPrjData->m_tasks.end(); it_task++ )
    {
        CNProjectTask &task = *it_task;

		task_index++;

		wchar_t szBuff[16]={0};
		//swprintf(szBuff, L"任务名称【%d】", task_index);
        
        //  swprintf is not avaliable for ios
        kn_string strTitle = _T("任务名称【");
        _itow(task_index,szBuff, 10);
        strTitle += szBuff;
        strTitle += _T("】");
        
        

        m_pRowElement = addRow( NULL, newTiXmlText( strTitle.c_str() ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"负责人" ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"成员" ), 1, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"开始日期" ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"开始时间" ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"结束日期" ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( L"结束时间" ), 0, STYLE_TITLE_1 );
        m_pTable->LinkEndChild( m_pRowElement );

        //任务信息行
        m_pRowElement = addRow( NULL, newTiXmlText( task.getName()));
        addCell( m_pRowElement, newTiXmlText( task.getLeaderName()));
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

        addCell( m_pRowElement, newTiXmlText( strMembers ), 1);

        addCell( m_pRowElement, newTiXmlText( ptime2str(task.getStartTime(), true).c_str()),
            0, STYLE_DATE_ONLY , "DateTime");
        addCell( m_pRowElement, newTiXmlText( ptime2str( task.getStartTime(), true).c_str()),
            0, STYLE_TIME_ONLY , "DateTime");
        addCell( m_pRowElement, newTiXmlText( ptime2str(task.getEndTime(), true).c_str()),
            0, STYLE_DATE_ONLY , "DateTime");
        addCell( m_pRowElement, newTiXmlText( ptime2str(task.getEndTime(), true).c_str()),
            0, STYLE_TIME_ONLY, "DateTime" );
        m_pTable->LinkEndChild( m_pRowElement );

        m_pRowElement = addRow( NULL, newTiXmlText( L"任务描述" ), 0, STYLE_TITLE_1 );
        addCell( m_pRowElement, newTiXmlText( task.getTaskDescribe()), DATA_COLS-2 );
        m_pTable->LinkEndChild( m_pRowElement );

        //子任务名称行
        m_pRowElement = addRow( NULL, newTiXmlText( L"子任务名称" ), 0, STYLE_TITLE_2 );
        addCell( m_pRowElement, newTiXmlText( L"负责人" ), 0, STYLE_TITLE_2 );
        addCell( m_pRowElement, newTiXmlText( L"计划天数" ), 0, STYLE_TITLE_2 );
        addCell( m_pRowElement, newTiXmlText( L"计划小时数" ), 0, STYLE_TITLE_2 );
		addCell( m_pRowElement, newTiXmlText( L"开始时间" ), 0, STYLE_TITLE_2 );
		addCell( m_pRowElement, newTiXmlText( L"结束时间" ), 0, STYLE_TITLE_2 );
        addCell( m_pRowElement, newTiXmlText( L"状态" ), 0, STYLE_TITLE_2 );
		addCell( m_pRowElement, newTiXmlText( L"任务困难度" ), 0, STYLE_TITLE_2 );
        m_pTable->LinkEndChild( m_pRowElement );

		ostringstream oss;

        //循环子任务数据行
        CNProjectTaskCard_LST& cards = task.getTaskCards();
        for ( CNProjectTaskCard_LST::iterator it = cards.begin(); it != cards.end(); ++it )
        {
            //保存一个taskcard
			m_pRowElement = addRow( NULL, newTiXmlText( it->getName()));
			kn_string str_value = m_pPrjData->m_respool.getResourceName( it->getLeaderId());
			addCell( m_pRowElement, newTiXmlText( str_value ));
			addCell( m_pRowElement, newTiXmlText( int2str( it->getPlanDays()).c_str()), 0, NULL, "Number" );
			addCell( m_pRowElement, newTiXmlText( int2str( it->getPlanHours()).c_str()), 0, NULL, "Number" );

			string time = ptime2str(it->getStartTime(), true);//开始时间
			writeDataTime(time);

			time = ptime2str(it->getEndTime(), true);//结束时间
			writeDataTime(time);

			str_value = m_pPrjData->getTaskCardStateById( it->getState());
			addCell( m_pRowElement, newTiXmlText( str_value ) );

			oss << (int)it->getDifficulty();
			addCell( m_pRowElement, newTiXmlText( oss.str().c_str() ) );
			oss.str("");

            m_pTable->LinkEndChild( m_pRowElement );
        }

		m_pRowElement = addRow( m_pTable, NULL, DATA_COLS-1); //添加一空行
    }

    addRow( m_pTable, NULL );

    return true;
}

//保存里程碑
bool CNProjectXLSXMLData::saveMilestones()
{
    if( !m_pTable )
    {
        return false;
    }

    //项目资源项
    m_pRowElement = addRow( m_pTable, newTiXmlText( L"里程碑" ), 0, STYLE_TITLE_1 );

    m_pRowElement = addRow( NULL, newTiXmlText( L"名称" ), 1, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"时间" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"备注" ), 4, STYLE_TITLE_1 );
    m_pTable->LinkEndChild( m_pRowElement );

    //数据行
    size_t stCount = m_pPrjData->m_milestones.size();
    CNProjectMileStone_LST::iterator it = m_pPrjData->m_milestones.begin();
    for ( ; it != m_pPrjData->m_milestones.end() ; ++it)
    {
        m_pRowElement = addRow( NULL, newTiXmlText( it->getName()), 1 );

        string time = ptime2str(it->getTime(), true);

        if( string::npos != time.find("00:00:00") )
        {
            addCell( m_pRowElement, newTiXmlText( time.c_str() ), 0, STYLE_DATE_ONLY , "DateTime");
        }
        else
        {
            addCell( m_pRowElement, newTiXmlText( time.c_str() ), 0, STYLE_DATE_TIME , "DateTime");
        }

        addCell( m_pRowElement, newTiXmlText( it->getMemo()), 4 );

        m_pTable->LinkEndChild( m_pRowElement );
    }

    addRow( m_pTable, NULL );

    return true;
}

TiXmlText* CNProjectXLSXMLData::newTiXmlText( const char* text )
{
    TiXmlText *p_content = (text == NULL) ? NULL : new TiXmlText( text );

    return p_content;
}

TiXmlText* CNProjectXLSXMLData::newTiXmlText( const kn_string &text )
{
    TiXmlText *p_content = (text.empty()) ? NULL : new TiXmlText( UnicodeToUTF8( text ).c_str());

    return p_content;
}

TiXmlElement* CNProjectXLSXMLData::addRow( TiXmlElement* parent,
                                           TiXmlText *   p_content,
                                           UINT          corss_cols,
                                           const char*   style )
{
    TiXmlElement* p_row = new TiXmlElement( "Row" );

    if( p_row )
    {
        TiXmlElement *cell = new TiXmlElement( "Cell" );
        cell->SetAttribute( "ss:StyleID", "Default" /*STYLE_EMPTY_LINE*/ );
        p_row->LinkEndChild( cell );       //第一列固定为空行

        cell = new TiXmlElement( "Cell" ); //第2列，变成了实际上的第1列
        if( corss_cols > 0 )
        {
            cell->SetAttribute( "ss:MergeAcross", corss_cols );
        }

        if( style )
        {
            cell->SetAttribute( "ss:StyleID", style );
        }
        else
        {
            cell->SetAttribute( "ss:StyleID", STYLE_EMPTY_LINE );
        }

        if( p_content != NULL )
        {
            TiXmlElement *data_node = new TiXmlElement( "Data" );
            if( data_node )
            {
                data_node->SetAttribute( "ss:Type", "String" );
                data_node->LinkEndChild( p_content );

                cell->LinkEndChild( data_node );
            }
        }
        else if( corss_cols == 0 ) //纯空行
        {
            cell->SetAttribute( "ss:StyleID", "Default" );
            cell->SetAttribute( "ss:MergeAcross", DATA_COLS - 1 );
        }

        p_row->LinkEndChild( cell );

        if( parent )
        {
            parent->LinkEndChild( p_row );
        }
    }

    return p_row;
}

//保存事件
bool CNProjectXLSXMLData::saveEvents()
{
    if( !m_pTable )
    {
        return false;
    }

    //项目资源项
    m_pRowElement = addRow( m_pTable, newTiXmlText( L"事件" ), 0, STYLE_TITLE_1 );

    m_pRowElement = addRow( NULL, newTiXmlText( L"名称" ), 1, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"时间" ), 0, STYLE_TITLE_1 );
    addCell( m_pRowElement, newTiXmlText( L"备注" ), 4, STYLE_TITLE_1 );
    m_pTable->LinkEndChild( m_pRowElement );

    //数据行
    size_t stCount = m_pPrjData->m_events.size();

    for ( size_t i = 0; i < stCount; i++ )
    {
        CNProjectEvent& event = m_pPrjData->m_events[i];

        m_pRowElement = addRow( NULL, newTiXmlText( event.getName()), 1 );

        string time = ptime2str(event.getTime(), true);

        if( string::npos != time.find("00:00:00") )
        {
            addCell( m_pRowElement, newTiXmlText( time.c_str() ), 0, STYLE_DATE_ONLY , "DateTime");
        }
        else
        {
            addCell( m_pRowElement, newTiXmlText( time.c_str() ), 0, STYLE_DATE_TIME , "DateTime");
        }

        addCell( m_pRowElement, newTiXmlText( event.getMemo()), 4 );

        m_pTable->LinkEndChild( m_pRowElement );
    }

    addRow( m_pTable, NULL );

    return true;
}

//保存xls样式，本方法只会保存使用到的几种基本样式，其余样式全部舍弃
bool CNProjectXLSXMLData::saveStyles()
{
    if( (!m_p_root) || (!m_pTable) )
    {
        return false;
    }

    TiXmlElement * pStyles = new TiXmlElement ( "Styles" );
    if( !pStyles )
    {
        return false;
    }

    //保存default style
    TiXmlElement * pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", "Default" );
        SetAttributeStr( pStyle, "ss:Name", "Normal" );
        pStyles->LinkEndChild( pStyle );
    }

    //保存日期样式
    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_DATE_ONLY );
        setCommonStyle( pStyle );
        TiXmlElement* p_format = add_node(NULL, "NumberFormat", NULL);
        if(p_format)
        {
            SetAttributeStr( p_format, "ss:Format", "Short Date" );
            pStyle->LinkEndChild( p_format );
        }
        pStyles->LinkEndChild( pStyle );
    }

    //保存时间样式
    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_TIME_ONLY );
        setCommonStyle( pStyle );
        TiXmlElement* p_format = add_node(NULL, "NumberFormat", NULL);
        if(p_format)
        {
            SetAttributeStr( p_format, "ss:Format", "h:mm:ss" );
            pStyle->LinkEndChild( p_format );
        }
        pStyles->LinkEndChild( pStyle );
    }

    //日期时间均保存
    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_DATE_TIME );
        setCommonStyle( pStyle );
        TiXmlElement* p_format = add_node(NULL, "NumberFormat", NULL);
        if(p_format)
        {
            SetAttributeStr( p_format, "ss:Format", "DateTime" );
            pStyle->LinkEndChild( p_format );
        }
        pStyles->LinkEndChild( pStyle );
    }

    //空行
    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_EMPTY_LINE );
        setCommonStyle( pStyle );
        pStyles->LinkEndChild( pStyle );
    }

    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_TITLE_1 );
        setCommonStyle( pStyle, "#99CCFF" );
        pStyles->LinkEndChild( pStyle );
    }

    pStyle = new TiXmlElement ( "Style" );
    if( pStyle )
    {
        SetAttributeStr( pStyle, "ss:ID", STYLE_TITLE_2 );
        setCommonStyle( pStyle, "#808000" );
        pStyles->LinkEndChild( pStyle );
    }

    m_p_root->LinkEndChild( pStyles );

    //设置列宽
    SetAttributeStr(m_pTable, "ss:ExpandedColumnCount", "10" );
    SetAttributeStr(m_pTable, "x:FullColumns", "1" );
    SetAttributeStr(m_pTable, "x:FullRows", "1" );
    SetAttributeStr(m_pTable, "ss:DefaultColumnWidth", "54" );
    SetAttributeStr(m_pTable, "ss:DefaultRowHeight", "13.5" );

    TiXmlElement* p_col = add_node(NULL, "Column" , NULL);
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "13.5" );
    m_pTable->LinkEndChild( p_col );

    p_col = add_node(NULL, "Column" , NULL);
    SetAttributeStr(p_col, "ss:Index", "2" );
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "150" );
    m_pTable->LinkEndChild( p_col );

    p_col = add_node(m_pTable, "Column" , NULL);
    SetAttributeStr(p_col, "ss:Index", "4" );
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "75" );

	p_col = add_node(m_pTable, "Column" , NULL);
    SetAttributeStr(p_col, "ss:Index", "5" );
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "75" );

	p_col = add_node(m_pTable, "Column" , NULL);
    SetAttributeStr(p_col, "ss:Index", "6" );
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "75" );

	p_col = add_node(m_pTable, "Column" , NULL);
	SetAttributeStr(p_col, "ss:Index", "8" );
	SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
	SetAttributeStr(p_col, "ss:Width", "75" );

	p_col = add_node(m_pTable, "Column" , NULL);
	SetAttributeStr(p_col, "ss:Index", "9" );
	SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
	SetAttributeStr(p_col, "ss:Width", "75" );

    p_col = add_node(NULL, "Column" , NULL);
    SetAttributeStr(p_col, "ss:Index", "10" );
    SetAttributeStr(p_col, "ss:AutoFitWidth", "0" );
    SetAttributeStr(p_col, "ss:Width", "13.5" );
    m_pTable->LinkEndChild( p_col );

    return true;
}

TiXmlElement* CNProjectXLSXMLData::addCell( TiXmlElement *pRow,
                                            TiXmlText *   p_content,
                                            UINT          cross_col,
                                            const char*   style,
                                            const char*   field_type
                                            )
{
    if( NULL == pRow )
    {
        return NULL;
    }

    TiXmlElement *cell = new TiXmlElement( "Cell" );
    if( cell )
    {
        if( cross_col > 0 )
        {
            cell->SetAttribute( "ss:MergeAcross", cross_col );
        }

        if( style != NULL )
        {
            cell->SetAttribute( "ss:StyleID", style );
        }
        else
        {
            cell->SetAttribute( "ss:StyleID", STYLE_EMPTY_LINE );
        }

		if( p_content )
		{
			TiXmlElement *data_node = new TiXmlElement( "Data" );
			if( data_node )
			{
				data_node->SetAttribute( "ss:Type", field_type );
				data_node->LinkEndChild( p_content );

				cell->LinkEndChild( data_node );
			}
		}


        pRow->LinkEndChild( cell );
    }

    return cell;
}

std::string CNProjectXLSXMLData::int2str( int intstr )
{
    char buf[10];
    sprintf( buf, "%d", intstr );
    std::string str = buf;
    return str;
}

TiXmlElement* CNProjectXLSXMLData::newXlsBorder( const char*Position, const char*LineStyle, const char* Weight )
{
    TiXmlElement *p_border = new TiXmlElement( "Border" );

    if( p_border )
    {
        SetAttributeStr( p_border, "ss:Position", Position );
        SetAttributeStr( p_border, "ss:LineStyle", LineStyle );
        SetAttributeStr( p_border, "ss:Weight", Weight );
    }

    return p_border;
}

void CNProjectXLSXMLData::setCommonStyle( TiXmlElement *p_style, const char* color )
{
    if( p_style == NULL )
    {
        return;
    }

    TiXmlElement *p_align = new TiXmlElement( "Alignment" );
    if( p_align )
    {
        SetAttributeStr( p_align, "ss:Horizontal", "Left" );
        SetAttributeStr( p_align, "ss:Vertical", "Center" );
        SetAttributeStr( p_align, "ss:WrapText", "1" );
        p_style->LinkEndChild( p_align );
    }

    TiXmlElement *borders = new TiXmlElement( "Borders" );
    if( borders )
    {
        TiXmlElement *border = newXlsBorder( "Bottom" );
        borders->LinkEndChild( border );

        border = newXlsBorder( "Left" );
        borders->LinkEndChild( border );

        border = newXlsBorder( "Right" );
        borders->LinkEndChild( border );

        border = newXlsBorder( "Top" );
        borders->LinkEndChild( border );
    }

    p_style->LinkEndChild( borders );

    TiXmlElement *pfont = new TiXmlElement( "Font" );
    if( pfont )
    {
        SetAttributeStr( pfont, "ss:FontName", "宋体" );
        SetAttributeStr( pfont, "x:CharSet", "134" );

        p_style->LinkEndChild( pfont );
    }

    if( color )
    {
        TiXmlElement *p_color = new TiXmlElement( "Interior" );
        if( p_color )
        {
            SetAttributeStr( p_color, "ss:Color", color );
            SetAttributeStr( p_color, "ss:Pattern", "Solid" );
            p_style->LinkEndChild( p_color );
        }
    }
}
