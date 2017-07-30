#include "HoarStoneManager.h"
#include "Player.h"
#include "Table.h"
#include "GameServer.h"

using namespace Lynx;
HoarStoneManager::HoarStoneManager()
{
	m_pPlayer = NULL;
	m_pHoarStoneList = NULL;
	m_pRuneDataMap = NULL;
}	


HoarStoneManager:: ~HoarStoneManager()
{

}

bool HoarStoneManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	m_pHoarStoneList = &m_pPlayer->mPlayerData.mHoarStoneData.mHoarStoneList;

	m_pRuneDataMap = &m_pPlayer->mPlayerData.mHoarStoneData.mRuneDataMap;

	for(List<HoarStoneData>::Iter * hoarStoneIter = m_pHoarStoneList->begin(); hoarStoneIter != NULL;
		hoarStoneIter = m_pHoarStoneList->next(hoarStoneIter))
	{
		mHoarStoneMap.insert(hoarStoneIter->mValue.mHoarStoneId,&hoarStoneIter->mValue);
	}

	return true;

}

void HoarStoneManager::release()
{
	//ָ���ÿգ�map ���
	
	m_pPlayer = NULL;
	m_pHoarStoneList = NULL;
	m_pRuneDataMap = NULL;
}


std::string HoarStoneManager::convertDataToJson()
{
	Json::Value root;
	
	for(List<HoarStoneData>::Iter * hoarStoneIter = m_pHoarStoneList->begin(); hoarStoneIter != NULL; 
		hoarStoneIter = m_pHoarStoneList->next(hoarStoneIter))
	{
		Json::Value hoarStoneRoot;
		hoarStoneRoot["level"] = hoarStoneIter->mValue.mStoneLv;
		hoarStoneRoot["piece"] = hoarStoneIter->mValue.mPieceCount;
		hoarStoneRoot["star"] = hoarStoneIter->mValue.mStoneStar;

		for(List<UInt64>::Iter * equipIter = hoarStoneIter->mValue.mEquipList.begin(); equipIter != NULL; 
			equipIter = hoarStoneIter->mValue.mEquipList.next(equipIter))
		{
			hoarStoneRoot["equip"].append(equipIter->mValue);
		}

		root["base"].append(hoarStoneRoot);

	}


	std::stringstream mystream;

	for(RuneDataMap::Iter*  runeIter = m_pRuneDataMap->begin(); runeIter != NULL; runeIter = m_pRuneDataMap->next(runeIter))
	{
		mystream.clear();
		mystream.str("");

		mystream << runeIter->mKey;
		std::string runeKeyStr;
		mystream >> runeKeyStr; 

		root["equip"][runeKeyStr] = runeIter->mValue;
	}


	Json::StyledWriter writer;

	std::string jsonStr = writer.write(root);

	//cout << jsonStr;

	return jsonStr;
}


void HoarStoneManager::hoarStoneActive(UInt64 playerUid, UInt64 hoarstoneIdx)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarstoneIdx);
	
	if(mapHoarStoneIter->mValue->mStoneLv)
	{
		//��ʾ��ҽ�ʯ�ȼ���Ϊ0��Ҳ���Ǹý�ʯ�Ѿ�������
		GCHoarStoneActiveResp activeResp;
		activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneActived;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		activeResp.mRespJsonStr = respStr;
		
		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(activeResp,connId);
		cout << respStr;
		return ;
	}

	if(mapHoarStoneIter)
	{
		bool checkRes = checkCondition(hoarstoneIdx);
		
		if(checkRes)
		{
			UInt64 hoarStoneId = mapHoarStoneIter->mValue->mHoarStoneId;
			UInt32 hoarStoneLv = mapHoarStoneIter->mValue->mStoneLv;
		
			//���ݱ��и��ģ�ԭ���ǰ�����һ��id���ң������ڱ���id�Ϳ��Բ��ң�����ע��
			//UInt32 nextHoarStoneLv = hoarStoneLv + 1;

			//HoarstoneLvTemplate * hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextHoarStoneLv);
			
			//����id���ȼ�����temp
			HoarstoneLvTemplate * hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,hoarStoneLv);
			if(!hoarStoneLvTemp)
			{
				//��ʾ��ҽ�ʯ�ȼ�����ˣ�����������
				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::HoarStoneLvLimit;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;
				return;
			}

			UInt64 nowCoin = m_pPlayer->getPlayerCoin();
			if(nowCoin < hoarStoneLvTemp->mCostCoin)
			{
				//��ʾ��ҽ�Ҳ���

				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::CoinNotEnough;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;

				return;
			}
			
			if(mapHoarStoneIter->mValue->mPieceCount < hoarStoneLvTemp->mCostPiece)
			{
				//��ʾ�����Ƭ����
				GCHoarStoneActiveResp activeResp;
				activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;

				Json::StyledWriter writer;
				std::string respStr = writer.write(root);

				activeResp.mRespJsonStr = respStr;

				const ConnId& connId = m_pPlayer->getConnId();

				NetworkSystem::getSingleton().sendMsg(activeResp,connId);

				cout << respStr;
				
				return;
			}

			nowCoin -= hoarStoneLvTemp->mCostCoin;
			mapHoarStoneIter->mValue->mPieceCount -= hoarStoneLvTemp->mCostPiece;

			mapHoarStoneIter->mValue->mStoneLv = 1;


			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = 0 - hoarStoneLvTemp->mCostCoin;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog30);


			m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

			//���½�ʯ�ɾ�
			m_pPlayer->getAchieveManager().updateAchieveData(HONORSTONEACTIVE, 1);

			GCHoarStoneActiveResp activeResp;
			activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["hoarstoneIdx"] = hoarstoneIdx;
			root["coin"] = nowCoin;
			root["piece"] = mapHoarStoneIter->mValue->mPieceCount;

			Json::StyledWriter writer;
			std::string respStr = writer.write(root);

			activeResp.mRespJsonStr = respStr;

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(activeResp,connId);

			cout << respStr;

		}
		else
		{
			//��ʾ��Ҳ����㼤������
			GCHoarStoneActiveResp activeResp;
			activeResp.mPacketID = BOC_PLAYER_HOARSTONE_ACTIVE_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::HoarStoneCondition;

			Json::StyledWriter writer;
			std::string respStr = writer.write(root);

			activeResp.mRespJsonStr = respStr;

			const ConnId& connId = m_pPlayer->getConnId();

			NetworkSystem::getSingleton().sendMsg(activeResp,connId);

			cout << respStr;

		}

	}
	else
	{
		//��ʾ���id������

		GCHoarStoneActiveResp activeResp;
		activeResp.mPacketID = LynxErrno::InvalidParameter;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneCondition;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		activeResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(activeResp,connId);


		cout << respStr;

	}
}
UInt32 HoarStoneManager::getHoarStonePiece(UInt64 hoarStoneId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
	if(mapHoarStoneIter)
	{
		return mapHoarStoneIter->mValue->mPieceCount;		
	}
	else
	{
		return 0;
	}
}
bool HoarStoneManager::addHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);

	

	if(mapHoarStoneIter)
	{
		UInt32 lv = mapHoarStoneIter->mValue->mStoneLv;
		UInt32 pCount = mapHoarStoneIter->mValue->mPieceCount;
		HoarstoneLvTemplate * stonelvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId, lv);

		if(stonelvTemp == NULL)
		{
			LOG_WARN("stonelvTemp not found!!");
			return false;
		}


		//if(pCount + pieceCount > stonelvTemp->mToMax)
		//{
		//	//������Ƭ��ΪͭǮ
		//	mapHoarStoneIter->mValue->mPieceCount = stonelvTemp->mToMax;
		//	UInt32 more = pCount + pieceCount - stonelvTemp->mToMax;
		//	HoarstoneBaseTemplate * hoarBaseTemp = HOARSTONEBASE_TABLE().get(hoarStoneId);

		//	UInt64 curCoin = m_pPlayer->getPlayerCoin();
		//	curCoin += hoarBaseTemp->mExchange * more;
		//
		//	
		//}
		//else
		//{
					mapHoarStoneIter->mValue->mPieceCount += pieceCount;
		/*}*/
	
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);




		return true;
	}
	else
	{
		return false;
	}
}

bool HoarStoneManager::setHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);

	if(mapHoarStoneIter)
	{
		mapHoarStoneIter->mValue->mPieceCount = pieceCount;
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);
		return true;
	}
	else
	{
		return false;
	}
}




void HoarStoneManager::hoarStoneLvUp(UInt64 hoarStoneLvUpId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneLvUpId);
	
	if(!mapHoarStoneIter)
	{
		//��ʾ��ҽ�ʯid����ȷ

		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;
		return;
	}


	UInt64 hoarStoneId = mapHoarStoneIter->mValue->mHoarStoneId;
	UInt32 hoarStoneLv = mapHoarStoneIter->mValue->mStoneLv;

	UInt32 nextHoarStoneLv = hoarStoneLv + 1;

	HoarstoneLvTemplate * hoarStoneNextLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextHoarStoneLv);
    HoarstoneLvTemplate * hoarStoneCurLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,hoarStoneLv);

	if(!hoarStoneNextLvTemp)
	{
		//��ʾ��ҽ�ʯ�ﵽ��ߵȼ�
		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::HoarStoneLvLimit;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;
		return;

	}

	UInt64 nowCoin = m_pPlayer->getPlayerCoin();
	UInt32 &nowPieceCount = mapHoarStoneIter->mValue->mPieceCount;

	if(nowCoin < hoarStoneCurLvTemp->mCostCoin)
	{
		//��ʾ��ҽ�Ҳ���

		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;

		return;
	}	
			
	if(nowPieceCount < hoarStoneCurLvTemp->mCostPiece)
	{
		//��ʾ�����Ƭ����
		GCHoarStoneLvUpResp lvUpResp;
		lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::MaterialNotEnough;

		Json::StyledWriter writer;
		std::string respStr = writer.write(root);

		lvUpResp.mRespJsonStr = respStr;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

		cout << respStr;

		return;
	}

	//ԭ�����߼��Ǵ�����������Ŀǰ��Ϊֻ����һ��
	UInt32 addLvel = 0;		
	/*
			while(nowPieceCount >= hoarStoneLvTemp->mCostPiece && nowCoin >= hoarStoneLvTemp->mCostCoin)
			{
				addLvel++;
				nowCoin -= hoarStoneLvTemp->mCostCoin;
				nowPieceCount -= hoarStoneLvTemp->mCostPiece;
	

				UInt32 nextLv = hoarStoneLvTemp->mLevel + 1;
				hoarStoneLvTemp = HOARSTONELV_TABLE().reverseGet(hoarStoneId,nextLv);

				if(!hoarStoneLvTemp)
				{
					break;
				}
						
			}*/	
			
	//�°汾��Ϊǿ��ֻ����һ��
	if(nowPieceCount >= hoarStoneCurLvTemp->mCostPiece && nowCoin >= hoarStoneCurLvTemp->mCostCoin)
	{
		addLvel++;
		nowCoin -= hoarStoneCurLvTemp->mCostCoin;
		nowPieceCount -= hoarStoneCurLvTemp->mCostPiece;

	}

	mapHoarStoneIter->mValue->mStoneLv += addLvel;

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_COIN;
	goods.num = 0 - hoarStoneCurLvTemp->mCostCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog31);

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

	GCHoarStoneLvUpResp lvUpResp;
	lvUpResp.mPacketID = BOC_PLAYER_HOARSTONE_LVUP_RESP;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["hoarstoneIdx"] = hoarStoneId;
	root["coin"] = nowCoin;
	root["piece"] = mapHoarStoneIter->mValue->mPieceCount;
	root["targetLevel"] = mapHoarStoneIter->mValue->mStoneLv;
	root["levelAdd"] = addLvel;

	Json::StyledWriter writer;
	std::string respStr = writer.write(root);

	lvUpResp.mRespJsonStr = respStr;

	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(lvUpResp,connId);

	cout << respStr;
	//���½�ʯǿ���ճ�����
	m_pPlayer->getAchieveManager().updateDailyTaskData(DLYHORNORSTONELVUPCNT, 1 );

}

void HoarStoneManager::sendStarResp(errorId errorId,const ConnId& connId)
{
	GCHoarStoneStarResp starResp;
	starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
	Json::Value root;
	root["errorId"] = errorId;
	Json::StyledWriter writer;

	starResp.mRespJsonStr = writer.write(root);
	NetworkSystem::getSingleton().sendMsg(starResp,connId);
	cout << starResp.mRespJsonStr;
}

void HoarStoneManager::hoarStoneStarUp(UInt64 hoarStoneId)
{
	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
	UInt32 starLv = mapHoarStoneIter->mValue->mStoneStar;
	UInt32 nextStarLv = starLv + 1;
	HoarstoneStarTemplate * nextHoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, nextStarLv);
	HoarstoneStarTemplate * hoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, starLv);

	const ConnId& connId = m_pPlayer->getConnId();

	if(nextHoarStoneTemp)
	{
		//�жϽ�ʯװ���ķ����Ƿ���������
		List<UInt64> &runeList = mapHoarStoneIter->mValue->mEquipList;
		
		List<UInt64>::Iter * runeIter1 = runeList.getn(0); 
		if(runeIter1->mValue != hoarStoneTemp->needEquipId1)
		{
			//��ʾ��ҷ��Ĳ��㣬��������������
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
	
			return ;
		}

		List<UInt64>::Iter * runeIter2 = runeList.getn(1); 
		if(runeIter2->mValue != hoarStoneTemp->needEquipId2)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter3 = runeList.getn(2); 
		if(runeIter3->mValue != hoarStoneTemp->needEquipId3)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter4 = runeList.getn(3); 
		if(runeIter4->mValue != hoarStoneTemp->needEquipId4)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter5 = runeList.getn(4); 
		if(runeIter5->mValue != hoarStoneTemp->needEquipId5)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		List<UInt64>::Iter * runeIter6 = runeList.getn(5); 
		if(runeIter6->mValue != hoarStoneTemp->needEquipId6)
		{
			sendStarResp(LynxErrno::HoarStoneCondition,connId);
			return ;
		}

		for(List<UInt64>::Iter * runeDataIter = runeList.begin(); runeDataIter != NULL; runeDataIter = runeList.next(runeDataIter))
		{
			runeDataIter->mValue = 0;
		}

		mapHoarStoneIter->mValue->mStoneStar++;

		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

		//��ʾ������ǳɹ�

		GCHoarStoneStarResp starResp;
		starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["hoarstoneIdx"] = mapHoarStoneIter->mValue->mHoarStoneId;
		root["targetStar"] = mapHoarStoneIter->mValue->mStoneStar;
		root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

		Json::StyledWriter writer;

		starResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(starResp,connId);
		cout << starResp.mRespJsonStr;
		return;

	}
	else
	{
		//��ʾ��ҵ�ǰ�Ǽ��Ѿ�����
		sendStarResp(LynxErrno::HoarStoneLvMax,connId);
			return ;
	}
	
}

//ԭ�������߼�������
//
//void HoarStoneManager::hoarStoneStarUp(UInt64 hoarStoneId)
//{
//	Map<UInt64, HoarStoneData*>::Iter * mapHoarStoneIter = mHoarStoneMap.find(hoarStoneId);
//	UInt32 nextStarLv = mapHoarStoneIter->mValue->mStoneStar + 1;
//	HoarstoneStarTemplate * nextHoarStoneTemp = HOARSTONESTAR_TABLE().reverseGet(hoarStoneId, nextStarLv);
//	const ConnId& connId = m_pPlayer->getConnId();
//
//	if(nextHoarStoneTemp)
//	{
//		//�жϽ�ʯװ���ķ����Ƿ���������
//		List<UInt64> &runeList = mapHoarStoneIter->mValue->mEquipList;
//		
//		List<UInt64>::Iter * runeIter1 = runeList.getn(0); 
//		if(runeIter1->mValue != nextHoarStoneTemp->needEquipId1)
//		{
//			//��ʾ��ҷ��Ĳ��㣬��������������
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//	
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter2 = runeList.getn(1); 
//		if(runeIter2->mValue != nextHoarStoneTemp->needEquipId2)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter3 = runeList.getn(2); 
//		if(runeIter3->mValue != nextHoarStoneTemp->needEquipId3)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter4 = runeList.getn(3); 
//		if(runeIter4->mValue != nextHoarStoneTemp->needEquipId4)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter5 = runeList.getn(4); 
//		if(runeIter5->mValue != nextHoarStoneTemp->needEquipId5)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		List<UInt64>::Iter * runeIter6 = runeList.getn(5); 
//		if(runeIter6->mValue != nextHoarStoneTemp->needEquipId6)
//		{
//			sendStarResp(LynxErrno::HoarStoneCondition,connId);
//			return ;
//		}
//
//		for(List<UInt64>::Iter * runeDataIter = runeList.begin(); runeDataIter != NULL; runeDataIter = runeList.next(runeDataIter))
//		{
//			runeDataIter->mValue = 0;
//		}
//
//		mapHoarStoneIter->mValue->mStoneStar++;
//
//		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);
//
//		//��ʾ������ǳɹ�
//
//		GCHoarStoneStarResp starResp;
//		starResp.mPacketID = BOC_PLAYER_HOARSTONE_STARUP_RESP;
//		Json::Value root;
//		root["errorId"] = LynxErrno::None;
//		root["hoarstoneIdx"] = mapHoarStoneIter->mValue->mHoarStoneId;
//		root["targetStar"] = mapHoarStoneIter->mValue->mStoneStar;
//		root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
//
//		Json::StyledWriter writer;
//
//		starResp.mRespJsonStr = writer.write(root);
//		NetworkSystem::getSingleton().sendMsg(starResp,connId);
//		cout << starResp.mRespJsonStr;
//		return;
//
//	}
//	else
//	{
//		//��ʾ��ҵ�ǰ�Ǽ��Ѿ�����
//	}
//	
//}

void HoarStoneManager::addHoarStoneRune(UInt64 runeId, UInt32 count)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	if(runeDataIter)
	{
		runeDataIter->mValue += count;
	}
	else
	{
		m_pRuneDataMap->insert(runeId,count);
	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

}

UInt32 HoarStoneManager::getHoarStoneRune(UInt64 runeId)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	if(runeDataIter)
	{
		return runeDataIter->mValue;
	}
	else
	{
		return 0;
	}

}


void HoarStoneManager::GMHoarStoneRuneSet(UInt64 runeId, UInt32 count)
{
	RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runeId);
	
	if(runeDataIter)
	{
		
			runeDataIter->mValue = count;
		
			if(!count)
			{
				m_pRuneDataMap->erase(runeDataIter);
			}
	}
	else
	{
		if(count)
		{
			m_pRuneDataMap->insert(runeId,count);
		}
		
	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

}


void HoarStoneManager::equipHoarStoneRune(List<runePosData> runeDataList, UInt64 hoarStoneID)
{
	Map<UInt64, HoarStoneData*>::Iter * hoarStoneIter =  mHoarStoneMap.find(hoarStoneID);
	List<UInt64> & equipList = hoarStoneIter->mValue->mEquipList;
	const ConnId& connId = m_pPlayer->getConnId();

	for(List<runePosData>::Iter * runePosIter = runeDataList.begin(); runePosIter != NULL; runePosIter = runeDataList.next(runePosIter))
	{
		RuneDataMap::Iter * runeDataIter = m_pRuneDataMap->find(runePosIter->mValue.runeID);
		
		if(!runeDataIter)
		{
			//��ʾ�����Ϣ����
			GCHoarStoneRuneEquipResp equipResp;
			equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer; 

			equipResp.mRespJsonStr = writer.write(root);


			NetworkSystem::getSingleton().sendMsg(equipResp,connId);

			cout << equipResp.mRespJsonStr;
			return;
		}

		if(!runeDataIter->mValue)
		{
			//��ʾ�����Ϣ����
			GCHoarStoneRuneEquipResp equipResp;
			equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer;

			equipResp.mRespJsonStr = writer.write(root);


			NetworkSystem::getSingleton().sendMsg(equipResp,connId);

			cout << equipResp.mRespJsonStr;
			return;
		}

		runeDataIter->mValue --;
		
		if(!runeDataIter->mValue)
		{
			m_pRuneDataMap->erase(runeDataIter);
		}
		
		List<UInt64>::Iter * equipIter = equipList.getn(runePosIter->mValue.runePos);
		equipIter->mValue = runePosIter->mValue.runeID;

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",runePosIter->mValue.runePos,runePosIter->mValue.runeID);
		LogicSystem::getSingleton().write_log(LogType93,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	}

	m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

	GCHoarStoneRuneEquipResp equipResp;
	equipResp.mPacketID = BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["hoarstoneIdx"] = hoarStoneID;

	UInt32 id = 0;
	std::stringstream mystream;
	

	for(List<UInt64>::Iter * equipIter = equipList.begin(); equipIter != NULL; equipIter = equipList.next(equipIter) )
	{
		mystream.clear();
		mystream.str("");
		std::string idStr;
		mystream << id;
		mystream >> idStr;

		root["equipList"][idStr] = equipIter->mValue;
		id++;

		//���û����Ƕ���ģ���equipIter��valueֵΪ0,��ô����runeRoot�Ĺ���
		if(!equipIter->mValue)
		{
			continue;
		}

		Json::Value runeRoot;
		RuneDataMap::Iter * runemapIter = m_pRuneDataMap->find(equipIter->mValue);
		
		//����map�����������
		if(runemapIter)
		{
			runeRoot["runeId"] = runemapIter->mKey;
			runeRoot["runeCount"] = runemapIter->mValue;
			root["runeList"].append(runeRoot);

			
		}
		else
		{
			//����map��û�з���
			runeRoot["runeId"] = equipIter->mValue;
			runeRoot["runeCount"] = 0;
			root["runeList"].append(runeRoot);
		}
		

	}

	

	Json::StyledWriter writer;
	
	equipResp.mRespJsonStr = writer.write(root);

	//cout <<equipResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(equipResp,connId);

	cout << equipResp.mRespJsonStr;
	
}

bool HoarStoneManager::checkCondition(UInt64 hoarStoneIndex)
{
	HoarstoneBaseTemplate * hoarStoneBaseTemp = HOARSTONEBASE_TABLE().get(hoarStoneIndex);
	if (hoarStoneBaseTemp == NULL)
	{
		LOG_WARN("hoarStoneBaseTemp not found!!");
		return false;
	}
 
	if(hoarStoneBaseTemp->mActiveConditionType1)
	{
		 
		UInt64 activeCondition1Param1 = hoarStoneBaseTemp->mActiveCondition1Param1;
		UInt64 activeCondition1Param2 = hoarStoneBaseTemp->mActiveCondition1Param2;

	
		bool res1 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType1,activeCondition1Param1,activeCondition1Param2);
		if(res1)
		{
			if(hoarStoneBaseTemp->mActiveConditionType2)
			{
				UInt64 activeCondition2Param1 = hoarStoneBaseTemp->mActiveCondition2Param1;
				UInt64 activeCondition2Param2 = hoarStoneBaseTemp->mActiveCondition2Param2;

				bool res2 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType2,activeCondition2Param1,activeCondition2Param2);
				if(res2)
				{
					if(hoarStoneBaseTemp->mActiveConditionType3)
					{
						UInt64 activeCondition3Param1 = hoarStoneBaseTemp->mActiveCondition3Param1;
						UInt64 activeCondition3Param2 = hoarStoneBaseTemp->mActiveCondition3Param2;

						bool res3 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType3,activeCondition3Param1,activeCondition3Param2);
						if(res3)
						{
							if(hoarStoneBaseTemp->mActiveConditionType4)
							{
								UInt64 activeCondition4Param1 = hoarStoneBaseTemp->mActiveCondition4Param1;
								UInt64 activeCondition4Param2 = hoarStoneBaseTemp->mActiveCondition4Param2;

								bool res4 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType4,activeCondition4Param1,activeCondition4Param2);
								
								if(res4)
								{
									if(hoarStoneBaseTemp->mActiveConditionType5)
									{
										UInt64 activeCondition5Param1 = hoarStoneBaseTemp->mActiveCondition5Param1;
										UInt64 activeCondition5Param2 = hoarStoneBaseTemp->mActiveCondition5Param2;

										bool res5 = m_pPlayer->conditionRes(hoarStoneBaseTemp->mActiveConditionType5,activeCondition5Param1,activeCondition5Param2);
										if(res5)
										{
											return true;
										}
										else
										{
											return false;
										}
									}
									else
									{
										return true;
									}
								}
								else
								{
									return false;
								}
							}
							else
							{
								return true;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

void HoarStoneManager::combinHoarStoneRune(UInt64 dstRuneId,UInt32 dstRuneCount, UInt32 transferId)
{
	HoarstoneRuneTemplate *hoarStoneRuneTemp = HOARSTONERUNE_TABLE().get(dstRuneId);
	if (hoarStoneRuneTemp == NULL)
	{
		LOG_WARN("hoarStoneRuneTemp not found!!");
		return;
	}
	const ConnId& connId = m_pPlayer->getConnId();

	if(hoarStoneRuneTemp)
	{
		Map<UInt64, UInt32> runeMaterialMap;
		UInt64 rune1 = hoarStoneRuneTemp->mCombine1;
		if(rune1)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(rune1);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount1 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}
			runeMaterialMap.insert(rune1,hoarStoneRuneTemp->mAmount1 * dstRuneCount);
		}
		
		if(hoarStoneRuneTemp->mCombine2)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine2);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount2 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}
			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine2,hoarStoneRuneTemp->mAmount2 * dstRuneCount);
		}
		
		if(hoarStoneRuneTemp->mCombine3)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine3);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount3 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}
			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine3,hoarStoneRuneTemp->mAmount3 * dstRuneCount);
		}

		if(hoarStoneRuneTemp->mCombine4)
		{
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(hoarStoneRuneTemp->mCombine4);

			if(!runeDataIter)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;

				return;
			}

			if(runeDataIter->mValue < hoarStoneRuneTemp->mAmount4 * dstRuneCount)
			{
				GCHoarStoneRuneCombResp resp;
				resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::MaterialNotEnough;
				Json::StyledWriter writer;
				resp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(resp,connId);

				cout << resp.mRespJsonStr;
				return;
			}

			runeMaterialMap.insert(hoarStoneRuneTemp->mCombine3,hoarStoneRuneTemp->mAmount4 * dstRuneCount);
		}

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		std::stringstream mystream;

		for(Map<UInt64, UInt32>::Iter* runeMaterialIter = runeMaterialMap.begin();runeMaterialIter!=NULL;
			runeMaterialIter = runeMaterialMap.next(runeMaterialIter))
		{
			mystream.clear();
			mystream.str("");
			RuneDataMap::Iter *runeDataIter = m_pRuneDataMap->find(runeMaterialIter->mKey);
			runeDataIter->mValue -= runeMaterialIter->mValue;
			mystream << runeDataIter->mKey;
			std::string keyStr;
			mystream >> keyStr;
			root["changeList"][keyStr] = runeDataIter->mValue;

			if(!runeDataIter->mValue)
			{
				m_pRuneDataMap->erase(runeDataIter);
			}
		}
		
		root["coin"] = m_pPlayer->getPlayerCoin();

		addHoarStoneRune(dstRuneId,dstRuneCount);
		RuneDataMap::Iter *runeDataIterNew = m_pRuneDataMap->find(dstRuneId);
		mystream.clear();
		mystream.str("");
		mystream << dstRuneId;
		std::string keyStr;
		mystream >> keyStr;
		root["changeList"][keyStr] = runeDataIterNew->mValue;

		root["transferId"] = transferId;
		m_pPlayer->getPersistManager().setDirtyBit(HOARSTONEDATABIT);

		GCHoarStoneRuneCombResp resp;
		resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(resp,connId);

		cout << resp.mRespJsonStr;

	}
	else
	{
		//��ʾ���id����ȷ
		GCHoarStoneRuneCombResp resp;
		resp.mPacketID = BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(resp,connId);

		cout << resp.mRespJsonStr;
		return ;
	}
}


HoarStoneData * HoarStoneManager::getHoarStoneData(UInt64 hoarStoneID)
{
	Map<UInt64, HoarStoneData*>::Iter * hoarStoneIter =  mHoarStoneMap.find(hoarStoneID);
	if(hoarStoneIter)
	{
		return hoarStoneIter->mValue;
	}
	else
	{
		return NULL;
	}
}