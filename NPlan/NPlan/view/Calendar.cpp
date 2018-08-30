
#include "Calendar.h"

kn_char LunarMonthName[][7]={_T("  "), 
	_T("正月"),
	_T("二月"),
	_T("三月"),
	_T("四月"),
	_T("五月"),
	_T("六月"),
	_T("七月"),
	_T("八月"),
	_T("九月"), 
	_T("十月"),
	_T("十一月"),
	_T("十二月")}; 

kn_char LunarDayName[][5]={_T("  "), 
	_T("初一"),_T("初二"),_T("初三"),_T("初四"),_T("初五"),_T("初六"),_T("初七"),_T("初八"),_T("初九"),_T("初十"), 
	_T("十一"),_T("十二"),_T("十三"),_T("十四"),_T("十五"),_T("十六"),_T("十七"),_T("十八"),_T("十九"),_T("二十"), 
	_T("廿一"),_T("廿二"),_T("廿三"),_T("廿四"),_T("廿五"),_T("廿六"),_T("廿七"),_T("廿八"),_T("廿九"),_T("三十")}; 

/*1900-2050年的农历数据 数据格式说明： 5位十六进制数字  例:04bd8
  1    位: 1表示闰月30天，0表示29天
2、3、4位: 转换二进制为:0100 1011 1101(1为30天，0为29天)
           04bd8表示为(13个月):29,30,29,29,30,29,30,30,30(闰8月),30,30,29,30;
  5    位: 如果有闰月，则为月份，没有则为0
 */
unsigned long int LunarInfo[]=
{
	0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,//1900---1909
	0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,//1910---1919
	0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,//1920
	0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,//1930
	0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,//1940
	0x06ca0,0x0b550,0x15355,0x04da0,0x0a5b0,0x14573,0x052b0,0x0a9a8,0x0e950,0x06aa0,//1950
	0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,//1960
	0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b6a0,0x195a6,//1970
	0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,//1980
	0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,//1990
	0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,//2004 07552
	0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,//2010
	0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530, //2028
	0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,//2030
	0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,//2040--2049
	/*0x14b63*///2050
};

Calendar::Calendar()
{
	m_uilunaryear=0;
	m_uilunarmonth=0;
	m_uilunarday=0;
}

Calendar::~Calendar()
{

}

UCHAR Calendar::GetLeapMonth(UINT LunarYear)
{
	return LunarInfo[LunarYear-1900]& 0xf;
}

UCHAR Calendar::GetLeapMonthDays(UINT LunarYear)
{
	if(GetLeapMonth(LunarYear))
		return(  ( (LunarInfo[LunarYear-1900]) & 0x10000 ) ? 30:29 );
	else
		return (0);
}

UCHAR Calendar::GetLunarCurentMonthDays(UINT LunarYear, UCHAR  LunarMonth)
{
	return( (LunarInfo[LunarYear-1900] & (0x10000>>LunarMonth) ) ? 30:29 );
}

UINT Calendar::GetLunarYearTotalDays(UINT LunarYear) // 农历当年总天数,354?355 384 383
{ 
	/*sum=12*29 */   //12个月 29天一月
	UINT sum=348;
	UINT i;
	for(i=0x8000;i>0x8; i>>=1) 
		sum+=(LunarInfo[LunarYear-1900]&i)?1:0;  //把大月的1加进去
	return (sum + GetLeapMonthDays(LunarYear));    //判断这年是否有闰月
}

UINT Calendar::Leap(UINT year)  //判断是否为闰年
{ 
	if( (year % 4 ==0 && year % 100 != 0) || year % 400 == 0)
		return 366; 
	else
		return 365; 
}

UCHAR Calendar::SloarMonthDays(UINT year ,UCHAR month) //判断当年当月天数
{ 
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
		return 31;
	if(month==4||month==6||month==9||month==11)
		return 30; 
	if(month==2 && Leap(year)==366)
		return 29;
	else
		return 28; 
}

//计算1900.1.1 到  输入年月的天数
UINT Calendar::GetSolarTotalDays(UINT SolarYear, UCHAR SolarMonth)
{ 
	UINT total;
	UINT temp_num;
	total=0;
	for(temp_num=1900;temp_num<SolarYear;temp_num++)
		total+=Leap(temp_num);
	for(temp_num=1;temp_num<SolarMonth;temp_num++)
		total+=SloarMonthDays(SolarYear,temp_num);
	return total;
}

kn_string Calendar::SolarToLunar(UINT year,UINT month,UINT day)/* 输入阳历时期 年  月  日 */
{
	/*
	阳历1900.1.1
	在阳历为1900.1.31  时阴历为1900.1.1 即1900年正月初一	*/
	UINT totalday=0;/*记录农历1900.1.1日到今天相隔的天数 */
	UCHAR   runyueflag=0;//标记是否有闰月
	UCHAR   LeapMonth=0;
	UCHAR   yearflag=0;

	if(year<1900 || year>2049 || month>12 || month==0 || (year==1900 && month==1) )
		return _T("");

	totalday=GetSolarTotalDays(year, month)+day-30; /* 阳历从1900.1.31(农历1900.1.1)到今天的总天数(减30 实际少了一天)。 */
	m_uilunaryear=1900;
	while(totalday>385) //385大于一年  留出一年多的时间用于条件计算
	{
		totalday-=GetLunarYearTotalDays(m_uilunaryear); //
		m_uilunaryear++;
	}
	if(totalday>GetLunarYearTotalDays(m_uilunaryear))  //排除m_uilunaryear有闰月的情况
	{
		totalday-=GetLunarYearTotalDays(m_uilunaryear);
		m_uilunaryear++;

	}
	LeapMonth=GetLeapMonth(m_uilunaryear);  //当前闰哪个月
	if(LeapMonth)
		runyueflag=1; //有闰月则一年为13个月
	else
		runyueflag=0;  //没闰月则一年为12个月

	if(totalday==0)   //刚好一年
	{
		m_uilunarday=GetLunarCurentMonthDays(m_uilunaryear,12);
		m_uilunarmonth=12;
	}
	else
	{
		m_uilunarmonth=1;
		while(m_uilunarmonth<=12)
		{
			if( runyueflag==1 && m_uilunarmonth==(LeapMonth+1) )  //闰月处理
			{
				if(totalday>GetLeapMonthDays(m_uilunaryear))
				{
					totalday-=GetLeapMonthDays(m_uilunaryear);  //该年闰月天数
				}
				//lunar_month--;
				runyueflag=0;
				continue;
			}
			if( totalday > GetLunarCurentMonthDays(m_uilunaryear,m_uilunarmonth ) )
			{
				totalday=totalday-GetLunarCurentMonthDays(m_uilunaryear,m_uilunarmonth);  //该年该月天数
				m_uilunarmonth++;
			}
			else
			{
				m_uilunarday=totalday;
				break;
			}
		}
	}
	kn_string str = LunarMonthName[m_uilunarmonth];
	if (m_uilunarday > 1)
	{
		str = LunarDayName[m_uilunarday];
	}
	return str;
}
