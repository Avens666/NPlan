//  File:        NProjectExcelData.h
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

#ifndef NProjectExcelData_h__
#define NProjectExcelData_h__

#include "NProjectBaseFile.h"
#include "NProjectTask.h"
#include "XExcel.h"
#include "NProjectDataStruct.h"

class CNProjectExcelData : public CNProjectBaseFile, public CXExcel
{
public:

    CNProjectExcelData( CNProjectData* pData );

    virtual ~CNProjectExcelData( void );

    virtual int load( kn_string &filename, nplan_fileType file_type );

    virtual int save( kn_string &filename, kn_char* lpTemplatefile = NULL );

private:

    //导入完整的项目信息
    int loadPrjData( kn_string &filename );
    //导入项目资源
    int loadResourcePool( kn_string &filename );
    //导入项目任务
    int loadTasks( kn_string &filename );

    //保存至EXCEL
    bool saveBaseInfo();

    bool saveResourcePool();

    bool saveTaskBoard();

    bool saveTasks();

    bool saveTask( CNProjectTask & task );

    bool saveTaskCard( CNProjectTaskCard & taskCard );

    bool saveMilestones();

    bool saveEvents();

    bool parserEvents( CXExcel &xlsReader );

    bool parserMilestones( CXExcel &xlsReader );

    bool parserTask( CXExcel &xlsReader );

    bool parserTaskCard( CXExcel &xlsReader, CNProjectTask& task );
private:

    int m_iRow;   //模板文件中的行号
};
#endif
#endif
