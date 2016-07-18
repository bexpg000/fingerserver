#include "ExchangeMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;

void 
ExchangeMsgHandler::onExchangeBuyItemReq(const ConnId& connId, ExchangeBuyItemReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId: %llu", connId);
        return;
    }
    ExchangeBuyItemResp resp;
    resp.mErrorId = player->getExchangeManager().exchangeItem(msg.mExchangeId, msg.mCount);
    player->sendMsgWithAck(resp);
}

void 
ExchangeMsgHandler::onGetArenaExchangeDataReq(const ConnId& connId, GetArenaExchangeDataReq& msg)
{
 //   Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
 //   if (!player)
 //   {
 //       LOG_WARN("Failed to get Player by connId: %llu", connId);
 //       return;
 //   }

 //   GetArenaExchangeDataResp resp;
 //   PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(player->getPlayerGuid());
 //   if (!playerInfo)
 //   {
 //       LOG_WARN("Failed to get Playerinfo by id:%llu", player->getPlayerGuid());
 //       resp.mErrorId = LynxErrno::InvalidParameter;
 //       player->sendMsgWithAck(resp);
 //       return;
 //   }
	////Ŀǰ���׳����²�����ע���ˣ�playerinfo��û�н��ױ�־�����������ֶ�, sec�Ժ��
 //   resp.mArenaExchangeFlag = playerInfo->mArenaInfo.mArenaExchangeFlag;
 //   resp.mYestodayBestRank = playerInfo->mArenaInfo.mYestodayBestRank;
 //   resp.mErrorId = LynxErrno::None;
 //   player->sendMsgWithAck(resp);
}

void 
ExchangeMsgHandler::onReceiveArenaHonorAwardReq(const ConnId& connId, ReceiveArenaHonorAwardReq& msg)
{
    Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
    if (!player)
    {
        LOG_WARN("Failed to get Player by connId: %llu", connId);
        return;
    }

    ReceiveArenaHonorAwardResp resp;
    resp.mErrorId = player->getExchangeManager().receiveHonorAward(resp.mArenaExchangeFlag);
    player->sendMsgWithAck(resp);
}