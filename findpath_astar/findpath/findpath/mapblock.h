/************************************************************************/
/*
@author:    junliang
@brief:     ����
@time:      20140309
*/
/************************************************************************/
#ifndef _MAP_H_
#define _MAP_H_

#include <vector>

#define NOWALK 0                //�������ߵ�
#define CANWALK 1				//������

struct MapBlock
{
	int x;				//������
	int y;				//������
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