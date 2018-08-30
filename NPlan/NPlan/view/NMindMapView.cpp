//  File:        NMindMapView.cpp
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
#include "NMindMapView.h"
#include "KSurfaceManager.h"
#include "KTextMultiLineDrawable.h"
#include "K9PatchImageDrawable.h"
#include "boost/lexical_cast.hpp"
#include "KSurfaceManager.h"
#include "KTextMultiLineDrawable.h"
#include "KScreen.h"
#include "NDragStaffView.h"
#include "KScreen.h"
#include "mainforwindows.h"
#include "NTaskSourceView.h"
#include "../file/utility.h"
#include "KFontManager.h"
#include "NMenu.h"

const static int prj_w = 140;
const static int prj_h = 80;
const static kn_int task_w = 120;
const static kn_int task_h = 75;
const static kn_int action_w = 98;
const static kn_int action_h = 68;

//////////////////////NMindMapActionView ////////////////////
NMindMapActionView::NMindMapActionView()
{
	m_b_edit = false;
	m_own_index = 0;
}

NMindMapActionView::~NMindMapActionView()
{
}

void NMindMapActionView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		// 设置区域
		m_own_index = GetParent()->findViewIndex(shared_from_this());
		GetParent()->changeViewLayerTop(shared_from_this());
		GetParent()->GetParent()->changeViewLayerTop(GetParent());
		GetParent()->GetParent()->GetParent()->changeViewLayerTop(GetParent()->GetParent());
		pMsg->setIdle(KMSG_RETURN_DILE);
	}
}

void NMindMapActionView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		// 还原原来的位置
		for(kn_int index = GetParent()->getViewCount() - 1; index >= m_own_index; --index)
		{
			GetParent()->swapViewLayer(shared_from_this(), GetParent()->getViewByIndex(index));
		}

		CActionView::OnUp(iScreenX, iScreenY, pMsg);
		// 释放聚焦控件
		KScreen* p =  GetScreen();
		KView_PTR v_null;

		if(p)
		{
			p->setFocusView(v_null, FALSE);
		}
	}
}

void NMindMapActionView::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	CMenuMindMap_PTR menu = CMenuMindMap_PTR(new CMenuMindMap());
	menu->Create(0, 0, 140, 115);
	menu->SetScreen(GetScreen());
	menu->init(pMsg->m_p_view, MIND_ACTION_MENU);
	menu->m_sign_menu_view_click.connect(this, &NMindMapActionView::OnMindActionMenu);
	//showmenu 传入view自身的相对坐标
	showMenu(menu, iScreenX - m_rect.left(), iScreenY - m_rect.top());
	InvalidateView();
}

void NMindMapActionView::Init(CNProjectTaskCard* p, kn_uint row)
{
	CActionView::init(p, row);
	m_bk_a = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/action_bg_a.9.png")), TRUE));
	m_bk_a->setRect(-2, -2, m_rect.width() + 5, m_rect.height() + 5);
	addDrawable(m_bk_a);
	m_bk_a->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_a->SetShow(false);
	m_onfoucs_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/prj_edit_select_bk.9.png")), true));
	m_onfoucs_drawable->setRect(-3, -3, m_rect.width() + 3 * 2, m_rect.height() + 3 * 2);
	addDrawable(m_onfoucs_drawable);
	m_onfoucs_drawable->SetShow(false);

}

void NMindMapActionView::shared_ptr_inited()
{
	CActionView::shared_ptr_inited();
	m_clicked_signal.connect(this, &NMindMapActionView::OnClickEvent);
	m_sign_key_up.connect(this, &NMindMapActionView::onKeyUp);
	m_sign_dclick.connect(this, &NMindMapActionView::OnEditAction);
}


void NMindMapActionView::showOnFoucsDrawable()
{
	m_onfoucs_drawable->SetShow(true);
	InvalidateView();
}

void NMindMapActionView::OnClickEvent(KView_PTR)
{
	setViewFocus();
	showOnFoucsDrawable();
}

void NMindMapActionView::onKeyUp(KMessageKey* kMsg)
{
	if(m_onfoucs_drawable->isShow() && bViewFocus())
	{
		//键盘消息
		kMsg->setIdle(KMSG_RETURN_DILE);
		int key = kMsg->m_key_id;

		if(key == Key_deleteKey)
		{
			onDeleteAction();
			m_onfoucs_drawable->SetShow(false);
		}
	}
}

void NMindMapActionView::onDeleteAction()
{
	if(m_b_edit)
	{
		return;
	}

	CNProjectTaskCard* p_data = getData();

	if(p_data)
	{
		getTask()->delTaskCard(*p_data);
		// 改变view的位置
		GetParent()->AddViewToDel(shared_from_this());
		GetScreen()->sendNUIMessage(PROJECT_EDIT_UPDATE_POS);
	}
}

void NMindMapActionView::hideOnFoucsDrawable()
{
	m_onfoucs_drawable->SetShow(false);
	InvalidateView();
}

void NMindMapActionView::setActionBkAShow(kn_bool bShow)
{
	m_bk_a->SetShow(bShow);
	InvalidateView();
}

void NMindMapActionView::OnMemDragUpIn(KView_PTR v)
{
	NDragStaffView_PTR drag_view = VIEW_PTR(NDragStaffView)(v);

	if(m_p_action && drag_view)
	{
		m_p_action->setLeaderId(drag_view->GetId());
		m_p_action->setPrjModify();
		updateLeaderInfo();
	}
}

void NMindMapActionView::OnEditAction(kn_int x, kn_int y, KMessageMouse* KMsg)
{
	kn_int mouse_x = x - m_rect.left();
	kn_int mouse_y = y - m_rect.top();
	// 如果双击区域是name区域
	RERect name_rect = m_p_name->GetRect();
	RERect time_rect = m_txt_date->GetRect();

	if((mouse_x > name_rect.left()) && (mouse_x < name_rect.right())
	   && (mouse_y > name_rect.top()) && (mouse_y < name_rect.bottom()))
	{
		ShowEditActionName();
	}
	// 如果双击区域是time区域
	else if((mouse_x > time_rect.left()) && (mouse_x < time_rect.right())
	        && (mouse_y > time_rect.top()) && (mouse_y < time_rect.bottom()))
	{
		ShowEditActionTime();
	}
}

void NMindMapActionView::ShowEditActionName()
{
	RERect name_edit_rect = RERect::MakeXYWH(GetPosition().x() + m_p_name->GetRect().left(), GetPosition().y() + m_p_name->GetRect().top() - 5, m_p_name->GetRect().width(), m_p_name->GetRect().height() + 5);
	m_name_edit_sign.emit(shared_from_this(), name_edit_rect, wipeOutStringLineBreak(m_p_action->getName()));
	m_b_edit = true;
}


void NMindMapActionView::ShowEditActionTime()
{
	RERect time_edit_rect = RERect::MakeXYWH(GetPosition().x() + m_txt_date->GetRect().left(), GetPosition().y() + m_txt_date->GetRect().top(), m_txt_date->GetRect().width(), m_txt_date->GetRect().height());
	kn_string str;

	if(m_p_action->getPlanDays() > 0)
	{
		str += boost::lexical_cast<kn_string>(m_p_action->getPlanDays());
		str += _T("d");
	}

	if(m_p_action->getPlanHours() > 0)
	{
		str += boost::lexical_cast<kn_string>(m_p_action->getPlanHours());
		str += _T("h");
	}

	m_time_edit_sign.emit(shared_from_this(), time_edit_rect, wipeOutStringLineBreak(str));
	m_b_edit = true;
}


void NMindMapActionView::OnEditActionNameOver(kn_string edit_str)
{
	m_b_edit = false;

	if(m_p_name->GetText() == edit_str || (edit_str == _T("")))
	{
		return;
	}

	m_p_action->setName(edit_str.c_str());
	m_p_action->setPrjModify();
	m_p_name->SetText(edit_str);
	IRESurface* sur = getSurfaceManager()->GetSurface(getBgColorByID(m_p_action->getLeaderId()));

	if(!sur)
	{
		return;
	}

	//取得图片主题色
	REColor bk_clr = sur->getColor(sur->GetWidth() / 2, 4);

	//使用了icon的drawable [1]位置
	if(m_p_name->getLine() > 1)
	{
		KColorDrawable_PTR txt_bk_drawable = KColorDrawable_PTR(new KColorDrawable(bk_clr));
		int h = min(44, m_p_name->getLine() * 16);
		txt_bk_drawable->setRect(1, 5, m_rect.width() - 1, h);
		m_lst_drawable[1] = txt_bk_drawable;
	}
}

void NMindMapActionView::OnEditActionTimeOver(kn_string edit_str)
{
	m_b_edit = false;

	if(m_txt_date->GetText() == edit_str || (edit_str == _T("")))
	{
		return;
	}

	vector<kn_int> vec_int;
	kn_int i_day = edit_str.find(_T("d"));

	if(i_day == -1)
	{
		vec_int.push_back(0);
	}
	else
	{
		vec_int.push_back(_wtoi(edit_str.substr(0, i_day).c_str()));
	}

	kn_int i_h = edit_str.find(_T("h"));

	if(i_h == -1)
	{
		vec_int.push_back(0);
	}
	else
	{
		vec_int.push_back(_wtoi(edit_str.substr(i_day + 1, i_h).c_str()));
	}

	if(vec_int.size() > 1)
	{
		m_p_action->setPlanDays(vec_int[0]);
		m_p_action->setPlanHours(vec_int[1]);
		m_p_action->setPrjModify();
		kn_string day_str = _T("");
		kn_string hour_str = _T("");

		if(vec_int[0] != 0)
		{
			day_str =  boost::lexical_cast<kn_string>(vec_int[0]) + _T("d");
		}

		if(vec_int[1] != 0)
		{
			hour_str =  boost::lexical_cast<kn_string>(vec_int[1]) + _T("h");
		}

		kn_string time_str = day_str + hour_str;

		if(time_str == _T(""))
		{
			time_str = _T("0h");
		}

		m_txt_date->SetText(time_str);
	}
}

void NMindMapActionView::OnMindActionMenu(KView_PTR v, kn_int id)
{
	switch(id)
	{
	case 1:
		ShowEditActionName();
		break;

	case 2:
		ShowEditActionTime();
		break;

	case 3:
		onDeleteAction();
		break;

	default:
		break;
	}
}


//////////////////////NMindMapItemAction /////////////////////
NMindMapBoardView::NMindMapBoardView()
{
}

NMindMapBoardView::~NMindMapBoardView()
{
}

void NMindMapBoardView::init(CNProjectTask* p, kn_uint row)
{
	if(p == NULL)
	{
		return;
	}

	m_row = row;
	m_p_task = p;
	kn_int color_type = m_p_task->getColorType();
	kn_string bk_file =  _T("./resource/edit_board0");
	bk_file += boost::lexical_cast<kn_string>(color_type);
	bk_file += _T(".9.png");
	m_bk = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(bk_file), TRUE));
	m_bk->setRect(0, 0, m_rect.width(), m_rect.height());
	addDrawable(m_bk);
	m_bk_a = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/taskboard_bka.9.png")), TRUE));
	m_bk_a->setRect(0, 0, m_rect.width(), m_rect.height());
	addDrawable(m_bk_a);
	m_bk_a->SetShow(FALSE);
	CNProjectTaskCard_LST& lst_action = m_p_task->getTaskCards();
	CNProjectTaskCard_LST::iterator it;

	for(it = lst_action.begin(); it != lst_action.end(); it++)
	{
		NMindMapActionView_PTR action_view = NMindMapActionView_PTR(new NMindMapActionView);
		action_view->Create(RERect::MakeXYWH(0, 0 , action_w, action_h));
		action_view->Init(&(*it), m_row);
		AddView(action_view);
		action_view->m_name_edit_sign.connect(this, &NMindMapBoardView::OnEditActionName);
		action_view->m_time_edit_sign.connect(this, &NMindMapBoardView::OnEditActionTime);
		action_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	}

	calcActionPos();
}

void NMindMapBoardView::OnMenDragUpIn(KMessageDrag* mag)
{
	// 转换成NMindMapItemAction的相对坐标
	REPoint pos_point;
	pos_point.set(mag->m_pos_x - m_rect.left(), mag->m_pos_y - m_rect.top());

	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];
		NMindMapActionView_PTR task_bar = VIEW_PTR(NMindMapActionView)(v);
		task_bar->setActionBkAShow(false);

		if(task_bar->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			if(task_bar->isPointInView(pos_point.x(), pos_point.y()))
			{
				task_bar->OnMemDragUpIn(mag->m_p_drag_view);
				break;
			}
		}
	}
}

void NMindMapBoardView::OnMenDragIn(KMessageDrag* mag)
{
	// 转换成NMindMapItemAction的相对坐标
	REPoint pos_point;
	pos_point.set(mag->m_pos_x - m_rect.left(), mag->m_pos_y - m_rect.top());

	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];

		if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			NMindMapActionView_PTR task_bar = VIEW_PTR(NMindMapActionView)(v);

			if(task_bar->isPointInView(pos_point.x(), pos_point.y()))
			{
				task_bar->setActionBkAShow(true);
			}
			else
			{
				task_bar->setActionBkAShow(false);
			}
		}
	}
}

void NMindMapBoardView::OnMenDragOut(KMessageDrag* mag)
{
	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];

		if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			NMindMapActionView_PTR task_bar = VIEW_PTR(NMindMapActionView)(v);
			task_bar->setActionBkAShow(false);
		}
	}
}

void NMindMapBoardView::setShowBkA(kn_bool bShow)
{
	m_bk_a->SetShow(bShow);
	InvalidateView();
}

kn_bool NMindMapBoardView::isPointInActionView(kn_int x, kn_int y)
{
	// 转换成NMindMapItemAction的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	kn_bool bIn = false;

	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];

		if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			if(v->isPointInView(pos_point.x(), pos_point.y()))
			{
				bIn = true;
				break;
			}
		}
	}

	return bIn;
}

void NMindMapBoardView::calcActionPos()
{
	RERect rect;
	int n = 0;

	if(m_lst_view.size() == 0)
	{
		return;
	}
	else
	{
		int count = m_rect.width() / m_lst_view[0]->GetRect().width();

		if(count == 0)
		{
			return;
		}

		for(kn_int i = 0; i < m_lst_view.size(); i ++)
		{
			KView_PTR v = m_lst_view[i];

			if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
			{
				rect = v->GetRect();
				int wblank = (m_rect.width() - count * (rect.width() + 4)) / 2 + 2;
				int x = n % count * (rect.width() + 4)  + wblank;
				int y = n / count * (rect.height() + 3) + 4;
				v->SetPosition(x, y);
				n++;
			}
		}
	}
}

void NMindMapBoardView::onDragDirect(KMessageDrag* mag)
{
	return;
}

void NMindMapBoardView::OnEditActionName(KView_PTR v, RERect rect, kn_string str)
{
	m_edit_action_view = VIEW_PTR(NMindMapActionView)(v);

	if(!m_name_edit)
	{
		m_name_edit = KEditView_PTR(new KEditView());
		m_name_edit->Create(rect);
		m_name_edit->Init(0, 5, RE_ColorBLACK);
		m_name_edit->SetCrossIndex(0);
		//设置点击鼠标光标定位点坐标，相当于KEditView_PTR的坐标
		m_name_edit->SetTextColor(RE_ColorBLACK);
		m_name_edit->setTextOff(2, 2);
		m_name_edit->SetFontSize(12);
		m_name_edit->setMultiLine(true);
		m_name_edit->setReadonly(false);
		m_name_edit->setCrossOff(3);
		KViewGroup::AddView(m_name_edit);
		m_name_edit->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	}

	m_name_edit->SetRect(rect);
	m_name_edit->SetText(str);
	m_name_edit->SetAllSelect(true);
	m_name_edit->setViewFocus();
	AddViewToDel(m_time_edit);
	m_time_edit.reset();
	UpdateUI();
}

void NMindMapBoardView::OnEditActionTime(KView_PTR v, RERect rect, kn_string str)
{
	m_edit_action_view = VIEW_PTR(NMindMapActionView)(v);

	if(!m_time_edit)
	{
		m_time_edit = KEditView_PTR(new KEditView());
		m_time_edit->Create(rect);
		m_time_edit->Init(5, 2, RE_ColorBLACK);
		m_time_edit->SetCrossIndex(0);
		//设置点击鼠标光标定位点坐标，相当于KEditView_PTR的坐标
		m_time_edit->setTextOff(2, 2);
		m_time_edit->SetFontSize(12);
		m_time_edit->setReadonly(false);
		m_time_edit->setCrossOff(2);
		KViewGroup::AddView(m_time_edit);
		m_time_edit->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	}

	m_time_edit->SetRect(rect);
	m_time_edit->SetText(str);
	m_time_edit->SetAllSelect(true);
	m_time_edit->setViewFocus();
	AddViewToDel(m_name_edit);
	m_name_edit.reset();
	UpdateUI();
}


void NMindMapBoardView::OnAddAction(kn_int row)
{
	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];

		if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			NMindMapActionView_PTR action_view = VIEW_PTR(NMindMapActionView)(v);
			action_view->setRow(row);
		}
	}

	CNProjectTaskCard_LST& lst_action = m_p_task->getTaskCards();
	CNProjectTaskCard_LST::iterator it = lst_action.end();
	--it;
	NMindMapActionView_PTR add_action = NMindMapActionView_PTR(new NMindMapActionView);
	add_action->Create(RERect::MakeXYWH(0, 0 , action_w, action_h));
	add_action->Init(&(*it), row);
	AddView(add_action);
	add_action->m_name_edit_sign.connect(this, &NMindMapBoardView::OnEditActionName);
	add_action->m_time_edit_sign.connect(this, &NMindMapBoardView::OnEditActionTime);
	add_action->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	add_action->showOnFoucsDrawable();
	m_edit_action_view = add_action;
}

void NMindMapBoardView::OnMouseDown(kn_int x, kn_int y)
{
	// 转换成NMindMapItemAction的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());

	// 不在name_edit区域
	if(m_name_edit && (!m_name_edit->isPointInView(pos_point.x(), pos_point.y())))
	{
		if(m_edit_action_view)
		{
			m_edit_action_view->OnEditActionNameOver(wipeOutStringLineBreak(m_name_edit->getText()));
		}

		// 释放聚焦控件
		KScreen* p =  GetScreen();
		KView_PTR v_null;

		if(p)
		{
			p->setFocusView(v_null, FALSE);
		}

		AddViewToDel(m_name_edit);
		m_name_edit.reset();
	}

	// 不在time_edit区域
	if(m_time_edit && (!m_time_edit->isPointInView(pos_point.x(), pos_point.y())))
	{
		if(m_edit_action_view)
		{
			m_edit_action_view->OnEditActionTimeOver(wipeOutStringLineBreak(m_time_edit->getText()));
		}

		// 释放聚焦控件
		KScreen* p =  GetScreen();
		KView_PTR v_null;

		if(p)
		{
			p->setFocusView(v_null, FALSE);
		}

		AddViewToDel(m_time_edit);
		m_time_edit.reset();
	}

	for(kn_int i = 0; i < m_lst_view.size(); i ++)
	{
		KView_PTR v = m_lst_view[i];

		if(v->getType() == (KVIEWTYPE) NPLAN_ACTION_VIEW)
		{
			if(!v->isPointInView(pos_point.x(), pos_point.y()))
			{
				NMindMapActionView_PTR mind_action = VIEW_PTR(NMindMapActionView)(v);
				mind_action->hideOnFoucsDrawable();
			}
		}
	}
}

NMindMapActionView_PTR NMindMapBoardView::getEditActionView()
{
	return m_edit_action_view;
}

///////////////////////////////////////////
NMindMapItemBase::NMindMapItemBase()
{

}
void NMindMapItemBase::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_clicked_signal.connect(this, &NMindMapItemBase::onClickEvent);
}


NMindMapItemBase::~NMindMapItemBase()
{
}

void NMindMapItemBase::addItem(NMindMapItemBase_PTR p)
{
	m_item_lst.push_back(p);
	AddView(p);
}

void NMindMapItemBase::calcPos()
{
}

int NMindMapItemBase::getItemHeight()
{
	return 1;
}

void NMindMapItemBase::setBkAShow(kn_bool bShow)
{
	m_bk_a->SetShow(bShow);
	InvalidateView();
}

void NMindMapItemBase::showOnFoucsDrawable()
{
	m_onfoucs_drawable->SetShow(true);
	InvalidateView();
}

void NMindMapItemBase::hideOnFoucsDrawable()
{
	m_onfoucs_drawable->SetShow(false);
	InvalidateView();
}

void NMindMapItemBase::onClickEvent(KView_PTR v)
{
	setViewFocus();
	showOnFoucsDrawable();
}

//////////////////////NMindMapItemTask /////////////////////
NMindMapItemTask::NMindMapItemTask()
{
	m_p_task = NULL;
	m_b_left = TRUE;
}

NMindMapItemTask::~NMindMapItemTask()
{
}

void NMindMapItemTask::setLeft(BOOL b)
{
	m_b_left = b;
}

void NMindMapItemTask::init(CNProjectTask* p)
{
	//task 项目里面包含 taskview控件和board控件
	if(p == NULL)
	{
		return;
	}

	m_p_task = p;
	m_task_view = CTaskView_PTR(new CTaskView);
	m_task_view->Create(RERect::MakeXYWH(0, 0, task_w, task_h));
	m_task_view->init(m_p_task);
	//	task_view->SetClip(RERect::MakeXYWH(0, 0 , task_w, task_h ));
	AddView(m_task_view);
	m_task_view->setBMove(false);
	m_bk_a = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/task_bg_a.9.png")), TRUE));
	m_bk_a->setRect(-3, -2, m_task_view->GetRect().width() + 3, m_task_view->GetRect().height() + 2);
	m_task_view->addDrawable(m_bk_a);
	m_bk_a->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_a->SetShow(false);

	if(m_b_left)
	{
		m_task_view->SetViewChangeType(KVIEW_LRMethod_Parent_Right,  KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	}
	else
	{
		m_task_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left,  KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	}

	m_leader = KStaticView_PTR(new KStaticView());
	m_leader->Create(36, task_h - 20, task_w - 55, 20);
	m_leader->SetTextColor(RE_ColorWHITE);
	m_leader->SetFontSize(13);
	m_leader->setTextAlign(REPaint::kRight_Align);
	AddView(m_leader);
	SetViewChangeType(KVIEW_LRMethod_Parent_Right,  KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_leader->SetText(m_p_task->getLeaderName());
	CNProjectTaskCard_LST& lst_action = m_p_task->getTaskCards();
	UINT count = lst_action.size();
	int row; //列数
	int col; //行数

	if(count <= 3)
	{
		row = count;
		col = 1;
	}
	else
	{
		row = sqrt((double)count);
		col = count / row;

		if(count % row != 0)
		{
			//除不尽 有余数
			col++;
		}
	}

	m_action_board = NMindMapBoardView_PTR(new NMindMapBoardView());
	m_action_board->Create(0, 0, task_w, task_h);
	m_action_board->init(m_p_task, row);
	AddView(m_action_board);
	m_action_board->SetViewChangeType(KVIEW_LRMethod_Parent_Left,  KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_child_line_drawable = KColorDrawable_PTR(new KColorDrawable(m_task_view->getBkColor()));
	m_child_line_drawable->setRect(0, 0, task_w, task_h);
	m_child_line_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Right,  KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	addDrawable(m_child_line_drawable);
	KLineShape* p_line_shape = new KLineShape();
	m_parient_line_drawable = KShapeDrawable_PTR(new KShapeDrawable(p_line_shape));
	m_parient_line_drawable->setRect(0, 0, task_w, task_h);
	addDrawable(m_parient_line_drawable);
	m_parient_line_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Right,  KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	m_onfoucs_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/prj_edit_select_bk.9.png")), true));
	m_onfoucs_drawable->setRect(0, 0, m_rect.width() + 2, m_rect.height());
	addDrawable(m_onfoucs_drawable);
	m_onfoucs_drawable->SetShow(false);

	calcPos();
}

void NMindMapItemTask::shared_ptr_inited()
{
	NMindMapItemBase::shared_ptr_inited();
	m_sign_key_up.connect(this, &NMindMapItemTask::onKeyUp);
	m_sign_dclick.connect(this, &NMindMapItemTask::OnEditTaskName);
}

int NMindMapItemTask::getItemHeight()  //返回当前项及子项的整体高度
{
	return m_action_board->GetRect().height() + 10;
}

void NMindMapItemTask::OnMemDragUpIn(KView_PTR v)
{
	setBkAShow(false);
	NDragStaffView_PTR staff_view = VIEW_PTR(NDragStaffView)(v);
	m_p_task->setLeaderId(staff_view->GetId());
	m_leader->SetText(staff_view->GetName());
}

void NMindMapItemTask::OnPointDragOut(KMessageDrag* mag)
{
	setBkAShow(false);
	// 转换成NMindMapItemTask的相对坐标
	REPoint pos_point;
	pos_point.set(mag->m_pos_x - m_rect.left(), mag->m_pos_y - m_rect.top());

	if(m_action_board->isPointInView(pos_point.x(), pos_point.y()))
	{
		if(mag->m_drag_type == STORYPAGE_ACTION_DRAG_UP)
		{
			OnActDragUpIn(mag->m_p_drag_view);
		}
		else if(mag->m_drag_type == STORYPAGE_ACTION_DRAG)
		{
			OnActDragIn(mag->m_p_drag_view);
		}
		else if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG_UP)
		{
			mag->m_pos_x = pos_point.x();
			mag->m_pos_y = pos_point.y();
			m_action_board->OnMenDragUpIn(mag);
		}
		else if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG)
		{
			mag->m_pos_x = pos_point.x();
			mag->m_pos_y = pos_point.y();
			m_action_board->OnMenDragIn(mag);
		}
	}
	else
	{
		if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG)
		{
			mag->m_pos_x = pos_point.x();
			mag->m_pos_y = pos_point.y();
			m_action_board->OnMenDragOut(mag);
		}

		m_action_board->setShowBkA(false);
	}
}

void NMindMapItemTask::OnActDragUpIn(KView_PTR v)
{
	setBkAShow(false);

	// 如果是当前board，则不变
	if(v->GetParent() == m_action_board)
	{
		return;
	}

	NMindMapActionView_PTR act_view = VIEW_PTR(NMindMapActionView)(v);
	CNProjectTaskCard* p_data = act_view->getData();

	if(p_data)
	{
		//改变data
		m_p_task->addTaskCard(*p_data);
		act_view->getTask()->delTaskCard(*p_data);
		act_view->setData(&(*(m_p_task->getTaskCards().rbegin())));
		// 改变view的位置
		act_view->m_name_edit_sign.disconnect_all();
		act_view->m_time_edit_sign.disconnect_all();
		v->GetParent()->AddViewToDel(v);
		m_action_board->AddView(v);
		act_view->m_name_edit_sign.connect(m_action_board.get(), &NMindMapBoardView::OnEditActionName);
		act_view->m_time_edit_sign.connect(m_action_board.get(), &NMindMapBoardView::OnEditActionTime);
	}
}

void NMindMapItemTask::calcPos()
{
	CNProjectTaskCard_LST& lst_action = m_p_task->getTaskCards();
	UINT count = lst_action.size();
	int row; //列数
	int col; //行数

	if(count <= 3)
	{
		row = count;
		col = 1;
	}
	else
	{
		row = sqrt((double)count);
		col = count / row;

		if(count % row != 0)
		{
			//除不尽 有余数
			col++;
		}
	}

	int aw = 102;
	int ah = 70;
	int board_y = task_h / 2 - (col * ah + 10) / 2;
	int board_w = 0;
	kn_int line_height = 1;
	m_child_line_drawable->SetShow(false);

	if(row != 0)
	{
		board_w = row * aw + 10;
		m_child_line_drawable->SetShow(true);
	}

	if(m_b_left)
	{
		m_action_board->SetRect(RERect::MakeXYWH(-board_w - 20,  board_y, board_w, col * ah + 10));
		m_child_line_drawable->setRect(m_action_board->GetRect().right(), m_action_board->GetRect().centerY(), 20, line_height);
	}
	else
	{
		m_action_board->SetRect(RERect::MakeXYWH(task_w + 20, board_y, board_w, col * ah + 10));
		m_child_line_drawable->setRect(m_rect.width(), m_rect.height() / 2, 20, line_height);
	}

	m_action_board->setShowBkA(false);
}

kn_bool NMindMapItemTask::isPointInActionView(kn_int x, kn_int y)
{
	// 转换成NMindMapItemTask的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	return m_action_board->isPointInActionView(pos_point.x(), pos_point.y());
}


kn_bool NMindMapItemTask::isPointInBoardView(kn_int x, kn_int y)
{
	// 转换成NMindMapItemTask的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());

	if(m_action_board->isPointInView(pos_point.x(), pos_point.y()))
	{
		return true;
	}

	return false;
}


void NMindMapItemTask::OnActDragIn(KView_PTR v)
{
	if(v->GetParent() != m_action_board)
	{
		m_action_board->setShowBkA(true);
		setBkAShow(true);
	}
}

void NMindMapItemTask::adjustConnectParientLinePos(const RERect& par_rect)
{
	KLineShape* p_line_shape = new KLineShape();
	kn_int drawable_left;
	kn_int drawable_top;
	kn_int drawable_width;
	kn_int drawable_height;
	kn_int stroke_width = 1;

	if(m_b_left)
	{
		drawable_left = min(m_rect.width(), par_rect.left());
		drawable_top = min(m_rect.height() / 2, par_rect.top() + par_rect.height() / 2) - stroke_width;
		drawable_width = abs(par_rect.left() - m_rect.width());
		drawable_height = abs(par_rect.top() + par_rect.height() / 2 - m_rect.height() / 2) + 2 * stroke_width;
		p_line_shape->SetStartPoint(REPoint::Make(m_rect.width() - drawable_left, m_rect.height() / 2 - drawable_top));
		p_line_shape->SetEndPoint(REPoint::Make(par_rect.left()  - drawable_left - 2, par_rect.top() + par_rect.height() / 2 - drawable_top));
	}
	else
	{
        drawable_left = min(0, (int)par_rect.right());
        drawable_top = min((int)m_rect.height() / 2, int(par_rect.top() + par_rect.height() / 2)) - stroke_width;
		drawable_width = abs(par_rect.right() - 0);
		drawable_height = abs(par_rect.top() + par_rect.height() / 2 - m_rect.height() / 2) + stroke_width * 2;
		p_line_shape->SetStartPoint(REPoint::Make(0  - drawable_left, m_rect.height() / 2 - drawable_top));
		p_line_shape->SetEndPoint(REPoint::Make(par_rect.right()  - drawable_left + 2, par_rect.top() + par_rect.height() / 2 - drawable_top));
	}

	REColor clor = m_task_view->getBkColor();
	p_line_shape->setFillColor(clor);
	p_line_shape->setFillOpacity(0);
	p_line_shape->setStrokeColor(clor);
	p_line_shape->setStrokeWidth(stroke_width);
	p_line_shape->initPaint();
	m_parient_line_drawable->setShape(p_line_shape);
	m_parient_line_drawable->setRect(drawable_left, drawable_top, drawable_width, drawable_height);
}

void NMindMapItemTask::OnEditTaskName(kn_int, kn_int, KMessageMouse*)
{
	ShowEditTaskName();
}

void NMindMapItemTask::ShowEditTaskName()
{
	m_edit_name = KEditView_PTR(new KEditView());
	m_edit_name->Create(36 , 10,  task_w - 55, task_h - 32);
	m_edit_name->Init(0, 2, RE_ColorBLACK);
	m_edit_name->SetCrossIndex(0);
	//设置点击鼠标光标定位点坐标，相当于KEditView_PTR的坐标
	m_edit_name->setTextOff(2, 2);
	//光标线的长度
	m_edit_name->setMultiLine(true);
	m_edit_name->setReadonly(false);
	m_edit_name->setCrossOff(3);
	m_edit_name->SetText(m_p_task->getName());
	AddView(m_edit_name);
	m_edit_name->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_edit_name->SetAllSelect(true);
	m_edit_name->setViewFocus();
	UpdateUI();
}

void NMindMapItemTask::OnMouseDown(kn_int x, kn_int y)
{
	if(!isPointInView(x, y))
	{
		if(m_onfoucs_drawable->isShow())
		{
			hideOnFoucsDrawable();
		}
	}

	// 转换成NMindMapItemTask的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	m_action_board->OnMouseDown(pos_point.x(), pos_point.y());

	if(m_edit_name)
	{
		if(!m_edit_name->isPointInView(pos_point.x(), pos_point.y()))
		{
			if(m_edit_name->isShow())
			{
				OnEditTaskNameOver(wipeOutStringLineBreak(m_edit_name->getText()));
				// 释放聚焦控件
				KScreen* p =  GetScreen();
				KView_PTR v_null;

				if(p)
				{
					p->setFocusView(v_null, FALSE);
				}

				AddViewToDel(m_edit_name);
				m_edit_name.reset();
				InvalidateView();
			}
		}
	}
}

void NMindMapItemTask::onKeyUp(KMessageKey* kMsg)
{
	int key = kMsg->m_key_id;

	if(m_onfoucs_drawable->isShow() && bViewFocus())
	{
		//键盘消息
		kMsg->setIdle(KMSG_RETURN_DILE);
		

		if(key == Key_insertKey)
		{
			onAddAction();
			m_onfoucs_drawable->SetShow(false);
		}
		else if(key == Key_deleteKey)
		{
			// todo:删除选中任务（正在编辑无效）
			if(!m_edit_name)
			{
				onDelTask();
				m_onfoucs_drawable->SetShow(false);
			}
		}
	}
}

void NMindMapItemTask::onAddAction()
{
	m_p_task->addTaskCard(_T("Action"), m_p_task->getLeaderId(), 1, 0, 0, 0);
	CNProjectTaskCard_LST& vec_task_card = m_p_task->getTaskCards();
	CNProjectTaskCard_LST::reverse_iterator it = vec_task_card.rbegin();
	(*it).setStartTime(m_p_task->getStartTime());
	(*it).setEndTime(m_p_task->getEndTime());
	CNProjectTaskCard_LST& lst_action = m_p_task->getTaskCards();
	UINT count = lst_action.size();
	int row; //列数
	int col; //行数

	if(count <= 3)
	{
		row = count;
		col = 1;
	}
	else
	{
		row = sqrt((double)count);
		col = count / row;

		if(count % row != 0)
		{
			//除不尽 有余数
			col++;
		}
	}

	m_action_board->OnAddAction(row);
	m_action_board->SetViewChangeType(KVIEW_LRMethod_Parent_Right,  KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	m_update_pos_sign.emit();
	// 调整位置，避免其在屏幕外
	NMindMapActionView_PTR add_action = m_action_board->getEditActionView();
	//计算出相当于NMindMapArea的坐标，方便移动
	kn_int view_left = add_action->GetRect().left() + m_action_board->GetRect().left() + m_rect.left() + GetParent()->GetRect().left();
	kn_int view_top = add_action->GetRect().top() + m_action_board->GetRect().top() + m_rect.top() + GetParent()->GetRect().top();
	kn_int view_right = view_left + add_action->GetRect().width();
	kn_int view_bottom = view_top + add_action->GetRect().height();
	m_rect_adjust_pos_sign.emit(RERect::MakeLTRB(view_left, view_top, view_right, view_bottom));
	// 需在位置调整完成后显示
	add_action->ShowEditActionName();
}

void NMindMapItemTask::onDelTask()
{
	m_del_task.emit(m_p_task, shared_from_this());
}

void NMindMapItemTask::OnEditTaskNameOver(kn_string edit_str)
{
	if(m_task_view->getNameText() == edit_str || (edit_str == _T("")))
	{
		return;
	}

	m_task_view->setNameText(edit_str);
	m_p_task->setName(edit_str.c_str());
	m_p_task->setModify(true);
}

void NMindMapItemTask::refreshTaskInfo()
{
	m_task_view->refreshInfo();
}

void NMindMapItemTask::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	CMenuMindMap_PTR menu = CMenuMindMap_PTR(new CMenuMindMap());
	menu->Create(0, 0, 140, 115);
	menu->SetScreen(GetScreen());
	menu->init(pMsg->m_p_view, MIND_TASK_MENU);
	menu->m_sign_menu_view_click.connect(this, &NMindMapItemTask::OnMindTaskMenu);
	//showmenu 传入view自身的相对坐标
	showMenu(menu, iScreenX - m_rect.left(), iScreenY - m_rect.top());
	InvalidateView();
}

void NMindMapItemTask::OnMindTaskMenu(KView_PTR v, kn_int id)
{
	switch(id)
	{
	case 1:
		onAddAction();
		break;

	case 2:
		ShowEditTaskName();
		break;

	case 3:
		onDelTask();
		break;

	default:
		break;
	}
}

//////////////////////NMindMapItemPrj /////////////////////
NMindMapItemPrj::NMindMapItemPrj()
{
	m_p_prj = NULL;
}

NMindMapItemPrj::~NMindMapItemPrj()
{
}

void NMindMapItemPrj::shared_ptr_inited()
{
	NMindMapItemBase::shared_ptr_inited();
	m_sign_key_up.connect(this, &NMindMapItemPrj::onKeyUp);
	m_sign_dclick.connect(this, &NMindMapItemPrj::OnEditPrjName);
}

void NMindMapItemPrj::init(CNProjectData* p)
{
	if(p == NULL)
	{
		return;
	}

	m_p_prj = p;
	m_bk = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/prj_bg.9.png")), TRUE));
	m_bk->setRect(0, 0, m_rect.width(), m_rect.height());
	addDrawable(m_bk);
	m_bk->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_a = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/prj_bg_a.9.png")), TRUE));
	m_bk_a->setRect(-3, -2, m_rect.width() + 3 * 2, m_rect.height() + 2 * 2);
	addDrawable(m_bk_a);
	m_bk_a->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_a->SetShow(false);
	m_name_text_drawable = KTextMultiLineDrawable_PTR(new KTextMultiLineDrawable(RE_ColorWHITE, 18, REPaint::kLeft_Align));
	m_name_text_drawable->SetRect(RERect::MakeXYWH(5, 5, m_rect.width() - 5, m_rect.height() - 5));
	addDrawable(m_name_text_drawable);
	m_name_text_drawable->SetText(m_p_prj->getName());
	m_name_text_drawable->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_name_text_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_leader = KStaticView_PTR(new KStaticView());
	m_leader->Create(RERect::MakeXYWH(0, m_rect.height() * 2 / 3, m_rect.width() - 10, m_rect.height() / 3));
	AddView(m_leader);
	m_leader->SetText(m_p_prj->getLeader());
	m_leader->setTextAlign(REPaint::kRight_Align);
	m_leader->SetTextColor(RE_ColorWHITE);
	m_leader->SetFontSize(16);
	m_leader->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_onfoucs_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/prj_edit_select_bk.9.png")), true));
	m_onfoucs_drawable->setRect(-3, -3, m_rect.width() + 3 * 2, m_rect.height() + 3 * 2);
	addDrawable(m_onfoucs_drawable);
	m_onfoucs_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_onfoucs_drawable->SetShow(false);

	CNProjectTask_LST* lst_task = m_p_prj->getpTasks();
	CNProjectTask_LST::iterator it_task;
	int i = 0;

	for(it_task = lst_task->begin(); it_task != lst_task->end(); it_task++)
	{
		CreatTaskView(i, &(*it_task));
		i++;
	}

	calcPos();
}


NMindMapItemTask_PTR NMindMapItemPrj::CreatTaskView(kn_int iPos, CNProjectTask* p)
{
	NMindMapItemTask_PTR task_bar = NMindMapItemTask_PTR(new NMindMapItemTask());

	if(iPos % 2 == 0)
	{
		//双数在左
		task_bar->Create(RERect::MakeXYWH(-task_w - 50, 0, task_w, task_h));
		task_bar->setLeft(TRUE);
	}
	else
	{
		task_bar->Create(RERect::MakeXYWH(m_rect.width() + 50, 0, task_w, task_h));
		task_bar->setLeft(FALSE);
	}

	task_bar->init(p);
	AddView(task_bar);
	task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	task_bar->m_del_task.connect(this, &NMindMapItemPrj::OnDelTask);
	task_bar->m_update_pos_sign.connect(this, &NMindMapItemPrj::OnUpdatePos);
	task_bar->m_rect_adjust_pos_sign.connect(this, &NMindMapItemPrj::adjustPosByViewRect);
	m_item_lst.push_back(task_bar);
	return task_bar;
}


void NMindMapItemPrj::calcPos()
{
	std::list<NMindMapItemBase_PTR>::iterator it_task;
	int total_h_left = 0;
	int total_h_right = 0;
	int i = 0;

	//左右分开计算总高度
	for(it_task = m_item_lst.begin(); it_task != m_item_lst.end(); it_task++)
	{
		if(i % 2 == 0)
		{
			total_h_left += (*it_task)->getItemHeight();
		}
		else
		{
			total_h_right += (*it_task)->getItemHeight();
		}

		i++;
	}

	//相对原点(0,0)居中对齐
	//左右分开设置总高度
	int left_y  = -total_h_left / 2  + m_rect.height() / 2;
	int right_y = -total_h_right / 2 + m_rect.height() / 2;
	i = 0;

	for(it_task = m_item_lst.begin(); it_task != m_item_lst.end(); it_task++)
	{
		if(i % 2 == 0)
		{
			(*it_task)->SetPositionY(left_y + ((*it_task)->getItemHeight() - (*it_task)->GetRect().height()) / 2);
			left_y += (*it_task)->getItemHeight();
		}
		else
		{
			(*it_task)->SetPositionY(right_y + ((*it_task)->getItemHeight() - (*it_task)->GetRect().height()) / 2);
			right_y += (*it_task)->getItemHeight();
		}

		// 转换成相对于task的矩形坐标
		RERect parent_rect = RERect::MakeXYWH(-(*it_task)->GetRect().left(), -(*it_task)->GetRect().top(), m_rect.width(), m_rect.height());
		(*it_task)->adjustConnectParientLinePos(parent_rect);
		i++;
	}
}

void NMindMapItemPrj::OnMemDragUpIn(KView_PTR v)
{
	NDragStaffView_PTR staff_view = VIEW_PTR(NDragStaffView)(v);
	m_p_prj->setLeaderId(staff_view->GetId());
	m_leader->SetText(staff_view->GetName());
}

void NMindMapItemPrj::OnPointDragOut(KMessageDrag* mag)
{
	setBkAShow(false);
	// 换成NMindMapItemPrj的相对坐标
	REPoint pos_point;
	pos_point.set(mag->m_pos_x - m_rect.left(), mag->m_pos_y - m_rect.top());
	std::list<NMindMapItemBase_PTR>::iterator itr;

	for(itr = m_item_lst.begin(); itr != m_item_lst.end(); ++itr)
	{
		// 在TaskView里
		if((*itr)->isPointInView(pos_point.x(), pos_point.y()))
		{
			if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG_UP)
			{
				(*itr)->OnMemDragUpIn(mag->m_p_drag_view);
			}
			else if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG)
			{
				(*itr)->setBkAShow(true);
			}
			else if(mag->m_drag_type == STORYPAGE_ACTION_DRAG_UP)
			{
				(*itr)->OnActDragUpIn(mag->m_p_drag_view);
			}
			else if(mag->m_drag_type == STORYPAGE_ACTION_DRAG)
			{
				(*itr)->OnActDragIn(mag->m_p_drag_view);
			}

			break;
		}
		else
		{
			mag->m_pos_x = pos_point.x();
			mag->m_pos_y = pos_point.y();
			(*itr)->OnPointDragOut(mag);
		}
	}
}

void NMindMapItemPrj::OnActDragUpIn(KView_PTR v)
{
}

void NMindMapItemPrj::OnUpdatePos()
{
	std::list<NMindMapItemBase_PTR>::iterator itr;

	for(itr = m_item_lst.begin(); itr != m_item_lst.end(); ++itr)
	{
		VIEW_PTR(NMindMapItemTask)(*itr)->calcPos();
	}

	calcPos();
	UpdateUI();
}

void NMindMapItemPrj::adjustPosByViewRect(RERect rect_by_area)
{
	kn_int leave_length = 80;
	RERect area_rect = RERect::MakeXYWH(GetParent()->GetRect().left() + leave_length, GetParent()->GetRect().top() + leave_length, GetParent()->GetRect().width() - leave_length * 2, GetParent()->GetRect().height() - leave_length * 2);
	kn_int x_offset = 0;
	kn_int y_offset = 0;

	if(rect_by_area.right() > area_rect.right())
	{
		x_offset = rect_by_area.right() - area_rect.right();
	}
	else if(rect_by_area.left() < area_rect.left())
	{
		x_offset = rect_by_area.left() - area_rect.left();
	}

	if(rect_by_area.bottom() > area_rect.bottom())
	{
		y_offset = rect_by_area.bottom() - area_rect.bottom() + 80;
	}
	else if(rect_by_area.top() < area_rect.top())
	{
		y_offset = rect_by_area.top() - area_rect.top();
	}

	while(m_ani_thread.isPlaying())
	{
		Sleep(20);
	}

	m_ani_thread.addAnimationHelper(shared_from_this(), PropertyPARA_ID_POS_X, GetRect().left() - x_offset, 500, 0, 0, KEasingCurve::OutSine);
	m_ani_thread.addAnimationHelper(shared_from_this(), PropertyPARA_ID_POS_Y, GetRect().top() - y_offset, 500, 0, 0, KEasingCurve::OutSine);
	m_ani_thread.Start();
}

kn_bool NMindMapItemPrj::isPointInActionView(kn_int x, kn_int y)
{
	// 换成NMindMapItemPrj的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	kn_bool bIn = false;
	std::list<NMindMapItemBase_PTR>::iterator itr;

	for(itr = m_item_lst.begin(); itr != m_item_lst.end(); ++itr)
	{
		if((*itr)->isPointInActionView(pos_point.x(), pos_point.y()))
		{
			bIn = true;
			break;
		}
	}

	return bIn;
}


kn_bool NMindMapItemPrj::isPointInBlank(kn_int x, kn_int y)
{
	// 是否在prj上
	if(isPointInView(x, y))
	{
		return false;
	}
	// 换成NMindMapItemPrj的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	std::list<NMindMapItemBase_PTR>::iterator itr;

	for(itr = m_item_lst.begin(); itr != m_item_lst.end(); ++itr)
	{
		NMindMapItemTask_PTR task_view = VIEW_PTR(NMindMapItemTask)(*itr);

		// 是否在TaskView上
		if((*itr)->isPointInView(pos_point.x(), pos_point.y()))
		{
			return false;
			break;
		}
		// 是否在BoardView
		else if(task_view->isPointInBoardView(pos_point.x(), pos_point.y()))
		{
			return false;
			break;
		}
	}

	return true;
}

void NMindMapItemPrj::OnActDragIn(KView_PTR v)
{
}

void NMindMapItemPrj::adjustConnectParientLinePos(const RERect& par_rect)
{
}

void NMindMapItemPrj::OnEditPrjName(kn_int, kn_int, KMessageMouse*)
{
	ShowEditPrjName();
}

void NMindMapItemPrj::ShowEditPrjName()
{
	m_edit_name = KEditView_PTR(new KEditView());

    RERect rcEdit = RERect::MakeXYWH(5, 5, m_rect.width() - 10, m_rect.height() * 5 / 6);
    m_edit_name->Create(rcEdit);
	m_edit_name->Init(0, 5, RE_ColorBLACK);
	m_edit_name->SetCrossIndex(0);
	//设置点击鼠标光标定位点坐标，相当于KEditView_PTR的坐标
	m_edit_name->setTextOff(2, 2);
	//光标线的长度
	m_edit_name->setCrossOff(3);
	m_edit_name->SetFontSize(18);
	m_edit_name->setMultiLine(true);
	m_edit_name->setReadonly(false);
	m_edit_name->SetText(wipeOutStringLineBreak(m_p_prj->getName()));
	AddView(m_edit_name);
	m_edit_name->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	//设置聚焦
	m_edit_name->SetAllSelect(true);
	m_edit_name->setViewFocus();
	InvalidateView();
}

void NMindMapItemPrj::OnMouseDown(kn_int x, kn_int y)
{
	if(!isPointInView(x, y))
	{
		if(m_onfoucs_drawable->isShow())
		{
			hideOnFoucsDrawable();
		}
	}

	// 换成NMindMapItemPrj的相对坐标
	REPoint pos_point;
	pos_point.set(x - m_rect.left(), y - m_rect.top());
	std::list<NMindMapItemBase_PTR>::iterator itr;

	for(itr = m_item_lst.begin(); itr != m_item_lst.end(); ++itr)
	{
		(*itr)->OnMouseDown(pos_point.x(), pos_point.y());
	}

	if(m_edit_name)
	{
		if(!m_edit_name->isPointInView(pos_point.x(), pos_point.y()))
		{
			if(m_edit_name->isShow())
			{
				OnEditPrjNameOver(wipeOutStringLineBreak(m_edit_name->getText()));
				// 释放聚焦控件
				KScreen* p =  GetScreen();
				KView_PTR v_null;

				if(p)
				{
					p->setFocusView(v_null, FALSE);
				}

				AddViewToDel(m_edit_name);
				m_edit_name.reset();
				InvalidateView();
			}
		}
	}
}

void NMindMapItemPrj::onKeyUp(KMessageKey* kMsg)
{
	int key = kMsg->m_key_id;

	if(key == 'T' || key == 't')
	{//ctrl+T 直接加task
		if(kMsg->m_key_flag == KMSG_KEYFLAG_CONTROL)
		{
			OnAddTask();
			kMsg->setIdle(KMSG_RETURN_DILE);
			m_onfoucs_drawable->SetShow(false);
		}
	}

	if(m_onfoucs_drawable->isShow() && bViewFocus())
	{
		//键盘消息

		if(key == Key_insertKey)
		{
			kMsg->setIdle(KMSG_RETURN_DILE);
			OnAddTask();
			m_onfoucs_drawable->SetShow(false);
		}
	}
}

void NMindMapItemPrj::OnAddTask()
{
	CNProjectTask task;
	task.setDataPointer(m_p_prj);
	task.setName(_T("Task"));
	m_p_prj->addTask(task);
	CNProjectTask_LST* lst_task = m_p_prj->getpTasks();
	NMindMapItemTask_PTR task_bar = CreatTaskView(lst_task->size() - 1, &m_p_prj->getTasks().back());
	task_bar->showOnFoucsDrawable();
	task_bar->ShowEditTaskName();
	OnUpdatePos();
	//计算出相当于NMindMapArea的坐标，方便移动
	kn_int view_left = task_bar->GetRect().left() + m_rect.left();
	kn_int view_top = task_bar->GetRect().top() + m_rect.top();
	kn_int view_right = view_left + task_bar->GetRect().width();
	kn_int view_bottom = view_top + task_bar->GetRect().height();
	adjustPosByViewRect(RERect::MakeLTRB(view_left, view_top, view_right, view_bottom));
}


void NMindMapItemPrj::OnDelTask(CNProjectTask* prj_data, KView_PTR v)
{
	/// del task data
	CNProjectTask_LST& lst_task = m_p_prj->getTasks();
    CNProjectTask_LST::iterator itr;

	for(itr = lst_task.begin(); itr != lst_task.end(); ++itr)
	{
		if((*itr).getId() == (prj_data->getId()))
		{

            lst_task.erase(itr);
			break;
		}
	}

	m_p_prj->setModify(true);
	/// 从 item_lst中删除
	NMindMapItemTask_PTR task_view = VIEW_PTR(NMindMapItemTask)(v);
	std::list<NMindMapItemBase_PTR>::iterator it_task;
	// 删除task所处位置
	kn_int del_pos = 0;

	for(it_task = m_item_lst.begin(); it_task != m_item_lst.end(); it_task++)
	{
		if((*it_task) == task_view)
		{
			m_item_lst.erase(it_task);
			break;
		}

		++del_pos;
	}

	// 计算需要调整的task的起始位置
	std::list<NMindMapItemBase_PTR>::iterator it_adjust_task = m_item_lst.begin();
	kn_int j = 0;

	for(j = 0; j < del_pos; ++j)
	{
		++it_adjust_task;
	}

	// 调整的task的位置和内容
	for(it_task = it_adjust_task; it_task != m_item_lst.end(); it_task++)
	{
		NMindMapItemTask_PTR task_bar = VIEW_PTR(NMindMapItemTask)(*it_task);

		if(j % 2 == 0)
		{
			//双数在左
			task_bar->SetPositionX(-task_w - 50);
			task_bar->setLeft(TRUE);
		}
		else
		{
			task_bar->SetPositionX(m_rect.width() + 50);
			task_bar->setLeft(FALSE);
		}

		// 刷新task的内容
		task_bar->refreshTaskInfo();
		++j;
	}

	AddViewToDel(task_view);
	OnUpdatePos();
}

kn_bool NMindMapItemPrj::OnUserMsg(KMessage* pMsg)
{
	kn_bool ret = FALSE;

	if(pMsg->m_msg_type == PROJECT_EDIT_UPDATE_POS)
	{
		OnUpdatePos();
		ret = TRUE;
	}

	return ret;
}

void NMindMapItemPrj::OnEditPrjNameOver(kn_string edit_str)
{
	if(m_name_text_drawable->GetText() == edit_str || (edit_str == _T("")))
	{
		return;
	}

	m_name_text_drawable->SetText(edit_str);
	m_p_prj->setName(edit_str.c_str());
}


void NMindMapItemPrj::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	CMenuMindMap_PTR menu = CMenuMindMap_PTR(new CMenuMindMap());
	menu->Create(0, 0, 140, 75);
	menu->SetScreen(GetScreen());
	menu->init(pMsg->m_p_view, MIND_PRJ_MENU);
	menu->m_sign_menu_view_click.connect(this, &NMindMapItemPrj::OnMindPrjMenu);
	//showmenu 传入view自身的相对坐标
	showMenu(menu, iScreenX - m_rect.left(), iScreenY - m_rect.top());
	InvalidateView();
}

void NMindMapItemPrj::OnMindPrjMenu(KView_PTR v, kn_int id)
{
	if(id == 1)
	{
		m_onfoucs_drawable->SetShow(false);
		OnAddTask();
	}
	else if(id == 2)
	{
		ShowEditPrjName();
	}
}

//////////////////////NMindMapArea /////////////////////
NMindMapArea::NMindMapArea()
{
	m_pos_bak.set(0, 0);
	m_mouse_pos_bak.set(0, 0);
	m_b_down = false;
}

NMindMapArea::~NMindMapArea()
{
}

void NMindMapArea::init(CNProjectData* p)
{
	//初始化 mindmap area
	m_mindmap_item_prj = NMindMapItemPrj_PTR(new NMindMapItemPrj());
	m_mindmap_item_prj->Create((m_rect.width() - prj_w) / 2, (m_rect.height() - prj_h) / 2, prj_w, prj_h);
	m_mindmap_item_prj->init(p);
	AddView(m_mindmap_item_prj);
	m_mindmap_item_prj->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
	SetClip(m_rect);
}

void NMindMapArea::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked && m_b_down)
	{
		CPropertyPos* p = new CPropertyPos(m_mindmap_item_prj, m_pos_bak.x() +  x - m_mouse_pos_bak.x() , m_pos_bak.y() +  y - m_mouse_pos_bak.y());
		GetScreen()->addProperty(p);
		UpdateUI();
	}
}

void NMindMapArea::OnDown(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	m_mindmap_item_prj->OnMouseDown(x, y);

	if(isCanMove(x, y))
	{
		m_pos_bak = m_mindmap_item_prj->GetPosition();
		m_mouse_pos_bak.set(x, pMsg->m_pos_y);
		m_b_down = true;
	}
}

void NMindMapArea::OnRDown( kn_int x, kn_int y, KMessageMouse* pMsg )
{
	m_mindmap_item_prj->OnMouseDown(x, y);
}

void NMindMapArea::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		m_b_down = false;
	}
}

void NMindMapArea::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_mindmap_item_prj->isPointInBlank(iScreenX, iScreenY))
	{
		CMenuMindMap_PTR menu = CMenuMindMap_PTR(new CMenuMindMap());
		menu->Create(0, 0, 140, 40);
		menu->SetScreen(GetScreen());
		menu->init(pMsg->m_p_view, MIND_AREA_MENU);
		menu->m_sign_menu_view_click.connect(this, &NMindMapArea::OnMindAreaMenu);
		//showmenu 传入view自身的相对坐标
		showMenu(menu, iScreenX - m_rect.left(), iScreenY - m_rect.top());
		InvalidateView();
	}
}

void NMindMapArea::onWheelDirect(KMessageMouseWheel* pMsg)
{
	// 换成NMindMapArea的相对坐标(m_pos_x, m_pos_y没有矫正为本地坐标，为屏幕坐标)
	REPoint pos_point;
	pos_point.set(pMsg->m_pos_x - GetParent()->GetPosition().x(), pMsg->m_pos_y - GetParent()->GetPosition().y());

	if(isPointInView(pos_point.x(), pos_point.y()))
	{
		if(pMsg->m_roll > 0)
		{
			CPropertyPos* p = new CPropertyPos(m_mindmap_item_prj, m_mindmap_item_prj->GetRect().left(), m_mindmap_item_prj->GetRect().top() + 50);
			GetScreen()->addProperty(p);
		}
		else
		{
			CPropertyPos* p = new CPropertyPos(m_mindmap_item_prj, m_mindmap_item_prj->GetRect().left(), m_mindmap_item_prj->GetRect().top() - 50);
			GetScreen()->addProperty(p);
		}

		InvalidateView();
	}
}

void NMindMapArea::onDragDirect(KMessageDrag* mag)
{
	// 换成NMindMapArea的相对坐标(屏幕坐标)
	REPoint pos_point;
	pos_point.set(mag->m_pos_x - GetParent()->GetPosition().x(), mag->m_pos_y - GetParent()->GetPosition().y());
	kn_bool b_in_prj = m_mindmap_item_prj->isPointInView(pos_point.x(), pos_point.y());

	if(b_in_prj)
	{
		if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG)
		{
			m_mindmap_item_prj->setBkAShow(true);
		}
		else if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG_UP)
		{
			m_mindmap_item_prj->OnMemDragUpIn(mag->m_p_drag_view);
		}
		else if(mag->m_drag_type == STORYPAGE_ACTION_DRAG_UP)
		{
			m_mindmap_item_prj->OnUpdatePos();
		}
	}
	else
	{
		mag->m_pos_x = pos_point.x();
		mag->m_pos_y = pos_point.y();
		m_mindmap_item_prj->OnPointDragOut(mag);
	}

	// 成员拖拽up消失
	if(mag->m_drag_type == TASK_MEMBER_CONST_DRAG_UP)
	{
		m_mindmap_item_prj->setBkAShow(false);
		UpdateUI();
	}
	// 调整位置
	else if(mag->m_drag_type == STORYPAGE_ACTION_DRAG_UP)
	{
		m_mindmap_item_prj->OnUpdatePos();
	}
}

kn_bool NMindMapArea::isCanMove(kn_int x, kn_int y)
{
	// 如果在drag点在action view里面 ，只移动action 整个project不移动
	kn_bool bMove = true;

	if(m_mindmap_item_prj->isPointInActionView(x, y))
	{
		bMove = false;
	}

	return bMove;
}

void NMindMapArea::updatePos()
{
	m_mindmap_item_prj->OnUpdatePos();
}

void NMindMapArea::OnMindAreaMenu(KView_PTR v, kn_int id)
{
	if(id == 1)
	{
		m_mindmap_item_prj->OnAddTask();
	}
}

//////////////////////NMindMapView /////////////////////
NMindMapView::NMindMapView()
{
}

NMindMapView::~NMindMapView()
{
}

void NMindMapView::init(CNProjectData* p)
{
	// 添加背景
	KColorDrawable_PTR bk_drawable = KColorDrawable_PTR(new KColorDrawable(ColorSetARGB(255, 255, 255, 255)));
	bk_drawable->setRect(0, 0, m_rect.width(), m_rect.height());
	addDrawable(bk_drawable);
	bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	bk_drawable->setAutoSize(TRUE);
	//初始化 mindmap area
	m_mindmap = NMindMapArea_PTR(new NMindMapArea());
	m_mindmap->Create(0, 0, m_rect.width(), m_rect.height());
	m_mindmap->init(p);
	AddView(m_mindmap);
}

void NMindMapView::viewSizeChange()
{
	KViewGroup::viewSizeChange();
	m_mindmap->updatePos();
}
