//  File:        KObject.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/24
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef _KOBJECT_H_
#define _KOBJECT_H_

#include "NE_pubhead.h"
#include "KLog.h"
class KObject
{


public:
	KObject(){};
	virtual ~KObject(){};
	virtual kn_string toString() {return kn_string();};


	
};
#endif //  _CKOBJECT_H_
