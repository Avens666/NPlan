//  File:        KListViewEx.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_KListViewEx__H___
#define K_KListViewEx__H___

#include "KListView.h"
#include "KScreen.h"
#include "K9PatchImageDrawable.h"

// 方向
enum DIR
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,	
	DIR_RIGHT,	
	DIR_UNKNOWN,
};

class KListViewEx : public KListView
{
public:
	KListViewEx();
	virtual ~KListViewEx();
	virtual void shared_ptr_inited();

	void showScale(kn_bool b);

	/**
	* item横向是否可以滑动
	*/
	void itemRoll(kn_bool b);

	/**
	* 删除某一行
	*/
	bool UI_DeleteItems(kn_int index);
	bool UI_DeleteItems(LSTVIEW p,kn_bool b_need_callback = true);

	/**
	* 重载消息
	*/
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 

	virtual void onKeyUp(KMessageKey* k);

	virtual kn_bool OnUserMsg(KMessage* pMsg);
	/**
	* Item左滑动消息事件
	*/
	virtual void onLeftMove(kn_int iScreenX, kn_int iScreenY); 
	/**
	* Item右滑动消息时间
	*/
	virtual void onRightMove(kn_int iScreenX, kn_int iScreenY); 

	void UI_UpdateItems( KView_PTR p ,kn_int index);

	/**
	* 九宫格的滑块
	*/
	void setScroller(kn_string img_path);
	void setScroller();
	void enableMouseMessage(kn_bool b );
	virtual void UI_addItemView(KView_PTR pItemView);
	void SetItemClip( RERect rect );
	/**
	* 回调
	*/
	sigslot::signal1<int> m_sign_dbclick_item;
	sigslot::signal1<int> m_sign_click_item;
	sigslot::signal1<int> m_sign_left_move;
	sigslot::signal1<vector<int>&> m_sign_del;
	sigslot::signal1<int> m_sign_right_move;
	sigslot::signal1<int> m_sign_focus_item;
	
	/**
	* 设置起始行
	*/
	void setStartIndex(kn_int i);
	void changeItemHeight( kn_int i,kn_int h,kn_int i_delayed = 0,kn_bool b_now_play = true);
	void changeItemHeight(KView_PTR p,kn_int h ,kn_int i_delayed = 0,kn_bool b_now_play = true);
	void UpdateScale();
	void haveChildList(kn_bool b);
private:
	/**
	* 位移后的Item坐标还原
	*/
	void onItemRePos(KView_PTR p);
	void showScroller(kn_bool b);
	kn_bool inChildList(KMessagePos* pMsg);
protected:
	kn_bool			m_b_item_can_move;
	LSTVIEW			m_vec_discard_p;	//被删掉的指针,析构的时候自己delete
	kn_bool			m_b_scale_dir;		//计算方向
	DIR				m_dir;			//列表滑动的方向
	kn_int			m_i_down_x;
	kn_int			m_i_down_y;
	kn_dword		m_tick_down;
	kn_int			m_i_down_index;

	kn_int			m_xDiffer;
	RERect			m_rect_back;	//备份
	LSTVIEW			m_del_p_view;
	LSTVIEW			m_destruct_del;
	
	vector<kn_int>	m_v_del_index;

	kn_bool			m_do_mouse_msg;
	kn_bool			m_b_need_callback;
	kn_bool			m_b_scroller_move;
	REPoint			m_last_scroller_point;

	K9PatchImageDrawable_PTR   m_9_draw_slider;

	//��û��Ƕ����list
	kn_bool			m_b_father_list;
};
typedef boost::shared_ptr<KListViewEx> KListViewEx_PTR;
#endif // _KLISTVIEWEX
