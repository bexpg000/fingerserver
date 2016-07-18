#ifndef __LYNX_COMMON_LIB_SKILL_TABLE_H__
#define __LYNX_COMMON_LIB_SKILL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct SkillTemplate 
	{
        // ����ģ��ID
		TemplateId mTemplateId;
        // ��������
        String mSkillName;
        // ��������ID
        DescripeId mDescripeId;
        // ��������
        String mDescription;
        // ���ܵȼ�
        UInt8 mLevel;
        // ����ս������
        //UInt8 mCombatType;
        // ���ܹ�������
        //UInt8 mAttackType;
        // ���ܵ��ҹ�ϵ
        //UInt8 mRelationship;
        // �����ͷ�����ŭ��
        UInt32 mTriggerAnger;
        // �����ͷ�����ŭ��
        UInt32 mIncAnger;
        // ������ȴ�غ���
        UInt8 mCoolDownRound;
        // ������
        UInt32 mPhysicsAttack;
        // ħ������
        UInt32 mMagicAttack;
        // ���ưٷֱ�
        float mCurePercent;
        // ��Ѫ�ٷֱ�
        float mSuckPercent;
        // ��ι�������
        //UInt8 mMultiAttack;
        // ������Ч����
        //UInt8 mAttackEffectType;
        //// ���ܴ����
        //UInt8 mAttackPoint;
        //// ���ܰ󶨵�
        //UInt8 mBindPoint;
        // ���λ������
        UInt8 mCameraPos;
        // ��ι���������
        //UInt8 mMultiNum;
        //// ��������Ч
        //String mFlyerEffect;
        //// ������Ч
        //String mBeHitEffect;
        // ������Ч
        String mDeadEffect;
        // ����ͼ��
        String mIcon;

        TemplateId mSkillBuffer1;
        TemplateId mSkillBuffer2;
        TemplateId mSkillBuffer3;

		SkillTemplate(): mTemplateId(0), mSkillName(""), mDescripeId(0), mDescription(""), mLevel(1),
            mTriggerAnger(0), mIncAnger(0), mCoolDownRound(0), mPhysicsAttack(0), mMagicAttack(0), 
            mCurePercent(0.0f), mSuckPercent(0.0f), mCameraPos(0), mDeadEffect(""), mIcon(""),
            mSkillBuffer1(0), mSkillBuffer2(0), mSkillBuffer3(0) {}

		LYNX_S11N_18(SkillTemplate, TemplateId, mTemplateId, String, mSkillName, 
            DescripeId, mDescripeId, String, mDescription,
            UInt8, mLevel, UInt32, mTriggerAnger, UInt32, mIncAnger, 
            UInt8, mCoolDownRound, UInt32, mPhysicsAttack, UInt32, mMagicAttack, 
            float, mSuckPercent, float, mCurePercent, UInt8, mCameraPos, 
            String, mDeadEffect, String, mIcon, TemplateId, mSkillBuffer1, 
            TemplateId, mSkillBuffer2, TemplateId, mSkillBuffer3);
	};

	typedef Map<UInt32, SkillTemplate> SkillTemplateMap;


	struct SkillTable
	{
		SkillTemplateMap mMap;

		LYNX_S11N_1(SkillTable, SkillTemplateMap, mMap);

		SkillTemplate* get(UInt32 id)
		{
			SkillTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
        bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SKILL_TABLE_H__


