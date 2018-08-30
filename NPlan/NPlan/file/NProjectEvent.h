//  File:        NProjectEvent.h
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
#ifndef NProjectEvent_h__
#define NProjectEvent_h__

#include "utility.h" 
#include "NProjectObject.h"
#include <boost/date_time/posix_time/posix_time.hpp> 

using namespace boost:: posix_time;

class CNProjectEvent : public CNProjectObject
{
public:

    CNProjectEvent( void );

    CNProjectEvent( const kn_char* _name, const kn_char* _time, const kn_char* _memo, int _id = 0 );

    ~CNProjectEvent( void );

private:  
    //事件发生的时间
    ptime m_time;

    //事件描述
    kn_string m_memo;

public:
    ptime getTime() const;

    kn_string getMemo() const;

    void setTime(kn_string &str_time );

    void setTime( const ptime& _time );

    void setMemo( const kn_char* _memo );
};

#define CNProjectEvent_LST std::list<CNProjectEvent>
 
#endif // NProjectEvent_h__
