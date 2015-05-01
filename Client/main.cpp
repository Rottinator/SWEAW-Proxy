#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../../SocketLibrary/SocketLibrary/SocketLibrary.h"

#define GAME_PORT 1234

#define SERVER_BROADCAST_PORT 5001
#define SERVER_DATA_PORT 5002

using namespace std;

UdpClient* localBroadcastClient;
UdpClient* localDataClient;
TcpClient* serverDataClient;
TcpClient* serverBroadcastClient;

char serverIp[30];
char localIp[30];

void ReceiveLocalBroadcast(char*, int, char*, unsigned short);
void ReceiveLocalData(char*, int, char*, unsigned short);

void ReceiveServerBroadcast(char*, int);
void ReceiveServerData(char*, int);

void InitLocalBroadcastClient();
void InitLocalDataClient();
void InitServerBroadcastClient();
void InitServerDataClient();

int main()
{
	//printf("Server-IP: ");
	//scanf("%s", &serverIp);
	//getchar();

	//printf("Lokale-IP: ");
	//scanf("%s", &localIp);
	//getchar();	

	InitSocketLibrary();

	InitLocalBroadcastClient();
	InitLocalDataClient();
	InitServerBroadcastClient();
	InitServerDataClient();	
	
	CleanupSocketLibrary();

	while(true)
	{
		SleepMilliseconds(1000);
	}

	return 0;
}

void ReceiveLocalBroadcast(char* data, int length, char* address, unsigned short port)
{
	serverBroadcastClient->Send(data, length);
	delete data;
	delete address;
}

void ReceiveLocalData(char* data, int length, char* address, unsigned short port)
{
	serverDataClient->Send(data, length);
	delete data;
	delete address;
}

void ReceiveServerBroadcast(char* data, int length)
{
	localDataClient->SendTo(data, length, IpAddress.Broadcast, GAME_PORT);
	delete data;
}

void ReceiveServerData(char* data, int length)
{
	localDataClient->SendTo(data, length, localIp, GAME_PORT);
	delete data;
}

void InitLocalBroadcastClient()
{
	localBroadcastClient = new UdpClient();
	bool success = localBroadcastClient->Bind(IpAddress.Any, GAME_PORT);
	if(success) 
	{
		printf("Local-Broadcast-Client erfolgreich initialisert\n");
		localBroadcastClient->StartReceiveData(ReceiveLocalBroadcast);
	}
	else
	{
		printf("Fehler beim Initialisieren des Local-Broadcast-Client\n");
		system("Pause");
		exit(0);
	}
}

void InitLocalDataClient()
{
	localDataClient = new UdpClient();
	bool success = localDataClient->Bind(IpAddress.Any, 0);
	if(success) 
	{
		printf("Local-Data-Client erfolgreich initialisert\n");
		localDataClient->StartReceiveData(ReceiveLocalData);
	}
	else
	{
		printf("Fehler beim Initialisieren des Local-Data-Clients\n");
		system("Pause");
		exit(0);
	}
}

void InitServerBroadcastClient()
{
	serverBroadcastClient = new TcpClient();
	bool success = serverBroadcastClient->Connect("192.168.2.143", 5001);
	if(success) 
	{
		printf("Broadcast-Client erfolgreich mit dem Server verbunden\n");
		serverBroadcastClient->StartReceiveData(ReceiveServerBroadcast);
	}
	else
	{
		printf("Fehler beim Verbinden des Broadcast-Clients zum Server\n");
		system("Pause");
		exit(0);
	}
}

void InitServerDataClient()
{
	serverDataClient = new TcpClient();
	bool success = serverDataClient->Connect("127.0.0.1", SERVER_DATA_PORT);
	if(success) 
	{
		printf("Data-Client erfolgreich mit dem Server verbunden\n");
		serverDataClient->StartReceiveData(ReceiveServerData);
	}
	else
	{
		printf("Fehler beim Verbinden des Data-Clients zum Server\n");
		system("Pause");
		exit(0);
	}
}