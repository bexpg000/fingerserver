#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class HeroDebrisManager : public MainThread
    {
    public:
        HeroDebrisManager();
        virtual ~HeroDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // ���Ӣ����Ƭ���Ƿ�����
        bool isHeroDebrisFull();
        bool isHeroDebrisFull(List<TemplateId>& heroDebrisList);
        bool isHeroDebrisFull(List<AwardLine>& awardList);
        // ���Ӣ����Ƭ���ո�����
        UInt8 getHeroDebrisFieldSpaceSlot();
        // ��������
        void lockHeroDebris(const Guid& itemId);
        // ��������
        void unLockHeroDebris(const Guid& itemId);

        // �����Ƭ
        const HeroDebrisData* getHeroDebrisByTemplateId(TemplateId templateId);
        const HeroDebrisData* getHeroDebrisByGuid(const Guid& guid);

        // ������Ƭ�������Ƭȫ������
        void destroyHeroDebris(Guid heroDebrisId);
        // ɾ����Ƭ, ����Ϊ0��ʾȫ��ɾ��
        void delHeroDebris(Guid heroDebrisId, UInt32 count = 0);
        // ��ձ���
        void clearBag();

        // ������Ƭ
        const HeroDebrisData*  createHeroDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // �ϳ�Ӣ��
        ErrorId combineHero(const Guid& heroDebrisId, Guid& heroId);
    private:
        // ʹ����Ƭ
        bool useHeroDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<HeroDebrisData>* mHeroDebrisList;
        Map<Guid, HeroDebrisData*> mHeroDebrisDataMap;
        Map<TemplateId, HeroDebrisData*> mHeroDebrisTemplateMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__
