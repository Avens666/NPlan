//  File:        NProjectObject.h
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
#ifndef NProjectObject_h__
#define NProjectObject_h__

#include "NE_type.h"

class CNProjectObject
{
public:

    CNProjectObject();

    CNProjectObject( const kn_char*_name, int _id = 0 );

    virtual ~CNProjectObject();

    //获得对象ID
    int getId() const;

    //设置对象ID
    void setId( int _id );

    void setId( const kn_char* _id );
    //获取对象名称
    kn_string getName() const;

    //设置对象名称
    void setName( const kn_char* _name );

    bool operator< (const CNProjectObject& other) const ;

private:

    //对象名称
    kn_string m_name;

    //对象编号id
    int m_id;
};
#endif // NProjectObject_h__
