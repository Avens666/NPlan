#ifndef K_CCalenda_VIEW_H__
#define K_CCalenda_VIEW_H__
#include "KViewGroup.h"
using namespace std;

typedef unsigned char       UCHAR;
typedef unsigned int        UINT;




class Calendar
{
public:
	Calendar();
	virtual ~Calendar();
	kn_string   SolarToLunar(UINT year,UINT month,UINT day);/* 输入阳历时期 年  月  日 */
	UINT    GetSolarTotalDays(UINT SolarYear, UCHAR   SolarMonth);
private:
	UCHAR   GetLeapMonth(UINT LunarYear); //确定是否存在农历的闰月 并返回闰月
	UCHAR   GetLeapMonthDays(UINT LunarYear);  //若存在闰月,返回闰月的天数,30?29
	UCHAR   GetLunarCurentMonthDays(UINT LunarYear, UCHAR   LunarMonth); //确定农历当月天数,30?29
	UINT    GetLunarYearTotalDays(UINT LunarYear); // 农历当年总天数,354?355 384 383
    UINT    Leap(UINT year);  //判断是否为闰年
	UCHAR   SloarMonthDays(UINT year ,UCHAR   month);//判断阳历当年当月天数
	
private:
	UINT m_uilunaryear;
	UINT m_uilunarmonth;
	UINT m_uilunarday;	
};

#endif