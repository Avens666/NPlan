//  File:        NTaskSourceView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/28
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NTaskSourceView.h"
#include "../manager/KStringManager.h"
#include "NTaskListItem.h"
#include "../file/NProjectData.h"
#include "KFontManager.h"
#include "../NPlan.h"

#ifdef WIN32
//句柄
extern HWND g_hWnd; 
#endif 

#define  TASK_W 720
NTaskSourceView::NTaskSourceView( CNProjectTask* task )
{
#ifdef ANDROID_NDK
	m_p_task = new CNProjectTask(*task);	// 由于domodal 失效，m_p_task 会被释放,临时处理
#else
	m_p_task = task;
#endif
	m_i_index = -1;
	m_down_title = false;


}
NTaskSourceView::~NTaskSourceView(void)
{
#ifdef ANDROID_NDK
	SAFE_DELETE (m_p_task);
#endif
}

void NTaskSourceView::init()
{
	//tagRECT rect;
	//GetWindowRect(g_hWnd,&rect);


	RERect  rect;
	int iSlideWidth = 15;
	int iGridRow = 1;
	int i_grid_col = 9;
	int i_width = 800;		//窗口宽度
	int i_height = 620;		//窗口高度
	int i_title_height = 38;	//标题栏高度
	int i_text_width = 100;	//文本宽度;
	int i_text_x = 40;		//文本框left坐标
	int i_edit_height = 32;	//编辑框高度;
	int i_edit_width = 619;
	int	i_list_width	= 680;	//列表宽度
	int	i_member_height	= 100;	//人员管理高度
	//初始化背景
	K9PatchImageDrawable_PTR p_tsk_bg  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png")),true));

	m_group_task = KViewGroup_PTR(new KViewGroup() );
	m_group_task->Create(RERect::MakeXYWH(0, 0, i_width,i_height));
	m_group_task->addDrawable(p_tsk_bg);
	AddView( (KView_PTR)m_group_task);

	//初始化标题

	m_text_title = KTextView_PTR (new KTextView() );
	m_text_title->SetText(getStringManager()->GetStringByID(_T("task_title")));
    m_text_title->setTextAlign(REPaint::kLeft_Align);
	m_text_title->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE);
	m_text_title->SetFontSize(18);
	m_text_title->Create(20,3,i_width,i_title_height);
	m_group_task->AddView(m_text_title);
	m_text_title->enableMessage(false);

	//名称
	{
		m_text_name = KTextView_PTR(new KTextView());
		m_text_name->SetText(getStringManager()->GetStringByID(_T("task_resource_name")));
        m_text_name->setTextAlign(REPaint::kLeft_Align);
		m_text_name->SetTextColor(0xffa6a6a6,0xffa6a6a6,0xffa6a6a6,0xffa6a6a6);
		m_text_name->SetFontSize(14);
		m_text_name->Create(i_text_x,m_text_title->GetRect().bottom() + 15,i_text_width,i_edit_height);
		m_text_name->enableMessage(false);
		m_group_task->AddView(m_text_name);
		m_edit_name = KEditView_PTR(new KEditView() );
		K9PatchImageDrawable_PTR p_name  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
		p_name->SetRect(RERect::MakeWH(i_edit_width,i_edit_height));
		K9PatchImageDrawable_PTR p_name_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/public_resource_text_a.9.png")),true));
		p_name_a->SetRect(RERect::MakeWH(i_edit_width,i_edit_height));
		m_edit_name->setFocusDrawable(p_name_a);
		m_edit_name->Create(m_text_name->GetRect().right(),m_text_title->GetRect().bottom() + 15,i_edit_width,i_edit_height);
		m_edit_name->Init(5,5,RE_ColorBLACK);
		m_edit_name->SetTextColor(0xFF444444,0xFF444444,0xFF444444,0xFF444444);;
		m_edit_name->setBKDrawable(p_name);
		m_edit_name->SetCrossIndex(0);
		m_edit_name->setTextOff(2,2);
		m_edit_name->setCrossOff(4);
		m_group_task->AddView(m_edit_name);
	}
	//时间
	{
		m_text_time_start = KTextView_PTR( new KTextView() );
		m_text_time_start->SetText(getStringManager()->GetStringByID(_T("task_resource_time_start")));
        m_text_time_start->setTextAlign(REPaint::kLeft_Align);
		m_text_time_start->SetTextColor(0xffa6a6a6,0xffa6a6a6,0xffa6a6a6,0xffa6a6a6);
		m_text_time_start->SetFontSize(14);
		m_text_time_start->Create(i_text_x,m_edit_name->GetRect().bottom() + 10,i_text_width,i_edit_height);
		m_text_time_start->enableMessage(false);
		m_group_task->AddView(m_text_time_start);

		m_edit_time_start = KEditView_PTR( new KEditView() );
		K9PatchImageDrawable_PTR p_time_start  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
		p_time_start->SetRect(RERect::MakeWH((i_edit_width - i_text_width)/2,i_edit_height));
		m_edit_time_start->Create(m_text_time_start->GetRect().right(),m_edit_name->GetRect().bottom() + 10,(i_edit_width - i_text_width)/2,i_edit_height);
		m_edit_time_start->Init(5,5,RE_ColorBLACK);
		m_edit_time_start->SetTextColor(0xFF444444,0xFF444444,0xFF444444,0xFF444444);
		m_edit_time_start->setBKDrawable(p_time_start);
		m_edit_time_start->SetCrossIndex(0);
		m_edit_time_start->setTextOff(2,2);
		m_edit_time_start->setCrossOff(4);
		m_edit_time_start->setReadonly(true);
		m_group_task->AddView(m_edit_time_start);


		m_text_time_end = KTextView_PTR( new KTextView() );
		m_text_time_end->SetText(getStringManager()->GetStringByID(_T("task_resource_time_end")));
        m_text_time_end->setTextAlign(REPaint::kCenter_Align);
		m_text_time_end->SetTextColor(0xffa6a6a6,0xffa6a6a6,0xffa6a6a6,0xffa6a6a6);
		m_text_time_end->SetFontSize(14);
		m_text_time_end->Create(m_edit_time_start->GetRect().right(),m_edit_name->GetRect().bottom() + 10,i_text_width,i_edit_height);
		m_text_time_end->enableMessage(false);
		m_group_task->AddView(m_text_time_end);

		m_edit_time_end = KEditView_PTR( new KEditView() );
		K9PatchImageDrawable_PTR p_time_end  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
		p_time_end->SetRect(RERect::MakeWH((i_edit_width - i_text_width)/2,i_edit_height));
		m_edit_time_end->Create(m_text_time_end->GetRect().right(),m_edit_name->GetRect().bottom() + 10,(i_edit_width - i_text_width)/2,i_edit_height);
		m_edit_time_end->Init(5,5,RE_ColorBLACK);
		m_edit_time_end->SetTextColor(0xFF444444,0xFF444444,0xFF444444,0xFF444444);
		m_edit_time_end->setBKDrawable(p_time_end);
		m_edit_time_end->SetCrossIndex(0);
		m_edit_time_end->setTextOff(2,2);
		m_edit_time_end->setCrossOff(4);
		m_edit_time_end->setReadonly(true);
		m_group_task->AddView(m_edit_time_end);
	}


	//资源管理
	{

		m_text_resource = KTextView_PTR( new KTextView());
		m_text_resource->SetText(getStringManager()->GetStringByID(_T("task_resource")));
        m_text_resource->setTextAlign(REPaint::kLeft_Align);
		m_text_resource->SetTextColor(0xffa6a6a6,0xffa6a6a6,0xffa6a6a6,0xffa6a6a6);
		m_text_resource->SetFontSize(14);
		m_text_resource->Create(i_text_x,m_edit_time_end->GetRect().bottom() + 15,i_text_width,i_edit_height);
		m_text_resource->enableMessage(false);
		m_group_task->AddView(m_text_resource);

		/**
		* 添加人员按钮
		*/
		m_group_task->createImgViewHelper(&m_btn_add_mem, 
			_T("./resource/btn_add_n.png")
			, _T("./resource/btn_add_n.png"),
			_T("./resource/btn_add_a.png"),
			i_width - i_text_x - 28,
			m_edit_time_end->GetRect().bottom()+20);
		m_btn_add_mem->setTip(getStringManager()->GetStringByID(_T("add_res")),NO_TIMER,4000);
		m_btn_add_mem->m_clicked_signal.connect(this,&NTaskSourceView::addMenber);

		/**
		* 添加人员Grid
		*/
		K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/public_resource_people_bg.9.png")),true));
		draw_resource->SetRect(RERect::MakeWH(i_width - 2 * i_text_x,i_member_height));
		RERect rect_grid = RERect::MakeXYWH(i_text_x,m_text_resource->GetRect().bottom() + 2,i_width - 2 * i_text_x,i_member_height);

		/**
		* 添加一个用于滑动的人员
		*/
		m_p_drag_view = NDragStaffView_PTR( new NDragStaffView(_T(""),(res_type)0,0) );
		m_p_drag_view->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
		m_p_drag_view->SetShow(false);
		m_p_drag_view->setOpacity(0x88);

		AddView(m_p_drag_view);
		m_grou_resource =KViewGroup_PTR( new KViewGroup() );
		m_grou_resource->Create(rect_grid);
		m_grou_resource->addDrawable(draw_resource);
		m_group_task->AddView(m_grou_resource);
		RERect rect;
		rect.setXYWH(0,7,draw_resource->GetRect().width() ,draw_resource->GetRect().height() - 14);
		m_view_satff_grid = NGridView_PTR(new NGridView(rect.width() / i_grid_col,rect.height() / iGridRow,rect) );
		m_view_satff_grid->Create(rect);
		m_grou_resource->AddView(m_view_satff_grid);
	}

	/**
	* 添加card
	*/
	m_group_task->createImgView9PatchIconHelper(&m_btn_add_card, _T("./resource/btn_n.9.png"), _T("./resource/btn_a.9.png"), _T("./resource/btn_f.9.png"), _T("./resource/add_icon.png"),i_width - i_text_x - 58, m_grou_resource->GetRect().bottom()+10, 58, 36, ICON_SENTER, ICON_SENTER);
	m_btn_add_card->m_clicked_signal.connect(this,&NTaskSourceView::addTaskCard);

	//列表标题
	KTextView_PTR text_list_title = KTextView_PTR(new KTextView);
	KTextDrawable_PTR draw_task_name = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorWHITE, 16, REPaint::kLeft_Align) );
	draw_task_name->SetRect(RERect::MakeXYWH(6, 0, 294, 34));

	KTextDrawable_PTR draw_task_owner = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorWHITE, 16, REPaint::kLeft_Align) );
	draw_task_owner->SetRect(RERect::MakeXYWH(290, 0, 161, 34) );

	KTextDrawable_PTR draw_task_time = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorWHITE, 16, REPaint::kLeft_Align) );
	draw_task_time->SetRect(RERect::MakeXYWH(453, 0, 127, 34) );

	KTextDrawable_PTR draw_task_state = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorWHITE, 16, REPaint::kLeft_Align) );
	draw_task_state->SetRect(RERect::MakeXYWH(580, 0, 118, 34) );

	setFontStyle(draw_task_name);
	setFontStyle(draw_task_owner);
	setFontStyle(draw_task_state);
	setFontStyle(draw_task_time);

	draw_task_name->SetText(getStringManager()->GetStringByID(_T("task_card_name")));
	draw_task_owner->SetText(getStringManager()->GetStringByID(_T("task_owner")));
	draw_task_state->SetText(getStringManager()->GetStringByID(_T("task_state")));
	draw_task_time->SetText(getStringManager()->GetStringByID(_T("task_time")));

	text_list_title->addDrawable(draw_task_name);
	text_list_title->addDrawable(draw_task_owner);
	text_list_title->addDrawable(draw_task_time);
	text_list_title->addDrawable(draw_task_state);
	K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/list_title.9.png")),true));
	draw_resource->SetRect(RERect::MakeWH(661,31));
	
	text_list_title->setBKDrawable(draw_resource);
	text_list_title->Create(i_text_x,m_grou_resource->GetRect().bottom() + 10,TASK_W,34);

	m_group_task->AddView(text_list_title);
	
	/**
	* ���card
	*/
	m_group_task->createImgViewHelper(&m_btn_add_card, 
		_T("./resource/list_add_n.png"),
		_T("./resource/list_add_a.png"),
		_T("./resource/list_add_f.png"),
		i_width - i_text_x - 33, m_grou_resource->GetRect().bottom()+11);
	m_btn_add_card->setTip(getStringManager()->GetStringByID(_T("add_action")),NO_TIMER,4000);
	m_btn_add_card->m_clicked_signal.connect(this,&NTaskSourceView::addTaskCard);
	//任务列表
	m_list_task = KListViewEx_PTR(new KListViewEx);

	//赋值
	m_edit_name->SetText(m_p_task->getName());
	m_edit_time_start->SetText(to_iso_extended_wstring(m_p_task->getStartTime()));
	m_edit_time_end->SetText(to_iso_extended_wstring(m_p_task->getEndTime()));

	//初始化游标
	KSlideView_PTR p_slide = KSlideView_PTR(new KSlideView);
	p_slide->Create( RERect::MakeXYWH( m_view_satff_grid->GetRect().width() - 18,5,18,m_view_satff_grid->GetRect().height() ));
	p_slide->init(PROCESS_Vertical);

	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));

	p_slide->setIconDrawable(normal);
	p_slide->showBK(TRUE);

	m_view_satff_grid->BindSlide(p_slide);

	vector <int> vec_mem = m_p_task->getMembers();

	for (int i = 0; i < vec_mem.size() ; i++)
	{
		NStaffView_PTR pView = NStaffView_PTR( new NStaffView(m_p_task->getMembersNameById(vec_mem[i])
			,m_p_task->getResourceTypeById(vec_mem[i]),vec_mem[i]) );
		pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
		m_view_satff_grid->AddView(pView);
		pView->m_sign_down.connect(this,&NTaskSourceView::onDownMem);
		pView->setCanLeader(true);
		if (vec_mem[i] == m_p_task->getLeaderId())
		{
			pView->setLeader(true);
		}
		m_vec_mem.push_back(pView);
	}

	CNProjectTaskCard_LST&  vec_task_card = m_p_task->getTaskCards();
	int action_num =  vec_task_card.size();

		kn_int i_item_row = 6;
	m_list_task->Create(i_text_x,text_list_title->GetRect().bottom(),TASK_W,i_item_row * 32);
	m_group_task->AddView(m_list_task);
	CNProjectTaskCard_LST::iterator it;
	int i = 0;
	for (it = vec_task_card.begin(); it != vec_task_card.end(); ++it )
	{


		K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
			getSurfaceManager()->GetSurface(_T("./resource/list_item_n.9.png")),true));
		draw_resource->SetRect(RERect::MakeWH(TASK_W,32));
		NTaskListItem_PTR item = NTaskListItem_PTR(new NTaskListItem(draw_resource) );
		item->initCtrl();
		item->Create(5,0,TASK_W,32);
        kn_char wTime[32]= {0};
		_stprintf(wTime,_T("%dd%dh"), (*it).getPlanDays(), (*it).getPlanHours());

		item->setValue((*it).getName(),
			m_p_task->getMembersNameById((*it).getLeaderId()),
			wTime,
			m_p_task->getStateStrById((*it).getState()));
		item->setTaskCardID( (*it).getId() );
		item->m_sign_del_card.connect(this,&NTaskSourceView::onDelTaskCard);
		m_list_task->UI_addItemView(item);

		i++;
	}
	m_list_task->setScroller(_T("./resource/list_scroll.9.png"));
	m_list_task->itemRoll(FALSE);
	m_list_task->m_sign_focus_item.connect(this,&NTaskSourceView::itemChange);
	m_list_task->m_sign_del.connect(this,&NTaskSourceView::itemDel);
	K9PatchImageDrawable_PTR p_drw_btn_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/btn_a.9.png")),true));

	K9PatchImageDrawable_PTR p_drw_btn_n  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/btn_n.9.png")),true));

	K9PatchImageDrawable_PTR p_drw_btn_f  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/btn_f.9.png")),true));

	kn_int i_btn_w = 100;
	kn_int i_btn_h = 40;
	p_drw_btn_a->SetRect(RERect::MakeWH(i_btn_w,i_btn_h));
	p_drw_btn_n->SetRect(RERect::MakeWH(i_btn_w,i_btn_h));
	p_drw_btn_f->SetRect(RERect::MakeWH(i_btn_w,i_btn_h));
	//取消
	KImgButtonView_PTR p_btn_ok ;
	m_group_task->createImgView9PatchHelper(&p_btn_ok,
		_T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"),
		_T("./resource/btn_ok_n.9.png"),
		i_width - i_btn_w - 45, m_list_task->GetRect().bottom() + 9, i_btn_w, i_btn_h);
	p_btn_ok->SetText(getStringManager()->GetStringByID(_T("ok")));
	p_btn_ok->SetFontSize(14);
	p_btn_ok->setStateChangeEnable(true);
	p_btn_ok->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	p_btn_ok->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    p_btn_ok->setTextAlign(REPaint::kCenter_Align);
	p_btn_ok->m_clicked_signal.connect(this,&NTaskSourceView::onBtnOk);
	//ȷ��
	KImgButtonView_PTR p_btn_cancel ;
	m_group_task->createImgView9PatchHelper(&p_btn_cancel,
		_T("./resource/btn_cancel_n.9.png"),
		_T("./resource/btn_cancel_a.9.png"),
		_T("./resource/btn_cancel_n.9.png"),
		p_btn_ok->GetRect().left() - 15  - i_btn_w, m_list_task->GetRect().bottom() + 9, i_btn_w, i_btn_h);
	p_btn_cancel->SetText(getStringManager()->GetStringByID(_T("cancel")));
	p_btn_cancel->SetFontSize(14);
	p_btn_cancel->setStateChangeEnable(true);
	p_btn_cancel->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	p_btn_cancel->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    p_btn_cancel->setTextAlign(REPaint::kCenter_Align);
	p_btn_cancel->m_clicked_signal.connect(this,&NTaskSourceView::onBtnCancle);

	p_tsk_bg->SetRect(RERect::MakeWH(i_width,p_btn_ok->GetRect().bottom() + 25));

	m_group_task->changeViewLayerTop(m_grou_resource);




	m_list_task->EnableClipMessage(false);

	m_view_satff_grid->m_sign_GridItemDClicked.connect(this,&NTaskSourceView::onResourceDClick);

	m_group_task->m_sign_down.connect(this,&NTaskSourceView::onDown);
	m_group_task->m_sign_move.connect(this,&NTaskSourceView::onMove);
	m_group_task->m_sign_up.connect(this,&NTaskSourceView::onUp);


	//�˳�
	KImgButtonView_PTR btn_exit;
	m_group_task->createImgViewHelper(&btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		i_width - 45,3);
	btn_exit->m_clicked_signal.connect(this,&NTaskSourceView::onBtnCancle);

}
void NTaskSourceView::MoveGrid(kn_int iValue)
{
	float f = (float) iValue/(float)m_silder->GetLimit();
	m_view_satff_grid->MoveTo(f);
}

void NTaskSourceView::onDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	iScreenX -=	m_group_task->GetRect().left();
	iScreenY -=	m_group_task->GetRect().top();
	if (m_text_title->isPointInView(iScreenX,iScreenY))
	{
		m_down_title = true;
		m_down_x = iScreenX;
		m_down_y = iScreenY;


	}
}

void NTaskSourceView::onMove( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	if (m_down_title)
	{
		m_group_task->SetPosition(iScreenX - m_down_x ,iScreenY - m_down_y  );
		InvalidateView(true);
	}
}

void NTaskSourceView::onUp( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	m_down_title = false;
}

void NTaskSourceView::onWheelDirect( KMessageMouseWheel* pMsg )
{		
	for (int i = 0; i < getViewCount() ; i++)
	{
		getViewByIndex(i)->DoMessage(pMsg);
	}
	pMsg->setIdle(KMSG_RETURN_DILE);
}
void NTaskSourceView::setFontStyle(KTextDrawable_PTR& p)
{
	p->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	p->SetFontSize(16);
}

/**
* 确定
*/
void NTaskSourceView::onBtnOk(KView_PTR p)
{
	kn_string strName = m_edit_name->getText();
	if (strName.size() == 0)
	{
		MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("input_name")).c_str(),getStringManager()->GetStringByID(_T("warning")).c_str(), 0);
		return;
	}
	saveTaskData();
	m_sign_btn_ok.emit();
	endModal(KN_REUSLT_OK,TRUE);
} 

/**
* 取消
*/
void NTaskSourceView::onBtnCancle(KView_PTR p)
{
	m_sign_btn_cancle.emit(m_p_task);
	endModal(KN_REUSLT_CANCLE,TRUE);
} 

/**
* 删除taskCard
*/
void NTaskSourceView::onDelTaskCard(kn_int id)
{

	LSTVIEW p;
	CNProjectTaskCard_LST& vec_task_card = m_p_task->getTaskCards();
	CNProjectTaskCard_LST::iterator it;
	int i = 0;
	for (it = vec_task_card.begin(); it != vec_task_card.end(); ++it )
	{
		NTaskListItem_PTR pView = VIEW_PTR(NTaskListItem) (m_list_task->getItemView(i) );

		if (pView->getTaskCardID() == id)
		{
			p.push_back(pView);
			break;
		}
		i++;
	}

	if ( m_list_task->UI_DeleteItems(p) )
	{//由于有动画互斥处理，需要判断返回值
		i=0;
		for (it = vec_task_card.begin(); it != vec_task_card.end(); ++it )
		{
			if ( (*it).getId() == id)
			{
				it = vec_task_card.erase(it);
				break;
			}
			i++;
		}
//		m_p_task->setTaskCards(vec_task_card);
	}

}

/**
* 保存数据
*/
void NTaskSourceView::saveTaskData()
{

	kn_int i_leader_id = -1;
	vector<kn_int> vec_members_id;
	// 保存人员列表
	kn_bool b_find_leader = false;
	for (VECSTAFFITR itr = m_vec_mem.begin();
		itr != m_vec_mem.end() ; ++itr)
	{
		if ( (*itr)->isLeader())
		{
			i_leader_id = (*itr)->GetId();
//			b_find_leader = true;
		}
		if ((*itr)->GetId() != -1)
		{
			vec_members_id.push_back((*itr)->GetId());
		}
	}
	m_p_task->setMembers(vec_members_id);

	//if (!b_find_leader && !vec_members_id.empty())
	//{
	//	i_leader_id = vec_members_id[0];
	//}

	// 保存taskcard
	CNProjectTaskCard_LST::iterator it;
	int i = 0;
	CNProjectTaskCard_LST& vec_task_card = m_p_task->getTaskCards();
	for (it = vec_task_card.begin(); it != vec_task_card.end(); ++it )
	{
		NTaskListItem_PTR pView = VIEW_PTR(NTaskListItem)( m_list_task->getItemView(i) );
		(*it).setName(pView->getTaskCardName().c_str());
		if (pView->getMember().size() > 0)
		{
			(*it).setLeaderId(m_p_task->getResourceIdByName(pView->getMember()[0].c_str()));
		}
		vector<kn_int> vec_time = pView->GetTaskTime();
		if (vec_time.size() > 1)
		{
			(*it).setPlanDays(vec_time[0]);
			(*it).setPlanHours(vec_time[1]);
		}

		i++;
	}
//	m_p_task->setTaskCards(m_vec_task_card);

	// 保存项目信息
	m_p_task->setTaskInfo(m_edit_name->getText().c_str(),
		m_p_task->getId(),
		m_p_task->getStartTime(),
		m_p_task->getEndTime(),
		i_leader_id);
}
void NTaskSourceView::addMenber(KView_PTR p)
{
#ifdef WIN32
	tagRECT rect;
	GetWindowRect(g_hWnd,&rect);
#else
    tagRECT rect = {0,0,1024,768};
#endif
	vector<CNProjectResource> vec_project_member = m_p_task->getProjectMember();

	vector<NMemberMsg>	vec_project_mem;
	for (vector<CNProjectResource>::iterator  itr = vec_project_member.begin(); 
		itr != vec_project_member.end()  ; ++itr)
	{
		vec_project_mem.push_back(NMemberMsg(*itr));
	}

	m_vec_task_mem.clear();
	for (VECSTAFFITR  itr = m_vec_mem.begin(); 
		itr != m_vec_mem.end()  ; ++itr)
	{
		NMemberMsg msg = (*itr)->getMsg();
		for (int i = 0; i < m_list_task->getItemCount() ; i++)
		{
			NTaskListItem_PTR pView = VIEW_PTR(NTaskListItem)(m_list_task->getItemView(i));
			vector<kn_string> str = pView->getMember();
			if (!str.empty())
			{
				if (str[0] == msg.str_name)
				{
					msg.b_update = false;
					break;
				}
			}
		}
		m_vec_task_mem.push_back(msg);
	}
	m_p_mem = NMemberSourceView_PTR(new NMemberSourceView(vec_project_mem,m_vec_task_mem) );

	m_p_mem->Create(0, 0, rect.right - rect.left, rect.bottom - rect.top);

	m_p_mem->initCtrl();

	m_p_mem->m_sign_btn_ok.connect(this,&NTaskSourceView::onEditMem);
	AddView(m_p_mem);

	m_p_mem->doModal();

}

void NTaskSourceView::addTaskCard(KView_PTR)
{
	m_p_task->addTaskCard(_T("Action"),m_p_task->getLeaderId(),1,0,0,0);
	CNProjectTaskCard_LST& vec_task_card = m_p_task->getTaskCards();
	CNProjectTaskCard_LST::reverse_iterator it = vec_task_card.rbegin();
	(*it).setStartTime(m_p_task->getStartTime() );
	(*it).setEndTime(m_p_task->getEndTime() );
	K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/list_item_n.9.png")),true));
	draw_resource->SetRect(RERect::MakeWH(TASK_W,32));
	NTaskListItem_PTR item = NTaskListItem_PTR(new NTaskListItem(KImageDrawable_PTR(draw_resource)) );
	item->initCtrl();
	item->Create(5,0,TASK_W,32);
	WCHAR wTime[32]= {0};
	_stprintf(wTime,_T("%dd%dh"), (*it).getPlanDays(), (*it).getPlanHours());

	item->setValue((*it).getName(),
		m_p_task->getMembersNameById((*it).getLeaderId()),
		wTime,
		m_p_task->getStateStrById((*it).getState()));
	item->setTaskCardID( (*it).getId() );
	item->m_sign_del_card.connect(this,&NTaskSourceView::onDelTaskCard);
	m_list_task->UI_addItemView(item);

}
void NTaskSourceView::onEditMem(vector<NMemberMsg>& v)
{

	for (int i = m_view_satff_grid->getViewCount() - 1; i >= 0  ; i--)
	{
		m_view_satff_grid->AddViewToDel(m_view_satff_grid->getViewByIndex(i));
		m_view_satff_grid->eraseView(m_view_satff_grid->getViewByIndex(i));
	}
	m_vec_mem.clear();
	kn_bool b_find_leader = false;
	for (vector<NMemberMsg>::iterator it  = v.begin();	it != v.end()  ; it++)
	{
		NStaffView_PTR pView =NStaffView_PTR( new NStaffView((*it)) );
		pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
		m_view_satff_grid->AddView(pView);
		pView->setCanLeader(true);
		m_vec_mem.push_back(pView);

		pView->m_sign_down.connect(this,&NTaskSourceView::onDownMem);
		if ((*it).id == m_p_task->getLeaderId())
		{
			pView->setLeader(true);
			b_find_leader = true;
		}
	}


	if (!b_find_leader && !m_vec_mem.empty())
	{
		m_vec_mem[0]->setLeader(TRUE);
		m_p_task->setLeaderId( m_vec_mem[0]->GetId() );
	}
}

void NTaskSourceView::onDownMem(kn_int x, kn_int y,KMessageMouse* msg)
{
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(msg->m_p_view);
	kn_string str ;
	pView->GetName(str);
	m_p_drag_view->SetResourceType(pView->GetResourceType());
	m_p_drag_view->SetName(str);
	m_p_drag_view->SetRect(pView->GetRect());
	m_p_drag_view->OnDown(x,y,msg);
}
void NTaskSourceView::onResourceDClick( KView_PTR view )
{
	NStaffView_PTR staff = VIEW_PTR(NStaffView)(view);
	if (!staff)
	{
		return;
	}
	m_p_task->setLeaderId(staff->GetId());
}

void NTaskSourceView::itemChange(kn_int i)
{
	if (i != m_i_index)
	{
		if (m_i_index != -1)
		{
			NTaskListItem_PTR pView = VIEW_PTR(NTaskListItem)(m_list_task->getItemView(m_i_index));
			if (pView)
			{
				pView->showDelete(FALSE);
			}
		}
		if (i != -1)
		{
			m_i_index = i;
			NTaskListItem_PTR pView = VIEW_PTR(NTaskListItem)(m_list_task->getItemView(m_i_index));
			if (pView)
			{
				pView->showDelete(TRUE);
			}
		}
	}
}



void NTaskSourceView::itemDel(vector<int>&)
{
	m_i_index = -1;
}
