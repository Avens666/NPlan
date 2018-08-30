//  File:        NSilderCursorView.cpp
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
#include "NSilderCursorView.h"
#include "KScreen.h"
#include "KShapeDrawable.h"
#include "KButtonView.h"
//=======================================
//NSilderCursorView   滑动条的游标
//
NSilderCursorView::NSilderCursorView(void)
{
	m_Direction = DIR_Y;
	m_bCanMove = true;
	m_bCanBoth = true;
	m_bIsFisrtDown = false;
	m_bIsAllMove = false;
}
NSilderCursorView::~NSilderCursorView(void)
{
}
void NSilderCursorView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//this->setBKDrawable(m_pressed_bk_drawable);
	m_startPoint.fX = iScreenX;
	m_startPoint.fY = iScreenY;
	m_recordY = iScreenY;
	m_yDiffer = m_rect.centerY() - iScreenY;
	m_recordX = iScreenX;
	m_xDiffer = m_rect.centerX() - iScreenX;
	m_sign_zoom_start.emit(iScreenX, iScreenY, pMsg);
	setState(BS_PRESSED);
	m_bIsFisrtDown = true;
	pMsg->setIdle(KMSG_RETURN_DILE);
}
void NSilderCursorView::OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if (m_bIsFisrtDown && m_bIsAllMove)
	{
		if (m_recordY != iScreenY)
		{
			m_Direction = DIR_Y;
		}
		else if (iScreenX != m_recordX)
		{
			m_Direction = DIR_X;
		}
		m_bIsFisrtDown = false;
	}
	if(m_b_mouse_picked )
	{
		CPropertyPos* p  = NULL;
		if (m_Direction == DIR_Y)
		{
			if (iScreenY > (m_startRect.centerY() + m_MaxStepSize - m_yDiffer))
			{
				iScreenY = m_startRect.centerY() + m_MaxStepSize - m_yDiffer;
			}
			else 
			{
				if (m_bCanBoth)
				{
					if (iScreenY < m_startRect.centerY() - m_MaxStepSize - m_yDiffer)
					{
						iScreenY = m_startRect.centerY() - m_MaxStepSize - m_yDiffer;
					}
				}
				else
				{
					if (iScreenY < m_startRect.centerY()  - m_yDiffer)
					{
						iScreenY = m_startRect.centerY()  - m_yDiffer;
					}
				}
			}
			if (m_bCanMove)
			{
				kn_int newY = iScreenY + m_yDiffer - m_rect.height() / 2;
				p = new CPropertyPos(shared_from_this(), m_rect.left(), newY);
				GetScreen()->addProperty(p);
			}
		}
		else
		{
			if (iScreenX > (m_startRect.centerX() + m_MaxStepSize - m_xDiffer))
			{
				iScreenX = m_startRect.centerX() + m_MaxStepSize - m_xDiffer;
			}
			else if (iScreenX < m_startRect.centerX() - m_MaxStepSize - m_xDiffer)
			{
				iScreenX = m_startRect.centerX() - m_MaxStepSize - m_xDiffer;
			}

			kn_int newX = iScreenX + m_xDiffer - m_rect.height() / 2;
			if (m_bCanMove)
			{
				p = new CPropertyPos(shared_from_this(), newX,m_rect.top());
				GetScreen()->addProperty(p);
			}
		}


		m_sign_move.emit(iScreenX,iScreenY);

		pMsg->setIdle(KMSG_RETURN_DILE);
	}
}
void NSilderCursorView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//this->setBKDrawable(m_normal_bk_drawable);
	setState(BS_NORMAL);
	//
	kn_int finalOffset = abs(iScreenY - m_startPoint.fY);

	if( finalOffset < 5 && bValidClick)
	{
		m_sign_showbtn.emit();
	}
	bValidClick = true;
	//发送无级缩放结束信号
	m_sign_zoom_finish.emit();
	//KMoveableView::OnUp(iScreenX, iScreenY,pMsg);

	//	OnMove(iScreenX, iScreenY);
}
//设置常态和激活态
void  NSilderCursorView::setBKGImage(const kn_string& normalResPath, const kn_string& pressedResPath)
{
	KDrawable_PTR normal = KImageDrawable_PTR(new KImageDrawable(normalResPath));
	KDrawable_PTR pressed = KImageDrawable_PTR(new KImageDrawable(pressedResPath));
	setBKG(normal,pressed);
}
//设置常态和激活态
void  NSilderCursorView::setBKG( KDrawable_PTR normal, KDrawable_PTR pressed)
{

	m_normal_bk_drawable = normal;
	m_pressed_bk_drawable = pressed;
	this->setBKDrawable(m_normal_bk_drawable);
}
//设置状态
void NSilderCursorView::setState( ViewState state, kn_bool bRefresh )
{
	//调用基类函数设置文本状态，传入False不激活刷新
	if (m_state != state)
	{
		writeLock lock(m_lst_drawable_mutex);
		switch(state)
		{
		case BS_NORMAL:
			m_lst_drawable[0] = m_normal_bk_drawable;
			break;
		case BS_PRESSED:
			m_lst_drawable[0] = m_pressed_bk_drawable;
			break;
		default:
			break;
		}
		lock.unlock();

		KTextView::setState(state, FALSE);

		if (bRefresh)
		{
			InvalidateView();
		}

	}
}
//设置最大的滚动距离()
void NSilderCursorView::setMaxStepSize( kn_int iStep,kn_bool bCanBoth /*= true*/ )
{
	m_MaxStepSize = iStep;
	m_bCanBoth = bCanBoth;
}
//设置滚动方向
void NSilderCursorView::SetRollDir( KSILDERDIR dir )
{
	m_Direction = dir;
}
kn_bool NSilderCursorView::Create(RERect& rect)
{
	m_startRect = rect;
//	setStackPoint(rect.left(),rect.top());
	return KMoveableView::Create(rect.left(), rect.top(), rect.width(), rect.height());
}
void NSilderCursorView::SetCanMove( kn_bool b ,const RERect& rect)
{
	m_bCanMove = b;
	if (!m_bCanMove)
	{
		CPropertyPos* p = new CPropertyPos(shared_from_this(),rect.left(),rect.top());
		GetScreen()->addProperty(p);
		UpdateUI();
	}
}
KSILDERDIR NSilderCursorView::GetRollDir()
{
	return m_Direction;
}
void NSilderCursorView::SetAllDir( kn_bool b )
{
	m_bIsAllMove = TRUE;
}
//=======================================
//KSilderTextView   背景中有文本的滑动条
//
KSilderTextView::KSilderTextView(void)
{
	m_bk_drawable = KColorDrawable_PTR(new KColorDrawable(RE_ColorBLACK) );

	addDrawable(m_bk_drawable);

	//缺省不显示任何内容，设置对应值时，缺省打开显示
	for(VEC_DRAWABLE::iterator ite = m_lst_drawable.begin();ite!=m_lst_drawable.end();ite++)
	{
		KDrawable_PTR p = (*ite);
		p->SetShow(FALSE);
	}
	m_iDis = 0;
	m_bAniIsValid = false;
}
KSilderTextView::~KSilderTextView(void)
{
}
void  KSilderTextView::setBKDrawable( KDrawable_PTR p )
{
	addDrawable(p);
}
void KSilderTextView::AddText( IN vector<SilderValue>VecStr,IN kn_int iDisPix,KDrawable_PTR normal,KDrawable_PTR active)
{

	m_vecText = VecStr;
	m_iDis = 0;
	m_iSectionDisPix = iDisPix;
	for (LSTVIEW::iterator itr = m_vecTextView.begin() ; 
		itr != m_vecTextView.end() ; ++itr)
	{
		this->eraseView(*itr);
	}
	m_vecTextView.clear();
	if (iDisPix < 0)
	{
		iDisPix = this->GetRect().width();
	}
	for (vector<SilderValue>::iterator itr  = VecStr.begin();
		itr != VecStr.end(); ++itr)
	{
		KImgButtonView_PTR tView = KImgButtonView_PTR(new KImgButtonView);
		RERect bgRect = this->GetRect();
		RERect ImgRect = normal->GetRect();
		if (normal)
		{
			tView->setBKDrawable(normal);
		}
		if (active)
		{
			tView->setBKG(normal,active,active,active);
		}
		kn_string str = ((kn_string)(*itr).strText);
		tView->enableMessage(false);
		tView->Create(RERect::MakeXYWH(m_iDis + (bgRect.width() - ImgRect.width())/2 , 
			(bgRect.height() - ImgRect.height())/2,ImgRect.width(), ImgRect.height()) );
		tView->SetText(str.c_str());
		kn_int iFontSize = tView->GetFontSize();
		tView->SetTextBound(RERect::MakeXYWH((tView->GetRect().width() -  tView->GetTextWidth())/2  , tView->GetRect().height()/2 - iFontSize/2,tView->GetTextWidth(), iFontSize));
		tView->SetTextColor(RE_ColorBLACK,RE_ColorBLACK,RE_ColorBLACK,RE_ColorBLACK);
		if (m_iDis >  bgRect.width()/2|| 
			m_iDis < 0)
		{
			tView->SetShow(FALSE);
		}
		else
		{
			tView->SetShow(TRUE);
		}

		AddView(tView);
		m_vecTextView.push_back(tView);

		kn_int iDis = (*itr).iDis;
		if (iDis <= 0)
		{
			iDis = iDisPix;
		}
		m_iDis += iDis;
	}
}
void KSilderTextView::MoveText(kn_int iValue)
{

	kn_int i = 0;
	for (LSTVIEW::iterator itr = m_vecTextView.begin() ; 
		itr != m_vecTextView.end() ; ++itr)
	{
		
		CPropertyPos* p  = new CPropertyPos((*itr),i*m_iDis/(m_vecTextView.size()) + iValue ,(*itr)->GetRect().top()) ;
		if (p)
		{
			(*itr)->GetScreen()->addProperty(p);
		}
		if (i*m_iDis/(m_vecTextView.size()) + iValue > this->GetRect().width() - 50 || 
			i*m_iDis/(m_vecTextView.size()) + iValue < 0)
		{
			(*itr)->SetShow(FALSE);
		}
		else
		{
			(*itr)->SetShow(TRUE);
		}
			i++;
	}

	UpdateUI();
}
kn_int SilderMoveToAnimationThread(LPVOID lpPara)
{
	KSilderTextView* pView = (KSilderTextView*)lpPara;
	LSTVIEW vecTextView = pView->GetTextView();
	kn_int iValue = pView->GetMoveDis();
	kn_int iDis = pView->GetTextDis();
	kn_int iSectionDisPix = pView->GetSectionDisPix();
	vector<RERect>	vecRect = pView->GetBackupTextRect();
	if (vecRect.size() != vecTextView.size())
	{
		return 0;
	}
	// 曲线类型
	KEasingCurve curve = KEasingCurve(KEasingCurve::Linear);
	static int iEasingCurveType = (int)KEasingCurve::OutElastic;
	// 动画持续时间
	const int TIME_MAX = 400;	// 0.5s

	int iStopTime = 0;

	int t0 = GetTickCount();
	//iEasingCurveType++;
	curve.SetType((KEasingCurve::Type)iEasingCurveType);

	while(true && pView->GetAniIsValid())
	{

		Sleep(10);

		int t1 = GetTickCount() - t0;
		int now = t1;
		int i = 0;
		kn_bool bMinIsBoundary = false;
		kn_bool bMaxIsBoundary = false;
		kn_int iDValue = 0;
		for (LSTVIEW::iterator itr = vecTextView.begin() ; 
			itr != vecTextView.end() ; ++itr)
		{
			RERect rect = vecRect[i];
			kn_int newX = rect.left() - iValue;
			if (newX > pView->GetRect().width()/2 && i == 0)
			{
				iDValue = newX - pView->GetRect().width()/2;
				bMinIsBoundary = true;
			}
			if (bMinIsBoundary)
			{
				newX -= iDValue;
			}

			if (newX < -iDis + pView->GetRect().width()/2 + iSectionDisPix && i == 0)
			{
				iDValue = pView->GetRect().width()/2 - newX - iDis + iSectionDisPix;
				bMaxIsBoundary = true;
			}
			if (bMaxIsBoundary)
			{
				newX += iDValue;
			}
			int x = curve.GetInterValue(rect.left() ,newX - rect.width()/2,now , TIME_MAX);
			int y = curve.GetInterValue(rect.top(),rect.top(),now , TIME_MAX);

			CPropertyPos* pPoint = new CPropertyPos((*itr), x, y);
			(*itr)->GetScreen()->addProperty(pPoint);


			if ((*itr)->GetRect().left() > pView->GetRect().width() - 50 || 
				(*itr)->GetRect().left() < 0)
			{
				(*itr)->SetShow(FALSE);
			}
			else
			{
				(*itr)->SetShow(TRUE);
			}
			i++;
		}
		


		pView->GetScreen()->SetRenderFlag(TRUE);
		if (now >= TIME_MAX)
		{
			break;
		}
	}
	return 0;
}
void KSilderTextView::MoveTo(kn_int iValue)
{
	BackupTextRect();
	NormalMove(iValue);
}
kn_int KSilderTextView::GetMoveDis()
{
	return m_iMoveDis;
}
void KSilderTextView::SetMoveDis( kn_int iDis )
{
	m_iMoveDis = iDis;
}
LSTVIEW KSilderTextView::GetTextView()
{
	return m_vecTextView;
}
kn_int KSilderTextView::GetTextDis()
{
	return m_iDis;
}
kn_bool KSilderTextView::GetAniIsValid()
{
	return m_bAniIsValid;
}
void KSilderTextView::SetAniIsValid( kn_bool b )
{
	m_bAniIsValid = b;
	if (!b)
	{
		m_ani_thread.clearAnimation();
		m_ani_thread.Stop();
	}
}
void KSilderTextView::BackupTextRect()
{
	m_vecRect.clear();
	for (LSTVIEW::iterator it = m_vecTextView.begin();
		it != m_vecTextView.end(); ++it)
	{
		m_vecRect.push_back((*it)->GetRect());
	}
}
vector<RERect> KSilderTextView::GetBackupTextRect()
{
	return m_vecRect;
}
kn_int KSilderTextView::GetSectionDisPix()
{
	return m_iSectionDisPix;
}
kn_bool KSilderTextView::IsShortcutArea(kn_int iPis,RERect& r,kn_int& iRect)
{
	kn_bool bIs = false;
	kn_int i = 0;
	for (LSTVIEW::iterator it = m_vecTextView.begin();
		it != m_vecTextView.end(); ++it)
	{
		KImgButtonView_PTR pView = VIEW_PTR(KImgButtonView )(*it);
		RERect rect = pView->GetRect();
		if (iPis >= rect.left() && iPis <= rect.right())
		{
			RERect bgRect = this->GetRect();
            pView->setState(KTextView::BS_FOCUS);
			bIs = true;
			r = rect ;
			r.offset(bgRect.left(),bgRect.top() + 1);
			iRect = i;
		}else
		{
            pView->setState(KTextView::BS_NORMAL);
		}
		++i;
	}
	return bIs;
}
void KSilderTextView::NormalMove(kn_int iValue)
{
	kn_bool bMinIsBoundary = false;
	kn_bool bMaxIsBoundary = false;
	kn_int iDValue = 0;
	int i = 0;
	for (LSTVIEW::iterator itr = m_vecTextView.begin() ; 
		itr != m_vecTextView.end() ; ++itr)
	{
		KImgButtonView_PTR pView = VIEW_PTR(KImgButtonView)(*itr);
		RERect rect = pView->GetRect();
		RERect rectbg = this->GetRect();
		kn_int newX = rect.left() - iValue;
		if (newX > rectbg.width()/2 && i == 0)
		{
			iDValue = newX - rectbg.width()/2 + rect.width()/2;
			bMinIsBoundary = true;
		}
		if (bMinIsBoundary)
		{
			newX -= iDValue;
		}

		if (newX < -m_iDis + rectbg.width()/2 + m_iSectionDisPix && i == 0)
		{
			iDValue = rectbg.width()/2 - newX - m_iDis + m_iSectionDisPix - rect.width()/2;
			bMaxIsBoundary = true;
		}
		if (bMaxIsBoundary)
		{
			newX += iDValue;
		}
		pView->SetPosition(newX,rect.top());
		if (newX + rect.width()/2 >= rectbg.width()|| 
			newX + rect.width()/2 <= 0)
		{
			pView->SetShow(FALSE);
		}
		else
		{
			pView->SetShow(TRUE);
		}
		i++;
	}
	ChangeBgRect();
}
void KSilderTextView::NormalMoveTo(kn_int iValue)
{

	m_iDis = 0;
	kn_bool bMinIsBoundary = false;
	kn_bool bMaxIsBoundary = false;
	kn_int iDValue = 0;
	int i = 0;
	for (LSTVIEW::iterator itr = m_vecTextView.begin() ; 
		itr != m_vecTextView.end() ; ++itr)
	{
		KImgButtonView_PTR pView = VIEW_PTR(KImgButtonView)(*itr);
		RERect rect = pView->GetRect();
		RERect rectbg = this->GetRect();
		kn_int newX = m_iDis + GetRect().width()/2 - rect.width()/2 - iValue;
		pView->SetPosition(newX,rect.top());
		if (newX + rect.width()/2 >= rectbg.width()|| 
			newX + rect.width()/2 <= 0)
		{
			pView->SetShow(FALSE);
		}
		else
		{
			pView->SetShow(TRUE);
		}
		kn_int iDis = m_vecText[i].iDis;
		if (iDis <= 0)
		{
			iDis = m_iSectionDisPix;
		}
		m_iDis += iDis;
		i++;
	}
	ChangeBgRect();
}

KTextView_PTR KSilderTextView::GetBgView()
{
	return m_p_view_bg;
}
void  KSilderTextView::ChangeBgRect()
{
	kn_int iLeft = 0;
	kn_int iWidth = this->GetRect().width();
	if (m_vecTextView[0]->GetRect().centerX() > iLeft)
	{
		iLeft += m_vecTextView[0]->GetRect().centerX();
		iWidth -= m_vecTextView[0]->GetRect().centerX();
	}
	if (m_vecTextView[m_vecTextView.size() - 1]->GetRect().centerX() < this->GetRect().width())
	{
		iWidth = m_vecTextView[m_vecTextView.size() - 1]->GetRect().centerX() - iLeft;
	}
	kn_int  h = m_vecTextView[0]->GetRect().centerY() - this->GetRect().height()/2;
	m_p_view_bg->SetRect(RERect::MakeXYWH(iLeft,h,iWidth,this->GetRect().height()));
}

void KSilderTextView::NormalAddText( IN vector<SilderValue>VecStr,
	IN kn_int iDisPix,
	KDrawable_PTR normal_l,
	KDrawable_PTR active_l,
	KDrawable_PTR normal_m,
	KDrawable_PTR active_m,
	KDrawable_PTR normal_r,
	KDrawable_PTR active_r)
{
	m_vecText = VecStr;
	m_iDis = 0;
	m_iSectionDisPix = iDisPix;
	for (LSTVIEW::iterator itr = m_vecTextView.begin() ; 
		itr != m_vecTextView.end() ; ++itr)
	{
		this->eraseView(*itr);
	}
	m_vecTextView.clear();
	if (iDisPix < 0)
	{
		iDisPix = this->GetRect().width();
	}
	for (vector<SilderValue>::iterator itr  = VecStr.begin();
		itr != VecStr.end(); ++itr)
	{
		KImgButtonView_PTR tView = KImgButtonView_PTR(new KImgButtonView);
		RERect bgRect = this->GetRect();
		RERect ImgRect = normal_l->GetRect();
		if (active_l && normal_l)
		{
			if (itr == VecStr.begin())
			{
				tView->setBKG(normal_l,active_l,active_l,active_l);
			}
			else if (itr == VecStr.end() - 1)
			{
				tView->setBKG(normal_r,active_r,active_r,active_r);
			}
			else
			{
				tView->setBKG(normal_m,active_m,active_m,active_m);
			}
		}
		kn_string str = ((kn_string)(*itr).strText);
		tView->enableMessage(false);
		tView->Create(RERect::MakeXYWH(m_iDis, 
			(bgRect.height() - ImgRect.height())/2,ImgRect.width(), ImgRect.height()) );
		tView->SetText(str.c_str());
		kn_int iFontSize = tView->GetFontSize();
		tView->SetTextBound(RERect::MakeXYWH((tView->GetRect().width() -  tView->GetTextWidth())/2  , tView->GetRect().height()/2 - iFontSize/2,tView->GetTextWidth(), iFontSize));
		tView->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE);
		AddView(tView);
		m_vecTextView.push_back(tView);

		kn_int iDis = (*itr).iDis;
		if (iDis <= 0)
		{
			iDis = iDisPix;
		}
		m_iDis += iDis;
	}
}

void KSilderTextView::NormalAddText( IN vector<SilderValue>VecStr, IN kn_int iDisPix, KDrawable_PTR normal, KDrawable_PTR active )
{
	NormalAddText(VecStr,iDisPix,normal,active,normal,active,normal,active);
}
