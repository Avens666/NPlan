//  File:        NE_macro.h
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
* 文件名称：macro.h
* 摘    要：基础宏定义
*
* 版    本：1.0
* 创建日期：2010/03/09
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#ifndef _NE_MACRO_H_
#define _NE_MACRO_H_

#define KNAPI               // API调用协议

#define IN      // 输入参数
#define OUT     // 输出参数
#define INOUT   // 输入、输出参数

// 安全删除一个需调用delete释放的对象
#define SAFE_DELETE(pObject) if(pObject) {delete (pObject); (pObject) = NULL;}
// 安全删除一组需调用delete[]释放的对象
#define SAFE_DELETE_GROUP(pObject) if(pObject) {delete[] (pObject); (pObject) = NULL;}

#ifndef PI
    #define PI		(3.14159265358979323846f)
#endif





#endif // _NE_MACRO_H_
