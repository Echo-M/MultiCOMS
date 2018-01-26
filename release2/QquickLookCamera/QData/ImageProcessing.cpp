#include "ImageProcessing.h"
#include "math.h"
#include <memory>

ImageProcessing::ImageProcessing(int height, int width)
{
	m_height = height;
	m_width = width;
}


ImageProcessing::~ImageProcessing()
{
}

void ImageProcessing::Transposition(unsigned char* dst, const unsigned char* src, int& width, int& height)
{
	width = m_height;
	height = m_width;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			dst[(y*height + x) * 3 + 0] = src[(x*width + y) * 3 + 0];//B
			dst[(y*height + x) * 3 + 1] = src[(x*width + y) * 3 + 1];//G
			dst[(y*height + x) * 3 + 2] = src[(x*width + y) * 3 + 2];//R
		}
	}
}

//24λ���ɫͼ��תangle�㣬ͼ��ߡ���ı䣬�հײ����ɰ�ɫ���
void ImageProcessing::Rotate(unsigned char* dst, const unsigned char* src, int& width, int& height, int angle)
{
	double angleRad = (double)angle / 180 * 3.1415926;
	double fcos = cos(angleRad);
	double fsin = sin(angleRad);
	width = m_width*abs(fcos) + m_height*abs(fsin);
	if (width % 4 != 0) width += 4 - width % 4;
	height = m_width*abs(fsin) + m_height*abs(fcos);
	if (height % 4 != 0) height += 4 - height % 4;
	memset(dst, 255, height * width * 3);
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y = 0; y < height; ++y)
	{
		for (x = 0; x < width; ++x)
		{
			/*x = x0*fcos - y0*fsin - 0.5*m_width*fcos + 0.5*m_height*fsin + 0.5*width;
			y = x0*fsin + y0*fcos - 0.5*m_width*fsin - 0.5*m_height*fcos + 0.5*height;*/
			x0 = x*fcos + y*fsin - 0.5*width*fcos - 0.5*height*fsin + 0.5*m_width;
			y0 = -x*fsin + y*fcos + 0.5*width*fsin - 0.5*height*fcos + 0.5*m_height;
			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
			{
				dst[(y*width + x) * 3 + 0] = src[(y0*m_width + x0) * 3 + 0];
				dst[(y*width + x) * 3 + 1] = src[(y0*m_width + x0) * 3 + 1];
				dst[(y*width + x) * 3 + 2] = src[(y0*m_width + x0) * 3 + 2];
			}
		}
	}
}

//24λ���ɫͼ���ţ�ratioΪ�������ӣ�ratio=2��Ϊ�Ŵ�2����
void ImageProcessing::Zoom(unsigned char* dst, const unsigned char* src, int& width, int& height, double ratio)
{
	width = m_width*ratio;
	if (width % 4 != 0) width += 4 - width % 4;
	height = m_height*ratio;
	if (height % 4 != 0) height += 4 - height % 4;
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y = 0; y < height; ++y)
	{
		for (x = 0; x < width; ++x)
		{
			x0 = x / ratio;
			y0 = y / ratio;
			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
			{
				dst[(y*width + x) * 3 + 0] = src[(y0*m_width + x0) * 3 + 0];
				dst[(y*width + x) * 3 + 1] = src[(y0*m_width + x0) * 3 + 1];
				dst[(y*width + x) * 3 + 2] = src[(y0*m_width + x0) * 3 + 2];
			}
		}
	}
}

bool ImageProcessing::Shift(unsigned char* dst, const unsigned char* src, int xx, int yy)
{
	if (xx >= m_width || yy >= m_height)
		return false;
	memset(dst, 255, m_width * m_height * 3);
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y = 0; y < m_height; ++y)
	{
		for (x = 0; x < m_width; ++x)
		{
			x0 = x - xx;
			y0 = y - yy;
			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
			{
				dst[(y*m_width + x) * 3 + 0] = src[(y0*m_width + x0) * 3 + 0];
				dst[(y*m_width + x) * 3 + 1] = src[(y0*m_width + x0) * 3 + 1];
				dst[(y*m_width + x) * 3 + 2] = src[(y0*m_width + x0) * 3 + 2];
			}
		}
	}
	return true;
}

#define TEMPLATE_SMOOTH_BOX 1 //Boxƽ��ģ��
#define TEMPLATE_SMOOTH_GAUSS  2 //��˹ƽ��ģ��
#define TEMPLATE_SHARPEN_LAPLACIAN 3 //������˹��ģ��
bool ImageProcessing::TemplateOperation(unsigned char* dst, const unsigned char* src, int templateType)
{
	int templateSmoothBox[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 }; //Boxƽ��ģ��
	int templateSmoothGauss[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 }; //��˹ƽ��ģ��
	int templateSharpenLaplacian[9] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 }; //������˹��ģ��
	float coef; //ģ��ǰ�����˵�ϵ��
	int templateArray[9]; //ģ������
	float tempNum; //������

	switch (templateType)//�ж�ģ������
	{
	case TEMPLATE_SMOOTH_BOX: //Boxƽ��ģ��
		coef = (float)(1.0 / 9.0);
		memcpy(templateArray, templateSmoothBox, 9 * sizeof(int));
		break;
	case TEMPLATE_SMOOTH_GAUSS: //��˹ƽ��ģ��
		coef = (float)(1.0 / 16.0);
		memcpy(templateArray, templateSmoothGauss, 9 * sizeof(int));
        break;
	case TEMPLATE_SHARPEN_LAPLACIAN:  //������˹��ģ��
		coef = (float)1.0;
		memcpy(templateArray, templateSharpenLaplacian, 9 * sizeof(int));
		break;
	}

	//�Ƚ�ԭͼֱ�ӿ�����������ʵ��Ҫ�ǿ�����ΧһȦ������
	memcpy(dst, src, m_height*m_width * 3);
	for (int y = 1; y < m_height - 1; y++) //ע��y�ķ�Χ�Ǵ�1��Height-2
	{
		for (int x = 1; x < m_width - 1; x++) //ע��x�ķ�Χ�Ǵ�1��Width-2
		{
			for (int i = 0; i < 3; ++i)
			{
				tempNum = (float)(src[(y*m_width + x) * 3 + i])*templateArray[0];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[1];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[2];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[3];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[4];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[5];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[6];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[7];
				tempNum += (float)(src[(y*m_width + x) * 3 + i])*templateArray[8];
				//������ϵ��
				tempNum *= coef;
				//ע��������Ĵ���
				if (tempNum > 255.0) dst[(y*m_width + x) * 3 + i] = 255;
				else if (tempNum < 0.0) dst[(y*m_width + x) * 3 + i] = (unsigned char)fabs(tempNum);
				else dst[(y*m_width + x) * 3 + i] = (unsigned char)tempNum;
			}
		}
	}
	return true;
}

bool ImageProcessing::LimbPatternM3(unsigned char* dst, const unsigned char* src)
{
	unsigned char bayerPattern[8][8] = 
	  {  0, 32,  8, 40,  2, 34, 10, 42,
		48, 16, 56, 24, 50, 18, 58, 26,
		12, 44,  4, 36, 14, 46,  6, 38,
		60, 28, 52, 20, 62, 30, 54, 22,
		 3, 35, 11, 43,  1, 33,  9, 41,
		51, 19, 59, 27, 49, 17, 57, 25,
		15, 47,  7, 39, 13, 45,  5, 37,
		63, 31, 55, 23, 61, 29, 53, 21 };
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			if ((src[y*m_width + x] >> 2) > bayerPattern[y&7][x&7]) //�����64���Ҷ�ֵ�ٱȽ�
			    dst[y*m_width + x] = 255; //��׵�
			else dst[y*m_width + x] = 0; //��ڵ�
		}
	}
	return true;
}