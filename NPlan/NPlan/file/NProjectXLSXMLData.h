//  File:        NProjectXLSXMLData.h
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
/**
 * Copyright (C) 2003-2013，光庭信息有限公司(http://www.kotei-info.com)
 * All rights reserved.
 *
 * 文件名称：CNProjectXLSXMLData.h
 * 摘    要：NProject解析由xls另存为的xml数据解析和保存类
 *
 * 版    本：1.0
 * 创建日期：2013年10月11日10:00:05
 * 作    者：mingg
 *
 * 修改历史：
 * [修改序列][修改日期][修改者][修改内容]
 **/

#ifndef _NProjectXLSXMLData_H
#define _NProjectXLSXMLData_H

#include "tinyxml.h"
#include "tinystr.h"
#include "NProjectBaseFile.h"
#include "FilePath.h"

class MyTiXmlDeclaration : public TiXmlDeclaration
{
public:
    MyTiXmlDeclaration(const char* _progid );
    void Print( FILE* cfile, int depth, TIXML_STRING* str ) const;
    void Print( FILE* cfile, int depth ) const {
		Print( cfile, depth, 0 );
	}
private:

    TIXML_STRING progid;
};

typedef enum
{
    UNKNOWN_Type,
    XLS_XML_Office,    //XLS 另存的xml文件
    XLS_XML_WPS        //mpp另存为的xml文件
} xlsxmlType;

//解析和保存由xls另存为的xml数据类，继承自CNProjectBaseFile，由基类中的数据指针CNProjectData进行数据的传递
//本类不会解析和保存xml文件中保存的样式信息
class CNProjectXLSXMLData : public CNProjectBaseFile
{
public:

    CNProjectXLSXMLData ( CNProjectData* pData );

    ~CNProjectXLSXMLData( void );

    //加载数据文件
    int load( kn_string &filename );

    //保存为可由MicroSoft Office Excel 和 WPS直接打开的xml格式文件
    int save( kn_string &filename );
   
private:

     //查询该文件是否是本类支持解析的xml文件
    bool isSupportedType();

    //解析项目基本信息
    int parsePrjBaseInfo();

    //解析资源组
    bool parseResourceGroup();

    //解析资源
    bool parseResource();

    //解析任务墙类型
    bool parseTaskBoard();

    //解析任务列表
    bool parseTasks();

    bool parserTask( CNProjectTask &task );

    //解析事件,里程碑列表
    bool parseMilestoneEvent( bool is_event );

    ptime parserDataTime( kn_string &date, kn_string &time );
private:

    //获取一行的所有列的内容
    bool getRowValue( TiXmlElement* row, vector <kn_string>& v_cols );

    bool addEvent( const vector <kn_string> &v_cols );

    bool addMileStone( const vector <kn_string> &v_cols );

    //保存xls样式，本方法只会保存使用到的几种基本样式，其余样式全部舍弃
    bool saveStyles();

    //保存项目基本信息
    bool saveBaseInfo();

    //保存资源组
    bool saveResourceGroup();

    //保存资源
    bool saveResource();

    //保存任务墙
    bool saveTaskBoard();

    //保存任务
    bool saveTasks();

    //保存里程碑
    bool saveMilestones();

    //保存事件
    bool saveEvents();

    void setCommonStyle( TiXmlElement *p_style, const char* color = NULL );

    TiXmlText* newTiXmlText( const char* text );

    TiXmlText* newTiXmlText( const kn_string &text );

    TiXmlElement* newXlsBorder( const char*Position, const char*LineStyle = "Continuous", const char* Weight = "1" );

    TiXmlElement* addRow( TiXmlElement* parent, 
                            TiXmlText *p_content, 
                            UINT corss_cols = 0, 
                            const char* style = NULL );

    TiXmlElement* addCell( TiXmlElement *pRow,
                           TiXmlText *   p_content,
                           UINT          across_col = 0,
                           const char*   style = NULL,
                           const char*   field_type = "String" );

	void writeDataTime(string& str_data_time);

    std::string int2str( int intstr );

private:

    //XML文档指针，用于文档的加载和保存
    TiXmlDocument *m_pXmldoc;

    //XML根节点指针
    TiXmlElement *m_p_root;

    //XML 当前Row节点指针
    TiXmlElement *m_pRowElement;

    //Table节点指针，保存时使用
    TiXmlElement *m_pTable;

    xlsxmlType m_filetype;

	kn_string	m_strPM; //为避免对原有解析造成影响，先保存PM名称，解析完资源后，再将名称转换为ID
};
#endif
