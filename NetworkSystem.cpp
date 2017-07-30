#include "NetworkSystem.h"
#include "ConfigParser.h"

#ifdef _WIN32
#include <winsock.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

using namespace Lynx;

NetworkSystem::NetworkSystem() : mConnIdIndex(1), mLastCheckClientConnectionKeepaliveTime(0)
{

}

NetworkSystem::~NetworkSystem()
{

}

bool 
NetworkSystem::initial()
{
	// ע���¼�
	REGISTER_EVENT(EVENT_CLIENT_CONNECTION_CLOSE, &NetworkSystem::onClientConnectionClosed);

	// ע���߳���Ϣ
	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionAcceptedNotify, NetworkSystem::onConnectionAcceptedNotify);
	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionReceiveNotify, NetworkSystem::onConnectionReceiveNotify);
	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionBrokenNotify, NetworkSystem::onConnectionBrokenNotify);

	// ע��������Ϣ
	ClientConnection::registerMsgs();

    String mainListenIp = ConfigParser::getSingleton().getMainListenIp();
    UInt16 mainListenPort = ConfigParser::getSingleton().getMainListenPort();
    String gmListenIp = ConfigParser::getSingleton().getGMListenIp();
    UInt16 gmListenPort = ConfigParser::getSingleton().getGMListenPort();

    if (!mListenerWorker.initial(0, mainListenIp, mainListenPort, gmListenIp, gmListenPort))
    {
        LOG_WARN("Failed to initial listener thread worker");
        return false;
    }

    for (UInt32 i = 0; i < NETWORK_THREAD_WORKER_COUNT; ++i)
    {
        if (!mNetworkWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial network thread worker [%u]", i);
            return false;
        }
    }

	gCanMsgRecived = true;

	LOG_INFO("Initial NetworkSystem");
	return true;
}

void 
NetworkSystem::release()
{
    for (Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.begin();
        iter != NULL; iter = mClientConnectionMap.next(iter))
    {
        ClientConnection* clientConnection = iter->mValue;
        if (clientConnection)
        {
            ConnectionCloseNotify notify;
            notify.mConnId = iter->mKey;
            postThreadMsgToNetworkWorker(notify, iter->mKey);
            clientConnection->release();
            XDELETE(clientConnection);
        }
    }
    mClientConnectionMap.clear();

    mListenerWorker.release();

    for (UInt32 i = 0; i < NETWORK_THREAD_WORKER_COUNT; ++i)
    {
        mNetworkWorkerArray[i].release();
    }

	// ע���¼�
	DEREGISTER_EVENT(EVENT_CLIENT_CONNECTION_CLOSE, &NetworkSystem::onClientConnectionClosed);

	// ע���߳���Ϣ
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionAcceptedNotify);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionReceiveNotify);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionBrokenNotify);
	
	// ע��������Ϣ
	ClientConnection::deregisterMsgs();
	LOG_INFO("Release NetworkSystem");
}

void 
NetworkSystem::update(const UInt64& accTime)
{
	// ListenWorker
	{
		StreamBuffer& threadMsgStream = mListenerWorker.getOutputMsgQueue().getMsgs();
		UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
		void* threadMsgPointer;
		ThreadMsg* threadMsg;
		for (UInt32 i = 0; i < msgCount; ++i)
		{
			threadMsgStream >> threadMsgPointer;
			threadMsg = (ThreadMsg*)threadMsgPointer;
			if (threadMsg)
			{
				dispatchThreadMsg(*threadMsg);
				deallocThreadMsg(threadMsg);
			}
		}
	}	

	// NetworkWorker
	for (UInt32 i = 0; i < NETWORK_THREAD_WORKER_COUNT; ++i)
	{
		StreamBuffer& threadMsgStream = mNetworkWorkerArray[i].getOutputMsgQueue().getMsgs();
		UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
		void* threadMsgPointer;
		ThreadMsg* threadMsg;
		for (UInt32 i = 0; i < msgCount; ++i)
		{
			threadMsgStream >> threadMsgPointer;
			threadMsg = (ThreadMsg*)threadMsgPointer;
			if (threadMsg)
			{
				dispatchThreadMsg(*threadMsg);
				deallocThreadMsg(threadMsg);
			}
		}
	}

	
    // ���ͻ���������Ч��
    if (accTime - mLastCheckClientConnectionKeepaliveTime > 30 * 1000)
    {
        mLastCheckClientConnectionKeepaliveTime = accTime;
        for (Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.begin();
            iter != mClientConnectionMap.end(); )
        {
			
            ClientConnection* clientConnection = iter->mValue;
            if (clientConnection)
            {
                UInt64 keepaliveTime = clientConnection->getKeepaliveTime();

			    Int64 invTime =	accTime - keepaliveTime ;			
				
				//һ��Сʱû��������Ϣ�����ڸ�Ϊ��ʮ���ӡ�
                if ( invTime >3600*1000)
                {
                   pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, clientConnection->getConnId());
					LOG_INFO("3600s not recieve mConnId =[%d]  ", clientConnection->getConnId());
					LOG_INFO("keepaliveTime is: %llu", keepaliveTime);
					LOG_INFO("accTime is: %llu", accTime);
					LOG_INFO("minus time is: %llu ", (accTime-keepaliveTime)/1000 );
                }

                iter = mClientConnectionMap.next(iter);
            }
            else
            {
                iter = mClientConnectionMap.erase(iter);
            }
        }
    }
	
}

String 
NetworkSystem::getClientConnectionFromIp(const ConnId& connId)
{
    ClientConnection* clientConnection = getClientConnection(connId);
    if (!clientConnection)
    {
        return "";
    }

    return clientConnection->getFromIp();
}

UInt16 
NetworkSystem::getClientConnectionFromPort(const ConnId& connId)
{
    ClientConnection* clientConnection = getClientConnection(connId);
    if (!clientConnection)
    {
        return 0;
    }

    return clientConnection->getFromPort();
}

void 
NetworkSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}

UInt64 
NetworkSystem::createLoginToken(const ConnId& connId)
{
    ClientConnection* clientConnection = getClientConnection(connId);
    if (clientConnection == NULL)
    {
        LOG_DEBUG("Failed to get client connection with connId %llu", connId);
        return 0;
    }
    String fromIp = clientConnection->getFromIp();
    UInt16 fromPort = clientConnection->getFromPort();
    UInt32 ip = inet_addr(fromIp.c_str());

    return GEN_LOGIN_TOKEN(ip, fromPort, connId % 65500);
}

void 
NetworkSystem::onConnectionAcceptedNotify(ConnectionAcceptedNotify& msg)
{
	TcpConnection* tcpConnection = (TcpConnection*)msg.mConnPointer;
	if (!tcpConnection)
	{
		LOG_WARN("Tcp connection pointer is NULL.");
		return;
	}

	//����connId
	ConnId connId = mConnIdIndex++;
	//�������Ӵ����󣬽����Ӱ���NetworkWorker��
	ConnectionOpenNotify connectionOpenNotify;
	connectionOpenNotify.mConnId = connId;
	connectionOpenNotify.mConnPointer = tcpConnection;
	postThreadMsgToNetworkWorker(connectionOpenNotify, connId);

	//�����ӷ���NetWorkSystem�ļ�����
	ClientConnection* clientConnection = XNEW(ClientConnection)();
	ASSERT(clientConnection);
	clientConnection->mConnId = connId;
    clientConnection->mType = msg.mType;
    clientConnection->mFromIp = tcpConnection->getFromIp();
    clientConnection->mFromPort = tcpConnection->getFromPort();

	mClientConnectionMap.insert(connId, clientConnection);	
}

void 
NetworkSystem::onConnectionReceiveNotify(ConnectionReceiveNotify& msg)
{
	Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.find(msg.mConnId);
	if (iter == NULL)
	{
		LOG_WARN("Client connection %llu don't exist", msg.mConnId);
		for (Vector<void*>::iterator jter = msg.mNodeList.begin();
			jter != msg.mNodeList.end(); ++jter)
		{
			XDELETE((StreamBuffer::Node*)(*jter));
		}
		ConnectionCloseNotify notify;
		notify.mConnId = msg.mConnId;
		postThreadMsgToNetworkWorker(notify, msg.mConnId);
		return;
	}

	ClientConnection* clientConnection = iter->mValue;
	if (!clientConnection)
	{
		for (Vector<void*>::iterator jter = msg.mNodeList.begin();
			jter != msg.mNodeList.end(); ++jter)
		{
			XDELETE((StreamBuffer::Node*)(*jter));
		}
		ConnectionCloseNotify notify;
		notify.mConnId = msg.mConnId;
		postThreadMsgToNetworkWorker(notify, msg.mConnId);
		return;
	}

	for (Vector<void*>::iterator i = msg.mNodeList.begin();
		i != msg.mNodeList.end(); ++i)
	{
		StreamBuffer::Node* node = (StreamBuffer::Node*)(*i);
		if (node)
		{
			clientConnection->mIStream.pushNode(node);
		}
		else
		{
			LOG_WARN("Logic error for stream buffer node is NULL.");
            pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, msg.mConnId);
            return;
		}
	}

    clientConnection->onReceived();
}

void 
NetworkSystem::onConnectionBrokenNotify(ConnectionBrokenNotify& msg)
{
	ConnId connId = msg.mConnId;
	Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.find(connId);
	if (iter == NULL)
	{
		LOG_DEBUG("Client connection isn't exist.");
		return;
	}

	ClientConnection* clientConnection = iter->mValue;
	if (clientConnection)
	{
		clientConnection->mBrokenFlag = true;
	}

	pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
	LOG_INFO("onConnectionBrokenNotify connId =[%d]  ", connId);
}

void NetworkSystem::closeAllConnection()
{
	
	for(Map<ConnId, ClientConnection*>::Iter*  iter = mClientConnectionMap.begin(); iter != NULL; 
		 )
	{
			ClientConnection* clientConnection = iter->mValue;

			if (!clientConnection->mBrokenFlag)
			{
				ConnectionCloseNotify notify;
				notify.mConnId = iter->mKey;
				postThreadMsgToNetworkWorker(notify, notify.mConnId);
			}

			iter = mClientConnectionMap.erase(iter);
			clientConnection->release();
			XDELETE(clientConnection);	
	}

	


	
}

void NetworkSystem::closeConnection(const ConnId& connId)
{
	pushEvent(EVENT_CLIENT_CONNECTION_CLOSE, connId);
	LOG_INFO("onConnectionBrokenNotify connId =[%d]  ", connId);

}


void 
NetworkSystem::onClientConnectionClosed(const Event& ev)
{
	if (ev.mEventArgs.empty())
	{
		LOG_WARN("Event EVENT_CLIENT_CONNECTION_CLOSE args is NULL");
		return;
	}
	ConnId connId = lynxAtoi<ConnId>(ev.mEventArgs[0].c_str());
	Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.find(connId);
	if (iter == NULL)
	{
		LOG_DEBUG("Client connection isn't exist.");
		return;
	}

	ClientConnection* clientConnection = iter->mValue;

	if (!clientConnection->mBrokenFlag)
	{
		ConnectionCloseNotify notify;
		notify.mConnId = connId;
		postThreadMsgToNetworkWorker(notify, connId);
	}

	mClientConnectionMap.erase(iter);
	clientConnection->release();
	XDELETE(clientConnection);	
}

bool NetworkSystem::getCanMsgRecived()
{
	return gCanMsgRecived;

}

void NetworkSystem::setCanMsgRecived(bool val)
{
	gCanMsgRecived = val;
}

void NetworkSystem::sendServerShutDown()
{

	GCClientChatResp resp;
	resp.mPacketID = BOC_CLIENTCHAT_RESP;

	Json::Value root;
	root["channelType"] = SYSTEMMSG;
	root["fromPlayer"] = 0;
	root["chatMsg"] = "game server will shutdown !!!";
	root["modelid"] = 0;
	root["name"] = "";
	root["chattime"] = TimeUtil::getTimeSec();
	root["msgType"] = 1;

	Json::StyledWriter writer;

	resp.mRespJsonStr = writer.write(root);

	cout << resp.mRespJsonStr << endl;

	



	UInt32 time = 0;

	for(UInt32 i = 0;i<3;i++)
	{
		time = (3 - i)*5;
		NetworkSystem::getSingleton().broadcastMsg(resp);
		TimeUtil::sleep(5*1000);
	}
	time = 0;
	NetworkSystem::getSingleton().broadcastMsg(resp);
}