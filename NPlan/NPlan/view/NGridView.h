//  File:        NGridView.h
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
#ifndef _NStaffGridView_H_
#define _NStaffGridView_H_

#include "KViewGroup.h"
#include "KButtonView.h"
#include "KTextView.h"
#include "KSurfaceManager.h"
#include "NStaffView.h"
#include "K9PatchImageDrawable.h"
#include "AnimationThread.h"
#include "NSilderCursorView.h"
#include "KSlideView.h"
class NGridView : public KViewGroup
{
public:
	NGridView( kn_int iWidth,kn_int iHeight,const RERect& rect);

	//添加
	virtual void AddView(KView_PTR v);
	//设置滚动方向
	void SetDir(KSILDERDIR dir);
	KSILDERDIR GetDir();

	virtual void SetRect(const RERect& rect);
	virtual void eraseView(KView_PTR v);
	//绑定滑动条
	void BindSlide(KSlideView_PTR pSlide);

	void onSlideMove(int);

	//移动
	void MoveTo(float fOffset);
	void WheelMove(float fOffset);

	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 

	virtual void onDragDirect(KMessageDrag* mag);
	void updateChildPos();
	void resetCursorPos();
	void onGridItemDoubleClicked(kn_int x, kn_int y,KMessageMouse* msg);
	sigslot::signal1<KMessageDrag*> m_sign_Drag;
	sigslot::signal1<KView_PTR> m_sign_GridItemDClicked;

	void setPreHeight(int);
private:

	//滑动条
	KSlideView_PTR m_pSlide;
	//宽
	kn_int m_orgin_w;
	kn_int m_iWidth;	
	//高
	kn_int m_iHeight;
	//行
	kn_int m_iRow;	
	//列
	kn_int m_iCol;	
	//滚动方向
	KSILDERDIR m_dir;
	//格子的rect
	RERect m_rectGrid;
	kn_int m_bAuto_set_pos;	//自己计算坐标;
	kn_int m_iHideHeight;	//隐藏的高度;
	kn_int m_pre_h; //预留高度，子元素有时出于美观考虑 不顶格
	kn_int m_offset;
};

typedef boost::shared_ptr<NGridView> NGridView_PTR;
#endif
