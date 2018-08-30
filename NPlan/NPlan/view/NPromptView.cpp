//  File:        NPromptView.cpp
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

#include "NPromptView.h"
#include "KConfigManager.h"
#include "K9PatchImageDrawable.h"
#include "KSurfaceManager.h"
#include "KStringManager.h"

NPromptView::NPromptView(void)
{
	m_display_prompt = true;
}

NPromptView::~NPromptView(void)
{
}

kn_bool NPromptView::IsDisplayPrompt()
{
	return m_display_prompt;
}

void NPromptView::Init(kn_string image_name)
{
	kn_string image_path = getConfigManager()->getLanguageImagePath() + image_name;
	KImageDrawable_PTR bk_drawable = KImageDrawable_PTR(new KImageDrawable(image_path));
	bk_drawable->setRect(0, 0, m_rect.width(), m_rect.height());
	addDrawable(bk_drawable);
	bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right,
		KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);


	m_check_box_view = KCheckboxView_PTR(new KCheckboxView());
	m_check_box_view->Create(10, m_rect.height() - 35, 80, 20);
	m_check_box_view->setResourse(	getSurfaceManager()->GetSurface(_T("./resource/rdo_a.png")), 
									getSurfaceManager()->GetSurface(_T("./resource/rdo_n.png")),
									getStringManager()->GetStringByID(_T("no_prompt")) );
	m_check_box_view->SetTextColor(RE_ColorWHITE);
	m_check_box_view->m_clicked_signal.connect(this, &NPromptView::onPromptCheckBoxClick);
	AddView(m_check_box_view);
	m_check_box_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left,
		KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
		

	KImgButtonView_PTR ok_btn;
	createImgView9PatchHelper(&ok_btn, _T("./resource/btn_ok_n.9.png"),_T("./resource/btn_ok_a.9.png"),_T("./resource/btn_ok_a.9.png"), m_rect.width() - 140, m_rect.height() - 40, 60, 30 );
	ok_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, 
		KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	ok_btn->m_clicked_signal.connect(this, &NPromptView::OnOkBtnClick);
	ok_btn->SetText(getStringManager()->GetStringByID(_T("ok")));
	ok_btn->setTextAlign(REPaint::kCenter_Align);

	KImgButtonView_PTR cancel_btn;
	createImgView9PatchHelper(&cancel_btn, _T("./resource/btn_ok_n.9.png"),_T("./resource/btn_ok_a.9.png"),_T("./resource/btn_ok_a.9.png"), m_rect.width() - 70, m_rect.height() - 40, 60, 30 );

	cancel_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, 
		KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	cancel_btn->m_clicked_signal.connect(this, &NPromptView::OnOkCancelClick);
	cancel_btn->SetText(getStringManager()->GetStringByID(_T("cancel")));
	cancel_btn->setTextAlign(REPaint::kCenter_Align);
}

void NPromptView::OnOkBtnClick( KView_PTR )
{
	endModal(KN_REUSLT_OK, TRUE);
}

void NPromptView::onPromptCheckBoxClick( KView_PTR )
{
	m_display_prompt = (!m_check_box_view->getCheck());
}

void NPromptView::OnOkCancelClick( KView_PTR )
{
	endModal(KN_REUSLT_CANCLE, TRUE);
}
