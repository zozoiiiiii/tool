/************************************************************************/
/*
@author:    junliang
@brief:     黑白图， 判断哪些地方可行走，哪些地方不可行走
@time:      20140222
*/
/************************************************************************/
#ifndef _SCENE_MAP_H_
#define _SCENE_MAP_H_

#include <Windows.h>
#include <vector>
#include <map>


#define NOWALK 0                //不可行走的
#define CANWALK 1				//可行走
//跳跃之类的由以后具体需求来实现


//坐标
struct Point
{
	int x;
	int y;

	Point()
	{
		x=-1;
		y=-1;
	}
	Point(int nX, int nY)
	{
		x=nX;
		y=nY;
	}

	void operator= (const Point & b)
	{
		this->x=b.x;
		this->y=b.y;
	}

};

inline bool operator == (const Point & a, const Point & b)
{
	return    (( a.x == b.x )&&(a.y==b.y) );
}


class Map
{
public:
	//导入黑白图
	void loadMap(const char* path);

	//指定坐标是否可通过
	bool canWalk(Point p);

	//两点间是否可直达
	bool canLineTo(Point s, Point e);
	int getWidth();
	int getHeight();
private:
	//获取一个字节内的指定位的值
	int checkBit(int val, int x);

	//判断点是否在两点之间的直线上
	bool checkPointOnLine(Point p, Point s, Point e);
private:
	BYTE* m_pData;
	int m_width;
	int m_height;
};
#endif