#include "AStar.h"
#include <Windows.h>


AStar::AStar(MapBlockData* mbd)
{
	m_mapblock_data=mbd;
}

void AStar::AddNodeToOpenList( Node * parent, int g, int sx, int sy,int ex,int ey)
{
	MapBlock mb;
	mb.x=sx;
	mb.y=sy;
	SearchNode::getInstance()->nodes.push_back(mb);
	Sleep(100);

	Node * node = new Node( sx, sy );

	node->parent = parent;

	if(parent) //如果父节点不为NULL
	{
		node->g = g; //G值
		node->h = 10*(abs( sx - ex ) + abs( sy - ey ));//H值
		node->f = node->g + node->h; //F值
	}
	else
	{
		node->g = 0;
		node->h = 0;
		node->f = 0;
	} 
	m_open_list.push_back( node );
}

Node* AStar::NodeInOpenList( int x, int y )
{
	list< Node* >::iterator iter = m_open_list.begin( );
	while( iter != m_open_list.end( ) )
	{
		if( x == ( * iter )->x && y == ( * iter )->y )
			return *iter;
		iter ++;
	}

	return NULL;
}
bool AStar::NodeInCloseList( int x, int y )
{
	list< Node* >::iterator iter = m_close_list.begin( );
	while( iter != m_close_list.end( ) )
	{
		if( x == ( * iter )->x && y == ( * iter )->y )
			return true;
		iter ++;
	} 
	return false;
}
void AStar::AddAdjNodeToOpenList( Node * parent,int mapw,int maph,int ex,int ey)
{
	int x = parent->x;
	int y = parent->y;
	MapBlock adj[8]=
	{
		{x-1,y-1},
		{x+0,y-1},
		{x+1,y-1},

		{x-1,y+0},
		{x+1,y+0},

		{x-1,y+1},
		{x+0,y+1},
		{x+1,y+1},
	};

	for( int i = 0; i < 8; i ++ )
	{
		int x=adj[i].x;
		int y=adj[i].y;
		// 节点必须在地图范围内 
		if( x >= 0 &&x < mapw &&
			y >= 0 &&y < maph )
		{
			// 节点必须可以通行
			if(canWalk(x,y))
			{
				//如果在close list中，则跳过
				if(NodeInCloseList(x, y))
					continue;

				//计算此子节点的g值   
				int newGVal;  
				if ((adj[i].x-x)!=0 && (adj[i].y-y)!=0)//位于对角线上   
				{  
					newGVal = parent->g + 14;
				}  
				else  
					newGVal = parent->g + 10;

				// 节点不存在 open list 里面
				Node* node=NodeInOpenList(x, y );
				if(NULL==node)
				{
					//添加到 open list 里面 
					AddNodeToOpenList( parent, newGVal, x, y,ex,ey );
				}
				else
				{
					//如果open表中已存在，则比较哪个节点的G更好
					int exist_node_g=node->g;
					if(exist_node_g>newGVal)
					{
						//h不变
						node->g = newGVal; //G值
						node->f = node->g + node->h; //F值
						node->parent=parent;
					}
				}
			}
		}
	}//end for
}

bool AStar::canWalk(int x, int y)
{
	int index=y * m_mapblock_data->column + x;
	if( CANWALK == m_mapblock_data->m_data[index])
		return true;

	return false;
}

Node * AStar::FindBestFNode()
{
	if(m_open_list.size()<=0)
		return NULL;

	list< Node* >::iterator iter = m_open_list.begin( );

	Node * min_node = *iter;
	int min_f = min_node->f;

	iter++;
	while( iter != m_open_list.end( ) )
	{
		//用小于等于，如果有相同的话，会选择最后一个点，对应 最近添加到open表的节点
		if( ( * iter )->f <= min_f )
		{
			min_f = ( * iter )->f;
			min_node = * iter;
		} 
		iter ++;
	} 
	return min_node;
}

Node * AStar::FindPath( int sx, int sy, int ex, int ey )
{
	Node * start_node = NULL;
	Node * best_f_node = NULL;
	Node * best_g_node = NULL;

	// 清空 open list 和 close list 
	m_open_list.clear( );
	m_close_list.clear( );

	// 添加起始节点到 open list 
	AddNodeToOpenList( NULL, 0, sx, sy,ex,ey);

	// 开始寻路 
	while( 0 != m_open_list.size( ) )
	{
		// 找到拥有最优 F 评价的节点 
		best_f_node = FindBestFNode( );

		// 如果是目标节点, 则完成寻路
		if( best_f_node->x == ex && best_f_node->y == ey )
			return best_f_node; 

		// 将最优 F 评价的节点放入到 close list 里面
		// 并且将其从 open list 里面移除 
		m_close_list.push_back( best_f_node);
		m_open_list.remove( best_f_node );

		// 将 best_f_node 节点的邻近节点添加到 open list 里面 
		AddAdjNodeToOpenList( best_f_node,m_mapblock_data->column, m_mapblock_data->row, ex,ey);
	}

	return NULL;
}

bool AStar::findpath(MapBlock s, MapBlock e, std::vector<MapBlock>& v)
{
	if(!canWalk(s.x, s.y))
		return false;

	if(!canWalk(e.x, e.y))
		return false;

	Node* node=FindPath(s.x, s.y, e.x, e.y);
	if(NULL==node)
		return false;

	while(node)
	{
		MapBlock p;
		p.x=node->x;
		p.y=node->y;

		v.push_back(p);
		node=node->parent;
	}

	//清理open表的内存
	std::list<Node*>::iterator itor=m_open_list.begin();
	for(; itor!=m_open_list.end(); itor++)
	{
		Node* tmp_node=*itor;
		delete tmp_node;
	}
	m_open_list.clear();

	//清理close表的内存
	itor=m_close_list.begin();
	for(; itor!=m_close_list.end(); itor++)
	{
		Node* tmp_node=*itor;
		delete tmp_node;
	}
	m_close_list.clear();

	return true;
}