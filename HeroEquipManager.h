#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class HeroEquipManager : public MainThread, public Singleton<HeroEquipManager>
	{
	public:
		HeroEquipManager();
		virtual ~HeroEquipManager();

		bool initial(Player* player);
		void release();

		//�����������ʱ��������������⣬���װ����������ʯ�۽���
		void checkLvUpActive();

		//����һ�óɾ�ʱ�����óɾͼ��

		void checkAchievementActive();

		//������ֶ�����ĳ����ʯ��

		bool manualActive(UInt32 equipPos, UInt32 gemSlot);
		
		//��ʯ��λ��1��6��listȡ��ֵӦ���ø�ֵ��һ
		bool getGemSlotOpenState(UInt32 equipPos, UInt32 gemSlotPos);

		bool getGemSlotOpenState(Map<UInt32, HeroEquipData *>::Iter * equipMapIter, UInt32 gemSlotPos);
		
		void enhanceEquip(UInt64 playerUid, UInt32 equipPos, UInt32 enhanceType);

		errorId enhanceLvUp(HeroEquipData& equipData, UInt32& playerLv, Map<UInt64, MaterialData*> &changeMaterialData, NewEquipLvTemplate * newEquipLvTemp);

		List<HeroEquipData> * getHeroEquipList(void);
		
		std::string convertDataToJson(void);

		void equipSetGem(UInt32 equipPos, UInt32 gemPos,UInt64 gemId);

		void sendSetRespToClient(errorId  errorId, UInt64 gemId, UInt32 equipPos, UInt32 equipGemSlot);

		void equipSetGemOnce(UInt32 equipPos);

		void sendSetOnceRespToClient(errorId errorId, Map<UInt32, GemData*>* sameGemMap = NULL, Map<UInt32, HeroEquipData *>::Iter * heroEquipData = NULL);

		void equipGemLvUp(UInt32 equipPos, UInt32 gemSlot);

		void equipGemUnload(UInt32 equipPos, UInt32 gemSlot);

		UInt64 changeCharacterEquip(UInt64 charactorId);

	private:

		void checkEquipActive(UInt32 equipPos, UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter * equipMapIter, bool & changeFlag);

		void dealEquipActive(UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter *equipMapIter, UInt64 equipId, UInt32 equipUnlockLv,bool & changeFlag );

		void initEquipGemBit(HeroEquipData &heroEquipData,  UInt32& level,UInt32 & condKey, UInt32 gemSlotBit);

		void checkGemActive(HeroEquipData &heroEquipData,bool & changeFlag);

		//����ȼ�����ʱ�򣬼�����Լ���ı�ʯ�ۣ��������ɾͺͽ�ң�Ԫ���ֶ�������
		void dealGemActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey,bool & changeFlag);

		//�����ֶ������ı�ʯ�ۣ�ͭǮ����Ԫ�����߽�����
		void dealGemManualActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey,UInt32 gemCondValue,errorId & errorId);

		static bool lvCompare(UInt32 lv1, UInt32 lv2); 

		Player* m_pPlayer;
		//װ��λ�ö�Ӧ��װ����Ϣ,map key��1��ʼ�����6
		Map<UInt32, HeroEquipData *> m_mapHeroEquipData;
		List<HeroEquipData> * m_pHeroEquipList;
		
		UInt32 *m_pHeroModel;
		UInt32 *m_pPlayerLv;
	
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__
