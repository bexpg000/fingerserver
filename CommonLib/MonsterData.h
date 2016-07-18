#ifndef __LYNX_COMMON_LIB_MONSTER_DATA_H__
#define __LYNX_COMMON_LIB_MONSTER_DATA_H__

#include "Definition.h"
#include "SkillData.h"

namespace Lynx
{
    struct MonsterBaseData
    {
        MonsterBaseData() : mMonsterId(0), mMonsterName(""), mTemplateId(0),
            mLevel(0), mHp(0), mStarLevel(0), mAnger(0), mPresentPos(0) {}

        // ��ɫGUID
		Guid mMonsterId;
        // ��ɫ����
        String mMonsterName;
        // ģ��ID
		TemplateId mTemplateId;
        // �ȼ�
        UInt32 mLevel;
        // HP
        UInt32 mHp;
        // �Ǽ�
        UInt32 mStarLevel;
        // ŭ��
        UInt32 mAnger;
        // ����״̬ 0������1��λ��1���󣩣�2��λ��2���У���3�� λ��3���ң���4���油1��5���油2
        UInt8 mPresentPos;

        LYNX_S11N_8(MonsterBaseData, Guid, mMonsterId, String, mMonsterName,
            TemplateId, mTemplateId, UInt32, mLevel, UInt32, mHp, UInt32, mStarLevel, 
            UInt32, mAnger, UInt8, mPresentPos);
    };

    struct MonsterExtendData
    {
        MonsterExtendData() : mAttack(0), mPhysicsDefense(0), mMagicDefense(0), mCritical(0), 
            mHit(0), mDodge(0), mHpUpper(0), mAngerUpper(0), mRescrit(0), mSuck(0), mCure(0),
            mAvoidDamage(0), mCriticalDamage(0) {}

        // ����
        UInt32 mAttack;
        // �������
        UInt32 mPhysicsDefense;
        // ħ������
        UInt32 mMagicDefense;
        // ����
        UInt32 mCritical;
        // ����
        UInt32 mHit;
        // ����
        UInt32 mDodge;
        // Ѫ������
        UInt32 mHpUpper;
        // ŭ������
        UInt32 mAngerUpper;
        // ����
        UInt32 mRescrit;
        // ��Ѫ
        UInt32 mSuck;
        // ����
        UInt32 mAvoidDamage;
        // ����
        UInt32 mCure;
        // �����˺�
        UInt32 mCriticalDamage;

        LYNX_S11N_13(MonsterExtendData, UInt32, mAttack, UInt32, mPhysicsDefense, 
            UInt32, mMagicDefense, UInt32, mCritical,
            UInt32, mHit, UInt32, mDodge, UInt32, mHpUpper, UInt32, mAngerUpper,
            UInt32, mRescrit, UInt32, mSuck, UInt32, mAvoidDamage, UInt32, mCure,
            UInt32, mCriticalDamage);
    };

    struct MonsterSkillData : public SkillData
    {
        MonsterSkillData() {}

        LYNX_S11N_4(MonsterSkillData, SkillItem, mCommonSkill, SkillItem, mAutoSkill,
            SkillItem, mSuperSkill, SkillItem, mEnterSkill);
    };

    struct MonsterData 
    {
        MonsterData() {}

        MonsterBaseData mBaseData;
        MonsterExtendData mExtendData;
        MonsterSkillData mSkillData;

        LYNX_S11N_3(MonsterData, MonsterBaseData, mBaseData, MonsterExtendData, mExtendData,
            MonsterSkillData, mSkillData);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_MONSTER_DATA_H__