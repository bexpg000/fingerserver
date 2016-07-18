#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BATTLECALC_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_MANAGER_H__


{

	// module("GameDefine", package.seeall)
	// 
	// 
	// GRUOND_Y = display.bottom + 130 		//�����Y����
	// FRAMES_PERSECOND = 30
	// 
	// PLAYER_TAG = 100
	// 
	// JOYSTICK_RADIUS = 35
	// JOYSTICK_LENGTH = 3 					//����ҡ�˴������� 3*JOYSTICK_RADIUS
	// 
	// DEFAULT_PLAYER_CAMP = 2
	// 
	// BAG_TABLEVIEW_NUM = 3
	// 
	// CHAR_SHADOW = 1
enum	CHAR_SHADOW_CONFIG = {
		scaleY = 0.5,
		offsetX = 0,
		offsetY = 0,
		opacity = 75,
		skewX = 0,
		rotate = 15
	};

enum	CHAR_ATTR = {
		MaxHP 				= 1,	//max hp
		MaxMP 				= 2,	//max mp
		MaxSP				= 3,	//max sp
		PA 					= 4,	//������
		MA 					= 5,	//�������
		PF 					= 6,	//��������
		MF 					= 7,	//��������
		HPRecover			= 8,    //hp�ظ��ٶ�
		MPRecover			= 9,    //mp�ظ��ٶ�
		SPRecover 			= 10,	//sp�ظ��ٶ�
		AttSpeed 			= 11,	//�����ٶ�
		CombinationAtt 		= 12,	//���м��
		MoveSpeed 			= 13,	//�ƶ��ٶ�
		JumpHeight 			= 14,	//����߶�
		FlyTimes 			= 15,	//���������մ���
		SPDamage			= 16,	//sp����
		AttPush 			= 17,	//����
		AttPushDef 			= 18,	//���˷���
		AttRaise 			= 19,	//����
		AttraiseDef 		= 20,	//�������
		Stiff				= 21,	//������ֱ
		StiffDef 			= 22,	//��ֱ����
		BaseCrit 			= 23,	//����������
		Critrate 			= 24,	//��������
		BaseToughness 		= 25,	//����
		VertigoDef 			= 26,	//ѣ�η���(����)
		VertigoTimeDef 		= 27,	//ѣ�η���(ʱ��)
		SlowDef 			= 28,	//���ٷ���(����)
		SlowTimeDef 		= 29,	//���ٷ���(ʱ��)
		PoisonDef 			= 30,	//�ж�����
		BloodDef 			= 31,	//���˷���
		KODef 				= 32,	//��ɱ����
		FloatVal            = 33,   //����
		Floatdef            = 34,	//���ɿ���
		Fall                = 35,	//����
		Falldef             = 36	//��������
	};

enum	CHAR_ATTR_KEY = {
		MaxHP			= "MaxHP",		//max hp
		MaxMP 			= "MaxMP",		//max mp
		MaxSP 			= "MaxSP",		//max sp
		PA 				= "PA",			//������
		MA 				= "MA",			//�������
		PF 				= "PF",			//��������
		MF 				= "MF",			//��������
		HPRecover 		= "HPRecover",  //hp�ظ��ٶ�
		MPRecover 		= "MPRecover",  //mp�ظ��ٶ�
		SPRecover 		= "SPRecover",	//sp�ظ��ٶ�
		AttSpeed 		= "AttSpeed",	//�����ٶ�
		CombinationAtt 	= "CombinationAtt",	//���м��
		MoveSpeed 		= "MoveSpeed",	//�ƶ��ٶ�
		JumpHeight 		= "JumpHeight",	//����߶�
		FlyTimes 		= "FlyTimes",	//�ɻ��ɴ���
		SPDamage 		= "SPDamage",	//sp����
		AttPush 		= "AttPush",	//����
		AttPushDef 		= "AttPushDef",	//���˷���
		AttRaise 		= "AttRaise",	//����
		AttraiseDef 	= "AttraiseDef",//�������
		Stiff 			= "Stiff",		//������ֱ
		StiffDef 		= "StiffDef",	//��ֱ����
		BaseCrit 		= "BaseCrit",	//����������
		Critrate 		= "Critrate",	//��������
		BaseToughness 	= "BaseToughness",	//����
		VertigoDef 		= "VertigoDef",		//ѣ�η���(����)
		VertigoTimeDef 	= "VertigoTimeDef",	//ѣ�η���(ʱ��)
		SlowDef 		= "SlowDef",	//���ٷ���(����)
		SlowTimeDef 	= "SlowTimeDef",	//���ٷ���(ʱ��)
		PoisonDef 		= "PoisonDef",	//�ж�����
		BloodDef 		= "BloodDef",	//���˷���
		KODef 			= "KODef", 		//��ɱ����
		FloatVal 		= "FloatVal",   //����
		Floatdef 		= "Floatdef",	//���ɿ���
		Fall 			= "Fall",		//����
		Falldef 		= "Falldef",	//��������
		[1]		= "MaxHP",			//max hp
		[2]		= "MaxMP",			//max mp
		[3]		= "MaxSP",			//max sp
		[4]		= "PA",				//������
		[5]		= "MA",				//�������
		[6]		= "PF",				//��������
		[7]		= "MF",				//��������
		[8]		= "HPRecover",    	//hp�ظ��ٶ�
		[9]		= "MPRecover",    	//mp�ظ��ٶ�
		[10]	= "SPRecover",		//sp�ظ��ٶ�
		[11]	= "AttSpeed",		//�����ٶ�
		[12]	= "CombinationAtt",	//���м��
		[13]	= "MoveSpeed",		//�ƶ��ٶ�
		[14]	= "JumpHeight",		//����߶�
		[15]	= "FlyTimes",		//�ɻ��ɴ���
		[16]	= "SPDamage",		//sp����
		[17]	= "AttPush",		//����
		[18]	= "AttPushDef",		//���˷���
		[19]	= "AttRaise",		//����
		[20]	= "AttraiseDef",	//�������
		[21]	= "Stiff",			//������ֱ
		[22]	= "StiffDef",		//��ֱ����
		[23]	= "BaseCrit",		//����������
		[24]	= "Critrate",		//��������
		[25]	= "BaseToughness",	//����
		[26]	= "VertigoDef",		//ѣ�η���(����)
		[27]	= "VertigoTimeDef",	//ѣ�η���(ʱ��)
		[28]	= "SlowDef",		//���ٷ���(����)
		[29]	= "SlowTimeDef",	//���ٷ���(ʱ��)
		[30]	= "PoisonDef",		//�ж�����
		[31]	= "BloodDef",		//���˷���
		[32]	= "KODef", 		    //��ɱ����
		[33]	= "FloatVal",   	//����
		[34]	= "Floatdef",		//���ɿ���
		[35]	= "Fall",			//����
		[36]	= "Falldef",		//��������
	}

enum	CHAR_ATTR_CHS_STR = {
		[1]		= "��������",
		[2]		= "��������",
		[3]		= "��������",
		[4]		= "������",
		[5]		= "�������",
		[6]		= "��������",
		[7]		= "��������",
		[8]		= "hp�ظ��ٶ�",
		[9]		= "mp�ظ��ٶ�",
		[10]	= "sp�ظ��ٶ�",
		[11]	= "�����ٶ�",
		[12]	= "���м��",
		[13]	= "�ƶ��ٶ�",
		[14]	= "����߶�",
		[15]	= "���������մ���",
		[16]	= "sp����",
		[17]	= "����",
		[18]	= "���˷���",
		[19]	= "����",
		[20]	= "�������",
		[21]	= "������ֱ",
		[22]	= "��ֱ����",
		[23]	= "����������",
		[24]	= "��������",
		[25]	= "����",
		[26]	= "ѣ�η���(����)",
		[27]	= "ѣ�η���(ʱ��)",
		[28]	= "���ٷ���(����)",
		[29]	= "���ٷ���(ʱ��)",
		[30]	= "�ж�����",
		[31]	= "���˷���",
		[32]	= "��ɱ����",
		[33]	= "����",
		[34]	= "���ɿ���",
		[35]	= "����",
		[36]	= "��������",
	};

enum	ATTRIBUTE_COUNT = 36	//��������

		//��ɫ����
		CHARACTER_TYPE = {
			Player 		= 1,	//���
			Monster 	= 2,	//��
			NPC 		= 3,	//npc
	};

// 	//��������
enum		MONSTER_TYPE = {
			Normal 		= 1,	//��ͨ��
			Elite 		= 2,	//��Ӣ��
			Boss 		= 3,	//boss
			Goblin 		= 4		//�粼��
	};

// 	//��������
		SKILL_TYPE = {
			Normal 		= 1,	//��ͨ
			Passive 	= 2,	//����
			Multi 		= 3,	//���
			Cumulate 	= 4,	//����
			Continue 	= 5,	//����
			Throw 		= 6,	//Ͷ��
			RaiseUp 	= 7,	//����
			FallDown 	= 8,	//��ѹ
	}

// 	//�˺�����
enum		HURT_TYPE = {
			Default 	= 1,	//Ĭ��
	};


// 	// buff������
enum		BUFF_TYPE = {
			Attr 		= 1,	//����
			State 		= 2,	//�쳣״̬
			Display 	= 3 	//����
	};
// 	//�쳣״̬��buff������
enum		BUFF_STATE_SUB_TYPE = {
			Vertigo 	= 1,	//ѣ��
			Slow 		= 2,	//����
			Poison 		= 3, 	//�ж�
			Blood 		= 4,	//����
			God 		= 5,	//�޵�
			Endure 		= 6,	//����
	};

// 	//�ӳɷ�ʽ
enum		VALUE_EFFECT_TYPE = {
			numval = 1,		//��ֵ�ӳ�
			numrate = 2,	//���ٷֱȼӳ�
	};

	//buff ��������������
#define 	MAX_BUFF_CONFIG_PARAM_NUM = 8

		//buff end

	enum	GROW_TYPE = {
			line = 1,			// y = ax + b
			parabolic = 2,		// y = ax^2 + bx + c
	}


enum	CHAR_NODE_PRIORITY = {
		CHAR 		= 1,
		WEAPON_L 	= 2,
		FASHION 	= 3,
		WEAPON_R 	= 4,
		EFFECT 		= 5,
	}
#define 	CHAR_NODE_PIC_CNT = 5	// CHAR_NODE_PRIORITY������

	enum	RES_PREFIX_TBL = {
			[1] = "_char",
			[2] = "_Lweapon",
			[3] = "_clothes",
			[4] = "_Rweapon", 
			[5] = "_effect", 
	};

	//���ɹ���״̬
	enum	CHARACTER_FLYING_STATE = {
			none 		= 0,		//û�л���
			startfly 	= 1,		//��ʼ��
			touchwall 	= 2,		//ײǽ����
			fallground 	= 3 		//�����󳯵�������
	};

	//�ӵ�����
enum		BULLET_TYPE = {
			line	= 1,	
			object	= 2,
	};
enum	BULLET_DESTORY_TYPE = {
		none = 0,
		outScreen = 1 , //������Ļ�� 
		stop = 2 ,      //�����ƶ�, 
		after = 3 ,     //����һ��ʱ��,
		trigger = 4,    //��ײ��
		stopAter = 5    //ֹͣ�ƶ�һ��ʱ��
	};


	//����AI�¼�
enum		SCENEAI_CONDITION_TYPE = {
			//�ֹ����
			monster_seed 			= 1,

			//time
			time_after 				= 101,  	//���֮��
			time_each 				= 102,		//ÿ�����
			hero_time_use 			= 103, 		//�೤ʱ��֮��
			//position
			pos_pass_id 			= 121, 		//ĳ��λ����ĳ��λ��
			pos_pass_camp 			= 122,
			pos_between 			= 123,

			//attr
			attr_atValue_id 		= 141,
			attr_changeValue_id 	= 142,
			attr_atPercent_id 		= 143,
			attr_changePercent_id 	= 144,
			attr_val_camp 			= 145, 		//ĳ��λĳ���Դ��ڡ����ڡ�С��ĳֵ/�ٷֱ�
			hero_hp_camp 			= 146,		//hero Hp����ĳֵʱ����
			//state
			state_die_id 			= 161,
			state_buff_id 			= 162,
			unit_dead 				= 163, 		//ĳ����λ����
			hero_relive_times 		= 164, 		//��Ҹ����������ĳ��
			//action
			action_start_id 		= 181,
			action_end_id 			= 182, 		//ĳ����������
			action_end_scene 		= 183,

			//num_camp
			monster_num_camp 		= 191, 		//monster�������ڡ����ڡ�С��ĳֵ

			// unit_distance
			unit_distance 			= 151, 		//������λ֮�����

			//chapter_progress
			chapter_progress 		= 171, 		//�ؿ�����
	};

	enum SCENEAI_EVENT_TYPE = {
		change_camp			= 101, 		 	 			//1
		play_skill			= 102, 			//�ͷż��� 1
		lock_attr			= 103,			//�������� 1
		show_talk			= 104,			//���Ŷ԰� 1
		create_monster		= 105, 			//����monster
		lock_scene			= 106, 			//��������
		unlock_scene		= 107, 			//��������
		play_ani			= 108, 			//���Ŷ��� 1
		add_buff			= 109, 			//���buff 1
		change_scene_label  = 110, 			//�ı䳡������
		boss_apear_alert 	= 111, 			//ǰ�����ܾ���
		update_timer        = 112,			//�ı��ʱ��
		start_timer         = 113, 			//��ʼ��ʱ��
		stop_timer          = 114,			//ֹͣ��ʱ��
		pause_timer         = 115, 			//��ͣ��ʱ��
		star_unget 			= 117,			//ʧȥһ���ǣ�Ĭ�ϻ�ã��ﵽһ������ʧȥ�������õ�time̫����
		chapter_victory 	= 118, 			//�ؿ�ʤ��
		unit_change_camp 	= 119, 			//�ض���λת����Ӫ
		camp_to_camp 		= 120, 			//�ض���Ӫת��Ϊ��һ����Ӫ
		play_bg_animation 	= 121,			//���ű����㶯��
		stop_bg_animation 	= 122, 			//ֹͣ��������
		unit_bg_to_map 		= 123, 			//monster�ӱ������ս����
	};


enum	SCENEAI_UNIT_ACTION_TYPE = {
		fall2up	    		=	3, //��������
		airhurt	    		=	4, //�����ܻ�
		airfall	    		=	5, //��������
		flyback2up			=	6, //����������
		hurt	    		= 	8, //����ܻ�������hurtbefore��
	};


	//��ɫ״̬
enum		CHAR_STATE = {
			idle		=	1,		//����վ��
			run			=	2,		//�ƶ�
			jump		=	3,		//��Ծ
			die			=	4,		//����
			hurt		=	5,		//�ܻ�
			fall		=	6,		//����
			victory		=	7,		//ʤ��
			attack		=	8,		//����
			show 		= 	9,		//չʾ
			showback 	= 	10, 	//չʾ����
	};

	//��ɫai״̬
	enum	AI_STATE = {
			none        =   0,      //
			idle		=	1,		//����
			fan			=	2,		//����
			walk		=	3,		//Ѱ·
			escape		=	4,		//����
			follow		=	5,		//����
			patrol		=	6,		//Ѳ��
			attack		=	7,		//����
			wander		=	8,		//�ǻ�
			hide 		=   9,  	//���
			show 		=   10,		//չʾ
			fall 		= 	11, 	//����
	};

	//����֡�¼�
enum		FRAME_EVENT_TYPE = {
			move 		=	1,		//�ƶ�
			wait		=	2,		//�ȴ�
			shake		=	3,		//��
			attack 		=	4,		//��������һ֡��
			attackKeep 			=	5,	//������֡���䣩
			mulitCheck_start	=	7,		//�μ��������ж����俪ʼ
			mulitCheck_end		=	8,		//�μ��������ж���������������ж�
			addBuff 	= 	9,		//����buff
			addBullet	=	10,		//�����ӵ�
			addMusicEffect		=	11,		//������Ч
			addAniEffect		=	12,		//������Ч
			allowInterrupt		=	13,		//�ڴ�֡�¼����������������ϼ���
	};

enum	JoystickDir = {
		up = "Joystick_up",
		down = "Joystick_down",
		right = "Joystick_right",
		left = "Joystick_left",
		stop = "Joystick_stop",
	};
enum	ButtonClick = {
		skill1 = "Btn_skill",
		skill2 = "Btn_skill2",
		soldierBtn = "Btn_soldier",
		attack = "Btn_attack",
		pause = "Btn_pause",
		jump = "Btn_jump",
	};

	//�ȽϷ���
	enum	CompareSign = {
			eq = 1,  // ����
			rt = 2,  // ����
			lt = 3,  // С��
			rte = 4, // ���ڵ���
			lte = 5,  // С�ڵ���
			inc = "+", //����
			dec = "-", //�ݼ�
	};

	//����ʱ �������ߵ�״̬
		//0:վ������,1:��������,2:���ڵ�,3:����
enum		BeHurtedState = {
			stand = 0,
			float = 1, 
			fall = 2, 
			floatX = 3,
	};

enum	ANI_KEY_NAME = {
		idle = "idle",
		run = "run",
		airhurt = "airhurt",
		airfall = "airfall",
		fall = "fall",
		fall2up = "fall2up",
		victory = "victory",
		die = "die",
		flyback = "flyback",
		show = "show",
		flybackfall = "flybackfall",
		flyback2up = "flyback2up",
		jumpbefore = "jumpbefore", 
		jumpup = "jumpup", 
		jumpdown = "jumpdown", 
		jumpafter = "jumpafter",
		showback = "showback",
		showstart = "showstart",
		escapewin = "escapewin",
		escapelose = "escapelose",
	};

	//��Դ����
enum		GAME_RES_TYPE = {
			coin 		= 1,
			rmb 		= 2,
			strength 	= 3,
			exp 		= 4,
			prestige 	= 5,
			item 		= 6,
	};

	//�ؿ���������
enum		STAGE_ACTIVE_CONDITION = {
			level 			= 1,
			preFinishStage 	= 2,
			mission 		= 3,
			preFinishStageStarCnt = 4,
			vipLevel		= 5,
			guildLevel		= 6,
	};

	//�ؿ�����
enum		STAGE_TYPE = {
			normal = 1,
			master = 2,
			activity = 3,

			[1] = "normal",
			[2] = "master",
			[3] = "activity",
	};

	//�ؿ���������
		STAGE_STAR_CONDITION = {
			useTime 			= 1,
			reviveTimes 		= 2,
			useMedicinetimes	= 3,
			leftHpValue			= 4,
			leftHpRate			= 5,
	};
	//�ؿ�������
#define 		STAGE_STAR_NUMBER = 3

		//ÿ�½����ؿ�����
#define 		CHAPTER_MAX_STAGE_NUMBER = 20

		//ɨ����������
#define 	SWEEP_ITEM_TYPE_NUM = 3

		//����ÿ�еı�������
#define 	ITEM_CNT_EACH_ROW = 5

		//��������
enum		ITEM_TYPE = {
			equip 			= 1,	//װ��
			gem 			= 2,	//��ʯ
			enhance		 	= 3,	//ǿ��ʯ
			cost 			= 4, 	//��Ϸ����Ʒ
			symbol 			= 5,	//����
			treasureBox 	= 6,	//����
			presentpackage 	= 7,	//���
			medicine 		= 8,	//ҩƷ�ָ������
			equip_piece 	= 9,	//װ����Ƭ
			hoarstone_piece = 10,	//��ʯ��Ƭ
			other 			= 11,	//������Ϸ����
	}

	//��ʯ������
enum		ITEM_GEM_SUBTYPE = {
			red 	= 1,	//��
			yellow 	= 2,	//��
			green 	= 3,	//��
			blue 	= 4,	//��
			purple	= 5,	//��
	};

	//ǿ��ʯ������
enum		ITEM_ENHANCE_SUBTYPE = {
			weapon 	= 1,	//����ǿ��ʯ	
			defence = 2,	//����ǿ��ʯ
			jewelry = 3, 	//��Ʒǿ��ʯ
	};

	//��Ϸ����Ʒ������
enum		ITEM_COST_SUBTYPE = {
			remeltStone 	= 1,		//װ��ϴ��ʯ
			sweepPaper		= 2,		//ɨ����
			rhymeSoul 		= 3,		//�ϻ�
			dungeonTicket	= 4,		//�����볡ȯ
	};

	//ҩƷ�ָ������������
		ITEM_MEDICINE_SUBTYPE = {
			1
	}

	//������Ϸ����������
enum		ITEM_OTHER_SUBTYPE = {
			key_gold	= 1,	//��Կ��
			key_silver	= 2,	//��Կ��
	};

	//����Ʒ��
	enum	ITEM_QUALITY = {
			white 	= 1,	//��
			green 	= 2,	//��
			blue 	= 3,	//��
			purple 	= 4,	//��
			golden 	= 5,	//��
	};

	//װ����
enum		CHARACTER_EQUIP_POS = 
	{
		weapon 	= 1,	//����
		head	= 2,	//ͷ
		chest 	= 3,	//��
		belt 	= 4,	//����
		leg 	= 5,	//��
		foot 	= 6,	//��
		jewelry = 7,	//����
		amulet 	= 8,	//����
	};

	//װ������Ǽ�
#define 		MAX_EQUIP_STAR_LEVEL = 5

		//���ߵĲ�����������
#define 	ITEM_PARAM_NUM = 8

		//��������
enum		BAG_TAB_TYPE = 
	{
		equip = 1,
		item  = 2,
		other = 3,
		[1] = "equip",
		[2] = "item",
		[3] = "other",
	};

	// �ؿ�����
enum		SELECT_TAG_TYPE = 
	{
		normal = 1,
		master = 2,
		[1] = "normal",
		[2] = "master",
	};
	//�ؿ�ɨ������
enum		SELECT_SWEEP_PROPERTY = 
	{
		cunzheng = 1,                //����
		power    = 2,                //С����ҩ
		gem      = 3,                //��ʯ
		[1]      = "cunzheng",
		[2]      = "power",
		[3]      = "gem",
	};
	//ʱװ�����Ч����������
#define 		MAX_FASHION_EFFECTIVE_ATTR_COUNT = 6

		//��ʯ��������
#define 	HOARSTONE_TYPE_NUM = 12

// 		//��Ӧ��Դ��
// 	enum	colorRes = {
// 			ResourceDef.IMAGE_BAG_BTNFRAME_WHITE, 
// 			ResourceDef.IMAGE_BAG_BTNFRAME_GREEN,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_BLUE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_PURPLE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE,
// 			ResourceDef.IMAGE_BAG_BTNFRAME_GRAY, 
// 	};

	//effect type
enum		EFFECT_RES_TYPE = {
			particle = 1,
			frameAction = 2,
			blend = 3,
			other = 4,
	};

enum	EFFECT_PLAY_TYPE = {
		PLAY_ONCE = 1,
		PLAY_TIME = 2,
		PLAY_FOREVER = 3,
		SPECIAL = 4,
	};

enum	EFFECT_POS_REFERENCE_TYPE = {
		Camera = 1,
		MapPos = 2,
		AttackerPos = 3,
		DefencerPos = 4,
	};

enum	EFFECT_PARTICLE_POSITION_TYPE = {
		free = 1,
		grouped = 2,
		relative = 3,
	};

	//refresh event
enum		GAME_REFRESH_EVENT = {
			addItem 	= "addItem",
			removeItem	= "removeItem",
			updateItem	= "updateItem",
			equipResult		= "equipResult",
			updateFashion	= "updateFashion",
			equipFashion 	= "equipFashiion",
			coinChange	= "coinChange",
			rmbChange	= "rmbChange",
			strengthChange	= "strengthChange",
	};

	//Ӷ��װ������
#define 	MAX_LANSQUENET_EQUIP_POS_NUM = 4

		//MonsterType
	enum	MonsterType = {
			monster = 0,
			boss 	= 1,
			goblin 	= 2,
			elite   = 3, 		//��Ӣ��
			lansquenet = 4,
	};

	//SoldierBtnScale
#define 	SoldierBtnScale = {
			[1] = 0.8,
			[2] = 0.5,
			[3] = 0.3, 
	};

	//Goblin_Escape_Type
	enum	Goblin_Escape_Type = {
			lose 	= 1,
			win 	= 2,
	};

	//infinite loop  ,cann't push cca.RepeatForver to sequence  in cocos2dx
#define 		REPEATFOREVER = 9999999999
}
#endif // __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_MANAGER_H__
