#ifndef __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

#include "MainThread.h"
#include "Player.h"

#include "Guild.h"

namespace Lynx
{
	
	class LogicSystem : public MainThread, public Singleton<LogicSystem>
	{
	public:
        LogicSystem();
		virtual ~LogicSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

		void  dailyRefresh(const UInt64& accTime);

        bool reloadTable(const String& tableName);
        bool reloadAllTable();

        void setPlayerGuid(const Guid& guidBase) { mPlayerGuidBase = guidBase; }

		const Guid& getPlayerGuid(){return mPlayerGuidBase;}
 
		//������itemGuid
        Guid generateItemGuid() {++mItemGuidBase;
			baseUidPersist();
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
		baseUidPersist();
		return mMaterialGuid; }

		void setMaterialGuid(const Guid& materialGuid)
		{
			mMaterialGuid = materialGuid;
		}

		//���ɽ�ɫid���Լ��ճ�����id
		Guid generateCharactorGuid(){++mCharactorGuid;
		baseUidPersist();
		return mCharactorGuid;}

		void setCharactorGuid(const Guid& charactorGuid)
		{
			mCharactorGuid = charactorGuid;
		}


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

		void updateDailyResetData(Player *player,UInt32 flag);

		void checkPlayerDataFromDB(Player& player);

		bool combinSendData(Guid playerID,UInt32 resType,Json::Value jvalue);

		bool combinSendDataTest(Guid playerID,UInt32 resType,const Json::Value &jvalue);

	public:
		//�����Ϣ
		std::string getDetailInfo(std::string playerUidStr); 

		//��ȡ��Ҹ�Ҫ��Ϣ
		bool getBaseInfo(UInt64 playerUid, BaseInfoData & baseInfoData);

		//��ȡbaseinfomap
		const Map<UInt64, BaseInfoData> &getBaseInfoMap();

		void insertBaseInfo(const BaseInfoData & baseInfoData);
		//��½������ʱ���������Ϣ
		void updateBaseInfo(const PlayerBaseData &baseData);

		void updateBaseInfo(const BaseInfoData & baseInfoData);

		Guid listRandOne(List<Guid> playerIDList);

			
		void dealSimililarPower(Guid playerID,Guid otherPlayerID,UInt32 times);

		void kickOutAllPlayer();

	 public:
	
	public:
		Set<Player >   getPlayerLvSet(Player * selfPlayer);
		

    private:
        void loadPlayerDataFromMemery(const ConnId& connId, Player* player);

	public:
		//����ǿ������ڴ���ҹ��ܣ��Ժ����δ˽ӿ� �����˲�Ҫʹ��
		//���쳽
		void debugTestDestroyPlayer(const Guid& guid);

		void closeConnection(Player* player);

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
		
		Guid mPlayerGuidBase;
		//sec ���ȫ��Ψһ��ItemUid
		Guid mItemGuidBase;

		Guid mLansquenetGuid;

		Guid mGuildGuid;

		Guid mEmailGuid;

		Guid mMaterialGuid;

		Guid mCharactorGuid;

		Map<UInt64, BaseInfoData> mPlayerBaseInfoMap;

		UInt32 mDailyResetFlag ;
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

