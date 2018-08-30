//  File:        TimeLine.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/23
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "TimeLine.h"
#include "KView.h"
#include "boost/lexical_cast.hpp"
#include "../manager/KStringManager.h"
#include "../manager/KConfigManager.h"
#include "KSurfaceManager.h"
#include "KFontManager.h"
using namespace std;
using namespace boost;
using namespace boost::gregorian;

void SetPaintEncodingAuto(REPaint& paint);

kn_string getMonthName(int m)
{
	kn_string str;
	switch(m)
	{
		case 1:
			str =_T("Jan");
			break;
		case 2:
			str =_T("Feb");
			break;
		case 3:
			str =_T("Mar");
			break;
		case 4:
			str =_T("Apr");
			break;
		case 5:
			str =_T("May");
			break;
		case 6:
			str =_T("Jun");
			break;
		case 7:
			str =_T("Jul");
			break;
		case 8:
			str =_T("Aug");
			break;
		case 9:
			str =_T("Sep");
			break;
		case 10:
			str =_T("Oct");
			break;
		case 11:
			str =_T("Nov");
			break;
		case 12:
			str =_T("Dec");
			break;
	}
	return str;
}

kn_string getDayOfWeek(date& d)
{
	kn_string str;
	if (d.is_not_a_date() || d.is_neg_infinity())
	{
		return str;
	}
	date::day_of_week_type dw = d.day_of_week();
	switch(dw)
	{
	case 0:
		str =getStringManager()->GetStringByID(_T("week0"));
		break;
	case 1:
		str =getStringManager()->GetStringByID(_T("week1"));
		break;
	case 2:
		str =getStringManager()->GetStringByID(_T("week2"));
		break;
	case 3:
		str =getStringManager()->GetStringByID(_T("week3"));
		break;
	case 4:
		str =getStringManager()->GetStringByID(_T("week4"));
		break;
	case 5:
		str =getStringManager()->GetStringByID(_T("week5"));
		break;
	case 6:
		str =getStringManager()->GetStringByID(_T("week6"));
		break;
	}
	return str;
}


kn_string getDayOfWeekAll(const date& d)
{
	kn_string str;
	if (d.is_not_a_date() || d.is_neg_infinity())
	{
		return str;
	}
	date::day_of_week_type dw = d.day_of_week();
	switch(dw)
	{
	case 0:
		str =getStringManager()->GetStringByID(_T("all_week0"));
		break;
	case 1:
		str =getStringManager()->GetStringByID(_T("all_week1"));
		break;
	case 2:
		str =getStringManager()->GetStringByID(_T("all_week2"));
		break;
	case 3:
		str =getStringManager()->GetStringByID(_T("all_week3"));
		break;
	case 4:
		str =getStringManager()->GetStringByID(_T("all_week4"));
		break;
	case 5:
		str =getStringManager()->GetStringByID(_T("all_week5"));
		break;
	case 6:
		str =getStringManager()->GetStringByID(_T("all_week6"));
		break;
	}
	return str;
}

kn_string getMonthString(const date &d)
{
	kn_string str;
	int m= d.month();
	//str = lexical_cast<kn_string>(d.month()); 
	switch (getConfigManager()->getLanguage())
	{
	case LAN_CN:
		str = lexical_cast<kn_string>(m); 
		str += _T("月");
		break;
	default:
		str = getMonthName(m); 
		break;
	}


	return str;
}

kn_string getTimeString(const ptime &p, bool b_hour)
{
	std::string  strtime = to_simple_string(p);
	kn_string str = _T("No date");

	if (p.is_not_a_date_time())
	{
		return str;
	}

	date d = p.date();
	time_duration td = p.time_of_day(); 
	int m = d.month();

	str = getMonthString(d);
	if (getConfigManager()->getLanguage() == LAN_CN)
	{
		str += lexical_cast<kn_string>(d.day());
		str += _T("日");
	}
	else
	{
		str += _T(".");
		str += lexical_cast<kn_string>(d.day());
	}

	int hour =  td.hours() ;

	if (hour > 0 && b_hour)
	{
		str += _T(" ");
		str += lexical_cast<kn_string>( hour); 
		str += _T("h");
	}
	return str;
}



CTimeLineDrawable::CTimeLineDrawable(void)
{
	m_b_clip_rect = FALSE;
	m_p_prj_calendar = NULL;
	//获取当前日期作为基准时间
	ptime pt(second_clock::local_time());  
	date d1 = pt.date();  
	ptime t1(d1);
	m_base_time = t1;
	m_timeline_one_day_width = 96;
	//m_min_unit_width = 20;
	m_hour_type = 12;
	//m_level_one_min_unit_width = 200;
	//最小二级刻度长度
	m_level_two_min_unit_width = 30;
	//最小三级刻度长度
	m_level_three_min_unit_width = 5;

	TIMELINE_LEVEL1  = 4*m_level_two_min_unit_width*100/YEAR_DAYS;
	TIMELINE_LEVEL2  = TIMELINE_LEVEL1*3;
	TIMELINE_LEVEL3  = m_level_two_min_unit_width*100;
	TIMELINE_LEVEL4 = TIMELINE_LEVEL3*m_hour_type/2;

	//一级刻度高度
	m_level_unit_height[0] = 5; 
	//二级刻度高度
	m_level_unit_height[1] = 5; 
	//三级刻度高度
	m_level_unit_height[2] = 5; 

	//时间轴相对于标准点的偏移
	m_timeline_offset = -100;
	setTimelineOneDayWidth(TIMELINE_LEVEL2/2);
	m_time_step_m = 0;
	m_time_step_d = days(0);
	m_time_step_h = hours(0);

	//m_paint.setTypeface(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	SetPaintEncodingAuto(m_paint);


	m_paint.setTextSize(14);
	m_paint.setTextAlign(SkPaint::kCenter_Align);

	m_time_area_rect.setXYWH(0,120, 100, 500);

	m_bk_color[0] = SkColorSetARGB(255, 240,240,240);
	m_bk_color[1] = SkColorSetARGB(255, 220,220,220);

	m_date_bk = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/date_bk.9.png")),true));
	m_date_bk->setRect(0,0,100, 23);
}

CTimeLineDrawable::~CTimeLineDrawable(void)
{

}

void CTimeLineDrawable::setTimeArea(RERect r)
{
	m_time_area_rect = r;
}

int CTimeLineDrawable::getTimeDurationWidth(ptime &start, ptime &end)
{
//	std::string  str = to_simple_string(end);
	if (start.is_not_a_date_time() || end.is_not_a_date_time() )
	{
		return TIMELINE_LEVEL2;
	}
	time_duration du = end - start;
	int h = du.hours();
	
	return 2400* (m_rect.width()-50) / h;	
}

//设置时间轴比例长度像素尺寸,一天长的像素单位*100
void CTimeLineDrawable::setTimelineOneDayWidth(int v, int base_x)
{
	writeLock lock(m_draw_flag_mutex);

	//int x = base_x - m_rect.left();
	float x = m_rect.width()/2;
	//矫正v
	v = max( min(TIMELINE_LEVEL4, v), TIMELINE_LEVEL1);
	m_timeline_offset = x - (x - m_timeline_offset)*v/m_timeline_one_day_width;

	//先统一清零
	m_time_step_m = (0);
	m_time_step_d = days(0);
	m_time_step_h = hours(0);

	if (v <= TIMELINE_LEVEL2 /2) //1年显示4个月（一级坐标为年，二级坐标步长为3个月）
	{
		m_draw_mode = TIMELINE_LEVEL1_YEAR;
		m_time_step_m = (3);
		m_level3_num = 3;
	}
	else if (v < TIMELINE_LEVEL2 ) //一年显示6个月（一级坐标为年，二级坐标步长为2个月）
	{
		m_draw_mode = TIMELINE_LEVEL1_YEAR;
		m_time_step_m = (2);
		m_level3_num = 2;
	}
	else if (v < TIMELINE_LEVEL2*3 ) //一年显示12个月（一级坐标为年，二级坐标步长为1个月）
	{
		m_draw_mode = TIMELINE_LEVEL1_YEAR;
		m_time_step_m = (1);
		m_level3_num = 3;

	}
	else if (v < TIMELINE_LEVEL2*4 ) //一级刻度变为月，一月显示3个二级刻度（一级坐标为月，二级坐标步长为10天)
	{
		m_draw_mode = TIMELINE_LEVEL1_MONTH;
		m_time_step_d = days(10);	
		m_level3_num = 5;
	}
	else if (v < TIMELINE_LEVEL2*10 ) //一级刻度变为月，一月显示6个二级刻度（一级坐标为月，二级坐标步长为5天)
	{
		m_draw_mode = TIMELINE_LEVEL1_MONTH;
		m_time_step_d = days(5);
		m_level3_num = 5;
	}
	else if (v < TIMELINE_LEVEL3/2 ) //一级刻度变为月，一月显示10个二级刻度（一级坐标为月，二级坐标步长为3天)
	{
		m_draw_mode = TIMELINE_LEVEL1_MONTH;
		m_time_step_d = days(3);

		m_level3_num = 3;
	}
	else if (v < TIMELINE_LEVEL3 ) //一级刻度变为月，一月显示10个二级刻度（一级坐标为月，二级坐标步长为3天)
	{
		m_draw_mode = TIMELINE_LEVEL1_MONTH;
		m_time_step_d = days(2);

		m_level3_num = 2;
	}
	else if (v < TIMELINE_LEVEL3*1.8 ) //（一级坐标为月，二级坐标步长为1天)
	{
		m_draw_mode = TIMELINE_LEVEL1_MONTH;
		m_time_step_d = days(1);
		m_level3_num = 1;
	}
	else if (v < TIMELINE_LEVEL3*6 *m_hour_type/24) //（一级坐标为天，二级坐标步长为12小时)
	{
		m_draw_mode = TIMELINE_LEVEL1_DAY;
		m_time_step_h = hours(12*m_hour_type/24);
		m_level3_num = 2;
	}
	else if (v < TIMELINE_LEVEL3*11 *m_hour_type/24) //（一级坐标为天，二级坐标步长为6小时)
	{
		m_draw_mode = TIMELINE_LEVEL1_DAY;
		m_time_step_h = hours(6*m_hour_type/24);
		m_level3_num = 2;
	}
	//else if (v < TIMELINE_LEVEL3*10 *m_hour_type/24) //（一级坐标为天，二级坐标步长为4小时)
	//{
	//	m_draw_mode = TIMELINE_LEVEL1_DAY;
	//	m_time_step_h = hours(4);
	//	m_level3_num = 4;
	//}
	else if (v < TIMELINE_LEVEL3*12 *m_hour_type/24) //（一级坐标为天，二级坐标步长为3小时)
	{
		m_draw_mode = TIMELINE_LEVEL1_DAY;
		m_time_step_h = hours(3);
		m_level3_num = 3;
	}
	else if (v < TIMELINE_LEVEL3*m_hour_type ) //（一级坐标为天，二级坐标步长为2小时)
	{
		m_draw_mode = TIMELINE_LEVEL1_DAY;
		m_time_step_h = hours(2);
		m_level3_num = 2;
	}
	else if (v = TIMELINE_LEVEL3*m_hour_type ) //（一级坐标为天，二级坐标步长为1小时)
	{
		m_draw_mode = TIMELINE_LEVEL1_DAY;
		m_time_step_h = hours(1);
		m_level3_num = 1;
	}

	m_timeline_one_day_width = v;
	
}

int CTimeLineDrawable::getTimelineOneDayWidth()
{
	return m_timeline_one_day_width;
}

//移动时间轴，像素坐标,可正可负 左右移动
void CTimeLineDrawable::offsetTimeLine(int v)
{
//	writeLock lock(m_draw_flag_mutex);
	m_timeline_offset += v;
}

//获得时间轴相对标准点的偏移，像素单位
int CTimeLineDrawable::getTimeLineOffset()
{
	return m_timeline_offset;
}

//设置时间轴相对标准点的偏移，像素单位
void CTimeLineDrawable::setTimeLineOffset(int v)
{
	m_timeline_offset = v;
}

//根据时间返回时间轴像素坐标
float CTimeLineDrawable::getTimePosition(const ptime &p)
{
	//相差的天数
	days d = p.date() - m_base_time.date();

	time_duration td = p.time_of_day();  

	//计算小时长度基准
	int h ;
	if (m_hour_type == 24)
	{
		h = td.hours() ;
	}
	else
	{//工作时间8:00--20:00  0<=h<=12
		h = max( min(td.hours() - 8, 12), 0);
	}
	//相差天数转成像素长度
	float day_width = d.days()*m_timeline_one_day_width/100.0 + h*m_timeline_one_day_width/(m_hour_type*100.0) ;
	 
	
	return m_timeline_offset + day_width;
}

ptime CTimeLineDrawable::getPositisonTimeInt(int x, int step_hour)
{
	ptime pt = getPositisonTime(x);
	std::string  str = to_simple_string(pt);
	if (m_draw_mode == TIMELINE_LEVEL1_DAY)
	{
		return pt;
	}
	else
	{//以天对齐
		date d = pt.date();
		ptime p1(date(d.year(), d.month(), d.day()), time_duration(step_hour, 0, 0));

		std::string str2 = to_simple_string(p1);
		return p1;
	}
}


//根据像素坐标返回时间
ptime CTimeLineDrawable::getPositisonTime(int x)
{
	//同标准点距离，转为天数 小数
	double n_days = (double)(x - m_timeline_offset) *100 / (m_timeline_one_day_width);
	int i_days = n_days;

	boost::gregorian::days d(i_days);
	
	int h;

	if (m_hour_type == 24)
	{
		h = (n_days - int(n_days))*24;
	}
	else
	{//工作时间8:00--20:00  0<=h<=12

		
		if (n_days < 0)
		{
			h = (n_days - int(n_days))*12;
			h -= 4;
		}
		else
		{
			h = (n_days - int(n_days))*12;
			h += 8;
		}

	}
	string strt = to_simple_string(m_base_time);
	ptime t;


//	MyTrace(_T("NUI m_timeline_one_day_width = %d, m_timeline_offset= %f"),m_timeline_one_day_width, m_timeline_offset);
	 
		t = m_base_time + d + time_duration(h, 0, 0);
	strt = to_simple_string(t);
	return t;
}

void CTimeLineDrawable::setHourType(int v)
{
	m_hour_type = v;
	TIMELINE_LEVEL4 = TIMELINE_LEVEL3*m_hour_type;
}

int CTimeLineDrawable::getDrawMode()
{
	return m_draw_mode;
}

//根据时间点判断在时间轴上的显示级别 0 1 
int CTimeLineDrawable::getTimelineLevel(ptime &p)
{
	date d = p.date();
	time_duration td = p.time_of_day();
	if (m_draw_mode == TIMELINE_LEVEL1_YEAR)
	{
		if (d.month() == 1 &&d.day() == 1 && td.hours() == 0)
		{
			return 0;
		}
		else 
		{
			return 1;
		}		
	}

	if (m_draw_mode == TIMELINE_LEVEL1_MONTH)
	{
		if (d.day() == 1 && td.hours() == 0)
		{
			return 0;
		}
		else 
		{
			return 1;
		}		
	}

	if (m_draw_mode == TIMELINE_LEVEL1_DAY)
	{
		if ( (m_hour_type ==24 && td.hours() == 0) ||(m_hour_type ==12 && td.hours() == 8)  )
		{
			return 0;
		}
		else 
		{
			return 1;
		}		
	}

	return 0;
}

void CTimeLineDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
//	writeLock lock(m_draw_flag_mutex);
	//if(pDstSurface == NULL) return;
	//if (m_b_show == FALSE)  return;
	ptime pt = getTimeLineBeginTime();

	if (pt.is_infinity())
	{//出错了
		return;
	}
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}
	//pDstSurface->Save();
	//if (pMatrix)
	//{
	//	pDstSurface->Concat(*pMatrix);
	//}

	//if(m_b_clip)
	//{
	//	pDstSurface->ClipRect(m_rect_clip);
	//}

	RERect rectDst = RERect::MakeXYWH(iDstX, iDstY, m_rect.width(), m_rect.height()+m_time_area_rect.top()+m_time_area_rect.height());
	pDstSurface->ClipRect(rectDst);

	iDstX +=m_rect.left();
	////////////// Draw Time Line ///////////////
	
//	m_paint.setARGB(255,50,50,50);
//	pDstSurface->DrawRect(RERect::MakeXYWH(iDstX , m_rect.top() + iDstY, m_rect.width(), m_rect.height()), m_paint );
	m_paint.setARGB(255,250,250,250);
	int bottom = iDstY + m_rect.height() -5; //时间轴线Y位置
	pDstSurface->DrawLine(iDstX, bottom, iDstX + m_rect.width(), bottom, m_paint);

	m_paint.setARGB(255,240,240,240);
	pDstSurface->DrawLine(iDstX + m_rect.width()/2, bottom, iDstX + m_rect.width()/2, bottom+5, m_paint);
//	pDstSurface->DrawCircle(iDstX + m_rect.width()/2, bottom, 5, m_paint  );



	ptime pt_next;
	//起始位置，一般为负数
	float x= getTimePosition(pt);
	int level;
	float last_x, last_level1_x;
	int clr, bw;
	int count = 0; //遍历计数 遍历是以二级坐标推进
	kn_string str;
	last_level1_x = 0; clr = 0;
	
	if (m_draw_mode == TIMELINE_LEVEL1_DAY)
	{
		pDstSurface->DrawLine(iDstX, bottom - m_level_unit_height[0]-40, iDstX + m_rect.width(), bottom - m_level_unit_height[0]-40, m_paint);
		pDstSurface->DrawLine(iDstX, bottom - m_level_unit_height[0]-16, iDstX + m_rect.width(), bottom - m_level_unit_height[0]-16, m_paint);
	}
	
	int level1_txt_y = bottom - 34;
	date d;
	ptime pt2; //向前推进的时间
	int x2; //向前推进的下一点x坐标
	int txt_w;
	BOOL b_holiday = FALSE;
	while(count <1000) //防止死循环
	{
		string strtmp = to_simple_string(pt);
		level = getTimelineLevel(pt);
		m_paint.setARGB(255,250,250,250);
		b_holiday = FALSE;
		//二级坐标提前推进，计算当前格子距离
		pt2 = goTimeLine(pt);
		x2= getTimePosition(pt2);

		if (x > -m_level_two_min_unit_width - getLevel1Length() && x < m_rect.width() + m_level_two_min_unit_width)
		{//画刻度

			if( (m_draw_mode == TIMELINE_LEVEL1_MONTH && m_level3_num == 1) || m_draw_mode == TIMELINE_LEVEL1_DAY )
			{
				d = pt.date();

				if (m_p_prj_calendar)
				{
					b_holiday = !m_p_prj_calendar->isWorkDay(d);
				}
				else if (d.day_of_week() == 0 || d.day_of_week() == 6 )
				{
					b_holiday = TRUE;
				}

			}

			//if (level == 0)
			//{
			//	m_paint.setStrokeWidth(2);
			//}
			//else
			{
				m_paint.setStrokeWidth(1);
			}

			//画刻度
			if(m_draw_mode == TIMELINE_LEVEL1_MONTH &&	m_level3_num == 1)
			{//月模式日期全展开 增加星期

				level1_txt_y = bottom - 38;
				pDstSurface->DrawLine(iDstX+x, bottom, iDstX+x, bottom - 31, m_paint);
			}
			else
			{
				level1_txt_y = bottom - 26;
				pDstSurface->DrawLine(iDstX+x, bottom, iDstX+x, bottom - m_level_unit_height[level], m_paint);
			}

			m_paint.setStrokeWidth(1);

			str = getLevelString(pt, level);
			if (level == 0)
			{//绘制一级文字
				if (m_draw_mode == TIMELINE_LEVEL1_DAY)
				{//一级文字背景
					pDstSurface->DrawLine(iDstX+x, bottom - m_level_unit_height[level]-40, iDstX+x, bottom - m_level_unit_height[level]-16, m_paint);
					if (b_holiday)
					{
						m_paint.setARGB(255,182,100,135);
					}
					else
					{
						m_paint.setARGB(255,82,127,168);
					}
					pDstSurface->DrawRect(RERect::MakeXYWH(iDstX+x+1, bottom - m_level_unit_height[level]-40+1, getLevel1Length(), 23), m_paint);
					m_paint.setARGB(255,250,250,250);
				}
				else
				{//背景图标
					bw = m_paint.measureText(str.c_str(),str.length() * sizeof(kn_char)) + 18;
					bw = max(bw, 30);
					m_date_bk->setRect( iDstX + x + getLevel1Length()/2 - bw/2, level1_txt_y - 18, bw,23);
					m_date_bk->Draw(pDstSurface);
				}
				//一级文字
				pDstSurface->DrawText( str,iDstX + x + getLevel1Length()/2 , level1_txt_y-1, m_paint );

			}

			////第一个二级刻度也画 二级文字
			str = getLevelString(pt, level+1);
			if(m_draw_mode == TIMELINE_LEVEL1_MONTH &&	m_level3_num == 1)
			{//月模式日期全展开 增加星期
				if (b_holiday)
				{
					m_paint.setARGB(255,182,100,135);
				}
				else
				{
					m_paint.setARGB(255,82,127,168);
				}
				pDstSurface->DrawRect(RERect::MakeXYWH(iDstX+x+1, level1_txt_y + 7, (x2 - x), bottom - 7 - level1_txt_y), m_paint);
				m_paint.setARGB(255,250,250,250);

				
				pDstSurface->DrawText( str,	 iDstX + x + (x2 - x)/2+1 , bottom - 18, m_paint );
				
				txt_w = m_paint.getTextSize();
				m_paint.setTextSize(12);
				str = getDayOfWeek(d);
				
				pDstSurface->DrawText( str,iDstX + x + (x2 - x)/2+1 , bottom - 4, m_paint );
				m_paint.setTextSize(txt_w); // 还原
				//上面一条线
				pDstSurface->DrawLine(iDstX, level1_txt_y +8, iDstX + m_rect.width(), level1_txt_y +7, m_paint);

			}
			else
			{
				pDstSurface->DrawText( str,iDstX + x, bottom - m_level_unit_height[level+1]-2, m_paint );
			}

			//if (count > 0 && (x-last_x )/m_level3_num > 2)
			//{//绘制三级刻度
			//	for (int i = 1; i < m_level3_num; i++)
			//	{
			//		pDstSurface->DrawLine(iDstX + x - (x-last_x)*i/m_level3_num, bottom, iDstX + x- (x-last_x)*i/m_level3_num, bottom - m_level_unit_height[2], m_paint);
			//	}
			//}

			//Draw area
			m_paint.setColor(m_bk_color[clr%2]);
			RERect rectbk = RERect::MakeXYWH(iDstX+ x , m_time_area_rect.top() + iDstY, x2-x, m_time_area_rect.height());
			pDstSurface->DrawRect( rectbk, m_paint );
			if (b_holiday)
			{
				m_paint.setARGB(90,97,37,92);
				pDstSurface->DrawRect( rectbk, m_paint );
			}

		//		last_level1_x = x;

		}
//		m_paint.setColor(m_bk_color[clr%2]);
//		pDstSurface->DrawRect(RERect::MakeXYWH(iDstX+ last_level1_x , m_time_area_rect.top() + iDstY, iDstX+m_rect.width(), m_time_area_rect.height() ), m_paint );

		clr++;

		if (x >  m_rect.width())
		{//绘制区域绘制完成
			break;
		}
		//二级坐标推进
		pt = pt2;

		last_x = x;
		x= x2;
		count++;
	}

//	m_paint.setARGB(200,255,255,255);
//	pDstSurface->DrawRect(RERect::MakeXYWH(iDstX, m_time_area_rect.top() + iDstY, m_time_area_rect.width(),30 ), m_paint );
	////////////////////////////////////////////////
	//pDstSurface->Restore();
	EndDraw(pDstSurface, pMatrix);
}


int  CTimeLineDrawable::getLevel1Length()
{
	if (m_draw_mode ==TIMELINE_LEVEL1_YEAR)
	{
		return 365*m_timeline_one_day_width/100;
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_MONTH)
	{
		return 30*m_timeline_one_day_width/100;
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_DAY)
	{
		return m_timeline_one_day_width/100;
	}

	return 365*m_timeline_one_day_width/100;
}


kn_string CTimeLineDrawable::getLevelString(ptime &p,int level)
{
	kn_string str;
	std::stringstream ss;
	date d =p.date();

	if (m_draw_mode ==TIMELINE_LEVEL1_YEAR)
	{
		if (level == 0)
		{
			ss << d.year();
			str = lexical_cast<kn_string>(d.year()); 
		}
		else
		{
			str = getMonthString(d);
		}
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_MONTH)
	{
		if (level == 0)
		{
			str = lexical_cast<kn_string>(d.year()); 
			if (getConfigManager()->getLanguage() == LAN_CN)
			{
				str += _T("年");
			}
			else
			{
				str += _T(".");
			}
			str += getMonthString(d);
		}
		else
		{
			str = lexical_cast<kn_string>(d.day()); 
		//	ss << d.day();
		}
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_DAY)
	{
		if (level == 0)
		{
			int m = d.month();
			str += getMonthString(d);
			if (getConfigManager()->getLanguage() == LAN_CN)
			{
				str += lexical_cast<kn_string>(d.day());
				str += _T("日");
			}
			else
			{
				str += _T(".");
				str += lexical_cast<kn_string>(d.day());
			}

			if (m_timeline_one_day_width > TIMELINE_LEVEL3*6 *m_hour_type/24)
			{//这个尺寸以上放得下
				str += _T("(");
				str += getDayOfWeek(d);
				str += _T(")");
			}

		}
		else
		{
			time_duration td = p.time_of_day();  
			int hour =  td.hours() ;
			str = lexical_cast<kn_string>( hour); 
			str += _T("h");
		}
	}

	return str;
}


ptime CTimeLineDrawable::goTimeLine(ptime &p)
{
	ptime pt = p+ months(m_time_step_m) + m_time_step_d + m_time_step_h;


	date d1 =p.date(); 
	date d2 =pt.date(); 
	if (m_draw_mode ==TIMELINE_LEVEL1_YEAR)
	{
		if (d1.year() == d2.year() )
		{
			return pt;
		}
		else
		{//跨年，截断到一级坐标
			ptime p1(date(d2.year(), 1, 1), time_duration(0, 0, 0));
			return p1;
		}
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_MONTH)
	{
		
		if (d1.month() == d2.month() )// && ( end_of_month_day(d2.year(),d2.month()) - d2.day()) > m_time_step_d.days() )
		{
			if (d2.day() == 31 && m_time_step_d.days() > 1)
			{
				ptime p1(date(d2.year(), d2.month(), 1), time_duration(0, 0, 0));
				return p1+months(1);
			}
			return pt;
		}
		else
		{//跨月，截断到一级坐标
			ptime p1(date(d2.year(), d2.month(), 1), time_duration(0, 0, 0));
			return p1;
		}
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_DAY)
	{
		if (d1.day() == d2.day() )
		{
			if (m_hour_type <24 )
			{
				time_duration td = pt.time_of_day();
				int h = td.hours();
				if (h>= 20)
				{
					ptime p1(date(d2.year(), d2.month(), d2.day()), time_duration(8, 0, 0));

					return p1 + days(1);
				}
				if (h < 8)
				{
					ptime p1(date(d2.year(), d2.month(), d2.day()), time_duration(8, 0, 0));
					return p1;
				}
			}

			return pt;
		}
		else
		{//跨天，截断到一级坐标
			ptime p1(date(d2.year(), d2.month(), d2.day()), time_duration(0, 0, 0));
			return p1;
		}
	}

	return pt;
}

//获取绘制区域的起始绘制点，在0点之前
ptime CTimeLineDrawable::getTimeLineBeginTime()
{
	ptime pt = getPositisonTime(0);
	if (pt.is_infinity())
	{
		return pt;
	}
	string strtmp = to_simple_string(pt);
	if (m_draw_mode ==TIMELINE_LEVEL1_YEAR)
	{
		date d =pt.date();
		ptime p1(date(d.year(), 1, 1), time_duration(0, 0, 0)); 
		pt =p1;

		//pt = pt - months(m_time_step_m);
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_MONTH)
	{
		date d =pt.date();
		ptime p1(date(d.year(), d.month(), 1), time_duration(0, 0, 0)); 
		pt =p1;
		
	}
	else if (m_draw_mode ==TIMELINE_LEVEL1_DAY)
	{
		date d =pt.date();
		ptime p1(date(d.year(), d.month(), d.day()), time_duration(0, 0, 0)); 
		if (m_hour_type < 24)
		{
			p1 += time_duration(8, 0, 0);
		}
		pt =p1;
	}
	return pt;
}

void CTimeLineDrawable::setCalendar(CNProjectCalendar* p)
{
	m_p_prj_calendar = p;
}
