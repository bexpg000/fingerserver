#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__

#include "MainThread.h"

/* ��Ӣ�۰�˵��
   1 ���߳���� ֻ��һ���� ID 1
   2 ���שʯ�������5������ID�ֱ�Ϊ2��3��4��5��6, ǰ5�ΰ�˳��ʹ�ã� �����ο�ʼ�� ��Զʹ��6
   3 ��ʯ���, ��5������ID�ֱ�Ϊ 7��8��9��10��11��ǰ5�ΰ�˳��ʹ�ã� �����ο�ʼ�� ��Զʹ��11
   4 ʮ�����, ��������ID�ֱ�Ϊ 12�� 13��ÿ��ʮ���飬 ʹ��һ��5�ǰ��� ʹ��һ��13�� ���������ʹ��12��ȡ
   5 ���ǳ��, ��һ��, IDΪ 14
*/

#define ITEM_SUMMON_PKG 1

#define FREE_DIAMOND_SUMMON_PKG_0 2
#define FREE_DIAMOND_SUMMON_PKG_1 3
#define FREE_DIAMOND_SUMMON_PKG_2 4
#define FREE_DIAMOND_SUMMON_PKG_3 5
#define FREE_DIAMOND_SUMMON_PKG_4 6
#define FREE_DIAMOND_SUMMON_PKG   6

#define DIAMOND_SUMMON_PKG_0 7
#define DIAMOND_SUMMON_PKG_1 8
#define DIAMOND_SUMMON_PKG_2 9
#define DIAMOND_SUMMON_PKG_3 10
#define DIAMOND_SUMMON_PKG_4 11
#define DIAMOND_SUMMON_PKG   11

#define BATCH_SUMMON_PKG_0 12
#define BATCH_SUMMON_PKG_1 13

#define SUPPER_HERO_PKG    14

namespace Lynx
{
    class Player;
    class SummonManager : public MainThread
    {
    public:
        SummonManager();
        virtual ~SummonManager();

        bool initial(Player* player);
        void release();

        // �ٻ�Ӣ�۽ӿ�
        ErrorId summonHeroByItem(List<Guid>& heroGuidList);
        ErrorId summonHeroByDiamond(List<Guid>& heroGuidList);
        ErrorId summonHeroByFreeDiamond(List<Guid>& heroGuidList);
        ErrorId summonHeroByBatch(List<Guid>& heroGuidList);

    private:
        Guid randPkg(const TemplateId& pkgId);

    private:
        Player* mPlayer;
        PlayerSummonData* mPlayerSummonData;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__
