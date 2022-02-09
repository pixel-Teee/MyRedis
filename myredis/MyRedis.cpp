#include "pch.h"
#include "MyRedis.h"

#include <iostream>

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

	//查询结果
	switch (reply->type)
	{
	case REDIS_REPLY_STATUS://返回状态一般是设置语句 比如set hmset
		//查询成功
		if (strcasecmp(reply->str, "ok") == 0)
		{
			std::cout << "RedisCommand [" << cmd << "] is successfully..." << std::endl;
			return 0;
		}
		if (strcasecmp(reply->str, "pong") == 0)
		{
			std::cout << "RedisCommand [" << cmd << "] is successfully..." << std::endl;
			return 0;
		}
		break;
	case REDIS_REPLY_ARRAY://查询 hmset
		std::cout << "RedisCommand [" << cmd << "] is successfully..." << std::endl;
		return 0;
	case REDIS_REPLY_INTEGER://hset hdel
		std::cout << "RedisCommand [" << cmd << "] is successfully..." << std::endl;
		return 0;
	}

	std::cout << "RedisCommand [" << cmd << "] is failed" << std::endl;
	std::cout << "err [" << reply->type << "] is failed" << std::endl;
	std::cout << "err [" << reply->str << "] is failed" << std::endl;
	return -1;
}

void MyRedis::Clear()
{
	if(reply != nullptr) freeReplyObject(reply);
	reply = nullptr;
}

char* MyRedis::value(int index, int& length)
{
	//empty string, when error or out of range, then return the empty string
	char s[1] = "";
	if(reply == nullptr) return s;
	//elements: element's num
	if(index >= reply->elements) return s;

	auto data = reply->element[index];
	if(data == nullptr) return s;

	length = data->len;
	if(length == 0 || data->str == nullptr || data->type == REDIS_REPLY_NIL) return s;

	return data->str;
}
