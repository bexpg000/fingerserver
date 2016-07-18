#ifndef __LYNX_COMMON_LIB_ITEM_DATA_2_CLIENT_H__
#define __LYNX_COMMON_LIB_ITEM_DATA_2_CLIENT_H__

#include "Definition.h"

namespace Lynx
{
	struct RandomAttr2Client
	{
		
		RandomAttr2Client():m_nKey(0),m_nValue(0){}
		//װ�����е�������Ե����࣬�﹥�������ħ���ȵ�sec
		UInt32 m_nKey;
		//��Ӧ�������Ӧ����ֵ
		UInt32 m_nValue;

		std::string m_strSendMsg;

		std::string constructString(int num = 0)
		{	
			m_strSendMsg.clear();
			std::stringstream mystream;
			mystream.clear();
			mystream.str("");
			
			mystream << m_nKey;
			std::string typeStr;
			mystream >> typeStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nValue;
			std::string randomAttrStr;
			mystream >> randomAttrStr;

			m_strSendMsg = typeStr + connectSymbols[num] + randomAttrStr;

			return m_strSendMsg;
		}

	};

	struct IntrinsicAttr2Client
	{
		IntrinsicAttr2Client():m_nKey(0),m_nValue(0){}
		//�������Զ�Ӧ������ID
		UInt32 m_nKey;
		//�������Ե���ֵ
		UInt32 m_nValue;
		std::string m_strSendMsg;
		std::string constructString(int num = 0)
		{
			std::stringstream mystream;
			
			mystream.clear();
			mystream.str("");
			mystream << m_nKey;
			std::string keyStr;
			mystream >> keyStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nValue;
			std::string valueStr;
			mystream >> valueStr;
			
			m_strSendMsg = keyStr + connectSymbols[num] + valueStr;
			
			return m_strSendMsg; 
		}
		
	};
	
	//һЩ���ߵ������ֶΣ���װ����ʼ�����ɵ�ʱ���������
	struct EquipAttribute2Client
	{
		EquipAttribute2Client():m_nEquipState(0),m_nEnhanceLevel(0),
			m_nEnhanceExp(0),m_nStarLevel(0),m_nStarExp(0),m_nRefineTimes(0){}
		//װ��״̬
		UInt32 m_nEquipState;

		//ǿ���ȼ�
		UInt32 m_nEnhanceLevel;

		//ǿ������
		UInt64 m_nEnhanceExp;  

		//�Ǽ�
		UInt32 m_nStarLevel;

		//�Ǽ�����
		UInt64 m_nStarExp;

		//ϴ������
		UInt32 m_nRefineTimes;

		//ÿ��װ������Ƕ�ı�ʯ��û����Ϊ0����������λ
		List<Guid> m_listGems;

		//ÿ��װ��Я���Ĺ��������б�
		List<IntrinsicAttr2Client> m_listIntrinsicAttrs;

		//ÿ��װ��Я������������б�
		List<RandomAttr2Client> m_listRandomAttrs;

		std::string constructString(int num = 0)
		{	
			m_strSendMsg.clear();
			std::stringstream mystream;
			
			mystream.clear();
			mystream.str("");
			mystream << m_nEquipState;
			std::string equipStateStr;
			mystream >> equipStateStr;
			

			mystream.clear();
			mystream.str("");
			mystream << m_nEnhanceLevel;
			std::string enhanceLevelStr;
			mystream >> enhanceLevelStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nEnhanceExp;
			std::string enhanceExpStr;
			mystream >> enhanceExpStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nStarLevel;
			std::string starLevelStr;
			mystream >> starLevelStr;
		
			mystream.clear();
			mystream.str("");
			mystream << m_nStarExp;
			std::string starExpStr;
			mystream >> starExpStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nRefineTimes;
			std::string refineStr;
			mystream >> refineStr;

			m_strSendMsg = equipStateStr + connectSymbols[num] + enhanceLevelStr + connectSymbols[num]
			+ enhanceExpStr + connectSymbols[num] + starLevelStr + connectSymbols[num]
			+ starExpStr + connectSymbols[num]+ refineStr ;

			for(List<IntrinsicAttr2Client>::Iter * iterIntrinsic = m_listIntrinsicAttrs.begin(); iterIntrinsic != NULL;
				iterIntrinsic = m_listIntrinsicAttrs.next(iterIntrinsic))
			{
				m_strSendMsg += connectSymbols[num] + iterIntrinsic->mValue.constructString(num+1);			
			}
			

			for(List<RandomAttr2Client>::Iter * iterRandom = m_listRandomAttrs.begin(); iterRandom != NULL; 
				iterRandom = m_listRandomAttrs.next(iterRandom))
			{
				m_strSendMsg += connectSymbols[num] + iterRandom->mValue.constructString(num+1);
			}

			return m_strSendMsg;
		}

		std::string m_strSendMsg;

	};

	


	struct ItemData2Client
	{
		
		ItemData2Client() : m_nUid(0), m_nItemId(0), m_nCurStackAmount(0), m_nItemGetTime(0) {}

		// ȫ��Ψһ��uuid
		Guid m_nUid;

		//�����ڵ��߱��е�ID
		Guid m_nItemId;

		// ��ǰ��������
		UInt32 m_nCurStackAmount;

		// ��Ʒ��õ�ʱ��
		UInt32 m_nItemGetTime;

		//һ��װ�����ԵĽṹ��
		EquipAttribute2Client m_itemAttribute;
		
		//�˸�����
		List<UInt32> m_listParam;
		
		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			std::stringstream mystream;
			mystream.clear();
			mystream.str("");

			mystream << m_nUid;
			std::string uidStr;
			mystream >> uidStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nItemId;
			std::string itemIdStr;
			mystream >> itemIdStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nCurStackAmount;
			std::string stackAmountStr;
			mystream >> stackAmountStr;

			mystream.clear();
			mystream.str("");
			mystream << m_nItemGetTime;
			std::string getTimeStr;
			mystream >> getTimeStr;

			m_strSendMsg = uidStr + connectSymbols[num] + itemIdStr + connectSymbols[num] + stackAmountStr + connectSymbols[num] + getTimeStr 
				+ connectSymbols[num] + m_itemAttribute.constructString(num+1);
			
			return m_strSendMsg;
		}

		std::string m_strSendMsg;

		LYNX_S11N_1(ItemData2Client,std::string, m_strSendMsg);	


	};

	struct PlayerItemListS2C 
	{
		PlayerItemListS2C(){}

		List<ItemData2Client> m_listItems;
		std::string m_strSendMsg;
		
		std::string constructString(int num = 0)
		{
			m_strSendMsg.clear();
			for(List<ItemData2Client>::Iter * it = m_listItems.begin(); it != NULL; it = m_listItems.next(it) )
			{
				if(m_listItems.size() == 1)
				{
					m_strSendMsg += it->mValue.constructString(num);
					return m_strSendMsg;
				}

				if(m_listItems.next(it) != NULL)
				{
					m_strSendMsg += it->mValue.constructString(num + 1) + connectSymbols[num]; 
				}
				else
				{
					m_strSendMsg += it->mValue.constructString(num+1);
				}
			}
			return m_strSendMsg;
		}

		
		LYNX_S11N_1(PlayerItemListS2C,std::string,m_strSendMsg);
	};
	
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ITEM_DATA_2_CLIENT_H__