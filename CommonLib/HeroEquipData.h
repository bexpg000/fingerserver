#ifndef __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__
#define __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct HeroEquipData
	{

		HeroEquipData() : m_nEquipId(0), m_nEquipLv(0),m_nActiveBit(AllGemLock),m_nStep(0){}

		//װ��id
		UInt64 m_nEquipId;
			
		//װ���ı�ʯ��λ��û�л�����Ч����0��-1��ת�����޷���64λ������,װ����ʯ
		List<UInt64> m_nGemsList;
		
		//װ��lv
		UInt32 m_nEquipLv;
		
		//32λ����ʾ�Ƿ񼤻�
		UInt32 m_nActiveBit;

		UInt32 m_nStep;

		LYNX_S11N_5(HeroEquipData, UInt64, m_nEquipId, UInt32, m_nEquipLv, List<UInt64>, m_nGemsList, UInt32, m_nActiveBit,UInt32, m_nStep);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__