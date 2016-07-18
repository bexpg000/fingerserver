#ifndef __LYNX_COMMON_LIB_ITEM_DATA_H__
#define __LYNX_COMMON_LIB_ITEM_DATA_H__

#include "Definition.h"

namespace Lynx
{
    

//==========================================================================================================
// BOC ItemData ���ݽṹ sec

	struct RandomAttribute
	{
		RandomAttribute():m_nKey(0),m_nValue(0),m_nQuality(0){}
		//װ�����е�������Ե����࣬�﹥�������ħ���ȵ�sec
		UInt32 m_nKey;
		//��Ӧ�������Ӧ����ֵ��ע�⣬����Ǳ����ʣ���ֵΪ��ֱ�
		UInt32 m_nValue;

		UInt32 m_nQuality;

		LYNX_S11N_3(RandomAttribute,UInt32,m_nKey,UInt32,m_nValue,UInt32, m_nQuality);
	};

	struct IntrinsicAttribute
	{
		IntrinsicAttribute():m_nKey(0),m_nValue(0){}

		//�������Զ�Ӧ������ID
		UInt32 m_nKey;
		//�������Ե���ֵ
		UInt32 m_nValue;
		LYNX_S11N_2(IntrinsicAttribute,UInt32,m_nKey,UInt32,m_nValue);
	};


	//һЩ���ߵ������ֶΣ���װ����ʼ�����ɵ�ʱ���������sec
	struct EquipAttribute
	{
		EquipAttribute():m_nEquipState(0),m_nEnhanceLevel(0),m_nEnhanceExp(0),
			m_nStarLevel(0),m_nStarExp(0),m_nRefineTimes(0),m_nPurplePoints(0),m_nPurpleTimes(0),m_nOrangePoints(0),m_nOrangeTimes(0){}

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
		List<IntrinsicAttribute> m_listIntrinsicAttrs;

		//ÿ��װ��Я������������б�
		List<RandomAttribute> m_listRandomAttrs;

		UInt32 m_nPurplePoints;

		UInt32 m_nPurpleTimes;

		UInt32 m_nOrangePoints;

		UInt32 m_nOrangeTimes;

		RandomAttribute m_nOldAttribute;

		UInt32 m_nOldAttributeIndex;

		LYNX_S11N_15(EquipAttribute, UInt32, m_nEquipState,UInt32,m_nEnhanceLevel,
			UInt64, m_nEnhanceExp, UInt32, m_nStarLevel,UInt64, m_nStarExp,
			UInt32, m_nRefineTimes,List<Guid>, m_listGems,List<IntrinsicAttribute>, m_listIntrinsicAttrs,
			List<RandomAttribute>, m_listRandomAttrs,UInt32, m_nPurplePoints,UInt32, m_nPurpleTimes,
			UInt32, m_nOrangePoints,UInt32, m_nOrangeTimes, RandomAttribute, m_nOldAttribute,
			UInt32, m_nOldAttributeIndex
			);
	};

	struct ItemData 
	{
	
		ItemData() : m_nUid(0), m_nItemId(0), m_nCurStackAmount(0),m_nItemGetTime(0),m_nDirtyType(0),m_nInvalid(0) {

		}

		// ȫ��Ψһ��uuid
		Guid m_nUid;

		//�����ڵ��߱��е�ID
		Guid m_nItemId;

		// ��ǰ��������
		UInt32 m_nCurStackAmount;

		// ��Ʒ��õ�ʱ��
		UInt32 m_nItemGetTime;

		//һ��װ�����ԵĽṹ��
		EquipAttribute m_itemAttribute;

		List<UInt32> m_listParam;

		UInt32 m_nDirtyType;

		UInt32 m_nInvalid;

		

		LYNX_S11N_8(ItemData, Guid, m_nUid, Guid ,m_nItemId,
			UInt32 ,m_nCurStackAmount,UInt32, m_nItemGetTime, EquipAttribute, m_itemAttribute, List<UInt32>, m_listParam,
			UInt32,m_nDirtyType,UInt32, m_nInvalid);	

	};

	struct IdAndCount
	{
		UInt64 mItemID;
		UInt32 mCount;

		LYNX_S11N_2(IdAndCount, UInt64, mItemID, UInt32 ,mCount);	

	};

	struct ItemsChangeEle
	{
		UInt64 mItemUid;
		UInt32 mCurStackAmount;
		OperationType mOption;
		
	};

	//������Ʒͨ��
	struct AllItemEle
	{
		AllItemEle():resType(0),  subType(0), count(0){}
		UInt64 resType;
		UInt64 subType;
		UInt32 count;

		LYNX_S11N_3(AllItemEle, UInt64, resType, UInt64 ,subType, UInt32, count);
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ITEM_DATA_H__