//  File:        NStaffGridView.h
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
#include "NSlideView.h"
class NStaffGridView : public KViewGroup
{
public:
	NStaffGridView(kn_int iRow,kn_int iCol,KDrawable_PTR p);
	NStaffGridView(kn_int iRow,kn_int iCol,const kn_string& imgPath,const RERect& rect);
	~NStaffGridView(void);

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void onWheelDirect(KMessageMouseWheel* pMsg);

	kn_int GetRow();
	kn_int GetCol();
	
	//设置滚动方向
	void SetDir(KSILDERDIR dir);
	KSILDERDIR GetDir();

	//获取总数
	kn_int GetStaffCount();

	//根据index获取人员
	NStaffView_PTR GetStaffByIndex(kn_int i);

	//根据名字获取人员
	VECSTAFF GetStaffByName(const kn_string& strName);
	
	//添加
	void AddStaff(NStaffView_PTR pView);

	//绑定滑动条

	void BindSlide(NSlideView_PTR pSlide);

	//获取小格区域
	void GetChildViewGrid(RERect& rect);
	//设置小格区域
	void SetChildViewGrid(const RERect& rect);

	void MoveTo(float fOffset);
	//设置View在Grip中的偏移
	void SetRectOffect(kn_int iLeft,kn_int iTop,kn_int iRight,kn_int iBottom);
private:
 	//坐标移动的动画
// 	void addViewMoveAnimation( KView* view,CAnimationThread &animaThread, kn_int animaDuration, kn_int destX, kn_int destY, KEasingCurve::Type curveType, CNUIAnimation::LoopType loopType,kn_int loopTime);
 	//初始化属性
 	void Init( kn_int iRow,kn_int iCol);

	void OnSilderDown();

private:

	KViewGroup_PTR m_Groupview_grid;

	NSlideView_PTR m_pSlide;

	//人员的数组
	VECSTAFF m_vecStaff;

	//行
	kn_int m_iRow;	

	//列
	kn_int m_iCol;	

	//滚动方向
	KSILDERDIR m_dir;

	//格子的rect
	RERect m_rectGrid;
	//格子的rect
	RERect m_rectBG;

	//动画线程
	CAnimationThread m_ani_thread;

	kn_bool m_bListCanMove;

	kn_int m_iClick_x;
	kn_int m_iClick_y;

	kn_int m_bAuto_set_pos;	//自己计算坐标;

	kn_int m_iHideHeight;	//隐藏的高度;

};

typedef boost::shared_ptr<NStaffGridView> NStaffGridView_PTR;
#endif

