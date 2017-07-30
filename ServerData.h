#ifndef __LYNX_SERVER_DATA_H__
#define __LYNX_SERVER_DATA_H__


#include "LogicSystem.h"



namespace Lynx
{
	struct ServerPublicData
	{
		ServerPublicData():gRechargeNum(0),gServerRobotRefreshFlag(0),gServerID(0),gRankGameRewardresetTime(0),gServerFirstOpenTime(0),gServerSpare5(0){}
		UInt32 gServerID ;//������ID
		UInt32 gRechargeNum ;//��ֵ����
		UInt32 gServerRobotRefreshFlag ;//���û��������ݱ�׼0 ��Ҫ����		
		UInt32 gRankGameRewardresetTime ;//��λ����������ʱ��
		UInt32 gServerFirstOpenTime ;//�������״ο���ʱ��
		UInt32 gServerSpare5 ;//

		LYNX_S11N_6(ServerPublicData,UInt32,gRechargeNum,UInt32, gServerRobotRefreshFlag,UInt32, gServerID,UInt32, gRankGameRewardresetTime,UInt32, gServerFirstOpenTime,UInt32, gServerSpare5);
	};

	struct ServerDailyData
	{
		ServerDailyData():gServerOnlineMaxNum(0),gServerOnlineNumSetTime(0),gServerOnlineNum(0),gSSpare1(0),gSSpare2(0),gSSpare3(0){}
		UInt32 gServerOnlineNum ;//��ǰ��������
		UInt32 gServerOnlineMaxNum ;//���������������
		UInt32 gServerOnlineNumSetTime ;//���������������ʱ��
		UInt32 gSSpare1 ;//
		UInt32 gSSpare2 ;//
		UInt32 gSSpare3 ;//

		LYNX_S11N_6(ServerDailyData,UInt32,gServerOnlineMaxNum,UInt32, gServerOnlineNumSetTime,UInt32, gServerOnlineNum,UInt32, gSSpare1,UInt32, gSSpare2,UInt32, gSSpare3);
	};

    class ServerData : public MainThread, public Singleton<ServerData>
	{
	public:
	
		void serverDatainit();
		ServerPublicData &getServerPublicData();
		void setServerPublicData(ServerPublicData &serverPublicData,bool neeSave);//true =save

		void saveOnlinePlayerGuid(Map<Guid, Player*> &mPlayerGuidMap);

		ServerDailyData &getServerDailyData();
		void setServerDailyData(ServerDailyData &serverDailyData,bool neeSave);//true =save

		void checkMaxOnlineNum();
		void addOnlineNum();
		void subOnlineNum();
	
	

	};
	static ServerPublicData mServerPublicData;
	static ServerDailyData mServerDailyData;

} // namespace Lynx

#endif // __LYNX_SERVER_DATA_H__

