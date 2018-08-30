//  File:        KNFile.h
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
* 文件名称：KNFile.h
* 摘    要：原始文件类，兼容普通文件及打包文件，现重新设计有KFie、KComplexFile、KPackedFile分别对应普通文件、打包文件、被压缩文件
*
* 版    本：1.0
* 创建日期：2010/03/30
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _KNFILE_H_
#define _KNFILE_H_

// 使用KNFile前请先Read Me：
// KNFile主要用于文件读写，为使文件读写独立于操作系统，特编写文件读写类
// KNFile主要功能有两个：读取变量值、读取一块内存 
// 请不要将与特定环境有关的变量用于数据读取，否则会引起错误
// 如：Type.h中定义，当没有定义UNICODE时:kn_char=char；当定义了UNICODE时:kn_char=wchar_t
// 这里，必须使用明确长度的变量，如：byte,short,long,float,double

// 最好使用KNFile进行文件读取，当操作系统变化，重新实现KNFile的成员函数，接口可以保持不变

// This class now can be only used in Win32.

// 如果要提高速度,将Read函数改为内联函数试试.

#include "NE_pubhead.h"

#include "KFile.h"

class KComplexFile;

typedef map<string, KComplexFile*> KComplexFileMap;

// ADD. hzf.2006.11.8.
// --Begin
//lint -e1735 -e1506 -e774 -e1740 -e1763
// --End

#define REVERSE2(x) ((((x)&0xff00)>>8)|(((x)&0x00FF)<<8))
#define REVERSE4(x) ((((x)&0xff000000)>>24)|(((x)&0x00ff0000)>>8)|(((x)&0x0000ff00)<<8)|(((x)&0x000000ff)<<24))

#define KNERR_FILE_ERR RESULT_UNKNOW
#define KNERR_FILE_OK  RESULT_OK

class KNFile : public KFile
{
public:
	KNFile();
	virtual ~KNFile();

public:
    // 收集所有打包文件
    static kn_int GetFileIndex(kn_string strConfigFileName, kn_string strPath);
    // 清理所有打包文件
    static kn_int ReleaseComplexFiles();

public:
	kn_bool IsFileOpen() const;

	/*
	* 函数功能：打开文件 
	* 参    数：
	* 		strFileName [in] ：文件名
	* 		dwShareMode [in]：打开模式(share/exclusive)
	*		dwCreationDisposition [in]：参考API:CreateFile()
	*		dwFlagsAndAttributes [in]：参考API:CreateFile()
	* 返 回 值：错误信息
	*/ 
	virtual kn_long OpenFile(const kn_string& strFileName,
							 kn_dword dwDesiredAccess = GENERIC_READ,
							 kn_dword dwShareMode = FILE_SHARE_READ,
							 kn_dword dwCreationDisposition = OPEN_EXISTING,
							 kn_dword dwFlagsAndAttributes = FILE_ATTRIBUTE_READONLY);

	/*
	* 函数功能：关闭文件
	* 返 回 值：错误信息
	*/ 
	virtual kn_long CloseFile();

	/*
	* 函数功能：移动文件句柄到指定位置，兼容被打包文件与普通文件
	* 参    数：
	* 		lOffset [in] ：移动大小
	* 		dwMoveMethod [in]：0-文件头  1-当前位置  2-文件尾
	* 返 回 值：错误信息
	*/ 
	virtual KNRESULT Seek(kn_long lOffset, kn_dword dwMoveMethod);

	/*
	* 函数功能：刷新缓存
	* 参    数：
	* 返 回 值：错误信息
	*/ 
	virtual KNRESULT Flush();

    // 如下接口为针对被打包文件而言
    //////////////////////////////////////////////////////////////////////////
    /* 函数功能：取得文件大小
    * 
    * @param ulSize    ulSize
    */
    virtual KNRESULT GetSize(IN kn_ulong& ulSize);


	/*
	* 函数功能：取得当前文件指针的位置 
	* 返 回 值：相对文件起始位置的位置
	*/ 
	virtual kn_long Tell();
    //////////////////////////////////////////////////////////////////////////

private:

    static KComplexFileMap m_mapComplexFiles;

private:
	// 针对被打包文件，当前文件在打包文件中的偏移
	kn_ulong m_ulPackedFileOffset;
};

// ADD. hzf.2006.11.8.
// --Begin
//lint +e1735 +e1506 +e774 +e1740 +e1763
// --End

#endif // _KNFILE_H_
