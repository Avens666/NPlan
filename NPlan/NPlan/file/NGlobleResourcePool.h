//  File:        NGlobleResourcePool.h
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
#ifndef _NPROJECT_GLOBLE_RESOURCEPOOL_H
#define _NPROJECT_GLOBLE_RESOURCEPOOL_H

#include "NProjectDataStruct.h"
#include "NProjectResource.h"
#include "NProjectResGroup.h"
#include "FilePath.h"

//全局资源池类
class CNGlobleResourcePool
{
public:

    CNGlobleResourcePool( void );

    ~CNGlobleResourcePool( void );

    //全局资源池获取单例
    //static CNGlobleResourcePool* getGlobleResPoolInstance()
    //{
    //    if ( m_pInstance == NULL )  //判断是否第一次调用
    //    {
    //        m_pInstance = new CNGlobleResourcePool();
    //    }

    //    return m_pInstance;
    //}

public:

    //加载全局资源文件
    int loadXMLFile( kn_string &filename );

    //保存全局资源文件
    int saveXMLFile( kn_string &filename );

	//加载全局资源文件
	int loadCSVFile( kn_string &filename );

    //增加资源
    void addResource( CNProjectResource & res );

    //增加资源组
    void addResGroup( CNProjectResGroup & resgp );

    //获取最大的资源ID
    int  getLargestResId();

    //获取最大资源组ID
    int  getLargestResGroupId();

	//获得资源数量
	int  getCount();

	//通过ID来判断其是否存在
	kn_bool isExitResByID(kn_int id);

	CNProjectResource* getResByIndex(kn_uint index);
	CNProjectResource* getResByID(int id);

	void clear();
protected:

    //全局资源池资源组列表
    std::vector <CNProjectResGroup> m_resourcegroups;

    //全局资源池资源列表
    std::vector <CNProjectResource> m_resources;

    ////全局资源池单例
    //static CNGlobleResourcePool * m_pInstance;
};

#endif
