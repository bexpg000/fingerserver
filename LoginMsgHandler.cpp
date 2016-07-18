#include "LoginMsgHandler.h"
#include "PassportThreadMsg.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void 
LoginMsgHandler::onReconnectReq(const ConnId& connId, ReconnectReq& msg)
{
    LogicSystem::getSingleton().reconnect(connId, msg.mPlayerGuid, msg.mLoginToken);
}



void LoginMsgHandler::onCGGameLoginReq(const ConnId& connId, CGGameLoginReq& cgloginReqMsg)//2001
{
	//cout << "dddddd" <<TimeUtil::getTimeMilliSec() << endl;
	std::stringstream mystream;
	mystream.clear();
	mystream.str("");
	
	mystream << cgloginReqMsg.mStrGUID;
	UInt64 playerGuid;
	mystream >> playerGuid;

	//***************************************************************************************************************
	//��Ϸ�ڲ����Խ׶ν��û�������ʱ���ڴ�����գ�ֻ�ڲ��Խ׶�ʹ��,�����˲�Ҫʹ��
	//�Ժ���Ϸ�������˹�������
	//LogicSystem::getSingleton().debugTestDestroyPlayer(playerGuid);
	//****************************************************************************************************************

	LogicSystem::getSingleton().loadPlayerData(connId,playerGuid);
}

void LoginMsgHandler::onCGCreatePlayerReq(const ConnId& connId, CGGameCreatePlayerReq& msg)
{
	std::stringstream mystream;
	mystream.clear();
	mystream.str("");

	Json::Value root;
	Json::Reader reader;

	if(reader.parse(msg.mReqStr,root))
	{
		UInt32 modelId = root["uid"].asUInt();
		std::string name = root["name"].asString();
		UInt64 accountID = root["accountId"].asUInt64();

		PersistCreatePlayerReq createPlayerReq;
		createPlayerReq.mConnId = connId;
		createPlayerReq.mPlayerModelId = modelId;
		createPlayerReq.mName = name.c_str();
		createPlayerReq.mAccountId = accountID;
		PersistSystem::getSingleton().postThreadMsg(createPlayerReq,modelId);
	}
	else
	{
		LOG_INFO("empty message polishequipreq!");
	}
}

void LoginMsgHandler::onCGAccountLoginReq(const ConnId& connId, CGAccountLoginReq&msg)
{
	std::stringstream mystream;
	mystream.clear();
	mystream.str("");

	Json::Value root;
	Json::Reader reader;

	if(reader.parse(msg.mReqStr,root))
	{
		UInt64 accountId = root["accountId"].asUInt64();
		PersistLoadPlayerByAccount loadPlayerByAccount;
		loadPlayerByAccount.mAccountId = accountId;
		loadPlayerByAccount.mConnId = connId;
		PersistSystem::getSingleton().postThreadMsg(loadPlayerByAccount, connId);
		
	}
	else
	{
		LOG_INFO("empty message accountLogin!");
	}
}