#ifndef __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum LoggerMsg
	{
// 		LOGGER_SERVERDATA_REQ				   	   = 1,		//��������
		LOGGER_PLAYER_LOGIN_REQ				   	   = 2,		//��ҵ�½log
		LOGGER_PLAYER_LOGOUT_REQ				   = 3,		//�������log

	};

	enum LogTyep
	{
	
		LOG_LOGIN			=1,//��¼
		LOG_LOGOUT			=2,//����
	};


	struct LoggerPlayerTypeLogReq
	{
		UInt32 serverID ;//���������
		UInt64 playerID ;//��ɫid
		std::string name ;//��ɫ����	
		UInt32 type ;//LogType ����
		std::string explain ;//˵��
		UInt64 accountID ;//�˺�id
		std::string tableName;// player_login_20160720

		LoggerPlayerTypeLogReq():accountID(1),playerID(0),name(""),type(0),explain(""),serverID(0),tableName(""){}

		LYNX_MESSAGE_7(LOGGER_PLAYER_LOGIN_REQ, LoggerPlayerTypeLogReq,UInt64,accountID,UInt64, playerID,std::string, name,UInt32, type,std::string, explain,UInt32, serverID,std::string, tableName);
	};

	struct LoggerPlayerLogoutReq
	{
		UInt32 serverID ;//���������
		UInt64 playerID ;//��ɫid
		std::string name ;//��ɫ����		
		UInt64 accountID ;//�˺�id
		std::string tableName;// player_login_20160720

		LoggerPlayerLogoutReq():accountID(1),playerID(0),name(""),serverID(0),tableName(""){}

		LYNX_MESSAGE_5(LOGGER_PLAYER_LOGOUT_REQ, LoggerPlayerLogoutReq,UInt64,accountID,UInt64, playerID,std::string, name,UInt32, serverID,std::string, tableName);
	};

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LOGGER_THREAD_MSG_H__