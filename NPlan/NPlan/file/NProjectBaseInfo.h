//  File:        NProjectBaseInfo.h
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
#ifndef NProjectBaseInfo_h__
#define NProjectBaseInfo_h__

#include "utility.h"
#include "NProjectObject.h"

class CNProjectBaseInfo: public CNProjectObject
{
public:

    CNProjectBaseInfo( void );

    CNProjectBaseInfo( const kn_char * _version, const kn_char * _name, int leader_id );

    ~CNProjectBaseInfo( void );

private:

    // 版本信息
    kn_string m_dataFormatVersion;

    //项目描述
    kn_string m_ProDescribe;

public:

    //获取版本信息
    kn_string getVersion() const;

    //获取负责人名称
    kn_string getProDescribe() const;

    //设置版本号
    void setVersion( const kn_char* _version );

    //设置项目描述
    void setProDescribe( const kn_char * _prodescribe );

    //设置基本信息
    void setBaseInfo( const kn_char* _version, const kn_char* _name, int leader_id, const kn_char* _describe );
};
#endif
