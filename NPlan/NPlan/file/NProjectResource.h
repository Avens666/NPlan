//  File:        NProjectResource.h
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
#ifndef NProjectResource_h__
#define NProjectResource_h__

#include "utility.h"
#include "NProjectObject.h"
#include "NProjectDataStruct.h"

class CNProjectResource : public CNProjectObject
{
public:

    CNProjectResource( void );

    CNProjectResource(const kn_char * _name, int _id, int _type, int groupId = 0 );

    CNProjectResource(const kn_char * _name, int _id, res_type _type, int groupId = 0 );

    ~CNProjectResource( void );

private:
    //资源类别
    res_type m_type;

    //所属于的资源组ID
    std::vector <int> m_group_ids;

public:
    //设置资源类型
    void setType( const kn_char* _type );

    //设置资源类型
    void setType( int _type );

    //设置资源所属的资源组
    void setGroup( int groupId );

    //设置资源所属的资源组
    void setGroups(const vector <int>& vectIds );

    //获取资源类型
    res_type getType();

    //获取资源所属的资源组列表
    vector <int> getGroups();

    //将res_type类型转化为kn_string类型
    kn_string resTypeConvertStr( res_type _type );

private:
	//int 类型的restype转换至res_type类型
	res_type convertResType(int _type);

	//string类型的restype转换至res_type类型(暂支持“男”，“女”，“设备”)
	res_type convertResType(const kn_char* _type );
};
#endif
