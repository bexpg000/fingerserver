#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class PetDebrisManager : public MainThread
    {
    public:
        PetDebrisManager();
        virtual ~PetDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // ��������Ƭ���Ƿ�����
        bool isPetDebrisFull();
        bool isPetDebrisFull(List<TemplateId>& petDebrisList);
        bool isPetDebrisFull(List<AwardLine>& awardList);
        // ��ó�����Ƭ���ո�����
        UInt8 getPetDebrisFieldSpaceSlot();
        // ��������
        void lockPetDebris(const Guid& itemId);
        // ��������
        void unLockPetDebris(const Guid& itemId);

        // �����Ƭ
        const PetDebrisData* getPetDebrisByTemplateId(TemplateId templateId);
        const PetDebrisData* getPetDebrisByGuid(const Guid& guid);

        // ������Ƭ�������Ƭȫ������
        void destroyPetDebris(Guid petDebrisId);
        // ɾ����Ƭ, ����Ϊ0��ʾȫ��ɾ��
        void delPetDebris(Guid petDebrisId, UInt32 count = 0);
        // ��ձ���
        void clearBag();

        // ������Ƭ
        const PetDebrisData* createPetDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // �ϳɳ���
        ErrorId combinePet(const Guid& petDebrisId, Guid& petId);
    private:
        // ʹ����Ƭ
        bool usePetDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<PetDebrisData>* mPetDebrisDataList;
        Map<TemplateId, PetDebrisData*> mPetDebrisTemplateMap;
        Map<Guid, PetDebrisData*> mPetDebrisDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__
