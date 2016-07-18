#ifndef __LYNX_COMMON_LIB_FIRE_PACKET_H__
#define __LYNX_COMMON_LIB_FIRE_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{

		FORBIDDEN_LOGIN				=	sFireMsgBase+1,	//��ֹ��½
		FIRE_CONDITION_RESP  		=   sFireMsgBase+2,	//������Ҽ������
		FIRE_CONFIRM_DATA_REQ		=   sFireMsgBase+4,	//�������
		FIRE_CONFIRM_DATA_RESP		=   sFireMsgBase+5,	//������Ҽ�ؽ��

		WELFALE_MARKET_BEGIN_REQ			=   sWelfaleMarketBase+32,	//�и�������ս
		WELFALE_MARKET_BEGIN_RESP			=   sWelfaleMarketBase+33,	//�и�������ս����
		WELFALE_MARKET_END_REQ				=   sWelfaleMarketBase+34,	//�и����н���
		WELFALE_MARKET_END_RESP				=   sWelfaleMarketBase+35,	//�и����н���

		TWELVE_PALACE_UNLOCK_REQ			=   sWelfaleMarketBase+60,	//ʮ���ڹ�����
		TWELVE_PALACE_UNLOCK_RESP			=   sWelfaleMarketBase+61,	//ʮ���ڹ���������
		TWELVE_PALACE_BEGIN_REQ				=   sWelfaleMarketBase+62,	//ʮ���ڹ���ս
		TWELVE_PALACE_BEGIN_RESP			=   sWelfaleMarketBase+63,	//ʮ���ڹ���ս����
		TWELVE_PALACE_END_REQ				=   sWelfaleMarketBase+64,	//ʮ���ڹ�����
		TWELVE_PALACE_END_RESP				=   sWelfaleMarketBase+65,	//ʮ���ڹ�����

		TWELVE_PALACE_RANDOMENTRY_REQ				=   sWelfaleMarketBase+66,	//����������
		TWELVE_PALACE_RANDOMENTRY_RESP				=   sWelfaleMarketBase+67,	//�����߷���

		

	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_FIRE_PACKET_H__