//  File:        NMenu.h
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
#ifndef NMenu_DEFINED
#define NMenu_DEFINED

#include "KMenuView.h"
#include "KTimer.h"

class CMenuScore : public KMenuBase
{
public:
	CMenuScore();
	virtual ~CMenuScore();

	virtual void init(KView_PTR, int score);
	void click( kn_int, kn_int,KMessageMouse* );
private:
	

};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuScore> CMenuScore_PTR;

//////////////////////////////////////////////
class CMenuColor : public KMenuBase
{
public:
	CMenuColor();
	virtual ~CMenuColor();

	virtual void init(KView_PTR);
	virtual void click( kn_int, kn_int,KMessageMouse* );
	sigslot::signal2<KView_PTR, int> m_sign_menu_view_click;
private:
	

};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuColor> CMenuColor_PTR;

//////////////////////////////////////////////
class CMenuSaveas : public CMenuColor
{
public:
	CMenuSaveas();
	virtual ~CMenuSaveas();

	virtual void init(KView_PTR);
	void click( kn_int, kn_int,KMessageMouse* );

};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuSaveas> CMenuSaveas_PTR;

//////////////////////////////////////////////
#define DAY_TYPE_HOLIDAY 1  //工作日
#define DAY_TYPE_WORKDAY 2  //休息日
#define DAY_TYPE_OVERTIME 3  //加班
class CMenuDaySet : public CMenuColor
{
public:
	CMenuDaySet();
	virtual ~CMenuDaySet();

	virtual void init(KView_PTR, int type);
	void click( kn_int, kn_int,KMessageMouse* );

};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuDaySet> CMenuDaySet_PTR;

class CMenuProject : public CMenuColor
{
public:
	CMenuProject();
	virtual ~CMenuProject();

	virtual void init(KView_PTR, int type);
	void click( kn_int, kn_int,KMessageMouse* );

	void onTimer(int);
protected:
//	KTimer_PTR m_timer;
};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuProject> CMenuProject_PTR;

//////////////////////////////////////////////
#define MIND_AREA_MENU 1 
#define MIND_PRJ_MENU 2  
#define MIND_TASK_MENU 3 
#define MIND_ACTION_MENU 4  
class CMenuMindMap : public CMenuColor
{
public:
	CMenuMindMap();
	virtual ~CMenuMindMap();

	virtual void init(KView_PTR, int type);
	void click( kn_int, kn_int,KMessageMouse* );
protected:
	//	KTimer_PTR m_timer;
};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<CMenuMindMap> CMenuMindMap_PTR;
#endif

