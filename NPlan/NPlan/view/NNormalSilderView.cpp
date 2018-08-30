//  File:        NNormalSilderView.cpp
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
#include "NNormalSilderView.h"
#include "NSilderCursorView.h"
#include "KButtonView.h"
#include "KEasingCurve.h"
#include "KScreen.h"
#include "KSurfaceManager.h"
extern kn_int g_iScreenWidth;
extern kn_int g_iScreenHeight;
NNormalSilderView::NNormalSilderView(void)
{
	m_bIsMini = true;
	m_iValue = 0;
	m_iExcursionValue = 0;
	m_iRect = 0;
	m_bUpAnimat = false;
	m_bFirstMove = true;
	m_iXDiff = 0;
	m_b_auto_hide = false;
	m_iValueBack = m_iValue;
}

NNormalSilderView::~NNormalSilderView(void)
{
}

//初始化
void NNormalSilderView::InitCtrl()
{

	m_pBtnSilderBg =KSilderTextView_PTR( new KSilderTextView );

	AddView(m_pBtnSilderBg);

	m_pBtnSilder = NSilderCursorView_PTR(new NSilderCursorView);
	AddView(m_pBtnSilder);

	//m_textView =KTextView_PTR( new KTextView);

	//m_textView->Create(RERect::MakeXYWH(50,50,100,100));

	//AddView(m_textView);



	m_pBtnSilder->m_sign_zoom_start.connect(this,&NNormalSilderView::OnBtnDown);
	m_pBtnSilder->m_sign_zoom_finish.connect(this,&NNormalSilderView::OnBtnUp);

	m_pBtnSilder->m_sign_move.connect(this,&NNormalSilderView::OnSilderMove);
}
//设置背景
void NNormalSilderView::SetSilderBG( IN const kn_string& strImgPath ,const RERect& rect)
{
	if (m_pBtnSilderBg)
	{
		K9PatchImageDrawable_PTR p  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(strImgPath.c_str()),true));
		p->SizeToImage();
		RERect rBound;
		rBound.setXYWH(0,0,rect.width(),rect.height());
		p->SetRect(rBound);
		RERect rec = rect;
		SetSilderBgRect(rec,false);
		m_pBtnSilderBg->setBKDrawable(p);

	}
}
//设置游标
void NNormalSilderView::SetSilderVernier( IN const kn_string& strImgPathN,IN const kn_string& setImgPathA )
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
void NNormalSilderView::SetRollDir( IN KSILDERDIR dir )
{
	if (m_pBtnSilder)
	{
		m_pBtnSilder->SetRollDir(dir);
	}
}

//设置最大值和最小值	//TODO
void NNormalSilderView::SetMaxValue( IN kn_int iMin /*= 0*/,IN kn_int iMax /*= 100*/ )
{
	if (m_iMin != iMin || m_iMax != iMax)
	{
		m_iMin = iMin;
		m_iMax = iMax;
	}
}

//设置当前值
void NNormalSilderView::SetValue( IN kn_int iValue)
{
	if (m_iTextValue != iValue)
	{
		m_iTextValue = iValue;
	}
	if (m_vecText.empty())
	{
		return;
	}
	if (!m_vecText.empty())
	{
		if (iValue < m_vecText[0].iValue)
		{
			iValue = m_vecText[0].iValue;
		}
		if (iValue > m_vecText.back().iValue)
		{
			iValue = m_vecText.back().iValue;
		}
	}
	if (!m_vecText.empty() && iValue <= m_vecText[0].iValue)
	{
		m_pBtnSilder->SetPositionX(m_rectMax.left());
		m_iValueBack = 0;
		m_iValue = 0;
	}
	else
	{

		//如果自动隐藏 就移动进度条.否则移动小球
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

				if (m_b_auto_hide)
				{
					m_pBtnSilderBg->NormalMoveTo(iPos);
				}
				else
				{
					m_pBtnSilder->SetPositionX(m_rectMax.left()+iPos);
				}
				m_iValueBack = iPos;
				m_iValue = iPos;
				break;
			}
			iTotal += iSectionPix;
			if (iValue == m_vecText[i].iValue)
			{
				if (m_b_auto_hide)
				{
					m_pBtnSilderBg->NormalMoveTo(iTotal);
				}
				else
				{
					m_pBtnSilder->SetPositionX(m_rectMax.left()+iTotal);
				}
				m_iValueBack = iTotal;
				m_iValue = iTotal;
				break;

			}}
	}

}

//获取当前值
kn_int NNormalSilderView::GetValue()
{
	return m_iTextValue;
}

//设置显示的文本
void NNormalSilderView::SetShowText( IN const vector<SilderValue>& vec,
	IN  kn_int iDisPix ,
	IN const kn_string& normalPath_left,
	IN const kn_string& activePath_left,
	IN const kn_string& normalPath_middle,
	IN const kn_string& activePath_middle,
	IN const kn_string& normalPath_right,
	IN const kn_string& activePath_right)
{
	m_vecText.clear();
	std::copy(vec.begin(),vec.end(),insert_iterator<vector<SilderValue> >(m_vecText,m_vecText.begin()));
	KImageDrawable_PTR normal_l,active_l,normal_m,active_m,normal_r,active_r;
	if (!normalPath_left.empty())
	{
		normal_l = KImageDrawable_PTR(new KImageDrawable( normalPath_left));
	}
	if (!activePath_left.empty())
	{
		active_l = KImageDrawable_PTR(new KImageDrawable(activePath_left));
	}	
	if (!normalPath_middle.empty())
	{
		normal_m = KImageDrawable_PTR(new KImageDrawable(normalPath_middle));
	}	
	if (!activePath_middle.empty())
	{
		active_m = KImageDrawable_PTR(new KImageDrawable( activePath_middle));
	}
	if (!normalPath_right.empty())
	{
		normal_r = KImageDrawable_PTR(new KImageDrawable(normalPath_right));
	}
	if (!activePath_right.empty())
	{
		active_r = KImageDrawable_PTR(new KImageDrawable( activePath_right));
	}

	m_iSectionDisPix = (iDisPix - active_l->GetRect().width())/(vec.size() - 1);
	m_pBtnSilderBg->NormalAddText(m_vecText,m_iSectionDisPix,normal_l,active_l,normal_m,active_m,normal_r,active_r);

	m_iDisPix = m_pBtnSilderBg->GetTextDis();

}

void NNormalSilderView::SetShowText( IN const vector<SilderValue>& vec, IN kn_int iDisPix , IN const kn_string& normalPath, IN const kn_string& activePath )
{
	SetShowText(vec,iDisPix ,normalPath,activePath,normalPath,activePath,
		normalPath,
		activePath);
}

//设置游标的大小
void NNormalSilderView::SetSilderRect( IN  RERect& rectVernier )
{
	if (m_pBtnSilder)
	{
		m_rectVernierStart = rectVernier;
		m_pBtnSilder->Create(rectVernier);
	}
}
//鼠标点击消息
void NNormalSilderView::OnBtnDown(kn_int iX,kn_int iY,KMessageMouse* pMsg)
{
	m_bUpAnimat = false;
	m_rectVernierStart = m_pBtnSilder->GetRect();
	m_iXDiff = iX - m_rectVernierStart.centerX();
}

//鼠标弹起
void NNormalSilderView::OnBtnUp()
{
	if (m_iValue != 0)
	{
		m_iValueBack = m_iValue;
	}
	m_signal_change_end.emit();
}

//设置背景大小
void NNormalSilderView::SetSilderBgRect( IN RERect & rectMax,kn_bool bIsMini /*= true*/ )
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
void NNormalSilderView::OnSilderMove(IN kn_int ix,IN kn_int iy)
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

		WCHAR wbuff[64] = {0};
		converValue(m_iValueBack+m_iCurValue);

		_stprintf(wbuff,_T("X:%d y:%d center:%d move:%d value:%d \n"),ix,iy,iCenterPos,m_iCurValue,m_iTextValue);
		OutputDebugString(wbuff);

	}

}
//坐标转化成实际值
void NNormalSilderView::converValue(kn_int iValue)
{
	if (m_vecText.empty())
	{
		return;
	}

	//实际值
	kn_int i_value;
	if (iValue == 0)
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

		float fVal(0) ;

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
	if (m_iTextValue != i_value)
	{//值改变才激活回调

		m_iTextValue = i_value;
		m_silder_value_change_sign.emit(m_iTextValue);
	}
}


//是否在区域内
bool NNormalSilderView::IsShortcutArea(kn_int iValue,kn_int ix,kn_int iy)
{
	//给状态赋值.弹起的时候添加动画
	m_bUpAnimat = m_pBtnSilderBg->IsShortcutArea(iValue,m_rectAnimat,m_iRect);
	if (m_bUpAnimat)
	{
		m_iCurValue = m_rectAnimat.centerX() -  m_rectVernierStart.centerX();
		m_pBtnSilder->SetCanMove(false,m_rectAnimat);
		return false;
	}
	m_pBtnSilder->SetCanMove(true,m_rectVernierStart);
	m_bFirstMove = true;
	UpdateUI();
	return false;
}
