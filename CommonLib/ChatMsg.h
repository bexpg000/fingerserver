#ifndef __LYNX_COMMON_LIB_CHAT_MSG_H__
#define __LYNX_COMMON_LIB_CHAT_MSG_H__

#include "Data.h"

namespace Lynx
{
	enum
	{
        SYNC_CHAT_STATUS_NOTIFY                      = sChatMsgBase + 1,
        SEND_CHAT_MSG_NOTIFY                         = sChatMsgBase + 2,
        GET_CHAT_MSG_REQ                             = sChatMsgBase + 3,
        GET_CHAT_MSG_RESP                            = sChatMsgBase + 4,
        FORBIDDEN_CHAT_NOTIFY                        = sChatMsgBase + 5,
	};

    struct SyncChatStatusNotify
    {
        SyncChatStatusNotify() : mChannel(0), mNewMsgCount(0) {}

        UInt8 mChannel; // 0 ���磬 1���ᣬ 2���
        UInt8 mNewMsgCount;

        LYNX_MESSAGE_2(SYNC_CHAT_STATUS_NOTIFY, SyncChatStatusNotify, UInt8, mChannel,
            UInt8, mNewMsgCount);
    };

    struct SendChatMsgNotify
    {
        SendChatMsgNotify() {}

        ChatData mChatData;

        LYNX_MESSAGE_1(SEND_CHAT_MSG_NOTIFY, SendChatMsgNotify, ChatData, mChatData);
    };

    struct GetChatMsgReq
    {
        GetChatMsgReq() : mChannel(0) {}

        UInt8 mChannel;// 0 ���磬 1���ᣬ 2���

        LYNX_MESSAGE_1(GET_CHAT_MSG_REQ, GetChatMsgReq, UInt8, mChannel);
    };

    struct GetChatMsgResp
    {
        GetChatMsgResp() : mChannel(0) {}

        UInt8 mChannel;// 0 ���磬 1���ᣬ 2���
        List<ChatData> mChatDataList;

        LYNX_MESSAGE_2(GET_CHAT_MSG_RESP, GetChatMsgResp, UInt8, mChannel, 
            List<ChatData>, mChatDataList);
    };

    struct ForbiddenChatNotify
    {
        ForbiddenChatNotify() : mWhy(0) {}

        UInt8 mWhy; // 1 ��ʾ�����������Ƶ�� 2���ܸ��Լ�����

        LYNX_MESSAGE_1(FORBIDDEN_CHAT_NOTIFY, ForbiddenChatNotify,
            UInt8, mWhy);
    };
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CHAT_MSG_H__