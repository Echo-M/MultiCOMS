#include "SaveToBmp24.h"

SaveToBmp24::SaveToBmp24()
{
}

SaveToBmp24::~SaveToBmp24()
{
}

bool SaveToBmp24::save(const unsigned char* src, int height, int width, std::string path, std::string name)
{
	int imagDataSize = height*width * 3; // imag data size

	//λͼ��һ���֣�λͼ�ļ�ͷ
	BITMAPFILEHEADER *bfHeader{ new BITMAPFILEHEADER };
	bfHeader->bfType = (WORD)0x4d42;  // string"BM"  
	bfHeader->bfSize = imagDataSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); // file size
	bfHeader->bfReserved1 = 0; // reserved  
	bfHeader->bfReserved2 = 0; // reserved  
	bfHeader->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); // real data λ��  

	//λͼ�ڶ����֣�λͼ��Ϣͷ  
	BITMAPINFOHEADER *biHeader{ new BITMAPINFOHEADER };
	biHeader->biSize = sizeof(BITMAPINFOHEADER);
	biHeader->biWidth = width;
	biHeader->biHeight = -height;//BMPͼƬ�����һ���㿪ʼɨ�裬��ʾʱͼƬ�ǵ��ŵģ�������-height������ͼƬ������  
	biHeader->biPlanes = 1;//Ϊ1�����ø�  
	biHeader->biBitCount = 24; // ÿ������ռ�õ�bit
	biHeader->biCompression = BI_RGB;//��ѹ��  
	biHeader->biSizeImage = imagDataSize;
	biHeader->biXPelsPerMeter = 0;//����ÿ��  
	biHeader->biYPelsPerMeter = 0;
	biHeader->biClrUsed = 0;//���ù�����ɫ��24λ��Ϊ0  
	biHeader->biClrImportant = 0;//ÿ�����ض���Ҫ 

	//���ļ�������
	//�ļ�·��
	std::string fpath;
	fpath += path;
	if (name.empty())//Ĭ��Ϊ��ʱ��
		fpath += "IMG_";
	else
		fpath += name + "_IMG_";
	SYSTEMTIME st;
	GetLocalTime(&st);
	char time[20];
	sprintf_s(time, sizeof(st), "%4d%2d%2d_%2d%2d%2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	for (int i = 0; time[i]; ++i)
	{
		if (time[i] == ' ') time[i] = '0';
	}
	fpath += time;
	fpath += ".bmp";
	std::fstream file(fpath, std::ios::out);//д�ļ�
	file.write((char*)bfHeader, sizeof(BITMAPFILEHEADER));
	file.write((char*)biHeader, sizeof(BITMAPINFOHEADER));
	file.write((char*)src, imagDataSize);
	file.close();
	return true;
}