//  File:        KScreen.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/1/7
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_SCREEN_H
#define K_SCREEN_H

#include "sigslot.h"
#include "KLayer.h"
#include "KView.h"
#include "DataSync.h"
#include "KViewGroup.h"
#include "KMenuView.h"
#include "KTipView.h"
#include "KTimer.h"
#include <map>

#define SCREEN_RENDER_MAIN_THREAD 0
#define SCREEN_RENDER_MULTI_THREAD 1

#define WIN_DEVICE          0
#define DDRAW_DEVICE        1
#define GL_DEVICE           2
#define LAYER_WIN_DEVICE    3
#define GL_LAYER_WIN_DEVICE 4
#define ANDROID_DEVICE      5
#define QT_SOFT_DEVICE      6
#define QT_GL_DEVICE        7
#define IOS_DEVICE          8
#define SKIAGL_DEVICE		9

class CNUIInstance;

//初始化需要的参数，使用结构，避免增加参数重定义函数
class KNUIInitPara
{
public:
	KNUIInitPara();
	~KNUIInitPara();
	kn_int m_w;
	kn_int m_h;
	REBitmap::Config m_format;
	HWND m_wnd;
	kn_int m_render_mode;// = SCREEN_RENDER_MULTI_THREAD;
	kn_int m_device_type;// = WIN_DEVICE;
};


#ifdef _WIN32_WCE
class KScreen : public KObject , public has_slots<>
#else
class KScreen : public KObject , public has_slots<multi_threaded_local>
#endif
{
protected:
	KNUIInitPara m_init_para;
	kn_int m_width;
	kn_int m_height;
	IRESurface* m_p_screen_surface;
	KLayer* m_p_layer;

	KViewGroup_PTR m_view_group;
	//menu和tip都在最上层显示，所以使用智能指针单独管理
	KMenuBase_PTR m_view_menu;
	KTipBase_PTR m_view_tip;
	//	窗口句柄
	HWND m_hwnd;
	HDC m_hdc;

	//是否进行渲染，渲染线程使用
	boost::shared_mutex m_render_flag_mutex;
	kn_bool m_render_flag;

	//是否启用局部刷新
	kn_bool m_b_draw_clip;
	//渲染模式，异步渲染SCREEN_RENDER_MULTI_THREAD 或者非异步渲染SCREEN_RENDER_MAIN_THREAD
	kn_int m_render_mode;
	//渲染设备类型 wnd？ GL？
	kn_int m_render_device_type;

	//渲染线程是否运行，设为false线程退出
	kn_bool m_render_run_flag;
	boost::thread* m_p_thread;
	// 显示刷新范围边线
	kn_bool m_b_draw_drawing;

	CDataSync	m_data_sync;

	LSTVIEW m_lst_modal_view;
	//建立焦点控件机制，解决拖动对象在底层的消息响应问题
	//焦点控件有优先相应消息的权利
	KView_PTR	m_focus_view;
	LSTVIEW	m_temp_focus_view;
	//active处理鼠标进入激活
	KView_PTR	m_active_view;

	LSTTIMER	m_lst_timer;
	
	//请求停止的动画id列表
	boost::shared_mutex m_stop_id_mutex;
	map<kn_int, kn_int>	m_stop_id_map; // 第一个是id, 第二个是要发送的msg id

	void dealAniStopMsg(); //zhic 20151213 处理动画播放结束消息，由动画线程发送修改为在渲染线程中发送此消息 保证获得消息后所有的动画渲染完成

	static kn_int renderThread(LPVOID lpPara);
	void Draw(kn_bool bFullRefresh);
	void Flip(LSTRECT &lst);
	//非实时刷新使用
	void OnDraw();
	void	RunRenderThread();
	void stopRenderThread();
	CNUIInstance* m_p_nui_instance;
public:
	KScreen();
	~KScreen();
	kn_bool Create(kn_int iWidth, kn_int iHeight, REBitmap::Config format, HWND hWnd,  kn_int render_mode = SCREEN_RENDER_MULTI_THREAD, kn_int device_type = WIN_DEVICE);
	kn_bool Create(KNUIInitPara& );
	void Release();

	void setDrawClip(kn_bool);
	kn_bool DoMessage(KMessage* pMsg);
	kn_int GetWidth();
	kn_int GetHeight();

	kn_bool GetRenderFlag();
	void	SetRenderFlag(kn_bool);

	kn_bool GetRenderRunFlag();
	void	SetRenderRunFlag(kn_bool);

	void	AddView(KView_PTR p);

	void	DeleteView(KView_PTR p);

	//////属性列表向上接口 b_check_change是否检查值是否相等，某些异步情况下设置要使用
	kn_bool	addProperty(CPropertyBase* p, kn_bool b_check_change=TRUE);
	//获得一个新id
	kn_int getNewID();
	//检查ID是否存在
	kn_bool checkID(kn_int&);
	//加入ID标识
	void addID(kn_int&);
	/////取得修改申请数据
	void	UI_DoSyncDataList();

	void setClipDraw(kn_bool b);

	//发送自定义消息，不同的操作系统实现不同
	void sendNUIMessage(kn_int msgid, KMessage* msg = NULL);
	void sendSystemMessage(kn_int msgid,  WPARAM, LPARAM );

	//DoModal 机制改为数组形式，可以支持demodal嵌套
	void addModalView(KView_PTR pView);
	void delModalView(KView_PTR pView);
	void clearModalView();
	///////////////////
	//b标识是否调用先前view的unfocus回调，view析构时不掉用
	void setFocusView(KView_PTR v, kn_bool b = TRUE);
	KView_PTR getFocusView();
	KView_PTR getActiveView();
	KView_PTR getModalView();
	
	//取临时的Focusview，在启动消息循环时赋值和使用，防止focusview在消息循环过程中被改动造成重复处理消息
	KView_PTR getTempFocusView();
//	KView* getTempModalView();

	void setViewActive(KView_PTR v, kn_bool b = TRUE);

	//显示menu
	void showMenu(KMenuBase_PTR );
	void showTip(KTipBase_PTR );
	//传入id删除对应id tip，id不符合不删除， -1就强制删除
	void hideTip(int id = NO_TIMER);

	void resize(int w, int h);
	HWND getWnd();
	void setWnd(HWND);

	void setNUIInstance(CNUIInstance* p);
	CNUIInstance* getNUIInstance();

	void addStopId(kn_int& id, kn_int& msg_id);

};


#endif
