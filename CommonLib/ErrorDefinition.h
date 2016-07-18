#ifndef __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__
#define __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__

#include "TypeDefinition.h"

namespace Lynx
{
	namespace LynxErrno
	{
		// CommonError
		static const errorId None                          = 0;		//�ɹ�
		static const errorId UnknownLogicError             = 1001;	//
		static const errorId InvalidIdentify               = 1002;	//
		static const errorId InvalidOperation              = 1003;	//
		static const errorId InvalidType                   = 1004;	//
		static const errorId InvalidPermission             = 1005;	//
		static const errorId InvalidParameter              = 1006;	//
		static const errorId InvalidState                  = 1007;	//  
        static const errorId TemplateNotExist              = 1008;	//
     
        static const errorId ItemNotExist                  = 1011;	//
        static const errorId EquipNotExist                 = 1012;	//
        static const errorId RuneNotExist                  = 1013;	// ���Ĳ�����
    
        static const errorId CoinNotEnough                 = 1016;	//ͭǮ����
        static const errorId LevelExpNotEnough             = 1017;	//���鲻��
        static const errorId StarExpNotEnough              = 1018;	//
		static const errorId StepExpNotEnough              = 1019;

        static const errorId LevelNotEnough                = 1020;	//�ȼ�����
        static const errorId StarNotEnouth                 = 1021;	//��������
      
      
    
        static const errorId RmbNotEnough                  = 1029;	//Ԫ������
        static const errorId BagIsFull                     = 1030;	//
        static const errorId ItemIsFull                    = 1031;	//
 
		static const errorId MaterialNotEnough             = 1036;	//

		static const errorId StrengthNotEnough             = 1037;	//ȱ������
		static const errorId TimesNotEnough				   = 1038;	//��������
		static const errorId OverMaxTimes			       = 1039;	//�������޴���
		static const errorId TimeNotRight		           = 1040;	//���ǿ���ʱ�� //���ʱ�䲻��ȷ
 		static const errorId NotOpen					   = 1041;	//δ����
 		static const errorId NetWorkError				   = 1042;	//�ͻ��˴������ݴ��󣬸������ԣ���С����

		static const errorId IsUnlocked					   = 1043;	//�Ѿ�����
		static const errorId IsLocked					   = 1044;	//δ����
		static const errorId bugleNotEnough				   = 1045;	//�Žǲ���
		static const errorId scoreNotEnough				   = 1046;	//���ֲ���
		static const errorId IsDad						   = 1047;	//������
		static const errorId IsBuying					   = 1048;	//���ڹ���

		static const errorId OnceTicketNotEnough			= 1049;	//һ��ȯ����
		static const errorId TenTicketNotEnough				= 1050;	//ʮ��ȯ����
		static const errorId BeingCooled				    = 1051;	//������ȴ

		

 		static const errorId VipLevelNotEnough             = 1081;	//vip�ȼ�����
		static const errorId VipBuyTimesNotEnough		   = 1082;	//vip�����������
		static const errorId TableElementNotExit           = 1100;  //����û���������
		static const errorId PlayerNotFound	   			   = 1101;  //û���ҵ����
		static const errorId ServantSwitchNotEnough	       = 1102;  //����֮�ⲻ��



		// LoginError
		static const errorId AccountNotExist               = 2001;	//
		static const errorId AccountHasExist               = 2002;	//
        static const errorId AccountAuthFailed             = 2003;	//
        static const errorId PlayerNotExist                = 2004;	//
        static const errorId LoadPlayerFailed              = 2005;	//
        static const errorId InitPlayerFailed              = 2006;	//
        static const errorId LoginTokenFailed              = 2007;	//
        static const errorId PlayerInfoNotExist            = 2008;	//
        static const errorId NameLengthError               = 2009;	//
        static const errorId ActiveNameSpace               = 2010;	//
        static const errorId PlayerNameForbidden           = 2011;	//

    
        // DataBase
        static const errorId SqlExecuteFail                = 5001;	//
        static const errorId CreatePlayerFailed            = 5002;	//
        static const errorId PlayerNameConflict            = 5003;	//

        // Email
        static const errorId InvalidEmailItem              = 6001;	//
        static const errorId EmailNotExist                 = 6002;	//

      
		//Gem
		static const errorId GemLevelLimit				   = 9005;  //
		static const errorId GemNotExist				   = 9006;  //
		static const errorId GemNotEnough				   = 9007;

		//Rhyme
		static const errorId RhymeLevelLimit               = 10001;	//
		//�Ϲ��Ѿ�����
		static const errorId AcupointActived               = 10002;	//

		static const errorId RhymeStepLimit                = 10003;

		//����
		static const errorId SkillLevelLimit			   = 11001;	//

		//ϴ��

		static const errorId MaterialRdEmpty               = 12001;	//

		//��ʯ
		static const errorId HoarStoneActived              = 13001;	//
		static const errorId HoarStoneLvLimit              = 13002;	//
		static const errorId HoarStoneCondition            = 13003;	//

		//װ��
		//װ���ȼ��ﵽ��߼���
		static const errorId EquipLvLimit				   = 14001;

		static const errorId EquipNoneActive			   = 14002;

		static const errorId EquipGemSlotLock			   = 14003;

		static const errorId EquipGemHasExist			   = 14004;

		static const errorId EquipGemTypeError			   = 14005;

		static const errorId EquipGemUnlocked			   = 14006;

		static const errorId FashionHasExit							= 14007;

		static const errorId FashionNotExit							= 14008;


		//�ؿ�

		static const errorId AttackPowerNotRight		   = 15001;//ս������

		static const errorId NumberNotRight				   = 15002;//��ֵ����ȷ

 		static const errorId SeatIsFull					   = 15003;//λ������
				
		static const errorId BeDissolved				   = 15004;//��ɢ
				
 		static const errorId NotFound					   = 15005;//û�ҵ�
		
		static const errorId TimeOut					   = 15010;//ʱ�䵽
		
		static const errorId NotRelive					   = 15011;//û�и���

		static const errorId LeaveStage					   = 15012;//�뿪�ؿ�

		static const errorId Found						   = 15013;//�ҵ�

		static const errorId Used						   = 15014;//��ʹ��

		static const errorId ReliveTimesUsedUp			   = 15015;//���������ʹ��
// 	
// 				
// 		static const errorId AttackPowerNotRight				   = 15001;

		//��Ʒ
		static const errorId JewelryLock                   = 16001;
		static const errorId JewelryEquiped                = 16002;
		static const errorId SlotEmpty                     = 16003;
		static const errorId SlotPosError                  = 16004;

		//Ӷ��
		static const errorId ServantPieceLess              = 17001;
		static const errorId ServantExist                  = 17002;
		static const errorId ServantLvLimit                = 17003;
		static const errorId ServantNotExit                = 17004;
		static const errorId ServantStarLimit              = 17005;
		static const errorId ServantFloorLimit             = 17006;
		static const errorId STreasureNotExit              = 17007;
		static const errorId STreasureNotEnough            = 17008;
		static const errorId ServantBattleOpen             = 17009;
		static const errorId ServantBattleLock             = 17010;
		static const errorId BattleQualityLvLimit          = 17011;

		//����
		static const errorId OtherFriendFull							= 18001; //���˵ĺ�������
		static const errorId OtherBlackFull							= 18002; //���˵ĺ���������
		static const errorId SelfFriendFull							= 18003; //�Լ��ĺ�������
		static const errorId SelfBlackFull							= 18004; //�Լ��ĺ���������

		//
		static const errorId Lose							= 19001; //ʧ��
		static const errorId Win							= 19002; //ʤ��
		static const errorId HadBuy							= 19003; //�Ѿ�����
		static const errorId RefreshTimesNotEnough			= 19004; //ˢ�´�������
		static const errorId IsActivated					= 19010; //�Ѿ�����
		static const errorId CatCoinNotEnough				= 19011; //è�Ҳ���
		static const errorId VipBuyCourageChallengeTimesNotEnough	  = 19012;	//vip����������ս��������
		static const errorId CoinGroupNotEnough				 = 19013;	//Ӳ����Ͻ�������

		static const errorId HadGot							= 19014; //�Ѿ���ȡ
		static const errorId HadSign						= 19015; //�Ѿ�ǩ��
		static const errorId SignCountNotEnough				= 19016; //ǩ����������
		static const errorId RechargeNumNotEnough			= 19017; //��ֵ��������
		static const errorId TaskNotFinished				= 19018; //����δ���

		



		//��ɫ
		static const errorId CharactorExits         = 21000; //��ɫ�Ѿ�����
		static const errorId CharactorNotExits   =  21001; //��ɫ������
		static const errorId IsNotCurCharactor  =  21002; //��ɫ���ǵ�ǰװ����ɫ 
	
		//�ɾͺ�ÿ������
		static const errorId EventNotFind       = 22000; //�ɾͶ�Ӧ���¼�û�ҵ�
		static const errorId AchIdNotMatch   = 22001; //�ɾ�id��ƥ��
		static const errorId AchIdNotExist      = 22002; //���ñ���û�ж�Ӧ��id
		static const errorId ConditionNotReach = 22003; //����δ���
		static const errorId DailyTaskIdNotMatch		= 22004;//�ճ�����id��ƥ��
		static const errorId DailyTaskHasExist          = 22005;//ÿ�������Ѿ�����
		

		

	} // namespace LynxErrno
} //namespace Lynx

#endif // __LYNX_COMMON_LIB_ERROR_DEFINITION_DEFINITION_H__