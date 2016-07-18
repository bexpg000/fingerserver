#ifndef __LYNX_COMMON_LIB_SKILL_DATA_2_CIENT_H__
#define __LYNX_COMMON_LIB_SKILL_DATA_2_CIENT_H__

#include "Definition.h"

namespace Lynx
{
	struct SkillItem2Client
	{
		SkillItem2Client():m_nID(0),m_nCD(0),m_nLevel(0),m_nEquipPos(0){}

		//���ܵ�ID��
		Guid   m_nID;
		//���ܵ�CD
		UInt32 m_nCD;
		//���ܵĵȼ�
		UInt32 m_nLevel;
		//����װ���Ĳ�λ 0 ��ʾûװ����1 ��ʾװ���ڲ�1,2��ʾװ���ڲ�2 ... 
		UInt32 m_nEquipPos;
		
		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			std::stringstream mystream;
			mystream.clear();
			mystream.str("");
			std::string IDStr;
			mystream << m_nID;
			mystream >> IDStr;
			m_strSendMsg += IDStr + connectSymbols[num];

			mystream.clear();
			mystream.str("");
			std::string CDStr;
			mystream << m_nCD;
			mystream >> CDStr;
			m_strSendMsg += CDStr + connectSymbols[num];

			mystream.clear();
			mystream.str("");
			std::string LevelStr;
			mystream << m_nLevel;
			mystream >> LevelStr;
			m_strSendMsg += LevelStr + connectSymbols[num] ;
		
			mystream.clear();
			mystream.str("");
			std::string EquipPosStr;
			mystream << m_nEquipPos;
			mystream >> EquipPosStr;
			m_strSendMsg += EquipPosStr;
		
			return m_strSendMsg;
		}
		
		std::string m_strSendMsg;
	};
}

#endif // __LYNX_COMMON_LIB_SKILL_DATA_2_CIENT_H__

