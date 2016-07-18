#ifndef __LYNX_COMMON_LIB_PLAYER_DATA_H__
#define __LYNX_COMMON_LIB_PLAYER_DATA_H__

#include "Definition.h"
#include "ItemData.h"


#include "SkillData.h"
#include "BufferData.h"
#include "FashionData.h"
#include "HoarStoneData.h"
#include "HeroEquipData.h"
#include "MaterialData.h"
#include "GemData.h"
#include "JewelryData.h"
#include "FriendData.h"
#include "EmailData.h"
#include "CharactorData.h"
#include "AchieveData.h"

#include "../FireConfirm/Box.h"
// #include "../FireConfirm/Shop.h"
#include"../jsoncpp/include/json/json.h"
// #include "../FireConfirm/Character.h"
//  #include "../FireConfirm/Character.h"

namespace Lynx
{
 
//===================================================================================================================================

//BOC �߼���ͨ�ŵ�playerdata 
	
	struct KeyValue
	{
		KeyValue():key(0),value(0){}
		UInt32 key;
		UInt32 value;
		LYNX_S11N_2(KeyValue,UInt32,key,UInt32,value);
	};
	struct Goods
	{
		Goods(): resourcestype(0),subtype(0),num(0){}
		UInt32 resourcestype;	//1 ��һ����� 2 �ڶ������ 3 �̶�����
		UInt32 subtype;			//1 ��� 2 Ԫ�� 100 ����װ��
		Int32 num;				//����
		LYNX_S11N_3(Goods,UInt32, resourcestype,UInt32, subtype,Int32, num);
	};
	struct Award
	{
		List<Goods> award;
		LYNX_S11N_1(Award,List<Goods>, award);
	};
	struct Card
	{
		List<Goods> card;
		LYNX_S11N_1(Card,List<Goods>, card);
	};
	struct Record
	{
		Record(): playerID(0),name(""),modleID(0),val(0){}
		Guid playerID;
		std::string name;
		UInt32 modleID;
		UInt32 val;
		LYNX_S11N_4(Record,Guid, playerID,std::string, name,UInt32, modleID,UInt32, val);

	};
	struct CatCoin
	{
		CatCoin(): catCoinID(0),num(0){}
		UInt32 catCoinID;
		Int32 num;
		LYNX_S11N_2(CatCoin,UInt32, catCoinID,Int32, num);
	};

	typedef Map<UInt32,Record> MapRecord;
	
	struct NeedSaveResetStages
	{
		UInt64 playerID;
		UInt32 stageID;
		UInt32 val;

		LYNX_S11N_3(NeedSaveResetStages,UInt64, playerID,UInt32, stageID,UInt32, val);
	};

	struct BaseInfoData
	{
		UInt64 playerUid;
		UInt32 modelId;
		UInt64 power;
		String name;
		UInt32 level;
		UInt32 vipLv;
		UInt64 leaveTime;		
	};

	struct PlayerBaseData
	{
		
		PlayerBaseData() : m_nPlayerID(0), m_nModelID(0),m_strPlayerName(""),m_strMood(""),
			          m_nLevel(0), m_nLevelExp(0),  mDirtyFlag(false),
			          m_nVipLevel(0), m_nVipExp(0), m_nCoin(0), m_nGold(0), 
					  m_nStrength(0),m_nFame(0),m_nPower(0),m_nLeavTime(0),m_strOldName(""){}
		
		//��ҽ�ɫID 
		Guid m_nPlayerID;

		//ģ��ID
		UInt32 m_nModelID;


		std::string  m_strPlayerName;

		//���˳ƺ�
		std::string  m_strPlayerTitle;
		
		//��ҵȼ�
		UInt32 m_nLevel;
		
		//��ҵȼ�����
		Guid m_nLevelExp;

		//���vip�ȼ�
		UInt32 m_nVipLevel;

		//���vip����
		Guid	m_nVipExp;

		//��ҽ��
		Guid	m_nCoin;

		//���RMB����
		Guid	m_nGold;					

		//�������
		UInt32  m_nStrength;

		Guid m_accountID;//id

		//�������
		Guid  m_nFame;

		//���ս��
		UInt32  m_nPower;

		std::string m_strMood;

		//ɨ�����ߣ�����ɨ������

		List<UInt32> m_listSweepCount;

		//bool ���ı�������ʾ�Ƿ������ݸ���
		bool mDirtyFlag;

		UInt64 m_nLeavTime;

		std::string m_strOldName;
	
		LYNX_S11N_17(PlayerBaseData, Guid, m_nPlayerID, UInt32 ,m_nModelID,
			         std::string ,m_strPlayerName, UInt32,m_nLevel, 
			         Guid, m_nLevelExp, UInt32, m_nVipLevel, Guid,	m_nVipExp,
					 Guid,m_nCoin,  Guid,m_nGold,UInt32,m_nStrength,
					 Guid, m_accountID, Guid,  m_nFame,UInt32,m_nPower,
					 std::string, m_strMood,List<UInt32>, m_listSweepCount, UInt64, m_nLeavTime,std::string, m_strOldName);

	};
	
	struct PlayerDailyResetData
	{
		
		PlayerDailyResetData():m_nArenaEnterCount(0),m_nAmphitheaterEnterCount(0),
		m_nAmphitheaterWinCount(0),m_nAmphitheaterLoseCount(0),m_nDailyChaiCount(0),
		m_nDailyMiCount(0),m_nDailyYouCount(0),m_nDailyYanCount(0),m_nTwelvepalaceEnterCount(0),
		m_strTwelvepalaceData(""),m_nCouragetrialDailyPoint(0),m_strCourageData(""),
		m_nActivepoint(10),m_nLastUpdateTime(0),m_nDailyMultipleCopyCount(0),m_NormalTimes(0)
		,m_SpecialTimes(0),m_EliteTimes(0),m_NormalMopUpTimes(0),m_EliteMopUpTimes(0),
		m_nTwelvePalaceUnlockCount(0),m_nTwelvePalaceResetFlag(0),m_nClimbTowerTimes(0),m_nCoinBuyTimes(0),
		m_nCoinFreeBuyTimes(0),notUsed3(0),notUsed4(0),notUsed5(0),notUsed6(0),notUsed7(0),
		spare5(0){}

	

		//�������������
		
		UInt16 m_nArenaEnterCount;

		//�������������
		UInt16 m_nAmphitheaterEnterCount;
		//������ʤ������
		UInt16 m_nAmphitheaterWinCount;
		//������ʧ�ܴ���
		UInt16 m_nAmphitheaterLoseCount;
		
		//�ճ����������
		UInt16 m_nDailyChaiCount;

		//�ճ������״���
		UInt16 m_nDailyMiCount;

		//�ճ������ʹ���
		UInt16 m_nDailyYouCount;

		//�ճ������δ���
		UInt16 m_nDailyYanCount;

		//ʮ�����������
		UInt16 m_nTwelvepalaceEnterCount;

		//ʮ������Ϣ
		std::string m_strTwelvepalaceData;

		//���˸���
		UInt32 m_nDailyMultipleCopyCount;
		//���˸���ƥ��������ÿ��
		List<Guid> m_MatchingList;
		//��ͨ�ؿ���ɴ���ÿ��
		UInt32 m_NormalTimes;
		//�ر�ؿ���ɴ���ÿ��
		UInt32 m_SpecialTimes;
		//��Ӣ�ؿ���ɴ���ÿ��
		UInt32 m_EliteTimes;
		//��ͨɨ������ÿ��
		UInt32 m_NormalMopUpTimes;
		//��Ӣɨ������ÿ��
		UInt32 m_EliteMopUpTimes;

		//���������ճ���õ���

		UInt16 m_nCouragetrialDailyPoint;

		std::string m_strCourageData;

		UInt32 m_nActivepoint;

		UInt32 m_nTwelvePalaceUnlockCount;//���� ʮ���ڹ���������Ʒ ����2�����÷�������Ϊ2 ԭ��m_nspare1

		UInt32 m_nTwelvePalaceResetFlag;//ʮ���ڹ�������ս������־λ

		UInt32 m_nClimbTowerTimes;//����ʣ�����

		UInt32 m_nTowerBuyTimes;//�����������

		UInt32 spare5;//û��

		UInt64 m_nLastUpdateTime;

		UInt32 m_nCoinBuyTimes;//��ҹ������
		UInt32 m_nCoinFreeBuyTimes;//��ҹ�������е���ѹ������
		UInt32 notUsed3;//
		UInt32 notUsed4;//
		UInt32 notUsed5;//
		UInt32 notUsed6;//
		UInt32 notUsed7;//


		List<KeyValue> m_mapIDToLock;

		LYNX_S11N_28(PlayerDailyResetData,UInt16, m_nArenaEnterCount,UInt16,m_nAmphitheaterEnterCount,
			UInt16 ,m_nAmphitheaterWinCount,UInt16 ,m_nAmphitheaterLoseCount,UInt16, m_nDailyChaiCount,
			UInt16, m_nDailyMiCount,UInt16, m_nDailyYouCount,UInt16,m_nDailyYanCount,UInt16, m_nTwelvepalaceEnterCount,
			std::string, m_strTwelvepalaceData,UInt16,m_nCouragetrialDailyPoint,std::string, m_strCourageData,
			UInt32, m_nActivepoint, UInt64 ,m_nLastUpdateTime,UInt32, m_nDailyMultipleCopyCount,
			UInt32, m_nTwelvePalaceUnlockCount,UInt32, m_nTwelvePalaceResetFlag,UInt32, m_nClimbTowerTimes,UInt32,
			m_nTowerBuyTimes,UInt32, spare5,UInt32, m_nCoinBuyTimes,UInt32, m_nCoinFreeBuyTimes,
			UInt32, notUsed3,UInt32, notUsed4,UInt32, notUsed5,UInt32, notUsed6,
			UInt32, notUsed7,List<KeyValue>, m_mapIDToLock
			);
	}; 
	
	struct PlayerItemListData 
	{
		PlayerItemListData(){}

		List<ItemData> m_listItems;
		LYNX_S11N_1(PlayerItemListData,List<ItemData>,m_listItems);
	};

	
	struct PlayerSkillListData
	{
		PlayerSkillListData(){
						
		}
	
		List<SkillData> m_listSkills;

		LYNX_S11N_1(PlayerSkillListData,List<SkillData>,m_listSkills);
	};

	

	struct PlayerBuffListData
	{
		PlayerBuffListData(){}
		
		List<BufferData> m_listBuffers;
		
		LYNX_S11N_1(PlayerBuffListData,List<BufferData>, m_listBuffers);

	};
	 
	struct PlayerFashionData
	{
		PlayerFashionData(){}

	
		//��ҵ�ʱװ�б������û��õģ�������list��û��õ�ʱ��Ϊ0����õ�ʱ��Ϊ��ȡʱ���
		List<FashionData> m_listFashionDatas;

		LYNX_S11N_1(PlayerFashionData,  List<FashionData>,m_listFashionDatas);
	};

	struct StageData
	{
		StageData():m_nStageID(0),m_nGetStar(0),m_nChallengTimes(0),m_nLastChallengTime(0){}
		UInt32 m_nStageID;
		UInt8 m_nGetStar;//507 �������Ϊ5 ��һ�λ�õ���ǰ3����
		UInt32 m_nChallengTimes;
		UInt64 m_nLastChallengTime;
		LYNX_S11N_4(StageData,UInt32, m_nStageID, UInt8, m_nGetStar,UInt32,m_nChallengTimes,UInt64,m_nLastChallengTime);
	};

	struct PlayerChapterData
	{
		PlayerChapterData():m_nChapterId(0),m_nIselite(0),m_nTreasure1getstate(0),
		m_nTreasure2getstate(0),m_nTreasure3getstate(0),m_nChapterstarttime(0),
		m_nChapterfinishtime(0){}

		UInt16 m_nChapterId;
		UInt8 m_nIselite;
		UInt8 m_nTreasure1getstate;
		UInt8 m_nTreasure2getstate;
		UInt8 m_nTreasure3getstate;
		List<StageData> m_listStageDatas;
		UInt64 m_nChapterstarttime;
		UInt64 m_nChapterfinishtime;

		LYNX_S11N_8(PlayerChapterData,UInt16,m_nChapterId,UInt8,m_nIselite,UInt8,m_nTreasure1getstate,
			UInt8,m_nTreasure2getstate,UInt8,m_nTreasure3getstate, List<StageData>, m_listStageDatas,
			UInt64,m_nChapterstarttime,UInt64,m_nChapterfinishtime);

	};

	struct PlayerChapterList
	{
		PlayerChapterList():m_RoomID(0),m_SceneID(0),m_StageID(0){}
		UInt32 m_RoomID;
		UInt32 m_SceneID;
		UInt32 m_StageID;
		

		List<KeyValue> m_ChapterUnlocked;//ʮ���ڹ��ѽ����½�
		List<PlayerChapterData> m_listChapterData;
// 		List<KeyValue> m_ResetStages;//���ù����½�

		LYNX_S11N_5(PlayerChapterList,List<PlayerChapterData> ,m_listChapterData,UInt32, m_RoomID,UInt32, m_SceneID,UInt32, m_StageID,List<KeyValue>,m_ChapterUnlocked/*,List<KeyValue>,m_ResetStages*/);
	};

	//�ͻ��˱�������
	struct PlayerLocalData
	{
		PlayerLocalData(){}

		List<KeyValue> m_IDToSelect;//ʮ���ڹ������߹�ѡ

		LYNX_S11N_1(PlayerLocalData,List<KeyValue> ,m_IDToSelect);
	};

	
	
	//ÿ��һ������ ��Ҫ�޸�ResetFireConfirmData()
	struct PlayerFireConfirmData	
	{
		PlayerFireConfirmData():m_ConfirmLevel(1),m_ConfirnFailTimes(0),m_ConfirnRightTimes(0),m_CopyID(0),m_CopyStartTime(0),
			m_AwardTimes(0),m_Times(0), m_trialTimes(0), m_trialOtherTimes(0),m_Star(0),m_Gain(0),m_JewlryCanAddNum(0){}

		//��֤�ȼ�
		UInt32 m_ConfirmLevel;
		//��֤ʧ�ܴ���
		UInt32 m_ConfirnFailTimes;
		//������֤��ȷ����
		UInt32 m_ConfirnRightTimes;
		//����ID
		UInt32 m_CopyID;
		//������ʼʱ��
		UInt32 m_CopyStartTime;
		//����֤����
		List<UInt32> m_ConfirmIDs;
		//���յ�����֤
		List<UInt32> m_RecConfirmIDs;
		
		//�����ʹ�ô���
		UInt32 m_Times;

		//���˸��������Լ�����
		UInt32 m_trialTimes;

		//���˸����������˴���
		UInt32 m_trialOtherTimes;

		//���˹ؿ���������
		UInt32 m_AwardTimes;	

		UInt32 m_Star;//��ͨ�ؿ�Ϊ���������˸���Ϊͨ��ʱ��

		List<UInt32> m_AwardTypes;//���ӽ�����ʽ����������

		//�̶������ӳ�ϵ�� ��ֵ��10000 �� 10000/10000��
		UInt32 m_Gain;
		//�̶�����
		List<Goods> m_FixedList;
		//�״�ͨ��
		List<Award> m_FirstAwardsList;
		//������Ʒ
		List<Award> m_AwardsList;
		//���ƽ���
		List<Card> m_CardsList;
		//�粼�ֵ���
		List<Goods> m_MonsterAwardList;
		//�����ߵ���
		List<Goods> m_IntruderAwardList;
		//����
		List<Goods> m_CostList;

		//�ϴη��͵���Ʒ
		List<Goods> m_LastSendItemList;

		//�ϴν�����Ʒ
		List<Award> m_LastAwardsList;

		//Ҫ��ends����ⷢ������
		List<Goods> m_SendList;

		Goods m_AddSendGoods;

		Json::Value m_AddSendjs;

		std::string m_SendStr;

		int m_JewlryCanAddNum;


		LYNX_S11N_25(PlayerFireConfirmData, UInt32, m_ConfirmLevel, UInt32 ,m_ConfirnFailTimes,UInt32 ,m_ConfirnRightTimes,UInt32, m_CopyID,UInt32, m_CopyStartTime,List<UInt32>,
			m_ConfirmIDs,List<UInt32>, m_RecConfirmIDs,List<Award>,m_AwardsList,List<Card>, m_CardsList,List<Goods>, m_MonsterAwardList,List<Goods>, m_LastSendItemList,
			List<Goods>, m_CostList,UInt32, m_AwardTimes,UInt32, m_Star,UInt32, m_trialOtherTimes,UInt32,m_trialTimes,UInt32, m_Times,List<UInt32>, m_AwardTypes,
			List<Award>, m_LastAwardsList,List<Goods>, m_FixedList,UInt32, m_Gain,List<Goods>, m_SendList,Goods,m_AddSendGoods,List<Goods>,m_IntruderAwardList,int, m_JewlryCanAddNum);

	};

	struct PlayerRhymeData
	{
		
		PlayerRhymeData():m_PlayerUid(0), m_RhymeExp(0), m_RhymeLv(0), m_RhymeStep(0), m_RhymeSoul(0), m_RhymeSkillIndex(0){}
		//���uid
		UInt64 m_PlayerUid;
		//��ҵ�ǰ���ĵȼ���Ӧ�ľ���ֵ
		UInt32 m_RhymeExp;

		//���ĵȼ�
		UInt32 m_RhymeLv;

		//���Ľ���
		UInt32 m_RhymeStep;

		//��ǰ�ϻ�����
		UInt64 m_RhymeSoul;

		//��ǰװ�����Ϲ���������0��ʼ
		UInt32 m_RhymeSkillIndex;

		std::string m_RhymeAcupointStr;

		List<UInt64> m_RhymeAcupointList;

		//���ɲݵͼ�
		UInt32 m_RhymeGrass;
		
		//���ɲ��м�
		UInt32 m_RhymeGrassMid;

		//���ɲݸ߼�

		UInt32 m_RhymeGrassHigh;

		LYNX_S11N_11(PlayerRhymeData, UInt64, m_PlayerUid, UInt32 ,m_RhymeExp,UInt32 ,m_RhymeLv,UInt32, m_RhymeStep,UInt64, m_RhymeSoul,UInt32, m_RhymeSkillIndex, List<UInt64>, m_RhymeAcupointList, std::string, m_RhymeAcupointStr,UInt32, m_RhymeGrass,UInt32, m_RhymeGrassMid,UInt32, m_RhymeGrassHigh);

	};

	struct PlayerCounterData
	{

 		PlayerCounterData():m_RewardLotteryOnceTime(0), m_RewardLotteryTenTime(0), m_RewardLotteryOnceTicket(0), m_RewardLotteryTenTicket(0),
			m_RewardLotteryOnceCount(0), m_RewardLotteryTenCount(0), m_RewardLotteryVipDefaultCount(0),
			m_RewardLotteryOnceFreeCount(0), m_RewardLotteryOnceTicketCount(0), m_RewardLotteryTenFreeCount(0), m_RewardLotteryTenTicketCount(0),
			m_RewardLotteryVipAwardID(0), m_RewardLotteryVipElseCount(0){}

		UInt32 m_RewardLotteryOnceTime;//��һ�ε���ʱ��

		UInt32 m_RewardLotteryTenTime;//��һ��ʮ����ʱ��

		UInt32 m_RewardLotteryOnceTicket;//����ȯ����

		UInt32 m_RewardLotteryTenTicket;//ʮ����ȯ����

		UInt32 m_RewardLotteryOnceFreeCount;//��ѵ������ ����maxkey�������������

		UInt32 m_RewardLotteryOnceTicketCount;//����ȯ���� ����maxkey�������������

		UInt32 m_RewardLotteryOnceCount;//������� ����maxkey�������������

		UInt32 m_RewardLotteryTenFreeCount;//���ʮ������� ����maxkey�������������

		UInt32 m_RewardLotteryTenTicketCount;//ʮ����ȯ���� ����maxkey�������������

		UInt32 m_RewardLotteryTenCount;//ʮ������� ����maxkey�������������

		UInt32 m_RewardLotteryVipDefaultCount;//vip���� ����maxkey�������������

		UInt32 m_RewardLotteryVipAwardID;//vip���� ����maxkey�������������

		UInt32 m_RewardLotteryVipElseCount;//vip���� ����maxkey�������������


		//������
		List<KeyValue> m_BoxCounter;

		//����������
		List<KeyValue> m_CopyCounter;

		//����������
		List<UInt32> m_RewardCounter;

		

		LYNX_S11N_18(PlayerCounterData,List<KeyValue>, m_BoxCounter,List<KeyValue>, m_CopyCounter,List<UInt32>, m_RewardCounter,
			UInt32,m_RewardLotteryOnceTime,UInt32,m_RewardLotteryTenTime,UInt32,m_RewardLotteryOnceTicket,UInt32,m_RewardLotteryTenTicket,
			UInt32,m_RewardLotteryOnceCount,UInt32,m_RewardLotteryTenCount,UInt32,m_RewardLotteryTenTicket,UInt32,m_RewardLotteryOnceFreeCount,
			UInt32,m_RewardLotteryOnceTicketCount,UInt32,m_RewardLotteryOnceCount,UInt32,m_RewardLotteryTenFreeCount,UInt32,m_RewardLotteryTenTicketCount,
			UInt32,m_RewardLotteryVipDefaultCount,UInt32,m_RewardLotteryVipAwardID,UInt32,m_RewardLotteryVipElseCount);


	};

	struct PlayerFoodsData
	{
		PlayerFoodsData(): beginTime(0),leftTimes(0),food1(0),food2(0),food3(0),vipLeftTimes(0),buyTime(0),vipFoodLeftNumber(0){}
		
		UInt32 leftTimes;//��ʹ�ô���
		UInt32 beginTime;//�ȴ�ʱ�� 0����ʱֹͣ�������㵹��ʱ�ڽ���
		UInt32 food1;//ʳ��1����
		UInt32 food2;
		UInt32 food3;
		UInt32 vipLeftTimes;//vip����������ģ�
		UInt32 buyTime;
		UInt32 vipFoodLeftNumber;//vip����ʳ��ʣ�����
		LYNX_S11N_8(PlayerFoodsData, UInt32, beginTime, UInt32 ,leftTimes,UInt32 ,food1,UInt32, food2,UInt32, food3,UInt32, vipLeftTimes,UInt32, buyTime,UInt32, vipFoodLeftNumber);
	};

	struct PlayerStrengthData
	{
		PlayerStrengthData(): beginTime(0),leftTimes(0),strength(0),vipLeftTimes(0),buyTime(0),vipFoodLeftNumber(0){}

		UInt32 leftTimes;//��ʹ�ô���
		UInt32 beginTime;//�ȴ�ʱ�� 0����ʱֹͣ�������㵹��ʱ�ڽ���
		UInt32 strength;//����
		UInt32 vipLeftTimes;//vip����������ģ�
		UInt32 buyTime;
		UInt32 vipFoodLeftNumber;//vip����ʳ��ʣ�����
		LYNX_S11N_6(PlayerStrengthData, UInt32, beginTime, UInt32 ,leftTimes,UInt32 ,strength,UInt32, vipLeftTimes,UInt32, buyTime,UInt32, vipFoodLeftNumber);
	};

	typedef Map<UInt64,UInt32> RuneDataMap;
	struct PlayerHoarStoneData
	{
		PlayerHoarStoneData(){}
		
		List<HoarStoneData> mHoarStoneList;
		 
		RuneDataMap mRuneDataMap;
		
		LYNX_S11N_2(PlayerHoarStoneData, List<HoarStoneData>,mHoarStoneList,RuneDataMap, mRuneDataMap);
	};


	struct PlayerEquipData
	{
		PlayerEquipData(){}

		List<HeroEquipData> mHeroEquipLists;

		LYNX_S11N_1(PlayerEquipData,List<HeroEquipData>, mHeroEquipLists);
	};

	struct PlayerMaterialData
	{
		PlayerMaterialData(){}

		List<MaterialData> mMaterialLists;

		LYNX_S11N_1(PlayerMaterialData, List<MaterialData>, mMaterialLists);
	};

	struct PlayerGemData
	{
		PlayerGemData(){}

		List<GemData> mGemLists;
		LYNX_S11N_1(PlayerGemData, List<GemData>, mGemLists);
	};

	struct JewelryEquipData
	{
		JewelryEquipData(): mJewelryBit(0){}

		List<UInt64> mJewelryList;
		UInt32 mJewelryBit;

		LYNX_S11N_2(JewelryEquipData, List<UInt64>, mJewelryList, UInt32, mJewelryBit);
	};

	struct PlayerJewelryData
	{
		PlayerJewelryData(){}
		List<JewelryData> mJewelryList;
		LYNX_S11N_1(PlayerJewelryData,List<JewelryData>, mJewelryList);
	};

	struct AwardData
	{
		UInt32 resType;
		UInt32 resID;
		UInt32 resCount;
		AwardData():resID(0),resType(0),resCount(0){}
		LYNX_S11N_3(AwardData,UInt32, resType,UInt32, resID,UInt32, resCount);
	};

	struct AssistBattleData
	{
		UInt32 quality;
		UInt64 equipAssist;
		AssistBattleData():quality(0), equipAssist(0){}

		LYNX_S11N_2(AssistBattleData,UInt32, quality,UInt64, equipAssist);
	};

	struct ServantBattleData
	{
		UInt32 mainBattleBit;
		List<UInt64> mainServantBattle;
		UInt32 assistBattleBit;
		List<AssistBattleData> assistServantBattle;
		LYNX_S11N_4(ServantBattleData,UInt32, mainBattleBit,List<UInt64>, mainServantBattle, UInt32, assistBattleBit, List<AssistBattleData>, assistServantBattle);
	};


	struct ServantData
	{
		UInt64 servantUid;
		UInt64 servantId;
		UInt32 pieceCount;
		UInt32 level;
		UInt32 star;
		UInt32 floor;
		UInt32 lvexp;
		List<UInt64> equipTreasures;
		LYNX_S11N_8(ServantData, UInt64, servantUid, UInt64, servantId, UInt32, pieceCount, UInt32, level, UInt32, star, UInt32, floor, List<UInt64>, equipTreasures, UInt32, lvexp);
	};

	struct ServantTreasure
	{
		ServantTreasure():treasureId(0),treasureUid(0), count(0){}
		UInt64 treasureUid;
		UInt64 treasureId;
		UInt32 count;
		LYNX_S11N_3(ServantTreasure, UInt64, treasureUid, UInt64 , treasureId, UInt32, count);
	};

	struct PlayerServantData
	{
		PlayerServantData():food1count(0),food2count(0),food3count(0),food4count(0),food5count(0){}
		List<ServantTreasure> treasureList;
		List<ServantData> servantList;
		ServantBattleData servantBattleData;

		UInt32 food1count;
		UInt32 food2count;
		UInt32 food3count;
		UInt32 food4count;
		UInt32 food5count;
	    
		UInt32 battleStone;

		UInt32 servantSwitch;

		LYNX_S11N_10(PlayerServantData, List<ServantTreasure>, treasureList, List<ServantData>, servantList, ServantBattleData, servantBattleData, UInt32, food1count
			,UInt32, food2count, UInt32, food3count, UInt32, food4count, UInt32 ,food5count, UInt32, battleStone,UInt32,servantSwitch);
	};

	struct ChatData
	{
		ChatData():playerUid(0), chatTime(0){}
	    UInt64 playerUid;
		UInt64 chatTime;

		LYNX_S11N_2(ChatData,UInt64, playerUid, UInt64, chatTime );

	};
	struct PlayerTowerData
	{
		PlayerTowerData():m_HighID(0),m_Score(0),m_AP(0),m_HP(0),m_SP(0){}		

		UInt32 m_HighID;
		UInt32 m_Score;
		UInt32 m_AP;
		int m_HP;
		int m_SP;

		LYNX_S11N_5(PlayerTowerData,UInt32, m_HighID,UInt32, m_Score,UInt32, m_AP,int, m_HP,int, m_SP);
	};

	struct PlayerCourageChallengeData
	{
		PlayerCourageChallengeData():m_LeftChallengeTimes(0),m_BeginTime(0),m_BuyTime(0),m_BuyTimes(0),monsterID1(0),monsterID2(0),
			monsterID3(0),m_LightOfLife(0),m_RefreshTimes(0),m_LuckyValues(0){}		


		UInt32 m_LeftChallengeTimes;
		UInt32 m_BeginTime;
		UInt32 m_BuyTime;
		UInt32 m_BuyTimes;
		UInt32 monsterID1;
		UInt32 monsterID2;
		UInt32 monsterID3;
		UInt32 contentID1;//�����awardid����contentid
		UInt32 contentID2;
		UInt32 contentID3;
		UInt32 m_LightOfLife;
		UInt32 m_RefreshTimes;
		UInt32 m_LuckyValues;

		List<UInt32> coinGroup;
		List<CatCoin> catCoins;	

		LYNX_S11N_15(PlayerCourageChallengeData,List<UInt32>, coinGroup,List<CatCoin>, catCoins,UInt32, m_LeftChallengeTimes,UInt32, m_BeginTime,
		UInt32, m_BuyTime,UInt32, m_BuyTimes,UInt32, monsterID1,UInt32, monsterID2,UInt32, monsterID3,UInt32, contentID1,UInt32, 
		contentID2,UInt32, contentID3,UInt32, m_LightOfLife,UInt32,m_RefreshTimes,UInt32, m_LuckyValues);
	};


	struct Item
	{
		Item(): itemID(0),position(0),buyTimes(0),buyTime(0),shopType(0){}

		UInt32 itemID;
		UInt32 shopType;
		UInt32 position;
		UInt32 buyTimes;
		UInt32 buyTime;
		Goods goods;
		LYNX_S11N_6(Item,UInt32, itemID, UInt32,position,UInt32, buyTimes,UInt32,buyTime,UInt32,shopType,Goods,goods);
	};

	struct ShopItems
	{
		ShopItems(): shopType(0),refreshTimes(0),refreshTime(0){}
		UInt32 shopType;
		UInt32 refreshTimes;
		UInt32 refreshTime;
		List<Item> items;

		LYNX_S11N_4(ShopItems,UInt32, shopType, UInt32,refreshTimes,UInt32, refreshTime,List<Item>, items);
	};


	struct PlayerShopData
	{
		PlayerShopData(){}	

		List<ShopItems> m_ShopItems;

		LYNX_S11N_1(PlayerShopData,List<ShopItems>, m_ShopItems);
	};



	struct PlayerChatData
	{
		PlayerChatData():disableFlag(0),disableBeginTime(0),disableTimeLong(0){}
	    List<ChatData> chatLists;
		UInt32 disableFlag;
		UInt64 disableBeginTime;
		UInt32 disableTimeLong;

		LYNX_S11N_4(PlayerChatData,  List<ChatData>, chatLists, UInt32, disableFlag, UInt64,  disableBeginTime, UInt32, disableTimeLong);
		
	};
	typedef Map<UInt32,MapRecord> TypeRecords;

	struct RecordsData
	{
		RecordsData(){}		

		TypeRecords m_TypeRecords;


		LYNX_S11N_1(RecordsData, TypeRecords, m_TypeRecords);
	};

	struct PlayerEmailData
	{
		PlayerEmailData(){}
		List<EmailData> emailList;
		LYNX_S11N_1(PlayerEmailData,  List<EmailData>, emailList);
	};

   struct PlayerCharactorData
   {
	   PlayerCharactorData(){}
	   List<CharactorData> charactorList;
	   LYNX_S11N_1(PlayerCharactorData, List<CharactorData>, charactorList);	   
   };
   struct KeyList
   {
	   UInt32 key;
	   List<Goods> values;
	    LYNX_S11N_2(KeyList,UInt32, key, List<Goods>, values);
   };

   struct PlayerInlineActivityData
   {
	   PlayerInlineActivityData():m_GrowFoundBuyFlag(0),m_MonthSignCount(0),m_MonthSignTime(0),m_OnlineWelFareOnlineTime(0),m_PeopleWelfareRechargeNum(0),
		   m_FirstLoginTime(0), m_SevenLoginGotCount(0),m_LastGetTime(0),m_LoginTime(0),m_TimeAwardRefreshTime(0){}		

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
	   List<KeyList> m_SevenDayTaskFinishList;//����ɵ�����

	   //�ֶ�ʱ��
	   UInt32 m_TimeAwardRefreshTime;//����ʱ��
	   List<UInt32> m_TimeAwardGotList;//����ȡ�Ľ���

	   //��¼
	   UInt32 m_LoginTime;//��½ʱ��


	   LYNX_S11N_16(PlayerInlineActivityData,UInt32, m_GrowFoundBuyFlag,List<UInt32>, m_GrowFoundGotList, UInt32, m_MonthSignCount, UInt32, m_MonthSignTime,
		   UInt32, m_OnlineWelFareOnlineTime,List<UInt32>, m_OnlineWelFareGotList,UInt32, m_PeopleWelfareRechargeNum,List<UInt32>, m_PeopleWelfareGotList,
		   UInt32, m_FirstLoginTime,UInt32, m_LastGetTime, UInt32, m_SevenLoginGotCount, List<KeyValue>, m_SevenDayTaskGotList, UInt32, m_TimeAwardRefreshTime,
		    List<UInt32>, m_TimeAwardGotList,UInt32, m_LoginTime,List<KeyList>, m_SevenDayTaskFinishList);
   };

	struct PlayerAchieveData
	{
		PlayerAchieveData(){}
		List<AchieveData> achieveList;
		 LYNX_S11N_1(PlayerAchieveData, List<AchieveData>, achieveList);	 
	};

	struct PlayerDailyTaskData
	{
		PlayerDailyTaskData():lastResetTime(0){}
		List<DailyTaskData> dailytaskList;
		UInt64 lastResetTime;
		LYNX_S11N_2(PlayerDailyTaskData, List<DailyTaskData>, dailytaskList, UInt64 ,lastResetTime);	
	};

	struct PlayerDailyActiveData
	{
		PlayerDailyActiveData():curActive(0), lastRestTime(0){}
		List<UInt32> flags;
		UInt32 curActive;

		UInt64 lastRestTime;
		LYNX_S11N_3(PlayerDailyActiveData, List<UInt32>, flags, UInt32, curActive, UInt64, lastRestTime);	
	};

    struct PlayerData 
    {
        PlayerData() {}
		
		PlayerBaseData mBaseData;
		
		PlayerHoarStoneData mHoarStoneData;

		PlayerDailyResetData mDailyRestData;

		PlayerBuffListData mBuffListData;
		
		PlayerFashionData mFashionData;

		PlayerSkillListData mSkillListData;

		PlayerItemListData mItemListData;

		PlayerChapterList mChapterListData;

		PlayerFireConfirmData mFireConfirmData;

		PlayerRhymeData mRhymeData;

		PlayerCounterData mCounterData;

		PlayerFoodsData mFoodsData;

		PlayerStrengthData mStrengthData;

		PlayerEquipData mEquipData;

		PlayerMaterialData mMaterialData;

		PlayerGemData mGemData;

		JewelryEquipData mJewelryEquipData;

		PlayerJewelryData mJewelryData;

		PlayerLocalData mLocalData;

		PlayerServantData mServantData;

		PlayerTowerData mTowerData;
		PlayerChatData mChatData;

		RecordsData mRecordsData;

		FriendData mFriendData;

		PlayerEmailData mEmailData;

		PlayerShopData mPlayerShopData;

		PlayerCharactorData mPlayerCharactorData;

		 PlayerCourageChallengeData mCourageChallengeData;

		 PlayerInlineActivityData mInlineActivityData;

		 PlayerAchieveData mAchieveData;

		 PlayerDailyTaskData mDailyTaskData;

		 PlayerDailyActiveData mDailyAcvData;

		 LYNX_S11N_31(PlayerData,  PlayerBaseData, mBaseData,PlayerDailyResetData, mDailyRestData,PlayerBuffListData,mBuffListData,
			 PlayerFashionData,mFashionData,PlayerSkillListData,mSkillListData, PlayerItemListData, mItemListData,
			 PlayerChapterList, mChapterListData,PlayerFireConfirmData,mFireConfirmData,PlayerRhymeData,mRhymeData,
			 PlayerCounterData, mCounterData,PlayerFoodsData, mFoodsData,PlayerStrengthData, mStrengthData,PlayerHoarStoneData,mHoarStoneData,PlayerEquipData, mEquipData,
			 PlayerMaterialData, mMaterialData,PlayerGemData, mGemData, JewelryEquipData, mJewelryEquipData, PlayerJewelryData, mJewelryData,
			 PlayerServantData, mServantData,PlayerChatData, mChatData,PlayerTowerData, mTowerData,RecordsData,mRecordsData, FriendData, mFriendData,
			 PlayerEmailData, mEmailData,PlayerShopData, mPlayerShopData, PlayerCharactorData, mPlayerCharactorData, PlayerCourageChallengeData, mCourageChallengeData,
			 PlayerInlineActivityData, mInlineActivityData,  PlayerAchieveData, mAchieveData, PlayerDailyTaskData, mDailyTaskData,
			  PlayerDailyActiveData, mDailyAcvData
			 );
		
    };
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_PLAYER_DATA_H__