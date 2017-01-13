/************************************************************************/
/*
@author:    junliang
@brief:     �ڰ�ͼ�� �ж���Щ�ط������ߣ���Щ�ط���������
@time:      20140222
*/
/************************************************************************/
#ifndef _SCENE_MAP_H_
#define _SCENE_MAP_H_

#include <Windows.h>
#include <vector>
#include <map>


#define NOWALK 0                //�������ߵ�
#define CANWALK 1				//������
//��Ծ֮������Ժ����������ʵ��


//����
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
	//����ڰ�ͼ
	void loadMap(const char* path);

	//ָ�������Ƿ��ͨ��
	bool canWalk(Point p);

	//������Ƿ��ֱ��
	bool canLineTo(Point s, Point e);
	int getWidth();
	int getHeight();
private:
	//��ȡһ���ֽ��ڵ�ָ��λ��ֵ
	int checkBit(int val, int x);

	//�жϵ��Ƿ�������֮���ֱ����
	bool checkPointOnLine(Point p, Point s, Point e);
private:
	BYTE* m_pData;
	int m_width;
	int m_height;
};
#endif