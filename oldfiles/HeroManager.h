#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class HeroManager : public MainThread
    {
    public:
        HeroManager();
        virtual ~HeroManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);
        void calculateHeroData(const Guid& heroId);

        const HeroData* createHero(const TemplateId& templateId, bool promp = true);
        bool destroyHero(Guid guid);
        const HeroData* getHeroDataByGuid(const Guid& guid) const;
        const HeroData* getHeroDataByPresentPos(const UInt8& presentPos) const;
        const Map<UInt8, HeroData*>& getPresentHeroDataMap() const { return mPlayerPresentHeroDataMap; }


        bool isHeroFull(List<TemplateId>& heroList);
        // Ϊ����Ӣ��ͳһ��Ӿ��飬 ��������
        void refinePresentHeros(List<Guid>& heroLevelUpList, UInt32 incExp);
        // Ϊһ��Ӣ����Ӿ��飬��������
        ErrorId refineHero(const Guid& heroId, UInt32& upLevelCount, UInt32 incExp);
        // ����Ӣ�۵ȼ��ӿ�
        ErrorId refineHero(const Guid& heroId, UInt32 levelCount);
        // ����Ӣ�۽ӿ�
        ErrorId advancedHero(const Guid& heroId, const List<Guid>& heroList, const List<Guid>& itemList);
        // ����Ӣ�۽ӿ�
        ErrorId awakenHero(const Guid& heroId, const List<Guid>& heroList, const List<Guid>& itemList);
        // �����ӿ�
        ErrorId smeltHero(const Guid& heroId, UInt32& gold, UInt32& levelExp, UInt32& soul);
        // �����ӿ�
        ErrorId rebornHero(const Guid& heroId, UInt32& gold, UInt32& levelExp,
            List<Guid>& heroList, List<Guid>& itemList);

        ErrorId setHeroPresentPos(const Guid& heroId, UInt8 presentPos);
        ErrorId exchangeHeroPresentPos(const Guid& heroGuid1, const Guid& heroGuid2);
        ErrorId sellHero(List<Guid>& sellList, UInt32& getGold);

    public:
        // ����Ϊ������ͬ���ӿ�
        void increaseHeroLevel(const Guid& heroId, UInt16 offset);
        void reduceHeroLevel(const Guid& heroId, UInt16 offset);
        void increaseHeroLevelExp(const Guid& heroId, UInt32 offset);
        void reduceHeroLevelExp(const Guid& heroId, UInt32 offset);
        void increaseHeroAdvanced(const Guid& heroId, UInt32 offset);
        void reduceHeroAdvanced(const Guid& heroId, UInt32 offset);
        void increaseHeroConsumeGold(const Guid& heroId, UInt32 offset);
        void reduceHeroConsumeGold(const Guid& heroId, UInt32 offset);

    private:
        struct HeroRequireCondition
        {
            HeroRequireCondition() : mRequireHeroLevel(0), mRequireGold(0) {}

            UInt32 mRequireHeroLevel;
            List<std::pair<TemplateId, UInt32> > mRequireItems;
            List<std::pair<TemplateId, UInt32> > mRequireHeros;
            UInt32 mRequireGold;
        };

        void getConditionTemplate(const HeroData& heroData, HeroRequireCondition& condition);
        
    private:
        Player* mPlayer;
        PlayerHeroData* mPlayerHeroData;
        Map<Guid, HeroData*> mPlayerHeroDataMap;
        Map<UInt8, HeroData*> mPlayerPresentHeroDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__
