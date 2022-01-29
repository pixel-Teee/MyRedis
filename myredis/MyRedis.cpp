#include "pch.h"
#include "MyRedis.h"

RedisBase* NewRedis()
{
	return new MyRedis();
}

void MyRedis::ClearContext()
{
	if(context != nullptr) redisFree(context);
	context = nullptr;
}

MyRedis::~MyRedis()
{
	Clear();
	ClearContext();
}

int MyRedis::Connect(const char* ip, int port)
{
	Clear();
	ClearContext();

	memset(redisip, 0, 20);
	memcpy(redisip, ip, 20);

	redisport = port;

	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	context = redisConnectWithTimeout(redisip, redisport, tv);
	if (context == nullptr)
	{
		printf("redis connect context==null\n");
		ClearContext();
		return -1;
	}
	if (context->err != 0)
	{
		printf("redis connect error: %s\n", context->errstr);
		ClearContext();
		return -2;
	}
	printf("redis connect successful [%s:%d] \n", redisip, redisport);
	return 0;
}

int MyRedis::ReConnect()
{
	if(IsConnect()) return 0;

	Clear();
	ClearContext();

	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	context = redisConnectWithTimeout(redisip, redisport, tv);
	if (context == nullptr)
	{
		printf("redis connect context==null\n");
		ClearContext();
		return -1;
	}
	if (context->err != 0)
	{
		printf("redis connect error: %s\n", context->errstr);
		ClearContext();
		return -2;
	}
	printf("redis connect successful [%s:%d] \n", redisip, redisport);
	return 1;
}

bool MyRedis::IsConnect()
{
	if(context == nullptr) return false;
	if(context->err != 0) return false;
	return true;
}

int MyRedis::RedisCommand(const char* cmd)
{
	if (ReConnect() != 0)
	{
		printf("redis connect filed! \n");
		return -1;
	}
	Clear();
	reply = (redisReply*)redisCommand(context, cmd);
	if (reply == nullptr)
	{
		printf("redis disconnect...1 \n");
		Clear();
		ClearContext();

		//RedisCommand(cmd);
		return -1;
	}
	if (!IsConnect())
	{
		printf("redis disconnect...2 \n");
	}

	//²éÑ¯½á¹û
	switch (reply->type)
	{
	
	}
	return 0;
}

void MyRedis::Clear()
{
	if(reply != nullptr) freeReplyObject(reply);
	reply = nullptr;
}

char* MyRedis::value(int index, int& length)
{
	return nullptr;
}
