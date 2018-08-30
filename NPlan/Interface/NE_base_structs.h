//  File:        NE_base_structs.h
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
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：base_structs.h
* 摘    要：定义通用基本数据结构
*
* 版    本：1.0
* 创建日期：2010/03/09
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/
#ifndef _NE_BASE_STRUCTS_H__
#define _NE_BASE_STRUCTS_H__

#include "NE_type.h"

/***********************************************************************************************************************
KNGEOCOORD
------------------------------------------------------------------------------------------------------------------------
	The KNGEOCOORD structure contains the absolute coordination information of a map object used by the navigation 
	system. 

	struct KNGEOCOORD
	{
		kn_ulong ulLongitude;
		kn_ulong ulLatitude;
	}

Members
	ulLongitude
		A kn_ulong value that specifies the absolute longitude of a map object. The unit of this value is 1/1024 second.

	ulLatitude
		A kn_ulong value that specifies the absolute latitude of a map object. The unit of this value is 1/1024 second.

Methods
	operator == Function
	--------------------------------------------------------------------------------------------------------------------
	Description
		Tests if the input KNGEOCOORD object is equal to this object.

	Syntax	
		kn_bool operator == (const KNGEOCOORD& coord) const

	Parameters
		coord
		[in] A const reference to a KNGEOCOORD object. 

	Return Value
		If the test succeeds, the return value is TRUE.
		If the test fails, the return value is FALSE.

	operator != Function
	--------------------------------------------------------------------------------------------------------------------
	Description
		Tests if the input KNGEOCOORD object is not equal to this object.

	Syntax	
		kn_bool operator != (const KNGEOCOORD& coord) const

	Parameters
		coord
			[in] A const reference to a KNGEOCOORD object. 

	Return Value
		If the test succeeds, the return value is TRUE.
		If the test fails, the return value is FALSE.

	operator < function
	--------------------------------------------------------------------------------------------------------------------
	Description
		Tests if this KNGEOCOORD object is less than the input KNGEOCOORD object.

	Syntax	
		kn_bool operator < (const KNGEOCOORD& coord) const

	Parameters
		coord
			[in] A const reference to a KNGEOCOORD object. 

	Return Value
		If the test succeeds, the return value is TRUE.
		If the test fails, the return value is FALSE.

	operator = function
	--------------------------------------------------------------------------------------------------------------------
	Description
		Assigns this KNGEOCOORD object with the input KNGEOCOORD object.

	Syntax	
		KNGEOCOORD& operator = (const KNGEOCOORD& coord)

	Parameters
		coord
			[in] A const reference to a KNGEOCOORD object. 

	Return Value
		A reference to this KNGEOCOORD object

***********************************************************************************************************************/
struct KNGEOCOORD
{
	kn_ulong ulLongitude;
	kn_ulong ulLatitude;

	kn_bool operator == (const KNGEOCOORD& coord) const
	{
		if (coord.ulLongitude == this->ulLongitude  &&  coord.ulLatitude == this->ulLatitude)
			return true;
		else
			return false;
	}

	kn_bool operator != (const KNGEOCOORD& coord) const
	{
		return !(*this == coord);
	}

	kn_bool operator < (const KNGEOCOORD& coord) const
	{
		return (this->ulLongitude < coord.ulLongitude
			|| (this->ulLongitude == coord.ulLongitude
			&& this->ulLatitude < coord.ulLatitude));
	}

	KNGEOCOORD& operator = (const KNGEOCOORD& coord)
	{
		this->ulLongitude = coord.ulLongitude;
		this->ulLatitude  = coord.ulLatitude;

		return *this;
	}

	KNGEOCOORD(const kn_ulong ulLong = 0, const kn_ulong ulLati = 0)
	{
		ulLongitude = ulLong;
		ulLatitude  = ulLati;
	}
};

/***********************************************************************************************************************
Coor4096
------------------------------------------------------------------------------------------------------------------------
	The Coor4096 structure contains the relative coordination information of a map object used by the navigation system. 
	It is an offset value to the left-bottom point of the parcel in which the map object lays.

	struct KNGEOCOORD
	{
		kn_word Longitude;
		kn_word Latitude;
	}

Members
	Longitude
		A kn_word value that specifies the offset value in longitude of a map object. The unit of this value is 1/8 
		second.

	Latitude
		A kn_word value that specifies the offset value in latitude of a map object. The unit of this value is 1/8 
		second.
***********************************************************************************************************************/
struct Coor4096
{
	kn_word Longitude;
	kn_word Latitude;
	Coor4096(): Longitude(0), Latitude(0)
	{
	}
	Coor4096(kn_word Lon, kn_word Lat) : Longitude(Lon), Latitude(Lat)
	{
	}
};

#endif // _NE_BASE_STRUCTS_H__
