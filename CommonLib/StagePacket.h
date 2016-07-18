#ifndef __LYNX_COMMON_LIB_STAGE_PACKET_H__
#define __LYNX_COMMON_LIB_STAGE_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CHAPTER_LIST_REQ					= sStageMsgBase + 1,
		BOC_CHAPTER_LIST_RESP					= sStageMsgBase + 2,
		BOC_CHAPTER_LIST_CHANG_REQ				= sStageMsgBase + 3,
		BOC_CHAPTER_LIST_CHANG_RESP				= sStageMsgBase + 4,

		TWELVE_PALACE_LIST_REQ					= sStageMsgBase + 5,//ʮ���ڹ��б�
		TWELVE_PALACE_LIST_RESP					= sStageMsgBase + 6,//ʮ���ڹ��б�
		TWELVE_PALACE_LIST_CHANG_REQ			= sStageMsgBase + 7,//ʮ���ڹ��б�
		TWELVE_PALACE_LIST_CHANG_RESP			= sStageMsgBase + 8,//ʮ���ڹ��б�


		CHAPTER_AWARD_RESP			=   sStageMsgBase+9,	//���ظ������㽱��
		CHAPTER_START_REQ			=   sStageMsgBase+10,	// ��ս�ؿ�
		CHAPTER_START_RESP			=   sStageMsgBase+11,	//��ս�ؿ��ظ�
		CHAPTER_END_REQ				=   sStageMsgBase+12,	//�ؿ�����
		CHAPTER_END_RESP			=   sStageMsgBase+13,	//�ؿ������ظ�
		CHAPTER_LEAVE_REQ			=   sStageMsgBase+14,	//�뿪�ؿ�
		CHAPTER_LEAVE_RESP			=   sStageMsgBase+15,	//�뿪�ؿ��ظ�

		CHAPTER_COUNTER_REQ			=   sStageMsgBase+16,	//�ؿ�������
		CHAPTER_COUNTER_RESP		=   sStageMsgBase+17,	//�ؿ��������ظ�
		CHAPTER_AWARD_CARD_REQ		=   sStageMsgBase+18,	//����
		CHAPTER_AWARD_CARD_RESP		=   sStageMsgBase+19,	//���ƻظ�
		CHAPTER_STAR_CHECK_REQ		=   sStageMsgBase+20,	//�ؿ�����
		CHAPTER_STAR_CHECK_RESP		=   sStageMsgBase+21,	//�ؿ�����
		CHAPTER_FOOD_REQ			=   sStageMsgBase+22,	//�ؿ���ʳ��
		CHAPTER_FOOD_RESP			=   sStageMsgBase+23,	//�ؿ���ʳ�ݷ���

		RECIEVED_PACKET_REQ			=   sStageMsgBase+24,	//�ͻ��˽��յ�Э��ظ�

		CHAPTER_RELIVE_REQ			=   sStageMsgBase+26,	//����
		CHAPTER_RELIVE_RESP			=   sStageMsgBase+27,	//�����

		CHAPTER_MOP_UP_REQ			=   sStageMsgBase+40,	//�ؿ�ɨ��
		CHAPTER_MOP_UP_RESP			=   sStageMsgBase+41,	//�ؿ�ɨ������


		CHAPTER_RECORD_REQ			=   sStageMsgBase+50,	//�ƹؼ�¼
		CHAPTER_RECORD_RESP			=   sStageMsgBase+51,	//�ƹؼ�¼����

		CHAPTER_TWELVE_PALACE_RECORD_REQ			=   sStageMsgBase+52,	//�ƹؼ�¼ʮ���ڹ�
		CHAPTER_TWELVE_PALACE_RECORD_RESP			=   sStageMsgBase+53,	//�ƹؼ�¼����ʮ���ڹ�

		CHAPTER_BOX_OPEN_REQ			=   sStageMsgBase+60,	//���½ڱ���
		CHAPTER_BOX_OPEN_RESP			=   sStageMsgBase+61,	//���½ڱ��䷵��

		CHAPTER_STRENGTH_REQ			=   sStageMsgBase+70,	//��������
		CHAPTER_STRENGTH_RESP			=   sStageMsgBase+71,	//��������

		COURAGE_CHALLENGE_REQ			=   sStageMsgBase+80,	//������ս����
		COURAGE_CHALLENG_RESP			=   sStageMsgBase+81,	//������ս����
		CAT_COIN_REQ					=   sStageMsgBase+82,	//Ӳ�Һ�����
		CAT_COIN_RESP					=   sStageMsgBase+83,	//Ӳ�Һз���
		COURAGE_CHALLENG_BEGIN_REQ			=   sStageMsgBase+84,	//������ս��ʼ
		COURAGE_CHALLENG_BEGIN_RESP			=   sStageMsgBase+85,	//������ս��ʼ����
		COURAGE_CHALLENG_END_REQ			=   sStageMsgBase+86,	//������ս����
		COURAGE_CHALLENG_END_RESP			=   sStageMsgBase+87,	//������ս����

		BATTLE_START_REQ			=   sStageMsgBase+100,	//ս����ʼ
		BATTLE_START_RESP			=   sStageMsgBase+101,	//ս����ʼ�ظ�

		CHAPTER_END_REQ1				=   sStageMsgBase+122,	//�ؿ�����
	};

	//-----------------CG-------------------
	struct CGStageListReq
	{
		CGStageListReq(): mPacketID(BOC_CHAPTER_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_CHAPTER_LIST_REQ, CGStageListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	//-----------------GC-------------------
	struct GCStageListResp
	{
		GCStageListResp(): mPacketID(BOC_CHAPTER_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CHAPTER_LIST_RESP, GCStageListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_STAGE_PACKET_H__