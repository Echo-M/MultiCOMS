#pragma once

#include "IDataProcessUnit.h"
#include "IDataItem.h"

class RotatedImageDataItem : public IDataItem, public IDataProcessUnit
{
protected:
	int m_height;//�任ǰ�ĸߣ�m_features����任��Ŀ�͸�
	int m_width;//�任ǰ�Ŀ�m_features����任��Ŀ�͸�
	int m_assWidth;
	int m_angle;
	std::shared_ptr<unsigned char> m_rotatedImageBuffer[2]; //�任���ͼ������,˫����
public:
	RotatedImageDataItem(int _angle);
	~RotatedImageDataItem();
	bool changeAngle(int _angle);
	//inherit from IDataItem, �任֮ǰ�Ŀ�͸�
	virtual bool setup(int assWidth, int imgWidth, int height) override;
	//copy area of image data
	virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const override;
protected:
	//inherit from IDataProcessUnit
	virtual void process() override;
	//˳ʱ����תһ���Ƕ�
	void Rotate(unsigned char* dst, const unsigned char* src, int angle);
};

