#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class RuneManager : public MainThread
    {
    public: 
        RuneManager();
        virtual ~RuneManager();

        bool initial(Player* player);
        void release();
        void calculateRuneData(UInt8 runePresentPos);

    public:
        // ���������Ƿ�����
        bool isRuneFull();
        bool isRuneFull(List<TemplateId>& runeList);
        bool isRuneFull(List<AwardLine>& awardList);
        // ��÷������ո�����
        UInt8 getRuneFieldSpaceSlot();
        // ��������
        void lockRune(const Guid& runeId);
        // ��������
        void unLockRune(const Guid& runeId);

        // �ݻٷ���
        void destroyRune(Guid runeId);
        // ��շ���
        void destroyAllRune();

        // ʹ�÷���
        ErrorId useRune(const Guid& runeId, UInt8 targetPos);
        // ��������
        const RuneData* createRune(TemplateId runeTemplateId, UInt8 source, bool promp = true);

        void setRuneRefineLevel(const Guid& runeId, UInt32 refineLevel);

        // ��ӷ���ǿ������, ��������
        ErrorId addRuneRefineExp(const Guid& runeId, UInt32 incExp, UInt32& refineLevel);

        // ǿ������
        ErrorId refineRune(const Guid& runeId, const List<Guid>& itemList, UInt32& refineLevel);

        const RuneData* getRuneDataByGuid(const Guid& runeId);

        void setRunePresentPos(const Guid& runeId, UInt8 pos);
        ErrorId sellRune(List<Guid>& runeList, UInt32& getGold);
    private:
        Player* mPlayer;
        List<RuneData>* mRuneList;
        Map<Guid, RuneData*> mRuneMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__


