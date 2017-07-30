#include "pvpthread.h"
using namespace Lynx;
#include "../ConfigParser.h"
PVPThread::PVPThread() : mUpdateTimer(NULL)
{

}

PVPThread::~PVPThread()
{

}

bool 
PVPThread::initial(UInt32 index)
{
	#ifdef __linux__
	mIOService = XNEW(IOService)(IOSERVICE_EPOLL_DRIVER);
	#else
	mIOService = XNEW(IOService)();
	#endif
	ASSERT(mIOService);


	mWorkerIndex = index + 1;
	
	m_nSockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == m_nSockServer)
	{

		#ifdef WIN32
// 		LOG_INFO ("socket server failed. errno=[%d]\n", WSAGetLastError());
		#endif

		#ifdef __linx__

// 		LOG_INFO("socket server failed. errno=[%d]\n", errno);
		#endif	
	
		return	false;
	}


	startup();

	LOG_INFO("Start PVP Thread [%u]", mWorkerIndex);

	sockaddr_in	addrServer = {0};
	
	addrServer.sin_family	= AF_INET;

	const char * addr = Lynx::ConfigParser::getSingleton().getMainListenIp().c_str();

	addrServer.sin_addr.s_addr = inet_addr(addr);
	addrServer.sin_port = htons(10000);
	if (0 != bind(m_nSockServer, (sockaddr*)&addrServer, sizeof(addrServer)))
	{
		#ifdef WIN32
		LOG_INFO ("bind server failed.errno=[%d]\n", WSAGetLastError());
		#endif

		#ifdef __linux__
// 		LOG_INFO("bind server failed.errno=[%d]\n", errno);
		#endif		

		return	false;
	}

	cout << "PVP Thread Start Success!!!"<<endl;

	setNoneBlock(m_nSockServer);

	return true;
}

void 
PVPThread::release()
{
	 if (mIOService)
    {
        mIOService->stop();
    }

	shutdown();
	join();

	 if (mIOService)
	{
		XSAFE_DELETE(mIOService);
	}

	LOG_INFO("Shutdown PVPWorker [%u]", mWorkerIndex);
	LOG_INFO("Release PVPThread");
}

void 
PVPThread::onThreadStarted()
{
	mUpdateTimer = XNEW(IOTimer)(mIOService);
    mUpdateTimer->set(10, IOTimerCallback(this, &PVPThread::onUpdateTimer), NULL);
}
void 
PVPThread::onThreadShutdown()
{	
	 if (mUpdateTimer)
    {
        mUpdateTimer->cancel();
        XDELETE(mUpdateTimer);
        mUpdateTimer = NULL;
    }
}

void 
PVPThread::onPreUpdate()
{
	if (mIOService)
    {
        mIOService->run();
    }
}

void 
PVPThread::onPostUpdate()
{

	
}



bool 
PVPThread::onUpdateTimer(void* args)
{
	//cout << "nice to meet you !!!" <<endl;
	int a = 100;	
	
		char	pcContent1[10240] = {0};
		sockaddr_in	addrUser1 = {0};
		#ifdef WIN32
		int	nLen1 = sizeof(addrUser1);
		#endif

		#ifdef __linux__
		socklen_t nLen1 = sizeof(addrUser1);
		#endif
		
		if (-1 == recvfrom(m_nSockServer, pcContent1, sizeof(pcContent1), 0, (sockaddr*)&addrUser1, &nLen1))
		{
			
			#ifdef WIN32
				int  erroNum = WSAGetLastError();
				if(erroNum != WSAEWOULDBLOCK && erroNum != WSAECONNRESET)
				{
// 					LOG_INFO ("socket server failed. errno=[%d]\n", erroNum);
				
				}

			#endif

			#ifdef __linux__
			if(errno != EWOULDBLOCK && errno != ECONNRESET)
			{
				LOG_INFO("socket server failed. errno=[%d]\n", errno);
			
			}
			else
			{
				//cout <<"nodata recv, would block!!!" <<endl;
			}
			
			#endif	
	
		}
		else 
		{
			cout << "msg come" <<endl;
			LOG_INFO ("connect user ip=[%s] port=[%d]\n", inet_ntoa(addrUser1.sin_addr), htons(addrUser1.sin_port));
			
			paraseMsg(pcContent1, addrUser1);

		}
		
		checkTimeOut();

	return true;
}


bool PVPThread::setNoneBlock(socketfd fd)
{
	#ifdef WIN32	
		u_long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
		LOG_INFO("fcntl(%d, F_GETFL)", (int)fd);
		return false;
	 }

	 return true;
	#endif

	#ifdef __linux__
		 
	 int flags;
	 if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
			LOG_INFO("fcntl(%d, F_GETFL)", (int)fd);
			return false;
	 }

	 if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
			LOG_INFO("fcntl(%d, F_GETFL)", (int)fd);
			return false;
	 }

	#endif


}

bool PVPThread::paraseMsg(char * msg, struct sockaddr_in  addrInfo)
{
   // "#AF;101;uuid;^"
	if(strlen(msg) < 4)
	{
		return false;
	}

	if(strncmp("#AF;", msg, 4))
	{
		return false;
	}

	std::string msgStr(msg);
	std::string ::size_type findIndex = msgStr.find(';');
	if(findIndex == std::string::npos)
	{
		return false;
	}

	msgStr = msgStr.substr(findIndex + 1);

	std::string ::size_type dataIndex =  msgStr.find(';');

	std::string msgIdStr = msgStr.substr(0,dataIndex);

	std::stringstream mystream;
	mystream << msgIdStr;
	unsigned long long msgId;
	mystream >> msgId;

	switch(msgId)
	{
		case 1:
		{
			std::string dataStr = msgStr.substr(dataIndex+1);
			dealReqPVPMsg(msgId, dataStr, addrInfo);
		}
		break;

		case 5:
		{
			std::string dataStr = msgStr.substr(dataIndex+1);
			dealGiveUp(msgId, dataStr, addrInfo);
		}
		break;

		case 7:
		{
			std::string dataStr = msgStr.substr(dataIndex+1);
			dealSyncHeart(msgId, dataStr, addrInfo);
		}
		break;

		case 11:
		{
			std::string dataStr = msgStr.substr(dataIndex+1);
			dealQuitReq(msgId, dataStr, addrInfo);
		}
		break;

		case 13:
		{
			std::string dataStr = msgStr.substr(dataIndex+1);
			dealEndReq(msgId, dataStr, addrInfo);
		}
		break;
	}

	return true;

}


bool PVPThread::dealReqPVPMsg(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo)
{
	std::string ::size_type uidIndex = dataStr.find(';');
	if(uidIndex == std::string::npos)
	{
		return false;
	}

	std::string uidStr = dataStr.substr(0, uidIndex);	
	std::stringstream mystream;
	mystream << uidStr;
	unsigned long long uid;
	mystream >> uid;
	//�ж�������pvp�����ֱ������
	std::map<unsigned long long , PVPData>::iterator pvpfind =  m_mapPVPIng.find(uid);
	if(pvpfind != m_mapPVPIng.end())
	{
		//������Ϣ���ͻ��ˣ���ʾ�յ�����
		// "#AF;101;uuid;^"
		char resp[] = "#AF;2;^";
		int respLen = strlen(resp);

		sendto(m_nSockServer, resp, respLen, 0, (sockaddr*)&addrInfo,  sizeof(sockaddr_in));

		return false;
	}

	//�ж�������pvp�ȴ������ֱ������
	std::map<unsigned long long , PVPData>::iterator waitfind =  m_mapWaitForPVP.find(uid);
	if(waitfind != m_mapWaitForPVP.end())
	{
		//������Ϣ���ͻ��ˣ���ʾ�յ�����,��ʱpvp�ȴ��������и����
		// "#AF;101;uuid;^"
		char resp[] = "#AF;2;^";
		int respLen = strlen(resp);

		sendto(m_nSockServer, resp, respLen, 0, (sockaddr*)&addrInfo,  sizeof(sockaddr_in));

		return false;
	}

	


	//����map�ҵ�����id����ȵ��������
	std::map<unsigned long long , PVPData >::iterator  waitIter  ;
	for( waitIter = m_mapWaitForPVP.begin(); waitIter != m_mapWaitForPVP.end();
		waitIter ++ )
	{
		if(waitIter->first != uid)
		{
			break;
		}
		
	}
	
	int nLen1 = sizeof(sockaddr_in);
	int nLen2 = sizeof(sockaddr_in);
	
	//������Ϣ���ͻ��ˣ���ʾ�յ�����
	// "#AF;101;uuid;^"
	char resp[] = "#AF;2;^";
	int respLen = strlen(resp);

	if (-1 == sendto(m_nSockServer, resp, respLen, 0, (sockaddr*)&addrInfo, nLen1))
	{
					#ifdef WIN32
					LOG_INFO ("send to connect user  data failed.\n", WSAGetLastError());
					#endif

					#ifdef __linux__
					 LOG_INFO ("send to connect user  data failed.\n", errno);
					#endif

					 cout << resp << endl;
					return	false;
	}


	//û���ҵ�ƥ������
	if(waitIter == m_mapWaitForPVP.end() )
	{
		struct sockaddr_in  otherInfo;
		memset(&otherInfo, 0, sizeof(struct sockaddr_in));
		
		PVPData pvpData;
		pvpData.mSelfUid = uid;
		pvpData.mOtherUid = 0;
		pvpData.mLastTime = time(0);
		pvpData.mLost = 0;
		pvpData.mGiveUp = 0;
		pvpData.mEndUp = 0;
		pvpData.mSelfAddr= addrInfo;
		pvpData.mOtherAddr = otherInfo;
		pvpData.mTimeOut = 0;
		pvpData.mLastCheck = 0;
	

		m_mapWaitForPVP.insert(pair<unsigned long long , PVPData>(uid,pvpData));
		cout << "player insert into waitmap:  " << uid <<endl;
		
		return true;
	}

	LOG_INFO ("peer user ip=[%s] port=[%d]\n", inet_ntoa(waitIter->second.mSelfAddr.sin_addr), 
		htons(waitIter->second.mSelfAddr.sin_port));

	//ƥ��ɹ�������Ϣ����ͬ���
	// "#AF;3;uuid1,ip1,port1|uuid2,ip2,port2;^"
	std::string portStr;
	char * addrChar = inet_ntoa(addrInfo.sin_addr);		
	int addrPort = htons(addrInfo.sin_port);
	mystream.clear();
	mystream.str("");
	mystream << addrPort;
	mystream >> portStr;
	std::string addrStr(addrChar);
	std::string matchStr = "#AF;3;" + uidStr + "," +  addrStr + "," + portStr + "|";

	std::string portStr2;
	mystream.clear();
	mystream.str("");
	char * addrChar2 = inet_ntoa(waitIter->second.mSelfAddr.sin_addr);
	int addrPort2 = htons(waitIter->second.mSelfAddr.sin_port);
	mystream << addrPort2;
	mystream >> portStr2;
	std::string addrStr2(addrChar2);
	mystream.clear();
	mystream.str("");
	std::string uidStr2;
	mystream << waitIter->first; 
	mystream >> uidStr2;

	matchStr += (uidStr2 + "," + addrStr2 + "," + portStr2 + ";^");
	cout <<matchStr <<endl;
	if (-1 == sendto(m_nSockServer, matchStr.c_str(), matchStr.length(), 0, (sockaddr*)&(waitIter->second.mSelfAddr), nLen2))
			{
					#ifdef WIN32
				 	LOG_INFO ("send to peer user  data failed.\n", WSAGetLastError());
					#endif

					#ifdef __linux__
					 LOG_INFO ("send to peer user  data failed.\n", errno);
					#endif

					
					return	false;
			}
		 cout << matchStr << endl;
		if (-1 == sendto(m_nSockServer, matchStr.c_str(), matchStr.length(), 0, (sockaddr*)&addrInfo, nLen1))
		{
					#ifdef WIN32
					LOG_INFO ("send to connect user  data failed.\n", WSAGetLastError());
					#endif

					#ifdef __linux__
					 LOG_INFO ("send to connect user  data failed.\n", errno);
					#endif

					 
					return	false;
		}
		 cout << matchStr << endl;
	

		//�Է���pvp��Ϣ
		PVPData pvpingData;
		pvpingData.mSelfUid = waitIter->second.mSelfUid;
		pvpingData.mLastTime = waitIter->second.mLastTime;
		pvpingData.mSelfAddr = waitIter->second.mSelfAddr;
		pvpingData.mLost = 0;
		pvpingData.mEndUp = 0;
		pvpingData.mGiveUp = 0;
		pvpingData.mOtherUid = uid;
		pvpingData.mOtherAddr = addrInfo;

			pvpingData.mTimeOut = 0;
		pvpingData.mLastCheck = 0;
		//���Է���������pvp��map��
		m_mapPVPIng.insert(pair<unsigned long long , PVPData>(waitIter->second.mSelfUid,pvpingData));

		//�Լ���pvp��Ϣ
		PVPData selfPVPData;
		selfPVPData.mSelfUid = uid;
		selfPVPData.mLastTime = time(0);
		selfPVPData.mSelfAddr = addrInfo;
		selfPVPData.mLost = 0;
		selfPVPData.mGiveUp = 0;
		selfPVPData.mEndUp = 0;
		selfPVPData.mOtherUid = waitIter->second.mSelfUid;
		selfPVPData.mOtherAddr = waitIter->second.mSelfAddr;
			selfPVPData.mTimeOut = 0;
		selfPVPData.mLastCheck = 0;
		//���Լ���uid���Լ���pvpdata����pvpmap��

		m_mapPVPIng.insert(pair<unsigned long long , PVPData>(uid,selfPVPData));
		
		//���waitIter�ӵȴ��������Ƴ�
		m_mapWaitForPVP.erase(waitIter);
	return true;
}

bool PVPThread:: dealGiveUp(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo)
{
	std::string ::size_type uidIndex = dataStr.find(';');
	if(uidIndex == std::string::npos)
	{
		return false;
	}

	std::string uidStr = dataStr.substr(0, uidIndex);	
	std::stringstream mystream;
	mystream << uidStr;
	unsigned long long uid;
	mystream >> uid;
	
	std::map<unsigned long long , PVPData >::iterator  waitIter ;
	for( waitIter = m_mapWaitForPVP.begin(); waitIter != m_mapWaitForPVP.end();
		waitIter ++ )
	{
		if(waitIter->first == uid)
		{
			break;
		}
		
	}
		
	//������Ϣ���ͻ��ˣ���ʾ�յ�����
		// "#AF;101;uuid;^"
		char resp[] = "#AF;6;^";
		int respLen = strlen(resp);
		int nLen1 = sizeof(struct sockaddr_in);

		if (-1 == sendto(m_nSockServer, resp, respLen, 0, (sockaddr*)&addrInfo, nLen1))
		{
					#ifdef WIN32
					LOG_INFO ("send to connect user  data failed.\n", WSAGetLastError());
					#endif

					#ifdef __linux__
					 LOG_INFO ("send to connect user  data failed.\n", errno);
					#endif

					
					return	false;
		}

		 cout << resp <<endl;
	//�ҵ�ƥ������
	if(waitIter != m_mapWaitForPVP.end() )
	{
		LOG_INFO ("peer user ip=[%s] port=[%d]\n", inet_ntoa(waitIter->second.mSelfAddr.sin_addr), htons(waitIter->second.mSelfAddr.sin_port));
		
		//�ҵ��ˣ���ô�����ӵȴ�������ɾ��
		m_mapWaitForPVP.erase(waitIter);
	}

	
	

	return true;
}

bool PVPThread::dealSyncHeart(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo)
{
	std::string ::size_type uidIndex = dataStr.find(';');
	if(uidIndex == std::string::npos)
	{
		return false;
	}

	std::string uidStr = dataStr.substr(0, uidIndex);	
	std::stringstream mystream;
	mystream << uidStr;
	unsigned long long uid;
	mystream >> uid;

	dataStr = dataStr.substr(uidIndex + 1);
	std::string ::size_type seqIndex = dataStr.find(';');
	std::string seqStr = dataStr.substr(0, seqIndex);
	mystream.clear();
	mystream.str("");
	int seq = 0;
	mystream << seqStr ;
	mystream >> seq;
	
	dealSyncHeartPVPIng(uid, seqStr, addrInfo);
	dealSyncHeartWaitPVP(uid, seqStr, addrInfo);
	return true;


	
}


void PVPThread::dealSyncHeartPVPIng(UInt64 uid, std::string seqStr,struct sockaddr_in  addrInfo)
{
	std::map<unsigned long long , PVPData >::iterator findIter = m_mapPVPIng.find(uid);
	if(findIter == m_mapPVPIng.end())
	{
		return ;
	}

	//��ʱ������Ϊ0
	findIter->second.mTimeOut = 0;

	findIter->second.mLastTime = time(0);
		//��ӻذ�
		//<<<< heartbeat ack 					;seqNumber;^
		std::string respStr = "#AF;8;" + seqStr+ ";^";

		if (-1 == sendto(m_nSockServer, respStr.c_str(), respStr.length(), 0, (sockaddr*)&(addrInfo),  sizeof(struct sockaddr_in)))
		{
					#ifdef WIN32
					LOG_INFO ("send to connect user  data failed.\n", WSAGetLastError());
					#endif

					#ifdef __linux__
					 LOG_INFO ("send to connect user  data failed.\n", errno);
					#endif

					
					return	;
		}
		 cout << respStr <<endl;
}

void PVPThread::dealSyncHeartWaitPVP(UInt64 uid, std::string seqStr,struct sockaddr_in  addrInfo)
{
	std::map<unsigned long long , PVPData >::iterator findIter = m_mapWaitForPVP.find(uid);
	if(findIter == m_mapWaitForPVP.end())
	{
		return ;
	}

	//��ʱ������Ϊ0
	findIter->second.mTimeOut = 0;

	findIter->second.mLastTime = time(0);
		//��ӻذ�
		//<<<< heartbeat ack 					;seqNumber;^
	std::string respStr = "#AF;8;" + seqStr+ ";^";

	if (-1 == sendto(m_nSockServer, respStr.c_str(), respStr.length(), 0, (sockaddr*)&(addrInfo),  sizeof(struct sockaddr_in)))
	{
			#ifdef WIN32
				LOG_INFO ("send to connect user  data failed.\n", WSAGetLastError());
			#endif

			#ifdef __linux__
				 LOG_INFO ("send to connect user  data failed.\n", errno);
			#endif

					
			return	;
	}
	
	cout << respStr <<endl;
}



bool PVPThread::dealQuitReq(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo)
{
	std::string ::size_type uidIndex = dataStr.find(';');
	if(uidIndex == std::string::npos)
	{
		return false;
	}

	std::string uidStr = dataStr.substr(0, uidIndex);	
	std::stringstream mystream;
	mystream << uidStr;
	unsigned long long uid;
	mystream >> uid;

	dataStr = dataStr.substr(uidIndex + 1);
	std::string ::size_type giveupIndex = dataStr.find(';');
	std::string giveupStr = dataStr.substr(0, giveupIndex);
	mystream.clear();
	mystream.str("");
	int giveUpUid = 0;
	mystream << giveupStr ;
	mystream >> giveUpUid;

	//������͵ķ������uid������ֱ�ӷ���
	std::map<unsigned long long , PVPData >::iterator findIter = m_mapPVPIng.find(giveUpUid);
	if(findIter == m_mapPVPIng.end())
	{
		return false;
	}
	//��������������Ҳ���������ôֱ�ӷ���
	std::map<unsigned long long , PVPData >::iterator selfIter = m_mapPVPIng.find(uid);
	if(selfIter == m_mapPVPIng.end() )
	{
		return false;
	}
	
	std::map<unsigned long long , PVPData >::iterator otherIter = m_mapPVPIng.find(selfIter->second.mOtherUid);
	if(otherIter == m_mapPVPIng.end() )
	{
		return false;
	}


	//���ڿͻ�����������ս�����޸����󷢳��Ŀͻ���uid��Ӧ��PVP�ֶ��з������λ
	//�����ĸ��ͻ��˷����������󣬷������λ������Ϊͬһ��������ud
	selfIter->second.mGiveUp = giveUpUid;
	//��ӻذ�
	
	//<<<< heartbeat ack 					;seqNumber;^
	std::string respStr = "#AF;12" + giveupStr+ ";^";

    sendto(m_nSockServer, respStr.c_str(), respStr.length(), 0, (sockaddr*)&(selfIter->second.mSelfAddr),  sizeof(struct sockaddr_in));
	cout <<"give up resp " << respStr <<endl;
	//�жϷ������Ƿ��յ�PVP�ĶԷ��ķ�����������յ��ˣ���ô����notify���߰����������Ϸ����

	if(!(otherIter->second.mGiveUp))
	{
		return true;
	}

	std::string notifyStr = "#AF;9" + giveupStr+ ";^";
	
	if(giveUpUid == uid)
	{
		//������������ҷ���������Ҫ������������ҷ��͵ھŸ���
		sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mOtherAddr),  sizeof(struct sockaddr_in));
	}
	else
	{
		//��������������Ҳ������������������ң���ô�������������ҷ���notify�ھŰ�
		sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mSelfAddr),  sizeof(struct sockaddr_in));
	}
	
	cout << "notify lost connection   " << notifyStr << endl;
	return true;
}

void PVPThread::checkTimeOut()
{
		long long int curtime = time(0);
		checkPVPIngTimeOut(curtime);

		checkWaitPVPTimeOut(curtime);

}

//���pvp�����г�ʱԪ��
void PVPThread::checkPVPIngTimeOut(UInt64 curtime)
{
	std::stringstream mystream;
	
		std::map<unsigned long long , PVPData>::iterator pvpIter = m_mapPVPIng.begin();
		while(pvpIter != m_mapPVPIng.end())
		{
			if(pvpIter->second.mLost == 1)
			{
				//�ж϶Է��Ƿ�Ҳ�Ѿ�����
				std::map<unsigned long long , PVPData>::iterator	otherIter = m_mapPVPIng.find(pvpIter->second.mOtherUid);
				if(otherIter == m_mapPVPIng.end() || otherIter->second.mLost == 1)
				{
					m_mapPVPIng.erase(pvpIter++);
				}
				else
				{
					pvpIter++;
				}
				continue;
			}
			
			//��ʱ��������3�Σ�����֪ͨ��
			if(pvpIter->second.mTimeOut >= 3)
			{
				//����ҵĶ�ʧ��������Ϊ1
				pvpIter->second.mLost = 1;
				std::string uidStr;
				mystream.clear();
				mystream.str("");
				mystream << pvpIter->first ;
				mystream >> uidStr;
				//#AF;9;lostUUID;^
				std::string notifyStr = "#AF;9;" + uidStr + ";^";
				int respLen = notifyStr.length();
				
				//�͵��ߵ����pk����ҵ�iterator
				std::map<unsigned long long , PVPData>::iterator otherIter = m_mapPVPIng.find(pvpIter->second.mOtherUid);
				if(otherIter == m_mapPVPIng.end())
				{
					 m_mapPVPIng.erase(pvpIter++);
					continue;
				}

				if (-1 == sendto(m_nSockServer, notifyStr.c_str(), respLen, 0, (sockaddr*)&( otherIter->second.mSelfAddr),  sizeof(struct sockaddr_in) ))
				{
					cout << "send notifystr failed !!!:    " << uidStr << endl;
				}
				cout << "notify other player uid : " << uidStr << "has lost!!!" <<endl;
				//����������
				pvpIter++;
				continue;
			}

			//�ж��Ƿ񳬹�2�룬����2�룬ÿ��2s��������
			if(curtime - (pvpIter->second.mLastTime) > 2 && curtime - (pvpIter->second.mLastCheck) > 2)
			{
				//���±��μ��ʱ�䣬���ҳ�ʱ��������
				pvpIter->second.mLastCheck = curtime;
				pvpIter->second.mTimeOut++;

			}
		
			pvpIter++;
		}
}
		
void PVPThread::checkWaitPVPTimeOut(UInt64 curtime)
{
	std::stringstream mystream;
	
		std::map<unsigned long long , PVPData>::iterator pvpIter = m_mapWaitForPVP.begin();
		while(pvpIter != m_mapWaitForPVP.end())
		{
			if(pvpIter->second.mLost == 1)
			{
				//�ȴ��������������
				m_mapWaitForPVP.erase(pvpIter++);
				continue;
			}
			
			//��ʱ��������3�Σ�����֪ͨ��
			if(pvpIter->second.mTimeOut >= 3)
			{
				//����ҵĶ�ʧ��������Ϊ1
				pvpIter->second.mLost = 1;
				std::string uidStr;
				mystream.clear();
				mystream.str("");
				mystream << pvpIter->first ;
				mystream >> uidStr;
				//#AF;9;lostUUID;^
				std::string notifyStr = "#AF;9;" + uidStr + ";^";
				int respLen = notifyStr.length();
				
			
				cout << "notify other player uid : " << uidStr << "has lost!!!" <<endl;
				//����������
				pvpIter++;
				continue;
			}

			//�ж��Ƿ񳬹�5�룬����2�룬ÿ��2s��������
			if(curtime - (pvpIter->second.mLastTime) > 5 && curtime - (pvpIter->second.mLastCheck) > 2)
			{
				//���±��μ��ʱ�䣬���ҳ�ʱ��������
				pvpIter->second.mLastCheck = curtime;
				pvpIter->second.mTimeOut++;

			}
		
			pvpIter++;
		}
}



bool PVPThread::dealEndReq(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo)
{
	std::string ::size_type uidIndex = dataStr.find(';');
	if(uidIndex == std::string::npos)
	{
		return false;
	}

	std::string uidStr = dataStr.substr(0, uidIndex);	
	std::stringstream mystream;
	mystream << uidStr;
	unsigned long long uid;
	mystream >> uid;

	//��������������Ҳ���������ôֱ�ӷ���
	std::map<unsigned long long , PVPData >::iterator selfIter = m_mapPVPIng.find(uid);
	if(selfIter == m_mapPVPIng.end() )
	{
		return false;
	}
	
	//�ͻ��˷��ͽ���������ô�ͽ����ͷ���Ӧ��pvp���ݽ��������Ϊ�Լ���uid
	selfIter->second.mEndUp = uid;
	//��ӻذ�
	
	//<<<< heartbeat ack 					;seqNumber;^
	std::string respStr = "#AF;14;^";

    sendto(m_nSockServer, respStr.c_str(), respStr.length(), 0, (sockaddr*)&(selfIter->second.mSelfAddr),  sizeof(struct sockaddr_in));
	cout << "resp #AF;14;^" << endl;
	//�ж�pvp����һ���Ƿ�Ҳ���͹��������󣬲��ҷ������Ѿ�������λ������һ������
	std::map<unsigned long long , PVPData >::iterator otherIter = m_mapPVPIng.find(selfIter->second.mOtherUid);
	if(otherIter == m_mapPVPIng.end() )
	{
		return false;
	}
	
	//�Է��Ѿ����ڵ���״̬
	if(otherIter->second.mLost)
	{
		//��˫��������ս�������ذ�
		//#AF;15;resultContentString;^
		std::string notifyStr = "#AF;15;resultContentString;^";
		sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mOtherAddr),  sizeof(struct sockaddr_in));
		sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mSelfAddr),  sizeof(struct sockaddr_in));
		cout << "notify #AF;15;resultContentString;^" <<endl;
		return true;
	}

	//�ж�PVP�ĶԷ��Ƿ�Ҳ�յ��˶Է��İ�������յ��ˣ���ô����notify��������
	if(!(otherIter->second.mEndUp))
	{
		return true;
	}

	std::string notifyStr = "#AF;15;resultContentString;^";
	sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mOtherAddr),  sizeof(struct sockaddr_in));
	sendto(m_nSockServer, notifyStr.c_str(), notifyStr.length(), 0, (sockaddr*)&(selfIter->second.mSelfAddr),  sizeof(struct sockaddr_in));
	cout << "notify #AF;15;resultContentString;^" <<endl;
	return true;
	

}