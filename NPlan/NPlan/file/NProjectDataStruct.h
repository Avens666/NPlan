//  File:        NProjectDataStruct.h
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
* 文件名称：NprojectDataStruct.h
* 摘    要：NPlan项目结构体
*
* 版    本：1.0
* 创建日期：2013年7月31日11:26:00
* 作    者：mingg
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/
#ifndef _NPROJECT_DATA_STRUCT_H
#define _NPROJECT_DATA_STRUCT_H
#include "utility.h"
#include "../manager/KConfigManager.h"
#include "../manager/KStringManager.h"
#include <boost/date_time/posix_time/posix_time.hpp>

enum nplan_fileType
{
	UNKNOWN_PRJ_FILE,
	XML_PRJ_FILE,
	XML_PRJ_BUFFER,     //xml文件流
	XML_RES_FILE,       //xml全局资源文件
	XML_TASKBOARD_FILE, //xml全局任务墙文件
	XLS_RESOURCE_FILE,  //excel格式的资源文件
	XLS_TASK_FILE,      //excel格式的任务文件
    XLS_PRJ_FILE,       //excel格式的项目信息
    XLS_XML_FILE,       //XLS 另存的xml文件
    MPP_XML_FILE        //mpp另存为的xml文件
} ;

enum res_type
{
	res_man,
	res_woman,
	res_device,
	res_unknown
} ;

#define  RES_NAME_UNKNOWN  _T("unknown")

//warning来源
enum warning_source
{
    WS_BaseInfo = 0,  //BaseInfo
    WS_Task_Board,    //TaskBoard
    WS_Task,          //任务
    WS_TaskCard,      //子任务
    WS_Event,         //事件
    WS_MileStone,     //里程碑
    WS_ResGroup,      //资源池
    WS_Resource,      //资源
    WS_ALL,           //所有对象
} ;

//warning等级
enum warning_type
{
    WT_Warning_Suggest = 0,   //建议
    WT_Warning_Info,          //提示
    WT_Warning_Warning,       //警告
    WT_Warning_Error,         //错误
    WT_Warning_FatalError,    //严重错误。
};

#endif
