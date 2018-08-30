#include "NE_pubhead.h"
#include "RERect.h"


bool RERectHelper::CheckAndJoinIntersetRect( LSTIRECT& lsRects )
{
	if(lsRects.size() < 2)
	{
		return true;
	}

	kn_bool ret = true;

	LSTIRECT::iterator itEnd = lsRects.end();
	itEnd--;

	for(LSTIRECT::iterator itBegin = lsRects.begin(); itBegin != itEnd; itBegin++)
	{
		LSTIRECT::iterator it = itBegin;
		it++;

		for(;it != lsRects.end(); it++)
		{
			//g_iLoop++;
			if(REIRect::Intersects(*itBegin, *it))
			{
				REIRect rectJoin = *itBegin;
				rectJoin.join(*it);

				//	用合并后的的矩形替换原先的两个矩形
				lsRects.insert(itBegin, rectJoin);

				// list插入后，迭代器指针不变,仍指向原有的元素
				lsRects.erase(itBegin);
				lsRects.erase(it);
				ret = false;

				// 跳出循环
				goto EndLoop;
			}
		}
	}
EndLoop:
	return ret;
}

void RERectHelper::Merge( LSTIRECT& lsRects )
{
	kn_bool bOK = false;

	//g_iLoop = 0;
	int iLoop = 0;
	// 直到全部合并完成
	while(!bOK)
	{
		bOK = CheckAndJoinIntersetRect(lsRects);
		iLoop++;
	}
	//printf("iLoop = %d", iLoop);
}

bool RERectHelper::CheckAndJoinIntersetRect( LSTRECT& lsRects )
{
	if(lsRects.size() < 2)
	{
		return true;
	}

	kn_bool ret = true;

	LSTRECT::iterator itEnd = lsRects.end();
	itEnd--;

	for(LSTRECT::iterator itBegin = lsRects.begin(); itBegin != itEnd; itBegin++)
	{
		LSTRECT::iterator it = itBegin;
		it++;

		for(;it != lsRects.end(); it++)
		{
			//g_iLoop++;
			if(RERect::Intersects(*itBegin, *it))
			{
				RERect rectJoin = *itBegin;
				rectJoin.join(*it);

				//	用合并后的的矩形替换原先的两个矩形
				lsRects.insert(itBegin, rectJoin);

				// list插入后，迭代器指针不变,仍指向原有的元素
				lsRects.erase(itBegin);
				lsRects.erase(it);
				ret = false;

				// 跳出循环
				goto EndLoop;
			}
		}
	}
EndLoop:
	return ret;
}

void RERectHelper::Merge( LSTRECT& lsRects )
{
	kn_bool bOK = false;

	//g_iLoop = 0;
	int iLoop = 0;
	// 直到全部合并完成
	while(!bOK)
	{
		bOK = CheckAndJoinIntersetRect(lsRects);
		iLoop++;
	}
	//printf("iLoop = %d", iLoop);
}