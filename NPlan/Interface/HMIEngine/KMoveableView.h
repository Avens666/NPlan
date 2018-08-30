//  File:        KMoveableView.h
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
#ifndef K_MOVEABLE_VIEW_H__
#define K_MOVEABLE_VIEW_H__

#include "KView.h"
#include "KTextView.h"

class KMoveableView : public KTextView
{
public:
	KMoveableView();
	virtual ~KMoveableView(void);
	// Slot
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	void enableXY(kn_bool b_x, kn_bool b_y);
	void setMovArea(RERect&);

	sigslot::signal0<> m_sign_view_move;
protected:
	kn_bool m_b_x; // x方向能否移动
	kn_bool m_b_y; // x方向能否移动

	RERect  m_move_area; //可移动的范围，左上角坐标相对父view的坐标范围
	REPoint  m_down_pos;
	
};
typedef boost::shared_ptr<KMoveableView> KMoveableView_PTR;

class KMoveBackView : public KMoveableView
{
protected:
	//返回位置，这是相对于父坐标的
	REPoint m_stack_point;
	CAnimationThread m_mov_ani_thread;
public:
	KMoveBackView();
	virtual ~KMoveBackView(void);
	void setStackPoint(int x, int y);

	virtual void OnUp(kn_int iScreenX, kn_int iScreenY,KMessageMouse* pMsg);
	virtual REPoint getStackPoint();
};
typedef boost::shared_ptr<KMoveBackView> KMoveBackView_PTR;
#endif

