#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class EquipManager : public MainThread
    {
    public:
        EquipManager();
        virtual ~EquipManager();

        bool initial(Player* player);
        void release();
        void calculateEquipData(UInt8 equipPresentPos);

    public:
        // ���װ�����Ƿ�����
        bool isEquipFull();
        bool isEquipFull(List<TemplateId>& equipList);
        bool isEquipFull(List<AwardLine>& awardList);
        // ��ñ���װ�����ո�����
        UInt8 getEquipFieldSpaceSlot();
        // ��������
        void lockEquip(const Guid& equipId);
        // ��������
        void unLockEquip(const Guid& equipId);

        // �ݻ�װ��
        void destroyEquip(Guid equipId);
        // �ݻ�ȫ��װ��
        void destroyAllEquip();

        // ʹ��װ��
        ErrorId useEquip(Guid equipId, UInt8 targetPos);
        // ���װ��
        const EquipData* createEquip(TemplateId equipTemplateId, UInt8 source, bool promp = true);
        // ����װ��ǿ���ȼ�
        void setEquipRefine(const Guid& equipId, UInt32 level);
        // װ����ħ
        ErrorId enchantEquip(const Guid& equipId, UInt32& enchantLevel);
        // ǿ��װ��
        ErrorId refineEquip(const Guid& equipId, UInt32& refineLevel);
        // �Զ�ǿ��װ��
        ErrorId autoRefineEquip(const Guid& equipId, UInt32& refineLevel);

        const EquipData* getEquipDataByGuid(const Guid& equipId);

        // �����ӿ�
        ErrorId smeltEquip(const Guid& equipId, UInt32& gold, UInt32& soul);

        // �����ӿ�
        ErrorId rebornEquip(const Guid& equipId, UInt32& gold, List<Guid>& itemList);

        // ����װ������λ��
        void setEquipPresentPos(const Guid& equipId, UInt8 pos);

        ErrorId sellEquip(List<Guid>& equipList, UInt32& getGold);
     
    private:
        Player* mPlayer;
        List<EquipData>* mEquipList;
        Map<Guid, EquipData*> mEquipMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__
