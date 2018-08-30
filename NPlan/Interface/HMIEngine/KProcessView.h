//  File:        KProcessView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef KPROCESS_VIEW_DEFINED
#define KPROCESS_VIEW_DEFINED

#include "KButtonView.h"
#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KShapeDrawable.h"
//进度条方向
enum PROCESS_DIR
{
	PROCESS_Horizontal, //横向
	PROCESS_Vertical, //垂直
};

class KProcessView : public KStaticView
{
public:
	KProcessView() ;
	virtual ~KProcessView();
	virtual void init(PROCESS_DIR e = PROCESS_Horizontal);

	virtual void setValue(int iPos);    //设定进度的起始位置
	int getValue();
	void enableProcessClip(kn_bool );
	void setIconDrawableRect(int l, int r, int w, int h);
protected:
	kn_int m_value;           //当前进度 0~100
	PROCESS_DIR m_e_dir; //进度条方向 
	kn_bool m_b_auto_center_text; //进度文字自动居中
	kn_bool m_b_process_clip; //是否使用裁剪方式绘制进度条
};
typedef boost::shared_ptr<KProcessView> KProcessView_PTR;

class KProcessCirleView : public KProcessView
{
public:
	KProcessCirleView() ;
	virtual ~KProcessCirleView();
	void init(REColor);
	virtual void setValue(int iPos);    //设定进度的起始位置
protected:
	KShapeDrawable_PTR m_arc_drawable;

};
typedef boost::shared_ptr<KProcessCirleView> KProcessCirleView_PTR;


// 载入时转圈的动画
class KLoadingView : public KView
{
public:
	KLoadingView();
	virtual ~KLoadingView();

	// 设置动画图片
	void setCycleImage(const kn_string& strResPath);

	void setCycleDrawable(KDrawable_PTR drawable);

	// 开始旋转动画, iCycleTime : 转一圈的ms数
	void startAnimation(int iCycleTime = 500,kn_int frame_time = 500);

	void endAnimation();

protected:
	 
	// 圆圈的图片
	KDrawable_PTR m_drawable_cycle;

	CAnimationThread m_ani_thread;


};


typedef boost::shared_ptr<KLoadingView> KLoadingView_PTR;

#endif // KPROCESS_VIEW_DEFINED

