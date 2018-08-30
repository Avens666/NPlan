//  File:        FilePath.h
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
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：FilePath.h
* 摘    要：文件路径相关函数
*
* 版    本：1.0
* 创建日期：2010/04/01
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _FILEPATH_H_
#define _FILEPATH_H_

#include "NE_pubhead.h"

struct KNMODULEPATH
{
    kn_string strCongigPath;
    kn_string strSavePath;
    kn_string strLogPath;
};

enum KModuleEnum
{
    ME_UNION,
    ME_MAPDISP,
    ME_POI,
    ME_POSITION,
    ME_ROUTEPLAN,
    ME_GUIDE,
};

typedef map<KModuleEnum, KNMODULEPATH> KModulePathMap;



KNAPI void SetDataPath(IN const kn_char *path);
KNAPI const kn_char* GetDataPath(void);

KNAPI void SetSavPath(IN const kn_char *path);
KNAPI const kn_char* GetSavPath(void);

KNAPI void SetSavUserPath(IN const kn_char *path);
KNAPI const kn_char* GetSavUserPath(void);

// 得到完整路径，在文件名前加上数据路径
KNAPI void GetFilePath(INOUT kn_string& strPath);
// 得到路径，不包括文件名
KNAPI void GetPath(const kn_char *allpath, kn_char *path);

KNAPI void SetCurrentPath(IN const kn_char *path);
KNAPI const kn_char*  GetCurrentPath();

KNAPI void SetDataFile(IN const kn_char *path);
KNAPI const kn_char* GetDataFile(void);

// Set/Get各模块的配置路径
KNAPI void SetConfigPath(const kn_char* szPath, KModuleEnum eModule = ME_UNION);
KNAPI const kn_char* GetConfigPath(KModuleEnum eModule);

// Set/Get各模块的实时信息存放路径
KNAPI void SetSavePath(const kn_char* szPath, KModuleEnum eModule = ME_UNION);
KNAPI const kn_char* GetSavePath(KModuleEnum eModule);

// Set/Get各模块的日志信息存放路径
KNAPI void SetLogPath(const kn_char* szPath, KModuleEnum eModule = ME_UNION);
KNAPI const kn_char* GetLogPath(KModuleEnum eModule);

bool IsDirectoryExist(LPCTSTR pszDir);

bool CreateDirectory(LPCTSTR pszDir);

kn_bool IsFileExist(const kn_char* strPath);
/**
* 获取一个目录下的所有文件名称(不循环迭代)
*/

void GetFilesArrayOfDirectory(IN const kn_char* strFileName, 
							  IN const kn_char* strFileType,
							  INOUT vector<kn_string>&vStrFileNameArray);

//删除文件跨平台函数
kn_int knDeleteFile(const kn_char* sz);

//	删除目录下所有文件及目录，成功则返回0
kn_int ClearDir(kn_char* szUpdateSrc);

//#ifdef NAVI_ENGINE
// 
//#endif

#endif // _FILEPATH_H_
