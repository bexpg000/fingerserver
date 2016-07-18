#ifndef __LYNX_COMMON_LIB_ITEM_TABLE_H__
#define __LYNX_COMMON_LIB_ITEM_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ItemTemplate 
	{
		// ģ��id
		TemplateId mTemplateId;

		// ������ǩtag
		UInt32	   mBagTab;

		//����id
		UInt32	   mTypeId;
		//С����
		UInt32	   mSubTypeId;
		//����
		String	   mName;
		//����
		String	   mDes;
		//ͼ��
		String	   mIcon;
		//Ʒ��
		UInt32	   mQuality;

		//��ʼʱ��
		UInt64	   mBeginTime;

		//ʧЧʱ��
		UInt64	   mInvalid;
		//�ɷ�ʹ��
		UInt32     mUseful;
		//ʹ������
		String		mUseLimit1;
		//ʹ������
		String		mUseLimit2;
		//ʹ������
		String		mUseLimit3;
		//ʹ������
		String		mUseLimit4;
		//ʹ������
		String		mUseLimit5;


		//���ʹ������
		UInt32		mAmountLimit;
		//ʹ�ô���
		UInt32		mUseTime;
		//Ч��ָ��
		UInt32		mPower;
		//CD
		UInt32		mCD;
		//�ѵ�����
		UInt32		mStackAmount;
		//����
		UInt32		mOrder;
		//�ɷ����
		UInt32		mSale;
		//�����۸�
		UInt32		mPrice;
		//��������
		String		mContent;

		LYNX_S11N_25(ItemTemplate, TemplateId, mTemplateId, UInt32, mBagTab,UInt32, mTypeId, 
									UInt32, mSubTypeId,String, mName, String, mDes, 
									String , mIcon, UInt32, mQuality, UInt64,  mBeginTime,
									UInt64	,mInvalid, UInt32 ,mUseful, String, mUseLimit1,
									String, mUseLimit2, String, mUseLimit3,String, mUseLimit4,
									String, mUseLimit5,UInt32 ,mAmountLimit, UInt32 ,mUseTime,
									 UInt32 ,mPower, UInt32 ,mCD, UInt32 ,	mStackAmount, 
									 UInt32, mOrder, UInt32 ,mSale, UInt32	,mPrice,
									 String, mContent
			         );

	};

	typedef Map<UInt32, ItemTemplate> ItemTemplateMap;


	struct ItemTable
	{
		ItemTemplateMap mMap;


		LYNX_S11N_1(ItemTable, ItemTemplateMap, mMap);

		ItemTemplate* get(UInt32 id)
		{
			ItemTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_ITEM_TABLE_H__


