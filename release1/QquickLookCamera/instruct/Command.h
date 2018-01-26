#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_
#include "../QData/initsock.h"

//PC->FPGA��ָ�������������ı� ��������ҪҪ��ָ���Ӳ�������ݣ�Ȼ��InputCOMS��ʼ�������ݡ�
class Command
{
public:
	Command(USHORT port_target, ULONG ip_target, USHORT port_local, ULONG ip_local);
	~Command();

	//�Զ��ع�
	bool AutoRun();

	//�ֶ��ع�
	bool ManulRun();

	//ֹͣCMOS Core����
	bool stopCMOS();

	//����Target Illumination
	//tai��0-1023��Ĭ��ֵΪ0xb8-------10��������
	bool setTAI(int tai = 0xb8);

	//�����ع�ʱ��
	//et��0-2EE0��-------16��������
	bool setET(int et);

	//����Digital Gain��DG��
	//dgΪС������������Ϊ5λ��С������Ϊ7λ
	bool setDG(double dg);

	//����AFE Gain��Column Gain��AG CG��
	//gainΪ0-10�е�����������ʾ���汶��
	//0--1x
	//1--1.14x
	//2--1.33x
	//3--1.60x
	//4--2.00x
	//5--2.29x
	//6--2.67x
	//7--3.20x
	//8--4.00x
	//9--5.33x
	//10--8.00x
	bool setAG_CG(int gain);

	//��������Ĵ���
	//addr���Ĵ�����ַ��9bit
	//data��16bit
	bool writeRegister(int addr, int data);


protected:
	CInitSock c;//Winsock���װ����ͷ�
	SOCKET sock;//����ͼ�������׽���
	sockaddr_in addr_local;//���ص�ַ
	sockaddr_in addr_target;//Զ�̵�ַ

private:
};

#pragma pack (push)
#pragma pack (1)
struct CMD
{
	//12 Bytes
	unsigned short frmhead{ 0xeb90 };//֡ͷ
	unsigned short cmdCount{ 0x0000 };//PCָ�����
	unsigned int cmd;//ָ��
	unsigned int utility;//����
};

#pragma pack (pop)
#endif