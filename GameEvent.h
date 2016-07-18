#ifndef __LYNX_GAME_SERVER_GAME_EVENT_H__
#define __LYNX_GAME_SERVER_GAME_EVENT_H__

#include "CommonLib.h"

namespace Lynx
{
	enum EventId
	{
		EVENT_SERVER_STARTUP,                     // �����������¼�
		EVENT_SERVER_SHUTDOWN,                    // �������ر��¼�
		EVENT_CLIENT_CONNECTION_NOT_EXIST,        // �ͻ������Ӷ��󲻴���
		EVENT_CLIENT_CONNECTION_CLOSE,            // �߼���رտͻ��������¼�
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_GAME_EVENT_H__

