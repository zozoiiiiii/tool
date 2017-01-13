/************************************************************************/
/*
@auhtor:    junliang
@brief:     A星寻路

最小单元：
1，格子
2，像素点
此处选择像素点


g(n)表示从起点到任意顶点n的实际距离
h(n)表示任意顶点n到目标顶点的估算距离
f(n)=g(n)+h(n)。

这个公式遵循以下特性：
如果h(n)为0，只需求出g(n)，即求出起点到任意顶点n的最短路径，则转化为单源最短路径问题，即Dijkstra算法
如果h(n)<=“n到目标的实际距离”，则一定可以求出最优解。而且h(n)越小，需要计算的节点越多，算法效率越低。


以下代码为网上搜索的一个版本。
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

	//当前坐标
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

	//添加新节点到 open list
	void AddNodeToOpenList( Node * parent, int g,int sx, int sy,int ex,int ey);
	
	//判断节点是否已经存在 open list 里面
	Node* NodeInOpenList( int x, int y );
	bool NodeInCloseList( int x, int y );

	// 添加特定节点的邻近节点到 open list
	void AddAdjNodeToOpenList( Node * parent,int mapw,int maph,int ex,int ey);
	
	//从 open list 中寻找最小 F 评价的节点
	Node * FindBestFNode();
private:
	MapBlockData* m_mapblock_data;

	list<Node*> m_open_list;
	list<Node*> m_close_list;
};

#endif