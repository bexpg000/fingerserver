#include "JewelryManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "PersistSystem.h"
#include "FireConfirm/Gift.h"
using namespace Lynx;


JewelryManager::JewelryManager()
{
	m_pPlayer = NULL;
	m_pJewelryList = NULL;
	m_pJewelryEquipData = NULL;
}

JewelryManager:: ~JewelryManager()
{

}


bool JewelryManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	m_pJewelryList = &m_pPlayer->mPlayerData.mJewelryData.mJewelryList;
	
	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		m_jewelryDataMap.insert(jewelryDataIter->mValue.m_nJewelryUid, &jewelryDataIter->mValue);
	}

	m_nPlayerGuid = m_pPlayer->getPlayerGuid();

	m_pJewelryEquipData = & m_pPlayer->mPlayerData.mJewelryEquipData;

	return true;
}

void JewelryManager::release()
{
	m_pPlayer = NULL;
	m_pJewelryList = NULL;
	m_pJewelryEquipData = NULL;
	m_jewelryDataMap.clear();

}

void JewelryManager::postUpdateMsg(JewelryData * jewelryData, UInt64 playerUid)
{
	PersistUpdateJewelryNotify updateJewelryNotify;
	updateJewelryNotify.m_jewelryData = *jewelryData;
	updateJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(updateJewelryNotify, playerUid);
}

void JewelryManager::postAddMsg(JewelryData * jewelryData, UInt64 playerUid)
{
	PersistAddJewelryNotify addJewelryNotify;
	addJewelryNotify.m_jewelryData = *jewelryData;
	addJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(addJewelryNotify, playerUid);
}

void JewelryManager::postDelMsg(UInt64 jewelryUid, UInt64 playerUid)
{
	PersistDelJewelryNotify delJewelryNotify;
	delJewelryNotify.m_nJewelryUid = jewelryUid;
	delJewelryNotify.m_nPlayerUid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(delJewelryNotify, playerUid);
}

//1~5֮�������������ֵ��Χ2~5
UInt32 JewelryManager::getRandomValue(UInt32 minValue, UInt32 maxValue)
{
	UInt32 diffValue = maxValue - minValue;

	if (diffValue <= 0)//todo: 0 ��bug
	{
		return minValue;
	}

	UInt32 result = rand()%diffValue;

	return minValue + result + 1;

}

List<JewelryData *> JewelryManager::playerGetJewelry(const UInt64 &jewelryId,const UInt32 & count)
{
	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryId);
	
	List<JewelryData *> newJewelryDatas;

	if(!newJewelryTemp)
	{
		//�������ԣ�ֱ�ӷ���
		return newJewelryDatas;
	}
	
	

	for(UInt32 i = 0; i < count; i++)
	{
		JewelryData * jewelryData = addJewelrys(jewelryId);
		
		postAddMsg(jewelryData,m_nPlayerGuid);

		newJewelryDatas.insertTail(jewelryData);
	}

//	jewelryUpdateToClient(newJewelryDatas);

	return newJewelryDatas;
}

void JewelryManager::jewelrySet(UInt32 jewelryPos, UInt64 jewelryUid)
{
	List<UInt64>::Iter * equipJewelryIter = m_pJewelryEquipData->mJewelryList.getn(jewelryPos -1);
	const ConnId & connId = m_pPlayer->getConnId();

	bool isOpen = getJewelrySlotOpenState(jewelryPos);

	if(!isOpen)
	{
		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::JewelryLock;
		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(setResp,connId);
		return;
	}

	//if(equipJewelryIter->mValue)
	//{
	//	//��ʾ����Ѿ���������Ʒ
	//	GCJewelrySetResp setResp;
	//	setResp.mPacketID = BOC_JEWELRY_SET_RESP;
	//	Json::Value root;
	//	root["errorId"] = LynxErrno::JewelryEquiped;
	//	Json::StyledWriter writer;
	//	setResp.mRespJsonStr = writer.write(root);

	//	NetworkSystem::getSingleton().sendMsg(setResp,connId);
	//	return;
	//}

	Map<UInt64, JewelryData*>::Iter * jewelryDataIter = m_jewelryDataMap.find(jewelryUid);
	
	if(!jewelryDataIter)
	{
		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(setResp,connId);

		return;
	}

	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryDataIter->mValue->m_nJewelryId);

		if(newJewelryTemp->mEquipSlotIdx != jewelryPos)
		{
			GCJewelrySetResp setResp;
			setResp.mPacketID = BOC_JEWELRY_SET_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::SlotPosError;
			Json::StyledWriter writer;
			setResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(setResp,connId);

			return;
		}

		equipJewelryIter->mValue = jewelryUid;

		postJewelryEquipMsg();

		GCJewelrySetResp setResp;
		setResp.mPacketID = BOC_JEWELRY_SET_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["jewelryPos"] = jewelryPos;
		root["jewelryUid"] = jewelryUid;

		Json::StyledWriter writer;
		setResp.mRespJsonStr = writer.write(root);
		cout << setResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(setResp,connId);

		char dest[1024]={0};
		snprintf(dest,sizeof(dest),"%d,%d",jewelryPos,jewelryUid);
		LogicSystem::getSingleton().write_log(LogType89,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	
}


void JewelryManager::jewelryUnload(UInt32 jewelryPos)
{
	List<UInt64>::Iter * equipJewelryIter = m_pJewelryEquipData->mJewelryList.getn(jewelryPos -1);
	const ConnId & connId = m_pPlayer->getConnId();

	bool isOpen = getJewelrySlotOpenState(jewelryPos);

	if(!isOpen)
	{
		GCJewelryUnloadResp unloadResp;
		unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::JewelryLock;
		Json::StyledWriter writer;
		unloadResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(unloadResp,connId);
		return;
	}

	//��ʾ�����Ʒ��Ϊ��
	if(!equipJewelryIter->mValue)
	{
		GCJewelryUnloadResp unloadResp;
		unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::SlotEmpty;
		Json::StyledWriter writer;
		unloadResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(unloadResp,connId);
		return;
	}

	equipJewelryIter->mValue = (UInt64)0;
	postJewelryEquipMsg();

	GCJewelryUnloadResp unloadResp;
	unloadResp.mPacketID = BOC_JEWELRY_UNLOAD_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["jewelryPos"] = jewelryPos;
	root["jewelryUid"] = equipJewelryIter->mValue;

	Json::StyledWriter writer;
	unloadResp.mRespJsonStr = writer.write(root);
	cout << unloadResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(unloadResp,connId);

	char dest[1024]={0};
	snprintf(dest,sizeof(dest),"%d,%d",jewelryPos,equipJewelryIter->mValue);
	LogicSystem::getSingleton().write_log(LogType90,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	
}

void JewelryManager::jewelrySell(List<UInt64> sellList)
{

	Json::Value root;
	const ConnId & connId = m_pPlayer->getConnId();
	for(List<UInt64>::Iter* sellIter =  sellList.begin(); sellIter != NULL; sellIter = sellList.next(sellIter) )
	{
		Map<UInt64, JewelryData*>::Iter * jewelryDataIter = m_jewelryDataMap.find(sellIter->mValue);
		if(!jewelryDataIter)
		{
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;

			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}
		
		List<UInt64>::Iter * equipIter = m_pJewelryEquipData->mJewelryList.find(sellIter->mValue, m_pJewelryEquipData->mJewelryList.begin(), m_pJewelryEquipData->mJewelryList.end());
		if(equipIter)
		{
			//��װ���е���Ʒ
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;

			root["errorId"] = LynxErrno::JewelryEquiped;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}

		NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryDataIter->mValue->m_nJewelryId);
		bool success = playerCostJewelry(sellIter->mValue);
		if(success)
		{
			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = newJewelryTemp->mPrice;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog6);

			root["jewelrySellList"].append(sellIter->mValue);
		}
		else
		{
			GCJewelrySellResp sellResp;
			sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;
			
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			sellResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(sellResp,connId);
			return;
		}
		
	}


	GCJewelrySellResp sellResp;
	sellResp.mPacketID = BOC_JEWELRY_SELL_RESP;
	
	root["errorId"] = LynxErrno::None;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

	Json::StyledWriter writer;
	sellResp.mRespJsonStr = writer.write(root);
	NetworkSystem::getSingleton().sendMsg(sellResp,connId);
	cout << sellResp.mRespJsonStr;

	
}

JewelryData* JewelryManager::addJewelrys(const UInt64 &jewelryId)
{
	NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(jewelryId);
	
	UInt64 jewelryUid = LogicSystem::getSingleton().generateItemGuid();
	JewelryData jewelryData;
	jewelryData.m_nJewelryUid = jewelryUid;
	jewelryData.m_nJewelryId = jewelryId;
	generateJewelryAttr(&jewelryData, newJewelryTemp);
	List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->insertTail(jewelryData);
	m_jewelryDataMap.insert(jewelryUid,&jewelryDataIter->mValue);

	return &jewelryDataIter->mValue;
	
}

bool JewelryManager::delJewelrys(const UInt64& jewelryUid)
{
	//���ж��Ƿ��������Ʒ
	Map<UInt64, JewelryData*>::Iter * jewelryMapIter = m_jewelryDataMap.find(jewelryUid);
	if(!jewelryMapIter)
	{
		return false;
	}

	m_jewelryDataMap.erase(jewelryMapIter);
	

	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		if(jewelryDataIter->mValue.m_nJewelryUid == jewelryUid)
		{
			m_pJewelryList->erase(jewelryDataIter);
			return true;
		}
	}
		
	return true;
}

void JewelryManager::generateJewelryAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp)
{
	jewelryData->m_staticAttrKey = newJewelryTemp->mStaticAttr1Key;

	UInt32 staticAttrValue = getRandomValue(newJewelryTemp->mStaticAttr1ValueMin, newJewelryTemp->mStaticAttr1ValueMax);
	jewelryData->m_staticAttrValue = staticAttrValue;
	generateRandomAttr(jewelryData, newJewelryTemp);

}	

void JewelryManager::generateRandomAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp)
{
	switch(newJewelryTemp->mPolishCount)
	{
		case 1:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				
			}
			break;
		case 2:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
			}
			break;
		case 3:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
			}
			break;
		case 4:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr4);
			}
			break;
		case 5:
			{
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr1);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr2);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr3);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr4);
				addRandomAttribute(jewelryData, newJewelryTemp->mRandomAttr5);
			}
			break;
		default:
			break;
	}
}



void JewelryManager::addRandomAttribute(JewelryData* jewelryData, UInt32 randomID)
{
	if(!randomID)
	{
		JewelryAttr randomAttr;
		jewelryData->m_randomAttrList.insertTail(randomAttr);
		return;
	}

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

	UInt32 CritDefPercent =  CritDamagePercent + attrRandom->mCritDefRate;

	UInt32 randomRes = rand()%CritDefPercent;

	JewelryAttr randomAttr;

	randomAttr.mRandomID = attrRandom->mId;

	//������������ֵ���ڵ���critDamagePercent����ֵ��С��CritDefPercent����ֵ,��ô�������������ΪcritDef
	if(randomRes < CritDefPercent && randomRes >= CritDamagePercent)
	{
		randomAttr.mKey = BaseToughness;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDefMin,attrRandom->mCritDefMax);
		randomAttr.mValue = randomValue;
	}
	//����Ϊcritdamage
	else	if(randomRes >= CritRatePercent)
	{
		randomAttr.mKey = BaseCrit;
		UInt32 randomValue = getRandomValue(attrRandom->mCritDamageMin,attrRandom->mCritDamageMax);
		randomAttr.mValue = randomValue;
	}
	//����maxSPPercent���ֵ��С��CritRate������Ϊcritrate
	else if(randomRes >= MaxSPPercent)
	{
		randomAttr.mKey = Critrate;
		UInt32 randomValue = getRandomValue(attrRandom->mCritRateMin,attrRandom->mCritRateMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MaxMPPercent)
	{
		//����ΪMAXsp
		randomAttr.mKey = MaxSP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxSPMin,attrRandom->mMaxSPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MaxHPPercent)
	{
		//����ΪMaxMP
		randomAttr.mKey = MaxMP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxMPMin, attrRandom->mMaxMPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MFPercent)
	{
		//����Ϊ���HP
		randomAttr.mKey = MaxHP;
		UInt32 randomValue = getRandomValue(attrRandom->mMaxHPMin, attrRandom->mMaxHPMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= PFPercent)
	{
		//����ΪMFPercent
		randomAttr.mKey = MF;
		UInt32 randomValue = getRandomValue(attrRandom->mMFMin,attrRandom->mMFMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= MAPercent)
	{
		//����ΪPFPercent
		randomAttr.mKey = PF;
		UInt32 randomValue = getRandomValue(attrRandom->mPFMin, attrRandom->mPFMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= PAPercent)
	{
		//����ΪMAPercent
		randomAttr.mKey = MA;
		UInt32 randomValue = getRandomValue(attrRandom->mMAMin,attrRandom->mMAMax);
		randomAttr.mValue = randomValue;
	}
	else if(randomRes >= 0)
	{
		//����ΪPAPercent
		randomAttr.mKey = PA;
		UInt32 randomValue = getRandomValue(attrRandom->mPAMin,attrRandom->mPAMax);
		randomAttr.mValue = randomValue;
	}
	else
	{
		assert(false);
	}

	jewelryData->m_randomAttrList.insertTail(randomAttr);
}


bool JewelryManager::playerCostJewelry( UInt64 jewelryUid)
{
	bool flag = delJewelrys(jewelryUid);
	
	if(flag)
	{
		postDelMsg(jewelryUid,m_nPlayerGuid);
		return true;
	}
	
	return false;
}


void  JewelryManager::jewelryPolish(UInt64 objJewelryUid, UInt64 materialJewelryUid, UInt32 objRdAttrIndex)
{

	Map<UInt64, JewelryData *>::Iter * objJewelryIter = m_jewelryDataMap.find(objJewelryUid);
	Map<UInt64, JewelryData *>::Iter * materialJewelryIter = m_jewelryDataMap.find(materialJewelryUid);

	if(!objJewelryIter || !materialJewelryIter)
	{
		//���ʹ�����ʾ���ͻ���
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return ;
	}
	
	List<UInt64>::Iter * equipIter = m_pJewelryEquipData->mJewelryList.find(materialJewelryUid, m_pJewelryEquipData->mJewelryList.begin(), m_pJewelryEquipData->mJewelryList.end());
	if(equipIter)
	{
		//��װ���е���Ʒ
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::JewelryEquiped;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}
	

	JewelryData * objJewelryData = objJewelryIter->mValue;
	JewelryData * materialJewelryData = materialJewelryIter->mValue;

	NewJewelryTemplate * objJewelryTemp = NEWJEWELRY_TABLE().get(objJewelryData->m_nJewelryId);
	NewJewelryTemplate * materialJewelryTemp = NEWJEWELRY_TABLE().get(materialJewelryData->m_nJewelryId);

	if(objJewelryTemp->mRefreshCoin > m_pPlayer->mPlayerData.mBaseData.m_nCoin)
	{
		//��ʾ���ͭǮ����
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::CoinNotEnough;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return ;
	}

	//����ͭǮ

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_COIN;
	goods.num = 0 - objJewelryTemp->mRefreshCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog7);


	AttrPolishTemplate *materialPolishTemp = ATTRPOLISH_TABLE().get(materialJewelryTemp->mLvRequire);

	JewelryAttr *jewelryAttrFind = NULL;
	
	//�Ƿ��г�ɫ����
	bool orangeFlag = false;

	//�ڲ���װ���еĳ�ɫ����,������ʶ�Ӧ��������ɵ�map,��Ҫ����Ŀ��װ���г�ɫϴ���������ڵ���1��
	//���Ҳ���װ�����г�ɫ���Ե����
	Map<UInt32, JewelryAttr*> orangeRdMap;
	UInt32 orangeRate = 0;

	//�Ƿ�����ɫ����
	bool purpleFlag = false;
	//�ڲ���װ���е���ɫ���ԣ�������ʶ�Ӧ��������ɵ�map����Ҫ����Ŀ��װ������ɫϴ���������ڵ���1��
	//���Ҳ���װ��������ɫ���Ե����
	Map<UInt32, JewelryAttr*> purpleRdMap;
	UInt32 purpleRate = 0;

	//����װ����������ɫ�����ԣ�������ʶ�Ӧ��������ɵ�map����Ҫ����Ŀ��װ������ɫ�ͳ�ɫ���۴���Ϊ0��
	//���߲���װ����û����ɫ���ɫ���Եĸ���ͳ�����
	Map<UInt32, JewelryAttr*> totalRdMap;
	UInt32 totalRate = 0;

	Vector<JewelryAttr*> rdAttrVecs; 

	
	creatQualityMap(materialJewelryData,materialPolishTemp,orangeFlag,orangeRdMap,orangeRate, purpleFlag,purpleRdMap,purpleRate,totalRdMap,totalRate);

	if(totalRdMap.empty())
	{
		//��ʾ������ݴ�����Ϊ������Ʒû���������
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}

	//���жϳ�ɫ������г�ɫϴ�����۵Ĵ���
	if(objJewelryData->m_orangeTimes >= 1)
	{

		//��ֹ�߻����ģ�����ͬ��ɫ�߼��ֿ�
		if(orangeFlag)
		{
			orangeSuccess(orangeRdMap,objJewelryData,jewelryAttrFind,orangeRate);
		}
		else
		{
			//�������װ��û�г�ɫ���ԣ��ж�Ŀ��װ������ɫϴ���ۼƴ����Ƿ�Ϊ���ڵ���1
			if(objJewelryData->m_purpleTimes >= 1)
			{
				//Ŀ��װ������ɫϴ�����۴����ﵽ���ڵ���һ,�жϲ���װ�����Ƿ�����ɫ����
				if(purpleFlag)
				{
					//�������װ������ɫ����
					purpleSuccess(purpleRdMap,objJewelryData,jewelryAttrFind,purpleRate);
				}
				else
				{
					//�������װ����û����ɫ���ԣ���ô����������map�����
					totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
				}
			}
			else
			{
				//���Ŀ��װ������ɫϴ������Ϊ0����ɫ����>= 1,���Ҳ���װ��û�г�ɫ���ԣ���ô��Ҫ��ȡ����װ��ȫ�����Ե�����߼�
				totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);

			}

		}
	}
	else
	{
		//Ŀ��װ���ĳ�ɫϴ������Ϊ0���ж�Ŀ��װ����ɫ���۴����Ƿ���ڵ���1
		if(objJewelryData->m_purpleTimes >= 1)
		{
			//Ŀ��װ������ɫϴ�����۴����ﵽ���ڵ���һ,�жϲ���װ�����Ƿ�����ɫ����
			if(purpleFlag)
			{
				//�������װ������ɫ����
				purpleSuccess(purpleRdMap,objJewelryData,jewelryAttrFind,purpleRate);
			}
			else
			{
				//�������װ����û����ɫ���ԣ���ô����������map�����
				totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
			}
		}
		else
		{
			//Ŀ��װ����û����ɫϴ�����۴�����Ҳû�г�ɫϴ�����۴�������ô���������������ѡȡ
			totalAttrRandom(totalRdMap,objJewelryData,jewelryAttrFind,totalRate);
		}
	}

	assert(jewelryAttrFind);

	List<JewelryAttr>::Iter * objRdIter = objJewelryData->m_randomAttrList.getn(objRdAttrIndex);

	if(!objRdIter)
	{
		//��ʾ������ݴ���
		GCJewelryPolishResp polishResp;

		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;

		Json::StyledWriter writer;
		polishResp.mRespJsonStr = writer.write(root);
		polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

		const ConnId& connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(polishResp, connId);

		return;
	}
	

	assert(objRdIter);
	PolishPointsTemplate * findPolishPointsTemp = NULL;
	for(PolishPointsTemplateMap::Iter * polishPointsMapIter = POLISHPOINTS_TABLE().mMap.begin(); 
		polishPointsMapIter != NULL;
		polishPointsMapIter = POLISHPOINTS_TABLE().mMap.next(polishPointsMapIter))
	{
		if(m_pPlayer->mPlayerData.mBaseData.m_nLevel < polishPointsMapIter->mValue.mLevel)
		{
			break;
		}
		
		findPolishPointsTemp = &polishPointsMapIter->mValue;
	}

	//�����Ҹ��ڻ��ߵ���40������ô����ϴ������
	if(findPolishPointsTemp)
	{
		//����ϴ�����֣����ݲ���װ������ɫƷ�ʽ�������

		MaterialPointsTemplate *materialPointsTem =  MATERIALPOINTS_TABLE().get(materialJewelryTemp->mLvRequire);
		switch(materialJewelryTemp->mQuality)
		{

			

		case Blue:
			{

				objJewelryData->m_orangePoints += materialPointsTem->mBluePoints;
				objJewelryData->m_purplePoints += materialPointsTem->mBluePoints;
			}
			break;

		case Purple:
			{

				objJewelryData->m_orangePoints += materialPointsTem->mPurplePoints;
				objJewelryData->m_purplePoints += materialPointsTem->mPurplePoints;
			}
			break;

		case Orange:
			{
				
				objJewelryData->m_orangePoints+= materialPointsTem->mOrangePoints;
				objJewelryData->m_purplePoints+= materialPointsTem->mOrangePoints;
			}
			break;

		default:
			break;

		}


		//����ͳ��Ŀ��װ����ǿ�����ֺͳ�ɫ����ɫ���ۼƴ���


		//�����ɫ���Ե��ۼƴ���
		while(objJewelryData->m_orangePoints>=findPolishPointsTemp->mOrangePoints)
		{
			objJewelryData->m_orangePoints -= findPolishPointsTemp->mOrangePoints;
			objJewelryData->m_orangeTimes++;
		}

		//������ɫ���Ե��ۼƴ���
		while(objJewelryData->m_purplePoints >= findPolishPointsTemp->mPurplePoints)
		{
			objJewelryData->m_purplePoints -= findPolishPointsTemp->mPurplePoints;
			objJewelryData->m_purpleTimes++;
		}

		if(objJewelryData->m_orangeTimes>3)
		{
			objJewelryData->m_orangeTimes = 3;
		}

		if(objJewelryData->m_purpleTimes>3)
		{
			objJewelryData->m_purpleTimes = 3;
		}

	}
 
	//��Ŀ��װ����ԭ�����Ա�������ʱ��������ڷ����ͻ���,���ڸ�Ϊ������װ���ֶ���

	objJewelryData ->m_oldJewelryAttr = objRdIter->mValue;
	objJewelryData ->m_oldAttrIndex = objRdAttrIndex;

	//ϴ���������Ժ������¼����µ�������Ŀ��װ���е�Ʒ��

	UInt64 attrNewId = getNewAttrId(*jewelryAttrFind,objJewelryTemp);
	//�����滻���������ʱ���Ʒ������Բ���װ���ģ��������и���Ϊ���Ŀ��װ����
	objRdIter->mValue = *jewelryAttrFind;
	//������õ���Ʒ�ʸ�ֵ��Ŀ��װ��
	objRdIter->mValue.mRandomID = attrNewId;

	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	//���̱��
	postUpdateMsg(objJewelryData,playerUid);


	//�����ҵ�������������Ե���������0��ʼ
	UInt32 materialAttrIndexFind = 0;
	List<JewelryAttr>& materialRdAttrList =  materialJewelryData->m_randomAttrList;
	for(List<JewelryAttr>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.mKey == jewelryAttrFind->mKey && materialRdIter->mValue.mValue ==  jewelryAttrFind->mValue &&
			materialRdIter->mValue.mRandomID == jewelryAttrFind->mRandomID)
		{
			break;
		}
		materialAttrIndexFind++;
	}

	playerCostJewelry(materialJewelryUid);
	
	//���ͳɹ���Ϣ���ͻ���
	GCJewelryPolishResp polishResp;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objuid"] = objJewelryUid;
	root["materialuid"] = materialJewelryUid;
	root["newAttr"]["type"] = objRdIter->mValue.mKey;
	root["newAttr"]["attrId"] = objRdIter->mValue.mRandomID;
	root["newAttr"]["value"] =  objRdIter->mValue.mValue;

	root["oldAttr"]["type"] = objJewelryData->m_oldJewelryAttr.mKey;
	root["oldAttr"]["value"] = objJewelryData->m_oldJewelryAttr.mValue;
	root["oldAttr"]["attrId"] = objJewelryData->m_oldJewelryAttr.mRandomID;

	root["objAttrIndex"] = objRdAttrIndex;
	root["materialAttrIndex"] = materialAttrIndexFind;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

	Json::StyledWriter writer;
	polishResp.mRespJsonStr = writer.write(root);
	polishResp.mPacketID = BOC_JEWELRY_POLISH_RESP;

	const ConnId& connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(polishResp, connId);

	//cout << polishResp.mRespJsonStr;

	//��������ѵ
	LogicSystem::getSingleton().updateSevenDayTask(m_pPlayer->getPlayerGuid(),SDT10,1);

}

void JewelryManager::jewelryReset(UInt64 objJewelryUid )
{
	Map<UInt64, JewelryData*>::Iter * objJewelryIter = m_jewelryDataMap.find(objJewelryUid);
	if(!objJewelryIter)
	{
		LOG_INFO("equip not find polishResetEquip!!");

		Json::Value root;
		root["errorId"] = LynxErrno::ItemNotExist;
		
		Json::StyledWriter writer;

		GCJewelryResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}

	if(m_pPlayer->mPlayerData.mBaseData.m_nGold < PolishNeedCoin)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::RmbNotEnough;

		Json::FastWriter writer;

		GCJewelryResetResp resetResp;
		resetResp.mRespJsonStr = writer.write(root);
		resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

		const ConnId& connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);

		return ;
	}


	UInt32 & rdIndex = objJewelryIter->mValue->m_oldAttrIndex;
	JewelryAttr & rdAttribute = objJewelryIter->mValue->m_oldJewelryAttr;

	List<JewelryAttr>::Iter * rdAttrIter = objJewelryIter->mValue->m_randomAttrList.getn(rdIndex);
	assert(rdAttrIter);
	rdAttrIter->mValue = rdAttribute;

	postUpdateMsg(objJewelryIter->mValue,m_nPlayerGuid);
	

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = 0 - PolishNeedCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog7);

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["objRdAttrIndex"] = objJewelryIter->mValue->m_oldAttrIndex;

	root["oldAttr"]["type"] = rdAttrIter->mValue.mKey;
	root["oldAttr"]["value"] =  rdAttrIter->mValue.mValue;
	root["oldAttr"]["attrId"] = rdAttrIter->mValue.mRandomID;
	root["jewelryUid"] = objJewelryUid;
	root["coin"] = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
	root["gold"] = m_pPlayer->mPlayerData.mBaseData.m_nGold;

	Json::StyledWriter writer;

	GCJewelryResetResp resetResp;
	resetResp.mRespJsonStr = writer.write(root);
	resetResp.mPacketID = BOC_JEWELRY_RESET_RESP;

	const ConnId& connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(resetResp,connId);

	cout << resetResp.mRespJsonStr;
}

UInt64 JewelryManager::getPARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr )
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mPAMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mPAMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mPAMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mPAMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}
}


UInt64 JewelryManager::getMARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr )
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMAMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMAMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMAMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMAMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}
}


UInt64 JewelryManager::getPFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mPFMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mPFMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mPFMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mPFMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMFMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMFMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMFMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMFMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}


UInt64 JewelryManager::getMaxHPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxHPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxHPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxHPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxHPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMaxMPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxMPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxMPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxMPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxMPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getMaxSPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mMaxSPMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mMaxSPMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mMaxSPMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mMaxSPMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getCritRateRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritRateMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritRateMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritRateMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritRateMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}


UInt64 JewelryManager::getCirtDamageRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritDamageMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritDamageMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritDamageMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritDamageMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getBaseToughnessRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr)
{
	UInt64 whiteKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,White);
	UInt64 greenKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Green);
	UInt64 blueKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Blue);
	UInt64 purpleKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Purple);
	UInt64 orangeKey = ATTRRANDOM_TABLE().getAttrRandomID(jewelryLv,Orange);
	//��attrRandom������������ֵ���Ǹ���Χ sec

	AttrRandomTemplate * whiteTemplate = ATTRRANDOM_TABLE().get(whiteKey);
	if(randomAttr.mValue <= whiteTemplate->mCritDefMax)
	{
		return whiteTemplate->mId;
	}
	else 
	{
		AttrRandomTemplate * greenTemplate = ATTRRANDOM_TABLE().get(greenKey);
		if(randomAttr.mValue <= greenTemplate->mCritDefMax)
		{
			return greenTemplate->mId;
		}
		else
		{
			AttrRandomTemplate * blueTemplate = ATTRRANDOM_TABLE().get(blueKey);
			if(randomAttr.mValue <= blueTemplate->mCritDefMax)
			{
				return blueTemplate->mId;
			}
			else
			{
				AttrRandomTemplate * purpleTemplate = ATTRRANDOM_TABLE().get(purpleKey);
				if(randomAttr.mValue <= purpleTemplate->mCritDefMax)
				{
					return purpleTemplate->mId;
				}
				else
				{
					AttrRandomTemplate * orangeTemplate = ATTRRANDOM_TABLE().get(orangeKey);
					return orangeTemplate->mId;
				}
			}
		}
	}	
}

UInt64 JewelryManager::getNewAttrId(const JewelryAttr &randomAttr, NewJewelryTemplate * objectTemplate)
{
	//ͨ��equip���ҵ�װ����Ӧ�ĵȼ�

	UInt32 level = objectTemplate->mLvRequire;

	if(randomAttr.mKey == PA)
	{
		return getPARdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MA)
	{
		return getMARdId(level, randomAttr);
	}
	else if(randomAttr.mKey == PF)
	{
		return getPFRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MF)
	{
		return getMFRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxHP)
	{
		return getMaxHPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxMP)
	{
		return getMaxMPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == MaxSP)
	{
		return getMaxSPRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == Critrate)
	{
		return getCritRateRdId(level,randomAttr);
	}
	else if(randomAttr.mKey == BaseCrit)
	{
		return getCritRateRdId(level, randomAttr);
	}
	else if(randomAttr.mKey == BaseToughness)
	{
		//����
		return getBaseToughnessRdId(level, randomAttr);

	}
	

	return 0;
}



void JewelryManager::creatQualityMap(JewelryData * materialJewelryData,AttrPolishTemplate *materialPolishTemp,bool& orangeFlag,
					 Map<UInt32, JewelryAttr*> &orangeRdMap,
					 UInt32& orangeRate, bool& purpleFlag,Map<UInt32, JewelryAttr*> &purpleRdMap,
					 UInt32& purpleRate,
					 Map<UInt32, JewelryAttr*> &totalRdMap,UInt32& totalRate)
{
	List<JewelryAttr>& materialRdAttrList =  materialJewelryData->m_randomAttrList;


	for(List<JewelryAttr>::Iter * materialRdIter = materialRdAttrList.begin(); materialRdIter != NULL; 
		materialRdIter = materialRdAttrList.next(materialRdIter))
	{
		if(materialRdIter->mValue.mKey == 0)
		{
			continue;
		}

		AttrRandomTemplate * attrRdTemp = ATTRRANDOM_TABLE().get(materialRdIter->mValue.mRandomID);
		if (attrRdTemp == NULL)
		{
			LOG_WARN("attrRdTemp not found!!");
			return;
		}
		if(attrRdTemp->mQuality == Orange)
		{
			orangeFlag = true;

			//��ɫ���ֵ�ֶ�����
			orangeRate += materialPolishTemp->mOrange;
			//ȫ�������ֵ�ֶ�����
			totalRate += materialPolishTemp->mOrange;
			//��ӳ�ɫ,key Ϊ��ǰ������������ֵ����
			orangeRdMap.insert(orangeRate,&materialRdIter->mValue);
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else if(attrRdTemp->mQuality == Purple)
		{
			purpleFlag = true;
			//��ɫ���ֵ�ֶ�����
			purpleRate += materialPolishTemp->mPurple;
			//ȫ���͵����ֵ�ֶ�����
			totalRate += materialPolishTemp->mPurple;
			//�����ɫ
			purpleRdMap.insert(purpleRate,&materialRdIter->mValue);
			//����������Ĳ���map
			totalRdMap.insert(totalRate,&materialRdIter->mValue);

		}
		else
		{
			//���������Է������������map��
			if(attrRdTemp->mQuality == White)
			{
				totalRate += materialPolishTemp->mWhite;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(attrRdTemp->mQuality == Green)
			{
				totalRate += materialPolishTemp->mGreen;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
			else if(attrRdTemp->mQuality == Blue)
			{
				totalRate += materialPolishTemp->mBlue;
				totalRdMap.insert(totalRate,&materialRdIter->mValue);
			}
		}

	}


}



void JewelryManager::orangeSuccess(Map<UInt32, JewelryAttr*> &orangeRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &orangeRate)
{
	//orangrate��Ҫ���Ǵ������װ�����ж�����ɫ���Ե�Ȩ�أ������һ��ֵ
	UInt32 orangeValue = rand()%orangeRate;
	UInt32 orangeBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter* orangeRdIter =  orangeRdMap.begin(); orangeRdIter != NULL; 
		orangeRdIter = orangeRdMap.next(orangeRdIter))
	{

		//������ֵ��ĳ����ɫ���Ե��ٽ�ֵ���ôȡ���������ԣ�ֱ������������һ�����ҵ�һ������������
		if(orangeValue < orangeRdIter->mKey)
		{
			materialRdFind = orangeRdIter->mValue;
			//���ĵ���ɫһ��ϴ�����۴���
			objectItemData->m_orangeTimes--;
			break;
		}

	}

	assert(materialRdFind);
}


void JewelryManager::purpleSuccess(Map<UInt32, JewelryAttr*> &purpleRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &purpleRate)
{
	UInt32 purpleValue = rand()%purpleRate;
	UInt32 purpleBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter* purpleRdIter =  purpleRdMap.begin(); purpleRdIter != NULL; 
		purpleRdIter = purpleRdMap.next(purpleRdIter))
	{

		//������ֵ��ĳ����ɫ���Ե��ٽ�ֵ���ôȡ���������ԣ�ֱ������������һ�����ҵ�һ������������
		if(purpleValue < purpleRdIter->mKey)
		{
			materialRdFind = purpleRdIter->mValue;
			//���ĵ�һ����ɫϴ�����۴���
			objectItemData->m_purpleTimes--;
			break;
		}

	}
	//����һ�����ҵ��������
	assert(materialRdFind);
}


void JewelryManager::totalAttrRandom(Map<UInt32, JewelryAttr*> &totalRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &totalRate)
{
	UInt32 totalValue = rand()%totalRate;
	UInt32 totalBase = 0;
	for(Map<UInt32, JewelryAttr*>::Iter * totalRdIter  = totalRdMap.begin(); totalRdIter != NULL;
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


std::string JewelryManager::convertDataToJson()
{
	/*UInt64 time1 = TimeUtil::getTimeMilliSec();
	cout << "1111111" <<endl << time1 << endl;*/
	//����m_pjewelryList һ���ǿ� sec
	assert(m_pJewelryList);
	Json::Value root;
	
	for(List<JewelryData>::Iter * jewelryDataIter = m_pJewelryList->begin(); jewelryDataIter != NULL; 
		jewelryDataIter = m_pJewelryList->next(jewelryDataIter))
	{
		Json::Value jewelryRoot;
		jewelryRoot["uuid"] = jewelryDataIter->mValue.m_nJewelryUid;
		jewelryRoot["typeId"] = jewelryDataIter->mValue.m_nJewelryId;
		

		
		for(List<JewelryAttr>::Iter * jewelryAttrIter = jewelryDataIter->mValue.m_randomAttrList.begin(); jewelryAttrIter != NULL; 
			jewelryAttrIter = jewelryDataIter->mValue.m_randomAttrList.next(jewelryAttrIter))
		{
			Json::Value attrRoot;
			attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
			attrRoot["key"] = jewelryAttrIter->mValue.mKey;
            attrRoot["value"] = jewelryAttrIter->mValue.mValue;

			jewelryRoot["randomAttrList"].append(attrRoot);
			
		}
		
		 
		jewelryRoot["staticAttrKey"] = jewelryDataIter->mValue.m_staticAttrKey;
		jewelryRoot["staticAttrValue"] = jewelryDataIter->mValue.m_staticAttrValue;

		
		
		jewelryRoot["purplePoints"] = jewelryDataIter->mValue.m_purplePoints;

		jewelryRoot["purpleTimes"] = jewelryDataIter->mValue.m_purpleTimes;

		jewelryRoot["orangePoints"] = jewelryDataIter->mValue.m_orangePoints;

		jewelryRoot["orangeTimes"] = jewelryDataIter->mValue.m_orangeTimes;

		root["jewelryList"].append(jewelryRoot);
	}

	/*UInt64 time2 = TimeUtil::getTimeMilliSec() ;
	cout << "2222" <<endl<< time2 - time1 << endl;*/

	UInt32 jewelrySlot = 0;
	for(List<UInt64>::Iter * jewelryIter = m_pJewelryEquipData->mJewelryList.begin(); jewelryIter != NULL; 
		jewelryIter = m_pJewelryEquipData->mJewelryList.next(jewelryIter))
	{
		jewelrySlot++;
		if(jewelryIter->mValue)
		{
			root["equipJewelrys"].append(jewelryIter->mValue);
		}
		else
		{
			bool isOpen = getJewelrySlotOpenState(jewelrySlot);
			if(isOpen)
			{
				root["equipJewelrys"].append(0);
			}
			else
			{
				root["equipJewelrys"].append(-1);
			}
		}
	}

	//UInt64 time3 = TimeUtil::getTimeMilliSec();
	//cout << "3333" <<endl<< time3 - time2 << endl; 


	Json::StyledWriter writer;
	return writer.write(root);
}

void JewelryManager::postJewelryEquipMsg()
{
	PersistUpdateJewelryEquipNotify jewelryEquipMsg;
	jewelryEquipMsg.m_nPlayerUid = m_nPlayerGuid;
	jewelryEquipMsg.m_jewelryEquipData = *m_pJewelryEquipData;

	PersistSystem::getSingleton().postThreadMsg(jewelryEquipMsg, m_nPlayerGuid);
}

void JewelryManager::checkLvAcitve()
{
	bool change = false;

	HeroEquipRuleTemplate *heroEquipRule = HEROEQUIPRULE_TABLE().get(m_pPlayer->mPlayerData.mBaseData.m_nModelID);
	if (heroEquipRule == NULL)
	{
		LOG_WARN("heroEquipRule not found!!");
		return;
	}
	UInt32 &playerLv = m_pPlayer->mPlayerData.mBaseData.m_nLevel;

	bool isOpen1 = getJewelrySlotOpenState(1);
	if(!isOpen1)
	{
		if(heroEquipRule->mJewelry1UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry1OpenBit;
			change = true;
		}
	}

	bool isOpen2 = getJewelrySlotOpenState(2);

	if(!isOpen2)
	{
		if(heroEquipRule->mJewelry2UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry2OpenBit;
			change = true;
		}
	}

	bool isOpen3 = getJewelrySlotOpenState(3);

	if(!isOpen3)
	{
		if(heroEquipRule->mJewelry3UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry3OpenBit;
			change = true;
		}
	}

	bool isOpen4 = getJewelrySlotOpenState(4);

	if(!isOpen4)
	{
		if(heroEquipRule->mJewelry4UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry4OpenBit;
			change = true;
		}
	}

	bool isOpen5 = getJewelrySlotOpenState(5);

	if(!isOpen5)
	{
		if(heroEquipRule->mJewelry5UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry5OpenBit;
			change = true;
		}
	}

	bool isOpen6 = getJewelrySlotOpenState(6);

	if(!isOpen6)
	{
		if(heroEquipRule->mJewelry6UnlockLv <= playerLv )
		{
			m_pJewelryEquipData->mJewelryBit = m_pJewelryEquipData->mJewelryBit | Jewelry6OpenBit;
			change = true;
		}
	}

	if(change)
	{
		postJewelryEquipMsg();
	}
}


bool JewelryManager::getJewelrySlotOpenState(UInt32 jewelrySlot)
{
	bool isOpen = false;
	switch(jewelrySlot)
	{
		case 1:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry1OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 2:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry2OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 3:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry3OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 4:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry4OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}	
			break;
		case 5:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry5OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		case 6:
			{
				if(m_pJewelryEquipData->mJewelryBit & Jewelry6OpenBit)
				{
					isOpen = true;
				}
				else
				{
					isOpen = false;
				}
			}
			break;
		
	}

	return isOpen;
	
}

Json::Value JewelryManager::getJewelryJsonNode(Json::Value &jewelryRoot,JewelryData* jewelryData)
{
	
	jewelryRoot["uuid"] = jewelryData->m_nJewelryUid;
	jewelryRoot["typeId"] = jewelryData->m_nJewelryId;



	for(List<JewelryAttr>::Iter * jewelryAttrIter = jewelryData->m_randomAttrList.begin(); jewelryAttrIter != NULL; 
		jewelryAttrIter = jewelryData->m_randomAttrList.next(jewelryAttrIter))
	{
		Json::Value attrRoot;
		attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
		attrRoot["key"] = jewelryAttrIter->mValue.mKey;
		attrRoot["value"] = jewelryAttrIter->mValue.mValue;

		jewelryRoot["randomAttrList"].append(attrRoot);

	}


	jewelryRoot["staticAttrKey"] = jewelryData->m_staticAttrKey;
	jewelryRoot["staticAttrValue"] = jewelryData->m_staticAttrValue;



	jewelryRoot["purplePoints"] = jewelryData->m_purplePoints;

	jewelryRoot["purpleTimes"] = jewelryData->m_purpleTimes;

	jewelryRoot["orangePoints"] = jewelryData->m_orangePoints;

	jewelryRoot["orangeTimes"] = jewelryData->m_orangeTimes;

	
	return jewelryRoot;

}

void JewelryManager::jewelryUpdateToClient(List<JewelryData *> getList)
{
	
	const ConnId& connId = m_pPlayer->getConnId();
	Json::Value root;

	Json::Value tmpValue;

	for(List<JewelryData*>::Iter * jewelryDataIter = getList.begin(); jewelryDataIter != NULL; 
		jewelryDataIter = getList.next(jewelryDataIter))
	{
		Json::Value jewelryRoot;
		getJewelryJsonNode(jewelryRoot,jewelryDataIter->mValue);
		root.append(jewelryRoot);
	}

	Json::StyledWriter writer;

	GCJewelryUpdateResp updateResp;
	updateResp.mPacketID = BOC_JEWELRYUPDATE_RESP;
	updateResp.mRespJsonStr = writer.write(root);

	cout << updateResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(updateResp,connId);
	
}

void JewelryManager::diaoluoSend(List<JewelryData *> getList)
{
	Json::Value jsonValue;
	for(List<JewelryData *>::Iter * iter = getList.begin();iter !=  NULL; iter = getList.next(iter) )
	{	
		Json::Value tmpValue;
		getJewelryJsonNode(tmpValue,iter->mValue);
		jsonValue.append(tmpValue);
	}

	 LogicSystem::getSingleton().combinSendData(m_pPlayer->getPlayerGuid(),AWARD_JEWELRY,jsonValue);
	
}

UInt32 JewelryManager::getJewelryCount()
{
	return m_pJewelryList->size();
}


void JewelryManager::jewelryEquipOnce(Map<UInt32 ,UInt64> equipMap)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	for(Map<UInt32 ,UInt64>::Iter * equipIter = equipMap.begin(); equipIter != NULL; equipIter = equipMap.end())
	{
		if(equipIter->mKey > 6 || equipIter ->mKey < 0)
		{

			GCJewelrySetOnceResp setOnceResp;
			setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;
			
			setOnceResp.mRespJsonStr = writer.write(root);
			cout << setOnceResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(setOnceResp, connId);
			return ;
		}

		Map<UInt64, JewelryData*>::Iter * jewelryFindIter = m_jewelryDataMap.find(equipIter->mValue);
		if(!jewelryFindIter)
		{
			GCJewelrySetOnceResp setOnceResp;
			setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			Json::StyledWriter writer;

			setOnceResp.mRespJsonStr = writer.write(root);
			cout << setOnceResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(setOnceResp, connId);
			return;
		}


	}

	Json::Value root;

	UInt32 equipIndex = 1;

	for(List<UInt64>::Iter * equipListIter = m_pJewelryEquipData->mJewelryList.begin(); equipListIter != NULL;
		equipListIter = m_pJewelryEquipData->mJewelryList.next(equipListIter))
	{
		bool isOpen = getJewelrySlotOpenState(equipIndex);
	
		if(!isOpen)
		{
			Json::Value equipRoot;

			equipRoot["pos"] = equipIndex;
			equipRoot["jewelryuid"] = -1;

			root["equipdata"].append(equipRoot);
			equipIndex ++;
			continue;
		}
		
		Map<UInt32 ,UInt64>::Iter * exitIter = equipMap.find(equipIndex);
		//map���д�λ�õĸ���
		if(exitIter)
		{
			equipListIter->mValue = exitIter->mValue;
		}

		Json::Value equipRoot;

		equipRoot["pos"] = equipIndex;
		equipRoot["jewelryuid"] = equipListIter->mValue;

		root["equipdata"].append(equipRoot);
		equipIndex ++;
	}

	root["errorId"] = LynxErrno::None;

	Json::StyledWriter writer;

	GCJewelrySetOnceResp setOnceResp;
	setOnceResp.mRespJsonStr = writer.write(root);
	setOnceResp.mPacketID = BOC_JEWELRY_SETONCE_RESP;

	cout << setOnceResp.mRespJsonStr;

	NetworkSystem::getSingleton().sendMsg(setOnceResp,connId);

	postJewelryEquipMsg();
	

}