#ifndef __LYNX_COMMON_LIB_BOX_PACKET_H__
#define __LYNX_COMMON_LIB_BOX_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		OPEN_GIFT_REQ                          = sItemMsgBase + 200,//�����
		OPEN_GIFT_RESP                         = sItemMsgBase + 201,//���ؿ�������
		OPEN_BOX_REQ                           = sItemMsgBase + 202,//������
		OPEN_BOX_RESP                          = sItemMsgBase + 203,//���ؿ����ӽ��


		OPEN_LOTTERY_DRAW_REQ                  =	sItemMsgBase + 206,//�齱
		OPEN_LOTTERY_DRAW_RESP                 =	sItemMsgBase + 207,//�齱
		PROPERTY_CHAGE_REQ					   =   sItemMsgBase + 210,	//���Ըı�
		PROPERTY_CHAGE_RESP					   =   sItemMsgBase + 211,	//���Ըı�ظ�
		PROPERTYS_CHAGE_REQ					   =   sItemMsgBase + 212,	//���Ըı�
		PROPERTYS_CHAGE_RESP				   =   sItemMsgBase + 213,	//���Ըı�ظ�

		OPEN_LOTTERY_MSG_REQ                  =	sItemMsgBase + 220,//�齱һ��
		OPEN_LOTTERY_MSG_RESP                 =	sItemMsgBase + 221,//�齱һ�η���
		OPEN_REWARD_LOTTERY_REQ                 =	sItemMsgBase + 222,//�齱һ��
		OPEN_REWARD_LOTTERY_RESP                =	sItemMsgBase + 223,//�齱һ�η���
	


	};

	enum 
	{
		

		TOWER_INFO_REQ                 =	sClimTowerMsgBase + 0,	//������Ϣ
		TOWER_INFO_RESP                =	sClimTowerMsgBase + 1,	//������Ϣ����
		TOWER_BEGIN_REQ				   =    sClimTowerMsgBase + 2,	//������ʼ
		TOWER_BEGIN_RESP			   =    sClimTowerMsgBase + 3,	//������ʼ����
		IN_TOWER_CHOICE_REQ			   =    sClimTowerMsgBase + 4,	//�����е�ѡ��
		IN_TOWER_CHOICE_RESP		   =    sClimTowerMsgBase + 5,	//�����е�ѡ��
		TOWER_FLOOR_END_REQ			   =    sClimTowerMsgBase + 6,	//����һ�����
		TOWER_FLOOR_END_RESP		   =    sClimTowerMsgBase + 7,	//����һ���������
		TOWER_END_REQ				   =    sClimTowerMsgBase + 8,	//��������
		TOWER_END_RESP				   =    sClimTowerMsgBase + 9,	//������������
		TOWER_RELIVE_REQ			   =    sClimTowerMsgBase + 10,	//������������
		TOWER_RELIVE_RESP			   =    sClimTowerMsgBase + 11,	//���������
		TOWER_MOPUP_REQ				 =    sClimTowerMsgBase + 12,	//����ɨ������
		TOWER_MOPUP_RESP			   =    sClimTowerMsgBase + 13,	//����ɨ������

		TOWER_BUFF_REQ				   =    sClimTowerMsgBase + 112,	//���������
		TOWER_BUFF_RESP				   =    sClimTowerMsgBase + 113,	//���������

	};

	enum 
	{
		SHOP_INFO_REQ                 =		sShopBase + 0,	//�̳���Ϣ
		SHOP_INFO_RESP                =		sShopBase + 1,	//�̳���Ϣ����
		SHOP_BUY_REQ				  =     sShopBase + 2,	//�̳ǹ���
		SHOP_BUY_RESP				  =     sShopBase + 3,	//�̳ǹ��򷵻�	

		STRENGTH_BUY_REQ              =		sShopBase + 4,	//��������
		STRENGTH_BUY_RESP             =		sShopBase + 5,	//�������򷵻�

		COIN_BUY_REQ                  =		sShopBase + 6,	//��ҹ���
		COIN_BUY_RESP                 =		sShopBase + 7,	//��ҹ��򷵻�


		COURAGE_SHOP_INFO_REQ                 =		sShopBase + 10,	//�����̵���Ϣ
		COURAGE_SHOP_INFO_RESP                =		sShopBase + 11,	//�����̵���Ϣ����
		COURAGE_SHOP_BUY_REQ				  =     sShopBase + 12,	//�����̵깺��
		COURAGE_SHOP_BUY_RESP				  =     sShopBase + 13,	//�����̵깺�򷵻�	
	};
	
	enum 
	{
// 		GROWTH_FOUND_REQ                 =		sInLineActivityBase + 0,	//�ɳ�����
// 		GROWTH_FOUND_RESP                =		sInLineActivityBase + 1,	//�ɳ�����
// 
// 		MONTH_SIGN_REQ					 =     sInLineActivityBase + 2,		//��ǩ��
// 		MONTH_SIGN_RESP			  		 =     sInLineActivityBase + 3,		//��ǩ��	
// 
// 		ONLINE_WELFARE_REQ              =		sInLineActivityBase + 4,	//���߽���
// 		ONLINE_WELFARE_RESP             =		sInLineActivityBase + 5,	//���߽���
// 
// 		PEOPLE_WELFARE_REQ              =		sInLineActivityBase + 6,	//ȫ����
// 		PEOPLE_WELFARE_RESP             =		sInLineActivityBase + 7,	//ȫ����
// 
// 		SEVENDAY_LOGIN_AWARD_REQ       =		sInLineActivityBase + 8,	//7���¼����
// 		SEVENDAY_LOGIN_AWARD_RESP      =		sInLineActivityBase + 9,	//7���¼����
// 
// 		SEVENDAY_TASK_REQ              =		sInLineActivityBase + 10,	//7��������
// 		SEVENDAY_TASK_RESP             =		sInLineActivityBase + 11,	//7��������
// 
// 		TIME_AWARD_REQ                 =		sInLineActivityBase + 12,	//��ʱ�ν���
// 		TIME_AWARD_RESP                =		sInLineActivityBase + 13,	//��ʱ�ν���

		SEND_ACTIVITY_RESP                =		sInLineActivityBase + 15,	//��¼ʱ������ݰ�

		ONE_ACTIVITY_REQ					       =		sInLineActivityBase + 16,	//�����
		ONE_ACTIVITY_RESP					       =		sInLineActivityBase + 17,	//��ʱ�ν���

	};



	enum 
	{


		RANKGAME_INFO_REQ              =	sRankGameBase + 0,	//��λ����Ϣ
		RANKGAME_INFO_RESP             =	sRankGameBase + 1,	//��λ����Ϣ����
		RANKGAME_RANKING_REQ		   =    sRankGameBase + 2,	//��λ�����а�
		RANKGAME_RANKING_RESP			=    sRankGameBase + 3,	//��λ�����а񷵻�
		RANKGAME_REPORT_REQ			   =    sRankGameBase + 4,	//��λ��ս��
		RANKGAME_REPORT_RESP		   =    sRankGameBase + 5,	//��λ��ս������	
		RANKGAME_SHOP_REQ			   =    sRankGameBase + 6,	//��λ���̵���Ϣ
		RANKGAME_SHOP_RESP			   =    sRankGameBase + 7,	//��λ���̵���Ϣ����
		RANKGAME_SHOPBUY_REQ			=    sRankGameBase + 8,	//��λ���̵깺��
		RANKGAME_SHOPBUY_RESP		   =    sRankGameBase + 9,	//��λ���̵깺�򷵻�
		RANKGAME_BEGIN_REQ				=    sRankGameBase + 10,	//��λ����ʼ
		RANKGAME_BEGIN_RESP			   =    sRankGameBase + 11,	//��λ����ʼ����
		RANKGAME_END_REQ			   =    sRankGameBase + 12,	//��λ������
		RANKGAME_END_RESP			   =    sRankGameBase + 13,	//
		RANKGAME_BUYTIMES_REQ			   =    sRankGameBase + 14,	//��λ���������
		RANKGAME_BUYTIMES_RESP			   =    sRankGameBase + 15,	//��λ�������������

		

	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_BOX_PACKET_H__
