#include "AchievementManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

AchievementManager::AchievementManager() : mPlayer(NULL), /*mPlayerAchievementData(NULL),*/ mDrity(false)
{

}


AchievementManager::~AchievementManager()
{

}

bool 
AchievementManager::initial(Player* player)
{
    //Ŀǰע�ͳɾ�ϵͳ��صĳ�ʼ�� sec
	//if (player == NULL)
    //{
    //    LOG_WARN("Player pointer is NULL.");
    //    return false;
    //}

    //mPlayer = player;
    //mPlayerAchievementData = &player->mPlayerData.mAchievementData;
    //
    //// ��ʼ���ɳ��ɾ��б�
    //UInt8 achieveType = 0;
    //if (!mPlayerAchievementData->mCurAchievementList.size())
    //{
    //    for (AchievementTemplateMap::Iter* it = ACHIEVEMENT_TABLE().mMap.begin();
    //        it != NULL; it = ACHIEVEMENT_TABLE().mMap.next(it))
    //    {
    //        if (it->mValue.mType > achieveType && achieveType < ACHIEVEMENT_MAX_COUNT)
    //        {
    //            mPlayerAchievementData->mCompleteAchievementList.insertTail((UInt32)0);
    //            mPlayerAchievementData->mOldAchievementList.insertTail((UInt32)0);
    //            mPlayerAchievementData->mCurAchievementList.insertTail(it->mValue.mId);
    //            achieveType = it->mValue.mType;
    //        }
    //    }
    //    mDrity = true;
    //}
    //for (AchievementTemplateMap::Iter* it = ACHIEVEMENT_TABLE().mMap.begin();
    //    it != NULL; it = ACHIEVEMENT_TABLE().mMap.next(it))
    //{
    //    AchievementTemplateMap::Iter* nextIter = ACHIEVEMENT_TABLE().mMap.next(it);
    //    if (!nextIter)
    //    {
    //        mMaxLevelTemplateIdMap.insert(it->mValue.mType, it->mValue.mId);
    //        break;
    //    }
    //    if (it->mValue.mType != nextIter->mValue.mType)
    //    {
    //        mMaxLevelTemplateIdMap.insert(it->mValue.mType, it->mValue.mId);
    //    }
    //}


    return true;
}

void 
AchievementManager::update(const UInt64& accTime)
{
    if(mDrity)
    {
        mDrity = false;
      /*  PersistUpdatePlayerAchievementDataNotify dbNotify;
        dbNotify.mAchievementData = *mPlayerAchievementData;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());*/
    }
}

void 
AchievementManager::release()
{
    mPlayer = NULL;
  //  mPlayerAchievementData = NULL;
}

bool SortByAdvanced(const HeroData& lhs, const HeroData& rhs)
{
    return lhs.mBaseData.mAdvanced > rhs.mBaseData.mAdvanced;
}

bool SortByAwaken(UInt8 lhs, UInt8 rhs)
{
    return lhs > rhs;
}

bool SortByEnchant(UInt32 lhs, UInt32 rhs)
{
    return lhs > rhs;
}

void 
AchievementManager::checkAchievement(UInt8 type, UInt32 value /* value = 0 */)
{
    
	//sec Ŀǰ�رճɾ͸���ϵͳ
	//UpdateAchievementBaseDataNotify notify;
    //UInt32 conditionValue = 0;
    //if (type == 0 || type > ACHIEVEMENT_MAX_COUNT)
    //{
    //    LOG_WARN("invalid achievement type");
    //    return;
    //}
    //UInt8 index = 0;
    //for (List<UInt32>::Iter* indexIter = mPlayerAchievementData->mCurAchievementList.begin();
    //    indexIter != NULL; indexIter = mPlayerAchievementData->mCurAchievementList.next(indexIter))
    //{
    //    AchievementTemplate* indexTemplate = ACHIEVEMENT_TABLE().get(indexIter->mValue);
    //    if (!indexTemplate)
    //    {
    //        LOG_WARN("Failed to get achievement template by id:%u", indexIter->mValue);
    //    }
    //    if(indexTemplate->mType == type)
    //        break;
    //    index ++;
    //}
    //List<UInt32>::Iter* oldIter = mPlayerAchievementData->mOldAchievementList.getn(index);
    //if (!oldIter)
    //{
    //    LOG_WARN("Failed to get old achievement by type:%lu", index);
    //    return;
    //}
    //Map<UInt8, UInt32>::Iter* maxIter = mMaxLevelTemplateIdMap.find(type);
    //if (!maxIter)
    //{
    //    LOG_WARN("achievement not match, %u", type);
    //    return;
    //}
    //AchievementTemplate* maxTemplate = ACHIEVEMENT_TABLE().get(maxIter->mValue);
    //if (!maxTemplate)
    //{
    //    LOG_WARN("Failed to get achievement type by id: %u", maxIter->mValue);
    //    return;
    //}
    //// �������߼��ɾ��Ѿ����
    //if (oldIter->mValue == maxIter->mValue)
    //{
    //    notify.mBaseData = mPlayerAchievementData->mBaseData;
    //    mPlayer->sendMsgWithAck(notify);
    //    return;
    //}
    //switch(type)
    //{
    //    // ��ս���ɾ�
    //case ACHIEVEMENT_TYPE_BATTLE_VALUE:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mBaseData.mBattleValue;
    //    }
    //    break;
    //    // ӵ��4��Ӣ����
    //case ACHIEVEMENT_TYPE_OWN_STAR_FOUR_HERO_CNT:
    //    {
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            HeroTemplate* heroTemplate = HERO_TABLE().get(it->mValue.mBaseData.mTemplateId);
    //            if (!heroTemplate)
    //            {
    //                LOG_WARN("Failed to get herotemplate by id:%u", it->mValue.mBaseData.mTemplateId);
    //                continue;
    //            }
    //            if (heroTemplate->mHeroStar == 4)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // ӵ��5��Ӣ����
    //case ACHIEVEMENT_TYPE_OWN_STAR_FIVE_HERO_CNT:
    //    {
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            HeroTemplate* heroTemplate = HERO_TABLE().get(it->mValue.mBaseData.mTemplateId);
    //            if (!heroTemplate)
    //            {
    //                LOG_WARN("Failed to get herotemplate by id:%u", it->mValue.mBaseData.mTemplateId);
    //                continue;
    //            }
    //            if (heroTemplate->mHeroStar == 5)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // ӵ��6��Ӣ����
    //case ACHIEVEMENT_TYPE_OWN_STAR_SIX_HERO_CNT:
    //    {
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            HeroTemplate* heroTemplate = HERO_TABLE().get(it->mValue.mBaseData.mTemplateId);
    //            if (!heroTemplate)
    //            {
    //                LOG_WARN("Failed to get herotemplate by id:%u", it->mValue.mBaseData.mTemplateId);
    //                continue;
    //            }
    //            if (heroTemplate->mHeroStar == 6)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // Ӣ��ǿ����ߵȼ�
    //case ACHIEVEMENT_TYPE_HERO_REFINE_LEVEL:
    //    {
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            if (it->mValue.mBaseData.mLevel > conditionValue)
    //            {
    //                conditionValue = it->mValue.mBaseData.mLevel;
    //            }
    //        }
    //    }
    //    break;
    //    // һ��Ӣ�۾��ѵȼ�
    //case ACHIEVEMENT_TYPE_ONE_HERO_AWAKEN_LEVEL:
    //    {
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            if (it->mValue.mBaseData.mAwakeCnt > conditionValue)
    //            {
    //                conditionValue = it->mValue.mBaseData.mAwakeCnt;
    //            }
    //        }
    //    }
    //    break;
    //    // ����Ӣ�۾��ѵȼ�
    //case ACHIEVEMENT_TYPE_TWO_HERO_AWAKEN_LEVEL:
    //    {
    //        std::vector<UInt8> heroVector;
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            heroVector.push_back(it->mValue.mBaseData.mAwakeCnt);
    //        }
    //        std::sort(heroVector.begin(), heroVector.end(), &SortByAwaken);
    //        if (heroVector.size())
    //        {
    //            conditionValue = heroVector.size() >= 2 ? heroVector.at(1) : heroVector.back();
    //        }
    //        
    //    }
    //    break;
    //    // ����Ӣ�۾��ѵȼ�
    //case ACHIEVEMENT_TYPE_THREE_HERO_AWAKEN_LEVEL:
    //    {
    //        std::vector<UInt8> heroVector;
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            heroVector.push_back(it->mValue.mBaseData.mAwakeCnt);
    //        }
    //        std::sort(heroVector.begin(), heroVector.end(), &SortByAwaken);
    //        if (heroVector.size())
    //        {
    //            conditionValue = heroVector.size() >= 3 ? heroVector.at(2) : heroVector.back();
    //        }
    //        
    //    }
    //    break;
    //    // �ĸ�Ӣ�۾��ѵȼ�
    //case ACHIEVEMENT_TYPE_FOUR_HERO_AWAKEN_LEVEL:
    //    {
    //        std::vector<UInt8> heroVector;
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            heroVector.push_back(it->mValue.mBaseData.mAwakeCnt);
    //        }
    //        std::sort(heroVector.begin(), heroVector.end(), &SortByAwaken);
    //        if (heroVector.size())
    //        {
    //            conditionValue = heroVector.size() >= 4 ? heroVector.at(3) : heroVector.back();
    //        }
    //    }
    //    break;
    //    // ���Ӣ�۾��ѵȼ�
    //case ACHIEVEMENT_TYPE_FIVE_HERO_AWAKEN_LEVEL:
    //    {
    //        std::vector<UInt8> heroVector;
    //        for (List<HeroData>::Iter* it = mPlayer->mPlayerData.mHeroData.mHeroDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mHeroData.mHeroDataList.next(it))
    //        {
    //            heroVector.push_back(it->mValue.mBaseData.mAwakeCnt);
    //        }
    //        std::sort(heroVector.begin(), heroVector.end(), &SortByAwaken);
    //        if (heroVector.size())
    //        {
    //            conditionValue = heroVector.size() >= 5 ? heroVector.at(3) : heroVector.back();
    //        }
    //    }
    //    break;
    //    // �ϳ�Ӣ������
    //case ACHIEVEMENT_TYPE_COMBINE_HERO_CNT:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mAchievementData.mBaseData.mCombineHeroCnt;
    //    }
    //    break;
    //    // ӵ����ɫװ������
    //case ACHIEVEMENT_TYPE_OWN_EQUIP_BLUE_CNT:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            EquipTemplate* equipTemplate = EQUIP_TABLE().get(it->mValue.mTemplateId);
    //            if (!equipTemplate)
    //            {
    //                LOG_WARN("Failed to get equiptemplate by id:%u", it->mValue.mTemplateId);
    //                continue;
    //            }
    //            if (equipTemplate->mStarLevel == 3)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // ӵ����ɫװ������
    //case ACHIEVEMENT_TYPE_OWN_EQUIP_PURPLE_CNT:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            EquipTemplate* equipTemplate = EQUIP_TABLE().get(it->mValue.mTemplateId);
    //            if (!equipTemplate)
    //            {
    //                LOG_WARN("Failed to get equiptemplate by id:%u", it->mValue.mTemplateId);
    //                continue;
    //            }
    //            if (equipTemplate->mStarLevel == 4)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // ӵ�г�ɫװ������
    //case ACHIEVEMENT_TYPE_OWN_EQUIP_ORANGE_CNT:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            EquipTemplate* equipTemplate = EQUIP_TABLE().get(it->mValue.mTemplateId);
    //            if (!equipTemplate)
    //            {
    //                LOG_WARN("Failed to get equiptemplate by id:%u", it->mValue.mTemplateId);
    //                continue;
    //            }
    //            if (equipTemplate->mStarLevel == 5)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // ӵ�к�ɫװ������
    //case ACHIEVEMENT_TYPE_OWN_EQUIP_RED_CNT:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            EquipTemplate* equipTemplate = EQUIP_TABLE().get(it->mValue.mTemplateId);
    //            if (!equipTemplate)
    //            {
    //                LOG_WARN("Failed to get equiptemplate by id:%u", it->mValue.mTemplateId);
    //                continue;
    //            }
    //            if (equipTemplate->mStarLevel == 6)
    //            {
    //                conditionValue ++;
    //            }
    //        }
    //    }
    //    break;
    //    // װ����ǿ������
    //case ACHIEVEMENT_TYPE_REFINE_EQUIP_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mRefineEquipCnt;
    //    }
    //    break;
    //    // װ�����ǿ���ȼ�
    //case ACHIEVEMENT_TYPE_EQUIP_MAX_REFINE_LEVEL:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            if (it->mValue.mRefineLevel > conditionValue)
    //            {
    //                conditionValue = it->mValue.mRefineLevel;
    //            }
    //        }
    //    }
    //    break;
    //    // װ����߸�ħ�ȼ�
    //case ACHIEVEMENT_TYPE_ONE_EQUIP_ENCHANT_LEVEL:
    //    {
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            if (it->mValue.mEnchantLevel > conditionValue)
    //            {
    //                conditionValue = it->mValue.mEnchantLevel;
    //            }
    //        }
    //    }
    //    break;
    //    // ����װ���ﵽ��ħ�ȼ�
    //case ACHIEVEMENT_TYPE_TWO_EQUIP_ENCHANT_LEVEL:
    //    {
    //        std::vector<UInt32> equipVector;
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            equipVector.push_back(it->mValue.mEnchantLevel);
    //        }
    //        std::sort(equipVector.begin(), equipVector.end(), &SortByEnchant);
    //        if (equipVector.size())
    //        {
    //            conditionValue = equipVector.size() >= 2 ? equipVector.at(1) : equipVector.back();
    //        }
    //    }
    //    break;
    //    // ���װ���ﵽ��ħ�ȼ�
    //case ACHIEVEMENT_TYPE_FIVE_EQUIP_ENCHANT_LEVEL:
    //    {
    //        std::vector<UInt32> equipVector;
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            equipVector.push_back(it->mValue.mEnchantLevel);
    //        }
    //        std::sort(equipVector.begin(), equipVector.end(), &SortByEnchant);
    //        if (equipVector.size())
    //        {
    //            conditionValue = equipVector.size() >= 5 ? equipVector.at(4) : equipVector.back();
    //        }
    //    }
    //    break;
    //    // ʮ��װ���ﵽ��ħ�ȼ�
    //case ACHIEVEMENT_TYPE_TEN_EQUIP_ENCHANT_LEVEL:
    //    {
    //        std::vector<UInt32> equipVector;
    //        for (List<EquipData>::Iter* it = mPlayer->mPlayerData.mEquipData.mEquipDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mEquipData.mEquipDataList.next(it))
    //        {
    //            equipVector.push_back(it->mValue.mEnchantLevel);
    //        }
    //        std::sort(equipVector.begin(), equipVector.end(), &SortByEnchant);
    //        if (equipVector.size())
    //        {
    //            conditionValue = equipVector.size() >= 10 ? equipVector.at(10) : equipVector.back();
    //        }
    //    }
    //    break;
    //    // �ϳ�װ������
    //case ACHIEVEMENT_TYPE_COMBINE_EQUIP_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mCombineEquipCnt;
    //    }
    //    break;
    //    // ������ߵȼ�
    //case ACHIEVEMENT_TYPE_PET_LEVEL:
    //    {
    //        for (List<PetData>::Iter* it = mPlayer->mPlayerData.mPetData.mPetDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mPetData.mPetDataList.end())
    //        {
    //            if (it->mValue.mBaseData.mLevel > conditionValue)
    //            {
    //                conditionValue = it->mValue.mBaseData.mLevel;
    //            }
    //        }
    //    }
    //    break;
    //    // ��������Ǽ�
    //case ACHIEVEMENT_TYPE_PET_STAR_LEVEL:
    //    {
    //        for (List<PetData>::Iter* it = mPlayer->mPlayerData.mPetData.mPetDataList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mPetData.mPetDataList.end())
    //        {
    //            if (it->mValue.mBaseData.mStarLevel > conditionValue)
    //            {
    //                conditionValue = it->mValue.mBaseData.mStarLevel;
    //            }
    //        }
    //    }
    //    break;
    //    // ����ϴ�츳����
    //case ACHIEVEMENT_TYPE_PET_RESET_TALENT_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mPetResetTalentCnt;
    //    }
    //    break;
    //    // ��ͨ�ؿ�ͨ���½�
    //case ACHIEVEMENT_TYPE_CLEAR_NORMAL_TOLLGATE_CNT:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // ��Ӣ�ؿ�ͨ���½�
    //case ACHIEVEMENT_TYPE_CLEAR_HARD_TOLLGATE_CNT:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // ����Ǽ���������
    //case ACHIEVEMENT_TYPE_GET_TOLLGATE_STAR_CNT:
    //    {
    //        for (List<List<TollgateData> >::Iter* it = mPlayer->mPlayerData.mTollgateData.mSceneList.begin();
    //            it != NULL; it = mPlayer->mPlayerData.mTollgateData.mSceneList.end())
    //        {
    //            for (List<TollgateData>::Iter* iIter = it->mValue.begin();
    //                iIter != NULL; iIter = it->mValue.next(iIter))
    //            {
    //                conditionValue += iIter->mValue.mEasyData.mStarCount;
    //                conditionValue += iIter->mValue.mHardData.mStarCount;
    //            }
    //        }
    //    }
    //    break;
    //    // �������ܲ������
    //case ACHIEVEMENT_TYPE_JION_ARENA_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mJoinArenaCnt;
    //    }
    //    break;
    //    // ������
    //case ACHIEVEMENT_TYPE_ARENA_WIN_STREAK_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mArenaWinStreakCnt;
    //    }
    //    break;
    //    // �ƽ�ð�����˺�
    //case ACHIEVEMENT_TYPE_GOLD_ISLAND_TOTAL_DAMAGE:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalGoldIslandTotalDamage;
    //    }
    //    break;
    //    // ��ħ��ʦð���״�ͨ��
    //case ACHIEVEMENT_TYPE_DSMX_FIRST_CLEAR:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // �������ð���״�ͨ��
    //case ACHIEVEMENT_TYPE_SSLD_FIRST_CLEAR:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // ���֮��ð���״�ͨ��
    //case ACHIEVEMENT_TYPE_MFZD_FIRST_CLEAR:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // �ͱ���ð�յ�ͨ�ز���
    //case ACHIEVEMENT_TYPE_BABEL_TOWER_CLEAR_FLOOR:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mCompleteBabelCnt;
    //    }
    //    break;
    //    // ���ط�ӡͨ�ز���
    //case ACHIEVEMENT_TYPE_BZFY_CLEAR_TOLLGATE_CNT:
    //    {
    //        conditionValue = value;
    //    }
    //    break;
    //    // �ܳ鿨����
    //case ACHIEVEMENT_TYPE_DRAW_CARD_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalDrawCnt;
    //    }
    //    break;
    //    // ʮ�������
    //case ACHIEVEMENT_TYPE_TEN_DRAW_CARD_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalTenDrawCnt;
    //    }
    //    break;
    //    // ���ﵽ�ȼ�
    //case ACHIEVEMENT_TYPE_GOLD_MINE_LEVEL:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mProduceData.mGoldMineralLevel;
    //    }
    //    break;
    //    // ��ʯ��ﵽ�ȼ�
    //case ACHIEVEMENT_TYPE_DIAMOND_MINE_LEVEL:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mProduceData.mDiamondMineralLevel;
    //    }
    //    break;
    //    // ����Ȫˮ�ﵽ�ȼ�
    //case ACHIEVEMENT_TYPE_EXP_SPRING_LEVEL:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mProduceData.mExpMineralLevel;
    //    }   
    //    break;
    //    // �ۻ���ý��
    //case ACHIEVEMENT_TYPE_GET_TOTAL_GOLD:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalGetGold;
    //    }
    //    break;
    //    // �ۻ������ʯ
    //case ACHIEVEMENT_TYPE_GET_TOTAL_DIAMOND:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalGetDiamond;
    //    }
    //    break;
    //    // �ۻ��������
    //case ACHIEVEMENT_TYPE_GET_TOTAL_SOUL:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalGetSoul;
    //    }
    //    break;
    //    // �ۻ��������
    //case ACHIEVEMENT_TYPE_GET_TOTAL_HONOR:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalGetHonor;
    //    }
    //    break;
    //    // ͼ����ʥ����Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_HOLY_CNT:
    //    {
    //        
    //    }
    //    break;
    //    // ͼ���ڰ�����Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_DARK_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ����Ȼ����Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_NATURE_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ��������Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_FIRE_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ��������Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_THUNDER_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ��ˮ����Ӣ�۴ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_WATER_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ������װ���ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_WEAPON_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ��ͷ��װ���ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_HAT_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ������װ���ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ARMOR_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ������װ���ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_CUISH_CNT:
    //    {

    //    }
    //    break;
    //    // ͼ����Ʒװ���ﵽ����
    //case ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ORNAMENT_CNT:
    //    {

    //    }
    //    break;
    //    // ӵ�к��Ѹ���
    //case ACHIEVEMENT_TYPE_BUDDY_CNT:
    //    {
    //        conditionValue = mPlayer->mPlayerData.mBuddyData.mBuddyDataList.size();
    //    }
    //    break;
    //    // ������������
    //case ACHIEVEMENT_TYPE_SENDOUT_ENERGY_CNT:
    //    {
    //        conditionValue = mPlayerAchievementData->mBaseData.mTotalSendoutEnergy;
    //    }
    //    break;
    //    // ������ߴ�������
    //case ACHIEVEMENT_TYPE_OWN_GUILD:
    //    {
    //        conditionValue = 1;
    //    }
    //    break;
    //}
    //bool complete = false;
    //List<UInt32>::Iter* it = mPlayerAchievementData->mCurAchievementList.getn(index);
    //AchievementTemplate* curTemplate = ACHIEVEMENT_TABLE().get(it->mValue);
    //if (!curTemplate)
    //{
    //    LOG_WARN("failed to get achievement template by id: %u", it->mValue);
    //    return;
    //}
    //
    //if (curTemplate->mType != maxTemplate->mType)
    //{
    //    LOG_WARN("achievement type not match, cur type: %u, maxlevel type:%u", curTemplate->mType, maxTemplate->mType);
    //    return;
    //}
    //List<UInt32>::Iter* completeIter = mPlayerAchievementData->mCompleteAchievementList.getn(index);
    //if (!completeIter)
    //{
    //    LOG_WARN("Failed to get complete achievement ty type:%u", index);
    //    return;
    //}
    //while(conditionValue >= curTemplate->mConditionValue && curTemplate->mId <= maxTemplate->mId)
    //{
    //    if (completeIter->mValue < curTemplate->mId)
    //    {
    //        complete = true;
    //        CompleteAchievementNotify completeNotify;
    //        completeNotify.mTemplateId = curTemplate->mId;
    //        mPlayer->sendMsgWithAck(completeNotify);
    //        completeIter->mValue = curTemplate->mId;
    //    }
    //    curTemplate = ACHIEVEMENT_TABLE().get(curTemplate->mId + 1);
    //    if (!curTemplate)
    //    {
    //        break;
    //    }      
    //}
    //if (complete)
    //{
    //    UpdateAchievementStatusNotify statusNotify;
    //    statusNotify.mCompleteId = completeIter->mValue;
    //    statusNotify.mCurId = it->mValue;
    //    statusNotify.mIndex = index;
    //    mPlayer->sendMsgWithAck(statusNotify);
    //}
    //notify.mBaseData = mPlayerAchievementData->mBaseData;
    //mPlayer->sendMsgWithAck(notify);

    //mDrity = true;
}

ErrorId
AchievementManager::getAchievementAward(UInt32 templateId, bool& maxLevel, UInt32& curTemplateId)
{
    //sec
	//AchievementTemplate* achievementTemplate = ACHIEVEMENT_TABLE().get(templateId);
    //if (!achievementTemplate)
    //{
    //    LOG_WARN("Failed to get achievement template by id:%u", templateId);
    //    return LynxErrno::InvalidParameter;
    //}
    //UInt8 index = 0;
    //bool match = false;
    //for (List<UInt32>::Iter* it = mPlayerAchievementData->mCurAchievementList.begin();
    //    it != NULL; it = mPlayerAchievementData->mCurAchievementList.next(it))
    //{
    //    index ++;
    //    if (it->mValue == templateId)
    //    {   
    //        match = true;
    //        break;
    //    }
    //}
    //if (!match)
    //{
    //    LOG_WARN("invalid templateid : %u", templateId);
    //    return LynxErrno::InvalidParameter;
    //}
    //List<UInt32>::Iter* curIter = mPlayerAchievementData->mCurAchievementList.getn(index - 1);
    //List<UInt32>::Iter* completeIter = mPlayerAchievementData->mCompleteAchievementList.getn(index - 1);
    //List<UInt32>::Iter* oldIter = mPlayerAchievementData->mOldAchievementList.getn(index - 1);
    //if (!curIter || !completeIter || !oldIter)
    //{
    //    LOG_WARN("Failed to get achievement award, achievement data is error");
    //    return LynxErrno::InvalidParameter;
    //}
    //if (curIter->mValue > completeIter->mValue)
    //{
    //    LOG_WARN("achievement award has been received %u", templateId);
    //    return LynxErrno::InvalidParameter;
    //}
    //Map<UInt8, UInt32>::Iter* maxIter = mMaxLevelTemplateIdMap.find(index);
    //if (!maxIter)
    //{
    //    LOG_WARN("failed to get max achievement data by type:%u", index);
    //    return LynxErrno::InvalidParameter;
    //}
    //if (oldIter->mValue == maxIter->mValue)
    //{
    //    LOG_WARN("failed to get achievement award");
    //    return LynxErrno::InvalidParameter;
    //}
    //// ��ȡ����
    //oldIter->mValue = curIter->mValue;
    //if (curIter->mValue < maxIter->mValue)
    //{
    //    curIter->mValue ++;
    //}
    //else
    //{
    //    maxLevel = true;
    //}
    //switch(achievementTemplate->mAwardType)
    //{
    //case ITEM_TYPE:
    //    {
    //        mPlayer->getItemManager().createItem(achievementTemplate->mAwardTempalteId, achievementTemplate->mAwardCnt, 0);
    //    }
    //    break;
    //case EQUIP_TYPE:
    //    {
    //        mPlayer->getEquipManager().createEquip(achievementTemplate->mAwardTempalteId, 0);
    //    }   
    //    break;
    //case RUNE_TYPE:
    //    {
    //        mPlayer->getRuneManager().createRune(achievementTemplate->mAwardTempalteId, 0);
    //    }
    //    break;
    //case HERO_TYPE:
    //    {
    //        mPlayer->getHeroManager().createHero(achievementTemplate->mAwardTempalteId);
    //    }
    //    break;
    //case PET_TYPE:
    //    {
    //        mPlayer->getPetManager().createPet(achievementTemplate->mAwardTempalteId);
    //    }
    //    break;
    //case HERO_DEBRIS_TYPE:
    //    {
    //        mPlayer->getHeroDebrisManager().createHeroDebris(achievementTemplate->mAwardTempalteId, achievementTemplate->mAwardCnt, 0);
    //    }
    //    break;
    //case PET_DEBRIS_TYPE:
    //    {
    //        mPlayer->getPetDebrisManager().createPetDebris(achievementTemplate->mAwardTempalteId, achievementTemplate->mAwardCnt, 0);
    //    }
    //    break;
    //default:
    //    {
    //        LOG_WARN("unknow item type: %u", achievementTemplate->mAwardType);
    //    }
    //}
    //UpdateAchievementStatusNotify notify;
    //notify.mCompleteId = completeIter->mValue;
    //notify.mCurId = curIter->mValue;
    //notify.mOldId = oldIter->mValue;
    //notify.mIndex = index - 1;
    //mPlayer->sendMsgWithAck(notify);
    //mDrity = true;

    //curTemplateId = curIter->mValue;

    return LynxErrno::None;
}