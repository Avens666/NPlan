//  File:        NScaleSilderView.cpp
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
#include "NScaleSilderView.h"
#include "NSilderCursorView.h"
#include "KButtonView.h"
#include "KEasingCurve.h"
#include "KScreen.h"
#include "KSurfaceManager.h"
extern kn_int g_iScreenWidth;
extern kn_int g_iScreenHeight;
NScaleSilderView::NScaleSilderView(void)
{
	m_bIsMini = true;
	m_iValue = 0;
	m_iExcursionValue = 0;
	m_iValueBack = m_iValue;
	m_iRect = 0;
	m_bUpAnimat = false;
	m_bFirstMove = true;
	m_iXDiff = 0;
}

NScaleSilderView::~NScaleSilderView(void)
{
}
//初始化
void NScaleSilderView::InitCtrl()
{
	
	m_pBtnSilderBg =KSilderTextView_PTR( new KSilderTextView );

	AddView(m_pBtnSilderBg);

	m_textView =KTextView_PTR( new KTextView);

	m_textView->Create(RERect::MakeXYWH(50,50,100,100));

	AddView(m_textView);

	m_pBtnSilder = NSilderCursorView_PTR(new NSilderCursorView);

	AddView(m_pBtnSilder);

	//m_pBtnSilder->SetAllDir(true);

	m_pBtnSilder->m_sign_zoom_start.connect(this,&NScaleSilderView::OnBtnDown);
	m_pBtnSilder->m_sign_zoom_finish.connect(this,&NScaleSilderView::OnBtnUp);

	m_pBtnSilder->m_sign_move.connect(this,&NScaleSilderView::OnSilderMove);
	m_timerHide.m_timeout_signal.connect(this, &NScaleSilderView::OnMinimize);
//	m_pBtnSilder->m_sign_active.connect(this,&NScaleSilderView::OnMaximize);
}
//设置背景
void NScaleSilderView::SetSilderBG( IN const kn_string& strImgPath ,const RERect& rect)
{
	if (m_pBtnSilderBg)
	{
		K9PatchImageDrawable_PTR p  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(strImgPath.c_str()),true));
		p->SizeToImage();

		RERect rBound;
		rBound.setXYWH(0,0,rect.width(),rect.height());
		p->SetRect(rBound);
		RERect rec = rect;
		SetSilderBgRect(rec);
		m_pBtnSilderBg->setBKDrawable(p);
		//m_pBtnSilderBg->SetClip(RERect::MakeXYWH(rec.left(),0,rec.width(),g_iScreenHeight));

	}
}
//设置游标
void NScaleSilderView::SetSilderVernier( IN const kn_string& strImgPathN,IN const kn_string& setImgPathA )
{
	if (m_pBtnSilder)
	{	

		KDrawable_PTR normal = KImageDrawable_PTR(new KImageDrawable(strImgPathN));
		KDrawable_PTR pressed = KImageDrawable_PTR(new KImageDrawable(setImgPathA));
		
		m_pBtnSilder->setBKG(normal,pressed);
		m_rectVernierStart.setXYWH((kn_int)(m_rectMax.left() + (m_rectMax.width() - normal->GetRect().width())/2),
			m_rectMax.top() + (kn_int)(m_rectMax.height() - normal->GetRect().height())/2,
			normal->GetRect().width(),normal->GetRect().height());
		m_pBtnSilder->Create(m_rectVernierStart);
	}

}
//设置滚动方向 //TODO
void NScaleSilderView::SetRollDir( IN KSILDERDIR dir )
{
	if (m_pBtnSilder)
	{
		m_pBtnSilder->SetRollDir(dir);
	}
}
//设置最大值和最小值	//TODO
void NScaleSilderView::SetMaxValue( IN kn_int iMin /*= 0*/,IN kn_int iMax /*= 100*/ )
{
	if (m_iMin != iMin || m_iMax != iMax)
	{
		m_iMin = iMin;
		m_iMax = iMax;
	}
}
//设置当前值
void NScaleSilderView::SetValue( IN kn_int iValue)
{
	if (m_iTextValue != iValue)
	{
		m_iTextValue = iValue;
	}
	if (m_vecText.empty())
	{
		return;
	}
	kn_int iPos = 0,iTotal = 0;
	for (int i =  1 ; i < m_vecText.size() ; i++)
	{
		kn_int iSectionPix = m_vecText[i - 1].iDis > 0 ? m_vecText[i - 1].iDis : m_iSectionDisPix;
		if (iValue < m_vecText[i].iValue && iValue >= m_vecText[i - 1].iValue)
		{

			if (!m_pBtnSilderBg->GetTextView().empty())
			{
				iSectionPix -= m_pBtnSilderBg->GetTextView()[0]->GetRect().width();
			}
			//实际的值
			float f1 = iValue - m_vecText[i - 1].iValue;
			//一段的值
			float f2 = m_vecText[i].iValue - m_vecText[i - 1].iValue;
			//转成像素
			kn_int iPix = (f1/f2)*iSectionPix;

			iPos = iTotal + iPix + m_pBtnSilderBg->GetTextView()[0]->GetRect().width()/2;

			m_pBtnSilderBg->NormalMoveTo(iPos);

			m_iValueBack = iPos;
			m_iValue = iPos;
			break;
		}
		iTotal += iSectionPix;
		if (iValue == m_vecText[i].iValue)
		{
			m_pBtnSilderBg->NormalMoveTo(iTotal);
			m_iValueBack = iTotal;
			m_iValue = iTotal;
			break;
		}
	}

}
//获取当前值
kn_int NScaleSilderView::GetValue()
{
	return m_iTextValue;
}
//设置显示的文本
void NScaleSilderView::SetShowText( IN const vector<SilderValue>& vec,IN  kn_int iDisPix ,kn_string normalPath,kn_string activePath)
{
	m_vecText.clear();


    std::copy(vec.begin(),vec.end(),insert_iterator< vector<SilderValue> >(m_vecText,m_vecText.begin()));
	KImageDrawable_PTR normal,active;
	if (!normalPath.empty())
	{
		normal = KImageDrawable_PTR(new KImageDrawable( normalPath));
	}
	if (!activePath.empty())
	{
		active = KImageDrawable_PTR(new KImageDrawable(activePath));
	}
	m_pBtnSilderBg->AddText(m_vecText,iDisPix,normal,active);
	
	m_iSectionDisPix = iDisPix;
	m_iDisPix = m_pBtnSilderBg->GetTextDis();

}
//获取本地图片的路径
kn_string NScaleSilderView::GetLocalImgPath()
{
	TCHAR szFilePath[256];
	GetModuleFileName(NULL,szFilePath,256);
	kn_string path=szFilePath;
	path = path.substr(0,path.rfind(_T('\\')));
	path = path.substr(0,path.rfind(_T('\\')));
	path+=_T("\\SilderDemo\\");
	return path;
}
//设置游标的大小
void NScaleSilderView::SetSilderRect( IN  RERect& rectVernier )
{
	if (m_pBtnSilder)
	{
		m_rectVernierStart = rectVernier;
		m_pBtnSilder->Create(rectVernier);
	}
}
//鼠标点击消息
void NScaleSilderView::OnBtnDown(kn_int iX,kn_int iY,KMessageMouse* pMsg)
{
	m_ani_thread.clearAnimation();
	m_ani_thread.Stop();
	if (m_timerHide.isActive())
	{
		m_timerHide.stop();
	}

	m_iCurValue = 0;
	m_pBtnSilderBg->SetAniIsValid(false);
	m_bUpAnimat = false;
	OnMaximize();
	m_iXDiff = iX - m_rectVernierStart.centerX();
}
//最小化
void NScaleSilderView::OnMinimize(kn_int iTime)
{
	if (m_ani_thread.getRunState() == CNUIAnimation::PLAYING)
	{
		m_ani_thread.Stop();

		m_ani_thread.clearAnimation();
	}
	if ((!m_bIsMini) &&(m_ani_thread.getRunState() != CNUIAnimation::PLAYING))
	{
		m_timerHide.stop();

		CNUIAnimation* pBackAnima = new CNUIAnimation (300, 0,0);
		CPropertyPos* pPointEnd = new CPropertyPos(m_pBtnSilder,m_rectVernierStart.left(),m_rectVernierStart.top());
		CPropertyAnimationData* pBackAnimationData = new CPropertyAnimationData(pPointEnd);
		pBackAnima->addAnimationData(pBackAnimationData);
		m_ani_thread.addAnimation(pBackAnima);

		CNUIAnimation* pNUIAnimation = new CNUIAnimation (300, 0,0);
		CPropertyOpacity* pRectStart = new CPropertyOpacity(m_pBtnSilderBg,0);
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pRectStart);
		pNUIAnimation->addAnimationData(pAnimationData);
		if (m_view_user_area)
		{
			CPropertyOpacity* pUserRectStart = new CPropertyOpacity(m_view_user_area,0);
			CPropertyAnimationData* pUseAnimationData = new CPropertyAnimationData(pUserRectStart);
			pNUIAnimation->addAnimationData(pUseAnimationData);
		}
		m_ani_thread.addAnimation(pNUIAnimation);
		m_ani_thread.Start();

		m_bIsMini = true;

		m_pBtnSilderBg->MoveTo(m_iCurValue);

		if (m_iValue != 0)
		{
			m_iValueBack = m_iValue;
		}
	}
}
//鼠标弹起
void NScaleSilderView::OnBtnUp()
{
	//m_timerHide.Start(1, 2000);
	OnMinimize(0);
}
//设置背景大小
void NScaleSilderView::SetSilderBgRect( IN RERect & rectMax,kn_bool bIsMini /*= true*/ )
{
	if (m_pBtnSilderBg)
	{
		m_bIsMini = bIsMini;
		m_rectMax = rectMax;
		m_rectMini = RERect::MakeXYWH(rectMax.x() + rectMax.width()/2,rectMax.y() + rectMax.height() / 2,0,0);
		if (bIsMini)
		{
			m_pBtnSilderBg->setOpacity(0);
			m_pBtnSilderBg->Create(rectMax);
		}
		else
		{
			m_pBtnSilderBg->Create(rectMax);
			
		}
	}
	if (m_pBtnSilder)
	{
		m_pBtnSilder->setMaxStepSize(rectMax.width()/2 - m_rectVernierStart.width()/2);
	}

}
//鼠标移动
void NScaleSilderView::OnSilderMove(IN kn_int ix,IN kn_int iy)
{
	if (ix  - m_pBtnSilderBg->GetRect().left()< m_pBtnSilderBg->GetTextView()[0]->GetRect().centerX())
	{
		if (m_bFirstMove)
		{
			RERect rect = m_pBtnSilderBg->GetTextView()[0]->GetRect();
			rect.offset(m_pBtnSilderBg->GetRect().left(),m_pBtnSilderBg->GetRect().top());
			m_pBtnSilder->SetCanMove(false,rect);
			m_bFirstMove = false;
		}
	}
	else if (ix - m_pBtnSilderBg->GetRect().left() > m_pBtnSilderBg->GetTextView()[m_pBtnSilderBg->GetTextView().size() - 1]->GetRect().centerX())
	{
		if (m_bFirstMove)
		{
			RERect rect = m_pBtnSilderBg->GetTextView()[m_pBtnSilderBg->GetTextView().size() - 1]->GetRect();
			rect.offset(m_pBtnSilderBg->GetRect().left(),m_pBtnSilderBg->GetRect().top());
			m_pBtnSilder->SetCanMove(false,rect);
			m_bFirstMove = false;
		}
	}
	else
	{
		kn_int i_value;
		m_pBtnSilder->SetCanMove(true,m_pBtnSilder->GetRect());
		m_bFirstMove = true;
		kn_int iCenterPos = m_rectVernierStart.centerX();
		m_iCurValue = ix - iCenterPos - m_iXDiff;

		//判断是否在快捷区域内
		RERect rectSilderBG = m_pBtnSilderBg->GetRect();
		if (IsShortcutArea(ix - rectSilderBG.left() - m_iXDiff,ix,iy))
		{
			return;
		}


		i_value = m_iValueBack + m_iCurValue ;
		if (i_value < -m_iExcursionValue)
		{
			i_value  =  -m_iExcursionValue;
		}
		kn_int iLastDis = m_vecText[m_vecText.size() - 2].iDis > 0 ? m_vecText[m_vecText.size() - 2].iDis : m_iSectionDisPix;
		if (i_value > m_iDisPix  -m_iExcursionValue  - iLastDis)
		{
			i_value = m_iDisPix  -m_iExcursionValue  - iLastDis;
		}
        kn_char wbuff[64] = {0};
		_stprintf(wbuff,_T("X:%d y:%d center:%d move:%d value:%d \n"),ix,iy,iCenterPos,m_iCurValue,(m_iValue ));
		OutputDebugString(wbuff);


		setSilderLOG(i_value);


	}

}
//坐标转化成实际值
void NScaleSilderView::setSilderLOG(kn_int iValue)
{
	if (m_vecText.empty())
	{
		return;
	}

	//实际值
	kn_int i_value;
	if (m_iValue == iValue)
	{
		return;
	}
	m_iValue = iValue;

	if (m_bUpAnimat && m_iRect < m_vecText.size() && m_iRect >=0)
	{
		i_value = m_vecText[m_iRect].iValue;
	}
	else
	{

		float fVal ;

		kn_int iPis = (iValue + m_iExcursionValue);
		 kn_int iTotal = 0;
		for (kn_int i = 0 ; i < m_vecText.size() ; ++i)
		{
			kn_int iSectionPix = m_vecText[i].iDis > 0 ? m_vecText[i].iDis : m_iSectionDisPix;
			iTotal += iSectionPix;
			if (iPis <= iTotal)
			{
				fVal = (float)i + (float)( iPis -  iTotal +  iSectionPix - m_rectVernierStart.width()/2)/(float)(iSectionPix - m_rectVernierStart.width());
				break;
			}
		}
		kn_int i = floor(fVal);

		//值的越界赋值
		if (i < 0 )
		{
			i_value = m_vecText[0].iValue;
		}
		else if (i >= m_vecText.size() - 1)
		{
			i_value =  m_vecText[m_vecText.size() - 1].iValue;
		}
		else
		{
			//小数部分算值
			i_value = (m_vecText[i + 1].iValue - m_vecText[i].iValue)*(fVal - i) + m_vecText[i].iValue;
		}

	}
// 	WCHAR wbuf[64]= {0};
// 	_stprintf(wbuf,_T("值:%d"),i_value);
// 	m_textView->SetText(wbuf);

	if (m_iTextValue != i_value)
	{//值改变才激活回调
		m_iTextValue = i_value;
		m_silder_value_change_sign.emit(m_iTextValue);
	}
}
//是否在区域内
bool NScaleSilderView::IsShortcutArea(kn_int iValue,kn_int ix,kn_int iy)
{
	//给状态赋值.弹起的时候添加动画
	m_bUpAnimat = m_pBtnSilderBg->IsShortcutArea(iValue,m_rectAnimat,m_iRect);
	if (m_view_user_area  )
	{
		if (m_view_user_area->GetRect().contains(ix,iy) /*|| 
			iy > m_view_user_area->GetRect().centerY()*/)
		{
			if (m_bFirstMove)
			{
				m_pBtnSilder->SetCanMove(false,m_view_user_area->GetRect());
				m_signal_custom_area.emit();
				m_bFirstMove = false;
			}

			return true;
		}
		if (m_bUpAnimat)
		{
			m_iCurValue = m_rectAnimat.centerX() -  m_rectVernierStart.centerX();
//			m_pBtnSilder->SetRect(m_rectAnimat);

			m_pBtnSilder->SetCanMove(false,m_rectAnimat);
// 			if (m_bFirstMove)
// 			{
// 				m_pBtnSilder->SetCanMove(false,m_rectAnimat);
// 				m_bFirstMove = false;
// 			}

			return false;
		}
		{
			m_pBtnSilder->SetCanMove(true,m_rectVernierStart);
			m_bFirstMove = true;
// 			CPropertyPos* p = new CPropertyPos(m_pBtnSilder,ix - m_iXDiff - m_rectVernierStart.width()/2,m_rectVernierStart.top());
// 			GetScreen()->addProperty(p);
			UpdateUI();
			return false;
		}
	}
	return false;
}
//添加自定义区域
void NScaleSilderView::AddArea( kn_int y,kn_string normalPath,kn_string activePath )
{
	m_view_user_area =KImgButtonView_PTR( new KImgButtonView);

	KDrawable_PTR normal = KImageDrawable_PTR(new KImageDrawable(normalPath));
	KDrawable_PTR focus = KImageDrawable_PTR(new KImageDrawable(activePath));
	m_rect_start_userarea.setXYWH(m_rectVernierStart.centerX() - normal->GetRect().width()/2,
								  m_rectVernierStart.centerY() - normal->GetRect().height()/2 + y,
							      normal->GetRect().width(),normal->GetRect().height());
	m_view_user_area->Create(m_rect_start_userarea);

	m_view_user_area->setBKG(normal,focus,focus,focus);

	if (m_bIsMini)
	{
		m_view_user_area->setOpacity(0);
		m_view_user_area->SetBoundRect(m_rect_start_userarea);
	}
	AddView(m_view_user_area);
	changeViewLayerBottom(m_view_user_area);
}
//最大化
void NScaleSilderView::OnMaximize()
{
	if (m_bIsMini && m_ani_thread.getRunState() != CNUIAnimation::PLAYING)
	{
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (300, 0,0);
		CPropertyOpacity* pStart = new CPropertyOpacity(m_pBtnSilderBg,255);

		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pStart);
		pNUIAnimation->addAnimationData(pAnimationData);
		pNUIAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
		pNUIAnimation->setLoopTimes(1);
		if (m_view_user_area)
		{
			CPropertyOpacity* pUserRectStart = new CPropertyOpacity(m_view_user_area,255);
			CPropertyAnimationData* pUseAnimationData = new CPropertyAnimationData(pUserRectStart);
			pNUIAnimation->addAnimationData(pUseAnimationData);
		}
		m_ani_thread.addAnimation(pNUIAnimation);
		m_ani_thread.Start();
		m_bIsMini = false;
	}
}
