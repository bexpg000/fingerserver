#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PLAYERINFO_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PLAYERINFO_MAMAGER_H__

#include "MainThread.h"


namespace Lynx
{
	class Player;
	struct PlayerBaseInfo
	{
		PlayerBaseInfo():m_pPlayerID(NULL),m_pModelID(NULL),m_pPlayerName(NULL),
		m_pMood(NULL),m_pLevel(NULL),m_pVipLevel(NULL),m_pPower(0),m_nWeaponResId(0),m_nFashionResId(0){}
		
		~PlayerBaseInfo(){
			m_pPlayerID = NULL;
			m_pModelID = NULL;
			m_pPlayerName = NULL;
			m_pMood = NULL;
			m_pLevel = NULL;
			m_pVipLevel = NULL;

			//���ݼ�������Ժ����
			m_pPower = 100;
			m_nWeaponResId = 0;
			m_nFashionResId = 0;

		}
		//��ҽ�ɫID 
		UInt64  *m_pPlayerID;

		//ģ��ID
		UInt32  *m_pModelID;

		std::string  * m_pPlayerName;

		std::string * m_pMood;

		UInt32 * m_pLevel;

		//���vip�ȼ�
		UInt32 * m_pVipLevel;

		//�⼸�������Ҳ���playerdata���Ӧ�Ĺ�����������Ҫ���㲢���û����и���ʱ����ͬ��
		UInt32   m_pPower;

		UInt32   m_nWeaponResId;

		UInt32   m_nFashionResId;

	};

	struct PlayerDetailInfo
	{
		PlayerDetailInfo():m_pPlayerID(NULL),m_pModelID(NULL),m_pPlayerName(NULL),m_pMood(NULL)
		,m_pLevel(NULL),m_pExp(NULL),m_pVipExp(NULL),m_pVipLevel(NULL),m_pCoin(NULL),
		m_pGold(NULL),m_pFame(NULL),m_pStrength(NULL),m_pListSweepCount(NULL),m_pPower(0),m_pArenaEnterCount(NULL),m_pAmphitheaterEnterCount(NULL)
		,m_pAmphitheaterWinCount(NULL),m_pAmphitheaterLoseCount(NULL),m_pDailyChaiCount(NULL),
		m_pDailyMiCount(NULL),m_pDailyYouCount(NULL),m_pDailyYanCount(NULL),m_pTwelvepalaceEnterCount(NULL),
		m_pStrTwelvepalaceData(NULL),m_pCouragetrialDailyPoint(NULL),m_pStrCourageData(NULL),m_pActivepoint(NULL),m_nDailyMultipleCopyCount(NULL),
		m_nTwelvePalaceUnlockCount(NULL){}
		
		~PlayerDetailInfo()
		{
			m_pPlayerID = NULL;
			m_pModelID = NULL;
			m_pPlayerName = NULL;
			m_pMood = NULL;
			m_pLevel = NULL;
			m_pExp = NULL;
			m_pVipExp = NULL;
			m_pVipLevel= NULL;
			m_pCoin= NULL;
			m_pGold= NULL;
			m_pFame= NULL;
			m_pStrength= NULL;
			m_pListSweepCount= NULL;
			m_pPower = NULL;
			m_pArenaEnterCount= NULL;
			m_pAmphitheaterEnterCount= NULL;
			m_pAmphitheaterWinCount= NULL;
			m_pAmphitheaterLoseCount= NULL;
			m_pDailyChaiCount= NULL;
			m_pDailyMiCount= NULL;
			m_pDailyYouCount= NULL;
			m_pDailyYanCount= NULL;
			m_pTwelvepalaceEnterCount= NULL;
			m_pStrTwelvepalaceData= NULL;
			m_pCouragetrialDailyPoint= NULL;
			m_pStrCourageData= NULL;
			m_pActivepoint= NULL;
			m_nDailyMultipleCopyCount =NULL;
			m_nTwelvePalaceUnlockCount =NULL;
			
		}

		//��ҽ�ɫID 
		UInt64  * m_pPlayerID;

		//���ģ��id
		UInt32  * m_pModelID;

		//�������
		std::string  * m_pPlayerName;	
		
		std::string * m_pMood;

		UInt32 * m_pLevel;
		
		UInt64 * m_pExp;
		
		UInt32 * m_pVipLevel;

		UInt64 * m_pVipExp;

		UInt64 * m_pCoin;

		UInt64 * m_pGold;

		UInt64 * m_pFame;
		
		UInt32 * m_pStrength;

		List<UInt32> *m_pListSweepCount;
		
		UInt32 *m_pPower;
			
		UInt16 *m_pArenaEnterCount;

		UInt16 *m_pAmphitheaterEnterCount;

		//������ʤ������
		UInt16 *m_pAmphitheaterWinCount;
		//������ʧ�ܴ���
		UInt16 *m_pAmphitheaterLoseCount;

		//�ճ����������
		UInt16* m_pDailyChaiCount;

		//�ճ������״���
		UInt16* m_pDailyMiCount;

		//�ճ������ʹ���
		UInt16* m_pDailyYouCount;

		//�ճ������δ���
		UInt16 * m_pDailyYanCount;

		//ʮ�����������
		UInt16* m_pTwelvepalaceEnterCount;

		//ʮ������Ϣ
		std::string* m_pStrTwelvepalaceData;

		//���������ճ���õ���

		UInt16* m_pCouragetrialDailyPoint;

		std::string* m_pStrCourageData;

		UInt32*  m_pActivepoint;

		UInt32* m_nDailyMultipleCopyCount;

		UInt32* m_nTwelvePalaceUnlockCount;
		


	};
	
	class PlayerInfoManager : public MainThread,  public Singleton<PlayerInfoManager>
	{
	public:
		PlayerInfoManager();
		virtual ~PlayerInfoManager();

		bool initial(Player* player);
		void release();
		std::string convertBaseInfoToJson(void);
		std::string convertDetailInfoToJson(void);
		void baseInfoSync();
		void detailInfoSync();
		UInt64 getPlayerGold(void);
		void  setPlayerGold(UInt64 gold);

		std::string rename(std::string newname);
		PlayerBaseData* getPlayerBaseData(void);

	private:
		void BaseInfoInitial(Player* player);
		void DetailInfoInitial(Player* player);
		PlayerBaseInfo m_PlayerBaseInfo;
		PlayerDetailInfo m_PlayerDetailInfo;
		Player* m_pPlayer;
		
		PlayerBaseData *m_pBaseData;

		PlayerDailyResetData *m_pDailyRestData;


	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_PLAYERINFO_MAMAGER_H__
