/************************************************************************/
/*
@author:    junliang
@brief:     网格
@time:      20140309
*/
/************************************************************************/
#ifndef _MAP_H_
#define _MAP_H_

#include <vector>

#define NOWALK 0                //不可行走的
#define CANWALK 1				//可行走

struct MapBlock
{
	int x;				//所在列
	int y;				//所在行
};

struct MapBlockData
{
	int row;
	int column;
	int* m_data;

	//void loadFile(const char* file);
	//void saveToFile(const char* file);
};

class SearchNode
{
public:
	static SearchNode* getInstance()
	{
		static SearchNode s_sn;
		return &s_sn;
	}

	std::vector<MapBlock> nodes;
};
#endif