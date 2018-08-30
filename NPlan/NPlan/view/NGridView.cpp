//  File:        NGridView.cpp
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
#include "NGridView.h"
#include "../manager/KStringManager.h"
#include "KScreen.h"
#include "../NPlan.h"

NGridView::NGridView( kn_int iWidth,kn_int iHeight,const RERect& rect)
{
	m_dir = DIR_Y;
	m_bAuto_set_pos = true;
	m_orgin_w = iWidth;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_offset = 0;
	SetRect(rect);
	m_pre_h = 0;
	m_b_change_child_size = FALSE;
}


KSILDERDIR NGridView::GetDir()
{
	return m_dir;
}

void NGridView::SetDir( KSILDERDIR dir )
{
	m_dir = dir;
}

void NGridView::SetRect(const RERect& rect)
{
	KViewGroup::SetRect(rect);
	m_rectGrid = rect;
	m_iRow = rect.height() / m_iHeight;
	m_iCol = rect.width() / m_orgin_w;

	//重新调整，排列更均匀
	m_iWidth = rect.width() / m_iCol;
	m_offset = 0;

	if(m_pSlide) m_pSlide->setValue(0);

	SetClip(rect);

	updateChildPos();
}

void NGridView::eraseView(KView_PTR v)
{
	KViewGroup::eraseView(v);
	updateChildPos();
}

void NGridView::AddView( KView_PTR v )
{
	KViewGroup::AddView(v);

	if (v)
	{
		v->m_sign_dclick.connect(this,&NGridView::onGridItemDoubleClicked);
		//处理rect
		RERect rect,pViewRect;
		pViewRect = v->GetRect();
		//判断坐标;
		kn_int iNewX = ((m_lst_view.size() - 1) % m_iCol) * m_iWidth +  m_rectGrid.left();
		kn_int iNewY = (m_lst_view.size() -1) / m_iCol * m_iHeight + m_offset;

		//居中处理
		iNewX += (m_iWidth - pViewRect.width())/2;
		iNewY += (m_iHeight - pViewRect.height())/2;


		v->SetPosition(iNewX,iNewY);
//		v->SetClip(RERect::MakeXYWH(0,0,m_rect.width(),m_rect.height() ) );
	}


	//
	if (m_pSlide)
	{
		m_pSlide->SetShow(m_lst_view.size() > m_iRow * m_iCol);
	}
	if (m_lst_view.size() > m_iRow * m_iCol)
	{//计算逻辑高度
		m_iHideHeight = ((m_lst_view.size() -  m_iRow * m_iCol)/ m_iCol + 1) * m_iHeight;
	}
}

void NGridView::BindSlide( KSlideView_PTR pSlide )
{
	m_pSlide = pSlide;
	m_pSlide->SetShow(m_lst_view.size() > m_iRow * m_iCol);
	m_pSlide->setMaxMin(0,1000);
	m_pSlide->setValue(0);
	m_pSlide->m_silder_value_change_sign.connect(this, &NGridView::onSlideMove );
}

void NGridView::onSlideMove(int v)
{
	if (m_pSlide)
	{
		float fv = (float)v/m_pSlide->getValueArea();
		MoveTo(fv);
	}

}
void NGridView::WheelMove(float f_offset)
{
	if (m_pSlide)
	{
		int v = m_pSlide->getValue();
		float fv = (float)v/m_pSlide->getValueArea() + f_offset;
		MoveTo(fv);

		m_pSlide->setValue(fv * m_pSlide->getValueArea());
	}

}

void NGridView::setPreHeight(int h)
{
	m_pre_h = h;
}

void NGridView::MoveTo(float fOffset)
{
	if (fOffset < 0.0 )
	{
		fOffset = 0;
	}

	if (fOffset > 1.0 )
	{
		fOffset = 1.0;
	}

	{
		//防止移动过快没有刷新到顶
		fOffset = fOffset < 0.01 ? 0.0 : fOffset;
		fOffset = fOffset > 0.99 ? 1.0 : fOffset;

		m_offset = - m_iHideHeight*fOffset + m_pre_h;
		kn_int i = 0;
		for (LSTVIEW::iterator itr = m_lst_view.begin() ; itr != m_lst_view.end();
			++itr)
		{
			KView_PTR pView = (*itr);
			kn_int iNewY = i / m_iCol * m_iHeight + m_offset;

			//居中处理
			iNewY += (m_iHeight - pView->GetRect().height())/2;
			pView->SetPosition(pView->GetRect().left(),iNewY );
			++i;
		}
	}

}

void NGridView::onWheelDirect(KMessageMouseWheel* pMsg)
{
	if (m_pSlide && m_pSlide->isShow())
	{
		//转化为全屏坐标
		KView_PTR p = GetParent();
		RERect r = m_rect;
		while (p)
		{
			r.offset(p->GetRect().left(),p->GetRect().top());
			p = p->GetParent();
		}
		if (r.contains(pMsg->m_pos_x, pMsg->m_pos_y ) )
		{
			if( pMsg->m_roll > 0)
			{
				WheelMove(-0.05);
			}
			else
			{
				WheelMove(0.05);
			}
			InvalidateView();
		}
	}
}

void NGridView::onDragDirect(KMessageDrag* msg)
{
	if (std::find(m_lst_view.begin(),m_lst_view.end(),msg->m_p_drag_view) != m_lst_view.end())
	{
		m_sign_Drag.emit(msg);
	}
}

/**
* 更新子控件的坐标
*/
void NGridView::updateChildPos()
{
	/**
	* 顶点还原
	*/

	for (int i = 0; i <  getViewCount() ; i++)
	{
		KView_PTR p = getViewByIndex(i);

		RERect rect,pViewRect;
		pViewRect = p->GetRect();
		//判断坐标;
		kn_int iNewX = (i % m_iCol) * m_iWidth +  m_rectGrid.left();
		kn_int iNewY = i/ m_iCol * m_iHeight + m_offset;

		//居中处理
		iNewX += (m_iWidth - pViewRect.width())/2;
		iNewY += (m_iHeight - pViewRect.height())/2;

		p->SetPosition(iNewX,iNewY);
	}

	if (m_pSlide)
	{
		m_pSlide->SetShow(m_lst_view.size() > m_iRow * m_iCol);
	}
}

/**
* 重置GridView游标位置到起点
*/
void NGridView::resetCursorPos()
{
	MoveTo(0.0);
	if (m_pSlide)
	{
		m_pSlide->setValue(0);
	}
}

void NGridView::onGridItemDoubleClicked(kn_int x, kn_int y,KMessageMouse* msg)
{
	if (msg->m_p_view!=NULL)
	{
		m_sign_GridItemDClicked.emit(msg->m_p_view);
	}
}

