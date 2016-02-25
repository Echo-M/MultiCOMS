#ifndef _INPUT_CMOS_HPP_
#define _INPUT_CMOS_HPP_
#include "IDataProcessUnit.h"
#include "initsock.h"
#include <fstream>

#define NUMOFRECVBUF 1500 //recvbuf�Ĵ�С ������process()�õ�

class InputCMOS : public IDataProcessUnit
{
protected:
	CInitSock c;//Winsock���װ����ͷ�
	SOCKET sock;//����ͼ�������׽���
	fd_set fdsock;//�׽��ּ���
	SOCKADDR_IN addr;//PC�˵�ַ
	
public:
	InputCMOS(USHORT port_pc, ULONG ip_pc);
	
	~InputCMOS();
	
protected:
	// ���ݴ������
	virtual void process() override;
};
#endif
