//  File:        NSettingView.cpp
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
#include "NSettingView.h"
#include "../manager/KStringManager.h"
#include "../manager/KProjectManager.h"
#include "../manager/KConfigManager.h"
#include "KFontManager.h"

#define CLOSE_SETTING				KNUIMSG_USER + 50  //界面结束消息

NSettingView::NSettingView(RERect rect)
{

	Create(rect);
}


NSettingView::~NSettingView(void)
{
}

void NSettingView::init(RERect rect)
{
	kn_int i_x_off = 20;	//横轴间距
	kn_int i_y_off = 30;	//

	kn_int i_title_height = 20;//标题高度

	kn_int i_row = 2;		//有几行

	//初始化背景
	K9PatchImageDrawable_PTR drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png") ),true) );
	drawable->setRect( 0,0 ,rect.width(),rect.height());
	addDrawable(drawable);

	KViewGroup_PTR group_setting = KViewGroup_PTR(new KViewGroup);
	RERect group_rect = RERect::MakeXYWH(i_x_off,i_title_height + i_y_off , rect.width() - 2*i_x_off -5,
		rect.height() - i_title_height - 2* i_y_off -5);
	group_setting->Create(group_rect);
	AddView(group_setting);

	//文言

	KTextDrawable_PTR pStr (new KTextDrawable(getStringManager()->GetStringByID(_T("set_lang")).c_str()));
	pStr->SetRect(RERect::MakeXYWH(0,10,group_rect.width(),(group_rect.height()/i_row)));
	pStr->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	pStr->SetFontSize(16);
	pStr->setTextAlign(REPaint::kCenter_Align);
	group_setting->addDrawable(pStr);

	//单选框
	m_p_rdo_set = KRadioboxGroup_PTR(new KRadioboxGroup);
	m_p_rdo_set->Create(RERect::MakeXYWH(30,(group_rect.height()/i_row) - 10,group_rect.width() - 60,(group_rect.height()/i_row)));
	m_p_rdo_set->m_sign_index_changed.connect(this,&NSettingView::onClickRdo);
	group_setting->AddView(m_p_rdo_set);


	vector<KRdoMsg> v_msg;
	KRdoMsg rdo;
	rdo.strClickResPath		= _T("./resource/rdo_a.png");
	rdo.strUnclickResPath	= _T("./resource/rdo_n.png");	 
	rdo.strMessage			= getStringManager()->GetStringByID(_T("set_cn"));
	v_msg.push_back(rdo);
	rdo.strMessage			= getStringManager()->GetStringByID(_T("set_en"));
	v_msg.push_back(rdo);
	rdo.strMessage			= getStringManager()->GetStringByID(_T("set_jp"));
	v_msg.push_back(rdo);
	setRdoResourse(v_msg);

	//标题
	KTextDrawable_PTR pTitle (new KTextDrawable(getStringManager()->GetStringByID(_T("set_title")).c_str()));
	pTitle->SetRect(RERect::MakeXYWH(18,2,group_rect.width() - 36,i_title_height + 20));
	pTitle->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
    pTitle->setTextAlign(REPaint::kLeft_Align);
	pTitle->setColor(RE_ColorWHITE);
	pTitle->SetFontSize(18);
	addDrawable(pTitle);

	//退出
	
	createImgViewHelper(&m_p_btn_cancel,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		rect.width() - 45,3);

	m_p_btn_cancel->m_clicked_signal.connect(this,&NSettingView::onBtnCancle);
}

void NSettingView::setRdoResourse( const vector<KRdoMsg>& vec )
{

	kn_int i_total_pix = m_p_rdo_set->GetRect().width()/vec.size();
	for (int i = 0 ; i < vec.size() ; ++i)
	{
		KRadioboxView_PTR view =KRadioboxView_PTR( new KRadioboxView) ;

		KDrawable_PTR draw_chk_bg= KImageDrawable_PTR(
			new KImageDrawable(getSurfaceManager()->GetSurface(vec[i].strClickResPath.c_str()),true));

		KDrawable_PTR draw_unchk_bg = KImageDrawable_PTR(
			new KImageDrawable(getSurfaceManager()->GetSurface(vec[i].strUnclickResPath.c_str()),true));

		view->Create(RERect::MakeXYWH(i*i_total_pix,0,i_total_pix,m_p_rdo_set->GetRect().height()));
		view->setResourseDrawable(draw_chk_bg,draw_unchk_bg,vec[i].strMessage);


		m_p_rdo_set->AddView(view);
		view->SetCenter();

		if (getConfigManager()->getLanguage() == i)
		{
			view->setCheck(true);
		}
	}
}

void NSettingView::onClickRdo( kn_int i )
{
	kn_char buf[8] = {0};
	 _itow(i,buf,10);
	getConfigManager()->setConfig(_T("language"),buf);
}
void NSettingView::onBtnCancle(KView_PTR p)
{
	endModal(KN_REUSLT_OK,TRUE);
} 
