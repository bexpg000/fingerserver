#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__

#include "MainThread.h"
namespace Lynx
{
struct polishStoneEle
	{
		UInt64 uid;
		UInt32 count;
	};

    class Player;
	class EquipManager;
	class ItemManager : public MainThread,public Singleton<ItemManager>
    {
    public:
        ItemManager();
        virtual ~ItemManager();

        bool initial(Player* player);
        void release();
      
		friend class EquipManager;
    public:
  
        //������Ʒ�Ƿ�ռ��
		bool isItemFull(UInt32 bagTab);
		//�ձ�������
		UInt32 getEmputyNum(UInt32 bagTab);
		// ���ٵ���, ��ѵ���ȫ������
        void destroyItem(Guid itemId);
        // ɾ�����ߣ�����Ϊ0��ʾȫ��ɾ��
        UInt32 delItem(const Guid itemUId, UInt32 count = 0);

		void delOneItem(ItemData* itemData);

		//������Ʒ�ı�ǩҳ���Ӷ�Ӧ������map��ɾ������Ʒ���������͸�����Ʒ��ص�ͳ�Ʋ���
		void delTypeMapEle(ItemData* itemData);

		//������Ʒ�ı�ǩҳ���Ӷ�Ӧ������map����Ӹ���Ʒ���������͸���Ʒ��ͳ�Ʋ����Լ����������Ժ����ݵ�����
		void addTypeMapEle(ItemData* itemData);

		UInt32 getRandomValue(UInt32 minValue, UInt32 maxValue);

        // ��õ���ͨ��GUID
         ItemData* getItemByGuid(const Guid& itemGuid);

        // ʹ�õ���
        errorId useItem(const Guid& itemUId, UInt32 count);
        // �������
		//ԭ��Ϸ
       // void addItem(const ItemData& itemData, UInt8 source);
        // ����Ϸ��ȡ��Ʒ,templateId Ϊitemid
		List<UInt64> getNewItems(Guid templateId, UInt32 count);
	
		List<UInt64> getNewItemsDefrent(List<IdAndCount> itemsList);

		List<UInt64> getNewItemsSame(Guid templateId, UInt32 count);

		//����itemid���һ�����ӵ���Ʒ
		UInt64 addOneItem(Guid templateId, UInt32 StackAmount);

		
		//����ĳ����Ʒ����Ϣ
		void updateOneItem(ItemData* itemData, UInt32 StackAmount);
		
		//��� count �� itemid ����Ʒ
		List<UInt64> addItems(Guid templateId, UInt32 count);

		//��ȡ�ǿյı���������
		UInt32 getNoneEmptyBags(UInt32 bagTap);

		//����ռ�ü���������
		UInt32 calculateCount(Guid templateId, UInt32 count);

		List<ItemData>* getItemDatasList(void);

      
        errorId sellItem(const List<Guid>& itemList, UInt32& getGold);

		std::string convertItemDataToJson();

		void addEquipAttribute(ItemData* itemData);

		void addRandomAttribute(ItemData* itemData, UInt32 randomID);
		
		//װ��ǿ��
		void enhanceEquip(UInt64 equipID, List<UInt64> materialLists);

		UInt32 getEnhanceLevelLimit(UInt32 starLv);

		UInt32 getTotalEnhanceLv(UInt32 starLv, UInt32 curEnhanceLv);
		
		//ϴ��װ��
		void  polishEquip(UInt64 objEquipUid, UInt64 materialEquipUid, UInt32 objRdAttrIndex);

		//�ָ���ϴ��֮ǰ��ԭ����
		void  polishResetEquip(UInt64 objEquipUid);

		//��ȡ�µ����������Ŀ��װ���е�Ʒ��
		UInt32 getAttrQuality(const RandomAttribute &randomAttr, EquipTemplate * materialTemplate);

		UInt32 getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		//����Ʒ��map

		void creatQualityMap(ItemData * materialItemData,AttrPolishTemplate *materialPolishTem,bool& orangeFlag,Map<UInt32, RandomAttribute*> &orangeRdMap,
			UInt32& orangeRate, bool& purpleFlag,Map<UInt32, RandomAttribute*> &purpleRdMap,UInt32& purpleRate,
			Map<UInt32, RandomAttribute*> &totalRdMap,UInt32& totalRate);
		
		//��ɫ���Աسɹ��滻�߼�

		void orangeSuccess(Map<UInt32, RandomAttribute*> &orangeRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &orangeRate);

		//��ɫ���Աسɹ��滻�߼�

		void purpleSuccess(Map<UInt32, RandomAttribute*> &purpleRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &purpleRate);

		//�������Ͻ���������滻�߼�

		void totalAttrRandom(Map<UInt32, RandomAttribute*> &totalRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &totalRate);
		
		void gemCombine(UInt64 gem1Uid, UInt64 gem2Uid);

		void gemCombineOnce(void);

		//��ʯ��Ƕ��װ����
		void gemSetEquip(UInt64 equipUid,UInt32 equipGemSlot, UInt64 gemUid);

		//��ʯж��, ����ж�º��µı�ʯuid�����Ϊ0�����ʾж��ʧ��
		UInt64 unload(ItemData * equipData, UInt32 gemIndex);

		//װ����ʯ�ϳ�
		void equipGemCombine(UInt64 equipUid, UInt32 gemSlot);

		//�ϳɶ����ʯ
		List<UInt64> combineGemsList(UInt64 gemItemID, UInt32 count);

		void combineGems( List<UInt64> &gemUidList, List<UInt64> &newGemList,GemCombineTemplate * gemCombineTemplate,const UInt32 &newGemCount, UInt32 & count);

		void constructGemList(List<UInt64> &uidList,ItemTemplate * gemTemplate, List<UInt64> &gemUidList);

		void sendCombineRes(List<UInt64> &newGemList, bool success = true, errorId errorId = LynxErrno::None);

		void equipGemUnload(UInt64 equipUid, UInt32 gemSlot);

  private:
	

        Player* mPlayer;
        List<ItemData>* m_pListItemDatas;
        Map<Guid, ItemData*> m_mapItemUuid;
		//�����ItemID����uuid,�жϱ������Ƿ���ڴ���Ʒ���Լ���Ʒ��������
	    Map<Guid,List<Guid> > m_mapItemId2Uids;
		
		//װ��UID ��map
		Map<UInt64, ItemData*> m_mapUidEquipType;

		//��װ������Ʒ,����ƷUID map
		Map<UInt64, ItemData*> m_mapUidItemType;

		//�������͵�UID map Ŀǰû�õ�
		Map<UInt64, ItemData*> m_mapUidOtherType;
		
		//װ����������Ʒռ�еĵ�һ����ǩҳ���������
		UInt32 m_nEquipOccupies;

		//����ռ�еĵڶ�����ǩҳ���������
		UInt32 m_nItemOccupies;

		//������Ʒռ�õĵ��������������

		UInt32 m_nOtherOccupies;
		
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__
