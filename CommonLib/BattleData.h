#ifndef __LYNX_COMMON_LIB_BATTLE_DATA_H__
#define __LYNX_COMMON_LIB_BATTLE_DATA_H__

#include "Definition.h"
#include "BufferData.h"
#include "SkillData.h"

namespace Lynx
{
#if 1
    struct BattleBaseData
    {
        BattleBaseData() : mGuidId(0), mTemplateId(0), mTag(""), 
            mCharacterType(0), mRelationship(0), mHp(0), mAnger(0), 
            mParalysisId(0), mBlindId(0), mConvergedId(0), mSilentId(0),
            mSealId(0), mPhyInvincibleId(0), mMagInvincibleId(0) {}

        // ��ɫGUID
        Guid mGuidId;
        // ģ��ID
        TemplateId mTemplateId;
        // ��ɫ��ǩ
        String mTag;
        // ��ɫ����
        UInt8 mCharacterType;
        // ��������
        UInt8 mRelationship;
        // HP
        UInt32 mHp;
        // ŭ��
        UInt32 mAnger;

        // ���
        UInt32 mParalysisId;
        // ��ä
        UInt32 mBlindId;
        // ����
        UInt32 mConvergedId;
        // ��Ĭ
        UInt32 mSilentId;
        // ��ӡ
        UInt32 mSealId;
        // �����޵�
        UInt32 mPhyInvincibleId;
        // ħ���޵�
        UInt32 mMagInvincibleId;
    };

    struct BattleExtendData
    {
        BattleExtendData() : mHpUpper(0), mAngerUpper(0),
            mAttack(0), mPhysicsDefense(0), mMagicDefense(0), 
            mCritical(0), mRescrit(0), mHit(0), mDodge(0),
            mSuck(0), mAvoidDamage(0), mCure(0), mCriticalDamage(0) {}

        // Ѫ������
        UInt32 mHpUpper;
        // ŭ������
        UInt32 mAngerUpper;
        // ������
        UInt32 mAttack;
        // �������
        UInt32 mPhysicsDefense;
        // ħ������
        UInt32 mMagicDefense;
        // ����
        UInt32 mCritical;
        // ����
        UInt32 mRescrit;
        // ����
        UInt32 mHit;
        // ����
        UInt32 mDodge;
        // ��Ѫ
        UInt32 mSuck;
        // ����
        UInt32 mAvoidDamage;
        // ����
        UInt32 mCure;
        // �����˺�
        UInt32 mCriticalDamage;

    };

    struct BattleBufferData
    {
        BattleBufferData() : mHpUpper(0), mAttack(0), mPhysicsDefense(0), mMagicDefense(0), 
            mCritical(0), mRescrit(0), mHit(0), mDodge(0),
            mSuck(0), mAvoidDamage(0), mCure(0), mCriticalDamage(0) {}

        // Ѫ������
        UInt32 mHpUpper;
        // ������
        UInt32 mAttack;
        // �������
        UInt32 mPhysicsDefense;
        // ħ������
        UInt32 mMagicDefense;
        // ����
        UInt32 mCritical;
        // ����
        UInt32 mRescrit;
        // ����
        UInt32 mHit;
        // ����
        UInt32 mDodge;
        // ��Ѫ
        UInt32 mSuck;
        // ����
        UInt32 mAvoidDamage;
        // ����
        UInt32 mCure;
        // �����˺�
        UInt32 mCriticalDamage;

        Map<UInt32, BufferData> mBufferMap;
        UInt32 mPassivityId[3];
    };

    struct BattleSkillItem
    {
        BattleSkillItem() : mTemplateId(0), mSkillAttackType(0),
            mCombatType(0), mSkillRelationship(0), mTriggerAnger(0),
            mAngerInc(0), mCoolDownRound(0), mMultiAttack(0), 
            mMultiNum(0), mHitPointCount(0) {}

        // ����ģ��ID
        TemplateId mTemplateId;
        // �������ͣ���λ��������壬���2����ȫ�壬ָ����
        UInt8 mSkillAttackType;
        // ���ܽ���Զ��
        UInt8 mCombatType;
        // ���ܵ���
        UInt8 mSkillRelationship;
        // ����ŭ��
        UInt32 mTriggerAnger;
        // ����ŭ��
        UInt32 mAngerInc;
        // ��ȴ�غ���
        UInt32 mCoolDownRound;
        // ��ι���
        UInt32 mMultiAttack;
        // ��ι���
        UInt32 mMultiNum;
        // ���������
        UInt32 mHitPointCount;
    };

    struct BattleSkillData
    {
        BattleSkillData(): mPassiveSkill1(0), mPassiveSkill2(0), mPassiveSkill3(0) {}

        UInt32 mPassiveSkill1;
        UInt32 mPassiveSkill2;
        UInt32 mPassiveSkill3;

        BattleSkillItem mCommonSkill;
        BattleSkillItem mAutoSkill;
        BattleSkillItem mSuperSkill;
        BattleSkillItem mEnterSkill;
		
		//sec ע�ͼ����ֶ�
        //ActionHitMap mActionHitMap;
    };

    struct BattleData 
    {
        BattleData() {}

        BattleBaseData mBaseData;
        BattleExtendData mExtendData;
        BattleBufferData mBufferData;
        BattleSkillData mSkillData;
    };
#endif

    struct BattleSimulationData
    {
        BattleSimulationData(): mStarCount(0), mRandomStartSeed(0) {}

        UInt32 mStarCount;
        UInt16 mRandomStartSeed;
        List<UInt32> mSuperSkillList;

        LYNX_S11N_3(BattleSimulationData, UInt32, mStarCount, UInt16, mRandomStartSeed,
            List<UInt32>, mSuperSkillList);
    };

    struct BattleVerifyData 
    {
        BattleVerifyData(): mIsVictory(false), mDeathCount(0), mRandomStartSeed(0) {}

        bool mIsVictory;
        UInt8 mDeathCount;
        UInt16 mRandomStartSeed;
        List<UInt32> mSuperSkillRecord;

        LYNX_S11N_4(BattleVerifyData, bool, mIsVictory, UInt8, mDeathCount, 
            UInt16, mRandomStartSeed, List<UInt32>, mSuperSkillRecord);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BATTLE_DATA_H__
