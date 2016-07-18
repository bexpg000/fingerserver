#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_MAMAGER_H__


#include "MainThread.h"
#include "../FireConfirm/Box.h"

namespace Lynx
{
	class Player;

	struct IDCount
	{
		UInt64 id;
		UInt32 count;
		LYNX_S11N_2(IDCount,UInt64, id,UInt32, count);
	};

	struct CGOpenChapterBox
	{
		CGOpenChapterBox(): chapterID(0),whichOne(0){}

		UInt32 chapterID;
		UInt32 whichOne;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				chapterID = root["chapterID"].asUInt();
				whichOne = root["whichOne"].asUInt();
			}
		}
		LYNX_MESSAGE_1(CHAPTER_BOX_OPEN_REQ, CGOpenChapterBox,std::string, strReceive);
	};


	class StageManager : public MainThread ,public Singleton<StageManager>
	{
	public:
		StageManager();
		virtual ~StageManager();

		bool initial(Player* player);
		void release();

		std::string convertStageDataToJson();

		UInt32 getChapterID(UInt32 stageID);

		UInt32 addStageData(Guid playerID,UInt32 copyID,Int32 num);//�������

		UInt32 showNexStage(Guid playerID,UInt32 copyID,Int32 num);//��ʾ����ս��һ��

		UInt32 chapterBoxCanOpen(PlayerChapterData chapterData,UInt32 whichOne);//�½��еı����Ƿ��ܴ�

		static void onOpenChapterBox(const ConnId& connId ,CGOpenChapterBox & msg );//���½��еı���

		void saveAndSend(Guid playerID,PlayerChapterList chapterList,List<UInt32> stageIDs);//�ؿ����ݴ��̲����͸��ͻ���

		UInt32 getNormalStageLeftTimes(const ConnId& connId,UInt32 stageID);//��ͨ�ؿ�ʣ����ս����

		UInt32 canChallengeStage(const ConnId& connId,UInt32 stageID);//�Ƿ�����ս�ؿ�

		UInt32 twelvePalaceCanDo(const ConnId& connId,UInt32 stageID);

		UInt32 isMeetCondition(const ConnId& connId,UInt32 stageID);

		UInt32 getStageStar(const ConnId& connId,UInt32 stageID);

		UInt32 getChapterType(UInt32 stageID);


		UInt32 binarytoStarNum(UInt32 num);//�����Ƶ���ת��������

		void stageReset(Player* player,bool needSend);//���ùؿ�����

		bool initePreStageIDs();

		void sendStageData(Guid playerID,Map<UInt16, PlayerChapterData *> mapIDToChapter,bool isLogin = false);

		Map<UInt16, PlayerChapterData *> getMapIDToChapter();

		void delDoNotMeet(const ConnId& connId,Map<UInt16, PlayerChapterData *> &mapIDToChapter);

		

		UInt32 getTwelvePalaceNeedReset();		

		void setTwelvePalaceNeedReset(UInt32 num);

		 Map<UInt64, StageTemplate> getMapIDToStageByChapterID(UInt32 id);
		
		 void addNeedSaveDateList(UInt32 stageID);

		 List<UInt32> getNeedSaveDateList();

		 void resetNeedSaveDateList();

		 void addMatchingPlayers(UInt64 playerID);
		
		 List<UInt64> getMatchingPlayers();
		
		 void resetMatchingPlayers();

		 
		 void resetChapterUnlock();

		  
		 UInt32 resetStagechallegeTimes(UInt32 stageID);

		 
		 UInt32 resetMultipleCopychallegeTimes();

		
		 UInt32 resetWelfaleMarketchallegeTimes(UInt32 id);

		
		 UInt32 resetClimbTowerchallegeTimes();

		 void resetgDocument();

		 void addgDocument(rapidjson::Value &object);

		 rapidjson::Document::AllocatorType& getgDocumentAllocator();

		 std::string getgDocumentStr();

		rapidjson::Document& getDocument();

		rapidjson::Value& getArry();
		  
		 
// 		 void addStageResetTimes(UInt32 stageID,UInt32 count);
// 
// 		  
// 		 UInt32 getStageResetTimes(UInt32 stageID);
		 

	private:
		void stageInitial();

		
		Player* m_pPlayer;

		List<PlayerChapterData> *m_pListChapterData;

		Map<UInt16, PlayerChapterData *> m_mapIDToChapter;

		 List<UInt32> needSaveDateList;

		 UInt32 mTwelvePalaceNeedReset;

		 List<UInt64> matchingPlayers;

		 


	};
	static Map<UInt32,UInt32> gPreStageIDs;

	static List<UInt32> gFirstAddStage;

	static rapidjson::Document gDocument;

	static rapidjson::Value gRapidArray(rapidjson::kArrayType);

	
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_MAMAGER_H__
