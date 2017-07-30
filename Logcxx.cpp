

#include "Logcxx.h"

#ifdef __linux__

#include<log4cxx/logger.h>
#include<log4cxx/propertyconfigurator.h>

using namespace log4cxx;

#endif

void Logcxx::renameFile(string oldFileName,string newFileName)
{

	char source[256];//�ļ�·��
	char newname[256];

	snprintf(source, sizeof(source), oldFileName.c_str());
	snprintf(newname, sizeof(newname), newFileName.c_str());


	if(!rename(source,newname))//ɾ���ɹ�
	{
		cout<<source<<" �ɹ�������Ϊ: "<<newname<<endl;
	}
	else//�޷�������:�ļ��򿪻���Ȩ��ִ��������
	{
		cout<<"�ļ��޷�������(����ԭ������):"<<endl;
		cout<<"\t"<<"1. "<<newname<<" �Ѵ���"<<endl
			<<"\t"<<"2. "<<newname<<" ����ʹ��,δ�ر�."<<endl
			<<"\t"<<"3. "<<"��û��Ȩ�����������ļ�."<<endl;
	}

}



string Logcxx::FileNameAddDate(string fileName)
{
	// 	log4j.appender.dailyFile.DatePattern='.'yyyy_MM_dd
	// 		log4j.appender.dailyFile.File=./Logs/logfile

	char buf[32] = {0};
	int fineNameLength = 32;
	string newFileName = "";

	TimeUtil::getTimeStr(buf, fineNameLength, TimeUtil::getTimeMilliSec());
	string dateString = (string)buf;
	newFileName = fileName +"."+dateString;

	return newFileName;
}



void Logcxx::initLog4cxx()
{	


#ifdef __linux__
	string fileBaseName = "Logs/logfile";
	Logcxx::renameFile(fileBaseName,Logcxx::FileNameAddDate(fileBaseName));

	// 	 Read configure file

	// �������õ������ļ���"Logs/logfile";
	PropertyConfigurator::configure("Data/log4cxx.properties");

	// 	 Get root logger
	rootLogger = log4cxx::Logger::getRootLogger();

	// 	 LOG4CXX_WARN(rootLogger,"write log 21111111111111111111111111111111111111111111111111111111112222222222222222 ");
	// 
	// 	
	//  	 UInt32 logLevel = LogWarn;
	//  	 string logStr = "first log !!!";
	//  	  log_writeFile(logStr,logLevel);
#endif

}

void Logcxx::log_writeFile(string logStr,UInt32 logLevel)
{

#ifdef __linux__

	if (logLevel == LogInfo)
	{
		LOG4CXX_INFO(rootLogger,logStr.c_str());
	}
	else if (logLevel == LogWarn)
	{
		LOG4CXX_WARN(rootLogger,logStr.c_str());
	}
	else 	if (logLevel == LogError)
	{
		LOG4CXX_ERROR(rootLogger,logStr.c_str());
	}
	else 	if (logLevel == LogFatal)
	{
		LOG4CXX_FATAL(rootLogger,logStr.c_str());
	}

#endif
}

void log_write(char * dest,UInt32 logLevel)
{
	String logStr;
	logStr = (String)dest;
	string str = logStr.c_str();
	Logcxx::getSingleton().log_writeFile(str,logLevel);

}

