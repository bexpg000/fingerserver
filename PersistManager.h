#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PERSIST_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PERSIST_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class PersistManager : public MainThread,  public Singleton<PersistManager>
	{
	public:
		PersistManager();
		virtual ~PersistManager();

		bool initial(Player* player);
		void release();
		
		//���ر�־λ������
		UInt32 & getDirtyBit();

		void setDirtyBit(UInt32 dirtyBit);

		void update(const UInt64& accTime);

	

	private:
		
		Player* m_pPlayer;
		//32λ������ÿһλ����ʾ��ͬ��״̬
		UInt32 m_nDirtyBit;

		UInt64  m_nLastSyncTime;

		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_PERSIST_MAMAGER_H__
