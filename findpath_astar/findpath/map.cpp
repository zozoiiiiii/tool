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
	当biBitCount=1时，8个像素占1个字节;
	当biBitCount=4时，2个像素占1个字节;
	当biBitCount=8时，1个像素占1个字节;
	当biBitCount=24时,1个像素占3个字节;
	此处黑白图为1，即bit为单位
	*/
	BITMAPFILEHEADER bmpHeader;//包含文件类型、大小和布局的信息
	BITMAPINFOHEADER bmpInfHeader;//包含位图的尺寸和颜色格式
	fread(&bmpHeader,sizeof(BITMAPFILEHEADER),1,fp);
	fread(&bmpInfHeader,sizeof(BITMAPINFOHEADER),1,fp);

	//定位到图像数据处
	fseek(fp,bmpHeader.bfOffBits,SEEK_SET);

	m_width=bmpInfHeader.biWidth;
	m_height=bmpInfHeader.biHeight;

	//一行像素需要的字节数, 需要能被8整除(一定要用微软这个宏，自己去整除的话，有问题)
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

	//一行像素需要的字节数
	int lineByte = WIDTHBYTES(m_width);
	int index=p.y*lineByte + p.x/8;
	BYTE val=m_pData[index];

	//该字节的第几位
	int bit_x=p.x%8;

	//判断该字节的指定位是0还是1， bmp格式是反的，所以用7减
	int bit_val=checkBit(val, 7-bit_x);
	if( 0 == bit_val)
	{
		return false;
	}

	return true;
}

bool Map::canLineTo(Point s, Point e)
{
	//检查起点
	if(!canWalk(s))
		return false;

	//检查终点
	if(!canWalk(e))
		return false;

	//判断所有中间会经过的点
	int smaller_x=min(s.x, e.x);
	int smaller_y=min(s.y, e.y);
	int bigger_x=max(s.x, e.x);
	int bigger_y=max(s.y, e.y);

	for(int i=smaller_x; i<=bigger_x; i++)
	for(int j=smaller_y; j<=bigger_y; j++)
	{
		//中间会经过的点
		Point p(i, j);

		//判断点P是否在直达线上
		if(checkPointOnLine(p, s, e))
		{
			//判断在直线上的点是否能通行
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
	//先判断是否在同一点
	if(p==s || p==e)
		return true;

	//在水平线上
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

	//肯定不是水平线
	if(p.x==s.x || p.y==s.y)
		return false;

	//直线斜率
	float k=(float(e.y-s.y))/(float(e.x-s.x));
	float k1=(float(p.y-s.y))/(float(p.x-s.x));

	//方向相同
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