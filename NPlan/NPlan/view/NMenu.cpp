//  File:        NMenu.cpp
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
#include "NMenu.h"
#include "KCheckboxView.h"
#include "K9PatchImageDrawable.h"
#include "boost/lexical_cast.hpp"
#include "KSurfaceManager.h"
#include "../manager/KStringManager.h"
#include "KFontManager.h"
/////////////////// CMenuScore //////////////////////////
CMenuScore::CMenuScore()
{

}

CMenuScore::~CMenuScore()
{
}

void CMenuScore::init(KView_PTR p_v , int score)
{
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_btn_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	//0-10 score
	int i;
	for (i = 0; i <= 10; i++)
	{
		KCheckboxView_PTR p = KCheckboxView_PTR(new KCheckboxView);
		if (i == 0)
		{
			p->Create(6 , 20, 31, 31);
		}
		else
		{
			p->Create(38 + (i-1)%5 * 32, 6+ (i-1)/5 * 32, 31, 31);
		}
		p->setResourse( getSurfaceManager()->GetSurface(_T("./resource/score_bk_a.png") ),
						getSurfaceManager()->GetSurface(_T("./resource/score_bk_n.png") ), _T(""));

		KTextDrawable_PTR text_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 16, REPaint::kLeft_Align) );
		text_drawable->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
		kn_string str = boost::lexical_cast<kn_string>(i);
		text_drawable->SetText( str );
		text_drawable->SetFontSize(11);
		text_drawable->setBold(TRUE);
		text_drawable->setTextAlign(REPaint::kCenter_Align);
		text_drawable->setRect(8,12,14,12);
		p->addDrawable(text_drawable);
		p->setName(str);
		p->m_sign_down.connect(this, &CMenuScore::click );

		if (i<= score)
		{
			p->setCheck(TRUE);
		}

		AddView(p);
	}
}

void CMenuScore::click(kn_int x, kn_int y,KMessageMouse* p)
{
	int i = boost::lexical_cast<int>(p->m_p_view->getName());
	m_sign_menu_click.emit(i);
}

/////////////////// CMenuScore //////////////////////////
CMenuColor::CMenuColor()
{

}

CMenuColor::~CMenuColor()
{
}

void CMenuColor::init(KView_PTR p_v )
{
	m_active_view = p_v;
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/action_color_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	//0-10 score
	int i;
	for (i = 1; i <= 9; i++)
	{
		KTextView_PTR p = KTextView_PTR(new KTextView());
		p->Create(12 + (i-1)%3 * 37, 10+ (i-1)/3 * 38, 37, 38);
		kn_string icon_file =  _T("./resource/task_color_0"); 
		icon_file += boost::lexical_cast<kn_string>( i);
		icon_file += _T(".png");
		KImageDrawable_PTR img = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( icon_file), TRUE) );
		img->SizeToImage();
		p->setBKDrawable(img);

		kn_string str = boost::lexical_cast<kn_string>(i);
		p->setName(str);
		p->m_sign_down.connect(this, &CMenuColor::click );

		AddView(p);
	}
	KTextView_PTR p_btn;
	createTextViewHelper(&p_btn, _T("./resource/btn_ok_n.9.png"), 20, 6+3*38 );
	kn_string str = _T("0");
	p_btn->setName(str );
	p_btn->SetRect( RERect::MakeXYWH( 20, 10+3*38, 75, 28) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->SetTextBound( RERect::MakeWH( 75, 28) );
	p_btn->SetText( getStringManager()->GetStringByID(_T("delete")) );
	p_btn->SetFontSize(12);
	p_btn->m_sign_down.connect(this, &CMenuColor::click );

	p_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
}

void CMenuColor::click(kn_int x, kn_int y,KMessageMouse* p)
{
	int i = boost::lexical_cast<int>(p->m_p_view->getName());
	m_sign_menu_view_click.emit(m_active_view, i);
}

/////////////////// CMenuSaveas //////////////////////////
CMenuSaveas::CMenuSaveas()
{

}

CMenuSaveas::~CMenuSaveas()
{
}

void CMenuSaveas::init(KView_PTR p_v )
{
	m_active_view = p_v;
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_btn_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	KTextView_PTR p_btn ;
	kn_string str ;
	createTextViewHelper(&p_btn, _T("./resource/nplan_file_icon.png"),35, 10 );
	str = _T("0");
	p_btn->setName(str );
	p_btn->SetText(_T("NPlan(nprj)"));
	p_btn->SetTextColor( ColorSetARGB(255,240,240,240), ColorSetARGB(255,255,255,0),ColorSetARGB(255,240,240,240),ColorSetARGB(255,240,240,240));
	p_btn->SetTextBound(RERect::MakeXYWH( -15, 85, 110, 20) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuSaveas::click );

	createTextViewHelper(&p_btn, _T("./resource/excel_file_icon.png"),145, 10 );
	str = _T("1");
	p_btn->setName(str );
	p_btn->SetText(_T("Excel(xls)"));
	p_btn->SetTextColor( ColorSetARGB(255,240,240,240), ColorSetARGB(255,255,255,0),ColorSetARGB(255,240,240,240),ColorSetARGB(255,240,240,240));
	p_btn->SetTextBound(RERect::MakeXYWH( -15, 85, 110, 20) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuSaveas::click );

	createTextViewHelper(&p_btn, _T("./resource/excelxml_file_icon.png"),255, 10 );
	str = _T("2");
	p_btn->setName(str );
	p_btn->SetText(_T("Excel Xml"));
	p_btn->SetTextColor( ColorSetARGB(255,240,240,240), ColorSetARGB(255,255,255,0),ColorSetARGB(255,240,240,240),ColorSetARGB(255,240,240,240));
	p_btn->SetTextBound(RERect::MakeXYWH( -15, 85, 110, 20) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuSaveas::click );

	createTextViewHelper(&p_btn, _T("./resource/project_file_icon.png"),365, 10 );
	str = _T("3");
	p_btn->setName(str );
	p_btn->SetText(_T("Project xml"));
	p_btn->SetTextColor( ColorSetARGB(255,240,240,240), ColorSetARGB(255,255,255,0),ColorSetARGB(255,240,240,240),ColorSetARGB(255,240,240,240));
	p_btn->SetTextBound(RERect::MakeXYWH( -15, 85, 110, 20) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuSaveas::click );

}

void CMenuSaveas::click(kn_int x, kn_int y,KMessageMouse* p)
{
	CMenuColor::click(x, y,p);

}

/////////////////// CMenuDaySet //////////////////////////
CMenuDaySet::CMenuDaySet()
{

}

CMenuDaySet::~CMenuDaySet()
{
}

void CMenuDaySet::init(KView_PTR v,int day)
{
	m_active_view = v;
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_btn_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	KTextView_PTR p_btn ;
	kn_string str ;
	int btn_y = 5;

	if (day != DAY_TYPE_WORKDAY)
	{
		createTextViewHelper(&p_btn, _T("./resource/menu_workday_btn.9.png"),10, btn_y );
		p_btn->setRect(10, btn_y, 100, 30 );
		str = _T("2");
		p_btn->setName(str );
		p_btn->SetText( getStringManager()->GetStringByID(_T("workday")));
		p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122));
		p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, 100, 30) );
		p_btn->setTextAlign(REPaint::kCenter_Align);
		p_btn->setStateChangeEnable(TRUE);
		p_btn->m_clicked_pos_signal.connect(this, &CMenuDaySet::click );
		btn_y+=25;
	}

	if (day != DAY_TYPE_HOLIDAY)
	{
		createTextViewHelper(&p_btn, _T("./resource/menu_holiday_btn.9.png"),10, btn_y );
		p_btn->setRect(10, btn_y, 100, 30 );
		str = _T("1");
		p_btn->setName(str );
		p_btn->SetText( getStringManager()->GetStringByID(_T("holiday")));
		p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255,188, 67, 75),ColorSetARGB(255, 188, 67, 75),ColorSetARGB(255, 188, 67, 75));
		p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, 100, 30) );
		p_btn->setTextAlign(REPaint::kCenter_Align);
		p_btn->setStateChangeEnable(TRUE);
		p_btn->m_clicked_pos_signal.connect(this, &CMenuDaySet::click );
		btn_y+=25;
	}

	//if (day != DAY_TYPE_OVERTIME)
	//{
	//	createTextViewHelper(&p_btn, _T("./resource/action_bk_3.9.png"),10, btn_y );
	//	p_btn->setRect(10, byn_y, 100, 20 );
	//	str = _T("1");
	//	p_btn->setName(str );
	//	p_btn->SetText( getStringManager()->GetStringByID(_T("holiday")));
	//	p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255, 239, 125, 28),ColorSetARGB(255, 239, 125, 28),ColorSetARGB(255, 239, 125, 28));
	//	p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, 100, 20) );
	//	p_btn->setTextAlign(REPaint::kCenter_Align);
	//	p_btn->m_sign_down.connect(this, &CMenuSaveas::click );
	//	btn_y+=25;
	//}

}

void CMenuDaySet::click(kn_int x, kn_int y,KMessageMouse* p)
{
	CMenuColor::click(x, y,p);

}


/////////////////// CMenuProject //////////////////////////
CMenuProject::CMenuProject()
{

}

CMenuProject::~CMenuProject()
{
//	m_timer->stop();
}

void CMenuProject::init(KView_PTR v,int day)
{
	m_active_view = v;
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_btn_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	KTextView_PTR p_btn ;
	kn_string str ;
	int btn_y = 5;
	int btn_w = 120;

	createTextViewHelper(&p_btn, _T("./resource/action_bk.9.png"),10, btn_y );
	p_btn->setRect(10, btn_y, btn_w, 30 );
	str = _T("1");
	p_btn->setName(str );
	p_btn->SetText( getStringManager()->GetStringByID(_T("open_prj")));
	p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122));
	p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, btn_w, 30) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->setStateChangeEnable(TRUE);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuProject::click );
	btn_y+=35;

	createTextViewHelper(&p_btn, _T("./resource/action_bk.9.png"),10, btn_y );
	p_btn->setRect(10, btn_y, btn_w, 30 );
	str = _T("2");
	p_btn->setName(str );
	p_btn->SetText( getStringManager()->GetStringByID(_T("del_prj")));
	p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122));
	p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, btn_w, 30) );
	p_btn->setTextAlign(REPaint::kCenter_Align);
	p_btn->setStateChangeEnable(TRUE);
	p_btn->m_clicked_pos_signal.connect(this, &CMenuProject::click );
	btn_y+=25;

//	m_timer = KTimer_PTR(new KTimer(2000, 1) );
//	m_timer->setView(shared_from_this());
//	m_timer->m_timeout_signal.connect(this, &CMenuProject::onTimer);
//	m_timer->start();
}

void CMenuProject::click(kn_int x, kn_int y,KMessageMouse* p)
{
	CMenuColor::click(x, y,p);

}

void CMenuProject::onTimer(kn_int x)
{

}

/////////////////// CMenuMindMap //////////////////////////

CMenuMindMap::CMenuMindMap()
{

}

CMenuMindMap::~CMenuMindMap()
{

}

void CMenuMindMap::init( KView_PTR v, int type )
{
	m_active_view = v;
	//BK
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_btn_bk.9.png") ), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	KTextView_PTR p_btn ;
	int btn_y = 5;
	int btn_w = 120;
	kn_int btn_num = 0;
	vector<kn_string> text;
	switch (type)
	{
	case MIND_AREA_MENU:
		{
			text.push_back(_T("add_task"));
		}
		break;
	case MIND_PRJ_MENU:
		{
			text.push_back(_T("add_task"));
			text.push_back(_T("edit_prj_name"));
		}
		break;
	case MIND_TASK_MENU:
		{
			text.push_back(_T("add_action"));
			text.push_back(_T("edit_task_name"));
			text.push_back(_T("delete_task"));
		}
		break;
	case MIND_ACTION_MENU:
		{
			text.push_back(_T("edit_action_name"));
			text.push_back(_T("edit_action_time"));
			text.push_back(_T("delete_action"));
		}
		break;
	default:
		break;
	}

	for (kn_int i = 0; i < text.size(); ++i)
	{
		createTextViewHelper(&p_btn, _T("./resource/action_bk.9.png"),10, btn_y );
		p_btn->setRect((m_rect.width() - btn_w)/2, btn_y, btn_w, 30 );
		p_btn->setName(boost::lexical_cast<kn_string>(i +1));
		p_btn->SetText( getStringManager()->GetStringByID(text[i]));
		p_btn->SetTextColor( ColorSetARGB(255,50,50,50), ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122),ColorSetARGB(255,43,71,122));
		p_btn->SetTextBound(RERect::MakeXYWH( 0, 0, btn_w, 30) );
		p_btn->setTextAlign(REPaint::kCenter_Align);
		p_btn->setStateChangeEnable(TRUE);
		p_btn->m_clicked_pos_signal.connect(this, &CMenuMindMap::click );
		btn_y+=35;
	}
}

void CMenuMindMap::click( kn_int x, kn_int y,KMessageMouse* kMsg)
{
	CMenuColor::click(x, y, kMsg);
}

