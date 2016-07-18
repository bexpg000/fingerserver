#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class EquipDebrisManager : public MainThread
    {
    public:
        EquipDebrisManager();
        virtual ~EquipDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // �����Ƭ���Ƿ�����
        bool isEquipDebrisFull();
        bool isEquipDebrisFull(List<TemplateId>& equipDebrisList);
        bool isEquipDebrisFull(List<AwardLine>& awardList);
        // �����Ƭ���ո�����
        UInt8 getEquipDebrisFieldSpaceSlot();
        // ��������
        void lockEquipDebris(const Guid& itemId);
        // ��������
        void unLockEquipDebris(const Guid& itemId);

        // �����Ƭ
        const EquipDebrisData* getEquipDebrisByTemplateId(TemplateId templateId);
        const EquipDebrisData* getEquipDebrisByGuid(const Guid& guid);

        // ������Ƭ�������Ƭȫ������
        void destroyEquipDebris(Guid equipDebrisId);
        // ɾ����Ƭ, ����Ϊ0��ʾȫ��ɾ��
        void delEquipDebris(Guid equipDebrisId, UInt32 count = 0);
        // ��ձ���
        void clearBag();

        // ������Ƭ
        const EquipDebrisData*  createEquipDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // �ϳ�
        ErrorId combineEquip(const Guid& equipDebrisId, Guid& equipId);
    private:
        // ʹ����Ƭ
        bool useEquipDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<EquipDebrisData>* mEquipDebrisList;
        Map<Guid, EquipDebrisData*> mEquipDebrisDataMap;
        Map<TemplateId, EquipDebrisData*> mEquipDebrisTemplateMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__
