#ifndef __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

#include "MainThread.h"
#include "Player.h"

#include "Guild.h"

namespace Lynx
{
	
	class LogicSystem : public MainThread, public Singleton<LogicSystem>
	{
		friend class ConsortSystem;
	public:
        LogicSystem();
		virtual ~LogicSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

		void  dailyRefresh(const UInt64& accTime);

		void  getDailyRefreshList(UInt32 refreshTime,List<UInt32>&refreshList);

		UInt32  getRefreshTime(UInt32 systemID);

		

		//�ɾ�ϵͳ�ճ����£�����֪ͨ�� Ĭ�������֪ͨ
		void dailyTaskRefresh(const UInt64& accTime);

        bool reloadTable(const String& tableName);
        bool reloadAllTable();

        void setPlayerGuid(const Guid& guidBase) { mPlayerGuidBase = guidBase; }

		const Guid& getPlayerGuid(){return mPlayerGuidBase;}
 
		Guid getItemGuid(){ return mItemGuidBase; }

		//������itemGuid
        Guid generateItemGuid() {++mItemGuidBase;
			//baseUidPersist();
			return mItemGuidBase; }
      		
		//����ȫ��Ψһ��itemUid
		void setItemGuid(const Guid& itemGuid)
		{
			mItemGuidBase = itemGuid;
		}

		void setLansquenetGuid(const Guid& lansquenetGuid)
		{
			mLansquenetGuid = lansquenetGuid;
		}

		void setGuildGuid(const Guid& guildGuid)
		{
		  mGuildGuid = guildGuid;
		}

		//������emailGuid
		Guid generateEmailGuid() {++mEmailGuid;
		return mEmailGuid; }

		void setEmailGuid(const Guid& emailGuid)
		{
			mEmailGuid = emailGuid;
		}

		//���ɲ���id
		Guid generateMaterialGuid() {++mMaterialGuid;
		//baseUidPersist();
		return mMaterialGuid; }

		void setMaterialGuid(const Guid& materialGuid)
		{
			mMaterialGuid = materialGuid;
		}

		//���ɽ�ɫid���Լ��ճ�����id
		Guid generateCharactorGuid(){++mCharactorGuid;
		//baseUidPersist();
		return mCharactorGuid;}

		void setCharactorGuid(const Guid& charactorGuid)
		{
			mCharactorGuid = charactorGuid;
		}

		Guid generateConsortGuid(){++mConsortGuid;
		//baseUidPersist();
		return mConsortGuid;}

		void setConsortGuid(const Guid& consortGuid)
		{
			mConsortGuid = consortGuid;
		}
	
		void setTicketGuid(const Guid& ticketGuid)
		{
			mTicketGuid = ticketGuid;
		}
	
		Guid generateTicketGuid(){++mTicketGuid;
		//baseUidPersist();
		return mTicketGuid;}



		//��Ҽ�������
		void baseUidPersist();

		Player* createPlayer(const ConnId& connId, const PlayerData& playerData, 
            const UInt64& loginToken);
		void destroyPlayerByGuid(const Guid& guid);
		void destroyPlayerByConnId(const ConnId& connId);
		void destroyPlayerByName(const String& playerName);
        void destroyPlayerByAccountId(const Guid& accountId);
        void destroyPlayerByAccountName(const String& accountName);
		Player* getPlayerByConnId(const ConnId& connId);
		Player* getPlayerByGuid(const Guid& guid);
		Player* getPlayerByName(const String& playerName);
        Player* getPlayerByAccountGuid(const Guid& guid);
        Player* getPlayerByAccountName(const String& accountName);

        const Map<ConnId, Player*>& getPlayerConnectionMap() const { return mPlayerConnectionMap; }

        void passportAuthSuccess(const ConnId& connId, const Guid& accountId, const String& accoutName);
        void passportAuthFail(const ConnId& connId, const String& accountName);
        
		void loadPlayerData(const ConnId& connId, const Guid& playerGuid);
        void loadPlayerDataSuccess(const ConnId& connId, const PlayerData& playerData);
        void loadPlayerDataFail(const ConnId& connId, const Guid& playerGuid);
        void reconnect(const ConnId& connId, const Guid& playerGuid, const UInt64& loginToken);

    // ϵͳ�㲥�ӿ�
        void broadcaseSystemNotice(const String& contents, UInt8 priority);
        void sendSystemNoticeToPlayer(const Guid& playerGuid, const String& contents, UInt8 priority);

		//���ʼ�
		void GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant, std::string title, std::string from);

    // ����ӿ�
    public:
        Guild* createGuild(const Guid& playerGuid, const String& guildName);
        void destroyGuildByGuid(const Guid& guildGuid);
        void destroyGuildByName(const String& guildName);
        Guild* getGuildByGuid(const Guid& guildGuid);
        Guild* getGuildByName(const String& guildName);

	public:
		//���������
		List<UInt32> getRandomList(int randomCount, int randomMax);

		void updateDailyResetData(Player *player,UInt32 flag,List<UInt32> &refreshIDs);

		void checkPlayerDataFromDB(Player& player);

		bool combinSendData(Guid playerID,UInt32 resType,Json::Value jvalue);

		bool combinSendDataTest(Guid playerID,UInt32 resType,const Json::Value &jvalue);

	public:
		//�����Ϣ
		std::string getDetailInfo(std::string playerUidStr); 

		void  setDetailInfo(UInt64 playerID, std::string detailInfo);

		//��ȡ��Ҹ�Ҫ��Ϣ
		bool getBaseInfo(UInt64 playerUid, BaseInfoData & baseInfoData);

		//��ȡbaseinfomap
		const Map<UInt64, BaseInfoData> &getBaseInfoMap();

		void insertBaseInfo(const BaseInfoData & baseInfoData);
		//��½������ʱ���������Ϣ
		void updateBaseInfo(const PlayerBaseData &baseData);

		void updateBaseInfo(const BaseInfoData & baseInfoData);

		//��¼�����߸�����Ұ����Ϣ
		void updateConsortInfo(UInt64 playerUid, const PlayerConsortData & playerConsortData);

		//��ʱ������Ҿ�������
		void updateConsortInfo(UInt64 playerUid, const ConsortInfoData& consortInfoData);

		//��ȡ��Ұ����Ϣ
		ConsortInfoData  * getConsortInfo(UInt64 playerUid);
		

		//���¹��Ỻ�棬Ŀǰ����ʹ��
		//void updateConsortInfo(const ConsortInfoData & consortInfoData);

		Guid listRandOne(Guid playerID,List<GuidValue> keyValueList,UInt32 flag,UInt32 initialValue);

			
		void dealSimililarPower(Guid playerID,Guid otherPlayerID,UInt32 times);

		void kickOutAllPlayer();

		void logPlayerLogin(PlayerBaseData baseData);

		void logPlayerLogout(PlayerBaseData baseData);

		void  updateSevenDayTask(Guid playerID,UInt32 eventType,UInt32 change);

	
		void initRoleInfoReq(UInt64 playerID,std::string name,UInt32 modleID);

		void write_log(UInt32 logType,UInt64 playerID, char * dest,UInt32 logLevel);

		void write_server_log(UInt32 logType,char * dest,UInt32 logLevel);


	 public:
	
		 //���޴���
		 void onPersistOffLineSaveReq(UInt64 playerUid);
	public:
		Set<Player >   getPlayerLvSet(Player * selfPlayer);
		
	public:
			//������Ľӿڴ���ɾͣ�������ϵͳ��Ϣ
		void sendSystemMsg(UInt64 tempId, std::string param1, UInt32 param2);
		//����GM��Ϣ
		void sendGMSystemMsg(std::string msg);

		void sendGMAwardBag(UInt64 playeruid,UInt64 awardId);

		void setForbidLogin(UInt32 i);

		void sendGMMsg(std::string msg,  UInt64 endtime, UInt32 term);

        //GM����ϵͳ�淨����
		//����ͨ�����̴���
		void gmResetTongBao(UInt64 playeruid);
		//�����������г�
		void gmResetMengChu(UInt64 playeruid);
       //������ʯ
		void gmResetYushiGF(UInt64 playeruid);
		//���а���
		void gmResetJiShiAB(UInt64 playeruid);
		//��ɽ����
		void gmResetBashanSL(UInt64 playeruid);
		//������ս
		void gmResetWuxianTZ(UInt64 playeruid);
       //��������
		void gmResetCourage(UInt64 playeruid);
		//��λ��
         void gmResetPaiWei(UInt64 playeruid);
		 //��λ������
		  void gmResetPaiWeiBuy(UInt64 playeruid);
		  //ʮ���ڹ��Ž�
		 void gmReset12HaoJiao(UInt64 playeruid);
		//��⿴���
		 void gmResetFoodsCook(UInt64 playeruid);
		 //�ͷ�����ѳ�һ��
		 void gmResetServantCall1(UInt64 playeruid);
		 //�ͷ�����ѳ�ʮ��
		 void gmResetServantCall10(UInt64 playeruid);
		 //ÿ������������
		 void gmResetDailyTask(UInt64 playeruid);
		 //ÿ��ǩ������ǩ�����
		 void gmResetMonthToday(UInt64 playeruid);
		  //���յ�¼���Ϊû��¼
		 void gmResetSevenDay(UInt64 playeruid);
		 //����Ѷ������м�¼
		 void gmResetSevenTrain(UInt64 playeruid);

		 //�ɳ��������
		 void gmResetGrowBuy(UInt64 playeruid);
		 //��ʱ����
		 void gmResetFenShi(UInt64 playeruid);
		 //���߽���
		 void gmResetOnlineAward(UInt64 playeruid);
		 //�ݲ���
		 void gmResetBaiCaiShen(UInt64 playeruid);
		 //��������
		 void gmResetStrengthBuy(UInt64 playeruid);

		 //���������ϵ�
		 void gmResetLianPu(UInt64 playeruid);
		 //ͨ���̵�����
		 void gmResetTongBaoS(UInt64 playeruid);
		 //�䱦���̵�����
		 void gmResetZBG(UInt64 playeruid);

		 //����è�̵�����
		 void gmResetServantShop(UInt64 playeruid);

		 const Vector<AchieveData>& getAchieveVec();

		 const Vector<DailyTaskData> & getDailyTaskVec();
    private:

		void initAllAchieve();	

		void initAllDailyTask();

		void addAchieveVec(const FirstAchievementMap& firstAchMap, UInt64 eventType);

		void addDailyTaskVec(DailyTaskTempMap* dailyTaskMap, UInt64 eventType);

        void loadPlayerDataFromMemery(const ConnId& connId, Player* player);

	public:
		//����ǿ������ڴ���ҹ��ܣ��Ժ����δ˽ӿ� �����˲�Ҫʹ��
		//���쳽
		void debugTestDestroyPlayer(const Guid& guid);

		void closeConnection(Player* player);

		void insertCreateSet(String name);

		bool isInCreateSet(String name);

		void eraseCreateSet(String name);

		void setLoginTime(UInt64 timemill);

		UInt64 getLoinTime();

		


	//��������ת����ʽ���ͻ�����ʶ���string���͵����ݽṹ
	private:

	private:
		void onClientConnectionClosed(const Event& ev);
		void onClientConnectionNotExist(const Event& ev);

		//���ÿ����������
	private:
		//���ÿ���������ݣ��������ͬһ�죬��ô������������
		//��ͬһ�죬����false
		//����ͬһ�죬����true�����������ж��Ƿ���ͻ��˷�����Ϣ
		bool checkReset(UInt64 time, Player * player);

	private:
       
        ServerId mServerId;
		Map<ConnId, Player*> mPlayerConnectionMap;
		Map<Guid, Player*> mPlayerGuidMap;
		StringMap<Player*> mPlayerNameMap;
        Map<Guid, Player*> mAccountGuidMap;
        StringMap<Player*> mAccountNameMap;
        UInt64 mLastSyncGuidBaseTime;

		UInt64 mLastRestGuidBaseTime;

		UInt64 mLastCheckOnlineWelfareTime;

		UInt64 mLastGetOnlineNumTime;

		UInt64 mForbidLogin;
		
		Guid mPlayerGuidBase;
		//sec ���ȫ��Ψһ��ItemUid
		Guid mItemGuidBase;

		Guid mLansquenetGuid;

		Guid mGuildGuid;

		Guid mEmailGuid;

		Guid mMaterialGuid;

		Guid mCharactorGuid;

		Guid mConsortGuid;

		Guid mTicketGuid;

		//��һ�����Ϣ���������ڶ�ȡ�����ɴ��� wwc
		Map<UInt64, BaseInfoData> mPlayerBaseInfoMap;
		//��ҹ�����Ϣ���������ڶ�ȡ�����ɴ��� wwc
		Map<UInt64, ConsortInfoData> mPlayerConsortInfoMap;

		UInt32 mDailyResetFlag ;

		List<GMMsg> mGMMsgList;

		Set<String> mCreatePlayerNameSet;

		Vector<AchieveData> mAllAchiveInit;

		//for test
		UInt64 gLoginTime;

		Vector<DailyTaskData> mAllDailyTaskInit;
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

