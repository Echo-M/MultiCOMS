#pragma once


class ImageProcessing
{
private:
	int m_height;
	int m_width;
public:
	ImageProcessing(int height, int width);
	~ImageProcessing();

public:
	//24λ���ɫͼ���ţ�ratioΪ�������ӣ�ratio=2��Ϊ�Ŵ�2����
	void Zoom(unsigned char* dst, const unsigned char* src, int& width, int& height, double ratio);
	//24λ���ɫͼ��תangle�㣬ͼ��ߡ���ı䣬�հײ����ɰ�ɫ���
	void Rotate(unsigned char* dst, const unsigned char* src, int& width, int& height, int angle);
	//24λ���ɫͼת�ã���ͬ����ת90�㣬�ߡ���Ե�
	void Transposition(unsigned char* dst, const unsigned char* src, int& width, int& height);
	//24λ���ɫͼƽ�ƣ��ߡ����䣬��x,y��Ϊ�����ƶ��������꣬�հײ����ɰ�ɫ���
	bool Shift(unsigned char* dst, const unsigned char* src, int x, int y);
	//24λ���ɫͼģ�����㣨ƽ�����񻯣�
	bool TemplateOperation(unsigned char* dst, const unsigned char* src, int templateType);
	//����M3(Bayer������),�Ҷ�ͼ��ת��Ϊ��ֵͼ�񣨴˴����ɻҶ�ͼ���ʾ��ֻ��255��ʾ�ס�0��ʾ�ڣ�
	bool LimbPatternM3(unsigned char* dst, const unsigned char* src);
};

