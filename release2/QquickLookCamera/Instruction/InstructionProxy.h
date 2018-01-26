#pragma once
#include "InstructionUnit.h"
class InstructionProxy
{
public:
	InstructionProxy(InstructionUnit::CMOSID _cmosId);
	~InstructionProxy();
	bool AECRun();
	bool setExpoTime(unsigned int _expoTime);
	bool SetFPS(int _fps);
	bool Stop();
	bool SetAGCG(float _total);
	bool SetDG(float _dg);
	//���ã���������ʱ�������ض˿�
	void setPortLocalSend(USHORT tmp){ port_local_send = tmp; }
	//���ã���������ʱ�������ض˿�
	void setPortLocalRecv(USHORT tmp){ port_local_recv = tmp; }
	//���ã���������ʱ����Ŀ��˿�
	void setPortFarSend(USHORT tmp){ port_far_send = tmp; }
	//���ñ���IP
	void setIPLocal(char *tmp){ ip_local=tmp; }
	//����Ŀ��IP
	void setIPFar(char *tmp){ ip_far=tmp; }
private:
	USHORT port_local_send;
	USHORT port_local_recv;
	USHORT port_far_send;
	char* ip_local;
	char* ip_far;
	InstructionUnit::CMOSID m_cmosId;
};

