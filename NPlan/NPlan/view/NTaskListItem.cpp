//  File:        NTaskListItem.cpp
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
#include "NTaskListItem.h"
#include "../manager/KStringManager.h"
#include "../file/utility.h"
#include "../NPlan.h"



NTaskListItem::NTaskListItem(KDrawable_PTR p)
{
	addDrawable(p);

	m_i_card_id = -1;

	m_bg_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/list_item_a.9.png")),true));
	m_bg_drawable->SetRect(p->GetRect());
	addDrawable(m_bg_drawable);
	m_bg_drawable->SetShow(false);
}
NTaskListItem::~NTaskListItem(void)
{
}

void NTaskListItem::initCtrl()
{
	kn_int iHeight = 22;
	kn_int i_name_width = 280;
	kn_int i_people_width = 162;
	kn_int i_time_width = 125;
	kn_int i_state_width = 120;
	kn_int i_interstice = 2;
	kn_int i_font_size = 16;
	//背景drawable



	m_edit_name =KEditView_PTR(new KEditView);
	m_edit_name->Create(0,5,i_name_width,iHeight);
	m_edit_name->Init(5,5,RE_ColorBLACK);
	m_edit_name->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_edit_name->SetCrossIndex(0);
	m_edit_name->setTextOff(2,2);
	m_edit_name->setCrossOff(4);
	m_edit_name->showBK(FALSE);
	m_edit_name->SetFontSize(i_font_size);

	m_edit_mem = KEditView_PTR(new KEditView);
	m_edit_mem->Create(m_edit_name->GetRect().right() + i_interstice,5,i_people_width,iHeight);
	m_edit_mem->Init(5,5,RE_ColorBLACK);
	m_edit_mem->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_edit_mem->SetCrossIndex(0);
	m_edit_mem->setTextOff(2,2);
	m_edit_mem->setCrossOff(4);
	m_edit_mem->showBK(FALSE);
	m_edit_mem->SetFontSize(i_font_size);
	m_edit_mem->setReadonly(true);

	m_edit_time = KEditView_PTR(new KEditView);
	m_edit_time->Create(m_edit_mem->GetRect().right() + i_interstice,5,i_time_width,iHeight);
	m_edit_time->Init(5,5,RE_ColorBLACK);
	m_edit_time->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_edit_time->SetCrossIndex(0);
	m_edit_time->setTextOff(2,2);
	m_edit_time->setCrossOff(4);
	m_edit_time->showBK(FALSE);
	m_edit_time->SetFontSize(i_font_size);

	m_edit_state = KEditView_PTR(new KEditView);
	m_edit_state->Create(m_edit_time->GetRect().right() + i_interstice,5,i_state_width,iHeight);
	m_edit_state->Init(5,5,RE_ColorBLACK);
	m_edit_state->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_edit_state->SetCrossIndex(0);
	m_edit_state->setTextOff(2,2);
	m_edit_state->setCrossOff(4);
	m_edit_state->showBK(FALSE);
	m_edit_state->SetFontSize(i_font_size);

	m_edit_state->setReadonly(true);


	AddView(m_edit_name);
	AddView(m_edit_mem);
	AddView(m_edit_time);
	AddView(m_edit_state);


	KDrawable_PTR del_n =  KDrawable_PTR(new KImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/list_btn_delete_n.png")),true));
	KDrawable_PTR del_a = KDrawable_PTR(new KImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/list_btn_delete_a.png")),true));
	RERect rect_del = del_a->GetRect();
	m_btn_delete = KImgButtonView_PTR(new KImgButtonView);
	m_btn_delete->setBKG(del_n,del_a,del_n,del_n);
	//m_btn_delete->SetOpacity(0x99);
	m_btn_delete->Create(rect_del);
	m_btn_delete->m_clicked_signal.connect(this,&NTaskListItem::btnDel);
	m_btn_delete->SetShow(FALSE);
	AddView(m_btn_delete);



}

void NTaskListItem::setValue( kn_string str_name,kn_string str_people,kn_string str_time,kn_string str_state )
{
	m_btn_delete->SetPosition(m_rect.right() - 10 - m_btn_delete->GetRect().width(),(m_rect.height() - m_btn_delete->GetRect().height()) / 2);
	m_edit_name->SetText(str_name);
	m_edit_mem->SetText(str_people);
	m_edit_time->SetText(str_time);
	m_edit_state->SetText(str_state);


}

void NTaskListItem::onMoveDirect(kn_int x, kn_int y, KMessageMouse* pMsg)
{

	//if (isPointInView(x,y))
	//{
	//	if (!m_btn_delete->IsShown())
	//	{
	//		m_btn_delete->SetShow(TRUE);
	//	}
	//}
	//else 
	//{
	//	if (m_btn_delete->IsShown() )
	//	{
	//		m_btn_delete->SetShow(FALSE);
	//	}
	//}

	//m_p_parent->InvalidateView();
}

void NTaskListItem::btnDel(KView_PTR p)
{
	if (m_i_card_id != -1)
	{
		m_sign_del_card.emit(m_i_card_id);
	}
}

void NTaskListItem::setTaskCardID( kn_int i )
{
	m_i_card_id = i;
}
kn_int NTaskListItem::getTaskCardID()
{
	return m_i_card_id;
}
kn_string NTaskListItem::getTaskCardName()
{
	return m_edit_name->getText();
}

vector<kn_string> NTaskListItem::getMember()
{
	return splitString2Vect(m_edit_mem->getText().c_str());
}

vector<kn_int>	NTaskListItem::GetTaskTime()
{
	vector<kn_int> vec_int;
	kn_string str_time = m_edit_time->getText();
	kn_int i_day = str_time.find(_T("d"));
	if (i_day == -1)
	{
		vec_int.push_back(0);
	}
	else
	{
		vec_int.push_back(_wtoi(str_time.substr(0,i_day).c_str()));
	}


	kn_int i_h = str_time.find(_T("h"));

	if (i_h == -1)
	{
		vec_int.push_back(0);
	}
	else
	{
		vec_int.push_back(_wtoi(str_time.substr(i_day + 1,i_h).c_str()));
	}

	
	return vec_int;
}

void NTaskListItem::updateDrawable( KDrawable_PTR p )
{
	if (m_lst_drawable[0] != p)
	{
		m_lst_drawable[0] = p;
	}
}
void NTaskListItem::showActive(kn_bool b)
{
	if (m_bg_drawable->isShow() != b)
	{
		m_bg_drawable->SetShow(b);
	}

	m_edit_name->SetTextColor(b?0xff539b48:0xff666666);
	m_edit_mem->SetTextColor(b?0xff539b48:0xff666666);
	m_edit_time->SetTextColor(b?0xff539b48:0xff666666);
	m_edit_state->SetTextColor(b?0xff539b48:0xff666666);
}

void NTaskListItem::setMember( const kn_string& str )
{
	m_edit_mem->SetText(str);
}

void NTaskListItem::onDragDirect( KMessageDrag* msg )
{
	
	kn_int i_x = msg->m_pos_x;
	kn_int i_y = msg->m_pos_y;
	if (msg)
	{
		kn_int ix,iy,lx,ly;
		ix = msg->m_pos_x;
		iy = msg->m_pos_y;
		GetScreenXY(lx,ly);

		if (msg->m_drag_type == TASK_GROUP_DRAG_UP)
		{	
			if ((ix > lx && ix < lx + m_rect.width()&&
				iy > ly && iy < ly + m_rect.height()))
			{
				kn_string str;
				(VIEW_PTR(NStaffView)(msg->m_p_drag_view))->GetName(str);
				setMember(str);
				showActive(false);
			}
		}
		if (msg->m_drag_type == TASK_GROUP_DRAG)
		{
			showActive((ix > lx && ix < lx + m_rect.width()&&
				iy > ly && iy < ly + m_rect.height()));
		}

		InvalidateView();
	}
}

void NTaskListItem::showDelete( kn_bool b )
{
	m_btn_delete->SetShow(b);
}
