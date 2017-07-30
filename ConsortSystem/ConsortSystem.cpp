#include "ConsortSystem.h"


#include "../GameServer.h"
#include "../LogicSystem.h"
#include "../FriendBlackManager.h"
using namespace Lynx;

ConsortSystem::ConsortSystem(){}

ConsortSystem::~ConsortSystem()
{}


bool ConsortSystem::initial()
{
	m_nLastSyncTime = 0;
	m_nLastRefreshTime = 0;
	LOG_INFO("Initial RankSystem success!!!!");
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

	char resetsql[1024] = {0};
    snprintf(resetsql,sizeof(resetsql),"call activetime_load()");
    LOG_DEBUG("Sql:%s", resetsql);
	bool resetres = mDBInterface.execSql(resetsql);
	MYSQL_RES * storeresRes = mDBInterface.storeResult();
	if(!storeresRes)
	{
		LOG_WARN("Load activetime_load  failed!");

		mDBInterface.freeResult(&storeresRes);
		return false;
	}

	MYSQL_ROW resetrow = mDBInterface.fetchRow(storeresRes);
	if(!resetrow)
	{
		mDBInterface.freeResult(&storeresRes);
// 		return false;
	}
	else
	{
		m_nActiveResetTime = lynxAtoi<UInt64>(resetrow[0]);
		m_nBusinessCatResetTime = lynxAtoi<UInt64>(resetrow[1]);
		m_nEyeSightResetTime = lynxAtoi<UInt64>(resetrow[2]);
		m_nTicketFriendTime = lynxAtoi<UInt64>(resetrow[3]);
		m_nTicketTimesTime = lynxAtoi<UInt64>(resetrow[4]);
		m_nLoyalTime = lynxAtoi<UInt64>(resetrow[5]);
		m_nKitchenTime = lynxAtoi<UInt64>(resetrow[6]);
	    m_nEloquenceTime = lynxAtoi<UInt64>(resetrow[7]);
		m_nWoodCatTime = lynxAtoi<UInt64>(resetrow[8]);

		calculateTicketActive((time_t)m_nTicketFriendTime);
		calculateWoodBegin((time_t)m_nWoodCatTime);
		mDBInterface.freeResult(&storeresRes);

	}

	

	LOG_INFO("Initial consortresettime success!!!!");

	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call consortrank_load()");

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		LOG_WARN("Load RankConsort  failed!");

		mDBInterface.freeResult(&storeRes);
		return false;

	}

	std::stringstream mystream;
	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
	while(row)
	{
		ConsortData consortData;
		consortData.m_nId = lynxAtoi<UInt64>(row[0]);
		consortData.m_nLv = lynxAtoi<UInt32>(row[1]);
		consortData.m_strName = row[2];
		consortData.m_nCount = lynxAtoi<UInt32>(row[3]);
		consortData.m_nPower = lynxAtoi<UInt64>(row[4]);
		consortData.m_strDesc = row[5];
		consortData.m_nRes = lynxAtoi<UInt32>(row[6]);
		consortData.m_nLeader = lynxAtoi<UInt64>(row[7]);
	    consortData.m_nExp  = lynxAtoi<UInt32>(row[8]);
		consortData.m_nCheck  = lynxAtoi<UInt32>(row[9]);
		consortData.m_nPowerLimit  = lynxAtoi<UInt64>(row[10]);
		consortData.m_nActive = lynxAtoi<UInt32>(row[11]);
		consortData.m_nSignLv = lynxAtoi<UInt32>(row[12]);
		consortData.m_nBusinessLv = lynxAtoi<UInt32>(row[13]);
		consortData.m_nWoodBloodLeft = lynxAtoi<UInt32>(row[14]);
		consortData.m_nEnhance1 = lynxAtoi<UInt32>(row[15]);
		consortData.m_nEnhance2 = lynxAtoi<UInt32>(row[16]);
		consortData.m_nEnhance3 = lynxAtoi<UInt32>(row[17]);

		List<ConsortData >::Iter * insertIter = m_consortList.insertTail(consortData);
		m_consortMap.insert(consortData.m_nId, &(insertIter->mValue));
		row = mDBInterface.fetchRow(storeRes);

	}


	mDBInterface.freeResult(&storeRes);

	LOG_INFO("Load ConsortRank successful!");


	for(List<ConsortData >::Iter * consortIter =  m_consortList.begin();  consortIter != m_consortList.end(); 
		consortIter = m_consortList.next(consortIter))
	{
		UInt64 &consortId = consortIter->mValue.m_nId;
		
		char sql[8192] = {0};
		snprintf(sql,sizeof(sql),"call consortmember_load(%llu)", consortId);

		//LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* storeRes = mDBInterface.storeResult();
		if(!storeRes)
		{
			LOG_WARN("Load consort %llu  member_load  failed!", consortId);

			mDBInterface.freeResult(&storeRes);
			return false;

		}

		MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
		while(row)
		{
			UInt64 playerUid = lynxAtoi<UInt64>(row[0]);
			consortIter->mValue.m_nMemberList.insertTail(playerUid);
			row = mDBInterface.fetchRow(storeRes);
		}

		mDBInterface.freeResult(&storeRes);
		//LOG_INFO("Load consort %llu member_load successful!", consortId);
	}

	
	for(List<ConsortData >::Iter * consortIter =  m_consortList.begin();  consortIter != m_consortList.end(); 
	consortIter = m_consortList.next(consortIter))
	{
		UInt64 &consortId = consortIter->mValue.m_nId;
		
		char sql[8192] = {0};

		snprintf(sql,sizeof(sql),"call consortapply_load(%llu)", consortId);

		//LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* storeRes = mDBInterface.storeResult();
		if(!storeRes)
		{
			LOG_WARN("Load consort %llu  consortapply_load  failed!", consortId);

			mDBInterface.freeResult(&storeRes);
			return false;

		}

		MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
		while(row)
		{
			ApplyData applyData;

			applyData.m_nApply = lynxAtoi<UInt64>(row[1]);
			applyData.m_nTime = lynxAtoi<UInt64>(row[2]);

			consortIter->mValue.m_applyList.insertTail(applyData);
			row = mDBInterface.fetchRow(storeRes);
		}

		mDBInterface.freeResult(&storeRes);
		//LOG_INFO("Load consort %llu apply successful!", consortId);
	}
	

	//������־ϵͳ
	for(List<ConsortData >::Iter * consortIter =  m_consortList.begin();  consortIter != m_consortList.end(); 
	consortIter = m_consortList.next(consortIter))
	{
		UInt64 &consortId = consortIter->mValue.m_nId;
		
		char sql[8192] = {0};

		snprintf(sql,sizeof(sql),"call consortlog_load(%llu)", consortId);

		LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* storeRes = mDBInterface.storeResult();
		if(!storeRes)
		{
			LOG_WARN("Load consort %llu  consortapply_load  failed!", consortId);

			mDBInterface.freeResult(&storeRes);
			return false;

		}

		MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
		while(row)
		{
			LogData logData;
			logData.m_nTempId = lynxAtoi<UInt32>(row[1]);
			logData.m_nConsortId = lynxAtoi<UInt64>(row[2]);
			logData.m_nPlayerUid = lynxAtoi<UInt64>(row[3]);
			logData.m_strPlayername = row[4];
			logData.m_strParam1 = row[5];
			logData.m_strParam2 = row[6];
			logData.m_nTime =  lynxAtoi<UInt64>(row[7]);


			consortIter->mValue.m_logList.insertTail(logData);
			row = mDBInterface.fetchRow(storeRes);
		}

		mDBInterface.freeResult(&storeRes);
		//LOG_INFO("Load consort %llu apply successful!", consortId);
	}
	

	///////////////////////////////////////////////
	char playerconsortallsql[2048] = {0};
	snprintf(playerconsortallsql, sizeof(playerconsortallsql), "call playerconsortall_load()");
	LOG_DEBUG("sql:%s", playerconsortallsql);

	bool plconsortallresult = mDBInterface.execSql(playerconsortallsql);
	if(!plconsortallresult)
	{
		LOG_WARN("load player consort  failed");
		return false;
	}

	MYSQL_RES* plconsortallrs = mDBInterface.storeResult();
	if(!plconsortallrs)
	{
		// ִ��ʧ��
		mDBInterface.freeResult(&plconsortallrs);
		return true ;		
	}

	MYSQL_ROW plconsortallrow = mDBInterface.fetchRow(plconsortallrs);
	
	while(plconsortallrow)
	{
		//select uuid, uid, name , power, viplevel, level ,leaveTime
		ConsortInfoData consortInfoData;
		consortInfoData.playerUid = lynxAtoi<UInt64>(plconsortallrow[0]);
		consortInfoData.consortId = lynxAtoi<UInt64>(plconsortallrow[1]);
		consortInfoData.curContribute = lynxAtoi<UInt32>(plconsortallrow[2]);
		consortInfoData.totalContribute = lynxAtoi<UInt32>(plconsortallrow[3]);
		consortInfoData.consortJob = lynxAtoi<UInt32>(plconsortallrow[4]);
		
	
		LogicSystem::getSingleton().updateConsortInfo(consortInfoData.playerUid,consortInfoData);
	
		plconsortallrow = mDBInterface.fetchRow(plconsortallrs);
	
	}
	
	mDBInterface.freeResult(&plconsortallrs); 



	/////////////////////////////////////////////////
	for(List<ConsortData >::Iter * consortIter =  m_consortList.begin();  consortIter != m_consortList.end(); 
	consortIter = m_consortList.next(consortIter))
	{
		UInt64 &consortId = consortIter->mValue.m_nId;
		
		char sql[8192] = {0};

		snprintf(sql,sizeof(sql)," select * from ticketfriend where  consortuid = %llu", consortId);

		//LOG_DEBUG("Sql:%s", sql);

		bool result = mDBInterface.execSql(sql);

		MYSQL_RES* storeRes = mDBInterface.storeResult();
		if(!storeRes)
		{
			LOG_WARN("Load consort %llu  ticketfriend  failed!", consortId);

			mDBInterface.freeResult(&storeRes);
			return false;

		}

		MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
		while(row)
		{
			TicketData ticketData;
			ticketData.m_nTicketId = lynxAtoi<UInt64>(row[1]);
			ticketData.m_nQuality = lynxAtoi<UInt64>(row[4]);
			
			convertTicketStr2List(&ticketData.m_ticketFriends ,string(row[2]) );
			convertGivenStr2List(&ticketData.m_givenList ,string(row[5]));

			List<TicketData>::Iter * insertIter = consortIter->mValue.m_ticketList.insertTail(ticketData);
			consortIter->mValue.m_ticketDataMap.insert(ticketData.m_nTicketId, &insertIter->mValue);

			row = mDBInterface.fetchRow(storeRes);
		}

		mDBInterface.freeResult(&storeRes);
		
	}
	

	//////////////////////////////////////////////////

	return true;

	
}

void ConsortSystem::release()
{
	m_consortMap.clear();
	m_consortList.clear();
	m_consortRankList.clear();
	m_consortRankMap.clear();
	LOG_INFO("Release RankSystem");
	
}

void ConsortSystem::updateConsortRankList()
{
	m_consortRankList.clear();
	m_consortRankMap.clear();
	char sql[8192] = {0};
	snprintf(sql,sizeof(sql),"call consortrank_load()");

	LOG_DEBUG("Sql:%s", sql);

	bool result = mDBInterface.execSql(sql);

	MYSQL_RES* storeRes = mDBInterface.storeResult();
	if(!storeRes)
	{
		LOG_WARN("Load RankConsort  failed!");

		mDBInterface.freeResult(&storeRes);
		return ;

	}

	std::stringstream mystream;
	MYSQL_ROW row = mDBInterface.fetchRow(storeRes);
	
	int index = 0;
	while(row)
	{
		index++;
		ConsortRankData consortRankData;
		consortRankData.m_nId = lynxAtoi<UInt64>(row[0]);
		
		consortRankData.m_nPower = lynxAtoi<UInt64>(row[4]);
		consortRankData.m_nRank = index;
		
		List<ConsortRankData >::Iter * listIter = m_consortRankList.insertTail(consortRankData);
		m_consortRankMap.insert(consortRankData.m_nId, &(listIter->mValue));
		row = mDBInterface.fetchRow(storeRes);

	}


	mDBInterface.freeResult(&storeRes);

	LOG_INFO("Load ConsortRank successful!");
}

void 
ConsortSystem::update(const UInt64& accTime)
{
   bool flag = false;
	//60000ms ����
	if(accTime - m_nLastSyncTime >= 60000 *2)
	{
	
		updateConsortRankList();
		//�ϴ�ͬ��ʱ�����
		m_nLastSyncTime = accTime;
		
	}

	if(accTime - m_nLastRefreshTime >=1000 )
	{	
			resetRefresh(accTime/1000);
			resetRefreshHalfDay(accTime/1000);
			resetticketfriend(accTime/1000);
			m_nLastRefreshTime = accTime;
	}
	
	
 
}

ConsortData * ConsortSystem::getConsortDataById(UInt64 consortId)
{
	Map<UInt64, ConsortData *>::Iter * findIter = m_consortMap.find(consortId);
	if(findIter == m_consortMap.end())
	{
		return NULL;
	}

	return findIter->mValue;
}

void ConsortSystem::destroyConsort(UInt64 consortId)
{
	Map<UInt64, ConsortData *>::Iter * mapIter = m_consortMap.find(consortId);
	if(mapIter != m_consortMap.end() )
	{
		m_consortMap.erase(mapIter);
	}


	List<ConsortData >::Iter * listIter = m_consortList.begin();
	for(  ; listIter != m_consortList.end();
		listIter = m_consortList.next(listIter))
	{
		if(listIter->mValue.m_nId == consortId)
		{
			break;
		}
	}

	
	if(listIter != m_consortList.end())
	{
		m_consortList.erase(listIter);
	}

	


}


ConsortRankData * ConsortSystem::getConsortRankData(UInt64 consortId)
{
	Map<UInt64, ConsortRankData *>::Iter * findIter = m_consortRankMap.find(consortId);
	if(findIter == m_consortRankMap.end())
	{
		return NULL;
	}

	return findIter->mValue;
}


const List<ConsortRankData > &ConsortSystem:: getConsortRankList()
{
	return m_consortRankList;
}


Map<UInt64, ConsortData *>  *ConsortSystem::getConsortMap(void)
{
	return & m_consortMap;
}

const List<ConsortData >& ConsortSystem::getConsortList()
{
	return m_consortList;
}


bool ConsortSystem::judgeSameNameConsort(std::string name)
{
	String consortname = name.c_str();
	for(List<ConsortData >::Iter * consortIter = m_consortList.begin(); consortIter != m_consortList.end();
		consortIter = m_consortList.next(consortIter))
	{
		if(consortIter->mValue.m_strName == consortname)
		{
			return true;
		}
	}
		
	return false;
}

UInt32 ConsortSystem::getConsortLastRank()
{
	return m_consortList.size();
}

void ConsortSystem::sendConsortMsg(UInt32 tempId, UInt64 consortId, UInt64 playerUid, std::string  playerName,  std::string param1,
			std::string param2, UInt64 logTime)
{
	Map<UInt64, ConsortData *>::Iter * findIter = m_consortMap.find(consortId);
	if(findIter == m_consortMap.end())
	{
		return ;
	}

	UInt32 modelid = 0;
	UInt32 consortjob = 0;
	Player * self = LogicSystem::getSingleton().getPlayerByGuid(playerUid);
	if(self)
	{
			modelid = self->getPlayerModelID();
			PlayerConsortData *  playerconsortdata = self->getConsortManager().getPlayerConsortData();
			consortjob = playerconsortdata->m_nConsortJob;
	}

	
	GCConsortMsgNotify msgNotify;

	Json::Value root;
	root["tempid"] = tempId;
	root["consortId"] = consortId;
	root["playerUid"] = playerUid;
	root["playerName"] = playerName;
	root["param1"] = param1;
	root["param2"] = param2;
	root["logtime"]= logTime;
	root["modelid"]= modelid;
	root["consortjob"]  = consortjob;

	Json::StyledWriter writer;
	msgNotify.mRespJsonStr =  writer.write(root);
	msgNotify.mPacketID = BOC_CONSORTMSG_NOTIFY;

	for(List<UInt64>::Iter * memberIter = findIter->mValue->m_nMemberList.begin();
		memberIter != findIter->mValue->m_nMemberList.end(); memberIter = findIter->mValue->m_nMemberList.next(memberIter))
	{
		Player * player = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
		if(!player)
		{
			continue;
		}
		
		const ConnId& connId = player->getConnId();

		if(!connId)
		{
			continue;
		}
		
		NetworkSystem::getSingleton().sendMsg(msgNotify,connId);
		
	}

	

}

bool ConsortSystem::eraseApplyFromCheckList(UInt64 consortId, UInt64 applyUid)
{
	Map<UInt64, ConsortData *>::Iter * findIter = m_consortMap.find(consortId);
	if(findIter == m_consortMap.end())
	{
		return false;
	}

	ConsortData * pConsortData = findIter->mValue;

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

		//�����Ƴ���ҵ�����
		pConsortData->m_applyList.erase(applyIter);
		
		return true;
}

ConsortData * ConsortSystem::createConsort(std::string name, Player *  player)
{
	//���¾�����Ϣ
	Guid consortGuid = LogicSystem::getSingleton().generateConsortGuid();
	ConsortData consortData;
	consortData.m_nId = consortGuid;
	consortData.m_nLv = 1;
	consortData.m_strName = name.c_str();
	consortData.m_nCount = 1;
	consortData.m_nPower = player->getPlayerPower();
	consortData.m_strDesc = "";
    consortData.m_nRes =0; 
	consortData.m_nLeader = player->getPlayerGuid();
	consortData.m_nExp = 0;
	consortData.m_nCheck = 0;
	consortData.m_nPowerLimit = POWERLIMIT;
	consortData.m_applyList.clear();
	consortData.m_logList.clear();
	consortData.m_nMemberList.insertTail(consortData.m_nLeader);
	consortData.m_nSignLv = consortData.m_nLv;
	consortData.m_nBusinessLv = consortData.m_nLv;
	List<ConsortData >::Iter * insertIter = m_consortList.insertTail(consortData);
	m_consortMap.insert(consortData.m_nId, &(insertIter->mValue));
	
	
	//������ҵľ����ֶ�
	player->getConsortManager().setPlayerConsortInfo(consortGuid, 1);

	//���Ų����
	PersistConsortCreate consortCreate;
	consortCreate.m_nConsortId = consortGuid;
	consortCreate.m_nConsortLv = 1;
	consortCreate.m_strConsortName = name.c_str();
	consortCreate.m_nCount = 1;
	consortCreate.m_nPower = player->getPlayerPower();
	consortCreate.m_strDescs =  "";
	consortCreate.m_nRes =  0;
	consortCreate.m_nExp = 0;
	consortCreate.m_nCheck = 0;
	consortCreate.m_nPowerLimit = POWERLIMIT;
	consortCreate.m_nLeader = consortData.m_nLeader;

	PersistSystem::getSingleton().postThreadMsg(consortCreate, consortCreate.m_nLeader);

	PlayerConsortData playerconsortData = player->getPlayerConsortData();
		

	//������Ҿ�����Ϣ��
	PersistPlayerConsortSave  playerConsortSave;
	playerConsortSave.m_nPlayerUid = consortCreate.m_nLeader;
	playerConsortSave.m_nConsortId =  consortGuid;
	playerConsortSave.m_nCurContribute = 0;
	playerConsortSave.m_nTotalContribute = 0;
	playerConsortSave.m_nConsortJob = 1;
	playerConsortSave.m_nLeaveTime = playerconsortData.m_nLeaveTime;

	PersistSystem::getSingleton().postThreadMsg(playerConsortSave, playerConsortSave.m_nPlayerUid);

	//�������ǩ�����ݴ���
	

	return &(insertIter->mValue);
}

////curTimeΪ�뼶��
//void ConsortSystem::resetRefresh(UInt64 curTime)
//{
//		bool isToday = TimeManager::timeIsOneDay(curTime, m_nActiveResetTime);
//
//		//ͬһ�첻��Ҫ���ã�ֱ�ӷ���
//		if(isToday)
//		{
//
//			return;
//		}
//
//		//�ж��Ƿ������㣬���Ϊ���ù����Ծ��ʱ�̣����������Ծ�ȶ�Ӧ�ĵȼ�ͬ��Ϊ����ʵ�ʵȼ�
//		UInt64 hours =  TimeUtil::convertSecToTodayHour(curTime);
//		//���
//		if(hours >= 0)
//		{
//			for(List<ConsortData >::Iter * consortIter = m_consortList.begin();  consortIter != NULL; 
//				consortIter = m_consortList.next(consortIter))
//			{
//				//��Ծ������Ϊ0������ǩ���ȼ�����Ϊ����ĸõȼ�
//				consortIter->mValue.m_nSignLv = consortIter->mValue.m_nLv;
//				consortIter->mValue.m_nActive = 0;
//				
//				//���칫��ͬ�������߳�Ա��Ծ����������ݰ�
//				 GCConsortSignSyncNotify  signSyncNotify;
//				 signSyncNotify.mPacketID = BOC_CONSORTSIGNSYNC_NOTIFY;
//				 Json::Value signNotifyRoot;
//				 signNotifyRoot["consortid"] = consortIter->mValue.m_nId;
//				 signNotifyRoot["consortlv"] = consortIter->mValue.m_nLv;
//				 signNotifyRoot["consortexp"] = consortIter->mValue.m_nExp;
//				 signNotifyRoot["consortactive"] = 0;
//				 signNotifyRoot["signlv"] = consortIter->mValue.m_nLv;
//				 for(UInt32 i = 0; i < 4; i++)
//				 {
//					 signNotifyRoot["signawards"].append(0);
//				 }
//
//				signNotifyRoot["sign"] = 0;
//           
//				//ˢ�´�������
//				signNotifyRoot["cattaskrefresh"] = 0;
//				signNotifyRoot["kitchentimes"] = 0;
//
//				Json::StyledWriter notifyWriter;
//
//				std::string jsonstr = notifyWriter.write(signNotifyRoot);
//				//LOG_INFO("consortsign notify is: %s ", jsonstr.c_str() );	
//				signSyncNotify.mRespJsonStr = jsonstr;
//
//				//�������ڴ����ǩ����ص��ڴ�����
//				for(List<UInt64>::Iter * memberIter = consortIter->mValue.m_nMemberList.begin(); memberIter != NULL;
//					memberIter = consortIter->mValue.m_nMemberList.next(memberIter))
//				{
//					//���ǩ�����ݴ��̣� 
//					PersistConsortSignUpdate consortSignUpdate;
//					consortSignUpdate.m_nPlayerUid = memberIter->mValue;
//					consortSignUpdate.m_nSign = 0;
//					for(int i = 0; i < 4; i++)
//					{
//							consortSignUpdate.m_nSignAwards.insertTail(UInt32(0));
//					}
//				
//					PersistSystem::getSingleton().postThreadMsg(consortSignUpdate, consortSignUpdate.m_nPlayerUid);
//
//					Player * player = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
//					//����֪ͨ���������
//					if(player)
//					{
//						//�޸�����ڴ�����
//						PlayerConsortData * playerConsortData = player->getConsortManager().getPlayerConsortData();
//						playerConsortData->m_nSign = 0;
//						for(List<UInt32>::Iter * signIter = playerConsortData->m_nSignAwards.begin(); signIter != NULL;
//							signIter =  playerConsortData->m_nSignAwards.next(signIter))
//						{
//							signIter->mValue = 0;
//						}
//						
//							////////////////////////////////////////////////////
//							//�������������������ˢ��
//						ConsortLvTemplate * consortLvTemp = CONSORTLV_TABLE().get(consortIter->mValue.m_nLv);
//						if(!consortLvTemp)
//						{
//							consortLvTemp = CONSORTLV_TABLE().get(10);
//						}
//						//����ˢ�´���
//						playerConsortData->m_nRefreshTimes  = 0;
//						
//						//������ҳ�����Ϸ����������Ϊ0
//						playerConsortData->m_nKitchenTimes  = 0;
//					
//						////////////////////////////////////////////////////
//					
//						//�ж�����Ƿ����ߣ����߷���ˢ��֪ͨ
//						const ConnId & memCon = player->getConnId();
//						if(memCon)
//						{
//							NetworkSystem::getSingleton().sendMsg(signSyncNotify,memCon);
//						}
//						
//
//					}
//				}
//
//			}
//			
//		
//
//
//			//�����ڴ��е�ǩ������ʱ�䡢��������ʱ��
//			m_nActiveResetTime = curTime;
//
//			//������Ϣ���̣�����resettime���ݿ��ʱ���
//			//��������������� �� ��ҹ�������������Ϣ�����������
//			PersistConsortSignReset consortSignReset;
//			consortSignReset.m_nResetTime = curTime;
//	
//
//			PersistSystem::getSingleton().postThreadMsg(consortSignReset, curTime);
//
//
//		}
//
//
//}

//curTimeΪ�뼶��
void ConsortSystem::resetRefresh(UInt64 curTime)
{
		bool todaySign = TimeManager::timeIsOneDay(curTime, m_nActiveResetTime);

		bool todayEyeSight = TimeManager::timeIsOneDay(curTime, m_nEyeSightResetTime);

		bool todayTicketTimes = TimeManager::timeIsOneDay(curTime, m_nTicketTimesTime);

		bool todayLoyalTimes = TimeManager::timeIsOneDay(curTime, m_nLoyalTime);

		bool todayKitchenTimes = TimeManager::timeIsOneDay(curTime, m_nKitchenTime);

		bool todayEloquenceTimes = TimeManager::timeIsOneDay(curTime, m_nEloquenceTime);

		bool todayWoodTimes = TimeManager::timeIsOneDay(curTime, m_nWoodCatTime);

		 if(todaySign && todayEyeSight&& todayTicketTimes&& todayLoyalTimes&& todayKitchenTimes&&todayEloquenceTimes&&todayWoodTimes)
		 {
				return;
		 }

		 GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
		 //��ǰʱ������ת��ΪСʱ
		UInt64 hours =  TimeUtil::convertSecToTodayHour(curTime);
		UInt32 flag = 0;
		if(!todaySign && hours >= globalValue.uConsortSignReset)
		{
			
				//�������û�Ծ�ȴ���
				PersistConsortSignReset timereset;
				timereset.m_nResetTime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);
				//����ʱ���
				m_nActiveResetTime = curTime;
				flag = 1;
		}

		 if(!todayTicketTimes && hours >= globalValue.uTicketFriendReset)
		 {
				
				//��������Ʊ�Ѽ������
				PersistTicketTimesReset timereset;
				timereset.resettime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);

				//����ʱ���
				m_nTicketTimesTime = curTime;
				flag = 1;
		 }

		if(!todayEyeSight && hours >= globalValue.uEyesightReset)
		{
				
				//��������ѵ������
				PersistEyeTimesReset timereset;
				timereset.resettime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);


				//����ʱ���
				m_nEyeSightResetTime = curTime;
				flag = 1;
		}

		if(!todayKitchenTimes && hours >= globalValue.uKitchenReset)
		{
				//���ͳ���ѵ������

				PersistKitchenTimesReset timereset;
				timereset.resettime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);

				//����ʱ���
				m_nKitchenTime = curTime;
				flag = 1;
		}
		
		if(!todayLoyalTimes && hours >= globalValue.uXingXiaReset)
		{
				//���������������
				PersistLoyalTimesReset timereset;
				timereset.resettime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);

				//����ʱ���
				m_nLoyalTime = curTime;
				flag = 1;
		}
		
		if(!todayEloquenceTimes && hours >= globalValue.uEloquenceReset)
		{
			m_nEloquenceTime = curTime;
            //�ڲ�ѵ������
			PersistEloquenceTimesReset  timereset;
				timereset.resettime = curTime;
				PersistSystem::getSingleton().postThreadMsg(timereset, 0);
				flag = 1;
		}
		
		if(!todayWoodTimes && hours >= globalValue.uWoodencatReset)
		{
			m_nWoodCatTime = curTime;
            //ľè��ѵ������
			PersistWoodTimesReset  woodreset;
			woodreset.resettime = curTime;
			PersistSystem::getSingleton().postThreadMsg(woodreset, 0);

			calculateWoodBegin((UInt64)curTime);

			PersistWoodTotalReset woodtotalReset;
			PersistSystem::getSingleton().postThreadMsg(woodtotalReset, 0);
			flag = 1;

		}

		//û���κι����淨������ͬ��ʱ��
		if(flag == 0)
		{
			return;
		}

		//�޸�����ڴ����ݺͻָ����ݰ����ͻ���

			for(List<ConsortData >::Iter * consortIter = m_consortList.begin();  consortIter != NULL; 
				consortIter = m_consortList.next(consortIter))
			{
				
				//���칫��ͬ�������߳�Ա��Ծ����������ݰ�
				 GCConsortSignSyncNotify  signSyncNotify;
				 signSyncNotify.mPacketID = BOC_CONSORTSIGNSYNC_NOTIFY;
				 Json::Value signNotifyRoot;
				 signNotifyRoot["consortid"] = consortIter->mValue.m_nId;
				 signNotifyRoot["consortlv"] = consortIter->mValue.m_nLv;
				 signNotifyRoot["consortexp"] = consortIter->mValue.m_nExp;
				 
				 if(!todaySign && hours >= globalValue.uConsortSignReset)
				 {
					 signNotifyRoot["consortactive"] = 0;
					 signNotifyRoot["signlv"] = consortIter->mValue.m_nLv;

					  for(UInt32 i = 0; i < 4; i++)
					 {
					    signNotifyRoot["signawards"].append(0);
				     }

					  signNotifyRoot["sign"] = 0;


					  //��Ծ������Ϊ0������ǩ���ȼ�����Ϊ����ĸõȼ�
						consortIter->mValue.m_nSignLv = consortIter->mValue.m_nLv;
						consortIter->mValue.m_nActive = 0;
				 }

				 //��������
				 if(!todayLoyalTimes && hours >= globalValue.uXingXiaReset)
				 {
						signNotifyRoot["cattaskrefresh"] = 0;
				 }
				
				 if(!todayKitchenTimes && hours >= globalValue.uKitchenReset)
				 {
						signNotifyRoot["kitchentimes"] = 0;
				 }
				
				 if(!todayEyeSight && hours >= globalValue.uEyesightReset)
				 {
						signNotifyRoot["eyetimes"] = 0;
				 }
           
				 if(!todayTicketTimes && hours >= globalValue.uTicketFriendReset)
				 {
						signNotifyRoot["tickettimes"] = 0;
				 }

				 if(!todayEloquenceTimes && hours >= globalValue.uEloquenceReset)
				{
						//����Ʊ�Ѽ������
						signNotifyRoot["eloquencetimes"] = 0;
				 }

				if(!todayWoodTimes && hours >= globalValue.uWoodencatReset)
				{
						
						signNotifyRoot["woodtimes"] = 0;
						signNotifyRoot["woodaward"] = 0;
					
						consortIter->mValue.m_nWoodBloodLeft = 0;
						consortIter->mValue.m_nEnhance1 = 0;
						consortIter->mValue.m_nEnhance2 = 0;
						consortIter->mValue.m_nEnhance3 = 0;
				}



				Json::StyledWriter notifyWriter;

				std::string jsonstr = notifyWriter.write(signNotifyRoot);
				LOG_INFO("consortsign notify is: %s ", jsonstr.c_str() );	
				signSyncNotify.mRespJsonStr = jsonstr;

				//�������ڴ����ǩ����ص��ڴ�����
				for(List<UInt64>::Iter * memberIter = consortIter->mValue.m_nMemberList.begin(); memberIter != NULL;
					memberIter = consortIter->mValue.m_nMemberList.next(memberIter))
				{
				
					Player * player = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
					//����֪ͨ���������
					if(player)
					{
						//�޸�����ڴ�����
						PlayerConsortData * playerConsortData = player->getConsortManager().getPlayerConsortData();


						if(!todaySign && hours >= globalValue.uConsortSignReset)
						{
								//����ǩ��
							    playerConsortData->m_nSign = 0;
								for(List<UInt32>::Iter * signIter = playerConsortData->m_nSignAwards.begin(); signIter != NULL;
									signIter =  playerConsortData->m_nSignAwards.next(signIter))
								{
									signIter->mValue = 0;
								}
						}

						 //��������
						 if(!todayLoyalTimes && hours >= globalValue.uXingXiaReset)
						 {
							 	//����ˢ�´���
									playerConsortData->m_nRefreshTimes  = 0;
						}
				
						if(!todayKitchenTimes && hours >= globalValue.uKitchenReset)
						{
										//������ҳ�����Ϸ����������Ϊ0
								playerConsortData->m_nKitchenTimes  = 0;
						 }
				
				 	  if(!todayEyeSight && hours >= globalValue.uEyesightReset)
				      {
						  //��������ѵ������
						  playerConsortData->m_nEyeSightTimes  = 0;
						  
				      }
           
					 if(!todayTicketTimes && hours >= globalValue.uTicketFriendReset)
					{
						//����Ʊ�Ѽ������
							playerConsortData->m_nTicketTimes = 0;
					 }

					 if(!todayEloquenceTimes && hours >= globalValue.uEloquenceReset)
					{
						//����Ʊ�Ѽ������
							playerConsortData->m_nEloquenceTimes = 0;
					 }

					if(!todayWoodTimes && hours >= globalValue.uWoodencatReset)
					{
						//ľè������
						playerConsortData->m_nWoodCatTimes = 0;
						playerConsortData->m_nWoodCatFlag = 0;
						
					}

					

						//�ж�����Ƿ����ߣ����߷���ˢ��֪ͨ
						const ConnId & memCon = player->getConnId();
						if(memCon)
						{
							NetworkSystem::getSingleton().sendMsg(signSyncNotify,memCon);
						}
						

					}
				}

		}
	


}



void ConsortSystem::resetRefreshHalfDay(UInt64 curTime)
{
	bool isToday = TimeManager::timeIsOneDay(curTime, m_nBusinessCatResetTime);

	//����ͬһ��
	if(!isToday)
	{
		//����ͬһ����������ݿ���ڴ棬��֪ͨ���
		m_nBusinessCatResetTime = curTime;
		PersistBusinessCatTimeReset resetmsg;
		resetmsg.m_nResetTime = curTime;

		PersistSystem::getSingleton().postThreadMsg(resetmsg, curTime);
		//��������ڴ棬��������Ϣ
		memNotifyHalfDay();

		return;
	}

	//��ͬһ�죬�ж��Ƿ���ͬһ������֮��
	UInt64 hours =  TimeUtil::convertSecToTodayHour(curTime);
	UInt64 resethours = TimeUtil::convertSecToTodayHour(m_nBusinessCatResetTime);

	if(hours < 12)
	{
		//�����жϹ���ͬһ�죬���ҵ�ǰʱ�䴦�ڵ����ǰ���죬����Ҫ���£�ֱ��return

		return;
	}

	if(resethours < 12)
	{
		//�ϴδ���ʱ�䴦�ڵ����ǰ���죬���ڵ�ʱ�䴦�ڵ���ĺ����
		//�������ݿ���ڴ棬֪ͨ���

		//����ͬһ����������ݿ���ڴ棬��֪ͨ���
		m_nBusinessCatResetTime = curTime;
		PersistBusinessCatTimeReset resetmsg;
		resetmsg.m_nResetTime = curTime;

		PersistSystem::getSingleton().postThreadMsg(resetmsg, curTime);
		//��������ڴ棬��������Ϣ
		memNotifyHalfDay();

	}

}

//Ʊ�Ѽ���ʱ��Ϊ12.~13. 20.~21.
//�˺���������ʱ�����(ǰһ��)�����á�
//������ͬһ�죬�ж�����ʱ��͵�ǰʱ�䲻��һ����Ϸʱ�����
//����
void ConsortSystem::resetticketfriend(UInt64 curTime)
{
	bool isToday = TimeManager::timeIsOneDay(curTime, m_nTicketFriendTime);
	
	//����ͬһ��
	if(!isToday)
	{
		
		//����������⡣
		generateTicketQuesions();
		ticketRefresh();
		//���й����������ʼ������

		//�����̡�
	
		//����ͬһ����������ݿ���ڴ棬
		m_nTicketFriendTime = curTime;
		calculateTicketActive((time_t)m_nTicketFriendTime);
		PersistTicketTimeReset  resetmsg;
		resetmsg.m_nResetTime = curTime;

		PersistSystem::getSingleton().postThreadMsg(resetmsg, curTime);
	

		return;
	}

	//��ͬһ�죬�ж��Ƿ���һ����Ϸʱ���
	UInt64 hours =  TimeUtil::convertSecToTodayHour(curTime);
	UInt64 resethours = TimeUtil::convertSecToTodayHour(m_nTicketFriendTime);

	//�ϴ�ͬ��ʱ������ڶ���13ʱ��ǰ������Ҫˢ����⡣
	//��ʱʱ��С��13Сʱ����ô�ϴ�����ʱ��һ��С��13Сʱ
	if(hours < 13)
	{
		if(m_ticketfriendMap.size() == 0)
		{
			generateTicketQuesions();
			calculateTicketActive((time_t)m_nTicketFriendTime);
		}
		
		return;
	}

	//�ϴ�����ʱ��С��13Сʱ������ʱ�����13Сʱ
	//��Ҫ���ã�����ϴ�����ʱ�����13Сʱ������Ҫ���á�
	if(resethours < 13)
	{
		
		//����������⡣

		//�����̡�
	
		generateTicketQuesions();
		ticketRefresh();

		//����ͬһ����������ݿ���ڴ棬
		m_nTicketFriendTime = curTime;
		PersistTicketTimeReset  resetmsg;
		resetmsg.m_nResetTime = curTime;

		PersistSystem::getSingleton().postThreadMsg(resetmsg, curTime);
	
		calculateTicketActive((time_t)m_nTicketFriendTime);

		return;
	}

	if(m_ticketfriendMap.size() == 0)
		{
			generateTicketQuesions();
			calculateTicketActive((time_t)m_nTicketFriendTime);
		}
}

//����Ʊ�Ѽ�����Ч�ڵ�ʱ���
void ConsortSystem::calculateTicketActive(time_t curTime)
{	
		struct tm *p = localtime(&curTime);
		if(p->tm_hour >= 13)
		{
			p->tm_hour = 21;
		}
		else
		{
			p->tm_hour = 13;
		}

		p->tm_min = 0;
		p->tm_sec = 0;
		
		m_nTicketAcitve = mktime(p);
}

void ConsortSystem::calculateWoodBegin(time_t curTime)
{
	struct tm *p = localtime(&curTime);
	
		p->tm_min = 0;
		p->tm_sec = 0;
		p->tm_hour = 11;
		
		m_nWoodCatBegin = mktime(p);
		p->tm_hour = 23;
		m_nWoodCatEnd = mktime(p);
}

UInt64 ConsortSystem::getWoodBeginTime()
{
	return m_nWoodCatBegin;
}

UInt64 ConsortSystem::getWoodEndTime()
{
	return m_nWoodCatEnd;
}

void ConsortSystem::clearWoodBattle(UInt64 consortId, UInt64 playeruid)
{
		ConsortData * pConsortData = ConsortSystem::getSingleton().getConsortDataById(consortId);
		
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

void ConsortSystem::memNotifyHalfDay()
{
			//////////////////////////


	for(List<ConsortData >::Iter * consortIter = m_consortList.begin();  consortIter != NULL; 
				consortIter = m_consortList.next(consortIter))
			{
				//��Ծ������Ϊ0������ǩ���ȼ�����Ϊ����ĸõȼ�
				consortIter->mValue.m_nBusinessLv = consortIter->mValue.m_nLv;
			
				
				//���칫��ͬ�������߳�Ա��Ծ����������ݰ�
				 BusinessCatNotify  businessSync;
				 businessSync.mPacketID = BOC_BUSINESSCATSYNC_NOTIFY;
				 Json::Value signNotifyRoot;
				
				 signNotifyRoot["businesslv"] = consortIter->mValue.m_nLv;
			
           
				//ˢ�´�������
				signNotifyRoot["businesscattimes"] = 0;
				

				Json::StyledWriter notifyWriter;

				std::string jsonstr = notifyWriter.write(signNotifyRoot);
				//LOG_INFO("businesscat  notify is: %s ", jsonstr.c_str() );	
				businessSync.mRespJsonStr = jsonstr;

				//�������ڴ��������è��ص��ڴ�����
				for(List<UInt64>::Iter * memberIter = consortIter->mValue.m_nMemberList.begin(); memberIter != NULL;
					memberIter = consortIter->mValue.m_nMemberList.next(memberIter))
				{
					
					Player * player = LogicSystem::getSingleton().getPlayerByGuid(memberIter->mValue);
					//����֪ͨ���������
					if(player)
					{
						//�޸�����ڴ�����
						PlayerConsortData * playerConsortData = player->getConsortManager().getPlayerConsortData();
						//���������������è����
						playerConsortData->m_nBusinessCatTimes = 0;
						//��Ʒ����״̬���
						for(List<UInt32>::Iter * buyIter = playerConsortData->m_nBuyList.begin();   
							buyIter != NULL; buyIter = playerConsortData->m_nBuyList.next(buyIter))
						{
							buyIter->mValue = 0;
						}
					
						////////////////////////////////////////////////////
					
						//�ж�����Ƿ����ߣ����߷���ˢ��֪ͨ
						const ConnId & memCon = player->getConnId();
						if(memCon)
						{
							//�˴�֪ͨ��ע��
						//	NetworkSystem::getSingleton().sendMsg(businessSync,memCon);
						}
						

					}
				} //for List<UInt64>::Iter * memberI

			}//for(List<ConsortDat

	/////////////////////////////////////////////////////
}

void ConsortSystem::ticketRefresh()
{
	PersistTicketAllDel  delmsg;
	PersistSystem::getSingleton().postThreadMsg(delmsg, 0);

	UInt32 index = 0;
	char sql[4096*10]={0};
	for(List<ConsortData >::Iter * consortIter = m_consortList.begin();  consortIter != NULL; 
				consortIter = m_consortList.next(consortIter))
	{
		consortIter->mValue.m_ticketList.clear(); 
		consortIter->mValue.m_ticketDataMap.clear();
		List<UInt32>* pTicketList = getTicketQuestion(consortIter->mValue.m_nLv);
		index++;
		for(List<UInt32>::Iter * ticketIter = pTicketList->begin(); ticketIter != NULL;
			ticketIter = pTicketList->next(ticketIter) )
		{
			TicketData ticketData;
			ticketData.m_nTicketId = ticketIter->mValue;
			ticketData.m_ticketFriends.clear();
			List<TicketData>::Iter * insertIter = consortIter->mValue.m_ticketList.insertTail(ticketData);
			consortIter->mValue.m_ticketDataMap.insert(ticketData.m_nTicketId, &(insertIter->mValue));
			
			char temp[256]={0};
			snprintf(temp,sizeof(temp),"insert into ticketfriend(ticketid, consortuid) values(%u,%llu); ", ticketData.m_nTicketId, consortIter->mValue.m_nId);
			strcat(sql,temp);

		}
		
		
		//////////////////////////////
		if(index == 10)
		{
			PersistTicketFriendInit  friendInit;
			friendInit.m_strExe = String(sql);
			LOG_INFO("ticketRefresh %s",friendInit.m_strExe.c_str());

			PersistSystem::getSingleton().postThreadMsg(friendInit, 0);
			 LOG_WARN("ticketRefresh do it 5");
			memset(sql,0,4096*10);
			index = 0;

		}
	
		///////////////////////////////

	}

	if(index)
	{
			PersistTicketFriendInit  friendInit;
			friendInit.m_strExe = String(sql);
			
			LOG_INFO("ticketRefresh %s",friendInit.m_strExe.c_str());

			PersistSystem::getSingleton().postThreadMsg(friendInit, 0);
			 LOG_WARN("ticketRefresh do it 6");

			memset(sql,0,4096*10);
			index = 0;
	}
}

void ConsortSystem::ticketQuestionInit(UInt64 consortId)
{
		Map<UInt64, ConsortData *>::Iter * consortIter = m_consortMap.find(consortId);

		consortIter->mValue->m_ticketList.clear(); 
		consortIter->mValue->m_ticketDataMap.clear();
		List<UInt32>* pTicketList = getTicketQuestion(consortIter->mValue->m_nLv);
		
		char sql[128*1000]={0};
		for(List<UInt32>::Iter * ticketIter = pTicketList->begin(); ticketIter != NULL;
			ticketIter = pTicketList->next(ticketIter) )
		{
			TicketData ticketData;
			ticketData.m_nTicketId = ticketIter->mValue;
			ticketData.m_ticketFriends.clear();
			List<TicketData>::Iter * insertIter = consortIter->mValue->m_ticketList.insertTail(ticketData);
			consortIter->mValue->m_ticketDataMap.insert(ticketData.m_nTicketId, &(insertIter->mValue));
			
			char temp[128]={0};
			snprintf(temp,sizeof(temp),"insert into ticketfriend(ticketid, consortuid) values(%u,%llu); ", ticketData.m_nTicketId, consortIter->mValue->m_nId);
			strcat(sql,temp);

		}
	
			PersistTicketFriendInit  friendInit;
			friendInit.m_strExe = String(sql);

			PersistSystem::getSingleton().postThreadMsg(friendInit, 0);
			


}


//����1~10������Ʊ�Ѽ������
void ConsortSystem::generateTicketQuesions()
{
	m_ticketfriendMap.clear();
	for(UInt32 i = 1; i < 11; i ++)
	{
		generateTicketQuestion(i);
	}
}

void ConsortSystem::generateTicketQuestion(UInt32 lv)
{
	 List<TicketFriendTemplate> * plist = TICKETFRIEND_TABLE().getLvList(lv);

	ConsortLvTemplate * consortlv = CONSORTLV_TABLE().get(lv);

	if(plist->size() < consortlv->mTicketTimes)
	{
		List<UInt32 > ticketList;
	
		for(List<TicketFriendTemplate>::Iter * ticketIter = plist->begin(); ticketIter != NULL;
			ticketIter = plist->next(ticketIter) )
		{
			ticketList.insertTail(ticketIter->mValue.mId);
		}

	    m_ticketfriendMap.insert(lv, ticketList);
		return ;	
	}

	List<TicketFriendTemplate> randomlist = *plist;
	List<UInt32> taskList;
	for(UInt32 i = 0; i < consortlv->mTicketTimes ;  i++)
	{
		//ȡ�����һ��Ԫ��
		 List<TicketFriendTemplate>::Iter * lastTaskIter = randomlist.last();
		//���һ��Ԫ�ص�tag��Ϊrate�ܺ�
		 UInt32 randRes = rand()%(lastTaskIter->mValue.mTag);
		 List<TicketFriendTemplate>::Iter *  findTaskIter = NULL;

		 for(List<TicketFriendTemplate>::Iter *  randomTaskIter =randomlist.begin(); 
			randomTaskIter != NULL; randomTaskIter = randomlist.next(randomTaskIter) )
		 {
			 if(randRes < randomTaskIter->mValue.mTag)
			 {
				findTaskIter = randomTaskIter;
				break;
			 }
		 }

		 assert(findTaskIter);
	
		 //Ϊ�����Ч�ʣ����ٱ������������ҵ��Ľڵ���һ������tag���£�����Ϊ�������ֵ
		 //���ҽ��ҵ��Ľڵ��Ƴ�
		 for(List<TicketFriendTemplate>::Iter * randomTaskIter = randomlist.next(findTaskIter); 
			randomTaskIter != NULL; randomTaskIter = randomlist.next(randomTaskIter))
		 {
			 randomTaskIter->mValue.mTag -= findTaskIter->mValue.mRate;
		 }

		 //�Ƚ��ҵ��Ľڵ���뵽taskList�����ڷ���
		 taskList.insertTail(findTaskIter->mValue.mId);
		 //���ҵ��Ľڵ��Ƴ�������֮ǰ�Ѿ�������ȥ���ýڵ���tag����
		 randomlist.erase(findTaskIter);

	}

	m_ticketfriendMap.insert(lv, taskList);

}

List<UInt32>* ConsortSystem::getTicketQuestion(UInt32 lv)
{
	Map<UInt32, List<UInt32> >::Iter * findIter = m_ticketfriendMap.find(lv);
	if(!findIter)
	{
		return NULL;
	}

	return &(findIter->mValue);
	
}

void ConsortSystem::convertTicketStr2List(List<TicketFriend> *pFriendList, std::string ticketStr)
{
		std::string ::size_type  findIndex = ticketStr.find(";");
		std::stringstream mystream;

		while(findIndex != std::string::npos)
		{
			//;��֮ǰ��ȫ����ȡ�����뵽�б���
			std::string elestr = ticketStr.substr(0,findIndex);
			std::string::size_type subindex = elestr.find(",");
			std::string playerstr = elestr.substr(0,subindex);
			std::string datastr = elestr.substr(subindex+1);
			std::string::size_type servantindex = datastr.find(",");
			std::string servantstr = datastr.substr(0, servantindex);
			std::string orderstr = datastr.substr(servantindex + 1);

			
			TicketFriend ticketFriend;
			
			mystream.clear();
			mystream.str("");
			mystream << playerstr;
            mystream >> ticketFriend.m_nPlayeruid;
			
			mystream.clear();
			mystream.str("");
			mystream << servantstr;
			mystream >> ticketFriend.m_nServantid;

			mystream.clear();
			mystream.str("");
			mystream << orderstr;
			mystream >> ticketFriend.m_nOrderIndex;

			pFriendList->insertTail(ticketFriend);

			ticketStr = ticketStr.substr(findIndex + 1);

			findIndex = ticketStr.find(';');

		}
}

void ConsortSystem::convertGivenStr2List(List<GivenData>* givenList, std::string givenStr)
{
		std::string ::size_type  findIndex = givenStr.find(";");
		std::stringstream mystream;

		while(findIndex != std::string::npos)
		{
			//;��֮ǰ��ȫ����ȡ�����뵽�б���
			std::string elestr = givenStr.substr(0,findIndex);
			std::string::size_type subindex = elestr.find(",");
			std::string namestr = elestr.substr(0,subindex);
			std::string gradestr = elestr.substr(subindex+1);
		
			GivenData givendata;
			
			mystream.clear();
			mystream.str("");
			mystream << gradestr;
            mystream >> givendata.m_nGrade;
			
			givendata.m_strName = namestr.c_str();
			givenList->insertTail(givendata);

			givenStr = givenStr.substr(findIndex + 1);

			findIndex = givenStr.find(';');

		}
}

std::string ConsortSystem::convertTicketList2Str(List<TicketFriend>* ticketList)
{
	std::stringstream mystream;
	std::string dataStr="";
	for(List<TicketFriend>::Iter * friendIter = ticketList->begin(); friendIter!= NULL;
		friendIter = ticketList->next(friendIter) )
	{
		mystream.clear();
		mystream.str("");
		std::string tempStr;
		mystream <<  friendIter->mValue.m_nPlayeruid;
		mystream >> tempStr;
		dataStr = dataStr + tempStr +",";
		mystream.clear();
		mystream.str("");
		tempStr.clear();
		mystream <<  friendIter->mValue.m_nServantid;
		mystream >> tempStr;
		dataStr = dataStr + tempStr +",";

		mystream.clear();
		mystream.str("");
		tempStr.clear();
		mystream <<  friendIter->mValue.m_nOrderIndex;
		mystream >> tempStr;
		dataStr = dataStr + tempStr +";";
	}

	return dataStr;
}

std::string ConsortSystem::converGivenList2Str(List<GivenData>* givenList)
{
	std::stringstream mystream;
	std::string dataStr="";
	for(List<GivenData>::Iter * givenIter = givenList->begin(); givenIter!= NULL;
		givenIter = givenList->next(givenIter) )
	{
		mystream.clear();
		mystream.str("");
		std::string tempStr;
		mystream <<  givenIter->mValue.m_nGrade;
		mystream >> tempStr;
		dataStr = dataStr + givenIter->mValue.m_strName.c_str() +",";
		
		dataStr = dataStr + tempStr +";";
	}

	return dataStr;
}



const DBInterface & ConsortSystem::getDBInterface()
{
	return  mDBInterface;
}

void  ConsortSystem::chatListJson(Json::Value  &root, UInt64 playeruid, UInt64 lastchattime)
{
			Json::Value chatDataRoot;
			
			char basedata[2048] = {0};

			snprintf(basedata, sizeof(basedata),  "call basedata_load(%llu)", playeruid);

			LOG_DEBUG("Sql:%s", basedata);

			bool result = mDBInterface.execSql(basedata);
 
        

			MYSQL_RES* rs = mDBInterface.storeResult();
			if(!rs)
			{
				// ִ��ʧ��
				mDBInterface.freeResult(&rs);
				return;		
			}

			MYSQL_ROW row = mDBInterface.fetchRow(rs);

			if(!row)
			{
				// ��ɫ������
				mDBInterface.freeResult(&rs);
				return;
				
			}

			chatDataRoot["lastChatTime"] = lastchattime;
			chatDataRoot["playeruid"] = playeruid;
			
			chatDataRoot["modelid"]  = lynxAtoi<UInt32>(row[0]);
			chatDataRoot["name"] = row[1];

			chatDataRoot["onLine"] = 0;

			root.append(chatDataRoot);

			mDBInterface.freeResult(&rs); 
}


bool ConsortSystem::getBaseInfoFromDb(UInt64 playerUid, BaseInfoData & baseInfoData)
{
	char basedata[2048] = {0};

	snprintf(basedata, sizeof(basedata),  "call basedata_load(%llu)", playerUid);

	LOG_DEBUG("Sql:%s", basedata);
	
	bool result = mDBInterface.execSql(basedata);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// ִ��ʧ��
		mDBInterface.freeResult(&rs);
		LOG_INFO("sql exec failed");
		return false;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	if(!row)
	{
		// ��ɫ������
		mDBInterface.freeResult(&rs);
		return false;

	}

	baseInfoData.modelId  = lynxAtoi<UInt32>(row[0]);
	baseInfoData.name = row[1];
	baseInfoData.power = lynxAtoi<UInt64>(row[2]);
	baseInfoData.vipLv = lynxAtoi<UInt32>(row[3]);
	baseInfoData.level = lynxAtoi<UInt32>(row[4]);
	baseInfoData.leaveTime = lynxAtoi<UInt64>(row[5]);

	mDBInterface.freeResult(&rs); 

	return true;
}


bool ConsortSystem::getOtherFriendInfo(UInt64 otherUid, FriendBlackInfo & friendblackInfo)
{
	char sql[4096] = {0};
	snprintf(sql, sizeof(sql), "call friendandblack_load(%llu)", otherUid);

	

	bool res = mDBInterface.execSql(sql);

	if(!res)
	{
		LOG_INFO("call friendandblack_load failed!! player: %llu",  otherUid);

		return false;

	}

	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// ִ��ʧ��

		
		mDBInterface.freeResult(&rs);

		return false ;		
	}


	MYSQL_ROW row = mDBInterface.fetchRow(rs);
	if(!row)
	{
		mDBInterface.freeResult(&rs);
		return false;
	}

	friendblackInfo.friendstr = row[1];
	friendblackInfo.blackstr = row[2];
	friendblackInfo.friendCount= lynxAtoi<UInt32>(row[3]);
	friendblackInfo.playerUid = otherUid;
	

	mDBInterface.freeResult(&rs); 

	return true;
}


void ConsortSystem::saveTicketFriends(UInt64 consortid, UInt32 ticketid)
{
	Map<UInt64, ConsortData *>::Iter* consortIter = m_consortMap.find(consortid);
	Map<UInt32, TicketData*>::Iter * ticketDataIter = consortIter->mValue->m_ticketDataMap.find(ticketid);
	
	std::string dataStr = convertTicketList2Str( &(ticketDataIter->mValue->m_ticketFriends) );
	
	PersistTicketDataUpdate ticketDataUpdate;
	ticketDataUpdate.m_nConsortId = consortid;
	ticketDataUpdate.m_nTicketId = ticketid;
	ticketDataUpdate.m_strData = dataStr.c_str();
	
	PersistSystem::getSingleton().postThreadMsg(ticketDataUpdate, consortid);
	
}

void ConsortSystem::saveTicketGivens(UInt64 consortid, UInt32 ticketid)
{
	Map<UInt64, ConsortData *>::Iter* consortIter = m_consortMap.find(consortid);
	Map<UInt32, TicketData*>::Iter * ticketDataIter = consortIter->mValue->m_ticketDataMap.find(ticketid);
	
	std::string dataStr = converGivenList2Str( &(ticketDataIter->mValue->m_givenList) );
	
	
	PersistTicketSupport givenUpdate;
	givenUpdate.m_nConsortId = consortid;
	givenUpdate.m_nTicketId = ticketid;
	givenUpdate.m_nQuality = ticketDataIter->mValue->m_nQuality;
	givenUpdate.m_strFriends = dataStr.c_str();
	
	PersistSystem::getSingleton().postThreadMsg(givenUpdate, consortid);
}

UInt64 ConsortSystem::getAwardActiveTime()
{
	return m_nTicketAcitve;
}




bool ConsortSystem::getOtherBeApplyGuidList(UInt64 otherUid, List<UInt64> &friendBeApplyGuidList)
{
	
	char sql[2048] = {0};

	snprintf(sql, sizeof(sql), "SELECT * FROM friend_be_apply_list WHERE playerGuid = %llu", otherUid);

	LOG_DEBUG("Sql:%s", sql);
	bool result = mDBInterface.execSql(sql);
	MYSQL_RES* rs = mDBInterface.storeResult();
	if(!rs)
	{
		// ִ��ʧ��
		mDBInterface.freeResult(&rs);
		return LynxErrno::SqlExecuteFail;		
	}

	MYSQL_ROW row = mDBInterface.fetchRow(rs);

	List<UInt64> playerIDList;
	UInt64 playerId;
	while(row)
	{
		for (UInt32 i = 1;i<=50;i++)
		{
			playerId =  lynxAtoi<UInt64>(row[i]);
			playerIDList.insertTail( playerId);
		}	
		row = mDBInterface.fetchRow(rs);
	}

	friendBeApplyGuidList = playerIDList;



	LOG_INFO("Load offline friend_be_apply_list %llu  from DB  successful!", otherUid);
	mDBInterface.freeResult(&rs); 
	return LynxErrno::None;

}
