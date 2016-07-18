#ifndef __LYNX_COMMON_LIB_CHAT_DATA_H__
#define __LYNX_COMMON_LIB_CHAT_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct ChatData
	{
		ChatData() : mChannel(0), mSenderGuid(0), mReceiverGuid(0), mSenderName(""), 
            mReceiverName(""), mSenderVipLevel(0), mContents(""), mSenderIcon(""),
            mReceiverIcon(""), mSenderLevel(0), mSenderBattleValue(0) {}

        UInt8 mChannel;                     // 0 ���磬 1 ���ᣬ 2 ˽��
        Guid mSenderGuid;                   // �������GUID, �����ϵͳ��0
        Guid mReceiverGuid;                 // Ƶ��Ϊ0��ֵ�����壬 Ƶ��Ϊ1��ʾĿ�깫��GUID, Ƶ��Ϊ2��ʾĿ�����GUID
		NameString mSenderName;             // ���������ƣ��ͻ��˲���Ҫ��д
        IconString mSenderIcon;             // ������ͷ��
		NameString mReceiverName;           // ���������ƣ��ͻ�����Ҫ��д
        IconString mReceiverIcon;           // ������ͷ��
        UInt8 mSenderVipLevel;              // ������VIP, �ͻ��˲���Ҫ��д
        UInt32 mSenderLevel;                // �����ߵȼ�
        UInt32 mSenderBattleValue;          // ������ս��
		String mContents;                   // �������ݣ����80���ֽ�

		LYNX_S11N_11(ChatData, UInt8, mChannel, NameString, mSenderName, Guid, mSenderGuid, 
            Guid, mReceiverGuid, UInt8, mSenderVipLevel, NameString, mReceiverName, String, mContents,
            IconString, mSenderIcon, IconString, mReceiverIcon, UInt32, mSenderLevel, UInt32, mSenderBattleValue);
	};

#if 1
    typedef Map<UInt32, ChatData> ChatHistoryMap;

    struct WorldChatHistoryData
    {
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_1(WorldChatHistoryData, ChatHistoryMap, mChatHistoryMap);
    };

    struct PlayerChatHistoryData
    {
        PlayerChatHistoryData() : mLastReadWorldChatId(0), mLastReadGuildChatId(0), mLastReadPlayerChatId(0) {}

        UInt32 mLastReadWorldChatId;
        UInt32 mLastReadGuildChatId;
        UInt32 mLastReadPlayerChatId;
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_4(PlayerChatHistoryData, UInt32, mLastReadWorldChatId,
            UInt32, mLastReadGuildChatId, UInt32, mLastReadPlayerChatId,
            ChatHistoryMap, mChatHistoryMap);
    };

    struct GuildChatHistoryData
    {
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_1(GuildChatHistoryData, ChatHistoryMap, mChatHistoryMap);
    };

    typedef Map<Guid, PlayerChatHistoryData> PlayerChatHistoryDataMap;
    typedef Map<Guid, GuildChatHistoryData> GuildChatHistoryDataMap;

    struct ChatHistoryData
    {
        WorldChatHistoryData mWorldChatHistoryData;
        PlayerChatHistoryDataMap mPlayerChatHistoryDataMap;
        GuildChatHistoryDataMap mGuildChatHistoryDataMap;

        LYNX_S11N_3(ChatHistoryData, WorldChatHistoryData, mWorldChatHistoryData,
            PlayerChatHistoryDataMap, mPlayerChatHistoryDataMap,
            GuildChatHistoryDataMap, mGuildChatHistoryDataMap);

    };
#endif
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CHAT_DATA_H__


