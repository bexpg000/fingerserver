#include "AchieveManager.h"
#include "PersistSystem.h"
#include "Player.h"
#include "./FireConfirm/StageCalc.h"
#include "LogicSystem.h"
using namespace Lynx;

AchieveManager::AchieveManager()
{
	m_pAchieveList = NULL;
	m_achieveMap.clear();
	m_dailyTaskMap.clear();
	m_dlyEventMap.clear();
}


AchieveManager:: ~AchieveManager()
{

}

bool AchieveManager::initial(Player* player)
{
	m_pPlayer = player;
	m_pAchieveList = &(player->mPlayerData.mAchieveData.achieveList);
	for(List<AchieveData>::Iter * achItor = m_pAchieveList->begin(); achItor != NULL; achItor = m_pAchieveList->next(achItor))
	{
		m_achieveMap.insert(achItor->mValue.m_nEventId, &(achItor->mValue));
	}
	
	m_nPlayerUid = m_pPlayer->getPlayerGuid();

	m_pDailyTaskList = &(player->mPlayerData.mDailyTaskData.dailytaskList);

	for(List<DailyTaskData>::Iter * dailyIter = m_pDailyTaskList->begin(); dailyIter != NULL; dailyIter = m_pDailyTaskList->next(dailyIter))
	{
		m_dailyTaskMap.insert(dailyIter->mValue.m_nTempId,  &(dailyIter->mValue));
		m_dlyEventMap.insert(dailyIter->mValue.m_nEventId, &(dailyIter->mValue));
	}

	m_pdailyActiveData = &(m_pPlayer->mPlayerData.mDailyAcvData);

	return true;
}


void AchieveManager::release()
{
	m_achieveMap.clear();
	m_dailyTaskMap.clear();
	m_dlyEventMap.clear();
}

bool AchieveManager::acvArgMatch(UInt64 achieveId)
{
	AchievementTemplate * acvTemp = ACHIEVEMENT_TABLE().get(achieveId);
	assert(acvTemp);
	UInt64 acvEvent = acvTemp->mEvent;

	Map<UInt64,  AchieveData *>::Iter * acvDataIter = m_achieveMap.find(acvEvent);
	assert(acvDataIter);

	if(acvDataIter->mValue->m_nArg >= acvTemp->mArg)
	{
		return true;
	}
	else
	{
		return false;
	}
}

AchieveData * AchieveManager::getSameEventAcvData(UInt64 achieveId)
{
	AchievementTemplate * acvTemp = ACHIEVEMENT_TABLE().get(achieveId);
	assert(acvTemp);
	UInt64 acvEvent = acvTemp->mEvent;

	Map<UInt64,  AchieveData *>::Iter * acvDataIter = m_achieveMap.find(acvEvent);
	assert(acvDataIter);

	return acvDataIter->mValue;
}

std::string AchieveManager::convertAchieveDataToJson()
{
	Json::Value root;
	for(Map<UInt64,  AchieveData *>::Iter * achItor = m_achieveMap.begin(); achItor != NULL; 
		achItor = m_achieveMap.next(achItor))
	{
		Json::Value achRoot;
		achRoot["achievementid"] = achItor->mValue->m_nAchieveId;
		achRoot["event"] = achItor->mValue->m_nEventId;
		achRoot["arg"] = achItor->mValue->m_nArg;
		achRoot["flag"] = achItor->mValue->m_nFlag;
		root["achievement"].append(achRoot);
	}

	for(Map<UInt64, DailyTaskData*>::Iter * dailyIter = m_dailyTaskMap.begin();  dailyIter != NULL; 
		dailyIter = m_dailyTaskMap.next(dailyIter))
	{
		Json::Value dailyRoot;
		dailyRoot["dailyid"] = dailyIter->mValue->m_nTempId;
		DailyTaskTemplate * dailyTemp = DAILYTASK_TABLE().get(dailyIter->mKey);
		if(dailyTemp)
		{
					dailyRoot["event"] = dailyTemp->mEvent;
		}

		dailyRoot["arg"] = dailyIter->mValue->m_nArg;
		dailyRoot["flag"] = dailyIter->mValue->m_nFlag;

		root["daily"].append(dailyRoot);
	}

	for(List<UInt32>::Iter * activeIter = m_pdailyActiveData->flags.begin();  activeIter!=NULL; 
		activeIter = m_pdailyActiveData->flags.next(activeIter))
	{
		root["active"]["flags"].append(activeIter->mValue);
	}
	
	root["active"]["curactive"] = m_pdailyActiveData->curActive;

	Json::StyledWriter writer;
	return writer.write(root);
}

void AchieveManager::postUpdateAchMsg(UInt64 playerUid,  const AchieveData & achData)
{
		AchieveUpdateMsg achUpdateMsg;
		achUpdateMsg.playerUid = playerUid;
		achUpdateMsg.achieveData = achData;
		PersistSystem::getSingleton().postThreadMsg(achUpdateMsg, playerUid);
}

void AchieveManager::postUpdateDailyMsg(UInt64 playerUid,  const DailyTaskData & dailyTaskData)
{
		PersistDailyTaskUpdateMsg dailyUpdateMsg;
		dailyUpdateMsg.playerUid = playerUid;
		dailyUpdateMsg.dailyTask = dailyTaskData;
		PersistSystem::getSingleton().postThreadMsg(dailyUpdateMsg, playerUid);
}

void AchieveManager::updateAchieveData(UInt64 eventType, UInt32 change)
{
	Map<UInt64,  AchieveData *>::Iter * findIter = m_achieveMap.find(eventType);

	if(findIter)
	{
		//�жϸóɾ��Ƿ������һ�����������һ���Ѿ����
		//ֻ�����һ���ɾ���ȡ��������λ�Żᱻ����Ϊ1
		if(findIter->mValue->m_nFlag)
		{
			//�óɾ��Ѿ���ɣ�����Ҫ�ټ�¼������
			return;
		}

		switch(eventType)
		{
			case  BARRIER  :
			{
				//���ݵ�ǰ����id�ҵ���Ӧ������ṹ��
				AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(findIter->mValue->m_nAchieveId);
				//������Ҫ�Ĳ�����change��ͬ����ʱchange��ʾ�ؿ�id
				if(achieveTemp && achieveTemp->mArg == change)
				{
						findIter->mValue->m_nArg = change;
						//����
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
			
			}
			
			break;

		case	  ELITEBARRIER :
			{

				//���ݵ�ǰ����id�ҵ���Ӧ������ṹ��
				AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(findIter->mValue->m_nAchieveId);
				//������Ҫ�Ĳ�����change��ͬ����ʱchange��ʾ�ؿ�id
				if(achieveTemp && achieveTemp->mArg == change)
				{
						findIter->mValue->m_nArg = change;
						//����
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
			
			}
			
			break;
		case	  RHYMEENHANCELV :
			{
				//change ������ʾ���Ľ����ͼ�����ɵ����֣���3���ļ�����ʾ3004
			
			   //��¼��ǰǿ������
				findIter->mValue->m_nArg = change;
			   //����
			   postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				
			
			}
			
			break;
		case RHYMESKILLACTIVE:
			//�Ϲ�����ɾ�ȥ��
			break;

		case	  HONORSTONEACTIVE :
			{
				//change ������ʾ���������ʯ�ĸ������ı�ֵ�����������ʹ�2
				
				
				findIter->mValue->m_nArg += change;
				
					//����
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				

			}
			
			break;
		case GETSERVANT:
			{
				//change ������ʾ��ļӶ���ĸ������ı�ֵ����ļ�����ʹ�2

				findIter->mValue->m_nArg += change;

				//����
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));

			}
		
			break;

		case	  PLAYERLV :
			{
				//change��ʾ��ҵȼ�����ֵ������ֵ��
				findIter->mValue->m_nArg = change;

				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case	  VIPLV :
			{
				//change��ʾ���vip�ȼ��ı���������ı�ֵ���������ʹ�2
				findIter->mValue->m_nArg += change;

				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  POWER:
			{
				//power��ʾս�����յ�ֵ������ֵ����ǰս��Ϊ10000������10000
				findIter->mValue->m_nArg  = change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;

		case	  OWN4STARSERVANT :
			{
				//change ��ʾ�»�õ�����Ӷ���������ı�ֵ�������ǻ�û��n��4��Ӷ��
				//change = n;
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case	  OWNPURPULESERVANT :
			{
				//change ��ʾ�»�õ���ɫӶ���������ı�ֵ��
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case	  UNLOCKSERVANTASSIS :
			{
				//������������λ�������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}

			break;
		case	  OWNGREENASSIST :
			{
				//����������ɫ����λ���ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case OWNBLUEASSIST:
			{
				//����������ɫ����λ���ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case OWNPURPLEASSIST:
			{
				//����������ɫ����λ���ı�ֵ
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case QUALIFYDANLV:
			{
					//��λ���������ӣ��ı�ֵ
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  QUALIFYSUCCESS:
			{
				//��λ��ʤ���������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case  ARIEASUCCESS:
			{
				//������ʤ���������ı�ֵ
					findIter->mValue->m_nArg += change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
	
			break;
		case BUYCHARACTOR:
			{
				//�����ɫ�������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case COURAGESUCCESS:
			{
				//��������ʤ���������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case WXCHALLENGESUC:
			{
				//������ս�ѶȲ�����ֵ
				findIter->mValue->m_nArg = change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case USERHYMESKILL:
			{
				//�ͷ��Ϲ��������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case USERHYMESPEED:
			{
				//�ͷ����������������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
		
			break;
		case CALLSERVANTCNT:
			{
				//ս�����ٻ�Ӷ���������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case FRIENDCNT:
			{
				//���Ӻ��Ѹ������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case MAXGEMLV:
			
				//ӵ�б�ʯ��ߵȼ�������ֵ
				if(findIter->mValue->m_nArg < change)
				{
					findIter->mValue->m_nArg =change;
					postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
				}
				
			
		
			break;
	
		case LINGRENCALLSERVANT:
			{
				//�����ٻ��������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			

			break;
		case 	XIBANCALLSERVANT:
			{
				//Ϸ���ٻ��������ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		case MINGLINGCALLSERVANT:
			{
				//�����ٻ����ı�ֵ
				findIter->mValue->m_nArg += change;
				postUpdateAchMsg(m_nPlayerUid, *(findIter->mValue));
			}
			
			break;
		default:
			break;
		}
	}
	//cqy
	InlineActivityManager::getSingleton().updateSevenDayTaskData(m_nPlayerUid,eventType,change);
}


//ע���ճ�������µ�change���Ǹı�ֵ������ֻ��Ҫ���Ӽ���
void AchieveManager::updateDailyTaskData(UInt64 eventType,   UInt32 change)
{
	
		Map<UInt64, DailyTaskData *>::Iter * findDlyIter = m_dlyEventMap.find(eventType);
		if(!findDlyIter)
		{
			return;
		}
		//�����Ѿ���ȡ�Ͳ��ٴ��
		if(findDlyIter->mValue->m_nFlag)
		{
			return;
		}

		//�����¼���Ӧ�Ĳ�������
		findDlyIter->mValue->m_nArg += change;
		postUpdateDailyMsg(m_nPlayerUid, *(findDlyIter->mValue));

		//cqy
		InlineActivityManager::getSingleton().updateSevenDayTaskData(m_nPlayerUid,eventType,change);

}



void AchieveManager::dealAddDailyReq(UInt64 dailytaskId)
{
	
}

void AchieveManager::getActiveAward(UInt64 index)
{
		const ConnId & connId = m_pPlayer->getConnId();
		List<UInt32>::Iter * listIter = m_pdailyActiveData->flags.getn(index-1);

		if(!listIter)
		{
			GCActiveAwardGetResp getResp;
			getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;
			Json::Value root;
			Json::StyledWriter writer;
			root["errorId"] = LynxErrno::InvalidParameter;
			getResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(getResp, connId);

			return;
		}

		if(listIter->mValue == 1)
		{
			GCActiveAwardGetResp getResp;
			getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;
			Json::Value root;
			Json::StyledWriter writer;
			root["errorId"] = LynxErrno::InvalidParameter;
			getResp.mRespJsonStr = writer.write(root);
			NetworkSystem::getSingleton().sendMsg(getResp, connId);
			return ;
		}

		//���ñ��λΪ1
		listIter->mValue = 1;
		//������

		
		//��Ծ�ȱ����
		PersistDailyActiveUpdateMsg dailyacvMsg;
		dailyacvMsg.playerUid = m_nPlayerUid;
		dailyacvMsg.dailyacvData = m_pPlayer->mPlayerData.mDailyAcvData;
		PersistSystem::getSingleton().postThreadMsg(dailyacvMsg, m_nPlayerUid);

		DailyActiveTemplate * dailyActiveTemp = DAILYACTIVE_TABLE().get(index);
		//�·�����
		getContant(dailyActiveTemp->mItemList);


		//ƴ�������ͻ���
		sendDlyActiveResp(index, dailyActiveTemp->mItemList);

}

void AchieveManager::finishDailyTask(UInt64 dailytaskId)
{
	const ConnId & connId = m_pPlayer->getConnId();

	Map<UInt64, DailyTaskData*>::Iter * findDailyIter = m_dailyTaskMap.find(dailytaskId);
	if(!findDailyIter)
	{
		GCDailyTaskFinishResp finishResp;
		finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::DailyTaskIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr = writer.write(root);
		cout << finishResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return ;
	}

	DailyTaskTemplate * dailyTaskTemp = DAILYTASK_TABLE().get(dailytaskId);
	if(findDailyIter->mValue->m_nArg < dailyTaskTemp->mArg)
	{

		GCDailyTaskFinishResp finishResp;
		finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr = writer.write(root);
		cout << finishResp.mRespJsonStr;
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return;
	}

	//������ȡ��־λ
	findDailyIter->mValue->m_nFlag = 1;
	

	//���ӻ�Ծ��
	m_pPlayer->mPlayerData.mDailyAcvData.curActive += dailyTaskTemp->mActive;
	//��Ծ�ȱ����
	PersistDailyActiveUpdateMsg dailyacvMsg;
	dailyacvMsg.playerUid = m_nPlayerUid;
	dailyacvMsg.dailyacvData = m_pPlayer->mPlayerData.mDailyAcvData;
	PersistSystem::getSingleton().postThreadMsg(dailyacvMsg, m_nPlayerUid);

	//�·�����
	getContant(dailyTaskTemp->mItemList);

	//�ճ�������ɴ���
	postUpdateDailyMsg(m_nPlayerUid, *(findDailyIter->mValue));

	

	//ƴ�������ͻ���
	sendDlyResp2Client(dailytaskId,findDailyIter->mValue->m_nEventId, findDailyIter->mValue->m_nArg, findDailyIter->mValue->m_nFlag, dailyTaskTemp->mItemList);

}

bool AchieveManager::finishAchieve(UInt64 eventType, UInt64 achId)
{
	Map<UInt64,  AchieveData *>::Iter * findIter = m_achieveMap.find(eventType);
	if(!findIter)
	{ 
		const ConnId & connId = m_pPlayer->getConnId();
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEDAILYLIST_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::EventNotFind;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	switch(eventType)
	{
			case	  PLAYERLV :
			{
				return dealAchPlayerLv(eventType, achId, findIter->mValue);
			}
	
			break;

			case  BARRIER  :
				{
					return dealAchBarrier(eventType, achId, findIter->mValue);
				}

				break;

			case	  ELITEBARRIER :
				{
					return dealAchEliteBarrier(eventType,achId, findIter->mValue);
				}

				break;
			case	  RHYMEENHANCELV :
				{
					return dealAchRhymeEnhance(eventType,achId, findIter->mValue);
				}

				break;
			case RHYMESKILLACTIVE:
				//�Ϲ�����ɾ�ȥ��
				break;

			case	  HONORSTONEACTIVE :
				{
					return dealAchHonStoneActive(eventType,achId, findIter->mValue);

				}

				break;
			case GETSERVANT:
				{
					return dealgetAchServant(eventType,achId, findIter->mValue);

				}

				break;

			
			case	  VIPLV :
				{
					return dealAchVipLv(eventType,achId, findIter->mValue);
				}

				break;
			case  POWER:
				{
					return dealAchPower(eventType,achId, findIter->mValue);
				}

				break;

			case	  OWN4STARSERVANT :
				{
					return dealAch4StarServant(eventType,achId, findIter->mValue);
				}

				break;
			case	  OWNPURPULESERVANT :
				{
					return dealAchPurpleServant(eventType,achId, findIter->mValue);
				}

				break;
			case	  UNLOCKSERVANTASSIS :
				{
					return dealAchAssistOpen(eventType,achId, findIter->mValue);
				}

				break;
			case	  OWNGREENASSIST :
				{
					
				}

				break;
			case OWNBLUEASSIST:
				{
				
				}

				break;
			case OWNPURPLEASSIST:
				{
					return dealAchPurpleAssist(eventType,achId, findIter->mValue);
				}

				break;
			case QUALIFYDANLV:
				{
					
				}

				break;
			case  QUALIFYSUCCESS:
				{
					return dealAchQualifySucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case  ARIEASUCCESS:
				{
					return dealAchAreaSucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case BUYCHARACTOR:
				{
					return dealAchBuyCharactorCnt(eventType,achId, findIter->mValue);
				}

				break;
			case COURAGESUCCESS:
				{
					return dealCourageSucCnt(eventType,achId, findIter->mValue);
				}

				break;
			case WXCHALLENGESUC:
				{
					return dealWXChallengeCnt(eventType,achId, findIter->mValue);
				}

				break;
			case USERHYMESKILL:
				{
					
				}

				break;
			case USERHYMESPEED:
				{
					
				}

				break;
			case CALLSERVANTCNT:
				{
					
				}

				break;
			case FRIENDCNT:
				{
						return dealAchFriendCnt(eventType,achId, findIter->mValue);
				}

				break;
		
			case MAXGEMLV:
				{
					return dealAchGemMaxLv(eventType,achId, findIter->mValue);
				}
				break;
			case LINGRENCALLSERVANT:
				{
					
					return dealAchLingRenCall(eventType,achId, findIter->mValue);
				}


				break;
			case 	XIBANCALLSERVANT:
				{
					return dealAchXibanCall(eventType,achId, findIter->mValue);
				}

				break;
			case MINGLINGCALLSERVANT:
				{
					return dealAchMingLingCall(eventType,achId, findIter->mValue);
				}

				break;
		
			

			default:
				break;

	}


	return true;
}

bool AchieveManager::dealAchPlayerLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��
		
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��
		
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж���ҵȼ��Ƿ���������
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
	
		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;

	}
	
	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);
	return true;

}

bool AchieveManager::dealAchBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}
	
	//�жϹؿ��ɾͲ����Ƿ����
	if(pAchData->m_nArg  !=  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//�ж���һ���ɾ���Ҫ�Ĺؿ��Ƿ�ͨ�أ�ͨ�����Ϊ��һ���ɾ͹ؿ�id������Ϊ0
		///////////////
		////////
		AchievementTemplate*  newAchieve = ACHIEVEMENT_TABLE().get(pAchData->m_nAchieveId);
		bool isPass = StageCalcManager::getSingleton().checkStageClearance(m_nPlayerUid,newAchieve->mArg);
		if(isPass)
		{
				pAchData->m_nArg = newAchieve->mArg;
		}
		else
		{
				pAchData->m_nArg = 0;
		}
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

		return true;
}

bool AchieveManager::dealAchEliteBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϹؿ��ɾͲ����Ƿ����
	if(pAchData->m_nArg  !=  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//�ж���һ���ɾ���Ҫ�Ĺؿ��Ƿ�ͨ�أ�ͨ�����Ϊ��һ���ɾ͹ؿ�id������Ϊ0
		///////////////
		////////

		AchievementTemplate*  newAchieve = ACHIEVEMENT_TABLE().get(pAchData->m_nAchieveId);
		bool isPass = StageCalcManager::getSingleton().checkStageClearance(m_nPlayerUid,newAchieve->mArg);
		if(isPass)
		{
			pAchData->m_nArg = newAchieve->mArg;
		}
		else
		{
			pAchData->m_nArg = 0;
		}
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);
	
	return true;
}

bool AchieveManager::dealAchRhymeEnhance(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж����ĳɾͲ����Ƿ����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//����ǿ������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchHonStoneActive(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϽ�ʯ�ɾͲ����Ƿ����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//��ʯ�����������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealgetAchServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϻ��Ӷ���ɾͲ����Ƿ����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ��Ӷ����������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchVipLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�vip�ȼ��Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ��Ӷ����������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPower(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAch4StarServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�4��Ӷ�������Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ��Ӷ����������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPurpleServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж���ɫӶ�������Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ����ɫӶ����������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchAssistOpen(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϿ���������λ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ������λ��������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchPurpleAssist(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж���ɫ����λ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		//ӵ����ɫ����λ��������Ҫ����arg
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchQualifySucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж���λ��ʤ�������Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchAreaSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϾ�����ʤ�������Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchBuyCharactorCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϹ����ɫ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealCourageSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж����������ɹ������Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealWXChallengeCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�������ս�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;
		
	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchFriendCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϺ��Ѹ����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchGemMaxLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϱ�ʯ�ȼ��Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchLingRenCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�������ļ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchXibanCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�Ϸ����ļ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}

bool AchieveManager::dealAchMingLingCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData)
{
	const ConnId & connId = m_pPlayer->getConnId();

	if(achId != pAchData->m_nAchieveId)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotMatch;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

		return false;
	}

	AchievementTemplate*  achieveTemp =  ACHIEVEMENT_TABLE().get(achId);
	if(!achieveTemp)
	{
		//����id��ƥ��

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::AchIdNotExist;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�ж�������ļ�����Ƿ�����
	if(pAchData->m_nArg  <  achieveTemp->mArg)
	{
		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
		Json::Value root;
		root["errorId"] = LynxErrno::ConditionNotReach;
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);
		return false;
	}

	//�жϸóɾ��Ƿ�ﵽ���һ����
	//���һ��ֻ��Ҫ����ȡ�������Ϊ1
	if(!(achieveTemp-> mNextId))
	{
		pAchData-> m_nFlag = 1;
	}
	else
	{
		//���µ���һ������id
		pAchData->m_nAchieveId = achieveTemp-> mNextId;
		//��һ������id��Ӧ����ɱ��Ϊ0��
		pAchData-> m_nFlag = 0;

	}

	//�·�����
	getContant(achieveTemp->mItemList);

	//����
	postUpdateAchMsg(m_nPlayerUid, *pAchData);

	//ƴ�������ͻ���
	sendResp2Client(achId,pAchData->m_nAchieveId,eventType, pAchData->m_nArg, pAchData->m_nFlag, achieveTemp->mItemList);

	return true;
}


void AchieveManager::getContant(const List<AllItemEle> &contantList)
{
	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL; 
		contantIter = contantList.next(contantIter))
	{
		m_pPlayer->getAllItemManager().addAwardMaterial(contantIter->mValue.resType,contantIter->mValue.subType,
			contantIter->mValue.count);
	}
		
}

void AchieveManager::sendResp2Client(UInt64 oldAchId, UInt64 newAchId,  UInt64 eventId, UInt32 arg, UInt32 flag,
									 const List<AllItemEle> &contantList)
{

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["oldachieveid"] = oldAchId;
	root["newachieveid"] = newAchId;
	root["event"] = eventId;
	root["arg"] = arg;
	root["flag"] = flag;

	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;
				

				////////////////////////////////////////////////////////////////////////
				
			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
					treasureIter = servantData->equipTreasures.next(treasureIter))
				{
					itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
				}

				root["contant"].append(itemRoot);
		} //if
		else
		{
			
				UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
				Json::Value itemRoot;
				itemRoot["resType"] = contantIter->mValue.resType;
				itemRoot["subType"] = contantIter->mValue.subType;
				itemRoot["count"] = count;

				root["contant"].append(itemRoot);
			

			}//else

		}//for

		GCAchieveFinishResp finishResp;
		finishResp.mPacketID = BOC_ACHIEVEFINISH_RESP;
	
		Json::StyledWriter writer;
		finishResp.mRespJsonStr =  writer.write(root);
		cout << finishResp.mRespJsonStr; 
		const ConnId & connId = m_pPlayer->getConnId();

		NetworkSystem::getSingleton().sendMsg(finishResp, connId);

	}


void AchieveManager::sendDlyResp2Client(UInt64 dlyId , UInt64 eventId, UInt32 arg, UInt32 flag,
						const List<AllItemEle> &contantList)
{

	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["dailyid"] = dlyId;
	root["event"] = eventId;
	root["arg"] = arg;
	root["flag"] = flag;
	root["curactive"] = m_pPlayer->mPlayerData.mDailyAcvData.curActive;

	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;


			////////////////////////////////////////////////////////////////////////

			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
				treasureIter = servantData->equipTreasures.next(treasureIter))
			{
				itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
			}

			root["contant"].append(itemRoot);
		} //if
		else
		{

			UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = count;

			root["contant"].append(itemRoot);


		}//else

	}//for

	GCDailyTaskFinishResp finishResp;
	finishResp.mPacketID = BOC_DAILYTASKFINISH_RESP;

	Json::StyledWriter writer;
	finishResp.mRespJsonStr =  writer.write(root);
	cout << finishResp.mRespJsonStr; 
	const ConnId & connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(finishResp, connId);


}


void AchieveManager::sendDlyActiveResp(UInt32 index, const List<AllItemEle> &contantList)
{
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	for(List<UInt32>::Iter * flagIter = m_pdailyActiveData->flags.begin();  flagIter != NULL;  
		flagIter = m_pdailyActiveData->flags.next(flagIter))
	{
		root["flags"].append(flagIter->mValue);
	}
	
	root["curactive"] = m_pPlayer->mPlayerData.mDailyAcvData.curActive;


	for(List<AllItemEle>::Iter * contantIter = contantList.begin();  contantIter != NULL;
		contantIter = contantList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = servantData->pieceCount;


			////////////////////////////////////////////////////////////////////////

			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
				treasureIter = servantData->equipTreasures.next(treasureIter))
			{
				itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
			}

			root["contant"].append(itemRoot);
		} //if
		else
		{

			UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["count"] = count;

			root["contant"].append(itemRoot);


		}//else

	}//for

	GCActiveAwardGetResp getResp;
	getResp.mPacketID = BOC_ACTIVEAWARDGET_RESP;

	Json::StyledWriter writer;
	getResp.mRespJsonStr =  writer.write(root);
	cout << getResp.mRespJsonStr; 
	const ConnId & connId = m_pPlayer->getConnId();

	NetworkSystem::getSingleton().sendMsg(getResp, connId);

}



	