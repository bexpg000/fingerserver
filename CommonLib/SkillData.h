#ifndef __LYNX_COMMON_LIB_SKILL_DATA_H__
#define __LYNX_COMMON_LIB_SKILL_DATA_H__

#include "Definition.h"
// #include "../FireConfirm/Character.h"

namespace Lynx
{

	struct SkillData
	{
		SkillData():m_nID(0),m_nCD(0),m_nLevel(0),m_nEquipPos(0){}

		//���ܵ�ID��
		Guid   m_nID;
		//���ܵ�CD
		UInt32 m_nCD;
		//���ܵĵȼ�
		UInt32 m_nLevel;
		//����װ���Ĳ�λ 0 ��ʾûװ����1 ��ʾװ���ڲ�1,2��ʾװ���ڲ�2 ... 
		UInt32 m_nEquipPos;

		LYNX_S11N_4(SkillData, Guid, m_nID, UInt32, m_nCD, UInt32, m_nLevel,UInt32,m_nEquipPos);
	};
	
	struct SkillEquipData
	{
		SkillEquipData():m_nID(0),m_nEquipPos(0){}
		//���ܵ�id
		UInt64 m_nID;
		//���ܵ�λ��
		UInt32 m_nEquipPos;

		LYNX_S11N_2(SkillEquipData, UInt64, m_nID, UInt32, m_nEquipPos);
	};

	struct SkillEquip
	{
		UInt32 normal_attack ;
		UInt32 jump_attack;
		UInt32 up ;
		UInt32 down ;
// 		Pos current;
// 		Pos back;	
	};
	struct SkillEquipList
	{
// 		List<SkillEquip> m_SkillEquipList;
	};



} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SKILL_DATA_H__