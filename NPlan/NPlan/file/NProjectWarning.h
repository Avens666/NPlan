//  File:        NProjectWarning.h
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
* 文件名称：NProjectWarning.h
* 摘    要：NPlan警告信息类
*
* 版    本：1.0
* 创建日期：2013年10月11日10:00:05
* 作    者：mingg
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _NProjectWarning_H
#define _NProjectWarning_H

#include "NProjectDataStruct.h"
#include "NProjectObject.h"

//警告信息类，NPlan警告信息。
class CNProjectWarning : public CNProjectObject
{
public:
    CNProjectWarning();
    ~CNProjectWarning( void );
    CNProjectWarning(int id, const kn_char* msg, warning_source source, warning_type _type);

    void setMsg(const kn_char* msg);
    kn_string getMsg();
    //获取警告信息类型
    warning_type getWarningType();
    //设置警告信息类型
    void setWarnningType(warning_type type);
    kn_string getWarningTypeStr( );
    //获取警告来源
    warning_source getWarningSource();
    //设置警告来源
    void setWarningSource(warning_source source);
protected:
    warning_type       m_warn_type; //warning类型
    warning_source     m_warn_source; //warning来源
};

#define CNProjectWarning_LST std::list<CNProjectWarning>
#endif
