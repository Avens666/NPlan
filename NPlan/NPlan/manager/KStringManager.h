//  File:        KStringManager.h
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
#ifndef K_STRING_MANAGER_H
#define K_STRING_MANAGER_H

#include "NE_type.h"
#include "boost/unordered_map.hpp"
#include "CharSet.h"

#ifdef WIN32
#include <comdef.h>
#endif
#define  mapStringMap boost::unordered_map<kn_string, kn_string>

#define LAN_CN 0
#define LAN_EN 1
#define LAN_JP 2
class KStringManager
{
private:


		mapStringMap m_StringManagerMap;
		kn_bool LoadStringFromFile(kn_string strFilePath);

public:
			KStringManager(kn_string strFilePath);
		~KStringManager();
		
		kn_string GetStringByID(kn_string nStringKey);
		kn_bool AddString(kn_string sKey,kn_string sValue);
		/**
		* string转换kn_string
		*/
		inline kn_string s2ns(const std::string& s)
		{

            return Char2KnString(s.c_str());
//#ifdef _UNICODE
//			_bstr_t t = s.c_str();
//			wchar_t*        pwchar = (wchar_t*)t;
//			std::wstring    result = pwchar;
//			return result;
//#else
//			return s;
//#endif
		};


};

 KStringManager* getStringManager();
 void releaseStringManager();

#endif
