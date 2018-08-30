//  File:        NNormalSilderView.h
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
#ifndef _NNORMALSILDERVIEW_H_
#define _NNORMALSILDERVIEW_H_
#include "KViewGroup.h"
#include "NSilderCursorView.h"
#include "AnimationThread.h"
#include "DataSync.h"
#include "KTimer.h"
#include "K9PatchImageDrawable.h"
#include "KButtonView.h"
class NNormalSilderView : public KViewGroup
{
public:
	NNormalSilderView(void);
	virtual ~NNormalSilderView(void);

	// ���û���������
	void SetSilderBG( IN const kn_string& strImgPath ,const RERect& rect);
	// ���û������α�ͼƬ
	void SetSilderVernier(IN const kn_string& strImgPathN,IN const kn_string& setImgPathA);

	// ���û�������������
	void SetRollDir(IN KSILDERDIR dir);

	// ���û����������ֵ����Сֵ
	void SetMaxValue(IN kn_int iMin = 0,IN kn_int iMax = 100);



	// ��ȡ��ǰֵ
	kn_int GetValue();

	// ���õ�ǰֵ
	void SetValue(IN kn_int iValue);

	// ���û������Ĺ�������ͻ���������ʾ���ı�
	void SetShowText( IN const vector<SilderValue>& vec, IN kn_int iDisPix , 
		IN const kn_string& normalPath_left, 
		IN const kn_string& activePath_left, 
		IN const kn_string& normalPath_middle,
		IN const kn_string& activePath_middle,
		IN const kn_string& normalPath_right, 
		IN const kn_string& activePath_right);
	void SetShowText( IN const vector<SilderValue>& vec, IN kn_int iDisPix , 
		IN const kn_string& normalPath, 
		IN const kn_string& activePath);
	// ���ñ������α������
	void SetSilderRect(IN  RERect& rectVernier);

	//���ñ��������� PS:һ��Ҫ�������α�����. Ҫ����2��������������Ի����ľ���
	void SetSilderBgRect(IN RERect& rectMax,IN kn_bool bIsMini = true);

	sigslot::signal1<kn_int> m_silder_value_change_sign;

	sigslot::signal0<> m_signal_custom_area;

	sigslot::signal0<> m_signal_change_end;
	void InitCtrl();
private:


	//չ��
	void OnBtnDown(kn_int iX,kn_int iY,KMessageMouse* pMsg);

	//��굯��
	void OnBtnUp();

	//����ƶ�����Ϣ����
	void OnSilderMove(IN kn_int ix,IN kn_int iy);
	void converValue(kn_int iValue);

	bool IsShortcutArea(kn_int iValue,kn_int ix,kn_int iy);
private:
	//��Сֵ	
	kn_int m_iMin;

	//���ֵ
	kn_int m_iMax;

	//��ǰֵ
	kn_int m_iValue;

	//��������
	kn_int m_iDisPix;

	//Ĭ��ÿһ�ڵĹ�������
	kn_int m_iSectionDisPix;

	kn_bool	m_b_auto_hide;

	//��ʾ���ı�
	vector<SilderValue> m_vecText;

	//�����ؼ�
	KSilderTextView_PTR m_pBtnSilderBg;

	//�����ؼ�
	NSilderCursorView_PTR m_pBtnSilder;

	//ԭʼ����
	RERect m_rectMini;
	RERect m_rectMax;
	RERect m_rectVernierStart;
	//�����ǲ�����С����
	kn_bool m_bIsMini;

	KTextView_PTR m_textView;
	// �����߳�
	CAnimationThread m_ani_thread;
	kn_int m_iCurValue;

	kn_int m_iExcursionValue;

	//�Ƿ���Ҫ��Ӷ���
	kn_bool m_bUpAnimat;
	//����Ҫ�ƶ��ľ���
	RERect m_rectAnimat;
	//��ǰ�����ڵڼ�������
	kn_int m_iRect;

	kn_bool m_bFirstMove;

	kn_int m_iTextValue;

	kn_int m_iXDiff;
	//������
	kn_int m_iValueBack;
	//�û��Զ�������
	KImgButtonView_PTR m_view_user_area;
	RERect m_rect_start_userarea;
};


typedef boost::shared_ptr<NNormalSilderView> NNormalSilderView_PTR;
#endif	//_KMAINVIEW_H_
