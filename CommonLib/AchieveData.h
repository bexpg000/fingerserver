#ifndef __LYNX_COMMON_LIB_ACHIEVE_DATA_H__
#define __LYNX_COMMON_LIB_ACHIEVE_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct AchieveData
	{


		AchieveData() : m_nAchieveId(0), m_nEventId(0),  m_nArg(0), m_nFlag(0){}

		//�ɾ�id
		Guid m_nAchieveId;
		//�������¼�
		Guid m_nEventId;
		//����
		UInt32 m_nArg;		
		//�Ƿ���ȡ����&���
		UInt32 m_nFlag;

		LYNX_S11N_4(AchieveData, Guid, m_nAchieveId, Guid ,m_nEventId, UInt32, m_nArg, UInt32, m_nFlag);


	};

	//ÿ����������
	struct DailyTaskData
	{

		DailyTaskData() : m_nEventId(0), m_nTempId(0),  m_nArg(0), m_nFlag(0){}

	
		//ÿ������ģ��id
		Guid m_nTempId;
		//����
		UInt32 m_nArg;		
		//�Ƿ���ȡ����&���
		UInt32 m_nFlag;

		UInt64 m_nEventId;

		LYNX_S11N_4(DailyTaskData, UInt64 ,m_nEventId, Guid, m_nTempId, UInt32, m_nArg,UInt32, m_nFlag);


	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACHIEVE_DATA_H__