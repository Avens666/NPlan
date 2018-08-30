//  File:        MilestoneEditView.cpp
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
#include "MilestoneEditView.h"
#include "../manager/KStringManager.h"
#include "../manager/KProjectManager.h"
#include "K9PatchImageDrawable.h"

MilestoneEditView::MilestoneEditView(void)
{
	m_p_data = NULL;
}


MilestoneEditView::~MilestoneEditView(void)
{
}

void MilestoneEditView::init(CNProjectMileStone* data)
{
	m_p_data = data;
	//初始化背景
	K9PatchImageDrawable_PTR drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png") ),true) );
	drawable->setRect( 0,0 , m_rect.width(),  m_rect.height() );
	addDrawable(drawable);

	KStaticView_PTR head;
	createTextHelper( &head, getStringManager()->GetStringByID(_T("edit_title")), 16, 20, 13, 200,  20 );
    head->setTextAlign(REPaint::kLeft_Align);
	//name
	KStaticView_PTR static_name_view;
	createTextHelper( &static_name_view, getStringManager()->GetStringByID(_T("milestone_name")), 16, 0, 70, m_rect.width(),  20 );
	static_name_view->SetTextColor(0xffa6a6a6);
	static_name_view->setTextAlign(REPaint::kCenter_Align);
	createEditViewHelper(&m_edit_name, 16, 70, 100, 250, 40);
	K9PatchImageDrawable_PTR p_name  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	p_name->SetRect(RERect::MakeWH(m_edit_name->GetRect().width(), m_edit_name->GetRect().height() ));
	m_edit_name->Init(5,5,RE_ColorBLACK);
	m_edit_name->setBKDrawable(p_name);
	m_edit_name->SetCrossIndex(0);
	m_edit_name->setTextOff(2,2);
	m_edit_name->setCrossOff(4);
	m_edit_name->SetTextColor(0xFF444444);
	m_edit_name->SetText(data->getName());
	
	KImgButtonView_PTR ok_btn;
	createImgView9PatchHelper(&ok_btn, _T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"), _T("./resource/btn_ok_n.9.png"), 
		m_rect.width()-370, m_rect.height() - 70, 100, 40);
	ok_btn->SetText( getStringManager()->GetStringByID(_T("ok")) );
	ok_btn->SetFontSize(14);
	ok_btn->setStateChangeEnable(true);
	ok_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	ok_btn->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    ok_btn->setTextAlign(REPaint::kCenter_Align);

	KImgButtonView_PTR del_btn;
	createImgView9PatchHelper(&del_btn, _T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"), _T("./resource/btn_ok_n.9.png"), 
		m_rect.width()-250, 
		m_rect.height() - 70,
		100, 40);
	del_btn->SetText( getStringManager()->GetStringByID(_T("delete")) );
	del_btn->SetFontSize(14);
	del_btn->setStateChangeEnable(true);
	del_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	del_btn->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    del_btn->setTextAlign(REPaint::kCenter_Align);

	KImgButtonView_PTR cacle_btn;
	createImgView9PatchHelper(&cacle_btn, _T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"), _T("./resource/btn_ok_n.9.png"),  m_rect.width()-130, m_rect.height() - 70, 100, 40);
	cacle_btn->SetText( getStringManager()->GetStringByID(_T("cancel")) );
	cacle_btn->SetTextBound(RERect::MakeXYWH(8,10,80,24));
	cacle_btn->SetFontSize(14);
    cacle_btn->setTextAlign(REPaint::kCenter_Align);
	cacle_btn->setStateChangeEnable(true);
	cacle_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	ok_btn->m_clicked_signal.connect(this,&MilestoneEditView::onBtnOk);
	del_btn->m_clicked_signal.connect(this,&MilestoneEditView::onBtnDel);
	cacle_btn->m_clicked_signal.connect(this,&MilestoneEditView::onBtnCancle);

	KImgButtonView_PTR btn_exit;
	createImgViewHelper(&btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		m_rect.width() - 45,3);
	btn_exit->m_clicked_signal.connect(this,&MilestoneEditView::onBtnCancle);
}
/**
* 确定
*/
void MilestoneEditView::onBtnOk(KView_PTR p)
{
	kn_string milestoneName = m_edit_name->getText();
	m_p_data->setName(milestoneName.c_str());
	endModal(KN_REUSLT_OK, TRUE);
} 
/**
* 取消
*/
void MilestoneEditView::onBtnCancle(KView_PTR p)
{
	endModal(KN_REUSLT_CANCLE, TRUE);
} 
/**
删除当前里程碑
*/
void MilestoneEditView::onBtnDel( KView_PTR p )
{
	endModal(KN_REUSLT_USER_DEL, TRUE);
}
