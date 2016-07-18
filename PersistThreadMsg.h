#ifndef __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum
	{
		PERSIST_LOAD_PLAYERDATA_REQ                = 1,         // ���������������
        PERSIST_LOAD_PLAYERDATA_RESP               = 2,         // ����������ݻ�Ӧ
        PERSIST_SYNC_BASE_GUID_NOTIFY              = 3,         // ͬ��itemguidͬ��֪ͨ
        PERSIST_LOAD_PLAYER_GUID_REQ               = 4,         // �������guid����
        PERSIST_LOAD_PLAYER_GUID_RESP              = 5,         // �������guid��Ӧ
        PERSIST_CREATE_PLAYER_REQ                  = 6,         // �����������
        PERSIST_CREATE_PLAYER_RESP                 = 7,         // ������һ�Ӧ
        PERSIST_ADD_ACCOUNT_REQ                    = 8,         // ����û��˻�����
        PERSIST_ADD_ACCOUNT_RESP                   = 9,         // ����û��˻���Ӧ
        
        PERSIST_DEL_ITEM_NOTIFY                    = 10,        // ɾ������֪ͨ
        PERSIST_UPDATE_ITEM_NOTIFY                 = 11,        // ���µ���֪ͨ
        PERSIST_UPDATE_EQUIP_NOTIFY                = 12,        // ����װ��֪ͨ
        
        PERSIST_UPDATE_HERO_DEBRIS_NOTIFY          = 14,        // ����Ӣ����Ƭ֪ͨ
        PERSIST_INSERT_ITEM_NOTIFY                 = 15,        // ��ӵ���֪ͨ
        PERSIST_INSERT_EQUIP_NOTIFY                = 16,        // ���װ��֪ͨ
      
        PERSIST_INSERT_HERO_DEBRIS_NOTIFY          = 18,        // ���Ӣ����Ƭ֪ͨ
        PERSIST_DEL_EQUIP_NOTIFY                   = 19,        // ɾ��װ��֪ͨ
       
        PERSIST_DEL_HERO_DEBRIS_NOTIFY             = 21,        // ɾ��Ӣ����Ƭ֪ͨ
        PERSIST_INSERT_RUNE_NOTIFY                 = 22,        // ��ӷ���֪ͨ 
        PERSIST_UPDATE_RUNE_NOTIFY                 = 23,        // ���·���֪ͨ
        PERSIST_DEL_RUNE_NOTIFY                    = 24,        // ɾ������֪ͨ
        PERSIST_INSERT_PET_DEBRIS_NOTIFY           = 25,        // ��ӳ�����Ƭ֪ͨ
        PERSIST_UPDATE_PET_DEBRIS_NOTIFY           = 26,        // ���³�����Ƭ֪ͨ
        PERSIST_DEL_PET_DEBRIS_NOTIFY              = 27,        // ɾ��������Ƭ֪ͨ
        PERSIST_INSERT_EQUIP_DEBRIS_NOTIFY         = 28,        // ���װ����Ƭ֪ͨ
        PERSIST_UPDATE_EQUIP_DEBRIS_NOTIFY         = 29,        // ����װ����Ƭ֪ͨ
        PERSIST_DEL_EQUIP_DEBRIS_NOTIFY            = 30,        // ɾ��װ����Ƭ֪ͨ

        PERSIST_INSERT_HERO_NOTIFY                 = 41,        // ���Ӣ��֪ͨ
        PERSIST_DEL_HERO_NOTIFY                    = 42,        // ɾ��Ӣ��֪ͨ
        PERSIST_UPDATE_HERO_NOTIFY                 = 43,        // ����Ӣ��֪ͨ
        PERSIST_INSERT_PET_NOTIFY                  = 44,        // ��ӳ���֪ͨ
        PERSIST_DEL_PET_NOTIFY                     = 45,        // ɾ������֪ͨ
        PERSIST_UPDATE_PET_NOTIFY                  = 46,        // ���³���֪ͨ


        PERSIST_UPDATE_PLAYER_DATA_NOTIFY          = 61,        // ͬ����һ�������֪ͨ
        PERSIST_UPDATE_PLAYER_ACCOUNT_DATA_NOTIFY  = 62,        // ͬ������˻�����֪ͨ
        PERSIST_RESET_PLAYER_TOLLGATE_DATA_NOTIFY  = 63,        // ���������������֪ͨ
        PERSIST_UPDATE_PLAYER_TOLLGATE_DATA_NOTIFY = 64,        // ͬ�������������֪ͨ
        PERSIST_INSERT_PLAYER_TOLLGATE_DATA_NOTIFY = 65,        // �����������
        PERSIST_UPDATE_PLAYER_TOLLGATE_AWARD_NOTIFY     = 66,   // ������Ҹ����콱��Ϣ    
        PERSIST_UPDATE_PLAYER_PRODUCE_DATA_NOTIFY  = 71,        // ͬ����Ҳ�������֪ͨ
        PERSIST_UPDATE_PLAYER_SYSTEM_AWARD_NOTIFY  = 72,        // �������ϵͳ��������֪ͨ
        PERSIST_UPDATE_PLAYER_ACTIVITY_DATA_NOTIFY = 73,        // ������һ����
        PERSIST_UPDATE_PLAYER_ACHIEVEMENT_DATA_NOTIFY   = 74,   // ������ҳɾ�����
        PERSIST_UPDATE_PLAYER_TOWER_DATA_NOTIFY    = 75,        // ���������������

        PERSIST_INSERT_EMAIL_NOTIFY                = 81,        // �����ʼ�����
        PERSIST_DELETE_EMAIL_NOTIFY                = 82,        // ɾ���ʼ�֪ͨ
        PERSIST_UPDATE_EMAIL_NOTIFY                = 83,        // �����ʼ�
		PERSIST_CLEAR_EMAIL_NOTIFY					 = 84,       // ��������ʼ�

        PERSIST_LOAD_OFFLINE_PLAYERBASEDATA_REQ    = 101,       // �������������������
        PERSIST_LOAD_OFFLINE_PLAYERBASEDATA_RESP   = 102,       // ��������������ݻ�Ӧ
        PERSIST_LOAD_OFFLINE_PLAYERDATA_REQ        = 103,       // �������������������
        PERSIST_LOAD_OFFLINE_PLAYERDATA_RESP       = 104,       // ��������������ݻ�Ӧ

        PERSIST_INSERT_CONSUME_RECORD_NOTIFY       = 121,       // ���Ӣ�����ļ�¼֪ͨ
        PERSIST_UPDATE_CONSUME_RECORD_NOTIFY       = 122,       // ����Ӣ�����ļ�¼֪ͨ
        PERSIST_DEL_CONSUME_RECORD_NOTIFY          = 123,       // ɾ��Ӣ�����ļ�¼֪ͨ

        PERSIST_INSERT_STORE_ITEM_NOTIFY           = 141,       // ����̳ǹ����¼
        PERSIST_UPDATE_STORE_ITEM_NOTIFY           = 142,       // �����̳ǹ����¼
        PERSIST_DEL_STORE_ITEM_NOTIFY              = 143,       // ɾ���̳ǹ����¼
        PERSIST_UPDATE_STORE_DATE_FLAG             = 144,       // �����̳ǹ�������
        PERSIST_UPDATE_SECRET_STORE_DATA_NOTIFY    = 145,       // ���������̵������Ϣ

        PERSIST_INSERT_EXCHANGE_ITEM_NOTIFY        = 161,       // ��Ӷһ���¼
        PERSIST_UPDATE_EXCHANGE_ITEM_NOTIFY        = 162,       // ���¶һ���¼
        PERSIST_DEL_EXCHANGE_ITEM_NOTIFY           = 163,       // ɾ���һ���¼
        PERSIST_UPDATE_EXCHANGE_DATE_FLAG          = 164,       // ���¶һ�����

        PERSIST_LOAD_ARENA_PLAYER_DATA_REQ         = 181,       // ���ؾ����������������
        PERSIST_LOAD_ARENA_PLAYER_DATA_RESP        = 182,       // ���ؾ�����������ݻ�Ӧ
        PERSIST_UPDATE_PLAYER_ARENA_BEST_RANK_NOTIFY    = 183,  // ������Ҿ������������֪ͨ
        PERSIST_UPDATE_ARENA_HERO_NOTIFY           = 184,       // ������Ҿ�����Ӣ��֪ͨ
        PERSIST_UPDATE_ARENA_PET_NOTIFY            = 185,       // ������Ҿ���������֪ͨ
        PERSIST_RESET_ARENA_BATTLE_CNT_NOTIFY      = 186,       // ���¾��������ս������

        PERSIST_LOAD_RANK_DATA_REQ                 = 201,       // �������а���������
        PERSIST_LOAD_RANK_DATA_RESP                = 202,       // �������а����ݻ�Ӧ
        PERSIST_UPDATE_PLAYER_RANK_BASE_NOTIFY     = 203,       // ����������а������Ϣ֪ͨ

        PERSIST_UPDATE_PLAYER_INFO_NOTIFY          = 211,       // ���������Ϣ֪ͨ
	
		PERSIST_LOAD_PLAYER_REQ_TEST			   = 212,		//sec ����Ӳ�����ҵ�½��Ϣ֪ͨ

		//manager����persistworker��������dirtyλ
		PERSIST_MANAGER_SETDIRTY_REQ			   = 213,		//

		PERSIST_MANAGER_SETDIRTY_RESP			   = 214,		//

		PERSIST_NOTIFY_WORKER_SAVEDB_REQ		   = 215,		//

		PERSIST_NOTIFY_WORKER_SAVEDB_RESP          = 216,		//

		PERSIST_OFF_LINE_SAVE_REQ				   = 217,		//

		PERSIST_FIRE_CONFIREM_SAVE_REQ			   = 300,		//ս����֤�浵

		PERSIST_BASE_DATA_SAVE_REQ				   = 301,		//��һ�����Ϣ�浵

		PERSIST_RANKING_SAVE_REQ				   = 302,		//��һ�����Ϣ�浵

		PERSIST_OFFLINE_REQ						   = 303,		//��һ�����Ϣ�浵

		PERSIST_FIND_SIMILAR_POWER				   = 304,		//��ѯս���������

		PERSIST_FIND_SIMILAR_POWER_RESP            = 305,         // ��ѯս��������ҷ���

		PERSIST_SHOP_SAVE_REQ					   = 306,		//�̳���Ϣ�浵

		PERSIST_SHOP_ITEM_SAVE_REQ				   = 307,		//�̳���Ʒ��Ϣ�浵

		PERSIST_CAT_COIN_SAVE_REQ				   = 308,		//è�Ҵ浵

		PERSIST_COIN_GROUP_SAVE_REQ				   = 309,		//è����ϴ浵




		PERSIST_JEWELRY_ADD                        = 400,       //֪ͨ���ݿ�������Ʒ
		PERSIST_JEWELRY_DEL                        = 401,       //֪ͨ���ݿ�ɾ����Ʒ
		PERSIST_JEWELRY_UPDATE                     = 402,        //֪ͨ���ݿ������Ʒ

		PERSIST_JEWELRYEQUIP_UPDATE                = 403,

		PERSIST_INLINE_ACTIVITY_POWER		   	   = 404,		//���û
		PERSIST_SERVERDATA_REQ				   	   = 405,		//��������
		PERSIST_ROBOTDATA_REQ				   	   = 406,		//����������

		PERSIST_SERVANT_INSERT                     = 500,       //֪ͨ���ݿ�����Ӷ��
		PERSIST_SERVANT_UPDATE                     = 501,       //֪ͨ���ݿ����Ӷ����
		PERSIST_SERVATN_DEL                        = 502,        //֪ͨ���ݿ�ɾ��Ӷ��
		

		PERSIST_SERVANTTREASURE_INSERT             = 600,		//���ݿ���ӱ���
		PERSIST_SERVANTTREASURE_UPDATE             = 601,       //���ݿ���±���

		PERSIST_LOADPLAYER_BY_ACCOUNT             = 700 ,//ͨ���˺ż������
		PERSIST_LOADPLAYER_BY_ACCOUNT_RESP    = 701, //ͨ���˺ż��ػظ�

		PERSIST_FRIENDOFFLINE_UPDATE     = 800,      //���ߺ��Ѹ������ݿ�
		PERSIST_FRIENDDATA_UPDATE      = 801 , 		//�������ݸ���
		PERSIST_LEAVETIME = 803	,
		PERSIST_FRIENDOFFLINE_ADD = 804,   //���ߺ������
		RERSIST_FRIENDOFFLINE_DEL  = 805,   //���ߺ���ɾ��

		PERSIST_ONLINELEVELRANK_UPDATE = 900, //�����ݿ�load���ݸ���������ҵȼ�����
		PERSIST_POWERRANK_UPDATE	 = 901, //�����ݿ�load���ݣ��������ս������

		PERSIST_ACHIEVE_UPDATE = 902,  //���³ɾ͵����ݿ�
		PERSIST_OFFLINEFRIEND_UPDATE = 903,//����������ҵĺ�������
		PERSIST_DAILYTASK_UPDATE = 904,//�ճ���������������
		PERSIST_DAILYACTIVE_UPDATE = 905, //�ճ���Ծ�ȱ����
		PERSIST_DAILYLASTTIME_UPDATE = 906, //�ճ���Ծ���ϴ�����ʱ�������Ϣ

		PERSIST_FASHION_INSERT = 1000,

		PERSIST_CHARACTOR_INSERT = 1002, //��ɫ�������ݿ�
		PERSIST_CHARACTOR_UPDATE = 1001 //��ɫ��������
	};

	struct Pos
	{
		Pos():x(0),y(0){}
		UInt32 x;//x����
		UInt32 y;//y����
	};
    
	struct PersistLoadPlayerGuidReq
	{
		Guid mAccountId;
		ConnId mConnId;
		String mAccountName;

		PersistLoadPlayerGuidReq(): mAccountId(0), mConnId(0), mAccountName("") {}

		LYNX_MESSAGE_3(PERSIST_LOAD_PLAYER_GUID_REQ, PersistLoadPlayerGuidReq, Guid, mAccountId,
			ConnId, mConnId, String, mAccountName);
	};

    struct PersistLoadPlayerDataResp 
    {
        PersistLoadPlayerDataResp() : mConnId(0), mPlayerGuid(0), merrorId(0), mAccountName("") {}

        ConnId mConnId;
        errorId merrorId;
        Guid mPlayerGuid;
        String mAccountName;
        PlayerData mPlayerData;

        LYNX_MESSAGE_4(PERSIST_LOAD_PLAYERDATA_RESP, PersistLoadPlayerDataResp,
            ConnId, mConnId, errorId, merrorId, Guid, mPlayerGuid, 
            PlayerData, mPlayerData);
    };

	struct PersistFindSimilarPowerResp 
	{
		PersistFindSimilarPowerResp() : playerID(0),times(0){}

		Guid playerID;
		UInt32 times;//10000���˸�����ʶ
		List<Guid> playerIDList;

		LYNX_MESSAGE_3(PERSIST_FIND_SIMILAR_POWER_RESP, PersistFindSimilarPowerResp,
			Guid, playerID,UInt32, times,List<Guid>, playerIDList);
	};

	
	struct PersistLoadOfflineDataResp 
	{
// 		PersistLoadOffLineDataResp() :{}

		PlayerBaseData mBaseData;


		LYNX_MESSAGE_1(PERSIST_OFFLINE_REQ, PersistLoadOfflineDataResp,PlayerBaseData, mBaseData);
	};

    struct PersistSyncBaseGuidNotify
    {
        PersistSyncBaseGuidNotify() : mPlayerGuid(0),mItemGuid(0),mLansquenetGuid(0),mGuildGuid(0),mEmailGuid(0),mMaterialGuid(0),
		mCharactorGuid(0){}

        UInt64 mPlayerGuid;

		UInt64 mItemGuid;

		UInt64 mLansquenetGuid;

		UInt64 mGuildGuid;

		UInt64 mEmailGuid;

		UInt64 mMaterialGuid;

		UInt64 mCharactorGuid;

        LYNX_MESSAGE_7(PERSIST_SYNC_BASE_GUID_NOTIFY, PersistSyncBaseGuidNotify,
            UInt64, mPlayerGuid,UInt64, mItemGuid,UInt64, mLansquenetGuid,UInt64, mGuildGuid,UInt64, mEmailGuid, UInt64, mMaterialGuid,
			UInt64, mCharactorGuid);
    };

    struct PersistLoadPlayerGuidResp
    {
        Guid mPlayerGuid;
        Guid mAccountId;
        ConnId mConnId;
        errorId merrorId;
        String mAccountName;

        PersistLoadPlayerGuidResp(): merrorId(0), mPlayerGuid(0), mConnId(0),
            mAccountName(""), mAccountId(0) {}

        LYNX_MESSAGE_5(PERSIST_LOAD_PLAYER_GUID_RESP, PersistLoadPlayerGuidResp, Guid, mPlayerGuid,
            errorId, merrorId, ConnId, mConnId, String, mAccountName, Guid, mAccountId);
    };

   	
	struct PersistLoadPlayerReqTest
	{
		std::string mPlayerName;
		ConnId mConnId;
		LYNX_MESSAGE_2(PERSIST_LOAD_PLAYER_REQ_TEST, PersistLoadPlayerReqTest, std::string, mPlayerName,ConnId,mConnId);
		
	};

//=======================================================================================================================================================================>
	//����ϷBOC��������

	struct PersistLoadPlayerDataReq 
	{
		PersistLoadPlayerDataReq() : mConnId(0), mPlayerGuid(0){}

		ConnId mConnId;
		Guid mPlayerGuid;
		

		LYNX_MESSAGE_2(PERSIST_LOAD_PLAYERDATA_REQ, PersistLoadPlayerDataReq,
			ConnId, mConnId, Guid, mPlayerGuid);
	};

	struct PersistCreatePlayerReq
	{
		PersistCreatePlayerReq():mConnId(0),mPlayerModelId(0),mName(""),mAccountId(0){}

		ConnId mConnId;
		UInt32 mPlayerModelId;
		String mName;
		UInt64 mAccountId;

		LYNX_MESSAGE_4(PERSIST_CREATE_PLAYER_REQ, PersistCreatePlayerReq,
			ConnId, mConnId, UInt32, mPlayerModelId,	String, mName, UInt64, mAccountId);
	};

	struct PersistCreatePlayerResp
	{
		PersistCreatePlayerResp():mRes(0),mPlayerUid(0){}
		UInt16 mRes;
		UInt64 mPlayerUid;
		ConnId mConnId;
		LYNX_MESSAGE_3(PERSIST_CREATE_PLAYER_RESP, PersistCreatePlayerResp,
			UInt16, mRes, UInt64, mPlayerUid, ConnId, mConnId);
		
	};

	struct PersistLoadPlayerByAccount
	{
		PersistLoadPlayerByAccount():mConnId(0),mAccountId(0){}
	
		UInt64 mAccountId;
		ConnId mConnId;
		LYNX_MESSAGE_2(PERSIST_LOADPLAYER_BY_ACCOUNT, PersistLoadPlayerByAccount,
			UInt64 ,mAccountId, ConnId, mConnId);
		
	};

	struct PersistLoadPlayerByAccountResp
	{
		PersistLoadPlayerByAccountResp():mConnId(0),mAccountId(0),mErrorId(0),mPlayerUid(0){}
		errorId mErrorId;
		UInt64 mAccountId;
		ConnId mConnId;
		UInt64 mPlayerUid;
		LYNX_MESSAGE_4(PERSIST_LOADPLAYER_BY_ACCOUNT_RESP, PersistLoadPlayerByAccountResp,
			UInt64 ,mAccountId, ConnId, mConnId, errorId ,mErrorId,UInt64,mPlayerUid);

	};

	//persistmanager ֪ͨpersistsystem dirtybit
	struct PersistManagerSetDirtyReq
	{
		PersistManagerSetDirtyReq():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_MANAGER_SETDIRTY_REQ, PersistManagerSetDirtyReq,
			UInt64, mPlayerUid, UInt32, mDirtyBit);

	};

	struct PersistManagerSetDirtyResp
	{
		PersistManagerSetDirtyResp():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_MANAGER_SETDIRTY_RESP, PersistManagerSetDirtyResp,
			UInt64, mPlayerUid, UInt32, mDirtyBit);
	};

	struct PersistNotifyWorkerSaveDbReq
	{
		PersistNotifyWorkerSaveDbReq():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_NOTIFY_WORKER_SAVEDB_REQ, PersistNotifyWorkerSaveDbReq,
			UInt64, mPlayerUid, UInt32, mDirtyBit);
	};

	struct PersistNotifyWorkerSaveDbResp
	{
		PersistNotifyWorkerSaveDbResp():mPlayerUid(0),mDirtyBit(0),merrorId(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;
		errorId merrorId;

		LYNX_MESSAGE_3(PERSIST_NOTIFY_WORKER_SAVEDB_RESP, PersistNotifyWorkerSaveDbResp,
			UInt64, mPlayerUid, UInt32, mDirtyBit,errorId, merrorId);
	};

	struct PersistDelItemNotify
	{
		Guid mItemGuid;
		Guid mPlayerGuid;
		// ������λ(��ͨ,װ��,װ����Ƭ,Ӣ����Ƭ)
		UInt8 mField;

		PersistDelItemNotify(): mItemGuid(0), mPlayerGuid(0), mField(0) {}

		LYNX_MESSAGE_3(PERSIST_DEL_ITEM_NOTIFY, PersistDelItemNotify, Guid, mItemGuid,
			Guid, mPlayerGuid, UInt8, mField);
	};


	struct PersistUpdateItemNotify
	{
		ItemData mItemData;
		Guid mPlayerGuid;

		PersistUpdateItemNotify(): mPlayerGuid(0) {}

		LYNX_MESSAGE_2(PERSIST_UPDATE_ITEM_NOTIFY, PersistUpdateItemNotify, ItemData, mItemData, Guid, mPlayerGuid);
	};

	struct PersistInsertItemNotify
	{
		ItemData mItemData;
		Guid mPlayerGuid;

		PersistInsertItemNotify(): mPlayerGuid(0) {} 

		LYNX_MESSAGE_2(PERSIST_INSERT_ITEM_NOTIFY, PersistInsertItemNotify, ItemData, mItemData, Guid, mPlayerGuid);
	};

	
	struct PersistOffLineSaveReq
	{
		
		UInt64 mPlayerUid;
		
		PersistOffLineSaveReq():mPlayerUid(0){}

		LYNX_MESSAGE_1(PERSIST_OFF_LINE_SAVE_REQ, PersistOffLineSaveReq,  UInt64, mPlayerUid);
	};


	struct PersistFireConfirmSaveReq
	{
		Guid playerGuid;

		PlayerFireConfirmData FireConfirmData;

		PersistFireConfirmSaveReq():playerGuid(0){}

		LYNX_MESSAGE_2(PERSIST_FIRE_CONFIREM_SAVE_REQ, PersistFireConfirmSaveReq,  Guid, playerGuid,PlayerFireConfirmData, FireConfirmData);
	};


	struct PersistPlayerSinglePropertySaveReq
	{

		Guid playerGuid;
		String strFlag;
		String strData;

 		PersistPlayerSinglePropertySaveReq():playerGuid(0),strFlag(""),strData(""){}

		LYNX_MESSAGE_3(PERSIST_BASE_DATA_SAVE_REQ, PersistPlayerSinglePropertySaveReq,  Guid, playerGuid,String, strFlag, String, strData);
	};

	struct PersistFindSimilarPowerReq
	{

		Guid playerGuid;
		UInt32 times;
		UInt32 high;
		UInt32 low;

		PersistFindSimilarPowerReq():playerGuid(0),times(1),high(0),low(0){}

		LYNX_MESSAGE_4(PERSIST_FIND_SIMILAR_POWER, PersistFindSimilarPowerReq,  Guid, playerGuid,UInt32, times,UInt32, high,UInt32, low);
	};

	struct PersistServerDataReq
	{
		UInt32 gRechargeNum ;//��ֵ����
		UInt32 gServerRobotRefreshFlag ;//0���µ��������
		UInt32 gServerSpare2 ;//��ֵ����
		UInt32 gServerSpare3 ;//��ֵ����
		UInt32 gServerSpare4 ;//��ֵ����
		UInt32 gServerSpare5 ;//��ֵ����

		PersistServerDataReq():gRechargeNum(1),gServerRobotRefreshFlag(0),gServerSpare2(0),gServerSpare3(0),gServerSpare4(0),gServerSpare5(0){}

		LYNX_MESSAGE_6(PERSIST_SERVERDATA_REQ, PersistServerDataReq,UInt32,gRechargeNum,UInt32, gServerRobotRefreshFlag,UInt32, gServerSpare2,UInt32, gServerSpare3,UInt32, gServerSpare4,UInt32, gServerSpare5);
	};
	struct PersistRobotDataReq
	{
		Guid uuid ;//
		UInt32 uid ;//
		String name ;//
		UInt32 level ;//
		UInt32 viplevel ;//
		UInt32 power ;//
// INSERT INTO robot(uuid,uid,name,level,viplevel,power)VALUES(1111,1,"1111",33,4,1312)
		PersistRobotDataReq():uuid(0),uid(0),name(""),level(0),viplevel(0),power(0){}

		LYNX_MESSAGE_6(PERSIST_ROBOTDATA_REQ, PersistRobotDataReq,Guid,uuid,UInt32, uid,String, name,UInt32, level,UInt32, viplevel,UInt32, power);
	};


	struct PersistInlineActivityReq
	{
		PersistInlineActivityReq():playerGuid(0),type(0),m_GrowFoundBuyFlag(0),m_MonthSignCount(0),m_MonthSignTime(0),m_OnlineWelFareOnlineTime(0),m_PeopleWelfareRechargeNum(0),
			m_FirstLoginTime(0), m_SevenLoginGotCount(0),m_LastGetTime(0),m_LoginTime(0),m_TimeAwardRefreshTime(0){}		

		Guid playerGuid;
		UInt32 type;
		//�ɳ�����
		UInt32 m_GrowFoundBuyFlag;//1�Ѿ�����0δ����
		List<UInt32> m_GrowFoundGotList;//����ȡ�Ľ���

		//��ǩ��
		UInt32 m_MonthSignCount;//ǩ���ڼ���
		UInt32 m_MonthSignTime;//ǩ����ʱ��

		//����ʱ������
		UInt32 m_OnlineWelFareOnlineTime;//����ʱ��
		List<UInt32> m_OnlineWelFareGotList;//����ȡ�Ľ���

		//ȫ��������
		UInt32 m_PeopleWelfareRechargeNum ;//��ֵ����
		List<UInt32> m_PeopleWelfareGotList;//����ȡ�Ľ���

		//7���¼
		UInt32 m_FirstLoginTime;//�״ε�½ʱ��
		UInt32 m_LastGetTime;//��һ���콱ʱ��
		UInt32 m_SevenLoginGotCount;//����ȡ����
		//7��
		List<KeyValue> m_SevenDayTaskGotList;//����ȡ�Ľ���
		List<KeyList> m_SevenDayTaskFinishList;//����ȡ�Ľ���

		//�ֶ�ʱ��
		UInt32 m_TimeAwardRefreshTime;//����ʱ��
		List<UInt32> m_TimeAwardGotList;//����ȡ�Ľ���

		//��¼
		UInt32 m_LoginTime;//��½ʱ��


		LYNX_MESSAGE_18(PERSIST_INLINE_ACTIVITY_POWER,PersistInlineActivityReq,UInt32, type,Guid, playerGuid,UInt32, m_GrowFoundBuyFlag,List<UInt32>, m_GrowFoundGotList, UInt32, m_MonthSignCount, UInt32, m_MonthSignTime,
			UInt32, m_OnlineWelFareOnlineTime,List<UInt32>, m_OnlineWelFareGotList,UInt32, m_PeopleWelfareRechargeNum,List<UInt32>, m_PeopleWelfareGotList,
			UInt32, m_FirstLoginTime,UInt32, m_LastGetTime, UInt32, m_SevenLoginGotCount, List<KeyValue>, m_SevenDayTaskGotList, UInt32, m_TimeAwardRefreshTime,
			List<UInt32>, m_TimeAwardGotList,UInt32, m_LoginTime,List<KeyList>, m_SevenDayTaskFinishList);
	};


	struct PersistAddJewelryNotify
	{
		
		PersistAddJewelryNotify():m_nPlayerUid(0){}

		JewelryData m_jewelryData;
		UInt64 m_nPlayerUid;
		
		
		LYNX_MESSAGE_2(PERSIST_JEWELRY_ADD,PersistAddJewelryNotify, UInt64, m_nPlayerUid,JewelryData ,m_jewelryData);
		
	};
	
	struct PersistRankingSaveReq
	{
		
		UInt32 type;
		UInt32 rank;
		Guid playerID;
		UInt32 val ;

		PersistRankingSaveReq():type(0),rank(0),playerID(0),val(0){}

		LYNX_MESSAGE_4(PERSIST_RANKING_SAVE_REQ, PersistRankingSaveReq, UInt32, type,UInt32, rank,Guid, playerID, UInt32, val);
	};

	struct PersistShopSaveReq
	{
		UInt32 shopType;
		UInt32 refreshTimes;
		Guid playerID;
		UInt32 refreshTime;
	
		PersistShopSaveReq():shopType(0),refreshTimes(0),playerID(0),refreshTime(0){}

		LYNX_MESSAGE_4(PERSIST_SHOP_SAVE_REQ, PersistShopSaveReq, UInt32, shopType,UInt32, refreshTimes,Guid, playerID,UInt32, refreshTime);
	};

	struct CatCoinSaveReq
	{
		UInt32 catCoinID;
		UInt32 num;
		Guid playerID;

		CatCoinSaveReq():catCoinID(0),num(0),playerID(0){}

		LYNX_MESSAGE_3(PERSIST_CAT_COIN_SAVE_REQ, CatCoinSaveReq, UInt32, catCoinID,UInt32, num,Guid, playerID);
	};

	struct CoinGroupSaveReq
	{
		UInt32 groupID;
		Guid playerID;

		CoinGroupSaveReq():groupID(0),playerID(0){}

		LYNX_MESSAGE_2(PERSIST_COIN_GROUP_SAVE_REQ, CoinGroupSaveReq, UInt32, groupID,Guid, playerID);
	};


	struct PersistShopItemSaveReq
	{

		UInt32 shopType;
		UInt32 position;
		Guid playerID;
		UInt32 buyTime;
		UInt32 buyTimes;
		UInt32 resType;
		UInt32 subType;
		UInt32 num;

		PersistShopItemSaveReq():shopType(0),position(0),playerID(0),buyTime(0),buyTimes(0),resType(0),subType(0),num(0){}

		LYNX_MESSAGE_8(PERSIST_SHOP_ITEM_SAVE_REQ, PersistShopItemSaveReq, UInt32, shopType,UInt32, position,Guid, playerID,UInt32, buyTime,UInt32, buyTimes,UInt32, resType,UInt32, subType,UInt32, num);
	};


	struct PersistOfflineDataReq
	{
		Guid playerID;
	
		PersistOfflineDataReq():playerID(0){}

		LYNX_MESSAGE_1(PERSIST_OFFLINE_REQ, PersistOfflineDataReq,Guid, playerID);
	};

	struct PersistDelJewelryNotify
	{
		PersistDelJewelryNotify(): m_nPlayerUid(0), m_nJewelryUid(0) {}

		UInt64 m_nPlayerUid;
		UInt64 m_nJewelryUid;
		
		LYNX_MESSAGE_2(PERSIST_JEWELRY_DEL, PersistDelJewelryNotify, UInt64, m_nPlayerUid,
			UInt64, m_nJewelryUid);
	};

	struct PersistUpdateJewelryNotify
	{

		PersistUpdateJewelryNotify():m_nPlayerUid(0){}

		JewelryData m_jewelryData;
		UInt64 m_nPlayerUid;


		LYNX_MESSAGE_2(PERSIST_JEWELRY_UPDATE,PersistUpdateJewelryNotify, UInt64, m_nPlayerUid,JewelryData ,m_jewelryData);

	};

	struct PersistUpdateJewelryEquipNotify
	{
		PersistUpdateJewelryEquipNotify():m_nPlayerUid(0){}
	    JewelryEquipData m_jewelryEquipData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_JEWELRYEQUIP_UPDATE,PersistUpdateJewelryEquipNotify, UInt64, m_nPlayerUid,JewelryEquipData ,m_jewelryEquipData);
	};

	  
	struct PersistAddServantNotify
	{
		PersistAddServantNotify():m_nPlayerUid(0){}
		ServantData m_servantData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANT_INSERT,PersistAddServantNotify, UInt64, m_nPlayerUid,ServantData ,m_servantData);
	};

	struct PersistUpdateServantNotify
	{
		PersistUpdateServantNotify():m_nPlayerUid(0){}
		ServantData m_servantData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANT_UPDATE,PersistUpdateServantNotify, UInt64, m_nPlayerUid,ServantData ,m_servantData);
	};

	struct PersistDelServantNotify
	{
		PersistDelServantNotify():m_nPlayerUid(0),m_nServantUid(0){}
		UInt64 m_nServantUid;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVATN_DEL,PersistDelServantNotify, UInt64, m_nServantUid,UInt64 ,m_nPlayerUid);
	};

	struct PersistAddServantTreasureNotify
	{
		PersistAddServantTreasureNotify():m_nPlayerUid(0){}
		ServantTreasure m_servantTreasure;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANTTREASURE_INSERT,PersistAddServantTreasureNotify, UInt64, m_nPlayerUid,ServantTreasure ,m_servantTreasure);
	};
	
	struct PersistUpdateServantTreasureNotify
	{
		PersistUpdateServantTreasureNotify():m_nPlayerUid(0){}
		ServantTreasure m_servantTreasure;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANTTREASURE_UPDATE,PersistUpdateServantTreasureNotify, UInt64, m_nPlayerUid,ServantTreasure ,m_servantTreasure);
	};

	struct PersistUpdateFriendOffLineNotify
	{
		PersistUpdateFriendOffLineNotify():m_nPlayerUid(0), m_nType(0),m_nMyselfUid(0){}
		UInt32 m_nType; //����,0��ʾ���Ӻ��ѣ�1��ʾɾ������
		UInt64 m_nPlayerUid; //Ҫɾ�����ѵ�uid
		UInt64 m_nMyselfUid; //�Լ���uid

		LYNX_MESSAGE_3(PERSIST_FRIENDOFFLINE_UPDATE, PersistUpdateFriendOffLineNotify, UInt64, m_nPlayerUid,UInt32, m_nType,
			UInt64 ,m_nMyselfUid);
	};

	struct PersistUpdateFriendNotify
	{
		PersistUpdateFriendNotify():m_nPlayerUid(0){}
		FriendData m_friendData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_FRIENDDATA_UPDATE, PersistUpdateFriendNotify, UInt64, m_nPlayerUid, FriendData ,m_friendData);
	};

	struct PersistUpdateLeaveTime
	{
			PersistUpdateLeaveTime():m_nPlayerUid(0), m_nLeaveTime(0){}
			UInt64 m_nPlayerUid;
			UInt64 m_nLeaveTime;
			LYNX_MESSAGE_2(PERSIST_LEAVETIME, PersistUpdateLeaveTime, UInt64, m_nPlayerUid, UInt64 ,m_nLeaveTime);
		
	};

	struct PersistAddFriendOffLineNotify
	{
		PersistAddFriendOffLineNotify():m_nPlayerUid(0),m_nMyselfUid(0),friendStr(""),blackStr(""){}	
		UInt64 m_nPlayerUid; //Ҫ��Ӻ��ѵ�uid
		UInt64 m_nMyselfUid; //�Լ���uid
		String friendStr;
		String blackStr;
		UInt32 friendCount;
		LYNX_MESSAGE_5(PERSIST_FRIENDOFFLINE_ADD, PersistAddFriendOffLineNotify, UInt64, m_nPlayerUid,
			UInt64 ,m_nMyselfUid, String, friendStr, String, blackStr, UInt32, friendCount);
	};
		
	struct PersistDelFriendOffLineNotify
	{
		PersistDelFriendOffLineNotify():m_nPlayerUid(0),m_nMyselfUid(0),friendStr(""),blackStr(""){}	
		UInt64 m_nPlayerUid; //Ҫɾ�����ѵ�uid
		UInt64 m_nMyselfUid; //�Լ���uid
		String friendStr;
		String blackStr;
		UInt32 friendCount;
		LYNX_MESSAGE_5(RERSIST_FRIENDOFFLINE_DEL, PersistDelFriendOffLineNotify, UInt64, m_nPlayerUid,
			UInt64 ,m_nMyselfUid, String, friendStr, String, blackStr, UInt32, friendCount);
	};

	struct PerisistUpdateOnlineLvRank
	{
		PerisistUpdateOnlineLvRank():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_ONLINELEVELRANK_UPDATE, PerisistUpdateOnlineLvRank, UInt64 ,m_nTime);
	};

	struct PersistGetPowerRank
	{
		PersistGetPowerRank():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_POWERRANK_UPDATE, PersistGetPowerRank, UInt64 ,m_nTime);
	};


	struct PersistAddEmail
	{
		PersistAddEmail(){}
		EmailData mEmailData;
		LYNX_MESSAGE_1(PERSIST_INSERT_EMAIL_NOTIFY,PersistAddEmail, EmailData,mEmailData);
		
	};

	struct PersistDelEmail
	{
		PersistDelEmail(): mEmailUid(0){}
		UInt64 mEmailUid;
		LYNX_MESSAGE_1(PERSIST_DELETE_EMAIL_NOTIFY,PersistDelEmail, UInt64,mEmailUid);

	};

	struct PersistUpdateEmail
	{
		PersistUpdateEmail(){}
		EmailData mEmailData;
		LYNX_MESSAGE_1(PERSIST_UPDATE_EMAIL_NOTIFY,PersistUpdateEmail, EmailData,mEmailData);

	};

	struct PersistClearEmail
	{
		PersistClearEmail(){}
		UInt64 mPlayerUid;
		LYNX_MESSAGE_1(PERSIST_CLEAR_EMAIL_NOTIFY,PersistClearEmail, UInt64, mPlayerUid);
		
	};

	struct FashionAdd
	{
		FashionAdd():mPlayerUid(0), mFashionUid(0), mFashionId(0){}
		UInt64 mPlayerUid;
		UInt64 mFashionUid;
		UInt64 mFashionId;
		LYNX_MESSAGE_3(PERSIST_FASHION_INSERT,FashionAdd, UInt64, mPlayerUid,UInt64, mFashionUid,UInt64, mFashionId);
		
	};

	struct CharactorAdd
	{
		CharactorAdd():charactorUid(0), charactorId(0), fashionId(0), playerUid(0){}
		UInt64 charactorUid;
		UInt64 charactorId;
		UInt64 fashionId;
		UInt64 playerUid;
		LYNX_MESSAGE_4(PERSIST_CHARACTOR_INSERT,CharactorAdd, UInt64, charactorUid,UInt64, charactorId,UInt64, fashionId,UInt64, playerUid);
	};

	struct CharactorUpdate
	{
		CharactorUpdate():charactorUid(0), fashionId(0){}
		UInt64 charactorUid;
		UInt64 fashionId;
		LYNX_MESSAGE_2(PERSIST_CHARACTOR_UPDATE,CharactorUpdate, UInt64, charactorUid,UInt64, fashionId);
	};

	struct AchieveUpdateMsg
	{
		AchieveUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		AchieveData achieveData;
		LYNX_MESSAGE_2(PERSIST_ACHIEVE_UPDATE,AchieveUpdateMsg, UInt64, playerUid, AchieveData, achieveData);
	};
	//�������ߺ��������ɾ�
	struct OffLineFriendUpdateMsg
	{
		OffLineFriendUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		UInt32 count;
		LYNX_MESSAGE_2(PERSIST_OFFLINEFRIEND_UPDATE,OffLineFriendUpdateMsg, UInt64, playerUid, UInt32, count);
		
	};

	struct PersistDailyTaskUpdateMsg
	{
		PersistDailyTaskUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		DailyTaskData dailyTask;
		LYNX_MESSAGE_2(PERSIST_DAILYTASK_UPDATE,PersistDailyTaskUpdateMsg, UInt64, playerUid, DailyTaskData, dailyTask);

	};

	struct PersistDailyActiveUpdateMsg
	{
		PersistDailyActiveUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		PlayerDailyActiveData dailyacvData;
		LYNX_MESSAGE_2(PERSIST_DAILYACTIVE_UPDATE,PersistDailyActiveUpdateMsg, UInt64, playerUid, 	PlayerDailyActiveData, dailyacvData);

	};


	struct PersistDailyLastTimeUpdateMsg
	{
		PersistDailyLastTimeUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		 UInt64 time;
		LYNX_MESSAGE_2(PERSIST_DAILYLASTTIME_UPDATE,PersistDailyLastTimeUpdateMsg, UInt64, playerUid, 	UInt64, time);

	};
	
	

	

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__
