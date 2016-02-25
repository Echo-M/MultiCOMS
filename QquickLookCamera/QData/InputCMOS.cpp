#include "InputCMOS.h"
#include <QDebug>
InputCMOS::InputCMOS(USHORT port, ULONG ip)
{
	//�����׽��ֶ��󣬲���Ϊ������ģʽ
	ULONG ul = 1;
	sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	::ioctlsocket(sock, FIONBIO, (ULONG*)&ul);//��Ϊ������ģʽ

	//����ַ�ṹ
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ip;
	addr.sin_port = htons(port);

	//���׽������Ӧ��ַ��
	if (::bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
			//printf("failed bind()...\n");
			return;
	}
}

InputCMOS::~InputCMOS()
{
	::closesocket(sock);
}

void InputCMOS::process()
{
	UCHAR *recvbuf = new UCHAR[NUMOFRECVBUF];//����UDP����
	sockaddr_in addr_far;
	int alen = sizeof(addr_far);
	//selectģ��
	//����׽��־�����׽��ּ���
	FD_ZERO(&fdsock); // �����
	FD_SET(sock, &fdsock);
	struct timeval timeout;
	timeout.tv_sec = 100;
	while (m_processing)
	{
		fd_set fdread = fdsock;//fdsock_img��copy
		int nret = ::select(0, &fdread, NULL, NULL, &timeout);//select()����ֵ����δ��I/O���׽��־���ĸ���

		if (nret == SOCKET_ERROR)
		{
			//printf("failed select()...\n");
			continue;
		}
		if (nret == 0)//��ʱ
		{
			//qDebug() << ("overtime...\n");
			continue;
		}

		if (nret > 0)
		{
			for (int i = 0; i<(int)fdsock.fd_count; i++)
			{
				memset(recvbuf, 0, sizeof(*recvbuf));//��ս��ջ�����
				if (FD_ISSET(sock, &fdread))//�����׽��ֿɶ�
				{
					int nrecv = ::recvfrom(sock, (char*)recvbuf, NUMOFRECVBUF, 0, (LPSOCKADDR)&addr_far, &alen);

					if (nrecv > 0)
					{
						if (m_outputBuffer[0].second)
							m_outputBuffer[0].second->push_back(recvbuf, nrecv);//�����ܵ����ݷ������������
						//qDebug() << ("\n�յ�%d���ֽ�", nrecv);
					}
				}
			}
		}
	}
}