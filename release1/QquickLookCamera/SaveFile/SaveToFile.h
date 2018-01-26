#ifndef _SAVE_TO_FILE_H_
#define _SAVE_TO_FILE_H_

#include <windows.h>
#include <fstream>
#include <string>
#include <time.h>
#include<qstring.h>
class SaveToFile
{
public:
	SaveToFile();
	virtual ~SaveToFile();
	/*
	//��ͼ�����ݱ���Ϊ�ļ����ɸ�������+ϵͳʱ��������Ĭ��Ϊ��IMG_YYYYMMDD_HHmmSS.bmp��
	//path����·������ʽʾ��"C:\\Users\\Administrator\\Desktop\\QView\\"   //���� \ Ҳ��ת���ַ�����ʼ�ַ�
	*/
	virtual bool save(const unsigned char* src, int height, int width, std::string path, std::string name = "") = 0;
};

#endif