#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"


namespace Lynx
{
	class Player;

	enum RewardLottery
	{
		RL_ONCE_FREE = 1,//
		RL_ONCE_TICKET = 2,
		RL_ONCE = 3,		
		RL_TEN_FREE	= 4,//
		RL_TEN	= 5,
		RL_VIP	= 6,
		RL_TEN_TICKET	= 7,
// 		���
// 		����ȯ
// 		һ��
// 		���ʮ��
// 		ʮ����
// 		vip��
// 		ʮ����ȯ

	};

//1,2,3����
	//4,5,7Ϸ��
	//6 vipֻ��һ��

	
	struct GiftReq
	{
		GiftReq(): giftID(0),giftNum(0){}

		UInt32 giftID;
		UInt32 giftNum;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				giftID = root["giftID"].asUInt();
				giftNum = root["giftNum"].asUInt();
			}
		}
		LYNX_MESSAGE_1(OPEN_GIFT_REQ, GiftReq,std::string, strReceive);
	};

	struct BoxReq
	{
		BoxReq(): boxID(0),boxNum(0){}

		UInt32 boxID;
		UInt32 boxNum;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				boxID = root["boxID"].asUInt();
				boxNum = root["boxNum"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_BOX_REQ, BoxReq,std::string, strReceive);

	};

	struct RewardLotteryMsgReq
	{
		RewardLotteryMsgReq(){}

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
			}
		}

		LYNX_MESSAGE_1(OPEN_LOTTERY_MSG_REQ, RewardLotteryMsgReq,std::string, strReceive);

	};
	struct RewardLotteryMsgResp
	{
		RewardLotteryMsgResp(): onceLeftTime(0),tenLeftTime(0),couponOnceNum(0),couponTenNum(0){}

		UInt32 onceLeftTime;
		UInt32 tenLeftTime;
		UInt32 couponOnceNum;
		UInt32 couponTenNum;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["onceLeftTime"] = Json::Value(onceLeftTime);
			root["tenLeftTime"] = Json::Value(tenLeftTime);
			root["couponOnceNum"] = Json::Value(couponOnceNum);
			root["couponTenNum"] = Json::Value(couponTenNum);


			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RewardLotteryReq
	{
		RewardLotteryReq():reqType(0){}

		UInt32 reqType;// 1	��ѣ�2����ȯ��3һ�飬4	���ʮ�飬5	ʮ���飬6	vip�飬7����ȯʮ���飬8 ����ȯvip��


		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				reqType = root["reqType"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_REWARD_LOTTERY_REQ, RewardLotteryReq,std::string, strReceive);

	};


	struct RewardLotteryResp
	{
		RewardLotteryResp(): reqType(0),result(0),onceLeftTime(0),tenLeftTime(0),couponOnceNum(0),couponTenNum(0),gold(0){}

		UInt32 reqType;
		UInt32 result;
		// 		UInt32 couponNum;
		List<UInt32> newServant;//1 �����Ӷ��
		// 		UInt32 leftTime;
		UInt32 gold;
		UInt32 onceLeftTime;
		UInt32 tenLeftTime;
		UInt32 couponOnceNum;
		UInt32 couponTenNum;

		List<Award> awards;
		List<Goods> ends;
		Json::Value allAttr;//������Ʒ����

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["onceLeftTime"] = Json::Value(onceLeftTime);
			root["tenLeftTime"] = Json::Value(tenLeftTime);		
			root["couponOnceNum"] = Json::Value(couponOnceNum);
			root["couponTenNum"] = Json::Value(couponTenNum);
			root["gold"] = Json::Value(gold);

			for(List<UInt32>::Iter *item = newServant.begin();item !=NULL;item = newServant.next(item) )
			{			
				root["newServant"].append(item->mValue);
			}


			for(List<Award>::Iter * it = awards.begin(); it != NULL; it = awards.next(it))			
			{
				Json::Value son;
				for(List<Goods>::Iter * iter = it->mValue.award.begin(); iter != NULL; iter = it->mValue.award.next(iter))
				{
					Json::Value leaf;
					leaf.append(iter->mValue.resourcestype);
					leaf.append(iter->mValue.subtype);
					leaf.append(iter->mValue.num);

					son["award"].append(leaf);
				}
				root["awards"].append(son);
			}

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);

				root["ends"].append(son);
			}

			root["allAttr"] = Json::Value(allAttr);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct LotteryDrawReq
	{
		LotteryDrawReq(): drawType(0){}

		UInt32 drawType;//1 2 3 ���飬ʮ���飬ʱ���
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				drawType = root["drawType"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_LOTTERY_DRAW_REQ, LotteryDrawReq,std::string, strReceive);

	};
	struct LotteryDrawResp
	{
		LotteryDrawResp(): drawType(0){}

		UInt32 drawType;
		List<Goods> goodsList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["drawType"] = Json::Value(drawType);

			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				root["resourcestype"].append(iter->mValue.resourcestype);
				root["subtype"].append(iter->mValue.subtype);
				root["num"].append(iter->mValue.num);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct GiftResp
	{
		GiftResp(): ID(0){}

		UInt32 ID;
		List<Goods> goodsList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ID"] = Json::Value(ID);

			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				root["resourcestype"].append(iter->mValue.resourcestype);
				root["subtype"].append(iter->mValue.subtype);
				root["num"].append(iter->mValue.num);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct AwardResp
	{
		AwardResp(): ID(0){}

		UInt32 ID;
		List<Goods> goodsList;
		List<Goods> CardsList;
		List<Goods> monsterDropList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ID"] = Json::Value(ID);			
			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["goodsList"].append(son);
			}
			for(List<Goods>::Iter * iter = CardsList.begin(); iter != NULL; iter = CardsList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["CardsList"].append(son);			
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CGPropertyChange
	{
		CGPropertyChange(): sFlag(""),sKey(""),sValue(""){}

		String sFlag;
		String sKey;
		String sValue;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				sFlag = root["sFlag"].asCString();
				sKey  = root["sKey"].asCString();
				sValue = root["sValue"].asCString();
			}
		}

		LYNX_MESSAGE_1(PROPERTY_CHAGE_REQ, CGPropertyChange,std::string, jsonStr);
	};


	struct PropertyChange
	{
		PropertyChange(): sFlag(""),sKey(""),sValue(""){}

		String sFlag;
		String sKey;
		String sValue;
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["sFlag"] = Json::Value(sFlag.c_str());
			root["sKey"] = Json::Value(sKey.c_str());
			root["sValue"] = Json::Value(sValue.c_str());
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct CGPropertysChange
	{
		CGPropertysChange(): type(0){}

		UInt32 type;//���� 1 ���� 2 ������� 3 ������
		List<Goods> goodsList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��      
			{
				type = root["type"].asUInt();

			}
		}

		LYNX_MESSAGE_1(PROPERTYS_CHAGE_REQ, CGPropertyChange,std::string, jsonStr);
	};


	struct PropertysChange
	{
		PropertysChange(): type(0){}

		UInt32 type;//���� 1 ���� 2 ������� 3 ������
		List<Goods> goodsList;
		std::string convertDataToJson()
		{				
			Json::Value root;     	
			root["type"] = Json::Value(type);
			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				Json::Value son; 
				son["resourcestype"]= Json::Value(iter->mValue.resourcestype);
				son["subtype"]= Json::Value(iter->mValue.subtype);
				son["num"]= Json::Value(iter->mValue.num);
				root["goodsList"].append(son);
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	class GiftManager : public Singleton<GiftManager>
	{
	public:
		static void onOpenGiftReq(const  ConnId& ,GiftReq & );		//����� 
		static void onOpenBoxReq(const  ConnId& ,BoxReq & );	//������
		static void onqiyongReq(const  ConnId& ,LotteryDrawReq & );//
		static void onPropertyChang(const  ConnId& ,CGPropertyChange & );		//���Ը���

		static void onRewardLotteryMsgReq(const  ConnId& ,RewardLotteryMsgReq & );	//�齱 �����ͷ���

		static void onRewardLotteryReq(const  ConnId& ,RewardLotteryReq & );	//�齱

		List<UInt32> getRewardLotteryIDs(UInt32 typeID);

		UInt32 getOpenLotteryID();

		UInt32 getRewardLotteryAwardID(Guid playerID,UInt32 id,UInt32 &awardID);

		UInt32 checkRewardLotteryTen(Guid playerID,UInt32 typeID);

		UInt32 checkRewardLotteryTenCount(Guid playerID,UInt32 typeID,UInt32 &count);

		void getLotteryLeftTime(Guid playerID,UInt32 &onceTime,UInt32 &tenTime);

		void getAllAttr(Guid playerID,UInt32 resType,UInt32 subType,Json::Value &jsonValue);

		void servanNeedChangeToPiece(Guid playerID, List<Award> &awards,List<Goods> &ends);

		void getGift(const ConnId& connId ,UInt32 id,ChapterEndResp &resp);//��ȡ���

		UInt32 modifyCoin(Guid playerID,Int32 num);//ͭǮ
		UInt32 modifyExp(Guid playerID,Int32 num);//����
		UInt32 modifyItem(Guid playerID,Guid templateID,Int32 num);//װ��
		UInt32 modifyShengWang(Guid playerID,Int32 num);//����
		UInt32 modifyRhyme(Guid playerID,Int32 num);//����		 

		UInt32 modifyYuanBao(Guid playerID,Int32 num);//Ԫ��
		UInt32 modifyMercenary(Guid playerID,Int32 num);//Ӷ��
		UInt32 updateCounter(Guid playerID,UInt32 boxID,Int32 num);//������
		UInt32 updateRewardCounter(Guid playerID,UInt32 boxID,Int32 num);//����������
		UInt32 modifyCounter(const ConnId& connId,UInt32 counterType,UInt32 counterID,Goods &goods);//���ӻ���������
		UInt32 updateFood(Guid playerID,UInt32 boxID,Int32 num);//��ʳ�ݴ浵

		UInt32 updateStrength(Guid playerID,UInt32 boxID,Int32 num);//�����浵

		UInt32 updateStageDatas(Guid playerID,List<Goods> itemList);



		// 		 UInt32 updateCopyCounter(Guid playerID,UInt32 copyID,Int32 num);//�����
		// 		 UInt32 subGold(Guid playerID,Int32 num);//ͭǮ
		// 		 UInt32 subExp(Guid playerID,Int32 num);//����
		// 		 UInt32 subItem(Guid playerID,Guid templateID,Int32 num);//װ��
		// 		 UInt32 subStength(Guid playerID,Int32 num);//����
		// 		 UInt32 subShengWang(Guid playerID,Int32 num);//����
		// 		 UInt32 subYuanBao(Guid playerID,Int32 num);//Ԫ��
		// 		 UInt32 subMercenary(Guid playerID,Int32 num);//Ӷ��

		UInt32 PlayerItemChangeResult(Guid playerID,UInt32 reFlashType,List<Goods> &itemList);
		UInt32 addToPlayer(Guid playerID,UInt32 reFlashType,List<Goods> itemList);
		void sendToPlayer(Guid playerID,UInt32 type,List<Goods> itemList,List<UInt64> newItemUids);
		void sendItemListChange(Guid playerID,List<UInt64> ItemUids);
		UInt32 subFromPlayer(Guid playerID,Goods goods);
		UInt32 getEmputyBagNum(Guid playerID);
		void getContentByID(UInt32 itemID,List<Goods> &ItemList);
		void getChapterAward(const  ConnId& connId,List<AwardMonsterDamage> awardMonsterList);
		void getAwardByID(UInt32 itemID,UInt32 num,List<Goods> &ItemList);
		void getAwardCardByID(UInt32 itemID,List<Goods> &ItemList);
		void updateSingleProperty(Guid playerID,String strFlag,String strData);
		void ChoiseNFromWeigthList(UInt32 num , List<Resource> inList,List<Goods> &outList);
		//�������//Ӷ���齱	

		void combineSame( List<Goods> &goodsList);//�ϲ���ͬ

		void splitKind( List<Goods> &goodsList, List<Goods> &kindgoodsList,UInt32 kind);//���һ����Դ

		void checkJewelry(List<Goods> &itemList,PlayerFireConfirmData &fireConfirmData,UInt32 isFull);
	};

	struct StepContent
	{
		UInt32 step;
		UInt32 contentID;
	};
	struct Counter
	{
		UInt32 itemID;
		List<StepContent> stepContent;
	};

	enum Lottery
	{
		LOTTERY_TYPE_TO_ID1 = 61001,//�齱���ͼ���ʹ��ID
		LOTTERY_TYPE_TO_ID2 = 61002,
		LOTTERY_TYPE_TO_ID3 = 61003,		
		LOTTERY_INTERVAL	= 48*60*60,//ʱ����ʱ��
		LOTTERY_MAX_DRAW	= 10,
	};


}


#endif // __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__




// resType	subType		
// 1	1	ͭǮ	��ɫ��Դ
// 1	2	Ԫ��	
// 1	3	����	
// 1	4	����	
// 1	5	������	
// 1	6	�ϻ�	
// 2	1	���ɲ�	����ϵͳ
// 2	2	������	
// 2	3	���ɲ�	
// 3	1	��ʳ���ƴ���	��ʳ��
// 3	2	ȫ����	
// 3	3	��ϡ����	
// 3	4	��β����	
// 5	1	��ʯ��Ƭ1��	��ʯϵͳ
// 5	2	��ʯ��Ƭ2��	
// 5	3	��ʯ��Ƭ3��	
// 5	4	��ʯ��Ƭ4��	
// 5	5	��ʯ��Ƭ5��	
// 5	6	��ʯ��Ƭ6��	
// 5	7	��ʯ��Ƭ7��	
// 5	8	��ʯ��Ƭ8��	
// 5	9	��ʯ��Ƭ9��	
// 5	10	��ʯ��Ƭ10��	
// 5	11	��ʯ��Ƭ11��	
// 5	12	��ʯ��Ƭ12��	
// 6	1	Ӷ����������1	Ӷ������
// 6	2	Ӷ����������2	
// 6	3	Ӷ����������3	
// 6	4	Ӷ����������4	
// 6	5	Ӷ����������5	
// 7	id	Ӷ��	Ӷ��
// 8	id	Ӷ����Ƭ	Ӷ��
// 9	id	Ӷ��װ��	Ӷ��
// 10	1	����֮��	Ӷ���һ�
// 11	id	material	װ������
// 12	id	new_gem	��ʯ
// 13	id	new_jewelry	��Ʒ
// 14	id	fashion	ʱװ
// 16	id	hoarstoneequip	��ʯ����
// 99	1	ͨ�����̴���	�и�����
// 99	2	�����г�����	
// 99	3	��ʯ��������	
// 99	4	�м���������	
// 99	5	��ɽ����	��ɽ����
// 99	6	ʮ���ڹ�	ʮ���ڹ�
// 99	7	������ս�Ѷ�1	ԭ����¥
// 99	8	������ս�Ѷ�2	
// 99	9	������ս�Ѷ�3	
// 99	10	��������	
// 99	11	��λ��	������
// 99	12	������	
// 99	13	���Ҷ�	
// 99	14	�ٻ�boss	����þ�
// 99	15	��ʱ�ٻ�boss	
// 99	16	����boss	
