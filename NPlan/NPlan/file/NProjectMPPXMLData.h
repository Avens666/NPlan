//  File:        NProjectMPPXMLData.h
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
* 文件名称：NProjectXLSXMLData.h
* 摘    要：NProject解析由xls另存为的xml数据解析和保存类
*
* 版    本：1.0
* 创建日期：2013年10月11日11:40:05
* 作    者：mingg
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/


#ifndef _NProjectMPPXMLData_H_
#define _NProjectMPPXMLData_H_

#include "NProjectBaseFile.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "FilePath.h"

class CNProjectMPPXMLData : public CNProjectBaseFile
{
public:

    CNProjectMPPXMLData (CNProjectData* pData );

    ~CNProjectMPPXMLData(void);

public:

    //加载数据文件
    int load( kn_string &filename );

    //保存为可由MicroSoft Project直接打开的xml格式文件
    int save( kn_string &filename );

private:

    //解析项目基本信息
    bool parseBaseInfo();

    //解析资源
    bool parseResources();

    //解析任务分配
    bool parseAssignment();

    //解析任务列表
    int parseTasks();

    int calcActionStateByPercent(int precent);
private:

    //查找第一个有效的任务节点
    TiXmlElement* findFirstValidTaskElem();

    //计算两个时间之间的间隔，返回间隔天数和小时数
    void calcTimeInterval( kn_string &start_time, kn_string &end_time, OUT int& days, OUT int& hours );

    bool save_all_resource( TiXmlElement* p_resources );
    
    void save_calendars();
    
    void save_assignment(TiXmlElement* assignments, ptime &action_stime, ptime &action_etime, vector<kn_uint> &vect_hours, CNProjectTaskCard_LST::iterator iter_taskcard, const string& str_uid);

    bool save_all_task( TiXmlElement* tasks, TiXmlElement* assignments );

private:

    //XML文档指针，用于文档的加载和保存
    TiXmlDocument *m_p_xmldoc;

    //XML根节点指针
    TiXmlElement *m_p_root;

    bool m_is_nplan_format; //是否为NPlan导出的扩展MS Project格式

};

#endif
