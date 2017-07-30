#include "ConsortManager.h"

#include "Player.h"
#include "ConfigParser.h"
#include "LogicSystem.h"
#include "PersistSystem.h"

#include "ConsortSystem/ConsortSystem.h"
#include <time.h>
#include "FireConfirm/RankGame.h"
#include "FireConfirm/GlobalValue.h"
#include "FireConfirm/Gift.h"
#include"FireConfirm/Shop.h"

using namespace Lynx;

ConsortManager::ConsortManager()
{
	m_pPlayer = NULL;
	m_nIndex = 0;
	m_strCode ="";
	m_nEyeGroup = 0;
}


ConsortManager:: ~ConsortManager()
{

}

bool ConsortManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;
	m_pPlayerConsortData = &m_pPlayer->mPlayerData.mConsortData;
	m_pTicketAwardList = &m_pPlayer->mPlayerData.mConsortData.m_nTicketAwardList;
	
	for(List<TicketAward>::Iter * ticketawardIter = m_pTicketAwardList->begin();
		ticketawardIter != NULL; ticketawardIter = m_pTicketAwardList->next(ticketawardIter) )
	{
		m_ticketAwardMap.insert(ticketawardIter->mValue.m_nTicketId,  &(ticketawardIter->mValue));
	}
	
	return true;

}


void ConsortManager::release()
{
	m_pPlayer = NULL;
	m_pPlayerConsortData = NULL;
	m_ticketAwardMap.clear();
	m_pTicketAwardList->clear();
	
}

std::string ConsortManager::convertDataToJson()
{
	Json::Value root;
	root["consortid"] =  m_pPlayerConsortData->m_nConsortId;
	if(m_pPlayerConsortData->m_nConsortId)
	{
		ConsortData * consortData = ConsortSystem::getSingleton().getConsortDataById(m_pPlayerConsortData->m_nConsortId);
		if(!consortData)
		{
			return "";
		}
		if(consortData)
		{
			root["consortlv"] = consortData->m_nLv;
			root["consortname"] = consortData->m_strName.c_str();
			root["needcheck"] = consortData->m_nCheck;
			root["powerlimit"] = consortData->m_nPowerLimit;
			root["count"] = consortData->m_nCount;
			//root["power"] = consortData->m_nPower;
			//root["rank"] = consortData->m_nRank;
			root["desc"] = consortData->m_strDesc.c_str();
			root["consortres"] = consortData->m_nRes;
			root["consortjob"] = m_pPlayerConsortData->m_nConsortJob;
			//ǩ���ֶ����

			root["consortactive"] = consortData->m_nActive;
			root["signlv"] = consortData->m_nSignLv;
			for(List<UInt32>::Iter * awardIter = m_pPlayerConsortData->m_nSignAwards.begin();
				awardIter != NULL; awardIter = m_pPlayerConsortData->m_nSignAwards.next(awardIter) )
			{
				root["signawards"].append(awardIter->mValue);
			}


			root["sign"] = m_pPlayerConsortData->m_nSign;

			root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;

			// ������������ֶ����
			root["taskrefresh"] = m_pPlayerConsortData->m_nRefreshTimes;

			//��ҳ���ѵ������
			root["kitchentimes"] = m_pPlayerConsortData->m_nKitchenTimes;
			UInt64 curTime = time(0);
			for(List<XingxiaTask>::Iter *  taskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin();
				taskIter != NULL;  taskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(taskIter) )
			{
				Json::Value catTaskRoot;
				catTaskRoot["taskid"] = taskIter->mValue.m_nTaskId;
				catTaskRoot["gettime"] = taskIter->mValue.m_nGettime;
				CatTaskTemplate * catTaskTemp = CATTASK_TABLE().get(taskIter->mValue.m_nTaskId);
				if(!catTaskTemp)
				{
					LOG_INFO("design error!!!");
					assert(false);
				}
				if(taskIter->mValue.m_nGettime == 0)
				{
					//δ��ǲ����ʣ��ʱ�䶼Ϊ0
					catTaskRoot["remaintime"] = 0;
				}
				else
				{
					//��ǲ�е�����Ҫ�жϾ���ʱ���Ƿ��������ʱ��
					UInt64 costTime = curTime - taskIter->mValue.m_nGettime;

					if(costTime > catTaskTemp->mTime)
					{
						catTaskRoot["remaintime"] = 0;
					}
					else
					{
						catTaskRoot["remaintime"] = catTaskTemp->mTime - costTime;
					}
				}

				root["taskdata"].append(catTaskRoot); 



			}//for(List<XingxiaTask>::

		}  //if(consortData)


	

	//����ǻ᳤���߸��᳤Ҫͬ������б�
	if(m_pPlayerConsortData->m_nConsortJob)
	{
		for(List<ApplyData>::Iter * applyIter = consortData->m_applyList.begin(); 
			applyIter != consortData->m_applyList.end(); 
			applyIter = consortData->m_applyList.next(applyIter))
		{
			Json::Value applyRoot;
			applyRoot["playeruid"] = applyIter->mValue.m_nApply;
			BaseInfoData baseInfo;
			bool flag = LogicSystem::getSingleton().getBaseInfo(applyIter->mValue.m_nApply,baseInfo);

			if(!flag  )
			{
				continue;
			}

			applyRoot["playername"] = baseInfo.name.c_str();
			applyRoot["playerlv"] = baseInfo.level;
			applyRoot["viplv"] = baseInfo.vipLv;
			applyRoot["model"] = baseInfo.modelId;
			applyRoot["leave"] = baseInfo.leaveTime;
			applyRoot["power"] = baseInfo.power;
			applyRoot["time"] = applyIter->mValue.m_nTime;

			root["checklist"].append(applyRoot);
		}
	}


	//��ӹ�����־ͬ��
	////////////////////////////////////////////////////////////////////////
	for(List<LogData>::Iter * logIter = consortData->m_logList.begin(); 
		logIter != consortData->m_logList.end(); logIter = consortData->m_logList.next(logIter) )
	{

		Json::Value logroot;
		logroot["tempid"] = logIter->mValue.m_nTempId;
		logroot["consortId"] = logIter->mValue.m_nConsortId;
		logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
		logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
		logroot["param1"] = logIter->mValue.m_strParam1.c_str();
		logroot["param2"] = logIter->mValue.m_strParam2.c_str();
		logroot["logtime"]= logIter->mValue.m_nTime;

		root["loglist"].append(logroot);
	}






	/////////////////////////////////////////////////////////////////


   }
	else
	{
		for(List<UInt64>::Iter * applyIter = m_pPlayerConsortData->m_nApplyList.begin();  applyIter != NULL; 
			applyIter = m_pPlayerConsortData->m_nApplyList.next(applyIter))
		{
			root["appications"].append(applyIter->mValue);
		}


	}

	root["curcontribut"] = m_pPlayerConsortData->m_nCurContribute;
	root["totalcontribut"] = m_pPlayerConsortData->m_nTotalContribute;

	UInt32 ticketawardmark = 0;
			if(m_pPlayerConsortData->m_nTicketAwardList.size())
			{
				List<TicketAward>::Iter * ticketAwardIter = m_pPlayerConsortData->m_nTicketAwardList.getn(0);
				if(ticketAwardIter->mValue.m_nActiveTime < time(0))
				{
					ticketawardmark = 1;
				}
			}

			root["ticketaward"] = ticketawardmark;


	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);

	//LOG_INFO("convertDataToJson = %s",jsonStr.c_str());
	return jsonStr;


}

void  ConsortManager::syncRankConsortList()
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(playerUid);
	if(!consortInfoData)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::InvalidParameter;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	    GCConsortListResp consortListResp;
		consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
		consortListResp.mRespJsonStr = jsonStr;
		LOG_INFO("ranksortlist: %s", jsonStr.c_str());
		
		NetworkSystem::getSingleton().sendMsg(consortListResp,connId);

		return ;
	}

	
	if(m_pPlayer->mPlayerData.mConsortData.m_nConsortId)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::PlayerHasConsort;
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	    GCConsortListResp consortListResp;
		consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
		consortListResp.mRespJsonStr = jsonStr;
		LOG_INFO("ranksortlist: %s", jsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(consortListResp,connId);
		return ;
	}

	Json::Value root;
	root["errorId"] =  LynxErrno::None;

	const List<ConsortRankData > & rankList = ConsortSystem::getSingleton().getConsortRankList();
	UInt32 totalSize = rankList.size();
	if(m_nIndex > totalSize)
	{
			m_nIndex = 0;
	}
	
	int sendSize = totalSize > 50 ? 50: totalSize;

	const List<ConsortRankData >::Iter * rankIter = rankList.getn(m_nIndex);
	UInt32 count = 0;
	while(count < sendSize)
	{
		ConsortData * consortDataIter = ConsortSystem::getSingleton().getConsortDataById(rankIter->mValue.m_nId);
		ASSERT(consortDataIter);
		Json::Value consortRoot;
		consortRoot["consortid"] = rankIter->mValue.m_nId;
		if(rankIter)
		{
			consortRoot["power"] = rankIter->mValue.m_nPower;
			consortRoot["rank"] = rankIter->mValue.m_nRank;
		}
		else
		{
			consortRoot["power"] = consortDataIter->m_nPower;
			consortRoot["rank"] = ConsortSystem::getSingleton().getConsortLastRank();
		}
		
		consortRoot["consortname"] = consortDataIter->m_strName.c_str();
		consortRoot["lv"] = consortDataIter->m_nLv;
		consortRoot["count"] = consortDataIter->m_nCount;
		consortRoot["condition"] = consortDataIter->m_nPowerLimit;
		consortRoot["check"] = consortDataIter->m_nCheck;
		consortRoot["leader"] = consortDataIter->m_nLeader;
		BaseInfoData baseInfo;
		LogicSystem::getSingleton().getBaseInfo(consortDataIter->m_nLeader, baseInfo);
		
		consortRoot["leadermodel"] = baseInfo.modelId;
		consortRoot["leadername"] = baseInfo.name.c_str();

		root["consortlist"].append(consortRoot);

		//��������
		count++;
		//��������������β������ͷ��
		rankIter = rankList.next(rankIter);
		if(rankIter == rankList.end())
		{
			rankIter = rankList.begin();
		}

		m_nIndex++;
	}

	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
	
	LOG_INFO("ranksortlist: %s", jsonStr.c_str());

	GCConsortListResp consortListResp;
	consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
	consortListResp.mRespJsonStr = jsonStr;

	NetworkSystem::getSingleton().sendMsg(consortListResp,connId);

	
	
}


void ConsortManager::syncConsortList()
{

	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(playerUid);
	if(!consortInfoData)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::InvalidParameter;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	    GCConsortListResp consortListResp;
		consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
		consortListResp.mRespJsonStr = jsonStr;
		LOG_INFO("ranksortlist: %s", jsonStr.c_str());
		
		NetworkSystem::getSingleton().sendMsg(consortListResp,connId);

		return ;
	}

	
	if(m_pPlayer->mPlayerData.mConsortData.m_nConsortId)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::PlayerHasConsort;
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	    GCConsortListResp consortListResp;
		consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
		consortListResp.mRespJsonStr = jsonStr;
		LOG_INFO("ranksortlist: %s", jsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(consortListResp,connId);
		return ;
	}

	Json::Value root;
	root["errorId"] =  LynxErrno::None;

	const List<ConsortData > & rankList = ConsortSystem::getSingleton().getConsortList();
	
	UInt32 totalSize = rankList.size();
	if(m_nIndex >= totalSize)
	{
			m_nIndex = 0;
	}
	
	int sendSize = totalSize > 50 ? 50: totalSize;

	const List<ConsortData >::Iter * rankIter = rankList.getn(m_nIndex);
	UInt32 count = 0;
	while(count < sendSize)
	{
		ConsortData * consortDataIter = ConsortSystem::getSingleton().getConsortDataById(rankIter->mValue.m_nId);
		ASSERT(consortDataIter);
		Json::Value consortRoot;
		consortRoot["consortid"] = rankIter->mValue.m_nId;
		consortRoot["power"] = rankIter->mValue.m_nPower;
		
		consortRoot["consortname"] = consortDataIter->m_strName.c_str();
		consortRoot["lv"] = consortDataIter->m_nLv;
		consortRoot["count"] = consortDataIter->m_nCount;
		consortRoot["condition"] = consortDataIter->m_nPowerLimit;
		consortRoot["check"] = consortDataIter->m_nCheck;
		consortRoot["leader"] = consortDataIter->m_nLeader;
		BaseInfoData baseInfo;
		LogicSystem::getSingleton().getBaseInfo(consortDataIter->m_nLeader, baseInfo);
		
		consortRoot["leadermodel"] = baseInfo.modelId;
		consortRoot["leadername"] = baseInfo.name.c_str();

		root["consortlist"].append(consortRoot);

		//��������
		count++;
		//��������������β������ͷ��
		rankIter = rankList.next(rankIter);
		if(rankIter == rankList.end())
		{
			rankIter = rankList.begin();
		}

		m_nIndex++;
	}

	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
	
	LOG_INFO("ranksortlist: %s", jsonStr.c_str());

	GCConsortListResp consortListResp;
	consortListResp.mPacketID = BOC_CONSORTLIST_RESP;
	consortListResp.mRespJsonStr = jsonStr;

	NetworkSystem::getSingleton().sendMsg(consortListResp,connId);


}


void ConsortManager::findConsortById(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(playerUid);
	if(!consortInfoData)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::InvalidParameter;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	  
		cout << jsonStr << endl;
		

		GCConsortFindResp consortFindResp;
		consortFindResp.mPacketID = BOC_CONSORTFIND_RESP;
		consortFindResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(consortFindResp,connId);

		return ;
	}

	if(m_pPlayer->mPlayerData.mConsortData.m_nConsortId)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::PlayerHasConsort;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	  
		cout << jsonStr << endl;
		

		GCConsortFindResp consortFindResp;
		consortFindResp.mPacketID = BOC_CONSORTFIND_RESP;
		consortFindResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(consortFindResp,connId);
		return ;
	}

	ConsortData * consortDataIter = ConsortSystem::getSingleton().getConsortDataById(consortId);
	ConsortRankData * consortRankIter = ConsortSystem::getSingleton().getConsortRankData(consortId);
	if(!consortDataIter && !consortRankIter)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::ConsortNotExist;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);

	  
		cout << jsonStr << endl;
		

		GCConsortFindResp consortFindResp;
		consortFindResp.mPacketID = BOC_CONSORTFIND_RESP;
		consortFindResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(consortFindResp,connId);

		return ;


	}

	
	Json::Value consortRoot;

	if(consortRankIter)
	{
			consortRoot["power"] = consortRankIter->m_nPower;
			consortRoot["rank"] = consortRankIter->m_nRank;
	}
	else
	{
		UInt32 rank = ConsortSystem::getSingleton().getConsortLastRank();
		consortRoot["power"] = consortDataIter->m_nPower;
		consortRoot["rank"] = rank;
	}



	consortRoot["consortid"] = consortDataIter->m_nId;
	
	consortRoot["consortname"] = consortDataIter->m_strName.c_str();
	consortRoot["lv"] = consortDataIter->m_nLv;
	consortRoot["count"] = consortDataIter->m_nCount;
	consortRoot["condition"] = consortDataIter->m_nPowerLimit;
	consortRoot["check"] = consortDataIter->m_nCheck;
	consortRoot["leader"] = consortDataIter->m_nLeader;
	BaseInfoData baseInfo;
	LogicSystem::getSingleton().getBaseInfo(consortDataIter->m_nLeader, baseInfo);
		
	consortRoot["leadermodel"] = baseInfo.modelId;
	consortRoot["leadername"] = baseInfo.name.c_str();
	consortRoot["errorId"] = LynxErrno::None;
	
	Json::StyledWriter writer;
	std::string jsonStr = writer.write(consortRoot);
	
	cout << jsonStr;

	GCConsortFindResp consortFindResp;
	consortFindResp.mPacketID = BOC_CONSORTFIND_RESP;
	consortFindResp.mRespJsonStr = jsonStr;

	NetworkSystem::getSingleton().sendMsg(consortFindResp,connId);

}

void ConsortManager::quickJoinConsort()
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	
	if(m_pPlayer->mPlayerData.mConsortData.m_nConsortId)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::PlayerHasConsort;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);
 
		cout << jsonStr << endl;
		
		GCConsortQuickJoinResp quickJoinResp;
		quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
		quickJoinResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
		return ;
	}
	
	Player * selfPlayer = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	UInt32 playerPower = selfPlayer->getPlayerPower() ;
	
	Map<UInt64, ConsortData *>  *  pConsortMap = ConsortSystem::getSingleton().getConsortMap();

	List<ConsortData *> consortTempList;
	for(Map<UInt64, ConsortData *>::Iter * consortIter = pConsortMap->begin();
		consortIter != pConsortMap->end(); consortIter = pConsortMap->next(consortIter))
	{
		//�����Ҫ��˻��߰��ս�����ƴ��ڵ������ս��
		//���߰����������10000���Ժ�������ݲ߻����õ���������
		ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(consortIter->mValue->m_nLv);
		
		if(!consortLvTemp)
		{
			continue;
		}

		if(consortIter->mValue->m_nCheck || consortIter->mValue-> m_nPowerLimit >= playerPower ||  
			consortIter->mValue->m_nCount >= consortLvTemp->mCount )
		{
			continue;
		}

		consortTempList.insertTail(consortIter->mValue);
	}

	//�����һ������Ҫ��˲�������δ���Ĺ���
	UInt32 totalSize = consortTempList.size();
	if(!totalSize)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::NoConsortMatch;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);
 
		cout << jsonStr << endl;
		
		GCConsortQuickJoinResp quickJoinResp;
		quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
		quickJoinResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
		return ;
	}

	UInt32 res = rand()%totalSize;

	//�þ��ŵĳ�Ա������
	List<ConsortData *> ::Iter * consortFinalIter = consortTempList.getn(res);
	consortFinalIter->mValue->m_nCount++;
	consortFinalIter->mValue->m_nMemberList.insertTail(playerUid);
	//����ս����֮����
	consortFinalIter->mValue->m_nPower += playerPower;



	//�����е������б�������и���ң���ô�������ҵ�����

	//���ҵ����id�������б��е�λ��
	List<ApplyData>::Iter * applyIter = consortFinalIter->mValue->m_applyList.begin();
		for( ;  applyIter != consortFinalIter->mValue->m_applyList.end();  
			applyIter = consortFinalIter->mValue->m_applyList.next(applyIter))
		{
			if(applyIter->mValue.m_nApply == playerUid)
			{
				
				break;
			}

		}
	
		if(applyIter != consortFinalIter->mValue->m_applyList.end())
		{
				//�����Ƴ���ҵ�����
				consortFinalIter->mValue->m_applyList.erase(applyIter);
		}
	

	//��������֮ǰ���������
	m_pPlayer->mPlayerData.mConsortData.m_nApplyList.clear();
	//�˴�������������ݿ�����ļ�¼

	//�������������Ϣ
	PersistConsortApplyClear applyClear;
	applyClear.m_nPlayerUid = playerUid;
	
	PersistSystem::getSingleton().postThreadMsg(applyClear, playerUid);

	

	//��������������������Ϊ0
	m_pPlayer->mPlayerData.mConsortData.m_nCurContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nTotalContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nConsortJob = 0;
	//���ŵ�id����Ϊ�þ���
	m_pPlayer->mPlayerData.mConsortData.m_nConsortId = consortFinalIter->mValue->m_nId;

	//������ҳ�פ�ڴ���Ϣ
	LogicSystem::getSingleton().updateConsortInfo(playerUid,m_pPlayer->mPlayerData.mConsortData);

	//��ʱ����
	//�����Ϣ����
	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = playerUid;
	playerConsortSave.m_nConsortId = m_pPlayer->mPlayerData.mConsortData.m_nConsortId;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
	playerConsortSave.m_nLeaveTime = m_pPlayer->mPlayerData.mConsortData.m_nLeaveTime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerUid);

	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = consortFinalIter->mValue->m_nId;
	consortSave.m_nConsortLv = consortFinalIter->mValue->m_nLv;
    consortSave.m_strConsortName = consortFinalIter->mValue->m_strName;
	consortSave.m_nCount = consortFinalIter->mValue->m_nCount;
	consortSave.m_nPower = consortFinalIter->mValue->m_nPower;
    consortSave.m_strDescs = consortFinalIter->mValue->m_strDesc;
	consortSave.m_nRes = consortFinalIter->mValue->m_nRes;
    consortSave.m_nExp = consortFinalIter->mValue->m_nExp;
	consortSave.m_nCheck = consortFinalIter->mValue->m_nCheck;
	consortSave.m_nPowerLimit = consortFinalIter->mValue->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);

	gameActiveInit(m_pPlayer, playerUid);


	//���������־
	addConsortLog(consortFinalIter->mValue, CNLMEMBERJOIN, consortFinalIter->mValue->m_nId, playerUid,
		m_pPlayer->getPlayerName().c_str(),  m_pPlayer->getPlayerName().c_str(), "", time(0));
	//����������������Ϣ

	//���ͻذ����ͻ���
	
	Json::Value root;
	root["errorId"] =  LynxErrno::None;
	root["consortid"] = consortFinalIter->mValue->m_nId;
	root["consortname"] = consortFinalIter->mValue->m_strName.c_str();
	root["consortlv"] = consortFinalIter->mValue->m_nLv;
	root["count"] = consortFinalIter->mValue->m_nCount;
	
	ConsortRankData * consortRankIter = ConsortSystem::getSingleton().getConsortRankData(consortFinalIter->mValue->m_nId);

	if(consortRankIter)
	{
			root["rank"] = consortRankIter->m_nRank;
			root["power"] = consortRankIter->m_nPower;
	}
	else
	{
		UInt32 rank = ConsortSystem::getSingleton().getConsortLastRank();
		root["rank"] = rank;
		root["power"] = consortFinalIter->mValue->m_nPower;
	}


	
	root["check"] = consortFinalIter->mValue->m_nCheck;
	root["condition"] = consortFinalIter->mValue->m_nPowerLimit;
	root["curcontribut"] = 0;
	root["totalcontribut"] = 0;
	root["desc"] = consortFinalIter->mValue->m_strDesc.c_str();
	root["consortres"] = consortFinalIter->mValue->m_nRes;
	root["consortjob"] = 0;
	

	root["leader"] = consortFinalIter->mValue->m_nLeader;

	BaseInfoData baseInfo;
	LogicSystem::getSingleton().getBaseInfo(consortFinalIter->mValue->m_nLeader, baseInfo);
		
	root["leadermodel"] = baseInfo.modelId;
	root["leadername"] = baseInfo.name.c_str();

	root["manutype"] = 1;
	root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;

	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
 
	cout << jsonStr << endl;

	GCConsortQuickJoinResp quickJoinResp;
	quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
	quickJoinResp.mRespJsonStr = jsonStr;

	NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);

	//���֪ͨ�������
	notifyOtherOnlineConcortMember(NOTIFYJOINCONSORT, playerUid, consortFinalIter->mValue);


}

void ConsortManager::joinConsort(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	
	if(m_pPlayer->mPlayerData.mConsortData.m_nConsortId)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::PlayerHasConsort;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);
 
		cout << jsonStr << endl;
		
		GCConsortQuickJoinResp quickJoinResp;
		quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
		quickJoinResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
		return ;
	}

	UInt32 playerPower = m_pPlayer->getPlayerPower() ;
	ConsortData *  consortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!consortData)
	{
		Json::Value root;
		root["errorId"] =  LynxErrno::ConsortNotExist;
	
	   Json::StyledWriter writer;
	   std::string jsonStr = writer.write(root);
 
		cout << jsonStr << endl;
		
		GCConsortQuickJoinResp quickJoinResp;
		quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
		quickJoinResp.mRespJsonStr = jsonStr;

		NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
		return ;
	}

	ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(consortData->m_nLv);

	if(consortData->m_nCount >= consortLvTemp->mCount)
	{
			Json::Value root;
			root["errorId"] =  LynxErrno::ConsortFull;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			GCConsortQuickJoinResp quickJoinResp;
			quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			quickJoinResp.mRespJsonStr = jsonStr;

			NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
			return ;
	}

	if(!consortLvTemp || consortData-> m_nPowerLimit >= playerPower  )
		{
			Json::Value root;
			root["errorId"] =  LynxErrno::PlayerPowerLess;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			GCConsortQuickJoinResp quickJoinResp;
			quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			quickJoinResp.mRespJsonStr = jsonStr;

			NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);
			return ;
		}

	if(consortData->m_nCheck)
		{
			List<UInt64>::Iter * findIter = m_pPlayerConsortData->m_nApplyList.find(consortId, m_pPlayerConsortData->m_nApplyList.begin(),
				m_pPlayerConsortData->m_nApplyList.end());
		
			if(findIter == m_pPlayerConsortData->m_nApplyList.end())
			{
				ApplyData applyData;
				applyData.m_nApply = playerUid;
				applyData.m_nTime = time(0) -1 ;

				//���ŵ���������б����������id��ʱ��
				consortData->m_applyList.insertTail(applyData);
				m_pPlayerConsortData->m_nApplyList.insertTail(consortId);

				//���̱������������Ϣ
				PersistConsortApplyInsert applyInsert;
				applyInsert.m_nConsortId = consortId;
				applyInsert.m_nPlayerUid = playerUid;
				applyInsert.m_nTime = applyData.m_nTime;
				PersistSystem::getSingleton().postThreadMsg(applyInsert, playerUid);
			}
		
		

			Json::Value root;
			root["errorId"] =  LynxErrno::ConsortNeedCheck;
			root["consortid"] = consortData->m_nId;
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			GCConsortQuickJoinResp quickJoinResp;
			quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			quickJoinResp.mRespJsonStr = jsonStr;

			NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);

			notifyOtherOnlineConcortMember(APPLYADD, playerUid, consortData, 1);
	

			return ;
		}



	//�þ��ŵĳ�Ա������
	
	consortData->m_nCount++;
	consortData->m_nMemberList.insertTail(playerUid);
	//����ս����֮����
	consortData->m_nPower += playerPower;

	//�����е������б�������и���ң���ô�������ҵ�����

	//���ҵ����id�������б��е�λ��
		List<ApplyData>::Iter * applyIter = consortData->m_applyList.begin();
		for( ;  applyIter != consortData->m_applyList.end();  applyIter = consortData->m_applyList.next(applyIter))
		{
			if(applyIter->mValue.m_nApply == playerUid)
			{
				
				break;
			}

		}
	
		if(applyIter != consortData->m_applyList.end())
		{
				//�����Ƴ���ҵ�����
				consortData->m_applyList.erase(applyIter);
		}
	

	//��������֮ǰ���������
	m_pPlayer->mPlayerData.mConsortData.m_nApplyList.clear();
	//�˴�������������ݿ�����ļ�¼

	//�������������Ϣ
	PersistConsortApplyClear applyClear;
	applyClear.m_nPlayerUid = playerUid;
	
	PersistSystem::getSingleton().postThreadMsg(applyClear, playerUid);

	

	//��������������������Ϊ0
	m_pPlayer->mPlayerData.mConsortData.m_nCurContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nTotalContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nConsortJob = 0;
	//���ŵ�id����Ϊ�þ���
	m_pPlayer->mPlayerData.mConsortData.m_nConsortId = consortData->m_nId;

	//������ҳ�פ�ڴ���Ϣ
	LogicSystem::getSingleton().updateConsortInfo(playerUid,m_pPlayer->mPlayerData.mConsortData);

	//��ʱ����
	//�����Ϣ����
	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = playerUid;
	playerConsortSave.m_nConsortId = m_pPlayer->mPlayerData.mConsortData.m_nConsortId;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
    playerConsortSave.m_nLeaveTime = m_pPlayer->mPlayerData.mConsortData.m_nLeaveTime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerUid);

	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = consortData->m_nId;
	consortSave.m_nConsortLv = consortData->m_nLv;
    consortSave.m_strConsortName = consortData->m_strName;
	consortSave.m_nCount = consortData->m_nCount;
	consortSave.m_nPower = consortData->m_nPower;
    consortSave.m_strDescs = consortData->m_strDesc;
	consortSave.m_nRes = consortData->m_nRes;
    consortSave.m_nExp = consortData->m_nExp;
	consortSave.m_nCheck = consortData->m_nCheck;
	consortSave.m_nPowerLimit = consortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);

	gameActiveInit(m_pPlayer, playerUid);


	//���������־

	//����������������Ϣ

	//���ͻذ����ͻ���
	
	Json::Value root;
	root["errorId"] =  LynxErrno::None;
	root["consortid"] = consortData->m_nId;
	root["consortname"] = consortData->m_strName.c_str();
	root["consortlv"] = consortData->m_nLv;
	root["count"] = consortData->m_nCount;
	
	ConsortRankData * consortRankData = ConsortSystem::getSingleton().getConsortRankData(consortData->m_nId);

	if(consortRankData)
	{
			root["rank"] = consortRankData->m_nRank;
			root["power"] = consortRankData->m_nPower;
	}
	else
	{
		UInt32 rank = ConsortSystem::getSingleton().getConsortLastRank();
		root["rank"] = rank;
		root["power"] = consortData->m_nPower;
	}
	

		


	root["check"] = consortData->m_nCheck;
	root["condition"] = consortData->m_nPowerLimit;
	root["curcontribut"] = 0;
	root["totalcontribut"] = 0;
	root["desc"] = consortData->m_strDesc.c_str();
	root["consortres"] = consortData->m_nRes;
	root["consortjob"] = 0;
	

	root["leader"] = consortData->m_nLeader;

	BaseInfoData baseInfo;
	LogicSystem::getSingleton().getBaseInfo(consortData->m_nLeader, baseInfo);
		
	root["leadermodel"] = baseInfo.modelId;
	root["leadername"] = baseInfo.name.c_str();
	root["manutype"] = 2;
	root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;

	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
 
	cout << jsonStr << endl;

	GCConsortQuickJoinResp quickJoinResp;
	quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
	quickJoinResp.mRespJsonStr = jsonStr;

	NetworkSystem::getSingleton().sendMsg(quickJoinResp,connId);

	std::string playerName = m_pPlayer->getPlayerName();
	addConsortLog(consortData, CNLMEMBERJOIN, consortData->m_nId, playerUid,
				playerName.c_str(),  playerName.c_str(), "", time(0));

	

	
	
		notifyOtherOnlineConcortMember(NOTIFYJOINCONSORT, playerUid, consortData);
	
	

}


void ConsortManager::clearApplyData(PlayerConsortData * playerConsortData)
{
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	for(List<UInt64>::Iter * applyIter = playerConsortData->m_nApplyList.begin(); applyIter != NULL;
	applyIter = playerConsortData->m_nApplyList.next(applyIter))
	{
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(applyIter->mValue);
		if(!pConsortData)
		{
			continue;
		}

		List<ApplyData>::Iter * playerIter = pConsortData->m_applyList.begin();
		for( ;  playerIter != pConsortData->m_applyList.end();  playerIter = pConsortData->m_applyList.next(playerIter))
		{
			if(playerIter->mValue.m_nApply == playerUid)
			{

				break;
			}

		}

		if(playerIter != pConsortData->m_applyList.end())
		{
			//�����Ƴ���ҵ�����
			pConsortData->m_applyList.erase(playerIter);
		}

	}
	
	//��������֮ǰ���������
	playerConsortData->m_nApplyList.clear();
	//�˴�������������ݿ�����ļ�¼

	//�������������Ϣ
	PersistConsortApplyClear applyClear;
	applyClear.m_nPlayerUid = playerUid;

	PersistSystem::getSingleton().postThreadMsg(applyClear, playerUid);
}


void ConsortManager::createConsort(std::string name)
{
	UInt32 vipLv = m_pPlayer->getVipLevel();
	UInt64 gold = m_pPlayer->getPlayerGold();
	const ConnId& connId = m_pPlayer->getConnId();
	if(vipLv < CREATENEEDVIPLV)
	{
		GCConsortQuickJoinResp consortCreateResp;
			consortCreateResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::VipLevelNotEnough;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("create resp: %s", jsonStr.c_str());
			
		
			consortCreateResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(consortCreateResp,connId);

		return ;
	}

	if(gold < CREATENEEDGOLD)
	{
			GCConsortQuickJoinResp consortCreateResp;
			consortCreateResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::RmbNotEnough;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("create resp: %s", jsonStr.c_str());
		
			consortCreateResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(consortCreateResp,connId);

		return ;
	}

	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
		bool isSame = ConsortSystem::getSingleton().judgeSameNameConsort(name);
		if(isSame)
		{
			GCConsortQuickJoinResp consortCreateResp;
			consortCreateResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::ConsortNameExist;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("create resp: %s", jsonStr.c_str());
		
			consortCreateResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(consortCreateResp,connId);
		}
		else
		{
			Goods goods;
			List<Goods> itemList;
			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - 1000;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog75);
			LOG_INFO("beginCreateConsort!!!!!!!");
			ConsortData * pConsortData = ConsortSystem::getSingleton().createConsort(name, m_pPlayer);
			clearApplyData(m_pPlayerConsortData);
			gameActiveInit(m_pPlayer, playerUid);

			ConsortSystem::getSingleton().ticketQuestionInit(pConsortData->m_nId);

			GCConsortQuickJoinResp consortCreateResp;
			consortCreateResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
			Json::Value root;
			root["errorId"] = LynxErrno::None;
				
			root["consortid"] = pConsortData->m_nId;
			root["consortname"] = pConsortData->m_strName.c_str();
			root["consortlv"] = pConsortData->m_nLv;
			root["count"] = pConsortData->m_nCount;
	
			UInt32 rank = ConsortSystem::getSingleton().getConsortLastRank();

			root["rank"] = rank;
			root["power"] = pConsortData->m_nPower;
			root["check"] = pConsortData->m_nCheck;
			root["condition"] = pConsortData->m_nPowerLimit;
			root["curcontribut"] = 0;
			root["totalcontribut"] = 0;
			root["desc"] = pConsortData->m_strDesc.c_str();
			root["consortres"] = pConsortData->m_nRes;
			root["consortjob"] = 1;
	
			root["leader"] = pConsortData->m_nLeader;

			BaseInfoData baseInfo;
			LogicSystem::getSingleton().getBaseInfo(pConsortData->m_nLeader, baseInfo);
		
			root["leadermodel"] = baseInfo.modelId;
			root["leadername"] = baseInfo.name.c_str();
			root["gold"] = gold - 1000;
			root["manutype"] = 0;
			root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;

			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("create resp: %s", jsonStr.c_str());
		
			consortCreateResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(consortCreateResp,connId);

			//������־����
			std::string playerName = m_pPlayer->getPlayerName();
			
			addConsortLog(pConsortData, CNLCREATE, pConsortData->m_nId, playerUid,
				playerName.c_str(),  playerName.c_str(), pConsortData->m_strName.c_str(),  time(0));
			
			
		}
}

void ConsortManager::consortMemberSync(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		GCConsortMemberResp memberResp;
		memberResp.mPacketID = BOC_CONSORTMEMBER_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			memberResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(memberResp,connId);

	
		return ;
	}


	Json::Value root;
	root["errorId"] =  LynxErrno::None;
	root["consortid"] = pConsortData->m_nId;
	root["consortname"] = pConsortData->m_strName.c_str();
	root["consortlv"] = pConsortData->m_nLv;
	root["count"] = pConsortData->m_nCount;
	
	ConsortRankData * consortRankData = ConsortSystem::getSingleton().getConsortRankData(pConsortData->m_nId);
    if(!consortRankData)
	{
		root["rank"] = ConsortSystem::getSingleton().getConsortLastRank() ;
		root["power"] = pConsortData->m_nPower;
	}
	else
	{
		root["rank"] = consortRankData->m_nRank;
		root["power"] = consortRankData->m_nPower;
	}
	
	root["check"] = pConsortData->m_nCheck;
	root["condition"] = pConsortData->m_nPowerLimit;

	root["desc"] = pConsortData->m_strDesc.c_str();
	root["consortres"] = pConsortData->m_nRes;
	root["exp"] = pConsortData->m_nExp;
	for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
		memberIter != pConsortData->m_nMemberList.end();  
		memberIter = pConsortData->m_nMemberList.next(memberIter))
	{
		Json::Value memberRoot;
		memberRoot["playeruid"]= memberIter->mValue;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(memberIter->mValue,baseInfo);
		ConsortInfoData *  consortInfo = LogicSystem::getSingleton().getConsortInfo(memberIter->mValue);
		if(!flag  || !consortInfo)
		{
			continue;
		}

		memberRoot["playername"] = baseInfo.name.c_str();
		memberRoot["playerlv"] = baseInfo.level;
		memberRoot["viplv"] = baseInfo.vipLv;
		memberRoot["model"] = baseInfo.modelId;
		memberRoot["leave"] = baseInfo.leaveTime;
		memberRoot["power"] = baseInfo.power;
		memberRoot["curcontribut"] = consortInfo->curContribute;
		memberRoot["totalcontribut"] = consortInfo->totalContribute;
		memberRoot["job"] = consortInfo->consortJob;
    
		root["member"].append(memberRoot);
        

	}

	GCConsortMemberResp memberResp;
		memberResp.mPacketID = BOC_CONSORTMEMBER_RESP;
	
		Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			memberResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(memberResp,connId);
}

void ConsortManager::consortDetailSync(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	if (RankGameManager::getSingleton().checkSystemOpen(m_pPlayer->getPlayerGuid(),SYSTEM_CONSORT) == false)
	{
		GCConsortDetailResp detailResp;
		detailResp.mPacketID = BOC_CONSORTDETAIL_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::LevelNotEnough;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("consort detail info is: %s ", jsonStr.c_str()) ;
		
			detailResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(detailResp,connId);

		return;
	}



	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		GCConsortDetailResp detailResp;
		detailResp.mPacketID = BOC_CONSORTDETAIL_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			LOG_INFO("consort detail info is: %s ", jsonStr.c_str()) ;
		
			detailResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(detailResp,connId);

	
		return ;
	}


	Json::Value root;
	root["errorId"] =  LynxErrno::None;
	root["consortid"] = pConsortData->m_nId;
	root["consortname"] = pConsortData->m_strName.c_str();
	root["consortlv"] = pConsortData->m_nLv;
	root["count"] = pConsortData->m_nCount;
	
	ConsortRankData * consortRankData = ConsortSystem::getSingleton().getConsortRankData(pConsortData->m_nId);
    if(!consortRankData)
	{
		root["rank"] = ConsortSystem::getSingleton().getConsortLastRank() ;
		root["power"] = pConsortData->m_nPower;
	}
	else
	{
		root["rank"] = consortRankData->m_nRank;
		root["power"] = consortRankData->m_nPower;
	}
	
	root["check"] = pConsortData->m_nCheck;
	root["condition"] = pConsortData->m_nPowerLimit;

	root["desc"] = pConsortData->m_strDesc.c_str();
	root["consortres"] = pConsortData->m_nRes;
	root["exp"] = pConsortData->m_nExp;
	for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
		memberIter != pConsortData->m_nMemberList.end();  
		memberIter = pConsortData->m_nMemberList.next(memberIter))
	{
		Json::Value memberRoot;
		memberRoot["playeruid"]= memberIter->mValue;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(memberIter->mValue,baseInfo);
		ConsortInfoData *  consortInfo = LogicSystem::getSingleton().getConsortInfo(memberIter->mValue);
		if(!flag  || !consortInfo)
		{
			continue;
		}

		memberRoot["playername"] = baseInfo.name.c_str();
		memberRoot["playerlv"] = baseInfo.level;
		memberRoot["viplv"] = baseInfo.vipLv;
		memberRoot["model"] = baseInfo.modelId;
		memberRoot["leave"] = baseInfo.leaveTime;
		memberRoot["power"] = baseInfo.power;
		memberRoot["curcontribut"] = consortInfo->curContribute;
		memberRoot["totalcontribut"] = consortInfo->totalContribute;
		memberRoot["job"] = consortInfo->consortJob;
    
		root["member"].append(memberRoot);
        

	}

	UInt64 checktime = time(0);
	root["checktime"] = checktime;

	for(List<ApplyData>::Iter * applyIter = pConsortData->m_applyList.begin(); 
		applyIter != pConsortData->m_applyList.end(); 
		applyIter = pConsortData->m_applyList.next(applyIter))
	{
		Json::Value applyRoot;
		applyRoot["playeruid"] = applyIter->mValue.m_nApply;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(applyIter->mValue.m_nApply,baseInfo);
		
		if(!flag  )
		{
			continue;
		}

		applyRoot["playername"] = baseInfo.name.c_str();
		applyRoot["playerlv"] = baseInfo.level;
		applyRoot["viplv"] = baseInfo.vipLv;
		applyRoot["model"] = baseInfo.modelId;
		applyRoot["leave"] = baseInfo.leaveTime;
		applyRoot["power"] = baseInfo.power;
		applyRoot["time"] = applyIter->mValue.m_nTime;
		root["applies"].append(applyRoot);
	}

	
		for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
		logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
		{
			 
				Json::Value logroot;
				logroot["tempid"] = logIter->mValue.m_nTempId;
				logroot["consortId"] = logIter->mValue.m_nConsortId;
				logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
				logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
				logroot["param1"] = logIter->mValue.m_strParam1.c_str();
				logroot["param2"] = logIter->mValue.m_strParam2.c_str();
				logroot["logtime"]= logIter->mValue.m_nTime;

				root["loglist"].append(logroot);
		}

		//ǩ���ֶ����

			root["consortactive"] = pConsortData->m_nActive;
			root["signlv"] = pConsortData->m_nSignLv;
			for(List<UInt32>::Iter * awardIter = m_pPlayerConsortData->m_nSignAwards.begin();
				awardIter != NULL; awardIter = m_pPlayerConsortData->m_nSignAwards.next(awardIter) )
			{
				root["signawards"].append(awardIter->mValue);
			}

		
			root["sign"] = m_pPlayerConsortData->m_nSign;

			/////////////////////////////////////////////////////////
				// ������������ֶ����
			root["taskrefresh"] = m_pPlayerConsortData->m_nRefreshTimes;
			root["kitchentimes"] = m_pPlayerConsortData->m_nKitchenTimes;
			root["eyetimes"] = m_pPlayerConsortData->m_nEyeSightTimes;
			root["eloquencetimes"] = m_pPlayerConsortData->m_nEloquenceTimes;

			UInt64 endTime = ConsortSystem::getSingleton().getAwardActiveTime();
			UInt64 curTime = time(0);
	        if(curTime  > endTime || curTime < endTime - 3600)
	       {
				root["ticketopen"] = 0;
	       }
			else
			{
				root["ticketopen"] = 1;
			}


			UInt32 ticketawardmark = 0;
			if(m_pPlayerConsortData->m_nTicketAwardList.size())
			{
				List<TicketAward>::Iter * ticketAwardIter = m_pPlayerConsortData->m_nTicketAwardList.getn(0);
				if(ticketAwardIter->mValue.m_nActiveTime < time(0))
				{
					ticketawardmark = 1;
				}
			}

			root["ticketaward"] = ticketawardmark;
			root["tickettimes"] = m_pPlayerConsortData->m_nTicketTimes;

			

			root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			root["woodaward"] = m_pPlayerConsortData->m_nWoodCatFlag;
		
			if(ticketawardmark)
			{
				UInt32 count = m_pPlayerConsortData->m_nTicketAwardList.size();
					root["ticketawardcount"] = m_pPlayerConsortData->m_nTicketAwardList.size();
			}


			
			for(List<XingxiaTask>::Iter *  taskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin();
				taskIter != NULL;  taskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(taskIter) )
			{
				Json::Value catTaskRoot;
				catTaskRoot["taskid"] = taskIter->mValue.m_nTaskId;
				catTaskRoot["gettime"] = taskIter->mValue.m_nGettime;
				CatTaskTemplate * catTaskTemp = CATTASK_TABLE().get(taskIter->mValue.m_nTaskId);

				if(taskIter->mValue.m_nGettime == 0)
				{
					//δ��ǲ����ʣ��ʱ�䶼Ϊ0
					catTaskRoot["remaintime"] = 0;
				}
				else
				{
					//��ǲ�е�����Ҫ�жϾ���ʱ���Ƿ��������ʱ��
					UInt64 costTime = curTime - taskIter->mValue.m_nGettime;

					if(costTime > catTaskTemp->mTime)
					{
						catTaskRoot["remaintime"] = 0;
					}
					else
					{
						catTaskRoot["remaintime"] = catTaskTemp->mTime - costTime;
					}
				}

				root["taskdata"].append(catTaskRoot); 

			}




			/////////////////////////////////////////////////////////////



	GCConsortDetailResp detailResp;
	detailResp.mPacketID = BOC_CONSORTDETAIL_RESP;
	
	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
 
	LOG_INFO("consort detail info is: %s ", jsonStr.c_str()) ;
		
	detailResp.mRespJsonStr = jsonStr;
	NetworkSystem::getSingleton().sendMsg(detailResp,connId);

}


void ConsortManager::checkApply(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		GCApplyCheckResp checkResp;
		checkResp.mPacketID = BOC_APPLYCHECK_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			checkResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(checkResp,connId);

	
		return ;
	}

	Json::Value root;
	root["errorId"] = LynxErrno::None;

	for(List<ApplyData>::Iter * applyIter = pConsortData->m_applyList.begin(); 
		applyIter != pConsortData->m_applyList.end(); 
		applyIter = pConsortData->m_applyList.next(applyIter))
	{
		Json::Value applyRoot;
		applyRoot["playeruid"] = applyIter->mValue.m_nApply;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(applyIter->mValue.m_nApply,baseInfo);
		
		if(!flag  )
		{
			continue;
		}

		applyRoot["playername"] = baseInfo.name.c_str();
		applyRoot["playerlv"] = baseInfo.level;
		applyRoot["viplv"] = baseInfo.vipLv;
		applyRoot["model"] = baseInfo.modelId;
		applyRoot["leave"] = baseInfo.leaveTime;
		applyRoot["power"] = baseInfo.power;
		applyRoot["time"] = applyIter->mValue.m_nTime;
		root["applies"].append(applyRoot);


	
	}

	UInt64 checktime = time(0);
	root["checktime"] = checktime;

	GCApplyCheckResp checkResp;
	checkResp.mPacketID = BOC_APPLYCHECK_RESP;
	
		Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			checkResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(checkResp,connId);

}

bool ConsortManager::judgeApplyExist(ConsortData * pConsortData, UInt64 applyUid)
{
	//���ҵ����id�������б��е�λ��
		List<ApplyData>::Iter * applyIter = pConsortData->m_applyList.begin();
		for( ;  applyIter != pConsortData->m_applyList.end();  applyIter = pConsortData->m_applyList.next(applyIter))
		{
			if(applyIter->mValue.m_nApply == applyUid)
			{
				
				break;
			}

		}

		//�����Ҳ��ھ��ŵ������б���ôֱ�ӷ���
		if(applyIter == pConsortData->m_applyList.end())
		{
		
			return false;
		}

		return true;
}

void ConsortManager::dealApply(UInt64 consortId, UInt64 applyUid, UInt32 accept)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	
	std::string playerName = m_pPlayer->getPlayerName();

	Player*  applyPlayer = LogicSystem::getSingleton().getPlayerByGuid(applyUid);
	//applyPlayer  ����첽�ж��Ƿ���빫���߼�
	if(applyPlayer)
	{
		PlayerConsortData * pPlayerConsortData = applyPlayer->getConsortManager().getPlayerConsortData();
		if(!pPlayerConsortData)
		{
				GCApplyDealResp dealResp;
					dealResp.mPacketID = BOC_APPLYDEAL_RESP;

					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					root["applyuid"] = applyUid;
					root["accept"] = accept;
					
					Json::StyledWriter writer;
					std::string jsonStr = writer.write(root);
 
					cout << jsonStr << endl;
		
					dealResp.mRespJsonStr = jsonStr;
					NetworkSystem::getSingleton().sendMsg(dealResp,connId);
					return;
		}


		if(pPlayerConsortData->m_nConsortId)
		{
			GCApplyDealResp dealResp;
			dealResp.mPacketID = BOC_APPLYDEAL_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerHasConsort;
			root["applyuid"] = applyUid;
					root["accept"] = accept;

			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			dealResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(dealResp,connId);
			return;
		}
	}
	else
	{
				ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(applyUid);
				if(!consortInfoData)
				{
					GCApplyDealResp dealResp;
					dealResp.mPacketID = BOC_APPLYDEAL_RESP;

					Json::Value root;
					root["errorId"] = LynxErrno::InvalidParameter;
					root["applyuid"] = applyUid;
					root["accept"] = accept;

					Json::StyledWriter writer;
					std::string jsonStr = writer.write(root);
 
					cout << jsonStr << endl;
		
					dealResp.mRespJsonStr = jsonStr;
					NetworkSystem::getSingleton().sendMsg(dealResp,connId);

					return;
				}
				if(consortInfoData->consortId)
				{
					GCApplyDealResp dealResp;
					dealResp.mPacketID = BOC_APPLYDEAL_RESP;

					Json::Value root;
					root["errorId"] = LynxErrno::PlayerHasConsort;
					root["applyuid"] = applyUid;
					root["accept"] = accept;

					Json::StyledWriter writer;
					std::string jsonStr = writer.write(root);
 
					cout << jsonStr << endl;
		
					dealResp.mRespJsonStr = jsonStr;
					NetworkSystem::getSingleton().sendMsg(dealResp,connId);
					return;
				}
	}


	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
		GCApplyDealResp dealResp;
		dealResp.mPacketID = BOC_APPLYDEAL_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::PlayerConsortNotFind;
		root["applyuid"] = applyUid;
		root["accept"] = accept;
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			dealResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(dealResp,connId);

	
		return ;
	}

	//�ȴ���û᳤���ڹ��������б��Ƴ���������룬���ҽ�����������б������������������
	//���ҵ����id�������б��е�λ��

	bool eraseres = judgeApplyExist(pConsortData, applyUid);
	if(!eraseres)
	{
			//�����Ҳ��ڹ���������б���ôֱ�ӷ���
			if(applyPlayer)
			{
				//����ҵ������б����Ƴ��ù���������¼
				applyPlayer->getConsortManager().eraseApply(consortId);
			}
		
			GCApplyDealResp dealResp;
			dealResp.mPacketID = BOC_APPLYDEAL_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["applyuid"] = applyUid;
			root["accept"] = accept;
			root["count"]=pConsortData->m_nCount;
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			dealResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(dealResp,connId);

			//���д��̣�ɾ�����ݿ��е���������
		
			PersistConsortApplyDel applyDel;
			applyDel.m_nConsortId = consortId;
			applyDel.m_nPlayerUid = applyUid;
		
			PersistSystem::getSingleton().postThreadMsg(applyDel, applyUid);

			return ;
	}
		
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(applyUid,baseInfo);

		//�����ж���agree����refuse

		if(!accept)
		{
			//�ܾ��������
			
			//����������ҵ������б�
			//������ڴ��У���ô��Ҫ����ڴ��е�������Ϣ
			if(applyPlayer)
			{
				applyPlayer->getConsortManager().eraseApply(consortId);
			}

			ConsortSystem::getSingleton().eraseApplyFromCheckList(consortId, applyUid);

			//���д��̣�ɾ�����ݿ��е���������
		
			PersistConsortApplyDel applyDel;
			applyDel.m_nConsortId = consortId;
			applyDel.m_nPlayerUid = applyUid;
		
			PersistSystem::getSingleton().postThreadMsg(applyDel, applyUid);

			//���ͻ��˻ذ�

				GCApplyDealResp dealResp;
				dealResp.mPacketID = BOC_APPLYDEAL_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::None;
				root["applyuid"] = applyUid;
				root["accept"] = accept;
				root["count"]=pConsortData->m_nCount;
				Json::StyledWriter writer;
				std::string jsonStr = writer.write(root);
 
				cout << jsonStr << endl;
		
				dealResp.mRespJsonStr = jsonStr;
				NetworkSystem::getSingleton().sendMsg(dealResp,connId);

				addConsortLog(pConsortData, CNLREFUSEAPPLY, pConsortData->m_nId, playerUid,
				playerName.c_str(), baseInfo.name.c_str(), "", time(0));

				//���֪ͨ
				notifyOtherOnlineConcortMember(APPLYREFUSE, playerUid,  pConsortData,  1 );


			return ;
		}


		//ͬ��������
		//�ж������Ƿ�ﵽ��������
		ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
		if(pConsortData->m_nCount >= consortLvTemp->mCount)
		{
				GCApplyDealResp dealResp;
				dealResp.mPacketID = BOC_APPLYDEAL_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::ConsortFull;
				root["applyuid"] = applyUid;
				root["accept"] = accept;
				root["count"]=pConsortData->m_nCount;
				Json::StyledWriter writer;
				std::string jsonStr = writer.write(root);
 
				cout << jsonStr << endl;
		
				dealResp.mRespJsonStr = jsonStr;
				NetworkSystem::getSingleton().sendMsg(dealResp,connId);

				

				return ;
		}
	
		pConsortData->m_nCount++;
		pConsortData->m_nMemberList.insertTail(applyUid);
		//����ս����֮����
		pConsortData->m_nPower += baseInfo.power;
		
		//���������б����Ƴ����������
		ConsortSystem::getSingleton().eraseApplyFromCheckList(consortId, applyUid);

	///////////////////////////////////////////////////////////////////

		//��������֮ǰ���������, ������ڴ�������
		if(applyPlayer)
		{
				//������֮ǰ���е������¼
				applyPlayer->getConsortManager().clearApply(applyUid);
				//������ҹ��������Ϣ
				applyPlayer->getConsortManager().setPlayerConsortInfo(pConsortData->m_nId);

				
		}
		else
		{
			//������ҳ�פ�ڴ���Ϣ
			ConsortInfoData * consortInfoData = LogicSystem::getSingleton().getConsortInfo(applyUid);

			consortInfoData->consortId = pConsortData->m_nId;
		
		
		}

			//�˴�������������ݿ�����ļ�¼

			//�������������Ϣ
		PersistConsortApplyClear applyClear;
		applyClear.m_nPlayerUid = applyUid;
		PersistSystem::getSingleton().postThreadMsg(applyClear, applyUid);


	//��ʱ����
	//�����Ϣ����
	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = applyUid;
	playerConsortSave.m_nConsortId = pConsortData->m_nId;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
	playerConsortSave.m_nLeaveTime = m_pPlayer->mPlayerData.mConsortData.m_nLeaveTime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, applyUid);

	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);

	////////////////////////////////////////////////////
		//����׼����ҹ������ݳ�ʼΪ���µ�
			

		gameActiveInit(applyPlayer,  applyUid);


		////////////////////////////////////////////////////////////////////////


		//���������ߣ���ô���ͼ���֪ͨ
	
		if(applyPlayer )
		{
			const ConnId& applyconnId = applyPlayer->getConnId();	
			if(applyconnId)
			{
					//���ͻذ����ͻ���
	
				Json::Value applyroot;
				applyroot["errorId"] =  LynxErrno::None;
				applyroot["consortid"] = pConsortData->m_nId;
				applyroot["consortname"] = pConsortData->m_strName.c_str();
				applyroot["consortlv"] = pConsortData->m_nLv;
				applyroot["count"] = pConsortData->m_nCount;
	
				ConsortRankData * consortRankData = ConsortSystem::getSingleton().getConsortRankData(pConsortData->m_nId);
				
				if(consortRankData)
				{
						applyroot["rank"] = consortRankData->m_nRank;
						applyroot["power"] = consortRankData->m_nPower;
				}
				else
				{
					UInt32 rank = ConsortSystem::getSingleton().getConsortLastRank();
					applyroot["rank"] = rank;
					applyroot["power"] = pConsortData->m_nPower;
				}



				
				applyroot["check"] = pConsortData->m_nCheck;
				applyroot["condition"] = pConsortData->m_nPowerLimit;
				applyroot["curcontribut"] = 0;
				applyroot["totalcontribut"] = 0;
				applyroot["desc"] = pConsortData->m_strDesc.c_str();
				applyroot["consortres"] = pConsortData->m_nRes;
				applyroot["consortjob"] = 0;
				applyroot["leader"] = pConsortData->m_nLeader;

				BaseInfoData leaderbaseInfo;
				LogicSystem::getSingleton().getBaseInfo(pConsortData->m_nLeader, leaderbaseInfo);
		
				applyroot["leadermodel"] = leaderbaseInfo.modelId;
				applyroot["leadername"] = leaderbaseInfo.name.c_str();
				applyroot["manutype"] = 3;
				applyroot["leavetime"] = applyPlayer->getConsortManager().getPlayerConsortData()->m_nLeaveTime;

				Json::StyledWriter writer;
				std::string jsonStr = writer.write(applyroot);
 
				LOG_INFO("Initial RankSystem success!!!!");
				cout << jsonStr << endl;

				GCConsortQuickJoinResp quickJoinResp;
				quickJoinResp.mPacketID = BOC_CONSORTQUICKJOIN_RESP;
				quickJoinResp.mRespJsonStr = jsonStr;

				NetworkSystem::getSingleton().sendMsg(quickJoinResp,applyconnId);
		
			}
			 
		
		}
	

		//�������ͨ���ذ����᳤�򸱻᳤
		

		GCApplyDealResp dealResp;
				dealResp.mPacketID = BOC_APPLYDEAL_RESP;

				Json::Value root;
				root["errorId"] = LynxErrno::None;
				root["applyuid"] = applyUid;
					root["accept"] = accept;
						root["count"]=pConsortData->m_nCount;
				Json::StyledWriter writer;
				std::string jsonStr = writer.write(root);
 
				cout << jsonStr << endl;
		
				dealResp.mRespJsonStr = jsonStr;
				NetworkSystem::getSingleton().sendMsg(dealResp,connId);

				//��ӹ�����־
				
				addConsortLog(pConsortData, CNLMEMBERJOIN, pConsortData->m_nId, applyUid,
					playerName.c_str(), baseInfo.name.c_str(), "", time(0));

				//���֪ͨ
				notifyOtherOnlineConcortMember(APPLYACCEPT, 0,  pConsortData,  0, applyUid );

}

void ConsortManager::eraseApply(UInt64 consortId)
{
	List<UInt64>::Iter * findIter = m_pPlayerConsortData->m_nApplyList.find(consortId, m_pPlayerConsortData->m_nApplyList.begin(),
		m_pPlayerConsortData->m_nApplyList.end());
	
	if(findIter)
	{
		m_pPlayerConsortData->m_nApplyList.erase(findIter);
	}
	
}

void ConsortManager::clearApply(UInt64 playerUid)
{
	
	for(List<UInt64>::Iter * consortIter = m_pPlayerConsortData->m_nApplyList.begin(); consortIter != m_pPlayerConsortData->m_nApplyList.end(); 
		consortIter = m_pPlayerConsortData->m_nApplyList.next(consortIter))
	{
		ConsortSystem::getSingleton().eraseApplyFromCheckList(consortIter->mValue, playerUid);
	}


	m_pPlayerConsortData->m_nApplyList.clear();
}

void ConsortManager::setPlayerConsortInfo(UInt64 consortId, UInt32 consortJob)
{
		//��������������������Ϊ0
	m_pPlayer->mPlayerData.mConsortData.m_nCurContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nTotalContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nConsortJob = consortJob;
	//���ŵ�id����Ϊ�þ���
	m_pPlayer->mPlayerData.mConsortData.m_nConsortId = consortId;

	//������ҳ�פ�ڴ���Ϣ
	LogicSystem::getSingleton().updateConsortInfo(m_pPlayer->getPlayerGuid(), m_pPlayer->mPlayerData.mConsortData);
}

void ConsortManager::clearPlayerConsortInfo()
{
	m_pPlayer->mPlayerData.mConsortData.m_nCurContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nTotalContribute = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nConsortJob = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nConsortId = 0;
	m_pPlayer->mPlayerData.mConsortData.m_nApplyList.clear();
	m_pPlayer->mPlayerData.mConsortData.m_nLeaveTime = time(0);
	//������ҳ�פ�ڴ���Ϣ
	LogicSystem::getSingleton().updateConsortInfo(m_pPlayer->getPlayerGuid(), m_pPlayer->mPlayerData.mConsortData);
}


void ConsortManager::dealApplyClear(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	


	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCApplyClearResp clearResp;
			clearResp.mPacketID = BOC_APPLYCLEAR_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			clearResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(clearResp,connId);
	}

	//�Ƴ��ڴ�����ҶԸù���������б�
	for(List<ApplyData>::Iter * applyIter = pConsortData->m_applyList.begin(); 
		applyIter != pConsortData->m_applyList.end();   applyIter = pConsortData->m_applyList.next(applyIter))
	{
		Player*  applyPlayer = LogicSystem::getSingleton().getPlayerByGuid(applyIter->mValue.m_nApply);
		if(applyPlayer)
		{
			applyPlayer->getConsortManager().eraseApply(consortId);
		}

	}

	//��չ������е�����
	pConsortData->m_applyList.clear();
		
	//���ݿ������������
	
		PersistConsortApplyAllClear applyClear;
		applyClear.m_nConsortId = consortId;
		PersistSystem::getSingleton().postThreadMsg(applyClear, playerUid);

		//������Ϣ���ͻ���

		GCApplyClearResp clearResp;
		clearResp.mPacketID = BOC_APPLYCLEAR_RESP;

		Json::Value root;
		root["errorId"] = LynxErrno::None;
	
		Json::StyledWriter writer;
		std::string jsonStr = writer.write(root);
 
		cout << jsonStr << endl;
		
		clearResp.mRespJsonStr = jsonStr;
		NetworkSystem::getSingleton().sendMsg(clearResp,connId);


		PlayerConsortData *   playerconsortData = m_pPlayer->getConsortManager().getPlayerConsortData();
		std::string jobstr;
			if(playerconsortData->m_nConsortJob == 1)
			{
				jobstr = "1";
			}
			else
			{
				jobstr = "2";
			}

	std::string playerName = m_pPlayer->getPlayerName();
	
		addConsortLog(pConsortData, CNLCLEARAPPLY, pConsortData->m_nId, playerUid,
			playerName.c_str(),  jobstr.c_str(), pConsortData->m_strName.c_str(),  time(0));

	//���֪ͨ
	notifyOtherOnlineConcortMember(APPLYCLEAR, 0,  pConsortData,  1 );

}

void ConsortManager::setConsortInfo(UInt64 consortId, UInt32 needCheck, UInt64 powerlimit)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	
	
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCConsortSetResp setResp;
			setResp.mPacketID = BOC_CONSORTSET_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);

			return ;
	}

	PlayerConsortData *   playerconsortData = m_pPlayer->getConsortManager().getPlayerConsortData();
		std::string jobstr;
			if(playerconsortData->m_nConsortJob == 1)
			{
				jobstr = "1";
			}
			else
			{
				jobstr = "2";
			}

	std::string playerName = m_pPlayer->getPlayerName();


	bool needNotifyCheck = false;
	bool needNotifyPowerlimit = false;
	if(pConsortData->m_nCheck != needCheck)
	{
		needNotifyCheck = true;
	}

		if(pConsortData->m_nPowerLimit != powerlimit)
		{
			needNotifyPowerlimit = true;
		}



	pConsortData->m_nCheck = needCheck;
	pConsortData->m_nPowerLimit = powerlimit;


	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);


			GCConsortSetResp setResp;
			setResp.mPacketID = BOC_CONSORTSET_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);

	if(needNotifyCheck)
	{
		std::string checkStr = "";
		if(needCheck)
		{
			checkStr = "1";
		}
		else
		{
			checkStr = "0";
		}

		addConsortLog(pConsortData, CNLCHANGECHECK, pConsortData->m_nId, playerUid,
		playerName.c_str(),  jobstr.c_str(), checkStr.c_str(), time(0));

		//���֪ͨ
		notifyOtherOnlineConcortMember(CHANGECHECKNOTIFY, 0,  pConsortData,  0 );
		
	}

	if(needNotifyPowerlimit)
	{
		addConsortLog(pConsortData, CNLCHANGEPOWERLIMIT, pConsortData->m_nId, playerUid,
			playerName.c_str(),  jobstr.c_str(), pConsortData->m_strName.c_str(),  time(0));

		//���֪ͨ
		notifyOtherOnlineConcortMember(CHANGEPOWERLIMIT, 0,  pConsortData,  0 );

	}

}

void ConsortManager::setDescs(UInt64 consortId, std::string descs)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	

	if(!m_pPlayerConsortData->m_nConsortJob)
	{
		GCSetDescsResp setResp;
			setResp.mPacketID = BOC_SETDESCS_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::ConsortJobNotMatch;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);
			return ;
		
	}


	
	
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCSetDescsResp setResp;
			setResp.mPacketID = BOC_SETDESCS_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);
	}


	pConsortData->m_strDesc = descs.c_str();


	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);


			PlayerConsortData *   playerconsortData = m_pPlayer->getConsortManager().getPlayerConsortData();
			std::string jobstr;
			if(playerconsortData->m_nConsortJob == 1)
			{
				jobstr = "1";
			}
			else
			{
				jobstr = "2";
			}

	std::string playerName = m_pPlayer->getPlayerName();
	addConsortLog(pConsortData, CNLCHANGEDESCS, pConsortData->m_nId, playerUid,
		playerName.c_str(),  jobstr.c_str(), playerName.c_str(), time(0));

			GCSetDescsResp setResp;
			setResp.mPacketID = BOC_SETDESCS_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
	
			for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
			logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
			{
			 
				Json::Value logroot;
				logroot["tempid"] = logIter->mValue.m_nTempId;
				logroot["consortId"] = logIter->mValue.m_nConsortId;
				logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
				logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
				logroot["param1"] = logIter->mValue.m_strParam1.c_str();
				logroot["param2"] = logIter->mValue.m_strParam2.c_str();
				logroot["logtime"]= logIter->mValue.m_nTime;

				root["loglist"].append(logroot);
			}

			UInt64 checktime = time(0);
			root["checktime"] = checktime;

			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);

		

	//���֪ͨ
		notifyOtherOnlineConcortMember(CHANGEDESC, playerUid,  pConsortData );
	
}

void ConsortManager::kickMember(UInt64 consortId, UInt64 kickId)
{

	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	
	if(!m_pPlayerConsortData->m_nConsortJob)
	{
		GCKickMemberResp kickResp;
			kickResp.mPacketID = BOC_KICKMEMBER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::ConsortJobNotMatch;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			kickResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickResp,connId);

			return ;
		
	}

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCKickMemberResp kickResp;
			kickResp.mPacketID = BOC_KICKMEMBER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			kickResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickResp,connId);

			return ;
	}

	if(pConsortData->m_nLeader == kickId)
	{
			GCKickMemberResp kickResp;
			kickResp.mPacketID = BOC_KICKMEMBER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::LeaderCannotKicked;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			kickResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickResp,connId);
		return ;
	}


	List<UInt64>::Iter * findIter = pConsortData->m_nMemberList.find(kickId, pConsortData->m_nMemberList.begin(),
		pConsortData->m_nMemberList.end());

	if(findIter == pConsortData->m_nMemberList.end())
	{
		GCKickMemberResp kickResp;
			kickResp.mPacketID = BOC_KICKMEMBER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::MemberNotExist;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			kickResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickResp,connId);

		return;
	}


	//��������Ա����
	UInt64 playerPower = 0;
	Player*  kickPlayer = LogicSystem::getSingleton().getPlayerByGuid(kickId);
	std::string kickPlayerName = "";
	if(kickPlayer)
	{
		playerPower = kickPlayer->getPlayerPower();
		kickPlayerName = kickPlayer->getPlayerName();
	}
	else
	{
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(kickId,baseInfo);
		
		playerPower = baseInfo.power;
			kickPlayerName = baseInfo.name.c_str();
	}

		//��������Ա����
	pConsortData->m_nMemberList.erase(findIter);
	pConsortData->m_nCount --;
	//����ս���۳�
	pConsortData->m_nPower -= playerPower;

	//�������

	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);


	//������ڴ�����ô�޸��ڴ���Ϣ
	if(kickPlayer)
	{
		//��ҵľ����������
		kickPlayer->getConsortManager().clearPlayerConsortInfo();

		const ConnId& kickConnId = kickPlayer->getConnId();
		//���������ߣ���ô���ͱ��߳��ľ���֪ͨ
		if(kickConnId)
		{
			Json::Value kickRoot;
		
			kickRoot["curcontribut"] = 0;
			kickRoot["totalcontribut"] = 0;
			kickRoot["consortjob"] = 0;
			kickRoot["consortid"] = 0;
			kickRoot["errorId"] = 0;
			kickRoot["leavetime"] = kickPlayer->mPlayerData.mConsortData.m_nLeaveTime;
			GCIsKickedNotify kickedNotify;
			kickedNotify.mPacketID = BOC_ISKICKED_NOTIFY;
		
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(kickRoot);
 
			cout << jsonStr << endl;
		
			kickedNotify.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickedNotify,kickConnId);

		}	
	}
	else
	{
		//������ҳ�פ�ڴ���Ϣ
		ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(kickId);
		consortInfoData->consortId = 0;
		consortInfoData->consortJob = 0;
		consortInfoData->curContribute = 0;
		consortInfoData->totalContribute = 0;
	}

	//����ľè���������
	ConsortSystem::getSingleton().clearWoodBattle(pConsortData->m_nId, kickId);

	//�������Ʊ�м�����ȡ����
	if(kickPlayer)
	{
		kickPlayer->getConsortManager().clearTicketAward();
	}
		
	 PersistTicketAwardDel awardDel;
	 awardDel.m_nPlayeruid = kickId;
	 PersistSystem::getSingleton().postThreadMsg(awardDel,  kickId);

	//�����Ϣ����

	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = kickId;
	playerConsortSave.m_nConsortId = 0;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
	playerConsortSave.m_nLeaveTime = time(0);
	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, kickId);

	//���������Ļذ����᳤���߸��᳤
	GCKickMemberResp kickResp;
			kickResp.mPacketID = BOC_KICKMEMBER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["count"] = pConsortData->m_nCount;
			root["power"] = pConsortData->m_nPower;
			root["kickuid"] = kickId;
		
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			kickResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(kickResp,connId);

			std::string playerName = m_pPlayer->getPlayerName();
			addConsortLog(pConsortData, CNLKICKMEMBER, pConsortData->m_nId, playerUid,
				playerName.c_str(),  kickPlayerName.c_str(), pConsortData->m_strName.c_str(),  time(0));

			//���֪ͨ
		notifyOtherOnlineConcortMember(KICKMEMBERNOTIFY, 0,  pConsortData );
			

}

void ConsortManager::destroyConsort(UInt64 consortId)
{
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	
	
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCDestroyConsortResp destroyResp;
			destroyResp.mPacketID = BOC_DESTROYCONSORT_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			destroyResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(destroyResp,connId);

			return ;
	}

	if(pConsortData->m_nLeader != playerUid  || pConsortData->m_nMemberList.size() > 1)
	{
			GCDestroyConsortResp destroyResp;
			destroyResp.mPacketID = BOC_DESTROYCONSORT_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::InvalidPermission;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			destroyResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(destroyResp,connId);

			return ;
	}

	//�����Ҿ�����Ϣ
	clearPlayerConsortInfo();
		
	m_pPlayer->getConsortManager().clearTicketAward();
	
	PersistTicketAwardDel awardDel;
	awardDel.m_nPlayeruid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(awardDel,  playerUid);


	//ɾ������
	ConsortSystem::getSingleton().destroyConsort(consortId);
		
	
	//�����Ϣ����
	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = playerUid;
	playerConsortSave.m_nConsortId = 0;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
	playerConsortSave.m_nLeaveTime = time(0);
	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerUid);

	//����ɾ�������߳���Ϣ

	PersistConsortDel consortDel;
	consortDel.m_nConsortId = consortId;
	PersistSystem::getSingleton().postThreadMsg(consortDel, playerUid);

	GCDestroyConsortResp destroyResp;
	destroyResp.mPacketID = BOC_DESTROYCONSORT_RESP;

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["curcontribut"] = 0;
	root["totalcontribut"] = 0;
	root["consortjob"] = 0;
	root["consortid"] = 0;
	root["errorId"] = 0;
	root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;


	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
 
	cout << jsonStr << endl;
		
	destroyResp.mRespJsonStr = jsonStr;
	NetworkSystem::getSingleton().sendMsg(destroyResp,connId);

}

void ConsortManager::leaveConsort(UInt64 consortId)
{
	
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();	
	
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
			GCLeaveConsortResp leaveResp;
			leaveResp.mPacketID = BOC_LEAVECONSORT_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::PlayerConsortNotFind;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			leaveResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(leaveResp,connId);

			return ;
	}


	if(pConsortData->m_nLeader == playerUid )
	{
			GCLeaveConsortResp leaveResp;
			leaveResp.mPacketID = BOC_LEAVECONSORT_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::LeaderCannotLeave;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			leaveResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(leaveResp,connId);

		return ;
	}


	List<UInt64>::Iter * findIter = pConsortData->m_nMemberList.find(playerUid, pConsortData->m_nMemberList.begin(),
		pConsortData->m_nMemberList.end());

	if(findIter == pConsortData->m_nMemberList.end())
	{
			GCLeaveConsortResp leaveResp;
			leaveResp.mPacketID = BOC_LEAVECONSORT_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::MemberNotExist;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 
			cout << jsonStr << endl;
		
			leaveResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(leaveResp,connId);

		return;
	}


	UInt64 playerPower = m_pPlayer->getPlayerPower();

		//��������Ա����
	

		//��������Ա����
	pConsortData->m_nMemberList.erase(findIter);
	pConsortData->m_nCount --;
	//����ս���۳�
	pConsortData->m_nPower -= playerPower;

	//�������

	//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);

	//����ľè���������
	ConsortSystem::getSingleton().clearWoodBattle(pConsortData->m_nId, playerUid);
	
	//������ڴ�����ô�޸��ڴ���Ϣ
	clearPlayerConsortInfo();

	m_pPlayer->getConsortManager().clearTicketAward();

	PersistTicketAwardDel awardDel;
	awardDel.m_nPlayeruid = playerUid;
	PersistSystem::getSingleton().postThreadMsg(awardDel,  playerUid);



	//�����Ϣ����

	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = playerUid;
	playerConsortSave.m_nConsortId = 0;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 0;
	playerConsortSave.m_nLeaveTime =time(0);
	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerUid);


	GCLeaveConsortResp leaveResp;
	leaveResp.mPacketID = BOC_LEAVECONSORT_RESP;

	Json::Value root;
	root["errorId"] = LynxErrno::None;

	root["curcontribut"] = 0;
	root["totalcontribut"] = 0;
	root["consortjob"] = 0;
	root["consortid"] = 0;
	root["leavetime"] = m_pPlayerConsortData->m_nLeaveTime;
	
	Json::StyledWriter writer;
	std::string jsonStr = writer.write(root);
 
	cout << jsonStr << endl;
		
	leaveResp.mRespJsonStr = jsonStr;
	NetworkSystem::getSingleton().sendMsg(leaveResp,connId);

	std::string playerName = m_pPlayer->getPlayerName();
	addConsortLog(pConsortData, CNLMEMBERLEAVE, pConsortData->m_nId, playerUid,
				playerName.c_str(),  playerName.c_str(), "", time(0));

	
			//���֪ͨ
		notifyOtherOnlineConcortMember(LEAVECONSORT, 0,  pConsortData );

}

void ConsortManager::updateConsortPower(UInt64 oldPower, UInt64 newPower)
{
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	if(!consortId)
	{
		return ;
	}

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	if(!pConsortData)
	{
		return ;
	}

	pConsortData->m_nPower = (pConsortData->m_nPower -oldPower + newPower);

		//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerUid);
}


void ConsortManager::setViceLeader(UInt64 otherUid, UInt32 setUp)
{
	
	const ConnId& connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	std::string playerName = m_pPlayer->getPlayerName();

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

	List<UInt64>::Iter * findIter = pConsortData->m_nMemberList.find(otherUid, pConsortData->m_nMemberList.begin(),
		pConsortData->m_nMemberList.end());

	if(findIter == pConsortData->m_nMemberList.end())
	{
			GCSetViceLeaderResp setResp;
			setResp.mPacketID = BOC_SETVICELEADER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::MemberNotExist;
	
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
			LOG_INFO("set vice leader resp is: %s", jsonStr.c_str());
			//cout << jsonStr << endl;
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);

		return;
	}

	Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherUid);
	PlayerConsortData * otherConsortData = NULL;

	UInt32 consortJob = 0, curcontribute = 0, totalcontribute = 0;
	UInt64 consortid = 0;
	UInt64 checktime = time(0);
		UInt64 leavetime = 0;
	//������ڴ��У���ô����setup�����޸�
	if(otherPlayer)
	{
		otherConsortData = otherPlayer->getConsortManager().getPlayerConsortData();
		if(setUp)
		{
			otherConsortData->m_nConsortJob = 2;
		}
		else
		{
			otherConsortData->m_nConsortJob = 0;
		}

		consortJob = otherConsortData->m_nConsortJob;
		curcontribute = otherConsortData->m_nCurContribute;
		totalcontribute = otherConsortData->m_nTotalContribute;
		consortid = otherConsortData->m_nConsortId;
		leavetime = otherConsortData->m_nLeaveTime;
		//������ҳ�פ�ڴ���Ϣ
		LogicSystem::getSingleton().updateConsortInfo(otherUid,*otherConsortData);

		//�����־
		if(setUp)
		{
				BaseInfoData otherInfo;
				LogicSystem::getSingleton().getBaseInfo(otherUid, otherInfo);
					
				addConsortLog(pConsortData, CNLSETUPVICELEADER, pConsortData->m_nId, playerUid,
				playerName.c_str(), otherInfo.name.c_str(), "", time(0));
		}
		else
		{
				BaseInfoData otherInfo;
				LogicSystem::getSingleton().getBaseInfo(otherUid, otherInfo);
					
				addConsortLog(pConsortData, CNLSETDOWNMEMBER, pConsortData->m_nId, playerUid,
				playerName.c_str(), otherInfo.name.c_str(), "", time(0));
		
		}
		
	
	
		//�ж϶Է��Ƿ����ߣ�����������Ϣ
		const ConnId & otherCon = otherPlayer->getConnId();
	
		if(otherCon)
		{
			///////////////////////////////////////////////////////////

				//����֪ͨ���̣��˴���ע��
			//	Json::StyledWriter notifyWriter;
			//	Json::Value notifyRoot;
			//	notifyRoot["consortJob"] = otherConsortData->m_nConsortJob;

			//	for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
			//			memberIter != pConsortData->m_nMemberList.end();  
			//				memberIter = pConsortData->m_nMemberList.next(memberIter))
			//	{
			//			Json::Value memberRoot;
			//			memberRoot["playeruid"]= memberIter->mValue;
			//			BaseInfoData baseInfo;
			//			bool flag = LogicSystem::getSingleton().getBaseInfo(memberIter->mValue,baseInfo);
			//			ConsortInfoData *  consortInfo = LogicSystem::getSingleton().getConsortInfo(memberIter->mValue);
			//			if(!flag  || !consortInfo)
			//			{
			//				continue;
			//			}

			//			memberRoot["playername"] = baseInfo.name.c_str();
			//			memberRoot["playerlv"] = baseInfo.level;
			//			memberRoot["viplv"] = baseInfo.vipLv;
			//			memberRoot["model"] = baseInfo.modelId;
			//			memberRoot["leave"] = baseInfo.leaveTime;
			//			memberRoot["power"] = baseInfo.power;
			//			memberRoot["curcontribut"] = consortInfo->curContribute;
			//			memberRoot["totalcontribut"] = consortInfo->totalContribute;
			//			memberRoot["job"] = consortInfo->consortJob;
   // 
			//			notifyRoot["member"].append(memberRoot);
   //     

			//	}

			//	//��־�б�
			//		for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
			//			logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
			//	{
			//			
			//				Json::Value logroot;
			//				logroot["tempid"] = logIter->mValue.m_nTempId;
			//				logroot["consortId"] = logIter->mValue.m_nConsortId;
			//				logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
			//				logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
			//				logroot["param1"] = logIter->mValue.m_strParam1.c_str();
			//				logroot["param2"] = logIter->mValue.m_strParam2.c_str();
			//				logroot["logtime"]= logIter->mValue.m_nTime;

			//				notifyRoot["loglist"].append(logroot);
			//	}
			//		
			//		notifyRoot["checktime"] = checktime;
			////////////////////////////////////////////////////////////////////

			//		GCConsortJobNotify jobnotify;
			//		jobnotify.mPacketID = BOC_CONSORTJOBCHANGE;
			//		jobnotify.mRespJsonStr = notifyWriter.write(notifyRoot);
			//		NetworkSystem::getSingleton().sendMsg(jobnotify,otherCon);
			//		LOG_INFO("notify job change is : %s", jobnotify.mRespJsonStr.c_str());
		}
	
	}
	else
	{
		//������ҳ�פ�ڴ���Ϣ
		ConsortInfoData  *  consortInfoData = LogicSystem::getSingleton().getConsortInfo(otherUid);
		leavetime = consortInfoData->leavetime;
		if(setUp)
		{
			consortInfoData->consortJob = 2;
		}
		else
		{
			consortInfoData->consortJob = 0;
		}
		 
		consortJob = consortInfoData->consortJob;
		curcontribute = consortInfoData->curContribute;
		totalcontribute = consortInfoData->totalContribute;
		consortid = consortInfoData->consortId;

		//�����־
		if(setUp)
		{
				BaseInfoData otherInfo;
				LogicSystem::getSingleton().getBaseInfo(otherUid, otherInfo);
					
				addConsortLog(pConsortData, CNLSETUPVICELEADER, pConsortData->m_nId, playerUid,
				playerName.c_str(), otherInfo.name.c_str(), "", time(0));
		}
		else
		{
				BaseInfoData otherInfo;
				LogicSystem::getSingleton().getBaseInfo(otherUid, otherInfo);
					
				addConsortLog(pConsortData, CNLSETDOWNMEMBER, pConsortData->m_nId, playerUid,
				playerName.c_str(), otherInfo.name.c_str(), "", time(0));
		
		}
		
	

	}


	//�����Ϣ����

	PersistPlayerConsortSave playerConsortSave;
	playerConsortSave.m_nPlayerUid = otherUid;
	playerConsortSave.m_nConsortId = consortid;
	playerConsortSave.m_nCurContribute = curcontribute;
	playerConsortSave.m_nTotalContribute = totalcontribute;   
	playerConsortSave.m_nConsortJob = consortJob;
	playerConsortSave.m_nLeaveTime = leavetime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerUid);

	//���ͻ��˻ذ�
	GCSetViceLeaderResp setResp;
			setResp.mPacketID = BOC_SETVICELEADER_RESP;

			Json::Value root;
			root["errorId"] = LynxErrno::None;
			root["playeruid"] = otherUid;
			root["setup"] = setUp;
			
			for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
						logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
			{
						
							Json::Value logroot;
							logroot["tempid"] = logIter->mValue.m_nTempId;
							logroot["consortId"] = logIter->mValue.m_nConsortId;
							logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
							logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
							logroot["param1"] = logIter->mValue.m_strParam1.c_str();
							logroot["param2"] = logIter->mValue.m_strParam2.c_str();
							logroot["logtime"]= logIter->mValue.m_nTime;

							root["loglist"].append(logroot);
			}

			
			root["checktime"] = checktime;
			Json::StyledWriter writer;
			std::string jsonStr = writer.write(root);
 

			LOG_INFO("set vice leader resp is: %s", jsonStr.c_str());
		
			setResp.mRespJsonStr = jsonStr;
			NetworkSystem::getSingleton().sendMsg(setResp,connId);

				//���֪ͨ
		notifyOtherOnlineConcortMember(SETVICELEADER, 0,  pConsortData );

}


void ConsortManager::getWoodCatData()
{

}


PlayerConsortData * ConsortManager::getPlayerConsortData()
{
	return m_pPlayerConsortData;
}

void ConsortManager::addConsortLog(ConsortData * pConsortData, UInt32 tempId, UInt64 consortId, UInt64 playerUid, std::string  playerName,  std::string param1,
			std::string param2, UInt64 logTime)
{
		assert(pConsortData->m_logList.size() <= 50);
		
		LogData logData;
		logData.m_nTempId = tempId;
		logData.m_nConsortId = consortId;
		logData.m_nPlayerUid = playerUid;
		logData.m_strPlayername = playerName.c_str();
		logData.m_strParam1 = param1.c_str();
		logData.m_strParam2 = param2.c_str();
		logData.m_nTime = logTime;

		pConsortData->m_logList.insertHead(logData);
		
		if(pConsortData->m_logList.size()>=51)
		{
			pConsortData->m_logList.erase(pConsortData->m_logList.last());
		}
		
		//�µ���־��Ϣ����
		
		PersistConsortLogInsert playerConsortLogInsert;
		playerConsortLogInsert.m_nTempId = tempId;
		playerConsortLogInsert.m_nConsortId = consortId;
		playerConsortLogInsert.m_nPlayerUid = playerUid;
		playerConsortLogInsert.m_strPlayerName = playerName.c_str();
		playerConsortLogInsert.m_strParam1 = param1.c_str();
		playerConsortLogInsert.m_strParam2 = param2.c_str();
		playerConsortLogInsert.m_nTime = logTime;
		
		

		PersistSystem::getSingleton().postThreadMsg(playerConsortLogInsert, playerUid);

		//������Ϣ��Ϸ��Ƶ��
		ConsortMsgTemplate * consortMsgTemp = CONSORTMSG_TABLE().get(tempId);
		if(consortMsgTemp->mVisible)
		{
			
			ConsortSystem::getSingleton().sendConsortMsg(tempId, consortId, playerUid, playerName, param1, param2, logTime);
		}

}


void ConsortManager::logListSync(UInt64 timelast)
{
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(m_pPlayerConsortData->m_nConsortId);
		const ConnId& connId = m_pPlayer->getConnId();

		if(!pConsortData)
		{
				Json::Value root;
				root["errorId"] = LynxErrno::InvalidParameter;
	
				Json::StyledWriter writer;
				std::string jsonStr = writer.write(root);
				LOG_INFO("loglistsync is:  %s",  jsonStr.c_str());

				GCConsortlogResp setResp;
				setResp.mPacketID = BOC_CONSORTLOG_RESP;
				setResp.mRespJsonStr = jsonStr;
				NetworkSystem::getSingleton().sendMsg(setResp,connId);

				return ;
				
		}

		Json::Value root;
		for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
		logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
		{
			   if(timelast <=  logIter->mValue.m_nTime)
			   {
				  break;
			   }
				Json::Value logroot;
				logroot["tempid"] = logIter->mValue.m_nTempId;
				logroot["consortId"] = logIter->mValue.m_nConsortId;
				logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
				logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
				logroot["param1"] = logIter->mValue.m_strParam1.c_str();
				logroot["param2"] = logIter->mValue.m_strParam2.c_str();
				logroot["logtime"]= logIter->mValue.m_nTime;

				root["loglist"].append(logroot);
		}

		 UInt64 checktime = time(0);
		root["checktime"] = checktime;
		root["errorId"] = LynxErrno::None;
		Json::StyledWriter writer;
		std::string jsonStr = writer.write(root);
		LOG_INFO("loglistsync is:  %s",  jsonStr.c_str());

		GCConsortlogResp setResp;
		setResp.mPacketID = BOC_CONSORTLOG_RESP;
	    setResp.mRespJsonStr = jsonStr;
		NetworkSystem::getSingleton().sendMsg(setResp,connId);

}

void ConsortManager::consortSign(UInt32 signType)
{
	const ConnId & connId = m_pPlayer->getConnId();
	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	std::string  playerName = m_pPlayer->getPlayerName();

	ConsortSignTemplate * consortSignTemp = CONSORTSIGN_TABLE().get(signType);

	if(!consortSignTemp)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		LOG_WARN("design table error  !");
		return ;
	}

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(m_pPlayerConsortData->m_nConsortId);
	if(!pConsortData)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		assert(false);
		return;
	}

	//����ȼ���Ӧ��Ԫ��
	ConsortLvTemplate * signlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nSignLv);

	//����ȼ���Ӧ��Ԫ��
	ConsortLvTemplate * consortlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);

	if(!signlvTemp)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		assert(false);
		return;
	}


	if(!consortlvTemp)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		assert(false);
		return;
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	if(pConsortData->m_nLv < globalValue.uGuildOpenSign)
	{
		LOG_INFO("guild level is not match!!!");

		Json::Value root;
			root["errorId"] = LynxErrno::ConsortLvNotEnough;
	
			Json::StyledWriter writer;
			GCConsortSignResp signResp;

			signResp.mRespJsonStr = writer.write(root);
			signResp.mPacketID = BOC_CONSORTSIGN_RESP;

			NetworkSystem::getSingleton().sendMsg(signResp,connId);


		return ;
	}


	UInt64 nowtime = time(0);
	if(nowtime - m_pPlayerConsortData->m_nLeaveTime < 24 * 60 *60)
	{
		Json::Value root;
			root["errorId"] = LynxErrno::JustLeaveConsort;
	
			Json::StyledWriter writer;
			GCConsortSignResp signResp;

			signResp.mRespJsonStr = writer.write(root);
			signResp.mPacketID = BOC_CONSORTSIGN_RESP;

			NetworkSystem::getSingleton().sendMsg(signResp,connId);
		return ;
	}

	std::string costStr = consortSignTemp->mCost.c_str();
	std::string ::size_type  findIndex = costStr.find(';');

	std::stringstream mystream; 
	std::string typeStr = "", subtypeStr= "", countStr = "";

	if(findIndex == std::string::npos)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		LOG_WARN("design table error  !");
		return ;
	}

	typeStr = costStr.substr(0,findIndex);
	costStr = costStr.substr(findIndex + 1);
	
	findIndex = costStr.find(';');

	if(findIndex == std::string::npos)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		GCConsortSignResp signResp;

		signResp.mRespJsonStr = writer.write(root);
		signResp.mPacketID = BOC_CONSORTSIGN_RESP;

		NetworkSystem::getSingleton().sendMsg(signResp,connId);
		LOG_WARN("design table error  !");
		return ;
	}

	subtypeStr = costStr.substr(0,findIndex);
	countStr = costStr.substr(findIndex + 1);
	mystream.clear();
	mystream.str("");
	mystream << typeStr;
	
	UInt64 typeId = 0, subtypeId = 0, countId = 0;
	mystream >>  typeId ;

	mystream.clear();
	mystream.str("");
	
	mystream << subtypeStr;
	mystream >> subtypeId;

	mystream.clear();
	mystream.str("");
	
	mystream << countStr;
	mystream >> countId;

	
	//���ж�Ǯ�Ƿ�
  
	bool match = m_pPlayer->getAllItemManager().costMaterial(typeId, subtypeId, countId,MiniLog76);
	if(!match)
	{
		Json::Value root;
			root["errorId"] = LynxErrno::CostNotEnouf;
	
			Json::StyledWriter writer;
			GCConsortSignResp signResp;

			signResp.mRespJsonStr = writer.write(root);
			signResp.mPacketID = BOC_CONSORTSIGN_RESP;

			NetworkSystem::getSingleton().sendMsg(signResp,connId);
		return;
	}

	//��Һ�ͭǮ�Ȼ�����Ϣ����
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	//���Ӹ��˹���
	m_pPlayerConsortData->m_nCurContribute += consortSignTemp->mHonor;
	m_pPlayerConsortData->m_nTotalContribute += consortSignTemp->mHonor;
	m_pPlayerConsortData->m_nSign = signType;

	UInt64 playerGuid = m_pPlayer->getPlayerGuid();
		//������ҳ�פ�ڴ���Ϣ
	LogicSystem::getSingleton().updateConsortInfo(playerGuid, m_pPlayer->mPlayerData.mConsortData);

	//ǩ����Ϣ����
	PersistConsortSignUpdate consortSignUpdate;
	consortSignUpdate.m_nPlayerUid = playerGuid;
	consortSignUpdate.m_nSignAwards = m_pPlayerConsortData->m_nSignAwards;
	consortSignUpdate.m_nSign = m_pPlayerConsortData->m_nSign;

	PersistSystem::getSingleton().postThreadMsg(consortSignUpdate, playerGuid);
	
	//����������ݿ����
	PersistPlayerConsortSave  playerConsortSave;
	playerConsortSave.m_nPlayerUid = playerGuid;
	playerConsortSave.m_nConsortId =  m_pPlayerConsortData->m_nConsortId;
	playerConsortSave.m_nCurContribute = m_pPlayerConsortData->m_nCurContribute;
	playerConsortSave.m_nTotalContribute = m_pPlayerConsortData->m_nTotalContribute;
	playerConsortSave.m_nConsortJob = m_pPlayerConsortData->m_nConsortJob;
	playerConsortSave.m_nLeaveTime = m_pPlayerConsortData->m_nLeaveTime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerConsortSave.m_nPlayerUid);

	//���ӹ����Ծ��

	
	pConsortData->m_nActive += consortSignTemp->mActivity;
	//�жϹ����Ծ�ȴ��ڵ���ȼ���Ӧ�Ļ�Ծ�����ߣ������Ϊ����ֵ
	if(pConsortData->m_nActive >= signlvTemp->mActive)
	{
			pConsortData->m_nActive = signlvTemp->mActive;
	}

	//�����Ծ�ȴ���
	PersistConsortActiveUpdate consortactiveUpdate;
	consortactiveUpdate.m_nActive = pConsortData->m_nActive;
	consortactiveUpdate.m_nConsortId = m_pPlayerConsortData->m_nConsortId;
	PersistSystem::getSingleton().postThreadMsg(consortactiveUpdate, consortactiveUpdate.m_nConsortId);
	
	UInt32 addExp = 0;
	UInt32 expRand = 0;
	int rate = rand()%6;
	int rateflag = 0;
	if(rate == 1)
	{
		addExp += (consortSignTemp->mConsortExp+ consortSignTemp->mConsortExp * 0.25);
		expRand = consortSignTemp->mConsortExp * 0.25;
		rateflag = 1;
	}
	else
	{
		addExp += consortSignTemp->mConsortExp;
	}
	//���ᾭ�����ӣ��ж��Ƿ�����

	ConsortLvTemplate * nextlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv + 1);
	//�ж�����
	if(!nextlvTemp)
	{
		//���ᾭ������
		pConsortData->m_nExp += addExp;
		//���ᾭ�����˻����������ֵΪ��ߵȼ���������
		if(pConsortData->m_nExp >= consortlvTemp->mExp)
		{
			pConsortData->m_nExp = consortlvTemp->mExp;
		}
		 
	}
	else
	{
		std::stringstream mystream;
	
		//û����������ô�ж������߼�
		//���ᾭ������
		pConsortData->m_nExp += addExp;
		while(pConsortData->m_nExp >= consortlvTemp->mExp)
		{
			//�ж���һ���Ƿ���ڣ�������˵����������ߵȼ�
			nextlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv + 1);
			if(!nextlvTemp)
			{
				//��������¾�������������Ϊ��������
				if(pConsortData->m_nExp >=  consortlvTemp->mExp)
				{
					pConsortData->m_nExp = consortlvTemp->mExp;
					break;
				}
				
			}

			mystream.clear();
			mystream.str("");
			pConsortData->m_nLv ++;
			pConsortData->m_nExp -= consortlvTemp->mExp;
			//��ǰ�ȼ���Ӧ��Ԫ��
			consortlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
			std::string param1;
			mystream << pConsortData->m_nLv;
			mystream >> param1;
			addConsortLog(pConsortData, CNLLVUP, pConsortData->m_nId, playerUid,
				playerName.c_str(), param1, "", time(0));
			
		}


	
	}


	//���������Ϣ����

		//������Ϣ����
	PersistConsortSave consortSave;
	consortSave.m_nConsortId = pConsortData->m_nId;
	consortSave.m_nConsortLv = pConsortData->m_nLv;
    consortSave.m_strConsortName = pConsortData->m_strName;
	consortSave.m_nCount = pConsortData->m_nCount;
	consortSave.m_nPower = pConsortData->m_nPower;
    consortSave.m_strDescs = pConsortData->m_strDesc;
	consortSave.m_nRes = pConsortData->m_nRes;
    consortSave.m_nExp = pConsortData->m_nExp;
	consortSave.m_nCheck = pConsortData->m_nCheck;
	consortSave.m_nPowerLimit = pConsortData->m_nPowerLimit;
		


	PersistSystem::getSingleton().postThreadMsg(consortSave, playerGuid);


	Json::Value notifyRoot;
	notifyRoot["consortid"] = pConsortData->m_nId;
	notifyRoot["consortlv"] = pConsortData->m_nLv;
	notifyRoot["consortexp"] = pConsortData->m_nExp;
	notifyRoot["consortactive"] = pConsortData->m_nActive;
	notifyRoot["signlv"] = pConsortData->m_nSignLv;


for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
		memberIter != pConsortData->m_nMemberList.end();  
		memberIter = pConsortData->m_nMemberList.next(memberIter))
	{
		Json::Value memberRoot;
		memberRoot["playeruid"]= memberIter->mValue;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(memberIter->mValue,baseInfo);
		ConsortInfoData *  consortInfo = LogicSystem::getSingleton().getConsortInfo(memberIter->mValue);
		if(!flag  || !consortInfo)
		{
			continue;
		}

		memberRoot["playername"] = baseInfo.name.c_str();
		memberRoot["playerlv"] = baseInfo.level;
		memberRoot["viplv"] = baseInfo.vipLv;
		memberRoot["model"] = baseInfo.modelId;
		memberRoot["leave"] = baseInfo.leaveTime;
		memberRoot["power"] = baseInfo.power;
		memberRoot["curcontribut"] = consortInfo->curContribute;
		memberRoot["totalcontribut"] = consortInfo->totalContribute;
		memberRoot["job"] = consortInfo->consortJob;
    
		notifyRoot["member"].append(memberRoot);
        

	}



	GCConsortActiveSyncNotify consortActiveNotify;
	Json::StyledWriter writer;
	consortActiveNotify.mRespJsonStr = writer.write(notifyRoot);
	consortActiveNotify.mPacketID = BOC_CONSORTACTIVE_NOTIFY;
	LOG_INFO("notify str is : %s", consortActiveNotify.mRespJsonStr.c_str());
		





	//֪ͨ���������߳�Ա���¹����Ծ��
	for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin();  memberIter != NULL;
		memberIter = pConsortData->m_nMemberList.next(memberIter) )
	{
		Player * member = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
		if(member)
		{
			const ConnId & memCon = member->getConnId();
			if(memCon != connId)
			{
						NetworkSystem::getSingleton().sendMsg(consortActiveNotify,memCon);
			}
		
		}
	}

	//����ǩ�����wwc
	m_pPlayer->getAchieveManager().updateDailyTaskData(DLYCONSORTSIGN, 1 );



	//�������ذ�
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["gold"] = m_pPlayer->getPlayerGold();
	root["coin"] = m_pPlayer->getPlayerCoin();

	root["consortid"] = pConsortData->m_nId;
	root["consortlv"] = pConsortData->m_nLv;
	root["consortexp"] = pConsortData->m_nExp;
	root["consortactive"] = pConsortData->m_nActive;

	root["curcon"] = m_pPlayerConsortData->m_nCurContribute;
	root["totalcon"] = m_pPlayerConsortData->m_nTotalContribute;

	root["signlv"] = pConsortData->m_nSignLv;
	root["critflag"] = rateflag;
	root["sign"] = signType;
	root["expRand"] = expRand;
	
	GCConsortSignResp signResp;

	signResp.mRespJsonStr = writer.write(root);
	signResp.mPacketID = BOC_CONSORTSIGN_RESP;
	LOG_INFO("signresp str is : %s", signResp.mRespJsonStr.c_str());
	NetworkSystem::getSingleton().sendMsg(signResp,connId);
}


void ConsortManager::consortSignAward(UInt32 awardIndex)
{
	const ConnId & connId = m_pPlayer->getConnId();

	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(m_pPlayerConsortData->m_nConsortId);
	if(!pConsortData)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		assert(false);
		return;
	}

	//����ȼ���Ӧ��Ԫ��
	ConsortLvTemplate * signlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nSignLv);

	//����ȼ���Ӧ��Ԫ��
	ConsortLvTemplate * consortlvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);

	if(!signlvTemp)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		assert(false);
		return;
	}


	if(!consortlvTemp)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		assert(false);
		return;
	}

		UInt64 nowtime = time(0);
	if(nowtime - m_pPlayerConsortData->m_nLeaveTime < 24 * 60 *60)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::JustLeaveConsort;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		LOG_INFO("sign award resp is : %s ", signAwardResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		return ;
	}


 	bool isMatch = judgeActiveMatch(awardIndex, pConsortData, signlvTemp);
	if(!isMatch)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::ConsortActiveNotEnough;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		LOG_INFO("sign award resp is : %s ", signAwardResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		assert(false);
		return;
	}

	getSingAward(awardIndex, signlvTemp,MiniLog75);

}

void ConsortManager::initialSignStruct()
{
	m_pPlayerConsortData->m_nSign = 0;
	m_pPlayerConsortData->m_nSignAwards.clear();
	for(UInt32 i = 0; i < 4; i++)
	{
		m_pPlayerConsortData->m_nSignAwards.insertTail(UInt32(0));
	}

	UInt64 playerGuid = m_pPlayer->getPlayerGuid();
	//ǩ����Ϣ����
	PersistConsortSignUpdate consortSignUpdate;
	consortSignUpdate.m_nPlayerUid = playerGuid;
	consortSignUpdate.m_nSignAwards = m_pPlayerConsortData->m_nSignAwards;
	consortSignUpdate.m_nSign = m_pPlayerConsortData->m_nSign;

	PersistSystem::getSingleton().postThreadMsg(consortSignUpdate, playerGuid);


	//����ˢ�´�����ʼ��
	m_pPlayerConsortData->m_nKitchenTimes = 0;

	PersistConsortKitQueUpdate kitQueUpdate;
	kitQueUpdate.m_nPlayerUid = playerGuid;
	kitQueUpdate.m_nRefreshTimes = m_pPlayerConsortData->m_nKitchenTimes;
	PersistSystem::getSingleton().postThreadMsg(kitQueUpdate, playerGuid);

}

bool ConsortManager::judgeActiveMatch(UInt32 awardIndex, ConsortData * pConsortData, ConsortLvTemplate * signlvTemp)
{
	
	switch(awardIndex)
	{
		case 0:
			if(pConsortData->m_nActive <signlvTemp->mActivePoint1)
			{
				return false;
			}
			else
			{
				return true;
			}
				
			break;
		case 1:
			if(pConsortData->m_nActive <signlvTemp->mActivePoint2)
			{
				return false;
			}
			else
			{
				return true;
			}

			break;
		case 2:
			if(pConsortData->m_nActive <signlvTemp->mActivePoint3)
			{
				return false;
			}
			else
			{
				return true;
			}
			break;
		case 3:
			if(pConsortData->m_nActive <signlvTemp->mActivePoint4)
			{
				return false;
			}
			else
			{
				return true;
			}
			break;
		default:
			return false;
			break;
			return false;
	}
}

void  ConsortManager::getSingAward(UInt32 awardIndex,  ConsortLvTemplate * signlvTemp,UInt32 systemID )
{
	const ConnId & connId = m_pPlayer->getConnId();
	//���ó������ӿڣ������õ�awardcontontid
	List<Goods> tmpItemList;
	UInt64 awardId = 0;
	switch(awardIndex)
	{
		case 0:
			awardId = signlvTemp->mActiveAward1;
			break;
		case 1:
			awardId = signlvTemp->mActiveAward2;
			break;
		case 2:
			awardId = signlvTemp->mActiveAward3;
			break;
		case 3:
			awardId = signlvTemp->mActiveAward4;
			break;
		default:
			break;
	}

	if(awardId == 0)
	{
		Json::StyledWriter writer;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidParameter;
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);
		LOG_INFO("sign award resp is : %s ", signAwardResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
		assert(false);
		return ;
	}

		//�����ڴ�����
		List<UInt32>::Iter * awardIter  = m_pPlayerConsortData->m_nSignAwards.getn(awardIndex);
		if(awardIter->mValue)
		{
			Json::StyledWriter writer;
			Json::Value root;
			root["errorId"] = LynxErrno::AwardGetten;
			GCConsortSignAwardResp  signAwardResp;
			signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
			signAwardResp.mRespJsonStr = writer.write(root);
			LOG_INFO("sign award resp is : %s ", signAwardResp.mRespJsonStr.c_str());
			NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
			assert(false);
			return;
		}


		awardIter->mValue = 1;
		
	 //�콱����
		//ǩ����Ϣ����
		UInt64 playerGuid = m_pPlayer->getPlayerGuid();
		PersistConsortSignUpdate consortSignUpdate;
		consortSignUpdate.m_nPlayerUid = playerGuid;
		consortSignUpdate.m_nSignAwards = m_pPlayerConsortData->m_nSignAwards;
		consortSignUpdate.m_nSign = m_pPlayerConsortData->m_nSign;

		PersistSystem::getSingleton().postThreadMsg(consortSignUpdate, playerGuid);
	
	
			List<Goods> awardList;
		GiftManager::getSingleton().getAwardByID(awardId,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		List<ReturnItemEle> rtItemList;

		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,systemID);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,systemID);
				rtItemList.insertTail(rtIemEle);
			}

			
		}
		
		Json::Value root;
		 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root);

		
		root["errorId"] = LynxErrno::None;
		root["index"] = awardIndex;
	
		for(List<UInt32>::Iter * awardIter  = m_pPlayerConsortData->m_nSignAwards.begin(); awardIter != NULL; 
			awardIter = m_pPlayerConsortData->m_nSignAwards.next(awardIter) )
		{
			root["awards"].append(awardIter->mValue);
		}
			
		

		Json::StyledWriter writer;
		std::string respstr = writer.write(root);
		
		GCConsortSignAwardResp  signAwardResp;
		signAwardResp.mPacketID = BOC_CONSORTSIGNAWARD_RESP;
		signAwardResp.mRespJsonStr = writer.write(root);

		LOG_INFO("get sign award is: %s", respstr.c_str());
		NetworkSystem::getSingleton().sendMsg(signAwardResp,connId);
}


void ConsortManager::notifyOtherOnlineConcortMember(UInt32 msgtype, UInt64 selfuid,  ConsortData * pConsortData,  UInt32 jobflag ,
													UInt64 notuid)
{
	
	Json::Value root;
	root["msgtype"] =  msgtype;
	root["consortid"] = pConsortData->m_nId;
	root["consortname"] = pConsortData->m_strName.c_str();
	root["consortlv"] = pConsortData->m_nLv;
	root["count"] = pConsortData->m_nCount;
	
	ConsortRankData * consortRankData = ConsortSystem::getSingleton().getConsortRankData(pConsortData->m_nId);
    if(!consortRankData)
	{
		root["rank"] = ConsortSystem::getSingleton().getConsortLastRank() ;
		root["power"] = pConsortData->m_nPower;
	}
	else
	{
		root["rank"] = consortRankData->m_nRank;
		root["power"] = consortRankData->m_nPower;
	}
	
	root["check"] = pConsortData->m_nCheck;
	root["condition"] = pConsortData->m_nPowerLimit;

	root["desc"] = pConsortData->m_strDesc.c_str();
	root["consortres"] = pConsortData->m_nRes;
	root["exp"] = pConsortData->m_nExp;
	for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
		memberIter != pConsortData->m_nMemberList.end();  
		memberIter = pConsortData->m_nMemberList.next(memberIter))
	{
		Json::Value memberRoot;
		memberRoot["playeruid"]= memberIter->mValue;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(memberIter->mValue,baseInfo);
		ConsortInfoData *  consortInfo = LogicSystem::getSingleton().getConsortInfo(memberIter->mValue);
		if(!flag  || !consortInfo)
		{
			continue;
		}

		memberRoot["playername"] = baseInfo.name.c_str();
		memberRoot["playerlv"] = baseInfo.level;
		memberRoot["viplv"] = baseInfo.vipLv;
		memberRoot["model"] = baseInfo.modelId;
		memberRoot["leave"] = baseInfo.leaveTime;
		memberRoot["power"] = baseInfo.power;
		memberRoot["curcontribut"] = consortInfo->curContribute;
		memberRoot["totalcontribut"] = consortInfo->totalContribute;
		memberRoot["job"] = consortInfo->consortJob;
    
		root["member"].append(memberRoot);
        

	}

	UInt64 checktime = time(0);
	root["checktime"] = checktime;

	for(List<ApplyData>::Iter * applyIter = pConsortData->m_applyList.begin(); 
		applyIter != pConsortData->m_applyList.end(); 
		applyIter = pConsortData->m_applyList.next(applyIter))
	{
		Json::Value applyRoot;
		applyRoot["playeruid"] = applyIter->mValue.m_nApply;
		BaseInfoData baseInfo;
		bool flag = LogicSystem::getSingleton().getBaseInfo(applyIter->mValue.m_nApply,baseInfo);
		
		if(!flag  )
		{
			continue;
		}

		applyRoot["playername"] = baseInfo.name.c_str();
		applyRoot["playerlv"] = baseInfo.level;
		applyRoot["viplv"] = baseInfo.vipLv;
		applyRoot["model"] = baseInfo.modelId;
		applyRoot["leave"] = baseInfo.leaveTime;
		applyRoot["power"] = baseInfo.power;
		applyRoot["time"] = applyIter->mValue.m_nTime;

		root["applies"].append(applyRoot);
	}

	
		for(List<LogData>::Iter * logIter = pConsortData->m_logList.begin(); 
		logIter != pConsortData->m_logList.end(); logIter = pConsortData->m_logList.next(logIter) )
		{
			 
				Json::Value logroot;
				logroot["tempid"] = logIter->mValue.m_nTempId;
				logroot["consortId"] = logIter->mValue.m_nConsortId;
				logroot["playerUid"] =  logIter->mValue.m_nPlayerUid;
				logroot["playerName"] = logIter->mValue.m_strPlayername.c_str();
				logroot["param1"] = logIter->mValue.m_strParam1.c_str();
				logroot["param2"] = logIter->mValue.m_strParam2.c_str();
				logroot["logtime"]= logIter->mValue.m_nTime;

				root["loglist"].append(logroot);
		}

		Json::StyledWriter writer;
		std::string jsonStr = writer.write(root);
 
		LOG_INFO("consort data notify info is: %s ", jsonStr.c_str()) ;

			for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
		memberIter != pConsortData->m_nMemberList.end();  
		memberIter = pConsortData->m_nMemberList.next(memberIter))
			{
				if(memberIter->mValue == notuid || memberIter->mValue == selfuid)
				{
					continue;
				}

				Player * pMember = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
				if(!pMember)
				{
					continue;
				}

				const ConnId & memberCon = pMember->getConnId();
				if(!memberCon)
				{
					continue;
				}
				//���Ϊ1��ʾ��֪ͨ�᳤���᳤
				if(jobflag)
				{
					PlayerConsortData * playerConsortData = pMember->getConsortManager().getPlayerConsortData();
					//����ͨ��Ա��������
					if(! (playerConsortData->m_nConsortJob))
					{
						continue;
					}
				}

				GCConsortDataNotify datanotify;
				datanotify.mPacketID = BOC_CONSORTDATANOTIFY;
	
				datanotify.mRespJsonStr = jsonStr;
				NetworkSystem::getSingleton().sendMsg(datanotify,memberCon);

			}


	
}


void ConsortManager::gameActiveInit(Player *  playerInit,  UInt64 uidInit)
{
	if(playerInit)
	{
			//���������ʼ����������
			playerInit->getConsortManager().initCatTask();
			//ǩ�����ʼ��
			playerInit->getConsortManager().initialSignStruct();
			
	}
	else
	{
			List<UInt32 > taskList;
			generateCatTask(4, taskList);
			List<XingxiaTask>  listXingxiaTasks; 
			for(List<UInt32 >::Iter * taskIter = taskList.begin(); taskIter != NULL; 
				taskIter = taskList.next(taskIter) )
			{
					XingxiaTask xingxiaTask;
					xingxiaTask.m_nTaskId = taskIter->mValue;
					xingxiaTask.m_nGettime = 0;
					listXingxiaTasks.insertTail(xingxiaTask);
			}

	
			PersistConsortLoyalUpdate msg;
			msg.m_listXingxiaTasks = listXingxiaTasks;
			msg.m_nPlayerUid = uidInit;
			msg.m_nRefreshTimes = 0;

			PersistSystem::getSingleton().postThreadMsg(msg, uidInit);

			//ǩ�����ʼ��
			//ǩ����Ϣ����
			List<UInt32 > awards;
			for(UInt32 i = 0; i < 4; i++)
			{
				awards.insertTail(UInt32(0));
			}


			PersistConsortSignUpdate consortSignUpdate;
			consortSignUpdate.m_nPlayerUid = uidInit;
			consortSignUpdate.m_nSignAwards = awards;
			consortSignUpdate.m_nSign = 0;

			PersistSystem::getSingleton().postThreadMsg(consortSignUpdate, uidInit);

	}

	
}

void ConsortManager::refreshCatTask()
{
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCCatTaskRefreshResp refreshResp;
		refreshResp.mPacketID = BOC_CATTASKREFRESHRESP;
		Json::StyledWriter writer;

		refreshResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(refreshResp, m_pPlayer->getConnId());
		LOG_INFO("Consortid not found!!!");
		assert(false);
		return ;
	}


	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	const ConnId & connId = m_pPlayer->getConnId();
		Vector< List<XingxiaTask>::Iter *> iterVecs;
		//�жϸ������Ƿ��Ѿ���������������
		for(List<XingxiaTask>::Iter * findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin();  
			findTaskIter != NULL;  findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(findTaskIter) )
		{
			if(findTaskIter->mValue.m_nGettime)
			{
				continue;
			}
			//���ҵ�Ϊδ��ǲ���������˫�ص���vector
			iterVecs.push_back(findTaskIter);
		}

		UInt32 taskCount = iterVecs.size();
		if(!taskCount)
		{
			Json::Value root;
			root["errorId"] = LynxErrno::NoRefreshTask;
			GCCatTaskRefreshResp refreshResp;
			refreshResp.mPacketID = BOC_CATTASKREFRESHRESP;
			Json::StyledWriter writer;

			refreshResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(refreshResp,connId);
			LOG_INFO("refresh task is:  %s", refreshResp.mRespJsonStr.c_str());
			assert(false);
			return ;
		}

		ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
		if(!consortLvTemp )
		{
			consortLvTemp = CONSORTLV_TABLE().get(10);
		}

		
		if(consortLvTemp->mXingManDo <=  m_pPlayerConsortData->m_nRefreshTimes)
		{
			//�ж����vip��Ӧ�Ļ��Ѵ����Ƿ�
			UInt32 vipLv = m_pPlayer->getVipLevel();
			VipTemplate * vipTemp = VIP_TABLE().get(vipLv);
			if(!vipTemp)
			{
				LOG_INFO("design table error!!!  vipTemp not found");
				assert(false);
				return ;
			}
			
			UInt32 costTimes = m_pPlayerConsortData->m_nRefreshTimes - consortLvTemp->mXingManDo;
			if(costTimes >= vipTemp->cattaskrefreshtimes)
			{
				//��ʾ��ҵ�ǰvip���ѵ�ˢ�´����Ѿ�����
				Json::Value root;
				root["errorId"] = LynxErrno::VipRefreshTask;
				GCCatTaskRefreshResp refreshResp;
				refreshResp.mPacketID = BOC_CATTASKREFRESHRESP;
				Json::StyledWriter writer;

				refreshResp.mRespJsonStr = writer.write(root);
				NetworkSystem::getSingleton().sendMsg(refreshResp,connId);
				LOG_INFO("refresh task is:  %s", refreshResp.mRespJsonStr.c_str());
				assert(false);
				return ;
			}

	

			GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
			//���ݻ��ѵ�����������ͳ����Ҫ�Ľ��
			UInt64 needGold = globalValue.uCatTaskInitGold + costTimes* globalValue.uCatTaskAddGold;
			//�ж���Ҫ������Ƿ񳬹�����ֵ���������Ϊ����ֵ
			if(needGold > 	globalValue.uCatTaskMaxGold)	
			{
				needGold = globalValue.uCatTaskMaxGold;
			}
			
				//�ж�Ԫ���Ƿ���
			UInt64 curGold = m_pPlayer->getPlayerGold();
			if(needGold >  curGold)
			{
					//��ʾ���Ԫ������
					Json::Value root;
					root["errorId"] = LynxErrno::RmbNotEnough;
					GCCatTaskRefreshResp refreshResp;
					refreshResp.mPacketID = BOC_CATTASKREFRESHRESP;
					Json::StyledWriter writer;

					refreshResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(refreshResp,connId);
					LOG_INFO("refresh task is:  %s", refreshResp.mRespJsonStr.c_str());
					assert(false);
					return ;
			}

			//�۳�Ԫ��			

			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - needGold;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog79);


		}

		//�������ˢ�´���
		m_pPlayerConsortData->m_nRefreshTimes ++;

		List<UInt32> taskList;
		generateCatTask(taskCount, taskList);
		UInt32 index = 0;

		Json::Value root;
		for(List<UInt32>::Iter * taskIter = taskList.begin();  taskIter != NULL;
			taskIter = taskList.next(taskIter) )
		{
			Json::Value taskRoot;
			iterVecs[index]->mValue.m_nTaskId = taskIter->mValue;
			iterVecs[index]->mValue.m_nGettime = 0;
			index++;

			taskRoot["taskid"]  = taskIter->mValue;
			taskRoot["gettime"] = 0;
			taskRoot["remaintime"] = 0;

			root["taskdata"].append(taskRoot);
		}

		//���������������

			PersistConsortLoyalUpdate msg;
			msg.m_listXingxiaTasks = m_pPlayerConsortData->m_listXingxiaTasks;
			msg.m_nPlayerUid = playerUid;
			msg.m_nRefreshTimes = m_pPlayerConsortData->m_nRefreshTimes;

			PersistSystem::getSingleton().postThreadMsg(msg, playerUid);


			//�������ͻ���
			
			root["refreshtimes"] = m_pPlayerConsortData->m_nRefreshTimes;
			root["errorId"] = LynxErrno::None;
			root["gold"] = m_pPlayer->getPlayerGold();

				GCCatTaskRefreshResp refreshResp;
					refreshResp.mPacketID = BOC_CATTASKREFRESHRESP;
					Json::StyledWriter writer;

					refreshResp.mRespJsonStr = writer.write(root);
					NetworkSystem::getSingleton().sendMsg(refreshResp,connId);
					LOG_INFO("refresh task is:  %s", refreshResp.mRespJsonStr.c_str());

}

void ConsortManager::initBusinessCat()
{

}


void ConsortManager::initCatTask()
{
	
	List<UInt32 > taskList;
	generateCatTask(4, taskList);
	UInt64 curTime = time(0);
	List<XingxiaTask> ::Iter * xingxiaIter = m_pPlayerConsortData->m_listXingxiaTasks.begin();
	for(List<UInt32 >::Iter * taskIter = taskList.begin(); taskIter != NULL;
		taskIter = taskList.next(taskIter) )
	{
		
		assert(xingxiaIter);

		xingxiaIter->mValue.m_nTaskId = taskIter->mValue;
		xingxiaIter->mValue.m_nGettime = 0;
		//��������б���һ��Ԫ��
		xingxiaIter = m_pPlayerConsortData->m_listXingxiaTasks.next(xingxiaIter);
	}

	m_pPlayerConsortData->m_nRefreshTimes = 0;

	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	PersistConsortLoyalUpdate msg;
	msg.m_listXingxiaTasks = m_pPlayerConsortData->m_listXingxiaTasks;
	msg.m_nPlayerUid = playerUid;
	msg.m_nRefreshTimes = m_pPlayerConsortData->m_nRefreshTimes;

	PersistSystem::getSingleton().postThreadMsg(msg, playerUid);
}


bool  ConsortManager::generateCatTask(UInt32 taskCount, List<UInt32> &taskList)
{
	assert(taskCount > 0);
	List<RandomTask> randomlist;
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		assert(false);
		return false;
	}
	
	List<CatTaskTemplate > * taskpool = CATTASK_TABLE().getTaskList(pConsortData->m_nLv);
	if(!taskpool)
	{
		LOG_INFO("task pool not found!!!, design error!!");
		assert(false);
		return false;
	}

	UInt32 totalRate = 0;
	for(List<CatTaskTemplate >::Iter * taskIter = taskpool->begin();  taskIter != NULL;
		taskIter = taskpool->next(taskIter) )
	{
		bool isExist = false;
		//�жϸ������Ƿ��Ѿ���������������
		for(List<XingxiaTask>::Iter * curtaskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin(); 
			curtaskIter != NULL;  curtaskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(curtaskIter) )
		{
			//��������������
			if(curtaskIter->mValue.m_nTaskId == taskIter->mValue.mId)
			{
				isExist = true;
				break;
			}
		}
		
		//��������������������ô���絽������У���������random�������µ�����
		if(!isExist)
		{
			totalRate += taskIter->mValue.mRate;
			RandomTask randomTask;
			randomTask.taskId = taskIter->mValue.mId;
			randomTask.tag = totalRate;
			randomTask.rate = taskIter->mValue.mRate;
			randomlist.insertTail(randomTask);
		}

	}

	if(randomlist.size() <  taskCount)
	{
		LOG_INFO("task pool task not enough!!");
		assert(false);
		return false;
	}

	if(randomlist.size() == taskCount)
	{
		for(List<RandomTask>::Iter * randomTaskIter = randomlist.begin(); 
			randomTaskIter != NULL; randomTaskIter = randomlist.next(randomTaskIter) )
		{
			taskList.insertTail(randomTaskIter->mValue.taskId);
		}

		return true;
	}

	//����غܴ���Ҫ���ȡ�������ļ���
	for(UInt32 i = 0; i < taskCount; i++)
	{
		//ȡ�����һ��Ԫ��
		 List<RandomTask>::Iter * lastTaskIter = randomlist.last();
		//���һ��Ԫ�ص�tag��Ϊrate�ܺ�
		 UInt32 randRes = rand()%(lastTaskIter->mValue.tag);
		 List<RandomTask>::Iter *  findTaskIter = NULL;
		 for(List<RandomTask>::Iter * randomTaskIter = randomlist.begin(); 
			randomTaskIter != NULL; randomTaskIter = randomlist.next(randomTaskIter) )
		 {
			 if(randRes < randomTaskIter->mValue.tag)
			 {
				findTaskIter = randomTaskIter;
				break;
			 }
		 }

		 assert(findTaskIter);
	
		 //Ϊ�����Ч�ʣ����ٱ������������ҵ��Ľڵ���һ������tag���£�����Ϊ�������ֵ
		 //���ҽ��ҵ��Ľڵ��Ƴ�
		 for(List<RandomTask>::Iter * randomTaskIter = randomlist.next(findTaskIter); 
			randomTaskIter != NULL; randomTaskIter = randomlist.next(randomTaskIter))
		 {
			 randomTaskIter->mValue.tag -= findTaskIter->mValue.rate;
		 }

		 //�Ƚ��ҵ��Ľڵ���뵽taskList�����ڷ���
		 taskList.insertTail(findTaskIter->mValue.taskId);
		 //���ҵ��Ľڵ��Ƴ�������֮ǰ�Ѿ�������ȥ���ýڵ���tag����
		 randomlist.erase(findTaskIter);

	}


	return  true;
}


void ConsortManager::getCatTask(UInt32 taskId)
{
	const ConnId & connId = m_pPlayer->getConnId();

	UInt64 playerUid = m_pPlayer->getPlayerGuid();

//////////////////////////
UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);

GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	if(pConsortData->m_nLv < globalValue.uGuildOpenCatTask)
	{
		LOG_INFO("guild level is not match!!!");

		GCCatTaskGetResp taskGetResp;
		Json::Value root;
		root["errorId"] = LynxErrno::ConsortLvNotEnough;


		taskGetResp.mPacketID = BOC_CATTASKGETRESP;
		Json::StyledWriter writer;
		taskGetResp.mRespJsonStr = writer.write(root);
		LOG_INFO("CAT TASK GET  IS:  %s", taskGetResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(taskGetResp,connId);

		return ;
	}

	////////////////////////

	List<XingxiaTask>::Iter * findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin(); 
		//�жϸ������Ƿ��Ѿ���������������
		for( ;  findTaskIter != NULL;  findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(findTaskIter) )
		{
			//��������������
			if(findTaskIter->mValue.m_nTaskId == taskId)
			{
		
				break;
			}
		}

	if(!findTaskIter)
	{
		GCCatTaskGetResp taskGetResp;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		
		taskGetResp.mPacketID = BOC_CATTASKGETRESP;
		Json::StyledWriter writer;
		taskGetResp.mRespJsonStr = writer.write(root);
		LOG_INFO("CAT TASK GET  IS:  %s", taskGetResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(taskGetResp,connId);
		LOG_WARN("taskid not found %u", taskId);
		assert(false);
		return;
	}


	UInt64 getTime  = time(0);
	//������ȡʱ�����
	findTaskIter->mValue.m_nGettime = getTime;

	//�������
	PersistConsortLoyalUpdate msg;
	msg.m_listXingxiaTasks = m_pPlayerConsortData->m_listXingxiaTasks;
	msg.m_nPlayerUid = playerUid;
	msg.m_nRefreshTimes = m_pPlayerConsortData->m_nRefreshTimes;

	PersistSystem::getSingleton().postThreadMsg(msg, playerUid);

	CatTaskTemplate * catTaskTemp = CATTASK_TABLE().get(taskId);


	GCCatTaskGetResp taskGetResp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["taskid"] = taskId;
	root["gettime"] = getTime;
	root["remaintime"] = catTaskTemp->mTime;

	taskGetResp.mPacketID = BOC_CATTASKGETRESP;
	Json::StyledWriter writer;
	taskGetResp.mRespJsonStr = writer.write(root);
	LOG_INFO("CAT TASK GET  IS:  %s", taskGetResp.mRespJsonStr.c_str());
	NetworkSystem::getSingleton().sendMsg(taskGetResp,connId);

}

void ConsortManager::getCatTaskAward(UInt32 taskId)
{
	const ConnId & connId = m_pPlayer->getConnId();

	UInt64 playerUid = m_pPlayer->getPlayerGuid();
	List<XingxiaTask>::Iter * findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.begin(); 
		//�жϸ������Ƿ��Ѿ���������������
		for( ;  findTaskIter != NULL;  findTaskIter = m_pPlayerConsortData->m_listXingxiaTasks.next(findTaskIter) )
		{
			//��������������
			if(findTaskIter->mValue.m_nTaskId == taskId)
			{
		
				break;
			}
		}

	if(!findTaskIter)
	{
		LOG_WARN("taskid not found %u", taskId);
		GCCatTaskGetAwardResp getAwardResp;
		getAwardResp.mPacketID = BOC_CATTASKGETAWARDRESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		Json::StyledWriter writer;
		getAwardResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(getAwardResp,connId);
		assert(false);
		return;
	}

	CatTaskTemplate * catTaskTemp = CATTASK_TABLE().get(taskId);

	UInt64 curTime  = time(0);

	//�жϵ�ǰʱ���Ƿ����ȡʱ���ʱ�������������ȡ��Ҫ��ʱ��
	UInt64 costTime = curTime -  findTaskIter->mValue.m_nGettime;
	if(costTime < catTaskTemp->mTime)
	{
		//ʱ��δ��
		LOG_WARN("task time not reach");
		GCCatTaskGetAwardResp getAwardResp;
		getAwardResp.mPacketID = BOC_CATTASKGETAWARDRESP;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		Json::StyledWriter writer;
		getAwardResp.mRespJsonStr = writer.write(root);
		LOG_INFO("CAT TASK GET AWARD IS:  %s", getAwardResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(getAwardResp,connId);

		return;
	}

	ReturnItemEle  rtItemEle1;
	ReturnItemEle  rtItemEle2;
	//��ȡ����
	m_pPlayer->getAllItemManager().addAwardMaterial(catTaskTemp->mType1, 
		catTaskTemp->mSubType1, catTaskTemp->mCount1, rtItemEle1,MiniLog79);
	m_pPlayer->getAllItemManager().addAwardMaterial(catTaskTemp->mType2, 
		catTaskTemp->mSubType2, catTaskTemp->mCount2, rtItemEle2,MiniLog79);

	List<ReturnItemEle> rtList;
	rtList.insertTail(rtItemEle1);
	rtList.insertTail(rtItemEle2);

	//�����µ�����
	List<UInt32> taskList;
	generateCatTask(1, taskList);

	findTaskIter->mValue.m_nTaskId = taskList.begin()->mValue;
	findTaskIter->mValue.m_nGettime = 0;

	//�������
	PersistConsortLoyalUpdate msg;
	msg.m_listXingxiaTasks = m_pPlayerConsortData->m_listXingxiaTasks;
	msg.m_nPlayerUid = playerUid;
	msg.m_nRefreshTimes = m_pPlayerConsortData->m_nRefreshTimes;

	PersistSystem::getSingleton().postThreadMsg(msg, playerUid);

	


	GCCatTaskGetAwardResp getAwardResp;
	getAwardResp.mPacketID = BOC_CATTASKGETAWARDRESP;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["taskid"] = findTaskIter->mValue.m_nTaskId;
	root["gettime"] = 0;
	root["remaintime"] = 0;
	m_pPlayer->getAllItemManager().convertItemListToStr(rtList, root);

	
	Json::StyledWriter writer;
	getAwardResp.mRespJsonStr = writer.write(root);
	LOG_INFO("CAT TASK GET AWARD IS:  %s", getAwardResp.mRespJsonStr.c_str());
	NetworkSystem::getSingleton().sendMsg(getAwardResp,connId);
}

//����ѵ����ʼ
void ConsortManager::beginKitchenQue()
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");		
		Json::Value root;
		GCKitchenBeginResp beginResp;
		beginResp.mPacketID = BOC_KITCHENBEGIN_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		beginResp.mRespJsonStr = writer.write(root);
		LOG_INFO("KITCHEN BEGIN IS:  %s", beginResp.mRespJsonStr.c_str());

		NetworkSystem::getSingleton().sendMsg(beginResp,connId);
		assert(false);
		return ;
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	if(pConsortData->m_nLv < globalValue.uGuildOpenKitchen)
	{
		LOG_INFO("guild level is not match!!!");

		Json::Value root;
			GCKitchenBeginResp beginResp;
			beginResp.mPacketID = BOC_KITCHENBEGIN_RESP;

			root["errorId"] = LynxErrno::ConsortLvNotEnough;

			Json::StyledWriter writer;
			beginResp.mRespJsonStr = writer.write(root);
			LOG_INFO("KITCHEN BEGIN IS:  %s", beginResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(beginResp,connId);

		return ;
	}


	UInt64 nowtime = time(0);
	if(nowtime - m_pPlayerConsortData->m_nLeaveTime < 24 * 60 *60)
	{
			Json::Value root;
			GCKitchenBeginResp beginResp;
			beginResp.mPacketID = BOC_KITCHENBEGIN_RESP;

			root["errorId"] = LynxErrno::JustLeaveConsort;

			Json::StyledWriter writer;
			beginResp.mRespJsonStr = writer.write(root);
			LOG_INFO("KITCHEN BEGIN IS:  %s", beginResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(beginResp,connId);
		return ;
	}



	ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nSignLv);
	if(!consortLvTemp)
	{
		consortLvTemp = CONSORTLV_TABLE().get(10);
	}
	
	//�������ÿ���淨������
	if(m_pPlayerConsortData->m_nKitchenTimes >= consortLvTemp->mKitchentimes)
	{
			Json::Value root;
			GCKitchenBeginResp beginResp;
			beginResp.mPacketID = BOC_KITCHENBEGIN_RESP;

			root["errorId"] = LynxErrno::KitchentimesNotEnouf;

			Json::StyledWriter writer;
			beginResp.mRespJsonStr = writer.write(root);
			LOG_INFO("KITCHEN BEGIN IS:  %s", beginResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(beginResp,connId);


		LOG_INFO("kitchentimes has use all !");
		//assert(false);
		return ;
	}
	
	//ˢ�´�������
	m_pPlayerConsortData->m_nKitchenTimes++;
	//���������
	const List<UInt32>& queList = consortLvTemp->kitchenQueList;
	assert(queList.size());
	UInt32 index = rand()%(queList.size() );

    List<UInt32>::Iter * findque = queList.getn(index);
	
	///////////////////////////////////////////////////////////////////////
	//����
	Json::Value root;
	GCKitchenBeginResp beginResp;
	beginResp.mPacketID = BOC_KITCHENBEGIN_RESP;

	root["errorId"] = LynxErrno::None;
	root["questionid"] = findque->mValue;
	root["kitchentimes"] = m_pPlayerConsortData->m_nKitchenTimes;

	
	Json::StyledWriter writer;
	beginResp.mRespJsonStr = writer.write(root);
	LOG_INFO("KITCHEN BEGIN IS:  %s", beginResp.mRespJsonStr.c_str());
	NetworkSystem::getSingleton().sendMsg(beginResp,connId);

	////////////////////////////////////////////////////////////////////////////////////////
	//����
	PersistConsortKitQueUpdate kitQueUpdate;
	kitQueUpdate.m_nPlayerUid = playerUid;
	kitQueUpdate.m_nRefreshTimes = m_pPlayerConsortData->m_nKitchenTimes;
	PersistSystem::getSingleton().postThreadMsg(kitQueUpdate, playerUid);

}

void ConsortManager::endKitchenQue(UInt32 questionId, UInt32 score,UInt32 systemID)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	KitchenQuestionTemplate * kitchenQueTemp = KITCHENQUE_TABLE().get(questionId);
	if(!kitchenQueTemp)
	{
		Json::Value root;
		CGKitchenEndResp endResp;
		endResp.mPacketID = BOC_KITCHENEND_RESP;

		root["errorId"] = LynxErrno::NotFoundQue;
	
		Json::StyledWriter writer;
		endResp.mRespJsonStr = writer.write(root);
		LOG_INFO("KITCHEN END IS:  %s", endResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(endResp,connId);

		LOG_INFO("question not found!!!");
		return ;
	}

	UInt32 awardID = 0;
	switch(score)
	{
	case 1:
		awardID = kitchenQueTemp->mAward1;
		break;
	case 2:
		awardID = kitchenQueTemp->mAward2;
		break;
	case 3:
		awardID = kitchenQueTemp->mAward3;
		break;
	case 4:
		awardID = kitchenQueTemp->mAward4;
		break;
	case 5:
		awardID = kitchenQueTemp->mAward5;
		break;
	}

	List<Goods> awardList;
	List<Goods> tmpItemList;

		GiftManager::getSingleton().getAwardByID(awardID,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		List<ReturnItemEle> rtItemList;

		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,systemID);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,systemID);
				rtItemList.insertTail(rtIemEle);
			}

			
		}
		
		Json::Value root;
		 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root);
		 root["kitchentimes"] = m_pPlayerConsortData->m_nKitchenTimes;
		 root["errorId"] = LynxErrno::None;

		CGKitchenEndResp endResp;
		endResp.mPacketID = BOC_KITCHENEND_RESP;

	
	
		Json::StyledWriter writer;
		endResp.mRespJsonStr = writer.write(root);
		LOG_INFO("KITCHEN END IS:  %s", endResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(endResp,connId);


}

void ConsortManager::businessCatTime()
{

	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		Json::Value root;
		LOG_INFO("Consortid not found!!!");
		 root["errorId"] = LynxErrno::ClienServerDataNotMatch;
		GCBusinessCatTimeResp businessCatTimeResp;
		businessCatTimeResp.mPacketID = BOC_BUSINESSCATTIME_RESP;


		Json::StyledWriter writer;
		businessCatTimeResp.mRespJsonStr = writer.write(root);
		LOG_INFO("BUSINESSCAT TIME RESP IS:  %s", businessCatTimeResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(businessCatTimeResp,connId);
		assert(false);
		return ;
	}



	UInt64 secOfToday = TimeUtil::getSecOfToday();
	UInt64 remainTime = 0;
	UInt32 type = 0;
	if(secOfToday/3600 < 12)
	{
		remainTime = 12 * 3600 - secOfToday;
	}
	else
	{
		remainTime = 24 * 3600 - secOfToday;
		type = 1;
	}

	Json::Value root;

		 root["remaintime"] = remainTime;
		 root["errorId"] = LynxErrno::None;
		root["type"] = type;

		root["businesslv"] = pConsortData->m_nBusinessLv;
	    root["businesscattimes"] = m_pPlayerConsortData->m_nBusinessCatTimes;

		 //��Ʒ����״̬���
		for(List<UInt32>::Iter * buyIter = m_pPlayerConsortData->m_nBuyList.begin();   
				buyIter != NULL; buyIter = m_pPlayerConsortData->m_nBuyList.next(buyIter))
		{
			root["shops"].append(buyIter->mValue);
			
		}


		GCBusinessCatTimeResp businessCatTimeResp;
		businessCatTimeResp.mPacketID = BOC_BUSINESSCATTIME_RESP;
	
	
		Json::StyledWriter writer;
		businessCatTimeResp.mRespJsonStr = writer.write(root);
		LOG_INFO("BUSINESSCAT TIME RESP IS:  %s", businessCatTimeResp.mRespJsonStr.c_str());
		NetworkSystem::getSingleton().sendMsg(businessCatTimeResp,connId);



}

void ConsortManager::businessCatReset()
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCBusinessCatResetResp resetResp;
		resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		resetResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(resetResp,connId);
		assert(false);
		return ;
	}

	UInt64 nowtime = time(0);
	if(nowtime - m_pPlayerConsortData->m_nLeaveTime < 24 * 60 *60)
	{
			Json::Value root;
			GCBusinessCatResetResp resetResp;
			resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

			root["errorId"] = LynxErrno::JustLeaveConsort;

			Json::StyledWriter writer;
			resetResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT RESET  IS:  %s", resetResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(resetResp,connId);
		return ;
	}

	ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nBusinessLv);

	if(!consortLvTemp)
	{
		consortLvTemp = CONSORTLV_TABLE().get(10);
	}
	
	//�������ÿ���淨������
	if(m_pPlayerConsortData->m_nBusinessCatTimes >= consortLvTemp->mBusinessTimes)
	{
			Json::Value root;
			GCBusinessCatResetResp resetResp;
			resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

			root["errorId"] = LynxErrno::BusinessCatNotEnouf;

			Json::StyledWriter writer;
			resetResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT RESET  IS:  %s", resetResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(resetResp,connId);


		LOG_INFO("business cat reset times has use all !");
		//assert(false);
		return ;
	}
	
	
			GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

			if(pConsortData->m_nLv < globalValue.uGuildOpenBusinessCat)
			{
					LOG_INFO("guild level is not match!!!");

						Json::Value root;
					GCBusinessCatResetResp resetResp;
					resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

					root["errorId"] = LynxErrno::ConsortLvNotEnough;

					Json::StyledWriter writer;
					resetResp.mRespJsonStr = writer.write(root);
					LOG_INFO("BUSINESS CAT RESET  IS:  %s", resetResp.mRespJsonStr.c_str());

					NetworkSystem::getSingleton().sendMsg(resetResp,connId);


					return ;
			}



			//���ݻ��ѵ�����������ͳ����Ҫ�Ľ��
			UInt64 needGold = globalValue.uBusinessCatInitGold + m_pPlayerConsortData->m_nBusinessCatTimes* globalValue.uCatTaskAddGold;
			//�ж���Ҫ������Ƿ񳬹�����ֵ���������Ϊ����ֵ
			if(needGold > 	globalValue.uBusinessCatMaxGold)	
			{
				needGold = globalValue.uBusinessCatMaxGold;
			}
			
				//�ж�Ԫ���Ƿ���
			UInt64 curGold = m_pPlayer->getPlayerGold();
			if(needGold >  curGold)
			{
					//��ʾ���Ԫ������
					Json::Value root;
					GCBusinessCatResetResp resetResp;
					resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

					root["errorId"] = LynxErrno::RmbNotEnough;

					Json::StyledWriter writer;
					resetResp.mRespJsonStr = writer.write(root);
					LOG_INFO("BUSINESS CAT RESET  IS:  %s", resetResp.mRespJsonStr.c_str());

					NetworkSystem::getSingleton().sendMsg(resetResp,connId);


					LOG_INFO("rmb not enougth !");
					//assert(false);
					return ;
			}

			//�۳�Ԫ��

			Goods goods;
			List<Goods> itemList;

			goods.resourcestype =AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = 0 - needGold;
			itemList.insertTail(goods);
			GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog79);
			
			//ˢ�´�������
	       m_pPlayerConsortData->m_nBusinessCatTimes++;

		   //��Ʒ����״̬���
			for(List<UInt32>::Iter * buyIter = m_pPlayerConsortData->m_nBuyList.begin();   
				buyIter != NULL; buyIter = m_pPlayerConsortData->m_nBuyList.next(buyIter))
			{
				buyIter->mValue = 0;
			}

			//����
		   PersistBusinessCatUpdate catupdate;
		   catupdate.m_nPlayerUid = playerUid;
		   catupdate.m_nRefreshTimes = m_pPlayerConsortData->m_nBusinessCatTimes;
			 catupdate.m_nBuyList = m_pPlayerConsortData->m_nBuyList;
		   PersistSystem::getSingleton().postThreadMsg(catupdate,  playerUid);

		   	Json::Value root;
					GCBusinessCatResetResp resetResp;
					resetResp.mPacketID = BOC_BUSINESSCATRESET_RESP;

					root["errorId"] = LynxErrno::None;
					root["gold"] = curGold - needGold;
					root["businesscattimes"] = m_pPlayerConsortData->m_nBusinessCatTimes;
					
 

					Json::StyledWriter writer;
					resetResp.mRespJsonStr = writer.write(root);
					LOG_INFO("BUSINESS CAT RESET  IS:  %s", resetResp.mRespJsonStr.c_str());

					NetworkSystem::getSingleton().sendMsg(resetResp,connId);



}

void ConsortManager::businessCatBuy(UInt32 buyindex)
{
		const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 playerUid = m_pPlayer->getPlayerGuid();

	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCBusinessCatBuyResp buyResp;
		buyResp.mPacketID = BOC_BUSINESSCATBUY_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		buyResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(buyResp,connId);
		assert(false);
		return ;
	}

	UInt64 nowtime = time(0);
	if(nowtime - m_pPlayerConsortData->m_nLeaveTime < 24 * 60 *60)
	{
			Json::Value root;
			GCBusinessCatBuyResp buyResp;
			buyResp.mPacketID = BOC_BUSINESSCATBUY_RESP;

			root["errorId"] = LynxErrno::JustLeaveConsort;

			Json::StyledWriter writer;
			buyResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT BUY  IS:  %s", buyResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(buyResp,connId);
		return ;
	}

	ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nBusinessLv);

	if(!consortLvTemp)
	{
		consortLvTemp = CONSORTLV_TABLE().get(10);
	}
	
	BusinessCatTemplate * busCatTemp = BUSINESSCAT_TABLE().get(	consortLvTemp->mBusinessId );

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	if(pConsortData->m_nLv < globalValue.uGuildOpenBusinessCat)
	{
		LOG_INFO("guild level is not match!!!");

			Json::Value root;
			GCBusinessCatBuyResp buyResp;
			buyResp.mPacketID = BOC_BUSINESSCATBUY_RESP;

			root["errorId"] = LynxErrno::ConsortLvNotEnough;

			Json::StyledWriter writer;
			buyResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT BUY  IS:  %s", buyResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(buyResp,connId);

		
		return ;
	}


	UInt64 secOfToday = TimeUtil::getSecOfToday();
	UInt64 remainTime = 0;
	UInt32 index = buyindex;
	if(secOfToday/3600 >= 12)
	{
		index += 8;
	}
	
	List<AllItemEle>::Iter * costIter = busCatTemp->mCostList.getn(index);
	List<AllItemEle>::Iter * awardIter = busCatTemp->mAwardList.getn(index);

	bool	 flag = m_pPlayer->getAllItemManager().costMaterial(costIter->mValue.resType, 
		costIter->mValue.subType, costIter->mValue.count,MiniLog79);
	
	if(!flag)
	{
			Json::Value root;
			GCBusinessCatBuyResp buyResp;
			buyResp.mPacketID = BOC_BUSINESSCATBUY_RESP;

			root["errorId"] = LynxErrno::CostNotEnouf;

			Json::StyledWriter writer;
			buyResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT BUY  IS:  %s", buyResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(buyResp,connId);

		return ;
	}

	ReturnItemEle  rtItemEle;

	m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resType, 
		awardIter->mValue.subType, awardIter->mValue.count, rtItemEle,MiniLog79);

	List<ReturnItemEle> rtList;
	rtList.insertTail(rtItemEle);
	//
		m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

		//������Ʒ����Ӧ��λ�ñ��Ϊ1

		List<UInt32>::Iter * buyIter = m_pPlayerConsortData->m_nBuyList.getn(buyindex);
		buyIter->mValue = 1;

		//����

		  PersistBusinessCatUpdate catupdate;
		   catupdate.m_nPlayerUid = playerUid;
		   catupdate.m_nRefreshTimes = m_pPlayerConsortData->m_nBusinessCatTimes;
			 catupdate.m_nBuyList = m_pPlayerConsortData->m_nBuyList;
		   PersistSystem::getSingleton().postThreadMsg(catupdate,  playerUid);

		//������Ϣ���ͻ���

		Json::Value root;
			GCBusinessCatBuyResp buyResp;
			buyResp.mPacketID = BOC_BUSINESSCATBUY_RESP;

			root["errorId"] = LynxErrno::None;
			root["index"] = buyindex;

			root["gold"] = m_pPlayer->getPlayerGold();
			root["coin"] = m_pPlayer->getPlayerCoin();
			root["curcontribute"] = m_pPlayerConsortData->m_nCurContribute;

			m_pPlayer->getAllItemManager().convertItemListToStr(rtList, root);


			Json::StyledWriter writer;
			buyResp.mRespJsonStr = writer.write(root);
			LOG_INFO("BUSINESS CAT BUY  IS:  %s", buyResp.mRespJsonStr.c_str());

			NetworkSystem::getSingleton().sendMsg(buyResp,connId);


}


void ConsortManager::updateGuidStr(std::string guidStr)
{
	m_pPlayer->mPlayerData.mBaseData.m_strGuid = guidStr;
	 PersistGuidStepUpdateMsg guidupdate;
	 guidupdate.playerUid = m_pPlayer->mPlayerData.mBaseData.m_nPlayerID;
		  guidupdate.guidStr = guidStr.c_str();
		   PersistSystem::getSingleton().postThreadMsg(guidupdate,  guidupdate.playerUid);
}

void ConsortManager::generateEyeQue()
{
	
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCEyequeResp eyequeResp;
		eyequeResp.mPacketID = BOC_EYEQUE_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		eyequeResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eyequeResp,connId);
		assert(false);
		return ;
	}
	
	ConsortLvTemplate *  consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
	if(m_pPlayerConsortData->m_nEyeSightTimes > consortLvTemp->mEyeSightTimes)
	{
		LOG_INFO("no use time eyesight!!!");
		Json::Value root;
		GCEyequeResp eyequeResp;
		eyequeResp.mPacketID = BOC_EYEQUE_RESP;

		root["errorId"] = LynxErrno::KitchentimesNotEnouf;

		Json::StyledWriter writer;
		eyequeResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eyequeResp,connId);
		assert(false);
		return ;
	}

	UInt64 playerUid = m_pPlayer->getPlayerGuid();



	UInt32 size = consortLvTemp->mEyeQueList.size();

	UInt32 index = rand()%size;

	UInt32 eyequeid = consortLvTemp->mEyeQueList.getn(index)->mValue;

	m_pPlayerConsortData->m_nEyeSightTimes++;
	
	m_nEyeGroup = eyequeid;

/////////////////////////////////////////////////////////////////////
	std::stringstream mystream;
	mystream.clear();
	mystream.str("");

	mystream << eyequeid;
	String groupStr = "";
	mystream >> groupStr;

	mystream.clear();
	mystream.str("");

	UInt64 nowtime = time(0);
	mystream << nowtime;
	String timeStr = "";
	mystream >> timeStr;
   
	if(playerUid%2)
	{
		m_strCode = timeStr+groupStr;
	}
	else
	{
		m_strCode = groupStr + timeStr;
	}
	
	/////////////////////////////////////////////

	//����
	PersistEyeTimesUpdate eyetimesupdate;
	eyetimesupdate.m_nEyeTimes = m_pPlayerConsortData->m_nEyeSightTimes;
	eyetimesupdate.m_nPlayeruid = playerUid;
	 PersistSystem::getSingleton().postThreadMsg(eyetimesupdate,  playerUid);

		Json::Value root;
		GCEyequeResp eyequeResp;
		eyequeResp.mPacketID = BOC_EYEQUE_RESP;

		root["errorId"] = LynxErrno::None;
		root["nowtime"] = nowtime;
		root["eyequegroup"] = eyequeid;
		root["eyetimes"] = m_pPlayerConsortData->m_nEyeSightTimes;

		Json::StyledWriter writer;
		eyequeResp.mRespJsonStr = writer.write(root);
		LOG_INFO("eyequeresp is %s", eyequeResp.mRespJsonStr.c_str() );
		NetworkSystem::getSingleton().sendMsg(eyequeResp,connId);
		
		return ;

}

void ConsortManager::getEyeAward(String  codeStr, UInt32 score)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCEyeawardResp eyeawardResp;
		eyeawardResp.mPacketID = BOC_EYEAWARD_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		eyeawardResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eyeawardResp,connId);
		assert(false);
		return ;
	}

	if(	m_strCode != codeStr)
	{
		LOG_INFO("code wrong !!!");
		Json::Value root;
		GCEyeawardResp eyeawardResp;
		eyeawardResp.mPacketID = BOC_EYEAWARD_RESP;

		root["errorId"] = LynxErrno::Md5Wrong;

		Json::StyledWriter writer;
		eyeawardResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eyeawardResp,connId);
		
		return ;
	}

	
	EyeQuestionTemplate * eyequeTemp = EYEQUESTION_TABLE().get(m_nEyeGroup);
	UInt32 awardID = 0;
	switch(score)
	{
	case 1:
		awardID = eyequeTemp->mAward1;
		break;
	case 2:
		awardID = eyequeTemp->mAward2;
		break;
	case 3:
		awardID = eyequeTemp->mAward3;
		break;
	case 4:
		awardID = eyequeTemp->mAward4;
		break;
	case 5:
		awardID = eyequeTemp->mAward5;
		break;
	}

	List<Goods> awardList;
	List<Goods> tmpItemList;

		GiftManager::getSingleton().getAwardByID(awardID,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		List<ReturnItemEle> rtItemList;

		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,0);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,0);
				rtItemList.insertTail(rtIemEle);
			}

			
		}
		
		Json::Value root;
		 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root);
		
		 root["errorId"] = LynxErrno::None;
	
		
		GCEyeawardResp eyeawardResp;
		eyeawardResp.mPacketID = BOC_EYEAWARD_RESP;

		Json::StyledWriter writer;
		eyeawardResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eyeawardResp,connId);

}


void ConsortManager::ticketJoin(UInt32 ticketId, UInt32 sceneIndex,UInt64 servantId)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketJoinResp ticketJoinResp;
		ticketJoinResp.mPacketID = BOC_TICKETJOIN_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		ticketJoinResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketJoinResp,connId);
	
		return ;
	}

	UInt64 endTime = ConsortSystem::getSingleton().getAwardActiveTime();
	UInt64 curTime = time(0);
	if(curTime  > endTime || curTime < endTime - 3600)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketJoinResp ticketJoinResp;
		ticketJoinResp.mPacketID = BOC_TICKETJOIN_RESP;

		root["errorId"] = LynxErrno::PlayerTimeError;

		Json::StyledWriter writer;
		ticketJoinResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketJoinResp,connId);
		
		return ;
	}

	ConsortLvTemplate* consortlvtemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
	UInt32 limit = 0;
	if(endTime/3600 < 20)
	{
			limit = consortlvtemp->mfriendlyTimes;
	}
	else
	{
		limit = consortlvtemp->mfriendlyTimes * 2;
	}

	if(m_pPlayerConsortData->m_nTicketTimes >= limit)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketJoinResp ticketJoinResp;
		ticketJoinResp.mPacketID = BOC_TICKETJOIN_RESP;

		root["errorId"] = LynxErrno::TicketJoinTimes;

		Json::StyledWriter writer;
		ticketJoinResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketJoinResp,connId);
		
		return ;
	}

	TicketFriendTemplate* friendTemp = TICKETFRIEND_TABLE().get(ticketId);
	UInt32 joincount = 0;
	
	if(sceneIndex == 0)
	{
			joincount = friendTemp->mJoinCount1;
	}

	if(sceneIndex == 1)
	{
			joincount = friendTemp->mJoinCount2;
	}

	if(sceneIndex == 2)
	{
			joincount = friendTemp->mJoinCount2;
	}

	Map<UInt32, TicketData*> ::Iter * findIter = pConsortData->m_ticketDataMap.find(ticketId);
	
	UInt32 nowjoincount = 0;
	for(List<TicketFriend>::Iter * ticketFriend = findIter->mValue->m_ticketFriends.begin(); 
		ticketFriend != NULL;  ticketFriend = findIter->mValue->m_ticketFriends.next(ticketFriend) )
	{
		if(ticketFriend->mValue.m_nOrderIndex == sceneIndex)
		{
			nowjoincount ++;
		}
	}

	if(nowjoincount  >=  joincount)
	{
	
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketJoinResp ticketJoinResp;
		ticketJoinResp.mPacketID = BOC_TICKETJOIN_RESP;

		root["errorId"] = LynxErrno::ScenePeapleFull;
		convertTicketDataToJson(root["ticketdata"], ticketId);
		root["tickettimes"] = m_pPlayerConsortData->m_nTicketTimes;

		Json::StyledWriter writer;
		ticketJoinResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketJoinResp,connId);
		return;
	}


	m_pPlayerConsortData->m_nTicketTimes++;
	//�淨��������
	 PersistTicketTimesUpdate  timesupdate;
	 timesupdate.m_nTimes = m_pPlayerConsortData->m_nTicketTimes;
	 timesupdate.m_nPlayeruid =  m_pPlayer->getPlayerGuid();

	 PersistSystem::getSingleton().postThreadMsg(timesupdate,  timesupdate.m_nPlayeruid);


	//�������
	TicketFriend ticketFriend;
	ticketFriend.m_nPlayeruid = m_pPlayer->getPlayerGuid();
	ticketFriend.m_nServantid = servantId;
	ticketFriend.m_nOrderIndex = sceneIndex;

	
	findIter->mValue->m_ticketFriends.insertTail(ticketFriend);
	//����Ʊ����Ϣ����
	
	ConsortSystem::getSingleton().saveTicketFriends(consortId, ticketId);
	
	UInt32 quality = findIter->mValue->m_nQuality;

	

	List<UInt64>::Iter * awardIdIter = friendTemp->mAwardList.getn(quality);
	UInt32 count = findIter->mValue->m_ticketFriends.size();

	//�Լ���������
	TicketAward ticketAward;
	ticketAward.m_nTicketId = ticketId;
	ticketAward.m_nPeapleCnt = count;
	ticketAward.m_nAwardId = awardIdIter->mValue;
	ticketAward.m_nActiveTime = endTime;
	
	List<TicketAward>::Iter * insertIter = m_pTicketAwardList->insertTail(ticketAward);
	m_ticketAwardMap.insert( ticketId,&(insertIter->mValue));

	//�Լ���������
	 PersistTicketAwardAdd  persistAwardAdd;
	 persistAwardAdd.m_nTicketId = ticketId;
	 persistAwardAdd.m_nAwardId = awardIdIter->mValue;
	 persistAwardAdd.m_nPeapleCnt = count;
	 persistAwardAdd.m_nEndTime = ticketAward.m_nActiveTime;
	 persistAwardAdd.m_nPlayeruid = m_pPlayer->getPlayerGuid();
	
	 PersistSystem::getSingleton().postThreadMsg(persistAwardAdd,  persistAwardAdd.m_nPlayeruid);

	 //���˽�������
	 for(List<TicketFriend>::Iter * ticketfriendIter = findIter->mValue->m_ticketFriends.begin(); 
		 ticketfriendIter != NULL; ticketfriendIter = findIter->mValue->m_ticketFriends.next(ticketfriendIter) )
	 {
		 if(ticketfriendIter->mValue.m_nPlayeruid ==persistAwardAdd.m_nPlayeruid)
		 {
			continue;
		 }
	
		 Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(ticketfriendIter->mValue.m_nPlayeruid);
		 if(otherPlayer)
		 {	 
			 Map<UInt32, TicketAward *> * pAwardMap = otherPlayer->getConsortManager().getAwardMap();
             if(!pAwardMap)
			 {
				continue;
			 }

			 Map<UInt32, TicketAward *>::Iter * awardIter =  pAwardMap->find(ticketId);
			 if(!awardIter)
			 {
				continue;
			 }
			 awardIter->mValue->m_nPeapleCnt = count;
		 }
		
			 //�����˴���
			
		  PersistTicketAwardUpdate  persistAwardupdate;
			 persistAwardupdate.m_nTicketId = ticketId;
			persistAwardupdate.m_nAwardId = ticketAward.m_nAwardId;
			persistAwardupdate.m_nPeapleCnt = count;
			persistAwardupdate.m_nEndTime = ticketAward.m_nActiveTime;
			 persistAwardupdate.m_nPlayeruid = ticketfriendIter->mValue.m_nPlayeruid;

			  PersistSystem::getSingleton().postThreadMsg(persistAwardupdate,  persistAwardupdate.m_nPlayeruid);

	 }

		Json::Value root;
		GCTicketJoinResp ticketJoinResp;
		ticketJoinResp.mPacketID = BOC_TICKETJOIN_RESP;

		root["errorId"] = LynxErrno::None;
		convertTicketDataToJson(root["ticketdata"], ticketId);
		root["tickettimes"] = m_pPlayerConsortData->m_nTicketTimes;
		Json::StyledWriter writer;
		ticketJoinResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketJoinResp,connId);
	
		
	
}

void ConsortManager::ticketGetAward()
{
	  	const ConnId & connId = m_pPlayer->getConnId();	
	List<ReturnItemEle> rtItemList;
	for(List<TicketAward>::Iter * awardIter = m_pTicketAwardList->begin(); awardIter != NULL;
		awardIter = m_pTicketAwardList->next(awardIter) )
	{
		float overridef = getAwardOverride(awardIter->mValue.m_nTicketId, awardIter->mValue.m_nPeapleCnt);
		getTicketAwardList(rtItemList,  awardIter->mValue.m_nAwardId, overridef);
	}

	m_ticketAwardMap.clear();
	m_pTicketAwardList->clear();

	//ɾ�����˽����б����
	PersistTicketAwardDel awardDel;
	awardDel.m_nPlayeruid = m_pPlayer->getPlayerGuid();
	 PersistSystem::getSingleton().postThreadMsg(awardDel,  awardDel.m_nPlayeruid);

	 //�ذ�

	 Json::Value root;
	 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root["contant"]);
		
	 root["errorId"] = LynxErrno::None;
	
		
		GCTicketAwardResp ticketAwardResp;
		ticketAwardResp.mPacketID = BOC_TICKETAWARD_RESP;

		Json::StyledWriter writer;
		ticketAwardResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketAwardResp,connId);

}

void ConsortManager::getTicketAwardList(List<ReturnItemEle> &rtItemList, UInt64 awardID, float overrider)
{
	List<Goods> awardList;
	List<Goods> tmpItemList;

		GiftManager::getSingleton().getAwardByID(awardID,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,0);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,0);
				rtItemList.insertTail(rtIemEle);
			}

			
		}

}

void ConsortManager::getTicketData()
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 endTime = ConsortSystem::getSingleton().getAwardActiveTime();
	UInt64 curTime = time(0);
	if(curTime  > endTime || curTime < endTime - 3600)
	{
		LOG_INFO("time error !!!");
		Json::Value root;
		GCTicketDataResp ticketDataResp;
		ticketDataResp.mPacketID = BOC_TICKETDATA_RESP;

		root["errorId"] = LynxErrno::PlayerTimeError;

		Json::StyledWriter writer;
		ticketDataResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketDataResp,connId);
		
		return ;
	}

	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketDataResp ticketDataResp;
		ticketDataResp.mPacketID = BOC_TICKETDATA_RESP;

		root["errorId"] = LynxErrno::ConsortNotExist;

		Json::StyledWriter writer;
		ticketDataResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketDataResp,connId);
	
		return ;
	}



	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["curtime"] = curTime;
	root["endtime"] = endTime;

	for(List<TicketData>::Iter * ticketDataIter = pConsortData->m_ticketList.begin();  ticketDataIter != NULL;
		ticketDataIter = pConsortData->m_ticketList.next(ticketDataIter) )
	{
		Json::Value ticketEleRoot;
		convertTicketDataToJson(ticketEleRoot,ticketDataIter->mValue.m_nTicketId);
		root["ticketlist"].append(ticketEleRoot);
		
	}
  
		GCTicketDataResp ticketDataResp;
		ticketDataResp.mPacketID = BOC_TICKETDATA_RESP;

		

		Json::StyledWriter writer;
		ticketDataResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketDataResp,connId);



}


float ConsortManager::getAwardOverride(UInt32 ticketId, UInt32 peaplecount)
{
	TicketFriendTemplate* ticketFriendTemp = TICKETFRIEND_TABLE().get(ticketId);
	if(ticketFriendTemp->mPeapleCount3 && peaplecount >= ticketFriendTemp->mPeapleCount3)
	{
		return 2.0;
	}
	else if(ticketFriendTemp->mPeapleCount2 && peaplecount >= ticketFriendTemp->mPeapleCount2)
	{
		return 1.5;
	}
	else if(ticketFriendTemp->mPeapleCount1 && peaplecount >= ticketFriendTemp->mPeapleCount1)
	{
		return (float)1.2;
	}

	return 1.0;
}

void ConsortManager::ticketSupport(UInt32 ticketId, UInt32 grade)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt32 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	
	if(!pConsortData)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketSupportResp ticketSupport;
		ticketSupport.mPacketID = BOC_TICKETSUPPORT_RESP;

		root["errorId"] = LynxErrno::ClienServerDataNotMatch;

		Json::StyledWriter writer;
		ticketSupport.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketSupport,connId);
	
		return ;
	}

	UInt64 endTime = ConsortSystem::getSingleton().getAwardActiveTime();
	UInt64 curTime = time(0);
	if(curTime  > endTime || curTime < endTime - 3600)
	{
		LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketSupportResp ticketSupport;
		ticketSupport.mPacketID = BOC_TICKETSUPPORT_RESP;

		root["errorId"] = LynxErrno::PlayerTimeError;

		Json::StyledWriter writer;
		ticketSupport.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketSupport,connId);
		
		return ;
	}

	Map<UInt32, TicketData*> ::Iter * findIter = pConsortData->m_ticketDataMap.find(ticketId);
		
		TicketFriendTemplate* ticketFriendTemp = TICKETFRIEND_TABLE().get(ticketId);
		UInt32 curGrade = findIter->mValue->m_nQuality;

		if(curGrade >=  grade)
		{
			LOG_INFO("Consortid not found!!!");
		Json::Value root;
		GCTicketSupportResp ticketSupport;
		ticketSupport.mPacketID = BOC_TICKETSUPPORT_RESP;

		root["errorId"] = LynxErrno::TicketGradeError;

		Json::StyledWriter writer;
		ticketSupport.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketSupport,connId);
		
		return ;
		}


		UInt32 curCost = 0;
		if(curGrade)
		{
			List<UInt32>::Iter * curCostIter = ticketFriendTemp->mSupportList.getn(curGrade - 1);
			curCost = curCostIter->mValue;
		}
		
		List<UInt32>::Iter * willCostIter = ticketFriendTemp->mSupportList.getn(grade - 1);
		UInt32 costGold = willCostIter->mValue - curCost;

		UInt64 gold = m_pPlayer->getPlayerGold();
		if(gold < costGold)
		{
			LOG_INFO("Consortid not found!!!");
			Json::Value root;
			GCTicketSupportResp ticketSupport;
			ticketSupport.mPacketID = BOC_TICKETSUPPORT_RESP;

			root["errorId"] = LynxErrno::RmbNotEnough;

			Json::StyledWriter writer;
			ticketSupport.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(ticketSupport,connId);
	
			return ;
		}


		Goods goods;
		List<Goods> itemList;

		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - costGold;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog80);

		findIter->mValue->m_nQuality = grade;
		
		
		GivenData givenData;
		givenData.m_strName = m_pPlayer->getPlayerName().c_str();
		givenData.m_nGrade = grade;

		findIter->mValue->m_givenList.insertTail(givenData);

		//�������
		ConsortSystem::getSingleton().saveTicketGivens(consortId, ticketId);

		//�����ұ������

		List<UInt64>::Iter * awardIdIter = ticketFriendTemp->mAwardList.getn(grade);
		 //Ʒ�����������仯
	 for(List<TicketFriend>::Iter * ticketfriendIter = findIter->mValue->m_ticketFriends.begin(); 
		 ticketfriendIter != NULL; ticketfriendIter = findIter->mValue->m_ticketFriends.next(ticketfriendIter) )
	 {
		
		 Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(ticketfriendIter->mValue.m_nPlayeruid);
		 if(otherPlayer)
		 {	 
			 Map<UInt32, TicketAward *> * pAwardMap = otherPlayer->getConsortManager().getAwardMap();
			 Map<UInt32, TicketAward *>::Iter * awardIter =  pAwardMap->find(ticketId);

			 awardIter->mValue->m_nAwardId = awardIdIter->mValue;

		 }
		
		 UInt64 endTime = ConsortSystem::getSingleton().getAwardActiveTime();
			 //�����˴���
			
		  PersistTicketAwardUpdate  persistAwardupdate;
			 persistAwardupdate.m_nTicketId = ticketId;
			 persistAwardupdate.m_nAwardId = awardIdIter->mValue;
			 persistAwardupdate.m_nPeapleCnt = findIter->mValue->m_ticketFriends.size();
			persistAwardupdate.m_nEndTime = endTime;
			 persistAwardupdate.m_nPlayeruid = ticketfriendIter->mValue.m_nPlayeruid;

			  PersistSystem::getSingleton().postThreadMsg(persistAwardupdate,  persistAwardupdate.m_nPlayeruid);

	 }
	
		Json::Value root;
		GCTicketSupportResp ticketSupport;
		ticketSupport.mPacketID = BOC_TICKETSUPPORT_RESP;

		root["errorId"] = LynxErrno::None;
		convertTicketDataToJson(root["ticketdata"], ticketId);
		
		Json::StyledWriter writer; 

		std::string otherStr = writer.write(root);
		root["gold"] = m_pPlayer->getPlayerGold();
		std::string selfStr = writer.write(root);

			for(List<UInt64>::Iter * memberIter = pConsortData->m_nMemberList.begin(); 
				memberIter != NULL; memberIter = pConsortData->m_nMemberList.next(memberIter) )
			{
			
				Player * otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
				if(!otherPlayer)
				{
					continue;
				}

				const ConnId & otherconnId =  otherPlayer->getConnId();
				if(!otherconnId)
				{
					continue;
				}

				if(memberIter->mValue == m_pPlayer->getPlayerGuid())
				{
					  ticketSupport.mRespJsonStr = selfStr;
					  NetworkSystem::getSingleton().sendMsg(ticketSupport,connId);
				}
				else
				{
					ticketSupport.mRespJsonStr = otherStr;
					NetworkSystem::getSingleton().sendMsg(ticketSupport,otherconnId);
					
				}
				
				
				
			}

}

Map<UInt32, TicketAward *> * ConsortManager::getAwardMap()
{
	return &m_ticketAwardMap;
}

void ConsortManager::convertTicketDataToJson(Json::Value & root,  UInt32 ticketId)
{
		UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
		Map<UInt32, TicketData*> ::Iter * findIter = pConsortData->m_ticketDataMap.find(ticketId);
		root["ticketid"] = findIter->mValue->m_nTicketId ;
		root["quality"] = findIter->mValue->m_nQuality;
		
		for(List<TicketFriend>::Iter * ticketFriendIter = findIter->mValue->m_ticketFriends.begin(); 
			ticketFriendIter != NULL;  ticketFriendIter = findIter->mValue->m_ticketFriends.next(ticketFriendIter))
		{
			Json::Value friendEle;
			friendEle["playeruid"] = ticketFriendIter->mValue.m_nPlayeruid;
			friendEle["servantid"] = ticketFriendIter->mValue.m_nServantid;
			friendEle["scene"] = ticketFriendIter->mValue.m_nOrderIndex;

			root["infolist"].append(friendEle);
			
		}

		for(List<GivenData>::Iter * givenIter = findIter->mValue->m_givenList.begin(); 
			givenIter != NULL;  givenIter = findIter->mValue->m_givenList.next(givenIter))
		{
			Json::Value givenEle;
			givenEle["name"] = givenIter->mValue.m_strName.c_str();
			givenEle["grade"] = givenIter->mValue.m_nGrade;
			root["givenlist"].append(givenEle);
			
		}


}

void ConsortManager::getTicketOneData(UInt32 ticketId)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	convertTicketDataToJson(root["ticketdata"],ticketId);	

	GCTicketOneDataResp ticketOneDataResp;
	ticketOneDataResp.mPacketID = BOC_TICKETONEDATA_RESP;

	Json::StyledWriter writer;
	ticketOneDataResp.mRespJsonStr = writer.write(root);

	NetworkSystem::getSingleton().sendMsg(ticketOneDataResp,connId);

}

void ConsortManager::beginEloquence()
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	ConsortLvTemplate *  consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);
	
	if(consortLvTemp->mEloquenTimes <= m_pPlayerConsortData->m_nEloquenceTimes)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::EloquenceTimes;
	
		GCEloquenceBeginResp eloquencebeginResp;
		eloquencebeginResp.mPacketID = BOC_ELOQUENCEBEGIN_RESP;

		Json::StyledWriter writer;
		eloquencebeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eloquencebeginResp,connId);
		return;
	}

	m_pPlayerConsortData->m_nEloquenceTimes++;

	//����

	PersistEloquenceTimesUpdate   eloquenceTimes;
	eloquenceTimes.times = m_pPlayerConsortData->m_nEloquenceTimes;
	eloquenceTimes.playeruid = m_pPlayer->getPlayerGuid();
	PersistSystem::getSingleton().postThreadMsg(eloquenceTimes, eloquenceTimes.playeruid);

	//�ذ����ͻ���
	
	    Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["eloquencetimes"] = m_pPlayerConsortData->m_nEloquenceTimes;

		GCEloquenceBeginResp eloquencebeginResp;
		eloquencebeginResp.mPacketID = BOC_ELOQUENCEBEGIN_RESP;

		Json::StyledWriter writer;
		eloquencebeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(eloquencebeginResp,connId);
		
	
}


void ConsortManager::getEloquenceAward(UInt32 win)
{
			const ConnId & connId = m_pPlayer->getConnId();	
			UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
			ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
			ConsortLvTemplate *  consortLvTemp = CONSORTLV_TABLE().get(pConsortData->m_nLv);

			UInt64 awardId = 0;
			if(win)
			{
				awardId = consortLvTemp->mEloquencewin;
			}
			else
			{
			   awardId = consortLvTemp->mEloquencelose;
			}
			
		
			List<Goods> awardList;
	        List<Goods> tmpItemList;

		GiftManager::getSingleton().getAwardByID(awardId,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		List<ReturnItemEle> rtItemList;
		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,0);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,0);
				rtItemList.insertTail(rtIemEle);
			}

			
		}


		Json::Value root;
		 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root);
		
		 root["errorId"] = LynxErrno::None;
	
		
		GCEloquenceAwardResp ticketAwardResp;
		ticketAwardResp.mPacketID = BOC_ELOQUENCEAWARD_RESP;

		Json::StyledWriter writer;
		ticketAwardResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(ticketAwardResp,connId);

}

void ConsortManager::getWoodData()
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 curtime = time(0);
	UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
	UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

	if(curtime < beginTime || curtime > endTime)
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodCatTimeError;
		 root["woodtype"] = 0;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}

		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 0;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);

}

void ConsortManager::convertWoodData(Json::Value &root)
{
	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	UInt32 count1 = pConsortData->m_listWoodPlayers1.size();
	UInt32 count2 = pConsortData->m_listWoodPlayers2.size();
	UInt32 count3 = pConsortData->m_listWoodPlayers3.size();
	UInt32 count4 = pConsortData->m_listWoodPlayers4.size();
	UInt32 count5 = pConsortData->m_listWoodPlayers5.size();

	root["woodcount"].append(count1);
	root["woodcount"].append(count2);
	root["woodcount"].append(count3);
	root["woodcount"].append(count4);
	root["woodcount"].append(count5);

	UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
	UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

	ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(pConsortData->m_nLv);
	
	root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
	root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
	root["curtime"] = (UInt64)time(0);
	root["endtime"] = endTime;
	root["gold"] = m_pPlayer->getPlayerGold();
	
	root["enhance"].append(pConsortData->m_nEnhance1); 
	root["enhance"].append(pConsortData->m_nEnhance2); 
	root["enhance"].append(pConsortData->m_nEnhance3); 


}

void ConsortManager::beginWoodCat(UInt32 woodIndex)
{
	
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 curtime = time(0);
	UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
	UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	UInt64 playeruid = m_pPlayer->getPlayerGuid();

	if(curtime < beginTime || curtime > endTime)
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodCatTimeError;
		 root["woodtype"] = 1;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}

	ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(pConsortData->m_nLv);
	if(m_pPlayerConsortData->m_nWoodCatTimes >=  consortwoodtemp->mTimes )
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodTimesLess;
	      root["woodtype"] = 1;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}


	if(pConsortData->m_nWoodBloodLeft >=  consortwoodtemp->mHp )
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodBloodLess;
	      root["woodtype"] = 1;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}



	if(woodIndex == 0)
	{
	
		if(consortwoodtemp->mLimit1 <= pConsortData->m_listWoodPlayers1.size())
		{
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::WoodPeapleFull;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
		}

		m_pPlayerConsortData->m_nWoodCatTimes++;
		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);

		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers1.find(playeruid, pConsortData->m_listWoodPlayers1.begin(), 
			pConsortData->m_listWoodPlayers1.end() );

		if(!findIter)
		{
			pConsortData->m_listWoodPlayers1.insertTail(playeruid);
		}
	
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);

		return;
	}

	if(woodIndex == 1)
	{
	
		if(consortwoodtemp->mLimit2 <= pConsortData->m_listWoodPlayers2.size())
		{
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::WoodPeapleFull;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
		}
	
		m_pPlayerConsortData->m_nWoodCatTimes++;
		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);


		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers2.find(playeruid, pConsortData->m_listWoodPlayers2.begin(), 
			pConsortData->m_listWoodPlayers2.end() );

		if(!findIter)
		{
			pConsortData->m_listWoodPlayers2.insertTail(playeruid);
		}

		   GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);

		return;

	}

	if(woodIndex == 2)
	{

		if(consortwoodtemp->mLimit3 <= pConsortData->m_listWoodPlayers3.size())
		{
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::WoodPeapleFull;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
		}

		m_pPlayerConsortData->m_nWoodCatTimes++;
		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);


		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers3.find(playeruid, pConsortData->m_listWoodPlayers3.begin(), 
			pConsortData->m_listWoodPlayers3.end() );

		if(!findIter)
		{
			pConsortData->m_listWoodPlayers3.insertTail(playeruid);
		}


			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
	}

	if(woodIndex == 3)
	{
		

		if(consortwoodtemp->mLimit4 <= pConsortData->m_listWoodPlayers4.size())
		{
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::WoodPeapleFull;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
		}

		m_pPlayerConsortData->m_nWoodCatTimes++;
		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);


		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers4.find(playeruid, pConsortData->m_listWoodPlayers4.begin(), 
			pConsortData->m_listWoodPlayers4.end() );

		if(!findIter)
		{
			pConsortData->m_listWoodPlayers4.insertTail(playeruid);
		}

			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
	}

	if(woodIndex == 4)
	{
		
		if(consortwoodtemp->mLimit5 <= pConsortData->m_listWoodPlayers5.size())
		{
			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::WoodPeapleFull;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
			return;
		}

		m_pPlayerConsortData->m_nWoodCatTimes++;
		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);


		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers5.find(playeruid, pConsortData->m_listWoodPlayers5.begin(), 
			pConsortData->m_listWoodPlayers5.end() );

		if(!findIter)
		{
			pConsortData->m_listWoodPlayers5.insertTail(playeruid);
		}

			GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 1;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);

			return;

	}



}

void ConsortManager::endWoodCat(UInt32 woodIndex, UInt32 res)
{
	const ConnId & connId = m_pPlayer->getConnId();	
	UInt64 curtime = time(0);
	UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
	UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	if(consortId == 0)
	{
		Json::Value root;
		root["errorId"] = LynxErrno::PlayerHasKicked;
		root["win"] = res;
		ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(1);
		
		UInt32 awardId = consortwoodtemp->mWinAward1;
		awardJson(root, awardId);

		Json::StyledWriter writer;
		GCWoodCatendResp catendResp;
		catendResp.mPacketID = BOC_WOODCATEND_RESP;
		catendResp.mRespJsonStr = writer.write(root);
		NetworkSystem::getSingleton().sendMsg(catendResp, connId);

		return;

	}


	ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
	UInt64 playeruid = m_pPlayer->getPlayerGuid();

	ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(pConsortData->m_nLv);


	if(woodIndex == 0)
	{
			List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers1.find(playeruid, pConsortData->m_listWoodPlayers1.begin(), 
			pConsortData->m_listWoodPlayers1.end() );
			if(findIter)
			{
				pConsortData->m_listWoodPlayers1.erase(findIter);
			}

			if(res ==2)
			{
					Json::Value root;
			
					root["errorId"] = LynxErrno::None;
			
					root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
					root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					root["curtime"] = (UInt64)time(0);
					UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
				    root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;

							Json::StyledWriter writer;
							 GCWoodCatendResp catendResp;
							 catendResp.mPacketID = BOC_WOODCATEND_RESP;
							catendResp.mRespJsonStr = writer.write(root);
							NetworkSystem::getSingleton().sendMsg(catendResp, connId);


				return;
			}

			UInt64 awardId = 0;
			UInt64 attack = 0;
			if(res)
			{
				awardId = consortwoodtemp->mWinAward1;
				attack = consortwoodtemp->mDelHp1;
			}
			else
			{
				awardId = consortwoodtemp->mLoseAward1;
			}

			if(attack)
			{
				pConsortData->m_nWoodBloodLeft += attack;
				
				if(pConsortData->m_nWoodBloodLeft > consortwoodtemp->mHp)
				{
					pConsortData->m_nWoodBloodLeft = consortwoodtemp->mHp;
				}

				//����
				PersistWoodTotalUpdate  woodtotalUpdate;
				woodtotalUpdate.consortid = consortId;
				woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
				woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
				woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
				woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
				PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);

			}

			
	

			//��ȡ����
			Json::Value root;
			
			root["errorId"] = LynxErrno::None;
			
			root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
			root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
				root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
						root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
						root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					   root["curtime"] = (UInt64)time(0);
					   UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
					   root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			 awardJson(root, awardId);

			Json::StyledWriter writer;
			 GCWoodCatendResp catendResp;
			 catendResp.mPacketID = BOC_WOODCATEND_RESP;
			 catendResp.mRespJsonStr = writer.write(root);
			 NetworkSystem::getSingleton().sendMsg(catendResp, connId);

			return;
	}

	if(woodIndex == 1)
	{
		
		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers2.find(playeruid, pConsortData->m_listWoodPlayers2.begin(), 
			pConsortData->m_listWoodPlayers2.end() );
			if(findIter)
			{
				pConsortData->m_listWoodPlayers2.erase(findIter);
			}

			if(res ==2)
			{
					Json::Value root;
			
					root["errorId"] = LynxErrno::None;
			
					root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
					root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					root["curtime"] = (UInt64)time(0);
					UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
				    root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;

							Json::StyledWriter writer;
							 GCWoodCatendResp catendResp;
							 catendResp.mPacketID = BOC_WOODCATEND_RESP;
							catendResp.mRespJsonStr = writer.write(root);
							NetworkSystem::getSingleton().sendMsg(catendResp, connId);


				return;
			}


			UInt64 awardId = 0;
			UInt64 attack = 0;
			if(res)
			{
				awardId = consortwoodtemp->mWinAward2;
				attack = consortwoodtemp->mDelHp2;
			}
			else
			{
				awardId = consortwoodtemp->mLoseAward2;
			}

			if(attack)
			{
				pConsortData->m_nWoodBloodLeft += attack;

				if(pConsortData->m_nWoodBloodLeft > consortwoodtemp->mHp)
				{
					pConsortData->m_nWoodBloodLeft = consortwoodtemp->mHp;
				}

				//����
				PersistWoodTotalUpdate  woodtotalUpdate;
				woodtotalUpdate.consortid = consortId;
				woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
				woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
				woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
				woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
				PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);

			}

			//��ȡ����
			Json::Value root;
			
			root["errorId"] = LynxErrno::None;
			
		root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
			root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
				root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
						root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
						root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					   root["curtime"] = (UInt64)time(0);
					   UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
					   root["endtime"] = endTime;

					    root["win"] = res;
					   root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						   root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			 awardJson(root, awardId);

			Json::StyledWriter writer;
			 GCWoodCatendResp catendResp;
			 catendResp.mPacketID = BOC_WOODCATEND_RESP;
			 catendResp.mRespJsonStr = writer.write(root);
			 NetworkSystem::getSingleton().sendMsg(catendResp, connId);

			return;
	}

	if(woodIndex == 2)
	{
			List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers3.find(playeruid, pConsortData->m_listWoodPlayers3.begin(), 
			pConsortData->m_listWoodPlayers3.end() );
			if(findIter)
			{
				pConsortData->m_listWoodPlayers3.erase(findIter);
			}

			if(res ==2)
			{
					Json::Value root;
			
					root["errorId"] = LynxErrno::None;
			
					root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
					root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					root["curtime"] = (UInt64)time(0);
					UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
				    root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;

							Json::StyledWriter writer;
							 GCWoodCatendResp catendResp;
							 catendResp.mPacketID = BOC_WOODCATEND_RESP;
							catendResp.mRespJsonStr = writer.write(root);
							NetworkSystem::getSingleton().sendMsg(catendResp, connId);


				return;
			}
		

			UInt64 awardId = 0;
			UInt64 attack = 0;
			if(res)
			{
				awardId = consortwoodtemp->mWinAward3;
				attack = consortwoodtemp->mDelHp3;
			}
			else
			{
				awardId = consortwoodtemp->mLoseAward3;
			}

			if(attack)
			{
				pConsortData->m_nWoodBloodLeft += attack;

				if(pConsortData->m_nWoodBloodLeft > consortwoodtemp->mHp)
				{
					pConsortData->m_nWoodBloodLeft = consortwoodtemp->mHp;
				}

				//����
				PersistWoodTotalUpdate  woodtotalUpdate;
				woodtotalUpdate.consortid = consortId;
				woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
				woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
				woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
				woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
				PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);

			}

			//��ȡ����
			Json::Value root;
			
			root["errorId"] = LynxErrno::None;
			
			root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
			root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
				root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
						root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
						root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					   root["curtime"] = (UInt64)time(0);
					   UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
					   root["endtime"] = endTime;
					    root["win"] = res;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						   root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			 awardJson(root, awardId);

			Json::StyledWriter writer;
			 GCWoodCatendResp catendResp;
			 catendResp.mPacketID = BOC_WOODCATEND_RESP;
			 catendResp.mRespJsonStr = writer.write(root);
			 NetworkSystem::getSingleton().sendMsg(catendResp, connId);

			 return;
	}

	if(woodIndex == 3)
	{
		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers4.find(playeruid, pConsortData->m_listWoodPlayers4.begin(), 
			pConsortData->m_listWoodPlayers4.end() );
			if(findIter)
			{
				pConsortData->m_listWoodPlayers4.erase(findIter);
			}

			if(res ==2)
			{
					Json::Value root;
			
					root["errorId"] = LynxErrno::None;
			
					root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
					root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					root["curtime"] = (UInt64)time(0);
					UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
				    root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;

							Json::StyledWriter writer;
							 GCWoodCatendResp catendResp;
							 catendResp.mPacketID = BOC_WOODCATEND_RESP;
							catendResp.mRespJsonStr = writer.write(root);
							NetworkSystem::getSingleton().sendMsg(catendResp, connId);


				return;
			}


			UInt64 awardId = 0;
			UInt64 attack = 0;
			if(res)
			{
				awardId = consortwoodtemp->mWinAward4;
				attack = consortwoodtemp->mDelHp4;
			}
			else
			{
				awardId = consortwoodtemp->mLoseAward4;
			}

			if(attack)
			{
				pConsortData->m_nWoodBloodLeft += attack;

				if(pConsortData->m_nWoodBloodLeft > consortwoodtemp->mHp)
				{
					pConsortData->m_nWoodBloodLeft = consortwoodtemp->mHp;
				}

				//����
				PersistWoodTotalUpdate  woodtotalUpdate;
				woodtotalUpdate.consortid = consortId;
				woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
				woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
				woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
				woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
				PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);

			}

			//��ȡ����
			Json::Value root;
			
			root["errorId"] = LynxErrno::None;
			
			root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
			root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
				root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
						root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
						root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					   root["curtime"] = (UInt64)time(0);
					   UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
					   root["endtime"] = endTime;
					    root["win"] = res;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						   root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			 awardJson(root, awardId);

			Json::StyledWriter writer;
			 GCWoodCatendResp catendResp;
			 catendResp.mPacketID = BOC_WOODCATEND_RESP;
			 catendResp.mRespJsonStr = writer.write(root);
			 NetworkSystem::getSingleton().sendMsg(catendResp, connId);

			 return;
	}

	if(woodIndex == 4)
	{
		List<UInt64>::Iter * findIter = pConsortData->m_listWoodPlayers5.find(playeruid, pConsortData->m_listWoodPlayers5.begin(), 
			pConsortData->m_listWoodPlayers5.end() );
			if(findIter)
			{
				pConsortData->m_listWoodPlayers5.erase(findIter);
			}

			if(res ==2)
			{
					Json::Value root;
			
					root["errorId"] = LynxErrno::None;
			
					root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
					root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					root["curtime"] = (UInt64)time(0);
					UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
				    root["endtime"] = endTime;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						  root["win"] = res;
						  root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;

							Json::StyledWriter writer;
							 GCWoodCatendResp catendResp;
							 catendResp.mPacketID = BOC_WOODCATEND_RESP;
							catendResp.mRespJsonStr = writer.write(root);
							NetworkSystem::getSingleton().sendMsg(catendResp, connId);


				return;
			}

			UInt64 awardId = 0;
			UInt64 attack = 0;
			if(res)
			{
				awardId = consortwoodtemp->mWinAward5;
				attack = consortwoodtemp->mDelHp5;
			}
			else
			{
				awardId = consortwoodtemp->mLoseAward5;
			}

			if(attack)
			{
				pConsortData->m_nWoodBloodLeft += attack;

				if(pConsortData->m_nWoodBloodLeft > consortwoodtemp->mHp)
				{
					pConsortData->m_nWoodBloodLeft = consortwoodtemp->mHp;
				}
				//����
				PersistWoodTotalUpdate  woodtotalUpdate;
				woodtotalUpdate.consortid = consortId;
				woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
				woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
				woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
				woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
				PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);

			}

			//��ȡ����
			Json::Value root;
			
			root["errorId"] = LynxErrno::None;
			
			root["woodcount"].append(pConsortData->m_listWoodPlayers1.size());
			root["woodcount"].append(pConsortData->m_listWoodPlayers2.size());
				root["woodcount"].append(pConsortData->m_listWoodPlayers3.size());
					root["woodcount"].append(pConsortData->m_listWoodPlayers4.size());
						root["woodcount"].append(pConsortData->m_listWoodPlayers5.size());
						root["woodblood"] = consortwoodtemp->mHp - pConsortData->m_nWoodBloodLeft;
					   root["curtime"] = (UInt64)time(0);
					   UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();
					   root["endtime"] = endTime;
					    root["win"] = res;
						  root["enhance"].append(pConsortData->m_nEnhance1);
						 root["enhance"].append(pConsortData->m_nEnhance2);
						  root["enhance"].append(pConsortData->m_nEnhance3);
						   root["woodtimes"] = m_pPlayerConsortData->m_nWoodCatTimes;
			 awardJson(root, awardId);

			Json::StyledWriter writer;
			 GCWoodCatendResp catendResp;
			 catendResp.mPacketID = BOC_WOODCATEND_RESP;
			 catendResp.mRespJsonStr = writer.write(root);
			 NetworkSystem::getSingleton().sendMsg(catendResp, connId);

			 return;
	}
}

void ConsortManager::awardJson(Json::Value & root,  UInt64 awardId)
{
		
	    List<Goods> awardList;
	    List<Goods> tmpItemList;

		GiftManager::getSingleton().getAwardByID(awardId,1,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,awardList);
		}

		List<ReturnItemEle> rtItemList;
		for(List<Goods>::Iter * awardIter = awardList.begin();  awardIter != NULL;  awardIter = awardList.next(awardIter) )
		{
			if(awardIter->mValue.resourcestype == AWARD_SERVANT || awardIter->mValue.resourcestype == AWARD_JEWELRY)
			{
				for(UInt32 i = 0; i < awardIter->mValue.num; i++)
				{
						ReturnItemEle rtIemEle;
						m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype, 1 , rtIemEle,0);
						rtItemList.insertTail(rtIemEle);
				}
			}
			else
			{
				ReturnItemEle rtIemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardIter->mValue.resourcestype,  awardIter->mValue.subtype,  awardIter->mValue.num,rtIemEle,0);
				rtItemList.insertTail(rtIemEle);
			}

			
		}


	
		 m_pPlayer->getAllItemManager().convertItemListToStr(rtItemList, root);
		
		
}

void ConsortManager::enhanceWood(UInt32 type)
{
		const ConnId & connId = m_pPlayer->getConnId();	
		UInt64 curtime = time(0);
		UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
		UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

		UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
		if(consortId == 0)
		{

			return;
		}


		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
		UInt64 playeruid = m_pPlayer->getPlayerGuid();

	ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(pConsortData->m_nLv);
		if(pConsortData->m_nWoodBloodLeft >=  consortwoodtemp->mHp )
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodBloodLess;
	      root["woodtype"] = 2;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}

	if(curtime < beginTime || curtime > endTime)
	{
		GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::WoodCatTimeError;
		 root["woodtype"] = 2;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
	}

		GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	   UInt32 costGold = 0;
		if(type == 0)
		{
			costGold = globalValue.uWoodencatattackbuffcost;
			if(pConsortData->m_nEnhance1 == 1)
			{
				GCWoodCatbeginResp catbeginResp;
				catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

				Json::Value root;	
				root["errorId"] = LynxErrno::WoodBuffExist;
				 root["woodtype"] = 2;
				convertWoodData(root);
				Json::StyledWriter writer;

				catbeginResp.mRespJsonStr = writer.write(root);

				NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
				return ;
			}
		}

		if(type == 1)
		{
		   costGold =  globalValue.uWoodencatdefensebuffcost;

		   if(pConsortData->m_nEnhance2 == 1)
			{
				GCWoodCatbeginResp catbeginResp;
				catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

				Json::Value root;	
				root["errorId"] = LynxErrno::WoodBuffExist;
				 root["woodtype"] = 2;
				convertWoodData(root);
				Json::StyledWriter writer;

				catbeginResp.mRespJsonStr = writer.write(root);

				NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
				return ;
			}
		}

		if(type == 2)
		{
			 costGold =  globalValue.uWoodencatCritbuffcost;
			 if(pConsortData->m_nEnhance3 == 1)
			{
				GCWoodCatbeginResp catbeginResp;
				catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

				Json::Value root;	
				root["errorId"] = LynxErrno::WoodBuffExist;
				 root["woodtype"] = 2;
				convertWoodData(root);
				Json::StyledWriter writer;

				catbeginResp.mRespJsonStr = writer.write(root);

				NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
				return ;
			}
		}

		UInt64 curGold = m_pPlayer->getPlayerGold();
		if(curGold < costGold)
		{
			GCWoodCatbeginResp catbeginResp;
		catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;

		 Json::Value root;	
		 root["errorId"] = LynxErrno::RmbNotEnough;
		 root["woodtype"] = 2;
		convertWoodData(root);
		Json::StyledWriter writer;

		catbeginResp.mRespJsonStr = writer.write(root);

		NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
		return;
		}


		if(type == 0)
		{
			pConsortData->m_nEnhance1 = 1;
		}

		if(type == 1)
		{
			pConsortData->m_nEnhance2 = 1;
		}

		if(type == 2)
		{
			pConsortData->m_nEnhance3 = 1;
		}


		 Goods goods;
		 List<Goods> itemList;

		 goods.resourcestype =AWARD_BASE;
		 goods.subtype = AWARD_BASE_GOLD;
		 goods.num = 0 - costGold;
		 itemList.insertTail(goods);
		 GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog82);


		PersistWoodTotalUpdate  woodtotalUpdate;
		woodtotalUpdate.consortid = consortId;
		woodtotalUpdate.enhance1 = pConsortData->m_nEnhance1;
		woodtotalUpdate.enhance2 = pConsortData->m_nEnhance2;
		woodtotalUpdate.enhance3 = pConsortData->m_nEnhance3;
		woodtotalUpdate.woodleft = pConsortData->m_nWoodBloodLeft;
		PersistSystem::getSingleton().postThreadMsg(woodtotalUpdate, playeruid);


		GCWoodCatbeginResp catbeginResp;
			catbeginResp.mPacketID = BOC_WOODCATBEGIN_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			 root["woodtype"] = 2;
			convertWoodData(root);
			Json::StyledWriter writer;

			catbeginResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(catbeginResp,connId);
}

void ConsortManager::woodTotalAward()
{
		const ConnId & connId = m_pPlayer->getConnId();	
		UInt64 curtime = time(0);
		UInt64 beginTime = ConsortSystem::getSingleton().getWoodBeginTime();
		UInt64 endTime = ConsortSystem::getSingleton().getWoodEndTime();

		UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
		UInt64 playeruid = m_pPlayer->getPlayerGuid();

		m_pPlayerConsortData->m_nWoodCatFlag = 1;
	
		ConsortWoodTemplate * consortwoodtemp = CONSORTWOOD_TABLE().get(pConsortData->m_nLv);

		PersistWoodSelfUpdate selfUpdate;
		selfUpdate.playeruid = playeruid;
		selfUpdate.times = m_pPlayerConsortData->m_nWoodCatTimes;
		selfUpdate.awardflag = m_pPlayerConsortData->m_nWoodCatFlag;
		PersistSystem::getSingleton().postThreadMsg(selfUpdate, playeruid);


	
			GCWoodCatAwardResp awardResp;
			awardResp.mPacketID = BOC_WOODTOTALAWARD_RESP;
			Json::Value root;	
			root["errorId"] = LynxErrno::None;
			root["woodaward"] = 1;
			convertWoodData(root);
			awardJson(root, consortwoodtemp->mTargetAward);
			Json::StyledWriter writer;

			awardResp.mRespJsonStr = writer.write(root);

			NetworkSystem::getSingleton().sendMsg(awardResp,connId);
}

void ConsortManager::clearWoodBattle()
{
	UInt64 consortId = m_pPlayerConsortData->m_nConsortId;
	if(!consortId)
	{
		return ;
	}
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
		UInt64 playeruid = m_pPlayer->getPlayerGuid();	
		if(!pConsortData)
		{
			return;
		}
	List<UInt64>::Iter * findIter1 = pConsortData->m_listWoodPlayers1.find(playeruid, pConsortData->m_listWoodPlayers1.begin(), 
			pConsortData->m_listWoodPlayers1.end() );
			if(findIter1)
			{
				pConsortData->m_listWoodPlayers1.erase(findIter1);
			}

			List<UInt64>::Iter * findIter2 = pConsortData->m_listWoodPlayers2.find(playeruid, pConsortData->m_listWoodPlayers2.begin(), 
			pConsortData->m_listWoodPlayers2.end() );
			if(findIter2)
			{
				pConsortData->m_listWoodPlayers2.erase(findIter2);
			}

			List<UInt64>::Iter * findIter3 = pConsortData->m_listWoodPlayers3.find(playeruid, pConsortData->m_listWoodPlayers3.begin(), 
			pConsortData->m_listWoodPlayers3.end() );
			if(findIter3)
			{
				pConsortData->m_listWoodPlayers3.erase(findIter3);
			}

			List<UInt64>::Iter * findIter4 = pConsortData->m_listWoodPlayers4.find(playeruid, pConsortData->m_listWoodPlayers4.begin(), 
			pConsortData->m_listWoodPlayers4.end() );
			if(findIter4)
			{
				pConsortData->m_listWoodPlayers4.erase(findIter4);
			}

			List<UInt64>::Iter * findIter5 = pConsortData->m_listWoodPlayers5.find(playeruid, pConsortData->m_listWoodPlayers5.begin(), 
			pConsortData->m_listWoodPlayers5.end() );
			if(findIter5)
			{
				pConsortData->m_listWoodPlayers5.erase(findIter5);
			}
}

void ConsortManager::clearTicketAward()
{
	m_pTicketAwardList->clear();
}