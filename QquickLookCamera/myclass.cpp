#include "myclass.h"
#include <iostream>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "QDebug"
MyClass::MyClass(QWidget *parent,Qt::WindowFlags f)
: QWidget(parent,f),
  gridLayout(new QGridLayout(this))
{
	//��ʼ�����ݻ�����
	std::dynamic_pointer_cast<CCirQueue>(m_cmosData0)->Initial(1024 * 1296 * 256, 1024 * 1296 * 4);	
	std::dynamic_pointer_cast<CCirQueue>(m_cmosData1)->Initial(1024 * 1296 * 256, 1024 * 1296 * 4);
	std::dynamic_pointer_cast<CCirQueue>(m_cmosData2)->Initial(1024 * 1296 * 256, 1024 * 1296 * 4);
	std::dynamic_pointer_cast<CCirQueue>(m_cmosData3)->Initial(1024 * 1296 * 256, 1024 * 1296 * 4);
	//����Դע�����������
	m_inputSrc0->registerOutputBuffer(0, m_cmosData0);  //�������������������
	m_inputSrc1->registerOutputBuffer(0, m_cmosData1);	
	m_inputSrc2->registerOutputBuffer(0, m_cmosData2);
	m_inputSrc3->registerOutputBuffer(0, m_cmosData3);
	//start�����̣߳�����m_thrProcess = std::thread([this](){this->process(); })��������process��Դ����InputCMOS�����������ݷ��뻺����0��
	m_inputSrc0->start();
	m_inputSrc1->start();
	m_inputSrc2->start();
	m_inputSrc3->start();
	//�����ṩע�����뻺����
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider0)->registerInputBuffer(0, m_cmosData0);  //�����뻺������ȡ����
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider1)->registerInputBuffer(0, m_cmosData1);
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider2)->registerInputBuffer(0, m_cmosData2);
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider3)->registerInputBuffer(0, m_cmosData3);

	//ע�����������
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider0)->registerOutputBuffer(0, m_cmosFinalData0);
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider1)->registerOutputBuffer(0, m_cmosFinalData1);
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider2)->registerOutputBuffer(0, m_cmosFinalData2);
	std::dynamic_pointer_cast<ArrayCameraDataItem>(m_dataProvider3)->registerOutputBuffer(0, m_cmosFinalData3);

	//��ת����ע�����뻺����
	std::dynamic_pointer_cast<rotateDateProvider>(m_rotateDateProvider0)->registerInputBuffer(0, m_cmosFinalData0);
	std::dynamic_pointer_cast<rotateDateProvider>(m_rotateDateProvider1)->registerInputBuffer(0, m_cmosFinalData1);
	std::dynamic_pointer_cast<rotateDateProvider>(m_rotateDateProvider2)->registerInputBuffer(0, m_cmosFinalData2);
	std::dynamic_pointer_cast<rotateDateProvider>(m_rotateDateProvider3)->registerInputBuffer(0, m_cmosFinalData3);

	//���ÿ��
	m_dataProvider0->setup(16, 1280, 1024);   //����ṩ����ArrayCameraDataItem��ȥ֡ͷ�����뻺����1��
	m_dataProvider1->setup(16, 1280, 1024);
	m_dataProvider2->setup(16, 1280, 1024);
	m_dataProvider3->setup(16, 1280, 1024);
	//����ͨ�����
	m_magnifier0->setCmosNumber(0);
	m_magnifier00->setCmosNumber(4);
	m_magnifier1->setCmosNumber(1);
	m_magnifier2->setCmosNumber(2);
	m_magnifier3->setCmosNumber(3);
    //���ڰ�����ָ�룬��ʼ��m_dataProvider
	m_magnifier0->setDataItemPtr(m_dataProvider0);
	m_magnifier00->setDataItemPtr(m_dataProvider0);
	m_magnifier1->setDataItemPtr(m_dataProvider1);
	m_magnifier2->setDataItemPtr(m_dataProvider2);
	m_magnifier3->setDataItemPtr(m_dataProvider3);
	//���ڰ�����ת��ָ�룬��ʼ��m_convertor
	m_magnifier0->setConvertor(std::shared_ptr<PixelConvertor>(new Bayer2RGB));   //��ʾ��ɫͼ��ʱ��Ҫ�Ĵ��ڹ��캯��
	m_magnifier00->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));//�Ŵ�Ҷ���ʾ���Ա�
	m_magnifier1->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));
	m_magnifier2->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));
	m_magnifier3->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));
	//���ñ����ļ�����ʼ��m_file
	m_magnifier0->setSave(std::shared_ptr<SaveToFile>(new SaveToBmp24));//��Ϊ��ɫͼ�����24
	m_magnifier00->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));//��Ϊ�Ҷ�ͼ��8λ
	m_magnifier1->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));
	m_magnifier2->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));
	m_magnifier3->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));

	//���ô��ڴ�С
	m_magnifier0->setMagnifierRange(1280, 1024);
	m_magnifier00->setMagnifierRange(1280, 1024);
	m_magnifier1->setMagnifierRange(1280, 1024);
	m_magnifier2->setMagnifierRange(1280, 1024);
	m_magnifier3->setMagnifierRange(1280, 1024);
	m_magnifier00->setWindowFlags(Qt::Window);
	m_magnifier00->show();//ͨ��0���Ŵ�ͼ
	m_magnifier00->resize(420, 320);
	//��ʱˢ�½���
	m_refreshTimer = new QTimer(this);	
	connect(m_refreshTimer, &QTimer::timeout, this,[this](){m_magnifier0->refresh();});//�����б��в����������ں����еľֲ������������ں�������ʹ�øñ�����
	connect(m_refreshTimer, &QTimer::timeout, [this](){m_magnifier00->refresh(); });
	connect(m_refreshTimer, &QTimer::timeout, [this](){m_magnifier1->refresh(); });
	connect(m_refreshTimer, &QTimer::timeout, [this](){m_magnifier2->refresh(); });
	connect(m_refreshTimer, &QTimer::timeout, [this](){m_magnifier3->refresh(); });
	m_refreshTimer->start(30); 
	//������˫������ת��
	QObject::connect(m_magnifier0, SIGNAL(leftMouseClicked()), this, SLOT(showMagnifier0()));
	QObject::connect(m_magnifier0, SIGNAL(doubleClickEvent()), this, SLOT(showMagnifierBack()));
	QObject::connect(m_magnifier1, SIGNAL(leftMouseClicked()), this, SLOT(showMagnifier1()));
	QObject::connect(m_magnifier1, SIGNAL(doubleClickEvent()), this, SLOT(showMagnifierBack()));
	QObject::connect(m_magnifier2, SIGNAL(leftMouseClicked()), this, SLOT(showMagnifier2()));
	QObject::connect(m_magnifier2, SIGNAL(doubleClickEvent()), this, SLOT(showMagnifierBack()));
	QObject::connect(m_magnifier3, SIGNAL(leftMouseClicked()), this, SLOT(showMagnifier3()));
	QObject::connect(m_magnifier3, SIGNAL(doubleClickEvent()), this, SLOT(showMagnifierBack()));
	//Ĭ�Ͻ���
	gridLayout->addWidget(m_magnifier0, 0, 0);
	gridLayout->addWidget(m_magnifier1, 0, 1);
	gridLayout->addWidget(m_magnifier2, 1, 0);
	gridLayout->addWidget(m_magnifier3, 1, 1);
	setLayout(gridLayout);
}

MyClass::~MyClass()
{
}
//���������bmp�ļ�
void MyClass::setSaveFile()
{
   // qDebug() << "--------------����bmp�ļ�-----------"; 
	m_magnifier0->saveToFile();
	m_magnifier00->saveToFile();
	m_magnifier1->saveToFile();
	m_magnifier2->saveToFile();
	m_magnifier3->saveToFile();
}
//�����ͼ����ת90��
void MyClass::setRotate90()
{
	 qDebug() << "--------------ͼ����ת90-----------";
	 std::cout << "---------ͼ����ת90----------";
	 m_rotateDateProvider0->setup(16, 1024, 1280);  //����ǰ�߳�ֹͣ����ʼ�µ��߳�
	 m_rotateDateProvider1->setup(16, 1024, 1280);
	 m_rotateDateProvider2->setup(16, 1024, 1280);
	 m_rotateDateProvider3->setup(16, 1024, 1280);

	m_magnifier0->setDataItemPtr(m_rotateDateProvider0);
//	m_magnifier00->setDataItemPtr(m_dataProvider0);
	m_magnifier1->setDataItemPtr(m_rotateDateProvider1);
	m_magnifier2->setDataItemPtr(m_rotateDateProvider2);
	m_magnifier3->setDataItemPtr(m_rotateDateProvider3);
}
//��������һ����С
void MyClass::showMagnifier0()
{
	//qDebug()<< "-----------myclassdianjichenggong------------------";
	gridLayout->removeWidget(m_magnifier0);  // m_magnifier0->setVisible(true);
	gridLayout->removeWidget(m_magnifier1);
	gridLayout->removeWidget(m_magnifier2);
	gridLayout->removeWidget(m_magnifier3);
	
	gridLayout->addWidget(m_magnifier0, 0, 0, 3, 1);
	gridLayout->addWidget(m_magnifier1, 0, 1, 1, 1);
	gridLayout->addWidget(m_magnifier2, 1, 1, 1, 1);
	gridLayout->addWidget(m_magnifier3, 2, 1, 1, 1);
	setLayout(gridLayout);
}
void MyClass::showMagnifier1()
{
	//qDebug()<< "-----------myclassdianjichenggong------------------";
	gridLayout->removeWidget(m_magnifier0);
	gridLayout->removeWidget(m_magnifier1);
	gridLayout->removeWidget(m_magnifier2);
	gridLayout->removeWidget(m_magnifier3);

	gridLayout->addWidget(m_magnifier1, 0, 0, 3, 1);
	gridLayout->addWidget(m_magnifier0, 0, 1, 1, 1);
	gridLayout->addWidget(m_magnifier2, 1, 1, 1, 1);
	gridLayout->addWidget(m_magnifier3, 2, 1, 1, 1);
	setLayout(gridLayout);
}
void MyClass::showMagnifier2()
{
	//qDebug()<< "-----------myclassdianjichenggong------------------";
	gridLayout->removeWidget(m_magnifier0);
	gridLayout->removeWidget(m_magnifier1);
	gridLayout->removeWidget(m_magnifier2);
	gridLayout->removeWidget(m_magnifier3);

	gridLayout->addWidget(m_magnifier2, 0, 0, 3, 1);
	gridLayout->addWidget(m_magnifier0, 0, 1, 1, 1);
	gridLayout->addWidget(m_magnifier1, 1, 1, 1, 1);
	gridLayout->addWidget(m_magnifier3, 2, 1, 1, 1);
	setLayout(gridLayout);
}
void MyClass::showMagnifier3()
{
	//qDebug()<< "-----------myclassdianjichenggong------------------";
	gridLayout->removeWidget(m_magnifier0);
	gridLayout->removeWidget(m_magnifier1);
	gridLayout->removeWidget(m_magnifier2);
	gridLayout->removeWidget(m_magnifier3);

	gridLayout->addWidget(m_magnifier3, 0, 0, 3, 1);
	gridLayout->addWidget(m_magnifier0, 0, 1, 1, 1);
	gridLayout->addWidget(m_magnifier1, 1, 1, 1, 1);
	gridLayout->addWidget(m_magnifier2, 2, 1, 1, 1);
	setLayout(gridLayout);
}
//˫����ԭĬ�ϴ���
void MyClass::showMagnifierBack()
{
	//qDebug()<< "-----------myclassdianjichenggong------------------";
	gridLayout->removeWidget(m_magnifier0);
	gridLayout->removeWidget(m_magnifier1);
	gridLayout->removeWidget(m_magnifier2);
	gridLayout->removeWidget(m_magnifier3);

	gridLayout->addWidget(m_magnifier0, 0, 0);
	gridLayout->addWidget(m_magnifier1, 0, 1);
	gridLayout->addWidget(m_magnifier2, 1, 0);
	gridLayout->addWidget(m_magnifier3, 1, 1);
	setLayout(gridLayout);
}

//void MyClass::mousePressEvent(QMouseEvent *e)
//{
//  if (e->button()==Qt::LeftButton)
//  {
//	  emit clicked();
//  }
//}