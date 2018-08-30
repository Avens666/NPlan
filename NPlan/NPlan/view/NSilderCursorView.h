//  File:        NSilderCursorView.h
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
#ifndef _NSilderCursorView_H_
#define _NSilderCursorView_H_
#include "KMoveableView.h"
#include "KViewGroup.h"
#include "KEasingCurve.h"
#include "AnimationThread.h"
//方向
enum KSILDERDIR
{
	DIR_X,	//横向
	DIR_Y,	//竖向
};
struct SilderValue
{
	//显示值
	kn_string strText;
	//内部值
	kn_int	  iValue;
	//距离下一个值的距离.  PS:是计算到下一个的距离,所以最后一个值的这个属性是没有用滴)
	kn_int	  iDis;

	SilderValue();
	SilderValue(const kn_string& str,kn_int i,kn_int dis = 0)
	{
		strText = str;
		iValue = i;
		iDis = dis;
	}
};
class NSilderCursorView:
	public/* KMoveBackView*/ KMoveableView
{
public:
	NSilderCursorView(void);
	~NSilderCursorView(void);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual kn_bool Create(RERect& rect);

	sigslot::signal2<kn_int,kn_int> m_sign_move;
	sigslot::signal1<kn_int> m_sign_zoom_out;
	sigslot::signal0<> m_sign_showbtn;
	sigslot::signal3<kn_int,kn_int,KMessageMouse*> m_sign_zoom_start;
	sigslot::signal0<> m_sign_zoom_finish;

	void  setBKGImage(const kn_string& normalResPath, const kn_string& pressedResPath);
	void setMaxStepSize(kn_int iStep,kn_bool bCanBoth = true);
	//设置滚动方向
	void SetRollDir(KSILDERDIR dir);
	KSILDERDIR GetRollDir();
	//设置滚动方向
	void SetCanMove(kn_bool b,const RERect& rect);
	void  setBKG( KDrawable_PTR normal, KDrawable_PTR pressed);
	//是否支持上下左右都能移动
	void SetAllDir(kn_bool b);
protected:
	void setState( ViewState state, kn_bool bRefresh=TRUE );
private:
	KDrawable_PTR m_normal_bk_drawable;
	KDrawable_PTR m_pressed_bk_drawable;
	REPoint m_startPoint;//记录mousedown坐标
	REPoint m_startCnterPoint;
	RERect m_startRect;
	kn_int m_MaxStepSize;

	kn_bool bValidClick;
	kn_int m_recordY;
	kn_int m_yDiffer;
	KSILDERDIR m_Direction;

	kn_int m_recordX;
	kn_int m_xDiffer;
	kn_bool m_bCanMove;
	kn_bool m_bCanBoth;	//是否正反都可以移动
	kn_bool m_bIsFisrtDown;
	kn_bool m_bIsAllMove;
};

typedef boost::shared_ptr<NSilderCursorView> NSilderCursorView_PTR;

class KSilderTextView:
	public KViewGroup
{
public:
	KSilderTextView(void);
	~KSilderTextView(void);
	//设置背景
	void  setBKDrawable( KDrawable_PTR normal);
	//添加文字 normal的是后面添加不隐藏的滑动条
	void AddText( IN vector<SilderValue>VecStr,IN kn_int iDisPix,KDrawable_PTR normal,KDrawable_PTR active);
	void NormalAddText( IN vector<SilderValue>VecStr,
		IN kn_int iDisPix,
		KDrawable_PTR normal_l,
		KDrawable_PTR active_l,
		KDrawable_PTR normal_m, 
		KDrawable_PTR active_m,
		KDrawable_PTR normal_r, 
		KDrawable_PTR active_r);
	void NormalAddText( IN vector<SilderValue>VecStr,
		IN kn_int iDisPix,
		KDrawable_PTR normal,
		KDrawable_PTR active);
	//getdis
	kn_int GetTextDis();
	//文字移动
	void MoveText(kn_int iValue);
	//移动到指定位置
	void MoveTo(kn_int iValue);
	//设置
	void SetMoveDis(kn_int iDis);
	void SetAniIsValid(kn_bool b);

	//获取
	kn_int GetMoveDis();
	kn_bool GetAniIsValid();
	kn_int 	GetSectionDisPix();
	//获取文字View
	LSTVIEW GetTextView();
	//获取文字View
	vector<RERect> GetBackupTextRect();

	kn_bool IsShortcutArea(kn_int iPis,RERect& r,kn_int& iRect);
	//绝对移动
	void NormalMoveTo(kn_int iValue);

	KTextView_PTR GetBgView();
private:
	//备份文字的初始化位置
	void BackupTextRect();
	//正常移移动,没有动画
	void NormalMove(kn_int iValue);
	void ChangeBgRect();
private:
	KTextView_PTR			m_p_view_bg;

	KDrawable_PTR		m_bk_drawable;

	LSTVIEW				m_vecTextView;

	kn_int				m_iDis;

	kn_int				m_iMoveDis;
	
	kn_bool				m_bAniIsValid;

	vector<RERect>		m_vecRect;

	CAnimationThread m_ani_thread;

	vector<SilderValue> m_vecText;


	//默认每一节的滚动距离
	kn_int m_iSectionDisPix;

};

typedef boost::shared_ptr<KSilderTextView> KSilderTextView_PTR;

#endif //_NSilderCursorView_H_
