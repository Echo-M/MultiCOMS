#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>
#include "./QData/InputCMOS.h"
#include "./QData/ImageDataItem.h"
#include "QData\RotatedImageDataItem.h"
#include "QData/ZoomedImageDataItem.h"
#include "./QView/IWindowItem.h"
#include "QView/ImageWindowItem.h"
#include "./QView/Utility/Pixel8To32.h"
#include "./QView/Utility/PixelBayerToRGB.h"

#include "./QView/ImageMagnifierItem.h"  //class ImageMagnifierItem;
#include "./SaveFile/SaveToBmp24.h"
#include "./SaveFile/SaveToBmpGray.h"
#include "../QData/CirQueue.h"
#include <QTimer>
#include <QDebug>
#include <QGridLayout>
class QGridLayout;

class MyClass : public QWidget
{
	Q_OBJECT

public:
	MyClass(InstructionUnit::CMOSID _cmosId, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~MyClass();
	void setSaveFile();
	bool onMainWindowClosed()
	{
		qDebug() << "mainWindow closed!!!\n";
		return m_window->onStopActionTriggerd();
	}
protected:
	void onMouseDoubleClicked()
	{
		m_magnifier->show();
	}
	/*void onMouseDoubleClicked()
	{
		rotateImage();
	}*/
private:
	InstructionUnit::CMOSID cmosId;
	int m_assWidth;
	int m_height;
	int m_imgWidth;
	int m_angle;
	const int m_BufPicNum;//�������洢������ͼ�������
	QGridLayout *gridLayout{ new QGridLayout(this) };
protected:
	//�������ݵ��ṩ����InputCMOS
	IDataProcessUnit* m_inputSrc;
	//����֡���ݵ��ṩ����ImageDataItem
	std::shared_ptr<IDataItem> m_dataProvider{ new ImageDataItem };
	//��ת�����ݵ��ṩ����RotatedImageDataItem
	std::shared_ptr<IDataItem> m_rotatedDataProvider{ new RotatedImageDataItem(m_angle) };
	//�������������
	std::shared_ptr<IBuffer> m_cmosData{ new CCirQueue };
	//�����������ͼ��֡
	std::shared_ptr<IBuffer> m_cmosImageData{ new CCirQueue };//ͼ��֡����

protected:
	QTimer* m_refreshTimer{ nullptr };
	//ImageMagnifierItem* m_magnifier{ new ImageMagnifierItem(this) }; 
	ImageWindowItem *m_window{ new ImageWindowItem };
	ImageMagnifierItem *m_magnifier{ new ImageMagnifierItem };

private:
	void rotateImage();
	//�Ŵ���ʾ
    //void showMagnifier();
};

#endif // MYCLASS_H
