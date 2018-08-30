//  File:        FilePath.cpp
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
* 文件名称：FilePath.cpp
* 摘    要：文件路径相关函数
*
* 版    本：1.0
* 创建日期：2010/04/01
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#include "FilePath.h"
#ifdef ANDROID_NDK
#include <sys/stat.h>
#include <linux/ncp.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "CharSet.h"
#define MAX_PATH	(260)
#elif defined(IOS_DEV)
#include <sys/stat.h>
#endif
#ifdef LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif
#include "CharSet.h"

#define UP_DIR_STRING	_T("..")
#define CUR_DIR_STRING	_T(".")
#define SPLIT_STRING	("#")
#define SPLIT_CHAR		('#')

kn_char s_DataPath[MAX_PATH] = {0};
kn_char s_CurrentPath[MAX_PATH] = {0};
kn_char s_SavPath[MAX_PATH] = {0};
kn_char s_SavUserPath[MAX_PATH] = {0};
//	系统使用的数据文件名,全路径,用于判断数据是否受控
kn_char s_wszDataFile[MAX_PATH] = {0};

// 各模块相关路径
static KModulePathMap s_mapModulePath;

/*
* 函数功能：设置Data目录
* 参数：
*		path [in]：Data所在目录
* 返回值：无
*/
void SetDataPath(IN const kn_char *path)
{
//#ifndef IOS_DEV
	_tcsncpy(s_DataPath, path, MAX_PATH);
//#else
//	#ifdef UNICODE
//		swprintf(s_DataPath, L"%s/Data/\0", path);
//	#else
//		sprintf(s_DataPath, "%s/Data/\0", path);
//	#endif
//#endif
}

/*
* 函数功能：得到Data目录
* 参数：无
* 返回值：Data目录
*/
const kn_char* GetDataPath(void)
{
    return s_DataPath;		
}

/*
* 函数功能：设置Data文件名
* 参数：
*		path [in]：Data文件名
* 返回值：无
*/
void SetDataFile(IN const kn_char *path)
{
    // 	memset(s_wszDataFile, 0, sizeof(s_wszDataFile));
    // 	_tcsncpy(s_wszDataFile, path, MAX_PATH);
    memcpy (s_wszDataFile, path, sizeof(s_wszDataFile));

}

/*
* 函数功能：得到Data文件名
* 参数：无
* 返回值：Data文件名
*/
const kn_char* GetDataFile(void)
{
    return s_wszDataFile;		
}


/*
* 函数功能：设置Sav目录
* 参数：
*		path [in]：Sav所在目录
* 返回值：无
*/
void SetSavPath(IN const kn_char *path)
{
    _tcsncpy(s_SavPath, path, MAX_PATH);
}

/*
* 函数功能：得到Sav目录
* 参数：无
* 返回值：Sav目录
*/
const kn_char* GetSavPath(void)
{
    return s_SavPath;		
}

/*
* 函数功能：设置用户的Sav目录
* 参数：
*		path [in]：Sav所在目录
* 返回值：无
*/
void SetSavUserPath(IN const kn_char *path)
{
	_tcsncpy(s_SavUserPath, path, MAX_PATH);
}

/*
* 函数功能：得到用户的Sav目录
* 参数：无
* 返回值：Sav目录
*/
const kn_char* GetSavUserPath(void)
{
	return s_SavUserPath;		
}

/*
* 函数功能：得到完整路径(Data路径 + 文件路径)
* 参数：
*		strPath [in|out]: 路径信息
* 返回值：无
*/
void GetFilePath(INOUT kn_string& strPath)
{
    // 	kn_char cBuf[MAX_PATH] = _T("");
    // 	_stprintf(cBuf, _T("%s%s\0"), s_DataPath, strPath.c_str());

    kn_string strTemp = strPath;

    strPath = s_DataPath;
    strPath += strTemp;

    //strPath = s_DataPath + strPath;
}


/*
* 函数功能：得到路径
* 参数：
*		allpath [in]: 完整路径(路径 + 文件名)
*		path [out]: 路径信息
* 返回值：无
*/

void GetPath(const kn_char* allpath, kn_char* path)
{
    kn_int len = _tcslen(allpath);
    kn_int i;
	kn_char szMatch = 0;
#ifdef LINUX
    szMatch = _T('/');
#else
	szMatch = _T('\\');
#endif

    for ( i = len-2; i >= 0; --i)
    {		
		if (allpath[i] == szMatch)
        {			
            _tcsncpy(path, allpath, i+1);
            path[i+1] = 0;
            break;
        }
    }
}



/*
* 函数功能：设置当前路径
* 参数：
*		path [out]: 路径信息
* 返回值：无
*/
void SetCurrentPath(IN const kn_char* path)
{
    _tcsncpy(s_CurrentPath, path, MAX_PATH);
}

/*
* 函数功能：得到当前路径
* 参数：无
* 返回值：当前路径
*/
const kn_char*  GetCurrentPath()
{
    return s_CurrentPath;
}

// Set/Get各模块的配置路径
void SetConfigPath(const kn_char* szPath, KModuleEnum eModule)
{
    s_mapModulePath[eModule].strCongigPath = szPath;
}

const kn_char* GetConfigPath(KModuleEnum eModule)
{
    return s_mapModulePath[eModule].strCongigPath.c_str();
}

// Set/Get各模块的实时信息存放路径
void SetSavePath(const kn_char* szPath, KModuleEnum eModule)
{
    s_mapModulePath[eModule].strSavePath = szPath;
}

const kn_char* GetSavePath(KModuleEnum eModule)
{
    return s_mapModulePath[eModule].strSavePath.c_str();
}

// Set/Get各模块的日志信息存放路径
void SetLogPath(const kn_char* szPath, KModuleEnum eModule)
{
    s_mapModulePath[eModule].strLogPath = szPath;
}

const kn_char* GetLogPath(KModuleEnum eModule)
{
    return s_mapModulePath[eModule].strLogPath.c_str();
}

bool IsDirectoryExist(LPCTSTR pszDir)
{
#ifdef WIN32
	DWORD dwAttributes = GetFileAttributes(pszDir);

	if((INVALID_FILE_ATTRIBUTES == dwAttributes) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 目录不存在
	{
		return false;
	}
#else
#ifdef UNICODE
	struct stat buf;
	char temp[MAX_PATH] = {0};
	wcstombs(temp, pszDir, wcslen(pszDir));
	if(stat(temp, &buf) == -1)
	{
		return false;
	}
#else
	struct stat buf;
	if(stat(pszDir, &buf) == -1)
	{
		return false;
	}
#endif
#endif
	return true;
}

bool CreateDirectory(const kn_char* pszDir)
{
#ifdef WIN32

	return CreateDirectory(pszDir, NULL);

#else

    #ifdef UNICODE
	    struct stat buf;
	    char temp[MAX_PATH] = {0};
	    wcstombs(temp, pszDir, wcslen(pszDir));
	    if (mkdir(temp, 0744) == 0)
	    {
		    return true;
	    }
	    else
	    {
		    return false;
	    }
    #else
	    if (mkdir(pszDir, 0744) == 0)
	    {
		    return true;
	    }
	    else
	    {
		    return false;
	    }
    #endif

#endif
}

void GetFilesArrayOfDirectory(IN const kn_char* strFileName, 
											IN const kn_char* strFileType,
											INOUT vector<kn_string>&vStrFileNameArray)
{
#ifdef WIN32
	kn_string strTempFileName;
	TCHAR szFind[MAX_PATH] = {0};
	_tcscpy(szFind, strFileName);
	_tcscat(szFind, strFileType);

	WIN32_FIND_DATA tFindData;
	memset(&tFindData, 0, sizeof(tFindData));
	HANDLE hdle = FindFirstFile(szFind, &tFindData);
	kn_int iCount = 0;
	if (INVALID_HANDLE_VALUE == hdle)
	{
		FindClose(hdle);
	}
	else
	{
		strTempFileName = tFindData.cFileName;
		vStrFileNameArray.push_back(strTempFileName);
		memset(&tFindData, 0, sizeof(tFindData));
		iCount++;
		while (FindNextFile(hdle, &tFindData))
		{
			strTempFileName = tFindData.cFileName;
			if (!strTempFileName.compare(CUR_DIR_STRING) || !strTempFileName.compare(UP_DIR_STRING))
			{
				continue;
			}
			// 将获取到的文件名称插入文件列表数组
			vStrFileNameArray.push_back(strTempFileName);
			strTempFileName.clear();
			iCount++;
		}
	}
	FindClose(hdle);	
#else
	DIR* pDir = NULL;
	struct dirent* ent = NULL;
	struct stat statbuff;
	LOGI("RTIC begin opendir");

    char szDirName[MAX_PATH] = {0};
    char szExFile [32] = {0};

#ifdef UNICODE

    wcstombs(szDirName, strFileName, MAX_PATH - 1);
    wcstombs(szExFile, strFileType, 31);

#else

    strncpy(szDirName, strFileName, MAX_PATH - 1);
    strncpy(szExFile, strFileType, 31);

#endif

    pDir =  opendir(szDirName);

	if (pDir == NULL)
	{
		return;
	}


    bool bFilter = false;
    char* pExStart = NULL;
    int iExLen = 0;
    if(szExFile[0] == 0 || strcmp(szExFile, "*") == 0 || strcmp(szExFile, "*.*") == 0)
    {
        // nothong todo

    }
    else
    {
        pExStart = strstr(szExFile, ".");
        if(pExStart != NULL)
        {
            bFilter = true;
            iExLen = strlen(pExStart);
        }
    }


	while ((ent = readdir(pDir)) != NULL)
	{

        bool bAdd = false;
        if(ent->d_type != DT_DIR)
		{
			if (strcmp(ent->d_name, "..") == 0 ||
				strcmp(ent->d_name, ".") == 0)
			{
				continue;
			}

            if(bFilter)
            {
                int iPos = strlen(ent->d_name) - iExLen;
                if(iPos > 0 && strncmp(ent->d_name + iPos, pExStart, iExLen) == 0)
                {
                       // OK
                    bAdd = true;
                }

            }
            else
            {
                bAdd = true;
            }

            if(bAdd)
            {
#ifdef UNICODE
            kn_char wcszFileName[MAX_PATH] = {0};
            mbstowcs(wcszFileName, ent->d_name, strlen(ent->d_name));
            vStrFileNameArray.push_back(wcszFileName);
#else
            vStrFileNameArray.push_back(ent->d_name);
#endif
            }
		}
	}
    //chdir("..");
	closedir(pDir);
	LOGI("RTIC end GetFiles");
#endif //	WIN32
}

kn_int knDeleteFile(const kn_char* sz)
{
	BOOL b;
#if defined(ANDROID_NDK) || defined(__APPLE__) || defined(LINUX)

    #ifdef UNICODE

        kn_string wstrFile = sz;
        string strName = Knstring2String(wstrFile);
        int ret = unlink(strName.c_str());

        b = (ret == 0);


    #else
        int ret =  unlink(sz);
        b = (ret == 0);
    #endif

#else
   //  win32
	b = DeleteFile(sz);
#endif
	return b;
}

kn_int ClearDir(kn_char* szUpdateSrc)
{
#if defined(ANDROID_NDK) || defined(__APPLE__) || defined(LINUX)
    LOGI("RTIC begin ClearDir %s", szUpdateSrc);
	DIR* pDir = NULL;
	struct dirent* ent = NULL;
	struct stat statbuff;
#ifdef UNICODE

    char temp[MAX_PATH] = {0};
    wcstombs(temp, szUpdateSrc, wcslen(szUpdateSrc));
    pDir = opendir(temp);

#else

    pDir = opendir(szUpdateSrc);

#endif

	if (pDir == NULL)
	{
        LOGE("RTIC opendir error %s", szUpdateSrc);
		return -1;
	}
    //chdir(szUpdateSrc);
	while ((ent = readdir(pDir)) != NULL)
	{

        char szFileName[MAX_PATH] = {0};
        sprintf(szFileName,"%s%s", szUpdateSrc, ent->d_name);


      //  LOGI("RTIC lstat file:%s, %s", ent->d_name, szFileName);

        lstat(szFileName, &statbuff);

		if (S_ISDIR(statbuff.st_mode))
		{
			if (strcmp(ent->d_name, "..") == 0 ||
				strcmp(ent->d_name, ".") == 0)
			{
				continue;
			}
            //ClearDir(ent->d_name);

            strcat(szFileName, "/");

#ifdef UNICODE
            kn_char wcszFileName[MAX_PATH] = {0};
            mbstowcs(wcszFileName, szFileName, strlen(szFileName));
            ClearDir(wcszFileName);
#else
            ClearDir(szFileName);
#endif
		}
		else
		{
            //unlink(ent->d_name);
            unlink(szFileName);

            LOGI ("delete file  %s", szFileName);
		}
	}
    //chdir("..");
	closedir(pDir);
	LOGI("RTIC end ClearDir");

	return 0;
#else
	kn_char szFind[MAX_PATH] = {0};
	_tcscpy(szFind, szUpdateSrc);
	_tcscat(szFind, _T("*"));
	WIN32_FIND_DATA tFindData;
	memset(&tFindData, 0, sizeof(tFindData));
	HANDLE hdle = FindFirstFile(szFind, &tFindData);
	if (INVALID_HANDLE_VALUE == hdle)
	{
		//	打开目录失败
		FindClose(hdle);
		return -1;
	}

	if ((tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		&& wcscmp(tFindData.cFileName, CUR_DIR_STRING) && wcscmp(tFindData.cFileName, UP_DIR_STRING))
	{
		TCHAR tmpPath[MAX_PATH] = {0};
		wcscpy(tmpPath, szUpdateSrc);
		wcscat(tmpPath, tFindData.cFileName);
		wcscat(tmpPath, L"\\");
		ClearDir(tmpPath);
		RemoveDirectory(tmpPath);
	}
	else
	{
		if (wcscmp(tFindData.cFileName, L".") && wcscmp(tFindData.cFileName, L".."))
		{
			TCHAR szDelFile[MAX_PATH] = {0};
			wcscpy(szDelFile, szUpdateSrc);
			wcscat(szDelFile, tFindData.cFileName);
			DWORD dwAttri = GetFileAttributes(szDelFile);
			if (dwAttri != INVALID_FILE_ATTRIBUTES)
			{
				SetFileAttributes(szDelFile, dwAttri & 0xFFFFFFFE);
				BOOL bRet = DeleteFile(szDelFile);

				if (!bRet)
				{
					ASSERT(0);
				}			
			}
			

		}

		memset(&tFindData, 0, sizeof(tFindData));
		while (FindNextFile(hdle, &tFindData))
		{

			if ((tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
				&& wcscmp(tFindData.cFileName, L".") && wcscmp(tFindData.cFileName, L".."))
			{
				TCHAR tmpPath[MAX_PATH] = {0};
				wcscpy(tmpPath, szUpdateSrc);
				wcscat(tmpPath, tFindData.cFileName);
				wcscat(tmpPath, L"\\");

				ClearDir(tmpPath);
				RemoveDirectory(tmpPath);
			}
			else
			{
				if (wcscmp(tFindData.cFileName, L".") && wcscmp(tFindData.cFileName, L".."))
				{
					TCHAR tmpPath[MAX_PATH] = {0};
					wcscpy(tmpPath, szUpdateSrc);
					wcscat(tmpPath, tFindData.cFileName);
					DWORD dwAttri = GetFileAttributes(tmpPath);
					SetFileAttributes(tmpPath, dwAttri & 0xFFFFFFFE);
					BOOL bRet = DeleteFile(tmpPath);
					if (!bRet)
					{
						ASSERT(0);
					}
				}
			}
		}
	}
	FindClose(hdle);
#endif
	return 0;
}

/**
* 文件是否存在
*/
kn_bool IsFileExist(const kn_char* strPath)
{
#if defined(WIN32)
        DWORD dwAttributes = GetFileAttributes(strPath);
        return ((INVALID_FILE_ATTRIBUTES != dwAttributes) &&
                (1 != (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)));
#else
        struct stat stat_buf;
        memset(&stat_buf, 0, sizeof(stat_buf));
#ifdef UNICODE

        char temp[MAX_PATH] = {0};
        wcstombs(temp, strPath, wcslen(strPath));
        if(-1 == lstat(temp, &stat_buf))
        {
            return false;
        }

#else

        if(-1 == lstat(strPath, &stat_buf))
        {
            return false;
        }

#endif
        if(0 == S_ISREG(stat_buf.st_mode))
        {
                return false;
        }

        return true;
#endif
}
