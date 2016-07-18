#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class SystemAwardManager : public MainThread
    {
    public:
        SystemAwardManager();
        virtual ~SystemAwardManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        // ��ȡ������½����
        ErrorId getContinousLoginAward(UInt32 loginDay);
        // ��ȡ�ۻ���½����
        ErrorId getAccumulateLoginAward(UInt32 loginDay);
        // ��ȡ�ȼ�����
        ErrorId getLevelAward(UInt32 level);

    private:
        Player* mPlayer;
        UInt32 mCurTime;
        UInt32 mOnlineDateFlag;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__
