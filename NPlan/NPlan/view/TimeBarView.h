//  File:        TimeBarView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/15
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_TIMEBAR_VIEW_H__
#define K_TIMEBAR_VIEW_H__

#include "KImageDrawable.h"
#include "KViewGroup.h"
#include "KTextDrawable.h"
#include "TimeLine.h"
#define KVIEW_TIMEBAR 1000;

#define PARA_ID_TimeBar PropertyPARA_ID_USER+3
#define PARA_ID_TimeBar_Scale PropertyPARA_ID_USER+5

class CTimeBarView: public KView
{
public:
	CTimeBarView(void);
	virtual ~CTimeBarView(void);
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void init();
	virtual void OnWheel(KMessageMouseWheel* pMsg);

	virtual void SetRect(const RERect& rect);

	////用于通用型动画参数，不新增属性类实现动画参数时使用
	virtual kn_bool isChange(kn_int para_id, kn_double v);
	virtual void doChange(kn_int para_id, kn_double v);
	virtual kn_double  getParaValue(kn_int para_id);

	//设置时间轴比例长度像素尺寸
	void setTimelineOneDayWidth(int v,kn_bool b = false);  //b是给上层回调消息用的，判断是否设置对应控件
	int getTimelineOneDayWidth();
	//根据时间段返回显示比例
	int getTimeDurateWidth(ptime &start, ptime &end);

	//获得时间轴相对标准点的偏移，像素单位
	int getTimeLineOffset();
	//设置时间轴相对标准点的偏移，像素单位
	void setTimeLineOffset(int v);

	//根据时间返回时间轴像素坐标
    float getTimePosition(const ptime &p);
	//根据像素坐标返回时间
	ptime getPositisonTime(int x);
	//根据像素坐标返回时间,增加根据显示比例增加舍入操作
	ptime getPositisonTimeInt(int x, int step_hour);

	//根据时间段 相对于显示宽度尺寸 返回显示比例, 
	int getTimeDurationWidth(ptime &start, ptime &end);

	RERect getTimelineArea();
	sigslot::signal1<bool> m_timeline_change_signal;
	void setCalendar(CNProjectCalendar* p);
protected:
	CTimeLineDrawable_PTR m_timeline;
	KTextDrawable_PTR m_text_drawable;

	int m_timeline_offset;
	int m_mouse_x;
	//时间轴高度，不含timeline area
	int m_timeline_height;
};
typedef boost::shared_ptr<CTimeBarView> CTimeBarView_PTR;

#endif

