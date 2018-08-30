//  File:        NMemberSourceView.cpp
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
#include "NMemberSourceView.h"
#include "KScreen.h"
#include "../manager/KStringManager.h"
#include "../file/NProjectData.h"
#include "KFontManager.h"
#include "../NPlan.h"


#define  I_GRID_COL	 (7)			//�к���
#define	 I_GRID_ROW	 (2)

#ifdef WIN32
//句柄
extern HWND g_hWnd; 
#endif 

NMemberSourceView::NMemberSourceView( const vector<NMemberMsg>& _vec_const,  vector<NMemberMsg>& _vec_child )
{
	std::copy(_vec_const.begin(),_vec_const.end(),std::back_inserter( m_vec_const ));
	std::copy(_vec_child.begin(),_vec_child.end(),std::back_inserter( m_vec_child ));
	m_down_title = false;
}
NMemberSourceView::~NMemberSourceView(void)
{

}

void NMemberSourceView::initCtrl()
{

#ifdef WIN32
    tagRECT rec;
	GetWindowRect(g_hWnd,&rec);
#else
    tagRECT rec = {0, 0, 1024, 768};
#endif
	int	i_width		= 700;			//���ڿ��
	int	i_height	= 550;			//���ڸ߶�
	int i_title_height = 38;		//�������߶�
	int	i_text_x	=20;			//����
	kn_int i_btn_w = 100;
	kn_int i_btn_h = 40;


	kn_int i_grid_off = 2;			//grid�߿��ƫ������
	kn_int i_grid_off_x = 5;			//grid�߿��ƫ������
	kn_int i_silder_off = 18;			//���������
	kn_int i_grid_space = 40;			//�������ڵļ��
	K9PatchImageDrawable_PTR p_tsk_bg  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png")),true));
	p_tsk_bg->SetRect(RERect::MakeWH(i_width,i_height));
	m_group_member = KViewGroup_PTR(new KViewGroup);

    RERect rectGM = RERect::MakeXYWH((rec.right - rec.left - i_width)/2,
            (rec.bottom - rec.top - i_height)/2,i_width,i_height);
    m_group_member->Create(rectGM);
	m_group_member->addDrawable(p_tsk_bg);
	AddView(m_group_member);

	//初始化标题
	m_group_member->createTextHelper(&m_text_title,getStringManager()->GetStringByID(_T("mem_edit")),18,23,3,i_width-46,i_title_height);
    m_text_title->setTextAlign(REPaint::kLeft_Align);
	m_text_title->SetTextColor(RE_ColorWHITE);
	m_text_title->SetFontSize(18);
	m_text_title->enableMessage(false);



	/**
	* 添加人员Grid
	*/
	m_grou_resource =KViewGroup_PTR( new KViewGroup) ;

	/**
	* 添加一个用于滑动的人员
	*/
	m_p_drag_mem_view = NDragStaffView_PTR(new NDragStaffView(_T(""),(res_type)0,0) );
	m_p_drag_mem_view->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
	m_p_drag_mem_view->SetFontSize(16);
	m_p_drag_const_view = NDragStaffView_PTR(new NDragStaffView(_T(""),(res_type)0,0) );
	m_p_drag_const_view->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
	m_p_drag_const_view->SetFontSize(16);
	AddView(m_p_drag_mem_view);
	AddView(m_p_drag_const_view);
	//半透明
	m_p_drag_mem_view->setOpacity(0x88);
	m_p_drag_const_view->setOpacity(0x88);

	//默认隐藏
	m_p_drag_mem_view->SetShow(false);
	m_p_drag_const_view->SetShow(false);
	
	//设置回调消息
	m_p_drag_mem_view->setDragMsg(TASK_MEMBER_DRAG,TASK_MEMBER_DRAG_UP);
	m_p_drag_const_view->setDragMsg(TASK_MEMBER_CONST_DRAG,TASK_MEMBER_CONST_DRAG_UP);

	K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_people_bg.9.png")),true));
	draw_resource->SetRect(RERect::MakeWH(i_width - 2 * i_text_x,(i_height - i_title_height)/2 - 60));
	m_grou_resource->Create(i_text_x,i_title_height + 10,i_width - 2 * i_text_x,2 * draw_resource->GetRect().height() + i_grid_space);
	m_group_member->AddView(m_grou_resource);

	RERect rect;
	rect.setXYWH(0,0,draw_resource->GetRect().width(),draw_resource->GetRect().height());

	/**
	* 添加2块滑动区域.方便clip
	*/
	m_group_mem_resource = KViewGroup_PTR(new KViewGroup);
	m_group_mem_resource->Create(rect);
	m_group_mem_resource->addDrawable(draw_resource);
	m_grou_resource->AddView(m_group_mem_resource);
	RERect grid_rect = RERect::MakeXYWH(i_grid_off_x,i_grid_off,rect.width() - i_silder_off - 2 * i_grid_off_x,rect.height() - 2 * i_grid_off);

	m_view_mem_grid = NGridView_PTR( new NGridView(grid_rect.width() / I_GRID_COL,grid_rect.height() / I_GRID_ROW,grid_rect) );
	m_view_mem_grid->Create(grid_rect);
	m_group_mem_resource->AddView(m_view_mem_grid);


	m_group_const_resource = KViewGroup_PTR(new KViewGroup);
	RERect rectCR = RERect::MakeXYWH(0,rect.bottom() + i_grid_space,rect.width(),rect.height());
	m_group_const_resource->Create(rectCR);
	m_group_const_resource->addDrawable(draw_resource);
	m_grou_resource->AddView(m_group_const_resource);

	m_view_const_grid = NGridView_PTR(new NGridView(grid_rect.width() / I_GRID_COL,grid_rect.height() / I_GRID_ROW,grid_rect) );
	m_view_const_grid->Create(grid_rect);
	m_group_const_resource->AddView(m_view_const_grid);





	//初始化游标

	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	m_p_slide =KSlideView_PTR(new KSlideView);
	m_p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));

	m_p_slide->setIconDrawable(normal);


	m_p_slide->Create( RERect::MakeXYWH(grid_rect.right() + 1,grid_rect.top() +5,i_silder_off,grid_rect.height()  - 10));
	m_p_slide->init(PROCESS_Vertical);
	m_p_slide->showBK(TRUE);
	m_view_mem_grid->BindSlide(m_p_slide);
	m_group_mem_resource->AddView(m_p_slide);

	m_p_slide2 = KSlideView_PTR(new KSlideView);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	m_p_slide2->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));

	m_p_slide2->setIconDrawable(normal);
	m_p_slide2->Create( RERect::MakeXYWH(grid_rect.right() + 1,grid_rect.top() + 5,i_silder_off,grid_rect.height() - 10));
	m_p_slide2->init(PROCESS_Vertical);	
	m_p_slide2->showBK(TRUE);
	m_view_const_grid->BindSlide(m_p_slide2);
	m_group_const_resource->AddView(m_p_slide2);

	/**
	* 确定和取消按钮
	*/
	m_group_member->createImgView9PatchHelper(&m_p_btn_ok,
		_T("./resource/btn_ok_n.9.png")
		,_T("./resource/btn_ok_a.9.png"),
		_T("./resource/btn_ok_n.9.png"),
		i_width - 22 - i_btn_w,m_grou_resource->GetRect().bottom() + 10,
		i_btn_w,i_btn_h);
	m_p_btn_ok->SetText(getStringManager()->GetStringByID(_T("ok")));
	m_p_btn_ok->SetFontSize(14);
	m_p_btn_ok->SetTextBound(RERect::MakeXYWH(8,10,80,24));
	m_p_btn_ok->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	m_p_btn_ok->setTextAlign(REPaint::kCenter_Align);
	m_p_btn_ok->setStateChangeEnable(true);
	m_p_btn_ok->m_clicked_signal.connect(this,&NMemberSourceView::onBtnOk);
	
	m_group_member->createImgView9PatchHelper(&m_p_btn_cancel,
		_T("./resource/btn_cancel_n.9.png")
		,_T("./resource/btn_cancel_a.9.png"),
		_T("./resource/btn_cancel_n.9.png"),
		m_p_btn_ok->GetRect().left() - i_btn_w - 15
		,m_grou_resource->GetRect().bottom()  + 10,i_btn_w,i_btn_h);

	m_p_btn_cancel->SetText(getStringManager()->GetStringByID(_T("cancel")));
	m_p_btn_cancel->SetFontSize(14);
	m_p_btn_cancel->SetTextBound(RERect::MakeXYWH(8,10,80,24));
	m_p_btn_cancel->setTextAlign(REPaint::kCenter_Align);
	m_p_btn_cancel->setStateChangeEnable(true);
	m_p_btn_cancel->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	m_p_btn_cancel->m_clicked_signal.connect(this,&NMemberSourceView::onBtnCancle);
	
	/**
	* 激活态背景
	*/
	p_mem_bg_a = K9PatchImageDrawable_PTR(
	new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/member_bg_a.9.png")),true));
	p_cosnt_bg_a = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/member_bg_a.9.png")),true));
	p_mem_bg_a->SetRect(rect);
	p_cosnt_bg_a->SetRect(rect);
	m_group_mem_resource->addDrawable(p_mem_bg_a);
	m_group_const_resource->addDrawable(p_cosnt_bg_a);
	p_mem_bg_a->SetShow(false);
	p_cosnt_bg_a->SetShow(false);


	/**
	* 初始化元素
	*/
	if (m_view_mem_grid)
	{	
		for (vector<NMemberMsg>::iterator itr = m_vec_child.begin();
		itr != m_vec_child.end();++itr)
		{
			NStaffView_PTR pView = NStaffView_PTR(new NStaffView((*itr)) );
			if (!pView->getCanUpdate())
			{
				KImageDrawable_PTR drawable = 
					KImageDrawable_PTR(new KImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/un_drag_icon.png")),true));
				pView->setIcon(drawable,0,0,true);
			}

			pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
			pView->setCanDBclick(false);

			pView->m_sign_down.connect(this,&NMemberSourceView::onDownMem);
			m_view_mem_grid->AddView(pView);
		}
	}

	/**
	* 初始化元素
	*/
	if (m_view_const_grid)
	{
		for (vector<NMemberMsg>::const_iterator itr = m_vec_const.begin();
			itr != m_vec_const.end();++itr)
		{
			vector<NMemberMsg>::iterator  it = std::find(m_vec_child.begin(),m_vec_child.end(),(*itr));
			if (it == m_vec_child.end())
			{
				NStaffView_PTR pView =NStaffView_PTR(new NStaffView((*itr)) );
				pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
				pView->setCanDBclick(false);
				pView->m_sign_down.connect(this,&NMemberSourceView::onDownConst);
				m_view_const_grid->AddView(pView);
			}
		}
	}


	m_view_mem_grid->BindSlide(m_p_slide);
	m_view_const_grid->BindSlide(m_p_slide2);

	m_group_member->m_sign_down.connect(this,&NMemberSourceView::onDown);
	m_group_member->m_sign_move.connect(this,&NMemberSourceView::onMove);
	m_group_member->m_sign_up.connect(this,&NMemberSourceView::onUp);


	/*
	 *	exit
	 */
	KImgButtonView_PTR btn_exit;
	m_group_member->createImgViewHelper(&btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		i_width - 45,3);

	btn_exit->m_clicked_signal.connect(this,&NMemberSourceView::onBtnCancle);
}

/**
* 标题移动相关
*/
void NMemberSourceView::onDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	iScreenX -=	m_group_member->GetRect().left();
	iScreenY -=	m_group_member->GetRect().top();
	if (m_text_title->isPointInView(iScreenX,iScreenY))
	{
		m_down_title = true;
		m_down_x = iScreenX;
		m_down_y = iScreenY;
	}
}

/**
* 标题移动相关
*/
void NMemberSourceView::onMove( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	if (m_down_title)
	{
		m_group_member->SetPosition(iScreenX - m_down_x ,iScreenY - m_down_y  );
		InvalidateView(true);
	}
}

/**
* 标题移动相关
*/
void NMemberSourceView::onUp( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	m_down_title = false;
}

/**
* 拖拽相关
*/
void NMemberSourceView::onMemberDrag(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;
	KView_PTR p = msg->m_p_drag_view;
	m_group_const_resource->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_group_const_resource->GetRect().width()&&
		iy > ly && iy < ly + m_group_const_resource->GetRect().height())
	{
		p_cosnt_bg_a->SetShow(true);
	}
	else
	{
		p_cosnt_bg_a->SetShow(false);
	}
	m_group_const_resource->InvalidateView();
}
/**
* 拖拽相关
*/
void NMemberSourceView::onMemDragEnd(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;
	KView_PTR p = msg->m_p_drag_view;
	m_group_const_resource->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_group_const_resource->GetRect().width()&&
		iy > ly && iy < ly + m_group_const_resource->GetRect().height())
	{
		kn_bool b_find = false;
		NStaffView_PTR pView;
		for (int i = 0; i <  m_view_mem_grid->getViewCount(); i++)
		{
			pView = VIEW_PTR(NStaffView)(m_view_mem_grid->getViewByIndex(i) );
			if (pView->getMsg() == (VIEW_PTR(NStaffView)(p))->getMsg())
			{
				b_find = true;
				break;
			}
		}
		if (b_find && pView)
		{
			swapView(pView,m_view_const_grid);
			pView->m_sign_down.disconnect_all();
			pView->m_sign_down.connect(this,&NMemberSourceView::onDownConst);

			NMemberMsg m = pView->getMsg();
			vector<NMemberMsg>::iterator it = std::find(m_vec_child.begin(),m_vec_child.end(),m);
			if (it != m_vec_child.end())
			{
				m_vec_child.erase(it);
				m_vec_const.push_back(m);
			}
		}
	}

	m_view_mem_grid->updateChildPos();
	p_cosnt_bg_a->SetShow(false);
	m_group_const_resource->InvalidateView();
}
/**
* 拖拽相关
*/
void NMemberSourceView::onConstDrag(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;

	KView_PTR p = msg->m_p_drag_view;
	m_group_mem_resource->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_group_mem_resource->GetRect().width()&&
		iy > ly && iy < ly + m_group_mem_resource->GetRect().height())
	{
		p_mem_bg_a->SetShow(true);
	}
	else
	{
		p_mem_bg_a->SetShow(false);
	}
	m_group_mem_resource->InvalidateView();
}
/**
* 拖拽相关
*/
void NMemberSourceView::onConstDragEnd(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;
	KView_PTR p = msg->m_p_drag_view;
	m_group_mem_resource->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_group_mem_resource->GetRect().width()&&
		iy > ly && iy < ly + m_group_mem_resource->GetRect().height())
	{
		kn_bool b_find = false;
		NStaffView_PTR pView;
		for (int i = 0; i <  m_view_const_grid->getViewCount(); i++)
		{
			pView = VIEW_PTR(NStaffView)(m_view_const_grid->getViewByIndex(i) );
			if (pView->getMsg()  == (VIEW_PTR(NStaffView)(p))->getMsg())
			{
				b_find = true;
				break;
			}
		}
		if (b_find && pView)
		{
			swapView(pView,m_view_mem_grid);
			pView->m_sign_down.disconnect_all();
			pView->m_sign_down.connect(this,&NMemberSourceView::onDownMem);

			NMemberMsg m = pView->getMsg();
			vector<NMemberMsg>::iterator it = std::find(m_vec_const.begin(),m_vec_const.end(),m);
			if (it != m_vec_const.end())
			{
				m_vec_const.erase(it);
				m_vec_child.push_back(m);
			}
		}
	}

	m_view_const_grid->updateChildPos();
	p_mem_bg_a->SetShow(false);
	m_group_mem_resource->InvalidateView();
}


void NMemberSourceView::onDownMem(kn_int x, kn_int y,KMessageMouse* msg)
{
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(msg->m_p_view);
	kn_string str ;
	pView->GetName(str);
	if (pView->getMsg().b_update)
	{
		m_p_drag_mem_view->SetID(pView->GetId());
		m_p_drag_mem_view->SetResourceType(pView->GetResourceType());
		m_p_drag_mem_view->SetName(str);

		m_p_drag_mem_view->OnDown(x,y,msg);
	}
}

void NMemberSourceView::onDownConst(kn_int x, kn_int y,KMessageMouse* msg)
{
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(msg->m_p_view);
	kn_string str ;
	pView->GetName(str);

	if (pView->getMsg().b_update)
	{
		m_p_drag_const_view->SetID(pView->GetId());
		m_p_drag_const_view->SetResourceType(pView->GetResourceType());
		m_p_drag_const_view->SetName(str);
		m_p_drag_const_view->OnDown(x,y,msg);
	}
}
/**
* 换位置
*/
void NMemberSourceView::swapView(NStaffView_PTR p,NGridView_PTR g)
{

	//交换控件
	p->GetParent()->eraseView(p);

	UpdateSlider( VIEW_PTR(NGridView)(p->GetParent()) );

	g->AddView(p);

	g->updateChildPos();

	m_grou_resource->InvalidateView();
}
/**
* 确定
*/
void NMemberSourceView::onBtnOk(KView_PTR p)
{
	m_sign_btn_ok.emit(m_vec_child);
	onCloseWindow();
} 

/**
* 取消
*/
void NMemberSourceView::onBtnCancle(KView_PTR p)
{
	onCloseWindow();
} 

/**
* 关闭窗口
*/
void NMemberSourceView::onCloseWindow()
{
	endModal(KN_REUSLT_OK,TRUE);
} 


/**
* 双击交换
*/
void NMemberSourceView::dClickConst(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	swapView( VIEW_PTR(NStaffView)(pMsg->m_p_view), m_view_mem_grid);
	pMsg->setIdle(KMSG_RETURN_DILE);
}

void NMemberSourceView::dClickMem(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

	swapView( VIEW_PTR(NStaffView)(pMsg->m_p_view),m_view_const_grid);
	pMsg->setIdle(KMSG_RETURN_DILE);
}
void NMemberSourceView::UpdateSlider(NGridView_PTR	g)
{
	kn_bool b  = g->getViewCount() > I_GRID_COL * I_GRID_ROW;
	if (b==false)
	{
		g->resetCursorPos();
	}
}
void NMemberSourceView::onDragDirect( KMessageDrag* msg )
{
	if (msg)
	{
		if (msg->m_drag_type == TASK_MEMBER_DRAG)
		{	
			onMemberDrag(msg);
		}
		if (msg->m_drag_type == TASK_MEMBER_DRAG_UP)
		{
			onMemDragEnd(msg);
		}
		if (msg->m_drag_type == TASK_MEMBER_CONST_DRAG)
		{	
			onConstDrag(msg);
		}
		if (msg->m_drag_type == TASK_MEMBER_CONST_DRAG_UP)
		{
			onConstDragEnd(msg);
		}
	}
}
