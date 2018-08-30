//  File:        UserCountSuggestion.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/3
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef USER_COUNT_SUGGESTION_H
#define USER_COUNT_SUGGESTION_H

#include "Connection.h"

class UserCountSuggestion
{
public:
	UserCountSuggestion();
	virtual ~UserCountSuggestion();
	int  QueryRequest();

private:
	bool ParseHost();
	bool SetRequest();

protected:
	std::string m_str_request;
	std::string m_str_host;
	Connection  m_conn;
};
#endif //USER_COUNT_SUGGESTION_H
