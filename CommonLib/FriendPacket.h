#ifndef __LYNX_COMMON_LIB_FRIEND_PACKET_H__
#define __LYNX_COMMON_LIB_FRIEND_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_FRIENDBLACKLIST_REQ					= sFriendBlackMsgBase + 1,   //�ͻ����������������Ѻͺ������б�
		BOC_FRIENDBLACKLIST_RESP				= sFriendBlackMsgBase + 2,  //�������ظ��ͻ��˺��Ѻͺ������б�
		BOC_FRIENDADD_REQ                           = sFriendBlackMsgBase + 3,  //�ͻ�������������Ӻ�������
		BOC_FRIENDADD_NOTIFY                     = sFriendBlackMsgBase + 4,     //������֪ͨ�ͻ���������Ӻ���
		BOC_ADDAGRU_REQ								= sFriendBlackMsgBase + 5,		//�ͻ���ͬ����߾ܾ���Ӻ��ѣ����͸�������
		BOC_ADDAGRU_RESP								= sFriendBlackMsgBase + 6,    //A����B��Bͬ�⣬���ʱ����������A�����Ƿ����ˣ����ͻ���B�ذ�
		//BOC_FRIENDADD_RESP							= sFriendBlackMsgBase + 7,//������֪ͨ�ͻ�����Ӻ��ѽ����ͬ��

		BOC_FRIENDDEL_REQ								= sFriendBlackMsgBase + 8, //�ͻ�������ɾ������
		//BOC_FRIENDDEL_NOTIFY						= sFriendBlackMsgBase + 9 , //Aɾ��B���ѣ����B���ߣ�������֪ͨ�ͻ���Bɾ������
		BOC_FRIENDDEL_RESP							= sFriendBlackMsgBase + 10 ,// Aɾ��B���ѣ��������ذ���A

		BOC_ADDBLACK_REQ                            = sFriendBlackMsgBase + 13,  //�ͻ�����Ӻ���������
		BOC_ADDBLACK_RESP							= sFriendBlackMsgBase + 14,  //��������Ӻ������ظ�
	
		BOC_DELBLACK_REQ								= sFriendBlackMsgBase + 15,  //�ͻ���ɾ������������
		BOC_DELBALCK_RESP								= sFriendBlackMsgBase + 16,  //������ɾ���������ظ�

		BOC_RECOMMENDLIST_REQ					= sFriendBlackMsgBase + 17, //�ͻ�����������б�
		BOC_RECOMMENDLIST_RESP				= sFriendBlackMsgBase + 18, //������ͬ�������б�ظ�

		BOC_FINDPLAYER_REQ                          = sFriendBlackMsgBase + 19, //����ָ�����
		BOC_FINDPLAYER_RESP                         = sFriendBlackMsgBase + 20

	};

	//-----------------CG-------------------
	struct CGFriendBlackListReq
	{
		CGFriendBlackListReq(): mPacketID(BOC_FRIENDBLACKLIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_FRIENDBLACKLIST_REQ, CGFriendBlackListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGFriendAddReq
	{
		CGFriendAddReq():mPacketID(BOC_FRIENDADD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDADD_REQ, CGFriendAddReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	//A��B��B���������ذ���ͬ�⻹�Ǿܾ�
	struct CGFriendAddBackReq
	{
		CGFriendAddBackReq():mPacketID(BOC_ADDAGRU_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ADDAGRU_REQ, CGFriendAddBackReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	
	//A����ɾ��B���ͻ����������������
	struct CGFriendDelReq
	{
		CGFriendDelReq():mPacketID(BOC_FRIENDDEL_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDDEL_REQ, CGFriendDelReq, UInt16, mPacketID, std::string, mReqJsonStr);
		
	};

	//A����ɾ��������
	struct CGBlackDelReq 
	{
		CGBlackDelReq():mPacketID(BOC_DELBLACK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_DELBLACK_REQ, CGBlackDelReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	//A������Ӻ�����
	struct CGBlackAddReq
	{
		CGBlackAddReq():mPacketID(BOC_ADDBLACK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ADDBLACK_REQ, CGBlackAddReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGRecommendListReq
	{
		CGRecommendListReq():mPacketID(BOC_RECOMMENDLIST_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_RECOMMENDLIST_REQ, CGRecommendListReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct CGFindPlayerReq
	{
		CGFindPlayerReq():mPacketID(BOC_FINDPLAYER_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_FINDPLAYER_REQ, CGFindPlayerReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	

	//-----------------GC-------------------
	struct GCFriendBlackListResp
	{
		GCFriendBlackListResp():mPacketID(BOC_FRIENDBLACKLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_FRIENDBLACKLIST_RESP,GCFriendBlackListResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCFriendAddNotify
	{
		GCFriendAddNotify():mPacketID(BOC_FRIENDADD_NOTIFY),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDADD_NOTIFY,GCFriendAddNotify,UInt16, mPacketID,std::string, mRespJsonStr);
	};
	
	//A��B���ѣ�Bͬ��󣬷��������AĿǰ�����Ƿ����ˣ�������ظ���B
	struct GCFriendAddBackResp
	{
		GCFriendAddBackResp():mPacketID(BOC_ADDAGRU_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ADDAGRU_RESP,GCFriendAddBackResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	//struct GCFriendAddResp
	//{
	//	GCFriendAddResp():mPacketID(BOC_FRIENDADD_RESP),mRespJsonStr(""){}
	//	UInt16 mPacketID;
	//	std::string mRespJsonStr;

	//	LYNX_MESSAGE_2(BOC_FRIENDADD_RESP,GCFriendAddResp,UInt16, mPacketID,std::string, mRespJsonStr);
	//};
	
	////Aɾ��B���ѣ����B���ߣ�������֪ͨ�ͻ���Bɾ������
	//struct GCFriendDelNotify
	//{
	//	GCFriendDelNotify():mPacketID(BOC_FRIENDDEL_NOTIFY),mRespJsonStr(""){}
	//	UInt16 mPacketID;
	//	std::string mRespJsonStr;

	//	LYNX_MESSAGE_2(BOC_FRIENDDEL_NOTIFY,GCFriendDelNotify,UInt16, mPacketID,std::string, mRespJsonStr);
	//};
			
	//Aɾ��B���ѣ��������ذ���A
	struct GCFriendDelResp
	{
		GCFriendDelResp():mPacketID(BOC_FRIENDDEL_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FRIENDDEL_RESP,GCFriendDelResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	
	struct GCBlackAddResp
	{
		GCBlackAddResp():mPacketID(BOC_ADDBLACK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ADDBLACK_RESP,GCBlackAddResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCBlackDelResp
	{
		GCBlackDelResp():mPacketID(BOC_DELBALCK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_DELBALCK_RESP,GCBlackDelResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCRecommendListResp
	{
		GCRecommendListResp():mPacketID(BOC_RECOMMENDLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_RECOMMENDLIST_RESP, GCRecommendListResp, UInt16, mPacketID, std::string, mRespJsonStr);

	};
	
	struct GCFindPlayerResp
	{
		GCFindPlayerResp():mPacketID(BOC_FINDPLAYER_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_FINDPLAYER_RESP, GCFindPlayerResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_FRIEND_PACKET_H__

