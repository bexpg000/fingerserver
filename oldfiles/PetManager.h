#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class PetManager : public MainThread
    {
    public:
        PetManager();
        virtual ~PetManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);
        void calculatePetData(const Guid& petId);

       // const PetData* createPet(const TemplateId& templateId, bool promp = true);
        bool destroyPet(Guid guid);
       // const PetData* getPetDataByGuid(const Guid& guid) const;
      //  const PetData* getPresentPet() const;

        // ���� �ĳ������ˣ�
        ErrorId awakenPet(const Guid& petId, const List<Guid>& petList, const List<Guid>& petDebrisList,
            UInt32& upLevel, TemplateId& newTemplateId);
        // ����ι��
        ErrorId refinePet(const Guid& petId, UInt32& upLevelCount, const Guid& itemId);
        // һ��ι��
        ErrorId refinePet(const Guid& petId, UInt32& upLevelCount);
        // ��ͨ����
     //   ErrorId getCommonTalent(const Guid& petId, PetTalentData& petTalentData);
        // �߼�����
      //  ErrorId getAdvanceTalent(const Guid& petId, PetTalentData& petTalentData);
        // �����츳
        ErrorId saveTalent(const Guid& petId);
        // ȡ���츳
        ErrorId cancelTalent(const Guid& petId);
        // ���ó���
        ErrorId setPetPresentPos(const Guid& petId, UInt8 pos);
        // �������
        ErrorId advancePet(const Guid& petId);

    private:
        // ��������ӵȼ����飬��������
        ErrorId addLevelExp(const Guid& petId, UInt32& upLevelCount, UInt32 incLevelExp);
        // ����������Ǽ����飬 ��������
        ErrorId addStarLevelExp(const Guid& petId, TemplateId& templateId, UInt32 incStarLevelExp);


    public:
        void increasePetLevel(const Guid& petId, UInt16 offset);
        void reducePetLevel(const Guid& petId, UInt16 offset);
        void increasePetLevelExp(const Guid& petId, UInt32 offset);
        void reducePetLevelExp(const Guid& petId, UInt32 offset);
        void increasePetStarLevelExp(const Guid& petId, UInt32 offset);
        void reducePetStarLevelExp(const Guid& petId, UInt32 offset);

    private:
        Player* mPlayer;
     //   PlayerPetData* mPlayerPetData;
      //  Map<Guid, PetData*> mPlayerPetDataMap;
     //   PetData* mPresentPetData;
     //   Map<Guid, PetTalentData> mPetTalentDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__
