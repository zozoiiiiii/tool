/************************************************************************/
/*
@auhtor:    junliang
@brief:     A��Ѱ·

��С��Ԫ��
1������
2�����ص�
�˴�ѡ�����ص�


g(n)��ʾ����㵽���ⶥ��n��ʵ�ʾ���
h(n)��ʾ���ⶥ��n��Ŀ�궥��Ĺ������
f(n)=g(n)+h(n)��

�����ʽ��ѭ�������ԣ�
���h(n)Ϊ0��ֻ�����g(n)���������㵽���ⶥ��n�����·������ת��Ϊ��Դ���·�����⣬��Dijkstra�㷨
���h(n)<=��n��Ŀ���ʵ�ʾ��롱����һ������������Ž⡣����h(n)ԽС����Ҫ����Ľڵ�Խ�࣬�㷨Ч��Խ�͡�


���´���Ϊ����������һ���汾��
@time:      20140218
*/
/************************************************************************/
#ifndef _ASTAR_H_
#define _ASTAR_H_
#include "mapblock.h"
#include <list>
#include <vector>
using namespace std;


struct Node
{
	Node( ){ x = y = f = g = h = 0; parent = NULL; }
	Node( int x, int y ){ this->x = x; this->y = y; parent = NULL; }

	//��ǰ����
	int x, y;
	int f, g, h;

	Node * parent;
};

class AStar
{
public:
	AStar(MapBlockData* mbd);
	bool findpath(MapBlock s, MapBlock e, std::vector<MapBlock>& v);
private:
	bool canWalk(int x, int y);
	Node* FindPath(int bx, int by, int sx, int sy);

	//����½ڵ㵽 open list
	void AddNodeToOpenList( Node * parent, int g,int sx, int sy,int ex,int ey);
	
	//�жϽڵ��Ƿ��Ѿ����� open list ����
	Node* NodeInOpenList( int x, int y );
	bool NodeInCloseList( int x, int y );

	// ����ض��ڵ���ڽ��ڵ㵽 open list
	void AddAdjNodeToOpenList( Node * parent,int mapw,int maph,int ex,int ey);
	
	//�� open list ��Ѱ����С F ���۵Ľڵ�
	Node * FindBestFNode();
private:
	MapBlockData* m_mapblock_data;

	list<Node*> m_open_list;
	list<Node*> m_close_list;
};

#endif