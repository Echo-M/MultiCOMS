#pragma once
#include "IDataProcessUnit.h"
#include "IDataItem.h"

class ZoomedImageDataItem : public IDataItem, public IDataProcessUnit
{
protected:
	int m_height;//�任ǰ�ĸߣ�m_features����任��Ŀ�͸�
	int m_width;//�任ǰ�Ŀ�m_features����任��Ŀ�͸�
	float m_ratio;
	std::shared_ptr<unsigned char> m_zoomedImageBuffer[2]; //�任���ͼ������,˫����
public:
	ZoomedImageDataItem(float _ratio);
	~ZoomedImageDataItem();
	//inherit from IDataItem, �任֮ǰ�Ŀ�͸�
	virtual bool setup(int assWidth, int imgWidth, int height) override;
	//copy area of image data
	virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const override;
protected:
	//inherit from IDataProcessUnit
	virtual void process() override;
	//�Ŵ�ratio��
	void Zoom(unsigned char* dst, const unsigned char* src, float _ratio);
};

