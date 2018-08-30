//  File:        NSwicthImagePage.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NSwicthImagePage.h"
#include "../manager/KConfigManager.h"
#include "KSurfaceManager.h"
#include "boost/lexical_cast.hpp"
#include "../NPlan.h"
#include "K9PatchImageDrawable.h"
#include "KFontManager.h"
#include "mainforwindows.h"
#include "KStringManager.h"
const static kn_int enter_btn_width = 160;
const static kn_int enter_btn_height = 48;
NSwicthImagePage::NSwicthImagePage( void )
{
	SetShieldMsg(true);
	m_index_page = 0;
}

NSwicthImagePage::~NSwicthImagePage( void )
{

}

void NSwicthImagePage::Init(kn_string imagePathKeyValue, kn_int imageNum)
{
	int addImageSucceed=0;
	kn_int first_image = 1;
	for (int i=0; i < imageNum; ++i)
	{
		kn_string image_path = imagePathKeyValue;
		image_path+=boost::lexical_cast<kn_string,int>(i+1);

		KStaticView_PTR image_view;
		createImageHelper(&image_view, getConfigManager()->getConfig(image_path), m_rect.width(), 0);

		if (image_view)
		{
			if (first_image)
			{
				first_image = 0;
				image_view->SetPositionX(0);
			}
			++addImageSucceed;
			image_view->setRect(image_view->GetRect().left(), image_view->GetRect().top(), m_rect.width(), m_rect.height());
			m_images.push_back(image_view);
		}
	}

	if (addImageSucceed==0)
	{
		return;
	}

	m_radio_group = KRadioboxGroup_PTR(new KRadioboxGroup);
	kn_int radio_group_width = (addImageSucceed - 1) * 30 + addImageSucceed * 10;
	m_radio_group->Create((m_rect.width() - radio_group_width)/2, m_rect.height() - 38, radio_group_width, 10);
	KImageDrawable_PTR da_boy  = KImageDrawable_PTR(new KImageDrawable(	getSurfaceManager()->GetSurface(_T("./resource/radio_n.png")),true));

	int buttonLeft = 0;
	for (int i=0; i < addImageSucceed; i++)
	{
		KRadioboxView_PTR radio_button =KRadioboxView_PTR(new KRadioboxView);
		radio_button->Create(buttonLeft, 0, 10, 10);
		//radio_button->setIconDrawable( da_boy );
		radio_button->setResourse( getSurfaceManager()->GetSurface(_T("./resource/radio_a_focus.png")), getSurfaceManager()->GetSurface(_T("./resource/radio_n.png")),_T(""));
		radio_button->enableMessage(FALSE);
		m_radio_group->AddView(radio_button);
		//m_radio_button.push_back(radio_button);
		buttonLeft = buttonLeft + 40;
	}
	m_radio_group->SetSelectedIndex(0);
	AddView(m_radio_group);

	kn_int btn_width = 60;
	kn_int btn_height = 60;
	m_last_page_btn = KImgButtonView_PTR(new KImgButtonView());
	m_last_page_btn->Create(-60, (m_rect.height() - 60)/2, 60, 60);
	KImageDrawable_PTR left_n_drawable = KImageDrawable_PTR(new KImageDrawable(_T("./resource/welcome_left_btn_n.png")));
	left_n_drawable->setRect(0, 0, btn_width, btn_height);
	KImageDrawable_PTR left_a_drawable = KImageDrawable_PTR(new KImageDrawable(_T("./resource/welcome_left_btn_a.png")));
	left_a_drawable->setRect(0, 0, btn_width, btn_height);
	m_last_page_btn->setBKG(left_n_drawable, left_a_drawable, left_a_drawable, left_n_drawable);
	AddView(m_last_page_btn);
	m_last_page_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_last_page_btn->m_clicked_signal.connect(this, &NSwicthImagePage::onBtnLastImage);

	m_next_page_btn = KImgButtonView_PTR(new KImgButtonView());
	m_next_page_btn->Create(m_rect.width() - 75, (m_rect.height() - 60)/2, 60, 60);
	AddView(m_next_page_btn);
	m_next_page_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_next_page_btn->m_clicked_signal.connect(this, &NSwicthImagePage::onBtnNextImage);
	KImageDrawable_PTR right_n_drawable = KImageDrawable_PTR(new KImageDrawable(_T("./resource/welcome_right_btn_n.png")));
	right_n_drawable->setRect(0, 0, btn_width, btn_height);
	KImageDrawable_PTR right_a_drawable = KImageDrawable_PTR(new KImageDrawable(_T("./resource/welcome_right_btn_a.png")));
	right_a_drawable->setRect(0, 0, btn_width, btn_height);
	m_next_page_btn->setBKG(right_n_drawable, right_a_drawable, right_a_drawable, right_n_drawable);

}

void NSwicthImagePage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &NSwicthImagePage::onKeyUp);
}


void NSwicthImagePage::onBtnLastImage(KView_PTR)
{
	while (m_ani_thread.isPlaying())
	{
		Sleep(20);
	}

	if (m_index_page - 1 < 0)
	{
		return;
	}
	if (m_index_page -1 == 0)
	{
		m_ani_thread.addAnimationHelper(m_last_page_btn, PropertyPARA_ID_POS_X, -60, 700,0, 0, KEasingCurve::Linear);
	}
	if (m_index_page - 1 == (m_images.size() - 2) )
	{
		arriveLastEndView();
	}
	m_ani_thread.addAnimationHelper(m_images[m_index_page], PropertyPARA_ID_POS_X, m_rect.width(), 700,0, 0 );
	m_index_page = m_index_page - 1;
	m_radio_group->SetSelectedIndex(m_index_page);
	m_ani_thread.addAnimationHelper(m_images[m_index_page], PropertyPARA_ID_POS_X, 0, 700,0, 0 );
	m_ani_thread.Start();
}

void NSwicthImagePage::arriveLastEndView()
{
	m_ani_thread.addAnimationHelper(m_next_page_btn, PropertyPARA_ID_POS_X, m_rect.width() - 75, 700,0, 0, KEasingCurve::Linear);
}

void NSwicthImagePage::onBtnNextImage(KView_PTR)
{
	while (m_ani_thread.isPlaying())
	{
		Sleep(20);
	}
	if (m_index_page == 0)
	{
		m_ani_thread.addAnimationHelper(m_last_page_btn, PropertyPARA_ID_POS_X, 15, 500,0, 0, KEasingCurve::OutQuad);
	}
	if (m_index_page == (m_images.size() - 1))
	{
		return;
	}
	if (m_index_page+1 < m_images.size())
	{
		m_ani_thread.addAnimationHelper(m_images[m_index_page], PropertyPARA_ID_POS_X, -m_rect.width(), 500,0, 0, KEasingCurve::OutQuad );
		m_index_page = m_index_page + 1;
		m_radio_group->SetSelectedIndex(m_index_page);
		m_ani_thread.addAnimationHelper(m_images[m_index_page], PropertyPARA_ID_POS_X, 0, 500, 0, 0, KEasingCurve::OutQuad );
	}
	if (m_index_page == (m_images.size() - 1))
	{
		arriveEndView();
	}
	m_ani_thread.Start();
}

void NSwicthImagePage::arriveEndView()
{
	m_ani_thread.addAnimationHelper(m_next_page_btn, PropertyPARA_ID_POS_X, m_rect.width(), 700,0, 0, KEasingCurve::Linear);
}

void NSwicthImagePage::OnDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	if ((iScreenX > m_last_page_btn->GetRect().right()) && (iScreenX < m_next_page_btn->GetRect().left()))
	{
		onBtnNextImage(pMsg->m_p_view);
	}
	
}

void NSwicthImagePage::onKeyUp( KMessageKey *kMsg )
{
	//键盘消息
	kMsg->setIdle(KMSG_RETURN_DILE);
	int key = kMsg->m_key_id;
	kn_int x = Key_spaceKey;
	kn_int y = Key_rightKey;
	kn_int z = Key_leftKey;
	if( (key == Key_spaceKey) || (key == Key_rightKey) || (key == Key_pageDownKey))
	{
		onBtnNextImage(kMsg->m_p_view);
	}
	else if (key == Key_leftKey || (key == Key_pageUpKey))
	{
		onBtnLastImage(kMsg->m_p_view);
	}
}

////////////////////////////NWelcomeImagePage///////////////////////////////////////
NWelcomeImagePage::NWelcomeImagePage()
{
	
}

NWelcomeImagePage::~NWelcomeImagePage()
{

}


void NWelcomeImagePage::Init( kn_string imagePathKeyValue, kn_int imageNum )
{
	NSwicthImagePage::Init(imagePathKeyValue, imageNum);
	kn_string image_show_over = _T("0");
	getConfigManager()->setConfig(_T("runfirst"), image_show_over);
}

void NWelcomeImagePage::arriveLastEndView()
{
	NSwicthImagePage::arriveLastEndView();
	if (m_enter_main_view_btn)
	{
		AddViewToDel(m_enter_main_view_btn);
		m_enter_main_view_btn.reset();
	}
}

void NWelcomeImagePage::arriveEndView()
{
	NSwicthImagePage::arriveEndView();
	if (!m_enter_main_view_btn)
	{
		m_enter_main_view_btn = KImgButtonView_PTR(new KImgButtonView());
		m_enter_main_view_btn->Create((m_rect.width() - 100)/2 -25, m_rect.height() - 60, enter_btn_width, enter_btn_height);
		AddView(m_enter_main_view_btn);
		m_enter_main_view_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
		m_enter_main_view_btn->m_clicked_signal.connect(this, &NWelcomeImagePage::OnEnterMainViewClick);
		K9PatchImageDrawable_PTR enter_n_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(_T("./resource/welcome_btn_n.9.png")));
		enter_n_drawable->setRect(0, 0, enter_btn_width, enter_btn_height);
		K9PatchImageDrawable_PTR enter_a_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(_T("./resource/welcome_btn_a.9.png")));
		enter_a_drawable->setRect(0, 0, enter_btn_width, enter_btn_height);
		m_enter_main_view_btn->setBKG(enter_n_drawable, enter_a_drawable, enter_a_drawable, enter_n_drawable);
		m_enter_main_view_btn->SetText(getStringManager()->GetStringByID(_T("start_use")));
		m_enter_main_view_btn->SetTextColor(RE_ColorWHITE);
		m_enter_main_view_btn->SetFontSize(22);
		m_enter_main_view_btn->setTextAlign(REPaint::kCenter_Align);
		m_enter_main_view_btn->SetFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
		m_enter_main_view_btn->setOpacity(0);
	}
	m_ani_thread.addAnimationHelper(m_enter_main_view_btn, PropertyPARA_ID_Opacity, 255, 700, 0, 0, KEasingCurve::OutQuad);
}

void NWelcomeImagePage::OnEnterMainViewClick( KView_PTR )
{
	while(m_ani_thread.isPlaying())
	{
		Sleep(20);
	}
	m_radio_group->SetShow(false);
	m_enter_main_view_btn->SetShow(false);
	m_next_page_btn->SetShow(false);
	m_last_page_btn->SetShow(false);
	m_ani_thread.addAnimationHelper(m_images[m_images.size()-1], PropertyPARA_ID_POS_X, -m_rect.width(), 700, 0, 0, KEasingCurve::OutQuad);
	m_ani_thread.setStopMsg(MSG_WELCOME_ANI_OVER);
	m_ani_thread.Start();
	SetShieldMsg(false);
}



