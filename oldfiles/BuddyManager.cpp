#include "BuddyManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

BuddyManager::BuddyManager() : mPlayer(NULL), mPlayerBuddyData(NULL)
{

}


BuddyManager::~BuddyManager()
{

}

bool 
BuddyManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//playerdata ��mBuddyDataע���� sec
    //mPlayerBuddyData = &mPlayer->mPlayerData.mBuddyData;

    // ��PlayerInfo�м��غ�������
    //LogicSystem& logicSystem = LogicSystem::getSingleton();
    //const PlayerInfo* playerInfo = logicSystem.getPlayerInfo(player->getPlayerGuid());
    //if (!playerInfo)
    //{
    //    LOG_WARN("Failed to get player info by guid %llu", player->getPlayerGuid());
    //    return false;
    //}

    //// ���غ����б����� sec playerinfo��û�к��ѵ�list���Ժ������������
    //for (UInt32 i = 0; i < BUDDY_MAX_COUNT; i++)
    //{
    //    Guid buddyGuid = playerInfo->mBuddyList[i];
    //    
    //    if (buddyGuid)
    //    {
    //        const PlayerInfo* playerInfo = logicSystem.getPlayerInfo(buddyGuid);
    //        if (playerInfo)
    //        {
    //            BuddyData buddyData(*playerInfo);
    //            List<BuddyData>::Iter* buddyDataIter = mPlayerBuddyData->mBuddyDataList.insertTail(buddyData);
    //            mBuddyDataMap.insert(buddyGuid, &buddyDataIter->mValue);           
    //        }
    //    }
    //}

    //// ���Invited���� ����buddyData�Ĺ��캯����ע���� �Ժ���Ӷ�Ӧ�Ĺ��캯��sec
    //list<guid>* invitebuddylist = logicsystem::getsingleton().getinvitedbuddylist(mplayer->getplayerguid());
    //if (invitebuddylist)
    //{
    //    for (list<guid>::iter* iter = invitebuddylist->begin();
    //        iter != null; iter = invitebuddylist->next(iter))
    //    {
    //        const playerinfo* buddyinfo = logicsystem.getplayerinfo(iter->mvalue);
    //        if (buddyinfo)
    //        {
    //            buddydata buddydata(*buddyinfo);
    //            list<buddydata>::iter* jter = mplayerbuddydata->minvitebuddydatalist.inserttail(buddydata);
    //            minvitebuddydatamap.insert(jter->mvalue.mbuddyguid, &jter->mvalue);
    //        }
    //    }
    //}
    // 
    return true;
}

void 
BuddyManager::release()
{
    mBuddyDataMap.clear();
    mInviteBuddyDataMap.clear();
}

ErrorId 
BuddyManager::inviteBuddy(const Guid& buddyGuid)
{
   /* const PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
    if (!playerInfo)
    {
        return LynxErrno::PlayerInfoNotExist;
    }

    if (!LogicSystem::getSingleton().insertInvitedBuddy(buddyGuid, mPlayer->getPlayerGuid()))
    {
        return LynxErrno::BuddyHasExist;
    }

    LogicSystem::getSingleton().insertInviteBuddy(mPlayer->getPlayerGuid(), buddyGuid);*/

    return LynxErrno::None;
}

ErrorId 
BuddyManager::addBuddy(const Guid& buddyGuid)
{
 //   // ���ж��Ƿ��Ѿ����ҵĺ�����
 //   Map<Guid, BuddyData*>::Iter* iter = mBuddyDataMap.find(buddyGuid);
 //   if (iter)
 //   {
 //       return LynxErrno::BuddyHasExist;
 //   }

 //   // �ж϶Է������б��Ƿ���
 //   PlayerInfo* buddyPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
 //   if (!buddyPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }
 //   
 //   Guid* buddyGuidPointer = NULL;
	////�Ժ���playerinfo����Ӻ���list�ٴ�ע�ͣ� sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (buddyPlayerInfo->mBuddyList[i] == 0)
 //       {
 //           buddyGuidPointer = &buddyPlayerInfo->mBuddyList[i];
 //           break;
 //       }
 //   }

 //   if (buddyGuidPointer == NULL)
 //   {
 //       return LynxErrno::BuddyBuddyListHasFull;
 //   }

 //   // �ж��Լ������б��Ƿ��Ѿ�����
 //   PlayerInfo* myPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
 //   if (!myPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }

	//
 //   Guid* playerGuidPointer = NULL;
	////�Ժ���playerinfo����Ӻ���list�ٴ�ע�ͣ� sec
	//
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (myPlayerInfo->mBuddyList[i] == 0)
 //       {
 //           playerGuidPointer = &myPlayerInfo->mBuddyList[i];
 //           break;
 //       }
 //   }

 //   if (playerGuidPointer == NULL)
 //   {
 //       return LynxErrno::PlayerBuddyListHasFull;
 //   }

 //   *buddyGuidPointer = mPlayer->getPlayerGuid();
 //   *playerGuidPointer = buddyGuid;

 //   // ɾ������˫��֮ǰ�������ϵ
 //   LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), buddyGuid);
 //   LogicSystem::getSingleton().removeInviteBuddy(mPlayer->getPlayerGuid(), buddyGuid);
 //   LogicSystem::getSingleton().removeInvitedBuddy(buddyGuid, mPlayer->getPlayerGuid());
 //   LogicSystem::getSingleton().removeInviteBuddy(buddyGuid, mPlayer->getPlayerGuid());

 //   // ͬ��DB�����б�
 //   PersistUpdatePlayerInfoNotify dbNotify;
 //   dbNotify.mPlayerInfo = *myPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, myPlayerInfo->mPlayerGuid);

 //   dbNotify.mPlayerInfo = *buddyPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, buddyPlayerInfo->mPlayerGuid);

    return LynxErrno::None;
}

ErrorId 
BuddyManager::delBuddy(const Guid& buddyGuid)
{
    //�Ӻ����б���ɾ�����
 //   PlayerInfo* buddyPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(buddyGuid);
 //   if (!buddyPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }
 //  // �Ժ���playerinfo����Ӻ���list�ٴ�ע�ͣ� sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (buddyPlayerInfo->mBuddyList[i] == mPlayer->getPlayerGuid())
 //       {
 //           buddyPlayerInfo->mBuddyList[i] = 0;
 //           break;
 //       }
 //   }

 //   // ���Լ��б���ɾ������
 //   PlayerInfo* myPlayerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
 //   if (!myPlayerInfo)
 //   {
 //       return LynxErrno::PlayerInfoNotExist;
 //   }

	////�Ժ���playerinfo����Ӻ���list�ٴ�ע�ͣ� sec
 //   for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
 //   {
 //       if (myPlayerInfo->mBuddyList[i] == buddyGuid)
 //       {
 //           myPlayerInfo->mBuddyList[i] = 0;
 //           break;
 //       }
 //   }

 //   // ͬ��DB�����б�
 //   PersistUpdatePlayerInfoNotify dbNotify;
 //   dbNotify.mPlayerInfo = *myPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, myPlayerInfo->mPlayerGuid);

 //   dbNotify.mPlayerInfo = *buddyPlayerInfo;
 //   PersistSystem::getSingleton().postThreadMsg(dbNotify, buddyPlayerInfo->mPlayerGuid);

    return LynxErrno::None;
}

ErrorId 
BuddyManager::getRecommendBuddyList(List<BuddyData>& recommendBuddyList)
{
    List<Guid> buddyList;
    LogicSystem::getSingleton().getRecommendBuddyList(mPlayer, buddyList);


	//�Ժ���Ӻ��ѵĹ��캯�������캯���Ĳ�����playerinfo��Ŀǰ��ע��sec
   /* for (List<Guid>::Iter* iter = buddyList.begin();
        iter != NULL; iter = buddyList.next(iter))
    {
        const PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(iter->mValue);
        if (playerInfo)
        {
            BuddyData buddyData(*playerInfo);
            recommendBuddyList.insertTail(buddyData);
        }
    }*/

    return LynxErrno::None;
}

ErrorId 
BuddyManager::syncInviteBuddyList(List<BuddyData>& addBuddyList, 
                                   List<BuddyData>& changeBuddyList,
                                   List<Guid>& delBuddyList)
{
	//ɾ�����������б��ṹ��������û���壬Ŀǰ��ע��sec
    //LogicSystem& logicSystem = LogicSystem::getSingleton();
    //List<Guid>* inviteBuddyDataList = LogicSystem::getSingleton().getInvitedBuddyList(mPlayer->getPlayerGuid());
    //if (!inviteBuddyDataList)
    //{
    //    return LynxErrno::None;
    //}

    //// �������ӵ�
    //for (List<Guid>::Iter* iter = inviteBuddyDataList->begin();
    //    iter != NULL; )
    //{
    //    const PlayerInfo* buddyInfo = logicSystem.getPlayerInfo(iter->mValue);
    //    if (!buddyInfo)
    //    {
    //        LOG_WARN("Failed to get player info %llu, del invite buddy", iter->mValue);
    //        iter = inviteBuddyDataList->erase(iter);
    //    }
    //    else
    //    {
    //        Map<Guid, BuddyData*>::Iter* jter = mInviteBuddyDataMap.find(iter->mValue);
    //        if (jter == NULL)
    //        {
    //            BuddyData buddyData(*buddyInfo);
    //            List<BuddyData>::Iter* buddyIter = mPlayerBuddyData->mInviteBuddyDataList.insertTail(buddyData);
    //            mInviteBuddyDataMap.insert(buddyIter->mValue.mBuddyGuid, &buddyIter->mValue);
    //            buddyIter->mValue.mDiffFlag = true;
    //            addBuddyList.insertTail(buddyData);
    //        }
    //        else // ���ڣ� �ж�ս���Ƿ�仯
    //        {
    //            if (jter->mValue->mBuddyBattleValue != buddyInfo->mPlayerBattleValue)
    //            {
    //                jter->mValue->syncFromPlayerInfo(*buddyInfo);
    //                changeBuddyList.insertTail(*jter->mValue);
    //            }
    //            jter->mValue->mDiffFlag = true;
    //        }   
    //        iter = inviteBuddyDataList->next(iter);
    //    }
    //}

    //for (List<BuddyData>::Iter* buddyIter = mPlayerBuddyData->mInviteBuddyDataList.begin();
    //    buddyIter != NULL; )
    //{
    //    if (buddyIter->mValue.mDiffFlag == true)
    //    {
    //        buddyIter->mValue.mDiffFlag = false;
    //        buddyIter = mPlayerBuddyData->mInviteBuddyDataList.next(buddyIter);
    //    }
    //    else
    //    {
    //        delBuddyList.insertTail(buddyIter->mValue.mBuddyGuid);
    //        mInviteBuddyDataMap.erase(buddyIter->mValue.mBuddyGuid);
    //        buddyIter = mPlayerBuddyData->mInviteBuddyDataList.erase(buddyIter);
    //    }
    //}
    return LynxErrno::None;
}

ErrorId 
BuddyManager::syncBuddyList(List<BuddyData>& addBuddyList, 
                            List<BuddyData>& changeBuddyList,
                            List<Guid>& delBuddyList)
{
	//ͬ�������б�����������Ϣ�ı�������ѵ����ӻ���ٵȵ� �� ���ѽṹû���壬������ע����sec
//    LogicSystem& logicSystem = LogicSystem::getSingleton();
//    PlayerInfo* playerInfo = logicSystem.getPlayerInfo(mPlayer->getPlayerGuid());
//    if (!playerInfo)
//    {
//        return LynxErrno::PlayerInfoNotExist;
//    }
//
//    for (UInt32 i = 0; i < BUDDY_MAX_COUNT; ++i)
//    {
//        if (playerInfo->mBuddyList[i] != 0)
//        {
//            PlayerInfo* buddyPlayerInfo = logicSystem.getPlayerInfo(playerInfo->mBuddyList[i]);
//            if (!buddyPlayerInfo)
//            {
//                LOG_WARN("Failed to get player info %llu, del buddy.", playerInfo->mBuddyList[i]);
//                playerInfo->mBuddyList[i] = 0;
//            }
//            else
//            {
//                Map<Guid, BuddyData*>::Iter* iter = mBuddyDataMap.find(playerInfo->mBuddyList[i]);
//                if (!iter)
//                {
//                    // �����ӵĺ���
//                    BuddyData buddyData(*buddyPlayerInfo);
//                    buddyData.mDiffFlag = true;
//                    List<BuddyData>::Iter* buddyDataIter = mPlayerBuddyData->mBuddyDataList.insertTail(buddyData);
//                    mBuddyDataMap.insert(playerInfo->mBuddyList[i], &buddyDataIter->mValue);
//
//                    addBuddyList.insertTail(buddyData);
//                }
//                else // �Ѿ����ڵĺ��ѣ� �ж�ս���Ƿ�仯
//                {
//                    if (iter->mValue->mBuddyBattleValue != buddyPlayerInfo->mPlayerBattleValue)
//                    {
//                        iter->mValue->syncFromPlayerInfo(*buddyPlayerInfo);
//                        changeBuddyList.insertTail(*iter->mValue);
//                    }
//                    iter->mValue->mDiffFlag = true;
//                }
//            }
//        }
//    }
//
//    for (List<BuddyData>::Iter* iter = mPlayerBuddyData->mBuddyDataList.begin();
//        iter != NULL; )
//    {
//        if (iter->mValue.mDiffFlag == true)
//        {
//            iter->mValue.mDiffFlag = false;
//            iter = mPlayerBuddyData->mBuddyDataList.next(iter);
//        }
//        else
//        {
//            delBuddyList.insertTail(iter->mValue.mBuddyGuid);
//            mBuddyDataMap.erase(iter->mValue.mBuddyGuid);
//            iter = mPlayerBuddyData->mBuddyDataList.erase(iter);
//        }
//    }
     return LynxErrno::None;
}

ErrorId 
BuddyManager::rejectBuddy(const Guid& buddyGuid) // ֻɾ���������ݣ� ������ϵĺ������ݣ� ��ͬ��diffɾ��
{
    if (buddyGuid == 0) // ɾ��ȫ������
    {
        //mInviteBuddyDataMap.clear();
        //mPlayerBuddyData->mInviteBuddyDataList.clear();
        LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), 0);
    }
    else
    {
        //mInviteBuddyDataMap.erase(buddyGuid);

        //for (List<BuddyData>::Iter* iter = mPlayerBuddyData->mInviteBuddyDataList.begin();
        //    iter != NULL; iter = mPlayerBuddyData->mInviteBuddyDataList.next(iter))
        //{
        //    if (iter->mValue.mBuddyGuid == buddyGuid)
        //    {
        //        mPlayerBuddyData->mInviteBuddyDataList.erase(iter);
        //        break;
        //    }
        //}

        LogicSystem::getSingleton().removeInvitedBuddy(mPlayer->getPlayerGuid(), buddyGuid);
        LogicSystem::getSingleton().removeInviteBuddy(buddyGuid, mPlayer->getPlayerGuid());
    }
    return LynxErrno::None;
}

