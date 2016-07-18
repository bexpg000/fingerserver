#include "FriendMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"
#include "RankSystem.h"
using namespace Lynx;

void FriendMsgHandler::onAddFriendReq(const ConnId& connId, CGFriendAddReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();
		
		//�Է����ָ��
		Player * otherplayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
		if(!otherplayer)
		{
			return;
		}

		//�Է�����Ƿ�����
		const ConnId & otherconnId = otherplayer->getConnId();
		if(!otherconnId)
		{
			return ;
		}
	
		//�Է�����Ƿ��ں�������
		bool inblack = selfplayer->getFriendBlackManager().judgeBlack(playerUid);
		if(inblack)
		{
			return;
		}

		//�Լ��Ƿ��ڶԷ���Һ�������
		bool inblack2 = otherplayer->getFriendBlackManager().judgeBlack(selfUid);
		if(inblack2)
		{
			return;
		}

		//��Ϣ֪ͨ�Է����
		GCFriendAddNotify addFriendNotify;
		addFriendNotify.mPacketID = BOC_FRIENDADD_NOTIFY;
		
		Json::Value sendroot;
		sendroot["fromuid"] = selfUid;
		sendroot["modelid"] = selfplayer->getPlayerModelID();
		sendroot["name"] = selfplayer->getPlayerName();
		sendroot["power"] = selfplayer->getPlayerPower();
		sendroot["viplv"] = selfplayer->getVipLevel();
		sendroot["level"] = selfplayer->getPlayerLeval();
		Json::StyledWriter writer;
		
		addFriendNotify.mRespJsonStr = writer.write(sendroot);

		cout << addFriendNotify.mRespJsonStr;
		//�Է���ҵ�connid�������Է����
		NetworkSystem::getSingleton().sendMsg(addFriendNotify,otherconnId);

	}
	else
	{
		LOG_INFO("empty message friend add!");
	}

}


void FriendMsgHandler::onAddFriendBack(const ConnId& connId, CGFriendAddBackReq& msg)
{
		Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!selfplayer)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		UInt64 otherUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();
		UInt32 agree = root["agree"].asUInt();

		//A����B�Ӻ������󣬷������յ�B�ܾ���Ӻ��ѵĻظ�����ô˫������֪ͨ��ֱ�ӷ���
		if(!agree)
		{
			return;
		}

		//�ж��Լ������Ƿ������
		UInt32 selfFriendCount = selfplayer->getFriendBlackManager().getFriendCount();

		if(selfFriendCount >= 50)
		{
			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::SelfFriendFull;
			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

			return;
		}

		
		//���Bͬ���ˣ���ô���A�ĺ����Ƿ�����
		Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
		//�ڴ����и��������
		if(otherPlayer)
		{
			UInt32 otherFriendCount = otherPlayer->getFriendBlackManager().getFriendCount();
			//�ж϶Է������Ƿ�����
			if(otherFriendCount >= 50)
			{
				//�Է����Ѵﵽ50�ˣ�����������
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::OtherFriendFull;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

				return;
			}

			//���˫��Ϊ����
				selfplayer ->getFriendBlackManager().addFriend(otherUid);
				otherPlayer ->getFriendBlackManager().addFriend(selfUid);
				selfplayer->getAchieveManager().updateAchieveData(FRIENDCNT,1);
				otherPlayer->getAchieveManager().updateAchieveData(FRIENDCNT,1);
				
				//��B,Ҳ����ͬ���һ���ذ�������ͬ��ɹ�

				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::None;
				root["playeruid"] = otherUid;

				//��ϸ��Ϣ
				root["modelid"] = otherPlayer->getPlayerModelID();
				root["name"] = otherPlayer->getPlayerName();
				root["power"] = otherPlayer->getPlayerPower();
				root["viplv"] = otherPlayer->getVipLevel();
				root["level"] = otherPlayer->getPlayerLeval();
				root["leaveTime"] = otherPlayer->getPlayerLeaveTime();
	

				//////////////////////////////////////////
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

				//�ж�A�Ƿ����ߣ�֪ͨ�ʼ������Ӻ��ѵ�һ����Ӻ��ѳɹ���Ϣ
				const ConnId&  otherconnId = otherPlayer->getConnId();
				if(otherconnId)
				{
					GCFriendAddBackResp friendAddResp;	
					friendAddResp.mPacketID = BOC_ADDAGRU_RESP;
					Json::Value addRespRoot;
					addRespRoot["playeruid"] =  selfUid;
					addRespRoot["errorId"] = LynxErrno::None;
					//���߶Է��Լ�����ϸ��Ϣ
					addRespRoot["modelid"] = selfplayer->getPlayerModelID();
					addRespRoot["name"] = selfplayer->getPlayerName();
					addRespRoot["power"] = selfplayer->getPlayerPower();
					addRespRoot["viplv"] = selfplayer->getVipLevel();
					addRespRoot["level"] = selfplayer->getPlayerLeval();
					addRespRoot["leaveTime"] = 0;


					Json::StyledWriter writer2;
					
					friendAddResp.mRespJsonStr = writer2.write(addRespRoot);
					cout << friendAddResp.mRespJsonStr;
					NetworkSystem::getSingleton().sendMsg(friendAddResp, otherconnId);
				}
				
				return;
		}
		else
		{
			//�ڴ���û�и���ҵ�����
			//�����ݿ������ҵ�����
			
			FriendBlackInfo otherFriendInfo;
			bool success = selfplayer ->getFriendBlackManager().getOtherFriendInfo(otherUid, otherFriendInfo);

			if(!success)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

				return;
			}

			if(otherFriendInfo.friendCount >= 50)
			{
				GCFriendAddBackResp  addBackResp;
				addBackResp.mPacketID = BOC_ADDAGRU_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::OtherFriendFull;
				Json::StyledWriter writer;
				addBackResp.mRespJsonStr = writer.write(root);
				cout << addBackResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

				return;
			}

			//����Һ���û����
			
			//˫����Ӻ���
			//B��ͬ���һ����Ӻ���
			selfplayer ->getFriendBlackManager().addFriend(otherUid);			
			//��B,Ҳ����ͬ���һ���ذ�������ͬ��ɹ�

			GCFriendAddBackResp  addBackResp;
			addBackResp.mPacketID = BOC_ADDAGRU_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			BaseInfoData otherbaseInfo;
			bool getRes = LogicSystem::getSingleton().getBaseInfo(otherUid, otherbaseInfo);
			
			if(!getRes)
			{
				    bool resultDb = selfplayer->getFriendBlackManager().getBaseInfoFromDb(otherUid,  otherbaseInfo);
					if(resultDb)
					{
						LogicSystem::getSingleton().insertBaseInfo(otherbaseInfo);
					}
			}

			root["modelid"] = otherbaseInfo.modelId;
			root["name"] = otherbaseInfo.name.c_str();
			root["power"] = otherbaseInfo.power;
			root["viplv"] = otherbaseInfo.vipLv;
			root["level"] = otherbaseInfo.level;
			root["leaveTime"] = otherbaseInfo.leaveTime;

			Json::StyledWriter writer;
			addBackResp.mRespJsonStr = writer.write(root);
			cout << addBackResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(addBackResp, connId);

			//���ڿ�ʼ�����һ����Aû�����ߣ����߸������ݿ�
			PersistAddFriendOffLineNotify addOffLine;
			addOffLine.m_nMyselfUid = otherUid;
			addOffLine.m_nPlayerUid = selfUid;
			addOffLine.blackStr = otherFriendInfo.blackstr;
			addOffLine.friendStr = otherFriendInfo.friendstr;
			addOffLine.friendCount = otherFriendInfo.friendCount;

			PersistSystem::getSingleton().postThreadMsg(addOffLine, otherUid);

			//����������Һ��������ɾ�
			OffLineFriendUpdateMsg achieveUpdate;
			achieveUpdate.playerUid = otherUid;
			achieveUpdate.count = otherFriendInfo.friendCount;
			PersistSystem::getSingleton().postThreadMsg(achieveUpdate, otherUid);
			
		}
	}
	else
	{
		LOG_INFO("empty message friend addback!");
	}

}


void FriendMsgHandler::onDelFriendReq(const ConnId&connId, CGFriendDelReq&msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Reader reader;
	Json::Value root;

	if(reader.parse(msg.mReqJsonStr, root))
	{
		UInt64 otherUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		//�ж϶Է��Ƿ����ڴ���
		Player* otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
		//���ڴ���
		if(otherPlayer)
		{
				
				bool selfDel = selfplayer->getFriendBlackManager().delFriend(otherUid);
				if(!selfDel)
				{
					GCFriendDelResp delResp;
					delResp.mPacketID = BOC_FRIENDDEL_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::PlayerNotExist;
					Json::StyledWriter writer;
					delResp.mRespJsonStr = writer.write(root);
					cout << delResp.mRespJsonStr;
					NetworkSystem::getSingleton().sendMsg(delResp, connId);
					return ;
				}
				
				bool otherDel = otherPlayer->getFriendBlackManager().delFriend(selfUid);

				if(!otherDel)
				{	
					GCFriendDelResp delResp;
					delResp.mPacketID = BOC_FRIENDDEL_RESP;
					Json::Value root;
					root["errorId"] = LynxErrno::PlayerNotExist;
					Json::StyledWriter writer;
					delResp.mRespJsonStr = writer.write(root);
					cout << delResp.mRespJsonStr;
					NetworkSystem::getSingleton().sendMsg(delResp, connId);
					return ;
				}

				//����ɾ���ɹ��Ļظ�
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::None;
				root["playeruid"] = otherUid;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);


				//��ʱ�ж϶Է��Ƿ����ߣ�������֪ͨ�Է����Է��Ӻ����б��н��Լ��Ƴ���
				const ConnId & otherconnId = otherPlayer->getConnId();
				if(otherconnId)
				{
					GCFriendDelResp delNotify;
					delNotify.mPacketID = BOC_FRIENDDEL_RESP;
					Json::Value root;
					root["playeruid"] = selfUid;
					root["errorId"] = LynxErrno::None;

					Json::StyledWriter writer;
					
					delNotify.mRespJsonStr = writer.write(root);
					cout << delNotify.mRespJsonStr;
					NetworkSystem::getSingleton().sendMsg(delNotify, otherconnId);
				}

		}
		else
		{
			//�����ڴ���

			FriendBlackInfo otherFriendInfo;
			bool success = selfplayer ->getFriendBlackManager().getOtherFriendInfo(otherUid, otherFriendInfo);

			if(!success)
			{
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			//�Լ��ȴӺ����б���ɾ���Է�

			bool successdel = selfplayer->getFriendBlackManager().delFriend(otherUid);
			if(!successdel)
			{
				GCFriendDelResp delResp;
				delResp.mPacketID = BOC_FRIENDDEL_RESP;
				Json::Value root;
				root["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				delResp.mRespJsonStr = writer.write(root);
				cout << delResp.mRespJsonStr;
				NetworkSystem::getSingleton().sendMsg(delResp, connId);
				return ;
			}

			//֪ͨ�ͻ���ɾ���ɹ�
			GCFriendDelResp delResp;
			delResp.mPacketID = BOC_FRIENDDEL_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			Json::StyledWriter writer;
			delResp.mRespJsonStr = writer.write(root);
			cout << delResp.mRespJsonStr;
			NetworkSystem::getSingleton().sendMsg(delResp, connId);

			//Ҫɾ���Ķ������ڴ��У���Ҫ֪ͨ���ݿ�ɾ��
			PersistDelFriendOffLineNotify delOffLine;
			delOffLine.m_nMyselfUid = otherUid;
			delOffLine.m_nPlayerUid = selfUid;
			delOffLine.blackStr = otherFriendInfo.blackstr;
			delOffLine.friendStr = otherFriendInfo.friendstr;
			delOffLine.friendCount = otherFriendInfo.friendCount;

			PersistSystem::getSingleton().postThreadMsg(delOffLine, otherUid);

		}
		
	}
	else
	{
		LOG_INFO("empty message friend del!");
	}

}


void FriendMsgHandler::onAddBlackReq(const ConnId& connId, CGBlackAddReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//�Է�uid
		UInt64 otherplayerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		bool success = selfplayer->getFriendBlackManager().addBlack(otherplayerUid);

		if(success)
		{
			//�ж϶Է��Ƿ��ں����б���
			bool infriend = selfplayer->getFriendBlackManager().judgeFriend(otherplayerUid);

			//�Ƴ��˴˵ĺ��ѹ�ϵ
			if(infriend)
			{
				//�ж϶Է��Ƿ����ڴ���
				Player* otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherplayerUid);
				if(otherPlayer)
				{
					//���ڴ���
					bool selfDel = selfplayer->getFriendBlackManager().delFriend(otherplayerUid);
					bool otherDel = otherPlayer->getFriendBlackManager().delFriend(selfUid);


					//��ʱ�ж϶Է��Ƿ����ߣ�������֪ͨ�Է����Է��Ӻ����б��н��Լ��Ƴ���
					const ConnId & otherconnId = otherPlayer->getConnId();
					if(otherconnId)
					{
						GCFriendDelResp delNotify;
						delNotify.mPacketID = BOC_FRIENDDEL_RESP;
						Json::Value root;
						root["playeruid"] = selfUid;
						root["errorId"] = LynxErrno::None;

						Json::StyledWriter writer;

						delNotify.mRespJsonStr = writer.write(root);
						cout << delNotify.mRespJsonStr;
						NetworkSystem::getSingleton().sendMsg(delNotify, otherconnId);
					}
					
				}
				else
				{
					//�����ڴ���
					FriendBlackInfo otherFriendInfo;
					bool success = selfplayer ->getFriendBlackManager().getOtherFriendInfo(otherplayerUid, otherFriendInfo);
					
					//�Լ��ȴӺ����б���ɾ���Է�

					bool successdel = selfplayer->getFriendBlackManager().delFriend(otherplayerUid);
					
					if(success)
					{
						//Ҫɾ���Ķ������ڴ��У���Ҫ֪ͨ���ݿ�ɾ��
						PersistDelFriendOffLineNotify delOffLine;
						delOffLine.m_nMyselfUid = otherplayerUid;
						delOffLine.m_nPlayerUid = selfUid;
						delOffLine.blackStr = otherFriendInfo.blackstr;
						delOffLine.friendStr = otherFriendInfo.friendstr;
						delOffLine.friendCount = otherFriendInfo.friendCount;

						PersistSystem::getSingleton().postThreadMsg(delOffLine, otherplayerUid);
					}
				

				}

			}

			GCBlackAddResp blackAddResp;
			blackAddResp.mPacketID = BOC_ADDBLACK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherplayerUid;
	
			////////////////////////////////////////////////////
			BaseInfoData otherbaseInfo;
			bool getRes = LogicSystem::getSingleton().getBaseInfo(otherplayerUid, otherbaseInfo);

			if(!getRes)
			{
				bool resultDb = selfplayer->getFriendBlackManager().getBaseInfoFromDb(otherplayerUid,  otherbaseInfo);
				if(resultDb)
				{
					LogicSystem::getSingleton().insertBaseInfo(otherbaseInfo);
				}
			}



			///////////////////////////////////////////////////////////
			root["modelid"] = otherbaseInfo.modelId;
			root["name"] = otherbaseInfo.name.c_str();
			root["power"] = otherbaseInfo.power;
			root["viplv"] = otherbaseInfo.vipLv;
			root["level"] = otherbaseInfo.level;
			root["leaveTime"] = otherbaseInfo.leaveTime;

			Json::StyledWriter writer;
			
			blackAddResp.mRespJsonStr = writer.write(root);

			cout <<blackAddResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackAddResp, connId);
		}
		else
		{
			
			GCBlackAddResp blackAddResp;
			blackAddResp.mPacketID = BOC_ADDBLACK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::PlayerNotExist;
			Json::StyledWriter writer;

			blackAddResp.mRespJsonStr = writer.write(root);

			cout <<blackAddResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackAddResp, connId);
		}
		

	}
	else
	{
		LOG_INFO("empty message black add!");
	}

}

void FriendMsgHandler::onDelBlackReq(const ConnId& connId, CGBlackDelReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//�Է�uid
		UInt64 playerUid = root["playeruid"].asUInt64();
		UInt64 selfUid = root["selfuid"].asUInt64();

		bool success = player->getFriendBlackManager().delBlack(playerUid);

		if(success)
		{
			GCBlackDelResp blackDelResp;
			blackDelResp.mPacketID = BOC_DELBALCK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = playerUid;

			Json::StyledWriter writer;

			blackDelResp.mRespJsonStr = writer.write(root);

			cout <<blackDelResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackDelResp, connId);
		}
		else
		{
			GCBlackDelResp blackDelResp;
			blackDelResp.mPacketID = BOC_DELBALCK_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::PlayerNotExist;
		

			Json::StyledWriter writer;

			blackDelResp.mRespJsonStr = writer.write(root);

			cout <<blackDelResp.mRespJsonStr;

			NetworkSystem::getSingleton().sendMsg(blackDelResp, connId);
		}


	}
	else
	{
		LOG_INFO("empty message black del!");
	}
}


void FriendMsgHandler::onRecommendListReq(const ConnId&connId, CGRecommendListReq&msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		//ԭ���Ľṹ
		/*const Set<Player > &playerSet = LogicSystem::getSingleton().getPlayerLvSet(player);

		Json::Value root;
		root["errorId"] = LynxErrno::None;

		for(const Set<Player >::Iter * setIter = playerSet.begin();  setIter != NULL;
			setIter = playerSet.next(setIter))
		{
			Json::Value playerRoot;
			playerRoot["playeruid"] = setIter->mValue.getPlayerGuid();
			playerRoot["modelid"] = setIter->mValue.getPlayerModelID();
			playerRoot["name"] = setIter->mValue.getPlayerName();
			playerRoot["power"] = setIter->mValue.getPlayerPower();
			playerRoot["viplv"] = setIter->mValue.getVipLevel();
			playerRoot["level"] = setIter->mValue.getPlayerLeval();
			root["playerlist"].append(playerRoot);
		}*/
		
		List<BaseInfoData> playerList;
		RankSystem::getSingleton().getOnlinePlayers(player->getPlayerLeval(), player, playerList);

		for(List<BaseInfoData>::Iter * setIter = playerList.begin();  setIter != NULL;
			setIter = playerList.next(setIter))
		{
			Json::Value playerRoot;
			playerRoot["playeruid"] = setIter->mValue.playerUid;
			playerRoot["modelid"] = setIter->mValue.modelId;
			playerRoot["name"] = setIter->mValue.name.c_str();
			playerRoot["power"] = setIter->mValue.power;
			playerRoot["viplv"] = setIter->mValue.vipLv;
			playerRoot["level"] = setIter->mValue.level;
			root["playerlist"].append(playerRoot);
		}

		Json::StyledWriter writer;
		GCRecommendListResp commendListResp;
		commendListResp.mPacketID = BOC_RECOMMENDLIST_RESP;
		commendListResp.mRespJsonStr = writer.write(root);
		cout << commendListResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(commendListResp, connId);
	}
	else
	{
		LOG_INFO("empty message commend List Req");
	}
}


void FriendMsgHandler::onFindPlayerReq(const ConnId &connId,  CGFindPlayerReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{
		std::string keyStr = root["keystr"].asString();
		UInt32 type = root["type"].asUInt();

		Player * findPlayer = NULL;

		//1 uuid
		if(type)
		{
			std::stringstream mystream;
			mystream << keyStr;
			UInt64 playerUid = 0;
			mystream >> playerUid;

			findPlayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);

			if(!findPlayer)
			{
				Json::Value playerRoot;
				playerRoot["errorId"] = LynxErrno::PlayerNotExist;
				Json::StyledWriter writer;
				GCFindPlayerResp findResp;
				findResp.mRespJsonStr = writer.write(playerRoot);
				cout << findResp.mRespJsonStr;
				findResp.mPacketID = BOC_FINDPLAYER_RESP;
				NetworkSystem::getSingleton().sendMsg(findResp, connId);
				return;
			}


		}
		else
		{
				findPlayer = LogicSystem::getSingleton().getPlayerByName(keyStr.c_str());
				if(!findPlayer)
				{
					Json::Value playerRoot;
					playerRoot["errorId"] = LynxErrno::PlayerNotExist;
					Json::StyledWriter writer;
					GCFindPlayerResp findResp;
					findResp.mRespJsonStr = writer.write(playerRoot);
					cout << findResp.mRespJsonStr;
					findResp.mPacketID = BOC_FINDPLAYER_RESP;
					NetworkSystem::getSingleton().sendMsg(findResp, connId);

					return;
				}
		}


			Json::Value playerRoot;
			playerRoot["playeruid"] = findPlayer->getPlayerGuid();
			playerRoot["modelid"] = findPlayer->getPlayerModelID();
			playerRoot["name"] = findPlayer->getPlayerName();
			playerRoot["power"] = findPlayer->getPlayerPower();
			playerRoot["viplv"] = findPlayer->getVipLevel();
			playerRoot["level"] = findPlayer->getPlayerLeval();
			playerRoot["errorId"] = LynxErrno::None;

			Json::StyledWriter writer;
			GCFindPlayerResp findResp;
			findResp.mRespJsonStr = writer.write(playerRoot);
			cout << findResp.mRespJsonStr;
			findResp.mPacketID = BOC_FINDPLAYER_RESP;
			NetworkSystem::getSingleton().sendMsg(findResp, connId);

			
	}
	else
	{
		LOG_INFO("empty message findPlayer  Req");
	}
}	