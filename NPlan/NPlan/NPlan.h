//  File:        NPlan.h
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
#ifndef NPLAN_____
#define NPLAN_____

#ifdef WIN32
#include "resource.h"
#endif
#include "message.h"

#define SWITCH_PAGE_ANI_STOP		KNUIMSG_USER+1
#define STORYPAGE_ACTION_DRAG		KNUIMSG_USER+2
#define STORYPAGE_ACTION_DRAG_UP	KNUIMSG_USER+3
#define STORYPAGE_DATA_UPDATE		KNUIMSG_USER+4
#define PROJECTPAGE_DATA_UPDATE		KNUIMSG_USER+5
#define PROJECTPAGE_IN				KNUIMSG_USER+6  //主界面切换到项目编辑界面画面
#define PROJECTPAGE_OUT				KNUIMSG_USER+7  //项目编辑界面 切换到 主界面画面
#define BOTTOM_MENU_HIDEANI_END		KNUIMSG_USER+8  //项目编辑界面 底部菜单按钮隐藏动画结束
#define BOTTOM_MENU_SHOWANI_END		KNUIMSG_USER+9  //项目编辑界面 底部菜单按钮隐藏动画结束
#define TASK_MEMBER_DRAG			KNUIMSG_USER+10	// TASK界面的人员编辑拖拽
#define TASK_MEMBER_DRAG_UP			KNUIMSG_USER+11 // TASK界面的人员编辑拖拽结束
#define TASK_GROUP_DRAG_UP			KNUIMSG_USER+12 // 拖拽窗口拖拽结束
#define TASK_GROUP_DRAG				KNUIMSG_USER+13 // 拖拽窗口拖拽
#define TIMEPAGE_TASK_EDIT			KNUIMSG_USER+14 // 任务编辑
#define TIMEPAGE_TASK_EDIT_ANI_HIDE	KNUIMSG_USER+15 // 任务编辑
#define TASK_MEMBER_CONST_DRAG		KNUIMSG_USER+16 // 成员列表界面的拖拽
#define TASK_MEMBER_CONST_DRAG_UP	KNUIMSG_USER+17 // 成员列表界面的拖拽
#define TIMEBAR_PROJ_SCALE_END		KNUIMSG_USER+18 // 时间轴界面项目时间缩放
#define CALENDAR_DAY_CLICK			KNUIMSG_USER+19 // 日历界面天数单击
#define SWITCH_MONTH_ANI_STOP		KNUIMSG_USER+20 // 日历月份切换动画结束
#define TASK_VIEW_DRAG				KNUIMSG_USER+21 //任务拖拽
#define TASK_VIEW_DRAG_UP			KNUIMSG_USER+22  //任务拖拽结束
#define PROJECT_EDIT_UPDATE_POS		KNUIMSG_USER+23 // 任务编辑位置更新调整
#define MSG_WELCOME_ANI_OVER		KNUIMSG_USER+24 //欢迎界面结束
#define MSG_SAVE_AS					KNUIMSG_USER+25 //另存为消息




 void sendUserInfo();

void CreateView();

void ReleaseAll();

 

void SetResourcePath();

void ResizeWindow(void* dst, int w, int h);

void SendOSMessage(int message, int wParam, int lParam);
 
#endif // NPLAN_____
