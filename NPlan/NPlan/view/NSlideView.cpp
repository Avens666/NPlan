//  File:        NSlideView.cpp
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
#include "NSlideView.h"
NSlideView::NSlideView( NSilderCursorView_PTR pCursor )
{
	m_pCursor = pCursor;
	m_iMin = 0;		//最小值
	m_iMax = 100;	//最大值


	m_pCursor->m_sign_move.connect(this,&NSlideView::OnCursorMove);

	m_RectCursor = m_pCursor->GetRect();

}

NSlideView::~NSlideView(void)
{
}


void NSlideView::SetSlideBg( const kn_string& imgPath,const RERect& bgRect,kn_int iRollDis )
{
	KTextView_PTR pView  = KTextView_PTR(new KTextView);
	K9PatchImageDrawable_PTR p  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(imgPath.c_str()),true));

	p->SizeToImage();
	p->SetRect(bgRect);
	pView->addDrawable(p);
	pView->SetRect(bgRect);
	m_pCursor->setMaxStepSize(iRollDis,false);

	AddView(pView);
	AddView(m_pCursor);
}

void NSlideView::SetCursorInitPos( const RERect& rect )
{
	m_RectCursor = rect;
	m_pCursor->SetRect(rect);
}

void NSlideView::SetValueLimit( kn_int iMin,kn_int iMax )
{
	m_iMax = iMax;
	m_iMin = iMin;
}


void NSlideView::OnCursorMove( IN kn_int ix,IN kn_int iy )
{
	if (m_pCursor->GetRollDir() == DIR_Y)
	{
		float iMoveProportion =	(m_pCursor->GetRect().centerY() - m_pCursor->GetRect().height() / 2 ) / (GetRect().height() - m_RectCursor.height());
		m_iValue = iMoveProportion*(m_iMax - m_iMin);
        kn_char wbuff[64] = {0};
		_stprintf(wbuff,_T(" value:%d \n x:%d y:%d"),(m_iValue ),ix,iy);
		OutputDebugString(wbuff);

		m_value_change_sign.emit(m_iValue);
	}
	
}

void NSlideView::SetValue( kn_int iValue )
{
	m_iValue = iValue;
	m_value_change_sign.emit(m_iValue);
}

kn_int NSlideView::GetValue()
{
	return m_iValue;
}

kn_int NSlideView::GetLimit()
{
	return m_iMax - m_iMin;
}
