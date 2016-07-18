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
		void addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count);

		UInt32 getAwardCount(UInt32 resType, UInt32 subTypeID);

		void addAwardMaterialsList(const List<AwardData> & awardList);

		//������Ʒ�߼�
		void costMaterialsList(const List<AwardData> & awardList);

		void costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count);

		void checkExp(const  ConnId& connId);

		void levelUp(const  ConnId& connId,UInt32 level);

		void addAwardGetSendJson(const List<AwardData> & awardList);

		private:
		

		Player* m_pPlayer;

		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__