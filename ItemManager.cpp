#include "ItemManager.h"
#include "Player.h"
#include "GameServer.h"
#include "EquipManager.h"
#include "Table.h"
using namespace Lynx;

ItemManager::ItemManager() : mPlayer(NULL), m_pListItemDatas(NULL),m_nEquipOccupies(0),m_nItemOccupies(0),m_nOtherOccupies(0)
{

}


ItemManager::~ItemManager()
{

}

bool 
ItemManager::initial(Player* player)
{
    //��ʯ����ҵ�item�ĳ�ʼ��
	if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
    m_pListItemDatas = &mPlayer->mPlayerData.mItemListData.m_listItems;
    
	for (List<ItemData>::Iter* it = m_pListItemDatas->begin();
        it != m_pListItemDatas->end();
        it = m_pListItemDatas->next(it))
    {
        //�������Ʒ�Ѿ���ɾ������ô���������Ʒ������ͳ����һ��
		if(it->mValue.m_nInvalid)
		{
			continue;
		}

		//map ����uid��Ӧ�� itemdata��ָ�� sec
		m_mapItemUuid.insert(it->mValue.m_nUid, &it->mValue);
		
		//ͨ��id���Ҷ�Ӧ��Map<Guid,List<Guid> > ������ָ��
		Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(it->mValue.m_nItemId);
		
		if(iterItemID2Uids)
		{
			//ȡ����Ӧ��uid List sec
			List<Guid> &uidlist = iterItemID2Uids->mValue;
			//��list ����Ҷ�Ӧ��uid sec
			List<Guid>::Iter *iterIdList = uidlist.find(it->mValue.m_nUid,uidlist.begin(),uidlist.end());
			
			//û�ҵ��Ͳ��뵽uidList��ҵ��Ͳ���������ֻ����һ��uid��¼���� sec
			if(!iterIdList)
			{
				uidlist.insertTail(it->mValue.m_nUid);
			}
		}
		else
		{
			//û�ҵ�itemID��Ӧ�� uid List,��ô����uid sec
			List<Guid> uidlist;
			uidlist.insertTail(it->mValue.m_nUid);
			m_mapItemId2Uids.insert(it->mValue.m_nItemId,uidlist);
		}

		//��������uid map ��id2uid map,������Ʒ�����ͽ����ŵ���ͬ���͵�map�
		//���㷢�͸��ͻ�����Ϣ
		UInt64 itemTableID = it->mValue.m_nItemId;
		
		ItemTemplate * itemTemplate = ITEM_TABLE().get(itemTableID);
		
		if(itemTemplate->mBagTab == 1)
		{
			//�ж��Ƿ���װ���������װ������װ����Ӧ��map
			if(itemTemplate->mTypeId == EQUIPTYPEID)
			{
				//��һ��ǩҳ��װ����Ӧ��map��Ϊ�˱��գ���һ��ǩҳ�¿��ܻ���װ����Ƭ֮���
				m_mapUidEquipType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
			}
			
			m_nEquipOccupies++;
		}
		else
		{
			if(itemTemplate->mBagTab == 2)
			{
				m_mapUidItemType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
				
				
				m_nItemOccupies++;
				
				
			}
			else
			{
				m_mapUidOtherType.insert(it->mValue.m_nUid, &it->mValue);
				assert(!it->mValue.m_nInvalid);
				m_nOtherOccupies++;
			}
		}
    }

    return true;
}

void 
ItemManager::release()
{
	//����� m_mapItemId2Uids��m_mapItemUuid�ṹ��iter sec
	
	m_mapItemUuid.clear();
	m_mapItemId2Uids.clear();
	m_pListItemDatas = NULL;
	mPlayer = NULL;

	m_mapUidEquipType.clear();
	m_mapUidItemType.clear();
	m_mapUidOtherType.clear();

	m_nEquipOccupies = 0;
	m_nItemOccupies = 0;
	m_nOtherOccupies = 0;
}


void 
ItemManager::destroyItem(Guid itemUid)
{
    delItem(itemUid, 0);
}

UInt32 
ItemManager::delItem(Guid itemUid, UInt32 count)
{    
	
	//����Ϸɾ������ sec �Ż�ɾ������

	Map<Guid, ItemData*>::Iter* it = m_mapItemUuid.find(itemUid);
	if (it->mValue->m_nUid == itemUid)
	{
		 // ����������ɾ������ sec
				
		 if (count && count < it->mValue->m_nCurStackAmount)
		 {
		     updateOneItem(it->mValue,it->mValue->m_nCurStackAmount - count);
								
		            return 1;
		 }

				//map��ͨ����Ӧ��uidɾ��itemdata�Ĺ�����ϵ sec
		 delOneItem(it->mValue);
		 return 1;
		        
	 }
	return 2;

}

void ItemManager::delOneItem(ItemData* itemData)
{
	itemData->m_nCurStackAmount = 0;
	m_mapItemUuid.erase(itemData->m_nUid);

	//ͨ��itemID���Ҷ�Ӧ��uid List sec
	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itemData->m_nItemId);
	assert(iterItemID2Uids);

	if(iterItemID2Uids)
	{
		//list �� ����ָ����uid sec
		List<Guid>& listItemUID = iterItemID2Uids->mValue;
		
		List<Guid>::Iter * iterUids = listItemUID.find(itemData->m_nUid,listItemUID.begin(),listItemUID.end());
		assert(iterUids);
		//�ҵ��󽫶�Ӧ��uid��list��ɾ�� sec
		listItemUID.erase(iterUids);
		if(listItemUID.empty())
		{
			m_mapItemId2Uids.erase(iterItemID2Uids);
		}
	}

	//listɾ��uid��Ӧ��itemData���� sec
	//m_pListItemDatas->erase(it); �Ѿ���Ϊ��־λ����Ƿ�ɾ����
	
	delTypeMapEle(itemData);

	itemData->m_nInvalid = 1;
	if(itemData->m_nDirtyType == NODIRTY)
	{
		itemData->m_nDirtyType = DELDIRTY;
		mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);
	}
	
}

void ItemManager::delTypeMapEle(ItemData* itemData)
{
	//ɾ����Ӧ�����ʹ洢map�е�Ԫ��

	ItemTemplate * itemTemplate = ITEM_TABLE().get(itemData->m_nItemId);

	if(itemTemplate->mBagTab == 1)
	{
		if(itemTemplate->mTypeId == EQUIPTYPEID)
		{
			m_mapUidEquipType.erase(itemData->m_nUid);
		}
		
		m_nEquipOccupies--;
	}
	else
	{
		if(itemTemplate->mBagTab == 2)
		{
			m_mapUidItemType.erase(itemData->m_nUid);
			m_nItemOccupies--;
		}
		else
		{
			m_mapUidOtherType.erase(itemData->m_nUid);
			m_nOtherOccupies--;
		}
	}
}

bool 
ItemManager::isItemFull(UInt32 bagTab)
{
   if(bagTab == 1)
   {
		return (MAXITEMCOUNT <= m_nEquipOccupies);
   }
   else if(bagTab == 2)
   {
		return(MAXITEMCOUNT <= m_nItemOccupies);
   }
   else
   {
		return(MAXITEMCOUNT <= m_nOtherOccupies);
   }

}

UInt32 ItemManager::getEmputyNum(UInt32 bagTab)
{
	if(bagTab == 1)
	{
		return (MAXITEMCOUNT - m_nEquipOccupies);
	}
	else if(bagTab == 2)
	{
		return(MAXITEMCOUNT - m_nItemOccupies);
	}
	else
	{
		return(MAXITEMCOUNT - m_nOtherOccupies);
	}
}

errorId 
ItemManager::useItem(const Guid& itemUid, UInt32 count)
{
	//sec BOC ʹ�ñ�����Ʒ
	Map<Guid, ItemData*>::Iter * iterMap = m_mapItemUuid.find(itemUid);

	if(!iterMap)
	{
		LOG_WARN("not find this item");
		return LynxErrno::ItemNotExist;
	}

	assert(iterMap);

	//�������Ʒ�Ѿ���ɾ���ˣ����ش�����
	if(iterMap->mValue->m_nInvalid)
	{
		return LynxErrno::ItemNotExist;
	}

	Guid itemID = iterMap->mValue->m_nItemId;

	ItemTemplate* itemTemplate = ITEM_TABLE().get(itemID); 

	if (!itemTemplate->mUseful)
	{
		LOG_WARN("Item can not use,templateid: %llu", itemID);
		return LynxErrno::InvalidParameter;
	}

	if (iterMap->mValue->m_nCurStackAmount < count)
	{
		return LynxErrno::InvalidParameter;
	}
	//����߼�Ч��
	switch(itemTemplate->mTypeId)
	{

	case EQUIPTYPEID:
		{

		}
		break;
	case COSTTYPEID:
		{

		}
		break;

	default:

		return LynxErrno::None;
	}

	delItem(itemUid, count);
	return LynxErrno::None;
}

UInt32 ItemManager::getNoneEmptyBags(UInt32 bagTab)
{
	if(bagTab == 1)
	{
		return  m_nEquipOccupies;
	}
	else if(bagTab == 2)
	{
		return m_nItemOccupies;
	}
	else
	{
		return  m_nOtherOccupies;
	}

	
}

void ItemManager::updateOneItem(ItemData* itemData, UInt32 StackAmount)
{
	itemData->m_nCurStackAmount = StackAmount;
	
	if(itemData->m_nDirtyType == NODIRTY)
	{
		itemData->m_nDirtyType = UPDATEDIRTY;
		mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);
	}
	
	
}


UInt64 ItemManager::addOneItem(Guid templateId, UInt32 stackAmount)
{
	UInt64 itemUid = LogicSystem::getSingleton().generateItemGuid();
	ItemData itemData;
	itemData.m_nUid = itemUid;
	itemData.m_nItemId = templateId;
	itemData.m_nItemGetTime = TimeUtil::getTimeSec();
	itemData.m_nCurStackAmount = stackAmount;
	for(unsigned int i = 0; i < PARAMSIZE; i++)
	{
		UInt64 ele = 0;
		itemData.m_listParam.insertTail(ele);
	}

	List<ItemData>::Iter *it = m_pListItemDatas->insertTail(itemData);
	//����Ʒuid��value ��Ӧ����map��
	m_mapItemUuid.insert(itemUid, &it->mValue);

	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(it->mValue.m_nItemId);

	if(iterItemID2Uids)
	{
		//ȡ����Ӧ��uid List sec	
		iterItemID2Uids->mValue.insertTail(it->mValue.m_nUid);		
	}
	else
	{
		//û�ҵ�itemID��Ӧ�� uid List,��ô����uid sec
		List<Guid> uidlist;
		uidlist.insertTail(it->mValue.m_nUid);
		m_mapItemId2Uids.insert(it->mValue.m_nItemId,uidlist);
	}

	addTypeMapEle(&it->mValue);

	//���dirty���
	it->mValue.m_nDirtyType = ADDDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);

	return itemUid;
}

void ItemManager::addTypeMapEle(ItemData* itemData)
{
	//����Ʒ�������ͣ������Ӧ������map��
	ItemTemplate * itemTemplate =  ITEM_TABLE().get(itemData->m_nItemId);

	if(itemTemplate->mBagTab == 1)
	{
		if(itemTemplate->mTypeId == EQUIPTYPEID)
		{
			m_mapUidEquipType.insert(itemData->m_nUid, itemData);
		}
		
		m_nEquipOccupies++;
		
		//�����װ�������װ�����Ե��������
		addEquipAttribute(itemData);
	}
	else
	{
		if(itemTemplate->mBagTab == 2)
		{
			m_mapUidItemType.insert(itemData->m_nUid, itemData);
			m_nItemOccupies++;
		}
		else
		{
			m_mapUidOtherType.insert(itemData->m_nUid, itemData);
			m_nOtherOccupies++;
		}
	}

}

UInt32 ItemManager::getRandomValue(UInt32 minValue, UInt32 maxValue)
{
	UInt32 diffValue = maxValue - minValue;

	if (diffValue ==0)//todo: 0 ��bug
	{
		return minValue;
	}

	UInt32 result = rand()%diffValue;

	return minValue + result + 1;
	
}

List<UInt64> ItemManager::addItems(Guid templateId, UInt32 count)
{
	List<UInt64> listUids;
	ItemTemplate* itemTemplate = ITEM_TABLE().get(templateId);
	//��Ʒ����������  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;
	
	//ȫ����ӵ������ռ��newbags��������

	while(count > maxStackAmount )
	{
		count -= maxStackAmount;
		UInt64 uid = addOneItem(templateId, maxStackAmount);
		listUids.insertTail(uid);
				
	}
			
	if(count > 0 )
	{
		UInt64 uid =addOneItem(templateId,count);
		listUids.insertTail(uid);
	}
	return listUids;	
}

//����ռ�ü���������
UInt32 ItemManager::calculateCount(Guid itemId, UInt32 count)
{
	ItemTemplate* itemTemplate = ITEM_TABLE().get(itemId);
	//��Ʒ����������  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;

	//��Ҫ��ռ�õı���������
	UInt32 newBags = 0;

	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itemId);

		//û�ҵ�itemid ��Ӧ��uuid�б�˵��������û�д�����Ʒ�������µ�itemdata,ռ��һ������������
	if(!iterItemID2Uids)
	{
		if(count%maxStackAmount)
		{
			newBags = count/maxStackAmount + 1;
		}
		else
		{
			newBags = count/maxStackAmount;
		}
	}
	else
	{
		//�ҵ�itemid��Ӧ��uuid�б�˵���������д�����Ʒ
		List<Guid> &uidList = iterItemID2Uids->mValue;
		for(List<Guid>::Iter * iterUids = uidList.begin(); iterUids != NULL; iterUids = uidList.next(iterUids))
		{
			Guid uid = iterUids->mValue;
	
			Map<Guid, ItemData*>::Iter *iterItemData =  m_mapItemUuid.find(uid);
			
			assert(!iterItemData->mValue->m_nInvalid);

			if(iterItemData->mValue->m_nInvalid)
			{
				continue;
			}
			
			UInt32 currStackAmount = iterItemData->mValue->m_nCurStackAmount;
			//�ҵ�δ�ѵ�����
			if(currStackAmount < maxStackAmount)
			{
				//���������˸���������������������
				if(count + currStackAmount > maxStackAmount)
				{
					count -=  (maxStackAmount - currStackAmount);
					break;
				}
				else 
				{					
					count = 0;
					break;
				}


			}

		}

		//��ʣ�����Ʒռ���µı�����	
		if(count > 0)
		{
			if(count%maxStackAmount)
			{
				newBags = count/maxStackAmount + 1;
			}
			else
			{
				newBags = count/maxStackAmount;
			}
		}

	}
	
	return newBags;
}

List<ItemData>* ItemManager::getItemDatasList(void)
{
	return m_pListItemDatas;
}

//��������ȡ���µ���Ʒ
 // ����Ϸ��ȡ��Ʒ,templateId Ϊitemid
List<UInt64> ItemManager::getNewItems(Guid itmeId, UInt32 count)
{
	//����
	List<UInt64> itemUidLists;

	ItemTemplate* itemTemplate = ITEM_TABLE().get(itmeId);
	//��Ʒ����������  sec
	UInt32 maxStackAmount = itemTemplate->mStackAmount;
	
	Map<Guid,List<Guid> >::Iter * iterItemID2Uids = m_mapItemId2Uids.find(itmeId);
	
	//û�ҵ�itemid ��Ӧ��uuid�б�˵��������û�д�����Ʒ�������µ�itemdata,ռ��һ������������
	if(!iterItemID2Uids)
	{
		itemUidLists = addItems(itmeId,count);
	}
	else
	{
		//�ҵ�itemid��Ӧ��uuid�б�˵���������д�����Ʒ
		List<Guid> &uidList = iterItemID2Uids->mValue;
		for(List<Guid>::Iter * iterUids = uidList.begin(); iterUids != NULL; iterUids = uidList.next(iterUids))
		{
			Guid uid = iterUids->mValue;
			Map<Guid, ItemData*>::Iter *iterItemData =  m_mapItemUuid.find(uid);
		
			//�������Ʒ�Ѿ���ɾ��
			assert(!iterItemData->mValue->m_nInvalid);
			if(iterItemData->mValue->m_nInvalid)
			{
				continue;
			}

			UInt32 currStackAmount = iterItemData->mValue->m_nCurStackAmount;
			//�ҵ�δ�ѵ�����
			if(currStackAmount < maxStackAmount)
			{
				//���������˸���������������������
				if(count + currStackAmount > maxStackAmount)
				{
					count -=  (maxStackAmount - currStackAmount);
					updateOneItem(iterItemData->mValue, maxStackAmount);
					itemUidLists.insertTail(iterItemData->mValue->m_nUid);
					break;
				}
				else 
				{
					currStackAmount += count;
					updateOneItem(iterItemData->mValue,currStackAmount);
					count = 0;
					itemUidLists.insertTail(iterItemData->mValue->m_nUid);
					break;
				}
				
				
			}
			
		}

		//��ʣ�����Ʒռ���µı�����	
		if(count > 0)
		{
			itemUidLists += addItems(itmeId,count);

		}
		
	}

	return itemUidLists;
}


List<UInt64> ItemManager::getNewItemsDefrent(List<IdAndCount> itemsList)
{
	UInt32 newItemOccupies = 0;
	UInt32 newEquipOccupies = 0;
	UInt32 newOtherOccupies = 0;

	List<UInt64> newItemUids;
	for(List<IdAndCount>::Iter * itemsIter = itemsList.begin(); itemsIter != NULL; itemsIter = itemsList.next(itemsIter))
	{
		UInt64 &itemID = itemsIter->mValue.mItemID;
		UInt32 &itemCount = itemsIter->mValue.mCount;
		ItemTemplate *itemTemplate = ITEM_TABLE().get(itemID);

		if (itemTemplate == NULL)
		{
			return newItemUids;
		}
		
		if(itemTemplate->mBagTab == 1)
		{
			newEquipOccupies += calculateCount(itemID,itemCount);
		}
		else if(itemTemplate->mBagTab == 2)
		{
			newItemOccupies += calculateCount(itemID,itemCount);
		}
		else
		{
			newOtherOccupies += calculateCount(itemID,itemCount);
		}

	}

	if(m_nEquipOccupies + newEquipOccupies > MAXITEMCOUNT || m_nItemOccupies + newItemOccupies > MAXITEMCOUNT 
		|| m_nOtherOccupies + newOtherOccupies > MAXITEMCOUNT)
	{
		//
		newItemUids.clear();
		return newItemUids;
	}
	else
	{
		for(List<IdAndCount>::Iter * itemsIter = itemsList.begin(); itemsIter != NULL; itemsIter = itemsList.next(itemsIter))
		{
			UInt64 &itemID = itemsIter->mValue.mItemID;
			UInt32 &itemCount = itemsIter->mValue.mCount;
			ItemTemplate *itemTemplate = ITEM_TABLE().get(itemID);
			
			if(itemTemplate == NULL)
			{
				LOG_WARN("itemid isn't in table %llu", itemID);
				continue;
			}

			newItemUids += getNewItems(itemID, itemCount);

		}

		return newItemUids;
	}
	
}

List<UInt64> ItemManager::getNewItemsSame(Guid templateId, UInt32 count)
{
	ItemTemplate *itemTemplate = ITEM_TABLE().get(templateId);
	
	UInt32 newItemOccupies = 0;
	UInt32 newEquipOccupies = 0;
	UInt32 newOtherOccupies = 0;

	List<UInt64> newItemUids;

	if (itemTemplate == NULL)
	{
		return newItemUids;
	}
	if(itemTemplate->mBagTab == 1)
	{
		newEquipOccupies += calculateCount(templateId,count);
	}
	else if(itemTemplate->mBagTab == 2)
	{
		newItemOccupies += calculateCount(templateId,count);
	}
	else
	{
		newOtherOccupies += calculateCount(templateId,count);
	}

	if(m_nEquipOccupies + newEquipOccupies > MAXITEMCOUNT || m_nItemOccupies + newItemOccupies > MAXITEMCOUNT 
		|| m_nOtherOccupies + newOtherOccupies > MAXITEMCOUNT)
	{
		//
		newItemUids.clear();
		return newItemUids;
	}
	else
	{
		newItemUids += getNewItems(templateId, count);
		
		return newItemUids;
	}
}


 ItemData* 
ItemManager::getItemByGuid(const Guid& itemGuid)
{
	Map<Guid, ItemData*>::Iter* findIter =  m_mapItemUuid.find(itemGuid);
	
	if(findIter)
	{
		return findIter->mValue;
	}
	else
	{
		return NULL;
	}
}



errorId
ItemManager::sellItem(const List<Guid>& itemList, UInt32& getGold)
{
	for (List<Guid>::Iter* it = itemList.begin();
	    it != NULL; it = itemList.next(it))
	{
	    Map<Guid, ItemData*>::Iter* existIter = m_mapItemUuid.find(it->mValue);
	    if (!existIter)
	    {
	        LOG_WARN("Item %llu not exit", it->mValue);
	        return LynxErrno::InvalidParameter;
	    }

	    ItemTemplate* itemTemplate = ITEM_TABLE().get(existIter->mValue->m_nItemId);
	    if (!itemTemplate)
	    {
	        LOG_WARN("Failed to get ItemTemplate by Id :%u", existIter->mValue->m_nItemId);
			return LynxErrno::ItemNotExist;
	    }

	    if (!itemTemplate->mSale)
	    {
	        return LynxErrno::InvalidParameter;
	    }
	}
	
	for (List<Guid>::Iter* it = itemList.begin();
	    it != NULL; it = itemList.next(it))
	{
	    Map<Guid, ItemData*>::Iter* existIter = m_mapItemUuid.find(it->mValue);
		assert(!existIter->mValue->m_nInvalid);
	    ItemTemplate* itemTemplate = ITEM_TABLE().get(existIter->mValue->m_nItemId);
	    //��Ǯ����
	    //mPlayer->increasePlayerGold(itemTemplate->mPrice * existIter->mValue->m_nCurStackAmount);
	    getGold += itemTemplate->mPrice * existIter->mValue->m_nCurStackAmount;
	    delItem(existIter->mValue->m_nUid);
	}
	
	mPlayer->mPlayerData.mBaseData.m_nCoin += getGold;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT);

	return LynxErrno::None;
}

std::string ItemManager::convertItemDataToJson()
{
	MsgItemDataToJson itemDataToJson;
	itemDataToJson.m_mapUidEquipType = m_mapUidEquipType;
	itemDataToJson.m_mapUidItemType = m_mapUidItemType;
	itemDataToJson.m_mapUidOtherType = m_mapUidOtherType;
	
	return itemDataToJson.convertDataToJson();

}

void ItemManager::addEquipAttribute(ItemData* itemData)
{
	//��������
	EquipTemplate * equipTemplate = EQUIP_TABLE().get(itemData->m_nItemId);

	UInt32 minValue1 = equipTemplate->mStaticAttrValMin1;
	UInt32 maxValue1 = equipTemplate->mStaticAttrValMax1;

	IntrinsicAttribute intrinsicAttr1;
	intrinsicAttr1.m_nKey = equipTemplate->mStaticAttrType1;
	UInt32 randomValue1 = getRandomValue(minValue1,maxValue1);
	intrinsicAttr1.m_nValue = randomValue1;

	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr1);


	UInt32 minValue2 = equipTemplate->mStaticAttrValMin2;
	UInt32 maxValue2 = equipTemplate->mStaticAttrValMax2;

	IntrinsicAttribute intrinsicAttr2;
	intrinsicAttr2.m_nKey = equipTemplate->mStaticAttrType2;
	UInt32 randomValue2 = getRandomValue(minValue2,maxValue2);
	intrinsicAttr2.m_nValue = randomValue2;

	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr2);


	UInt32 minValue3 = equipTemplate->mStaticAttrValMin3;
	UInt32 maxValue3 = equipTemplate->mStaticAttrValMax3;

	IntrinsicAttribute intrinsicAttr3;
	intrinsicAttr3.m_nKey = equipTemplate->mStaticAttrType3;
	UInt32 randomValue3 = getRandomValue(minValue3,maxValue3);
	intrinsicAttr3.m_nValue = randomValue3;
	itemData->m_itemAttribute.m_listIntrinsicAttrs.insertTail(intrinsicAttr3);

	if(equipTemplate->mRandomRuleId1)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId1);
	}

	if(equipTemplate->mRandomRuleId2)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId2);
	}

	if(equipTemplate->mRandomRuleId3)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId3);
	}

	if(equipTemplate->mRandomRuleId4)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId4);
	}

	if(equipTemplate->mRandomRuleId5)
	{
		addRandomAttribute(itemData,equipTemplate->mRandomRuleId5);
	}
}

void ItemManager::addRandomAttribute(ItemData* itemData, UInt32 randomID)
{
	AttrRandomTemplate* attrRandom = ATTRRANDOM_TABLE().get(randomID);

	//PA �ٷֱȵ����ֵ
	UInt32 PAPercent = attrRandom->mPARate;

	//MA �ٷֱȵ����ֵ���ж�ʱҪ�����������ֵ�Ƿ��� PA �� MA֮��
	UInt32 MAPercent = PAPercent + attrRandom->mMARate;

	UInt32 PFPercent = MAPercent + attrRandom->mPFRate;

	UInt32 MFPercent = PFPercent + attrRandom->mMFRate;

	UInt32 MaxHPPercent = MFPercent + attrRandom->mMaxHPRate;

	UInt32 MaxMPPercent = MaxHPPercent + attrRandom->mMaxMPRate;

	UInt32 MaxSPPercent = MaxMPPercent + attrRandom->mMaxSPRate;

	UInt32 CritRatePercent = MaxSPPercent + attrRandom->mCritRateRate;

	UInt32 CritDamagePercent = CritRatePercent + attrRandom->mCritDamageRate;

	UInt32 CritDefPercent =  CritDamagePercent + attrRandom->mCritDamageRate;

	UInt32 randomRes = rand()%100;

	RandomAttribute randomAttr;

	randomAttr.m_nQuality = attrRandom->mQuality;

	//������������ֵ���ڵ���critDamagePercent����ֵ��С��CritDefPercent����ֵ,��ô�������������ΪcritDef
	if(randomRes < CritDefPercent && randomRes >= CritDamagePercent)
	{
		randomAttr.m_nKey = BaseToughness;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDefMin,attrRandom->mCritDefMax);
		randomAttr.m_nValue = randomValue;
	}
	//����Ϊcritdamage
	else	if(randomRes >= CritRatePercent)
	{
		randomAttr.m_nKey = BaseCrit;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDamageMin,attrRandom->mCritDamageMax);
		randomAttr.m_nValue = randomValue;
	}
	//����maxSPPercent���ֵ��С��CritRate������Ϊcritrate
	else if(randomRes >= MaxSPPercent)
	{
		randomAttr.m_nKey = Critrate;
		UInt32 randomValue = getRandomValue(attrRandom->mCritRateMin,attrRandom->mCritRateMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MaxMPPercent)
	{
		//����ΪMAXsp
		randomAttr.m_nKey = MaxSP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxSPMin,attrRandom->mMaxSPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MaxHPPercent)
	{
		//����ΪMaxMP
		randomAttr.m_nKey = MaxMP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxMPMin, attrRandom->mMaxMPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MFPercent)
	{
		//����Ϊ���HP
		randomAttr.m_nKey = MaxHP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxHPMin, attrRandom->mMaxHPMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= PFPercent)
	{
		//����ΪMFPercent
		randomAttr.m_nKey = MF;
		UInt32 randomValue = getRandomValue(attrRandom->mMFMin,attrRandom->mMFMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= MAPercent)
	{
		//����ΪPFPercent
		randomAttr.m_nKey = PF;
		UInt32 randomValue = getRandomValue(attrRandom->mPFMin, attrRandom->mPFMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= PAPercent)
	{
		//����ΪMAPercent
		randomAttr.m_nKey = MA;
		UInt32 randomValue = getRandomValue(attrRandom->mMAMin,attrRandom->mMAMax);
		randomAttr.m_nValue = randomValue;
	}
	else if(randomRes >= 0)
	{
		//����ΪPAPercent
		randomAttr.m_nKey = PA;
		UInt32 randomValue = getRandomValue(attrRandom->mPAMin,attrRandom->mPAMax);
		randomAttr.m_nValue = randomValue;
	}
	else
	{
		assert(false);
	}

	itemData->m_itemAttribute.m_listRandomAttrs.insertTail(randomAttr);
}

UInt32 ItemManager::getEnhanceLevelLimit(UInt32 starLv)
{
	UInt32 lvLimit = 0;
	switch(starLv)
	{
	case 0:
		lvLimit = 10;
		break;
	case 1:
		lvLimit = 12;
		break;
	case 2:
		lvLimit = 14;
		break;
	case 3:
		lvLimit = 16;
		break;
	case 4:
		lvLimit = 18;
		break;
	case 5:
		lvLimit = 20;
		break;
	default:
		break;

	}
	return lvLimit;
}

UInt32 ItemManager::getTotalEnhanceLv(UInt32 starLv, UInt32 curEnhanceLv)
{
	UInt32 totalLv = 0;
	
	//ͳ�Ƶ�ǰ�Ǽ�֮ǰ�����еȼ��ۼӣ��統ǰ�Ǽ�Ϊ2��1������ôͳ��0�Ǻ�1��������Ӧ��ǿ���ȼ�
	for(UInt32 i = 0; i < starLv; i++)
	{
		totalLv += getEnhanceLevelLimit(i)+1;
	}

	//�ټ��ϵ�ǰ�Ǽ��е�ǿ���ȼ�
	totalLv += curEnhanceLv;
	
	return totalLv;
}

void ItemManager::enhanceEquip(UInt64 equipID, List<UInt64> materialLists)
{
	Map<Guid, ItemData*>::Iter * iterFind = m_mapItemUuid.find(equipID);
	
	if(!iterFind)
	{
		LOG_INFO("NO Equip IDs!");
		//��ʾ���װ��ʧЧ����Ϣ�Ժ�д
		return;
	}
		
	UInt32 enhanceLevel = iterFind->mValue->m_itemAttribute.m_nEnhanceLevel;
	UInt32 starLevel = iterFind->mValue->m_itemAttribute.m_nStarLevel;
	//Ҫǿ��װ�������ñ���Ϣ
	EquipTemplate * equipEnhance = EQUIP_TABLE().get(iterFind->mValue->m_nItemId);
	UInt32 starMax = equipEnhance->mMaxStar;
	UInt32 maxLvLimit = getEnhanceLevelLimit(starMax);
	if(starLevel == starMax && enhanceLevel == maxLvLimit)
	{
		LOG_INFO("yidadaozuidaqianghuadengji");
		//֪ͨ���
		return;
	}
	
	UInt64 enhanceExp = iterFind->mValue->m_itemAttribute.m_nEnhanceExp;
	//ͳ�Ʋ������еľ���
	UInt64 totalExp = 0;
	for(List<UInt64>::Iter * iterMaterial = materialLists.begin(); iterMaterial != NULL; iterMaterial = materialLists.next(iterMaterial))
	{
		Map<Guid, ItemData*>::Iter * materialFind = m_mapItemUuid.find(iterMaterial->mValue);
	
		if(!materialFind)
		{
			continue;
		}

		ItemTemplate * materialTemplate = ITEM_TABLE().get(materialFind->mValue->m_nItemId);

		//����ǿ��ʯ����ľ��黹û�и�������������дװ�����ϵĴ����߼�
		if(materialTemplate->mTypeId == EQUIPTYPEID)
		{
			EquipTemplate * equipTemplate = EQUIP_TABLE().get(materialFind->mValue->m_nItemId);
			totalExp += equipTemplate->mMaterialExp;
			delItem(iterMaterial->mValue,1);
		}
		else if(materialTemplate->mTypeId == ENHANCETYPEID)
		{
			
			totalExp += materialTemplate->mPower;
			delItem(iterMaterial->mValue,1);
		}
		
	}

	enhanceExp += totalExp;

	//UInt32 totalEnhanceLv = getTotalEnhanceLv(starLevel,enhanceLevel);

	AttrEnhanceTemplate* curEnhanceTemp = ATTRENHANCE_TABLE().reverseGet(starLevel,enhanceLevel);
	
	//�������õ�id�Ǵӵ�ǰ������������һ����������ԣ����Ե�ǰid +1�ҵ���Ӧ�ı���
	UInt64 nextID = curEnhanceTemp->mID + 1;
	AttrEnhanceTemplate * attrEnhanceTemp = ATTRENHANCE_TABLE().get(nextID);

	UInt64 enhanceExpLimit =attrEnhanceTemp ->mExp;

	UInt64 needCoin = attrEnhanceTemp ->mCostCoin;

	UInt64 playerCoin = mPlayer->getPlayerCoin();

	if(playerCoin < needCoin)
	{
		//��ʾ��Ҵ�����Ϣ����Ҳ���
		LOG_INFO("Not enouph gold!");
		return ;
	}
	else
	{
		playerCoin = playerCoin - needCoin;
		mPlayer->setPlayerCoin(playerCoin);
	}
	
	//ѭ���жϣ������Ʒ��ʱ��þ���ֵ���ܾ���ֵ����ǿ������ֵ���ޣ���ô������������
	while(enhanceExp >= enhanceExpLimit)
	{
		
			//��ȡ���Ǽ���ǿ���ȼ�����
			UInt32 enhanceLvLimit = getEnhanceLevelLimit(starLevel);
			
			if(enhanceLevel < enhanceLvLimit)
			{
				enhanceLevel++;
				nextID++;
				//�ܾ���ֵ�۳���������ֵ��
				enhanceExp = enhanceExp - enhanceExpLimit;
				//������һ����ľ���ֵ
				AttrEnhanceTemplate * attrEnhanceTemplate = ATTRENHANCE_TABLE().get(nextID);
				enhanceExpLimit = attrEnhanceTemplate->mExp;
			}
			else 
			{
				if(starLevel < equipEnhance->mMaxStar)
				{
					//�Ǽ��ȼ�����
					starLevel++;
					//�ܵ�ǿ���ȼ�����
					nextID++;
					//��ǰ�Ǽ��µĵȼ���0
					enhanceLevel = 0;

					//�ܾ���ֵ�۳���������ֵ��
					enhanceExp = enhanceExp - enhanceExpLimit;
					//������һ����ľ���ֵ
					AttrEnhanceTemplate * attrEnhanceTemplate = ATTRENHANCE_TABLE().get(nextID);
					enhanceExpLimit = attrEnhanceTemplate->mExp;
					

				}
				else
				{
					//��ʾ������ǣ������������������Ϣ�Ժ�д
					break;

				}
			}
	}

	//���ԼӳɾͲ������ˣ�ֻ�����Ǽ���ǿ���ȼ����Լ�ǿ������
	iterFind->mValue->m_itemAttribute.m_nStarLevel = starLevel;
	iterFind->mValue->m_itemAttribute.m_nEnhanceExp = enhanceExp;
	iterFind->mValue->m_itemAttribute.m_nEnhanceLevel = enhanceLevel;

	iterFind->mValue->m_nDirtyType = UPDATEDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT|BASEDATABIT);

	//֪ͨ���ǿ���ɹ����Լ�������ĸ��£���Ϣ�Ժ�д
	Json::Value root;
	root["errorId"] = 0;
	root["equipuid"] = iterFind->mValue->m_nUid;
	root["enhancelv"] = iterFind->mValue->m_itemAttribute.m_nEnhanceLevel;
	root["enhanceStar"] = iterFind->mValue->m_itemAttribute.m_nStarLevel;
	root["enhanceExp"] = iterFind->mValue->m_itemAttribute.m_nEnhanceExp;
	root["nowCoin"] = playerCoin;

	for(List<UInt64>::Iter * iterMaterial = materialLists.begin(); iterMaterial != NULL; iterMaterial = materialLists.next(iterMaterial))
	{
		Map<Guid, ItemData*>::Iter * materialFind = m_mapItemUuid.find(iterMaterial->mValue);

		if(!materialFind)
		{
			Json::Value itemEle;
			itemEle["uid"] = iterMaterial->mValue;
			itemEle["count"] = 0;
			
			root["itemList"].append(itemEle);
		}
		else
		{
			Json::Value itemEle;
			itemEle["uid"] = iterMaterial->mValue;
			itemEle["count"] = materialFind->mValue->m_nCurStackAmount;
			
			root["itemList"].append(itemEle);
		}

	}

	Json::FastWriter writer;
	std::string respJson = writer.write(root);

	EquipEhanceResp enhanceResp;
	enhanceResp.mPacketID = EQUIP_ENHANCE_RESP;
	enhanceResp.mRespJsonStr = respJson;
	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);


}

void  ItemManager::polishEquip(UInt64 objEquipUid, UInt64 materialEquipUid, UInt32 objRdAttrIndex)
{
	
	
	Map<Guid, ItemData*>::Iter * objEquipIter = m_mapItemUuid.find(objEquipUid);
	Map<Guid, ItemData*>::Iter * materialEquipIter = m_mapItemUuid.find(materialEquipUid);

	if(!objEquipUid || !materialEquipIter)
	{
		LOG_WARN("error uid ");

		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		

		return;
	}

	ItemData * objectItemData = objEquipIter->mValue;
	ItemData * materialItemData = materialEquipIter->mValue;

	EquipTemplate * materialTemplate = EQUIP_TABLE().get(materialEquipIter->mValue->m_nItemId);

	EquipTemplate * objectTemplate = EQUIP_TABLE().get(objEquipIter->mValue->m_nItemId);

	if(objectTemplate->mRefreshcoin > mPlayer->mPlayerData.mBaseData.m_nCoin)
	{
		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		return;
	}

	Map<Guid,List<Guid> >::Iter * polishStoneListIter = m_mapItemId2Uids.find(PolishStoneID);
	ItemTemplate * polishStoneTemplate = ITEM_TABLE().get(PolishStoneID);
	

	UInt32 totalCountPolishStone = 0;
	UInt32 notFullUid = 0;
	//��ʶ�Ѿ��ѵ�����ϴ��ʯuid
	List<UInt64> listPolishStoneUids;

	//����������ѵ�����ϴ��ʯuid List�������ҵ�δ�ѵ�����uid
	for(List<Guid>::Iter * polishStoneIter = polishStoneListIter->mValue.begin(); polishStoneIter != NULL; 
		polishStoneIter = polishStoneListIter->mValue.next(polishStoneIter))
	{
		Map<Guid, ItemData*>::Iter * polishDataIter =  m_mapItemUuid.find(polishStoneIter->mValue);
		totalCountPolishStone += polishDataIter->mValue->m_nCurStackAmount;

		if(polishDataIter->mValue->m_nCurStackAmount < polishStoneTemplate->mStackAmount)
		{
			notFullUid = polishStoneIter->mValue;
		}
		else
		{
			listPolishStoneUids.insertTail(polishStoneIter->mValue);
		}
	}

	//ϴ��ʯ������������ʾ���
	if(totalCountPolishStone < objectTemplate->mRefreshstone)
	{
		EquipPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::MaterialNotEnough;
		root["equipuid"] = objEquipUid;
		root["newAttr"]["type"] = 0;
		root["newAttr"]["value"] = 0;
		root["newAttr"]["quality"] = 0;

		root["oldAttr"]["type"] = 0;
		root["oldAttr"]["value"] = 0;
		root["oldAttr"]["quality"] = 0;
		root["objAttrIndex"] = objRdAttrIndex;
		root["materialAttrIndex"] = 0;

		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
		return;
	}

	//Ŀǰ�Ȳ����ǽ�����ĺͲ���ϴ��ʯ����
	//�Ժ����ϴ��ʯ�ͽ�������߼�(�������)

	List<polishStoneEle> polishStoneLists;

	UInt32 needPolishStoneCount = objectTemplate->mRefreshstone;
	if(notFullUid)
	{
		//ɾ��δ�ѵ����ģ����Ҹ�������ϴ��ʯ����
		Map<Guid, ItemData*>::Iter * itemDataIter = m_mapItemUuid.find(notFullUid);
		
		//needPolishStoneCount С�ڵ�ǰ�������������Ҳ�Ϊ0
		if(needPolishStoneCount < itemDataIter->mValue->m_nCurStackAmount )
		{
			if(needPolishStoneCount)
			{
				delItem(notFullUid, needPolishStoneCount);

				polishStoneEle stonesEle;
				stonesEle.count = itemDataIter->mValue->m_nCurStackAmount;
				stonesEle.uid = notFullUid;
				polishStoneLists.insertTail(stonesEle);
			}
					
		}
		else
		{
			//������Ҫ��ϴ��ʯ�������ڱ�����δ�ѵ������Ǹ�������ϴ��ʯ������������ɾ��������ӣ����Ҹ���ϴ��ʯ����
			needPolishStoneCount -= itemDataIter->mValue->m_nCurStackAmount;
			delItem(notFullUid);

			polishStoneEle stonesEle;
			stonesEle.count = 0;
			stonesEle.uid = notFullUid;
            polishStoneLists.insertTail(stonesEle);

			//����ѵ����ĸ���
			for(List<UInt64>::Iter * stoneUidIter = listPolishStoneUids.begin(); stoneUidIter != NULL; 
				stoneUidIter = listPolishStoneUids.next(stoneUidIter))
			{
				if(needPolishStoneCount == 0)
				{
					break;
				}

				if(needPolishStoneCount > polishStoneTemplate->mStackAmount)
				{
					//ɾ����Щ�ѵ�����
					needPolishStoneCount -= polishStoneTemplate->mStackAmount;
					delItem(stoneUidIter->mValue);

					polishStoneEle stonesEle;
					stonesEle.count = 0;
					stonesEle.uid = stoneUidIter->mValue;
					polishStoneLists.insertTail(stonesEle);

				}
				else
				{
					//���һ��������
					delItem(stoneUidIter->mValue,needPolishStoneCount);
					//������ȫ��ɾ�����ѵ�����-��Ҫ�ģ���ʣ���������id����list��

					polishStoneEle stonesEle;
					stonesEle.count = polishStoneTemplate->mStackAmount-needPolishStoneCount;
					stonesEle.uid = stoneUidIter->mValue;
					polishStoneLists.insertTail(stonesEle);

					break;
				}
			}
		}
		
	}
	else
	{
		//�����ϴ��ʯ���ѵ�����

		for(List<UInt64>::Iter * stoneUidIter = listPolishStoneUids.begin(); stoneUidIter != NULL; 
			stoneUidIter = listPolishStoneUids.next(stoneUidIter))
		{
			if(needPolishStoneCount == 0)
			{
				break;
			}
			
			if(needPolishStoneCount > polishStoneTemplate->mStackAmount)
			{
				needPolishStoneCount -= polishStoneTemplate->mStackAmount;
				delItem(stoneUidIter->mValue);

				polishStoneEle stonesEle;
				stonesEle.count = 0;
				stonesEle.uid = stoneUidIter->mValue;

				polishStoneLists.insertTail(stonesEle);
			}
			else
			{
				delItem(stoneUidIter->mValue,needPolishStoneCount);
				
				polishStoneEle stonesEle;
				stonesEle.count = polishStoneTemplate->mStackAmount-needPolishStoneCount;
				stonesEle.uid = stoneUidIter->mValue;

				polishStoneLists.insertTail(stonesEle);
				break;
			}
		}
	}

	//���Ľ�Ǯ
	mPlayer->mPlayerData.mBaseData.m_nCoin -= objectTemplate->mRefreshcoin ;

	mPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	AttrPolishTemplate *materialPolishTem = ATTRPOLISH_TABLE().get(materialTemplate->mLevelRequire);

	RandomAttribute *materialRdFind = NULL;

	//�Ƿ��г�ɫ����
	bool orangeFlag = false;
	//�ڲ���װ���еĳ�ɫ����,������ʶ�Ӧ��������ɵ�map,��Ҫ����Ŀ��װ���г�ɫϴ���������ڵ���1��
	//���Ҳ���װ�����г�ɫ���Ե����
	Map<UInt32, RandomAttribute*> orangeRdMap;
	UInt32 orangeRate = 0;

	bool purpleFlag = false;
	//�ڲ���װ���е���ɫ���ԣ�������ʶ�Ӧ��������ɵ�map����Ҫ����Ŀ��װ������ɫϴ���������ڵ���1��
	//���Ҳ���װ��������ɫ���Ե����
	Map<UInt32, RandomAttribute*> purpleRdMap;
	UInt32 purpleRate = 0;

	//����װ����������ɫ�����ԣ�������ʶ�Ӧ��������ɵ�map����Ҫ����Ŀ��װ������ɫ�ͳ�ɫ���۴���Ϊ0��
	//���߲���װ����û����ɫ���ɫ���Եĸ���ͳ�����
	Map<UInt32, RandomAttribute*> totalRdMap;
	UInt32 totalRate = 0;
	
	Vector<RandomAttribute*> rdAttrVecs; 
	creatQualityMap(materialItemData,materialPolishTem,orangeFlag,orangeRdMap,orangeRate, purpleFlag,purpleRdMap,purpleRate,totalRdMap,totalRate);

	//���жϳ�ɫ������г�ɫϴ�����۵Ĵ���
	if(objectItemData->m_itemAttribute.m_nOrangeTimes >= 1)
	{
		
		//��ֹ�߻����ģ�����ͬ��ɫ�߼��ֿ�
		if(orangeFlag)
		{
			orangeSuccess(orangeRdMap,objectItemData,materialRdFind,orangeRate);
		}
		else
		{
			//�������װ��û�г�ɫ���ԣ��ж�Ŀ��װ������ɫϴ���ۼƴ����Ƿ�Ϊ���ڵ���1
			if(objectItemData->m_itemAttribute.m_nPurpleTimes >= 1)
			{
				//Ŀ��װ������ɫϴ�����۴����ﵽ���ڵ���һ,�жϲ���װ�����Ƿ�����ɫ����
				if(purpleFlag)
				{
					//�������װ������ɫ����
					purpleSuccess(purpleRdMap,objectItemData,materialRdFind,purpleRate);
				}
				else
				{
					//�������װ����û����ɫ���ԣ���ô����������map�����
					totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
				}
			}
			else
			{
				//���Ŀ��װ������ɫϴ������Ϊ0����ɫ����>= 1,���Ҳ���װ��û�г�ɫ���ԣ���ô��Ҫ��ȡ����װ��ȫ�����Ե�����߼�
				totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);

			}
		
		}
	}
	else
	{
		//Ŀ��װ���ĳ�ɫϴ������Ϊ0���ж�Ŀ��װ����ɫ���۴����Ƿ���ڵ���1
		if(objectItemData->m_itemAttribute.m_nPurpleTimes >= 1)
		{
			//Ŀ��װ������ɫϴ�����۴����ﵽ���ڵ���һ,�жϲ���װ�����Ƿ�����ɫ����
			if(purpleFlag)
			{
				//�������װ������ɫ����
				purpleSuccess(purpleRdMap,objectItemData,materialRdFind,purpleRate);
			}
			else
			{
				//�������װ����û����ɫ���ԣ���ô����������map�����
				totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
			}
		}
		else
		{
			//Ŀ��װ����û����ɫϴ�����۴�����Ҳû�г�ɫϴ�����۴�������ô���������������ѡȡ
			totalAttrRandom(totalRdMap,objectItemData,materialRdFind,totalRate);
		}
	}

	int a = 5;
	assert(materialRdFind);
	List<RandomAttribute>::Iter * objRdIter = objectItemData->m_itemAttribute.m_listRandomAttrs.getn(objRdAttrIndex);

	PolishPointsTemplate * findPolishPointsTemp = NULL;
	for(PolishPointsTemplateMap::Iter * polishPointsMapIter = POLISHPOINTS_TABLE().mMap.begin(); 
		polishPointsMapIter != NULL;
		polishPointsMapIter = POLISHPOINTS_TABLE().mMap.next(polishPointsMapIter))
	{
		if(mPlayer->mPlayerData.mBaseData.m_nLevel < polishPointsMapIter->mValue.mLevel)
		{
			break;
		}

		findPolishPointsTemp = &polishPointsMapIter->mValue;
	}

	//�����Ҹ��ڻ��ߵ���40������ô����ϴ������
	if(findPolishPointsTemp)
	{
		//����ϴ�����֣����ݲ���װ������ɫƷ�ʽ�������
		switch(materialTemplate->mQuality)
		{

		case Blue:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mBluePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mBluePoints;
			}
			break;

		case Purple:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mPurplePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mPurplePoints;
			}
			break;

		case Orange:
			{
				MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialTemplate->mLevelRequire);
				objectItemData->m_itemAttribute.m_nOrangePoints+= materialPointsTem->mOrangePoints;
				objectItemData->m_itemAttribute.m_nPurplePoints+= materialPointsTem->mOrangePoints;
			}
			break;

		default:
			break;

		}


		//����ͳ��Ŀ��װ����ǿ�����ֺͳ�ɫ����ɫ���ۼƴ���

		
		//�����ɫ���Ե��ۼƴ���
		while(objectItemData->m_itemAttribute.m_nOrangePoints>=findPolishPointsTemp->mOrangePoints)
		{
			objectItemData->m_itemAttribute.m_nOrangePoints -= findPolishPointsTemp->mOrangePoints;
			objectItemData->m_itemAttribute.m_nOrangeTimes++;
		}

		//������ɫ���Ե��ۼƴ���
		while(objectItemData->m_itemAttribute.m_nPurplePoints >= findPolishPointsTemp->mPurplePoints)
		{
			objectItemData->m_itemAttribute.m_nPurplePoints -= findPolishPointsTemp->mPurplePoints;
			objectItemData->m_itemAttribute.m_nPurpleTimes++;
		}

		if(objectItemData->m_itemAttribute.m_nOrangeTimes>3)
		{
			objectItemData->m_itemAttribute.m_nOrangeTimes = 3;
		}

		if(objectItemData->m_itemAttribute.m_nPurpleTimes>3)
		{
			objectItemData->m_itemAttribute.m_nPurpleTimes = 3;
		}

	}
	
	

	//��Ŀ��װ����ԭ�����Ա�������ʱ��������ڷ����ͻ���,���ڸ�Ϊ������װ���ֶ���

	objectItemData ->m_itemAttribute.m_nOldAttribute = objRdIter->mValue;
	objectItemData->m_itemAttribute.m_nOldAttributeIndex = objRdAttrIndex;

	//ϴ���������Ժ������¼����µ�������Ŀ��װ���е�Ʒ��
	
	UInt32 attrQualityNew = getAttrQuality(*materialRdFind,objectTemplate);
	//�����滻���������ʱ���Ʒ������Բ���װ���ģ��������и���Ϊ���Ŀ��װ����
	objRdIter->mValue = *materialRdFind;
	//������õ���Ʒ�ʸ�ֵ��Ŀ��װ��
	objRdIter->mValue.m_nQuality = attrQualityNew;
	
	//���̱��
	objectItemData->m_nDirtyType = UPDATEDIRTY;
	
	//�����ҵ�������������Ե���������0��ʼ
	UInt32 materialAttrIndexFind = 0;
	List<RandomAttribute>& materialRdAttrList =  materialItemData->m_itemAttribute.m_listRandomAttrs;
	for(List<RandomAttribute>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.m_nKey == materialRdFind->m_nKey && materialRdIter->mValue.m_nValue ==  materialRdFind->m_nValue &&
			materialRdIter->mValue.m_nQuality == materialRdFind->m_nQuality)
		{
			break;
		}
		materialAttrIndexFind++;
	}

	delItem(materialEquipUid,1);

	//������Ϣ���ͻ���
	EquipPolishResp polishResp;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["equipuid"] = objEquipUid;
	root["newAttr"]["type"] = objRdIter->mValue.m_nKey;
	root["newAttr"]["value"] = objRdIter->mValue.m_nValue;
	root["newAttr"]["quality"] = objRdIter->mValue.m_nQuality;

	root["oldAttr"]["type"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nKey;
	root["oldAttr"]["value"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nValue;
	root["oldAttr"]["quality"] = objectItemData ->m_itemAttribute.m_nOldAttribute.m_nQuality;
	root["objAttrIndex"] = objRdAttrIndex;
	root["materialAttrIndex"] = materialAttrIndexFind;

	root["materialEquipid"] = materialEquipUid;
	
	for(List<polishStoneEle>::Iter * stoneEleIter = polishStoneLists.begin(); stoneEleIter!=NULL; stoneEleIter = polishStoneLists.next(stoneEleIter))
	{
		Json::Value stoneEle;
		stoneEle["uid"] = stoneEleIter->mValue.uid;
		stoneEle["count"] = stoneEleIter->mValue.count;

		root["polishStoneList"].append(stoneEle);
	}

	Json::FastWriter writer;
	std::string respJson = writer.write(root);

	polishResp.mRespJsonStr = respJson;
	polishResp.mPacketID = EQUIP_POLISH_RESP;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(polishResp,connId);
}

UInt32 ItemManager::getAttrQuality(const RandomAttribute &randomAttr, EquipTemplate * objectTemplate)
{
	//ͨ��equip���ҵ�װ����Ӧ�ĵȼ�

	UInt32 level = objectTemplate->mLevelRequire;


	if(randomAttr.m_nKey == PA)
	{
		return getPAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MA)
	{
		return getMAQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == PF)
	{
		return getPFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MF)
	{
		return getMFQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxHP)
	{
		return getMaxHPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxMP)
	{
		return getMaxMPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == MaxSP)
	{
		return getMaxSPQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == Critrate)
	{
		return getCritRateQuality(level,randomAttr);
	}
	else if(randomAttr.m_nKey == BaseCrit)
	{
		return getCirtDamageQuality(level, randomAttr);
	}
	else if(randomAttr.m_nKey == BaseToughness)
	{
		//����
		return getBaseToughnessQuality(level, randomAttr);

	}
	else
	{
		return 0;
	}
}


UInt32 ItemManager::getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mPAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mPAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mPAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mPAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr )
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMAMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMAMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMAMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMAMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mPFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mPFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mPFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mPFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMFMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMFMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMFMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMFMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxHPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxHPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxHPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxHPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxMPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxMPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxMPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxMPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mMaxSPMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mMaxSPMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mMaxSPMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mMaxSPMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritRateMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritRateMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritRateMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritRateMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}

UInt32 ItemManager::getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDamageMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDamageMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDamageMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDamageMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}


UInt32 ItemManager::getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr)
{
	UInt32 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,White);
	UInt32 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Green);
	UInt32 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Blue);
	UInt32 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Purple);
	UInt32 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(equipLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.m_nValue <= whiteTemplate->mCritDefMax)
	{
		return White;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.m_nValue <= greenTemplate->mCritDefMax)
		{
			return Green;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.m_nValue <= blueTemplate->mCritDefMax)
			{
				return Blue;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.m_nValue <= purpleTemplate->mCritDefMax)
				{
					return Purple;
				}
				else
				{
					return Orange;
				}
			}
		}
	}
}



void  ItemManager::polishResetEquip(UInt64 objEquipUid)
{
	Map<Guid, ItemData*>::Iter * objEquipIter = m_mapItemUuid.find(objEquipUid);
	if(!objEquipUid)
	{
		LOG_INFO("equip not find polishResetEquip!!");

		Json::Value root;
		root["errorId"] = LynxErrno::ItemNotExist;
		root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

		root["oldAttr"]["type"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nKey;
		root["oldAttr"]["value"] =  objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nValue;
		root["oldAttr"]["quality"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nQuality;

		Json::FastWriter writer;

		EquipPolishResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}

	if(mPlayer->mPlayerData.mBaseData.m_nCoin < PolishNeedCoin)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

		root["oldAttr"]["type"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nKey;
		root["oldAttr"]["value"] =  objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nValue;
		root["oldAttr"]["quality"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute.m_nQuality;

		Json::FastWriter writer;

		EquipPolishResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}
	

	UInt32 & rdIndex = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;
	RandomAttribute & rdAttribute = objEquipIter->mValue->m_itemAttribute.m_nOldAttribute;

	List<RandomAttribute>::Iter * rdAttrIter = objEquipIter->mValue->m_itemAttribute.m_listRandomAttrs.getn(rdIndex);
	assert(rdAttrIter);
	rdAttrIter->mValue = rdAttribute;
	
	objEquipIter->mValue->m_nDirtyType = UPDATEDIRTY;
	mPlayer->getPersistManager().setDirtyBit(ITEMDATABIT|BASEDATABIT);
	mPlayer->mPlayerData.mBaseData.m_nCoin -= PolishNeedCoin;
	
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objRdAttrIndex"] = objEquipIter->mValue->m_itemAttribute.m_nOldAttributeIndex;

	root["oldAttr"]["type"] = rdAttrIter->mValue.m_nKey;
	root["oldAttr"]["value"] =  rdAttrIter->mValue.m_nValue;
	root["oldAttr"]["quality"] = rdAttrIter->mValue.m_nQuality;
    root["equipUid"] = objEquipUid;

	Json::FastWriter writer;
	
	EquipPolishResetResp resetResp;
	resetResp.mRespJsonStr = writer.write(root);
	resetResp.mPacketID = EQUIP_POLISH_RESET_RESP;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resetResp,connId);
}

void ItemManager::creatQualityMap(ItemData * materialItemData,AttrPolishTemplate *materialPolishTem,bool& orangeFlag,
								  Map<UInt32, RandomAttribute*> &orangeRdMap,
								 UInt32& orangeRate, bool& purpleFlag,Map<UInt32, RandomAttribute*> &purpleRdMap,
								 UInt32& purpleRate,
								 Map<UInt32, RandomAttribute*> &totalRdMap,UInt32& totalRate)
{
	List<RandomAttribute>& materialRdAttrList =  materialItemData->m_itemAttribute.m_listRandomAttrs;


	for(List<RandomAttribute>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.m_nKey == 0)
		{
			continue;
		}
		
		if(materialRdIter->mValue.m_nQuality == Orange)
		{
			orangeFlag = true;

			//��ɫ���ֵ�ֶ�����
			orangeRate += materialPolishTem->mOrange;
			//ȫ�������ֵ�ֶ�����
			totalRate += materialPolishTem->mOrange;
			//��ӳ�ɫ,key Ϊ��ǰ������������ֵ����
			orangeRdMap.insert(orangeRate,&materialRdIter->mValue);
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else if(materialRdIter->mValue.m_nQuality == Purple)
		{
			purpleFlag = true;
			//��ɫ���ֵ�ֶ�����
			purpleRate += materialPolishTem->mPurple;
			//ȫ���͵����ֵ�ֶ�����
			totalRate += materialPolishTem->mPurple;
			//�����ɫ
			purpleRdMap.insert(purpleRate,&materialRdIter->mValue);
			//����������Ĳ���map
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else
		{
			//���������Է������������map��
			if(materialRdIter->mValue.m_nQuality == White)
			{
				totalRate += materialPolishTem->mWhite;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(materialRdIter->mValue.m_nQuality == Green)
			{
				totalRate += materialPolishTem->mGreen;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(materialRdIter->mValue.m_nQuality == Blue)
			{
				totalRate += materialPolishTem->mBlue;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
		}

	}

	if(totalRdMap.empty())
	{
		Json::Value root;
		root["errorId"] = LynxErrno::MaterialRdEmpty;
		
		EquipPolishResp polishResp;
		Json::FastWriter writer;
		std::string respJson = writer.write(root);

		polishResp.mRespJsonStr = respJson;
		polishResp.mPacketID = EQUIP_POLISH_RESP;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(polishResp,connId);
	}
}

void ItemManager::orangeSuccess(Map<UInt32, RandomAttribute*> &orangeRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &orangeRate)
{
	//orangrate��Ҫ���Ǵ������װ�����ж�����ɫ���Ե�Ȩ�أ������һ��ֵ
	UInt32 orangeValue = rand()%orangeRate;
	UInt32 orangeBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter* orangeRdIter =  orangeRdMap.begin(); orangeRdIter != NULL; 
		orangeRdIter = orangeRdMap.next(orangeRdIter))
	{
		
		//������ֵ��ĳ����ɫ���Ե��ٽ�ֵ���ôȡ���������ԣ�ֱ������������һ�����ҵ�һ������������
		if(orangeValue < orangeRdIter->mKey)
		{
			materialRdFind = orangeRdIter->mValue;
			//���ĵ���ɫһ��ϴ�����۴���
			objectItemData->m_itemAttribute.m_nOrangeTimes--;
			break;
		}

	}

	assert(materialRdFind);
}

void ItemManager::purpleSuccess(Map<UInt32, RandomAttribute*> &purpleRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &purpleRate)
{
	UInt32 purpleValue = rand()%purpleRate;
	UInt32 purpleBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter* purpleRdIter =  purpleRdMap.begin(); purpleRdIter != NULL; 
		purpleRdIter = purpleRdMap.next(purpleRdIter))
	{

		//������ֵ��ĳ����ɫ���Ե��ٽ�ֵ���ôȡ���������ԣ�ֱ������������һ�����ҵ�һ������������
		if(purpleValue < purpleRdIter->mKey)
		{
			materialRdFind = purpleRdIter->mValue;
			//���ĵ�һ����ɫϴ�����۴���
			objectItemData->m_itemAttribute.m_nPurpleTimes--;
			break;
		}

	}
	//����һ�����ҵ��������
	assert(materialRdFind);
}

void ItemManager::totalAttrRandom(Map<UInt32, RandomAttribute*> &totalRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &totalRate)
{
	UInt32 totalValue = rand()%totalRate;
	UInt32 totalBase = 0;
	for(Map<UInt32, RandomAttribute*>::Iter * totalRdIter  = totalRdMap.begin(); totalRdIter != NULL;
		totalRdIter = totalRdMap.next(totalRdIter))
	{
		
		//������ֵ��ĳ�����Ե��ٽ�ֵ���ôȡ���������ԣ�ֱ������������һ�����ҵ��������
		if(totalValue < totalRdIter->mKey)
		{
			materialRdFind = totalRdIter->mValue;
			break;
		}
	}

	assert(materialRdFind);
}

void ItemManager::gemCombine(UInt64 gem1Uid, UInt64 gem2Uid)
{
	Map<Guid, ItemData*>::Iter * gem1Iter = m_mapItemUuid.find(gem1Uid);
	Map<Guid, ItemData*>::Iter * gem2Iter =m_mapItemUuid.find(gem2Uid);
	
	UInt64 gem1ItemId = gem1Iter->mValue->m_nItemId;
	UInt64 gem2ItemId = gem2Iter->mValue->m_nItemId;
	
	if(gem1ItemId != gem2ItemId)
	{
		//��Ӵ���ظ�
		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;
		root["gems"] = "";
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		resp.mRespJsonStr = jsonStr;
		
		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);
		return;
	}
	else
	{
		//�ϳɵ��߼�����
		GemCombineTemplate * gem1Template = GEMCOMBINE_TABLE().get(gem1ItemId);
			
		if(gem1Template->mNextGem == 0)
		{
			//��Ӵ���ظ�
			GemCombineResp resp;
			resp.mPacketID = GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidIdentify;
			root["gems"] = "";
			Json::FastWriter writer;
			std::string jsonStr = writer.write(root);

			resp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);
			return;
		}

		if(gem1Uid == gem2Uid)
		{
			//�ѵ�uid��ͬ
			//�жϱ������Ƿ���
			if(m_nItemOccupies < MAXITEMCOUNT)
			{
				//��ǰ�пհ׵ı�����
				delItem(gem1Uid,2);
			}
			else
			{
				//��ǰû�пհ׵ı�����
				//���ȿ��������ɵı�ʯ�Ƿ��ڱ����д��ڣ�
				Map<Guid,List<Guid> >::Iter * itemId2UidsIter = m_mapItemId2Uids.find(gem1Template->mNextGem);

				ItemTemplate * newGemTemplate = ITEM_TABLE().get(gem1Template->mNextGem);

				//���౦ʯ�ڱ����д���
				if(itemId2UidsIter)
				{
					//���ڱ���Ƿ���Ҫռ��һ���µı������bool
					bool isNeedNewBag = true;
					//������ڣ��ҵ�δ�ѵ�����
					List<Guid> & uidsList = itemId2UidsIter->mValue;
					for(List<Guid>::Iter * uidsIter = uidsList.begin(); uidsIter != NULL; uidsIter = uidsList.next(uidsIter))
					{
						Map<Guid, ItemData*>::Iter * dataIter = m_mapItemUuid.find(uidsIter->mValue);
						assert(dataIter);
						if(dataIter->mValue->m_nCurStackAmount <= newGemTemplate->mStackAmount-1)
						{
							//�����д��ڴ�����Ʒ�Ķѵ�����δ�������ҷ����µ���Ʒ���ᵼ��ռ���µı�������
							isNeedNewBag = false;
						}
					}

					if(!isNeedNewBag)
					{
						//��ǰ�ı�������
						delItem(gem1Uid,2);
					}
					else
					{
						//��ǰ�����񲻹���
						//�ж�ɾ�����б�ʯ�Ƿ����ɱ��������һ��
						Map<Guid, ItemData*> ::Iter * oldGemIter = m_mapItemUuid.find(gem1Uid);
						
						//ɾ����ǰ��ʯ�ᵼ�����һ��������
						if(oldGemIter->mValue->m_nCurStackAmount <= 2)
						{
							delItem(gem1Uid,2);
						}
						else
						{
							//ɾ����ǰ��ʯ���ᵼ�����һ�������񣬲��һ����µı�����ռ�ã����ҵ�ǰ�����Ѿ�����
							//��ʾ��ұ������Ӳ���
							Json::Value root;
							GemCombineResp resp;
							resp.mPacketID = GEM_COMBINE_RESP;
							root["gems"] = "";
									

							root["errorId"] = LynxErrno::BagIsFull;


							Json::FastWriter writer;
							std::string jsonStr = writer.write(root);

							resp.mRespJsonStr = jsonStr;

							const ConnId& connId = mPlayer->getConnId();

							NetworkSystem::getSingleton().sendMsg(resp,connId);

							return ;
						}

					}
				}
				else
				{
					//���౦ʯ�ڱ����в�����

					//��ǰ�����񲻹���
					//�ж�ɾ�����б�ʯ�Ƿ����ɱ��������һ��
					Map<Guid, ItemData*> ::Iter * oldGemIter = m_mapItemUuid.find(gem1Uid);

					//ɾ����ǰ��ʯ�ᵼ�����һ��������
					if(oldGemIter->mValue->m_nCurStackAmount <= 2)
					{
						delItem(gem1Uid,2);
					}
					else
					{
						//ɾ����ǰ��ʯ���ᵼ�����һ�������񣬲��һ����µı�����ռ�ã����ҵ�ǰ�����Ѿ�����
						//��ʾ��ұ������Ӳ���

						GemCombineResp resp;
						resp.mPacketID = GEM_COMBINE_RESP;
						Json::Value root;
						root["gems"] = "";


						root["errorId"] = LynxErrno::BagIsFull;


						Json::FastWriter writer;
						std::string jsonStr = writer.write(root);

						resp.mRespJsonStr = jsonStr;

						const ConnId& connId = mPlayer->getConnId();

						NetworkSystem::getSingleton().sendMsg(resp,connId);

						return ;
					}

				}

				
			}
		}
		else
		{
			//�ѵ�uid����ͬ, �������������ϳ�һ����ʯ��˵����һ��������ֻ��һ����ʯ�������������ӿ϶��ǹ��õ�
		
			
				delItem(gem1Uid,1);
				delItem(gem2Uid,1);
			
			
		}

		Json::Value root;
		List<UInt64> gemList = getNewItems(gem1Template->mNextGem,1);
		for(List<UInt64>::Iter* iterList = gemList.begin(); iterList != NULL; iterList = gemList.next(iterList))
		{
			Map<Guid, ItemData*>::Iter* mapIter = m_mapItemUuid.find(iterList->mValue);
			
			Json::Value gemEle;
			gemEle["gemUid"] = iterList->mValue;
			gemEle["stackAmount"] = mapIter->mValue->m_nCurStackAmount;

			root["gems"].append(gemEle);
		}

		root["errorId"] = LynxErrno::None;

		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		
		
		
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		resp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

	}
	
}

void ItemManager::gemCombineOnce()
{
	//���飬ÿ����ɫ���ۺϳ�һ����ʯ������
	UInt32 gemLv1Color[6] = {0};
	//���ڻ�ȡ�µı�ʯuid�б�
	List<UInt64> newGemUidLists;
	
	Vector<UInt64> oldGemUidVecs;
	Map<UInt64, UInt32 > newGemId2Count;
	
	//��Ϊһ���ϳ���Ҫ�����еı�ʯ�����һ���ı�ʯ��Ȼ�����¼��㣬�ж��������������԰ѵ�ǰ�����б�ʯ��ռ�ı�����ȫ����Ϊ�յ�
	for(Map<Guid, ItemData*>::Iter * mapItemDataIter = m_mapUidItemType.begin(); mapItemDataIter!= NULL; 
		mapItemDataIter = m_mapUidItemType.next(mapItemDataIter))
	{
		//�ҵ�itemid
		UInt64 itemID = mapItemDataIter->mValue->m_nItemId;

		ItemTemplate * itemTemplate = ITEM_TABLE().get(itemID);
		//�ж��Ƿ�Ϊ��ʯ����
		if(itemTemplate->mTypeId == GEMTYPEID)
		{

			//��ǰ�ѵ�����
			UInt32 curStackAmount = mapItemDataIter->mValue->m_nCurStackAmount;
			GemCombineOnceTemplate * gemCombineOnceTemplate = GEMCOMBINEONCE_TABLE().get(itemID);

			//��ǰ�ѵ�����*ÿһ��ת��Ϊһ����ʯ�������õ��ܹ���һ��ͬ���͵ı�ʯ����
			gemLv1Color[itemTemplate->mSubTypeId] += ((gemCombineOnceTemplate->mCount)*curStackAmount);
			oldGemUidVecs.push_back(mapItemDataIter->mValue->m_nUid);

		}
	}

	//ͳ��һ���Ժϳ������ɵı�ʯ������id
	for(UInt32 i = 1; i < 6; i++)
	{
		//��ʯ������
		UInt32 &type = i;
		//��ʯ�����Ͷ�Ӧһ����ʯ�ܹ�����
		UInt32 totalCount = gemLv1Color[i];
		
		if(totalCount <= 0)
		{
			continue;
		}
		
		for(UInt32 level = 15; level > 0; level--)
		{
			//�����ͺ͵ȼ���Ӧ��������id�ṹ��
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(type,level);

			if(gemIdAndCount.mID == 0)
			{
				continue;
			}

			int newGemCounts = 0;
			while(totalCount >= gemIdAndCount.mCount)
			{
				newGemCounts++;
				totalCount -= gemIdAndCount.mCount;

			}

			if(newGemCounts)
			{
				newGemId2Count.insert(gemIdAndCount.mID, newGemCounts);
			}
			
			
		}

	}

	UInt32 totalNewGemCount = 0;
	for(Map<UInt64, UInt32 >::Iter * newGemId2CountIter = newGemId2Count.begin(); newGemId2CountIter != NULL; 
		newGemId2CountIter = newGemId2Count.next(newGemId2CountIter) )
	{
		
		ItemTemplate * newGemItem = ITEM_TABLE().get(newGemId2CountIter->mKey);
		
		while(newGemId2CountIter->mValue >newGemItem->mStackAmount)
		{
			totalNewGemCount++;
			newGemId2CountIter->mValue -= newGemItem->mStackAmount;
		}

		totalNewGemCount++;
		
	}

	if(totalNewGemCount - oldGemUidVecs.size() + m_nItemOccupies > MAXITEMCOUNT)
	{
		//��ʾ��ұ����Ѿ����ˣ����ܺϳ�

		Json::Value root;

		root["gems"] = "";
		root["errorId"] = LynxErrno::BagIsFull;

		Json::FastWriter writer;
		std::string jsongStr = writer.write(root);
		GemCombineResp resp;
		resp.mPacketID = GEM_COMBINE_RESP;
		resp.mRespJsonStr = jsongStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);

		return ;

	}
	
	for(UInt32 i = 0; i < oldGemUidVecs.size(); i++)
	{
		delItem(oldGemUidVecs[i],0);
	}

	

	for(UInt32 i = 1; i < 6; i++)
	{
		//��ʯ������
		UInt32 &type = i;
		//��ʯ�����Ͷ�Ӧһ����ʯ�ܹ�����
		UInt32 totalCount = gemLv1Color[i];
		
		if(totalCount <= 0)
		{
			continue;
		}

		for(UInt32 level = 15; level > 0; level--)
		{
			//�����ͺ͵ȼ���Ӧ��������id�ṹ��
			GemIdAndCount gemIdAndCount  = GEMCOMBINEONCE_TABLE().getIdAndCount(type,level);
			
			if(gemIdAndCount.mID == 0)
			{
				continue;
			}
				
			int newGemCounts = 0;
			while(totalCount >= gemIdAndCount.mCount)
			{
				newGemCounts++;
				totalCount -= gemIdAndCount.mCount;
				
			}
			
			if(newGemCounts)
			{
				newGemUidLists += getNewItems(gemIdAndCount.mID,newGemCounts);
			}
			
		}
		
	}

	Json::Value root;

	for(List<UInt64>::Iter* newUidIter = newGemUidLists.begin(); newUidIter != NULL; newUidIter = newGemUidLists.next(newUidIter))
	{
		Json::Value gemEle;
		Map<Guid, ItemData*>::Iter * itemDataIter = m_mapUidItemType.find(newUidIter->mValue);
		gemEle["stackAmount"] = itemDataIter->mValue->m_nCurStackAmount;
		gemEle["gemUid"] = newUidIter->mValue;
		gemEle["itemId"] = itemDataIter->mValue->m_nItemId;
		gemEle["gettime"] = itemDataIter->mValue->m_nItemGetTime;
		
		for(List<UInt32>::Iter * paramIter = itemDataIter->mValue->m_listParam.begin(); paramIter != NULL; 
			paramIter = itemDataIter->mValue->m_listParam.next(paramIter))
		{
			gemEle["param"].append(paramIter->mValue);
		}

		
		root["gems"].append(gemEle);
	}

	
	for(UInt32 i = 0; i < oldGemUidVecs.size(); i++)
		
	{
		Json::Value gemEle;
		
		gemEle["stackAmount"] = 0;
		gemEle["gemUid"] = oldGemUidVecs[i];
		gemEle["itemId"] = 0;
		gemEle["gettime"] = 0;
        gemEle["param"].append(0);
		root["gems"].append(gemEle);
	}
	
	root["errorId"] = LynxErrno::None;
	Json::FastWriter writer;
	std::string jsongStr = writer.write(root);
	GemCombineResp resp;
	resp.mPacketID = GEM_COMBINE_RESP;
	resp.mRespJsonStr = jsongStr;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resp,connId);


}

//��ʯ��Ƕ��װ����
void ItemManager::gemSetEquip(UInt64 equipUid, UInt32 equipGemSlot,UInt64 gemUid)
{
	Map<UInt64, ItemData*>::Iter * equipDataIter = m_mapUidEquipType.find(equipUid);
	Map<UInt64, ItemData*>::Iter * gemDataIter = m_mapUidItemType.find(gemUid);
	
	if(equipDataIter == NULL || gemDataIter == NULL)
	{
		GemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;

		root["gemUid"] = 0;
		root["gemCount"] = 0;
		root["equipUid"] = 0;
		root["equipGemList"].append(0);

		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		gemSetResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

		return;
	}
	else
	{
		ItemData* equipData = equipDataIter->mValue;
		ItemData* gemData = gemDataIter->mValue;

		List<Guid>::Iter * gemIdIter = equipData->m_itemAttribute.m_listGems.getn(equipGemSlot);
		
		if(gemIdIter->mValue)
		{
			
			//�ж�װ���ϵĲ�λ�Ѿ���Ƕ�˱�ʯ,��ж�±�ʯ���ж��ܷ�ж�سɹ�
			if(unload(equipData,equipGemSlot) == 0)
			{
				//ж��ʧ�ܣ���ʾ�û�������ʯʧ��
				GemSetResp gemSetResp;

				Json::Value root;
				root["errorId"] = LynxErrno::BagIsFull;

				root["gemUid"] = 0;
				root["gemCount"] = 0;
				root["equipUid"] = 0;
				root["equipGemList"].append(0);

				Json::FastWriter writer;
				std::string jsonStr = writer.write(root);

				gemSetResp.mRespJsonStr = jsonStr;

				const ConnId& connId = mPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

				return;
			}
			else
			{
				//���ò�λ�ı�ʯ�����µ�,װ������Ƕ���Ǳ�ʯ��itemid
				gemIdIter->mValue = gemData->m_nItemId;
				equipData->m_nDirtyType = UPDATEDIRTY;

				//ɾ��һ����ʯ
				delItem(gemUid,1);
			
			}
			
		}
		else
		{
			
			//װ���ϵĲ�λû�б�ʯ
			//װ����λ������Ϊ��ʯ��itemId
			gemIdIter->mValue = gemData->m_nItemId;
			equipData->m_nDirtyType = UPDATEDIRTY;


			//ɾ��һ����ʯ
			delItem(gemUid,1);
		}

		GemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;
		
		root["gemUid"] = gemUid;
		Map<UInt64, ItemData*>::Iter *gemSetIter = m_mapUidItemType.find(gemUid);
		
		if(gemSetIter)
		{
			root["gemCount"] = gemSetIter->mValue->m_nCurStackAmount;
		}
		else
		{
			root["gemCount"] = 0;
		}
		
		root["equipUid"] = equipUid;

		for(List<Guid>::Iter * gemListIter = equipData->m_itemAttribute.m_listGems.begin();gemListIter != NULL; 
			gemListIter = equipData->m_itemAttribute.m_listGems.next(gemListIter))
		{
			root["equipGemList"].append(gemListIter->mValue);
		}
		

		Json::FastWriter writer;
		std::string jsonStr = writer.write(root);

		gemSetResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);
	}
}


//��ʯж��, ����ж�º��µı�ʯuid�����Ϊ0�����ʾж��ʧ��
UInt64 ItemManager::unload(ItemData * equipData, UInt32 gemIndex)
{
	List<Guid>::Iter * gemIter = equipData->m_itemAttribute.m_listGems.getn(gemIndex);
	List<UInt64> gemUids = getNewItemsSame(gemIter->mValue,1);

	if(!gemUids.empty())
	{
		//װ����Ӧ�ı�ʯ������Ϊ��
		gemIter->mValue = 0;
		return gemUids.first()->mValue;
	}
	else
	{
		return 0;
	}
}


void ItemManager::equipGemCombine(UInt64 equipUid, UInt32 gemSlot)
{
	Map<Guid, ItemData*>::Iter * equipIter = m_mapItemUuid.find(equipUid);
	List<Guid>::Iter * gemIter = equipIter->mValue->m_itemAttribute.m_listGems.getn(gemSlot);
	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemIter->mValue);
	
	if(gemCombineTemplate->mNextGem)
	{
		Map<Guid,List<Guid> >::Iter * uidListIter = m_mapItemId2Uids.find(gemIter->mValue);
		
		//�Ƿ��ҵ�������ͬ�����Ʒ
		if(uidListIter)
		{
			
			//����ҵ�
			List<UInt64> &uidList = uidListIter->mValue;

			//���ڲ���δ�ѵ����ı�����
			Map<Guid, ItemData*>::Iter * gemDataFind = NULL;
			for(List<UInt64>::Iter * uidIter = uidList.begin(); uidIter != NULL; uidIter = uidList.next(uidIter))
			{
				ItemTemplate * gemTemplate = ITEM_TABLE().get(gemIter->mValue);

				Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(uidIter->mValue);
				gemDataFind = gemDataIter;
				if(gemDataIter->mValue->m_nCurStackAmount < gemTemplate->mStackAmount)
				{
					break;
				}
			}

			//����ǰ�Ķѵ�����-1�����ҽ�װ���ı�ʯ�ۻ�Ϊ�߼���ʯ
			
			delItem(gemDataFind->mValue->m_nUid,1);	
			

			gemIter->mValue = gemCombineTemplate->mNextGem;
			equipIter->mValue->m_nDirtyType = UPDATEDIRTY;	
		
			
			EquipGemCombineResp combineResp;
			combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["gemUid"] = gemDataFind->mValue->m_nUid;
			root["gemCount"] = gemDataFind->mValue->m_nCurStackAmount;
			root["gemItemID"] = gemDataFind->mValue->m_nItemId;
            root["equipUid"] = equipIter->mValue->m_nUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin();gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			Json::FastWriter writer;
			std::string jsonStr = writer.write(root); 

			combineResp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		}
		else
		{
			//��ʾ��ұ�����û�д��౦ʯ���޷��ϳ�

			EquipGemCombineResp combineResp;
			combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::ItemNotExist;
			root["gemUid"] = 0;
			Json::FastWriter writer;
			std::string jsonStr = writer.write(root); 
			
			combineResp.mRespJsonStr = jsonStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(combineResp,connId);

			return;

		}
		
		
	}
	else
	{
		//��ʯ�ȼ��ﵽ���ޣ���ʾ���

		EquipGemCombineResp combineResp;
		combineResp.mPacketID = EQUIP_GEM_COMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::GemLevelLimit;
		root["gemUid"] = 0;
		Json::FastWriter writer;
		std::string jsonStr = writer.write(root); 

		combineResp.mRespJsonStr = jsonStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(combineResp,connId);

		return;
	}
	
}

List<UInt64> ItemManager::combineGemsList(UInt64 gemItemID,UInt32 count)
{
	List<UInt64> newGemList;
	UInt32 newGemCount = count/2;

	GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemItemID);
	
	if(gemCombineTemplate->mNextGem)
	{
		Map<Guid,List<Guid> >::Iter*  uidListIter = m_mapItemId2Uids.find(gemItemID);

		//�Ƿ��ҵ�������ͬ�����Ʒ
		if(uidListIter)
		{
			//����ҵ�
			List<UInt64> &uidList = uidListIter->mValue;
			ItemTemplate * gemTemplate = ITEM_TABLE().get(gemItemID);

			//���ж�ʣ��Ŀո����Ƿ�������һ��
			if(m_nItemOccupies < MAXITEMCOUNT)
			{
				//�пճ��ĸ��ӣ���ô��������±���һ�����������

				//���ڴ洢�ı�����uid���ѵ��������ٵ��࣬��Ҫ����������ݽṹ����ɾ���͸��£���Ϊɾ�����������ԭ���ṹ������ʧЧ
				List<UInt64> gemUidList;
				constructGemList(uidList, gemTemplate, gemUidList);
				combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);
				//�ϳɳɹ������͸��ͻ���
				
				sendCombineRes(newGemList);

				return newGemList;
			}
			else
			{
				//���ڴ洢�ı�����uid���ѵ��������ٵ��࣬��Ҫ����������ݽṹ����ɾ���͸��£���Ϊɾ�����������ԭ���ṹ������ʧЧ
				List<UInt64> gemUidList;
				constructGemList(uidList, gemTemplate, gemUidList);
				UInt64 firstGemUid  = gemUidList.first()->mValue;
				Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(firstGemUid);
				
				//�����ĵı�ʯ�����������ڳ�ԭ�е�һ�����ӣ����������ɵ��Ƿ��ռ��һ������ĵ�Ԫ��
				if(gemDataIter ->mValue->m_nCurStackAmount > count)
				{
					UInt32 newOccupies = calculateCount(gemCombineTemplate->mNextGem,newGemCount);
					if(newOccupies >= 1)
					{
						//�����ɵ���Ʒ��Ҫ����ռ��һ����Ԫ�����Իᵼ�±����������ʾ��ұ��������޷��ϳ�
						sendCombineRes(newGemList,false,LynxErrno::BagIsFull);
						return newGemList;
					}
					else
					{
						//�ϳ�
						combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);

						sendCombineRes(newGemList);
						return newGemList;
					}
				}
				else
				{
					combineGems( gemUidList, newGemList, gemCombineTemplate,newGemCount,count);
					sendCombineRes(newGemList);
					return newGemList;
				}
			}
		}
		else
		{
			//��ʾ��ұ�����û�д˱�ʯ���޷��ϳ�
			sendCombineRes(newGemList,false,LynxErrno::ItemNotExist);
			return newGemList;
		}

		
	
	}
	else
	{
		sendCombineRes(newGemList,false,LynxErrno::GemLevelLimit);
		
		return newGemList;
	}
}


void ItemManager::combineGems(List<UInt64> &gemUidList, List<UInt64> &newGemList,GemCombineTemplate * gemCombineTemplate,const UInt32 &newGemCount,UInt32 & count)
{

	for(List<UInt64>::Iter * gemUidIter = gemUidList.begin(); gemUidIter != NULL; 
		gemUidIter = gemUidList.next(gemUidIter))
	{
		Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(gemUidIter->mValue);

		//Ҫ���ĵı�ʯ�ȵ�Ԫ����Ķ࣬����ɾ����Ԫ����ı�ʯ
		if(gemDataIter->mValue->m_nCurStackAmount <= count)
		{
			count -= gemDataIter->mValue->m_nCurStackAmount;
			delOneItem(gemDataIter->mValue);
			newGemList.insertTail(gemDataIter->mValue->m_nUid);
		}
		else
		{
			//��Ԫ����ı�Ҫɾ���ı�ʯ�����࣬���Ը��µ�Ԫ����ı�ʯ
			updateOneItem(gemDataIter->mValue,gemDataIter->mValue->m_nCurStackAmount - count);
			count = 0;
			newGemList.insertTail(gemDataIter->mValue->m_nUid);
			break;
		}
	}

	newGemList += getNewItems(gemCombineTemplate->mNextGem,newGemCount);
}


void ItemManager::constructGemList(List<UInt64> &uidList,ItemTemplate * gemTemplate, List<UInt64> &gemUidList)
{
	for(List<UInt64>::Iter * uidIter = uidList.begin(); uidIter != NULL; uidIter = uidList.next(uidIter))
	{
		Map<Guid, ItemData*>::Iter * gemDataIter =  m_mapItemUuid.find(uidIter->mValue);


		if(gemDataIter->mValue->m_nCurStackAmount < gemTemplate->mStackAmount)
		{
			gemUidList.insertHead(uidIter->mValue);
		}
		else
		{
			gemUidList.insertTail(uidIter->mValue);
		}
	}
}

void ItemManager::sendCombineRes(List<UInt64> &newGemList, bool success, errorId  errorId)
{
	Json::Value root;
	if(success)
	{
		for(List<UInt64>::Iter* newUidIter = newGemList.begin(); newUidIter != NULL; newUidIter = newGemList.next(newUidIter))
		{
			Json::Value gemEle;
			Map<Guid, ItemData*>::Iter * itemDataIter = m_mapUidItemType.find(newUidIter->mValue);
			
			if(itemDataIter)
			{
				gemEle["stackAmount"] = itemDataIter->mValue->m_nCurStackAmount;
				gemEle["gemUid"] = newUidIter->mValue;
				gemEle["itemId"] = itemDataIter->mValue->m_nItemId;
				gemEle["gettime"] = itemDataIter->mValue->m_nItemGetTime;
				

				for(List<UInt32>::Iter * paramIt = itemDataIter->mValue->m_listParam.begin(); paramIt != NULL; 
					paramIt = itemDataIter->mValue->m_listParam.next(paramIt))
				{
					gemEle["param"].append(paramIt->mValue);
				}

				root["gems"].append(gemEle);
			}
			else
			{
				gemEle["stackAmount"] = 0;
				gemEle["gemUid"] = newUidIter->mValue;
				gemEle["itemId"] = 0;
				gemEle["gettime"] = 0;
				gemEle["param"].append(0);

				root["gems"].append(gemEle);
			}
		
		}

		root["errorId"] = LynxErrno::None;
	}
	else
	{
		root["errorId"] = errorId;
		root["gems"] = "";
	}
	
	Json::FastWriter writer;
	std::string jsongStr = writer.write(root);
	GemCombineResp resp;
	resp.mPacketID = GEM_COMBINE_RESP;
	resp.mRespJsonStr = jsongStr;

	const ConnId& connId = mPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resp,connId);
}

void ItemManager::equipGemUnload(UInt64 equipUid, UInt32 gemSlot)
{
	Map<Guid, ItemData*>::Iter * equipIter = m_mapItemUuid.find(equipUid);
	
	if(equipIter)
	{
		UInt64 newGemUid = unload(equipIter->mValue,gemSlot);
		if(newGemUid)
		{
			Map<Guid, ItemData*>::Iter * newGemIter = m_mapItemUuid.find(newGemUid);
			assert(newGemIter);
			Json::Value root;

			root["errorId"] = LynxErrno::None;
			root["equipUid"] = equipUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			root["newGem"]["id"] = newGemUid;
			root["newGem"]["itemTypeId"] = newGemIter->mValue->m_nItemId;
			root["newGem"]["gettime"] = newGemIter->mValue->m_nItemGetTime;
			root["newGem"]["count"] = newGemIter->mValue->m_nCurStackAmount;

			for(List<UInt32>::Iter * paramIter = newGemIter->mValue->m_listParam.begin(); paramIter!=NULL;
				paramIter = newGemIter->mValue->m_listParam.next(paramIter))
			{
				root["newGem"]["param"].append(paramIter->mValue);
			}

			Json::FastWriter writer;
			std::string jsongStr = writer.write(root);

			GemUnloadResp resp;
			resp.mPacketID = GEM_UNLOAD_RESP;
			resp.mRespJsonStr = jsongStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);

		}
		else
		{
			Json::Value root;
			//��ʾ��жʧ��
			root["errorId"] = LynxErrno::BagIsFull;
			root["equipUid"] = equipUid;

			for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
				gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
			{
				root["equipGemList"].append(gemListIter->mValue);
			}

			root["newGem"]["id"] = 0;
			root["newGem"]["itemTypeId"] = 0;
			root["newGem"]["gettime"] = 0;
			root["newGem"]["count"] = 0;

		    root["newGem"]["param"].append(0);
			

			Json::FastWriter writer;
			std::string jsongStr = writer.write(root);

			GemUnloadResp resp;
			resp.mPacketID = GEM_UNLOAD_RESP;
			resp.mRespJsonStr = jsongStr;

			const ConnId& connId = mPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(resp,connId);

		}
		
	}
	else
	{
		//��ʾ���װ����id����ȷ

		Json::Value root;
		//��ʾ��жʧ��
		root["errorId"] = LynxErrno::InvalidIdentify;
		root["equipUid"] = equipUid;

		for(List<Guid>::Iter * gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.begin(); gemListIter != NULL; 
			gemListIter = equipIter->mValue->m_itemAttribute.m_listGems.next(gemListIter))
		{
			root["equipGemList"].append(gemListIter->mValue);
		}

		root["newGem"]["id"] = 0;
		root["newGem"]["itemTypeId"] = 0;
		root["newGem"]["gettime"] = 0;
		root["newGem"]["count"] = 0;

		root["newGem"]["param"].append(0);


		Json::FastWriter writer;
		std::string jsongStr = writer.write(root);

		GemUnloadResp resp;
		resp.mPacketID = GEM_UNLOAD_RESP;
		resp.mRespJsonStr = jsongStr;

		const ConnId& connId = mPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resp,connId);
	}
}