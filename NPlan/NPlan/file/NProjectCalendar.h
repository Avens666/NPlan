//  File:        NProjectCalendar.h
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
#ifndef _NProject_Calendar_h__
#define _NProject_Calendar_h__

#include "NE_type.h"
#include "utility.h"

#define WORKINGDAY_START_ID (10000u)
#define HOLIDAY_START_ID (WORKINGDAY_START_ID*2)
#define OVERTIME_START_ID (HOLIDAY_START_ID*3)

using namespace boost:: posix_time;
using namespace boost::gregorian;

typedef enum tagDayType{
    HOLIDAY = 0,      //假期
    WORK_DAY,     //工作日
    OVERTIME_WORK //加班
} DAY_TYPE;


//工作日日历 每周从周1开始
class CNProjectBaseCalendar
{
public:
	CNProjectBaseCalendar();

	virtual ~CNProjectBaseCalendar();

	void setWorkingTime(const time_duration & time); //设置上班时间
	void setClosingTime(const time_duration & time); //设置下班时间
	void setWorkHours(kn_byte hours);
	void setWorkDaysPerWeek(kn_byte days); //每周工作几天

	time_duration getWorkingTime() const;  //获取上班时间
	time_duration getClosingTime() const;  //获取下班时间
	kn_byte getWorkingHoursPerDay() const;
	kn_byte getWorkDaysPerWeek() const;

	time_duration getRestStartTime() const;//获取午休开始时间
	time_duration getRestEndTime() const;  //获取午休结束时间
protected :
	time_duration m_from_time; //工作日上班时间
	time_duration m_to_time;  //工作日下班时间

	time_duration m_rest_start; //午休开始时间
	time_duration m_rest_end;   //午休结束时间

	kn_byte m_hours_per_day;  //每天工作小时数
	kn_byte m_days_per_week; //每周工作几天
};

class CNProjectExceptionCalendar // 例外日历
{
public:
	CNProjectExceptionCalendar();
	//(每周几)
	CNProjectExceptionCalendar(kn_byte byWeek/*1-7*/, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration(), date start_date = date(not_a_date_time), date end_date  = date(not_a_date_time) );
	//每月多少号
	CNProjectExceptionCalendar(kn_word byDate/*1-31*/, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration(), date start_date = date(not_a_date_time), date end_date = date(not_a_date_time) );
	//某个时间段
	CNProjectExceptionCalendar(date &start_date, date &end_date, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration() );

	CNProjectExceptionCalendar(const CNProjectExceptionCalendar& other);
	kn_bool isValid();
	kn_bool isWorkDay() const ; //是否为工作日
	kn_bool inException(const ptime& time) const ;	//当前日期是否在例外日历中
	kn_bool inException(const date& dt) const ;		//当前日期是否在例外日历中

	date getStartDate() const ;
	date getEndDate() const ;
	time_duration getStartTime() const ;
	time_duration getEndTime() const ;
	DAY_TYPE getdayType() const ; //获取工作日等类型
	string getdayTypeString() const ; //获取工作日等类型
	kn_int getRule() const ;
private:
    date m_start_date;			//起始日期
    date m_end_date;
    time_duration m_start_time; //起始时间
    time_duration m_end_time;
    DAY_TYPE m_day_type;		//假期 0 工作日 1 加班 2
    kn_int m_except_rule;		//例外规则   bit0-bit7: rule type, bit8-bit15:week or date,
};

#define CNProjectCalendar_LST std::list<CNProjectExceptionCalendar>

//项目日历
class CNProjectCalendar : public CNProjectBaseCalendar
{
public:
	CNProjectCalendar();
	CNProjectCalendar(const CNProjectCalendar& other);

	kn_bool isWorkDay(const date & dt) const ; //是否为工作日

	void setWorkDay(/*const*/ date & dt); //将某天设置为工作日

	void setHoliday(/*const*/ date & dt); //将某天设置为假日

	kn_uint getWorkDays(const ptime& from_time, const ptime& to_time ) const ;

	//加班时间不计算在内，只计算工作时间（导出Project时需要使用）
	vector<kn_uint> getWorkHours(const ptime& from_time,  const ptime& to_time ) const ;

	const CNProjectCalendar_LST * getWorkingdays() const;

	const CNProjectCalendar_LST * getHolidays() const ;

	const CNProjectCalendar_LST * getOvertimeWorkings() const ;

	//添加例外规则(每周几)
	kn_uint addException(kn_byte byWeek/*1-7*/, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration(), date start_date = date(not_a_date_time), date end_date  = date(not_a_date_time) );
	//添加例外规则(//每月多少号)
	kn_uint addException(kn_word byDate/*1-31*/, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration(), date start_date = date(not_a_date_time), date end_date = date(not_a_date_time) );
	//添加例外规则(//某个时间段)
	kn_uint addException(date &start_date, date &end_date, DAY_TYPE dayType, time_duration s_time = time_duration(), time_duration end_time = time_duration() );
	//删除例外
	kn_bool delException(kn_uint index);
	//清除所有例外
	void clearAllException();

	//TODO:单双休，单周上5.5天，双周上5天工作制
private:
    kn_byte getWorkingHours(const time_duration& frm_time, const time_duration& t_time) const ;

private:
	std::list<CNProjectExceptionCalendar> m_lst_workingdays; //工作日（通常的周1-周5的工作日无需在此设置，在基础日历中，此处特指十一调休的休息日等）

	std::list<CNProjectExceptionCalendar> m_lst_holiday;  //假期

	std::list<CNProjectExceptionCalendar> m_lst_overtime; //加班
};

#endif //_NProject_Calendar_h__
