//  File:        NProjectResourcePool.h
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
#ifndef _NPROJECT_RESOURCEPOOL_H
#define _NPROJECT_RESOURCEPOOL_H

#include "NProjectDataStruct.h"
#include "NProjectResGroup.h"
#include "NProjectResource.h"
#include "NGlobleResourcePool.h"
#include "utility.h"
class CNProjectResourcePool: public CNGlobleResourcePool
{
public:
    CNProjectResourcePool( void );

    ~CNProjectResourcePool( void );

    //获取所有的资源组
   std::vector <CNProjectResGroup> & getResourceGroups();

    //获取一个资源所属资源组ID列表
    std::vector <int> getResourceBelongGroups( int resId );

    //通过资源组ID获取相关联的名称
    kn_string getResourceGroupName( int groupId );

    //通过资源组名称获取相资源组ID
    int findResourceGroupID( const kn_char* name );

    //通过资源名称获取相资源组ID
    int findResourceID( const kn_char* name );

    //通过资源ID获取相关联的名称
    kn_string getResourceName( int resId );

	CNProjectResource* getResourceByID( int resId );

	res_type getResourceType( int resId );

    void setResourceGroups( std::vector <CNProjectResGroup> & resourcegroups );

    //获取所有的资源项
    std::vector <CNProjectResource> & getResources();

    void setResources( std::vector <CNProjectResource > &resources );

    //设置资源所属组别
    void setResourceGroupIDs( CNProjectResource & res, const kn_char* lpGroups );

    //增加一个资源组,返回资源组id
    int addResGroup( CNProjectResGroup & resGroup );

    //增加一个资源组((返回新增资源组的id, groupId = 0时自动计算)
    int addResGroup( kn_char* name, int groupId = 0 );

    //增加一个资源项，返回该资源的id
    int addResource( CNProjectResource & res );

    //增加一个资源项(资源ID可以自动计算)，返回资源id
    CNProjectResource & addResource( kn_string& name, int type, int id = 0 );
	
	//通过ID删除一个资源项
	bool delResourceByID(int id);
	
	//修改一个资源项
	bool modifyResource(kn_string& name,int type, int id, int newId);
	
	//通过ID查找资源是否存在
	bool checkResourceByID(int id);

    //获取一个资源组有哪些资源
    std::vector <int> getGroupHaveResource( int _groupid );

    //获取未使用的资源ID
    int getUnusedResourceID();

    //获取未使用的资源ID
    int getUnusedResourceGroupID();

    kn_string getResMembersNameStr( int leader_id );

    kn_string typeInt2Str( int type );

};
#endif
