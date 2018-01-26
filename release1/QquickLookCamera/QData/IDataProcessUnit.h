#ifndef _I_DATA_PROCESS_UNIT_HPP_
#define _I_DATA_PROCESS_UNIT_HPP_

#include <thread>
#include <memory>
#include "IBuffer.h"
#include "CirQueue.h"
class IBuffer;
class SaveProxy;
class QString;

//ʹ��ģ������Ϊ�޷�Ϊ�������͵ı���ָ�������С��vector��map֮��ı�����������ʧ
//template<int MaxInputChannel, int MaxOutputChannel>
#define MaxInputChannel 8
#define MaxOutputChannel 128
class IDataProcessUnit
{
protected:
	//������������λ���ȣ���ʶ�Զ����ֽ�Ϊһ�����������ݣ���Щ����Ԫ����Ҫʹ�ô���ֵ
	//һ��ֻ�����뻺������Ҫ�˽ⳤ����Ϣ
	typedef std::pair<int, std::shared_ptr<IBuffer>> IntBufferPair; 
	
	IntBufferPair m_inputBuffer[MaxInputChannel]; //���뻺����
	IntBufferPair m_outputBuffer[MaxOutputChannel]; //���������
	bool m_discardable{ false }; //�����ݳ�����������ʱ���Ƿ����������ݣ����µĻ��Ǿɵ����д���
	bool m_processing{ false }; //�߳��˳���ʶ
	std::thread m_thrProcess; //�߳̾��
protected:
	IDataProcessUnit(){
		for (int i = 0; i < MaxInputChannel; ++i) {
			m_inputBuffer[i].first = -1;
			m_inputBuffer[i].second = nullptr;
		}
	}

public:
	//���ݴ��������ڴ�
	virtual void process() {
	}

public:
	virtual ~IDataProcessUnit() { 
		stop(); 
	}
	int maxInputChannel() const { return MaxInputChannel; }
	int maxOutputChannel() const { return MaxOutputChannel; }



	//ע�����뻺����
	virtual bool registerInputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize = -1);
	//ע�����������
	virtual bool registerOutputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize = -1);
	//�������뻺�������ݵ�Ԫ��С(�ֽ���)
	virtual bool resetInputUnitSize(int channel, int unitSize);
	//����������������ݵ�Ԫ��С(�ֽ���)
	virtual bool resetOutputUnitSize(int channel, int unitSize);

	virtual bool saveToFile(SaveProxy& /*saveProxy*/, const QString& /*savePath*/) { return false; }
	virtual void stopSaving(){}

public:
	//����
	virtual bool start() {
		if (m_processing)
			return false;
		m_processing = true;
		m_thrProcess = std::thread([this](){this->process(); });
		return true;
	}
	//ֹͣ
	virtual void stop() {
		m_processing = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrProcess.joinable())
			m_thrProcess.join();
		//��λ����
		//reset();
	}
};

#endif