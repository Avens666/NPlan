//  File:        NScaleSilderView.h
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
#ifndef _KMAINVIEW_H_
#define _KMAINVIEW_H_
#include "KViewGroup.h"
#include "NSilderCursorView.h"
#include "AnimationThread.h"
#include "DataSync.h"
#include "KTimer.h"
#include "K9PatchImageDrawable.h"
#include "KButtonView.h"
class NScaleSilderView : public KViewGroup
{
public:
	NScaleSilderView(void);
	~NScaleSilderView(void);

	//构造后必须调这个
	void InitCtrl();

	// 设置滑动条背景
	void SetSilderBG( IN const kn_string& strImgPath ,const RERect& rect);
	// 设置滑动条游标图片
	void SetSilderVernier(IN const kn_string& strImgPathN,IN const kn_string& setImgPathA);

	// 设置滑动条滚动方向
	void SetRollDir(IN KSILDERDIR dir);

	// 设置滑动条的最大值和最小值
	void SetMaxValue(IN kn_int iMin = 0,IN kn_int iMax = 100);

	// 获取当前值
	kn_int GetValue();

	// 设置当前值
	void SetValue(IN kn_int iValue);

	// 设置滑动条的滚动距离和滑动条中显示的文本
	void SetShowText( IN const vector<SilderValue>& vec,IN kn_int iDisPix ,kn_string normalPath =_T(""),kn_string activePath =_T(""));
	// 设置背景和游标的区域
	void SetSilderRect(IN  RERect& rectVernier);

	//设置背景的区域 PS:一定要先设置游标区域. 要根据2个的区域来算可以滑动的距离
	void SetSilderBgRect(IN RERect& rectMax,IN kn_bool bIsMini = true);


	// 设置背景隐藏的时间(单位:10毫秒)
	void SetHideBgTime(kn_int iTime);
	void AddArea(kn_int y,kn_string normalPath,kn_string activePath);
	
	sigslot::signal1<kn_int> m_silder_value_change_sign;

	sigslot::signal0<> m_signal_custom_area;
private:

	//获取本地路径
	kn_string GetLocalImgPath();

	//展开
	void OnBtnDown(kn_int iX,kn_int iY,KMessageMouse* pMsg);
	//隐藏
	void OnMinimize(IN kn_int iTime);

	//鼠标弹起
	void OnBtnUp();

	//鼠标移动的消息处理
	void OnSilderMove(IN kn_int ix,IN kn_int iy);
	void setSilderLOG(kn_int iValue);
	bool IsShortcutArea(kn_int iValue,kn_int ix,kn_int iy);
	void OnMaximize();
private:
	//最小值	
	kn_int m_iMin;

	//最大值
	kn_int m_iMax;
	
	//当前值
	kn_int m_iValue;
	//备个份
	kn_int m_iValueBack;
// 	//每次移动的距离
// 	kn_int m_iMoveValue;
	
	//滚动距离
	kn_int m_iDisPix;

	//默认每一节的滚动距离
	kn_int m_iSectionDisPix;

	//显示的文本
	vector<SilderValue> m_vecText;

	//背景控件
	KSilderTextView_PTR m_pBtnSilderBg;
	
	//滑动控件
	NSilderCursorView_PTR m_pBtnSilder;

	//原始区域
	RERect m_rectMini;
	RERect m_rectMax;
	RERect m_rectVernierStart;
	//背景是不是最小化的
	kn_bool m_bIsMini;

	KTextView_PTR m_textView;
	KTimer m_timerHide;
	// 动画线程
	CAnimationThread m_ani_thread;
	kn_int m_iCurValue;

	kn_int m_iExcursionValue;

	//是否需要添加动画
	kn_bool m_bUpAnimat;
	//动画要移动的矩形
	RERect m_rectAnimat;
	//当前矩形在第几个区域
	kn_int m_iRect;

	kn_bool m_bFirstMove;

	kn_int m_iTextValue;

	kn_int m_iXDiff;
	//用户自定义区域
	KImgButtonView_PTR m_view_user_area;
	RERect m_rect_start_userarea;
};
typedef boost::shared_ptr<NScaleSilderView> NScaleSilderView_PTR;

#endif	//_KMAINVIEW_H_
