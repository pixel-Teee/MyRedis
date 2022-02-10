#pragma once
#include "RedisBase.h"

#include "hiredis\hiredis.h"
#define NO_QFORKIMPL//��һ�б�������в�������ʹ��
#include "Win32_Interop\win32fixes.h"

#pragma comment(lib, "hiredis.lib")
#pragma comment(lib, "Win32_Interop.lib")

using namespace std;

class MyRedis : public RedisBase
{
private:
	redisContext *context;//��������
	redisReply* reply;//�ظ�����
	char redisip[20];//����IP
	int redisport;//�˿ں�
	void ClearContext();
public:
	virtual ~MyRedis();
	virtual int Connect(const char* ip, int port);//����
	virtual int ReConnect();//�������ӣ��Զ�����
	virtual bool IsConnect();

	virtual int RedisCommand(const char* cmd);
	virtual void Clear();
	virtual char* value(int index, int& length);

	virtual void* getReply();
};
