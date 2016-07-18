#include "ChatManager.h"
#include "Player.h"
#include "ConfigParser.h"
#include "LogicSystem.h"
using namespace Lynx;

ChatManager::ChatManager()
{
	m_pPlayer = NULL;
}


ChatManager:: ~ChatManager()
{

}

bool ChatManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	
	m_pChatList = &player->mPlayerData.mChatData.chatLists;

	String ip = ConfigParser::getSingleton().getMainDbIp();
	UInt16 port = ConfigParser::getSingleton().getMainDbPort();
	String username = ConfigParser::getSingleton().getMainDbUsername();
	String password = ConfigParser::getSingleton().getMainDbPassword();
	String dbname = ConfigParser::getSingleton().getMainDbName();

	if (!mDBInterface.initial(ip.c_str(), port, username.c_str(),
		password.c_str(), dbname.c_str()))
	{
		LOG_WARN("Failed to connect mysql server. [%s %s %s %s]",
			ip.c_str(), username.c_str(), password.c_str(), dbname.c_str());
		return false;
	}

	return true;

}


void ChatManager::release()
{
	m_pPlayer = NULL;
	mDBInterface.release();
	
}

void ChatManager::insertPlayerToChatList(UInt64 playerUid)
{
	List<ChatData>::Iter * findChatIter = NULL;
	
	//���ҵ�ǰ��������б����Ƿ���playeruid���
	for(List<ChatData>::Iter * chatIter = m_pChatList->begin(); chatIter != NULL; chatIter = m_pChatList->next(chatIter))
	{
		if(chatIter->mValue.playerUid == playerUid)
		{
			findChatIter = chatIter;
			break;
		}
			
	}
	//���ڸ���ң����Ƴ�֮ǰ�������¼	
	if(findChatIter)
	{
		m_pChatList->erase(findChatIter);
	}
		
	ChatData chatData;
	chatData.chatTime = TimeUtil::getTimeSec();
	chatData.playerUid = playerUid;

	m_pChatList->insertTail(chatData);

	checkChatListFull();

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);
	
}

void ChatManager::checkChatListFull()
{
	//ѭ���Ƴ�����10�����������
	UInt32 listSize = m_pChatList->size() ;
	if(listSize > 10)
	{
		List<ChatData> ::Iter * chatIter = m_pChatList->begin();
		for(UInt32 i = 0; i < listSize - 10; i++)
		{
			if(chatIter)
			{
				chatIter = m_pChatList->erase(chatIter);
			}
			
		}
	}
}



void ChatManager::setBlackList(UInt64 playeruid)
{
	//��Ӻ���������


	const ConnId& connId = m_pPlayer->getConnId();

	GCSetBlackListResp blackListResp;
	blackListResp.mPacketID = BOC_SETBLACKLIST_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["playeruid"] = playeruid;
	Json::StyledWriter writer;
	blackListResp.mRespJsonStr = writer.write(root);

		
	NetworkSystem::getSingleton().sendMsg(blackListResp, connId);
}


void ChatManager::addFriendList(UInt64 playeruid)
{
	//��Ӻ��ѹ���

	//�����ǻذ�

	const ConnId& connId = m_pPlayer->getConnId();

	GCAddFriendResp addFriendResp;
	addFriendResp.mPacketID = BOC_ADDFRIEND_RESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["playeruid"] = playeruid;
	Json::StyledWriter writer;
	addFriendResp.mRespJsonStr = writer.write(root);


	NetworkSystem::getSingleton().sendMsg(addFriendResp, connId);

}

void ChatManager::showDetailInfo(UInt64 playeruid)
{
	//�鿴���鹦��

	const ConnId& connId = m_pPlayer->getConnId();

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playeruid);
	
	//��Ҳ�����
	if(!player)
	{
		char basedata[2048] = {0};

		snprintf(basedata, sizeof(basedata),  "call Player_Load(%llu)", playeruid);

		LOG_DEBUG("Sql:%s", basedata);
		bool result = mDBInterface.execSql(basedata);
		MYSQL_RES* rs = mDBInterface.storeResult();
		if(!rs)
		{
			// ִ��ʧ��
			mDBInterface.freeResult(&rs);

			GCShowDetailResp showDetailResp;
			showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;
			
			Json::StyledWriter writer;
			showDetailResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

			return;

				
		}

		MYSQL_ROW row = mDBInterface.fetchRow(rs);

		if(!row)
		{
			// ��ɫ������
			mDBInterface.freeResult(&rs);

			GCShowDetailResp showDetailResp;
			showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::InvalidParameter;

			Json::StyledWriter writer;
			showDetailResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

			return;
			

		}


		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["modelid"] = lynxAtoi<UInt32>(row[1]);
		root["name"] = row[2];
		root["level"] = lynxAtoi<UInt32>(row[3]);
		root["vipLv"] = lynxAtoi<UInt32>(row[5]);
		root["playeruid"] = playeruid;
		root["attack"] = lynxAtoi<Guid>(row[16]);
	
		

		mDBInterface.freeResult(&rs); 

		GCShowDetailResp showDetailResp;
		showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

		Json::StyledWriter writer;
		showDetailResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

		return;


	}

	
		
		
		GCShowDetailResp showDetailResp;
		showDetailResp.mPacketID = BOC_SHOWDETAIL_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["modelid"] = player->getPlayerModelID();
		root["name"] = player->getPlayerName();
		root["level"] = player->getPlayerLeval();
		root["vipLv"] = player->getVipLevel();
		root["playeruid"] = playeruid;
		root["attack"] = player->getPlayerPower();

		Json::StyledWriter writer;
		showDetailResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(showDetailResp, connId);

		return;
	
}

std::string ChatManager::convertDataToJson()
{
	Json::Value root;

	root["disableflag"] = m_pPlayer->mPlayerData.mChatData.disableFlag;

	root["disablebegintime"] = m_pPlayer->mPlayerData.mChatData.disableBeginTime;

	root["disableTimeLong"] = m_pPlayer->mPlayerData.mChatData.disableTimeLong;


	for(List<ChatData>::Iter * chatIter = m_pChatList->begin(); chatIter != NULL; 
		chatIter = m_pChatList->next(chatIter) )
	{
		
		Json::Value chatDataRoot;

		Player * player = LogicSystem::getSingleton().getPlayerByGuid(chatIter->mValue.playerUid);
		if(player)
		{
			chatDataRoot["lastChatTime"] = chatIter->mValue.chatTime;
			chatDataRoot["playeruid"] = chatIter->mValue.playerUid;

			chatDataRoot["modelid"] = player->getPlayerModelID();
			chatDataRoot["name"] = player->getPlayerName();
			
			if(player->getConnId())
			{
				chatDataRoot["onLine"] = 1;
			}
			else
			{
				chatDataRoot["onLine"] = 0;
			}

			root["chatplayers"].append(chatDataRoot);
		}
		else
		{
			char basedata[2048] = {0};

			snprintf(basedata, sizeof(basedata),  "call basedata_load(%llu)", chatIter->mValue.playerUid);

			LOG_DEBUG("Sql:%s", basedata);
			bool result = mDBInterface.execSql(basedata);
			MYSQL_RES* rs = mDBInterface.storeResult();
			if(!rs)
			{
				// ִ��ʧ��
				mDBInterface.freeResult(&rs);
				continue;		
			}

			MYSQL_ROW row = mDBInterface.fetchRow(rs);

			if(!row)
			{
				// ��ɫ������
				mDBInterface.freeResult(&rs);
				continue;
				
			}

			chatDataRoot["lastChatTime"] = chatIter->mValue.chatTime;
			chatDataRoot["playeruid"] = chatIter->mValue.playerUid;
			
			chatDataRoot["modelid"]  = lynxAtoi<UInt32>(row[0]);
			chatDataRoot["name"] = row[1];

			chatDataRoot["onLine"] = 0;

			root["chatplayers"].append(chatDataRoot);

			mDBInterface.freeResult(&rs); 

		}
		
	}


	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);

	//cout << jsonStr;
	return jsonStr;


}


void ChatManager::receiveMsg(UInt32 channelType, UInt64 fromPlayer, UInt64 toPlayer, std::string ChatMsg)
{
	const ConnId& connId = m_pPlayer->getConnId();
	
	if(channelType == WORLDCHAT)
	{
		const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

		for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
			mapIter = connectionMap.next(mapIter))
		{
			//���Ƿ�����
			if(mapIter->mKey != connId)
			{
				GCClientChatResp clientChatResp;
				clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;
				Json::Value root;
				root["channelType"] = channelType;
				root["fromPlayer"] = fromPlayer;
				root["chatMsg"] = ChatMsg;
				root["modelid"] = m_pPlayer->getPlayerModelID();
				root["name"] = m_pPlayer->getPlayerName();
				root["chattime"] = TimeUtil::getTimeSec();

				Json::StyledWriter writer;

				clientChatResp.mRespJsonStr = writer.write(root);

				cout << clientChatResp.mRespJsonStr << endl;

				NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);

				
			}
			
		}

	}

	if(channelType == CONSORTIACHAT)
	{

	}

	if(channelType == PRIVATECHAT)
	{
		Player*  player = LogicSystem::getSingleton().getPlayerByGuid(toPlayer);

		if(!player)
		{
			GCOffLineNotify offLineNotify;
			offLineNotify.mPacketID = BOC_OFFLINE_NOTIFY;
			Json::Value root;
			root["playeruid"] = toPlayer;

			Json::StyledWriter writer;

			offLineNotify.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(offLineNotify, connId);

			cout << offLineNotify.mRespJsonStr;

			return;
		}

		const ConnId& toConnId = player->getConnId();

		if(!toConnId)
		{
			GCOffLineNotify offLineNotify;
			offLineNotify.mPacketID = BOC_OFFLINE_NOTIFY;
			Json::Value root;
			root["playeruid"] = toPlayer;

			Json::StyledWriter writer;

			offLineNotify.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(offLineNotify, connId);

			cout << offLineNotify.mRespJsonStr;

			return; 
		}

		player->getChatManager().insertPlayerToChatList(fromPlayer);

		
		GCClientChatResp clientChatResp;
		clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

		Json::Value root;
		root["channelType"] = channelType;
		root["fromPlayer"] = fromPlayer;
		root["chatMsg"] = ChatMsg;
		root["modelid"] = m_pPlayer->getPlayerModelID();
		root["name"] = m_pPlayer->getPlayerName();
		root["chattime"] = TimeUtil::getTimeSec();

		Json::StyledWriter writer;

		clientChatResp.mRespJsonStr = writer.write(root);

		cout << clientChatResp.mRespJsonStr;

		NetworkSystem::getSingleton().sendMsg(clientChatResp, toConnId);

	}

	
}


void ChatManager::clientForbidChat(UInt64 playerUid, UInt32 timelong, UInt64 timebegin)
{
	//const ConnId& connId = m_pPlayer->getConnId();


	m_pPlayer->mPlayerData.mChatData.disableFlag = true;

	m_pPlayer->mPlayerData.mChatData.disableBeginTime = timebegin;

	m_pPlayer->mPlayerData.mChatData.disableTimeLong = timelong;

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);

}

void ChatManager::clientResetChat(UInt64 playerUid)
{
	m_pPlayer->mPlayerData.mChatData.disableFlag = false;
	
	m_pPlayer->mPlayerData.mChatData.disableBeginTime = 0;

	m_pPlayer->mPlayerData.mChatData.disableTimeLong = 0;

	m_pPlayer->getPersistManager().setDirtyBit(CHATDATABIT);

}

void ChatManager::sendSystemMsg(std::string ChatMsg)
{
	const Map<ConnId, Player*> connectionMap = LogicSystem::getSingleton().getPlayerConnectionMap();

	for( Map<ConnId, Player*>::Iter* mapIter = connectionMap.begin(); mapIter != NULL; 
		mapIter = connectionMap.next(mapIter))
	{
		
		
		GCClientChatResp clientChatResp;
		clientChatResp.mPacketID = BOC_CLIENTCHAT_RESP;

		Json::Value root;
		root["channelType"] = SYSTEMMSG;
		root["fromPlayer"] = 0;
		root["chatMsg"] = ChatMsg;
		root["modelid"] = m_pPlayer->getPlayerModelID();
		root["name"] = m_pPlayer->getPlayerName();
		root["chattime"] = TimeUtil::getTimeSec();

		Json::StyledWriter writer;

		clientChatResp.mRespJsonStr = writer.write(root);

		cout << clientChatResp.mRespJsonStr << endl;

		NetworkSystem::getSingleton().sendMsg(clientChatResp, mapIter->mKey);
		

	}
}

void ChatManager::checkOnline(UInt64 playerUid)
{
	const ConnId& connId = m_pPlayer->getConnId();

	Json::Value root;
	for(List<ChatData>::Iter * chatDataIter =  m_pChatList->begin(); chatDataIter != NULL; 
		chatDataIter = m_pChatList->next(chatDataIter) )
	{
		Player*  chatPlayer = LogicSystem::getSingleton().getPlayerByGuid(chatDataIter->mValue.playerUid);
		//�ڴ���û�и����˵���Ѿ�����
		if(!chatPlayer)
		{
			Json::Value chatRoot;
			chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
			chatRoot["onLine"] = 0;

			root.append(chatRoot);

		}
		else
		{
			const ConnId & connId = chatPlayer->getConnId();
			//���connidΪ0��ʾ���ߣ���0��ʾ����
			if(connId)
			{
				Json::Value chatRoot;
				chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
				chatRoot["onLine"] = 1;

				root.append(chatRoot);
			}
			else
			{
				Json::Value chatRoot;
				chatRoot["playeruid"] = chatDataIter->mValue.playerUid;
				chatRoot["onLine"] = 0;

				root.append(chatRoot);
			}
			
			

		}
	}

	Json::StyledWriter writer;
	
	GCChatOnlineResp chatOnlineResp;
	chatOnlineResp.mPacketID = BOC_CLIENTONLINE_RESP;
	chatOnlineResp.mRespJsonStr = writer.write(root);
	cout << chatOnlineResp.mRespJsonStr;
	NetworkSystem::getSingleton().sendMsg(chatOnlineResp,connId);

}