//  File:        NProjectCalendar.cpp
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
#include "NProjectCalendar.h"


CNProjectBaseCalendar::CNProjectBaseCalendar():m_days_per_week(5)
	, m_hours_per_day(8)
	, m_from_time(9, 0, 0)
	, m_to_time(18, 0, 0)
	, m_rest_start(12, 0, 0)
	, m_rest_end(13, 0, 0)
{
}

CNProjectBaseCalendar::~CNProjectBaseCalendar()
{
}

//设置上班时间
void CNProjectBaseCalendar::setWorkingTime(const time_duration & time)
{
	m_from_time = time;
}

//设置下班时间
void CNProjectBaseCalendar::setClosingTime(const time_duration & time)
{
	m_to_time = time;
}

void CNProjectBaseCalendar::setWorkHours(kn_byte hours)
{
	if(hours <= 24)
	{
		m_hours_per_day = hours;
	}
}

kn_byte CNProjectBaseCalendar::getWorkingHoursPerDay() const
{
	return m_hours_per_day;
}

//获取上班时间
time_duration CNProjectBaseCalendar::getWorkingTime() const
{
    return m_from_time;
}

//获取下班时间
time_duration CNProjectBaseCalendar::getClosingTime() const
{
    return m_to_time;
}

//每周工作几天
void CNProjectBaseCalendar::setWorkDaysPerWeek(kn_byte days)
{
	if(days <= 7)
	{
		m_days_per_week = days;
	}
}

kn_byte CNProjectBaseCalendar::getWorkDaysPerWeek() const
{
	return m_days_per_week;
}

//获取午休开始时间
time_duration CNProjectBaseCalendar::getRestStartTime() const
{
	return m_rest_start;
}

//获取午休结束时间
time_duration CNProjectBaseCalendar::getRestEndTime() const
{
	return m_rest_end;
}

CNProjectExceptionCalendar::CNProjectExceptionCalendar(const CNProjectExceptionCalendar& other)
{
	m_day_type = other.getdayType();
	m_start_date = other.getStartDate();
	m_end_date = other.getEndDate();
	m_start_time = other.getStartTime();
	m_end_time = other.getEndTime();
	m_except_rule = other.getRule();
}

CNProjectExceptionCalendar::CNProjectExceptionCalendar():m_except_rule(0xFFFFFFFF)
{
}

kn_bool CNProjectExceptionCalendar::isValid()
{
    return (m_except_rule != 0xFFFFFFFF);
}

date CNProjectExceptionCalendar::getStartDate() const
{
    return m_start_date;
}

date CNProjectExceptionCalendar::getEndDate() const
{
    return m_end_date;
}

time_duration CNProjectExceptionCalendar::getStartTime() const
{
    return m_start_time;
}

time_duration CNProjectExceptionCalendar::getEndTime() const
{
    return m_end_time;
}

kn_int CNProjectExceptionCalendar::getRule() const
{
    return m_except_rule;
}

//获取工作日等类型
string CNProjectExceptionCalendar::getdayTypeString() const 
{
	string str_value;
	switch(m_day_type)
	{
	case WORK_DAY:
		str_value = "1";
		break;
	case OVERTIME_WORK:
		str_value = "2";
		break;		
	default:
		str_value = "0";
		break;
	}

	return str_value;
}

//获取工作日等类型
DAY_TYPE CNProjectExceptionCalendar::getdayType() const
{
    return m_day_type;
}

CNProjectExceptionCalendar::CNProjectExceptionCalendar(kn_byte byWeek/*1-7*/, DAY_TYPE dayType, time_duration s_time, time_duration end_time,  date start_date , date end_date)
{
    m_except_rule = 0xFFFFFFFF; //in valid

    if( (byWeek >= 1) && (byWeek <= 7) )
    {
        switch (dayType)
        {
        case HOLIDAY:
        case WORK_DAY:
        case OVERTIME_WORK:
            {
                m_except_rule = 1; //week
                m_except_rule |= (byWeek << 8);

                m_start_date = start_date;
                m_end_date = end_date;

                m_start_time = s_time;
                m_end_time = end_time;

                m_day_type = dayType;
            }
            break;
        default:
            break;
        }
    }
}

//每月多少号
CNProjectExceptionCalendar::CNProjectExceptionCalendar(kn_word byDate/*1-31*/, DAY_TYPE dayType, time_duration s_time, time_duration end_time, date start_date, date end_date)
{
    m_except_rule = 0xFFFFFFFF; //in valid
    if( (byDate >= 1) && (byDate <= 31) )
    {
        switch (dayType)
        {
        case HOLIDAY:
        case WORK_DAY:
        case OVERTIME_WORK:
            {
                m_except_rule = 2; //day
                m_except_rule |= (byDate << 8);

                m_start_date = start_date;
                m_end_date = end_date;
                m_start_time = s_time;
                m_end_time = end_time;

                m_day_type = dayType;
            }
            break;
        default:
            break;
        }
    }
}

//某个时间段
CNProjectExceptionCalendar::CNProjectExceptionCalendar(date &start_date, date &end_date, DAY_TYPE dayType, time_duration s_time, time_duration end_time )
{
    m_except_rule = 0xFFFFFFFF; //in valid
    if( !start_date.is_not_a_date() )
    {
        switch (dayType)
        {
        case HOLIDAY:
        case WORK_DAY:
        case OVERTIME_WORK:
            {
                m_except_rule = 0; //date

                m_start_date = start_date;
                m_end_date = end_date;
                m_start_time = s_time;
                m_end_time = end_time;

                m_day_type = dayType;
            }
            break;
        default:
            break;
        }
    }
}

 //是否为工作日
kn_bool CNProjectExceptionCalendar::isWorkDay() const
{
	return (m_day_type == WORK_DAY);
}

//当前日期是否在例外日历中
kn_bool CNProjectExceptionCalendar::inException(const ptime& time) const
{
	kn_bool _result ;
    date cur_date = time.date();

    if( !m_start_date.is_special() )
    {
        if( cur_date < m_start_date )
        {
            return false;
        }
    }

    if( !m_end_date.is_special() )
    {
        if( cur_date > m_end_date )
        {
            return false;
        }
    }

    time_duration tm = time.time_of_day();

    if ( m_start_time != time_duration() )
    {
        if( tm < m_start_time )
        {
            return false;
        }
    }

    if ( m_end_time != time_duration() )
    {
        if( tm > m_end_time )
        {
            return false;
        }
    }

    kn_byte rule = m_except_rule & 0xFF;
    switch (rule)
    {
    case 1: //week
    {
        int week = cur_date.day_of_week();
        if ( week == 0 )
        {
            week = 7;// 周日
        }

        _result = ( week == ((m_except_rule >> 8)& 0xFF) );
    }
    break;
    case 2: //day
        _result = ( cur_date.day() == ((m_except_rule >> 8)& 0xFF) );
        break;
    case 0: //date
        break;
    default:
        break;
    }

    return _result;
}

//当前日期是否在例外日历中
kn_bool CNProjectExceptionCalendar::inException(const date& dt) const
{
    kn_bool _result = true;

	if( !m_start_date.is_special() )
	{
		if( dt < m_start_date )
		{
			return false;
		}
	}

	if( !m_end_date.is_special() )
	{
		if( dt > m_end_date )
		{
			return false;
		}
	}

	kn_byte rule = m_except_rule & 0xFF;
	switch (rule)
	{
    case 1: //week
        {
            int week = dt.day_of_week();
            if ( week == 0 )
            {
                week = 7;// 周日
            }

           _result = ( week == ((m_except_rule >> 8)& 0xFF) );
        }
	    break;
    case 2: //day
        _result = ( dt.day() == ((m_except_rule >> 8)& 0xFF) );
        break;
    case 0: //date
        break;
	default:
	    break;
	}

	return _result;
}

CNProjectCalendar::CNProjectCalendar()
{
}

CNProjectCalendar::CNProjectCalendar(const CNProjectCalendar& other)
{
	m_from_time = other.getWorkingTime();
	m_to_time = other.getClosingTime();

	m_rest_start = other.getRestStartTime();
	m_rest_end = other.getRestEndTime();

	m_hours_per_day = other.getWorkingHoursPerDay();
	m_days_per_week = other.getWorkDaysPerWeek();

	const CNProjectCalendar_LST  * p_lists = other.getWorkingdays();
	if( NULL != p_lists )
	{
		copy(p_lists->begin(), p_lists->end(), back_inserter(m_lst_workingdays));
	}

	p_lists = other.getHolidays();
	if( NULL != p_lists )
	{
		copy(p_lists->begin(), p_lists->end(), back_inserter(m_lst_holiday));
	}

	p_lists = other.getOvertimeWorkings();
	if( NULL != p_lists )
	{
		copy(p_lists->begin(), p_lists->end(), back_inserter(m_lst_overtime));
	}
}

//是否为工作日
//说明：一天只上半天班，也认为当天是工作日(加班不算工作日)
kn_bool CNProjectCalendar::isWorkDay(const date & dt) const
{
    //优先判断例外日历
	CNProjectCalendar_LST::const_iterator it = m_lst_workingdays.begin();
	for(; it != m_lst_workingdays.end(); ++it)
	{
        if( it->inException(dt) )
        {
            return it->isWorkDay();
        }
	}

	it = m_lst_holiday.begin();
	for(; it != m_lst_holiday.end(); ++it)
	{
        if( it->inException(dt) )
        {
            return false;
        }
	}

	kn_bool _result = true;
	date::day_of_week_type dw = dt.day_of_week();
	if( (0 == dw)  || (6 == dw) )//周日，周六 默认为休息日，否则需要设置例外
    {
        _result = false;
    }

    return _result;
}

kn_byte CNProjectCalendar::getWorkingHours(const time_duration& frm_time, const time_duration& t_time) const
{
	time_duration start_time = frm_time;
	time_duration end_time = t_time;

    if( frm_time < getWorkingTime() )
    {
        start_time = getWorkingTime();
    }

    if( t_time > getClosingTime() ) //移去非工作时间
    {
        end_time = getClosingTime();
    }

    time_duration rest_frm_time = max(start_time, getRestStartTime() );
    time_duration rest_t_time = min(end_time, getRestEndTime() );

    time_duration wrk_time = end_time - start_time;

    if( rest_t_time > rest_frm_time ) //有交集
    {
        wrk_time -= (rest_t_time - rest_frm_time); //减去午休时间
    }

    kn_byte hours = wrk_time.hours();
    return hours;
}

//加班时间不计算在内，只计算工作时间
vector<kn_uint> CNProjectCalendar::getWorkHours(const ptime& from_time, const ptime& to_time ) const
{
	vector<kn_uint> vect_hours;

	if( (!from_time.is_not_a_date_time()) && (!to_time.is_not_a_date_time()) )
	{
		kn_uint hours(0);

		date from_date = from_time.date();
		date to_date = to_time.date();
		if ( (from_date == to_date) ) //一天之内
		{
			if( isWorkDay(from_date) && (to_time > from_time) )
			{
				hours = getWorkingHours(from_time.time_of_day(), to_time.time_of_day() );
			}
			vect_hours.push_back(hours);
		}
		else if(to_date > from_date)
		{
			if ( isWorkDay(from_date) ) //第一天
			{
				hours = getWorkingHours(from_time.time_of_day(), CNProjectBaseCalendar::getClosingTime() );
				vect_hours.push_back(hours);
			}
			else
			{
				vect_hours.push_back(0); //非工作日
			}

			boost::gregorian::days one_day( 1 );
			for(date dt = from_date + one_day; dt !=  to_date; dt += one_day )
			{
				if ( isWorkDay(dt) )
				{
					vect_hours.push_back( CNProjectBaseCalendar::getWorkingHoursPerDay() );
				}
				else
				{
					vect_hours.push_back(0); //非工作日
				}
			}

			if ( isWorkDay(to_date) ) //最后一天
			{
				hours = getWorkingHours(CNProjectBaseCalendar::getWorkingTime(), to_time.time_of_day() );
				vect_hours.push_back(hours);
			}
			else
			{
				vect_hours.push_back(0); //非工作日
			}
		}
	}

    return vect_hours;
}

kn_uint CNProjectCalendar::getWorkDays(const ptime& from_time, const ptime& to_time ) const
{
    kn_uint working_days(0);

    if( (!from_time.is_not_a_date_time()) && (!to_time.is_not_a_date_time()) )
    {
        date to_date = to_time.date();

		boost::gregorian::days one_day( 1 );

        for(date dt = from_time.date(); dt != to_date ;  dt += one_day)
        {
            if ( isWorkDay( dt ) )
            {
                ++working_days;
            }
        }
    }

    return working_days;
}

//将某天设置为工作日
void CNProjectCalendar::setWorkDay(/*const*/ date & dt)
{
	kn_bool bFind(false);
	CNProjectCalendar_LST::iterator it = m_lst_workingdays.begin();
	for(; it != m_lst_workingdays.end(); ++it)
	{
        if( it->inException(dt) )
        {
            bFind = true;
			break;
        }
	}

	if(!bFind)
	{
		CNProjectExceptionCalendar work_day(dt, dt, WORK_DAY);
		m_lst_workingdays.push_back(work_day);
	}

	it = m_lst_holiday.begin();
	for(; it != m_lst_holiday.end(); ++it)
	{
        if( it->inException(dt) )
        {
			m_lst_holiday.erase(it);
            break;
        }
	}
}

//将某天设置为假日
void CNProjectCalendar::setHoliday(/*const*/ date & dt)
{
	CNProjectCalendar_LST::iterator it = m_lst_workingdays.begin();
	for(; it != m_lst_workingdays.end(); ++it)
	{
        if( it->inException(dt) )
        {
			m_lst_workingdays.erase(it);
			break;
        }
	}

	kn_bool bFind(false);

	it = m_lst_holiday.begin();
	for(; it != m_lst_holiday.end(); ++it)
	{
        if( it->inException(dt) )
        {
			bFind = true;
			break;
        }
	}

	if(!bFind)
	{
		CNProjectExceptionCalendar holiday(dt, dt, HOLIDAY);
		m_lst_holiday.push_back(holiday);
	}
}

//添加例外规则(每周几)
kn_uint CNProjectCalendar::addException(kn_byte byWeek/*1-7*/, DAY_TYPE dayType, time_duration s_time, time_duration end_time,  date start_date , date end_date)
{
	kn_uint index(0);

    CNProjectExceptionCalendar exp(byWeek, dayType, s_time, end_time, start_date, end_date);
    if( exp.isValid() )
    {
        switch ( exp.getdayType() )
        {
        case HOLIDAY:
            m_lst_holiday.push_back(exp);
			index = m_lst_holiday.size() + HOLIDAY_START_ID;
            break;
        case WORK_DAY:
            m_lst_workingdays.push_back(exp);
			index = m_lst_workingdays.size() + WORKINGDAY_START_ID;
            break;
        case OVERTIME_WORK:
            m_lst_overtime.push_back(exp);
			index = m_lst_overtime.size() + OVERTIME_START_ID;
            break;
        default:
            break;
        }
    }

	return index;
}

//添加例外规则(//每月多少号)
kn_uint CNProjectCalendar::addException(kn_word byDate/*1-31*/, DAY_TYPE dayType, time_duration s_time, time_duration end_time, date start_date, date end_date )
{
	kn_uint index(0);

	CNProjectExceptionCalendar exp(byDate, dayType, s_time, end_time, start_date, end_date);
	if( exp.isValid() )
	{
		switch ( exp.getdayType() )
		{
		case HOLIDAY:
			m_lst_holiday.push_back(exp);
			index = m_lst_holiday.size() + HOLIDAY_START_ID;
			break;
		case WORK_DAY:
			m_lst_workingdays.push_back(exp);
			index = m_lst_workingdays.size() + WORKINGDAY_START_ID;
			break;
		case OVERTIME_WORK:
			m_lst_overtime.push_back(exp);
			index = m_lst_overtime.size() + OVERTIME_START_ID;
			break;
		default:
			break;
		}
	}

	return index;
}

//添加例外规则(//某个时间段)
kn_uint CNProjectCalendar::addException(date &start_date, date &end_date, DAY_TYPE dayType, time_duration s_time, time_duration end_time)
{
	kn_uint index(0);

	CNProjectExceptionCalendar exp(start_date, end_date, dayType, s_time, end_time);
	if( exp.isValid() )
	{
		switch ( exp.getdayType() )
		{
		case HOLIDAY:
			m_lst_holiday.push_back(exp);
			index = m_lst_holiday.size() + HOLIDAY_START_ID;
			break;
		case WORK_DAY:
			m_lst_workingdays.push_back(exp);
			index = m_lst_workingdays.size() + WORKINGDAY_START_ID;
			break;
		case OVERTIME_WORK:
			m_lst_overtime.push_back(exp);
			index = m_lst_overtime.size() + OVERTIME_START_ID;
			break;
		default:
			break;
		}
	}

	return index;
}

const CNProjectCalendar_LST * CNProjectCalendar::getWorkingdays() const
{
	return &m_lst_workingdays;
}

const CNProjectCalendar_LST * CNProjectCalendar::getHolidays() const
{
	return &m_lst_holiday;
}

const CNProjectCalendar_LST * CNProjectCalendar::getOvertimeWorkings() const
{
	return &m_lst_overtime;
}

kn_bool CNProjectCalendar::delException(kn_uint index)
{
	kn_bool _result = false;

	//每种例外设置个数不会超过1W条
	if( index > WORKINGDAY_START_ID &&   (index < (OVERTIME_START_ID + WORKINGDAY_START_ID) ) )
	{
		if(index < HOLIDAY_START_ID)
		{
			CNProjectCalendar_LST::iterator  iter = m_lst_workingdays.begin();
			std::advance(iter, index - WORKINGDAY_START_ID );
			m_lst_workingdays.erase(iter); //工作日
			_result = true;
		}
		else if(index < OVERTIME_START_ID)
		{
			CNProjectCalendar_LST::iterator  iter = m_lst_holiday.begin();
			std::advance(iter, index - HOLIDAY_START_ID );
			m_lst_holiday.erase(iter); //假期
			_result = true;
		}
		else
		{
			CNProjectCalendar_LST::iterator  iter = m_lst_overtime.begin();
			std::advance(iter, index - OVERTIME_START_ID );
			m_lst_overtime.erase(iter); //加班
			_result = true;
		}
	}

	return _result;
}

void CNProjectCalendar::clearAllException()
{
	m_lst_workingdays.clear();
	m_lst_holiday.clear();
	m_lst_overtime.clear();
}


