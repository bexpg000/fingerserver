#ifndef __LYNX_COMMON_LIB_CONSORT_DATA_H__
#define __LYNX_COMMON_LIB_CONSORT_DATA_H__

#include "Definition.h"

//���쳽
//�����������
//2016 10 31


namespace Lynx
{

	//�����������ݽṹ
	    struct ApplyData
		{
			ApplyData():m_nApply(0), m_nTime(0){}
			//���id
		   UInt64 m_nApply;
		   //�������ʱ��
		   UInt64 m_nTime;
		   LYNX_S11N_2(ApplyData, UInt64, m_nApply, UInt64, m_nTime);
		};

		struct LogData
		{
			LogData(): m_nConsortId(0), m_nTime(0), m_nTempId(0),
				m_strParam1(""), m_strParam2(""), m_nPlayerUid(0), m_strPlayername(""){}
		
			UInt64 m_nTime;
			UInt64 m_nTempId;
			UInt64 m_nConsortId;
			String m_strParam1;
			String m_strParam2;
			UInt64 m_nPlayerUid;
			String m_strPlayername;
			LYNX_S11N_7(LogData,  UInt64, m_nTime, UInt64 ,m_nTempId,
				String, m_strParam1, String ,m_strParam2, UInt64 ,m_nPlayerUid, String, m_strPlayername, UInt64, m_nConsortId);

		};

		struct TicketFriend
		{	
			TicketFriend():m_nPlayeruid(0), m_nServantid(0){}
			UInt64 m_nPlayeruid;
			UInt64 m_nServantid;
		    UInt32 m_nOrderIndex;
		   LYNX_S11N_3(TicketFriend, UInt64, m_nPlayeruid, UInt64, m_nServantid, UInt32 , m_nOrderIndex);
		};

		struct GivenData
		{
			GivenData():m_strName(""), m_nGrade(0){}
			String m_strName;
			UInt32 m_nGrade;
		  
		   LYNX_S11N_2(GivenData, String, m_strName, UInt32, m_nGrade);
		};

		 struct TicketData
		{
			TicketData():m_nTicketId(0),m_nQuality(0){}
			//Ʊ������id
		   UInt32 m_nTicketId;
		   List<TicketFriend> m_ticketFriends;
		   List<GivenData> m_givenList;
		   UInt32 m_nQuality;
		   LYNX_S11N_4(TicketData, UInt32, m_nTicketId,  List<TicketFriend>, m_ticketFriends,
			   UInt32, m_nQuality ,List<GivenData> ,m_givenList);
		};


	//������Ϣ��ȫ�����Žṹ��
	struct ConsortData
	{

		ConsortData() : m_nId(0), m_nLv(0),  m_strName(""), m_nCount(0), m_nPower(0), 
		m_strDesc(""), m_nRes(0), m_nLeader(0),m_nExp(0), m_nCheck(0), m_nPowerLimit(0), m_nSignLv(0), m_nActive(0),m_nBusinessLv(0),
		m_nWoodBloodLeft(0), m_nEnhance1(0), m_nEnhance2(0), m_nEnhance3(0){}

		//consortid
		Guid m_nId;
		//consortlv
		UInt32 m_nLv;
		//consortname
		String m_strName;
		//count
		UInt32 m_nCount;
		//power
		UInt64 m_nPower;
        //��������
		String m_strDesc;
		//�����ֶΣ�������Դ��Ŀǰû��
		UInt32 m_nRes;
		//����Ļ᳤
		UInt64  m_nLeader;
		//����ľ���
		UInt32 m_nExp;
		//������˱��
		UInt32 m_nCheck;
		//����ս������
		UInt32 m_nPowerLimit;
		
		//��������б�
		List<ApplyData> m_applyList;
		//��־�б�
		List<LogData> m_logList;
        //��Ա�б�
		List<UInt64> m_nMemberList;


		//���ᵱ��ȼ�,��ֹ����Ӱ��ǩ���߼�
		UInt32 m_nSignLv;
		//�����Ծ��
		UInt32 m_nActive;
		//��������è����ȼ�
		UInt32 m_nBusinessLv;
		//����Ʊ�Ѽ���
		Map<UInt32, TicketData*> m_ticketDataMap;
		List<TicketData> m_ticketList;

		//��ǰ��ҹ�����ɹ�����ʧ����Ѫ��

		UInt32 m_nWoodBloodLeft;
		//����ǿ����׼
		UInt32 m_nEnhance1;
		UInt32 m_nEnhance2;
		UInt32 m_nEnhance3;

		List<UInt64> m_listWoodPlayers1;
		List<UInt64> m_listWoodPlayers2;
		List<UInt64> m_listWoodPlayers3;
		List<UInt64> m_listWoodPlayers4;
		List<UInt64> m_listWoodPlayers5;


		LYNX_S11N_27(ConsortData, Guid ,m_nId, UInt32, m_nLv, String, m_strName, UInt32, m_nCount,
					UInt64, m_nPower,  String , m_strDesc,  UInt32,  m_nRes, UInt64, m_nLeader,
					List<ApplyData>, m_applyList, List<LogData>, m_logList, List<UInt64>, m_nMemberList,
					UInt32, m_nExp,UInt32, m_nCheck, UInt32 ,m_nPowerLimit,UInt32, m_nSignLv, UInt32 ,m_nActive,
					UInt32, m_nBusinessLv, List<TicketData>, m_ticketList,UInt32, m_nWoodBloodLeft,UInt32 ,m_nEnhance1,
					UInt32, m_nEnhance2, UInt32, m_nEnhance3, List<UInt64> ,m_listWoodPlayers1, List<UInt64>, m_listWoodPlayers2,
					List<UInt64>, m_listWoodPlayers3,List<UInt64>, m_listWoodPlayers4,List<UInt64>, m_listWoodPlayers5
			);


	};

	//���Ÿ�Ҫͬ����Ϣ
	struct ConsortRankData
	{
		ConsortRankData():m_nId(0), m_nPower(0), m_nRank(0){}
		//consortid
		Guid m_nId;
		//power
		UInt64 m_nPower;
		//rank
		UInt32 m_nRank;

		LYNX_S11N_3(ConsortRankData, Guid ,m_nId, UInt64, m_nPower,
				UInt32, m_nRank
			);
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CONSORT_DATA_H__