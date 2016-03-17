#include "ImageMagnifierItem.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

ImageMagnifierItem::ImageMagnifierItem(QWidget *parent, Qt::WindowFlags f, QImage::Format str)
    : QWidget(parent, f),
	strFormat(str)
{
	//m_image = new QImage(m_range, QImage::Format_ARGB32);  //default range 5*5, scale to the rectangle of window
	m_image = new QImage(m_range, strFormat);
    //unsigned char* bits = m_image->bits(); for (int i=0; i<25*25; ++i)   *((unsigned int*)(bits+i*4)) = (((10*i)<<8) | (0xff<<24));    
}

ImageMagnifierItem::~ImageMagnifierItem()
{

}

void ImageMagnifierItem::paintEvent(QPaintEvent *)//ʵ���ػ棬��ʾͼ��
{
    displayModeDisplay();
}
void  ImageMagnifierItem::mousePressEvent(QMouseEvent *e)//�����л���ʾ
{
	if (e->button()== Qt::LeftButton )
	{
		emit leftMouseClicked();
	}
}
void ImageMagnifierItem::mouseDoubleClickEvent(QMouseEvent *e)//˫���ָ�  //��ô֪������˫��
{
	emit doubleClickEvent();
}
void ImageMagnifierItem::setCmosNumber(int number)//������ʾͨ��
{
	cmosNumber = number;
}
int ImageMagnifierItem::setDataItemPtr(std::shared_ptr<IDataItem>& ptr)
{
   IWindowItem::setDataItemPtr(ptr);
   const FeaturesOfDataItem* features = m_dataProvider->constDataFeatures();
   if (!features)
       return ERR_INTERNAL_ERROR;
   m_imgBuffer.reset(new unsigned char[m_range.width()*m_range.height()*static_cast<int>(features->payloadDataSize)]);    //ͼ����������ָ��
   return EXEC_SUCCESS;
}

int ImageMagnifierItem::saveToFile() const
{
	if (!m_file)
	{
		return ERR_NOT_SET_FILE;
	}
	if (!m_dataProvider || !m_imgBuffer || !m_image)
		return ERR_NOT_SET_DATAPTR;
	unsigned char *buffer{ new unsigned char[m_range.width()*m_range.height()] };
	unsigned char *bufferConverted{ new unsigned char[m_range.width()*m_range.height() * 3] };
	m_dataProvider->copyArea(0, 0, m_range.width(), m_range.height(), buffer);

	std::string name;
	char temp[10];
	sprintf_s(temp, 10, "COMS%d", cmosNumber);
	name += temp;
	
	if (cmosNumber == 0)
	{
		m_convertor->convert(bufferConverted, buffer, m_range.height(), m_range.width());
		m_file->save(bufferConverted, m_range.height(), m_range.width(), /*"../QquickLookCamera/BMP/"*/"C:\\Users\\CMOS\\Desktop\\MultiCOMS\\BMP\\", name);
	}
	else  //�Ҷ�ͼ��ֱ�Ӵ洢ԭʼbayer(8Ϊ���)����   //���ȫ��ת����洢��8->32ת���д�
		m_file->save(buffer, m_range.height(), m_range.width(), "C:\\Users\\CMOS\\Desktop\\MultiCOMS\\BMP\\", name);  //����Ϊ��ָ�룬������ʵ�ַ�������ҷ���displayModeDisplay()��ʵʱ����
	
	delete buffer;
	delete bufferConverted;
}

int ImageMagnifierItem::displayModeDisplay() const
{
    if (!m_dataProvider || !m_imgBuffer || !m_image)
        return ERR_NOT_SET_DATAPTR;
    if (!m_convertor)       //����ת���������ʾ
        return ERR_NOT_SET_CONVERTOR;
    const FeaturesOfDataItem* features = m_dataProvider->constDataFeatures();
    if (!features)
        return ERR_INTERNAL_ERROR;

    //fetch pixel around cursor, convert to rgba format
    QRect rtWnd = geometry();//��ʾ�����Ͻ����꼰��ʾ���ĳ���
	//m_range��ʾͼ��Ĵ�С
    QRect rtRange(
                m_cursorPostion.x()-m_range.width()/2,
                m_cursorPostion.y()-m_range.height()/2,
                m_range.width(),
                m_range.height()
                );
    QRect rtData(0,0,features->payloadDataWidth, features->linesPerFrame);
    //qDebug()<<rtWnd<<rtRange<<"->";
    if (!rtData.contains(rtRange))//������������ͼ����ʾ��
    {
        //adjust target rectangle
        if (!rtData.contains(0, rtRange.top()))
            rtRange.setTop(0);
        if (!rtData.contains(rtRange.left(), 0))
            rtRange.setLeft(0);
        if (!rtData.contains(rtRange.right(), 0))
        {
            rtRange.setRight(rtData.right());
            int left = rtData.right()-m_range.width();
            rtRange.setLeft(left < 0 ? 0 : left);
        }
        if (!rtData.contains(rtRange.right(), rtRange.bottom()))
        {
            rtRange.setBottom(rtData.bottom());
            int top = rtData.bottom()-m_range.height();
            rtRange.setTop(top < 0 ? 0 : top);
        }

        rtRange.setWidth(m_range.width());
        rtRange.setHeight(m_range.height());
    }
    //qDebug()<<rtRange<<"<-\n";
    unsigned char *pixel = m_imgBuffer.get();
	//put sth useful information
	QString str;
	str.sprintf("COMS %d", cmosNumber);  //ͼ���ϱ��ͨ��
	
	//	 str.sprintf("[%d,%d, %d*%d] H:%.2f, V:%.2f", rtRange.left()*features->colSampleLevel, rtRange.top()*features->rowSampleLevel, rtRange.width(), rtRange.height(),
	//	 static_cast<float>(pic.width())/rtRange.width(), static_cast<float>(pic.height())/rtRange.height());
	
    if (!m_dataProvider->copyArea(rtRange.left(), rtRange.top(), rtRange.width(), rtRange.height(), pixel))
        return ERR_INVALID_PARAM; 
    m_convertor->convert(m_image->bits(), pixel, rtRange.width(), rtRange.height());

	//scale and paint
    QPainter painter(const_cast<ImageMagnifierItem*>(this));
    QPixmap pic = QPixmap::fromImage(*m_image).scaled(rtWnd.width(), rtWnd.height(), Qt::KeepAspectRatio);
    QRect rtDst((rtWnd.width()-pic.width())/2,(rtWnd.height()-pic.height())/2,pic.width(), pic.height());
    painter.drawPixmap(rtDst, pic);//��ʾͼ��

	painter.setPen(m_fontColor);
	painter.drawText(rtDst, str);
    return EXEC_SUCCESS;
}

IWindowItem* ImageMagnifierItem::clone() const
{
	ImageMagnifierItem* item = new ImageMagnifierItem;
	item->setConvertor(m_convertor);
	item->setSave(m_file);
//	item->setRgbSave(m_rgbFile);
	return item;
}


int ImageMagnifierItem::refresh()  //����update(),�Զ�����paintEvet();�ٵ���displayModeDisplay().
{
    update();    
    return EXEC_SUCCESS;
}

void ImageMagnifierItem::setMagnifierRange(int width, int height)
{
    if (width == m_image->width() && height == m_image->height())
        return;
    if (width < 0 || height < 0)
        return;
    if (!m_dataProvider)
        return ;
    const FeaturesOfDataItem* features = m_dataProvider->constDataFeatures();
    if (!features)
        return;
    if (width > features->payloadDataWidth || height > features->linesPerFrame)
        return ;

    m_imgBuffer.reset();

    m_range = {width, height};
    if (m_image)
        delete m_image;
 // m_image = new QImage(m_range, QImage::Format_ARGB32);
	m_image = new QImage(m_range,strFormat);
    m_imgBuffer.reset(new unsigned char[m_range.width()*m_range.height()*static_cast<int>(features->payloadDataSize)]);

    QWidget::update();
}
