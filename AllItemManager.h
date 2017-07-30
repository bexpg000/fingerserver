#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;

	class AllItemManager : public MainThread ,public Singleton<AllItemManager>
	{
	public:
		AllItemManager();
		virtual ~AllItemManager();

		bool initial(Player* player);
		void release();

		std::string convertAllMaterialDataToJson();
		
		//resType ��Դ�����ͣ���Ӧ��ͬ�ı�,ConstDefinition.h���ж�Ӧ�����ͺ�
		//subTypeID �����ͣ���Ӧһ����Դ�в�ͬ��id������ͬ���ñ����id
		//�����Ʒ������
		void  addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count, ReturnItemEle& rtItemEle,UInt32 systemID);

		UInt32 getAwardCount(UInt32 resType, UInt32 subTypeID);

		//�������б��е���Ʒƴ���ַ���
		void convertItemListToStr(const List<ReturnItemEle> & rtItemEleList, Json::Value & root);

		void addAwardMaterialsList(const List<AwardData> & awardList,UInt32 systemID);

		//������Ʒ�߼�
		void costMaterialsList(const List<AwardData> & awardList,UInt32 systemID);

		bool costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count,UInt32 systemID);

		
		void addAwardGetSendJson(const List<AwardData> & awardList,UInt32 systemID);

		private:
		

		Player* m_pPlayer;

		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__