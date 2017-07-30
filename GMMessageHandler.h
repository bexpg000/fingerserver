#ifndef __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct GMMessageHandler
	{
		static void onMsgGMReq(const ConnId&, CGGMReq&);
		//��ֹ�����˵�½
		static void onMsgGMForbidLoginReq(const ConnId& connId, CGGMForbidLoginReq& msg);
		//��ĳ��������
		static void onMsgGMKickPlayerReq(const ConnId& connId, CGGMKickPlayerReq& msg);
		//��ֹ����
		static void onMsgGMForbidChatReq(const ConnId & connId, CGGMForbidChatReq & msg);
		//�����ʼ�
		static void onMsgGMSendEmailReq(const ConnId & connId, CGGMSendEmailReq & msg);
		//�ߵ������ˣ��رշ�����
		static void onMsgGMKickAllReq(const ConnId & connId, CGGMKickAllReq & msg);
		//����ϵͳ֪ͨ
		static void onMsgGMNoticeReq(const ConnId & connId, CGGMNoticeReq & msg);
		//GM�������ÿ���淨��������ɾͣ�����ѵ����
		static void onMsgGMReset(const ConnId & connId, CGGMResetReq & msg);
		//GM�������id
		static void onMsgGMAwardBag(const ConnId & connId, CGGAwardBagReq & msg);

		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_GM_MSG_HANDLER_H__

