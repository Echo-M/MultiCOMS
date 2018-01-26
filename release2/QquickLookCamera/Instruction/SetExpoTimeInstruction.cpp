#include "SetExpoTimeInstruction.h"


SetExpoTimeInstruction::SetExpoTimeInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far)
: InstructionUnit(port_local_send, port_local_recv, port_far_send, ip_local, ip_far)
{

}


SetExpoTimeInstruction::~SetExpoTimeInstruction()
{
}

void SetExpoTimeInstruction::thrSend()
{
	sendCmdReg(160, 0x0010);//��Ϊ�ֶ��ع�
	sendCmdReg(161, 0x60B8);
	sendCmdReg(201, m_expoTime);//�����ع�ʱ��
}
