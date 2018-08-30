//  File:        NProjectBaseFile.h
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
#ifndef NProjectBaseFile_h__
#define NProjectBaseFile_h__

#include "NProjectDataStruct.h"
#include "NProjectData.h"

//NPlan项目文件的基类
class CNProjectBaseFile
{
public:
    CNProjectBaseFile( CNProjectData* pData );

	virtual ~CNProjectBaseFile();

public:

    void setProjectFileType( nplan_fileType fileType );

    //加载数据文件    //查找当前工作目录

    virtual  int load( kn_string &filename );

    virtual  int load( kn_string &filename, nplan_fileType file_type );

    //保存文件
    virtual int save( kn_string &filename);

    virtual int save( kn_string &filename, kn_char* lpTemplatefile = NULL );

protected:

    //导入，解析文件并导入到项目
    virtual bool parseBaseInfo() { return false; };

    virtual bool loadResourcePool(){ return false; };

    virtual bool parseTaskBoard(){ return false; };

    virtual bool loadTasks(){ return false; };

    virtual bool parseEvents(){ return false; };

    virtual bool parseMilestones(){ return false; };

    //导出，从项目中导出数据至文件
    virtual bool saveBaseInfo(){ return false; };

    virtual bool saveResourcePool(){ return false; };

    virtual bool saveTaskBoard(){ return false; };

    virtual bool saveTasks(){ return false; };

    virtual bool saveMilestones(){ return false; };

    virtual bool saveEvents( ){ return false; };

    virtual bool checkWarning(CNProjectTask& task);

    virtual bool saveWarning();

protected:
    //数据指针
    CNProjectData* m_pPrjData;

private:

    nplan_fileType m_FileType;
};
#endif
