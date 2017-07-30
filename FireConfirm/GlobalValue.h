#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../PlatformLib/PlatformLib.h"




namespace Lynx
{

	struct GlobalValue
	{
		UInt32 id;
		UInt32 typeID;
		List<UInt32> ids;

		//meishiwu

		UInt32 uMSWweight1;
		UInt32 uMSWweight2;
		UInt32 uMSWweight3;
		UInt32 uMSWmax;
		UInt32 uMSWrecover;
		UInt32 uMSWeachBuy;
		UInt32 uMSWprice;
		UInt32 uMSWtime;
		UInt32 uMSWrate1;
		UInt32 uMSWrate2;
		UInt32 uMSWrate3;

		//saodang
		UInt32 uSDrate;
		UInt32 uSDsweeptimes;
		UInt32 uSDsweepfreetimes;
		UInt32 uSDamount;
		List<UInt32> uSDprices;
		//specialrelive		
		UInt32 uSRLdayfreetimes;
		UInt32 uSRLmaxtimes;
		List<UInt32>uSRLcosts;
		
		//trail
		UInt32 uTRIALratecosts;
		UInt32 uTRIALFriendRate;
		UInt32 uTRIALFastRate;
		UInt32 uTRIALGainsRate;
		UInt32 uTRIALVipNeed;




		//twelvepalace
		UInt32 uTPdailyreset;
		UInt32 uTPneeditemid;
		UInt32 uTPcanresetitem;
		UInt32 uTPresetlowerlimit;
		UInt32 uTPbuycost;
		UInt32 uTPbuybosscost;
		UInt32 uTPhornbuycost;
		UInt32 uTPeachdaybuytimes;
		

		//climbtower
		UInt32 uCLIMBTdailyreset;
		UInt32 uCLIMBTneeditemid;
		UInt32 uCLIMBTcanresetitem;
		UInt32 uCLIMBTresetlowerlimit;
		UInt32 uCLIMBTbuycost;


		//strength
		UInt32 uSTmax;
		UInt32 uSTtime;
		List<UInt32> uSTprices;
		//mysticalshopcost
		List<UInt32> uMSHOPrefreshtimes;
		List<UInt32> uSSHOPrefreshtimes;
		List<UInt32> uSSHOPbuytimes;

		List<UInt32> uCOINSHOPrefreshtimes;
		List<UInt32> uCOURAGESHOPrefreshtimes;

		//territorylucky
		List<UInt32> uTERRITORYLrefreshs;		
		List<UInt32> uTERRITORYLcosts;
		List<UInt32> uTERRITORYLbuytimess;
		UInt32 uTERRITORYLmaxtimes;
		UInt32 uTERRITORYLrecovertime;
		UInt32 uTERRITORYLchallage;
		UInt32 uTERRITORYLfreerefreshtimes;
		UInt32 uTERRITORYLfreerefresh;	
		UInt32 uTERRITORYLrmbrefresh;

		//gainwealth
		UInt32 uGAINWEALTHfreetimes;


		//growthfound
		UInt32 GROWTHFOUNDviplvneed;
		UInt32 GROWTHFOUNDcost;

		//������Ҳ�����ս�ۡ�ʮ���ڹ����������������˸�����
		UInt32	uReliveBaseCost;
		UInt32	uReliveRatio;
		UInt32	uReliveMaxCost;

		//������˸������ֲ��������θ�����󻨷�160Ԫ����
		UInt32	uRelivePlayer2BaseCost;
		UInt32	uRelivePlayer2Ratio;
		UInt32	uRelivePlayer2MaxCost;

		//�������������
		UInt32 uFISHEATmaxtimes;
		UInt32	uFISHEATgivecoin;
		UInt32	uFISHEATeatfish;
		UInt32 uFISHEATmaxpresscount;


		//������
		UInt32 uRankGameTimes;

		List<UInt32> uRankGameBuyTimesCost;
		List<UInt32>	uRankGameNum;
		UInt32 uRankGamewinScore;
		UInt32	uRankGameFailScoreCost;
		UInt32	uRankGameReportNum;
		UInt32 uRankGameTime;
		UInt32 uRankGameShopRefreshTimes;
		UInt32 uRankGameShopRefreshTime;
		UInt32 uRankGameShopRefreshCost;
		UInt32 uRankGameOpenLevel;
		UInt32 uRankGameRewardResetTime;
		UInt32 uRankGameMaxFinishTime;

		 
		//��������
		UInt32 uCatTaskInitGold;
		UInt32 uCatTaskAddGold;
		UInt32 uCatTaskMaxGold;

		//����è
		UInt32 uBusinessCatInitGold;
		UInt32 uBusinessCatAddGold;
		UInt32 uBusinessCatMaxGold;
	
		//�����淨�����ȼ������ݹ���ȼ���
		
		//èԱ�⿪���ȼ�
		UInt32 uGuildOpenBattle;
		//èԱ�⿪���ȼ�
		UInt32 uGuildOpenYuanWai; 
		//����è�⿪���ȼ�
		UInt32 uGuildOpenBusinessCat;
		//����ѵ��
		UInt32 uGuildOpenKitchen ;
		//Ʊ�Ѽ���
		UInt32 uGuildOpenTicket ;
		//ǩ�������ȼ�
		UInt32 uGuildOpenSign ;

		//��������
		UInt32 uGuildOpenCatTask; 

		//�������ڵ���д����Ϊ[ʱ��,id,ʱ��,id]
		List<UInt32> uSystemRefreshTime;

		//��ҳ�ʼ����
		std::string sRole1; 
		UInt32 uRolePower1; 
		std::string sRole2; 
		UInt32 uRolePower2; 
		std::string sRole3; 
		UInt32 uRolePower3; 

		//�ؿ����ʱ�����ӵ����ֵ
		UInt32 uCopyFinishTimeDeviation; 

		//�����淨ˢ��ʱ��
		
		//ǩ��
		UInt32 uConsortSignReset ;
		
		//Ʊ�Ѽ���
		UInt32 uTicketFriendReset;
		//����ѵ��
		UInt32 uEyesightReset ;
		//������ѵ
		UInt32 uKitchenReset ;
		//��������
		UInt32 uXingXiaReset ;
		//�ڲ�ѵ��
		UInt32 uEloquenceReset;
		//ľè������
		UInt32 uWoodencatReset;

		//�ͷ������������
		UInt32 uRewardLotteryFreeOnceMaxTimes;

		//ľè�󷨹���buff����
		//����buff����
		UInt32 uWoodencatattackbuffcost;

		//����buff����
		UInt32 uWoodencatdefensebuffcost ;
		//����buff����
		UInt32 uWoodencatCritbuffcost ;


	};
	
	class GlobalValueManager: public Singleton<GlobalValueManager>
	{
	
	public:
		bool initGlobalValue();
		bool checkUTFStringIsSame(String uString,String str);
		void getListFromString(String str,List<UInt32> &list);


		GlobalValue &getGlobalValue();
		GlobalValue *getGlobalValueData();




	
		
		
	};

	static GlobalValue gGlobalValueTemplate;




	 



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
