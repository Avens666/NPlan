//  File:        NSlideView.h
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
#ifndef _NSlideView_H_
#define _NSlideView_H_
#include "KViewGroup.h"
#include "AnimationThread.h"
#include "DataSync.h"
#include "KTimer.h"
#include "K9PatchImageDrawable.h"
#include "NSilderCursorView.h"
#include "KSurfaceManager.h"
class NSlideView : public KViewGroup
{
public:
	NSlideView(NSilderCursorView_PTR pCursor);
	~NSlideView(void);
	//imgPath:九宫格资源  bgRect:背景区域   iRollDis:可滑动区域
	void SetSlideBg(const kn_string& imgPath,const RERect& bgRect,kn_int iRollDis );
	//设置游标初始位置
	void SetCursorInitPos(const RERect& rect);

	void SetValueLimit(kn_int iMin,kn_int iMax);
	kn_int GetLimit();
	void SetValue(kn_int iValue);
	kn_int GetValue();
	sigslot::signal1<kn_int> m_value_change_sign;

	void MoveTo(float fOffset);
	void OnSilderDown();

private:
	void OnCursorMove(IN kn_int ix,IN kn_int iy);
private:
	NSilderCursorView_PTR m_pCursor;

	kn_int m_iMin;	//最小值
	kn_int m_iMax;	//最大值
	kn_int m_iValue;//当前值
	RERect m_RectCursor;	//游标初始位置


};

typedef boost::shared_ptr<NSlideView> NSlideView_PTR;

#endif	//_KMAINVIEW_H_
