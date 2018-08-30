//  File:        TimeLine.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/16
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#pragma once
//时间轴数据类  陈治 2013.007.28
#include <boost/date_time/posix_time/posix_time.hpp>
#include "DataSync.h"
#include "KDrawable.h"
#include "K9PatchImageDrawable.h"
#include "../file/NProjectCalendar.h"
using namespace boost::posix_time;
using namespace boost::gregorian;

#define YEAR_DAYS 365

#define TIMELINE_LEVEL1_YEAR  0
#define TIMELINE_LEVEL1_MONTH 1
#define TIMELINE_LEVEL1_DAY   2

kn_string getMonthString(const date &d);
kn_string getTimeString(const ptime &p,bool b_hour=TRUE);
//CTimeLine用于吧时间和像素关联起来
class CTimeLineDrawable :public KDrawable
{
public:
	CTimeLineDrawable(void);
	~CTimeLineDrawable(void);

	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);

	
	//设置时间轴比例长度像素尺寸, 一天长的像素单位*100
	void setTimelineOneDayWidth(int v, int base_x = 0);
	int getTimelineOneDayWidth();

	//根据时间段 相对于显示宽度尺寸 返回显示比例, 
	int getTimeDurationWidth(ptime &start, ptime &end);

	//移动时间轴，像素坐标,可正可负 左右移动
	void offsetTimeLine(int v);
	//获得时间轴相对标准点的偏移，像素单位
	int getTimeLineOffset();
	//设置时间轴相对标准点的偏移，像素单位
	void setTimeLineOffset(int v);

	//设置时间块区域，只有top和height有效
	void setTimeArea(RERect );

	//根据时间返回时间轴像素坐标
    float getTimePosition(const ptime &p);
	//根据时间点判断在时间轴上的显示级别 0 1 2
	int getTimelineLevel(ptime &p);

	//根据像素坐标返回时间
	ptime getPositisonTime(int x);

	//根据像素坐标返回时间,增加根据显示比例增加舍入操作
	ptime getPositisonTimeInt(int x , int step_hour); //按天返回时，小时偏移

	void setHourType(int v);

	int getDrawMode();
	//
	ptime getTimeLineBeginTime();
	////二级坐标推进
	ptime goTimeLine(ptime &p);

	kn_string getLevelString(ptime &p, int level);
	
	//获取1级刻度步长
	int getLevel1Length();

	void setCalendar(CNProjectCalendar* p);

protected:
	K9PatchImageDrawable_PTR m_date_bk;
	boost::shared_mutex m_draw_flag_mutex;

	int TIMELINE_LEVEL1	;
	int TIMELINE_LEVEL2	;
	int TIMELINE_LEVEL3	;
	int TIMELINE_LEVEL4 ;

	REColor m_bk_color[2];

	RERect m_time_area_rect;

//	int m_min_unit_width; //
	//坐标轴刻度分为三级,
	//最小一级刻度长度 像素
//	int m_level_one_min_unit_width; //200
	//最小二级刻度长度
	int m_level_two_min_unit_width; //50
	//最小三级刻度长度
	int m_level_three_min_unit_width; //10

	//一级刻度高度
	int m_level_unit_height[3]; //


	//一天长的像素单位*100,根据此值决定当前比例级别和各级刻度显示方式
	int m_timeline_one_day_width;


	//基准时间，标准点,基准时间用于确定基本坐标位置，其他时间根据此位置进行相对计算，
	//基准时间为运行时类构造时的系统当前日期,逻辑位置为像素X坐标0点
	ptime m_base_time;

	//时间轴相对于标准点的偏移
	float m_timeline_offset;

	//小时显示的类型  24小时还是工作时间（12小时）
	int m_hour_type;

	//二级步长时间跨度 （由于每个月和每年的天数不同，同常规的坐标刻度不同，每个月可能长度不一样，需要按时间步进）
	int m_time_step_m;
	days	m_time_step_d;
	time_duration m_time_step_h;
	//根据坐标轴缩放程度计算出的绘制模式
	int m_draw_mode; 
	//一级坐标是年 月 还是日
	int m_level1_type;
	//一个二级坐标内部的三级坐标数量
	int m_level3_num;

	CNProjectCalendar* m_p_prj_calendar;

};

typedef boost::shared_ptr<CTimeLineDrawable> CTimeLineDrawable_PTR;
