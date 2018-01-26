#pragma once
#include "initsock.h"
#include <thread>
#pragma pack (push)
#pragma pack (1)
struct CMD
{
	//16 byte + 8 byte
	unsigned int synWord{ 0 }; // ����ͬ���� - 4 byte
	unsigned int dataLength{ 0 }; // �������ݳ��� - 4 byte
	unsigned short counter{ 0 }; // ��������� - 2 byte
	unsigned short form{ 0 }; // ��������
	unsigned short timeStamp{ 0 }; // ʱ���
	unsigned short checkSum{ 0 }; // У���

	unsigned long long data{ 0 }; // ��������
};

#pragma pack (pop)
class Instruction
{
//���캯��&��������
public:
	Instruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~Instruction();
	void closeSock();

//����
public:
	//ö������CMOSID
	enum CMOSID{ NoCMOS, CMOSE, CMOS1, CMOS2, CMOS3 };
private:
	CMOSID cmosId;
	CMD *cmd;
	bool SENDFLAG = false;

	unsigned int m_ag_cg{ 0x01e2 };//�Ĵ���ֵ
	unsigned int m_dg{ 0x0080 };//�Ĵ���ֵ
	int m_FPS{ 18 };
	unsigned int m_frLength{ 10000 };//֡��
	unsigned int m_frExpoTime{ 2000 };//�ع�ʱ�䣬С��֡��

	bool m_recving{ false }; //�߳��˳���ʶ
	bool m_sendingAECRun{ false }; //�߳��˳���ʶ
	bool m_sendingsetExpoTime{ false }; //�߳��˳���ʶ
	bool m_sendingSetFPS{ false }; //�߳��˳���ʶ
	bool m_sendingSetDG{ false };
	bool m_sendingSetAGCG{ false };

	std::thread m_thrSendSetFPS; //�߳̾��
	std::thread m_thrRecv; //�߳̾��
	std::thread m_thrSendsetExpoTime; //�߳̾��
	std::thread m_thrSendAECRun; //�߳̾��
	std::thread m_thrSendSetDG;
	std::thread m_thrSendSetAGCG;
//socket
protected:
	CInitSock c;//Winsock���װ����ͷ�
	SOCKET sock_send;
	SOCKET sock_recv;
	sockaddr_in addr_local_send;
	sockaddr_in addr_far_send;
	sockaddr_in addr_local_recv;
//
public:
	//
	bool startSendAECRun()
	{
		if (m_sendingAECRun)
			return false;
		m_sendingAECRun = true;
		m_thrSendAECRun = std::thread([this](){this->AECRun(); });
		return true;
	}
	void stopSendAECRun()
	{
		m_sendingAECRun = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendAECRun.joinable())
			m_thrSendAECRun.join();
	}
	//
	bool startSendsetExpoTime(unsigned int _expoTime)
	{
		m_frExpoTime = _expoTime;
		if (m_sendingsetExpoTime)
			return false;
		m_sendingsetExpoTime = true;
		m_thrSendsetExpoTime = std::thread([this](){this->ManualExposure(m_frExpoTime); });
		return true;
	}
	void stopSendsetExpoTime()
	{
		m_sendingsetExpoTime = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendsetExpoTime.joinable())
			m_thrSendsetExpoTime.join();
	}
	//
	bool startSendSetFPS(int _fps)
	{
		m_FPS = _fps;
		if (m_sendingSetFPS)
			return false;
		m_sendingSetFPS = true;
		m_thrSendSetFPS = std::thread([this](){this->SetFPS(m_FPS); });
		return true;
	}
	void stopSendSetFPS()
	{
		m_sendingSetFPS = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetFPS.joinable())
			m_thrSendSetFPS.join();
	}
	//
	bool startSendSetAGCG(int _total)
	{
		m_ag_cg = _total;
		if (m_sendingSetAGCG)
			return false;
		m_sendingSetAGCG = true;
		m_thrSendSetAGCG = std::thread([this](){this->SetAGCG(m_ag_cg); });
		return true;
	}
	void stopSendSetAGCG()
	{
		m_sendingSetAGCG = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetAGCG.joinable())
			m_thrSendSetAGCG.join();
	}
	//
	bool startSendSetDG(int _dg)
	{
		if (m_sendingSetDG)
			return false;
		m_sendingSetDG = true;
		m_thrSendSetDG = std::thread([this](){this->SetDG(m_dg); });
		return true;
	}
	void stopSendSetDG()
	{
		m_sendingSetDG = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetDG.joinable())
			m_thrSendSetDG.join();
	}
	//
	bool startRecv()
	{
		if (m_recving)
			return false;
		m_recving = true;
		m_thrRecv = std::thread([this](){this->receive(); });
		return true;
	}
	void stopRecv()
	{
		m_recving = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrRecv.joinable())
			m_thrRecv.join();
	}
//����
private:
	unsigned short cmdCounter;//����ָ���������ֵΪ0
public:
	void SetCmosId(CMOSID id);
	void Stop()
	{
		PowerDown();
	}
protected:
	void ClearFIFO();
	//��ʼ�ϴ�ͼ���Զ��ع�
	void AECRun();
	//�ֶ��ع�
	void ManualExposure(unsigned int _expoTime);
	void SetFPS(int _fps);
	void SetAGCG(float _gainTotal);
	void SetDG(float _dg);

	bool buildCmdReg(int _addr, int _data);
	void sendCmdReg(int _addr, int _data);
	void PowerUp();
	void EnableClockManagement1();
	void EnableClockManagement2();
	void RequiredRegisterUpload();
	void ExtraConfig();
	void SoftPowerUp();
	void EnableSequencer();

	void DisableSequencer();
	void SoftPowerDown();
	void DisableClockManagement2();
	void DisableClockManagement1();
	void PowerDown();
//����
public:
	bool DONEFLAG = false;
protected:
	void receive();
};