#ifndef __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__
#define __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__

#include "TypeDefinition.h"

#define TCP_MAX_CACHE_ISTREAM_LENGTH  2097152  /* 2 * 1024 * 1024 */
#define TCP_MAX_CACHE_OSTREAM_LENGTH  2097152  /* 2 * 1024 * 1024 */

// �ʼ��ռ����������
#define EMAIL_MULTICAST_MAX_COUNT 50
// �ʼ�Я�������������
#define EMAIL_ITEM_MAX_COUNT 5
// �ʼ��ı�������󳤶�
#define EMAIL_CONTENT_MAX_LENGTH 200

// �������������ʷ����
#define WORLD_CHAT_MAX_HISTORY_COUNT 200
// �������������ʷ����
#define GUILD_CHAT_MAX_HISTORY_COUNT 100
// ������������ʷ����
#define PLAYER_CHAT_MAX_HISTORY_COUNT 50
// �ͻ���������ʷ����
#define CLIENT_CHAT_HISOTRY_COUNT 24

// ����������
#define BUDDY_MAX_COUNT 50
// ��������������
#define INVITE_BUDDY_MAX_COUNT 50
// ����Ƽ���������
#define RECOMMEND_BUDDY_MAX_COUNT 5

// �����ʯ���ֵ (7λ)
#define PLAYER_RMB_MAX_VALUE  9999999 

// ��Ҿ������ֵ (8λ����)
#define PLAYER_EXP_MAX_VALUE 99999999
// ��ҵȼ����ֵ
#define PLAYER_LEVEL_MAX_VALUE 999
// �������ֵ���ֵ
#define PLAYER_ENERGY_MAX_VALUE 999
// ���ս�����ֵ (8λ��)
#define PLAYER_BATTLE_VALUE_MAX_VALUE 99999999
// ��ҽ�����ֵ(42�ڣ�
#define PLAYER_GOLD_MAX_VALUE 4294967295
// Ӣ�۾������ֵ
#define HERO_EXP_MAX_VALUE 99999999
// ���ﾭ�����ֵ
#define PET_EXP_MAX_VALUE 99999999
// ���������(42�ڣ�
#define PLAYER_SOUL_MAX_VALUE 4294967295
// ����������
#define PLAYER_HONOR_MAX_VALUE 4294967295
// Ӣ�۵ȼ����ֵ
#define HERO_LEVEL_MAX_VALUE 999
// ����ȼ����ֵ
#define PET_LEVEL_MAX_VALUE 999
// Ӣ�۽������ֵ
#define HERO_ADVANCED_MAX_VALUE 9
// Ӣ��Ѫ�����ֵ����λ��)
#define HERO_HP_MAX_VALUE 9999999
// Ӣ���Ǽ����ֵ
#define HERO_STAR_LEVEL_MAX_VALUE 9
// �����Ǽ��������ֵ
#define PET_STAR_LEVEL_EXP_MAX_VALUE 4294967295
// Ӣ��ŭ�����ֵ
#define HERO_ANGER_MAX_VALUE 999
// Ӣ�����������ֵ����λ��
#define HERO_ATTACK_MAX_VALUE 999999
// Ӣ������������ֵ����λ��
#define HERO_PHYSICS_DEFENSE_MAX_VALUE 999999
// Ӣ��ħ���������ֵ����λ��
#define HERO_MAGIC_DEFENSE_MAX_VALUE 999999
// Ӣ�۱������ֵ��5λ��
#define HERO_CRITICAL_MAX_VALUE 99999
// Ӣ���������ֵ��5λ��
#define HERO_HIT_MAX_VALUE 99999
// Ӣ���������ֵ��5λ��
#define HERO_DODGE_MAX_VALUE 99999
// Ӣ��Ѫ���������ֵ��7λ��
#define HERO_HP_UPPER_MAX_VALUE 9999999
// Ӣ��ŭ���������ֵ
#define HERO_ANGER_UPPER_MAX_VALUE 999
// Ӣ���������ֵ
#define HERO_ENDURANCE_MAX_VALUE 99999 
// Ӣ���������ֵ
#define HERO_INTELLIGNECE_MAX_VALUE 99999
// Ӣ���������ֵ
#define HERO_POWER_MAX_VALUE 99999
// Ӣ�ۿ������ֵ
#define HERO_RESILIENCE_MAX_VALUE 99999 
// ���ļ�¼Ӣ������
#define CONSUME_RECORD_TYPE_HERO    1
// ���ļ�¼���͵���
#define CONSUME_RECORD_TYPE_ITEM    2

#define  HURT_AIR_FALL_SPEED  0

#define  SlowMotionFactor  0.5 // ����������ٶȵ�ϵ�� �������ٶ�Ϊ 1 
#define SlowMotionDuration  2 * SlowMotionFactor // ����������ʱ�䣬��ʱ��ָ�����
//��Ծ���

#define JUMP_V0  200
#define JUMP_KEEP_V  -1
#define HURT_DELAY_TIME 500 // 0.05 �����ܻ�ͣ��ʱ��
#define BOUNCE_MIN_SPEED  -150
#define BOUNCE_DECREASE_RATE  0.2

//����
#define HIT_RECORD_MIN_TIME  3
#define HIT_CHANGE_MAX_TIME  0.1
#define HIT_TICK_TIME  0.01

//�������ֵ
#define MAX_MONEY  999999
#define MAX_RMB  999999

//����tick
#define SCENE_TICK_TIME  0.05

//�������ʱ��
#define DROP_BEFORE_FLY_DELAY_BASE_TIME  0.75
#define DROP_FLY_TIME  0.5
//�������Y��ƫ��
#define DROP_FLY_OFFSETY  20

//ҡ���¼�Ƶ��
#define JOYSTICK_TIME_INTERVAL  0.05 //ҡ�˷����¼���Ƶ��

//����ƶ��ٶ�
#define DEFAULT_HERO_MOVE_SPEED  750       //��Ϸ��Ĭ��Ӣ���ƶ����ٶȣ���Ӣ���ƶ�����Ϊ0ʱ���������ֵ�������㣩
//����ʱ�����ٶ�
#define HURT_FLY_SPEED  2200
#define HURT_FLY_WALL_BOUNCE_TIME  0.15
#define HURT_FLY_WALL_BOUNCE_OFFSETX  40

//�������
#define DIE_TO_FADEOUT_DELAY_TIME  1.5
#define DIE_FADEOUT_TIME  1

//�ܴ��ʱ��ɫʱ��
#define HURT_FLASH_KEEPTIME  0.25
//�ܴ��ʱpushλ��ʱ��
#define HURT_PUSH_MOVE_TIME  0.01
//�ڿ����ܴ��ʱ���Ŀ��������ٶ�
#define HURT_AIR_FALL_SPEED  0

//����ز���
#define SHAKE_CHAR_OFFSET  2
#define SHAKE_CHAR_TIME  0.01
#define SHAKE_SCREEN_OFFSET  3.5
#define SHAKE_SCREEN_TIME  0.12

//shuffle tiles ����
#define SHUFFLE_CHAR_TIME  0.5
#define SHUFFLE_CHAR_SIZE  cc.size(200, 200)
#define SHUFFLE_CHAR_TO_FADEOUT_TIME  0.3

//hp���ڴ�ֵ,ϵͳ���о�����ʾ (���С��1�����磬0.2��ʾ���ڰٷ�֮��ʮ�� �������1������20��ʾ��ǰѪ��С��20)
#define SYSTEM_ALERT_LOW_HP  4000
//--�������޹���Ŀ��ʱ�� ��Ϊ�ı�ʱ����
#define MONSTER_PATROL_INTERVAL  5

//--��Ļ��ī��������ֵ����2�����������ֵ��5������������ļ����� 2/5 ��
#define SCREEN_INK_TOTAL_NUMBER  5
//--��Ļ��ī��ʧʱ��
#define SCREEN_INK_FADEOUT_TIME  2
//--��Ļ��īÿ�ε���Ч������
#define SCREEN_INK_EACH_USE_COUNT  5

//--ҡ�������任���ܰ�ť����ʱ���ŵ���Чid
#define JOYSTICK_CHANGE_SKILL_EFFECT_ID  20
//--���ﱻ����ʱ���ŵ����Ѷ���
#define MONSTER_EXPLODE_EFFECT_ID  21

//--��������cdʱ��
#define BAG_ORDER_TIME  5
//--�������ӵ���Ŀ
#define BAG_ITEM_NUM  100 

#define ITEM_GOLD_TEMPLATEID        101
#define ITEM_DIAMOND_TEMPLATEID     102
#define ITEM_SOUL_TEMPLATEID        103
#define ITEM_ENERGY_TEMPLATEID      104
//�������������ݴ浵ʱ�����
#define SAVE_INV_TIME      5000
//����������������ã�����
#define RESET_INV_TIME    1000
//������������������������
#define SAVE_ONLINE_NUM_TIME      180000
//���������߽���������ã�����
#define CHECK_ONLEWELFARE_TIME    60000

// �ģ��ID
#define Activity_Setting_Rune_TemplateID 1
#define Activity_Setting_Gold_TemplateID 2
#define Activity_Setting_Enchant_TemplateID 3
#define Activity_Setting_Pet_TemplateID 4
#define Activity_Setting_StoreHouse_TemplateID 5
#define Activity_Setting_TemplateID_Max 5

#define ITEM_EQUIP_TYPEID		 1
#define ITEM_ITEMS_TYPEID		 2
#define ITEM_DIAMOND_TYPEID		 3
#define ITEM_MATERIAL_TYPEID	 4
#define	ITEM_GIFT_TYPEID		 5
#define ITEM_TREASUREBOX_TYPEID	 6



namespace Lynx
{
    enum HeroPersentPos
    {
        HERO_PRESENT_POS_BAG                 = 0,
        HERO_PRESENT_POS_LEFT                = 1,
        HERO_PRESENT_POS_MIDDLE              = 2,
        HERO_PRESENT_POS_RIGHT               = 3,
        HERO_PRESENT_POS_ALTERNATE_1         = 4,
        HERO_PRESENT_POS_ALTERNATE_2         = 5,
    };

    enum PassportAuthStatus
    {
        PASSPORT_AUTH_INVALID           = 0,
        PASSPORT_AUTH_SUCCESS           = 1,
        PASSPORT_AUTH_ACCOUNT_NOT_EXIT  = 2,
        PASSPORT_AUTH_PASSWORD_ERROR    = 3,
    };

    //����������λ
    enum BagItemField
    {
        BAG_ITEM_FIELD_NIVALID          = 0,
        BAG_COMMON_ITEM_FIELD           = 1,
        BAG_EQUIP_ITEM_FIELD            = 2,
        BAG_EQUIP_DEBRIS_ITEM_FIELD     = 3,
        BAG_HERO_DEBRIS_ITEM_FIELD      = 4,
    };

    // ����ʹ��Ч������
    enum ItemUseEffect
    {
        ITEM_USE_INVALID                    = 0,
        ITEM_USE_TRUASURE_CHESTS            = 1,    // ����
        ITEM_USE_GIFT_BAG                   = 2,    // ���
        ITEM_USE_KEY                        = 3,    // Կ��
        ITEM_USE_TREASURE_WITHOUT_KEY       = 4,    // ����(����ҪԿ�׿���)
        ITEM_USE_PET_EXP                    = 5,    // ��������
        ITEM_USE_COMBINE_EQUIP              = 6,    // �ϳ�װ��
        ITEM_USE_ADD_ENERGY                 = 7,    // �������
        ITEM_USE_ADD_DIAMOND                = 8,    // �����ʯ
        ITEM_USE_ADD_GOLD                   = 9,    // ��ý��
        ITEM_USE_ADD_SPRING_EXP             = 10,   // ��þ���Ȫˮ����
        ITEM_USE_CHANGE_PLAYER_NAME         = 11,   // ����
        ITEM_USE_ADD_SOUL                   = 12,   // �������
    };

    enum EnchantType
    {
        // ������
        ENCHANT_ATTACK_TYPE               = 1,
        // �������
        ENCHANT_PHYSICS_DEFENSE_TYPE      = 2,
        // ħ������
        ENCHANT_MAGIC_DEFENSE_TYPE        = 3,
        // ����
        ENCHANT_CRITICAL_TYPE             = 4,
        // ����
        ENCHANT_HIT_TYPE                  = 5,
        // ����
        ENCHANT_DODGE_TYPE                = 6,
        // Ѫ������
        ENCHANT_HP_TYPE                   = 7,
        // ŭ������
        ENCHANT_ANGER_TYPE                = 8,
    };

    enum MonsterType
    {
        // ��ͨ��
        MONSTER_COMMON    = 1,
        // BOSS
        MONSTER_BOSS      = 2,

		MONSTER_NPC 	  = 3,	////npc
    };

    enum ItemSourceType
    {
        ITEM_SOURCE_INVALID             = 0,
        ITEM_SOURCE_GM_COMMAND          = 1,
        ITEM_SOURCE_SYSTEM_AWARD        = 2,
    };

    enum TollgateDifficulty
    {
        TOLLGATE_INVALID                = 0,
        TOLLGATE_EASY                   = 1,
        TOLLGATE_NORMAL                 = 2,
        TOLLGATE_HARD                   = 3,
    };

    enum ItemType
    {
        EQUIP_TYPE        = 1,
        ITEM_TYPE         = 2,
        HERO_TYPE         = 3,
        HERO_DEBRIS_TYPE  = 4,
        PET_TYPE          = 5,
        RUNE_TYPE         = 6,
        PET_DEBRIS_TYPE   = 7,
        EQUIP_DEBRIS_TYPE   = 8,
    };

    enum StoreItemLimitNumType
    {
        STORE_ITEM_LIMIT_NUM_NONE   = 0,
        STORE_ITEM_LIMIT_NUM_DAILY  = 1,
        STORE_ITEM_LIMIT_NUM_CAREER = 2,
    };

    enum StoreItemLimitTimeType
    {
        STORE_ITEM_LIMIT_TIME_NONE      = 0,
        STORE_ITEM_LIMIT_TIME_SELL      = 1,
        STORE_ITEM_LIMIT_TIME_DISCOUNT  = 2,
    };

    enum StorItemType
    {
        STORE_ITEM_TYPE_INVALID         = 0,
        STORE_ITEM_TYPE_ITEM            = 1,
        STORE_ITEM_TYPE_GIFTBAG         = 2,
    };

    enum SystemAwardType
    {
        SYSTEM_AWARD_TYPE_INVALID           = 0,
        SYSTEM_AWARD_TYPE_CONTINOUS_LOGIN   = 1,
        SYSTEM_AWARD_TYPE_LEVEL             = 2,
        SYSTEM_AWARD_TYPE_ACCUMULATE_LOGIN  = 3,
    };

    enum BattleCombatType
    {
        BATTLE_CLOSE    = 1,
        BATTLE_FAR      = 2,
    };

    enum BattleRelationship
    {
        BATTLE_FRIEND   = 1,
        BATTLE_FOE      = 2,
    };

    enum BattleCharacterType
    {
        BATTLE_MONSTER  = 1,
        BATTLE_HERO     = 2,
        BATTLE_PET      = 3,
    };

    // ���ܹ������ͣ���λ��������壬���2����ȫ�壬Ѫ���ģ�Ѫ���ٵ�����
    enum SkillAttackType
    {
        SKILL_ONLY_POSITION     = 1,
        SKILL_RANDOM_ONE        = 2,
        SKILL_RANDOM_TWO        = 3,
        SKILL_ALL               = 4,
        SKILL_MOST_BLOOD_ONE    = 5,
        SKILL_LEAST_BLOOD_ONE   = 6,
    };

    // ���ܵ���
    enum SkillRelationship
    {
        SKILL_FOE       = 1,
        SKILL_FRIEND    = 2,
    };



    enum BattleActorPosType
    {
        BATTLE_ENEMY_LEFT       = 1,
        BATTLE_ENEMY_MIDDLE     = 2,
        BATTLE_ENEMY_RIGHT      = 3,
        BATTLE_ENEMY_PET        = 4,
        BATTLE_PLAYER_LEFT      = 5,
        BATTLE_PLAYER_MIDDLE    = 6,
        BATTLE_PLAYER_RIGHT     = 7,
        BATTLE_PLAYER_PET       = 8,
    };

    enum BufferType
    {
        BUFFER_TYPE_BUFF    = 1,
        BUFFER_TYPE_DEBUFF  = 2,
    };

    //0.����������������������buff
    //1.�������ܹ�������
    //2.����������
    //3.�����ι���ɱ��Ŀ�괥��
    //4.��������������
    enum BufferTriggerType
    {
        BUFFER_TRIGGER_ALL            = 0,
        BUFFER_TRIGGER_DODGE          = 1,
        BUFFER_TRIGGER_BEHIT          = 2,
        BUFFER_TRIGGER_KILL_IN_ONCE   = 3,
        BUFFER_TRIGGER_CRITICAL       = 4,
    };
    /*
    1������
    2��������
    3���������
    4��ħ������
    5��������
    6��������
    7������
    8������
    9���˺�����
    10����Ѫ
    11������
    12�������˺�
    */
    enum BufferPropertyType
    {
        BUFFER_PROPERTY_HP               = 1,
        BUFFER_PROPERTY_ATTACK           = 2,
        BUFFER_PROPERTY_PHYDEF           = 3,
        BUFFER_PROPERTY_MAGDEF           = 4,
        BUFFER_PROPERTY_CRITICAL         = 5,
        BUFFER_PROPERTY_RESCRIT          = 6,
        BUFFER_PROPERTY_HIT              = 7,
        BUFFER_PROPERTY_DODGE            = 8,
        BUFFER_PROPERTY_DAMAGE_RED       = 9,
        BUFFER_PROPERTY_SUCK_BLOOD       = 10,
        BUFFER_PROPERTY_CURE             = 11,
        BUFFER_PROPERTY_CRITICAL_DAMAGE  = 12,
    };

    /*
    1��ȼ�գ��ж�ǰ���ܵ�������ħ���˺����������ٷֱȣ�
    2���ж����ж�ǰ���ܵ�ˮ����ħ���˺����������ٷֱȣ�
    3����Ѫ���ж�ǰ���ܵ������˺����������ٷֱȣ�
    4�����ƣ��ж�ǰ���ܵ������ָ����������ٷֱȣ�
    5����ԣ��ж�ǰ�����������ж����̣������ж��ļ��ʣ�
    6����ä������miss�ʣ�����ͨ�����б���ǰ���㣬miss���ʣ�������ǰ��
    7�����𣨶Է����幥�������Ŀ�깥��ʱ����Ϊ����Ŀ�꣬�޲�������ѡĿ��ǰ��
    8����Ĭ���޷�ʹ���Զ����ܣ�ֻ��ʹ���չ����޲�����������ǰ��
    9�������޵У��������������˺�����Ѫ�˺����޲�����������ǰ��
    10��ħ���޵У�����ħ���������˺���ȼ�գ��ж����˺���������ԣ����𣬳�Ĭ����ä����ӡЧ����������ǰ��
    11����ӡ������ӡ���չ��޷����ŭ����������
    12�������������˺��ٷֱȵ��˺���������������ǰ��
    13������ָ������������ްٷֱȵ���������������������Ч��
    14��������Ч��ֵ��Ӧbuffer���ͣ�ɾ�����������Ӧ����buffer����������Ч��
    15����ŭ����������Ч��
    16����ŭ����������Ч��
    */
    enum BufferEffectType
    {
        BUFFER_EFFECT_BURN               = 1,
        BUFFER_EFFECT_POISONING          = 2,
        BUFFER_EFFECT_BLEED              = 3,
        BUFFER_EFFECT_CURE               = 4,
        BUFFER_EFFECT_PARALYSIS          = 5,
        BUFFER_EFFECT_BLIND              = 6,
        BUFFER_EFFECT_CONVERGED          = 7,
        BUFFER_EFFECT_SILENT             = 8,
        //DAMAGE_RED         = 9,
        BUFFER_EFFECT_PHY_INVINCIBLE     = 9,
        BUFFER_EFFECT_MAG_INVINCIBLE     = 10,
        BUFFER_EFFECT_SEAL               = 11,
        BUFFER_EFFECT_REBOUND            = 12,
        BUFFER_EFFECT_SELF_CURE          = 13,
        BUFFER_EFFECT_CLEAN              = 14,
        //SUCK_BLOOD         = 16,
        BUFFER_EFFECT_ANGER_RED          = 15,
        BUFFER_EFFECT_ANGER_INC          = 16,
    };

    // Ŀ�����ͣ��Լ���Ŀ��
    enum BufferTargetType
    {
        BUFFER_TARGET_SELF   = 1,
        BUFFER_TARGET_TARGET = 2,
    };

    // �ɾ�����
    enum AchievementType
    {
        // ��ս��
        ACHIEVEMENT_TYPE_BATTLE_VALUE               = 1,
        // ӵ��4��Ӣ����
        ACHIEVEMENT_TYPE_OWN_STAR_FOUR_HERO_CNT     = 2,
        // ӵ��5��Ӣ����
        ACHIEVEMENT_TYPE_OWN_STAR_FIVE_HERO_CNT     = 3,
        // ӵ��6��Ӣ����
        ACHIEVEMENT_TYPE_OWN_STAR_SIX_HERO_CNT      = 4,
        // Ӣ��ǿ����ߵȼ�
        ACHIEVEMENT_TYPE_HERO_REFINE_LEVEL          = 5,
        // һ��Ӣ�۾��ѵȼ�
        ACHIEVEMENT_TYPE_ONE_HERO_AWAKEN_LEVEL      = 6,
        // ����Ӣ�۾��ѵȼ�
        ACHIEVEMENT_TYPE_TWO_HERO_AWAKEN_LEVEL      = 7,
        // ����Ӣ�۾��ѵȼ�
        ACHIEVEMENT_TYPE_THREE_HERO_AWAKEN_LEVEL    = 8,
        // �ĸ�Ӣ�۾��ѵȼ�
        ACHIEVEMENT_TYPE_FOUR_HERO_AWAKEN_LEVEL     = 9,
        // ���Ӣ�۾��ѵȼ�
        ACHIEVEMENT_TYPE_FIVE_HERO_AWAKEN_LEVEL     = 10,
        // �ϳ�Ӣ������
        ACHIEVEMENT_TYPE_COMBINE_HERO_CNT           = 11,
        // ӵ����ɫװ������
        ACHIEVEMENT_TYPE_OWN_EQUIP_BLUE_CNT         = 12,
        // ӵ����ɫװ������
        ACHIEVEMENT_TYPE_OWN_EQUIP_PURPLE_CNT       = 13,
        // ӵ�г�ɫװ������
        ACHIEVEMENT_TYPE_OWN_EQUIP_ORANGE_CNT       = 14,
        // ӵ�к�ɫװ������
        ACHIEVEMENT_TYPE_OWN_EQUIP_RED_CNT          = 15,
        // װ����ǿ������
        ACHIEVEMENT_TYPE_REFINE_EQUIP_CNT           = 16,
        // װ�����ǿ���ȼ�
        ACHIEVEMENT_TYPE_EQUIP_MAX_REFINE_LEVEL     = 17,
        // װ����߸�ħ�ȼ�
        ACHIEVEMENT_TYPE_ONE_EQUIP_ENCHANT_LEVEL    = 18,
        // ����װ���ﵽ��ħ�ȼ�
        ACHIEVEMENT_TYPE_TWO_EQUIP_ENCHANT_LEVEL    = 19,
        // ���װ���ﵽ��ħ�ȼ�
        ACHIEVEMENT_TYPE_FIVE_EQUIP_ENCHANT_LEVEL   = 20,
        // ʮ��װ���ﵽ��ħ�ȼ�
        ACHIEVEMENT_TYPE_TEN_EQUIP_ENCHANT_LEVEL    = 21,
        // �ϳ�װ������
        ACHIEVEMENT_TYPE_COMBINE_EQUIP_CNT          = 22,
        // ���޴ﵽ��ߵȼ�
        ACHIEVEMENT_TYPE_PET_LEVEL                  = 23,
        // ���޴ﵽ����Ǽ�
        ACHIEVEMENT_TYPE_PET_STAR_LEVEL             = 24,
        // ����ϴ�츳����
        ACHIEVEMENT_TYPE_PET_RESET_TALENT_CNT       = 25,
        
        // ��ͨ�ؿ�ͨ���½�
        ACHIEVEMENT_TYPE_CLEAR_NORMAL_TOLLGATE_CNT  = 51,
        // ��Ӣ�ܿ�ͨ���½�
        ACHIEVEMENT_TYPE_CLEAR_HARD_TOLLGATE_CNT    = 52,
        // ����Ǽ�����������
        ACHIEVEMENT_TYPE_GET_TOLLGATE_STAR_CNT      = 53,
        // �������ܲ������
        ACHIEVEMENT_TYPE_JION_ARENA_CNT             = 54,
        // ��������ʤ����
        ACHIEVEMENT_TYPE_ARENA_WIN_STREAK_CNT       = 55,
        // �ƽ�ð�����˺�
        ACHIEVEMENT_TYPE_GOLD_ISLAND_TOTAL_DAMAGE   = 56,
        // ��ħ��ʦð���״�ͨ��
        ACHIEVEMENT_TYPE_DSMX_FIRST_CLEAR           = 57,
        // �������ð���״�ͨ��
        ACHIEVEMENT_TYPE_SSLD_FIRST_CLEAR           = 58,
        // ���֮��ð���״�ͨ��
        ACHIEVEMENT_TYPE_MFZD_FIRST_CLEAR           = 59,
        // �ͱ���ð��ͨ�ز���
        ACHIEVEMENT_TYPE_BABEL_TOWER_CLEAR_FLOOR    = 60,
        // ���ط�ӡͨ���½�
        ACHIEVEMENT_TYPE_BZFY_CLEAR_TOLLGATE_CNT    = 61,

        // �ܳ鿨����
        ACHIEVEMENT_TYPE_DRAW_CARD_CNT              = 101,
        // ʮ�������
        ACHIEVEMENT_TYPE_TEN_DRAW_CARD_CNT          = 102,
        // ���ﵽ�ȼ�
        ACHIEVEMENT_TYPE_GOLD_MINE_LEVEL            = 103,
        // ��ʯ��ﵽ�ȼ�
        ACHIEVEMENT_TYPE_DIAMOND_MINE_LEVEL         = 104,
        // ħ��֮Ȫ�򵽵ȼ�
        ACHIEVEMENT_TYPE_EXP_SPRING_LEVEL           = 105,
        // �ۼƻ�ý��
        ACHIEVEMENT_TYPE_GET_TOTAL_GOLD             = 106,
        // �ۼƻ����ʯ
        ACHIEVEMENT_TYPE_GET_TOTAL_DIAMOND          = 107,
        // �ۼƻ������
        ACHIEVEMENT_TYPE_GET_TOTAL_SOUL             = 108,
        // �ۼƻ������
        ACHIEVEMENT_TYPE_GET_TOTAL_HONOR            = 109,
        // ͼ����ʥ����Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_HOLY_CNT   = 110,
        // ͼ���ڰ�����Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_DARK_CNT   = 111,
        // ͼ����Ȼ����Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_NATURE_CNT = 112,
        // ͼ��������Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_FIRE_CNT   = 113,
        // ͼ��������Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_THUNDER_CNT    = 114,
        // ͼ��ˮ����Ӣ�۴ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_HERO_WATER_CNT      = 115,
        // ͼ������װ���ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_WEAPON_CNT    = 116,
        // ͼ��ͷ��װ���ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_HAT_CNT       = 117,
        // ͼ������װ���ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ARMOR_CNT     = 118,
        // ͼ������װ���ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_CUISH_CNT     = 119,
        // ͼ����Ʒװ���ﵽ����
        ACHIEVEMENT_TYPE_ILLUSTRATE_EQUIP_ORNAMENT_CNT  = 120,
        // ӵ�к��Ѹ���
        ACHIEVEMENT_TYPE_BUDDY_CNT                  = 121,
        // ������������
        ACHIEVEMENT_TYPE_SENDOUT_ENERGY_CNT         = 122,
        // ������ߴ�������
        ACHIEVEMENT_TYPE_OWN_GUILD                  = 123,
    };

    enum AchievementStatus
    {
        // �ɾ�δ���
        ACHIEVEMENT_STATUS_UNDERWAY                 = 0,
        // �ɾʹ�����콱
        ACHIEVEMENT_STATUS_COMPLETE                 = 1,
        // �ɾʹ��δ�콱
        ACHIEVEMENT_STATUS_ACCEPT_AWARD             = 2,
    };

    enum TowerBufferType
    {
        TOWER_BUFFER_HP                  = 1,
        TOWER_BUFFER_ATTACK              = 2,
        TOWER_BUFFER_CRITICAL            = 3,
        TOWER_BUFFER_RESCRIT             = 4,
        TOWER_BUFFER_HIT                 = 5,
        TOWER_BUFFER_DODGE               = 6,
        TOWER_BUFFER_DAMAGE_RED          = 7,
    };

    enum TowerPassConditionType
    {
        TOWER_PASS_CONDITION_WIN             = 1,
        TOWER_PASS_CONDITION_WIN_IN_ROUND    = 2,
        TOWER_PASS_CONDITION_DEATH_COUNT     = 3,
        TOWER_PASS_CONDITION_REMAIN_HP       = 4,
    };

    // ���ÿ�����
    const UInt32 MITERAL_GOLD_OUTPUT_PERSEC         = 100;
    // ��ʯÿ�����
    const UInt32 MITERAL_DIAMOND_OUTPUT_PERSEC      = 100;
    // Ȫˮ����ÿ�����
    const UInt32 SPRING_EXP_OUTPUT_PERSEC           = 100;
    // �ƹ�����ÿ�����
    const UInt32 TAVERN_ENERGY_OUTPUT_PERSEC        = 100;
    // �����ٱ���
    const float GOLD_SPEED_UP_LEVEL                 = 1.0;
    // ��ʯ���ٱ���
    const float DIAMOND_SPEED_UP_LEVEL              = 1.0;
    // Ȫˮ������ٱ���
    const float EXP_SPEED_UP_LEVEL                  = 1.0;
    // �ƹ��������ٱ���
    const float ENERGY_SPEED_UP_LEVEL               = 1.0;


    // �����������
    const UInt8 BAG_MAX_SIZE                        = 99;
    const UInt32 ITEM_MAX_COUNT                     = 999999;

    // Ӣ��װ�����λ��
    const UInt8 PLAYER_EQUIP_MAX_POSITION           = 25;
    // ����Ӣ��װ��������� 
    const UInt8 PLAYER_EQUIP_PER_HERO_MAX_COUNT     = 5;
    // ����װ�����λ��
    const UInt8 PLAYER_RUNE_MAX_POSITION            = 25;
    // ����Ӣ��װ�������������
    const UInt8 PLAYER_RUNE_PER_HERO_MAX_COUNT      = 5;
    // װ��ǿ�����ȼ�
    const UInt32 EQUIP_REFINE_MAX_LEVEL             = 160;
    // ��ҹؿ��������ֵ
    const UInt32 TOLLGATE_MAX_COUNT                 = 300;

    // �����̵�һ��ˢ�´���
    const UInt8 SECRET_STORE_RESET_TIMES_DAILY      = 12;
    // �����̵���Ʒ����
    const UInt8 SECRET_STORE_ITEM_MAX_COUNT         = 6;

    // ���а���ʾ��Ҹ���
    const UInt32 PLAYER_COUNT_ON_RANK               = 50;

    // ����������������
    const UInt32 ARENA_PLAYER_MAX_COUNT             = 100000;
    // ������������������
    //const UInt32 ARENA_ROBOT_MAX_COUNT              = 10000;
    const UInt32 ARENA_ROBOT_MAX_COUNT              = 19;
    // �����������ս����
    const UInt32 ARENA_BATTLE_MAX_COUNT             = 10;

    // �����������������
    const UInt32 RANDOM_FIRST_NAME_MAX_COUNT        = 10;
    // ���������������
    const UInt32 RANDOM_SECOND_NAME_MAX_COUNT       = 10;


    // ����ǩ����������
    const UInt8 CONTINOUS_LOGIN_AWARD_MAX_DATE_NUM  = 7;

    // �ؿ�ÿ��ˢ��ʱ��
    const UInt32 TOLLGATE_BATTLE_COUNT_RESET_TIME   = 3600 * 5;
    // ������ÿ��ˢ��ʱ��
    const UInt32 ARENA_BATTLE_COUNT_RESET_TIME      = 3600 * 22;
    // ͬһ���ͳɾ��������
    const UInt8 ACHIEVEMENT_TYPE_MAX_COUNT          = 50;
    // �ɾ��������
    const UInt8 ACHIEVEMENT_MAX_COUNT               = 150;
    // ����ÿ��ˢ��ʱ��
    const UInt32 STORE_HOUSE_BATTLE_COUNT_RESET_TIME   = 3600 * 5;

    // �����������ô���
    const UInt8 TOWER_RESET_MAX_COUNT               = 10;
    // ���������ˢ��ʱ��
    const UInt32 TOWER_RESET_TIME                   = 3600 * 5;
    // �������ܿ����ȼ�����
    const UInt8 TOWER_OPEN_LEVEL_LIMIT              = 1;
    // ����������
    const UInt8 TOWER_FLOOR_MAX_COUNT               = 30;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//���װ������������� 
#define MAXEQUIPSKILL 4
#define MAXSKILLLIST  10
#define MAXBUFFERCOUNT 20
//�����������
#define MAXITEMCOUNT   200
#define MAXCONSYMBOLCOUNT 8
//������ʱװ����
#define MAXFASHIONCOUNT  10

//���װ��Я�������ʯ����

#define MAXGEMCOUNT  5

//װ�����Ĺ�����������
#define MAXINTRINSICATTRCOUNT  4

//װ�����������������
#define MAXRANDOMATTRCOUNT  5

//��Ʒ�Ĳ��������С

#define PARAMSIZE 8

//ÿһ�½����ؿ���
#define CHAPTERSTAGECOUNT 20
//ǿ�ƶϿ��ͻ��˴�����ʱ���� 100*30s
#define KEEP_ALIVE_COUNT  40

enum ItemType
{
	EQUIPTYPEID = 1, 
	GEMTYPEID = 3,
	ENHANCETYPEID = 30003,
	COSTTYPEID = 4,
	SYMBOLTYPEID =5,
	TREASUREBOXTYPEID = 6,
	PRESENTPACKAGETYPEID = 7,
	MEDICINETYPEID = 8,
	EQUIPPIECETYPEID = 9,
	HOARSTONEPIECETYPEID = 10,
	OTHERTYPEID = 11
};

enum gemSubType
{
	SubTypeRed = 1,
	SubTypeYellow = 2,
	SubTypeGreen = 3,
	SubTypeBlue = 4,
	SubTypePurple = 5
};

enum CostSubType
{
	REFRESHSTONE = 1, //װ��ϴ��ʯ
	SWEEPPAPER   = 2//ɨ����
};

#define PolishStoneID 30004401
#define PolishNeedCoin 10



enum AttributeType 
{
	////max hp
	MaxHP 				= 1,
	////max mp
	MaxMP 				= 2,
	////max sp
	MaxSP				= 3,
	////������
	PA 					= 4,
	////�������
	MA					= 5,
	////��������
	PF 					= 6,
	////��������
	MF 					= 7,
	////hp�ظ��ٶ�
	HPRecover			= 8,
	////mp�ظ��ٶ�
	MPRecover			= 9,
	////sp�ظ��ٶ�
	SPRecover 			= 10,
	////�����ٶ�
	AttSpeed 			= 11,
	////���м��
	CombinationAtt 		= 12,
	////�ƶ��ٶ�
	MoveSpeed 			= 13,
	////����߶�
	JumpHeight 			= 14,
	////���������մ���
	FlyTimes 			= 15,
	////sp����
	SPDamage			= 16,
	////����
	AttPush 			= 17,
	////���˷���
	AttPushDef 			= 18,
	////����
	AttRaise 			= 19,
	////�������
	AttraiseDef 		= 20,
	////������ֱ
	Stiff				= 21,
	////��ֱ����
	StiffDef 			= 22,
	////����������
	BaseCrit 			= 23,
	////��������
	Critrate 			= 24,
	////����
	BaseToughness 		= 25,
	////ѣ�η���(����)
	VertigoDef 			= 26,
	////ѣ�η���(ʱ��)
	VertigoTimeDef 		= 27,

	////���ٷ���(����)
	SlowDef 			= 28,
	////���ٷ���(ʱ��)
	SlowTimeDef 		= 29,
	////�ж�����
	PoisonDef 			= 30,
	////���˷���
	BloodDef 			= 31,
	////��ɱ����
	KODef 				= 32,
	////����
	FloatVal            = 33, 
	////���ɿ���
	Floatdef            = 34,
	////����
	Fall                = 35,
	////��������
	Falldef             = 36,

};

enum EquipGemSlotBit
{
	//װ����ʯ�ۣ���һ����ʯ��λ
	Gem1SlotBit = 0x00000001,
	//װ����ʯ�ۣ���2����ʯ��λ
	Gem2SlotBit = 0x00000002,
	//װ����ʯ�ۣ���3����ʯ��λ
	Gem3SlotBit = 0x00000004,
	//װ����ʯ�ۣ���4����ʯ��λ
	Gem4SlotBit = 0x00000008,
	//װ����ʯ�ۣ���5����ʯ��λ
	Gem5SlotBit = 0x00000010,
	//װ����ʯ�ۣ���6����ʯ��λ
	Gem6SlotBit = 0x00000020,

	AllGemLock = 0x00000000

};

enum JewelryOpenBit
{
	//��һ����Ʒ�����ı�־λ������0x00000001
	Jewelry1OpenBit = 0x00000001,
	//�ڶ�����Ʒ�����ı�־λ
	Jewelry2OpenBit = 0x00000002,
	//��������Ʒ������־λ
	Jewelry3OpenBit = 0x00000004,

	//���ĸ���Ʒ������־λ
	Jewelry4OpenBit = 0x00000008,

	//�������Ʒ������־λ
	Jewelry5OpenBit = 0x00000010,

	//��������Ʒ������־λ
	Jewelry6OpenBit = 0x00000020,

	AllJewelryLock = 0x00000000


};

enum BOCConditionType
{
	NeedCoin = 1,
	PlayerLv = 7,
	HoarStoneLv = 8
};

enum QualityType
{
	White = 1,
	Green = 2,
	Blue  = 3,
	Purple = 4,
	Orange = 5,
	Gold = 6
};

enum DirtyType
{
	NODIRTY = 0,
	ADDDIRTY = 1,
	UPDATEDIRTY = 2,
	DELDIRTY = 3
};


enum OperationType
{
	ADDOPERATION = 0,
	DELOPERATION = 1
};

enum DataIndex
{
	BASEDATAINDEX = 0,
	DAILYRESETINDEX = 1,
	SKILLDATAINDEX = 2,
	BUFFDATAINDEX = 3,
	ITEMDATAINDEX = 4,
	FASHIONDATAINDEX = 5,
	STAGEDATAINDEX = 6,
	RHYMEDATAINDEX = 7,
	HOARSTONEDATAINDEX = 8
};

enum DataBit
{
	BASEDATABIT = 0x00000001,
	DAILYRESETBIT = 0x00000002,
	SKILLDATABIT = 0x00000004,
	BUFFDATABIT = 0x00000008,
	ITEMDATABIT = 0x00000010,
	FASHIONDATABIT = 0x00000020,
	STAGEDATABIT = 0x00000040,
	RHYMEDATABIT = 0x00000080,
	HOARSTONEDATABIT = 0x00000100,
	MATERIALDATABIT =  0x00000200,
	HEROEQUIPDATABIT = 0x00000400,
	GEMDATABIT       = 0x00000800,
	JEWELRYDATABIT   = 0x00001000,
	LOCALDATABIT   = 0x00002000,
	CHAPTERUNLOCKEDTABIT   = 0x00004000,
	BOXCOUNTER   = 0x00008000,
	SERVANTFOODBIT = 0x00010000,
	SERVANTBATTLEBIT = 0x00020000,
	TOWERDATABIT = 0x00040000,
	CHATDATABIT =  0x00080000,
	FOODDATABIT =  0x00100000,
	STRENGTHDATABIT =  0x00200000,
	COUNTERDATABIT =  0x00400000,
	COURAGECHALLENGEDATABIT =  0x00800000,
	RANKGAMEDATABIT =  0x01000000,
	BUYCOINDATABIT =  0x02000000,
	FRIENDBEAPPLYDATABIT =  0x04000000,


	MAXBIT =  0x80000000
};

enum ServantInfoLock
{
	FIRSTLOCK = 0x00000001,
	SECONDLOCK = 0x00000002,
	THIRDLOCK = 0x00000004
};

enum ProcessState
{
	PROCESSFREE = 0,
	PROCESSBUSY = 1
};

// // buff������
enum BUFF_TYPE 
{
	BUFF_TYPE_ATTR  		= 1,	//����
	BUFF_TYPE_STATE  		= 2,	//�쳣״̬
	BUFF_TYPE_DISPLAY   	= 3 	//����
};
//�쳣״̬��buff������
enum BUFF_STATE_SUB_TYPE
{
	BUFF_STATE_SUB_TYPE_VERTIGO  	= 1,	//ѣ��
	BUFF_STATE_SUB_TYPE_SLOW  		= 2,	//����
	BUFF_STATE_SUB_TYPE_POISON  	= 3, 	//�ж�
	BUFF_STATE_SUB_TYPE_BLOOD  		= 4,	//����
	BUFF_STATE_SUB_TYPE_GOD  		= 5,	//�޵�
	BUFF_STATE_SUB_TYPE_ENDURE  	= 6,	//����
	BUFF_STATE_SUB_TYPE_MAX			= 7	//���ֵ
};

enum CHAR_STATE 
{
	CHAR_STATE_IDLE		=	1,		//����վ��
	CHAR_STATE_RUN		=	2,		//�ƶ�
	CHAR_STATE_JUMP		=	3,		//��Ծ
	CHAR_STATE_DIE		=	4,		//����
	CHAR_STATE_HURT		=	5,		//�ܻ�
	CHAR_STATE_FALL		=	6,		//����
	CHAR_STATE_VICTORY	=	7,		//ʤ��
	CHAR_STATE_ATTACK	=	8,		//����
	CHAR_STATE_SHOW		= 	9,		//չʾ
	CHAR_STATE_SHOWBACK	= 	10 		//չʾ����
};

enum GROW_TYPE
{
	GROW_TYPE_LINE		 = 1,			// y = ax + b
	GROW_TYPE_PARABOLIC  = 2		// y = ax^2 + bx + c
};



// #define  display_bottom = 0;
// #define GRUOND_Y = display_bottom + 130 		////�����Y����
// #define  SCENE_EDGEX = 1000//�ؿ�����
// #define  SCENE_EDGEY = 1000//�ؿ��߶�
// #define GRAVITY  -2580
// #define GravityDeclineRate  0.01
enum SingleDefine
{
	display_bottom = 0,
	GRUOND_Y =display_bottom + 130 ,
	 FIRST_POSX =  10,
	CHAR_SHADOW = 0,
	SCENE_EDGEX = 10000,
	SCENE_EDGEY = 10000,
	GRAVITY = -2580,
};
#define  FRAMES_PERSECOND = 30

#define PLAYER_TAG = 100

#define JOYSTICK_RADIUS = 35
#define JOYSTICK_LENGTH = 3 					//����ҡ�˴������� 3*JOYSTICK_RADIUS
#define TOPUI_ZORDER = 1000

#define DEFAULT_PLAYER_CAMP = 2

#define BAG_TABLEVIEW_NUM = 3

// #define CHAR_SHADOW = 0

enum CHAR_SHADOW_CONFIG 
{					////��Ӱ����
	CHAR_SHADOW_CONFIG_SCALEY  = 5000,            //           //Y������
	CHAR_SHADOW_CONFIG_OFFSETX  = 0,				//		//Xƫ����
	CHAR_SHADOW_CONFIG_OFFSETY = 0,					//	//Yƫ����
	CHAR_SHADOW_CONFIG_OPACITY = 75,			//			//͸����
	CHAR_SHADOW_CONFIG_SKEWX = 0,				//			//��б
	CHAR_SHADOW_CONFIG_ROTATE = 15				//			//��ת�Ƕ�
};
// 
// CHAR_ACTION_SHADOW_CONFIG {
// 	[1]		=	{	//idle			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[2]		=	{	//run			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[3]		=	{	//jump			
// 	offsetX = 0,
// 	offsetY = -200,
// 	rotate = 0
// 	};,
// 	[4]		=	{	//die			
// 	offsetX = 0,
// 	offsetY = -200,
// 	rotate = 0
// 	};,
// 	[5]		=	{	//hurt			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[6]		=	{	//fall		
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 0
// 	};,
// 	[7]		=	{	//victory			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[8]		=	{	//attack			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[9] 	= 	{	//show			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};,
// 	[10] 	= 	{	//showback			
// 	offsetX = 0,
// 	offsetY = 0,
// 	rotate = 15
// 	};
// };



enum CHAR_ATTR 
{
	CHAR_ATTR_MaxHP 				= 1,	//max hp
	CHAR_ATTR_MaxMP 				= 2,	//max mp
	CHAR_ATTR_MaxSP				= 3,	//max sp
	CHAR_ATTR_PA 					= 4,	//������
	CHAR_ATTR_MA 					= 5,	//�������
	CHAR_ATTR_PF 					= 6,	//��������
	CHAR_ATTR_MF 					= 7,	//��������
	CHAR_ATTR_HPRecover			= 8,    //hp�ظ��ٶ�
	CHAR_ATTR_MPRecover			= 9,    //mp�ظ��ٶ�
	CHAR_ATTR_SPRecover 			= 10,	//sp�ظ��ٶ�
	CHAR_ATTR_AttSpeed 			= 11,	//�����ٶ�
	CHAR_ATTR_CombinationAtt 		= 12,	//���м��
	CHAR_ATTR_speed 			= 13,	//�ƶ��ٶ�
	CHAR_ATTR_JumpHeight 			= 14,	//����߶�
	CHAR_ATTR_FlyTimes 			= 15,	//���������մ���
	CHAR_ATTR_SPDamage			= 16,	//sp����
	CHAR_ATTR_AttPush 			= 17,	//����
	CHAR_ATTR_AttPushDef 			= 18,	//���˷���
	CHAR_ATTR_AttRaise 			= 19,	//����
	CHAR_ATTR_AttraiseDef 		= 20,	//�������
	CHAR_ATTR_Stiff				= 21,	//������ֱ
	CHAR_ATTR_StiffDef 			= 22,	//��ֱ����
	CHAR_ATTR_BaseCrit 			= 23,	//����������
	CHAR_ATTR_Critrate 			= 24,	//��������
	CHAR_ATTR_BaseToughness 		= 25,	//����
	CHAR_ATTR_VertigoDef 			= 26,	//ѣ�η���(����)
	CHAR_ATTR_VertigoTimeDef 		= 27,	//ѣ�η���(ʱ��)
	CHAR_ATTR_SlowDef 			= 28,	//���ٷ���(����)
	CHAR_ATTR_SlowTimeDef 		= 29,	//���ٷ���(ʱ��)
	CHAR_ATTR_PoisonDef 			= 30,	//�ж�����
	CHAR_ATTR_BloodDef 			= 31,	//���˷���
	CHAR_ATTR_KODef 				= 32,	//��ɱ����
	CHAR_ATTR_FloatVal            = 33,   //����
	CHAR_ATTR_Floatdef            = 34,	//���ɿ���
	CHAR_ATTR_Fall                = 35,	//����
	CHAR_ATTR_Falldef             = 36	//��������
};

enum ECHAR_ATTR_KEY 
{
// 	MaxHP			= "MaxHP",		//max hp
// 	MaxMP 			= "MaxMP",		//max mp
// 	MaxSP 			= "MaxSP",		//max sp
// 	PA 				= "PA",			//������
// 	MA 				= "MA",			//�������
// 	PF 				= "PF",			//��������
// 	MF 				= "MF",			//��������
// 	HPRecover 		= "HPRecover",  //hp�ظ��ٶ�
// 	MPRecover 		= "MPRecover",  //mp�ظ��ٶ�
// 	SPRecover 		= "SPRecover",	//sp�ظ��ٶ�
// 	AttSpeed 		= "AttSpeed",	//�����ٶ�
// 	CombinationAtt 	= "CombinationAtt",	//���м��
// 	MoveSpeed 		= "MoveSpeed",	//�ƶ��ٶ�
// 	JumpHeight 		= "JumpHeight",	//����߶�
// 	FlyTimes 		= "FlyTimes",	//�ɻ��ɴ���
// 	SPDamage 		= "SPDamage",	//sp����
// 	AttPush 		= "AttPush",	//����
// 	AttPushDef 		= "AttPushDef",	//���˷���
// 	AttRaise 		= "AttRaise",	//����
// 	AttraiseDef 	= "AttraiseDef",//�������
// 	Stiff 			= "Stiff",		//������ֱ
// 	StiffDef 		= "StiffDef",	//��ֱ����
// 	BaseCrit 		= "BaseCrit",	//����������
// 	Critrate 		= "Critrate",	//��������
// 	BaseToughness 	= "BaseToughness",	//����
// 	VertigoDef 		= "VertigoDef",		//ѣ�η���(����)
// 	VertigoTimeDef 	= "VertigoTimeDef",	//ѣ�η���(ʱ��)
// 	SlowDef 		= "SlowDef",	//���ٷ���(����)
// 	SlowTimeDef 	= "SlowTimeDef",	//���ٷ���(ʱ��)
// 	PoisonDef 		= "PoisonDef",	//�ж�����
// 	BloodDef 		= "BloodDef",	//���˷���
// 	KODef 			= "KODef", 		//��ɱ����
// 	FloatVal 		= "FloatVal",   //����
// 	Floatdef 		= "Floatdef",	//���ɿ���
// 	Fall 			= "Fall",		//����
// 	Falldef 		= "Falldef",	//��������
// 	[1]		= "MaxHP",			//max hp
// 	[2]		= "MaxMP",			//max mp
// 	[3]		= "MaxSP",			//max sp
// 	[4]		= "PA",				//������
// 	[5]		= "MA",				//�������
// 	[6]		= "PF",				//��������
// 	[7]		= "MF",				//��������
// 	[8]		= "HPRecover",    	//hp�ظ��ٶ�
// 	[9]		= "MPRecover",    	//mp�ظ��ٶ�
// 	[10]	= "SPRecover",		//sp�ظ��ٶ�
// 	[11]	= "AttSpeed",		//�����ٶ�
// 	[12]	= "CombinationAtt",	//���м��
// 	[13]	= "MoveSpeed",		//�ƶ��ٶ�
// 	[14]	= "JumpHeight",		//����߶�
// 	[15]	= "FlyTimes",		//�ɻ��ɴ���
// 	[16]	= "SPDamage",		//sp����
// 	[17]	= "AttPush",		//����
// 	[18]	= "AttPushDef",		//���˷���
// 	[19]	= "AttRaise",		//����
// 	[20]	= "AttraiseDef",	//�������
// 	[21]	= "Stiff",			//������ֱ
// 	[22]	= "StiffDef",		//��ֱ����
// 	[23]	= "BaseCrit",		//����������
// 	[24]	= "Critrate",		//��������
// 	[25]	= "BaseToughness",	//����
// 	[26]	= "VertigoDef",		//ѣ�η���(����)
// 	[27]	= "VertigoTimeDef",	//ѣ�η���(ʱ��)
// 	[28]	= "SlowDef",		//���ٷ���(����)
// 	[29]	= "SlowTimeDef",	//���ٷ���(ʱ��)
// 	[30]	= "PoisonDef",		//�ж�����
// 	[31]	= "BloodDef",		//���˷���
// 	[32]	= "KODef", 		    //��ɱ����
// 	[33]	= "FloatVal",   	//����
// 	[34]	= "Floatdef",		//���ɿ���
// 	[35]	= "Fall",			//����
// 	[36]	= "Falldef",		//��������
};

// CHAR_ATTR_CHS_STR {
// 	[1]		= "��������",
// 	[2]		= "��������",
// 	[3]		= "��������",
// 	[4]		= "������",
// 	[5]		= "�������",
// 	[6]		= "��������",
// 	[7]		= "��������",
// 	[8]		= "HP�ظ��ٶ�",
// 	[9]		= "MP�ظ��ٶ�",
// 	[10]	= "SP�ظ��ٶ�",
// 	[11]	= "�����ٶ�",
// 	[12]	= "���м��",
// 	[13]	= "�ƶ��ٶ�",
// 	[14]	= "����߶�",
// 	[15]	= "���������մ���",
// 	[16]	= "SP����",
// 	[17]	= "����",
// 	[18]	= "���˷���",
// 	[19]	= "����",
// 	[20]	= "�������",
// 	[21]	= "������ֱ",
// 	[22]	= "��ֱ����",
// 	[23]	= "����������",
// 	[24]	= "��������",
// 	[25]	= "����",
// 	[26]	= "ѣ�η���(����)",
// 	[27]	= "ѣ�η���(ʱ��)",
// 	[28]	= "���ٷ���(����)",
// 	[29]	= "���ٷ���(ʱ��)",
// 	[30]	= "�ж�����",
// 	[31]	= "���˷���",
// 	[32]	= "��ɱ����",
// 	[33]	= "����",
// 	[34]	= "���ɿ���",
// 	[35]	= "����",
// 	[36]	= "��������",
// };

#define  ATTRIBUTE_COUNT = 36	//��������

//��ɫ����
enum CHARACTER_TYPE 
{
	CHARACTER_TYPE_Player 		= 1,	//���
	CHARACTER_TYPE_Monster 	= 2,	//��
	CHARACTER_TYPE_NPC 		= 3,	//npc
};

//��������
enum MONSTER_TYPE 
{
	MONSTER_TYPE_Normal 		= 1,	//��ͨ��
	MONSTER_TYPE_Elite 		= 2,	//��Ӣ��
	MONSTER_TYPE_Boss 		= 3,	//boss
	MONSTER_TYPE_Goblin 		= 4		//�粼��
};

//��������
enum SKILL_TYPE
{
	SKILL_TYPE_Normal 		= 1,	//��ͨ
	SKILL_TYPE_Passive 	= 2,	//����
	SKILL_TYPE_Multi 		= 3,	//���
	SKILL_TYPE_Cumulate 	= 4,	//����
	SKILL_TYPE_Continue 	= 5,	//����
	SKILL_TYPE_Throw 		= 6,	//Ͷ��
	SKILL_TYPE_RaiseUp 	= 7,	//����
	SKILL_TYPE_FallDown 	= 8,	//��ѹ
};

//�˺�����
enum HURT_TYPE 
{
	HURT_TYPE_Default 	= 1,	//Ĭ��
};





//�ӳɷ�ʽ
enum VALUE_EFFECT_TYPE 
{
	VALUE_EFFECT_TYPE_numval = 1,		//��ֵ�ӳ�
	VALUE_EFFECT_TYPE_numrate = 2,	//���ٷֱȼӳ�
};

//buff ��������������
#define  MAX_BUFF_CONFIG_PARAM_NUM = 8

//buff end

// enum  GROW_TYPE 
// {
// 	GROW_TYPE_line = 1,			// y = ax + b
// 	GROW_TYPE_parabolic = 2,		// y = ax^2 + bx + c
// };


enum CHAR_NODE_PRIORITY
{
	CHAR_NODE_PRIORITY_CHAR 		= 1,
	CHAR_NODE_PRIORITY_WEAPON_L 	= 2,
	CHAR_NODE_PRIORITY_FASHION 	= 3,
	CHAR_NODE_PRIORITY_WEAPON_R 	= 4,
	CHAR_NODE_PRIORITY_EFFECT 		= 5,
};
#define CHAR_NODE_PIC_CNT = 5	// CHAR_NODE_PRIORITY������

enum RES_PREFIX_TBL 
{
// 	[1] = "_char",
// 	[2] = "_Lweapon",
// 	[3] = "_clothes",
// 	[4] = "_Rweapon", 
// 	[5] = "_effect", 
};

//���ɹ���״̬
enum CHARACTER_FLYING_STATE 
{
	CHARACTER_FLYING_STATE_none 		= 0,		//û�л���
	CHARACTER_FLYING_STATE_startfly 	= 1,		//��ʼ��
	CHARACTER_FLYING_STATE_touchwall 	= 2,		//ײǽ����
	CHARACTER_FLYING_STATE_fallground 	= 3 		//�����󳯵�������
};

//�ӵ�����
enum BULLET_TYPE 
{
	BULLET_TYPE_line	= 1,	
	BULLET_TYPE_object	= 2,
};
enum BULLET_DESTORY_TYPE 
{
	BULLET_DESTORY_TYPE_none = 0,
	BULLET_DESTORY_TYPE_outScreen = 1 , //������Ļ�� 
	BULLET_DESTORY_TYPE_stop = 2 ,      //�����ƶ�, 
	BULLET_DESTORY_TYPE_after = 3 ,     //����һ��ʱ��,
	BULLET_DESTORY_TYPE_trigger = 4,    //��ײ��
	BULLET_DESTORY_TYPE_stopAter = 5,   //ֹͣ�ƶ�һ��ʱ��
	BULLET_DESTORY_TYPE_senderGone=6    //�����ӵ��ı�������
};


//����AI�¼�
enum SCENEAI_CONDITION_TYPE 
{
	//�ֹ����
	SCENEAI_CONDITION_TYPE_monster_seed 			= 1,

	//time
	SCENEAI_CONDITION_TYPEtime_after 				= 101,  	//���֮��
	SCENEAI_CONDITION_TYPEtime_each 				= 102,		//ÿ�����
	SCENEAI_CONDITION_TYPEhero_time_use 			= 103, 		//�೤ʱ��֮��
	//position
	SCENEAI_CONDITION_TYPEpos_pass_id 			= 121, 		//ĳ��λ����ĳ��λ��
	SCENEAI_CONDITION_TYPEpos_pass_camp 			= 122,
	SCENEAI_CONDITION_TYPEpos_between 			= 123,

	//attr
	SCENEAI_CONDITION_TYPEattr_atValue_id 		= 141,
	SCENEAI_CONDITION_TYPEattr_changeValue_id 	= 142,
	SCENEAI_CONDITION_TYPEattr_atPercent_id 		= 143,
	SCENEAI_CONDITION_TYPEattr_changePercent_id 	= 144,
	SCENEAI_CONDITION_TYPEattr_val_camp 			= 145, 		//ĳ��λĳ���Դ��ڡ����ڡ�С��ĳֵ/�ٷֱ�
	SCENEAI_CONDITION_TYPEhero_hp_camp 			= 146,		//hero Hp����ĳֵʱ����
	//state
	SCENEAI_CONDITION_TYPEstate_die_id 			= 161,
	SCENEAI_CONDITION_TYPEstate_buff_id 			= 162,
	SCENEAI_CONDITION_TYPEunit_dead 				= 163, 		//ĳ����λ����
	SCENEAI_CONDITION_TYPEhero_relive_times 		= 164, 		//��Ҹ����������ĳ��
	//action
	SCENEAI_CONDITION_TYPEaction_start_id 		= 181,
	SCENEAI_CONDITION_TYPEaction_end_id 			= 182, 		//ĳ����������
	SCENEAI_CONDITION_TYPEaction_end_scene 		= 183,

	//num_camp
	SCENEAI_CONDITION_TYPEmonster_num_camp 		= 191, 		//monster�������ڡ����ڡ�С��ĳֵ

	// unit_distance
	SCENEAI_CONDITION_TYPEunit_distance 			= 151, 		//������λ֮�����

	//chapter_progress
	SCENEAI_CONDITION_TYPEchapter_progress 		= 171, 		//�ؿ�����
};

enum SCENEAI_EVENT_TYPE 
{
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
	bg_monster_move 	= 124, 			//monster�ڱ������ƶ�
	bg_monster_animation = 125, 		//monster�ڱ����㲥���ض�����
	bg_create_monster 	= 126, 			//�ӱ������ֹ�
	active_monster 		= 127, 			//����monster
};


enum SCENEAI_UNIT_ACTION_TYPE
{
	fall2up	    		=	3, //��������
	airhurt	    		=	4, //�����ܻ�
	airfall	    		=	5, //��������
	flyback2up			=	6, //����������
	hurt	    		= 	8, //����ܻ�������hurtbefore��
};


// ��ɫ״̬
// enum CHAR_STATE 
// {
// 	idle		=	1,		//����վ��
// 	run			=	2,		//�ƶ�
// 	jump		=	3,		//��Ծ
// 	die			=	4,		//����
// 	hurt		=	5,		//�ܻ�
// 	fall		=	6,		//����
// 	victory		=	7,		//ʤ��
// 	attack		=	8,		//����
// 	show 		= 	9,		//չʾ
// 	showback 	= 	10, 	//չʾ����
// };

//��ɫai״̬
enum AI_STATE 
{
	AI_STATE_none        =   0,      //
	AI_STATE_idle		=	1,		//����
	AI_STATE_fan			=	2,		//����
	AI_STATE_walk		=	3,		//Ѱ·
	AI_STATE_escape		=	4,		//����
	AI_STATE_follow		=	5,		//����
	AI_STATE_patrol		=	6,		//Ѳ��
	AI_STATE_attack		=	7,		//����
	AI_STATE_wander		=	8,		//�ǻ�
	AI_STATE_hide 		=   9,  	//���
	AI_STATE_show 		=   10,		//չʾ
	AI_STATE_fall 		= 	11, 	//����
	AI_STATE_fixedwalk 	= 	12, 	//��̶���move
};

//����֡�¼�
enum FRAME_EVENT_TYPE
{
	FRAME_EVENT_TYPE_move 		=	1,		//�ƶ�
	FRAME_EVENT_TYPE_wait		=	2,		//�ȴ�
	FRAME_EVENT_TYPE_shake		=	3,		//��
	FRAME_EVENT_TYPE_attack 		=	4,		//��������һ֡��
	FRAME_EVENT_TYPE_attackKeep 			=	5,	//������֡���䣩
	FRAME_EVENT_TYPE_mulitCheck_start	=	7,		//�μ��������ж����俪ʼ
	FRAME_EVENT_TYPE_mulitCheck_end		=	8,		//�μ��������ж���������������ж�
	FRAME_EVENT_TYPE_addBuff 	= 	9,		//����buff
	FRAME_EVENT_TYPE_addBullet	=	10,		//�����ӵ�
	FRAME_EVENT_TYPE_addMusicEffect		=	11,		//������Ч
	FRAME_EVENT_TYPE_addAniEffect		=	12,		//������Ч
	FRAME_EVENT_TYPE_allowInterrupt		=	13,		//�ڴ�֡�¼����������������ϼ���
	FRAME_EVENT_TYPE_ctrlDirect_start	=	14,		//������Ƽ��ܷ���ʼ
	FRAME_EVENT_TYPE_ctrlDirect_end 		=	15,		//������Ƽ��ܷ������
};

enum JoystickDir 
{
	JOYSTICKDIR_UP = 1,
	JOYSTICKDIR_down = 2,
	JOYSTICKDIR_right = 3,
	JOYSTICKDIR_left = 4,
	JOYSTICKDIR_stop = 5,

};
enum ButtonClick 
{
	ButtonClick_skill1 = 1,
	ButtonClick_skill2 = 2,
	ButtonClick_soldierBtn = 3,
	ButtonClick_attack = 4,
	ButtonClick_pause = 5,
	ButtonClick_jump = 6,
	ButtonClick_direction = 7,
};
enum SkillEquip
{
	SkillEquip_normal_attack = 1,
	SkillEquip_jump_attack =2,
	SkillEquip_up =3,
	SkillEquip_down = 4,

};

//�ȽϷ���
enum CompareSign 
{
	CompareSign_eq = 1, // -- ����
	CompareSign_rt = 2, // -- ����
	CompareSign_lt = 3,//-- С��
	CompareSign_rte = 4, //-- ���ڵ���
	CompareSign_lte = 5, // -- С�ڵ���
// 	CompareSign_ = "+", //--����
// 	CompareSign_dec = "-", //--�ݼ�
};

//����ʱ �������ߵ�״̬
//0:վ������,1:��������,2:���ڵ�,3:����
enum BeHurtedState 
{
	BeHurtedState_stand = 0,
	BeHurtedState_float = 1, 
	BeHurtedState_fall = 2, 
	BeHurtedState_floatX = 3,
};

enum ANI_KEY_NAME 
{
// 	idle = "idle",
// 	run = "run",
// 	airhurt = "airhurt",
// 	airfall = "airfall",
// 	fall = "fall",
// 	fall2up = "fall2up",
// 	victory = "victory",
// 	die = "die",
// 	flyback = "flyback",
// 	show = "show",
// 	flybackfall = "flybackfall",
// 	flyback2up = "flyback2up",
// 	jumpbefore = "jumpbefore", 
// 	jumpup = "jumpup", 
// 	jumpdown = "jumpdown", 
// 	jumpafter = "jumpafter",
// 	showback = "showback",
// 	showstart = "showstart",
// 	escapewin = "escapewin",
// 	escapelose = "escapelose",
};

//��Դ����
enum GAME_RES_TYPE 
{
	GAME_RES_TYPE_coin 		= 1,
	GAME_RES_TYPE_rmb 		= 2,
	GAME_RES_TYPE_strength 	= 3,
	GAME_RES_TYPE_exp 		= 4,
	GAME_RES_TYPE_prestige 	= 5,
	GAME_RES_TYPE_item 		= 6,
};

//�ؿ���������
enum STAGE_ACTIVE_CONDITION 
{
	STAGE_ACTIVE_CONDITION_level 			= 1,
	STAGE_ACTIVE_CONDITION_preFinishStage 	= 2,
	STAGE_ACTIVE_CONDITION_mission 		= 3,
	STAGE_ACTIVE_CONDITION_preFinishStageStarCnt = 4,
	STAGE_ACTIVE_CONDITION_vipLevel		= 5,
	STAGE_ACTIVE_CONDITION_guildLevel		= 6,
};

//�ؿ�����
enum STAGE_TYPE 
{
	STAGE_TYPE_normal = 1,
	STAGE_TYPE_master = 2,
	STAGE_TYPE_activity = 3,
// 
// 	[1] = "normal",
// 	[2] = "master",
// 	[3] = "activity",
};

//�ؿ���������
enum STAGE_STAR_CONDITION 
{
	STAGE_STAR_CONDITION_useTime 			= 1,
	STAGE_STAR_CONDITION_reviveTimes 		= 2,  //�������
	STAGE_STAR_CONDITION_useMedicinetimes	= 3,
	STAGE_STAR_CONDITION_leftHpValue			= 4,
	STAGE_STAR_CONDITION_leftHpRate			= 5,
};
//�ؿ�������
#define STAGE_STAR_NUMBER = 3

//ÿ�½����ؿ�����
#define CHAPTER_MAX_STAGE_NUMBER = 20

//ɨ����������
#define SWEEP_ITEM_TYPE_NUM = 3

//����ÿ�еı�������
#define ITEM_CNT_EACH_ROW = 4

//��������
enum ITEM_TYPE 
{
	ITEM_TYPE_equip 			= 1,		//װ��
	ITEM_TYPE_gem 			= 3,		//��ʯ
	ITEM_TYPE_enhance		 	= 30003,	//ǿ��ʯ
	ITEM_TYPE_cost 			= 4, 		//��Ϸ����Ʒ
	ITEM_TYPE_treasureBox 	= 6,		//����
	ITEM_TYPE_presentpackage 	= 7,		//���
	ITEM_TYPE_medicine 		= 8,		//ҩƷ�ָ������
	ITEM_TYPE_equip_piece 	= 9,		//װ����Ƭ
	ITEM_TYPE_material		= 10,		//����
	ITEM_TYPE_other 			= 11,		//������Ϸ����
};

//��ʯ������
enum ITEM_GEM_SUBTYPE 
{
	ITEM_GEM_SUBTYPE_red 	= 1,	//��
	ITEM_GEM_SUBTYPE_yellow 	= 2,	//��
	ITEM_GEM_SUBTYPE_green 	= 3,	//��
	ITEM_GEM_SUBTYPE_blue 	= 4,	//��
	ITEM_GEM_SUBTYPE_purple	= 5,	//��
};

//ǿ��ʯ������
enum ITEM_ENHANCE_MINITYPE {
	ITEM_ENHANCE_MINITYPE_weapon 	= 1,	//����ǿ��ʯ	
	ITEM_ENHANCE_MINITYPE_defence = 2,	//����ǿ��ʯ
	ITEM_ENHANCE_MINITYPE_jewelry = 3, 	//��Ʒǿ��ʯ
};

//��Ϸ����Ʒ������
enum ITEM_COST_SUBTYPE
{
	ITEM_COST_SUBTYPE_enhance 		= 5,		//װ��ǿ��ʯ
	ITEM_COST_SUBTYPE_remeltStone 	= 1,		//װ��ϴ��ʯ
	ITEM_COST_SUBTYPE_sweepPaper		= 2,		//ɨ����
	// rhymeSoul 		= 3,		//�ϻ�
	ITEM_COST_SUBTYPE_dungeonTicket	= 4,		//�����볡ȯ
};

//ҩƷ�ָ������������
enum ITEM_MEDICINE_SUBTYPE
{
// 	1
};

//������Ϸ����������
enum ITEM_OTHER_SUBTYPE 
{
	key_gold	= 1,	//��Կ��
	key_silver	= 2,	//��Կ��
};

//����������
enum ITEM_TREASURE_MINITYPE
{
	ITEM_TREASURE_MINITYPE_common_gold		= 100,
	ITEM_TREASURE_MINITYPE_common_silver	= 101,
	ITEM_TREASURE_MINITYPE_common_cu		= 102,
	ITEM_TREASURE_MINITYPE_common_fe		= 103,
	ITEM_TREASURE_MINITYPE_weapon_lv_first = 200,
	ITEM_TREASURE_MINITYPE_weapon_lv_end	= 209,
	ITEM_TREASURE_MINITYPE_defence 	= 300,
	ITEM_TREASURE_MINITYPE_jewelry 	= 400,
	ITEM_TREASURE_MINITYPE_gem 		= 500,
	ITEM_TREASURE_MINITYPE_random 		= 600,
};

//���������
enum ITEM_PACKAGE_MINITYPE 
{

};

//����Ʒ��
enum ITEM_QUALITY 
{
	ITEM_QUALITY_white 	= 1,	//��
	ITEM_QUALITY_green 	= 2,	//��
	ITEM_QUALITY_blue 	= 3,	//��
	ITEM_QUALITY_purple 	= 4,	//��
	ITEM_QUALITY_golden 	= 5,	//��
};

//װ����
enum CHARACTER_EQUIP_POS 

{
	CHARACTER_EQUIP_POS_weapon 	= 1,	//����
	CHARACTER_EQUIP_POS_head	= 2,	//ͷ
	CHARACTER_EQUIP_POS_chest 	= 3,	//��
	CHARACTER_EQUIP_POS_belt 	= 4,	//����
	CHARACTER_EQUIP_POS_leg 	= 5,	//��
	CHARACTER_EQUIP_POS_foot 	= 6,	//��
	CHARACTER_EQUIP_POS_jewelry = 7,	//����
	CHARACTER_EQUIP_POS_amulet 	= 8,	//����
};

//װ������
enum EQUIP_TYPE 
{
// 	[1] = 1, 		//����
// 	[2] = 2,		//����
// 	[3] = 2,
// 	[4] = 2,
// 	[5] = 2,
// 	[6] = 2,
// 	[7] = 3,		//��Ʒ
// 	[8] = 3
};

//װ������Ǽ�
#define MAX_EQUIP_STAR_LEVEL = 5

//���ߵĲ�����������
#define ITEM_PARAM_NUM = 8

//��������
enum BAG_TAB_TYPE 

{
	BAG_TAB_TYPE_equip = 1,
	BAG_TAB_TYPE_item  = 2,
	BAG_TAB_TYPE_other = 3,
// 	[1] = "equip",
// 	[2] = "item",
// 	[3] = "other",
};

// �ؿ�����
enum SELECT_TAG_TYPE

{
	SELECT_TAG_TYPE_normal = 1,
	SELECT_TAG_TYPE_master = 2,
// 	[1] = "normal",
// 	[2] = "master",
};
//�ؿ�ɨ������
enum SELECT_SWEEP_PROPERTY 
{
	SELECT_SWEEP_PROPERTY_cunzheng = 1,                //����
	SELECT_SWEEP_PROPERTY_power    = 2,                //С����ҩ
	SELECT_SWEEP_PROPERTY_gem      = 3,                //��ʯ
// 	[1]      = "cunzheng",
// 	[2]      = "power",
// 	[3]      = "gem",
};
//��ʳ�ݿ���״̬
#define FoodOpenPosition = false
//ʱװ�����Ч����������
#define MAX_FASHION_EFFECTIVE_ATTR_COUNT = 5

//��ʯ��������
#define HOARSTONE_TYPE_NUM = 12
//��ʯ����Ǽ�
#define HOARSTONE_MAX_STAR = 10
//��ʯװ�������ģ�װ������
#define HOARSTONE_EQUIP_SLOT_NUM = 6

//����������
#define RHYME_MAX_STEP_COUNT = 10
//ÿ����Ϸ��ɫ���Ϲ�����
#define GAMECHARACTER_RHYME_MAX_SKILL_COUNT = 3
//ÿ���������Ѩ����
#define STEP_MAX_ACUPOINT_COUNT = 20


//�ճ��ؿ�����
enum DAILY_DUNGEON_TYPE
{
	DAILY_DUNGEON_TYPE_coin 	= 1,
	DAILY_DUNGEON_TYPE_exp		= 2,
	DAILY_DUNGEON_TYPE_gem 	= 3,
	DAILY_DUNGEON_TYPE_rhyme 	= 4
};
//�ճ��ؿ���������
#define DAILY_DUNGEON_TYPE_COUNT = table.nums(DAILY_DUNGEON_TYPE)
//�ճ��ؿ��Ѷ�����
#define DAILY_DUNGEON_MAX_DIFFICULT = 6

//��Ӧ��Դ��
enum colorRes 
{
// 	ResourceDef.IMAGE_BAG_BTNFRAME_WHITE, 
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_BLUE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_PURPLE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GRAY, 
};

enum colorRes_new
{
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_GREEN_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
// 	ResourceDef.IMAGE_BAG_BTNFRAME_ORANGE_new,
};

//effect type
enum EFFECT_RES_TYPE
{
	EFFECT_RES_TYPE_particle = 1,
	EFFECT_RES_TYPE_frameAction = 2,
	EFFECT_RES_TYPE_blend = 3,
	EFFECT_RES_TYPE_other = 4,
};

enum EFFECT_PLAY_TYPE
{
	EFFECT_PLAY_TYPE_PLAY_ONCE = 1,
	EFFECT_PLAY_TYPE_PLAY_TIME = 2,
	EFFECT_PLAY_TYPE_PLAY_FOREVER = 3,
	EFFECT_PLAY_TYPE_SPECIAL = 4,
};

enum EFFECT_POS_REFERENCE_TYPE 
{
	EFFECT_POS_REFERENCE_TYPE_Camera = 1,
	EFFECT_POS_REFERENCE_TYPE_MapPos = 2,
	EFFECT_POS_REFERENCE_TYPE_AttackerPos = 3,
	EFFECT_POS_REFERENCE_TYPE_DefencerPos = 4,
};

enum EFFECT_PARTICLE_POSITION_TYPE 
{
	EFFECT_PARTICLE_POSITION_TYPE_free = 1,
	EFFECT_PARTICLE_POSITION_TYPE_grouped = 2,
	EFFECT_PARTICLE_POSITION_TYPE_relative = 3,
};

//refresh event
enum GAME_REFRESH_EVENT 
{
// 	addItem 	= "addItem",
// 	removeItem	= "removeItem",
// 	updateItem	= "updateItem",
// 	equipResult		= "equipResult",
// 	updateFashion	= "updateFashion",
// 	equipFashion 	= "equipFashiion",
// 	coinChange	= "coinChange",
// 	rmbChange	= "rmbChange",
// 	strengthChange	= "strengthChange",
// 	stageStar  = "stageStar",
// 	stageEnd    = "stageEnd",
// 	returnAccounts = "returnAccounts",
// 	refreshItemData = "refreshItemData",
// 	foodData  = "foodData"
};

//Ӷ��װ������
#define MAX_LANSQUENET_EQUIP_POS_NUM = 4

//MonsterType
enum MonsterType 
{
	MonsterType_monster = 0,
	MonsterType_boss 	= 1,
	MonsterType_goblin 	= 2,
	MonsterType_elite   = 3, 		//��Ӣ��
	MonsterType_lansquenet = 4,
};

//SoldierBtnScale
enum SoldierBtnScale 
{
// 	[1] = 0.8,
// 	[2] = 0.5,
// 	[3] = 0.3, 
};

//Goblin_Escape_Type
enum Goblin_Escape_Type 
{
	Goblin_Escape_Type_lose 	= 1,
	Goblin_Escape_Type_win 	= 2,
};

//infinite loop  ,cann't push cca.RepeatForver to sequence  in cocos2dx
#define REPEATFOREVER = 9999999999


//screen shake type

enum ScreenShakeType 
{
	ScreenShakeType_rhomb = 1, 			//����
	ScreenShakeType_top_bottom = 2, 	//����

}; 


//bg monster acton id
enum ActionID 
{
	ActionID_show = 1,
	ActionID_idle = 2,
};

//ˢ������

enum CraeteMonsterType
{
	CraeteMonsterType_createByAi 		= 1, 		//ͨ�������¼��ֵ�ս����
	CraeteMonsterType_createByBgAi 	= 2, 		//ͨ�������¼��ֵ������㣬Ȼ������ս����
	CraeteMonsterType_plantInMap 		= 3, 		//ͨ���༭���ֵ���ͼ
};

//���ڱ༭���е�monster�Ƿ񼤻�
enum MonsterActive
{
	MonsterActive_Active = 1,
	MonsterActive_UnActive = 0,
};

//monster ����
enum MonsterFace
{
	MonsterFace_faceLeft = 0,
	MonsterFace_faceRight = 1,
};
//������tableView����
enum BagTblType 
{
	BagTblType_Fashion = 1,
	BagTblType_Bag = 2,
};

//����ϵͳtableView����
enum ForgeTblType 
{
	ForgeTblType_Enhance = 1,
	ForgeTblType_Polish = 2,
	ForgeTblType_Inlay	= 3,
};

//enhance_item_num
#define ENHANCE_ITEM_NUM = 4
#define POLISH_ITEM_NUM = 4

//ǿ��ϵͳ����������
enum Enhance_Data_Type
{
	Enhance_Data_Type_equip 			= 1, 		//װ��
	Enhance_Data_Type_enhanceStone 	= 2, 		//ǿ��ʯ
	Enhance_Data_Type_gem 			= 3,		//��ʯ
};

//enhance_bottom_alert
enum Enhance_Bottom_Alert 
{
	Enhance_Bottom_Alert_Null 			= 1,
	Enhance_Bottom_Alert_RareQuality 	= 2,
	Enhance_Bottom_Alert_HightAbility 	= 3,
};


//tips btn index
enum TipsBntIndex 
{

};

//gemHeighstTypeId
enum gemHeighstTypeId 
{
// 	[1] = 20001515,
// 	[2] = 20002515,
// 	[3] = 20003515,
// 	[4] = 20004515,
// 	[5] = 20005515,
};

//box leftTop Icon type
enum leftTopIconType
{
	leftTopIconType_equiped 	= 1,
	leftTopIconType_maxLevel 	= 2,
	leftTopIconType_canPolish 	= 3,
	leftTopIconType_recommend	= 4, 	//�Ƽ�
	leftTopIconType_canCompose 	= 5,
	leftTopIconType_canEquip 	= 6,
};


// enum ReturnType //����ֵ
// {
// 	SUCCESS =0,				//�ɹ�
// 	NOT_ENOUGH_STRENGTH =1, //ȱ������
// 	NOT_ENOUGH_COIN =2,		//ȱ�ٽ��
// 	NOT_ENOUGH_GOLD =3,		//ȱ��Ԫ��
// 	NOT_ENOUGH_TIMES =4,	//��������	
// 	OVER_MAX_TIMES =5,		//�������޴���
// 	NOT_RIGHT_TIME =6,		//���ǿ���ʱ��
// 	FAIL_OTHER = 20,			//����ʧ��
// };
enum ReFlashType
{
	REFLASH_ROLE_PROPERTY	 =1,//��������
	REFLASH_BACKPACK		 =2,//����
	REFLASH_EQUIP			 =3,//װ��
	REFLASH_CHAPTER			 =4,//�ؿ�
	REFLASH_AWARD			 =5,//�������
	REFLASH_BOX				 =6,//����
	REFLASH_GIFT			 =7,//���
	REFLASH_VIP				 =8,//VIP
	REFLASH_AWARD_CARD		 =9,//����
	REFLASH_LOTTERY			 =10,//����
	REFLASH_WELFALE_MARKET	 =11,//�и�����
	REFLASH_FOOD			 =23,//��ʳ��
};

enum CopyType
{

	STAGE_NORMAL	 = 1,	//��ͨ
	STAGE_ELITE		 = 2,	//��Ӣ2
	STAGE_BONUS		 = 3,	//����3
	STAGE_MASTAR	 = 4,	//��ʦ4
	STAGE_SPECIAL	 = 5,	//����5
	STAGE_TRIAL		 = 6,	//���˸���
	STAGE_TWELVEPALACE    = 7,	//ʮ���ڹ�
	STAGE_COURAGE    = 10,	//��������
	STAGE_WELFARE    = 116,	//


};
enum
{
	STAGE_RECORD_TYPE			= 21, //���˹ؿ�
	TWELVE_PALACE_RECORD_TYPE   = 22, //ʮ���ڹ�
	TWELVE_PALACE_MY_RECORD		= 23, //ʮ���ڹ����˼�¼
	TWELVE_PALACE_RESET_TIMES	= 24, //ʮ���ڹ����ô���

	LOCAL_DATA_SELECT			= 25, //������Ϣ�������߹�ѡ
	CHAPTER_DATA_LOCK			= 26, //�½ڽ�����Ϣ
	
	
};
enum AwardType
{
	AWARD_BASE = 1,//������Ʒ��ͭǮ��Ԫ�����ϻ꣬���������飬����
    AWARD_GRASS = 2,//�������ɲ�
	AWARD_FOOD = 3,//��ʳ��
	AWARD_WELFAREMARKET = 4,//�и�����
 
	AWARD_HOARSTONEPIECE = 5,//��ʯ��Ƭ
	AWARD_SERVANTMATERIAL = 6, //Ӷ�����ĵ��ߣ��������ֱ㵱��ԭʯ
	AWARD_SERVANT = 7,//Ӷ��
	AWARD_SERVANTPIECE = 8, //Ӷ����Ƭ
	AWARD_SERVANTTREASURE = 9,//Ӷ������
	AWARD_SERVANTSWITCH = 10, //Ӷ��������֮�⣬����Ӷ���һ�
    AWARD_ENHANCEMATERIAL = 11, //ǿ������
	AWARD_GEM = 12,
	AWARD_JEWELRY = 13,
	AWARD_FASHION = 14,
	AWARD_HOARSTONEEQUIP = 16,//����
	AWARD_HEROEQUIP = 17,//Ӣ��װ����Ŀǰ���ɻ�ȡ

	AWARD_CONSORTCONTRIBUTE = 32,





	//........................�������ݲ��ǲ߻������ȡ����Ʒ��ֻ�����ɷ���ͨ����ӵ�
	AWARD_TOWERBUFF = 18,//����buff
	AWARD_TOWERATTR = 19,//��������
	AWARD_STAGEDATA = 20,//�ؿ���Ϣ
	AWARD_COUNTER = 21,//������
	AWARD_REWARDCOUNTER = 22,//����������
	AWARD_STRENGTHDATA = 24,//����
	AWARD_TWELVEPALACE_STAGEDATA = 25,//ʮ���ڹ��ؿ���Ϣ
	AWARD_SHOP = 26,//������Ʒ
	AWARD_SERVANTSHOP = 27,//Ӷ����Ʒ
	AWARD_GIFTSHOP = 28,//�����Ʒ
	
	AWARD_CATCOIN = 29,//��������Ӳ��
	AWARD_CURRENCY = 30,//ͨ��
	AWARD_FREETICKET = 31,//�齱ȯ

	

	AWARD_RANDOM_SERVANTPIECE = 80, //���Ӷ����Ƭ
	AWARD_JEWELRY_SELL = 113,//��Ʒ��
	AWARD_RANDOM_GEM = 120,//�����ʯ

	


};

enum AwardBase
{
	AWARD_BASE_COIN = 1,	//ͭǮ
	AWARD_BASE_GOLD = 2,	//Ԫ��
	AWARD_BASE_STRENGTH = 3,	//����
	AWARD_BASE_EXP = 4,	//����
	AWARD_BASE_COMPETITIVE_POINT = 5,	//�����㣨���ף�
	AWARD_BASE_RHYME_SOUL = 6,	//�ϻ�	
	AWARD_BASE_SCORE = 7,	//��������

	AWARD_BASE_VIP_EXP = 9,	//VIP����
	AWARD_BASE_VIP_LEVEL = 10,	//VIP�ȼ�

	AWARD_BASE_RANKEDSCORE = 20001,	//��λ������
	AWARD_BASE_LEVEL = 100	//�ȼ�


};
enum RhymeGrass
{
	SmallGrass = 1,
	MidGrass =2,
	LargeGrass = 3

};

enum RobotData
{
	RobotMinRoleID = 1000,
	RobotMaxRoleID = 10000,
	RankGameInitScore = 1000
};

enum AwardTowerAttr
{
	AWARD_TOWERATTR_AP = 1,//������
	AWARD_TOWERATTR_HP =2,//����ֵ

};


enum ServantMaterial
{
	ServantFood1 = 1,
	ServantFood2 = 2,
	ServantFood3 = 3,
	ServantFood4 = 4,
	ServantFood5 = 5,
	ServantBattleStone = 6
};

//��������ݿ�����Ϣ
enum ServantMainBatBit
{
	SERVANT_MAIN_NONE =  0x00000000,
	SERVANT_MAIN1_OPEN = 0x00000001,
	SERVANT_MAIN2_OPEN = 0x00000002,
	SERVANT_MAIN3_OPEN = 0x00000004,
	SERVANT_MAIN4_OPEN = 0x00000008,
	SERVANT_MAIN5_OPEN = 0x00000010

};


//����������ݿ�����Ϣ
enum ServantAssistBatBit
{
	SERVANT_ASSIST_NONE =  0x00000000,
	SERVANT_ASSIST1_OPEN = 0x00000001,
	SERVANT_ASSIST2_OPEN = 0x00000002,
	SERVANT_ASSIST3_OPEN = 0x00000004,
	SERVANT_ASSIST4_OPEN = 0x00000008,
	SERVANT_ASSIST5_OPEN = 0x00000010,
	SERVANT_ASSIST6_OPEN = 0x00000020,
	SERVANT_ASSIST7_OPEN = 0x00000040
};


enum ChannelType
{
	WORLDCHAT = 1, //����
	CONSORTIACHAT = 2,//��������
	PRIVATECHAT = 3,//˽��
	SYSTEMMSG = 4,//ϵͳ��Ϣ
	GMMSG = 5,//GM��Ϣ
	CONSORTMASG = 10 //������ļ
};

enum GlobelVar
{
	MAXJEWELRYCOUNT = 96,
	OFFLINESAVETIME = 120,//���߱���ʱ��120s
	RANKGAMECOPYID = 100000000,//��λ���ؿ�id

	GETAWARDSCOPYID = 99999999,//��ȡ������Ʒ��fireconfirm
	XINSHOUYINDAOSTAGE = 10000,//���������ؿ�����������

};

enum Ticket
{	
	ONCEFREELOTTERYTICKET = 1,
	TENFREELOTTERYTICKET	 = 2,
};

enum Foods
{	
	// 		1	��ʳ���ƴ���
	// 			2	ȫ����
	// 			3	��ϡ����
	// 			4	��β����
	
	FOODTYPE1 = 1,
	FOODTYPE2 = 2,
	FOODTYPE3 = 3,
	FOODTYPE4 = 4,
};

//�ɾͣ��������¼�id
 enum AssocialEvent
 {
	BARRIER = 2001,   //ͨ�عؿ�
	ELITEBARRIER = 2002,   //ͨ�ؾ�Ӣ�ؿ�
	RHYMEENHANCELV = 2003,  //����ǿ����ָ��
	RHYMESKILLACTIVE = 2004,  //�Ϲ�����
	HONORSTONEACTIVE = 2005, //��ʯ����
	GETSERVANT = 2006, //���һ��������Ӷ��
	PLAYERLV = 2007, //��ҵȼ��ﵽĳ����
	VIPLV = 2008, // vip�ﵽĳһ����
	POWER = 2009, //ս�����ﵽ��ֵ
	OWN4STARSERVANT = 2010,  //ӵ������Ӷ������
	OWNPURPULESERVANT = 2011, //ӵ����ɫӶ������
	UNLOCKSERVANTASSIS = 2012, //��������λ����
	OWNGREENASSIST = 2013, //ӵ����ɫ����λ����

	OWNBLUEASSIST = 2014,  //ӵ����ɫ����λ����
	OWNPURPLEASSIST = 2015,  //ӵ����ɫ����λ����
	QUALIFYDANLV = 2016,  //��λ������
	QUALIFYSUCCESS = 2017, //��λ��ʤ������
	ARIEASUCCESS = 2018,  //������ʤ������
	BUYCHARACTOR = 2019,  //�����ɫ����
	COURAGESUCCESS = 2020,  //���������ɹ�����

	WXCHALLENGESUC = 2021,  //ͨ��ĳһ¥
	USERHYMESKILL = 2022,  //�ͷ��Ϲ�����


	USERHYMESPEED = 2023, //�ͷ�������������
	CALLSERVANTCNT = 2024,  //ս�����ٻ�Ӷ������
	FRIENDCNT = 2025,  //���Ѹ���
	OWNLV5GEMCNT = 2026,  //ӵ���弶��ʯ����
	OWNLV10GEMCNT = 2027,  //ӵ��ʮ����ʯ����
	OWNLV15GEMCNT = 2028,  //ӵ��ʮ�弶��ʯ����
	LINGRENCALLSERVANT = 2029, //�����ٻ�����
	XIBANCALLSERVANT = 2030, //Ϸ�����ٻ�����
	MINGLINGCALLSERVANT = 2031, //�����ٻ�����
	MAXGEMLV = 2032 //��ʯ���ȼ�
 };

 //ÿ������������¼�id
 enum DailyEvent
 {
		DLYBARRIERCNT = 1001,//��ͨ����ͨ�ش���������ɨ��
		DLYELITEBARRIRECNT    = 1002,//��Ӣ����ͨ�ش���,����ɨ��
		DLYLINGRENCALLCNT   = 1003, //������ļ����
		DLYXIBANCALLCNT       = 1004, //Ϸ����ļ����
		DLYMULTYSUCCESS      = 1005,  //�ɹ���ս���˸���������ɨ��
		DLYTELVESUCCESS		= 1006,  //�ɹ���ս12�ڹ�������ɨ��
		DLYDOMARKETCNT		= 1007, //�����и�������ս������ʤ��
		DLYDOCOURAGECNT	= 1008, //��������������ս������ʤ��
		DLYDOWXCHALLENGE  = 1009, //����������ս����������ʤ��
		DLYDOQUALIFYCNT		= 1010, //������λ������������ʤ��
		DLYDOARIEACNT			= 1011,//��ս�������ж��ִ���������ʤ��
		DLYRHYMEENHANCECNT		= 1012, //����ǿ������
		DLYHORNORSTONELVUPCNT	= 1013, //��ʯ��������
		DLYBAICAISHEN                    = 1014, //�ݲ����������Ѻ͸��Ѷ��������
        DLYBUYENERGE						= 1015,//��������
		DLYFOODCOOK						= 1017,//��ʳ�����
		DLYCONSORTSIGN					= 1016//Ϸ��ǩ��
		
 };

 //7��ѵ
 enum SevenDayTranningEvent
 {
	 SDT01		= 3001,//�ɹ�ͨ������ؿ�%d��
	 SDT02     = 3002,//ǿ������װ��%d��
	 SDT03     = 3003, //����%d�������ϳɲ���
	 SDT04      = 3004, //�������⼼��%d��
	 SDT05      = 3005,  //����%d����������
	 SDT06		= 3006,  //ɨ�����⾫Ӣ�ؿ�%d��
	 SDT07		= 3007, //�ݲ���%d��
	 SDT08		= 3008, //����%d��������ļ
	 SDT09		 = 3009, //�����⾩��è������������%d��
	 SDT10		= 3010, //����%d����Ʒϴ������
	 SDT11		= 3011,//�ɹ���ս���˸���%d��
	 SDT12		= 3012, //��ս�и�����%d��
	 SDT13		= 3013, //��ս����ʮ���ڹ��ؿ�%d��
	 SDT14      = 3014, //ˢ�������̵�%d��
	 SDT15		= 3015,//�ɹ���ս�����ʦ�ؿ�%d��
	 SDT16      = 3016,//�ɹ�����%d������������ս
	 SDT17      = 3017,//����%d��λ����ս
// 	 SDT18      = 3018,//����%d��ͬ��PVP��ս  
	 SDT19      = 3019,//��ս%d��������ս�淨
	 SDT20      = 3020,//�ھ���è�̵깺��%d����Ʒ
	 SDT21      = 3021//�ɹ���ս%d������ʮ���ڹ���������

 };

 //������־����id
enum ConsortLogEvent
 {
		CNLCREATE = 111, //��������
		CNLLVUP	= 112, //��������
		CNLSETUPVICELEADER = 113, //�������ø��᳤
		CNLCHANGENAME = 114, //�������
		CNLMEMBERJOIN = 115, //�����Ա����
		CNLMEMBERLEAVE = 116, //��Ա�뿪
		CNLCHANGEDESCS = 117, //���Ĺ�������
		CNLCHANGECHECK = 118, //���Ĺ�����ļ��ʽ
		CNLCHANGEPOWERLIMIT = 119,  //���ļ���ս������
		CNLKICKMEMBER = 120,//���߳�����
		CNLREFUSEAPPLY	= 121, //�ܾ�����
		CNLCLEARAPPLY = 122,
		CNLSETDOWNMEMBER = 123
};

//���᳣������
enum ConsortCondition
{
	CREATENEEDVIPLV = 3, //���ᴴ����Ҫvip�ȼ�
	CREATENEEDGOLD = 1000, //���ᴴ����Ҫ��Ԫ��
    POWERLIMIT = 1000 //����ս�����ƣ�����1000�ſɼ���
};

enum ConsortNotifyMsgType
{
	NOTIFYJOINCONSORT = 1,
	APPLYADD = 2,
	APPLYREFUSE = 3,
	APPLYACCEPT = 4,
	APPLYCLEAR = 5,
	CHANGECHECKNOTIFY = 6,
	CHANGEPOWERLIMIT = 7,
	CHANGEDESC = 8,
	KICKMEMBERNOTIFY = 9,
	SETVICELEADER = 10,
	LEAVECONSORT = 11

};
// װ��
// ��ͷ
// ð��
// ��ͨ�Ѷ�ð��
// �ʼ�
// ����
// ����
// ��ֵ
// ��������
// �ݲ���
// ����
// ���߽���
// ��������ѵ
// ��ɫ��Ϣ
// ��ʽ
// ����è
// ͼ��
// �ͷ���
// ����
// ����
// �ɾ�
// ÿ������
// �����½����
// ��ʳ��
// ȫ����
// �䱦��
// ����̵�
// �䱦�̵�
// ��ǩ��
// ����
// ����
// ����㵱
// ���������
// ������
// ��λ��
// ������
// ���а�
// ��Ʒ
// ��ʯ
// ��ʦ�Ѷ�ð��
// ʮ���ڹ�
// ����
// ��ɽ����
// ʱװ
// �и�����
// ͨ������
// �����г�
// ��ʯ����
// �м�����
// ����è�̵�
// ��������
// Ϸ��
// ����
// ������ս


enum SYSTEM_ID
{
	SYSTEM_SHOP 			= 2001,
	SYSTEM_ZHAOFUJISHI 	= 1501,
	SYSTEM_SHANGFALING 	= 701,
	SYSTEM_RANK 			= 2201,
	SYSTEM_FOOD 			= 2101,
	SYSTEM_GUILD 			= 2301,//0
	SYSTEM_PVP			= 1702,//0
	SYSTEM_COURAGE 		= 1802,
	SYSTEM_PALACE 			= 1901,
	SYSTEM_CHALLENGE 		= 1801,
	SYSTEM_MULTI 			= 1601,
	SYSTEM_EQUIP 			= 104,//0
	SYSTEM_SERVANT 		= 501,//0
	SYSTEM_JEWELRY		= 201,//0
	SYSTEM_SKILL 			= 401,//0
	SYSTEM_HOARSTONE 		= 302,//0
	SYSTEM_RHYME 			= 301,//0
	SYSTEM_GROW 			= 300,//0
	SYSTEM_SETTING 		= 1101,//0
	SYSTEM_FRIEND 			= 601,//0
	SYSTEM_CHAT 			= 1201,//0
	SYSTEM_EMAIL 			= 1001,
	SYSTEM_MISSION 		= 901,//0
	SYSTEM_FULI 			= 1401,//0
	SYSTEM_PAY				= 1301,//0
	SYSTEM_ONLINEAWARD		= 1403,//0
	SYSTEM_STAGE			= 801,//0
	SYSTEM_NOTICE 			= 1102,//0
	SYSTEM_CONSORT			= 2301,
	SYSTEM_TASK			= 5000,
	SYSTEM_NEWPLAYERGUIDE			= 5001,
	SYSTEM_ELSEAWARD			= 5002,
};

enum systemidrefreshtime
{
	TIME_REFRESH_ID01 = 802,  //��ͨ�Ѷ�ð�գ�������ͨ+��Ӣ�ؿ�������ս����ÿ������ʱ��
	TIME_REFRESH_ID02 = 803,  //��ʦ�Ѷ�ð�գ��������粼�ֹؿ�������ս����ÿ������ʱ��
	TIME_REFRESH_ID03 = 903,  //ÿ������ÿ������ʱ��
	TIME_REFRESH_ID04 = 1302,  //��������ÿ�����ô���ʱ��
	TIME_REFRESH_ID05 = 1303,  //�ݲ���ÿ�����ô���ʱ��
	TIME_REFRESH_ID06 = 1404,  //��ǩ��ÿ���ж�ʱ��
	TIME_REFRESH_ID07 = 1408,  //����㵱ÿ������ʱ��
	TIME_REFRESH_ID08 = 1501,  //�и�����ÿ�տ���ս��������ʱ��
	TIME_REFRESH_ID09 = 1601,  //��ɽ����ÿ�տ���ս��������ʱ��
	TIME_REFRESH_ID10 = 1702,  //��λ��ÿ�տ���ս��������ʱ��
	TIME_REFRESH_ID11 = 1703,  //��λ��ÿ�տɶ��⹺���������ʱ��
	TIME_REFRESH_ID12 = 1801,  //������սÿ�տ���ս��������ʱ��
	TIME_REFRESH_ID13 = 1802,  //��������ͨ���̵�ÿ������ʱ��
	TIME_REFRESH_ID14 = 1803,  //��������ÿ�տɶ��⹺���������ʱ��
	TIME_REFRESH_ID15 = 1804,  //�����������ˢ����ս�����������ʱ��1
	TIME_REFRESH_ID16 = 1805,  //�����������ˢ����ս�����������ʱ��2
	TIME_REFRESH_ID17 = 1901,  //ʮ���ڹ�ÿ�ո�������ʱ��
	TIME_REFRESH_ID18 = 701,  //�ͷ����������������ʱ��
};

enum LOG_LEVEL
{
	LogInfo	= 1,
	LogWarn	=	2,
	LogError	= 3,
	LogFatal	= 4,

};

enum LOG_TYPE
{
	CoinChange = 25,
	GoldChange = 26,
	ServantChange = 27,
	ServantPieceChange = 28,
	JewelryChange = 29,

	LogType50 = 50, //	�����˺�
	LogType51 = 51, //  ������ɫ
	LogType52 = 52, // ��Ծ�û�
	LogType53 = 53, //	7��������½+������
	LogType54 = 54, //   
	LogType55 = 55, //	
	LogType56 = 56, //	��ҵ�½
	LogType57 = 57, //�������

	LogType58 = 58, //�������ʱ��

	LogType59 = 59, //	��������
	LogType60 = 60, //�����������
	LogType61 = 61, //ÿ���½��ͬ�˺�����

	LogType62 = 62, //�����ָ�
	LogType63= 63, //��������
	LogType64 = 64, //gm�޸�����
	LogType65 = 65, //��ʳ�����ָ�
	LogType66 = 66, //��ʳ�ܸ����仯
	LogType67 = 67, //������ʳ����ֱ�ӱ����ʳ����

	LogType68 = 68, //��ʯ
	LogType69 = 69, //����
	LogType70 = 70, //�ϻ�
	LogType71 = 71, //��λ������
	LogType72 = 72, //�����㣨���ף�
	LogType73 = 73, //���ɲ�
	LogType74 = 74, //��ʯ��Ƭ
	LogType75 = 75, //ǿ������
	LogType76 = 76, //����
	LogType77 = 77, //��ͨ�ؿ�����
	LogType78 = 78, //ʮ���ڹ��ؿ�����
	LogType79 = 79, //Ӷ�����ĵ��ߣ��������ֱ㵱��ԭʯ
	LogType80 = 80, //Ӷ������
	LogType81 = 81, //Ӷ��������֮��
	LogType82 = 82, //��������Ӳ��
	LogType83 = 83, //�ͷ����ȯ
	LogType84 = 84, //�ͷ���ʮ����ȯ
	LogType85 = 85, //��������ͨ��
	LogType86 = 86, //���ṱ��
	LogType87 = 87, //ʱװ����
	LogType88 = 88, //��ɫ�л�
	LogType89 = 89, //��Ʒװ��
	LogType90 = 90, //��Ʒж��
	LogType91 = 91, //����è����
	LogType92 = 92, //������Ƕ
	LogType93 = 93, //��ʯװ������
	LogType94 = 94, //����è����λ����
	LogType95 = 95, //
	LogType96 = 96,//��ʽ�л�
	LogType97 = 97,//
	LogType98 = 98,//��ʯж��

	LogType99 = 99,//�ȼ�
	LogType100 = 100,//gm���ȼ�
	LogType101 = 101,//VIP�ȼ�
	LogType102 = 102,//gm��VIP�ȼ�
	LogType103 = 103,//vip����



	LogType200 = 200//


};

enum MINI_LOG
{
	MiniLog1=1,//װ��ǿ��
	MiniLog2=2,//װ������
	MiniLog3=3,//GM�����޸�
	MiniLog4=4,//װ��ϴ��
	MiniLog5=5,//��ɫ������
	MiniLog6=6,//��Ʒ����
	MiniLog7=7,//��Ʒϴ��
	MiniLog8=8,//���˸���˫������
	MiniLog9=9,//���˸�����ս
	MiniLog10=10,//ʱװ����
	MiniLog11=11,//	�����ɫ
	MiniLog12=12,//
	MiniLog13=13,//
	MiniLog14=14,//
	MiniLog15=15,//��ʽ����
	MiniLog16=16,//
	MiniLog17=17,//
	MiniLog18=18,//
	MiniLog19=19,//
	MiniLog20=20,//����è����
	MiniLog21=21,//����è����λ
	MiniLog22=22,//
	MiniLog23=23,//
	MiniLog24=24,//
	MiniLog25=25,//��������
	MiniLog26=26,//
	MiniLog27=27,//
	MiniLog28=28,//
	MiniLog29=29,//
	MiniLog30=30,//��ʯ����
	MiniLog31=31,//��ʯ����
	MiniLog32=32,//
	MiniLog33=33,//
	MiniLog34=34,//
	MiniLog35=35,//����ǿ��
	MiniLog36=36,//
	MiniLog37=37,//
	MiniLog38=38,//
	MiniLog39=39,//
	MiniLog40=40,//������ǩ����ȡ
	MiniLog41=41,//����
	MiniLog42=42,//ȫ����
	MiniLog43=43,//����㵱
	MiniLog44=44,//7���¼
	MiniLog45=45,//7������
	MiniLog46=46,//���߽���
	MiniLog47=47,//�ɳ�����
	MiniLog48=48,//
	MiniLog49=49,//
	MiniLog50=50,//����ÿ������
	MiniLog51=51,//����ɾ�
	MiniLog52=52,//�����ؿ���ս
	MiniLog53=53,//��Ӣ�ؿ���ս
	MiniLog54=54,//��ʦ�ؿ���ս
	MiniLog55=55,//�ؿ�����
	MiniLog56=56,//��ͨ�ؿ���ս
	MiniLog57=57,//��ͨ�ؿ�ɨ��
	MiniLog58=58,//�ؿ���������
	MiniLog59=59,//�ؿ�����
	MiniLog60=60,//�и�������ս
	MiniLog61=61,//�и�����ɨ��
	MiniLog62=62,//�����ؿ�ɨ��
	MiniLog63=63,//��Ӣ�ؿ�ɨ��
	MiniLog64=64,//��ʦ�ؿ�ɨ��
	MiniLog65=65,//ʮ���ڹ�����Ž�
	MiniLog66=66,//ʮ���ڹ�������ս����
	MiniLog67=67,//ʮ���ڹ���ս
	MiniLog68=68,//ʮ���ڹ�ɨ��
	MiniLog69=69,//
	MiniLog70=70,//��ʳ�ݹ������
	MiniLog71=71,//
	MiniLog72=72,//
	MiniLog73=73,//
	MiniLog74=74,//
	MiniLog75=75,//Ϸ�� ����
	MiniLog76=76,//Ϸ�� ǩ��
	MiniLog77=77,//Ϸ�� ����
	MiniLog78=78,//Ϸ�� ��������
	MiniLog79=79,//Ϸ�� ����è
	MiniLog80=80,//Ϸ�� ������ѵ
	MiniLog81=81,//Ϸ�� ��������
	MiniLog82=82,//Ϸ�� �һ�
	MiniLog83=83,//
	MiniLog84=84,//��������ͨ���̵�ˢ��
	MiniLog85=85,//��������ˢ��
	MiniLog86=86,//���������������
	MiniLog87=87,//����������ս
	MiniLog88=88,//��������ͨ���̵깺��
	MiniLog89=89,//Ӳ�Һ�
	MiniLog90=90,//�ͷ������������ļ
	MiniLog91=91,//�ͷ���������ļ
	MiniLog92=92,//�ͷ���Ϸ�������ļ
	MiniLog93=93,//�ͷ���Ϸ����ļ
	MiniLog94=94,//�ͷ���������ļ
	MiniLog95=95,//�ͷ���ͭ����ļ
	MiniLog96=96,//����ȯʮ����
	MiniLog97=97,//����ȯvip��
	MiniLog98=98,//
	MiniLog99=99,//
	MiniLog100=100,//�䱦���䱦
	MiniLog101=101,//�䱦�󾩾�è
	MiniLog102=102,//�䱦���䱦ˢ��
	MiniLog103=103,//�䱦�󾩾�èˢ��
	MiniLog104=104,//�䱦�����
	MiniLog105=105,//������ս�������
	MiniLog106=106,//������ս��ս
	MiniLog107=107,//������սɨ��
	MiniLog108=108,//������սѡ����
	MiniLog109=109,//������ս����
	MiniLog110=110,//��λ���������
	MiniLog111=111,//��λ�������̵깺��
	MiniLog112=112,//��λ����λ����
	MiniLog113=113,//��λ����ս
	MiniLog114=114,//��λ�������̵�ˢ��
	MiniLog115=115,//��ɽ������ս
	MiniLog116=116,//��ɽ��������
	MiniLog117=117,//
	MiniLog118=118,//
	MiniLog119=119,//
	MiniLog120=120,//�ʼ����ʼ�
	MiniLog121=121,//
	MiniLog122=122,//
	MiniLog123=123,//
	MiniLog124=124,//
	MiniLog125=125,//���췢��Ϣ
	MiniLog126=126,//
	MiniLog127=127,//
	MiniLog128=128,//
	MiniLog129=129,//
	MiniLog130=130,//�ݲ�����Ѱ�
	MiniLog131=131,//�ݲ���1�ΰ�
	MiniLog132=132,//�ݲ���10�ΰ�
	MiniLog133=133,//�ݲ�������
	MiniLog134=134,//
	MiniLog135=135,//VIP����vip
	MiniLog136=136,//��������
	MiniLog137=137,//�ͻ��˽���������
	MiniLog138=138,//
	MiniLog139=139,//
	MiniLog140=140,//����� 
	MiniLog141=141,//
	MiniLog142=142,//
	MiniLog143=143,//
	MiniLog144=144,//�״ε�¼����
	MiniLog145=145,//��������
	MiniLog146=146,//
	MiniLog147=147,//
	MiniLog148=148,//

};

#endif // __LYNX_COMMON_LIB_CONST_DEFINITION_DEFINITION_H__