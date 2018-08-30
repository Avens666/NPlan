//  File:        KWaveEffectDrawable.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/27
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_WAVEEFFECT_DRAWABLE_H
#define K_WAVEEFFECT_DRAWABLE_H


#include "KImageDrawable.h"

#define WAVE_EFFECT_TOTALFRAME 64
class KWaveEffectDrawable : public KImageDrawable
{
public :
	KWaveEffectDrawable();
	KWaveEffectDrawable(const kn_string& strFile);
	KWaveEffectDrawable(IRESurface* pSurface, kn_bool b_out = FALSE);
	virtual ~KWaveEffectDrawable();
	unsigned char* m_p_data;
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);
	void setFrame(int n);
	int getFrame();
protected:
	int m_i_frame;
	IRESurface* m_wave_surface;
};
typedef boost::shared_ptr<KWaveEffectDrawable> KWaveEffectDrawable_PTR;

#endif
