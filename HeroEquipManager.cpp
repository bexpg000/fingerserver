#include "HeroEquipManager.h"
#include "Player.h"
#include "Table.h"
#include "GameServer.h"

using namespace Lynx;
HeroEquipManager::HeroEquipManager()
{
	m_pPlayer = NULL;
	
}	


HeroEquipManager:: ~HeroEquipManager()
{

}

bool HeroEquipManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	
	UInt32 equipPos = 1;
	for(List<HeroEquipData>::Iter * equipDataIter = player->mPlayerData.mEquipData.mHeroEquipLists.begin(); equipDataIter != NULL; 
		equipDataIter = player->mPlayerData.mEquipData.mHeroEquipLists.next(equipDataIter))
	{
		m_mapHeroEquipData.insert(equipPos, &equipDataIter->mValue);
		equipPos++;		
	}
	
	m_pHeroEquipList = &(player->mPlayerData.mEquipData.mHeroEquipLists);

	m_pHeroModel = &player->mPlayerData.mBaseData.m_nModelID;
	m_pPlayerLv = &player->mPlayerData.mBaseData.m_nLevel;

	return true;

}

void HeroEquipManager::release()
{
	//ָ���ÿգ�map ���

	m_pPlayer = NULL;
	m_pHeroEquipList = NULL;
	m_pHeroModel = NULL;
	m_pPlayerLv = NULL;
	m_mapHeroEquipData.clear();
	
}

errorId HeroEquipManager::enhanceLvUp(HeroEquipData& equipData, UInt32& playerLv, Map<UInt64, MaterialData*> &changeMaterialData, NewEquipLvTemplate * newEquipLvTemp)
{
	if(equipData.m_nEquipLv < *m_pPlayerLv)
	{
		MaterialManager & materialManager = m_pPlayer->getMaterialManager();

		if(m_pPlayer->mPlayerData.mBaseData.m_nCoin <newEquipLvTemp->mLvUpNeedCoin )
		{
			//��ʾ��ҽ�Ҳ���
			
			return LynxErrno::CoinNotEnough;
		}
		//�ж��Ƿ�������Դ����Ҫ
		if(newEquipLvTemp->mLvUpNeedResId1 && newEquipLvTemp->mLvUpNeedResId2)
		{
			//�ж���Ҳ��ϵ������Ƿ���ڵ���������������������������˳�����ʧ�ܰ����ͻ���,�������������ָ�������Ĳ���
			MaterialData * materialData1 = materialManager.getMaterialData(newEquipLvTemp->mLvUpNeedResId1);
			MaterialData * materialData2 = materialManager.getMaterialData(newEquipLvTemp->mLvUpNeedResId2);
			if(!materialData1 || !materialData2)
			{
				return LynxErrno::MaterialNotEnough;
			}

			UInt32  res1count = materialData1->m_nCount;
			UInt32  res2count = materialData2->m_nCount;

			if(res1count < newEquipLvTemp->mLvUpNeedResCnt1 || res2count < newEquipLvTemp->mLvUpNeedResCnt2)
			{
				//��ʾ��Ҳ��ϲ���

				return LynxErrno::MaterialNotEnough;
			}

			materialManager.delMaterial(newEquipLvTemp->mLvUpNeedResId1,newEquipLvTemp->mLvUpNeedResCnt1);
			materialManager.delMaterial(newEquipLvTemp->mLvUpNeedResId2,newEquipLvTemp->mLvUpNeedResCnt2);

			//�ڸı��б�����Ҳ�����Ʒ
			Map<UInt64, MaterialData*>::Iter * changeIter1 = changeMaterialData.find(newEquipLvTemp->mLvUpNeedResId1);
			//û�ҵ��ͽ�������ı��б���
			if(!changeIter1)
			{
				changeMaterialData.insert(materialData1->m_nId, materialData1);
			}

			Map<UInt64, MaterialData*>::Iter * changeIter2 = changeMaterialData.find(newEquipLvTemp->mLvUpNeedResId2);
			//û�ҵ��ͽ�������ı��б���
			if(!changeIter2)
			{
				changeMaterialData.insert(materialData2->m_nId, materialData2);
			}

			equipData.m_nEquipLv ++;
			
			if(newEquipLvTemp->mIsStepUp)
			{	
			  equipData.m_nStep++;
			}
		
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - newEquipLvTemp->mLvUpNeedCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog2);


			return LynxErrno::None;

		}
		else
		{
			//���ֻ��Ҫһ����Դ����ô�ж�
			if(newEquipLvTemp->mLvUpNeedResId1)
			{ 
				//�ж���Ҳ��ϵ������Ƿ���ڵ���������������������������˳�����ʧ�ܰ����ͻ���,�������������ָ�������Ĳ���
				MaterialData * materialData1 = materialManager.getMaterialData(newEquipLvTemp->mLvUpNeedResId1);

				if(!materialData1)
				{
					//��ʾ��Ҳ��ϲ���
					return LynxErrno::MaterialNotEnough;
				}

				UInt32  res1count = materialData1->m_nCount;
				if(res1count < newEquipLvTemp->mLvUpNeedResCnt1 )
				{
					//��ʾ��Ҳ��ϲ���

					return LynxErrno::MaterialNotEnough;
				}

				Map<UInt64, MaterialData*>::Iter * changeIter1 = changeMaterialData.find(newEquipLvTemp->mLvUpNeedResId1);
				//û�ҵ��ͽ�������ı��б���
				if(!changeIter1)
				{
					changeMaterialData.insert(materialData1->m_nId, materialData1);
				}

				materialManager.delMaterial(newEquipLvTemp->mLvUpNeedResId1,newEquipLvTemp->mLvUpNeedResCnt1);
				
			}

			if(newEquipLvTemp->mLvUpNeedResId2)
			{
				//�ж���Ҳ��ϵ������Ƿ���ڵ���������������������������˳�����ʧ�ܰ����ͻ���
				MaterialData * materialData2 = materialManager.getMaterialData(newEquipLvTemp->mLvUpNeedResId2);
				if(!materialData2)
				{
					return LynxErrno::MaterialNotEnough;
				}

				UInt32  res2count = materialData2->m_nCount;

				if(res2count < newEquipLvTemp->mLvUpNeedResCnt2 )
				{
					//��ʾ��Ҳ��ϲ���

					return LynxErrno::MaterialNotEnough;
				}

				Map<UInt64, MaterialData*>::Iter * changeIter2 = changeMaterialData.find(newEquipLvTemp->mLvUpNeedResId2);
				//û�ҵ��ͽ�������ı��б���
				if(!changeIter2)
				{
					changeMaterialData.insert(materialData2->m_nId, materialData2);
				}

				materialManager.delMaterial(newEquipLvTemp->mLvUpNeedResId2,newEquipLvTemp->mLvUpNeedResCnt2);
			}

			equipData.m_nEquipLv ++;

			if(newEquipLvTemp->mIsStepUp)
			{	
				equipData.m_nStep++;
			}


			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - newEquipLvTemp->mLvUpNeedCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog2);


			return LynxErrno::None;
		}

	}
	else
	{
		//װ���Ѵﵽ���ȼ�

		return LynxErrno::EquipLvLimit;
	}
}

void HeroEquipManager::enhanceEquip(UInt64 playerUid, UInt32 equipPos, UInt32 enhanceType)
{
	Map<UInt32, HeroEquipData *>::Iter * equipDataIter = m_mapHeroEquipData.find(equipPos);
	
	UInt64& equipId = equipDataIter->mValue->m_nEquipId;
	
	const ConnId& connId = m_pPlayer->getConnId();

	
	if(!equipId)
	{
		//����ʧ����Ϣ��װ��δ����
		
		GCHeroEquipEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EquipNoneActive;
		Json::StyledWriter writer;
		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);

		cout << enhanceResp.mRespJsonStr;
		return;
	}

	UInt32 &equipLv = equipDataIter->mValue->m_nEquipLv;
	UInt32 lastEquipLv = equipDataIter->mValue->m_nEquipLv;

	NewEquipLvTemplate * nextEquipLvTempB = NEWEQUIPLV_TABLE().reverseGet(equipId, equipLv + 1);
	
	if(!nextEquipLvTempB)
	{
		GCHeroEquipEnhanceResp enhanceResp;
		enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EquipLvLimit;
		Json::StyledWriter writer;
		enhanceResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);

		cout << enhanceResp.mRespJsonStr;
		return;
	}

	if(enhanceType)
	{
		//1��ʾ���ǿ��

		NewEquipLvTemplate * newEquipLvTemp = NEWEQUIPLV_TABLE().reverseGet(equipId, equipLv);
		Map<UInt64, MaterialData*> changeMaterialData;
		errorId enhanceRes = enhanceLvUp(*equipDataIter->mValue, *m_pPlayerLv,changeMaterialData,newEquipLvTemp);
		
		if(enhanceRes != LynxErrno::None)
		{
			GCHeroEquipEnhanceResp enhanceResp;
			enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
			Json::Value root;
			root["errorId"] = enhanceRes;
			Json::StyledWriter writer;
			enhanceResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);
			cout << enhanceResp.mRespJsonStr;
			return ;
		}
		else
		{
			newEquipLvTemp = NEWEQUIPLV_TABLE().reverseGet(equipId, equipLv);
			while(enhanceRes == LynxErrno::None && !newEquipLvTemp->mIsStepUp)
			{
				enhanceRes = enhanceLvUp(*equipDataIter->mValue, *m_pPlayerLv,changeMaterialData,newEquipLvTemp);
				newEquipLvTemp = NEWEQUIPLV_TABLE().reverseGet(equipId, equipLv);
			}

			m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);

			GCHeroEquipEnhanceResp enhanceResp;
			enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["equipid"] = equipDataIter->mValue->m_nEquipId;
			root["enhancelv"] = equipDataIter->mValue->m_nEquipLv;
			root["enhancestep"] = equipDataIter->mValue->m_nStep;
			root["nowCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
			root["equippos"] = equipDataIter->mKey;

			for(Map<UInt64, MaterialData*>::Iter * changematerialIter = changeMaterialData.begin(); changematerialIter != NULL; 
				changematerialIter = changeMaterialData.next(changematerialIter))
			{
				Json::Value materialroot;
				materialroot["id"] = changematerialIter->mValue->m_nId;
				materialroot["count"] = changematerialIter->mValue->m_nCount;

				root["itemList"].append(materialroot);
			}
			//

			Json::StyledWriter writer;
			enhanceResp.mRespJsonStr = writer.write(root);
			cout << enhanceResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);

			if (enhanceType == 1 &&root["errorId"] == 0)
			{
				UInt32 max = equipLv - lastEquipLv;
				//��������ѵ
				LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT02,max);
			}

			return;

		}

		
		
	}
	else
	{
		//0��ʾǿ��һ��
		Map<UInt64, MaterialData*> changeMaterialData;

		NewEquipLvTemplate * newEquipLvTemp = NEWEQUIPLV_TABLE().reverseGet(equipId, equipLv);
		errorId enhanceRes = enhanceLvUp(*equipDataIter->mValue, *m_pPlayerLv,changeMaterialData,newEquipLvTemp);
		
		if(enhanceRes != LynxErrno::None)
		{
			GCHeroEquipEnhanceResp enhanceResp;
			enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
			Json::Value root;
			root["errorId"] = enhanceRes;
			Json::StyledWriter writer;
			enhanceResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);
			cout << enhanceResp.mRespJsonStr;
			return ;
		}
		else
		{
			m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);

			GCHeroEquipEnhanceResp enhanceResp;
			enhanceResp.mPacketID = BOC_HERO_EQUIPENHANCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["equipid"] = equipDataIter->mValue->m_nEquipId;
			root["enhancelv"] = equipDataIter->mValue->m_nEquipLv;
			root["enhancestep"] = equipDataIter->mValue->m_nStep;
			root["nowCoin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
			root["equippos"] = equipDataIter->mKey;

			for(Map<UInt64, MaterialData*>::Iter * changematerialIter = changeMaterialData.begin(); changematerialIter != NULL; 
				changematerialIter = changeMaterialData.next(changematerialIter))
			{
				Json::Value materialroot;
				materialroot["id"] = changematerialIter->mValue->m_nId;
				materialroot["count"] = changematerialIter->mValue->m_nCount;

				root["itemList"].append(materialroot);
			}

			Json::StyledWriter writer;
			enhanceResp.mRespJsonStr = writer.write(root);
			cout << enhanceResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(enhanceResp,connId);

			if (enhanceType == 0 &&root["errorId"] == 0)
			{
				//��������ѵ
				LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT02,1);
			}
			
			

			return;

		}
	}
}

//gemSlotPos ��1��ʼ, //���쳽
bool HeroEquipManager::getGemSlotOpenState(UInt32 equipPos, UInt32 gemSlotPos)
{
	bool openFlag = false;
	Map<UInt32, HeroEquipData *>::Iter * equipDataIter = m_mapHeroEquipData.find(equipPos);

	if(!equipDataIter)
	{
		return false;
	}

	switch(gemSlotPos)
	{
		case 1:
			{

				if(equipDataIter->mValue->m_nActiveBit & Gem1SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}

			}
			break;
		case 2:
			{
				if(equipDataIter->mValue->m_nActiveBit & Gem2SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}
			}
			break;
		case 3:
			{
				if(equipDataIter->mValue->m_nActiveBit & Gem3SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}
			}
			break;
		case 4:
			{
				if(equipDataIter->mValue->m_nActiveBit & Gem4SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}
			}
			break;
		case 5:
			{
				if(equipDataIter->mValue->m_nActiveBit & Gem5SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}
			}
			break;
		case 6:
			{
				if(equipDataIter->mValue->m_nActiveBit & Gem6SlotBit)
				{
					openFlag = true; 
				}
				else
				{
					openFlag = false;
				}
			}
			break;
		default:
			{
				openFlag = false;
			}
			
			break;

	}

	return openFlag;
}

bool HeroEquipManager::getGemSlotOpenState(Map<UInt32, HeroEquipData *>::Iter * equipDataIter, UInt32 gemSlotPos)
{
	bool openFlag = false;
	
	assert(equipDataIter);
	switch(gemSlotPos)
	{
	case 1:
		{

			if(equipDataIter->mValue->m_nActiveBit & Gem1SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}

		}
		break;
	case 2:
		{
			if(equipDataIter->mValue->m_nActiveBit & Gem2SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}
		}
		break;
	case 3:
		{
			if(equipDataIter->mValue->m_nActiveBit & Gem3SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}
		}
		break;
	case 4:
		{
			if(equipDataIter->mValue->m_nActiveBit & Gem4SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}
		}
		break;
	case 5:
		{
			if(equipDataIter->mValue->m_nActiveBit & Gem5SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}
		}
		break;
	case 6:
		{
			if(equipDataIter->mValue->m_nActiveBit & Gem6SlotBit)
			{
				openFlag = true; 
			}
			else
			{
				openFlag = false;
			}
		}
		break;
	default:
		{
			openFlag = false;
		}

		break;

	}

	return openFlag;
}

void HeroEquipManager::checkLvUpActive()
{
	bool changeFlag = false;
	for(Map<UInt32, HeroEquipData *>::Iter *equipMapIter = m_mapHeroEquipData.begin(); equipMapIter != NULL; 
	equipMapIter = m_mapHeroEquipData.next(equipMapIter))
	{
		if(equipMapIter->mValue->m_nEquipId == 0)
		{
			checkEquipActive(equipMapIter->mKey,*m_pHeroModel, equipMapIter,changeFlag);
		}
		else
		{
			checkGemActive(*equipMapIter->mValue,changeFlag);
		}
	}
	
	if(changeFlag)
	{
		m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
	}
}

//����һ�óɾ�ʱ�����óɾͼ��

void HeroEquipManager::checkAchievementActive()
{
	//�߻�û����ϸ���ݺͱ�񣬷�����Ҳû�гɾ�ϵͳ�����Խӿ�û��д
}

//������ֶ�����ĳ����ʯ��

bool HeroEquipManager::manualActive(UInt32 equipPos, UInt32 gemSlot)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipDataIter = m_mapHeroEquipData.find(equipPos);

	bool isOpen = getGemSlotOpenState(heroEquipDataIter,gemSlot);
	
	errorId errorId  = LynxErrno::EquipGemUnlocked;
	if(isOpen)
	{
		//�������߿ͻ����Ѿ�����
		GCHeroEquipGemManualActiveResp manualResp;
		manualResp.mPacketID = BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EquipGemUnlocked;
		Json::FastWriter writer;
		manualResp.mRespJsonStr = writer.write(root);
		

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(manualResp,connId);

		
		return false;
	}
	else
	{
		 EquipGemRuleTemplate * equipGemRuleTemp = EQUIPGEMRULE_TABLE().get(heroEquipDataIter->mValue->m_nEquipId);
		 if (equipGemRuleTemp == NULL)
		 {
			 LOG_WARN("equipGemRuleTemp not found!!");
			 return false;
		 }
		 
		 switch(gemSlot)
		 {
			case 1:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem1SlotBit, equipGemRuleTemp->mGemLevel1, equipGemRuleTemp->mCondKey1,equipGemRuleTemp->mCondValue1,errorId);
				break;
			case 2:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem2SlotBit, equipGemRuleTemp->mGemLevel2, equipGemRuleTemp->mCondKey2,equipGemRuleTemp->mCondValue2,errorId);
				break;
			case 3:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem3SlotBit, equipGemRuleTemp->mGemLevel3, equipGemRuleTemp->mCondKey3,equipGemRuleTemp->mCondValue3,errorId);
				break;
			case 4:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem4SlotBit, equipGemRuleTemp->mGemLevel4, equipGemRuleTemp->mCondKey4,equipGemRuleTemp->mCondValue4,errorId);
				break;
			case 5:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem5SlotBit, equipGemRuleTemp->mGemLevel5, equipGemRuleTemp->mCondKey5,equipGemRuleTemp->mCondValue5,errorId);
				break;
			case 6:
				dealGemManualActive(*(heroEquipDataIter->mValue), Gem6SlotBit, equipGemRuleTemp->mGemLevel6, equipGemRuleTemp->mCondKey6,equipGemRuleTemp->mCondValue6,errorId);
				break;
			default:
				break;
		 }

		 if(errorId == LynxErrno::None)
		 {
			 m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
			 //���ͳɹ��İ����ͻ���

			 GCHeroEquipGemManualActiveResp manualResp;
			 manualResp.mPacketID = BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP;
			 Json::Value root;
			 root["errorId"] = errorId;
			 root["equipPos"] = equipPos;
			 root["equipId"] = heroEquipDataIter->mValue->m_nEquipId;
			 root["gemPos"] = gemSlot;
			 root["gemId"] = 0;
			 root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
			 root["rmb"] = m_pPlayer->mPlayerData.mBaseData.m_nGold;

			 Json::FastWriter writer;
			 manualResp.mRespJsonStr = writer.write(root);

			 const ConnId& connId = m_pPlayer->getConnId();

			 NetworkSystem::getSingleton().sendMsg(manualResp,connId);

			 return true;
		 }
		 else
		 {
			 //�������߿ͻ����д���
			 GCHeroEquipGemManualActiveResp manualResp;
			 manualResp.mPacketID = BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP;
			 Json::Value root;
			 root["errorId"] = errorId;

			 if(gemSlot == 6 && equipGemRuleTemp->mCondKey6 == 3)
			 {
				 AchieveData * acvData = m_pPlayer->getAchieveManager().getSameEventAcvData(equipGemRuleTemp->mCondValue6);
				root["achieveId"] = equipGemRuleTemp->mCondValue6;
				root["achieveArg"] = acvData->m_nArg;
			 }

			 Json::FastWriter writer;
			 manualResp.mRespJsonStr = writer.write(root);

			 const ConnId& connId = m_pPlayer->getConnId();

			 NetworkSystem::getSingleton().sendMsg(manualResp,connId);

			return false;
		 }
		
		
		

		

		
		 
	}
}

void HeroEquipManager::checkEquipActive(UInt32 equipPos, UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter *equipMapIter, bool &changeFlag)
{
	HeroEquipRuleTemplate * heroEquipRuleTemp = HEROEQUIPRULE_TABLE().get(heroModelId);
	if (heroEquipRuleTemp == NULL)
	{
		LOG_WARN("heroEquipRuleTemp not found!!");
		return;
	}
	

	switch(equipPos)
	{
		case 1:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId1, heroEquipRuleTemp->mEquip1UnlockLv,changeFlag);
			}
			break;
		case 2:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId2, heroEquipRuleTemp->mEquip2UnlockLv, changeFlag);
			}
			break;
		case 3:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId3, heroEquipRuleTemp->mEquip3UnlockLv, changeFlag);
			}
			break;
		case 4:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId4, heroEquipRuleTemp->mEquip4UnlockLv,changeFlag);
			}
			break;
		case 5:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId5, heroEquipRuleTemp->mEquip5UnlockLv,changeFlag);
			}
			break;
		case 6:
			{
				dealEquipActive(heroModelId,equipMapIter,heroEquipRuleTemp->mEquipId6, heroEquipRuleTemp->mEquip6UnlockLv,changeFlag);
			}
			break;	
	}
}


void HeroEquipManager::dealEquipActive( UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter *equipMapIter, UInt64 equipId, UInt32 equipUnlockLv, bool &changeFlag )
{
	
	if(equipUnlockLv <= *m_pPlayerLv)
	{
		equipMapIter->mValue->m_nEquipLv = 1;
		equipMapIter->mValue->m_nEquipId = equipId;

		
		EquipGemRuleTemplate * equipGemRuleTemp = EQUIPGEMRULE_TABLE().get(equipId);
		if (equipGemRuleTemp == NULL)
		{
			LOG_WARN("equipGemRuleTemp not found!!");
			return;
		}

		dealGemActive(*equipMapIter->mValue, Gem1SlotBit,equipGemRuleTemp->mGemLevel1,equipGemRuleTemp->mCondKey1,changeFlag);

		dealGemActive(*equipMapIter->mValue, Gem2SlotBit, equipGemRuleTemp->mGemLevel2, equipGemRuleTemp->mCondKey2,changeFlag);
		dealGemActive(*equipMapIter->mValue, Gem3SlotBit, equipGemRuleTemp->mGemLevel3, equipGemRuleTemp->mCondKey3,changeFlag);

		dealGemActive(*equipMapIter->mValue, Gem4SlotBit, equipGemRuleTemp->mGemLevel4, equipGemRuleTemp->mCondKey4,changeFlag);
		dealGemActive(*equipMapIter->mValue, Gem5SlotBit, equipGemRuleTemp->mGemLevel5, equipGemRuleTemp->mCondKey5,changeFlag);
		dealGemActive(*equipMapIter->mValue, Gem6SlotBit, equipGemRuleTemp->mGemLevel6, equipGemRuleTemp->mCondKey6,changeFlag);
		
		m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
	}
}


void HeroEquipManager::initEquipGemBit(HeroEquipData &heroEquipData,  UInt32& level,UInt32 & condKey, UInt32 gemSlotBit)
{
	if(level <= 1 && condKey == 0)
	{
		heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | gemSlotBit;
	}
}

void HeroEquipManager::checkGemActive(HeroEquipData &heroEquipData,bool & changeFlag)
{
	EquipGemRuleTemplate * equipGemRuleTemp  = EQUIPGEMRULE_TABLE().get(heroEquipData.m_nEquipId);
	if (equipGemRuleTemp == NULL)
	{
		LOG_WARN("equipGemRuleTemp not found!!");
		return;
	}
	
	 dealGemActive(heroEquipData, Gem1SlotBit, equipGemRuleTemp->mGemLevel1, equipGemRuleTemp->mCondKey1,changeFlag);
	 dealGemActive(heroEquipData, Gem2SlotBit, equipGemRuleTemp->mGemLevel2, equipGemRuleTemp->mCondKey2,changeFlag);
	 dealGemActive(heroEquipData, Gem3SlotBit, equipGemRuleTemp->mGemLevel3, equipGemRuleTemp->mCondKey3,changeFlag);
	
	 dealGemActive(heroEquipData, Gem4SlotBit, equipGemRuleTemp->mGemLevel4, equipGemRuleTemp->mCondKey4,changeFlag);
	 dealGemActive(heroEquipData, Gem5SlotBit, equipGemRuleTemp->mGemLevel5, equipGemRuleTemp->mCondKey5,changeFlag);
	 dealGemActive(heroEquipData, Gem6SlotBit, equipGemRuleTemp->mGemLevel6, equipGemRuleTemp->mCondKey6,changeFlag);

}

void  HeroEquipManager::dealGemActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey, bool & changeFlag)
{
	//˵����ʯ��û�б�����
	if(!(heroEquipData.m_nActiveBit & dirtyBit))
	{
		if(*m_pPlayerLv >= gemUnlockLv && !gemCondKey)
		{
			//��ҵȼ����ڵ��ڱ�ʯ�۵ļ���ȼ���ȣ����ұ�ʯ���޶��⼤����������ô��ʯ�۱��Զ���������Ǵ���δ����״̬
			heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | dirtyBit;
			changeFlag = true;
		}
		
	}
	
}

//�����ֶ������ı�ʯ�ۣ�ͭǮ����Ԫ��������
void HeroEquipManager::dealGemManualActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey,UInt32 gemCondValue, errorId & errorId)
{
	//����ʯ�۵���bitλ��ĳһ����ʯ��bitλ��λ&���õ�0��δ��������0����
	if(heroEquipData.m_nActiveBit & dirtyBit)
	{
		//�Ѿ���������ʾ���
		errorId = LynxErrno::EquipGemUnlocked;
		return;
	}
	else
	{
		//��ҵȼ�С�ڱ�ʯ�۽����ȼ�
		if(*m_pPlayerLv < gemUnlockLv)
		{
			//��ҵȼ��������������
			errorId = LynxErrno::LevelNotEnough;
			return ;
		}

		if(!gemCondKey)
		{
			//��ҵȼ����ڵ��ڱ�ʯ�۵ļ���ȼ���ȣ����ұ�ʯ���޶��⼤����������ô��ʯ�۱��Զ���������Ǵ���δ����״̬
			heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | dirtyBit;
			errorId = LynxErrno::None;
			return;
		}
		else
		{
			switch(gemCondKey)
			{
				case 1:
					{
						UInt32 coin = m_pPlayer->getPlayerCoin();
						if(coin < gemCondValue)
						{
							//ͭǮ����
							errorId = LynxErrno::CoinNotEnough;
							return;
						}
						else
						{
							errorId = LynxErrno::None;
							heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | dirtyBit;
							coin -= gemCondValue;

							Goods goods;
							List<Goods> itemList;

							goods.resourcestype =AWARD_BASE;
							goods.subtype = AWARD_BASE_COIN;
							goods.num = 0 - gemCondValue;
							itemList.insertTail(goods);
							GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog15);

							return;
						}
					}
					break;
				case 2:
					{
						UInt32 gold = m_pPlayer->getPlayerGold();
						if(gold < gemCondValue)
						{
							errorId = LynxErrno::RmbNotEnough;
							return;
						}
						else
						{
							errorId = LynxErrno::None;
							heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | dirtyBit;
							
							Goods goods;
							List<Goods> itemList;

							goods.resourcestype =AWARD_BASE;
							goods.subtype = AWARD_BASE_GOLD;
								goods.num = 0 - gemCondValue;
							itemList.insertTail(goods);
							GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog79);


							return;
						}
					}
					break;
			 case 3:
				 {
						bool res = m_pPlayer->getAchieveManager().acvArgMatch(gemCondValue);
						
						if(res)
						{
							errorId = LynxErrno::None;
							heroEquipData.m_nActiveBit = heroEquipData.m_nActiveBit | dirtyBit;
						}
						else
						{
							errorId = LynxErrno::ConditionNotReach;
						}
						
						return;
				 }
				 break;
				default:
					{
						errorId = LynxErrno::InvalidParameter;
					}
					break;
			}


		}
	}
}

bool HeroEquipManager::lvCompare(UInt32 lv1, UInt32 lv2)
{
	return lv1 >= lv2;
}

List<HeroEquipData> * HeroEquipManager::getHeroEquipList(void)
{
	return m_pHeroEquipList;
}

std::string HeroEquipManager::convertDataToJson(void)
{
	Json::Value root;
	for(Map<UInt32, HeroEquipData *>::Iter * equipDataMapIter = m_mapHeroEquipData.begin(); equipDataMapIter != NULL; 
		equipDataMapIter = m_mapHeroEquipData.next(equipDataMapIter))
	{
		Json::Value equipRoot;
		equipRoot["id"] = equipDataMapIter->mValue->m_nEquipId;
		equipRoot["level"] = equipDataMapIter->mValue->m_nEquipLv;
		equipRoot["stepLevel"] = equipDataMapIter->mValue->m_nStep;
		equipRoot["pos"] = equipDataMapIter->mKey;
		UInt32 gemSlot = 1;
		for(List<UInt64>::Iter * gemIter = equipDataMapIter->mValue->m_nGemsList.begin(); gemIter != NULL; 
			gemIter = equipDataMapIter->mValue->m_nGemsList.next(gemIter) )
		{
			if(gemIter->mValue)
			{
				equipRoot["gem"].append(gemIter->mValue);
			}
			else
			{
				//��ʯ��λ0��Ҫ�ж��Ƿ���
				bool openFlag = getGemSlotOpenState(equipDataMapIter, gemSlot);
				//��λ�Ѿ�����
				if(openFlag)
				{
					equipRoot["gem"].append(gemIter->mValue);
				}
				else
				{
					//δ����
					equipRoot["gem"].append(-1);
				}
				
			}
			
			gemSlot++;
		}

		root.append(equipRoot);
		
	}
	
	Json::StyledWriter writer;
	return writer.write(root);
}

void HeroEquipManager::equipSetGem(UInt32 equipPos, UInt32 gemPos,UInt64 gemId)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipDataIter = m_mapHeroEquipData.find(equipPos);
	
	if(heroEquipDataIter)
	{
		bool isOpen = getGemSlotOpenState(heroEquipDataIter, gemPos);
		
		if(isOpen)
		{
			//��ʯλ��-1��list������0��5
			List<UInt64>::Iter * gemIter = heroEquipDataIter->mValue->m_nGemsList.getn(gemPos-1);
			if(gemIter)
			{
				if(gemIter->mValue)
				{
					//������ʾ����Ѿ���Ƕ�˱�ʯ
					sendSetRespToClient(LynxErrno::EquipGemHasExist,gemId,equipPos,gemPos);
					return;
				}

				NewGemTemplate * newGemTemp = NEWGEM_TABLE().get(gemId);

				assert(gemId);
				
				if(!newGemTemp)
				{
					sendSetRespToClient(LynxErrno::None,gemId,equipPos,gemPos);
					return;
				}

				if(heroEquipDataIter->mKey != newGemTemp->mEquipType)
				{
					//������ʾ��ұ�ʯ���ʹ���
					sendSetRespToClient(LynxErrno::EquipGemTypeError,gemId,equipPos,gemPos);
					return;
				}
				
				//���ı�ʯ
				bool delRes = m_pPlayer->getGemManager().playerConsumeGems(gemId,1);
				if(delRes)
				{
					gemIter->mValue = gemId;
					//����
					sendSetRespToClient(LynxErrno::None,gemId,equipPos,gemPos);
					m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
					return;
				}
				else
				{
					//������ʾ��ұ�ʯ������
					sendSetRespToClient(LynxErrno::InvalidParameter,gemId,equipPos,gemPos);
					return;
				}
				
				
				
			}
			else
			{
				//��ʾ��Ҳ�������
				sendSetRespToClient(LynxErrno::InvalidParameter,gemId,equipPos,gemPos);
				return;
			}
		}
		else
		{
			//��ʾ��ұ�ʯ��δ����
			sendSetRespToClient(LynxErrno::EquipGemSlotLock,gemId,equipPos,gemPos);
			return;
		}
		
	}
	else
	{
		//���ز�������
		sendSetRespToClient(LynxErrno::InvalidParameter,gemId,equipPos,gemPos);
		return;

	}
}


void HeroEquipManager::sendSetRespToClient(errorId  errorId, UInt64 gemId, UInt32 equipPos, UInt32 equipGemSlot)
{
	
	if(errorId == LynxErrno::None)
	{
		GCHeroEquipGemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = errorId;
		root["gemId"] = gemId;

		GemData * gemData = m_pPlayer->getGemManager().getGemData(gemId);

		root["gemCount"] = gemData->m_nCount;
		root["equipPos"] = equipPos;
		root["equipGemSlot"] = equipGemSlot;

		Json::StyledWriter writer;
		gemSetResp.mRespJsonStr = writer.write(root);
		gemSetResp.mPacketID = BOC_HERO_EQUIPGEM_SET_RESP;


		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

		cout << gemSetResp.mRespJsonStr;

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",equipPos,gemId);
		LogicSystem::getSingleton().write_log(LogType92,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	}
	else
	{
		GCHeroEquipGemSetResp gemSetResp;

		Json::Value root;
		root["errorId"] = errorId;
		
		Json::StyledWriter writer;
		gemSetResp.mRespJsonStr = writer.write(root);
		gemSetResp.mPacketID = BOC_HERO_EQUIPGEM_SET_RESP;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(gemSetResp,connId);

		cout << gemSetResp.mRespJsonStr;
	}
	

}


void HeroEquipManager::equipSetGemOnce(UInt32 equipPos)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipData = m_mapHeroEquipData.find(equipPos);

	if(heroEquipData)
	{
		//������װ���ı�ʯж������
		for(List<UInt64>::Iter * gemEquipIter = heroEquipData->mValue->m_nGemsList.begin(); gemEquipIter != NULL; 
			gemEquipIter = heroEquipData->mValue->m_nGemsList.next(gemEquipIter) )
		{
			//��ʯid��0����װ����ж����
			if(gemEquipIter->mValue)
			{
				m_pPlayer->getGemManager().playerGetGems(gemEquipIter->mValue,1);
				gemEquipIter->mValue = 0;
			}
			
		}

		//�ҵ�ͬ���಻ͬ�ȼ��ı�ʯ��map
		Map<UInt32, GemData*>* sameGemMap = m_pPlayer->getGemManager().getSameTypeGems(equipPos);

		Vector<UInt32> gemLvVec;
		if(sameGemMap)
		{
			//�ҵ��˾͹���һ���ȼ��ɸߵ��͵�vector
			for(Map<UInt32, GemData*>::Iter * lvGemIter = sameGemMap->begin(); lvGemIter != NULL; 
				lvGemIter = sameGemMap->next(lvGemIter))
			{
				gemLvVec.push_back(lvGemIter->mKey);
			}

			sort(gemLvVec.begin(), gemLvVec.end(), lvCompare);

		}
		else
		{
			//û�ҵ�˵��û�д��౦ʯ����ôֱ�ӷ��أ���ʾ�û�û�д��౦ʯ

			sendSetOnceRespToClient(LynxErrno::GemNotExist);
			return;
		}

		//�ҵ���װ��
		UInt32 gemSlot = 0;
		//��Ч��vector������ʼλ��
		UInt32 usefullIndex = 0;
		for(List<UInt64>::Iter * gemIter = heroEquipData->mValue->m_nGemsList.begin();gemIter != NULL;
			gemIter = heroEquipData->mValue->m_nGemsList.next(gemIter))
		{
			//��ʯ��λ����id��1~6
			gemSlot++;

			if(gemIter->mValue)
			{
				//�Ѿ���Ƕ�˱�ʯ������������һѭ��
				continue;
			}

			bool isOpen = getGemSlotOpenState(equipPos,gemSlot);
			if(!isOpen)
			{
				//�ж��Ƿ�����û������Ҳ����ѭ��
				continue;
			}


			//��ʯ��Ϊ����״̬
			if(isOpen)
			{


				for(UInt32 i = usefullIndex; i < gemLvVec.size(); i++)
				{
					Map<UInt32, GemData*>::Iter * lvGemIter = sameGemMap->find(gemLvVec[i]);
					assert(lvGemIter);
					if(lvGemIter->mValue->m_nCount)
					{
						m_pPlayer->getGemManager().playerConsumeGems(lvGemIter->mValue->m_nId,1);
						gemIter->mValue = lvGemIter->mValue->m_nId;
						m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);

						break;
					}
					else
					{
						//����Ϊ0,˵�������������ȼ��ı�ʯû���ˣ�Ϊ����߱���Ч�ʣ���¼��һ���ȼ��Ŀ�ʼ����
						usefullIndex = i+1;
					}
				}
			}
			else
			{
				//û�����Ĳ�λ����Ƕ

				continue;
			}

		}

		sendSetOnceRespToClient(LynxErrno::None,sameGemMap,heroEquipData);
	}
	else
	{
		//û�ҵ�,�������ͻ��˲�������

		sendSetOnceRespToClient(LynxErrno::InvalidParameter);
		return ;
	}
}



//ԭһ����Ƕ���˹��ܱ������Ժ�������� ���쳽
//void HeroEquipManager::equipSetGemOnce(UInt32 equipPos)
//{
//	Map<UInt32, HeroEquipData *>::Iter * heroEquipData = m_mapHeroEquipData.find(equipPos);
//	
//	if(heroEquipData)
//	{
//		
//		
//		//�ҵ�ͬ���಻ͬ�ȼ��ı�ʯ��map
//		Map<UInt32, GemData*>* sameGemMap = m_pPlayer->getGemManager().getSameTypeGems(equipPos);
//
//		Vector<UInt32> gemLvVec;
//		if(sameGemMap)
//		{
//			//�ҵ��˾͹���һ���ȼ��ɸߵ��͵�vector
//			for(Map<UInt32, GemData*>::Iter * lvGemIter = sameGemMap->begin(); lvGemIter != NULL; 
//				lvGemIter = sameGemMap->next(lvGemIter))
//			{
//				gemLvVec.push_back(lvGemIter->mKey);
//			}
//
//			sort(gemLvVec.begin(), gemLvVec.end(), lvCompare);
//			
//		}
//		else
//		{
//			//û�ҵ�˵��û�д��౦ʯ����ôֱ�ӷ��أ���ʾ�û�û�д��౦ʯ
//
//			sendSetOnceRespToClient(LynxErrno::GemNotExist);
//			return;
//		}
//
//		//�ҵ���װ��
//		UInt32 gemSlot = 0;
//		//��Ч��vector������ʼλ��
//		UInt32 usefullIndex = 0;
//		for(List<UInt64>::Iter * gemIter = heroEquipData->mValue->m_nGemsList.begin();gemIter != NULL;
//			gemIter = heroEquipData->mValue->m_nGemsList.next(gemIter))
//		{
//			//��ʯ��λ����id��1~6
//			gemSlot++;
//
//			if(gemIter->mValue)
//			{
//				//�Ѿ���Ƕ�˱�ʯ������������һѭ��
//				continue;
//			}
//			
//			bool isOpen = getGemSlotOpenState(equipPos,gemSlot);
//			if(!isOpen)
//			{
//				//�ж��Ƿ�����û������Ҳ����ѭ��
//				continue;
//			}
//			
//			
//			//��ʯ��Ϊ����״̬
//			if(isOpen)
//			{
//				
//
//				for(UInt32 i = usefullIndex; i < gemLvVec.size(); i++)
//				{
//					Map<UInt32, GemData*>::Iter * lvGemIter = sameGemMap->find(gemLvVec[i]);
//					assert(lvGemIter);
//					if(lvGemIter->mValue->m_nCount)
//					{
//						m_pPlayer->getGemManager().playerConsumeGems(lvGemIter->mValue->m_nId,1);
//						gemIter->mValue = lvGemIter->mValue->m_nId;
//						m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
//						
//						break;
//					}
//					else
//					{
//						//����Ϊ0,˵�������������ȼ��ı�ʯû���ˣ�Ϊ����߱���Ч�ʣ���¼��һ���ȼ��Ŀ�ʼ����
//						usefullIndex = i+1;
//					}
//				}
//			}
//			else
//			{
//				//û�����Ĳ�λ����Ƕ
//				
//				continue;
//			}
//			
//		}
//
//		sendSetOnceRespToClient(LynxErrno::None,sameGemMap,heroEquipData);
//	}
//	else
//	{
//        //û�ҵ�,�������ͻ��˲�������
//
//		sendSetOnceRespToClient(LynxErrno::InvalidParameter);
//		return ;
//	}
//}


void HeroEquipManager::sendSetOnceRespToClient(errorId errorId, Map<UInt32, GemData*>* sameGemMap , Map<UInt32, HeroEquipData *>::Iter * heroEquipData )
{
	GCHeroEquipGemSetOnceResp resp;
	resp.mPacketID = BOC_HERO_EQUIPGEM_SETONCE_RESP;
	
	if(errorId != LynxErrno::None)
	{
		Json::Value root;
		root["errorId"] = errorId;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		cout << resp.mRespJsonStr;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp,connId);

	}
	else
	{
		Json::Value root;
		root["errorId"] = errorId;
		UInt32 gemSlot = 1;
		for(List<UInt64>::Iter * equipGemIter = heroEquipData->mValue->m_nGemsList.begin(); equipGemIter != NULL; 
			equipGemIter = heroEquipData->mValue->m_nGemsList.next(equipGemIter))
		{
			if(equipGemIter->mValue)
			{
				root["changeEquip"]["gem"].append(equipGemIter->mValue);
			}
			else
			{
				bool isOpen  = getGemSlotOpenState(heroEquipData,gemSlot);
				
				if(isOpen)
				{
					//����Ϊ0
					root["changeEquip"]["gem"].append(equipGemIter->mValue);
				}
				else
				{
					//û����Ϊ-1
					root["changeEquip"]["gem"].append(-1);
				}

			}

			gemSlot ++;
			
		}
		
		root["changeEquip"]["pos"] = heroEquipData->mKey;
		root["changeEquip"]["quipId"] = heroEquipData->mValue->m_nEquipId;

		for(Map<UInt32, GemData*>::Iter * sameGemIter =  sameGemMap->begin(); sameGemIter != NULL; sameGemIter = sameGemMap->next(sameGemIter))
		{
			Json::Value gemRoot; 
			gemRoot["gemID"] = sameGemIter->mValue->m_nId;
			gemRoot["gemCount"] = sameGemIter->mValue->m_nCount;
			
			root["changeGems"].append(gemRoot);

		}

		
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		cout << resp.mRespJsonStr;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp,connId);
	}
}

void HeroEquipManager::equipGemLvUp(UInt32 equipPos, UInt32 gemSlot)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipDataIter = m_mapHeroEquipData.find(equipPos);
	const ConnId & connId = m_pPlayer->getConnId();

	if(heroEquipDataIter)
	{
		List<UInt64>::Iter * gemIDIter = heroEquipDataIter->mValue->m_nGemsList.getn(gemSlot-1);

		GemCombineTemplate * gemCombineTemplate = GEMCOMBINE_TABLE().get(gemIDIter->mValue);
		if (gemCombineTemplate == NULL)
		{
			LOG_WARN("gemCombineTemplate not found!!");
			GCHeroEquipGemLvUpResp gemLvUpResp;
			Json::Value root;
			root["errorId"] = LynxErrno::ClienServerDataNotMatch;
			Json::StyledWriter writer;
			gemLvUpResp.mRespJsonStr = writer.write(root);
			gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;

			NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);
			return;
		}
		GemData* gemData = m_pPlayer->getGemManager().getGemData(gemIDIter->mValue);
		
		if(gemCombineTemplate->mNextGem)
		{
			//�жϱ�ʯ�����Ƿ�С����Ҫ�����������С������ʾ�û���ʯ�������㣬�˴���Ҫ����-1����Ϊװ���ı�ʯ��һ��
			if(gemData->m_nCount < gemCombineTemplate->mAmount - 1)
			{
				GCHeroEquipGemLvUpResp gemLvUpResp;
				Json::Value root;
				root["errorId"] = LynxErrno::GemNotEnough;
				Json::StyledWriter writer;
				gemLvUpResp.mRespJsonStr = writer.write(root);
				gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;
	
				NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);
				
				return;
			}

			//װ���ϵı�ʯ��������Ҫ�������Ľӿ�
			bool delRes = m_pPlayer->getGemManager().playerConsumeGems(gemIDIter->mValue,gemCombineTemplate->mAmount - 1);
			if(!delRes)
			{
				//��ʾ��Ҳ�������
				GCHeroEquipGemLvUpResp gemLvUpResp;
				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
				Json::StyledWriter writer;
				gemLvUpResp.mRespJsonStr = writer.write(root);
				gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;

				NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);
				return ;
			}

			gemIDIter->mValue = gemCombineTemplate->mNextGem;
			m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
		
			//���ͳɹ������ݰ����ͻ���
			
			//��������ѵ
			LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT03,1);
				
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["equipPos"] = equipPos;
			root["gemPos"] = gemSlot;
			root["equipId"] = heroEquipDataIter->mValue->m_nEquipId;
			root["gemId"] = gemIDIter->mValue;
			root["changeGemId"] = gemData->m_nId;
			root["changeGemCount"] = gemData->m_nCount;

			GCHeroEquipGemLvUpResp gemLvUpResp;
			
			Json::StyledWriter writer;
			gemLvUpResp.mRespJsonStr = writer.write(root);
			gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;

			NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);
			cout << gemLvUpResp.mRespJsonStr;
			return ;
		}
		else
		{
			//��ʾ��ұ�ʯ�ﵽ��ߵȼ�

			GCHeroEquipGemLvUpResp gemLvUpResp;
			Json::Value root;
			root["errorId"] = LynxErrno::GemLevelLimit;
			Json::StyledWriter writer;
			gemLvUpResp.mRespJsonStr = writer.write(root);
			gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;

			NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);
			
			return ;
		}
	}
	else
	{
		//��ʾ��Ҳ�������
		GCHeroEquipGemLvUpResp gemLvUpResp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		gemLvUpResp.mRespJsonStr = writer.write(root);
		gemLvUpResp.mPacketID = BOC_HERO_EQUIPGEM_LVUP_RESP;

		NetworkSystem::getSingleton().sendMsg(gemLvUpResp,connId);

		return;

	}
}


void HeroEquipManager::equipGemUnload(UInt32 equipPos, UInt32 gemSlot)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipDataIter = m_mapHeroEquipData.find(equipPos);
	const ConnId & connId = m_pPlayer->getConnId();

	if(heroEquipDataIter)
	{
		List<UInt64>::Iter * gemIDIter = heroEquipDataIter->mValue->m_nGemsList.getn(gemSlot-1);
		
		if(!gemIDIter)
		{
			//��ʾ��Ҳ�������
			GCHeroEquipGemUnloadResp gemUnloadResp;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			gemUnloadResp.mRespJsonStr = writer.write(root);
			gemUnloadResp.mPacketID = BOC_HERO_EQUIPGEM_UNLOAD_RESP;

			NetworkSystem::getSingleton().sendMsg(gemUnloadResp,connId);
			return;
		}
		
		UInt64 gemId = gemIDIter->mValue;
		m_pPlayer->getGemManager().playerGetGems(gemIDIter->mValue,1);
		gemIDIter->mValue = 0;
		m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);
		GemData* gemData = m_pPlayer->getGemManager().getGemData(gemId);

		//��ʾ��Ҳ�������
		GCHeroEquipGemUnloadResp gemUnloadResp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["equipPos"] = equipPos;
		root["equipId"] = heroEquipDataIter->mValue->m_nEquipId;
		root["gemPos"] = gemSlot;
		root["gemId"] = gemId;
		root["gemCount"] = gemData->m_nCount;

		Json::StyledWriter writer;
		gemUnloadResp.mRespJsonStr = writer.write(root);
		gemUnloadResp.mPacketID = BOC_HERO_EQUIPGEM_UNLOAD_RESP;

		NetworkSystem::getSingleton().sendMsg(gemUnloadResp,connId);
		cout << gemUnloadResp.mRespJsonStr;

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",equipPos,gemId);
		LogicSystem::getSingleton().write_log(LogType98,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	}
	else
	{
		//��ʾ��Ҳ�������
		GCHeroEquipGemUnloadResp gemUnloadResp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		gemUnloadResp.mRespJsonStr = writer.write(root);
		gemUnloadResp.mPacketID = BOC_HERO_EQUIPGEM_UNLOAD_RESP;

		NetworkSystem::getSingleton().sendMsg(gemUnloadResp,connId);

		return;

	}
}


UInt64 HeroEquipManager::changeCharacterEquip(UInt64 charactorId)
{
	Map<UInt32, HeroEquipData *>::Iter * heroEquipDataIter = m_mapHeroEquipData.find(1);
	UInt64 equipId = heroEquipDataIter->mValue->m_nEquipId;
	//װ����λδ����
	if(!equipId)
	{
		return 0;
	}
	
	HeroEquipRuleTemplate * heroEquipRuleTemp = HEROEQUIPRULE_TABLE().get(charactorId);
	if (heroEquipRuleTemp == NULL)
	{
		LOG_WARN("heroEquipRuleTemp not found!!");
		return 0;
	}
	
	heroEquipDataIter->mValue->m_nEquipId =  heroEquipRuleTemp->mEquipId1;

	m_pPlayer->getPersistManager().setDirtyBit(HEROEQUIPDATABIT);

	return heroEquipRuleTemp->mEquipId1;
}
