//  File:        KNFile.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2018/8/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2005，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KNFile.cpp
* 摘    要：KNFile实现类
*
* 版    本：2.0.0
* 创建日期：2006.12.9
* 作    者：
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
* (1) 20070717.朱生国.增加对打包文件的支持
* (2) 20070719.xiaojingd.修正Seek函数指针定位计算错误
* (3) 20070720
**/

#include "KNFile.h"
#include "KGlobalFunction.h"
#include "KProfile.h"

#define MAXSTRLEN (255)

#ifdef LINUX
#define  PATH_SEPARATOR '/'
#else
#define PATH_SEPARATOR	'\\'
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KNFile::KNFile()
{
	m_ulPackedFileOffset   = 0;
}

KNFile::~KNFile()
{
	CloseFile();
}

kn_bool KNFile::IsFileOpen() const
{
	return IsOpen();
}

/*
* 函数功能：打开文件 
* 参    数：
* 		strFileName [in] ：文件名
* 		dwShareMode [in]：打开模式(share/exclusive)
*		dwCreationDisposition [in]：参考API:CreateFile()
*		dwFlagsAndAttributes [in]：参考API:CreateFile()
* 返 回 值：错误信息
*/ 
kn_long KNFile::OpenFile(const kn_string& strFileName,
						 kn_dword dwDesiredAccess,
						 kn_dword dwShareMode,
						 kn_dword dwCreationDisposition,
						 kn_dword dwFlagsAndAttributes)
{
    KNRESULT result = RESULT_UNKNOW;
#ifdef KOTEI_LINUX
    ConvertSlash(const_cast<kn_char*>(strFileName.c_str()));
#endif

	m_strName = strFileName;

    string strName;
    static char filename[MAXSTRLEN];
#ifdef UNICODE
    memset(filename, 0, MAXSTRLEN);
     WideCharToMultiByte(936, NULL, (LPCTSTR)strFileName.c_str(), strFileName.size(),
         (LPSTR)filename, strFileName.size() * sizeof(kn_char), NULL, NULL);
#else
     strncpy(filename, strFileName.c_str(), strFileName.length());
#endif

    strName = filename;

    // 处理普通文件
    result = KFile::Open(strFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsAndAttributes);

	if (result == KNRESULT(0)) {
		KFile::GetSize(m_ulSize); // 设置普通文件的Size.
	}

    return result;
}

/*
* 函数功能：关闭文件
* 返 回 值：错误信息
*/ 
kn_long KNFile::CloseFile()
{
    return Close();
}


/**
* 函数功能：取得文件大小
* 
* @param ulSize    ulSize
*/
KNRESULT KNFile::GetSize(IN kn_ulong& ulSize)
{
    ulSize = m_ulSize;

    return RESULT_OK;
}


/*
* 函数功能：移动文件句柄到指定位置，兼容被打包文件与普通文件
* 参    数：
* 		lOffset [in] ：移动大小
* 		dwMoveMethod [in]：0-文件头  1-当前位置  2-文件尾
* 返 回 值：错误信息
*/ 
KNRESULT KNFile::Seek(kn_long lOffset, kn_dword dwMoveMethod)
{
	ASSERT(m_hFile != NULL);		
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    // m_lCurson为逻辑文件的位置
    // m_ulSize为逻辑文件的大小
	if (m_bIsOpen)
	{
		kn_long lCursor = 0; // 计算如果操作成功，最终的定位位置
		if (dwMoveMethod == 0)		// 文件开始
		{
			lCursor = lOffset;
		}
		else if (dwMoveMethod == 1)	// 当前位置
		{
			lCursor = m_lCursor + lOffset;
		}
		else if (dwMoveMethod == 2)	// 文件末尾
		{
			lCursor = m_ulSize - lOffset;
		}

		if (dwMoveMethod == 0)		// 文件开始
		{
			lOffset += m_ulPackedFileOffset; // // 若为普通文件，m_ulPackedFileOffset == 0
		}
		else if (dwMoveMethod == 2)	// 文件末尾
		{
			kn_ulong ulSize = 0;

            KNRESULT result = KFile::GetSize(ulSize); // 整个物理文件的大小
			if (result != RESULT_OK)
			{
				return result;
			}
			lOffset += ulSize - (m_ulPackedFileOffset + m_ulSize); // 若为普通文件，m_ulSize == ulSize，m_ulPackedFileOffset == 0
		}

		if (fseek(m_hFile, lOffset, dwMoveMethod) != 0)
		{
			return RESULT_UNKNOW;
		}

		m_lCursor = lCursor; // 操作成功，则更新m_lCursor.

		return RESULT_OK;
	}

	return KNRESULT(EC_ERROR, SC_BASELIB, EF_FILE, KN_ERROR_NOT_OPEN);	// 文件还没有打开
}

/*
* 函数功能：取得当前文件指针的位置 
* 返 回 值：相对文件起始位置的位置
*/ 
kn_long KNFile::Tell()
{
	if (m_bIsOpen)
	{
		return m_lCursor;
	}
	else
	{
		return RESULT_UNKNOW;
	}
}


/*
* 函数功能：刷新缓存
* 参    数：
* 返 回 值：错误信息
*/ 
KNRESULT KNFile::Flush()
{
	return (fflush(m_hFile) == 0) ? RESULT_OK : RESULT_UNKNOW;
}
