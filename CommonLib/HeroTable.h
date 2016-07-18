#ifndef __LYNX_COMMON_LIB_HERO_TABLE_H__
#define __LYNX_COMMON_LIB_HERO_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HeroTemplate_EX
	{
		float mVertigoDef;//���η����� todoadd

		float mVertigoTimeDef;//����ʱ��������

		float mBaseToughness;//����

		float mFallDef;//��������

		String mDefenceAreaStand;//վ��ʱ������Χ

		String mDefenceAreaLie;//����ʱ������Χ

		LYNX_S11N_6(HeroTemplate_EX,float, mVertigoDef,float, mVertigoTimeDef,float, mBaseToughness, float, mFallDef,
			String, mDefenceAreaStand,String, mDefenceAreaLie)
	};

	struct HeroTemplate 
	{
		UInt32 mId;//��ɫģ��id
		String mName;//��ɫ����
		String mDesc;//����
		String mRes;//��Դ
		String mStrSkillList;//�����б�
		String mStrSkillListOnCreate;//

		List<UInt64> m_listOriginSkills;//��ʼ����

		List<UInt64> m_listLearnSkills;//��ѧϰ����

		UInt64 mNeedLv;
		UInt64 mNeedVipLv;
		UInt64 mCost;

		float mPA;//��������
		float mPF;//���������

		float mMA;//ħ��������
		float mMF;//ħ��������

		float mMaxHP;//���Ѫ��
		float mMaxMP;//�������
		float mMaxSP;//������ֵ

		float mMPRecover;//ħ���ָ���
		float mSPRecover;//����ָ���
		float mAttackSpeed;//�����ٶ�

		float mCombinationAtt;//���м��
		float mMoveSpeed;//�ƶ��ٶ�

		float mJumpHeight;//����߶�
		float mFlyTimes;//���������մ���
		float mSPDamage;//sp����

		float mAttPush;//����
		float mAttPushDef;//���˷���

		float mAttRaise;//����
		float mAttRaiseDef;//�������
		
		float mStiff;//������ֱ
		float mStiffDef;//��ֱ����

		float mBaseCrit;//����������
		float mCritRate;//��������

		float mCritDef;//����������
		float mStunDef;//���η�����

		float mStunTimeDef;//����ʱ������

		float mSlowDef;//���ٷ�����

		float mSlowTimeDef;//����ʱ�������

		float mPoisonDef;//�ж�������

		float mBloodDef;//���˷���
		
		float mKODef;//��ɱ����

		float mFloatVal;//����

		float mFloatDef;//���ɿ���

		float mFall;//����

	

	

		HeroTemplate_EX heroEx;//������̫���˻ᱨ����ӵ�����HeroTemplate_EX

		LYNX_S11N_46(HeroTemplate, UInt32, mId,String, mName,String, mDesc,		
			String, mRes,String, mStrSkillList,String ,mStrSkillListOnCreate,
				float, mPA,float, mMF,float, mMaxHP,
				float, mMaxMP,float, mMaxSP,float, mMPRecover,
				float, mSPRecover,float, mAttackSpeed,float, mCombinationAtt,
				float, mMoveSpeed, float,mJumpHeight,float ,mFlyTimes,
				float, mSPDamage,float ,mAttPush ,float, mAttPushDef,
				float, mAttRaise,float, mAttRaiseDef,float, mStiff,
				float, mStiffDef,float, mBaseCrit,float, mCritRate,
				float, mCritDef,float, mStunDef,float, mStunTimeDef,
				float, mSlowDef,float, mSlowTimeDef,float, mPoisonDef,
				float, mBloodDef,float, mKODef,float, mFloatVal,
				float, mFloatDef,float, mFall,  float, mPF,
				float, mMA, List<UInt64>, m_listOriginSkills,List<UInt64>,m_listLearnSkills,
				HeroTemplate_EX, heroEx, UInt64, mNeedLv,
		        UInt64, mNeedVipLv, UInt64, mCost	);
	};

	typedef Map<UInt32, HeroTemplate> HeroTemplateMap;


	struct HeroTable
	{
		HeroTemplateMap mMap;


		LYNX_S11N_1(HeroTable, HeroTemplateMap, mMap);

		HeroTemplate* get(UInt32 id)
		{
			HeroTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_HERO_TABLE_H__