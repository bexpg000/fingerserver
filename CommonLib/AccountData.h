#ifndef __LYNX_COMMON_LIB_ACCOUNT_DATA_H__
#define __LYNX_COMMON_LIB_ACCOUNT_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct AccountData
	{
		AccountData() : mAccountId(0), mAccountName(""), mLoginClientIp(""), mIdCard(""),
            mAdult(false), mApplingAdult(false), mLoginTime(0), mRmb(0),
            mGMLevel(0), mIsActived(false), mLogoutTime(0) {}

        // �˻�ID
		Guid mAccountId;
        // �˻���
		NameString mAccountName;
        // �ͻ��˵�½IP
        CharArray<16> mLoginClientIp;
        // ���֤
        CharArray<20> mIdCard;
        // ��ͨ����������֤
        bool mAdult;
        // �������������֤
        bool mApplingAdult;
        // ��¼��ʱ���
        UInt32 mLoginTime;
        // �ϴεǳ�ʱ��
        UInt32 mLogoutTime;
        // �ʻ����
        UInt32 mRmb;
        // GM�ȼ�(0Ϊ��ͨ�˻���>=1 ΪGM�ʻ�)
        UInt8 mGMLevel;
        // �Ƿ񶳽ᣨ�Ѿ�����true  δ����false��
        bool mIsActived;
        //// vip�ȼ�����
        //UInt32 mVipExp;

		LYNX_S11N_11(AccountData, Guid, mAccountId, NameString, mAccountName, CharArray<16>, mLoginClientIp,
            CharArray<20>, mIdCard, bool, mAdult, bool, mApplingAdult, UInt32, mLoginTime, UInt32, mRmb,
            UInt8, mGMLevel, bool, mIsActived, UInt32, mLogoutTime);

#if __CSharp__
        void testFunc()
        {
            
        }
#endif
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ACCOUNT_DATA_H__

