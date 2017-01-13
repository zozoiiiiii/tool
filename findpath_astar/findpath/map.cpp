#include "map.h"
#include <io.h>
//#include <amvideo.h>


#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4);

int Map::checkBit(int val, int x)
{

	switch(x)
	{
	case 0:
		return (val)&0x1;
	case 1:
		return (val)&0x2;
	case 2:
		return (val)&0x4;
	case 3:
		return (val)&0x8;
	case 4:
		return (val)&0x10;
	case 5:
		return (val)&0x20;
	case 6:
		return (val)&0x40;
	case 7:
		return (val)&0x80;
	default:
		break;
	}

	return 999;
}

void Map::loadMap(const char* path)
{
	FILE* fp=fopen(path, "rb");
	if(fp==NULL)
		return;

	/*
	��biBitCount=1ʱ��8������ռ1���ֽ�;
	��biBitCount=4ʱ��2������ռ1���ֽ�;
	��biBitCount=8ʱ��1������ռ1���ֽ�;
	��biBitCount=24ʱ,1������ռ3���ֽ�;
	�˴��ڰ�ͼΪ1����bitΪ��λ
	*/
	BITMAPFILEHEADER bmpHeader;//�����ļ����͡���С�Ͳ��ֵ���Ϣ
	BITMAPINFOHEADER bmpInfHeader;//����λͼ�ĳߴ����ɫ��ʽ
	fread(&bmpHeader,sizeof(BITMAPFILEHEADER),1,fp);
	fread(&bmpInfHeader,sizeof(BITMAPINFOHEADER),1,fp);

	//��λ��ͼ�����ݴ�
	fseek(fp,bmpHeader.bfOffBits,SEEK_SET);

	m_width=bmpInfHeader.biWidth;
	m_height=bmpInfHeader.biHeight;

	//һ��������Ҫ���ֽ���, ��Ҫ�ܱ�8����(һ��Ҫ��΢������꣬�Լ�ȥ�����Ļ���������)
	int lineByte = WIDTHBYTES(m_width);
	m_pData = new BYTE[m_height*lineByte];

	for(int i=m_height-1;i>=0;i--)
	{
		for(int j=0;j<lineByte;j++)
		{
			int index=i*lineByte+j;
			fread(&m_pData[index],sizeof(BYTE), 1, fp);
		}
	}

	fclose(fp);
}

bool Map::canWalk(Point p)
{
	if(p.x < 0 || p.y < 0 || p.x > m_width || p.y > m_height)
		return false;

	//һ��������Ҫ���ֽ���
	int lineByte = WIDTHBYTES(m_width);
	int index=p.y*lineByte + p.x/8;
	BYTE val=m_pData[index];

	//���ֽڵĵڼ�λ
	int bit_x=p.x%8;

	//�жϸ��ֽڵ�ָ��λ��0����1�� bmp��ʽ�Ƿ��ģ�������7��
	int bit_val=checkBit(val, 7-bit_x);
	if( 0 == bit_val)
	{
		return false;
	}

	return true;
}

bool Map::canLineTo(Point s, Point e)
{
	//������
	if(!canWalk(s))
		return false;

	//����յ�
	if(!canWalk(e))
		return false;

	//�ж������м�ᾭ���ĵ�
	int smaller_x=min(s.x, e.x);
	int smaller_y=min(s.y, e.y);
	int bigger_x=max(s.x, e.x);
	int bigger_y=max(s.y, e.y);

	for(int i=smaller_x; i<=bigger_x; i++)
	for(int j=smaller_y; j<=bigger_y; j++)
	{
		//�м�ᾭ���ĵ�
		Point p(i, j);

		//�жϵ�P�Ƿ���ֱ������
		if(checkPointOnLine(p, s, e))
		{
			//�ж���ֱ���ϵĵ��Ƿ���ͨ��
			if(!canWalk(p))
				return false;
		}
	}

	return true;
}

int Map::getWidth()
{
	return m_width;
}

int Map::getHeight()
{
	return m_height;
}

bool Map::checkPointOnLine(Point p, Point s, Point e)
{
	//���ж��Ƿ���ͬһ��
	if(p==s || p==e)
		return true;

	//��ˮƽ����
	if(e.x==s.x)
	{
		if(p.x==e.x)
			return true;
		else
			return false;
	}

	if(e.y==s.y)
	{
		if(p.y==e.y)
			return true;
		else
			return false;
	}

	//�϶�����ˮƽ��
	if(p.x==s.x || p.y==s.y)
		return false;

	//ֱ��б��
	float k=(float(e.y-s.y))/(float(e.x-s.x));
	float k1=(float(p.y-s.y))/(float(p.x-s.x));

	//������ͬ
	if((k>0&&k1>0)
		||(k<0&&k1<0))
	{
		if((k-k1)<=0.1&&(k-k1)>=-0.1)
			return true;
		else
			return false;
	}

		return false;
}