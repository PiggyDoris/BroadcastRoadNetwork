#pragma once
class Server
{
public:
	Server();
	~Server();
	void sendBroadcastData();
	void sendIndex();
	void sendData();

private:
	int packageSize;
};

