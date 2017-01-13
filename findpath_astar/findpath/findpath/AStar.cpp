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

	if(parent) //������ڵ㲻ΪNULL
	{
		node->g = g; //Gֵ
		node->h = 10*(abs( sx - ex ) + abs( sy - ey ));//Hֵ
		node->f = node->g + node->h; //Fֵ
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
		// �ڵ�����ڵ�ͼ��Χ�� 
		if( x >= 0 &&x < mapw &&
			y >= 0 &&y < maph )
		{
			// �ڵ�������ͨ��
			if(canWalk(x,y))
			{
				//�����close list�У�������
				if(NodeInCloseList(x, y))
					continue;

				//������ӽڵ��gֵ   
				int newGVal;  
				if ((adj[i].x-x)!=0 && (adj[i].y-y)!=0)//λ�ڶԽ�����   
				{  
					newGVal = parent->g + 14;
				}  
				else  
					newGVal = parent->g + 10;

				// �ڵ㲻���� open list ����
				Node* node=NodeInOpenList(x, y );
				if(NULL==node)
				{
					//��ӵ� open list ���� 
					AddNodeToOpenList( parent, newGVal, x, y,ex,ey );
				}
				else
				{
					//���open�����Ѵ��ڣ���Ƚ��ĸ��ڵ��G����
					int exist_node_g=node->g;
					if(exist_node_g>newGVal)
					{
						//h����
						node->g = newGVal; //Gֵ
						node->f = node->g + node->h; //Fֵ
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
		//��С�ڵ��ڣ��������ͬ�Ļ�����ѡ�����һ���㣬��Ӧ �����ӵ�open��Ľڵ�
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

	// ��� open list �� close list 
	m_open_list.clear( );
	m_close_list.clear( );

	// �����ʼ�ڵ㵽 open list 
	AddNodeToOpenList( NULL, 0, sx, sy,ex,ey);

	// ��ʼѰ· 
	while( 0 != m_open_list.size( ) )
	{
		// �ҵ�ӵ������ F ���۵Ľڵ� 
		best_f_node = FindBestFNode( );

		// �����Ŀ��ڵ�, �����Ѱ·
		if( best_f_node->x == ex && best_f_node->y == ey )
			return best_f_node; 

		// ������ F ���۵Ľڵ���뵽 close list ����
		// ���ҽ���� open list �����Ƴ� 
		m_close_list.push_back( best_f_node);
		m_open_list.remove( best_f_node );

		// �� best_f_node �ڵ���ڽ��ڵ���ӵ� open list ���� 
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

	//����open����ڴ�
	std::list<Node*>::iterator itor=m_open_list.begin();
	for(; itor!=m_open_list.end(); itor++)
	{
		Node* tmp_node=*itor;
		delete tmp_node;
	}
	m_open_list.clear();

	//����close����ڴ�
	itor=m_close_list.begin();
	for(; itor!=m_close_list.end(); itor++)
	{
		Node* tmp_node=*itor;
		delete tmp_node;
	}
	m_close_list.clear();

	return true;
}