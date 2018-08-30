//  File:        KListViewEx.cpp
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
#include "KListViewEx.h"
#include "KSurfaceManager.h"

#define LIST_ANI_STOP			(KNUIMSG_USER + 301)
#define LIST_ANI_RE_POS			(KNUIMSG_USER + 302)
#define LIST_ANI_CHANGE_ITEM    (KNUIMSG_USER + 303)


#define LIST_SCALE_WIDTH 14
KListViewEx::KListViewEx()
{
	m_do_mouse_msg = true;
	m_b_item_can_move = true;
	
	m_b_need_callback = true;
	m_b_father_list = false;
	m_b_scroller_move = false;
}

void KListViewEx::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &KListViewEx::onKeyUp);
}


KListViewEx::~KListViewEx()
{
	for (LSTVIEW::iterator it = m_destruct_del.begin();
		it!= m_destruct_del.end() ; ++it)
	{
		if ((*it))
		{
			AddViewToDel((*it));
		}
	}
}

void KListViewEx::showScale( kn_bool b )
{
	if (m_p_scroller)
	{
		m_p_scroller->SetShow(b);
	}
}

bool KListViewEx::UI_DeleteItems( kn_int index )
{
	if (m_ani_thread.isPlaying())
	{
		return FALSE;
	}

	if (m_i_item_count > index && index >= 0)
	{
		KView_PTR pView = m_p_item_groupview->getViewByIndex(index);

		//enableMessage(false);
		LSTVIEW v;
		v.push_back(pView);
		UI_DeleteItems(v);
	}

	return TRUE;
}

bool KListViewEx::UI_DeleteItems(LSTVIEW p,kn_bool b_need_callback)
{
	m_v_del_index.clear();
	if (m_ani_thread.isPlaying())
	{
		return FALSE;
	}

	m_b_need_callback = b_need_callback;
	m_del_p_view.clear();
	if (!p.empty())
	{
		m_del_p_view = p;
		kn_int i_item_ani = 300;	//Item的动画时间
		kn_int i_list_ani = 200;	//列表上移的动画时间
		kn_int i_del_ani  = 20 ;	//循环删除时的间隔
		for (int i = 0; i < p.size() ; i++)
		{
			//自己隐藏
			//CNUIAnimation* pHideAnimation = new CNUIAnimation (i_item_ani, i*i_del_ani,0);
			//CPropertyPos* pOpacStart = new CPropertyPos(p[i],p[i]->GetRect().left(),p[i]->GetRect().top());
			//CPropertyPos* pOpacDst = new CPropertyPos(p[i],p[i]->GetRect().left() - p[i]->GetRect().width(),p[i]->GetRect().top());
			//CPropertyAnimationData* pHide = new CPropertyAnimationData(pOpacStart,pOpacDst);
			//pHideAnimation->addAnimationData(pHide);
			//pHideAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
			//pHideAnimation->setLoopTimes(1);
			//m_ani_thread.addAnimation(pHideAnimation);

			m_ani_thread.addAnimationHelper(  p[i], PropertyPARA_ID_Opacity, 0, i_item_ani, i*i_del_ani,0);
			m_v_del_index.push_back(m_p_item_groupview->findViewIndex( p[i]));
		}
		//其他item上移
		for (int i = m_p_item_groupview->findViewIndex(p[0]) + 1;
			i < m_p_item_groupview->getViewCount() ; i++)
		{
			kn_int i_move_h = 0;
			kn_bool b_is_same = false;
			for (int j = 0; j < p.size() ; j++)
			{
				if (m_p_item_groupview->getViewByIndex(i) == p[j])
				{
					b_is_same = true;
					break;
				}
				if (i > m_p_item_groupview->findViewIndex(p[j]))
				{
					i_move_h += p[j]->GetRect().height();
				}

			}
			if (b_is_same)
			{
				continue;
			}
			KView_PTR pView = m_p_item_groupview->getViewByIndex(i);
			//CNUIAnimation* pMoveAnimation = new CNUIAnimation (i_list_ani, i_item_ani,0);
			//CPropertyPos* pMoveStart = new CPropertyPos(pView,pView->GetRect().left(),pView->GetRect().top());
			//CPropertyPos* pMoveDst = new CPropertyPos(pView,pView->GetRect().left(),pView->GetRect().top() - i_move_h);
			//CPropertyAnimationData* pMove = new CPropertyAnimationData(pMoveStart,pMoveDst);
			//pMoveAnimation->addAnimationData(pMove);
			//pMoveAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
			//pMoveAnimation->setLoopTimes(1);
			//m_ani_thread.addAnimation(pMoveAnimation);

			m_ani_thread.addAnimationHelper( pView, PropertyPARA_ID_POS_Y, pView->GetRect().top() - i_move_h, i_list_ani, i_item_ani,0);
		}

		m_ani_thread.setStopMsg(LIST_ANI_STOP);
		m_ani_thread.Start();
		enableMouseMessage(false);
	}

	return TRUE;
}

void KListViewEx::itemRoll( kn_bool b )
{
	m_b_item_can_move = b;
}

void KListViewEx::onKeyUp(KMessageKey* k)
{
	// 	if ( k->m_key_id == VK_ESCAPE)
	// 	{
	// 		UI_DeleteItems(3);
	// 	}
}

void KListViewEx::onDownDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	if (!m_do_mouse_msg)
	{
		return;
	}
	if (inChildList(pMsg))
	{
		//��������б�.��Ϣ�ø�����
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
		return;
	}
	m_b_scale_dir = true;
	m_i_down_x = iScreenX;
	m_i_down_y = iScreenY;
	m_tick_down = GetTickCount();
	m_dir = DIR_UNKNOWN;
	m_xDiffer = m_rect.centerX() - iScreenX;
	//判断点击第几个Item;
	m_i_down_index = -1;

	if (m_p_item_groupview)
	{
		for (int i = 0; i < m_p_item_groupview->getViewCount() ; i++)
		{
			if (m_p_item_groupview->getViewByIndex(i)->isPointInView(
				iScreenX - m_p_item_groupview->GetRect().left() - m_rect.left()
				,iScreenY  - m_p_item_groupview->GetRect().top() - m_rect.top()))
			{
				m_i_down_index = i;
				m_rect_back = m_p_item_groupview->getViewByIndex(i)->GetRect();
				break;
			}
		}
	}

	//�������������.
	//��RECTת����ȫ������
	kn_int ileft = m_p_scroller->GetRect().left();
	kn_int itop = m_p_scroller->GetRect().top();
	KViewGroup_PTR p = m_p_scroller->GetParent();
	ileft += p->GetRect().left();
	itop += p->GetRect().top();
	//�ж���Ϣ�Ƿ��ھ��ο���
	if (ileft <= iScreenX && itop <= iScreenY &&
		ileft +  m_p_scroller->GetRect().width() >= iScreenX&&
		itop +  m_p_scroller->GetRect().height() >= iScreenY)
	{
		m_b_scroller_move = true;
	}
	KListView::onDownDirect( iScreenX, iScreenY,  pMsg );

}

void KListViewEx::onMoveDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	int i = 0;
	for (; i < m_p_item_groupview->getViewCount() ; i++)
	{
		RERect r = m_p_item_groupview->getViewByIndex(i)->GetBoundRect();
		if (m_p_item_groupview->getViewByIndex(i)->isPointInView(
			iScreenX - m_p_item_groupview->GetRect().left() - m_rect.left()
			,iScreenY  - m_p_item_groupview->GetRect().top() - m_rect.top()))
		{
			m_sign_focus_item.emit(i);
			break;
		}
	}
	if (i == m_p_item_groupview->getViewCount())
	{
		m_sign_focus_item.emit(-1);
	}
	if (!m_b_mouse_picked||!m_do_mouse_msg)
	{
		return;
	}
	if (inChildList(pMsg))
	{
		//��������б�.��Ϣ�ø�����
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
		return;
	}
	if (m_b_item_can_move)
	{
		//前50毫秒判断是横向移动还是纵向移动;
		if (m_b_scale_dir && (GetTickCount() - m_tick_down) > 50)
		{
			float f = 1.732f	;	//Ϊ��y���жϱ�x��һ��,��tan60 
			m_b_scale_dir = false;
			kn_int i_dis_x = iScreenX - m_i_down_x;
			kn_int i_dis_y = iScreenY - m_i_down_y;
			kn_int ix=  abs(i_dis_x);
			kn_int iy = f *abs(i_dis_y);
			if (ix < iy)
			{
				//数轴滑动
				if (i_dis_y > 0)
				{
					m_dir = DIR_DOWN;
				}
				else
				{
					m_dir = DIR_UP;
				}
			}
			else
			{
				//横轴滑动
				if (i_dis_x > 0)
				{
					m_dir = DIR_RIGHT;
				}
				else
				{
					m_dir = DIR_LEFT;
				}
			}

		}
		if (!m_b_scale_dir)
		{
			//横轴Item的滚动
			if (m_dir == DIR_RIGHT || m_dir == DIR_LEFT)
			{
				if (m_i_down_index >= 0 && m_i_down_index < m_p_item_groupview->getViewCount())
				{
					KView_PTR pView = m_p_item_groupview->getViewByIndex(m_i_down_index);
					kn_int newX = iScreenX + m_xDiffer - m_rect.width()/2;
					CPropertyPos* p = new CPropertyPos(pView,newX,pView->GetRect().top());
					pView->GetScreen()->addProperty(p);
					UpdateUI();
					if (iScreenX < m_rect.left() || iScreenX > m_rect.right())
					{
						pMsg->setIdle(KMSG_RETURN_DILE);
						onUpDirect(iScreenX,iScreenY,pMsg);		
						m_b_mouse_picked = false;
						m_dw_touctpoint_count = 0; 
					}
				}
				return;
			}
		}
		else
		{
			return;
		}
	}
	//�������������.
	if(m_b_scroller_move)
	{
		/// ����Ƶ������
		if(abs((int)m_first_press_point.fY -  iScreenY) > 3)
		{
			//���ƶ������ߵ�����ת�����ƶ��б������
			double d1 = (double)(m_rect.height() - m_p_scroller->GetRect().height());
			if (d1 == 0.0)
			{
				//�����ĸ������
				return;
			}
			double d =  (double)(m_p_item_groupview->GetRect().height()-m_rect.height())/d1;

			if(m_p_item_groupview == NULL || m_p_scroller == NULL)
			{
				return;
			}
			int iNewPosY =(double)(m_first_press_point.fY -  iScreenY)*d + m_i_press_top; 
			int iYPos = adjustInViewport(iNewPosY);
			CPropertyPos* pPos = new CPropertyPos(m_p_item_groupview, m_p_item_groupview->GetRect().left(), iYPos);
			GetScreen()->addProperty(pPos);
			int iYScoller = calcScrollerPos(iYPos);
			CPropertyPos* pPosScroller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iYScoller);
			GetScreen()->addProperty(pPosScroller);


			UpdateUI();
		}
	}
	else
	{
		KListView::onMoveDirect( iScreenX, iScreenY,  pMsg );
	}
}

void KListViewEx::onUpDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	m_b_scroller_move = false;
	if (!m_b_mouse_picked||!m_do_mouse_msg)
	{
		return;
	}
	if (inChildList(pMsg))
	{
		//��������б�.��Ϣ�ø�����
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
		return;
	}
	KListView::onUpDirect(iScreenX,iScreenY,pMsg);
	if (m_b_item_can_move)
	{
		if (m_dir == DIR_LEFT)
		{
			onLeftMove(iScreenX,iScreenY);
		}
		else if (m_dir == DIR_RIGHT)
		{
			onRightMove(iScreenX,iScreenY );
		}
		m_b_scale_dir = false;

	}

	kn_bool b_in = isPointInView(iScreenX, iScreenY) ;
	if (b_in && m_i_down_index != -1)
	{
		m_sign_click_item.emit(m_i_down_index);
	}

	//pMsg->setIdle(KMSG_RETURN_DILE);
}

void KListViewEx::onWheelDirect( KMessageMouseWheel* pMsg )
{
	if (!m_do_mouse_msg || (m_p_item_groupview && m_p_item_groupview->GetRect().height() <  m_rect.height()))
	{
		return;
	}
	if (inChildList(pMsg))
	{
		//��������б�.��Ϣ�ø�����
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
		return;
	}
	kn_int ileft = 0,itop = 0;
	KViewGroup_PTR p = GetParent();

	while (p)
	{
		ileft += p->GetRect().left();
		itop += p->GetRect().top();
		p = p->GetParent();
	}

	RERect rect;
	rect.setXYWH(m_rect.left() + ileft,m_rect.top() + itop,m_rect.width(),m_rect.height());

	//如果滚轴在列表区域内滚动.就跟着滚
	if (rect.contains(pMsg->m_pos_x, pMsg->m_pos_y ) )
	{
		KListView::onWheelDirect( pMsg);
	}
}
void KListViewEx::onItemRePos(KView_PTR p)
{
	if (p)
	{
		CNUIAnimation* pHideAnimation = new CNUIAnimation (300, 0,0);
		CPropertyPos* pOpacStart = new CPropertyPos(p,p->GetRect().left(),p->GetRect().top());
		CPropertyPos* pOpacDst = new CPropertyPos(p,m_rect_back.left(),m_rect_back.top());
		CPropertyAnimationData* pHide = new CPropertyAnimationData(pOpacStart,pOpacDst);
		pHideAnimation->addAnimationData(pHide);
		pHideAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
		pHideAnimation->setLoopTimes(1);
		m_ani_thread.addAnimation(pHideAnimation);
		m_ani_thread.setStopMsg(LIST_ANI_RE_POS);
		m_ani_thread.Start();
		enableMouseMessage(false);
	}
}

void KListViewEx::onLeftMove( kn_int iScreenX, kn_int iScreenY )
{
	if (m_i_down_index != -1)
	{
		if (m_i_down_x - iScreenX < m_rect_back.width()/4 )
		{
			onItemRePos(m_p_item_groupview->getViewByIndex(m_i_down_index));
		}
		else
		{
			UI_DeleteItems(m_i_down_index);
			m_sign_left_move.emit(m_i_down_index);
		}
	}
}

void KListViewEx::onRightMove( kn_int iScreenX, kn_int iScreenY )
{
	if (m_i_down_index != -1)
	{
		onItemRePos(m_p_item_groupview->getViewByIndex(m_i_down_index));

		if (iScreenX - m_i_down_x > m_rect_back.width()/4)
		{
			m_sign_right_move.emit(m_i_down_index);
		}
	}
}

kn_bool KListViewEx::OnUserMsg( KMessage* pMsg )
{

	kn_bool b_is_change = false;
	if ( pMsg->m_msg_type == LIST_ANI_STOP)
	{
		if (!m_del_p_view.empty() && m_p_item_groupview)
		{
			m_ani_thread.clearAnimation();
			RERect rect = m_p_item_groupview->GetRect();
			kn_int i_del_h = 0;
			for (int i = 0; i < m_del_p_view.size() ; i++)
			{
				kn_int last_cont = m_p_item_groupview->getViewCount();
				i_del_h += m_del_p_view[i]->GetRect().height();
				m_p_item_groupview->eraseView(m_del_p_view[i]);
				m_i_item_count = m_p_item_groupview->getViewCount();
				if (m_i_item_count != last_cont )
				{
					b_is_change = true;
					m_destruct_del.push_back(m_del_p_view[i]);
				}
			}
			if (b_is_change)
			{
				m_p_item_groupview->SetRect(RERect::MakeXYWH(rect.left(),rect.top(),rect.width(),rect.height() - i_del_h));
				if (m_p_item_groupview->GetRect().height() < m_rect.height())
				{
					m_p_item_groupview->SetRect(RERect::MakeXYWH(rect.left(),0,rect.width(),m_p_item_groupview->GetRect().height()));
				}
				else if (m_p_item_groupview->GetRect().bottom() < m_rect.bottom())
				{
					m_p_item_groupview->SetRect(RERect::MakeXYWH(rect.left(), m_rect.height() - m_p_item_groupview->GetRect().height()
						,rect.width(),m_p_item_groupview->GetRect().height()));
				}
			}

			if(m_p_scroller && m_9_draw_slider)
			{
				// 重设滚动条的大小
				RERect rectItems = m_p_item_groupview->GetRect();
				int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

				m_p_scroller->SetRect(RERect::MakeXYWH(m_rect.width() - 8,
					(m_rect.height() - iHeight) * ((float)rectItems.top()/(float)(m_rect.height() - rectItems.height())) , 8, iHeight));

				m_9_draw_slider->SetRect(RERect::MakeXYWH(0, 0, LIST_SCALE_WIDTH, iHeight));

				m_p_scroller->SetShow(m_rect.height() < m_p_item_groupview->GetRect().height());

				m_b_enable_drag = m_rect.height() < m_p_item_groupview->GetRect().height();
			}


		}
		enableMouseMessage(true);
		if (m_b_need_callback && b_is_change)
		{
			m_sign_del.emit(m_v_del_index);
		}
		return TRUE;
	}
	if (pMsg->m_msg_type == LIST_ANI_RE_POS)
	{
		enableMouseMessage(true);
		m_ani_thread.clearAnimation();
		return TRUE;
	}
	return FALSE;
}
void KListViewEx::enableMouseMessage(kn_bool b )
{
	m_do_mouse_msg = b;
}

void KListViewEx::OnDClick( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	if (m_p_item_groupview)
	{
		for (int i = 0; i < m_p_item_groupview->getViewCount() ; i++)
		{
			if (m_p_item_groupview->getViewByIndex(i)->isPointInView(
				iScreenX - m_p_item_groupview->GetRect().left() - m_rect.left()
				,iScreenY  - m_p_item_groupview->GetRect().top() - m_rect.top()))
			{
				m_sign_dbclick_item.emit(i);
				break;
			}
		}
	}
	pMsg->setIdle(KMSG_RETURN_CONTINUE);
}
void KListViewEx::UI_UpdateItems( KView_PTR p ,kn_int index)
{
	if (m_i_item_count > index && index >= 0)
	{
		KView_PTR pView = m_p_item_groupview->getViewByIndex(index);
		// 自上而下添加
		RERect rectGroup = m_p_item_groupview->GetRect();
		RERect rectItem = p->GetRect();


		for (int i = index + 1; i <  m_p_item_groupview->getViewCount(); i++)
		{
			KView_PTR p_view = m_p_item_groupview->getViewByIndex(i);
			RERect rc = p_view->GetRect();
			p_view->SetRect(RERect::MakeXYWH(rc.left(),rc.top() +  rectItem.height() - pView->GetRect().height(),rc.width(),rc.height()));
		}

		p->SetRect(RERect::MakeXYWH(0, pView->GetRect().top(), rectItem.width(), rectItem.height()));

		m_p_item_groupview->SetRect(RERect::MakeXYWH(0, 0, rectItem.width(), rectGroup.height() +  rectItem.height() - pView->GetRect().height()));

		m_p_item_groupview->AddView(p);

		for (int i = 1; i <  m_p_item_groupview->getViewCount() - index; i++)
		{
			m_p_item_groupview->changeViewLayerDown(p);

		}
		m_p_item_groupview->AddViewToDel(pView);
		// 未超出高度则不显示滚动条
		if(m_p_scroller)
		{
			kn_bool bShow = m_rect.height() < m_p_item_groupview->GetRect().height();
			m_p_scroller->SetShow(bShow);
			m_b_enable_drag = bShow;
		}
	}
}
// 设置滚动条
void KListViewEx::setScroller(kn_string img_path)
{
	if (!m_p_item_groupview)
	{
		return;
	}

	if(m_p_scroller && m_9_draw_slider)
	{
		// 重设滚动条的大小
		RERect rectItems = m_p_item_groupview->GetRect();
		int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

		m_p_scroller->SetRect(RERect::MakeXYWH(m_rect.width() - LIST_SCALE_WIDTH, 0, LIST_SCALE_WIDTH, iHeight));

		m_9_draw_slider->SetRect(RERect::MakeXYWH(0, 0, LIST_SCALE_WIDTH, iHeight));

		return;
	}

	RERect rectItems = m_p_item_groupview->GetRect();

	// 把viewport在listview中的位置，映射到滚动条

	// 确认滚动条的长度
	int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

	//iHeight = 20;

	// 滚动条的位置
	int iPosY = calcScrollerPos (rectItems.top());
	int iPosX = m_rect.width() - LIST_SCALE_WIDTH;

	m_p_scroller = KView_PTR(new KView);
	m_p_scroller->Create(iPosX, iPosY, LIST_SCALE_WIDTH, iHeight);

	m_9_draw_slider = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(img_path),true));

	m_9_draw_slider->SetRect(RERect::MakeXYWH(0, 0, LIST_SCALE_WIDTH, iHeight));

	m_p_scroller->addDrawable(m_9_draw_slider);
	m_p_scroller->SetShow(m_rect.height() < m_p_item_groupview->GetRect().height());
	m_b_enable_drag = m_rect.height() < m_p_item_groupview->GetRect().height();
	AddView(m_p_scroller);
}

// 设置滚动条
void KListViewEx::setScroller()
{
	KListView::setScroller();
}
void KListViewEx::showScroller(kn_bool b)
{
	if (m_p_scroller)
	{
		CNUIAnimation* pHideAnimation = new CNUIAnimation (200,0);
		CPropertyOpacity* pOpacStart;
		CPropertyOpacity* pOpacDst;
		if (b)
		{
			pOpacStart = new CPropertyOpacity(m_p_scroller,0);
			pOpacDst = new CPropertyOpacity(m_p_scroller,255);
		}
		else
		{
			pOpacDst = new CPropertyOpacity(m_p_scroller,0);
			pOpacStart = new CPropertyOpacity(m_p_scroller,255);
		}
		CPropertyAnimationData* pHide = new CPropertyAnimationData(pOpacStart,pOpacDst);
		pHideAnimation->addAnimationData(pHide);
		pHideAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
		pHideAnimation->setLoopTimes(1);
		m_ani_thread.addAnimation(pHideAnimation);
		m_ani_thread.Start();
	}
}

void KListViewEx::setStartIndex(kn_int i)
{
	KView_PTR p = m_p_item_groupview->getViewByIndex(i);
	CNUIAnimation* pHideAnimation = new CNUIAnimation (200, 0,0);
	CPropertyPos* pOpacStart = new CPropertyPos(m_p_item_groupview,m_p_item_groupview->GetRect().left(),m_p_item_groupview->GetRect().top());
	CPropertyPos* pOpacDst = new CPropertyPos(m_p_item_groupview,m_p_item_groupview->GetRect().left(),-p->GetRect().top());
	CPropertyAnimationData* pHide = new CPropertyAnimationData(pOpacStart,pOpacDst);
	pHideAnimation->addAnimationData(pHide);
	pHideAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
	pHideAnimation->setLoopTimes(1);
	m_ani_thread.addAnimation(pHideAnimation);

	m_ani_thread.Start();

}
void KListViewEx::UI_addItemView(KView_PTR pItemView)
{
	KListView::UI_addItemView(pItemView);
	if(m_p_scroller && m_9_draw_slider)
	{
		// 重设滚动条的大小
		RERect rectItems = m_p_item_groupview->GetRect();
		int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

		m_p_scroller->SetRect(RERect::MakeXYWH(m_rect.width() - LIST_SCALE_WIDTH, 0, LIST_SCALE_WIDTH, iHeight));

		m_9_draw_slider->SetRect(RERect::MakeXYWH(0, 0, LIST_SCALE_WIDTH, iHeight));
	}
}

void KListViewEx::changeItemHeight( kn_int i,kn_int h,kn_int i_delayed,kn_bool b_now_play)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	kn_int i_list_ani = 200;
	KView_PTR p = m_p_item_groupview->getViewByIndex(i);
	CNUIAnimation* pHideAnimation = new CNUIAnimation (i_list_ani, i_delayed,0);
	RERect r_end   = RERect::MakeXYWH(p->GetRect().left(), p->GetRect().top(), p->GetRect().width(), h) ;
	kn_int i_height =  h - p->GetRect().height();

	CPropertyRect* pOpacDst = new CPropertyRect(p,r_end);
	CPropertyAnimationData* pHide = new CPropertyAnimationData(pOpacDst);
	pHideAnimation->addAnimationData(pHide);
	m_ani_thread.addAnimation(pHideAnimation);

	for (int i = m_p_item_groupview->findViewIndex(p) + 1;
		i < m_p_item_groupview->getViewCount() ; i++)
	{
		KView_PTR pView = m_p_item_groupview->getViewByIndex(i);

		m_ani_thread.addAnimationHelper( pView, PropertyPARA_ID_POS_Y, pView->GetRect().top()+i_height, i_list_ani,i_delayed,0);
	}
	if (b_now_play)
	{
		m_ani_thread.Start();
	}
}


void KListViewEx::changeItemHeight(KView_PTR p,kn_int h ,kn_int i_delayed,kn_bool b_play)
{
	changeItemHeight(m_p_item_groupview->findViewIndex(p),h,i_delayed,b_play);

}

void KListViewEx::SetItemClip( RERect rect )
{
	m_p_item_groupview->SetClip(rect);
}

void KListViewEx::UpdateScale()
{
	if(m_p_scroller && m_9_draw_slider)
	{
		// ����������Ĵ�С
		RERect rectItems = m_p_item_groupview->GetRect();
		int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

		m_p_scroller->SetRect(RERect::MakeXYWH(m_rect.width() - LIST_SCALE_WIDTH, 0, LIST_SCALE_WIDTH, iHeight));

		m_9_draw_slider->SetRect(RERect::MakeXYWH(0, 0, LIST_SCALE_WIDTH, iHeight));

		m_p_scroller->SetShow(m_rect.height() < m_p_item_groupview->GetRect().height());
	}
}

kn_bool KListViewEx::inChildList(KMessagePos* pMsg)
{
	if (!m_b_father_list)
	{
		return false;
	}

	//��ת��ȫ������
	kn_int ileft = 0,itop = 0;

	KViewGroup_PTR pParent = GetParent();

	while (pParent)
	{
		ileft += pParent->GetRect().left();
		itop += pParent->GetRect().top();
		pParent = pParent->GetParent();
	}
	kn_int x = pMsg->m_pos_x - ileft;
	kn_int y = pMsg->m_pos_y - itop;
	for (int i = 0; i < m_p_item_groupview->getViewCount() ; i++)
	{
		//����Ū�ݹ�   ֻ���2��
		KView_PTR p = m_p_item_groupview->getViewByIndex(i);
		if (p->getType() == KVIEW_GROUP)
		{
			KViewGroup_PTR pGroup = VIEW_PTR(KViewGroup)(p);
			for (int i = 0; i < pGroup->getViewCount(); i++)
			{
				KView_PTR pChild = pGroup->getViewByIndex(i);

				if (pChild &&pChild->getType() == m_e_viewtype)
				{
					if (pChild->isPointInView(x - pGroup->GetRect().left() -  pChild->GetRect().left(),
						y - pGroup->GetRect().top()  -  pChild->GetRect().top() ))
					{
						return true;
					}
				}
			}
		}
		else
		{
			if (p &&p->getType() == m_e_viewtype)
			{
				if (p->isPointInView(pMsg->m_pos_x - p->GetRect().left(), pMsg->m_pos_y - p->GetRect().top() ))
				{
					return true;
				}
			}
		}

	}
	return false;

}

void KListViewEx::haveChildList(kn_bool b)
{
	m_b_father_list = b;
}
