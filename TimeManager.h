#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__

#include <time.h>
#include "PlatformLib/PlatformLib.h"

namespace Lynx
{
	class TimeManager: public Singleton<TimeManager>
	{
	public:
		//�ж�ĳ��ʱ���Ƿ��Ǵ��ڽ���
		static bool timeIsToday(UInt64 timeParam); 

		//�ж�����ʱ���Ƿ���ͬһ��
		static bool timeIsOneDay(UInt64 curtime, UInt64 srctime);
		//�ж�ĳ��ʱ���Ƿ��ڱ�����
		static bool timeIsToWeek(UInt64  timeParam);
		//�ж�ĳ��ʱ���Ƿ����賿4���Ų���ͬһ��
		static bool timeIsTodayDelayHours(UInt64 timeParam,UInt32 hours); 
		//�ж�ʱ���Ƿ��������ͬһ����
		static bool timeIsSameMonth(UInt32 time);
		//��ȡ��ǰ�Ǽ��� 
		static Int32 getMonth(Int32 time);
		
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__