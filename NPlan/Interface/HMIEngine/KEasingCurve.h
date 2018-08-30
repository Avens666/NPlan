//  File:        KEasingCurve.h
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
#ifndef KEasingCurve_DEFINED
#define KEasingCurve_DEFINED

#include "NE_type.h"

class KEasingCurveFunction;

class KEasingCurve
{
public:
	enum Type {
		Linear,
		InQuad, OutQuad, InOutQuad, OutInQuad,
		InCubic, OutCubic, InOutCubic, OutInCubic,
		InQuart, OutQuart, InOutQuart, OutInQuart,
		InQuint, OutQuint, InOutQuint, OutInQuint,
		InSine, OutSine, InOutSine, OutInSine,
		InExpo, OutExpo, InOutExpo, OutInExpo,
		InCirc, OutCirc, InOutCirc, OutInCirc,
		InElastic, OutElastic, InOutElastic, OutInElastic,
		InBack, OutBack, InOutBack, OutInBack,
		InBounce, OutBounce, InOutBounce, OutInBounce,
		InCurve, OutCurve, SineCurve, CosineCurve,
		Custom, NCurveTypes
	};

	KEasingCurve(Type type = Linear);
	KEasingCurve(const KEasingCurve &other);
	~KEasingCurve();

	KEasingCurve &operator=(const KEasingCurve &other);
	bool operator==(const KEasingCurve &other) const;
	inline bool operator!=(const KEasingCurve &other) const
	{ 
		return !(this->operator==(other)); 
	}

	kn_double Amplitude() const;
	void SetAmplitude(kn_double amplitude);

	kn_double Period() const;
	void SetPeriod(kn_double period);

	kn_double Overshoot() const;
	void SetOvershoot(kn_double overshoot);

	Type GetType() const;
	void SetType(Type type);

	kn_double valueForProgress(kn_double progress) const;

	typedef kn_double (*EasingFunction)(kn_double progress);
	void SetCustomType(EasingFunction func);
	EasingFunction CustomType() const;
	kn_double GetInterValue(kn_double iStart, kn_double iEnd, kn_double index, kn_double iSize);
	kn_double GetInterValue(kn_double iStart, kn_double iEnd, double v);
private:
	void SetTypeHelper(Type type);

	Type m_type;
	KEasingCurveFunction* m_pConfig;
	EasingFunction m_func;
};

#endif

