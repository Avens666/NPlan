//  File:        NAboutView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2017/5/7
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NAboutView.h"
#include "../manager/KStringManager.h"
#include "../manager/KConfigManager.h"
#include "K9PatchImageDrawable.h"
#ifdef WIN32
#include <shellapi.h>
#endif
NAboutView::NAboutView(void)
{
	m_ver = 1000;
	m_version	= _T("NPlan V1.0.2 version");	
	switch (getConfigManager()->getLanguage())
	{
	case LAN_CN:
		m_title		= _T("关于NPLan");	
		m_product	= _T("产品设计：陈治");	
		m_develop	= _T("主页： nplan.nuiengine.com");
		m_engine	= _T("基于NUI 界面引擎");
		m_copyright	= _T("联系方式: nplan@nuiengine.com   All rights served");
		m_contact = _T("联系我们");
		m_txt_path = _T("./resource/about_text.png");
		break;
	case LAN_JP:
		m_title		= _T("About NPLan");	
		m_product	= _T("プロダクトデザイナー：陳治");	
		m_develop	= _T("サイト:  nplan.nuiengine.com");
		m_engine	= _T("NUI画面エンジンに基づく");
		m_copyright	= _T("連絡方法: nplan@nuiengine.com   All rights served");
		m_contact = _T("お问い合わせ");
		m_txt_path = _T("./resource/about_text_jp.png");
		break;
	default:
		m_title		= _T("About NPLan");	
		m_product	= _T("Product Manager：Chen Zhi");	
		m_develop	= _T("Web Site: nplan.nuiengine.com");
		m_engine	= _T("Powered by NUI Engine");
		m_copyright	= _T("Contact: nplan@nuiengine.com   All rights served");
		m_contact = _T("Contact us");
		m_txt_path = _T("./resource/about_text_en.png");
		break;
	}


}


NAboutView::~NAboutView(void)
{
}

void NAboutView::init()
{
	//底部栏

	K9PatchImageDrawable_PTR drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png")),true));
	drawable->setRect(0,0 ,m_rect.width(),  m_rect.height());
	addDrawable(drawable);

	KStaticView_PTR txt_view ;
	createTextHelper(&txt_view, m_title, 20, 20, 14, m_rect.width(), 20);
    txt_view->setTextAlign(REPaint::kLeft_Align);

	createImageHelper(&txt_view, _T("./resource/about_icon.png"), 20, 55 );
	createImageHelper(&txt_view, m_txt_path, txt_view->GetRect().width() + 25, txt_view->GetRect().bottom() - 18 );
	createImageHelper(&txt_view, _T("./resource/line.png"), 13, 106 );
	
	txt_view->SetRect(RERect::MakeXYWH(10, 106, m_rect.width()-30, txt_view->GetRect().height() ));

	createTextHelper(&txt_view, m_version, 16, 20, 120, 380, 20);
	txt_view->SetTextColor(0xff539b48);
	createTextHelper(&txt_view, m_product, 16, 20, 145, 380, 20);
	txt_view->SetTextColor(0xff222222);

	//web
	KTextView_PTR btn_web;
	createTextViewHelper(&btn_web, _T(""), 20,170,380, 20);
	btn_web->SetText(m_develop);
	btn_web->SetTextColor(0xff222222,RE_ColorBLUE,RE_ColorMAGENTA,RE_ColorBLACK);
	btn_web->setStateChangeEnable(TRUE);
	btn_web->setCursorChangeEnable(TRUE);
	btn_web->m_clicked_signal.connect(this,&NAboutView::OnBtnWeb);
	/////

	createImageHelper(&txt_view, _T("./resource/nuilogo.png"), 20, 200 );
	createTextHelper(&txt_view, m_engine, 12, 20, 240, 380, 20);
	txt_view->SetTextColor(0xff666666);
	
	createTextHelper(&txt_view, m_copyright, 12, 20, 260, 380, 20);
	txt_view->SetTextColor(0xff666666);
	

	KTextView_PTR contact_v = KTextView_PTR(new KTextView);
	contact_v->SetText( m_contact );
	contact_v->SetTextBound( RERect::MakeXYWH(0,0, 100,30) );
    contact_v->setTextAlign(REPaint::kCenter_Align);
	contact_v->SetTextColor(0xff666666,RE_ColorBLUE,RE_ColorMAGENTA,RE_ColorWHITE);
	K9PatchImageDrawable_PTR contact_v_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/btn_email_n.9.png")),true));
	contact_v_drawable->setRect(0,0 ,100,  30);
	contact_v->SetFontSize(12);
	contact_v->Create(20, 300, 100, 30);

	contact_v->setBKDrawable(contact_v_drawable);
	contact_v->setStateChangeEnable(TRUE);
	contact_v->setCursorChangeEnable(TRUE);
	contact_v->m_clicked_signal.connect(this,&NAboutView::OnBtnContact);
	AddView(contact_v);

	KImgButtonView_PTR p_btn_ok;
	createImgView9PatchHelper(&p_btn_ok,_T("./resource/btn_ok_n.9.png"), 
		_T("./resource/btn_ok_a.9.png"), _T("./resource/btn_ok_n.9.png"), 
		m_rect.width()-100 , m_rect.height() - 55, 80, 30);
	p_btn_ok->SetText(getStringManager()->GetStringByID(_T("ok")));
	p_btn_ok->SetFontSize(16);

	p_btn_ok->setStateChangeEnable(true);
	p_btn_ok->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	p_btn_ok->SetTextBound(RERect::MakeXYWH(0,0,80,30));
    p_btn_ok->setTextAlign(REPaint::kCenter_Align);
	p_btn_ok->m_clicked_signal.connect(this,&NAboutView::OnBtnOK);

	KImgButtonView_PTR btn_exit;
	createImgViewHelper(&btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		m_rect.width() - 45,3);
	btn_exit->m_clicked_signal.connect(this,&NAboutView::OnBtnOK);
}



void NAboutView::OnBtnOK(KView_PTR V)
{
	endModal(KN_REUSLT_CANCLE, TRUE);

}

void NAboutView::OnBtnContact(KView_PTR)
{
#ifdef WIN32
	ShellExecute(NULL,NULL,_T("mailto:nplan@nuiengine.com"),NULL,NULL,SW_SHOW);

#endif

}

void NAboutView::OnBtnWeb(KView_PTR)
{
#ifdef WIN32
//	ShellExecute(NULL,NULL,_T("mailto:nplan@nuiengine.com"),NULL,NULL,SW_SHOW);
	ShellExecute(NULL,_T("open"),_T("http://nplan.nuiengine.com"),NULL,NULL,SW_SHOW);
#endif

}
