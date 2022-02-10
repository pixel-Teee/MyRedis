#pragma once
#include "RedisBase.h"

#include "hiredis\hiredis.h"
#define NO_QFORKIMPL//这一行必须加这行才能正常使用
#include "Win32_Interop\win32fixes.h"

#pragma comment(lib, "hiredis.lib")
#pragma comment(lib, "Win32_Interop.lib")

using namespace std;

class MyRedis : public RedisBase
{
private:
	redisContext *context;//连接内容
	redisReply* reply;//回复内容
	char redisip[20];//连接IP
	int redisport;//端口号
	void ClearContext();
public:
	virtual ~MyRedis();
	virtual int Connect(const char* ip, int port);//连接
	virtual int ReConnect();//重新连接，自动连接
	virtual bool IsConnect();

	virtual int RedisCommand(const char* cmd);
	virtual void Clear();
	virtual char* value(int index, int& length);

	virtual void* getReply();
};
