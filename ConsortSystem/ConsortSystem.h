#ifndef __LYNX_GAME_SERVER_CONSORTSYSTEM_H__
#define __LYNX_GAME_SERVER_CONSORTSYSTEM_H__

#include "../MainThread.h"
#include "../Guild.h"
#include "../DBInterface.h"
#include "../CommonLib/ConsortData.h"
//���쳽
namespace Lynx
{
	class ConsortInfoData;
	class FriendBlackManager;
	class FriendBlackInfo;
	class ConsortSystem : public MainThread, public Singleton < ConsortSystem >
	{
	public:

		ConsortSystem();
		virtual ~ConsortSystem();

		bool initial();
		void release();

		void update(const UInt64& accTime);
	
		ConsortData * getConsortDataById(UInt64 consortId);

		void destroyConsort(UInt64 consortId);
		
		void updateConsortRankList();

		const List<ConsortRankData > &getConsortRankList();

		const List<ConsortData >& getConsortList();

		ConsortRankData * getConsortRankData(UInt64 consortId);

		Map<UInt64, ConsortData *>  *getConsortMap(void);

		ConsortData * createConsort(std::string name, Player *  player);
	
		//�ж�ͬ������
		bool judgeSameNameConsort(std::string name);

		UInt32 getConsortLastRank();

		void sendConsortMsg(UInt32 tempId, UInt64 consortId, UInt64 playerUid, std::string  playerName,  std::string param1,
			std::string param2, UInt64 logTime);
	
		//���ݹ���id���ĳ����ҵ�����
		bool eraseApplyFromCheckList(UInt64 consortId, UInt64 applyUid);

		const DBInterface & getDBInterface();
	
		//���������б�
		void  chatListJson(Json::Value & root, UInt64 playeruid, UInt64 lastchattime);

		//��DB��ȡbase��Ϣ
		bool getBaseInfoFromDb(UInt64 playerUid, BaseInfoData & baseInfoData);

		//��DB��ȡ���������Ϣ
		bool getOtherFriendInfo(UInt64 otherUid, FriendBlackInfo & friendblackInfo);

		bool getOtherBeApplyGuidList(UInt64 otherUid, List<UInt64> &friendBeApplyGuidList);

		void saveTicketFriends(UInt64 consortid, UInt32 ticketid);

		void saveTicketGivens(UInt64 consortid, UInt32 ticketid);

		UInt64 getAwardActiveTime();

		//Ʊ�м���������Ⲣ����
		void ticketQuestionInit(UInt64 consortId);

		UInt64 getWoodBeginTime();

		UInt64 getWoodEndTime();

		void clearWoodBattle(UInt64 consortId, UInt64 playeruid);

	private:

		//�ڶ�������һ���ֻ�����ݲ�ͬ��ʱ�����ò�ͬ��
		////curTimeΪ�뼶��
		void resetRefresh(UInt64 curTime);

	//ˢ��һ���ְ�������
		void resetRefreshHalfDay(UInt64 curTime);

	//����Ʊ�Ѽ���
		void resetticketfriend(UInt64 curTime);

		//����Ʊ�Ѽ�����Ч�ڵ�ʱ���
		void calculateTicketActive(time_t curTime);

		//��������ڴ����ݣ�������ˢ��֪ͨ���˽ӿ����ڰ�������ˢ��
		void memNotifyHalfDay();

		//Ʊ�Ѽ���
		void ticketRefresh();

		


		//����1~10������Ʊ�Ѽ������
		void generateTicketQuesions();
		
		void generateTicketQuestion(UInt32 lv);

		List<UInt32>* getTicketQuestion(UInt32 lv);
	
		void convertTicketStr2List(List<TicketFriend>* ticketList, std::string ticketStr);

		void convertGivenStr2List(List<GivenData>* givenList, std::string givenStr);

		std::string convertTicketList2Str(List<TicketFriend>* ticketList);

		std::string converGivenList2Str(List<GivenData>* givenList);

		void calculateWoodBegin(time_t curtime);

	

        //��ʱ���ϴδ��ʱ��,���뼶
		UInt64 m_nLastSyncTime;
		UInt64 m_nLastRefreshTime;
		DBInterface mDBInterface;
		//ÿ������ǩ����Ծ�ȵ�ʱ��� ,�뼶��
		UInt64 m_nActiveResetTime;

		//ÿ12��Сʱ����è�̵�ˢ�µ�ʱ������뼶��
		UInt64 m_nBusinessCatResetTime;

		//ÿ������ѵ������ʱ������뼶��
		UInt64 m_nEyeSightResetTime;

		//Ʊ�Ѽ����������ʱ������뼶��
		UInt64 m_nTicketFriendTime;

		//Ʊ�Ѽ�������Ʊ�Ѽ��������ʱ���
		UInt64 m_nTicketTimesTime;

		//�������������������ʱ���
		UInt64 m_nLoyalTime;

		//���������ѵ��������ʱ���
		UInt64 m_nKitchenTime;

		//����ڲ�ѵ������ʱ���
		UInt64 m_nEloquenceTime;

		//ľè������ʱ���
		UInt64 m_nWoodCatTime;


		//�ڴ���ά����consort map�����ڹ���consortList������
		Map<UInt64, ConsortData *> m_consortMap;
		//consortList���ݣ��ڴ��н��б�������ڸ������ݿ�ͼ���
		List<ConsortData > m_consortList;
		//��ʱ�������ݿ���load consort���ݣ�����������list ��
		//�����ڷ��͸��ͻ���������Ϣ
		List<ConsortRankData > m_consortRankList;
		//�������ݣ������ڷ��͸��ͻ������л򵥶����ң����������ݿ����
		Map<UInt64, ConsortRankData *> m_consortRankMap;

		Map<UInt32, List<UInt32> >m_ticketfriendMap; 

		//��ǰƱ�Ѽ������ʱ�����ǰƱ�Ѽ���������¸�Ʊ�Ѽ������ʱ���
		UInt64 m_nTicketAcitve;

		//ľè�󷨿�ʼʱ���
		UInt64 m_nWoodCatBegin;
		//ľè�󷨽���ʱ���
		UInt64 m_nWoodCatEnd;
		
	
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RANKSYSTEM_H__

